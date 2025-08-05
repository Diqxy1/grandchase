#pragma once

#ifdef SSO_LOGIN


////////////////////////////////////////////////////////////////////////////////
//  SSO Error code table.
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Define                         Value       Description
//-----------------------------------------------------------------------------
#define ERR_NOERROR 					0		// No error.(Success)
#define ERR_KDCCONNECTFAIL				10010	// KDC에 접속할 수 없습니다.
#define ERR_SSOAGENTVERSIONFAIL			10011	// SSO Agent버젼이 KDC와 호환되지 않습니다.
#define ERR_INPUTDATAVALIDATECHECKFAIL	10012	// 입력값이 유효하지 않습니다.
#define ERR_USER_NOTFOUND				10013	// 계정이 존재하지 않습니다.
#define ERR_USER_PASSWDMISMATCH			10014	// 패스워드가 틀립니다.
#define ERR_TGT_VERIFY					10015	// TGT 검증을 할 수 없습니다.
#define ERR_TGT_UPDATEVERIFYCOUNT		10016	// TGT 검증을 할 수 없습니다.(counter error)
#define ERR_TGT_UPDATEVERIFYSEQ			10017	// TGT 검증을 할 수 없습니다.(sequence error)
#define ERR_TGT_NOTEXIST				10018	// TGT가 없습니다.(로그아웃 상태입니다.)
#define ERR_TGT_EXIST					10019	// TGT가 이미 있습니다.(로그인된 상태입니다.)
#define ERR_SERVICENOTFOUND				10020	// 서비스 이름이 존재하지 않습니다.
#define ERR_DIDNOTUSERMAPPING			10021	// 서비스에 계정이 매핑되지 않았습니다.
#define ERR_PASSWDCHANGEFAIL			10022	// 패스워드변경을 실패했습니다.
#define ERR_TICKET_PARSE				10023	// Ticket을 분석할 수 없습니다.
#define ERR_ST_VERIFY					10024	// ST검증을 실패했습니다.
#define ERR_ENVIRONMENT                 10025   // SSO관련 모듈이 시스템에 등록되어있지 않습니다.
#define ERR_PERMISSION                  10026   // 권한이 없습니다. (2006-05-30 added)
#define ERR_LOCKACCOUNT				    10027	// 계정이 잠겨있습니다.
//-----------------------------------------------------------------------------
#define ERR_P_PROTOCOLFAIL				20010	// SSO Protocol failure.
#define ERR_P_STEP2_SENDRESPONSE		20011	// SSO Protocol failure (KDC Step2 Send Error) 
#define ERR_P_STEP3_VERIFY				20012	// SSO Protocol failure (KDC Step3 Veirfy Error)
#define ERR_P_STEP4_ENCODE				20013	// SSO Protocol failure (KDC Step4 Enc Error)
#define ERR_P_ABNORMAL					20014	// SSO Protocol failure (KDC 통신중 연결이 끊겼습니다.)
#define ERR_P_NOTIMPLEMENTATION			20020	// SSO Protocol failure (구현되지않은 기능입니다.)
#define ERR_P_CMDNOTRECOGNISE			20021	// SSO Protocol failure (Command not recognise)
#define ERR_P_KDCEXCEPTION				20022	// SSO Protocol failure (KDC catch except)
#define ERR_P_BUFFPACKINGFAIL			20023	// SSO Protocol failure (Buffer packing fail)
#define ERR_P_CRYPTOFAIL				20024	// SSO Protocol failure (Data encryption fail.)
#define ERR_P_DB_TGTCOUNTFAIL			20025	// SSO Protocol failure (DB TGT Count check fail)
#define ERR_P_DB_OPERATIONFAIL			20026	// SSO Protocol failure (DB Operation fail)
#define ERR_CATCHEXCEPTION				30000	// SSO Protocol failure (알 수 없는 에러가 발생하였습니다.)
//-----------------------------------------------------------------------------


//com을 사용하기 위한 include와 import
#include <comdef.h>
#include <ObjBase.h>
#include <atlbase.h>
#include <atlcom.h>
#include <string>

#import "./SSOModule/SSOAgent.dll" no_namespace named_guids
#import "./SSOModule/SSODaemon.exe" no_namespace named_guids
extern CComModule _Module;

/****************************************************************************
 2006-04-06 목요일 프로그래머 권희재 

다음은 중국에서 쓰는 인증 시스템, SSO관련 작업 관련 클래스이다.
SSO는 COM기반으로 돌아가는 시스템이며, 로그인 서버로 접속하기 전에 
KDC(Key Distribution Center)서버로 먼저 접속하여 ST를 받아오는 시스템이다.
로그인 서버로 접속할 때 이 ST를 같이 날리며, 
이 ST(Service Ticket)가 로그인 서버에서 통과되어야만 게임서버로 접속할 수 있다.

Client에서는 Sega에서 제공한 SSOAgent.dll과 SSODaemon.exe를 사용하며,
이 COM Server를 통해서 KDC와 통신한다.

작업은 다음의 네 단계로 진행된다.(중국에만 적용된다.)

1. Com Server의 등록 -  Registry에 등록한다.
2. Com 의 생성 -  COM Instance를 생성한다.
3. Com 을 이용하여 Service Ticket을 발급받는다. 
4. 게임을 종료할 때에 Com을 Registry에서 제거한다.

******************************************************************************/

class CSSOClient
{
public:
	CSSOClient(void);
	~CSSOClient(void);

	// For	Singleton
	static CSSOClient&	GetInstance( void );

	// SSO Client Daemon Event Handler
	class ATL_NO_VTABLE _EventHandler : 
		public CComObjectRoot,
		public IDispatchImpl<ISSOClientDaemonEvents,&IID_ISSOClientDaemonEvents,&LIBID_SSODAEMONLib>
	{
	public:
		BEGIN_COM_MAP(_EventHandler)
			COM_INTERFACE_ENTRY(IDispatch)
			COM_INTERFACE_ENTRY(ISSOClientDaemonEvents)
		END_COM_MAP()

		STDMETHOD(raw_DoStatusChange)( long nReason );
		STDMETHOD(raw_UpdateServiceParam)( enum ServiceParam param, BSTR bstrParam );
		STDMETHOD(raw_ServiceLogonDuplicated)( long nSvcType, BSTR bstrServiceID );
		STDMETHOD(raw_ServiceLogon)( long nSvcType, BSTR bstrServiceID );
		STDMETHOD(raw_ServiceLogout)( long nSvcType, BSTR bstrServiceID );
	};

	bool RegisterSSOComponent();	//Client사이드의 component 등록
	bool UnRegisterSSOComponent();	//Com Regi에서 제거한다.

	bool CreateSSOClient();
	bool ReleaseSSOClient();

	HRESULT StartSSOClient();
	HRESULT EndSSOClient();
	HRESULT ForceLogout();


	//ST를 발급받는 가장 중요한 함수이다.
	std::wstring GetServiceTicket( const char* strKDCAddr , const int& iKDCPort , const char* strServiceID );
	bool                m_bIsSelfLogout;

private:
	ISSOClientAgentPtr  m_pSSOClientAgent;
	ISSOClientDaemonPtr m_pSSOClientDaemon; 
	DWORD				m_dwCookie;
	std::string			m_strSvcId;
	
};

#endif	//SSO_LOGIN 