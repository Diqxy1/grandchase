#include "stdafx.h"
#include "GCClientErrorManager.h"
#include "KNC/ClientErr.h"

const DWORD dwLOW_LATENCY = 120;
const DWORD dwHIGH_LATENCY = 180;
const DWORD dwMAX_LATENCY = 1000;
const DWORD dwLIMIT_LATENCY = 10000;
const float fSEND_TICK = 60 * 30.0f; // 30분k


ImplementSingleton( GCClientErrorManager )

GCClientErrorManager::GCClientErrorManager()
{
    m_mapErrorCount.clear();
    m_setOnceCount.clear();
    fSendTime = 0.0f;
}

GCClientErrorManager::~GCClientErrorManager()
{
}

bool GCClientErrorManager::FrameMove(float fElapsedTime)
{
    if ( !KP2P::GetInstance()->IsConnectedToGameServer() ) 
        return true;

    fSendTime += fElapsedTime;
    
    if ( fSendTime >= fSEND_TICK ) 
    {
        SendCollectError();
        fSendTime = 0.0f;
    }

    return true;
}

void GCClientErrorManager::ErrorCollect( const int iErrorType, const int iErrorID, const int nCount )
{
    switch( iErrorType ) 
    {
    case KEventErr::ERR_P2PFAIL:
    case KEventErr::ERR_CLIENTRPT:
    case KEventErr::ERR_CUSTOM:
    case KEventErr::ERR_NETWORK:
        {
            std::pair<int,int> keyPair( iErrorType, iErrorID );
            if ( m_mapErrorCount.find( keyPair ) == m_mapErrorCount.end() )
            {
                m_mapErrorCount.insert( std::make_pair( keyPair, nCount ) );
            }
            else
            {
                m_mapErrorCount[keyPair] += nCount;
            }
        }
        break;
    default:
        break;
    }

    return;

}

void GCClientErrorManager::ErrorCollect( const int iErrorType, const int iErrorID )
{
    ErrorCollect( iErrorType, iErrorID, 1 );
    return;
}

void GCClientErrorManager::ErrorCollectData( const int iErrorType, const int iErrorID, const std::wstring strErrComment, 
                                             DWORD dwReserve1, DWORD dwReserve2, DWORD dwReserve3 )
{
    switch( iErrorType ) 
    {
    case KEventErr::ERR_P2PFAIL:
    case KEventErr::ERR_CLIENTRPT:
    case KEventErr::ERR_NETWORK:
        break;
    case KEventErr::ERR_CUSTOM:
        {
            std::pair<int,int> keyPair( iErrorType, iErrorID );
            
            //  데이터 실는 부분은 한번만 보낸다.
            if ( m_setOnceCount.find( keyPair ) != m_setOnceCount.end() )
                break;


            m_setOnceCount.insert( keyPair );

            KP2P::GetInstance()->Send_ClientError( iErrorType, iErrorID, strErrComment, dwReserve1, dwReserve2, dwReserve3 );

            ErrorCollect( iErrorType, iErrorID );
        }
        break;
    default:
        break;
    }

    return;
}

void GCClientErrorManager::SendCollectError()
{
    //  데이터 Send
    if ( m_mapErrorCount.empty() ) 
        return;

    KP2P::GetInstance()->Send_ClientErrorCount( m_mapErrorCount );

    m_mapErrorCount.clear();
}

void GCClientErrorManager::Send_PingTimeReport( bool bRelay, IN DWORD dwAvgPing_ )
{
    int iPacketID = 0;
    if ( dwAvgPing_ == 0 )
        return;

    if ( 0 < dwAvgPing_ && dwAvgPing_ < dwLOW_LATENCY )
    {
        if ( bRelay )
            iPacketID = KNetWorkErr::UR_PING_UNDER_120;
        else
            iPacketID = KNetWorkErr::PING_UNDER_120;
    }
    else if ( dwLOW_LATENCY <= dwAvgPing_ && dwAvgPing_ < dwHIGH_LATENCY )
    {
        if ( bRelay )
            iPacketID = KNetWorkErr::UR_PING_BITWEEN_120_180;
        else
            iPacketID = KNetWorkErr::PING_BITWEEN_120_180;
    }
    else if ( dwHIGH_LATENCY <= dwAvgPing_ && dwAvgPing_ < dwMAX_LATENCY )
    {
        if ( bRelay )
            iPacketID = KNetWorkErr::UR_PING_OVER_180;
        else
            iPacketID = KNetWorkErr::PING_OVER_180;
    }
    else if ( dwMAX_LATENCY <= dwAvgPing_ && dwAvgPing_ < dwLIMIT_LATENCY )
    {
        if ( bRelay )
            iPacketID = KNetWorkErr::UR_PING_OVER_1000;
        else
            iPacketID = KNetWorkErr::PING_OVER_1000;
    }
    else if ( dwLIMIT_LATENCY <= dwAvgPing_ )
    {
        if ( bRelay )
            iPacketID = KNetWorkErr::UR_PING_OVER_10000;
        else
            iPacketID = KNetWorkErr::PING_OVER_10000;
    }

    ErrorCollect( KEventErr::ERR_NETWORK, iPacketID );

    return;
}

// 평균 FPS를 보내줍니다
void GCClientErrorManager::Send_AveFPSReport( IN int iAveFPS )
{
    int iPacketID = 0;
    if ( iAveFPS < FPS_20 )
    {
        iPacketID = KClientErr::CE_AVE_FPS_UNDER_20;
    }
    else if ( FPS_20 <= iAveFPS && iAveFPS < FPS_30 )
    {
        iPacketID = KClientErr::CE_AVE_FPS_BETWEEN_20_30;
    }
    else if ( FPS_30 <= iAveFPS && iAveFPS < FPS_40 )
    {
        iPacketID = KClientErr::CE_AVE_FPS_BETWEEN_30_40;
    }
    else if ( FPS_40 <= iAveFPS && iAveFPS < FPS_50 )
    {
        iPacketID = KClientErr::CE_AVE_FPS_BETWEEN_40_50;
    }
    else if ( FPS_50 <= iAveFPS && iAveFPS < FPS_60 )
    {
        iPacketID = KClientErr::CE_AVE_FPS_BETWEEN_50_60;
    }
    else // FPS_60 이상
    {
        iPacketID = KClientErr::CE_AVE_FPS_OVER_60;
    }

    ErrorCollect( KEventErr::ERR_CLIENTRPT, iPacketID );

    return;
}