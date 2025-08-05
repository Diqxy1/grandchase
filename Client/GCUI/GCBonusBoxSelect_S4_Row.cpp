#include "stdafx.h"
#include "GCBonusBoxSelect_S4_Row.h"
#include "GCItemImgWnd.h"
#include "KGCNumberControl.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCBonusBoxSelect_S4_Row );
IMPLEMENT_WND_FACTORY( KGCBonusBoxSelect_S4_Row );
IMPLEMENT_WND_FACTORY_NAME( KGCBonusBoxSelect_S4_Row, "gc_bonus_box_select_s4_row" );

KGCBonusBoxSelect_S4_Row::KGCBonusBoxSelect_S4_Row( void )
: m_pkStaticOwner( NULL )
, m_pk10PosNum( NULL )
, m_pk1PosNum( NULL )
, m_iKeyNum( 0 )
, m_bSelected( false )
, m_dwUID( 0 )
, m_bDisableClick( false )
, m_bSpecial( false )
, m_bMine( false )
{
    LINK_CONTROL( "static_owner", m_pkStaticOwner );
    LINK_CONTROL( "pos10_num", m_pk10PosNum );
    LINK_CONTROL( "pos1_num", m_pk1PosNum );

    for( int i = 0 ; i < BOX_NUM; ++i )
    {
        m_bSelectedIndex[i] = false;

        char strTemp[ MAX_PATH ] = "";

        m_pkItemImg[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_img_wnd%d", i );
        LINK_CONTROL( strTemp, m_pkItemImg[i] );

        m_pkBonusBox[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_box%d", i );
        LINK_CONTROL( strTemp, m_pkBonusBox[i] );

        m_pkBonusSPBox[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bonus_box%d_sp", i );
        LINK_CONTROL( strTemp, m_pkBonusSPBox[i] );

        m_pkBackgroundBox[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_img_bg%d", i );
        LINK_CONTROL( strTemp, m_pkBackgroundBox[i] );

        m_pkItemImgBackground[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_img_background%d", i );
        LINK_CONTROL( strTemp, m_pkItemImgBackground[i] );

        m_pkBackgroundSpliter[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "bg_line%d", i );
        LINK_CONTROL( strTemp, m_pkBackgroundSpliter[i] );

        m_pkStaticItemName[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "static_bonus_item_name%d", i );
        LINK_CONTROL( strTemp, m_pkStaticItemName[i] );

        m_pkBoxNumber[i] = NULL;
        sprintf_s(strTemp, MAX_PATH, "box_number%d", i);
        LINK_CONTROL( strTemp, m_pkBoxNumber[i]);

        m_pkBoxNumberBack[i] = NULL;
        sprintf_s(strTemp, MAX_PATH, "box_number_bg%d", i);
        LINK_CONTROL( strTemp, m_pkBoxNumberBack[i]);

        for ( char j = 0; j < GC_CHAR_NUM; ++j )
        {
            m_pkMyCharFace[j] = NULL;
            sprintf_s(strTemp, MAX_PATH, "headup_display_char%d", j);
            LINK_CONTROL( strTemp, m_pkMyCharFace[j]);
        }
    }
}

KGCBonusBoxSelect_S4_Row::~KGCBonusBoxSelect_S4_Row( void )
{
}

void KGCBonusBoxSelect_S4_Row::OnCreate( void )
{
    m_bSelected = false;
    m_bDisableClick = false;
    m_bSpecial = false;
    m_dwUID = 0;
    for( int i = 0 ; i < BOX_NUM; ++i )
    {
        m_bSelectedIndex[i] = false;
    }

    m_pkStaticOwner->SetAlign( DT_CENTER );

    for(int i = 0 ; i < BOX_NUM; ++i )
    {
        m_pkItemImgBackground[i]->InitState( true, false, NULL );
        m_pkBonusBox[i]->InitState( false, false, NULL );
        m_pkBonusSPBox[i]->InitState( false, false, NULL );
        m_pkStaticItemName[i]->SetAlign( DT_CENTER );
        m_pkItemImg[i]->ToggleRender( true );
		m_pkItemImg[i]->SetItemInfo(0);
#ifdef USE_JOYSTICK
        m_pkItemImgBackground[i]->SetHotJoystickKey( EJK_KEY_1 + i );    // 조이스틱키로 보상 상자 선택할 수 있게 
#endif

        m_pkItemImgBackground[i]->SetHotKey( DIK_1 + (0x01 * static_cast<BYTE>(i)) );    // 숫자키로 보상 상자 선택할 수 있게 
        m_pkBoxNumber[i]->InitState(false, false, this);
        m_pkBoxNumberBack[i]->InitState(false, false, this);
    }
}

void KGCBonusBoxSelect_S4_Row::OnCreateComplete( void )
{
    m_bSelected = false;
    m_bDisableClick = false;
    m_bSpecial = false;
    m_dwUID = 0;
    for( int i = 0 ; i < BOX_NUM; ++i )
    {
        m_bSelectedIndex[i] = false;
    }

    m_pkStaticOwner->SetAlign( DT_CENTER );

    for(int i = 0 ; i < BOX_NUM; ++i )
    {
        m_pkItemImgBackground[i]->InitState( true, false, NULL );
        m_pkBonusBox[i]->InitState( false, false, NULL );
        m_pkBonusSPBox[i]->InitState( false, false, NULL );
        m_pkStaticItemName[i]->SetAlign( DT_CENTER );
        m_pkItemImg[i]->ToggleRender( true );
#ifdef USE_JOYSTICK
        m_pkItemImgBackground[i]->SetHotJoystickKey( EJK_KEY_1 + i );    // 조이스틱키로 보상 상자 선택할 수 있게 
#endif

        m_pkItemImgBackground[i]->SetHotKey( DIK_1 + (0x01 * static_cast<BYTE>(i)) );    // 숫자키로 보상 상자 선택할 수 있게 
        m_pkBoxNumber[i]->InitState(false, false, this);
        m_pkBoxNumberBack[i]->InitState(false, false, this);
    }
}

void KGCBonusBoxSelect_S4_Row::ActionPerformed( const KActionEvent& event )
{
    for( int i = 0 ; i < BOX_NUM; ++i )
    {
        if( event.m_pWnd  == m_pkItemImgBackground[i] )
        {
            if ( D3DWE_BUTTON_CLICK == event.m_dwCode )
            {
                g_KDSound.Play( "988024" );
                ClickBonusBox( i );
                return;
            }
            else if ( D3DWE_CURSOR_ENTER == event.m_dwCode )
            {
                g_KDSound.Play( "988025" );
            }
        }
    }
}

void KGCBonusBoxSelect_S4_Row::ClickBonusBox( int iSelectIndex_ )
{
    if( m_iKeyNum <= 0 || iSelectIndex_ < 0 || iSelectIndex_ >= BOX_NUM )
        return;

    SetClickedBonusBox( iSelectIndex_ );
    g_kSelectBonusBox->m_dwUserUID = g_kGlobalValue.m_kUserInfo.dwUID;
    g_kSelectBonusBox->m_ucSelectIndex = iSelectIndex_;
    g_kSelectBonusBox->Type = GC_PID_SELECT_BONUSBOX;
    SendP2PPacket( g_kSelectBonusBox, sizeof( KGC_PID_SELECT_BONUSBOX), KNC_BROADCAST, _RELIABLE);
    KGCPC_MEMO( "GC_PID_SELECT_BONUSBOX" );
    m_bSelected = true;
    m_bSelectedIndex[iSelectIndex_] = true;

    if ( m_pkStaticOwner->GetText() == g_kGlobalValue.m_kUserInfo.strNickName )
        TRACE( L"CLICK >> Select = %d\n", iSelectIndex_ );

    SetMyKeyNum( --m_iKeyNum );

    if ( m_iKeyNum <= 0 )
    {
        g_pkGameOverlayUI->GetQuestResultS3()->GetBonusBoxSelect()->StopCountDown();
        m_bDisableClick = true;
    }
}

void KGCBonusBoxSelect_S4_Row::SetClickedBonusBox( int iSelectIndex_ )
{
    if( iSelectIndex_ < 0 || iSelectIndex_ >= BOX_NUM )
        return;

    m_pkItemImgBackground[iSelectIndex_]->InitState( true, false, NULL );
    m_pkItemImgBackground[iSelectIndex_]->Lock( true );
    m_pkBackgroundSpliter[iSelectIndex_]->Lock( true );
}

void KGCBonusBoxSelect_S4_Row::SetSpecialBoxInfo( KRewardInfo kRewardInfo_ )
{
    /*
    enum REWARD_TYPE {
        RT_EMPTY = 0,   // 꽝
        RT_CHARACTER,   // 캐릭터 아이템
        RT_ITEM_1,      // 잡템1
        RT_ITEM_2,      // 잡템2
        RT_GP_50,       // GP 50%
        RT_GP_25,       // GP 25%

        RT_MAX,
    };

    DWORD                                           m_dwUID;            // UserUID
    char                                            m_cCharType;        // 캐릭터 타입
    std::set<USHORT>                                m_setSelectIndex;   // 선택한 보상 번호
    int                                             m_nRewardGP;        // 실제 획득 GP
    std::vector<KItem>                              m_vecRewardItem;    // 실제 획득 보상아이템
    std::vector<KRewardBox>                         m_vecItemList;      // 상자 아이템 정보(정렬은 서버에서). vector<보상정보>

    DECL_DATA( KRewardBox )
    {
    USHORT         m_usBoxType;    // 보상타입
    KDropItemInfo  m_kRewardItem;  // 보상 ItemID
    int            m_nRewardGP;    // 보상 GP
    };
    */
    std::vector<KRewardBox>::iterator vit = kRewardInfo_.m_vecItemList.begin();
    for ( int i = 0; vit != kRewardInfo_.m_vecItemList.end(); ++vit, ++i )
    {
        bool bGP = (*vit).m_usBoxType == static_cast<USHORT>(KRewardInfo::RT_GP_50) || (*vit).m_usBoxType == static_cast<USHORT>(KRewardInfo::RT_GP_25);

        GCItem* pItem = g_MyD3D->m_kItemMgr.GetItemData((*vit).m_kRewardItem.m_ItemID / 10);
        if ( pItem == NULL || ( pItem->dwGoodsID == 0 && !bGP ) )
        {
            m_pkItemImg[i]->ShowItem( false );
            m_pkItemImg[i]->ToggleRender( false );
            m_pkStaticItemName[i]->ToggleRender( false );
            continue;
        }

        if ( (*vit).m_usBoxType == static_cast<USHORT>(KRewardInfo::RT_GP_25) )
        {
            // 일단 GP쿠폰
            m_pkItemImg[i]->SetItemInfo( KGCItemManager::ITEM_GP_COUPON_S4_1 );
            m_pkStaticItemName[i]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BONUS_GP_STRING, "i", (*vit).m_nRewardGP ) );
            m_pkItemImg[i]->ToggleRender( true );

            if( g_kGlobalValue.m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_kGlobalValue.m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );
            
            if( g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );

            if( g_MyD3D->m_TempPlayer.m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );
        }
        else if ( (*vit).m_usBoxType == static_cast<USHORT>(KRewardInfo::RT_GP_50) )
        {
            m_pkItemImg[i]->SetItemInfo( KGCItemManager::ITEM_GP_COUPON_S4_2 );
            m_pkStaticItemName[i]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BONUS_GP_STRING, "i", (*vit).m_nRewardGP ) );
            m_pkItemImg[i]->ToggleRender( true );

            if( g_kGlobalValue.m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_kGlobalValue.m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );

            if( g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_MyD3D->MyPlayer[ g_MyD3D->Get_MyPlayer() ]->m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );

            if( g_MyD3D->m_TempPlayer.m_kUserInfo.dwUID == kRewardInfo_.m_dwUID )
                g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().SetCurrentGP( kRewardInfo_.m_nCurrentGP );
        }
        else if ( (*vit).m_usBoxType == static_cast<USHORT>(KRewardInfo::RT_CHARACTER) )
        {
            // 번쩍 번쩍! 파티클로 하자 ㅋ
            float fOffsetX = 0.0f;
            float fOffsetY = 0.0f;
            std::string strParticle;
            if ( m_bMine )
            {
                strParticle = "Special_Reward_Big";
                fOffsetX = 34.0f * GC_SCREEN_DIV_WIDTH;
                fOffsetY = 35.0f * GC_SCREEN_DIV_WIDTH;
            }
            else
            {
                strParticle = "Special_Reward_Small";
                fOffsetX = 21.0f * GC_SCREEN_DIV_WIDTH;
                fOffsetY = 20.5f * GC_SCREEN_DIV_WIDTH;
            }

            GCFUNC::CreateSequenceToUI( m_pkItemImg[i], strParticle, fOffsetX, fOffsetY );
            m_pkItemImg[i]->SetItemInfo( pItem->dwGoodsID );
            m_pkStaticItemName[i]->SetText( pItem->strItemName );

#if defined(NATION_LATINAMERICA)
            m_pkStaticItemName[i]->SetAutoScroll(true);
#endif

        }
        else
        {
            m_pkItemImg[i]->SetItemInfo( pItem->dwGoodsID );
            m_pkStaticItemName[i]->SetText( pItem->strItemName );

#if defined(NATION_LATINAMERICA)
            m_pkStaticItemName[i]->SetAutoScroll(true);
#endif
        }
    }

    std::set<USHORT>::iterator sit = kRewardInfo_.m_setSelectIndex.begin();
    for ( ; sit != kRewardInfo_.m_setSelectIndex.end(); ++sit )
    {
        m_pkItemImgBackground[(*sit)]->Lock( true );
        m_pkBackgroundSpliter[(*sit)]->Lock( true );
        m_pkStaticItemName[(*sit)]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 198, 0 ) );
        m_bSelectedIndex[(*sit)] = true;

        if ( m_pkStaticOwner->GetText() == g_kGlobalValue.m_kUserInfo.strNickName )
            TRACE( L"PACKET >> Select = %d\n", (int)((*sit)) );
    }
}

void KGCBonusBoxSelect_S4_Row::InitRow( int iPlayerIndex, bool bMine_ )
{
    bool bMine = iPlayerIndex == g_MyD3D->Get_MyPlayer();
    m_pk10PosNum->ToggleRender( bMine );
    m_pk1PosNum->ToggleRender( bMine );
    m_bSelected = false;
    m_bMine = bMine_;

    for ( int i = 0; i < BOX_NUM; ++i )
    {
        m_bSelectedIndex[i] = false;
        m_pkItemImgBackground[i]->InitState( true, false, NULL );
        m_pkItemImgBackground[i]->Lock( false );
        m_pkBackgroundBox[i]->ToggleRender( false );
        m_pkItemImg[i]->ShowItem( false );
        m_pkStaticItemName[i]->ToggleRender( false );
        m_pkStaticItemName[i]->SetText();
        m_pkStaticItemName[i]->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

        m_pkBonusBox[i]->ToggleRender( !m_bSpecial );
        m_pkBonusSPBox[i]->ToggleRender( m_bSpecial );
        m_pkBoxNumber[i]->ToggleRender( bMine );
        m_pkBoxNumberBack[i]->ToggleRender( bMine );
        m_pkBackgroundSpliter[i]->ToggleRender( false );
        m_pkBackgroundSpliter[i]->Lock( false );
    }

    PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex];
    if ( NULL != pPlayer && pPlayer->IsLive() )
    {
        m_pkStaticOwner->SetText( L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx" );
        m_dwUID = pPlayer->m_kUserInfo.dwUID;

        for ( int i = 0; i < GC_CHAR_NUM; ++i )
        {
            m_pkMyCharFace[i]->ToggleRender( i == pPlayer->GetCurrentChar().iCharType );
        }
    }
}

void KGCBonusBoxSelect_S4_Row::InitBeforeBoxOpen( void )
{
    for ( int i = 0; i < BOX_NUM; ++i )
    {
        if ( NULL != g_MyD3D->GetPlayerByUID(m_dwUID) && g_MyD3D->GetPlayerByUID(m_dwUID)->IsLive() )
        {
            m_pkItemImg[i]->ShowItem( true );
        }
        m_pkItemImgBackground[i]->InitState( true, false, NULL );
        m_pkStaticItemName[i]->ToggleRender( true );
        m_pkBackgroundBox[i]->ToggleRender( true );
        m_pkBonusBox[i]->ToggleRender( false );
        m_pkBonusSPBox[i]->ToggleRender( false );
        m_pkBoxNumber[i]->ToggleRender( false );
        m_pkBoxNumberBack[i]->ToggleRender( false );
        m_pkBackgroundSpliter[i]->ToggleRender( true );
    }
}

bool KGCBonusBoxSelect_S4_Row::SetMyKeyNum( int iKey_ )
{
    m_iKeyNum = iKey_;
    if ( m_iKeyNum < 0 )
        m_iKeyNum = 0;

    m_pk10PosNum->SetNumber( m_iKeyNum / 10 );
    m_pk1PosNum->SetNumber( m_iKeyNum % 10 );
    m_pk10PosNum->ToggleRender( true );
    m_pk1PosNum->ToggleRender( true );

    return m_iKeyNum > 0;
}

void KGCBonusBoxSelect_S4_Row::AutoSelect( void )
{
    if ( this->IsRenderOn() && false == m_bSelected && m_iKeyNum > 0 )
    {
        ClickBonusBox(0);
    }
    m_bDisableClick = true;
}

void KGCBonusBoxSelect_S4_Row::FrameMoveInEnabledState( void )
{
    for ( int i = 0; i < BOX_NUM; ++i )
    {
        if ( m_bSelectedIndex[i] )
        {
            m_pkItemImgBackground[i]->Lock( true );
            m_pkBackgroundSpliter[i]->Lock( true );
        }
        else if ( m_bDisableClick )
        {
            m_pkItemImgBackground[i]->SetWndMode( D3DWM_DEFAULT );
            m_pkBackgroundSpliter[i]->SetWndMode( D3DWM_DEFAULT );
        }
    }
}

void KGCBonusBoxSelect_S4_Row::EnableClick()
{
    for(int i = 0 ; i < BOX_NUM; ++i )
    {
        m_pkItemImgBackground[i]->InitState( true, true, this );
    }
}

void KGCBonusBoxSelect_S4_Row::InitAbsent( void )
{
    m_pkStaticOwner->SetText();
    m_dwUID = 0;

    for ( int i = 0; i < BOX_NUM; ++i )
    {
        m_bSelectedIndex[i] = false;
        m_pkItemImgBackground[i]->InitState( true, false, NULL );
        m_pkItemImgBackground[i]->Lock( false );
        m_pkBackgroundBox[i]->ToggleRender( false );
        m_pkItemImg[i]->ShowItem( false );
        m_pkStaticItemName[i]->ToggleRender( false );
        m_pkStaticItemName[i]->SetText();
        m_pkBonusBox[i]->ToggleRender( false );
        m_pkBonusSPBox[i]->ToggleRender( false );
        m_pkBoxNumber[i]->ToggleRender( false );
        m_pkBoxNumberBack[i]->ToggleRender( false );
        m_pkBackgroundSpliter[i]->ToggleRender( false );
        m_pkBackgroundSpliter[i]->Lock( false );
    }

    for ( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        m_pkMyCharFace[i]->ToggleRender( false );
    }
}

void KGCBonusBoxSelect_S4_Row::OpenBonusBoxEffect( void )
{
    g_KDSound.Play( "988023" );
    for ( int i = 0; i < BOX_NUM; ++i )
    {
        float fOffsetX = static_cast<float>( m_pkBonusBox[i]->GetWidth() / 2.0f );
        float fOffsetY = static_cast<float>( m_pkBonusBox[i]->GetHeight() / 2.0f );
        GCFUNC::CreateSequenceToUI( m_pkBonusBox[i], "Box_Open_01", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBox[i], "Box_Open_02", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBox[i], "Box_Open_03", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBox[i], "Box_Open_04", fOffsetX, fOffsetY );
        GCFUNC::CreateSequenceToUI( m_pkBonusBox[i], "Box_Open_05", fOffsetX, fOffsetY );
    }
}