#include "IndigoUtil.h"

#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include <cstdlib>
#include "Log4.h"
//FILE_NAME_FOR_LOG

BEGIN_NAMESPACE( IndigoUtil )

int GetConstantK( int iRP )
{
    if( iRP >= 2400 )       return 20;  // k = 20 ( 2400 ≤ rp )
    else if( iRP >= 1800 )  return 30;  // k = 30 ( 1800 ≤ rp ＜ 2400 )
    else if( iRP >= 0 )     return 50;  // k = 50 (    0 ≤ rp ＜ 1800 )

    return 0;
}

int GetRPChange( int iMyRP, int iOurRPAvg, int iEnemyRPAvg, bool bWin, int iLeftArena )
{
    LIF( iMyRP >= 0 );
    LIF( iOurRPAvg >= 0 );
    LIF( iEnemyRPAvg >= 0 );
    if( 0 > iLeftArena || iLeftArena > 3 )
    {
        START_LOG( cerr, L"iLeftArena 값 이상 : " << iLeftArena ) << END_LOG;
    }

    iLeftArena = std::min<int>( iLeftArena, 3 );  // 3보다 더 큰 값이면 3으로 맞춘다.
    iLeftArena = std::max<int>( iLeftArena, 0 );  // 0보다 더 작은 값이면 0으로 맞춘다.
    
    int iRPChange = 0;

    int iRPGap = iEnemyRPAvg - iOurRPAvg;
    iRPGap = std::min<int>( iRPGap, 400 );    // 400보다 더 큰 값이면 400으로 맞춘다.
    iRPGap = std::max<int>( iRPGap, -400 );   // -400보다 더 작은 값이면 -400으로 맞춘다.

    int iConstK = GetConstantK( iOurRPAvg );

    float fWinRate = 1.F / ( 1 + ::pow( 10.F, (iRPGap) / 400.F ) );

    if( bWin )
    {
        iRPChange = static_cast<int>(iConstK * ( 1.F - fWinRate ));
    }
    else
    {
        iRPChange = static_cast<int>(-1 * iConstK * fWinRate);
    }

    LIF( -50 <= iRPChange && iRPChange <= 50 );

    if( iMyRP + iRPChange < 0 ) // RP가 0이하로 내려가는 경우, 딱 0이 되도록 변화량(iRPChange)를 조절한다.
    {
        iRPChange = -iMyRP;
    }

    // 남아있는 arena 수에 따라 2점씩의 보너스를 준다.
    int iBonuse = 0;
    if( bWin )  iBonuse = 2 * iLeftArena;

    iRPChange += iBonuse;

    fWinRate = ceil( fWinRate * 100.f );

    START_LOG( clog, L"RPChange : " << iRPChange )     // 디버깅을 위해 rp 변화량 출력은 꼭 필요할듯 하다.
        << BUILD_LOG( iMyRP )
        << BUILD_LOG( iConstK )
        << BUILD_LOG( iOurRPAvg )
        << BUILD_LOG( iEnemyRPAvg )
        << BUILD_LOG( iRPGap )
        << BUILD_LOG( bWin )
        << BUILD_LOG( iLeftArena )
        << BUILD_LOG( iBonuse )
        << BUILD_LOG( fWinRate ) << END_LOG;

    return iRPChange;
}

int GetRPChange( int iMyRP, std::pair<int,int> prAvgRP, bool bIsBlueTeam, bool bWin, int iLeftArena )
{
    if( bIsBlueTeam )  return GetRPChange( iMyRP, prAvgRP.first, prAvgRP.second, bWin, iLeftArena );
    else               return GetRPChange( iMyRP, prAvgRP.second, prAvgRP.first, bWin, iLeftArena );
}

END_NAMESPACE

#include "GameServer.h"
#include "DBLayer.h"
#include "UserEvent.h"

ImplementSingleton( KServerOpenTime )

KServerOpenTime::KServerOpenTime() 
:
    m_bCheckEnable( false ),
    m_dwUpdateTick( ::GetTickCount() )
{
    // 기본적으로 요일별 시간을 기획서에 명시된 대로 설정해둔다.
    for( int i = 0; i < 7; i++ )
    {
        if( i == 0 || i == 6 )  // sat. sun.
        {
            m_prDayOfWeek[i].first  = CTime( 2006, 5, 16,  8, 0, 0 );
            m_prDayOfWeek[i].second = CTime( 2006, 5, 16, 22, 0, 0 );
        }
        else
        {
            m_prDayOfWeek[i].first  = CTime( 2006, 5, 16, 10, 0, 0 );
            m_prDayOfWeek[i].second = CTime( 2006, 5, 16, 22, 0, 0 );
        }
    }

    CheckDBUpdate();
    SetToadysEntranceTime();
}

KServerOpenTime::~KServerOpenTime()
{
}

void KServerOpenTime::SetEnable( bool bEnable_ )
{
    m_bCheckEnable = bEnable_;
}

void KServerOpenTime::CheckDBUpdate()
{
    SiKGameServer()->GetDBLayer()->QueueingID( KUserEvent::DB_EVENT_CHECK_OPEN_TIME_REQ, NULL, 0, 0 );

    m_dwUpdateTick = ::GetTickCount();  // event queueing과 동시에 tick을 갱신. 문제가 발생한 경우 이쪽이 더 안전함.
}

void KServerOpenTime::Tick()
{
    if( ::GetTickCount() - m_dwUpdateTick > 60 * 60 * 1000 )
    {
        CheckDBUpdate();
    }
}

bool KServerOpenTime::CheckEntranceEnable() const
{
    if( !m_bCheckEnable ) return true;      // check flag가 꺼져 있다면 항상 입장 가능.

    CTime cCurTime = CTime::GetCurrentTime();
    CTime cOpenTime = GetToadysOpenTime();
    CTime cCloseTime = GetToadysCloseTime();

    KLocker lock( m_csOpenTime );
    if( !m_vecExceptTime.empty() )  // 예외일자가 있다면 이를 먼저 확인한다.
    {
        std::vector< std::pair<CTime,CTime> >::const_iterator vit;

        for( vit = m_vecExceptTime.begin(); vit != m_vecExceptTime.end(); vit++ )
        {
            // 시작 시각의 날짜가 오늘과 같은 경우에만 체크한다.
            if( vit->first.GetYear() == cCurTime.GetYear() &&
                vit->first.GetMonth() == cCurTime.GetMonth() &&
                vit->first.GetDay() == cCurTime.GetDay() )
            {
                if( vit->first < cCurTime && cCurTime < vit->second )   // 현재시각이 설정시각의 사이에 있음.
                    return true;    // 입장 가능.
                else
                    return false;
            }
        }
    }

    return ( cOpenTime < cCurTime && cCurTime < cCloseTime );
}

void KServerOpenTime::UpdateDataFromDB( IN const std::vector< boost::tuple<short,CTime,CTime> > vecData_ )
{
    KLocker lock( m_csOpenTime );

    std::vector< boost::tuple<short,CTime,CTime> >::const_iterator vit;

    m_vecExceptTime.clear();

    for( vit = vecData_.begin(); vit != vecData_.end(); vit++ )
    {
        switch( vit->get<0>() ) {
        case -1:
            m_vecExceptTime.push_back( std::make_pair( vit->get<1>(), vit->get<2>() ) );
        	break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            m_prDayOfWeek[ vit->get<0>() ].first = vit->get<1>();
            m_prDayOfWeek[ vit->get<0>() ].second = vit->get<2>();
            break;
        default:
            START_LOG( cerr, L"Invalid value of 'DayOfWeek'. value : " << vit->get<0>() )
                << BUILD_LOG( vecData_.size() ) << END_LOG;
        }
    }

    SetToadysEntranceTime();
}

ImplToStringW( KServerOpenTime )
{
    KLocker lock( m_csOpenTime );

    START_TOSTRING2W( KServerOpenTime )
        << TOSTRINGWb( m_bCheckEnable )
        << TOSTRINGWb( CheckEntranceEnable() )
        << TOSTRINGW( m_vecExceptTime.size() );

    std::vector< std::pair<CTime,CTime> >::const_iterator vit;

    for( vit = m_vecExceptTime.begin(); vit != m_vecExceptTime.end(); vit++ )
    {
        stm_ << L"   " << (LPCWSTR)vit->first.Format( KNC_TIME_FORMAT ) 
             << L" ~ " << (LPCWSTR)vit->second.Format( KNC_TIME_FORMAT ) << std::endl;
    }

    UINT DayOfWeek[] = {
        LOCALE_SABBREVDAYNAME7,  // Sunday
        LOCALE_SABBREVDAYNAME1,  
        LOCALE_SABBREVDAYNAME2,
        LOCALE_SABBREVDAYNAME3,
        LOCALE_SABBREVDAYNAME4,
        LOCALE_SABBREVDAYNAME5,
        LOCALE_SABBREVDAYNAME6  // Saturday
    };

    TCHAR strWeekday[256] = {0};

    for( int i = 0; i < 7; i++ )
    {
        ::GetLocaleInfo(LOCALE_USER_DEFAULT, DayOfWeek[i], strWeekday, sizeof(strWeekday));

        stm_ << L"    " << strWeekday << L"  " << (LPCWSTR)m_prDayOfWeek[i].first.Format( L"%H:%M:%S" )
            << L" ~ " << (LPCWSTR)m_prDayOfWeek[i].second.Format( L"%H:%M:%S" ) << std::endl;
    }

    return stm_;
}

void KServerOpenTime::SetToadysEntranceTime()
{
    // 상위에서 lock 걸어주고 있음.
    CTime cCurTime = CTime::GetCurrentTime();

    // 해당되는 예외일자가 없다면 요일별 시각을 확인한다.
    int nDayOfWeek = cCurTime.GetDayOfWeek() - 1;   // 0 ~ 6 의 값으로 만든다.

    // 오늘 날짜와, 설정된 시간값을 가지는 CTime 변수를 만든다.
    m_cTodaysOpenTime = CTime( cCurTime.GetYear(), cCurTime.GetMonth(), cCurTime.GetDay(),
        m_prDayOfWeek[nDayOfWeek].first.GetHour(), 
        m_prDayOfWeek[nDayOfWeek].first.GetMinute(), 
        m_prDayOfWeek[nDayOfWeek].first.GetSecond() );

    m_cTodaysCloseTime = CTime( cCurTime.GetYear(), cCurTime.GetMonth(), cCurTime.GetDay(),
        m_prDayOfWeek[nDayOfWeek].second.GetHour(), 
        m_prDayOfWeek[nDayOfWeek].second.GetMinute(),
        m_prDayOfWeek[nDayOfWeek].second.GetSecond() );
}

CTime KServerOpenTime::GetToadysOpenTime() const
{
    KLocker lock( m_csOpenTime );
    return m_cTodaysOpenTime;
}

CTime KServerOpenTime::GetToadysCloseTime() const
{
    KLocker lock( m_csOpenTime );
    return m_cTodaysCloseTime;
}