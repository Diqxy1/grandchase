#include "stdafx.h"
#include "KGCSubscriptionDlg.h"
#include "KGCSubscriptionBox.h"
#include "../KDSound.h"
#include "../MyD3D.h"
#include "../GCGameResult.h"
#include "GCItemInfoBox.h"
#include "Packet.h"
#include "P2P.H"


IMPLEMENT_CLASSNAME( KGCSubscriptionDlg );
IMPLEMENT_WND_FACTORY( KGCSubscriptionDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSubscriptionDlg, "gc_subscription_dlg" );

KGCSubscriptionDlg::KGCSubscriptionDlg(void)
: m_pkSubscriptionBtn( NULL )
, m_pkCloseBnt( NULL )
, m_pkItemView( NULL )
, m_pkItemInfoBox(NULL)
, m_pkBlueBagTitle(NULL)
, m_pkBlueBagDesc(NULL)
, m_pkRedBagTitle(NULL)
, m_pkRedBagDesc(NULL)
, m_pkGoldBagTitle(NULL)
, m_pkGoldBagDesc(NULL)
{

	InitSubscriptionInfo();

	LINK_CONTROL( "subscription_run_btn", m_pkSubscriptionBtn );
	LINK_CONTROL( "subscription_close_button", m_pkCloseBnt );
	
	LINK_CONTROL( "item_image_view", m_pkItemView );

	LINK_CONTROL( "item_info_box"       , m_pkItemInfoBox );

	LINK_CONTROL( "subscription_static_desc1"       , m_pkStaticDesc1 );
	LINK_CONTROL( "subscription_static_desc2"       , m_pkStaticDesc2 );

	LINK_CONTROL( "blue_bag_title"       , m_pkBlueBagTitle );
	LINK_CONTROL( "blue_bag_desc"       , m_pkBlueBagDesc );

	LINK_CONTROL( "red_bag_title"       , m_pkRedBagTitle );
	LINK_CONTROL( "red_bag_desc"       , m_pkRedBagDesc );

	LINK_CONTROL( "gold_bag_title"       , m_pkGoldBagTitle );
	LINK_CONTROL( "gold_bag_desc"       , m_pkGoldBagDesc );
}

KGCSubscriptionDlg::~KGCSubscriptionDlg(void)
{

}

void KGCSubscriptionDlg::InitSubscriptionInfo()
{
	m_iCurEventUID = 0;
	m_iAcquiredAmount = 0;
	m_iExchangedAmount = 0;
	m_iHoldingAmount = 0;
	m_iCurrentAmount = 0;
	m_iNeedAmount = 0;
	m_dwTicketID = 0;
	m_iGiftItemID = 0;
}

void KGCSubscriptionDlg::OnCreate()
{
	m_pkSubscriptionBtn->InitState(true, true, this );
	m_pkCloseBnt->InitState( true, true, this );
    m_pkCloseBnt->SetHotKey(DIK_ESCAPE);
	m_pkItemView->InitState(true);	
	m_pkItemInfoBox->InitState(false, true, this);
#if !defined( NATION_IDN )
	m_pkStaticDesc1->InitState(true, true, this );
	m_pkStaticDesc1->SetText(L"");
	m_pkStaticDesc2->InitState(true, true, this );
	m_pkStaticDesc2->SetText(L"");
#endif
	Update();
}		

void KGCSubscriptionDlg::SaveEventID()
{
	std::map<int, std::pair<KSubscription,DWORD>>::iterator mit = g_kGlobalValue.m_mapSubscriptionEvent.begin();
	for(  ; mit != g_kGlobalValue.m_mapSubscriptionEvent.end(); ++mit )
	{
		
		m_mapNPCEVENTNUM.insert(std::make_pair( mit->second.first.m_dwNPCID, mit->first ));
	}

}

void KGCSubscriptionDlg::FrameMoveInEnabledState( void )
{

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkItemView->CheckPosInWindowBound(vMousePos) )
	{
        D3DXVECTOR2 vecPos( m_pkItemView->GetCurrentWindowPos() );
        vecPos.x = ( vecPos.x + m_pkItemView->GetWidth() ) *m_fWindowScaleX;
        vecPos.y *= m_fWindowScaleY;

        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
	}
	else
	{
		//m_pkItemInfoBox->ToggleRender( false );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}

void KGCSubscriptionDlg::ActionPerformed(const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCloseBnt,		KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkSubscriptionBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickExchangeBtn );
	SpeakToActionListener( event );
}


void KGCSubscriptionDlg::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCSubscriptionDlg::OnClickExchangeBtn()
{
	g_KDSound.Play( "31" );

	if( m_iCurrentAmount == 0 )
		return ;

	g_pkUIScene->m_pkSubscriptionBox->SetSubscriptionInfo(m_iCurEventUID, m_iHoldingAmount, m_dwTicketID, m_iNeedAmount, m_iCurrentAmount );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUBSCRIPTION_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCSubscriptionDlg::Update()
{	
	// 맵에 정보가 비어 있으면 값은 초기화시키고 아무일도 않는다.
	if( g_kGlobalValue.m_mapSubscriptionEvent.size() == 0 )
	{
		InitSubscriptionInfo();
		return;
	}

	//정보가 맵에 담겨오지만 현재로서는 하나만 사용하기 때문에 하나에 담아서 쓴다. 현재 기획상 ui구조가 여러이벤트를 
	//보여주는 구조도 아니다...
	//KEVENT_SUBSCRIPTION_INFO_NOT::iterator mit = g_kGlobalValue.m_mapSubscriptionEvent.begin();
	int	iNPCType = 0;

	
	std::map<DWORD,int>::iterator itNPCID = m_mapNPCEVENTNUM.find(m_eBagType);

	KEVENT_SUBSCRIPTION_INFO_NOT::iterator mit = g_kGlobalValue.m_mapSubscriptionEvent.find(itNPCID->second);

	if( mit != g_kGlobalValue.m_mapSubscriptionEvent.end() ){
		m_iCurEventUID = mit->first;
		m_iAcquiredAmount = g_pItemMgr->GetInventoryItemDuration( mit->second.first.m_kRewardItem.m_ItemID / 10 );
		m_iNeedAmount = mit->second.first.m_nNeedTicketNum;
		m_iExchangedAmount = m_iAcquiredAmount * m_iNeedAmount;
		m_iHoldingAmount = g_pItemMgr->GetInventoryItemDuration( mit->second.first.m_dwTicketItemID / 10);
		m_dwTicketID = mit->second.first.m_dwTicketItemID / 10;
		m_iGiftItemID = mit->second.first.m_kRewardItem.m_ItemID / 10;
	}
	else{
		InitSubscriptionInfo();
		return;
	}

	m_pkItemView->SetItemInfo( m_iGiftItemID );

	if( m_iCurrentAmount > 0 )
		m_pkSubscriptionBtn->Lock(false);
	else
		m_pkSubscriptionBtn->Lock(true);

	GCItem* kItem = g_pItemMgr->GetItemData(m_iGiftItemID);	
	//m_pkItemInfoBox->SetItem(kItem);
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
	GCItem* kTicketItem = g_pItemMgr->GetItemData(m_dwTicketID);
#if defined(NATION_USA) || defined(NATION_PHILIPPINE) || defined( NATION_EU )
	m_pkStaticDesc1->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SUBSCRIPTION_DESC1, "i", m_iNeedAmount/*iGiftFactor*/) );
#elif defined( NATION_KOREA ) 
    m_pkStaticDesc1->SetMultiLine( true );
    m_pkStaticDesc1->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SUBSCRIPTION_DESC1 ) );
#else
	m_pkStaticDesc1->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SUBSCRIPTION_DESC1, "li", kTicketItem->strItemName, m_iNeedAmount/*iGiftFactor*/) );
#endif

#if !defined( NATION_KOREA )
	m_pkStaticDesc2->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SUBSCRIPTION_DESC2, "l", kItem->strItemName ) );
#endif
}

void KGCSubscriptionDlg::OnDestroyComplete( void )
{
}

void KGCSubscriptionDlg::OnCreateComplete( void )
{		
}

void KGCSubscriptionDlg::PostChildDraw()
{
	D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();

	D3DXVECTOR2 vPos1( 170.0f*m_fWindowScaleX, 45.0f*m_fWindowScaleY );
	D3DXVECTOR2 vPos2( 178.0f*m_fWindowScaleX, 45.0f*m_fWindowScaleY );
	D3DXVECTOR2 vPos3( 178.0f*m_fWindowScaleX, 76.0f*m_fWindowScaleY);
#if defined( _DEBUG )//테스트용
 	KLuaManager luaMgr;
 	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
 
 	float x1 = 0.0f, y1 = 1.0f;
 	LUA_GET_VALUE_DEF( "x1", x1, 1.0f );
 	LUA_GET_VALUE_DEF( "y1", y1, 1.0f );
 	float x2 = 0.0f, y2 = 1.0f;
 	LUA_GET_VALUE_DEF( "x2", x2, 1.0f );
 	LUA_GET_VALUE_DEF( "y2", y2, 1.0f );
	float x3 = 0.0f, y3 = 1.0f;
	LUA_GET_VALUE_DEF( "x3", x3, 1.0f );
	LUA_GET_VALUE_DEF( "y3", y3, 1.0f );
	//float x4 = 0.0f, y4 = 1.0f;
	//LUA_GET_VALUE_DEF( "x4", x4, 1.0f );
	//LUA_GET_VALUE_DEF( "y4", y4, 1.0f );
	vPos1.x = x1;
	vPos1.y = y1;
	vPos2.x = x2*m_fWindowScaleX;
	vPos2.y = y2*m_fWindowScaleY;
	vPos3.x = x3*m_fWindowScaleX;
	vPos3.y = y3*m_fWindowScaleY;

#endif

    g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos2, (DWORD)m_iHoldingAmount );
    g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos3, (DWORD)m_iNeedAmount );

    //  아래는 예시용 코드인듯. 일단 남겨둔다.
	//g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos1, (DWORD)m_iCurrentAmount );
	//g_pkUIMgr->RenderNumber("blue_gradation_num", vPos + vPos3, (DWORD)m_iExchangedAmount );
}


void KGCSubscriptionDlg::SetBagType( int eType )
{
	m_pkBlueBagTitle->ToggleRender( false );
	m_pkBlueBagDesc->ToggleRender( false );
	m_pkRedBagTitle->ToggleRender( false );
	m_pkRedBagDesc->ToggleRender( false );
	m_pkGoldBagTitle->ToggleRender( false );
	m_pkGoldBagDesc->ToggleRender( false );

	m_eBagType = (eBagType)(eType);
	switch( eType )
	{
	case BLUE_BAG:
		m_pkBlueBagTitle->ToggleRender( true );
		m_pkBlueBagDesc->ToggleRender( true );
		break;
	case RED_BAG:
		m_pkRedBagTitle->ToggleRender( true );
		m_pkRedBagDesc->ToggleRender( true );
		break;
	case GOLD_BAG:
		m_pkGoldBagTitle->ToggleRender( true );
		m_pkGoldBagDesc->ToggleRender( true );
		break;
	default:
		break;
	}
}