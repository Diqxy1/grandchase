#include "stdafx.h"
#include "GCHotTimeSearchDlg.h"
#include "GCJustInTime.h"

IMPLEMENT_CLASSNAME( KGCHotTimeSearchDlg );
IMPLEMENT_WND_FACTORY( KGCHotTimeSearchDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCHotTimeSearchDlg, "gc_hottimesearchdlg" );

KGCHotTimeSearchDlg::KGCHotTimeSearchDlg( void )
{
    m_tEventTIme = 0;
    m_itemID = 0;
    m_pkStaticDesc = NULL;
    m_pkStaticTimeTitle = NULL;
    m_pkStaticTime = NULL;
    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;
    m_pkEquipmentItemWnd = NULL;

    LINK_CONTROL( "static_hottime_desc", m_pkStaticDesc );
    LINK_CONTROL( "static_time_title", m_pkStaticTimeTitle );
    LINK_CONTROL( "static_time", m_pkStaticTime );

    LINK_CONTROL( "btn_ok", m_pkBtnOK );
    LINK_CONTROL( "btn_cancle", m_pkBtnCancle );

    LINK_CONTROL( "item_img", m_pkEquipmentItemWnd );
}

KGCHotTimeSearchDlg::~KGCHotTimeSearchDlg( void )
{
}

void KGCHotTimeSearchDlg::OnCreate( void )
{
    m_pkStaticDesc->SetFontColor(0xffE9EEF3);
    m_pkStaticDesc->SetAlign(DT_LEFT);
    m_pkStaticDesc->InitState(true, false, this);  
    m_pkStaticDesc->SetMultiLine( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_HOT_TIME_SEARCH_DESC ) );

    m_pkStaticTimeTitle->SetFontColor(0xffFEC600);
    m_pkStaticTimeTitle->SetFontOutline(true,0xff14212f);
    m_pkStaticTimeTitle->SetFontSpace( 1.0f );
    m_pkStaticTimeTitle->SetAlign(DT_CENTER);
    m_pkStaticTimeTitle->SetText( g_pkStrLoader->GetString( STR_ID_HOT_TIME_TITLE ));

    m_pkStaticTime->SetFontColor(0xffE9EEF3);
    m_pkStaticTime->SetFontOutline(true,0xff14212f);
    m_pkStaticTime->SetFontSpace( 1.0f );
    m_pkStaticTime->SetAlign(DT_CENTER);
    m_pkStaticTime->SetText( L"" );

    m_pkEquipmentItemWnd->InitState( true );

    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkBtnCancle->SetHotKey( DIK_ESCAPE );
}

void KGCHotTimeSearchDlg::FrameMoveInEnabledState( void )
{
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    bool bCheck = false;
    if( m_pkEquipmentItemWnd->CheckPosInWindowBound( vMousePos ) )
    {
        // 아이템 정보 표시창 위치 보정.
        D3DXVECTOR2 vecPos = m_pkEquipmentItemWnd->GetCurrentWindowPos();
        RenderItemInfoBox( vecPos, m_itemID );
        bCheck = true;
    }

    if( !bCheck )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    } 
}

void KGCHotTimeSearchDlg::OnCreateComplete( void )
{
    UpdateInfo();
}

void KGCHotTimeSearchDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkBtnCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCHotTimeSearchDlg::OnOK()
{
    KEVENT_JUST_IN_TIME_REWARD_REQ  kPacket;

    kPacket = m_tEventTIme;

    KP2P::GetInstance()->Send_EventJustInTimeRewardReq( kPacket );

    g_kJustInTime.SetJustInTimeState( JIT_WAIT_REWARD );
    g_kJustInTime.CreateJustInTimeToolTip();

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_JUST_IN_TIME_MESSAGE1 ) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, false, false, false);

	//	검색 기능 제거
//    g_kGlobalValue.SearchProcess( GC_GLOBAL_DEFINE::strSearchURL );

    OnClose();
}

void KGCHotTimeSearchDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCHotTimeSearchDlg::UpdateInfo()
{
    m_pkEquipmentItemWnd->SetItemInfo(m_itemID);
    m_pkEquipmentItemWnd->ShowItem(true);

    CTime kTime = m_tEventTIme + g_kGlobalValue.m_tmServerSyncTime;

    std::wstring strTime = boost::str( boost::wformat( L"%02d:%02d" )%kTime.GetHour()%kTime.GetMinute() );

    m_pkStaticTime->SetText( strTime );
}

bool KGCHotTimeSearchDlg::CheckCondition(  IN const KDialogInfo& kInfo_ )
{
    m_itemID = static_cast<GCITEMID>( kInfo_.m_lParam );
    m_tEventTIme = static_cast<GCITEMID>( kInfo_.m_lParam2);
    return true;
}

void KGCHotTimeSearchDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID )
{
    vPos_.x *= m_fWindowScaleX;
    vPos_.y *= m_fWindowScaleY;
    vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

    GCItem* pItem = g_pItemMgr->GetItemData(_itemID);

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}