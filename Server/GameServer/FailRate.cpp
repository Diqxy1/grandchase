#include <winsock2.h>
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include <iomanip>  // std::setw()
#include <sstream>  // std::ostringstream
#include "FailRate.h"
#include "UserPacket.h"
#include "NetError.h"
#include "GameServer.h"
#include "UserEvent.h"
#include "Log4.h"

#   undef _ENUM
#   define _ENUM( id, comment ) L ## #comment,
const wchar_t* KFailRate::ms_szCountReason[] = {
#include "FailRate_def.h"
};

const wchar_t* KFailRate::ms_szClientErrReason[] = {
#include "ClientErr_def.h"
};

#   undef _ENUM
#   define _ENUM( id, comment ) L ## #id,
const wchar_t* KFailRate::ms_szCountIDs[] = {
#include "FailRate_def.h"
};

const wchar_t* KFailRate::ms_szClientErrIDs[] = {
#include "ClientErr_def.h"
};


ImplementSingleton( KFailRate );
ImplOstmOperatorW2A( KFailRate );

KFailRate::KFailRate(void)
:m_dwLastTick( ::GetTickCount() )
,m_bWriteToDB( false )
,m_dwFailRateWriteTick( 1000 * 60 )
,m_nUpdateMinute(30)
{
    ::ZeroMemory( m_anFailCount, sizeof( m_anFailCount ) );
    ::ZeroMemory( m_anClientErrCount, sizeof( m_anClientErrCount ) );
    UpdateFlushTime( m_nUpdateMinute );
}

KFailRate::~KFailRate(void)
{
}

ImplToStringW( KFailRate )
{
    {
        KLocker lock( m_csUpdateTime );

        stm_ << std::boolalpha 
            << L"-> KFailRate " << std::endl
            << L"    Next Update : " << (LPCTSTR)m_tmUpdateTime.Format( KNC_TIME_FORMAT )
            << L"데이터 DB 저장 여부 : " << m_bWriteToDB << std::endl
            << std::setw( 8 ) << L"Count" << L"    Description " << std::endl
            << std::setw( 80 ) << std::setfill( L'-' ) << L'-' << std::setfill( L' ' ) << std::endl;
    }

    { // locking 영역을 조절하기 위한 scope
        KLocker lock( m_csFailCount );

        for( int i = 0; i < KFailRate::COUNT_TOTAL; i++ )
        {
            stm_ << std::setw( 8 ) << m_anFailCount[ i ]
            << "  " << ms_szCountReason[ i ]
            << std::endl;
        }

        stm_ << std::endl;

    } // locking 영역을 조절하기 위한 scope

    return stm_;
}

int KFailRate::IncreaseCount( int nIndex_ )
{
    if( nIndex_ < 0 || nIndex_ >= KFailRate::COUNT_TOTAL )
    {
        START_LOG( cwarn, L"카운트 항목 인덱스가 올바르지 않음" )
            << BUILD_LOG( nIndex_ )
            << BUILD_LOG( KFailRate::COUNT_TOTAL ) << END_LOG;
        return -1;
    }

    KLocker lock( m_csFailCount );
    m_anFailCount[ nIndex_ ]++;

    START_LOG( clog, ms_szCountReason[nIndex_] << L" : " << m_anFailCount[nIndex_] ) << END_LOG;

    return m_anFailCount[ nIndex_ ];
}

int KFailRate::IncreaseCount( int nIndex_, int nNum_ )
{
    if( nIndex_ < 0 || nIndex_ >= KFailRate::COUNT_TOTAL || nNum_ < 0 )
    {
        START_LOG( cwarn, L"카운트 항목 인덱스가 올바르지 않음" )
            << BUILD_LOG( nIndex_ )
            << BUILD_LOG( nNum_ )
            << BUILD_LOG( KFailRate::COUNT_TOTAL ) << END_LOG;
        return -1;
    }

    KLocker lock( m_csFailCount );
    m_anFailCount[ nIndex_ ] += nNum_;

    START_LOG( clog, ms_szCountReason[nIndex_] << L" : " << m_anFailCount[nIndex_] ) << END_LOG;

    return m_anFailCount[ nIndex_ ];

}

int KFailRate::IncreaseCErrCount( IN int nIndex_ )
{

    if( nIndex_ < 0 || nIndex_ >= KFailRate::CE_UNKNOWN )
    {
        START_LOG( cwarn, L"알수 없는 에러 항목" )
            << BUILD_LOG( nIndex_ ) << END_LOG;

        nIndex_ = KFailRate::CE_UNKNOWN;
    }

    KLocker lock( m_csClientErrCount );
    m_anClientErrCount[ nIndex_ ]++;

    START_LOG( clog, ms_szClientErrReason[nIndex_] << L" : " << m_anClientErrCount[nIndex_] ) << END_LOG;

    return m_anClientErrCount[ nIndex_ ];
}

int KFailRate::IncreaseCErrCount( IN int nIndex_, int nNum_ )
{

    if( nIndex_ < 0 || nIndex_ >= KFailRate::CE_UNKNOWN )
    {
        START_LOG( cwarn, L"알수 없는 에러 항목" )
            << BUILD_LOG( nIndex_ ) 
            << BUILD_LOG( nNum_ ) 
            << END_LOG;

        nIndex_ = KFailRate::CE_UNKNOWN;
    }

    KLocker lock( m_csClientErrCount );
    m_anClientErrCount[ nIndex_ ] += nNum_;

    START_LOG( clog, ms_szClientErrReason[nIndex_] << L" : " << m_anClientErrCount[nIndex_] ) << END_LOG;

    return m_anClientErrCount[ nIndex_ ];
}

void KFailRate::Tick()
{
    if( ::GetTickCount() - m_dwLastTick < m_dwFailRateWriteTick ) {
        return;
    }
    m_dwLastTick = ::GetTickCount();

    if( CheckUpdate() ) {
        WriteToDB();
    }
}

void KFailRate::WriteToDB()
{
    KLocker lock1( m_csFailCount );
    KLocker lock2( m_csClientErrCount );

    if( m_bWriteToDB )
    {
        std::vector<int> vecData( KFailRate::COUNT_TOTAL );
        std::copy( m_anFailCount, m_anFailCount + KFailRate::COUNT_TOTAL, vecData.begin() );

        // DB에 결과를 기록하기 위해 db thread로 정보 전달.
        SiKGameServer()->GetDBLayer()->QueueingEvent( 
            KUserEvent::DB_EVENT_INSERT_P2PFAILRATE_DATA,
            L"__FailRate", 0, 0, vecData );

        // 클라이언트의 오류 리포팅
        std::vector<int> vecCErrData( KFailRate::CE_TOTAL );
        std::copy( m_anClientErrCount, m_anClientErrCount + KFailRate::CE_TOTAL, vecCErrData.begin() );

        SiKGameServer()->GetDBLayer()->QueueingEvent( 
            KUserEvent::DB_EVENT_INSERT_CLIENTERR_DATA,
            L"__ClientErr", 0, 0, vecCErrData );
    }
    else
    {
        START_LOG( cerr, L"DB에 업데이트하지 않도록 설정되어 있으므로, 데이터 리셋만 진행됨." ) << END_LOG;
    }

    // DB에 30분 마다 기록하도록 변경한 이후 콘솔에는 출력하지 않도록 수정하였습니다(2013. 02. 28)
    //ToLog();    // 로그파일과 콘솔창으로 dump
    //ClientErrLog();

    ::ZeroMemory( m_anFailCount, sizeof( m_anFailCount ) );
    ::ZeroMemory( m_anClientErrCount, sizeof( m_anClientErrCount ) );
}

//bool KFailRate::AnalysisJoinRoomAck( KEVENT_JOIN_ROOM_ACK& kAck_ )

void KFailRate::SetWriteToDB( bool bWriteToDB_ )
{
    m_bWriteToDB = bWriteToDB_;

    START_LOG( cerr, L"Update to DB : " << m_bWriteToDB ) << END_LOG;
}

void KFailRate::ToLog()
{
    // 로그파일과 콘솔창으로 dump
    std::wostringstream strstm;
    ToString( strstm );
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG; // ENDL을 하지 않으면, 파일엔 아직 flush되지 않은 상태로 남는다.
}

void KFailRate::ClientErrLog()
{
    std::wostringstream strstm;

    KLocker lock( m_csClientErrCount );
    strstm << L"\n_____ Client Err Report _____\n";
    for( int i = 0 ; i < KFailRate::CE_TOTAL ; ++i  )
    {
        strstm << L"    " << ms_szClientErrReason[i] << L" : " << m_anClientErrCount[i] << std::endl;
    }
    START_LOG( cerr, L"" << strstm.str() ) << END_LOG;
}

const wchar_t* KFailRate::IDToString( int nReason_ )
{
    if( nReason_ < 0 || nReason_ > KFailRate::COUNT_TOTAL )
    {
        return ms_szCountReason[ KFailRate::COUNT_TOTAL ];
    }

    return ms_szCountReason[ nReason_ ];
}

const wchar_t* KFailRate::CErrIDToString( int nReason_ )
{
    if( nReason_ < 0 || nReason_ > KFailRate::CE_TOTAL )
    {
        return ms_szClientErrReason[ KFailRate::CE_TOTAL ];
    }

    return ms_szClientErrReason[ nReason_ ];
}

const wchar_t* KFailRate::IDString( int nReason_ )
{
    if( nReason_ < 0 || nReason_ > KFailRate::COUNT_TOTAL )
    {
        return ms_szCountIDs[ KFailRate::COUNT_TOTAL ];
    }

    return ms_szCountIDs[ nReason_ ];
}

const wchar_t* KFailRate::CErrIDString( int nReason_ )
{
    if( nReason_ < 0 || nReason_ > KFailRate::CE_TOTAL )
    {
        return ms_szClientErrIDs[ KFailRate::CE_TOTAL ];
    }

    return ms_szClientErrIDs[ nReason_ ];
}

void KFailRate::UpdateFlushTime( IN const int& nMinute_ )
{
    CTime tmCurrent = CTime::GetCurrentTime();
    CTimeSpan ts(0,0,nMinute_,0); // 30분 주기
    tmCurrent += ts;
    m_tmUpdateTime = CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), tmCurrent.GetHour(), tmCurrent.GetMinute(), tmCurrent.GetSecond() );
}

bool KFailRate::CheckUpdate()
{
    KLocker lock( m_csUpdateTime );
    if( m_tmUpdateTime > CTime::GetCurrentTime() ) {
        return false;
    }
    UpdateFlushTime( m_nUpdateMinute );
    return true;
}

void KFailRate::SetUpdateTime( IN int nMinute_ )
{
    KLocker lock( m_csUpdateTime );
    m_nUpdateMinute = nMinute_;
}
