#include "stdafx.h"
#include "GCAccountNickNameFrame.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCAccountNickNameFrame );
IMPLEMENT_WND_FACTORY( KGCAccountNickNameFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCAccountNickNameFrame, "gc_account_nickname_frame" );

KGCAccountNickNameFrame::KGCAccountNickNameFrame( void )
{
    m_pkBtnConfirm = NULL;
    LINK_CONTROL( "confirm", m_pkBtnConfirm );

}

KGCAccountNickNameFrame::~KGCAccountNickNameFrame( void )
{
}

void KGCAccountNickNameFrame::OnCreate( void )
{
    KGCIntroNickNameFrame::OnCreate();

    m_pkBtnConfirm->InitState( true , true , this );

    m_pkEditNickname->SetLimitText( GC_ACCOUNT_NICKNAME_SIZE );

}

void KGCAccountNickNameFrame::ActionPerformed( const KActionEvent& event )
{
    KGCIntroNickNameFrame::ActionPerformed( event );
    GCWND_MSG_MAP( m_pkBtnConfirm,                KD3DWnd::D3DWE_BUTTON_CLICK,    OnConfirm );

}

void KGCAccountNickNameFrame::OnConfirm()
{

    Result_ReportNickName = INT_MAX;

    KP2P::GetInstance()->Send_RegisterNickNameReq( GetNickNameInEditBox().c_str() );


    g_MyD3D->WaitForServerAck(Result_ReportNickName, INT_MAX, 10000, -6 );

    switch ( Result_ReportNickName )
    {
    case 0: // 0 : 성공
        {

            g_kGlobalValue.m_kUserInfo.strNickName = GetNickNameInEditBox().c_str();
            g_MyD3D->m_TempPlayer.m_kUserInfo.strNickName = GetNickNameInEditBox().c_str(); 

            // 메시지서버에 자신의 닉네임 정보를 알린다.
            if( KP2P::GetInstance()->m_kMsgServer != NULL )
            {
                if( KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() )
                {
                    KP2P::GetInstance()->m_kMsgServer->Send_ChangeNickName( g_kGlobalValue.m_kUserInfo.strNickName );			                    
                }
            }

            ToggleRender( false );
           

            return;
        }
    case -4: // -4 : 부적절한 닉네임(빈칸, 탭, \', 욕설, 크기가0, 제한크기초과..등)
        {
            //MessageBox( eMessageBoxType , L"", L"", GC_MBOX_USE_NORMAL,0,0, true);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_CHAR ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
            return ;
        }
    case -2: // -2 : 해당 유저는 이미 닉네임이 등록되어 있음.
    case -3: // -3 : 이미 존재하고 있는 닉네임이 있음.(다른 유저가 이미 등록했음)
        {
            //MessageBox( eMessageBoxType , L"", L"", GC_MBOX_USE_NORMAL,0,0, true);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ALREADY_EXIST ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
            return ;
        }
    case -5: // -5 : 부적절한 길이(크기가0, 제한크기초과)
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ),
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
            return ;
        }
    case -6: // 시간 초과
        {
            //MessageBox( eMessageBoxType , L"", L"", GC_MBOX_USE_NORMAL,0,0, true);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
            return ;
        }
    default: // 알 수 없는 오류
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_REGISTER_NICKNAME_ACK, Result_ReportNickName );	
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_NICK_NAME_ERROR ),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );

            return ;
        }
    }
}

void KGCAccountNickNameFrame::FrameMoveInEnabledState()
{
    KGCIntroNickNameFrame::FrameMoveInEnabledState();
    if ( IsPositive() )
        m_pkBtnConfirm->Lock(false);
    else
        m_pkBtnConfirm->Lock(true);

}
