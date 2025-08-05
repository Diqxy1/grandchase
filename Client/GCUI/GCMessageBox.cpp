#include "stdafx.h"
#include "GCMessageBox.h"
#include "gcui/KGCGPCapsuleDlg.h"
#include "GCAchieveManager.h"
//
//


IMPLEMENT_CLASSNAME( KGCMessageBox );
IMPLEMENT_WND_FACTORY( KGCMessageBox );
IMPLEMENT_WND_FACTORY_NAME( KGCMessageBox, "gc_message_box" );

KGCMessageBox::KGCMessageBox( void )
: m_pkCheck(NULL)
, m_pkCansel(NULL)
, m_pkJoinUser(NULL)
, m_pkUserInfo(NULL)
{
    m_iBtnType   = 0;
    m_bIsSizingBox = false;
    m_pkText[0] = NULL;
	m_pkText[1] = NULL;
	m_pkText[2] = NULL;
	m_pkText[3] = NULL;
    
    m_pkOK      = NULL;
    m_pkYes     = NULL;
    m_pkSlash   = NULL;
    m_pkNo      = NULL;
	m_pkBackDesc = NULL;

    LINK_CONTROL( "message_box_text1",      m_pkText[0] );
    LINK_CONTROL( "message_box_text2",      m_pkText[1] );
	LINK_CONTROL( "message_box_text3",      m_pkText[2] );
	LINK_CONTROL( "message_box_text4",      m_pkText[3] );
    LINK_CONTROL( "message_box_ok_button",  m_pkOK );
    LINK_CONTROL( "message_box_yes_button", m_pkYes );
    LINK_CONTROL( "message_box_slash",      m_pkSlash );
	LINK_CONTROL( "message_box_no_button",  m_pkNo );

	LINK_CONTROL( "message_box_check_button",  m_pkCheck );
	LINK_CONTROL( "message_box_cansel_button",  m_pkCansel );
	LINK_CONTROL( "message_box_join_button",  m_pkJoinUser );
	LINK_CONTROL( "message_box_info_button",  m_pkUserInfo );

	LINK_CONTROL( "std_frame_sizing",  m_pkBackDesc );

}

KGCMessageBox::~KGCMessageBox( void )
{
    // empty
}

void KGCMessageBox::OnCreate( void )
{

	switch ( m_iBtnType ) {
		case MBT_YES_NO :
			m_pkOK->InitState(false, true,this );
			m_pkYes->InitState(true, true,this );
			m_pkNo->InitState(true, true,this );
			m_pkCheck->InitState(false, true,this );
			m_pkCansel->InitState(false, true,this );
			m_pkJoinUser->InitState(false, true,this );
			m_pkUserInfo->InitState(false, true,this );
			break;

		case MBT_CHECK_CANSEL :
			m_pkOK->InitState(false, true,this );
			m_pkYes->InitState(false, true,this );
			m_pkNo->InitState(false, true,this );
			m_pkCheck->InitState(true, true,this );
			m_pkCansel->InitState(true, true,this );
			m_pkJoinUser->InitState(false, true,this );
			m_pkUserInfo->InitState(false, true,this );
			break;

		case MBT_JOIN_CANSEL :
			m_pkOK->InitState(false, true,this );
			m_pkYes->InitState(false, true,this );
			m_pkNo->InitState(false, true,this );
			m_pkCheck->InitState(false, true,this );
			m_pkCansel->InitState(true, true,this );
			m_pkJoinUser->InitState(true, true,this );
			m_pkUserInfo->InitState(false, true,this );
			break;

		case MBT_INFO_CANSEL :
			m_pkOK->InitState(false, true,this );
			m_pkYes->InitState(false, true,this );
			m_pkNo->InitState(false, true,this );
			m_pkCheck->InitState(false, true,this );
			m_pkCansel->InitState(true, true,this );
			m_pkJoinUser->InitState(false, true,this );
			m_pkUserInfo->InitState(true, true,this );
			break;

		case MBT_OK :
		default:
			m_pkOK->InitState(true, true,this );
			m_pkYes->InitState(false, true,this );
			m_pkNo->InitState(false, true,this );
			m_pkCheck->InitState(false, true,this );
			m_pkCansel->InitState(false, true,this );
			m_pkJoinUser->InitState(false, true,this );
			m_pkUserInfo->InitState(false, true,this );
			break;

	}

	if(m_pkBackDesc != NULL)
	{
		m_pkBackDesc->InitState(true,false);
	}
}

void KGCMessageBox::OnCreateComplete( void )
{
    SpeakToActionListener( KActionEvent( this, MBR_CREATED ) );
}

void KGCMessageBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, MBR_DESTROYED ) );
}

void KGCMessageBox::FrameMoveInEnabledState( void )
{
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING )
        Destroy();
    if ( m_iBtnType == MBT_OK )
    {
        if ( g_pkInput->IsDown( DIK_RETURN ) )
        {
            SpeakToActionListener( KActionEvent( this, MBR_OK ) );
			ReleaseCapture();
			g_pkInput->IgnoreDown(DIK_RETURN,30);
        }

		if ( g_pkInput->IsDown( DIK_ESCAPE ) )
		{
			SpeakToActionListener( KActionEvent( this, MBR_OK ) );
			ReleaseCapture();
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}
    }
	else if ( m_iBtnType == MBT_YES_NO )
    {	//단축키와 중복으로 먹는거 방지 하기 위해서 추가 작업 처리
		
        if ( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_Y ) )
        {	
            SpeakToActionListener( KActionEvent( this, MBR_YES ) );
			ReleaseCapture();
        }
        if ( g_pkInput->IsDown( DIK_ESCAPE ) || g_pkInput->IsDown( DIK_N ) )
        {
            SpeakToActionListener( KActionEvent( this, MBR_NO ) );
			ReleaseCapture();
        }

		if (g_pkInput->IsDown(DIK_RETURN))
		{
			g_pkInput->IgnoreDown(DIK_RETURN,30);
		}

		if (g_pkInput->IsDown(DIK_Y))
		{
			g_pkInput->IgnoreDown(DIK_Y,30);
		}

		if (g_pkInput->IsDown(DIK_ESCAPE))
		{
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}

		if (g_pkInput->IsDown(DIK_N))
		{
			g_pkInput->IgnoreDown(DIK_N,30);
		}
    }
	else if ( m_iBtnType == MBT_JOIN_CANSEL ) {
		if ( g_pkInput->IsDown( DIK_RETURN ) )
		{	
			SpeakToActionListener( KActionEvent( this, MBR_JOIN_USER ) );
			ReleaseCapture();
		}
		if ( g_pkInput->IsDown( DIK_ESCAPE ) )
		{
			SpeakToActionListener( KActionEvent( this, MBR_CANSEL ) );
			ReleaseCapture();
		}

		if (g_pkInput->IsDown(DIK_RETURN))
		{
			g_pkInput->IgnoreDown(DIK_RETURN,30);
		}

		if (g_pkInput->IsDown(DIK_ESCAPE))
		{
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}

	}

	else if ( m_iBtnType == MBT_CHECK_CANSEL ) {
		if ( g_pkInput->IsDown( DIK_RETURN ) )
		{	
			SpeakToActionListener( KActionEvent( this, MBR_CHACK ) );
			ReleaseCapture();
		}
		if ( g_pkInput->IsDown( DIK_ESCAPE ) )
		{
			SpeakToActionListener( KActionEvent( this, MBR_CANSEL ) );
			ReleaseCapture();
		}

		if (g_pkInput->IsDown(DIK_RETURN))
		{
			g_pkInput->IgnoreDown(DIK_RETURN,30);
		}

		if (g_pkInput->IsDown(DIK_ESCAPE))
		{
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}
	}
	else if ( m_iBtnType == MBT_INFO_CANSEL ) {
		if ( g_pkInput->IsDown( DIK_RETURN ) )
		{	
			SpeakToActionListener( KActionEvent( this, MBR_USER_INFO ) );
			ReleaseCapture();
		}
		if ( g_pkInput->IsDown( DIK_ESCAPE ) )
		{
			SpeakToActionListener( KActionEvent( this, MBR_CANSEL ) );
			ReleaseCapture();
		}

		if (g_pkInput->IsDown(DIK_RETURN))
		{
			g_pkInput->IgnoreDown(DIK_RETURN,30);
		}

		if (g_pkInput->IsDown(DIK_ESCAPE))
		{
			g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}
	}

}

void KGCMessageBox::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
        case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
        case KD3DWnd::D3DWE_BUTTON_CLICK:
        {
            if ( event.m_pWnd == m_pkOK )
            {
                SpeakToActionListener( KActionEvent( this, MBR_OK ) );
            }
            else if ( event.m_pWnd == m_pkYes )
            {
                SpeakToActionListener( KActionEvent( this, MBR_YES ) );
            }
            else if ( event.m_pWnd == m_pkNo )
            {
                SpeakToActionListener( KActionEvent( this, MBR_NO ) );
            }
			else if ( event.m_pWnd == m_pkCheck )
			{
				SpeakToActionListener( KActionEvent( this, MBR_CHACK ) );
			}
			else if ( event.m_pWnd == m_pkCansel )
			{
				SpeakToActionListener( KActionEvent( this, MBR_CANSEL ) );
			}
			else if ( event.m_pWnd == m_pkJoinUser )
			{
				SpeakToActionListener( KActionEvent( this, MBR_JOIN_USER ) );
			}
			else if ( event.m_pWnd == m_pkUserInfo )
			{
				SpeakToActionListener( KActionEvent( this, MBR_USER_INFO ) );
			}
            break;
        }
    }
}

void KGCMessageBox::SetType( int iType )
{
    m_iBtnType = iType;

	switch ( m_iBtnType ) {
		case MBT_YES_NO :
			m_pkOK->ToggleRender(false);
			m_pkYes->ToggleRender(true);
			m_pkNo->ToggleRender(true);
			m_pkCheck->ToggleRender(false);
			m_pkCansel->ToggleRender(false);
			m_pkJoinUser->ToggleRender(false);
			m_pkUserInfo->ToggleRender(false);
			break;

		case MBT_CHECK_CANSEL :
			m_pkOK->ToggleRender(false);
			m_pkYes->ToggleRender(false);
			m_pkNo->ToggleRender(false);
			m_pkCheck->ToggleRender(true);
			m_pkCansel->ToggleRender(true);
			m_pkJoinUser->ToggleRender(false);
			m_pkUserInfo->ToggleRender(false);
			break;

		case MBT_JOIN_CANSEL :
			m_pkOK->ToggleRender(false);
			m_pkYes->ToggleRender(false);
			m_pkNo->ToggleRender(false);
			m_pkCheck->ToggleRender(false);
			m_pkCansel->ToggleRender(true);
			m_pkJoinUser->ToggleRender(true);
			m_pkUserInfo->ToggleRender(false);
			break;

		case MBT_INFO_CANSEL :
			m_pkOK->ToggleRender(false);
			m_pkYes->ToggleRender(false);
			m_pkNo->ToggleRender(false);
			m_pkCheck->ToggleRender(false);
			m_pkCansel->ToggleRender(true);
			m_pkJoinUser->ToggleRender(false);
			m_pkUserInfo->ToggleRender(true);
			break;

		case MBT_OK :
		default:
			m_pkOK->ToggleRender(true);
			m_pkYes->ToggleRender(false);
			m_pkNo->ToggleRender(false);
			m_pkCheck->ToggleRender(false);
			m_pkCansel->ToggleRender(false);
			m_pkJoinUser->ToggleRender(false);
			m_pkUserInfo->ToggleRender(false);
			break;

	}
}

void KGCMessageBox::SetUse( DWORD dwUse )
{
    m_dwUse = dwUse;
}

void KGCMessageBox::SetText( const std::wstring& strText1, const std::wstring& strText2 )
{
	for(int t = 0; t < 4; t++)
	{
		if( m_pkText[t] )
		{
			m_pkText[t]->SetText();
		}
	}
	int iIndex = 0;	
	std::wstring *pString = NULL;
	std::wstring strTemp;

	for(int i = 0; i < 2; i++)
	{
		if( i == 0 )
			pString = (std::wstring*)&strText1;
		else
			pString = (std::wstring*)&strText2;

		std::wstring::size_type idx = pString->find(L'_');

		while(idx != std::wstring::npos)		
		{
			if( iIndex >= 4 )
				return;
			m_pkText[iIndex++]->SetText( pString->substr(0, idx) );
			strTemp = pString->substr(idx+1, pString->size()-1);
			pString = &strTemp;
			idx = pString->find(L'_');
		}
		if( iIndex >= 4 )
			return;        
        
        if ( strText2 == L"" )
        {
            m_pkText[iIndex]->SetTextAutoMultiline( *pString );
            break;
        }
        else if( (iIndex != 0 ) && ( strText2 != L"" ) && !m_bIsSizingBox )    // 일단 임시로 이렇게 해봤습니다.....
        {
            m_pkText[iIndex]->SetTextAutoMultiline( *pString );
            break;
        }
        else
            m_pkText[iIndex++]->SetText( *pString );		
	}
}

void KGCMessageBox::SetText( int iIndex, const std::wstring& strText )
{
	m_pkText[iIndex]->SetText( strText );    
}

const std::wstring& KGCMessageBox::GetText( int iIndex ) const
{
    return m_pkText[iIndex]->GetText();
}

DWORD KGCMessageBox::GetUse( void ) const
{
    return m_dwUse;
}

bool KGCMessageBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    switch( kInfo_.m_nMsgBoxType )
    {
    case KGCUIScene::GC_MBOX_OK:
	case KGCUIScene::GC_MBOX_OK2:
	case KGCUIScene::GC_MBOX_SIZING_OK:
    case KGCUIScene::GC_MBOX_EXIT_ROOM:
        this->SetType( MBT_OK );
        if( kInfo_.m_nMsgBoxType == KGCUIScene::GC_MBOX_SIZING_OK )
            m_bIsSizingBox = true;
        break;
    case KGCUIScene::GC_MBOX_YES_NO:
    case KGCUIScene::GC_MBOX_EXIT_GAME:
    case KGCUIScene::GC_PET_FEEDING:
    case KGCUIScene::GC_MBOX_EXTEND_INVENTORY:
    case KGCUIScene::GC_MBOX_EXIT_SQUARE:
    case KGCUIScene::GC_MBOX_TOURNAMENT_BOX:
    case KGCUIScene::GC_MBOX_LEVEL_RESET_ITEM:
    case KGCUIScene::GC_MBOX_EXTEND_LOOK_INVENTORY:
        this->SetType( MBT_YES_NO );
        break;
    case KGCUIScene::GC_MBOX_GP_CAPSULE_MESSAGE_OK:
        this->SetType( MBT_YES_NO );
        g_pkUIScene->m_pkGPCapsuleDlg->LockAll(false);
        g_pkUIScene->m_pkGPCapsuleDlg->m_pkGPCapsuleItemListDlg->LockAll(false);
        break;
	case KGCUIScene::GC_MBOX_OK_CANSEL_BOX:
		this->SetType( MBT_CHECK_CANSEL );
		break;
	case KGCUIScene::GC_MBOX_JOIN_CANSEL_BOX:
		this->SetType( MBT_JOIN_CANSEL );
		break;
	case KGCUIScene::GC_MBOX_INFO_CANSEL_BOX:
		this->SetType( MBT_INFO_CANSEL );
		break;


    }
    SetText( kInfo_.m_strText1, kInfo_.m_strText2 );
    SetUse( kInfo_.m_nMsgBoxUse);
    SetlParam(  kInfo_.m_lParam );
    SetlParam2( kInfo_.m_lParam2 );
    SetWindowZOrder(D3DWZ_TOPMOST);


	if(m_bIsSizingBox)
	{	
		int y =0;
		int yy =0;

		//KLuaManager luaMgr;
		//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
		//LUA_GET_VALUE_DEF( "y", y, 0 );
		//LUA_GET_VALUE_DEF( "yy", yy, 0 );

		m_pkBackDesc->SetSize(static_cast<int>(kInfo_.m_lParam) ,static_cast<int>(kInfo_.m_lParam2) * 10 +150);
	
		D3DXVECTOR2 vecOKbtnPos = m_pkOK->GetFixedWindowPos();
		vecOKbtnPos.y += static_cast<float>(static_cast<int>(kInfo_.m_lParam2) * 10);
		vecOKbtnPos.x = static_cast<float>(m_pkBackDesc->GetWidth())/2.0f+20.0f;
		m_pkText[0]->SetWidth(static_cast<int>(kInfo_.m_lParam)-10);
		m_pkText[1]->SetWidth(static_cast<int>(kInfo_.m_lParam)-10);
		m_pkText[2]->SetWidth(static_cast<int>(kInfo_.m_lParam)-10);
		m_pkText[3]->SetWidth(static_cast<int>(kInfo_.m_lParam)-10);

	}

    if( kInfo_.m_nMsgBoxType == KGCUIScene::GC_MBOX_EXIT_GAME ) {
        SiKGCAchieveManager()->OccurAction( SAchieve::OPEN_EXIT_UI, GC_CHAR_INVALID );
    }
    return true;
}