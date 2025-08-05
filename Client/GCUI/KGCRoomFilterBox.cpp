#include "stdafx.h"
//
#include "KGCRoomFilterBox.h"


#include "KGCRoomFilterList.h"
//
#include "KGCRoomManager.h"
#include "KGCRoomListManager.h"

//

IMPLEMENT_CLASSNAME( KGCRoomFilterBox );
IMPLEMENT_WND_FACTORY( KGCRoomFilterBox );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomFilterBox, "gc_room_filter_box" );

#define RENDER_FRAME (20)

KGCRoomFilterBox::KGCRoomFilterBox( void )
{
    m_pkCurrentCategory = NULL;
    m_pkList            = NULL;

    LINK_CONTROL( "current_category",   m_pkCurrentCategory );
    LINK_CONTROL( "category_list",      m_pkList );

    m_iRenderFrame = RENDER_FRAME;
    m_iCurrentCategoryIndex = 0;
}

KGCRoomFilterBox::~KGCRoomFilterBox( void )
{
}

void KGCRoomFilterBox::OnCreate()
{
    m_pkCurrentCategory->InitState( true );
    m_pkList->InitState( false, true, this );

    m_pkCurrentCategory->UpdateCategory( (char)GC_GMC_INVALID );
}

void KGCRoomFilterBox::FrameMoveInEnabledState()
{
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    bool bCheckBound = m_pkList->CheckPosInWindowBound( vMousePos ) && m_pkList->IsRenderOn();
    bCheckBound = bCheckBound || CheckPosInWindowBound( vMousePos );

    if( bCheckBound )
    {
        m_iRenderFrame = RENDER_FRAME;
        m_pkList->SetChildInputCheck( true );
    }
    else if( m_iRenderFrame > 0 )
    {
        --m_iRenderFrame;
        m_pkList->SetChildInputCheck( false );
    }

    if( m_iRenderFrame == 0 )
    {
        m_pkList->ToggleRender( false );
    }
    else
    {
        BYTE alpha = static_cast<BYTE>( 255.0f * (float)( m_iRenderFrame ) / (float)RENDER_FRAME );
        m_pkList->SetAllWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
        m_pkList->SetChildFontAlpha( alpha );
    }

    // 뭔가 선택이 되었다면 현재 선택된거 업데이트 해준다
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) && (m_iCurrentCategoryIndex != m_pkList->GetSelectCategoryIndex() || m_pkList->GetCheckClick()) )
    {
        m_iCurrentCategoryIndex = m_pkList->GetSelectCategoryIndex();
        m_pkList->SetCheckClick( false );

        EGCGameModeCategory eGameModeCategory = GC_GMC_MATCH;
        switch( m_iCurrentCategoryIndex )
        {
            case KGCRoomListManager::ECATEGORY_ALL:             eGameModeCategory = GC_GMC_INVALID; break;
            case KGCRoomListManager::ECATEGORY_MATCH:           eGameModeCategory = GC_GMC_MATCH; break;
            case KGCRoomListManager::ECATEGORY_TAGMATCH:        eGameModeCategory = GC_GMC_TAG_MATCH; break;
            case KGCRoomListManager::ECATEGORY_MONSTERCRUSADER: eGameModeCategory = GC_GMC_MONSTER_CRUSADER; break;
            case KGCRoomListManager::ECATEGORY_MONSTERHUNT:     eGameModeCategory = GC_GMC_MONSTER_HUNT; break;
            case KGCRoomListManager::ECATEGORY_ANGLE_EGG:       eGameModeCategory = GC_GMC_ANGELS_EGG; break;
            case KGCRoomListManager::ECATEGORY_DEATHMATCH:      eGameModeCategory = GC_GMC_DEATHMATCH; break;
            case KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH:eGameModeCategory = GC_GMC_FATAL_DEATHMATCH; break;
            case KGCRoomListManager::ECATEGORY_GUILD:           eGameModeCategory = GC_GMC_GUILD_BATTLE; break;
            case KGCRoomListManager::ECATEGORY_DOTA:            eGameModeCategory = GC_GMC_DOTA; break;
        }

        m_pkCurrentCategory->UpdateCategory( (char)eGameModeCategory );
        SiKGCRoomListManager()->SetRoomFilter( eGameModeCategory );

        int currentDifficulty = 0;

        if (SiKGCRoomListManager()->IsHellMode()) {
            currentDifficulty = GC_DUNGEON_LEVEL_HELL;
        }
        else if (SiKGCRoomListManager()->IsBreakMode()) {
            currentDifficulty = GC_DUNGEON_LEVEL_BREAK;
        }
        else {
            currentDifficulty = -1;
        }

		KP2P::GetInstance()->Send_RoomListReq( false, SiKGCRoomListManager()->GetWaitRoom(), SiKGCRoomListManager()->GetRoomFilter(), currentDifficulty, SiKGCRoomListManager()->GetCurrentPage() );
    }
}

void KGCRoomFilterBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCRoomFilterBox::ShowList()
{
    m_pkList->ToggleRender( true );
    m_pkList->Update();
    m_pkList->InitTextColor();

    m_iRenderFrame = RENDER_FRAME;
    m_pkList->SetChildInputCheck( true );
}

void KGCRoomFilterBox::Update()
{
    m_pkCurrentCategory->UpdateCategory( SiKGCRoomListManager()->GetRoomFilter() );
}
