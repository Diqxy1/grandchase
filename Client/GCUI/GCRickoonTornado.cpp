#include "stdafx.h"
#include "GCRickoonTornado.h"
#include "../MyD3D.h"
#include "GCUI/GCMyInfoScene.h"
#include "KGCMath.h"
#include "gcui/GCItemInfoBox.h"
#include "gcui/GCUINumber.h"
#include "gcui/KGCTreasureItemBox.h"
#include "GCUI/GCItemInformationBoxPair.h"
#include "GCCharacterUI.h"
#include "../GCStateMachine.h"
#include "gcui/GCCharacterSelectBox.h"
#include "../GCRenderManager.h"

IMPLEMENT_CLASSNAME( KGCRickoonTornado );
IMPLEMENT_WND_FACTORY( KGCRickoonTornado );
IMPLEMENT_WND_FACTORY_NAME( KGCRickoonTornado, "gc_event_rickoon_tornado" );

KGCRickoonTornado::KGCRickoonTornado( void )
: m_pkPet(NULL)
, m_fZoom(0.25f)
, m_fPosX(-0.11f)
, m_fPosY(0.0f)
, m_fPosZ(0.303f)
, m_iSelectedChar(-1)
, m_iCoinCount(0)
, m_iLuckyCount(0)
, m_iRewardItemListIndex(0)
, m_iRewardItemListSize(0)
, m_fRewardSwapTime(0.0f)
, m_fSlotSwapTime(0.0f)
, m_fReawdLastTime(0.0f)
, m_fSlotLastTime(0.0f)
, m_fSlotRollTime(0.0f)
, m_bRickoonOn( false )
, m_bLuckyTornado( false)
, m_dwType(0)
, m_dwRewardType(0)
, m_bRewardDelay( false )
, m_fRewardDely(0.0f)
, m_fRewardDelyLast(0.0f)
, m_fParticleLife(0.1f)
, m_fParticleX(-0.11f)
, m_dwRewardItemID(0)
, m_bRool( false )
, m_bIsComplete (false)
, m_bEnableEvent(false)
, m_dwExposeItemToUser( 0 )
{
    char str[MAX_PATH] = {0,};

    for( int i = 0; i < ESLOT_ITEM_NUM; i++ )
    {
        m_pkImgItem[i] = NULL;
        sprintf_s( str, "img_item%d", i );
        LINK_CONTROL( str, m_pkImgItem[i] );
    }

    for( int i = 0; i < EREWARD_ITEM_NUM; i++ )
    {
        m_pkImgRewardItem[i] = NULL;
        sprintf_s( str, "img_reward_item%d", i );
        LINK_CONTROL( str, m_pkImgRewardItem[i] );
    }

    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        m_pkSelectCharFace[i] = NULL;
        sprintf_s( str, "character_face%d", i );
        LINK_CONTROL( str, m_pkSelectCharFace[i] );

        m_pkSelectCharName[i] = NULL;
        sprintf_s( str, "character_name%d", i );
        LINK_CONTROL( str, m_pkSelectCharName[i] );
    }

    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_char_right", m_pkBtnCharRight );
    LINK_CONTROL( "btn_start", m_pkBtnStart );
    LINK_CONTROL( "btn_stop", m_pkBtnStop );
    LINK_CONTROL( "btn_coin_buy", m_pkBtnBuy );
    LINK_CONTROL( "btn_reward_left", m_pkBtnRewardListLeft );
    LINK_CONTROL( "btn_reward_right", m_pkBtnRewardListRight );
    LINK_CONTROL( "btn_luckystart", m_pkBtnLuckyStart );

    LINK_CONTROL( "lucky_item_slot", m_pkLuckyItemSlot );

    //m_pkLuckyItemSlotOne = NULL;
    //LINK_CONTROL( "lucky_item_slot_one", m_pkLuckyItemSlotOne );

    LINK_CONTROL( "static_coin_name", m_pkStaticCoinName );
    LINK_CONTROL( "static_lucky_name", m_pkStaticLuckyCoinName );
    LINK_CONTROL( "static_coin_count", m_pkStaticCoinCount );
    LINK_CONTROL( "static_lucky_count", m_pkStaticLuckyCount );

    LINK_CONTROL( "img_lucky_coin", m_pkImgLuckyCoin );

    m_mapRewardList.clear();   
}

KGCRickoonTornado::~KGCRickoonTornado( void )
{
    if( m_pkPet != NULL )
        m_pkPet->EndPet();
    SAFE_DELETE( m_pkPet);
}
const D3DXVECTOR3 KGCRickoonTornado::AddVector( const D3DXVECTOR3& first, const D3DXVECTOR3& second )
{
    return D3DXVECTOR3( first.x + second.x, first.y + second.y, second.z );
}


void KGCRickoonTornado::ActionPerformed( const KActionEvent& event )
{
    GCWND_WND_CODE_MAP( m_pkBtnCharRight,	OnClickCharMove );

    if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if(m_pkBtnClose == event.m_pWnd)
        {
            if ( g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() )
            {
                return;
            }
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
            m_pkPet->EndPet();
            return;
        }

        else if( m_pkBtnStart == event.m_pWnd )
        {
            if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );    
                return;
            }
            m_pkPet->SetPetMotion( EGCPETMOTION::ATTACK );
            m_pkBtnStart->ToggleRender( false );
            m_pkBtnLuckyStart->ToggleRender( false );
            m_pkBtnStop->ToggleRender( true );
            ButtonLock();            
            //            RollStart();
        }
        else if( m_pkBtnLuckyStart == event.m_pWnd && m_pkBtnLuckyStart->IsRenderOn() )
        {
            if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );    
                return;
            }
            m_pkPet->SetPetMotion( EGCPETMOTION::ATTACK );
            m_pkBtnStart->ToggleRender( false );
            m_pkBtnLuckyStart->ToggleRender( false );
            m_pkBtnStop->ToggleRender( true );
            ButtonLock();
            //            RollStart();
        }
        else if ( m_pkBtnStop == event.m_pWnd )
        {
            if ( !m_vecRewardItem.empty() )
            {
                m_pkBtnStop->Lock( true );
                RollStop();
            }
        }
        else if ( m_pkBtnRewardListLeft == event.m_pWnd)
        {
            m_iRewardItemListIndex -= 3;
            UpdateRewardItemList();
        }
        else if ( m_pkBtnRewardListRight == event.m_pWnd)
        {
            m_iRewardItemListIndex += 3;
            UpdateRewardItemList();
        }
        
        else if ( m_pkBtnBuy == event.m_pWnd )
        {
            OnBuyBtn();
        }
    }
}

void KGCRickoonTornado::OnCreate( void )
{
    SetRickoon();
    ButtonUnLock();

    m_bRool = false;

    m_vecRewardItem.clear();
    m_vecUseItem.clear();

    m_fParticleLife = 0.1f;
    m_fRewardSwapTime = m_fSlotSwapTime = 0.0f;
    m_fReawdLastTime = m_fSlotLastTime = (float)timeGetTime()/1000;

    if (m_iSelectedChar == -1)
    {
        for( int i = 0; i < ESLOT_ITEM_NUM; i++ )
        {       
#if defined( NATION_KOREA )
            if( i == 1 ) { 
                m_pkImgItem[i]->InitState( true );
            }
            else { 
                m_pkImgItem[i]->InitState( false );
                m_bSlotEffect[i] = false;
                continue;
            }
#endif

            m_pkImgItem[i]->InitState( true );
            m_pkImgItem[i]->SetItemInfo( m_dwExposeItemToUser/10 );
            m_bSlotEffect[i] = false;
        }
    }

    m_iSelectedChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[g_kGlobalValue.m_kUserInfo.cCharIndex].iCharType;
    m_iRewardItemListSize = m_mapRewardList[m_iSelectedChar].size();

    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        m_pkSelectCharFace[i]->InitState( false, true, this);
        m_pkSelectCharName[i]->InitState( false, true, this);
    }

    m_pkSelectCharFace[m_iSelectedChar]->ToggleRender( true );
    m_pkSelectCharName[m_iSelectedChar]->ToggleRender( true );

    m_pkBtnClose->InitState( true, true, this );
    
#if defined( NATION_KOREA )
    m_pkBtnCharRight->InitState( false, true, this );
#else
    m_pkBtnCharRight->InitState( true, true, this );
#endif
    m_pkBtnCharRight->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHARACTER_SELECT_UP ) );
    m_pkBtnStart->InitState( true, true, this );
    m_pkBtnStop->InitState( false, true, this );
    m_pkBtnBuy->InitState( true, true, this );
    m_pkBtnRewardListLeft->InitState( true, true, this );
    m_pkBtnRewardListRight->InitState( true, true, this );
    m_pkBtnLuckyStart->InitState( false, true, this );

    m_pkLuckyItemSlot->InitState( false, true, this );
    //m_pkLuckyItemSlotOne->InitState( false );

    m_pkImgLuckyCoin->InitState( true, true, this );
    m_pkImgLuckyCoin->SetToolTip( g_pkStrLoader->GetString( STR_ID_RICKOON_LUCKY_COIN_TOOLTIP ) );

    GCItem* kItem = g_pItemMgr->GetItemData( RK_COIN_ID );
    m_pkStaticCoinName->InitState( true );
    m_pkStaticCoinName->SetAlign( DT_LEFT );
    m_pkStaticCoinName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
    m_pkStaticCoinName->SetText( kItem->strItemName );

    kItem = g_pItemMgr->GetItemData( RK_LUCKY_COIN_ID );
    m_pkStaticLuckyCoinName->InitState( true );
    m_pkStaticLuckyCoinName->SetAlign( DT_LEFT );
    m_pkStaticLuckyCoinName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
    m_pkStaticLuckyCoinName->SetText( kItem->strItemName );

    SetCoinCount();

    m_pkStaticCoinCount->InitState( true );
    m_pkStaticCoinCount->SetAlign( DT_LEFT );
    m_pkStaticCoinCount->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
    m_pkStaticLuckyCount->InitState( true );
    m_pkStaticLuckyCount->SetAlign( DT_LEFT );
    m_pkStaticLuckyCount->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );

    CheckLucky();

    srand((unsigned)time(NULL));

    for( int i = 0; i < EREWARD_ITEM_NUM; i++ )
    {
        m_pkImgRewardItem[i]->InitState( true );
    }

    if (m_iRewardItemListSize > 0)
        m_iRewardItemListIndex = rand()%m_iRewardItemListSize;
    UpdateRewardItemList();

    m_pkBtnCharRight->SetHotKey( DIK_UP );

    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X        = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y        = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width    = static_cast<DWORD>(VIEWPORT_WIDTH * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height   = static_cast<DWORD>(VIEWPORT_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ     = 0.0f;
    m_viewport.MaxZ     = 1.0f;
}

void KGCRickoonTornado::OnCreateComplete( void )
{
    m_bIsComplete = true;
}

void KGCRickoonTornado::OnDestroy( void )
{
    //    m_iSelectedChar = -1;
    m_fPosX = -0.11f;
}

void KGCRickoonTornado::FrameMoveInEnabledState( void )
{

    UpdateCoinCount();

    CalTime();

    if( m_pkPet->m_iPetID != -1 )
    {
        if( m_pkPet->cFrame >= (int)m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame() - 1 )
        {
            if( m_pkPet->usMotion == EGCPETMOTION::ATTACK )
            {
                m_pkPet->SetPetMotion( EGCPETMOTION::TELEPORT_READY );
            }
        }
    }

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    bool bCheck = false;
    for(int i = 0; i < EREWARD_ITEM_NUM; ++i)
    {
        if( m_pkImgRewardItem[i]->CheckPosInWindowBound( vMousePos ) && (int)m_pkImgRewardItem.size() > i )
        {
            // 아이템 정보 표시창 위치 보정.
            D3DXVECTOR2 vecPos =m_pkImgRewardItem[i]->GetCurrentWindowPos();

            vecPos.x *= m_fWindowScaleX;
            vecPos.y *= m_fWindowScaleY;
            vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

            GCItem* pItem = g_pItemMgr->GetItemData( m_pkImgRewardItem[i]->GetItemID() );

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            bCheck = true;
            break;
        }
    }

    if( !bCheck )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }


    if( (g_pkInput->IsDown( DIK_ESCAPE )
#ifdef USE_JOYSTICK
        || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
#endif
        )
        && g_pkUIScene->IsCashBoxRenderOn() == false && m_bRool == false )
    {
        m_pkPet->EndPet();
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    }
}

void KGCRickoonTornado::PostChildDraw( void )
{
    D3DXMATRIX  mat1, mat2, mat3, matWorld;
    D3DXMATRIX  mat4;
    D3DVIEWPORT9 orgViewport;
    g_pGCDeviceManager2->Flush();
    g_pGCDeviceManager2->PushState();    
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );        

        g_pGCDeviceManager2->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );

        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );


        g_pd3dDevice->GetViewport( &orgViewport );
        g_pd3dDevice->SetViewport( &m_viewport );

        g_pd3dDevice->SetFVF( SKINVERTEX::FVF );

        // 퍼스펙티브 설정
        ::D3DXMatrixPerspectiveLH( &mat3, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ),
            1.0f, 1.0f, 100.0f );
        ::D3DXMatrixScaling( &mat1, 1.0f, 1.0f, 0.01f );
        ::D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
        ::D3DXMatrixTranslation( &mat3, 0.0f, 0.172f, 0.5f );
        ::D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
        g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat1 );

        ::D3DXMatrixIdentity(&matWorld);
        ::D3DXMatrixRotationX( &mat3, 0.0f );
        ::D3DXMatrixRotationY( &mat2, 0.0f );
        ::D3DXMatrixMultiply( &mat1, &mat3, &mat2 );
        ::D3DXMatrixScaling( &mat2, m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
        ::D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
        ::D3DXMatrixTranslation( &mat2, 0.0f, 0.081f - ( 0.40f - m_fZoom ) * 0.25f, 0.32f );
        ::D3DXMatrixMultiply( &matWorld, &mat3, &mat2 );
        g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX(1), &matWorld );
        //m_pMeshKOGRoom->Render();


        if( m_pkPet->m_iPetID != -1 )
        {
            ::D3DXMatrixIdentity(&mat1);
            ::D3DXMatrixIdentity(&mat2);
            ::D3DXMatrixIdentity(&mat3);
            ::D3DXMatrixIdentity(&matWorld);

            float fScale = 1.5f * m_pkPet->m_fScale;

            //             KLuaManager luaMgr;
            //             GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
            // 
            //             LUA_GET_VALUE_DEF( "posz", m_fPosZ, 0.303f );  

            m_fParticleX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x;
            m_fPosX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;

            ::D3DXMatrixTranslation( &mat3, m_vPetAxisCorrection.x, m_vPetAxisCorrection.y, m_vPetAxisCorrection.z );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );                        
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat1 );            
            ::D3DXMatrixRotationY( &mat2, -0.7f );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat2 );            
            ::D3DXMatrixTranslation( &mat3, m_fPosX, m_fPosY, m_fPosZ );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );

            m_pkPet->m_pObject->SetWorldMat( &matWorld );

            if( m_pkPet->usMotion != EGCPETMOTION::TELEPORT_READY || 
                m_pkPet->cFrame < static_cast<int>( m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame() ) - 1)
            {
                m_pkPet->m_pObject->SetFrame( &m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ) );

                m_pkPet->cFrame = ( m_pkPet->cFrame + 1 ) % m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame();

                if ( m_pkPet->usMotion == EGCPETMOTION::ATTACK && m_pkPet->cFrame == 45 )
                {
                    RollStart();
                    g_KDSound.Play("Rickoon_tornado_skill");
                }
            }
            else
            {
                if (m_bRickoonOn)
                    m_pkBtnStop->Lock( false );
            }

            g_RenderManager->Render(false, m_pkPet->m_pObject );

        }
    }
    g_pGCDeviceManager2->PopState();

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCRickoonTornado::SetRickoon()
{
    if( m_pkPet == NULL )
        m_pkPet = new KPet();

    m_pkPet->StartPet( -1, 56, PET_LEVEL_IN_SHOP, 0, EGCPETLOCATION::GAME, L"" ,false);
    m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );    
    m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );
    m_vPetAxisCorrection = m_pkPet->GetPetAxisCorrection();
}

void KGCRickoonTornado::OnClickCharMove( KD3DWnd* pWnd, DWORD dwCode )
{
    if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    OpenCharacterSelcectUI();
}

void KGCRickoonTornado::OpenCharacterSelcectUI()
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return;
    if(g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn())
        return;
    float x = 40.0f;
    float y = -230.0f;

    x += GetFixedWindowPos().x ;
    y += GetFixedWindowPos().y;

    D3DXVECTOR2 vpos( x, y);

    g_pkUIScene->m_pkCharacterSelectBox->InitCharacterSelectBox(g_MyD3D->m_pStateMachine->GetState(),
        boost::bind(&KGCRickoonTornado::SetSelectedChar,this,_1,_2), 
        boost::bind(&KGCRickoonTornado::UpdateCharacterSelect,this), 
        vpos,
        KGCCharacterSelectBox::STATE_POSITION_DOWN,KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL, 0, 0, m_iSelectedChar );
    //	g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect(vpos);
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);
    m_pkBtnCharRight->SetWndMode(D3DWM_LOCK);
}

void KGCRickoonTornado::UpdateCharacterSelect()
{
    m_pkBtnCharRight->SetWndMode(D3DWM_DEFAULT);
}

void KGCRickoonTornado::SetSelectedChar( int iCharType, int iPromotion)
{
    if ( m_iSelectedChar >= 0 && m_iSelectedChar < GC_CHAR_NUM)
    {
        m_pkSelectCharFace[m_iSelectedChar]->ToggleRender( false );
        m_pkSelectCharName[m_iSelectedChar]->ToggleRender( false );
    }
    m_iSelectedChar = iCharType;
    m_pkSelectCharFace[m_iSelectedChar]->ToggleRender( true );
    m_pkSelectCharName[m_iSelectedChar]->ToggleRender( true );
    m_iRewardItemListSize = m_mapRewardList[m_iSelectedChar].size();
    UpdateRewardItemList();
}

void KGCRickoonTornado::SetInitInfo( DWORD dwLuckyEnableCount, std::map< DWORD, std::map< DWORD, std::set<DWORD> > > mapNonFailRewardList, GCITEMID dwExposeItemToUser )
{
    m_dwExposeItemToUser = dwExposeItemToUser;
    m_iLuckyEnableCount = dwLuckyEnableCount;    

    std::map< DWORD, std::map< DWORD, std::set<DWORD> > >::iterator itor = mapNonFailRewardList.begin();

    for ( ; itor != mapNonFailRewardList.end(); ++itor)
    {
        std::vector< DWORD > templist;
        std::map< DWORD, std::set<DWORD> >::iterator mit;
        for (mit = itor->second.begin(); mit != itor->second.end(); ++mit)
        {
            if ( mit->first == 3) continue;
            std::set< DWORD >::iterator sit;
            for (sit = mit->second.begin(); sit != mit->second.end(); ++sit)
            {
                templist.push_back(*sit/10);
            }
        }
        m_mapRewardList[itor->first] = templist;
    }
}

void KGCRickoonTornado::UpdateRewardItemList()
{
    if (m_mapRewardList[m_iSelectedChar].empty())
        return;

    if ( m_iRewardItemListIndex < 0 )
        m_iRewardItemListIndex = m_mapRewardList[m_iSelectedChar].size() - 3;

    else if ( m_iRewardItemListIndex > m_iRewardItemListSize )
        m_iRewardItemListIndex = 0;        

    std::vector< DWORD >::iterator vit = m_mapRewardList[m_iSelectedChar].begin() + m_iRewardItemListIndex;

    for ( int i = 0; i < EREWARD_ITEM_NUM; ++i, ++vit )
    {
        if ( vit >= m_mapRewardList[m_iSelectedChar].end() )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin();
        }
        else if ( vit < m_mapRewardList[m_iSelectedChar].begin() )
        {
            vit = m_mapRewardList[m_iSelectedChar].end () - 3;
        }

        m_pkImgRewardItem[i]->SetItemInfo( *vit );
    }

    m_fRewardSwapTime = 0.0f;
}

void KGCRickoonTornado::UpdateSlotItemImg( int iSlotIdx/* = 0*/ )
{
//#if defined( NATION_KOREA )
//    if( iSlotIdx != 1 ) { 
//        return;
//    }
//#endif
    std::vector< DWORD >::iterator vit;

#if defined( NATION_KOREA )
    if ( !m_vecRewardItem.empty() ) { 
        if( !m_bSlotEffect[ 1 ] ) { 
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            m_pkImgItem[ 1 ]->SetItemInfo( *vit );
        }
    }
#else
    if ( m_dwRewardType != 3 && !m_vecRewardItem.empty() )
    {        
        if ( iSlotIdx > 0 && iSlotIdx < 4 )
        {
            if ( !m_bSlotEffect[iSlotIdx - 1] )
            {
                m_pkImgItem[iSlotIdx - 1]->SetItemInfo( m_dwRewardItemID/10 );
                ShowslotEffect( iSlotIdx - 1 );
            }
        }
    }
    else if ( !m_vecRewardItem.empty() )
    {
        if ( iSlotIdx == 1 )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            if ( !m_bSlotEffect[0] )
            {
                m_pkImgItem[0]->SetItemInfo( *vit );
                ShowslotEffect( 0 );
            }            
        }
        else if ( iSlotIdx == 2 )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;

            while( m_pkImgItem[0]->GetItemID() == *vit)
            {
                vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            }

            if ( !m_bSlotEffect[1] )
            {
                m_pkImgItem[1]->SetItemInfo( *vit );
                ShowslotEffect( 1 );
            }
        }
        else if ( iSlotIdx == 3 )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            if ( !m_bSlotEffect[2] )
            {
                m_pkImgItem[2]->SetItemInfo( *vit );
                ShowslotEffect( 2 );
            }
        }
    }

    for ( int i = iSlotIdx; i < ESLOT_ITEM_NUM; ++i)
    {
        vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
        m_pkImgItem[i]->SetItemInfo( *vit );
    }
#endif

    g_KDSound.Play("Rickoon_tornado_swap");
}

void KGCRickoonTornado::ButtonLock()
{
    m_pkBtnCharRight->SetWndMode(D3DWM_LOCK);
    m_pkBtnClose->Lock( true );
    m_pkBtnBuy->Lock( true );
    m_pkBtnRewardListLeft->Lock( true );
    m_pkBtnRewardListRight->Lock( true );
    m_pkBtnStop->Lock( true );
    m_bRool = true;

}

void KGCRickoonTornado::ButtonUnLock()
{
    m_pkBtnCharRight->SetWndMode(D3DWM_DEFAULT);
    m_pkBtnClose->Lock( false );
    m_pkBtnBuy->Lock( false );
    m_pkBtnRewardListLeft->Lock( false );
    m_pkBtnRewardListRight->Lock( false );
    m_pkBtnStop->Lock( false );
    m_bRool = false;
}

void KGCRickoonTornado::CalTime()
{
    //     KLuaManager luaMgr;
    //     GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    // 
    //     float rewardTime, slotTime, slotRollTime;
    // 
    //     LUA_GET_VALUE_DEF( "rTime", rewardTime, 3.0f );
    //     LUA_GET_VALUE_DEF( "sTime", slotTime, 0.3f );
    //     LUA_GET_VALUE_DEF( "srTime", slotRollTime, 5.0f );
    // 


    float currenttime = (float)timeGetTime()/1000;

    m_fRewardSwapTime += currenttime - m_fReawdLastTime;
    m_fParticleLife -= currenttime - m_fReawdLastTime;
    m_fReawdLastTime = currenttime;

    if ( m_fRewardSwapTime > REWARD_SWAP_TIME && m_iRewardItemListSize > 0)
    {
        m_iRewardItemListIndex = rand()%m_iRewardItemListSize;
        UpdateRewardItemList();        
    }

    //     float dx, dy;
    // 
    //     LUA_GET_VALUE_DEF( "dx", dx, 0.0f );
    //     LUA_GET_VALUE_DEF( "dy", dy, 0.0f );

    if (m_fParticleLife < 0 && m_pkPet->usMotion == EGCPETMOTION::ATTACK && m_pkPet->cFrame >= 45)
    {
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Rickoon_Spin_04", m_fParticleX - 0.1f, m_fPosY + 0.025f, 0.5f ) , GC_LAYER_UI);
        m_fParticleLife = 0.1f;
    }

    if ( m_bRickoonOn )
    {
        m_fSlotSwapTime += currenttime - m_fSlotLastTime;
        m_fSlotRollTime += currenttime - m_fSlotLastTime;
        m_fSlotLastTime = currenttime;

        if (m_fSlotRollTime > SLOT_ROLL_TIME)
        {
            if ( m_fSlotSwapTime > SLOT_SWAP_TIME )
            {
                UpdateSlotItemImg( (int)(m_fSlotRollTime - SLOT_ROLL_TIME) );
                if ( m_fSlotRollTime - SLOT_ROLL_TIME >= 3.0f )
                {
                    m_fSlotSwapTime = 0.0f;
                    RollStop();
                }
                m_fSlotSwapTime = 0.0f;
            }
        }

        else if ( m_fSlotSwapTime > SLOT_SWAP_TIME )
        {
            UpdateSlotItemImg();
            m_fSlotSwapTime = 0.0f;
        }
    }

    if ( m_bRewardDelay )
    {
        m_fRewardDely += currenttime - m_fRewardDelyLast;
        m_fRewardDelyLast = currenttime;

        if (m_fRewardDely > 1.0f)
        {
            RewardItem();
            //            m_fRewardDely = 0.0f;
            m_bRewardDelay = false;
        }
    }
    else if ( !g_pkUIScene->IsWaitMsgBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX ) && m_fRewardDely > 0.0f)
    {
        ButtonUnLock();
        CheckLucky();
        m_fRewardDely = 0.0f;
    }
}

void KGCRickoonTornado::OnBuyBtn()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANT_BUY_IN_ROOM), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    if( false == g_pItemMgr->CheckExtraInventoryExist() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    DWORD dwBuyItemID = RK_COIN_ID;

    GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );

    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else
        g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif

    }
    else
    {

        std::wostringstream strmText1;
        std::wostringstream strmText2;

#if defined ( _KOREAN ) || defined( _SPANISH )
        strmText1 << pItem->strItemName;
        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH ) || defined (NATION_THAILAND)
        strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
        strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
        strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
            strmText1.str(), strmText2.str(),
            KGCUIScene::GC_MBOX_USE_BUY, dwBuyItemID, pItem->iItemTypeValue, false, true );
    }
    InitRollState();


}

void KGCRickoonTornado::RollStart()
{
    //     if( false ==g_MyD3D->m_kItemMgr.CheckExtraInventoryExist() )
    //     {
    //         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );    
    //         return;
    //     }
    //     m_pkPet->SetPetMotion( EGCPETMOTION::ATTACK );

    m_pkBtnStart->ToggleRender( false );
    m_pkBtnLuckyStart->ToggleRender( false );
    m_pkBtnStop->ToggleRender( true );
    ButtonLock();
    m_bRickoonOn = true;
    m_fSlotLastTime = (float)timeGetTime()/1000;

    if ( m_bLuckyTornado ) m_dwType = 1;
    else m_dwType = 0;

    KP2P::GetInstance()->Send_RickoonActionReq( m_dwType, m_iSelectedChar, RK_COIN_ID*10, RK_LUCKY_COIN_ID*10 );
}

void KGCRickoonTornado::RollStop()
{
#if defined( NATION_KOREA )
    for ( int i = 0; i < ESLOT_ITEM_NUM; ++i)
    {
        m_pkImgItem[i]->SetItemInfo( m_dwRewardItemID/10 );

        ShowslotEffect( i );
    }
#else
    if (m_dwRewardType != 3 || m_bLuckyTornado)
    {
        for ( int i = 0; i < ESLOT_ITEM_NUM; ++i)
        {
            m_pkImgItem[i]->SetItemInfo( m_dwRewardItemID/10 );

            ShowslotEffect( i );
        }
    }
#endif

#if !defined( NATION_KOREA )
    else if ( m_fSlotRollTime - SLOT_ROLL_TIME > 0.0f )
    {   
        std::vector< DWORD >::iterator vit;
        int tempIndex, randIndex; 

        randIndex = rand()%m_iRewardItemListSize;

        if ( m_fSlotRollTime - SLOT_ROLL_TIME >= 2.0f )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            m_pkImgItem[2]->SetItemInfo( *vit );

            ShowslotEffect( 2 );
        }
        else if ( m_fSlotRollTime - SLOT_ROLL_TIME > 1.0f )
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + randIndex;
            m_pkImgItem[1]->SetItemInfo( *vit );

            ShowslotEffect( 1 );

            tempIndex = rand()%m_iRewardItemListSize;
            while( tempIndex == randIndex )
            {
                tempIndex = rand()%m_iRewardItemListSize;
            }

            vit = m_mapRewardList[m_iSelectedChar].begin() + tempIndex;
            m_pkImgItem[2]->SetItemInfo( *vit );

            ShowslotEffect( 2 );
        }
        else
        {
            vit = m_mapRewardList[m_iSelectedChar].begin() + rand()%m_iRewardItemListSize;
            m_pkImgItem[0]->SetItemInfo( *vit );
            ShowslotEffect( 0 );

            randIndex = rand()%m_iRewardItemListSize;

            vit = m_mapRewardList[m_iSelectedChar].begin() + randIndex;
            m_pkImgItem[1]->SetItemInfo( *vit );
            ShowslotEffect( 1 );

            tempIndex = rand()%m_iRewardItemListSize;
            while( tempIndex == randIndex )
            {
                tempIndex = rand()%m_iRewardItemListSize;
            }

            vit = m_mapRewardList[m_iSelectedChar].begin() + tempIndex;
            m_pkImgItem[2]->SetItemInfo( *vit );
            ShowslotEffect( 2 );
        }          
    }
#endif

    m_bRickoonOn = false;

    m_fRewardDelyLast = (float)timeGetTime()/1000;
    m_bRewardDelay = true;
}

void KGCRickoonTornado::SetRewardItemList( DWORD dwRewardType, std::vector<KItem> vecUseItem, std::vector<KItem> vecItem, DWORD dwRewardItemID )
{
    m_dwRewardType = dwRewardType;
    m_vecRewardItem = vecItem;
    m_vecUseItem = vecUseItem;
    m_dwRewardItemID = dwRewardItemID;
}

void KGCRickoonTornado::SetCoinCount()
{
    KItem* pkInvenItem = NULL;
    if(g_pItemMgr->FindInventoryForItemID(RK_COIN_ID, &pkInvenItem))
    {
        m_pkStaticCoinCount->SetText(pkInvenItem->m_nCount);
        m_iCoinCount = pkInvenItem->m_nCount;
        m_pkBtnStart->Lock( false );
        m_pkBtnLuckyStart->Lock( false );
    }
    else
    {
        m_pkStaticCoinCount->SetText(0);
        m_iCoinCount = 0;
        m_pkBtnStart->Lock( true );
        m_pkBtnLuckyStart->Lock( true );
    }

    if(g_pItemMgr->FindInventoryForItemID(RK_LUCKY_COIN_ID, &pkInvenItem))
    {
        m_pkStaticLuckyCount->SetText(pkInvenItem->m_nCount);
        m_iLuckyCount = pkInvenItem->m_nCount;
        m_pkBtnLuckyStart->Lock( false );
    }
    else
    {
        m_pkStaticLuckyCount->SetText(0);
        m_iLuckyCount = 0;
        m_pkBtnLuckyStart->Lock( true );
    }
}

void KGCRickoonTornado::RewardItem()
{
    if ( !m_vecRewardItem.empty() )
    {
        std::wstring strMsg;
        std::vector<KItem> vecTemp = m_vecRewardItem;
        g_pItemMgr->GetNewReceiveItemNum(m_vecRewardItem, vecTemp);

        for(int i = 0; i < (int)m_vecRewardItem.size(); i++ )
        {
            if (m_vecRewardItem[i].m_ItemID / 10 == RK_LUCKY_COIN_ID)
                continue;

            GCItem* kItem = g_pItemMgr->GetItemData( m_vecRewardItem[i].m_ItemID / 10 );
            KItem& kRewardItem = vecTemp[i];
            CONTINUE_NIL( kItem );

            if( kRewardItem.m_nPeriod == KItem::UNLIMITED_ITEM && kRewardItem.m_nCount == KItem::UNLIMITED_ITEM ) {
                strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE1, "l", kItem->strItemName );
            }
            else if( kRewardItem.m_nCount != KItem::UNLIMITED_ITEM )
            {
                DWORD dwReceiveCount = kRewardItem.m_nCount;
                KItem* pkInvenItem = g_pItemMgr->m_kInventory.FindItemByItemID( kRewardItem.m_ItemID / 10 );

                if ( pkInvenItem != NULL ) // 이미 존재 하는 아이템이면 획득 량만 보여 주기 위해서.
                {
                    int iShortCutNum = g_MyD3D->GetMyShortCutSlot()->GetCurEquipItemNum(m_vecRewardItem[i].m_ItemID / 10); 
                    dwReceiveCount -= (pkInvenItem->m_nCount + iShortCutNum );
                }


                strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE2, "li", kItem->strItemName, (int)dwReceiveCount );
            }
            else {
                strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE3, "li", kItem->strItemName, (int) vecTemp[i].m_nPeriod );
            }

             

            if( -1 != kItem->dwGoodsID )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kItem->dwGoodsID, 0, false, true );
                g_pkUIScene->m_pkTreaureItemBox->SetContentAutoMultiLine(strMsg);
            }
        }

        g_pItemMgr->m_kInventory.AddItemList( m_vecRewardItem, true );
        g_pItemMgr->m_kInventory.AddItemList( m_vecUseItem, true );

#if defined( NATION_KOREA )
        g_KDSound.Play("Rickoon_tornado_congratulation");
#else
        if ( m_dwRewardType != 3 )
            g_KDSound.Play("Rickoon_tornado_congratulation");
#endif


        //        CheckLucky();
    }
    InitRollState();
}

void KGCRickoonTornado::CheckLucky()
{
    if ( m_iLuckyCount >= m_iLuckyEnableCount && g_pItemMgr->FindInventoryForItemID(RK_COIN_ID))
    {
        m_bLuckyTornado = true;

#if !defined( NATION_KOREA )
        m_pkLuckyItemSlot->ToggleRender( true );
#else
        //m_pkLuckyItemSlotOne->ToggleRender( true );
#endif


        m_pkBtnLuckyStart->ToggleRender( true );
        m_pkBtnStart->ToggleRender( false );

        //         KLuaManager luaMgr;
        //         GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        // 
        //         float px, y, py;
        // 
        //         LUA_GET_VALUE_DEF( "x", px, 0.0f );
        //         LUA_GET_VALUE_DEF( "Y", y, 0.0f );
        //         LUA_GET_VALUE_DEF( "py", py, 0.0f );

        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Rickoon_Lucky", 0.02f, -0.3f, 0.5f ) , GC_LAYER_UI);

        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_01", 0.02f, 0.2f, 0.5f ) , GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_03", 0.02f, 0.2f, 0.5f ) , GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_04", 0.02f, 0.2f, 0.5f ) , GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_05", 0.02f, 0.2f, 0.5f ) , GC_LAYER_UI);

        g_KDSound.Play("Rickoon_tornado_lucky");
    }
}

void KGCRickoonTornado::ShowslotEffect( int slotNum )
{
#if defined( NATION_KOREA )
    if( slotNum != 1 ) { 
        return;
    }
#endif
    //        KLuaManager luaMgr;
    //        GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    float px;

    //         LUA_GET_VALUE_DEF( "px", px, 0.0f );
    //         LUA_GET_VALUE_DEF( "ty", py, 0.0f );

    switch(slotNum)
    {
    case 0:
        px = -0.175f;
        break;
    case 1:
        px = 0.03f;
        break;
    case 2:
        px = 0.22f;
        break;
    }

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_01", px, -0.12f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_02", px, -0.12f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_04", px, -0.12f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_05", px, -0.12f, 0.5f ) , GC_LAYER_UI);

    g_KDSound.Play("Rickoon_tornado_stop");
    m_bSlotEffect[slotNum] = true;
}

void KGCRickoonTornado::UpdateCoinCount()
{
    KItem* pkInvenItem = NULL;
    if(g_pItemMgr->FindInventoryForItemID(RK_COIN_ID, &pkInvenItem))
    {
        m_pkStaticCoinCount->SetText(pkInvenItem->m_nCount);
        m_iCoinCount = pkInvenItem->m_nCount;
    }
    else
    {
        m_pkStaticCoinCount->SetText(0);
        m_iCoinCount = 0;
    }

    if(g_pItemMgr->FindInventoryForItemID(RK_LUCKY_COIN_ID, &pkInvenItem))
    {
        m_pkStaticLuckyCount->SetText(pkInvenItem->m_nCount);
        m_iLuckyCount = pkInvenItem->m_nCount;
    }
    else
    {
        m_pkStaticLuckyCount->SetText(0);
        m_iLuckyCount = 0;
    }
}

void KGCRickoonTornado::InitRollState()
{
    m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
    m_pkBtnStart->ToggleRender( true );
    m_pkBtnStop->ToggleRender( false );
    m_fParticleX = m_fPosX = -0.11f;
    m_fSlotSwapTime = 0.0f;
    m_fSlotRollTime = 0.0f;


    SetCoinCount();
    m_vecRewardItem.clear();
    m_vecUseItem.clear();

    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateData();

    m_bLuckyTornado = false;
    m_pkLuckyItemSlot->ToggleRender( false );
    //m_pkLuckyItemSlotOne->ToggleRender( false );
    m_pkBtnLuckyStart->ToggleRender( false );

    for( int i = 0; i < ESLOT_ITEM_NUM; i++ )
    {
#if defined( NATION_KOREA ) 
        if( i == 1 ) { 
            m_pkImgItem[i]->InitState( true );
        }
        else {
            m_pkImgItem[i]->InitState( false );
        }
#else
        m_pkImgItem[i]->InitState( true );
#endif
        m_bSlotEffect[i] = false;
    }
}

bool KGCRickoonTornado::GetCreateComplete()
{
    return m_bIsComplete;
};

void KGCRickoonTornado::SetEnableEvent( bool bEnableEvent )
{
    m_bEnableEvent = bEnableEvent;
};

bool KGCRickoonTornado::GetEnableEvent()
{
    return m_bEnableEvent;
}

void KGCRickoonTornado::Close()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    KGCHotEventBtn::SetHotStateOff();

    m_bEnableEvent = false;
}
