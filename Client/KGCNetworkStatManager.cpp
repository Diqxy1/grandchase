#include "stdafx.h"
#include "KGCNetworkStatManager.h"
#include "GCClientErrorManager.h"
#include "KNC/ClientErr.h"

ImplementSingleton( KGCNetworkStatManager )

const int nLimitCountCheck[KGCNetworkStatManager::EM_TICK_STAT_SIZE] = { 1000, 30000, 3000, 300000 };

KGCNetworkStatManager::KGCNetworkStatManager()
: m_bLogFile ( false )
{
    //  기본값 1분
    memset( m_dwTickCount, 0, sizeof( m_dwTickCount[0] ) * EM_TICK_STAT_SIZE );
    memset( m_dwTickGaps, 0, sizeof( m_dwTickGaps[0] ) * EM_TICK_STAT_SIZE );


    m_dwTickGaps[EM_TICK_TR_PACKET_COUNT] = 1 * 60 * 1000;    // 1분마다
    m_dwTickGaps[EM_TICK_TR_PACKET_SIZE] = 1 * 60 * 1000;    // 1분마다
    m_dwTickGaps[EM_TICK_UR_PACKET_COUNT] = 1 * 60 * 1000;    // 1분마다
    m_dwTickGaps[EM_TICK_UR_PACKET_SIZE] = 1 * 60 * 1000;    // 1분마다
    
    for ( int i = 0 ; i < EM_TICK_STAT_SIZE; ++i ) 
    {
        SStatData  sStat;
        m_mapStatCount.insert( std::pair<int,SStatData>(i, sStat) );
    }

    for ( int i = 0 ; i < EM_MODE_STAT_SIZE; ++i ) 
    {
        SStatData  sStat;
        m_mapModeStatCount.insert( std::pair<int,SStatData>(i, sStat) );
    }

    

#if !defined( __PATH__ )
    m_bLogFile = true;
#endif 

    if ( m_bLogFile ) 
    {
        FILE* pLogFile = NULL;
        pLogFile = fopen(  "networkstat.txt", "wb" );

        if ( pLogFile ) 
            fclose( pLogFile );
    }

}
    
KGCNetworkStatManager::~KGCNetworkStatManager()
{
}

void KGCNetworkStatManager::FrameMove()
{
    DWORD dwCurTick = GetTickCount();

    for ( int i = 0; i < EM_TICK_STAT_SIZE; ++i ) 
    {
        if ( dwCurTick - m_dwTickCount[i] >= m_dwTickGaps[i] ) 
        {
            WriteLog( i );            
            ClearStatData( i );            
            m_dwTickCount[i] = dwCurTick;
        }
    }
}

void KGCNetworkStatManager::WriteLog( int emLogType )
{
    if ( !m_bLogFile ) 
        return;

    FILE* pLogFile = NULL;
    pLogFile = fopen(  "networkstat.txt", "ab" );

    if ( !pLogFile ) 
        return;

    std::string curtime = GCUTIL_STR::GCStrWideToChar( (LPCTSTR)(CTime::GetCurrentTime().Format( KNC_TIME_FORMAT )) );

    int nCount = m_mapStatCount[emLogType].m_nCount;

    std::string strEnum = GetEnumString( emLogType );
    fprintf( pLogFile, "%s\t%s\t%d\n", curtime.c_str(), 
        strEnum.c_str(), nCount );
    fflush( pLogFile );
    fclose( pLogFile );
}

std::string KGCNetworkStatManager::GetEnumString( int nType )
{
    switch( nType )
    {
        case EM_TICK_TR_PACKET_COUNT:   return EnumToString( EM_TICK_TR_PACKET_COUNT );
        case EM_TICK_TR_PACKET_SIZE:    return EnumToString( EM_TICK_TR_PACKET_SIZE );
        case EM_TICK_UR_PACKET_COUNT:   return EnumToString( EM_TICK_UR_PACKET_COUNT );
        case EM_TICK_UR_PACKET_SIZE:    return EnumToString( EM_TICK_UR_PACKET_SIZE );
        default: return "UNKWONTYPE";
    }
}

bool KGCNetworkStatManager::IsValidStat( int nType )
{
    if ( nType < 0 || nType >= EM_TICK_STAT_SIZE )
        return false;

    return true;
}

bool KGCNetworkStatManager::IsValidModeStat( int nType )
{
    if ( nType < 0 || nType >= EM_MODE_STAT_SIZE )
        return false;

    return true;
}

void KGCNetworkStatManager::AddStatCount( int nType, int nCount )
{
    if ( !IsValidStat( nType ) )
        return;

    // Debug: Mostra estatísticas em tempo real
    std::cout << "Estatística [" << GetEnumString(nType) << "] +" << nCount
        << " (Total: " << m_mapStatCount[nType].m_nCount + nCount << ")" << std::endl;

    m_mapStatCount[nType].m_nCount += nCount;

    if ( m_mapStatCount[nType].m_nCount >= nLimitCountCheck[nType] ) 
    {

        std::cerr << "AVISO: Limite excedido para " << GetEnumString(nType)
            << " (" << m_mapStatCount[nType].m_nCount << ")" << std::endl;

        if ( m_bLogFile && g_pkChatManager ) 
        {
            std::string strMessage = boost::str(boost::format("%1% %2% count over")%GetEnumString( nType )%m_mapStatCount[nType].m_nCount); 
            std::wstring wstrMessage = KncUtil::toWideString( strMessage );

            g_pkChatManager->AddSystemMsg( 0, wstrMessage );
        }

        if ( !m_mapStatCount[nType].m_bErrorSend )
        {
            SendClientError( nType );
        }
    }
}

void KGCNetworkStatManager::AddStatModeCount( int nType, int nCount )
{
    if ( !IsValidModeStat( nType ) )
        return;

    m_mapModeStatCount[nType].m_nCount += nCount;
}

void KGCNetworkStatManager::ClearStatData( int nType )
{
    if ( !IsValidStat( nType ) )
        return;

    m_mapStatCount[nType].m_nCount = 0;
    m_mapStatCount[nType].m_bErrorSend = false;
}

void KGCNetworkStatManager::ClearStatModeData( int nType )
{
    if ( !IsValidModeStat( nType ) )
        return;

    m_mapModeStatCount[nType].m_nCount = 0;
    m_mapModeStatCount[nType].m_bErrorSend = false;
}
void KGCNetworkStatManager::ClearStatModeDataAll()
{
    for ( int i = 0; i < EM_MODE_STAT_SIZE; ++i ) 
    {
        ClearStatModeData( i );
    }
}


int KGCNetworkStatManager::GetStatCount( int nType )
{
    if ( !IsValidStat( nType ) )
        return -1;

    return m_mapStatCount[nType].m_nCount;
}

int KGCNetworkStatManager::GetStatModeCount( int nType )
{
    if ( !IsValidModeStat( nType ) )
        return -1;

    return m_mapModeStatCount[nType].m_nCount;
}

void KGCNetworkStatManager::SendClientError( int nType )
{
    switch( nType )
    {
    case EM_TICK_TR_PACKET_COUNT:
        {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TR_PACKET_COUNT_LIMIT );
            break;
        }
    case EM_TICK_TR_PACKET_SIZE:   
        {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_TR_PACKET_SIZE_LIMIT );
            break;
        }
    case EM_TICK_UR_PACKET_COUNT:
        {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UR_PACKET_COUNT_LIMIT );
            break;
        }
    case EM_TICK_UR_PACKET_SIZE:   
        {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UR_PACKET_SIZE_LIMIT );
            break;
        }
    default: 
        return;
    }

    m_mapStatCount[nType].m_bErrorSend = true;
}