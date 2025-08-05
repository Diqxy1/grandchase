#include "stdafx.h"
#include ".\gcrankingreward.h"
//

#include "MyD3D.h"


IMPLEMENT_CLASSNAME( KGCRankingReward );
IMPLEMENT_WND_FACTORY( KGCRankingReward );
IMPLEMENT_WND_FACTORY_NAME( KGCRankingReward, "gc_ranking_reward" );

KGCRankingReward::KGCRankingReward(void)
: m_iCharType( 0 )
, m_iMonthlyRank( 1 )
, m_bUseForGuild( false )
, m_pkGuildWholeItemName	( 0 )
, m_pkGuildWeeklyItemName	( 0 )
, m_pkGuildMonthlyItemName	( 0 )
, m_pkGuildWholeItemDesc	( 0 )
, m_pkGuildWeeklyItemDesc	( 0 )
, m_pkGuildMonthlyItemDesc	( 0 )
, m_iGuildTotalRank			( GUILD_WHOLE_1 )
, m_iGuildMonthlyRank		( GUILD_MONTHLY_1 )
, m_iGuildWeeklyRank		( GUILD_WEEKLY_1 )
{
	m_pkDesc = NULL;
	LINK_CONTROL( "static_desc",	m_pkDesc );

	m_pkWeeklyItemName = NULL;
	m_pkWeeklyItemDesc = NULL;
	m_pkWeeklyItemBox = NULL;
	LINK_CONTROL( "weekly_item_name", m_pkWeeklyItemName );
	LINK_CONTROL( "weekly_item_desc", m_pkWeeklyItemDesc );
	LINK_CONTROL( "weekly_item_box", m_pkWeeklyItemBox );

	for( int i = 0 ; i < 2 ; ++i )
	{
		m_pkMonthlyItemName[i] = NULL;
		m_pkMonthlyItemDesc[i] = NULL;
		m_pkMonthlyItemBox[i] = NULL;

		std::stringstream stmname;
		stmname<<"monthly_item_name"<<i;
		LINK_CONTROL( stmname.str().c_str(), m_pkMonthlyItemName[i] );
		
		std::stringstream stmdesc;
		stmdesc<<"monthly_item_desc"<<i;
		LINK_CONTROL( stmdesc.str().c_str(), m_pkMonthlyItemDesc[i] );

		std::stringstream stmbox;
		stmbox<<"monthly_item_box"<<i;
		LINK_CONTROL( stmbox.str().c_str(), m_pkMonthlyItemBox[i] );
	}

    m_pkBtnSelectCharacter = NULL;
    LINK_CONTROL( "btn_character_select",   m_pkBtnSelectCharacter );

	for( int i = 0 ; i < 3 ; ++i )
	{
		m_apkRank[i] = NULL;
		std::stringstream str;
		str<<"btn_rank"<<i;
		LINK_CONTROL( str.str().c_str(), m_apkRank[i] );
	}

	m_pkClosebtn = NULL;
	LINK_CONTROL( "btn_close",		m_pkClosebtn );

	//------------------------------------------------------------------------
	//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091117 : 이동석
	LINK_CONTROL( "guild_whole_rank_frame",		m_apkGuildUIWnd[ GUILD_WHOLE_RANK_FRAME ] );
	LINK_CONTROL( "guild_whole_item_name",		m_apkGuildUIWnd[ GUILD_WHOLE_ITEM_NAME ] );
	LINK_CONTROL( "guild_whole_item_desc",		m_apkGuildUIWnd[ GUILD_WHOLE_ITEM_DESC ] );
	LINK_CONTROL( "guild_whole_item_box",		m_apkGuildUIWnd[ GUILD_WHOLE_ITEM_BOX ] );
	LINK_CONTROL( "guild_whole_ranker_title",	m_apkGuildUIWnd[ GUILD_WHOLE_RANKER_TITLE ] );
	LINK_CONTROL( "guild_whole_desc_frame1",	m_apkGuildUIWnd[ GUILD_WHOLE_DESC_FRAME1 ] );
	LINK_CONTROL( "guild_whole_desc_frame2",	m_apkGuildUIWnd[ GUILD_WHOLE_DESC_FRAME2 ] );
	LINK_CONTROL( "guild_whole_1",				m_apkGuildUIWnd[ GUILD_WHOLE_1 ] );
	LINK_CONTROL( "guild_whole_2",				m_apkGuildUIWnd[ GUILD_WHOLE_2 ] );
	LINK_CONTROL( "guild_whole_3",				m_apkGuildUIWnd[ GUILD_WHOLE_3 ] );

	LINK_CONTROL( "guild_weekly_rank_frame",	m_apkGuildUIWnd[ GUILD_WEEKLY_RANK_FRAME ] );
	LINK_CONTROL( "guild_weekly_item_name",		m_apkGuildUIWnd[ GUILD_WEEKLY_ITEM_NAME ] );
	LINK_CONTROL( "guild_weekly_item_desc",		m_apkGuildUIWnd[ GUILD_WEEKLY_ITEM_DESC ] );
	LINK_CONTROL( "guild_weekly_item_box",		m_apkGuildUIWnd[ GUILD_WEEKLY_ITEM_BOX ] );
	LINK_CONTROL( "guild_weekly_ranker_title",	m_apkGuildUIWnd[ GUILD_WEEKLY_RANKER_TITLE ] );
	LINK_CONTROL( "guild_weekly_desc_frame1",	m_apkGuildUIWnd[ GUILD_WEEKLY_DESC_FRAME1 ] );
	LINK_CONTROL( "guild_weekly_desc_frame2",	m_apkGuildUIWnd[ GUILD_WEEKLY_DESC_FRAME2 ] );
	LINK_CONTROL( "guild_weekly_1",				m_apkGuildUIWnd[ GUILD_WEEKLY_1 ] );
	LINK_CONTROL( "guild_weekly_2",				m_apkGuildUIWnd[ GUILD_WEEKLY_2 ] );
	LINK_CONTROL( "guild_weekly_3",				m_apkGuildUIWnd[ GUILD_WEEKLY_3 ] );

	LINK_CONTROL( "guild_monthly_rank_frame",	m_apkGuildUIWnd[ GUILD_MONTHLY_RANK_FRAME ] );
	LINK_CONTROL( "guild_monthly_item_name",	m_apkGuildUIWnd[ GUILD_MONTHLY_ITEM_NAME ] );
	LINK_CONTROL( "guild_monthly_item_desc",	m_apkGuildUIWnd[ GUILD_MONTHLY_ITEM_DESC ] );
	LINK_CONTROL( "guild_monthly_item_box",		m_apkGuildUIWnd[ GUILD_MONTHLY_ITEM_BOX ] );
	LINK_CONTROL( "guild_monthly_ranker_title",	m_apkGuildUIWnd[ GUILD_MONTHLY_RANKER_TITLE ] );
	LINK_CONTROL( "guild_monthly_desc_frame1",	m_apkGuildUIWnd[ GUILD_MONTHLY_DESC_FRAME1 ] );
	LINK_CONTROL( "guild_monthly_desc_frame2",	m_apkGuildUIWnd[ GUILD_MONTHLY_DESC_FRAME2 ] );
	LINK_CONTROL( "guild_monthly_1",			m_apkGuildUIWnd[ GUILD_MONTHLY_1 ] );
	LINK_CONTROL( "guild_monthly_2",			m_apkGuildUIWnd[ GUILD_MONTHLY_2 ] );
	LINK_CONTROL( "guild_monthly_3",			m_apkGuildUIWnd[ GUILD_MONTHLY_3 ] );

	LINK_CONTROL( "guild_ranking_reward_frame",	m_apkGuildUIWnd[ GUILD_RANKING_REWARD_FRAME ] );
	//------------------------------------------------------------------------
}

KGCRankingReward::~KGCRankingReward(void)
{
}

void KGCRankingReward::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		for( int i = 0 ; i < 3 ; ++i ){
			if( event.m_pWnd == m_apkRank[i] ){
				m_iMonthlyRank = i + 1;
				UpdateRankerReward();
			}
		}

		
		//#	길드 전체 123
		for( int i=GUILD_WHOLE_1; i<GUILD_WHOLE_3+1; ++i ){
			if( event.m_pWnd == m_apkGuildUIWnd[i] ){
				m_iGuildTotalRank	=	i;
				UpdateRankerReward();
			}
		}//	for end

		//#	길드 월간 123
		for( int i=GUILD_MONTHLY_1; i<GUILD_MONTHLY_3+1; ++i ){
			if( event.m_pWnd == m_apkGuildUIWnd[i] ){
				m_iGuildMonthlyRank	=	i;
				UpdateRankerReward();
			}
		}//	for end

		//# 길드 주간 123
		for( int i=GUILD_WEEKLY_1; i<GUILD_WEEKLY_3+1; ++i ){
			if( event.m_pWnd == m_apkGuildUIWnd[i] ){
				m_iGuildWeeklyRank	=	i;
				UpdateRankerReward();
			}
		}//	for end


		// 닫기 버튼 누질렀음
		if( event.m_pWnd == m_pkClosebtn )
			SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}

    UpdateRankerReward();
    GCWND_MSG_MAP( m_pkBtnSelectCharacter, KD3DWnd::D3DWE_BUTTON_CLICK, OpenCharacterSelcectUI );
}

void KGCRankingReward::OpenCharacterSelcectUI( void )
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return;
    if(g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn())
        return;

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	float x = 285.0f;
	float y = 180.0f;
	
	//LUA_GET_VALUE_DEF( "x", x, 0.5f );    
	//LUA_GET_VALUE_DEF( "y", y, 0.5f );    

    D3DXVECTOR2 vPos = m_pkBtnSelectCharacter->GetFixedWindowLocalPos();

    vPos.x += x;
    vPos.y += y;

    g_pkUIScene->m_pkCharacterSelectBox->InitCharacterSelectBox(g_MyD3D->m_pStateMachine->GetState(), 
        boost::bind(&KGCRankingReward::SendChangeChange,this,_1), 
        boost::bind(&KGCRankingReward::UpdateCharacterSelect,this), vPos, KGCCharacterSelectBox::STATE_POSITION_DOWN, KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect(vPos);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowZOrder(D3DWZ_TOPMOST);
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_LOCK);
}

void KGCRankingReward::SendChangeChange(int iChar)
{
    m_iCharType = iChar;
    g_pkUIScene->m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent(g_pkUIScene->m_pkCharacterSelectBox, EWNDMESSAGE_CLOSE) );
}

void KGCRankingReward::UpdateCharacterSelect( void )
{
    UpdateRankerReward();
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_DEFAULT);
}

void KGCRankingReward::OnCreate( void )
{
	m_pkDesc->InitState( true );
	m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_RANKING5 ) );

	m_pkWeeklyItemName->InitState( true );
	m_pkWeeklyItemDesc->InitState( true );
	m_pkMonthlyItemName[0]->InitState( true );
	m_pkMonthlyItemDesc[0]->InitState( true );
	m_pkMonthlyItemName[1]->InitState( true );
	m_pkMonthlyItemDesc[1]->InitState( true );

	m_pkClosebtn->InitState( true, true, this );

    m_pkBtnSelectCharacter->InitState(true, true, this);
	
	for( int i = 0 ; i < 3 ; ++i )
		m_apkRank[i]->InitState( true, true, this );

	//------------------------------------------------------------------------
	//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091117 : 이동석
	for( size_t i=0; i<GUILD_RANKING_WND_NUM_END; ++i )
		m_apkGuildUIWnd[i]->InitState( true, true, this );
	
	m_pkGuildWholeItemName		=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_WHOLE_ITEM_NAME]);
	m_pkGuildWeeklyItemName		=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_WEEKLY_ITEM_NAME]);
	m_pkGuildMonthlyItemName	=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_MONTHLY_ITEM_NAME]);

	m_pkGuildWholeItemDesc		=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_WHOLE_ITEM_DESC]);
	m_pkGuildWeeklyItemDesc		=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_WEEKLY_ITEM_DESC]);
	m_pkGuildMonthlyItemDesc	=	reinterpret_cast<KD3DStatic*>(m_apkGuildUIWnd[GUILD_MONTHLY_ITEM_DESC]);
	//------------------------------------------------------------------------

	UpdateRankerReward();
}

void KGCRankingReward::PostChildDraw( void )
{
	//#	길드 랭킹의 상품 내역 : 우선 데이터가 없어서 보통랭킹의 상품내역을 띄워 놓습니다.
	if( m_bUseForGuild ){
		//g_pItemMgr->Draw_ShopImage( 투디좌표, 엑스스케일, 와이스케일, 텍스쳐, 세트아이템이냐?, 세트넘버?  )

		D3DXVECTOR2	addVec2 = D3DXVECTOR2( 18.0f * m_fWindowScaleX, 8.0f * m_fWindowScaleY );
		//#	전체
		if( m_pkGuildTotalItem.ItemTexture != NULL ){
			g_pItemMgr->Draw_ShopImage(	
				m_apkGuildUIWnd[GUILD_WHOLE_ITEM_BOX]->GetRelocatedWindowPos() + addVec2,
				90.0f * m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_pkGuildTotalItem.ItemTexture, 
				m_pkGuildTotalItem.iSetNumber != -1,
				m_pkGuildTotalItem.iSetNumber,-1,m_pkGuildTotalItem.dwGoodsID );
		}

		//#	월간
		if( m_pkGuildMonthlyItem.ItemTexture != NULL ){
			g_pItemMgr->Draw_ShopImage(	
				m_apkGuildUIWnd[GUILD_MONTHLY_ITEM_BOX]->GetRelocatedWindowPos() + addVec2,
				90.0f * m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_pkGuildMonthlyItem.ItemTexture, 
				m_pkGuildMonthlyItem.iSetNumber != -1,
				m_pkGuildMonthlyItem.iSetNumber,-1,m_pkGuildMonthlyItem.dwGoodsID  );
		}

		//#	주간
		if( m_pkGuildWeeklyItem.ItemTexture != NULL ){
			g_pItemMgr->Draw_ShopImage(	
				m_apkGuildUIWnd[GUILD_WEEKLY_ITEM_BOX]->GetRelocatedWindowPos() + addVec2,
				90.0f * m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_pkGuildWeeklyItem.ItemTexture, 
				m_pkGuildWeeklyItem.iSetNumber != -1,
				m_pkGuildWeeklyItem.iSetNumber,-1,m_pkGuildWeeklyItem.dwGoodsID  );
		}


	//#	보통 랭킹의 상품 내역
	}else{
		if( m_WeeklyItem.ItemTexture != NULL )
		{
			g_pItemMgr->Draw_ShopImage(	m_pkWeeklyItemBox->GetRelocatedWindowPos() + D3DXVECTOR2( 15.0f * m_fWindowScaleX, 6.0f * m_fWindowScaleY), 90.0f * m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_WeeklyItem.ItemTexture, 
				m_WeeklyItem.iSetNumber != -1, m_WeeklyItem.iSetNumber,-1,m_WeeklyItem.dwGoodsID  );
		}

		if( m_MonthlyItem[0].ItemTexture != NULL && m_MonthlyItem[1].ItemTexture != NULL )
		{
			g_pItemMgr->Draw_ShopImage(	m_pkMonthlyItemBox[0]->GetRelocatedWindowPos() + D3DXVECTOR2( 15.0f * m_fWindowScaleX, 6.0f * m_fWindowScaleY), 90.0f* m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_MonthlyItem[0].ItemTexture, 
				m_MonthlyItem[0].iSetNumber != -1, m_MonthlyItem[0].iSetNumber,-1,m_MonthlyItem[0].dwGoodsID  );

			g_pItemMgr->Draw_ShopImage(	m_pkMonthlyItemBox[1]->GetRelocatedWindowPos() + D3DXVECTOR2( 15.0f * m_fWindowScaleX, 6.0f * m_fWindowScaleY), 90.0f * m_fWindowScaleX, 90.0f * m_fWindowScaleY, 
				m_MonthlyItem[1].ItemTexture, 
				m_MonthlyItem[1].iSetNumber != -1, m_MonthlyItem[1].iSetNumber,-1,m_MonthlyItem[1].dwGoodsID  );
		}
	}
}

void KGCRankingReward::UpdateRankerReward()
{
	//#	길드 목적으로 사용할때
	if( m_bUseForGuild ){
#if defined( NATION_KOREA ) || defined( NATION_TAIWAN )|| defined(NATION_PHILIPPINE) || defined(NATION_USA) || defined(NATION_CHINA)
        static const int ciGUILD	=	GC_CHAR_NUM;
#elif defined(NATION_BRAZIL) || defined (NATION_THAILAND) || defined (NATION_IDN) || defined(NATION_LATINAMERICA)
        static const int ciGUILD	=	1000;
#else
        static const int ciGUILD	=	9;	//	rankerreward.stg 와 숫자를 맞춘것 (현재 태국 11
#endif
		//#	Total ranking
		std::vector<DWORD>	vecTotalReward	=	g_kGlobalValue.m_kRanking.GetRankerReward( 
													KGCRankingDlg::RT_Total,
													ciGUILD, m_iGuildTotalRank - GUILD_WHOLE_1 + 1 );

		if( !vecTotalReward.empty() ){
			GCItem*				TotalItem		=	g_pItemMgr->GetItemData( vecTotalReward[0] );

    	    m_pkGuildWholeItemName->SetText( TotalItem->strItemName );
			m_pkGuildWholeItemDesc->SetTextAutoMultiline( TotalItem->strItemDesc );

			m_pkGuildTotalItem.dwGoodsID	=	TotalItem->dwGoodsID;
			m_pkGuildTotalItem.iSetNumber	=	( TotalItem->dwSlotPosition & ESP_SET_ITEM ? TotalItem->iSetItemNumber : -1 );

			SAFE_RELEASE( m_pkGuildTotalItem.ItemTexture );
			m_pkGuildTotalItem.ItemTexture = GetItemTexture( TotalItem->dwGoodsID );
		}

		//#	Monthly ranking
		std::vector<DWORD>	vecMonthlyReward	=	g_kGlobalValue.m_kRanking.GetRankerReward( 
													KGCRankingDlg::RT_Monthly,
													ciGUILD, m_iGuildMonthlyRank - GUILD_MONTHLY_1 + 1 );
		if( !vecMonthlyReward.empty() ){
			GCItem*				MonthlyItem			=	g_pItemMgr->GetItemData( vecMonthlyReward[0] );
			m_pkGuildMonthlyItemName->SetText( MonthlyItem->strItemName );
			m_pkGuildMonthlyItemDesc->SetTextAutoMultiline( MonthlyItem->strItemDesc );

			m_pkGuildMonthlyItem.dwGoodsID	=	MonthlyItem->dwGoodsID;
			m_pkGuildMonthlyItem.iSetNumber	=	( MonthlyItem->dwSlotPosition & ESP_SET_ITEM ? MonthlyItem->iSetItemNumber : -1 );

			SAFE_RELEASE( m_pkGuildMonthlyItem.ItemTexture );
			m_pkGuildMonthlyItem.ItemTexture = GetItemTexture( MonthlyItem->dwGoodsID );
		}

		//#	Weekly ranking
		std::vector<DWORD>	vecWeeklyReward		=	g_kGlobalValue.m_kRanking.GetRankerReward(
													KGCRankingDlg::RT_Weekly,
													ciGUILD, m_iGuildWeeklyRank - GUILD_WEEKLY_1 + 1 );
		if( !vecWeeklyReward.empty() ){
			GCItem*				WeeklyItem			=	g_pItemMgr->GetItemData( vecWeeklyReward[0] );
			m_pkGuildWeeklyItemName->SetText( WeeklyItem->strItemName );
			m_pkGuildWeeklyItemDesc->SetTextAutoMultiline( WeeklyItem->strItemDesc );

			m_pkGuildWeeklyItem.dwGoodsID	=	WeeklyItem->dwGoodsID;
			m_pkGuildWeeklyItem.iSetNumber	=	( WeeklyItem->dwSlotPosition & ESP_SET_ITEM ? WeeklyItem->iSetItemNumber : -1 );

			SAFE_RELEASE( m_pkGuildWeeklyItem.ItemTexture );
			m_pkGuildWeeklyItem.ItemTexture = GetItemTexture( WeeklyItem->dwGoodsID );
		}

	//#	길드 사용이 아닐때
	}else{
		std::vector<DWORD> vecWeeklyReward = g_kGlobalValue.m_kRanking.GetRankerReward( KGCRanking::RT_WEEKLY, m_iCharType, 1 );

		if( vecWeeklyReward.empty() == false )
		{
			GCItem* weeklyitem = g_pItemMgr->GetItemData( vecWeeklyReward[0] );
			m_pkWeeklyItemName->SetText( weeklyitem->strItemName );
			m_pkWeeklyItemDesc->SetTextAutoMultiline( weeklyitem->strItemDesc );

			m_WeeklyItem.dwGoodsID = weeklyitem->dwGoodsID;
			m_WeeklyItem.iSetNumber = (weeklyitem->dwSlotPosition&ESP_SET_ITEM?weeklyitem->iSetItemNumber:-1);

			SAFE_RELEASE( m_WeeklyItem.ItemTexture );
			m_WeeklyItem.ItemTexture = GetItemTexture( weeklyitem->dwGoodsID );
		}

		std::vector<DWORD> vecMonthlyReward = g_kGlobalValue.m_kRanking.GetRankerReward( KGCRanking::RT_MONTHLY, m_iCharType, m_iMonthlyRank );

		for( int i = 0 ; i < (int)vecMonthlyReward.size() && i < 2 ; ++i )
		{
			GCItem* monthlyitem = g_pItemMgr->GetItemData( vecMonthlyReward[i] );

			m_pkMonthlyItemName[i]->SetText( monthlyitem->strItemName );
			m_pkMonthlyItemDesc[i]->SetTextAutoMultiline( monthlyitem->strItemDesc );

			m_MonthlyItem[i].dwGoodsID = monthlyitem->dwGoodsID;
			m_MonthlyItem[i].iSetNumber = (monthlyitem->dwSlotPosition&ESP_SET_ITEM?monthlyitem->iSetItemNumber:-1);

			SAFE_RELEASE( m_MonthlyItem[i].ItemTexture );
			m_MonthlyItem[i].ItemTexture = GetItemTexture( monthlyitem->dwGoodsID );
		}

	}

}


GCDeviceTexture* KGCRankingReward::GetItemTexture( int iGoodsID )
{
	GCItem* item = g_pItemMgr->GetItemData( iGoodsID );

	if( item->dwSlotPosition & ESP_SET_ITEM )
	{
		return g_pItemMgr->CreateShopSetItemTexture( item->iSetItemNumber,item->dwGoodsID );
	}
	else
	{
		return g_pItemMgr->CreateItemTexture( iGoodsID, 0 );
	}
}

void KGCRankingReward::FrameMoveInEnabledState( void )
{
    POINT ptMPos = g_pkInput->GetMousePos();
    D3DXVECTOR2 vTmpPos;
    vTmpPos.x = (float)ptMPos.x;
    vTmpPos.y = (float)ptMPos.y;

	for ( int i = 0; i < 3; ++i ){
        if( false == m_apkRank[i]->CheckPosInWindowBound( vTmpPos ) )
           m_apkRank[i]->SetWndMode( (m_iMonthlyRank == i+1) ? D3DWM_SELECT : D3DWM_DEFAULT );
	}

	//#	길드 전체 123 상태 관리
	for( int i=GUILD_WHOLE_1; i<GUILD_WHOLE_3+1; ++i ){
		if( !m_apkGuildUIWnd[i]->CheckPosInWindowBound( vTmpPos ) )
			m_apkGuildUIWnd[i]->SetWndMode( ( m_iGuildTotalRank == i ) ? D3DWM_SELECT : D3DWM_DEFAULT );
	}//	for end

	//#	길드 월간 123 상태 관리
	for( int i=GUILD_MONTHLY_1; i<GUILD_MONTHLY_3+1; ++i ){
		if( !m_apkGuildUIWnd[i]->CheckPosInWindowBound( vTmpPos ) )
			m_apkGuildUIWnd[i]->SetWndMode( ( m_iGuildMonthlyRank == i ) ? D3DWM_SELECT : D3DWM_DEFAULT );
	}//	for end

	//# 길드 주간 123 상태 관리
	for( int i=GUILD_WEEKLY_1; i<GUILD_WEEKLY_3+1; ++i ){
		if( !m_apkGuildUIWnd[i]->CheckPosInWindowBound( vTmpPos ) )
			m_apkGuildUIWnd[i]->SetWndMode( ( m_iGuildWeeklyRank == i ) ? D3DWM_SELECT : D3DWM_DEFAULT );
	}
}


//------------------------------------------------------------------------
//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091117 : 이동석
void KGCRankingReward::ToggleUseForGuild( bool bUseForGuild )
{
	m_bUseForGuild	=	bUseForGuild;

	//	길드 전용 UI
	for( size_t i=0; i<GUILD_RANKING_WND_NUM_END; ++i )
		m_apkGuildUIWnd[i]->ToggleRender( m_bUseForGuild );
}
