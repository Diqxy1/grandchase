#pragma once
#include "soapH.h"
#include "IntEvent.h"
#include "NUserEvent.h"
#include "SubThread.h"
#include "NUserPacket.h"

#include <Thread/Thread.h>

class KCenterSimLayer;
class KCnSoapAuth : public KSubThread< KNUserEvent >
{
public:
    KCnSoapAuth(void);
    ~KCnSoapAuth(void);

    // derived from KThread
    virtual bool Begin();
    virtual void Run();

    static void SetWsdlPath( IN const char* szWsdlPath );
    static void SetWebMethodPath( IN const char* szWebMethodPath );
    static void SetGameServiceID( IN const char* szGameServiceID );

protected:
    // derived from KSubThread
    virtual void ProcessIntEvent( const KIntEvent& kIntEvent_ );

    void GSoapInit();
    void GSoapDestory();
    void test();

protected:
    static struct KCommonData
    {
        KncCriticalSection   m_csSoapAuth;
        std::string          m_strWsdlPath;
        std::string          m_strWebMethodPath;
        std::string          m_strGameServiceID;
    } ms_ComnData;

    struct soap*                p_gf_soap;

    INT_DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
};
