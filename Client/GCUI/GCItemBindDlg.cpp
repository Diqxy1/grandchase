#include "stdafx.h"
#include "GCItemBindDlg.h"

IMPLEMENT_CLASSNAME( KGCItemBindDlg );
IMPLEMENT_WND_FACTORY( KGCItemBindDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCItemBindDlg, "gc_item_bind_dlg" );

KGCItemBindDlg::KGCItemBindDlg( void )
: m_pkItemImg( NULL )
, m_pkStaticItemName( NULL )
, m_pkStaticCharName( NULL )
, m_pkBtnClose( NULL )
, m_pkBtnOK( NULL )
, m_pkBtnCancle( NULL )
, m_pkConfirmPopup( NULL )
, m_itemUID( 0 )
, m_cCharType( -1 )
, m_pInvenItem( NULL )
, m_pItemData( NULL )
{
    LINK_CONTROL( "ItemImg",        m_pkItemImg );
    LINK_CONTROL( "StaticItemName", m_pkStaticItemName );
    LINK_CONTROL( "StaticCharName", m_pkStaticCharName );
    LINK_CONTROL( "BtnClose",       m_pkBtnClose );
    LINK_CONTROL( "BtnOK",          m_pkBtnOK );
    LINK_CONTROL( "BtnCancle",      m_pkBtnCancle );
    LINK_CONTROL( "ConfirmPopup",   m_pkConfirmPopup );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkCharIcon[i] = NULL;

        char temp[128];
        sprintf_s( temp, 127, "CharIcon%d", i );
        LINK_CONTROL( temp, m_apkCharIcon[i] );
    }
}

KGCItemBindDlg::~KGCItemBindDlg( void )
{
}

void KGCItemBindDlg::OnCreate( void )
{
    m_pkItemImg->InitState( true, false, this );
    m_pkStaticItemName->InitState( true, false, this );
    m_pkStaticCharName->InitState( true, false, this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkConfirmPopup->InitState( false, false, this );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkCharIcon[i]->InitState( false, false, this );
    }

    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemBindDlg::OnClickClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnOK->DisconnectAllEventProcedure();
    m_pkBtnOK->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemBindDlg::OnClickOK );
    m_pkBtnOK->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER ,this, &KGCItemBindDlg::EnterMouseCursorOKBtn );
    m_pkBtnOK->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_LEAVE ,this, &KGCItemBindDlg::LeaveMouseCursorOKBtn );

    m_pkBtnCancle->DisconnectAllEventProcedure();
    m_pkBtnCancle->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemBindDlg::OnClickCancle );
    m_pkBtnCancle->SetHotKey( DIK_ESCAPE );

    m_pkStaticItemName->SetAlign( DT_CENTER );
    m_pkStaticCharName->SetAlign( DT_CENTER );
}

bool KGCItemBindDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    // m_lParam  : GCITEMUID
    // m_lParam2 : 캐릭터 타입 
    if( kInfo_.m_lParam2 < GC_CHAR_ELESIS || kInfo_.m_lParam2 > GC_CHAR_NUM - 1 ) { 
        return false;
    }

    m_itemUID = static_cast<GCITEMUID>( kInfo_.m_lParam );
    m_cCharType = static_cast<char>( kInfo_.m_lParam2 );

    m_pInvenItem = NULL;
    m_pItemData = NULL;
    if( false == g_pItemMgr->FindInventory( m_itemUID, &m_pInvenItem, &m_pItemData ) ) { 
        return false;
    }
    
    InitDlgInfo();
    return true;
}

void KGCItemBindDlg::ActionPerformed( const KActionEvent& event ) {}

void KGCItemBindDlg::OnClickClose( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );    
}

void KGCItemBindDlg::OnClickOK( void )
{
    if( m_pItemData == NULL || m_pInvenItem == NULL ) { 
        SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );    
    }

    // 펫 일 때 처리 
    // 추후 다른 귀속 아이템이 생기면 뒤로 쭉 붙이면 됩니다. 
    if( m_pItemData->eItemKind == GIK_PET ) { 
        BindPetItem();
    }

    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );    
}

void KGCItemBindDlg::OnClickCancle( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );    
}

void KGCItemBindDlg::SetCharacterIcon( void )
{
    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        if( static_cast<int>( m_cCharType ) == i ) { 
            m_apkCharIcon[i]->ToggleRender( true );
        }
        else {
            m_apkCharIcon[i]->ToggleRender( false );
        }
    }
}

void KGCItemBindDlg::SetItemImg( void )
{
    m_pkItemImg->SetItemInfo( m_pItemData->dwGoodsID );
    m_pkItemImg->ShowItem( true );
}

void KGCItemBindDlg::SetStatics( void )
{
    m_pkStaticItemName->SetText( m_pItemData->strItemName );
    m_pkStaticCharName->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BIND_ITEM_TO, "l", g_kGlobalValue.GetCharName( static_cast<int>( m_cCharType ) ) ) );
}

void KGCItemBindDlg::InitDlgInfo( void )
{
    LeaveMouseCursorOKBtn();
    SetCharacterIcon();
    SetItemImg();
    SetStatics();
}

void KGCItemBindDlg::BindPetItem( void )
{
    Result_CreatePet = INT_MAX;
    KP2P::GetInstance()->Send_CreatePetReq( m_cCharType, m_pItemData->dwGoodsID, m_itemUID, L"" );
    g_MyD3D->WaitForServerAck( Result_CreatePet, INT_MAX, 15000, -6 );

    switch( Result_CreatePet ) { 
        case 0: // 성공
            if( m_pItemData->dwGoodsID != 30798) {  // 키티펫은 프리뷰 안띄움 
                g_pkUIScene->MessageBox( KGCUIScene::GC_PET_PREVIEW );
                g_pkUIScene->m_pkPetPreview->SetMyInfoPreview();
                g_pkUIScene->m_pkPetPreview->SetDescState( 2 );
                g_pkUIScene->m_pkPetPreview->SetNowPet( g_kGlobalValue.GetPetIDFromItemID( m_pItemData->dwGoodsID ), 0 ); // 처음생성되는 놈이니 진화레벨은 0 이겠지..
                g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState( PET_MENT_FIRST_MEETING );
            }
            // 펫 생성에 성공했다면 원래 끼고 있던 펫은 벗기고 생성된 펫을 끼운다
            g_MyD3D->m_TempPlayer.UnEquipPet( m_cCharType );
            g_MyD3D->m_TempPlayer.SetEquipPetInfo( m_cCharType, m_itemUID );
            g_MyD3D->m_TempPlayer.AutoEquipPetItem();
            g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            g_pkUIScene->m_pkMyInfoScene->OnBindPet( m_itemUID );
            break;

        case 1: // 펫 존재하지 않음
        case 2: // 펫 아이템을 가지고 있지 않음 
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ), g_pkStrLoader->GetString( STR_ID_NO_EXIST_PET ) );
            break;

        case 3: // 신규 생성 하려는 펫이 이미 존재함 
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ), g_pkStrLoader->GetString( STR_ID_ALREADY_EXIST_PET ) );
            break;

        default:
            {                   
                std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_CREATE_PET_ACK, Result_CreatePet );	
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strTemp );
            }
            break;
    }
}

void KGCItemBindDlg::EnterMouseCursorOKBtn( void )
{
    m_pkConfirmPopup->ToggleRender( true );
}

void KGCItemBindDlg::LeaveMouseCursorOKBtn( void )
{
    m_pkConfirmPopup->ToggleRender( false );
}