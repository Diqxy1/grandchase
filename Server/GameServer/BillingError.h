// 071012. woosh.
// BillingError 클래스를 싱글톤으로 제작
// GSSimLayer 에서 1시간마다 
// BillingError에게  DB로 에러리포트 저장하는 이벤트를 날리라고 한다.
// 원래 GSSimLayer에서 DBLayer에 이벤트를 바로 넣을 수 있는데, 
// BillingError 에 에러 데이터가 있기 때문에 함수 호출을 이용해서 방법상 약간 돌아가게 되었다.

#pragma once
#include "KNCSingleton.h"
#include <Thread/Locker.h>
#include <ToString.h>
#include <map>
#include <KncUtil.h>

class KBillingError
{
    DeclToStringW;
    DeclareSingleton( KBillingError );

public:
    KBillingError(void);
    virtual ~KBillingError(void);

    void ErrorDetect( std::string strError );
    void ReportToDB();
protected:
    std::map<std::wstring, DWORD>    m_mapBillingErrCount; // 071011. woosh.  std::string or std::wstring 
    
    mutable KncCriticalSection       m_csBillingReport;

};

DefSingletonInline( KBillingError );
DeclOstmOperatorA( KBillingError );
