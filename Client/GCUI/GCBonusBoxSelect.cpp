#include "stdafx.h"
#include "GCBonusBoxSelect.h"
#include "GCItemImgWnd.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCBonusBoxSelect );
IMPLEMENT_WND_FACTORY( KGCBonusBoxSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusBoxSelect, "gc_bonus_box_select" );

KGCBonusBoxSelect::KGCBonusBoxSelect( void )
: m_pkSelectBoxTitle( NULL )
, m_pkLimitTimeTitle( NULL )
, m_dwSelectLimitTime( SELECT_BONUS_BOX_TIME_LIMIT )
, m_bAlreadySelectMine( false )
, m_dwTimeCheck( 0 )
, m_bSendedOpenBoxPacket( false )
, m_eAnimState( BBAS_SHOW_TITLE )
, m_dwCurrentBonusBoxAnimIndex( 0 )
, m_dwCurrentNumberAnimIndex( 0 )
, m_dwCurrentNumberBackAnimIndex( 0 )
, m_fTimeParameter( 0.0f)
, m_bAllProcessComplete(+ false )
, m_bSendReadyPacket( false )
, m_fBonusBoxRotateZ( 0.0f )
, m_bSendStartBonusBoxSelect( false )
, m_pFrameMove( NULL )
, m_vTileInitPos( 0.0f, 0.0f )
{
    LINK_CONTROL( "select_box_title", m_pkSelectBoxTitle );
    LINK_CONTROL( "time_background", m_pkLimitTimeTitle );

    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkItemImg[i] = NULL;

        char strTemp[ MAX_PATH ] = "";
        sprintf_s( strTemp, MAX_PATH, "item_img_wnd%d", i );
        LINK_CONTROL( strTemp, m_pkItemImg[i] );

        m_pkBonusBox[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_box%d", i );
        LINK_CONTROL( strTemp, m_pkBonusBox[i] );

        m_pkBonusBoxOpened[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_box_opened%d", i );
        LINK_CONTROL( strTemp, m_pkBonusBoxOpened[i] );

        m_pkOwnerBackGround[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "owner_background%d", i );
        LINK_CONTROL( strTemp, m_pkOwnerBackGround[i] );

        m_pkStaticOwner[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "static_owner%d", i );
        LINK_CONTROL( strTemp, m_pkStaticOwner[i] );

        m_pkStaticBonusGP[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "static_bonus_gp%d", i );
        LINK_CONTROL( strTemp, m_pkStaticBonusGP[i] );

        m_pkStaticBonusItemName[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "static_bonus_item_name%d", i );
        LINK_CONTROL( strTemp, m_pkStaticBonusItemName[i] );

        m_pkItemImgBackground[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_img_background%d", i );
        LINK_CONTROL( strTemp, m_pkItemImgBackground[i] );
        
        m_pkBonusGpBackground[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_gp_background%d", i );
        LINK_CONTROL( strTemp, m_pkBonusGpBackground[i] );
       
        m_pkItemNameBackground[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_name_background%d", i );
        LINK_CONTROL( strTemp, m_pkItemNameBackground[i] );

        m_pkBoxNumber[i] = NULL;
        sprintf_s(strTemp, MAX_PATH, "box_number%d", i);
        LINK_CONTROL( strTemp, m_pkBoxNumber[i]);

        m_pkBoxNumberBack[i] = NULL;
        sprintf_s(strTemp, MAX_PATH, "box_number_back%d", i);
        LINK_CONTROL( strTemp, m_pkBoxNumberBack[i]);

        m_pkSelectBoxComment = NULL;
        LINK_CONTROL( "select_box_comment", m_pkSelectBoxComment);

        m_bSelected[i] = false;
        m_dwUserUID[i] = 0;
    }

    for( int i = 0 ; i <  BBAS_MAX; ++i )
    {
        m_pInitAnimFunc[i] = NULL;
        m_pFrameMoveAnimFunc[i] = NULL;
    }

    m_pInitAnimFunc[ BBAS_SHOW_TITLE ] = &KGCBonusBoxSelect::InitShowTitle;
    m_pInitAnimFunc[ BBAS_SHOW_BONUS_BOX ] = &KGCBonusBoxSelect::InitShowBonusBox;
    m_pInitAnimFunc[ BBAS_BEFORE_TIME_COUNT ] = &KGCBonusBoxSelect::InitBeforeTimeCount;
    m_pInitAnimFunc[ BBAS_TIME_COUNT ] = &KGCBonusBoxSelect::InitTimeCount;
    m_pInitAnimFunc[ BBAS_BEFORE_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect::InitBeforeBonusBoxOpen;
    m_pInitAnimFunc[ BBAS_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect::InitBonusBoxOpen;
    m_pInitAnimFunc[ BBAS_ALL_PROCESS_COMPLETE ] = &KGCBonusBoxSelect::InitAllProcessComplete;

    m_pFrameMoveAnimFunc[ BBAS_SHOW_TITLE ] = &KGCBonusBoxSelect::FrameMoveShowTitle;
    m_pFrameMoveAnimFunc[ BBAS_SHOW_BONUS_BOX ] = &KGCBonusBoxSelect::FrameMoveShowBonusBox;
    m_pFrameMoveAnimFunc[ BBAS_BEFORE_TIME_COUNT ] = &KGCBonusBoxSelect::FrameMoveBeforeTimeCount;
    m_pFrameMoveAnimFunc[ BBAS_TIME_COUNT ] = &KGCBonusBoxSelect::FrameMoveTimeCount;
    m_pFrameMoveAnimFunc[ BBAS_BEFORE_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect::FrameMoveBeforeBonusBoxOpen;
    m_pFrameMoveAnimFunc[ BBAS_BONUS_BOX_OPEN ] = &KGCBonusBoxSelect::FrameMoveBonusBoxOpen;
    m_pFrameMoveAnimFunc[ BBAS_ALL_PROCESS_COMPLETE ] = &KGCBonusBoxSelect::FrameMoveAllProcessComplete;
}

KGCBonusBoxSelect::~KGCBonusBoxSelect( void )
{
}

void KGCBonusBoxSelect::OnCreate( void )
{
    if( IsCreate() == false )
    {
        for(int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
        {
            m_pkBonusBox[i]->InitState( false, false, this );
            m_pkStaticOwner[i]->SetAlign( DT_CENTER );
            m_pkStaticBonusGP[i]->SetAlign( DT_CENTER );
            m_pkStaticBonusItemName[i]->SetAlign( DT_CENTER );

            m_pkBonusBox[i]->SetHotKey( DIK_1 + (0x01 * static_cast<BYTE>(i)) );    // 숫자키로 보상 상자 선택할 수 있게 
            m_pkBoxNumber[i]->InitState(false, false, this);
            m_pkBoxNumberBack[i]->InitState(false, false, this);
        }

        m_pkSelectBoxComment->InitState(false, false, this);

        InitAnimation();
    }
}

void KGCBonusBoxSelect::OnCreateComplete( void )
{
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_bSelected[i] = false;
        m_dwUserUID[i] = 0;

        m_pkOwnerBackGround[i]->ToggleRender(false );
        m_pkStaticOwner[i]->ToggleRender( false );
    }

    m_setReadyUserUID.clear();
    m_vecBoxSelectInfo.clear();
    m_bAllProcessComplete = false;
    SetAnimState( BBAS_SHOW_TITLE );
}

void KGCBonusBoxSelect::FrameMoveInEnabledState( void )
{
    if( m_pFrameMove )
        (this->*m_pFrameMove)();
}

void KGCBonusBoxSelect::PostChildDraw( void )
{
    if( BBAS_TIME_COUNT == m_eAnimState )
    {
        D3DXVECTOR2 vPos = GetRelocatedWindowPos();
        g_pkUIMgr->RenderNumber("white_num", D3DXVECTOR2( vPos.x + (385.0f * m_fWindowScaleX ), vPos.y + ( 90.0f * m_fWindowScaleY ) ), m_dwSelectLimitTime , D3DCOLOR_ARGB( 0xff,0xff,0xff,0xff), true, true, -1.0f,true );
    }
}

void KGCBonusBoxSelect::ActionPerformed( const KActionEvent& event )
{

    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        if( event.m_pWnd  == m_pkBonusBox[i] && D3DWE_BUTTON_CLICK == event.m_dwCode)
        {
            ClickBonusBox( i );
            return;
        }
    }
}

void KGCBonusBoxSelect::ClickBonusBox( int iSelectIndex_ )
{
    if( iSelectIndex_ < 0 || iSelectIndex_ >= OLD_MAX_DUNGEON_PLAYER_NUM )
        return;

    if( BBAS_TIME_COUNT != m_eAnimState || m_bSelected[iSelectIndex_] || m_bAlreadySelectMine )
        return;

    g_kSelectBonusBox->m_dwUserUID = g_kGlobalValue.m_kUserInfo.dwUID;
    g_kSelectBonusBox->m_ucSelectIndex = iSelectIndex_;
    g_kSelectBonusBox->Type = GC_PID_SELECT_BONUSBOX;
    SendP2PPacket( g_kSelectBonusBox, sizeof( KGC_PID_SELECT_BONUSBOX), KNC_BROADCAST, _RELIABLE);
    KGCPC_MEMO( "GC_PID_SELECT_BONUSBOX" );
}

bool KGCBonusBoxSelect::SelectBonusBox( int iSelectIndex_, DWORD dwUserUID_ )
{
    if( false == CheckEnableSelect( iSelectIndex_, dwUserUID_) )
        return false;

    m_dwUserUID[ iSelectIndex_] = dwUserUID_;
    m_bSelected[iSelectIndex_] = true;

    m_pkBonusBox[iSelectIndex_]->SetWndMode( D3DWM_DEFAULT );
    m_pkBonusBox[iSelectIndex_]->SetSelfInputCheck( false );
    m_pkOwnerBackGround[iSelectIndex_]->ToggleRender( true );

    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUserUID_ );
    if( NULL != pPlayer )
    {
        m_pkStaticOwner[iSelectIndex_]->SetText( L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx" );
        m_pkStaticOwner[iSelectIndex_]->ToggleRender( true );
    }

    if( dwUserUID_ == g_kGlobalValue.m_kUserInfo.dwUID )
    {
        m_bAlreadySelectMine = true;
        for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
        {
            m_pkBonusBox[i]->SetSelfInputCheck( false );
        }
    }

    m_vecBoxSelectInfo.push_back( std::make_pair( iSelectIndex_, dwUserUID_) );
    return true;
}

void KGCBonusBoxSelect::OpenBonusBox()
{
    std::vector< std::pair< int, DWORD> >::iterator vitBonusBox = m_vecBoxSelectInfo.begin();

    for( ; vitBonusBox != m_vecBoxSelectInfo.end(); ++vitBonusBox )
    {
        int iSelectIndex = (*vitBonusBox).first;
        if( iSelectIndex < 0 || iSelectIndex >= OLD_MAX_DUNGEON_PLAYER_NUM )
            continue;

        DWORD dwUserUID = (*vitBonusBox).second;
        //SelectBonusBox( iSelectIndex, dwUserUID );

        m_pkBonusBoxOpened[iSelectIndex]->ToggleRender( true );
                
        std::vector< KBonusBox >::iterator vitBonusBox = std::find_if( m_vecBonusBoxInfo.begin(), m_vecBonusBoxInfo.end(), boost::bind( &KBonusBox::m_dwUID, _1) == dwUserUID );

        if( m_vecBonusBoxInfo.end() != vitBonusBox)
        {
            GCITEMID ItemID = (*vitBonusBox).m_ItemID;
            GCItem* pItem = g_pItemMgr->GetItemData( ItemID );

            if( NULL != pItem && ItemID != 0 )
            {
                m_pkStaticBonusItemName[iSelectIndex]->SetText( pItem->strItemName );
                m_pkStaticBonusItemName[iSelectIndex]->ToggleRender( true );
                m_pkItemImg[ iSelectIndex ]->SetItemInfo( ItemID);
                m_pkItemImg[ iSelectIndex ]->ToggleRender( true );
                m_pkItemImgBackground[iSelectIndex]->ToggleRender( true );
                m_pkItemNameBackground[iSelectIndex]->ToggleRender( true );
            }

            if( 0 != (*vitBonusBox).m_nBonusGP )
            {
                m_pkStaticBonusGP[iSelectIndex]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BONUS_GP_STRING, "i", (*vitBonusBox).m_nBonusGP) );
                m_pkStaticBonusGP[iSelectIndex]->ToggleRender(true );
                m_pkBonusGpBackground[iSelectIndex]->ToggleRender( true );
            }
        }
    }
}

bool KGCBonusBoxSelect::CheckEnableSelect( int iSelectIndex_, DWORD dwUserUID_)
{
    if( iSelectIndex_ < 0 || iSelectIndex_ >= OLD_MAX_DUNGEON_PLAYER_NUM )
        return false;

    if( m_bSelected[iSelectIndex_])
        return false;

    for( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i)
    {
        if( dwUserUID_ == m_dwUserUID[i] )
            return false;
    }

    return true;
}


int KGCBonusBoxSelect::GetSelectedIndexByUserUID( DWORD dwUserUID_ )
{
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        if( dwUserUID_ == m_dwUserUID[i] )
            return i;
    }
    return -1;
}


int KGCBonusBoxSelect::GetRandomIndex()
{
    for(int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        if( false == m_bSelected[i] )
            return i;
    }
    return -1;
}


void KGCBonusBoxSelect::MakeSelectBoxInfo()
{
    for( int i = 0 ; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->IsLive() )
        {
            int iExtraIndex = -1;
            if( -1 == GetSelectedIndexByUserUID( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) &&
                -1 != ( iExtraIndex = GetRandomIndex() ) )
            {
                m_bSelected[ iExtraIndex ] = true;
                m_dwUserUID[ iExtraIndex ] = g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID;
            }
        }
    }
}

void KGCBonusBoxSelect::SendSelectBoxInfo()
{
    RandomSelect();
    g_kOpenBonusBox->Type = GC_PID_OPEN_BONUSBOX;
    g_kOpenBonusBox->m_vecBonusBoxSelectInfo = m_vecBoxSelectInfo;
    SERIALIZE_P2P_PACKET( (*g_kOpenBonusBox) );
	SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE );
	KGCPC_MEMO( "GC_PID_OPEN_BONUSBOX" );
}

void KGCBonusBoxSelect::SetBonusBoxList( std::vector< KBonusBox > vecBonusBoxInfo_ )
{
    m_vecBonusBoxInfo.swap( vecBonusBoxInfo_);
}

bool KGCBonusBoxSelect::CheckAllUserSelect()
{
    for(int i = 0 ; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->IsLive() )
        {
            if( -1 == GetSelectedIndexByUserUID( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) )
                return false;
        }
    }
    return true;
}

void KGCBonusBoxSelect::InitAnimation()
{
    m_EffectSelectBoxTtitle.InitValue( m_pkSelectBoxTitle->GetFixedWindowLocalPos(), m_pkSelectBoxTitle->GetWidth(), m_pkSelectBoxTitle->GetHeight() );
    m_EffectSelectBoxTtitle.SetFramePerAlterationScale( 20, 20, 2000, 100 );

    float fNewHalfWidth = static_cast<float>( m_pkSelectBoxTitle->GetWidth() * 10.0f ); // 2000은 100의 20배 // 반만필요하니까 2로 나눠서 10
    float fNewHalfHeight = static_cast<float>( m_pkSelectBoxTitle->GetHeight() * 10.0f );

    m_vTileInitPos.x = m_pkSelectBoxTitle->GetFixedWindowLocalPos().x + static_cast< float >( m_pkSelectBoxTitle->GetWidth() / 2.0f )- fNewHalfWidth;
    m_vTileInitPos.y =  m_pkSelectBoxTitle->GetFixedWindowLocalPos().y + static_cast< float >(m_pkSelectBoxTitle->GetHeight() / 2.0f )- fNewHalfHeight;

    m_EffectLimitTimeTitle.InitValue( m_pkLimitTimeTitle->GetFixedWindowLocalPos(), m_pkLimitTimeTitle->GetWidth(), m_pkLimitTimeTitle->GetHeight() );
    m_EffectLimitTimeTitle.SetFramePerAlterationPos(15, 15, D3DXVECTOR2( -500.0f, 161.0f), m_pkLimitTimeTitle->GetFixedWindowLocalPos() );

    m_EffectSelectCommentTitle.InitValue( m_pkSelectBoxComment->GetFixedWindowLocalPos(), m_pkSelectBoxComment->GetWidth(), m_pkSelectBoxComment->GetHeight() );
    m_EffectSelectCommentTitle.SetFramePerAlterationPos(15, 15, D3DXVECTOR2( -500.0f, 187.0f), m_pkSelectBoxComment->GetFixedWindowLocalPos() );

    for(int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_EffectBonusBox[i].InitValue( m_pkBonusBox[i]->GetFixedWindowLocalPos() , m_pkBonusBox[i]->GetWidth(), m_pkBonusBox[i]->GetHeight() );
        m_EffectBonusBox[i].SetFramePerAlterationScale( 10, 10, 1000, 100 );

        m_EffectBoxNumber[i].InitValue( m_pkBoxNumber[i]->GetFixedWindowLocalPos() , m_pkBoxNumber[i]->GetWidth(), m_pkBoxNumber[i]->GetHeight() );
        m_EffectBoxNumber[i].SetFramePerAlterationScale( 10, 10, 1000, 100 );

        m_EffectBoxNumberBack[i].InitValue( m_pkBoxNumberBack[i]->GetFixedWindowLocalPos() , m_pkBoxNumberBack[i]->GetWidth(), m_pkBoxNumberBack[i]->GetHeight() );
        m_EffectBoxNumberBack[i].SetFramePerAlterationScale( 10, 10, 1000, 100 );

//         float fBonusBoxNewHalfWidth = static_cast<float>( m_pkBonusBox[i]->GetWidth() * 5.0f ); // 1000은 100의 10배 // 반만필요하니까 2로 나눠서 5
//         float fBonusBoxNewHalfHeight = static_cast<float>( m_pkBonusBox[i]->GetHeight() * 5.0f );
// 
//         D3DXVECTOR2 vBonusBoxInitPos;
//         vBonusBoxInitPos.x = m_pkBonusBox[i]->GetFixedWindowLocalPos().x + static_cast< float >( m_pkBonusBox[i]->GetWidth() / 2.0f )- fBonusBoxNewHalfWidth;
//         vBonusBoxInitPos.y =  m_pkBonusBox[i]->GetFixedWindowLocalPos().y + static_cast< float >(m_pkBonusBox[i]->GetHeight() / 2.0f )- fBonusBoxNewHalfHeight;
//         m_pkBonusBox[i]->SetWindowPosDirect( vBonusBoxInitPos );
    }
}

bool KGCBonusBoxSelect::CheckAllUserReady()
{
    for(int i = 0 ; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->IsLive() )
        {
            if( m_setReadyUserUID.end() == m_setReadyUserUID.find( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) )
                return false;
        }
    }
    return true;
}

void KGCBonusBoxSelect::AddReadyUser( DWORD dwUserUid_)
{
    m_setReadyUserUID.insert( dwUserUid_ );
}

void KGCBonusBoxSelect::OpenBonusBoxEffect()
{
    std::vector< std::pair< int, DWORD> >::iterator vitBonusBox = m_vecBoxSelectInfo.begin();

    for( ; vitBonusBox != m_vecBoxSelectInfo.end(); ++vitBonusBox )
    {
        int iSelectIndex = (*vitBonusBox).first;
        if( iSelectIndex < 0 || iSelectIndex >= OLD_MAX_DUNGEON_PLAYER_NUM )
            continue;

        float fOffsetX = static_cast<float>( m_pkBonusBoxOpened[iSelectIndex]->GetWidth() / 2.0f );
        float fOffsetY = static_cast<float>( m_pkBonusBoxOpened[iSelectIndex]->GetHeight() / 2.0f );
        GCFUNC::CreateSequenceToUI( m_pkBonusBoxOpened[iSelectIndex], "Box_Open_01", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBoxOpened[iSelectIndex], "Box_Open_02", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBoxOpened[iSelectIndex], "Box_Open_03", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBoxOpened[iSelectIndex], "Box_Open_04", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBoxOpened[iSelectIndex], "Box_Open_05", fOffsetX, fOffsetY );
    }
}

void KGCBonusBoxSelect::RandomSelect()
{
    for( int i = 0 ; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->IsLive() )
        {
            int iSelectedIndex = GetSelectedIndexByUserUID( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
            if( -1 == iSelectedIndex)
                iSelectedIndex = GetRandomIndex();

            SelectBonusBox( iSelectedIndex, g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        }
    }
}

void KGCBonusBoxSelect::SetAnimState( BONUS_BOX_ANIM_STATE eAnimState_ )
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

void KGCBonusBoxSelect::InitShowTitle()
{
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
//        m_pkStaticOwner[i]->ToggleRender( false );
//        m_pkOwnerBackGround[i]->ToggleRender( false );
        m_pkBonusBox[i]->ToggleRender( false );
        m_pkBonusBox[i]->SetSelfInputCheck( false );
        m_pkItemImg[i]->ToggleRender( false );
        m_pkStaticBonusGP[i]->ToggleRender( false );
        m_pkStaticBonusItemName[i]->ToggleRender( false );
        m_pkItemImgBackground[i]->ToggleRender( false );
        m_pkBonusGpBackground[i]->ToggleRender( false );
        m_pkItemNameBackground[i]->ToggleRender( false );
        m_pkBonusBoxOpened[i]->ToggleRender(false );

        m_pkBoxNumber[i]->ToggleRender(false);
        m_pkBoxNumberBack[i]->ToggleRender(false);
    }

    m_pkLimitTimeTitle->ToggleRender( false );
    m_pkSelectBoxComment->ToggleRender( false );

    m_EffectSelectBoxTtitle.Reset();
    m_pkSelectBoxTitle->SetWindowPosDirect( m_vTileInitPos );
}

void KGCBonusBoxSelect::InitShowBonusBox()
{
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_EffectBonusBox[i].Reset();
        m_EffectBoxNumber[i].Reset();
        m_EffectBoxNumberBack[i].Reset();
    }

    m_pkSelectBoxTitle->SetWidth( m_EffectSelectBoxTtitle.GetOriginalWidth() );
    m_pkSelectBoxTitle->SetHeight( m_EffectSelectBoxTtitle.GetOriginalHeight() );
    m_pkSelectBoxTitle->SetWindowPosDirect( m_EffectSelectBoxTtitle.GetOriginalPos() );

    m_dwCurrentBonusBoxAnimIndex = 0;
    m_dwCurrentNumberAnimIndex = 0;
    m_dwCurrentNumberBackAnimIndex = 0;

}

void KGCBonusBoxSelect::InitBeforeTimeCount()
{
    for( int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i)
    {
        m_pkBonusBox[i]->ToggleRender( true );
        m_pkBonusBox[i]->SetWidth( m_EffectBonusBox[i].GetOriginalWidth() );
        m_pkBonusBox[i]->SetHeight( m_EffectBonusBox[i].GetOriginalHeight() );
        m_pkBonusBox[i]->SetWindowPosDirect( m_EffectBonusBox[i].GetOriginalPos() );

        m_pkBoxNumber[i]->ToggleRender(true);
        m_pkBoxNumber[i]->SetWidth( m_EffectBoxNumber[i].GetOriginalWidth() );
        m_pkBoxNumber[i]->SetHeight( m_EffectBoxNumber[i].GetOriginalHeight() );
        m_pkBoxNumber[i]->SetWindowPosDirect( m_EffectBoxNumber[i].GetOriginalPos() );

        m_pkBoxNumberBack[i]->ToggleRender(true);
        m_pkBoxNumberBack[i]->SetWidth( m_EffectBoxNumberBack[i].GetOriginalWidth() );
        m_pkBoxNumberBack[i]->SetHeight( m_EffectBoxNumberBack[i].GetOriginalHeight() );
        m_pkBoxNumberBack[i]->SetWindowPosDirect( m_EffectBoxNumberBack[i].GetOriginalPos() );
    }

    m_EffectLimitTimeTitle.Reset();
    m_EffectSelectCommentTitle.Reset();

    m_pkLimitTimeTitle->SetWindowPosDirect( D3DXVECTOR2( -500.0f, 161.0f ) );
    m_pkLimitTimeTitle->ToggleRender( true );

    m_pkSelectBoxComment->SetWindowPosDirect( D3DXVECTOR2( -500.0f, 187.0f ) );
    m_pkSelectBoxComment->ToggleRender( true );

    m_bSendReadyPacket = false;
    m_bSendStartBonusBoxSelect = false;
}

void KGCBonusBoxSelect::InitTimeCount()
{
    m_pkLimitTimeTitle->SetWidth( m_EffectLimitTimeTitle.GetOriginalWidth() );
    m_pkLimitTimeTitle->SetHeight( m_EffectLimitTimeTitle.GetOriginalHeight());
    m_pkLimitTimeTitle->SetWindowPosDirect( m_EffectLimitTimeTitle.GetOriginalPos() );

    m_pkSelectBoxComment->SetWidth( m_EffectSelectCommentTitle.GetOriginalWidth() );
    m_pkSelectBoxComment->SetHeight( m_EffectSelectCommentTitle.GetOriginalHeight());
    m_pkSelectBoxComment->SetWindowPosDirect( m_EffectSelectCommentTitle.GetOriginalPos() );

    m_dwSelectLimitTime = SELECT_BONUS_BOX_TIME_LIMIT;
    m_bAlreadySelectMine = false;

    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkBonusBox[i]->SetSelfInputCheck( true );
    }
}

void KGCBonusBoxSelect::InitBeforeBonusBoxOpen()
{
    for( int i = 0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        m_pkBonusBox[i]->SetSelfInputCheck( false);
        m_pkBonusBox[i]->SetWndMode( D3DWM_DEFAULT );
    }

    m_bSendedOpenBoxPacket = false;
}

void KGCBonusBoxSelect::InitBonusBoxOpen()
{
    m_fTimeParameter = 0.0f;
    m_fBonusBoxRotateZ = 0.0f;
}

void KGCBonusBoxSelect::InitAllProcessComplete()
{
    OpenBonusBox();
    //for(int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i)
    //{
    //    m_pkBonusBox[i]->SetRotateValue( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
    //}
}

void KGCBonusBoxSelect::FrameMoveShowTitle()
{
    if( !m_EffectSelectBoxTtitle.IsEndOfScale() )
    {
        DWORD dwPreWidth = m_EffectSelectBoxTtitle.GetCurrentWidth();
        DWORD dwPreHeight = m_EffectSelectBoxTtitle.GetCurrentHeight();
        m_EffectSelectBoxTtitle.ScaleStep();

        DWORD dwCurrentWidth = m_EffectSelectBoxTtitle.GetCurrentWidth();
        DWORD dwCurrentHeight = m_EffectSelectBoxTtitle.GetCurrentHeight();


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

        m_pkSelectBoxTitle->SetWidth( dwCurrentWidth );
        m_pkSelectBoxTitle->SetHeight( dwCurrentHeight );

        m_pkSelectBoxTitle->SetWindowPosDirect( m_pkSelectBoxTitle->GetFixedWindowLocalPos() + vMovePos );
        if( m_EffectSelectBoxTtitle.IsEndOfScale() )
        {
            g_KDSound.Play( "136" );
        }
    }

    m_EffectSelectBoxTtitle.ShowStep();
    //다음으로 넘어가자
    if( m_EffectSelectBoxTtitle.IsEndOfShow() )
    {
        SetAnimState( BBAS_SHOW_BONUS_BOX );
    }
}

void KGCBonusBoxSelect::FrameMoveShowBonusBox()
{
    m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].ShowStep();
    m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].ShowStep();
    m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].ShowStep();

    if( false == m_pkBonusBox[m_dwCurrentBonusBoxAnimIndex]->IsRenderOn() )
        m_pkBonusBox[m_dwCurrentBonusBoxAnimIndex]->ToggleRender( true );

    if( false == m_pkBoxNumber[m_dwCurrentNumberAnimIndex]->IsRenderOn() )
        m_pkBoxNumber[m_dwCurrentNumberAnimIndex]->ToggleRender( true );

    if( false == m_pkBoxNumberBack[m_dwCurrentNumberBackAnimIndex]->IsRenderOn() )
        m_pkBoxNumberBack[m_dwCurrentNumberBackAnimIndex]->ToggleRender( true );

    if( !m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].IsEndOfScale() )
    {
        m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].ScaleStep();

        DWORD dwCurrentWidth = m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].GetCurrentWidth();
        DWORD dwCurrentHeight = m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].GetCurrentHeight();

        m_pkBonusBox[m_dwCurrentBonusBoxAnimIndex]->SetWidthDirect( dwCurrentWidth );
        m_pkBonusBox[m_dwCurrentBonusBoxAnimIndex]->SetHeightDirect( dwCurrentHeight );

        if( m_EffectBonusBox[m_dwCurrentBonusBoxAnimIndex].IsEndOfScale() )
        {
            ++m_dwCurrentBonusBoxAnimIndex; 
            g_KDSound.Play( "136" );
        }
    }

    if( !m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].IsEndOfScale() )
    {
        m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].ScaleStep();

        DWORD dwNumWidth = m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].GetCurrentWidth();
        DWORD dwNumHeight = m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].GetCurrentHeight();

        m_pkBoxNumber[m_dwCurrentNumberAnimIndex]->SetWidthDirect( dwNumWidth );
        m_pkBoxNumber[m_dwCurrentNumberAnimIndex]->SetHeightDirect( dwNumHeight );
    }

    if( !m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].IsEndOfScale() )
    {
        m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].ScaleStep();

        DWORD dwNumBackWidth = m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].GetCurrentWidth();
        DWORD dwNumBackHeight = m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].GetCurrentHeight();

        m_pkBoxNumberBack[m_dwCurrentNumberBackAnimIndex]->SetWidthDirect( dwNumBackWidth );
        m_pkBoxNumberBack[m_dwCurrentNumberBackAnimIndex]->SetHeightDirect( dwNumBackHeight );
    }

    if( m_EffectBoxNumber[m_dwCurrentNumberAnimIndex].IsEndOfScale() )
        ++m_dwCurrentNumberAnimIndex;
    if( m_EffectBoxNumberBack[m_dwCurrentNumberBackAnimIndex].IsEndOfScale() )
        ++m_dwCurrentNumberBackAnimIndex;

    if( m_dwCurrentBonusBoxAnimIndex >= OLD_MAX_DUNGEON_PLAYER_NUM )
    {
        SetAnimState( BBAS_BEFORE_TIME_COUNT );
    }
}

void KGCBonusBoxSelect::FrameMoveBeforeTimeCount()
{
    if( !m_EffectLimitTimeTitle.IsEndOfPos() )
    {
        m_EffectLimitTimeTitle.PosStep();
        m_pkLimitTimeTitle->SetWindowPosDirect( m_EffectLimitTimeTitle.GetCurrentPos());

        if( m_EffectLimitTimeTitle.IsEndOfPos() )
        {
            g_KDSound.Play( "136" );
        }
    }

    m_EffectLimitTimeTitle.ShowStep();

    if( m_EffectLimitTimeTitle.IsEndOfShow())
    {
        if( 1 ==  g_MyD3D->GetPlayerNum() )
        {
            SetAnimState( BBAS_TIME_COUNT );
        }
        else
        {
            if( false == m_bSendReadyPacket )
            {
                g_kReadyBonusBoxSelect->Type = GC_PID_READY_BONUSBOX_SELECT;
				SendP2PPacket( g_kReadyBonusBoxSelect, sizeof( KGC_PID_READY_BONUSBOX_SELECT), KNC_BROADCAST, _RELIABLE );
				KGCPC_MEMO( "GC_PID_READY_BONUSBOX_SELECT" );
                m_bSendReadyPacket = true;
            }

            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
                if( ( ++m_dwTimeCheck >= GC_FPS_LIMIT * 5 ) || CheckAllUserReady() )
                {
                    if( false == m_bSendStartBonusBoxSelect )
                    {
                        g_kReadyBonusBoxSelect->Type = GC_PID_START_BONUSBOX_SELECT;
						SendP2PPacket( g_kReadyBonusBoxSelect, sizeof( KGC_PID_START_BONUSBOX_SELECT), KNC_BROADCAST, _RELIABLE );
						KGCPC_MEMO( "GC_PID_START_BONUSBOX_SELECT" );
                        m_bSendStartBonusBoxSelect = true;
                    }
                }
            }
        }
    }
}

void KGCBonusBoxSelect::FrameMoveTimeCount()
{
    if( m_dwSelectLimitTime <= 0 || CheckAllUserSelect() )
    {
        SetAnimState( BBAS_BEFORE_BONUS_BOX_OPEN );
        return;
    }

    if(++m_dwTimeCheck >= GC_FPS_LIMIT )
    {
        m_dwTimeCheck = 0;

        if( 0 < m_dwSelectLimitTime )
            --m_dwSelectLimitTime;
    }
}

void KGCBonusBoxSelect::FrameMoveBeforeBonusBoxOpen()
{
    if( false == m_bSendedOpenBoxPacket && 
        g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( 1 == g_MyD3D->GetPlayerNum() )
        {
            RandomSelect();
            m_bSendedOpenBoxPacket = true;
            SetAnimState( BBAS_BONUS_BOX_OPEN );
        }
        else
        {
            if( ++m_dwTimeCheck >= ( GC_FPS_LIMIT * 3 ) || CheckAllUserSelect() )
            {
                SendSelectBoxInfo();
                m_bSendedOpenBoxPacket = true;
            }
        }
    }
}

void KGCBonusBoxSelect::FrameMoveBonusBoxOpen()
{
    if( 2.0f * D3DX_PI <= m_fBonusBoxRotateZ )
    {
        if( 0.0f == m_fTimeParameter )
            g_KDSound.Play( "991028" );

        m_fBonusBoxRotateZ = 0.0f;
        m_fTimeParameter += 0.01f;
        if( m_fTimeParameter >= 0.12f)
            m_fTimeParameter = 0.12f;
    }

    m_fBonusBoxRotateZ += 0.7f;
    if( m_fBonusBoxRotateZ >= 2.0f * D3DX_PI )
        m_fBonusBoxRotateZ = 2.0f * D3DX_PI;

    D3DXVECTOR3 vRotate(0.0f, 0.0f, m_fTimeParameter * sinf(m_fBonusBoxRotateZ ) );

 /*   for(int i = 0; i < OLD_MAX_DUNGEON_PLAYER_NUM; ++i)
    {
        m_pkBonusBox[i]->SetRotateValue( vRotate );
    }*/

    if( 0.12f <= m_fTimeParameter)
    {
        if( 0 == m_dwTimeCheck )
        {
            OpenBonusBoxEffect();
        }
        else if(m_dwTimeCheck >= 25)
        {
            SetAnimState( BBAS_ALL_PROCESS_COMPLETE );
        }

        ++m_dwTimeCheck;
    }
}

void KGCBonusBoxSelect::FrameMoveAllProcessComplete()
{
    for(int i=0 ; i < OLD_MAX_DUNGEON_PLAYER_NUM ; ++i )
    {  
        m_pkBoxNumber[i]->ToggleRender(false);
        m_pkBoxNumberBack[i]->ToggleRender(false);
    }

    if(++m_dwTimeCheck >= ( GC_FPS_LIMIT * 5) )
    {
        m_bAllProcessComplete = true;
    }
}
