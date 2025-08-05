#include "stdafx.h"
#include "GCNeonsignSelectBox.h"
//

//
#include "GCNeonsignItemDesc.h"
#include "GCNeonsignMessage.h"
#include "GCNeonsignItemList.h"
//
//
//
#include "KGCAnnounceMgr.h"
#include "GCChatManager.h"
#include "GCNeonsignPreview.h"

IMPLEMENT_CLASSNAME( KGCNeonsignSelectBox );
IMPLEMENT_WND_FACTORY( KGCNeonsignSelectBox );
IMPLEMENT_WND_FACTORY_NAME( KGCNeonsignSelectBox, "gc_neonsign_select_box" );

KGCNeonsignSelectBox::KGCNeonsignSelectBox(void)
: m_pkTitle(NULL)
, m_pkCloseBtn(NULL)
, m_pkPreviewBtn(NULL)
, m_pkItemDesc(NULL)
, m_pkMessage(NULL)
, m_pkItemList(NULL)
{
    LINK_CONTROL( "title",              m_pkTitle );
    LINK_CONTROL( "close_btn",          m_pkCloseBtn );
    LINK_CONTROL( "btn_preview",        m_pkPreviewBtn );
    LINK_CONTROL( "neonsign_item_desc", m_pkItemDesc );
    LINK_CONTROL( "announce_message",   m_pkMessage );
    LINK_CONTROL( "neonsign_item_list", m_pkItemList );
}

KGCNeonsignSelectBox::~KGCNeonsignSelectBox(void)
{

}

void KGCNeonsignSelectBox::OnCreate()
{
    m_pkTitle->InitState( true );
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
    m_pkPreviewBtn->InitState( true, true, this );
    SetTitleText( false );
    m_pkItemDesc->InitState( true );
    m_pkMessage->InitState( true );
    m_pkItemList->InitState( true );
    SetItem(-1);
}

void KGCNeonsignSelectBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkPreviewBtn, D3DWE_BUTTON_CLICK, OnPreviewBtn );
}

void KGCNeonsignSelectBox::OnClose()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCNeonsignSelectBox::OnDestroyComplete()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCNeonsignSelectBox::OnPreviewBtn()
{
    if( -1 == m_pkItemList->GetItemID() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_ERROR3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    if( true == m_pkMessage->SetBytes() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_ERROR1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    std::wstring strMessage;
    m_pkMessage->GetMessage( strMessage );
    GCUTIL_STR::Trim( strMessage );

    std::wstring strAfter = strMessage;
    g_pkChatManager->MessageFilter( strAfter );
    if( strAfter != strMessage )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_ERROR5 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    if( false == m_pkMessage->IsValidate() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    SetTitleText( true );

    KSignBoardData kData;
    SimpleAnnounce::InitAnnounceData( kData );
    kData.m_dwColor = m_pkMessage->GetMessageColor();
    kData.m_ItemID = m_pkItemList->GetItemID();
    kData.m_dwSenderUID = g_kGlobalValue.m_kUserInfo.dwUID;
    m_pkMessage->GetMessage( kData.m_strMsg );
    kData.m_strSenderNick = g_kGlobalValue.m_kUserInfo.strNickName;

    g_pkUIScene->m_pkNeonsignPreview->SetPreviewData( kData );
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NEONSIGN_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
}

void KGCNeonsignSelectBox::SetTitleText( bool bPreview )
{
    if( true == bPreview )
    {
        m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_NOW_PREVIEWING ) );
    }
    else
    {
        m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_CHOICE_NEONSIGN ) );
    }

    m_pkTitle->SetAlign( DT_CENTER );
}

void KGCNeonsignSelectBox::SetItem( int nItemID )
{
    m_pkItemDesc->SetItem( nItemID );
}

void KGCNeonsignSelectBox::SetSignBoardType( KSignBoardData::SIGN_BOARD_TYPE eType )
{
    m_pkItemList->SetSignBoardType( eType );
}
