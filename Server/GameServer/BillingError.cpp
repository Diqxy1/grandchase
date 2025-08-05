#include "BillingError.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"

ImplementSingleton( KBillingError );

ImplOstmOperatorW2A( KBillingError );

KBillingError::KBillingError(void)
{
    m_mapBillingErrCount.clear();
}

KBillingError::~KBillingError(void)
{
}

ImplToStringW( KBillingError )
{
    KLocker lock( m_csBillingReport );

    std::map<std::wstring, DWORD>::const_iterator mit;
    for( mit = m_mapBillingErrCount.begin() ; mit != m_mapBillingErrCount.end() ; ++mit )
    {
        stm_ << mit->first << ", " << mit->second << std::endl;
    }
    return stm_;
}

void KBillingError::ErrorDetect( std::string strError_ )
{
    std::map<std::wstring, DWORD>::iterator mit;
    std::wstring wstrOutput = KncUtil::toWideString( strError_ );

    KLocker lock( m_csBillingReport );

    mit = m_mapBillingErrCount.find( wstrOutput );

    if( mit == m_mapBillingErrCount.end() )
    {
        m_mapBillingErrCount.insert( std::make_pair( wstrOutput, 1 ) ); // 처음 들어온 에러라면 맵에 넣기
    }
    else
    {
        mit->second = mit->second + 1; // 에러 카운트 증가하려는 의도
    }

    START_LOG( clog, wstrOutput )
        << BUILD_LOG( m_mapBillingErrCount.size() ) << END_LOG;

}

void KBillingError::ReportToDB()
{
    KLocker lock( m_csBillingReport );

    if( !m_mapBillingErrCount.empty() )
    {
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_CASH_ERROR_REPORT, NULL, 0, 0, m_mapBillingErrCount );
        m_mapBillingErrCount.clear();
    }
}
