#include "stdafx.h"
#ifdef SSO_LOGIN

#include ".\ssoclient.h"
//







#pragma comment( lib, "shell32.lib" )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4101 )

CComModule		_Module;

CSSOClient::CSSOClient(void)
{
	m_pSSOClientAgent	= NULL;
	m_pSSOClientDaemon	= NULL; 
	m_dwCookie			= 0;

	m_strSvcId          = "";	
	m_bIsSelfLogout		= false;

	_Module.Init( NULL , (HINSTANCE)GetModuleHandle(NULL) );
}

CSSOClient::~CSSOClient(void)
{
	_Module.Term();
}

CSSOClient& CSSOClient::GetInstance( void )
{
	static CSSOClient	s_SingletonInstance;

	return s_SingletonInstance;
}


// -------------------------------------------------------------------------------------------------
// SSO Client Daemon Event Handler Implementation
// -------------------------------------------------------------------------------------------------

/// 사용자 컴퓨터에서 TGT로그아웃이 발생했을 때 이벤트가 발생합니다.
/// 이벤트 발생후 클라이언트 프로그램은 안내메시지를 표출하고 자동종료하여야 합니다.
/// @param nReason 로그아웃 이유
/// @return HRESULT
STDMETHODIMP CSSOClient::_EventHandler::raw_DoStatusChange( long nReason )
{
	//이미 종료중이거든 ㅡㅡ? 그냥 리턴.
	if ( CSSOClient::GetInstance().m_bIsSelfLogout == true )
	{
		return	S_OK;
	}
	
	//자기 자신이 로그아웃을 불렀을 는 그냥 리턴하자
	switch(nReason) 
	{
	case ReasonForcedLogout:     //중복로그인의 경우이다. 이  때에는 중복로그인하셨다는 메세지를 띄우고 종료.
		{
			//강제로 종료해 주어야 한다.
			g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED1 ).c_str(), 
					 				 g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED2 ).c_str() );
		}
		break;
	case ReasonNormal:
	default:
		{
			//강제로 종료해 주어야 한다.
			g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED3 ).c_str(),
									 g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED2 ).c_str() );
		}
		break;
	}

	return S_OK;
}

/// 클라이언트의 서비스데이터가 변경될 경우 이벤트가 발생합니다.
/// (이벤트 데이터타입의 데이터를 요청하고 변경내용을 반영하여야 합니다.)
/// @param param 데이터타입
/// @param bstrParam 변경 파라미터 문자열
/// @return HRESULT
STDMETHODIMP CSSOClient::_EventHandler::raw_UpdateServiceParam( enum ServiceParam param, BSTR bstrParam )
{
	// You must logout. (-> sample에 이렇게 써 있었다. 로그아웃해야 하나?)
	switch ( param )
	{
		// 1 : avatar 변경되었음.
	case SP_AvataChanged:
		{
			break;
		}
		// 2 : Billing cash 변경(충전)되었음.
	case SP_BillingChanged:
		{
			break;
		}
	}
	return S_OK;
}

/// 동일한 PC에서 서비스에 중복로그인 했을 경우 이벤트가 발생합니다.
/// @param nSvcType 서비스타입(Launcher=1, Game=2, Web=3, Messenger=4)
/// @param bstrServiceID 로그인된 Service ID
/// @return HRESULT
STDMETHODIMP CSSOClient::_EventHandler::raw_ServiceLogonDuplicated( long nSvcType, BSTR bstrServiceID )
{
	// You must logout. (-> sample에 이렇게 써 있었다. 로그아웃해야 하나?)
	// TODO : 뭔가 해야 하나?
	return S_OK;
}

/// 서비스에 로그인 했을 경우 이벤트가 발생합니다.
/// @param nSvcType 서비스타입(Launcher=1, Game=2, Web=3, Messenger=4)
/// @param bstrServiceID 로그인된 Service ID
/// @return HRESULT
STDMETHODIMP CSSOClient::_EventHandler::raw_ServiceLogon( long nSvcType, BSTR bstrServiceID )
{
	// TODO : 뭔가 해야 하나?
	return S_OK;
}

/// 서비스에 로그아웃 했을 경우 이벤트가 발생합니다.
/// @param nSvcType 서비스타입(Launcher=1, Game=2, Web=3, Messenger=4)
/// @param bstrServiceID 로그인된 Service ID
/// @return HRESULT
STDMETHODIMP CSSOClient::_EventHandler::raw_ServiceLogout( long nSvcType, BSTR bstrServiceID )
{
	// TODO : 뭔가 해야 하나?
	return S_OK;
}

bool CSSOClient::RegisterSSOComponent()
{
	// '-s'를 붙여야 메세지 박스가 안 뜬다.
	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-s SSOIPC.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
	}

	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","SSODaemon.exe","-RegServer","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
	}

	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-s SSOAgent.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
	}	
	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-s CryptoHelper.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
	}

	return true;
}

bool CSSOClient::UnRegisterSSOComponent()
{
	// '-s'를 붙여야 메세지 박스가 안 뜬다.
	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-u -s CryptoHelper.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
		return false;
	}
	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-u -s SSOAgent.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
		return false;
	}

	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","SSODaemon.exe","-UnRegServer","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
		return false;
	}

	if ( reinterpret_cast<int>( ShellExecuteA( NULL,"open","regsvr32","-u -s SSOIPC.dll","./SSOModule", SW_HIDE ) ) <= 32 )
	{
		ASSERT( false );
		return false;
	}

	return true;

}

bool CSSOClient::CreateSSOClient()
{
	try
	{
		m_pSSOClientAgent = ISSOClientAgentPtr(__uuidof(SSOClientAgent));    // <-- First Create SSOAgent
		m_pSSOClientDaemon = ISSOClientDaemonPtr(__uuidof(SSOClientDaemon)); // <-- Next Create SSODaemon


		CComObject<_EventHandler>* pHandler;
		CComObject<_EventHandler>::CreateInstance( &pHandler );
		IUnknown* pUnk = m_pSSOClientDaemon;
		HRESULT hr = AtlAdvise( pUnk, pHandler->GetUnknown(), IID_ISSOClientDaemonEvents, &m_dwCookie );
		START_LOG( cout, L"AtlAdvise m_dwCookie : ") << BUILD_LOG( m_dwCookie );
					
		if(FAILED(hr))
		{
			START_LOG( cout, L"CreateInstance failed : ");
			throw "CreateInstance failed!";
		}
		
	}
	catch(_com_error &e)
	{
		WCHAR szErrMsg[256];
		_tcscpy( szErrMsg , e.ErrorMessage() );	
		MessageBoxW( NULL , szErrMsg , L"CreateSSOClient" , NULL );
        ASSERT( false );
		return false;
	}

	return true;
}

bool CSSOClient::ReleaseSSOClient()
{
	try
	{
		if ( m_pSSOClientDaemon )
		{
			IUnknown* pUnk = m_pSSOClientDaemon;
			AtlUnadvise( pUnk, IID_ISSOClientDaemonEvents, m_dwCookie );
		}
		m_pSSOClientAgent = NULL;
		m_pSSOClientDaemon = NULL;

	}
	catch(_com_error &e)
	{
		WCHAR szErrMsg[256];
		_tcscpy( szErrMsg , e.ErrorMessage() );	
		MessageBoxW( NULL , szErrMsg , L"ReleaseSSOClient" , NULL );
		ASSERT( false );
		return false;
	}

	return true;
}

std::wstring CSSOClient::GetServiceTicket( const char* strKDCAddr , const int& iKDCPort , const char* strServiceID )
{
	m_strSvcId = strServiceID;
	bstr_t bstrImgKey, bstrAssoc;
	bstrImgKey = _T("11111");
	bstrAssoc = _T("");


	//한번 발급받은 ST는 두 번 사용할 수 없다. 그렇기에 매번 함수를 불러서 새로 발급받자.
	std::wstring strTicket = L"";

	if( m_pSSOClientAgent )
	{		
		m_pSSOClientAgent->KDCAddress = strKDCAddr;	
		m_pSSOClientAgent->KDCPort	  = iKDCPort;		
	}
	else
	{
		return strTicket;
	}

	if ( !m_pSSOClientAgent->CheckTGT() )
	{
		/*bstr_t bstrLoginID , bstrLoginPwd;
		bstrLoginID  = "sdt123";
		bstrLoginPwd = "kog!@#";
		if (!m_pSSOClientAgent->RequestLogin(bstrLoginID,bstrLoginPwd))
		{*/
		ForceLogout();
		g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_KDC_CHECKTGT_ERR1 ).c_str(),
								 g_pkStrLoader->GetString( STR_ID_KDC_CHECKTGT_ERR2 ).c_str() );
		return strTicket;
  	}


	variant_t vtSt = m_pSSOClientAgent->GetServiceTicket( CType_Game, m_strSvcId.c_str(), TRUE, bstrImgKey, bstrAssoc);
	// This returned variant can be a BSTR if the method succeeded, otherwise it can be BOOL.
	if ( vtSt.vt == VT_BSTR )
	{
		strTicket = (wchar_t*)(_bstr_t)vtSt;
	}
	else
	{
		long iErrCode = m_pSSOClientAgent->GetLastErrorCode;
        START_LOG( cout, L"Failed To receive Service Ticket" );
		
		// 마지막 에러코드가 10021이면 클로즈 베타에 신청하지 않은 유저가 게임을 실행한 경우이다.
		if ( iErrCode == ERR_DIDNOTUSERMAPPING )
		{
			g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_ERR_CB_SERVICE_TICKET1 ).c_str(),
									 g_pkStrLoader->GetString( STR_ID_ERR_CB_SERVICE_TICKET2 ).c_str() );
		}
		else		
		{
			WCHAR strTemp[15];
			wsprintf( strTemp , L"(%d)" , iErrCode);
			std::wstring  strErr = strTemp;
			START_LOG( cout, L"Last ErrCode is : " ) << BUILD_LOG(strErr);

			strErr = g_pkStrLoader->GetString( STR_ID_KDC_GET_ST_ERROR2 ) + strErr;
			g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_KDC_GET_ST_ERROR1 ).c_str(),
									 strErr.c_str() );
		}
	}

	return strTicket;

}


HRESULT CSSOClient::StartSSOClient()
{
	//레지스트리에 SSO를 등록한다.
	//RegisterSSOComponent();

	//COM을 생성한다.
	if(CreateSSOClient() == false)
	{
		return E_FAIL;
	}
	else
	{		
		return S_OK;
	}
}

HRESULT CSSOClient::EndSSOClient()
{
	
	//게임 종료 전에, 로그인했던 서비스로부터 로그아웃해야 한다. 
	if( m_pSSOClientAgent == NULL )
		return E_FAIL;

	BOOL bTGT = m_pSSOClientAgent->CheckTGT();
	BOOL bSID = FALSE;
	if( m_strSvcId != "" )
	{
		bSID = TRUE;
	}
	else
	{
		START_LOG( cout, L"Service ID is NULL: ERR!  ");
	}

	if ( bTGT & bSID )
	{
		//정상 종료일 경우, 즉 자기가 RequestLogout할 때에는 Do_Status로 이벤트 날라가도 리턴하도록 변수 셋팅...
		m_bIsSelfLogout = true;

		if ( !(m_pSSOClientAgent->RequestLogout( CType_Game , ReasonNormal , m_strSvcId.c_str() )) )
		{
			START_LOG( cout, g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_ERR1 ).c_str());
  		}
	}

	//COM을 해제한다.
	if ( ReleaseSSOClient() == false )
	{
		return E_FAIL;
	}

	//레지스트리에서 제거한다.
	//UnRegisterSSOComponent();

	return S_OK;
}

HRESULT CSSOClient::ForceLogout( )
{
	if( m_pSSOClientAgent == NULL )
		return E_FAIL;

	START_LOG( cout, L"Request Logout Forced Called ");
	
	m_bIsSelfLogout = true;
	//서비스 ID를 NULL로 두면 모든 SSO관련 응용프로그램(웹,런쳐)는 로그아웃된다.
	if (!(m_pSSOClientAgent->RequestLogout( CType_Game , ReasonForcedLogout , "" ) ) ) 
	{
		START_LOG( cout, L"Request Logout Forced: failed ");

		//다른 메세지를 띄워야 한다. 지금 일로 들어오지 않아도, 즉 RequestLogOut이 성공해도 
		//DoStatusChange에서 이 메세지를 부르게 되어 있다. 
		/*KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED1 ).c_str(), 
						g_pkStrLoader->GetString( STR_ID_KDC_REQUEST_LOGOUT_FORCED2 ).c_str(),
						KMsgBox::MBOX_OK);*/
		return E_FAIL;
	}

	return S_OK;
}

#endif //SSO_LOGIN