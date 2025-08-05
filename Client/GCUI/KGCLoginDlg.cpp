#include "stdafx.h"
#include "KGCLoginDlg.h"
#include "../VersionDef.h"
#include "../MyD3D.h"
#include "KGCLoginScene.h"
#include "../GCStateMachine.h"
#include "../Procedure.h"
#include "NProtect.h"
#include <io.h>

#include "OpenProcess.h"

#if defined ( MD5_PASSWORD_LOGIN )
#include "../GCUtil/MD5/MD5Checksum.h"
#endif

#include "NetError.h"
#include "GCFatigueInfoBox.h"
#include "GCSmartPanel.h"
#include "GCVirtualKeyManager.h"

IMPLEMENT_CLASSNAME( KGCLoginDlg );
IMPLEMENT_WND_FACTORY( KGCLoginDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCLoginDlg, "login_dlg" );


extern void CheckFileSizeAndSHA( KENU_VERIFY_ACCOUNT_REQ* kReq_, std::string strFileName_, std::string strFilePath_ );
extern void SendCheckFileSizeAndSHA( KENU_VERIFY_ACCOUNT_REQ* kReq_);

extern std::wstring GetSHA1FromFile(char* strFilePath_ );

#define MAX_PWD_LENGTH 8

KGCLoginDlg::KGCLoginDlg(void)
: m_pkSaveID(NULL),
m_pkSmartPanel(NULL),
m_pkVirtualKeyManager(NULL),
m_bCursorInVK(false),
m_pkSaveIDMsg(NULL),
m_pkBackCapsLockOn(NULL),
m_pkCapsLockOnMsg(NULL)
{
	//가상 키보드
	m_pBtnVKPassID = NULL;
	m_pBtnVKPassWD = NULL;

    LINK_CONTROL( "edit_id",            m_pEditID );
    LINK_CONTROL( "edit_passwd",        m_pEditPasswd );
    LINK_CONTROL( "find_pass_bt",       m_pButtonFindPass );
#if defined(NATION_CHINA)
    LINK_CONTROL( "save_id",            m_pkSaveID );
    LINK_CONTROL( "save_id_msg",        m_pkSaveIDMsg );
    LINK_CONTROL( "smartpanel",         m_pkSmartPanel );
    LINK_CONTROL( "virtualkeyboard",    m_pkVirtualKeyManager );
    LINK_CONTROL( "virtualkeyboard_onoff_bt",    m_pButtonVK );
    LINK_CONTROL( "back_capslock_on_msg",   m_pkBackCapsLockOn );
    LINK_CONTROL( "capslock_on_msg",        m_pkCapsLockOnMsg );
#else
    LINK_CONTROL( "find_id_bt",         m_pButtonFindID );
    //가상 키보드
    LINK_CONTROL( "btn_vk_pw",          m_pBtnVKPassWD );
    LINK_CONTROL( "btn_vk_id",          m_pBtnVKPassID );
    LINK_CONTROL( "join_bt",            m_pButtonJoin );
#endif
    
    LINK_CONTROL( "login_bt",           m_pButtonLogin );
    LINK_CONTROL("checkbox_save_id",    m_pCheckBox );	
	
#if defined(NATION_THAILAND)
	LINK_CONTROL("btn_funbox",m_pWndMainBtnFunbox);
	LINK_CONTROL("btn_playpark",m_pWndMainBtnPlayPark);

	LINK_CONTROL("type_funbox",m_pWndTabBtnFunbox);
	LINK_CONTROL("type_playpark",m_pWndTabBtnPlayPark);

	LINK_CONTROL("background",m_pWndBackGround);
	LINK_CONTROL("background_fb",m_pWndBackGround_fb);
	LINK_CONTROL("background_pp",m_pWndBackGround_pp);
#endif

}

KGCLoginDlg::~KGCLoginDlg(void)
{
}

void KGCLoginDlg::ActionPerformed( const KActionEvent& event )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_LOGIN )
        return;
    
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            return;
        }
        case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            return;
        }
	}

    if( event.m_pWnd == m_pEditID )
    {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB )
        {
            SetSelectedID();
        }
        return;
    }

    GCWND_MSG_MAP( m_pEditPasswd, KD3DWnd::D3DWE_EDIT_RETURN, OnLogin )
    GCWND_MSG_MAP( m_pEditPasswd, KD3DWnd::D3DWE_EDIT_TAB, m_pEditID->SetFocus )
    GCWND_MSG_MAP( m_pCheckBox, KD3DWnd::D3DWE_BUTTON_CLICK, m_pCheckBox->ToggleCheck )
    GCWND_MSG_MAP( m_pButtonLogin, KD3DWnd::D3DWE_BUTTON_CLICK, OnLogin )

#if defined(NATION_THAILAND)

	if( event.m_pWnd == m_pWndMainBtnFunbox || event.m_pWnd ==m_pWndTabBtnFunbox)
	{
		if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		{
			m_iFunBoxPostfix = 0;
			UpdateLoginType();		
		}
	}

	if( event.m_pWnd == m_pWndMainBtnPlayPark || event.m_pWnd == m_pWndTabBtnPlayPark )
	{
		if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		{
			m_iFunBoxPostfix = 1;
			UpdateLoginType();		
		}
	}



#endif

#if !defined( NATION_USA ) && !defined( NATION_IDN ) && !defined( NATION_PHILIPPINE ) && !defined( NATION_EU )
    GCWND_MSG_MAP( m_pButtonJoin, KD3DWnd::D3DWE_BUTTON_CLICK, OnJoin )
	GCWND_MSG_MAP( m_pButtonFindID, KD3DWnd::D3DWE_BUTTON_CLICK, OnFindID )
	GCWND_MSG_MAP( m_pButtonFindPass, KD3DWnd::D3DWE_BUTTON_CLICK, OnFindPass )
#elif defined(NATION_CHINA)
    GCWND_MSG_MAP( m_pButtonFindPass, KD3DWnd::D3DWE_BUTTON_CLICK, OnFindPass )
    GCWND_MSG_MAP( m_pkSaveID, KD3DWnd::D3DWE_BUTTON_CLICK, m_pCheckBox->ToggleCheck )

    if( m_pkSmartPanel && ( event.m_pWnd == m_pkSmartPanel )  && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        SetSelectedID();
    }
    if( m_pkVirtualKeyManager && ( event.m_pWnd == m_pkVirtualKeyManager )  && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnVirtualKeyDown( event.m_dwlParam );
    }
    if( m_pButtonVK && ( event.m_pWnd == m_pButtonVK )  && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if ( m_pkVirtualKeyManager->IsRenderOn() )
        {
            m_pkVirtualKeyManager->ToggleRender(false);
            m_pButtonVK->Activate(true);
        }
        else
        {
            m_pkVirtualKeyManager->ToggleRender(true);
            m_pButtonVK->Activate(false);
        }
    }
#endif
}
void KGCLoginDlg::OnCreate( void )
{

    m_pEditID->InitState( true, false, this );
    m_pEditPasswd->InitState( true, false, this );
    m_pEditPasswd->SetPasswd( true );

    // 아이디 입력창에 각 나라별 디비에 저장되는 아이디 길이만큼만 입력할수있게 한다
    m_pEditID->SetLimitText( MAX_LOGIN_ID_SIZE );
    
    #if defined( NATION_HONGKONG ) || defined( NATION_TAIWAN ) || defined( NATION_CHINA )
    {
        m_pEditPasswd->SetLimitText( MAX_LOGIN_PASSWORD_SIZE );
    }
    #endif


#if defined(NATION_THAILAND)

	m_pWndMainBtnPlayPark->InitState( true, true, this );
	m_pWndMainBtnFunbox->InitState( true, true, this );

	m_pWndTabBtnFunbox->InitState( true, true, this );
	m_pWndTabBtnPlayPark->InitState( true, true, this );

#endif


	if ( m_pBtnVKPassWD )
	{
		m_pBtnVKPassWD->InitState( false );
	}
	if ( m_pBtnVKPassID )
	{
		m_pBtnVKPassID->InitState( false );
	}

#if defined( NATION_HONGKONG )
	m_pButtonFindPass->InitState( true, true, this );
	m_pButtonFindID->InitState( true, true, this );
	m_pButtonJoin->InitState( true, true, this );
#elif !defined( NATION_USA ) && !defined( NATION_EU ) && !defined(NATION_CHINA)
	m_pButtonFindPass->InitState( true, true, this );
	m_pButtonFindID->InitState( true, true, this );
	m_pButtonJoin->InitState( true, true, this );
#elif defined(NATION_CHINA)
    m_pButtonFindPass->InitState( true, true, this );
    m_pkSaveID->InitState(true,true,this);
    m_pkSaveIDMsg->InitState(false,true,this);
#endif
    m_pButtonLogin->InitState( true, true, this );
    m_pCheckBox->InitState( true, true, this );

#if defined( NATION_IDN ) || defined( NATION_PHILIPPINE )
    m_pButtonFindPass->ToggleRender( false );
    m_pButtonFindID->ToggleRender( false );
    m_pButtonJoin->ToggleRender( false );
#endif

#if defined(NATION_THAILAND)
	UpdateLoginType();
#endif



#if !defined( __PATH__ ) && !defined( NATION_CHINA )
	char strTemp[MAX_PATH] = {0, };
	const char seps[] = ",";
	char *token;


	if( __argc > 1 )
	{
		strcpy( strTemp, __argv[1] );
		token = strtok( strTemp, seps );
		token = strtok( NULL, seps );
		token = strtok( NULL, seps );
		if( token != NULL )
		{
			m_pEditID->SetText( GCUTIL_STR::GCStrCharToWide( token ) );
			m_pEditPasswd->SetText( GCUTIL_STR::GCStrCharToWide( token ) );
		}
	}
#endif

#ifdef NATION_CHINA
    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_AGREEMENT_OK);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_AGREEMENT_OK, this, &KGCLoginDlg::OnAgreementOK);
    if ( m_pkSmartPanel )
        m_pkSmartPanel->InitState( true, true, this );
    if ( m_pkVirtualKeyManager )
        m_pkVirtualKeyManager->InitState( true, true, this );
    if ( m_pButtonVK )
        m_pButtonVK->InitState( true, true, this );
    if ( m_pkBackCapsLockOn )
        m_pkBackCapsLockOn->InitState(false);
    if ( m_pkCapsLockOnMsg )
    {
        m_pkCapsLockOnMsg->InitState(false);
        m_pkCapsLockOnMsg->SetAlign( DT_CENTER );
    }
#endif
}

void KGCLoginDlg::FrameMoveInEnabledState()
{
    // 키를 누르는 소리
    if( m_pEditID->HasFocus())
    {
        for ( int i = 0; i < 256; i++ )
        {
            if ( g_pkInput->IsDown( i ) ){
                g_KDSound.Play( "71" );
                KeydownEditNick();
            }
        }
    }
#if defined(NATION_CHINA)
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if ( m_pkVirtualKeyManager->IsRenderOn() 
        && m_pkVirtualKeyManager->CheckPosInWindowBound(vMousePos) )
        m_bCursorInVK = true;
    else
        m_bCursorInVK = false;

    m_pkSaveIDMsg->ToggleRender(m_pCheckBox->GetCheck());

    if ( m_pkSmartPanel && m_pEditID->HasFocus() )
    {
        m_pkSmartPanel->SetEditID( m_pEditID->GetText() );
    }

    if ( m_pEditPasswd->HasFocus() )
    {
        if( ::OpenClipboard( NULL ) )
        {
            ::EmptyClipboard();
            ::CloseClipboard();
        }

        if ( GetKeyState(VK_CAPITAL) )
            CapsLockOn( true );
        else
            CapsLockOn( false );
    }
    else
        CapsLockOn( false );
#endif
}

#if defined(NATION_THAILAND)
void KGCLoginDlg::UpdateLoginType()
{
	if (m_iFunBoxPostfix==-1)
	{
		
		m_pEditID->ToggleRender(false);
		m_pEditPasswd->ToggleRender(false);
		m_pButtonFindPass->ToggleRender(false);
		m_pButtonFindID->ToggleRender(false);
		m_pButtonLogin->ToggleRender(false);
		m_pButtonJoin->ToggleRender(false);
		m_pCheckBox->ToggleRender(false);

		m_pWndTabBtnFunbox->ToggleRender(false);
		m_pWndTabBtnPlayPark->ToggleRender(false);
		m_pWndBackGround->ToggleRender(false);

		m_pWndMainBtnPlayPark->ToggleRender(true);
		m_pWndMainBtnFunbox->ToggleRender(true);

		m_pWndBackGround->ToggleRender(true);
		m_pWndBackGround_fb->ToggleRender(false);
		m_pWndBackGround_pp->ToggleRender(false);

	}
	else
	{
		m_pEditID->ToggleRender(true);
		m_pEditPasswd->ToggleRender(true);
		m_pButtonFindPass->ToggleRender(true);
		m_pButtonFindID->ToggleRender(true);
		m_pButtonLogin->ToggleRender(true);
		m_pButtonJoin->ToggleRender(true);
		m_pCheckBox->ToggleRender(true);

		m_pWndTabBtnFunbox->ToggleRender(true);
		m_pWndTabBtnPlayPark->ToggleRender(true);
		m_pWndBackGround->ToggleRender(true);

		m_pWndMainBtnPlayPark->ToggleRender(false);
		m_pWndMainBtnFunbox->ToggleRender(false);


		m_pWndBackGround->ToggleRender(false);

		if (m_iFunBoxPostfix==0)
		{
			m_pWndTabBtnFunbox->Lock(true);
			m_pWndTabBtnPlayPark->Lock(false);
			m_pWndBackGround_fb->ToggleRender(true);
			m_pWndBackGround_pp->ToggleRender(false);
			
		}
		else
		{
			m_pWndTabBtnFunbox->Lock(false);
			m_pWndTabBtnPlayPark->Lock(true);
			m_pWndBackGround_fb->ToggleRender(false);
			m_pWndBackGround_pp->ToggleRender(true);
		}

	}
}
#endif


void KGCLoginDlg::SkipLogin() {
	OnLogin();
}

void KGCLoginDlg::OnLogin()
{
	Result_Connect_LoginServer = INT_MAX;

	KENU_VERIFY_ACCOUNT_REQ kReq;
	kReq.m_dwAuthType = g_kGlobalValue.m_dwAuthType;
	kReq.m_nProtocolVer = LOGIN_SERVER_VERSION;

    //kReq.m_hdSerial = g_MyD3D->GetSerialNumber();

	if (g_kGlobalValue.m_bLoginFirst) {

		if (wcslen(m_pEditID->GetText()) == 0)
		{
			std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_EMPTY_ID);
			((KGCLoginScene*)m_pParent)->OnError(this, strMsg.c_str());
			return;
		}
		if (wcslen(m_pEditPasswd->GetText()) == 0)
		{
			std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_EMPTY_PASSWD);
			((KGCLoginScene*)m_pParent)->OnError(this, strMsg.c_str());
			return;
		}

		kReq.m_strLogin = GCUTIL_STR::GCStrWideToChar(m_pEditID->GetText());
	}
	else {
		kReq.m_strLogin = g_kGlobalValue.m_strUser;
	}

	kReq.m_bBeanfunUser = false;
	//    kReq.m_iAge = 14;

#if defined(__PATH__) && defined(NATION_TAIWAN)
    std::wstring wstrLoginID = m_pEditID->GetText();
    NPGameGuardSend( wstrLoginID.c_str() );
#endif

	//FunBox의 경우, Asiasoft인지 funbox인지 구분 짓게 한다.
#if defined(NATION_THAILAND)
#if defined(__PATH__)
	//본섭일 경우에만 구분짓도록 처리
	kReq.m_nFunBoxPostfix = m_iFunBoxPostfix; 
#else
	kReq.m_nFunBoxPostfix = m_iFunBoxPostfix; // 일단 태섭도 구분합니다.
	kReq.m_nFunBoxBonus = 0;
#endif
#else
	kReq.m_nFunBoxBonus = 0;
#endif

    #if defined ( MD5_PASSWORD_LOGIN )
    {
		if (g_kGlobalValue.m_bLoginFirst) {
			std::wstring strMD5String;
			int iPasswdSize = ::lstrlenW(m_pEditPasswd->GetText());
			BYTE* pbyteData = new BYTE[iPasswdSize + 1];
			::memcpy(pbyteData, KncUtil::toNarrowString(m_pEditPasswd->GetText(), iPasswdSize + 1).c_str(), (size_t)(iPasswdSize + 1));
			strMD5String = CMD5Checksum::GetMD5(pbyteData, iPasswdSize);
			kReq.m_strPasswd = KncUtil::toNarrowString(strMD5String);
			SAFE_DELETE_ARRAY(pbyteData);
		}
		else {
			kReq.m_strPasswd = g_kGlobalValue.m_strPass;
		}
    }


    #else
    {
        kReq.m_strPasswd = GCUTIL_STR::GCStrWideToChar( m_pEditPasswd->GetText() );

        // 아이디나 패스워드에 잘못된 문자가 들어있으면 걸러준다
        if( !CheckValidate( kReq.m_strLogin ) || !CheckValidate( kReq.m_strPasswd ) )
        {
            if ( !CheckValidate( kReq.m_strLogin ) )
            {
                m_pEditID->SetText( L"" );
            }
            if ( !CheckValidate( kReq.m_strPasswd ) )
            {
                m_pEditPasswd->SetText( L"" );
            }

            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1 ) + L"\n"
                                + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2 );
            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str() );

            return;
        }
    }
    #endif

    m_pEditPasswd->SetText(L"");
    // 로긴 서버 접속
    if ( KP2P::GetInstance()->IsConnectedToLoginServer() == false )
    {
        ConnectLoginServer();
    }

    //ID저장
    g_MyD3D->m_KGCOption.SetSaveID( m_pCheckBox->GetCheck(), (WCHAR*)m_pEditID->GetText() );

#if defined (NATION_THAILAND)
	g_MyD3D->m_KGCOption.SetSaveFunboxPostFix( m_pCheckBox->GetCheck(), m_iFunBoxPostfix );
#endif	


#if defined(CHECK_DUPLICATED_FILE)
    if(g_pGCDeviceManager2->GetMassFileManager()->IsDuplicateExist()){

        KP2P::GetInstance()->Send_ENU_OVERLAP_FILE_INFO();

        std::vector<KomfileManager::SDuplicateInfo> vecDuplicateFile = g_pGCDeviceManager2->GetMassFileManager()->GetDuplicatedFileList();

        std::vector<std::string> deleteFile;
    #ifdef __PATH__
        std::vector<KomfileManager::SDuplicateInfo>::iterator vit = vecDuplicateFile.begin();
        for(;vit != vecDuplicateFile.end();vit++){
            if(std::find(deleteFile.begin(),deleteFile.end(),vit->strOriginKomFileName) == deleteFile.end()){
                DeleteFileA(vit->strOriginKomFileName.c_str());
                deleteFile.push_back(vit->strOriginKomFileName);
            }
            if(std::find(deleteFile.begin(),deleteFile.end(),vit->strDuplicatedKomFileName) == deleteFile.end()){
                DeleteFileA(vit->strDuplicatedKomFileName.c_str());
                deleteFile.push_back(vit->strDuplicatedKomFileName);
            }
        }
    #endif  //  ifdef __PATH__
        MessageBoxW( NULL, g_pkStrLoader->GetString( STR_ID_DUPLICATED_FILE_ERROR ).c_str(), NULL, MB_OK );

    #ifdef __PATH__
        exit(0);
		return;
    #endif //   ifdef __PATH__
    }
#endif  //  if defined(CHECK_DUPLICATED_FILE)    


//#if !defined(_DEBUG)
	//DWORD dwPrevTime = ::timeGetTime();
    //CheckFileSizeAndSHA(&kReq, "main.exe",          "main.exe");
    //CheckFileSizeAndSHA(&kReq, "script.kom",        "stage/script.kom");
    //CheckFileSizeAndSHA(&kReq, "char_script.kom",   "stage/char_script.kom");
    //CheckFileSizeAndSHA(&kReq, "ai.kom",            "stage/ai/ai.kom");
    //CheckFileSizeAndSHA(&kReq, "fan_map.kom",       "stage/fan_map/fan_map.kom");
    //CheckFileSizeAndSHA(&kReq, "ui.kom",            "stage/ui/ui.kom");
    //CheckFileSizeAndSHA(&kReq, "playertemplate.kom",	"stage/playertemplate.kom");
    //CheckFileSizeAndSHA(&kReq, "aipet.kom",           "stage/aipet.kom");
    //CheckFileSizeAndSHA(&kReq, "minigame.kom",	       "stage/MiniGame/minigame.kom");
    //CheckFileSizeAndSHA(&kReq, "map.kom",             "map.kom");

	//DWORD dwCurTime = ::timeGetTime() - dwPrevTime;
	//char str[1024] = {0,};
	//sprintf_s( str, "소모시간 %d ms", dwCurTime );
	//::MessageBoxA( NULL, str, NULL, MB_OK );
//#endif
	SendCheckFileSizeAndSHA(&kReq);

    g_kGlobalValue.RunLogProgram("10");
    g_kGlobalValue.RunLogProgram("12");

	//
    KP2P::GetInstance()->m_spNUserProxy->SendPacket( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, kReq, false, true );

    g_MyD3D->WaitForServerAck( Result_Connect_LoginServer, INT_MAX, 50000, TIME_OUT_VALUE );

    // 0 - 성공
    // 1 - Protocol Version이 틀림
    // 2 - 아직 행동이 정의되지 않은 인증방식
    // 3 - 인증 실패
    // 4 - rename 실패
    // 5 - 이중접속 감지
    // 6 - Center Gash 에서 Login 을 얻을수 없다
    // 7 - ID or Password에 이상한 문자들어가있음
    // 8 - 요청한 유저와 반환 유저가 다름_Gash_
    // 9 - Login 길이가 0 이다
    // 10 - 틀린 Password
    // 11 - 존재하지 않는 계정
    // 12 - 신규 유저 등록 실패
    // 13 - 연령 제한

	// 23 - 플레이파크 이메일 중복 오류

    switch( Result_Connect_LoginServer )
    {
        case 0: // 성공
        {
            //g_kGlobalValue.m_kLoginInfo.strID = kReq.m_strLogin;
            //g_kGlobalValue.m_kLoginInfo.strPass = kReq.m_strPasswd;

            g_MyD3D->m_pStateMachine->GoServer();            

#if defined(SHA_CHECK)
            //SHA 체크 가능하도록 bool 변수 셋팅
            KSHAChecker::GetInstance()->SetbWork(true);
#endif
            return;
        }
        case 1: // 프로토콜 버젼이 다름
        case 15: // script crc가 다름
        {
            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION1 ) + L"\n"
                                + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION2 );

            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), true );

            return;
        }
        case 2: // 아직 행동이 정의되지 않은 인증방식
        {
            return;
        }
        case 4: // rename 실패
        case 6: // Center Gash 에서 Login 을 얻을수 없다
        case 8: // 요청한 유저와 반환 유저가 다름_Gash
        {
            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_GASH_CERTIFY_FAIL1 ) + L"\n"
                + g_pkStrLoader->GetString( STR_ID_INQUIRE_TO_CUSTOMER_SUPPORT );

            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), true );
            return;
        }
        case 5: // 이중접속 감지
        {
            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT1 ) + L"\n"
                                + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT2 );

            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), true );
            return;
        }
		case 3: // 090408. 틀린패스워드나 존재하지 않는 계정일때 이걸 보내준다함. 10,11은 현재 쓰지 않는대요.
        case 7: // ID or Password에 이상한 문자들어가있음
        case 9: // Login 길이가 0 이다
        case 11: // 존재하지 않는 계정
        case 12: // 신규 유저 등록 실패
        case NetError::ERR_NUSER_VERIFY_31 :
            if ( SiKGCFatigueManager()->GetConnectType() == KGCFatigueManager::FUT_MULTI_CONNECT ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FATIGUE_SYSTEM_MESSAGE2, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                return;
            }
            else
            {
                std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1 ) + L"\n"
                                    + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2 );

                ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str() );
                return;
            }
        case 10: // 틀린 Password
        {
            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1 ) + L"\n"
                + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2 );

            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str() );
            return;
        }
		case 14:
		{
			std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_ACCESS_RESTRICT );

			( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str() );
			return;
		}
		// 연령제한 걸리면..한국 이외에는 어떻게 되는지 몰라..
		case 13:
		{
			std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_YOUNGER_BOY_CONNECT1 ) + L"\n"
				+ g_pkStrLoader->GetString( STR_ID_YOUNGER_BOY_CONNECT2 );
			( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str() );
			return;
			
		}
        case 16:
        {
            g_MyD3D->ExitWithMsgDlg(	g_pkStrLoader->GetString( STR_ID_LOGIN_ERROR_MESSAGE ).c_str(), L"" );
                return;
        }
		case 18:	//Wrong OTP : Bean Fun
        {
            g_MyD3D->ExitWithMsgDlg(	g_pkStrLoader->GetString( STR_ID_BEANFUN_ERROR1 ).c_str(), L"" );
            return;
        }
        case 19:    //빈펀으로 전환한 유저가 빈펀이 아닌 패쳐실행을 통해 접속함
		{
			g_MyD3D->ExitWithMsgDlg(	g_pkStrLoader->GetString( STR_ID_BEANFUN_ERROR0 ).c_str(),
				L"" );
			return;
		}
		case 20: //웹 인증실패
			{
				//g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_LOGIN_ERROR_MESSAGE2), 
				//	L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
				//	false, true );

				std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_LOGIN_ERROR_MESSAGE2 );

				( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), false );
				return;
			}
		case 23:
			{
				std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_PLAYPARK_EMAIL_ERROR );
				( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), true );
				return;
			}
        case NetError::ERR_NUSER_VERIFY_35:
        	SiKGCFatigueManager()->SetAuthType( KGCFatigueManager::FUT_AUTH_NONE );
        	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FATIGUE_SYSTEM_MESSAGE2, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        	return;
        case -41585 :
        case NetError::ERR_NUSER_VERIFY_37 :
        case NetError::ERR_NUSER_VERIFY_11:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_JOIN_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_10:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_63:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_38:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_64:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_5 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_34:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_6 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_39:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_JOIN_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_7 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_40:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_8 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_41:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_42:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_10 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_43:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_11 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_44:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_12 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_45:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_13 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_46:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_14 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_47:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_15 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_48:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_16 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_49:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_17 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_50:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_18 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_51:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_19 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_52:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_20 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_53:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_21 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_54:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_22 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_55:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_23 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_56:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_25 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_57:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_26 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_58:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_27 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_59:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_28 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_60:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_29 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_61:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_30 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_62:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_31 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_65:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_24 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        case NetError::ERR_NUSER_VERIFY_66:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK_CANSEL_BOX, g_pkStrLoader->GetString( STR_BILLING_CN_MESSAGE_33 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;

        case TIME_OUT_VALUE:
        default:
        {
            std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1 ) + L"\n"
                                + g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2 );

            strMsg += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, Result_Connect_LoginServer );	
            
            ( (KGCLoginScene*)m_pParent )->OnError( this, strMsg.c_str(), true );
            return;
        }
    }    
}

void KGCLoginDlg::OnJoin()
{
    if ( !g_kGlobalValue.m_bAlreadyRegistered )
        g_MyD3D->m_pStateMachine->GoRegister();
}

void KGCLoginDlg::OnFindID()
{
#if defined(NATION_THAILAND)
	if (m_iFunBoxPostfix==0)
		PopUpWebPage( HOMEPAGE_FINDID );
	else
		PopUpWebPage( HOMEPAGE_FINDID2 );
#else
	PopUpWebPage( HOMEPAGE_FINDID );
#endif
}

void KGCLoginDlg::OnFindPass()
{
#if defined(NATION_THAILAND)
	if (m_iFunBoxPostfix==0)
		PopUpWebPage( HOMEPAGE_FINDPASS );
	else
		PopUpWebPage( HOMEPAGE_FINDPASS2 );
#else
	PopUpWebPage( HOMEPAGE_FINDPASS );
#endif
}

void KGCLoginDlg::ConnectLoginServer()
{    
    g_kGlobalValue.ServerInfo.dwServerIP = ::inet_addr(LOGIN_SERVER_IP);
    g_kGlobalValue.ServerInfo.usServerPort = std::stoi(LOGIN_SERVER_PORT);

    if( KP2P::GetInstance()->Init( g_MyD3D->m_hWnd ) == false )
    {
        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(),
                       g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str(),
                       KMsgBox::MBOX_OK );
        g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(),
                                 g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str() );
        return;
    }

    if( KP2P::GetInstance()->ConnectToLoginServer(LOGIN_SERVER_IP, g_kGlobalValue.ServerInfo.usServerPort, false ) == false )
    {
        g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL1 ).c_str(),
                       g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL2 ).c_str() );       

        return;
    }
#if defined(CHECK_DUPLICATED_FILE)
    if(g_pGCDeviceManager2->GetMassFileManager()->IsDuplicateExist()){
        KP2P::GetInstance()->Send_ENU_OVERLAP_FILE_INFO();
        MessageBoxW( NULL, g_pkStrLoader->GetString( STR_ID_DUPLICATED_FILE_ERROR ).c_str(), NULL, MB_OK );
    }
#endif
}

bool KGCLoginDlg::CheckValidate( std::string& strData )\
{

#ifndef __PATH__
	return true;
#endif

    char chNum[2]   = {'0','9'};
    char loAlpha[2] = {'a','z'};
    char upAlpha[2] = {'A','Z'};

    std::string::iterator sit;
    for( sit = strData.begin() ; sit != strData.end() ; ++sit )
    {
        if( !(  ( chNum[0] <= *sit && chNum[1] >= *sit ) ||
            ( loAlpha[0] <= *sit && loAlpha[1] >= *sit ) ||
            ( upAlpha[0] <= *sit && upAlpha[1] >= *sit ))  )
        {
            return false;
        }
    }
    return true;
}

void KGCLoginDlg::SetIDString( const std::wstring& strIDString_ )
{ 
	m_pEditID->SetText( strIDString_.c_str() );
}

void KGCLoginDlg::SetPWString( const std::wstring& strPWString_ )
{
	m_pEditPasswd->SetText( strPWString_.c_str() );
}

void KGCLoginDlg::OnAgreement()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AGREEMENT2, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

}

void KGCLoginDlg::OnAgreementOK()
{
    g_MyD3D->m_pStateMachine->GoServer();            

}

void KGCLoginDlg::KeydownEditNick()
{
#if defined(NATION_CHINA)

    std::wstring strCheckNewNickname = m_pEditID->GetText();  //글자를 얻는다.  

    if( strCheckNewNickname.empty() )
    {
        return;
    }


    static WCHAR strProhibition[] ={ L' ', 9, L'\'', L'\"', L',', L'　', 10,13  };
    std::wstring strTemp = L"";
    for ( int j = 0 ; j < (int)strCheckNewNickname.size() ; ++j) {
        bool bValid = true;

        for ( int i = 0 ; i < (sizeof(strProhibition) / sizeof(wchar_t)) ; ++i ){
            if ( strCheckNewNickname[j] == strProhibition[i] ) {
                bValid = false;
            }
        }
        if ( bValid )
            strTemp += strCheckNewNickname[j];
    }


    if ( strCheckNewNickname != strTemp )
        m_pEditID->SetText(strTemp.c_str(),false);
    //m_pkEditNickname->GetCurCursorIndex()
    //strCheckNewNickname->

#endif


}

void KGCLoginDlg::SetSelectedID()
{
    m_pEditPasswd->SetFocus();
#if defined(NATION_CHINA)
    m_pEditID->SetText( m_pkSmartPanel->GetSelectedID().c_str() );
#endif
}

bool KGCLoginDlg::IsRenderSmartPanel()
{
    return m_pkSmartPanel->IsRenderPopup();
}

void KGCLoginDlg::OnInitialize()
{
    m_pCheckBox->SetCheck( g_MyD3D->m_KGCOption.m_bSaveID );
    m_pEditID->SetFocus();

    if( m_pCheckBox->GetCheck() )
    {
        m_pEditID->SetText( g_MyD3D->m_KGCOption.m_wcsUserID );
        m_pEditPasswd->SetFocus();
		if( m_pkSaveIDMsg != NULL )
			m_pkSaveIDMsg->ToggleRender(true);
    }
#ifdef NATION_CHINA
    m_pkCapsLockOnMsg->SetText(g_pkStrLoader->GetString( STR_ID_CAPSLOCK_ON));
#endif
}

void KGCLoginDlg::OnVirtualKeyDown( KEVETPARM param_ )
{
    int idx;
    std::wstring strTemp;

    switch( param_ )
    {
    case KGCVirtualKeyManager::EVK_TEXT:
        if ( m_pEditID->HasFocus() )
        {
            strTemp = m_pEditID->GetText();
            idx = m_pEditID->GetCursorIndex();

            strTemp.insert( idx, m_pkVirtualKeyManager->GetClickKeyData() );
            m_pEditID->SetText( strTemp.c_str(), false );
            m_pEditID->SetSelection( idx + 1 , idx + 1 );
        }
        else if ( m_pEditPasswd->HasFocus() )
        {
            strTemp = m_pEditPasswd->GetText();
            idx = m_pEditPasswd->GetCursorIndex();

            strTemp.insert( idx, m_pkVirtualKeyManager->GetClickKeyData() );
            m_pEditPasswd->SetText( strTemp.c_str(), false );
            m_pEditPasswd->SetSelection( idx + 1 , idx + 1 );
        }
        break;
    case KGCVirtualKeyManager::EVK_BACKSPACE:

        if ( m_pEditID->HasFocus() )
        {
            strTemp = m_pEditID->GetText();
            idx = m_pEditID->GetCursorIndex() - 1;

            if ( idx >= 0 )
            {
                strTemp.erase( idx, 1 );
                m_pEditID->SetText( strTemp.c_str(), false );
                m_pEditID->SetSelection( idx, idx );
            }
        }
        else if ( m_pEditPasswd->HasFocus() )
        {
            strTemp = m_pEditPasswd->GetText();
            idx = m_pEditPasswd->GetCursorIndex() - 1;

            if ( idx >= 0 )
            {
                strTemp.erase( idx, 1 );
                m_pEditPasswd->SetText( strTemp.c_str(), false );
                m_pEditPasswd->SetSelection( idx, idx );
            }
        }
        break;
    case KGCVirtualKeyManager::EVK_ENTER:
        if ( m_pEditID->HasFocus() )
            SetSelectedID();
        else if ( m_pEditPasswd->HasFocus() )
            OnLogin();
        break;
    }        

    m_pkVirtualKeyManager->ShuffleKeyData();
}

void KGCLoginDlg::CapsLockOn( bool capslock_ )
{
    m_pkBackCapsLockOn->ToggleRender(capslock_);

#ifdef NATION_CHINA
    m_pkCapsLockOnMsg->ToggleRender(capslock_);
#endif
}