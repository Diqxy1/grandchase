#include "stdafx.h"
#include "KGCRegisterDlg.h"
#include "../VersionDef.h"
#include "../MyD3D.h"
#include "KGCRegisterScene.h"
#include "../GCStateMachine.h"
#include "../Procedure.h"
#include "NProtect.h"
#include <io.h>
#include "OpenProcess.h"
#if defined ( MD5_PASSWORD_LOGIN )
#include "../GCUtil/MD5/MD5Checksum.h"
#endif
#include "NetError.h"

IMPLEMENT_CLASSNAME( KGCRegisterDlg );
IMPLEMENT_WND_FACTORY( KGCRegisterDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRegisterDlg, "register_dlg" );

#define MAX_PWD_LENGTH 8

KGCRegisterDlg::KGCRegisterDlg(void)
{
    LINK_CONTROL( "edit_id",            m_pEditID );
    LINK_CONTROL( "edit_passwd",        m_pEditPasswd );
    LINK_CONTROL( "edit_email",         m_pEditEmail );
    LINK_CONTROL( "back_bt",            m_pButtonBack );
    LINK_CONTROL( "confirm_bt",         m_pButtonRegister );
}

KGCRegisterDlg::~KGCRegisterDlg(void)
{
}

void KGCRegisterDlg::ActionPerformed( const KActionEvent& event )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_REGISTER )
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

    GCWND_MSG_MAP( m_pEditPasswd, KD3DWnd::D3DWE_EDIT_TAB, m_pEditEmail->SetFocus )
    GCWND_MSG_MAP( m_pEditEmail, KD3DWnd::D3DWE_EDIT_RETURN, OnRegister )
    GCWND_MSG_MAP( m_pButtonRegister, KD3DWnd::D3DWE_BUTTON_CLICK, OnRegister )
    GCWND_MSG_MAP( m_pButtonBack, KD3DWnd::D3DWE_BUTTON_CLICK, OnBack )
}
void KGCRegisterDlg::OnCreate( void )
{
    m_pEditID->InitState( true, false, this );
    m_pEditPasswd->InitState( true, false, this );
    m_pEditPasswd->SetPasswd( true );
    m_pEditEmail->InitState( true, false, this );
    m_pEditID->SetLimitText( MAX_LOGIN_ID_SIZE );
	m_pButtonBack->InitState( true, true, this );
    m_pButtonRegister->InitState( true, true, this );
}

void KGCRegisterDlg::FrameMoveInEnabledState()
{
    if( m_pEditID->HasFocus())
    {
        for ( int i = 0; i < 256; i++ )
        {
            if ( g_pkInput->IsDown( i ) ){
                g_KDSound.Play( "71" );
            }
        }
    }
}

void KGCRegisterDlg::OnRegister()
{
    if ( !g_kGlobalValue.m_bAlreadyRegistered )
    {
        Result_Register_User = INT_MAX;

        KENU_ACCOUNT_REGISTER_REQ kReq;
        kReq.m_nProtocolVer = LOGIN_SERVER_VERSION;

        if (wcslen(m_pEditID->GetText()) == 0)
        {
            std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_EMPTY_ID);
            ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str());
            return;
        }
        if (wcslen(m_pEditPasswd->GetText()) == 0)
        {
            std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_EMPTY_PASSWD);
            ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str());
            return;
        }
        if (wcslen(m_pEditEmail->GetText()) == 0)
        {
            std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_EMPTY_EMAIL);
            ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str());
            return;
        }

        kReq.m_strLogin = GCUTIL_STR::GCStrWideToChar(m_pEditID->GetText());
        kReq.m_strEmail = GCUTIL_STR::GCStrWideToChar(m_pEditEmail->GetText());

#if defined ( MD5_PASSWORD_LOGIN )
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
#endif

        if (KP2P::GetInstance()->IsConnectedToLoginServer() == false)
        {
            ConnectLoginServer();
        }

        KP2P::GetInstance()->m_spNUserProxy->SendPacket(KNUserEvent::ENU_ACCOUNT_REGISTER_REQ, kReq, false, true);
        g_MyD3D->WaitForServerAck(Result_Register_User, INT_MAX, 50000, TIME_OUT_VALUE);

        switch ( Result_Register_User )
        {
            case 0:
            {
                OnBack();
                return;
            }
            case -1:
            {
                std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_REGISTER_TEXT_02);
                ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str());
                return;
            }
            case -2:
            {
                std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_REGISTER_TEXT_03);
                ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str());
                return;
            }
            case TIME_OUT_VALUE:
            default:
            {
                std::wstring strMsg = g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1) + L"\n" + g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2);
                strMsg += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KNUserEvent::ENU_ACCOUNT_REGISTER_REQ, Result_Register_User);
                ((KGCRegisterScene*)m_pParent)->OnError(this, strMsg.c_str(), true);
                return;
            }
        }
    }
    else
    {

    }
}

void KGCRegisterDlg::OnBack()
{
    g_MyD3D->m_pStateMachine->GoLogin();
}

void KGCRegisterDlg::ConnectLoginServer()
{    
    g_kGlobalValue.ServerInfo.dwServerIP = ::inet_addr( LOGIN_SERVER_IP );
    g_kGlobalValue.ServerInfo.usServerPort = std::stoi( LOGIN_SERVER_PORT );

    if( KP2P::GetInstance()->Init( g_MyD3D->m_hWnd ) == false )
    {
        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(), g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str(), KMsgBox::MBOX_OK );
        g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(), g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str() );
        return;
    }

    if( KP2P::GetInstance()->ConnectToLoginServer(LOGIN_SERVER_IP, g_kGlobalValue.ServerInfo.usServerPort, false ) == false )
    {
        g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL1 ).c_str(), g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL2 ).c_str() );       
        return;
    }
}

bool KGCRegisterDlg::CheckValidate( std::string& strData )
{
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

void KGCRegisterDlg::SetIDString( const std::wstring& strIDString_ )
{ 
	m_pEditID->SetText( strIDString_.c_str() );
}

void KGCRegisterDlg::SetPWString( const std::wstring& strPWString_ )
{
	m_pEditPasswd->SetText( strPWString_.c_str() );
}

void KGCRegisterDlg::SetEmailString( const std::wstring& strEmailString_ )
{ 
	m_pEditEmail->SetText( strEmailString_.c_str() );
}

void KGCRegisterDlg::OnInitialize()
{
    m_pEditID->SetFocus();
}