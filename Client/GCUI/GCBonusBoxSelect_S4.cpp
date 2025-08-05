#include "stdafx.h"
#include "GCBonusBoxSelect_S4.h"
#include "GCItemImgWnd.h"
#include "GCItemManager.h"
#include "GCBonusBoxSelect_S4_Row.h"

IMPLEMENT_CLASSNAME( KGCBonusBoxSelect_S4 );
IMPLEMENT_WND_FACTORY( KGCBonusBoxSelect_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusBoxSelect_S4, " gc_bonus_box_select_s4" );

KGCBonusBoxSelect_S4::KGCBonusBoxSelect_S4( void )
: m_pkGeneralTitle( NULL )
, m_pkSpecialTitle( NULL )
, m_pkRewardTitle( NULL )
, m_pFrameMove( NULL )
, m_dwTimeCheck( 0 )
, m_eAnimState( BBAS_SHOW_TITLE )
, m_bAllProcessComplete( false )
, m_bSendBoxIndex( false )
, m_bStopCount( false )
, m_bSpecialReward( false )
, m_vTileInitPos( 0.0f, 0.0f )
{
    m_mapUserSelect.clear();

    LINK_CONTROL( "title_wnd", m_pkGeneralTitle );
    LINK_CONTROL( "title_wnd_special", m_pkSpecialTitle );
    LINK_CONTROL( "title_wnd_reward", m_pkRewardTitle );


    char strTemp[ MAX_PATH ] = "";
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkBonusBoxRow[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_box_select_s4_row%d", i );
        LINK_CONTROL( strTemp, m_pkBonusBoxRow[i] );
    }

    for( int i = 0 ; i < SELECT_BONUS_BOX_TIME_LIMIT; ++i )
    {
        sprintf_s( strTemp, MAX_PATH, "box_count_down_%d", i );
        LINK_CONTROL( strTemp, m_pkCountNum[i] );
    }

    for( int i = 0 ; i <  BBAS_MAX; ++i )
    {
        m_pInitAnimFunc[i] = NULL;
        m_pFrameMoveAnimFunc[i] = NULL;
    }

    m_pInitAnimFunc[ BBAS_SHOW_TITLE ] = &KGCBonusBoxSelect_S4::InitShowTitle;
    m_pInitAnimFunc[ BBAS_SHOW_BONUS_BOX ] = &KGCBonusBoxSelect_S4::InitShowBonusBox;
    m_pInitAnimFunc[ BBAS_BEFORE_TIME_COUNT ] = &KGCBonusBoxSelect_S4::InitBeforeTimeCount;
    m_pInitAnimFunc[ BBAS_TIME_COUNT ] = &KGCBonusBoxSelect_S4::InitTimeCount;
    m_pInitAnimFunc[ BBAS_BEFORE_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect_S4::InitBeforeBonusBoxOpen;
    m_pInitAnimFunc[ BBAS_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect_S4::InitBonusBoxOpen;
    m_pInitAnimFunc[ BBAS_ALL_PROCESS_COMPLETE ] = &KGCBonusBoxSelect_S4::InitAllProcessComplete;

    m_pFrameMoveAnimFunc[ BBAS_SHOW_TITLE ] = &KGCBonusBoxSelect_S4::FrameMoveShowTitle;
    m_pFrameMoveAnimFunc[ BBAS_SHOW_BONUS_BOX ] = &KGCBonusBoxSelect_S4::FrameMoveShowBonusBox;
    m_pFrameMoveAnimFunc[ BBAS_BEFORE_TIME_COUNT ] = &KGCBonusBoxSelect_S4::FrameMoveBeforeTimeCount;
    m_pFrameMoveAnimFunc[ BBAS_TIME_COUNT ] = &KGCBonusBoxSelect_S4::FrameMoveTimeCount;
    m_pFrameMoveAnimFunc[ BBAS_BEFORE_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect_S4::FrameMoveBeforeBonusBoxOpen;
    m_pFrameMoveAnimFunc[ BBAS_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect_S4::FrameMoveBonusBoxOpen;
    m_pFrameMoveAnimFunc[ BBAS_ALL_PROCESS_COMPLETE ] = &KGCBonusBoxSelect_S4::FrameMoveAllProcessComplete;
}

KGCBonusBoxSelect_S4::~KGCBonusBoxSelect_S4( void )
{
}

void KGCBonusBoxSelect_S4::InitUIs( void )
{
    m_mapUserSelect.clear();
    m_bAllProcessComplete = false;
    m_bSendBoxIndex = false;
    m_bStopCount = false;
    m_bSpecialReward = false;

	m_bOpenItemImg = false;

    int iIndex = 1;
    for(int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(g_pGameBoard->m_pStartingMember[i].dwUID);
        if ( NULL == pPlayer )
            continue;

        if ( pPlayer->IsLocalPlayer() )
        {
            m_pkBonusBoxRow[0]->InitState( true, true, this );
            m_pkBonusBoxRow[0]->InitRow( i, true );
        }
        else if ( pPlayer->IsLive() )
        {
            m_pkBonusBoxRow[iIndex]->InitState( true, false, NULL );
            m_pkBonusBoxRow[iIndex]->InitRow( i, false );
            ++iIndex;
        }
    }

    for( int i = 0 ; i < SELECT_BONUS_BOX_TIME_LIMIT; ++i )
    {
        m_pkCountNum[i]->ToggleRender( false );
    }

    InitAnimation();
}

void KGCBonusBoxSelect_S4::OnCreate( void )
{
    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        return;

    InitUIs();
}

void KGCBonusBoxSelect_S4::OnCreateComplete( void )
{
    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        return;

    InitUIs();

    SetAnimState( BBAS_SHOW_TITLE );
}

void KGCBonusBoxSelect_S4::FrameMoveInEnabledState( void )
{
    if( m_pFrameMove )
        (this->*m_pFrameMove)();
}

void KGCBonusBoxSelect_S4::PostChildDraw( void )
{
}

void KGCBonusBoxSelect_S4::ActionPerformed( const KActionEvent& event )
{
    if( BBAS_TIME_COUNT != m_eAnimState  )
        return;
}

void KGCBonusBoxSelect_S4::SetKeyNum( int iKey_ )
{
    m_pkBonusBoxRow[0]->SetMyKeyNum( iKey_ );
}

void KGCBonusBoxSelect_S4::SetUserSelectBox( DWORD dwUserUID_, USHORT usBoxIndex_ )
{
    for ( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID != dwUserUID_ && ( m_pkBonusBoxRow[i]->GetOwnerUID() == dwUserUID_ ) )
        {
            m_pkBonusBoxRow[i]->SetClickedBonusBox( usBoxIndex_ );
            break;
        }
    }

    if ( false == g_kGlobalValue.m_kUserInfo.bHost )
        return;

    if ( m_mapUserSelect.end() == m_mapUserSelect.find( dwUserUID_ ) )
    {
        std::set<USHORT> tmpSet;
        tmpSet.insert( usBoxIndex_ );
        m_mapUserSelect[dwUserUID_] = tmpSet;
    }
    else
        m_mapUserSelect[dwUserUID_].insert( usBoxIndex_ );
}

void KGCBonusBoxSelect_S4::SetAnimState( BONUS_BOX_ANIM_STATE eAnimState_ )
{
    if( eAnimState_ < 0 || eAnimState_ >= BBAS_MAX )
        return;

    m_dwTimeCheck = 0;

    int iStartIndex = 0;
    if( m_eAnimState <= eAnimState_ )
        iStartIndex = static_cast< int >( m_eAnimState );

    int iAnimState = static_cast<int>( eAnimState_ );
    for( int i = iStartIndex; i <=  iAnimState; ++i )
    {
        if( m_pInitAnimFunc[i] )
            ( this->*m_pInitAnimFunc[i] )();
    }

    m_pFrameMove = m_pFrameMoveAnimFunc[ eAnimState_ ];

    m_eAnimState = eAnimState_;
}

void KGCBonusBoxSelect_S4::InitShowTitle()
{
    m_EffectBoxTtitle.Reset();
    //m_pkSpecialTitle->SetWindowPosDirect( m_vTileInitPos );

    int iIndex = 1;
    for(int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkBonusBoxRow[i]->InitState( true, false, NULL );
        if ( m_pkBonusBoxRow[i]->GetOwnerUID() == 0 )
        {
            m_pkBonusBoxRow[i]->InitAbsent();
            m_pkBonusBoxRow[i]->SetAllWindowColor( D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
        }

        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(g_pGameBoard->m_pStartingMember[i].dwUID);
        if ( NULL == pPlayer )
            continue;

        if ( pPlayer->IsLocalPlayer() )
        {
            m_pkBonusBoxRow[0]->InitState( true, true, this );
            m_pkBonusBoxRow[0]->InitRow( i, true );
            m_pkBonusBoxRow[0]->SetAllWindowColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        }
        else
        {
            //  6명 파티일때 해당 UI가 켜지는 문제로 여기서 에러 생긴다.
            if ( iIndex >= OLD_MAX_DUNGEON_PLAYER_NUM ) 
                break;

            if ( pPlayer->IsLive() )
            {
                m_pkBonusBoxRow[iIndex]->InitRow( i, false );
                m_pkBonusBoxRow[iIndex]->SetAllWindowColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
                ++iIndex;
            }
        }
    }
}

void KGCBonusBoxSelect_S4::InitShowBonusBox()
{
    //m_pkSpecialTitle->SetWidth( m_EffectBoxTtitle.GetOriginalWidth() );
    //m_pkSpecialTitle->SetHeight( m_EffectBoxTtitle.GetOriginalHeight() );
    //m_pkSpecialTitle->SetWindowPosDirect( m_EffectBoxTtitle.GetOriginalPos() );
}

void KGCBonusBoxSelect_S4::InitBeforeTimeCount()
{

}

void KGCBonusBoxSelect_S4::InitTimeCount()
{
    m_pkBonusBoxRow[0]->EnableClick();
}

void KGCBonusBoxSelect_S4::InitBeforeBonusBoxOpen()
{
}

void KGCBonusBoxSelect_S4::InitBonusBoxOpen()
{
}

void KGCBonusBoxSelect_S4::InitAllProcessComplete()
{

}

void KGCBonusBoxSelect_S4::FrameMoveShowTitle()
{
    if( !m_EffectBoxTtitle.IsEndOfScale() )
    {
        DWORD dwPreWidth = m_EffectBoxTtitle.GetCurrentWidth();
        DWORD dwPreHeight = m_EffectBoxTtitle.GetCurrentHeight();
        m_EffectBoxTtitle.ScaleStep();

        DWORD dwCurrentWidth = m_EffectBoxTtitle.GetCurrentWidth();
        DWORD dwCurrentHeight = m_EffectBoxTtitle.GetCurrentHeight();


        D3DXVECTOR2 vMovePos;
        if( dwPreWidth > dwCurrentWidth )
        {
            vMovePos.x = static_cast<float>( dwPreWidth - dwCurrentWidth ) * 0.5f;
            vMovePos.y = static_cast<float>( dwPreHeight - dwCurrentHeight )  * 0.5f;
        }
        else
        {
            vMovePos.x -= static_cast<float>( dwCurrentWidth - dwPreWidth ) * 0.5f;
            vMovePos.y -= static_cast<float>( dwCurrentHeight- dwPreHeight )  * 0.5f;
        }

        //m_pkSpecialTitle->SetWidth( dwCurrentWidth );
        //m_pkSpecialTitle->SetHeight( dwCurrentHeight );

        //m_pkSpecialTitle->SetWindowPosDirect( m_pkSpecialTitle->GetFixedWindowLocalPos() + vMovePos );
        if( m_EffectBoxTtitle.IsEndOfScale() )
        {
            g_KDSound.Play( "136" );
        }
    }

    m_EffectBoxTtitle.ShowStep();
    //다음으로 넘어가자
    if( m_EffectBoxTtitle.IsEndOfShow() )
    {
        
        
        if ( m_bSpecialReward )
        {
            float fOffsetX = 200.0f * GC_SCREEN_DIV_WIDTH;
            float fOffsetY = 21.0f * GC_SCREEN_DIV_WIDTH;

			//KLuaManager luaMgr;
			//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
			//LUA_GET_VALUE_DEF( "f_x", fOffsetX, 0.0f );
			//LUA_GET_VALUE_DEF( "f_y", fOffsetY, 0.0f );

            GCFUNC::CreateSequenceToUI( m_pkSpecialTitle, "Special_Reward_01", fOffsetX, fOffsetY );
            GCFUNC::CreateSequenceToUI( m_pkSpecialTitle, "Special_Reward_02", fOffsetX, fOffsetY );
            GCFUNC::CreateSequenceToUI( m_pkSpecialTitle, "Special_Reward_03", fOffsetX, fOffsetY );
            GCFUNC::CreateSequenceToUI( m_pkSpecialTitle, "Special_Reward_04", fOffsetX, fOffsetY );
            GCFUNC::CreateSequenceToUI( m_pkSpecialTitle, "Special_Reward_05", fOffsetX, fOffsetY );
        }

        SetAnimState( BBAS_SHOW_BONUS_BOX );
    }
}

void KGCBonusBoxSelect_S4::FrameMoveShowBonusBox()
{
    if(++m_dwTimeCheck >= ( GC_FPS_LIMIT * 1) )
    {
        SetAnimState( BBAS_BEFORE_TIME_COUNT );
    }
}

void KGCBonusBoxSelect_S4::FrameMoveBeforeTimeCount()
{
    if(++m_dwTimeCheck >= ( GC_FPS_LIMIT * 1) )
    {
        SetAnimState( BBAS_TIME_COUNT );
    }
}

void KGCBonusBoxSelect_S4::FrameMoveTimeCount()
{
    ++m_dwTimeCheck;

    for ( int i = 0; i < SELECT_BONUS_BOX_TIME_LIMIT; ++i )
    {
        int index = (SELECT_BONUS_BOX_TIME_LIMIT - 1) - (m_dwTimeCheck / GC_FPS_LIMIT);
        if ( index < 0 )
            index = 0;
        m_pkCountNum[i]->ToggleRender( (i == index) && !m_bStopCount );
    }

    if( m_dwTimeCheck >= ( GC_FPS_LIMIT * ( SELECT_BONUS_BOX_TIME_LIMIT + 1 ) ) )
    {
        m_pkBonusBoxRow[0]->AutoSelect();
    }

    if ( o6 )
    {
        int iCount = 0;
        for ( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
        {
            PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(g_pGameBoard->m_pStartingMember[i].dwUID);
            if ( NULL == pPlayer )
                continue;

            if ( pPlayer->IsLive() )
            {
                ++iCount;
                if ( m_mapUserSelect.end() != m_mapUserSelect.find( pPlayer->m_kUserInfo.dwUID ) )
                    --iCount;
            }
        }

        if ( iCount == 0 )
        {
            SetAnimState( BBAS_BEFORE_BONUS_BOX_OPEN );
        }
    }
}

void KGCBonusBoxSelect_S4::FrameMoveBeforeBonusBoxOpen()
{
    if ( false == g_kGlobalValue.m_kUserInfo.bHost )
        return;

    if ( o6 )
    {
        int iCount = 0;
        for ( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
        {
            PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(g_pGameBoard->m_pStartingMember[i].dwUID);
            if ( NULL == pPlayer )
                continue;

            if ( pPlayer->IsLive() )
            {
                ++iCount;
                if ( m_mapUserSelect.end() != m_mapUserSelect.find( pPlayer->m_kUserInfo.dwUID ) )
                    --iCount;
            }
        }

        if ( iCount == 0 && false == m_bSendBoxIndex )
        {
            SiKP2P()->GetInstance()->Send_SpecialRewardReq( m_mapUserSelect );
            m_bSendBoxIndex = true;
        }
    }
}

void KGCBonusBoxSelect_S4::FrameMoveBonusBoxOpen()
{
    if ( m_dwTimeCheck == 0 )
    {
        m_pkBonusBoxRow[m_dwTimeCheck]->OpenBonusBoxEffect();
    }
    else if ( m_dwTimeCheck == 25 )
    {
        for(int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
        {
            m_pkBonusBoxRow[i]->InitBeforeBoxOpen();
        }
		m_bOpenItemImg = true;
    }

    if(++m_dwTimeCheck >= ( GC_FPS_LIMIT * 3) )
    {
        SetAnimState( BBAS_ALL_PROCESS_COMPLETE );
    }
}

void KGCBonusBoxSelect_S4::FrameMoveAllProcessComplete()
{
    if(++m_dwTimeCheck >= ( GC_FPS_LIMIT * 3) )
    {
        m_bAllProcessComplete = true;
    }
}

void KGCBonusBoxSelect_S4::SetEachUserBoxInfo( KRewardInfoList kRewardInfoList_ )
{
    // 세부 보상 정보
    // KRewardInfo
    // m_dwUID;            // UserUID
    // m_cCharType;        // 캐릭터 타입
    // m_setSelectIndex;   // 선택한 보상 번호
    // m_nRewardGP;        // 실제 획득 GP
    // m_vecRewardItem;    // 실제 획득 보상아이템
    // m_vecItemList;      // 상자 아이템 정보(정렬은 서버에서). vector< pair<보상타입,ItemID> >

    KRewardInfoList::iterator vit = kRewardInfoList_.begin();
    int iIndex = 1;
    for( ; vit != kRewardInfoList_.end(); ++vit )
    {
        for( int i = 0 ; i < MAX_PLAYER_NUM; ++i )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
            if( NULL != pPlayer && pPlayer->IsLive() && pPlayer->m_kUserInfo.dwUID == (*vit).m_dwUID )
            {
                if ( pPlayer->IsLocalPlayer() )
                {
                    m_pkBonusBoxRow[0]->SetSpecialBoxInfo( (*vit) );

					std::vector<KItem>::iterator vit2 = (*vit).m_vecRewardItem.begin();

                    for ( ; vit2 != (*vit).m_vecRewardItem.end(); ++ vit2 )
                    {
						int iReceiveCount = (*vit2).m_nCount;
						KItem* pkInvenItem = g_pItemMgr->m_kInventory.FindItemByItemUID( (*vit2).m_ItemUID );
						if( iReceiveCount != -1 && pkInvenItem != NULL )
						{
							iReceiveCount -= pkInvenItem->m_nCount;
						}
                        g_pItemMgr->m_kInventory.AddItem( (*vit2), true );
						g_pkUIScene->m_pkItemReceiveBox->AddItem( (*vit2).m_ItemUID, iReceiveCount );
                    }
                }
                else
                {
                    m_pkBonusBoxRow[iIndex++]->SetSpecialBoxInfo( (*vit) );
                }
            }
        }
    }

    for ( int i = iIndex; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        //m_pkBonusBoxRow[i]->ToggleRender( false );
    }

    SetAnimState( BBAS_BONUS_BOX_OPEN );
}

void KGCBonusBoxSelect_S4::TitleSetting( bool IsSpecial_ )
{
    m_pkGeneralTitle->ToggleRender( !IsSpecial_ );
    m_pkSpecialTitle->ToggleRender( IsSpecial_ );
    m_pkRewardTitle->ToggleRender( true );
    
    for ( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkBonusBoxRow[i]->SetBoxType( IsSpecial_ );
    }

    m_bSpecialReward = IsSpecial_;
}


void KGCBonusBoxSelect_S4::InitAnimation( void )
{
    m_EffectBoxTtitle.InitValue( m_pkSpecialTitle->GetFixedWindowLocalPos(), m_pkSpecialTitle->GetWidth(), m_pkSpecialTitle->GetHeight() );
    m_EffectBoxTtitle.SetFramePerAlterationScale( 20, 20, 2000, 100 );

    float fNewHalfWidth = static_cast<float>( m_pkSpecialTitle->GetWidth() * 10.0f ); // 2000은 100의 20배 // 반만필요하니까 2로 나눠서 10
    float fNewHalfHeight = static_cast<float>( m_pkSpecialTitle->GetHeight() * 10.0f );

    //m_vTileInitPos.x = m_pkSpecialTitle->GetFixedWindowLocalPos().x + static_cast< float >( m_pkSpecialTitle->GetWidth() / 2.0f )- fNewHalfWidth;
    //m_vTileInitPos.y =  m_pkSpecialTitle->GetFixedWindowLocalPos().y + static_cast< float >(m_pkSpecialTitle->GetHeight() / 2.0f )- fNewHalfHeight;
}

void KGCBonusBoxSelect_S4::SetEnd()
{
	switch(m_eAnimState)
	{
	case BBAS_SHOW_TITLE:
		m_EffectBoxTtitle.SetEnd();
		break;
	case BBAS_SHOW_BONUS_BOX:
		m_dwTimeCheck = GC_FPS_LIMIT;
		break;
	case BBAS_BEFORE_TIME_COUNT:	
		break;
	case BBAS_TIME_COUNT:
		m_dwTimeCheck = ( GC_FPS_LIMIT * ( SELECT_BONUS_BOX_TIME_LIMIT + 1 ) );
		break;
	case BBAS_BEFORE_BONUS_BOX_OPEN:
		break;
	case BBAS_BONUS_BOX_OPEN:
		if (m_bOpenItemImg)
			m_dwTimeCheck = ( GC_FPS_LIMIT * 3);
		else
			m_dwTimeCheck = 25;
		break;
	case BBAS_ALL_PROCESS_COMPLETE:
		m_dwTimeCheck = ( GC_FPS_LIMIT * 3);
		break;
	}
}
