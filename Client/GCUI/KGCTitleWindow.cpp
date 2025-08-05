#include "stdafx.h"
#include ".\kgctitlewindow.h"
//


//
#include "../mission/GCCTMissionManager.h"
#include "../MyD3D.h"
//
#include "../gcui/GCItemInfoBox.h"



STitleItem::STitleItem()
{
	m_eState				= KGCTitleSlotRow::TSB_NONE;
	m_iPrevTitleID			= -1;
	m_iMissionID      		= -1;
	m_iExtraMissionID 		= -1;
	m_iTitleItemID    		= -1;
	m_fHpRecoveryIncrease	= 0.0f;
	m_fStateConfuseDecrease	= 0.0f;
	m_fArenaTimeIncrease	= 0.0f;
	m_pTexture				= NULL;
}

void STitleItem::SetTexture( int iTitleID /*= -1 */ )
{
	m_pTexture = g_pItemMgr->CreateItemTexture( iTitleID==-1?m_iTitleItemID/10:iTitleID/10, 0 );
}

void STitleItem::ReleaseTexture()
{
	SAFE_RELEASE( m_pTexture );
}

IMPLEMENT_CLASSNAME( KGCTitleWindow );
IMPLEMENT_WND_FACTORY( KGCTitleWindow );
IMPLEMENT_WND_FACTORY_NAME( KGCTitleWindow, "gc_title_window" );

KGCTitleWindow::KGCTitleWindow(void)
{
    char str[256] = "";
    for ( int i = 0; i < NUM_OF_TITLE_ROW; ++i )
    {
        m_pkTitleSlotRow[i]       = NULL;

        sprintf( str, "title_slot_row%d", i+1 );
        LINK_CONTROL( str,               m_pkTitleSlotRow[i] );
    }

    m_pkClose   = NULL;
    LINK_CONTROL( "close_button",               m_pkClose );

    m_pkScrollBar   = NULL;
    LINK_CONTROL( "replay_box_scroll",               m_pkScrollBar );

    m_pkEmoticonBtn         = NULL;
    m_pkSelTitleBtn         = NULL;
    m_pkPetBtn              = NULL;
    m_pkMonsterCardBook     = NULL;
    LINK_CONTROL( "emoticon_button",         m_pkEmoticonBtn );
    LINK_CONTROL( "title_button_big",        m_pkSelTitleBtn );
    LINK_CONTROL( "pet_button",              m_pkPetBtn );
    LINK_CONTROL( "monster_card_button",     m_pkMonsterCardBook );

    m_iRow = 0;
}

KGCTitleWindow::~KGCTitleWindow(void)
{
	DestroyTitleShoImage();
}

void KGCTitleWindow::OnCreate( void )
{
    for ( int i = 0; i < NUM_OF_TITLE_ROW; ++i )
    {
        m_pkTitleSlotRow[i]->InitState( true, true, this );
    }

    m_pkClose->InitState( true, true, this );
    m_pkClose->SetHotKey( DIK_ESCAPE );

    m_pkScrollBar->InitState( true, true, this );

    m_pkEmoticonBtn->InitState( true, true, this );
    m_pkSelTitleBtn->InitState( true, true, this );
    m_pkPetBtn->InitState( true, true, this );

	//resource 에서 On/Off 하도록. 즉 리소스에 존재 하면 작동하도록
	if (m_pkMonsterCardBook)
		m_pkMonsterCardBook->InitState( true, true, this );


    m_pkScrollBar->SetScrollPos( 0 );
    m_iRow = 0;

    LoadingItemList();
    
    if ( !GC_GLOBAL_DEFINE::bRealTimeLoadEtcTex ) 
    {
        CreateTitleShopImage();
    }
}

void KGCTitleWindow::ActionPerformed( const KActionEvent& event )
{
    if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        g_KDSound.Play( "31" ); 
        if( event.m_pWnd == m_pkClose )
        {
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
        else if ( event.m_pWnd == m_pkEmoticonBtn )
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_EMOTICON_COLLECTION ) );
        }
        else if( event.m_pWnd == m_pkPetBtn )
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_PET_COLLECTION ) );
        }
        else if( event.m_pWnd == m_pkMonsterCardBook )
        {
			if (m_pkMonsterCardBook)
			{
				SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_BOOK, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
			}
        }
    }

	if( KGCTitleSlotRow::REFRESH_TITLE_LIST == event.m_dwlParam )
	{
		for(int i=0;i<NUM_OF_TITLE_ROW;++i)
		{
			if( event.m_pWnd == m_pkTitleSlotRow[i] )
			{
				RefreshTitleList();

				if( -1 == event.m_dwCode )
					RefreshAllTitleIndex();
				else
					RefreshTitleIndex( GetFirstTitleIndex() + (i*KGCTitleSlotRow::NUM_OF_TITLE_COL) + event.m_dwCode );

				break;
			}
		}
	}

    if( event.m_pWnd == m_pkScrollBar )
    {
        if( m_pkScrollBar->GetScrollPos() != m_iRow )
        {
            if( g_pkUIScene->m_pkItemInformationBox )
                g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
            RefreshAllTitleIndex();
            m_iRow = m_pkScrollBar->GetScrollPos();
        }
    }
}

void KGCTitleWindow::PostChildDraw( void )
{
}

void KGCTitleWindow::FrameMoveInEnabledState( void )
{
    if( g_pkUIScene )
    {
        if( g_pkUIScene->m_pkItemInformationBox )
        {
            D3DXVECTOR2 vPos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
            if( !CheckPosInWindowBound( vPos ) )
			{
                g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
			}
        }
    }
}

void KGCTitleWindow::RefreshTitleIndex( int iIndex )
{
    if( (int)m_vecTitleList.size() <= iIndex )
        return;

    const int iSlotIndex = GetSlotIndexByIndex( iIndex );
    if( -1 == iSlotIndex )
        return;

    int iRow = (iSlotIndex > 0) ? (iSlotIndex / KGCTitleSlotRow::NUM_OF_TITLE_COL) : 0;
    int iCol = (iSlotIndex > 0) ? (iSlotIndex % KGCTitleSlotRow::NUM_OF_TITLE_COL) : 0;

	if( m_mapTitleItem.find( m_vecTitleList[iIndex] ) == m_mapTitleItem.end() )
		return;

	STitleItem sTitle = m_mapTitleItem[m_vecTitleList[iIndex]];

    GCItem *pItem = g_pItemMgr->GetItemData( sTitle.m_iTitleItemID/10);    
    if( NULL == pItem )
        return;

    if ( !sTitle.m_pTexture ) 
    {
        sTitle.SetTexture();
    }

	m_pkTitleSlotRow[iRow]->SetItemInfo( iCol, sTitle.m_iTitleItemID/10, sTitle.m_pTexture );

	if( GetNotMyPreviousTitleCount( sTitle.m_iTitleItemID ) )
		m_pkTitleSlotRow[iRow]->SetMissionID( iCol, sTitle.m_iTitleItemID );
	else
		m_pkTitleSlotRow[iRow]->SetMissionID( iCol, sTitle.m_iExtraMissionID );

    m_pkTitleSlotRow[iRow]->SetUseBtnType( iCol, sTitle.m_eState );
}

void KGCTitleWindow::RefreshAllTitleIndex( void )
{
    ClearAllTitleIndex();

    m_pkScrollBar->SetScrollPageSize( 1 );
    m_pkScrollBar->SetScrollRangeMin( 0 );

    // 스크롤바 사이즈를 구하는 식.
    int iScrollSize = ((int)m_vecTitleList.size() / KGCTitleSlotRow::NUM_OF_TITLE_COL);
    if( (int)m_vecTitleList.size() % KGCTitleSlotRow::NUM_OF_TITLE_COL > 0 )
        ++iScrollSize;
    if( iScrollSize >= (NUM_OF_TITLE_ROW-1) )
        iScrollSize -= (NUM_OF_TITLE_ROW-1);

    m_pkScrollBar->SetScrollRangeMax( iScrollSize );

    for(int i=GetFirstTitleIndex();i<(int)m_vecTitleList.size();++i)
    {
        RefreshTitleIndex(i);
    }
}

void KGCTitleWindow::ClearAllTitleIndex( void )
{
    for(int i=0;i<NUM_OF_TITLE_ROW;++i)
    {
        m_pkTitleSlotRow[i]->ReSet();
    }
}

int KGCTitleWindow::GetFirstTitleIndex( void )
{
    return (m_pkScrollBar->GetScrollPos() * KGCTitleSlotRow::NUM_OF_TITLE_COL);
}

void KGCTitleWindow::RefreshTitleList(void)
{
    for(int i=0;i<(int)m_vecTitleList.size();++i)
    {
		int iMissionID = 0;
		STitleItem sTitle = m_mapTitleItem[m_vecTitleList[i]];

		if( sTitle.m_iMissionID == -1 )
		{
			int iState = KGCTitleSlotRow::TSB_NONE;
			// 장착되어있을때 처리를 해주어야한다.
			if( g_pItemMgr->IsEquipTitleItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), sTitle.m_iTitleItemID/10 ) )
				iState = KGCTitleSlotRow::TSB_OFF;
			//else if( g_kGlobalValue.m_kUserCollectionMission.IsCompleteCTMission( sMissionInfo->m_dwID ) )
			else if( g_pItemMgr->FindInventoryForItemID( sTitle.m_iTitleItemID/10 ) )
				iState = KGCTitleSlotRow::TSB_ON;
			else
				iState = KGCTitleSlotRow::TSB_NONE;

			m_mapTitleItem[m_vecTitleList[i]].m_eState = (KGCTitleSlotRow::TITLE_SLOT_BTN)iState;

			continue;
		}

		if( GetNotMyPreviousTitleCount(m_vecTitleList[i]) )
			iMissionID = sTitle.m_iMissionID;
		else
			iMissionID = sTitle.m_iExtraMissionID;

        const SMissionInfo *sMissionInfo = SiKGCCTMissionManager()->GetMissionInfo( iMissionID );

        if( sMissionInfo == NULL )
            continue;

		bool bCompleteAllMissions = false;
		int iCompleteSubCount = 0;
		for(int j=0;j<(int)sMissionInfo->m_vecSubMissions.size();++j)
		{
			const SSubMissionInfo* sSubInfo = SiKGCCTMissionManager()->GetSubMission( sMissionInfo->m_vecSubMissions[j] );

			const int iCurCount = g_kGlobalValue.m_kUserCollectionMission.GetCompletionCount( sMissionInfo->m_dwID, sMissionInfo->m_vecSubMissions[j] );
			const int iCompleteCount = sSubInfo->m_sCompletion.m_iCount;

			if( iCurCount >= iCompleteCount )
				iCompleteSubCount++;
		}
		if( sMissionInfo->m_vecSubMissions.size() == iCompleteSubCount )
			bCompleteAllMissions = true;

        int iState = KGCTitleSlotRow::TSB_NONE;

        // 장착되어있을때 처리를 해주어야한다.
        if( g_pItemMgr->IsEquipTitleItem( &g_MyD3D->m_TempPlayer.GetCurrentChar(), sTitle.m_iTitleItemID/10 ) )
            iState = KGCTitleSlotRow::TSB_OFF;
        //else if( g_kGlobalValue.m_kUserCollectionMission.IsCompleteCTMission( sMissionInfo->m_dwID ) )
		else if( g_pItemMgr->FindInventoryForItemID( sTitle.m_iTitleItemID/10 ) )
            iState = KGCTitleSlotRow::TSB_ON;
        else if( bCompleteAllMissions )
            iState = KGCTitleSlotRow::TSB_GET;
        else 
            iState = KGCTitleSlotRow::TSB_CONDITION;

		m_mapTitleItem[m_vecTitleList[i]].m_eState = (KGCTitleSlotRow::TITLE_SLOT_BTN)iState;
    }
}

int KGCTitleWindow::GetSlotIndexByIndex(int iIndex)
{
    const int ciFirst_SIndex   = GetFirstTitleIndex();
    const int ciEnd_SIndex     = ciFirst_SIndex + (NUM_OF_TITLE_ROW*KGCTitleSlotRow::NUM_OF_TITLE_COL);

    if( iIndex < ciFirst_SIndex || iIndex >= ciEnd_SIndex )
        return -1;

    return iIndex - GetFirstTitleIndex();
}

void KGCTitleWindow::LoadingItemList( void )
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr , "collection_missionlist.lua" ) == false )
        return ;

    m_vecTitleList.clear();
	m_mapTitleItem.clear();


    LUA_BEGIN_TABLE( "COLLECTION_MISSIONLIST", return )
    {
        char strTable[MAX_PATH];
        strcpy( strTable, "TITLE" );
        LUA_BEGIN_TABLE( strTable, return )
        {
            for ( int j = 1; ; ++j )
            {
                LUA_BEGIN_TABLE( j, break )
                {
					/*
                    int iMissionID      = -1;
					int iExtraMissionID = -1;
                    int iTitleItemID    = -1;
                    float fHpRecoveryIncrease		= 0.0f;
					float fStateConfuseDecrease		= 0.0f;
					float fArenaTimeIncrease		= 0.0f;
                    LUA_GET_VALUE_DEF("MissionID", iMissionID, -1);
					LUA_GET_VALUE_DEF("ExtraMissionID", iExtraMissionID, -1);					
                    LUA_GET_VALUE_DEF("TitleID", iTitleItemID, -1);
                    LUA_GET_VALUE_DEF("HpRecoveryIncrease", fHpRecoveryIncrease, 0.0f);
					LUA_GET_VALUE_DEF("StateConfuseDecrease", fStateConfuseDecrease, 0.0f);
					LUA_GET_VALUE_DEF("ArenaTimeIncrease", fArenaTimeIncrease, 0.0f);

                    if( -1 != iMissionID || -1 != iTitleItemID )
					{
                        m_vecTitleList.push_back( std::make_pair(iMissionID, KGCTitleSlotRow::TSB_NONE) );
						m_vecExtraMission.push_back( iExtraMissionID );

						m_vecHpRecoveryIncrease.push_back( std::make_pair(iTitleItemID, fHpRecoveryIncrease) );
						m_vecStateConfuseDecrease.push_back( std::make_pair(iTitleItemID, fStateConfuseDecrease) );
						m_vecArenaTimeIncrease.push_back( std::make_pair(iTitleItemID, fArenaTimeIncrease) );
					}
					//*/


					STitleItem sTitleItem;
					LUA_GET_VALUE( "PrevTitleID",			sTitleItem.m_iPrevTitleID,			break );
					LUA_GET_VALUE( "MissionID",				sTitleItem.m_iMissionID,			break );
					LUA_GET_VALUE( "ExtraMissionID",		sTitleItem.m_iExtraMissionID,		break );					
					LUA_GET_VALUE( "TitleID",				sTitleItem.m_iTitleItemID,			break );
					LUA_GET_VALUE( "HpRecoveryIncrease",	sTitleItem.m_fHpRecoveryIncrease,	break );
					LUA_GET_VALUE( "StateConfuseDecrease",	sTitleItem.m_fStateConfuseDecrease, break );
					LUA_GET_VALUE( "ArenaTimeIncrease",		sTitleItem.m_fArenaTimeIncrease,	break );

					m_mapTitleItem.insert( MapTitleItem::value_type( sTitleItem.m_iTitleItemID, sTitleItem ) );
					m_vecTitleList.push_back( sTitleItem.m_iTitleItemID );
                }
                LUA_END_TABLE( break )
            }
        }
        LUA_END_TABLE( return )
    }
    LUA_END_TABLE( return );
}

bool KGCTitleWindow::IsTitleItemID(const int iItemID )
{
    return m_mapTitleItem.find( iItemID ) != m_mapTitleItem.end();
}

bool KGCTitleWindow::IsTitleItemListIndex(const int iTitleID)
{
    if( m_mapTitleItem.find( iTitleID ) == m_mapTitleItem.end() ) 
        return false; 
    
    return true; 
}

float KGCTitleWindow::GetTitleItemAbilityPercent(const int iTitleID, const COLLECTION_TITLE_ABILITY_TYPE enType)
{
	switch( enType )
	{
		case CAT_HP:
			return GetTitleHpRecoveryPercent( iTitleID );
			break;
		case CAT_STATECONFUSE:
			return GetTitleStateConfusePercent( iTitleID );
			break;
		case CAT_ARENA:
			return GetTitleArenaTimePercent( iTitleID );
			break;
	}

	return 0.0f;
}

float KGCTitleWindow::GetTitleHpRecoveryPercent(const int iTitleID)
{
	if( IsTitleItemListIndex(iTitleID) )
	{
		// 능력치 감소율 제거.
		return m_mapTitleItem[iTitleID].m_fHpRecoveryIncrease;
	}

	return 0.0f;
}

float KGCTitleWindow::GetTitleStateConfusePercent(const int iTitleID)
{
	if( IsTitleItemListIndex(iTitleID) )
	{
		// 능력치 감소율 제거.
		return m_mapTitleItem[iTitleID].m_fStateConfuseDecrease;
	}

	return 0.0f;
}

float KGCTitleWindow::GetTitleArenaTimePercent(const int iTitleID)
{
	if( IsTitleItemListIndex(iTitleID) )
	{
		// 능력치 감소율 제거.
		return m_mapTitleItem[iTitleID].m_fArenaTimeIncrease;
	}

	return 0.0f;
}

float KGCTitleWindow::GetTitleCalcDecreaseAbility(const int iIndex, const float fAbilityValue)
{
	if( !IsTitleItemListIndex(iIndex) )
		return 0.0f;

	const int iTotalTitleCnt = (iIndex);		// 이전의 모든 칭호 수

	if( 0 == iTotalTitleCnt )
		return 0.0f;

	return ( fAbilityValue - ( fAbilityValue * 0.75f * (float)GetNotMyPreviousTitleCount(iIndex) / (float)iTotalTitleCnt ) );
}

int KGCTitleWindow::GetNotMyPreviousTitleCount(const int iTitleID)
{
	if( !IsTitleItemListIndex(iTitleID) )
		return 0;

	int iNotMyTitleCnt = 0;

	int iPrevID = iTitleID;
	while( true )
	{
		STitleItem sTitle = m_mapTitleItem[iPrevID];
		iPrevID = sTitle.m_iPrevTitleID;

		if( iPrevID == -1 )
			break;

		if( false == g_pItemMgr->FindInventoryForItemID( iPrevID / 10 ) )
			++iNotMyTitleCnt;
	}

	return iNotMyTitleCnt;
}

float KGCTitleWindow::GetTitleDecreasePercentByItemID(const int iTitleID)
{
	if( IsTitleItemID( iTitleID ) == false )
		return 0.0f;

	int iMyTitleCnt = 1;
	int iPrevID = iTitleID;
	while( true )
	{
		STitleItem sTitle = m_mapTitleItem[iPrevID];
		iPrevID = sTitle.m_iPrevTitleID;

		if( iPrevID == -1 )
			break;

		++iMyTitleCnt;
	}

	return ( ( 100 * 0.75f * (float)GetNotMyPreviousTitleCount(iTitleID) / (float)iMyTitleCnt ) );
}

bool KGCTitleWindow::IsCompleteCTMissionAndNotGet( void )
{		
	for(int i=0;i<(int)m_vecTitleList.size();++i)
	{
		if( KGCTitleSlotRow::TSB_GET == m_mapTitleItem[m_vecTitleList[i]].m_eState )
			return true;
	}

	return false;
}

void KGCTitleWindow::CreateTitleShopImage(void)
{
	DestroyTitleShoImage();

	MapTitleItem::iterator mit;
	for( mit = m_mapTitleItem.begin() ; mit != m_mapTitleItem.end() ; ++mit )
	{
		mit->second.SetTexture();
	}
}

void KGCTitleWindow::DestroyTitleShoImage(void)
{
	MapTitleItem::iterator mit;
	for( mit = m_mapTitleItem.begin() ; mit != m_mapTitleItem.end() ; ++mit )
	{
		mit->second.ReleaseTexture();
	}
}

bool KGCTitleWindow::IsCurrentUseMissionID( const int iTitleID )
{
	if( IsTitleItemID( iTitleID ) == false )
		return false;

	if( GetNotMyPreviousTitleCount( iTitleID ) )
	{
		return true;
	}
	else
	{
		if( iTitleID == m_mapTitleItem[iTitleID].m_iExtraMissionID )
			return true;
	}

	return false;
}