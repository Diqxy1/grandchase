#include "stdafx.h"
#include "GCAutoMatchRankingReward.h"
#include "GCItemImgWnd.h"
#include "GCCharacterSelectBox.h"

IMPLEMENT_CLASSNAME( KGCAutoMatchRankingReward );
IMPLEMENT_WND_FACTORY( KGCAutoMatchRankingReward );
IMPLEMENT_WND_FACTORY_NAME( KGCAutoMatchRankingReward, "gc_automatch_ranking_reward" );

KGCAutoMatchRankingReward::KGCAutoMatchRankingReward( void )
: m_pkDesc( NULL ) 
, m_pkClosebtn( NULL ) 
, m_pkBtnSelectCharacter( NULL )
, m_nRPBtnState( 0 )
, m_nWinBtnState( 0 )
, m_iCharType( 0 )
{
    char str[MAX_PATH] = {0,};

    for ( int i = 0; i < EM_REWARD_NUM; ++i ) 
    {
        m_pkRPRewardBtn[i] = NULL;
        m_pkWinRewardBtn[i] = NULL;

        sprintf( str, "rp_ranker_btn%d", i );          
        LINK_CONTROL( str,  m_pkRPRewardBtn[i] );

        sprintf( str, "win_ranker_btn%d", i );          
        LINK_CONTROL( str,  m_pkWinRewardBtn[i] );

    }

    for ( int i = 0; i < EM_REWARD_ITEM_NUM; ++i ) 
    {
        m_pkRPRewardItem[i] = NULL;
        m_pkWinRewardItem[i] = NULL;

        sprintf( str, "rp_reward_item%d", i );          
        LINK_CONTROL( str,  m_pkRPRewardItem[i] );

        sprintf( str, "win_reward_item%d", i );          
        LINK_CONTROL( str,  m_pkWinRewardItem[i] );
    }

    LINK_CONTROL( "static_desc", m_pkDesc );
    LINK_CONTROL( "btn_close", m_pkClosebtn );
    LINK_CONTROL( "btn_character_select", m_pkBtnSelectCharacter );
}

KGCAutoMatchRankingReward::~KGCAutoMatchRankingReward( void )
{
}

void KGCAutoMatchRankingReward::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnSelectCharacter, KD3DWnd::D3DWE_BUTTON_CLICK, OpenCharacterSelcectUI );
    GCWND_MSG_MAP( m_pkClosebtn, KD3DWnd::D3DWE_BUTTON_CLICK, CloseUI );

    for ( int i = 0; i < EM_REWARD_NUM; ++i ) 
    {
        if( event.m_pWnd == m_pkRPRewardBtn[i] && event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            OnClickRewardBtn( KRankRewardInfo::RT_RP, i );
        }

        if( event.m_pWnd == m_pkWinRewardBtn[i] && event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            OnClickRewardBtn( KRankRewardInfo::RT_WINRECORD, i );
        }
    }
}

void KGCAutoMatchRankingReward::OnCreate( void )
{ 
    m_pkDesc->InitState( true );
    m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_AUTOMATCH_REWARD_DESC ) );

    m_pkClosebtn->InitState( true, true, this );

    m_pkBtnSelectCharacter->InitState(true, true, this);

    for ( int i = 0; i < EM_REWARD_NUM; ++i ) 
    {
        m_pkRPRewardBtn[i]->InitState( true, true, this );
        m_pkWinRewardBtn[i]->InitState( true, true, this );
    }

    for ( int i = 0; i < EM_REWARD_ITEM_NUM; ++i ) 
    {
        m_pkRPRewardItem[i]->InitState( true );
        m_pkRPRewardItem[i]->ShowItem( false );

        m_pkWinRewardItem[i]->InitState( true );
        m_pkWinRewardItem[i]->ShowItem( false );
    }

    UpdateRankerReward();
}

void KGCAutoMatchRankingReward::OnClickRewardBtn( int nRankType, int nRankGroup )
{
    SetRewardItem( m_iCharType, nRankType, nRankGroup );
}

void KGCAutoMatchRankingReward::CloseUI()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCAutoMatchRankingReward::OpenCharacterSelcectUI( void )
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
        boost::bind(&KGCAutoMatchRankingReward::SendChangeChange,this,_1), 
        boost::bind(&KGCAutoMatchRankingReward::UpdateCharacterSelect,this), vPos, KGCCharacterSelectBox::STATE_POSITION_DOWN, KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect(vPos);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowZOrder(D3DWZ_TOPMOST);
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_LOCK);
}

void KGCAutoMatchRankingReward::SendChangeChange(int iChar)
{
    m_iCharType = iChar;
    g_pkUIScene->m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent(g_pkUIScene->m_pkCharacterSelectBox, EWNDMESSAGE_CLOSE) );
}

void KGCAutoMatchRankingReward::UpdateCharacterSelect( void )
{
    UpdateRankerReward();
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_DEFAULT);
}

void KGCAutoMatchRankingReward::UpdateRankerReward()
{
    for ( int i = 0; i < EM_REWARD_NUM; ++i ) 
    {
        if ( i == m_nRPBtnState ) 
        {
            m_pkRPRewardBtn[i]->Lock( true );    
        }
        else
        {
            m_pkRPRewardBtn[i]->Lock( false );    
        }


        if ( i == m_nWinBtnState ) 
        {
            m_pkWinRewardBtn[i]->Lock( true );    
        }
        else
        {
            m_pkWinRewardBtn[i]->Lock( false );    
        }
    }

    std::vector<GCITEMID> vecRPReward = g_kGlobalValue.m_kRanking.GetAutoMatchReward( KRankRewardInfo::RT_RP, m_iCharType, m_nRPBtnState );
    std::vector<GCITEMID> vecWinReward = g_kGlobalValue.m_kRanking.GetAutoMatchReward( KRankRewardInfo::RT_WINRECORD, m_iCharType, m_nWinBtnState );


    for ( int i = 0; i < EM_REWARD_ITEM_NUM; ++i ) 
    {
        if ( (int)vecRPReward.size() > i ) 
        {
            GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( vecRPReward[i]/10  );

            m_pkRPRewardItem[i]->SetItemInfo( vecRPReward[i]/10, -1, (pItemInfo->dwSlotPosition&ESP_SET_ITEM), pItemInfo->iSetItemNumber );
            m_pkRPRewardItem[i]->ShowItem( true );        
            m_pkRPRewardItem[i]->ToggleRender( true );
        }
        else
        {
            m_pkRPRewardItem[i]->SetItemInfo( 0 );
            m_pkRPRewardItem[i]->ShowItem( false ); 
            m_pkRPRewardItem[i]->ToggleRender( false );
        }

        if ( (int)vecWinReward.size() > i ) 
        {
            GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( vecWinReward[i]/10  );
            m_pkWinRewardItem[i]->SetItemInfo( vecWinReward[i]/10, -1, (pItemInfo->dwSlotPosition&ESP_SET_ITEM), pItemInfo->iSetItemNumber );
            m_pkWinRewardItem[i]->ShowItem( true );     
            m_pkWinRewardItem[i]->ToggleRender( true );
        }
        else
        {
            m_pkWinRewardItem[i]->SetItemInfo( 0 );
            m_pkWinRewardItem[i]->ShowItem( false );  
            m_pkWinRewardItem[i]->ToggleRender( false );
        }
    }    
}

void KGCAutoMatchRankingReward::ResetRewardItem()
{
    m_iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    m_nRPBtnState = 0;
    m_nWinBtnState = 0;

    UpdateRankerReward();
}

void KGCAutoMatchRankingReward::SetRewardItem( int iCharType, int nRankType, int nRankGroup )
{
    m_iCharType = iCharType;

    if ( nRankType < KRankRewardInfo::RT_RP || nRankType > KRankRewardInfo::RT_WINRECORD ) 
        return;

    if ( nRankGroup < 0 || nRankGroup >= EM_REWARD_NUM ) 
        return;


    switch( nRankType ) 
    {
    case KRankRewardInfo::RT_RP:
        {
            m_nRPBtnState = nRankGroup;
        }
        break;
    case KRankRewardInfo::RT_WINRECORD:
        {
            m_nWinBtnState = nRankGroup;
        }
        break;
    default:
        break;
    }


    UpdateRankerReward();
}

void KGCAutoMatchRankingReward::FrameMoveInEnabledState( void )
{

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    for ( int i = 0; i < EM_REWARD_ITEM_NUM; ++i ) 
    {
        if( m_pkRPRewardItem[i]->CheckPosInWindowBound( vMousePos ) && m_pkRPRewardItem[i]->IsRenderOn() )
        {
            // 아이템 정보 표시창 위치 보정.
            D3DXVECTOR2 vecPos = m_pkRPRewardItem[i]->GetCurrentWindowPos();
            GCITEMID itemID = m_pkRPRewardItem[i]->GetItemID();
            RenderItemInfoBox( vecPos, itemID );
        }

        if( m_pkWinRewardItem[i]->CheckPosInWindowBound( vMousePos ) && m_pkWinRewardItem[i]->IsRenderOn() )
        {
            // 아이템 정보 표시창 위치 보정.
            D3DXVECTOR2 vecPos = m_pkWinRewardItem[i]->GetCurrentWindowPos();
            GCITEMID itemID = m_pkWinRewardItem[i]->GetItemID();
            RenderItemInfoBox( vecPos, itemID );
        }
    }
}

void KGCAutoMatchRankingReward::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID )
{
    vPos_.x *= m_fWindowScaleX;
    vPos_.y *= m_fWindowScaleY;
    vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

    GCItem* pItem = g_pItemMgr->GetItemData(_itemID);
    if ( !pItem ) 
        return;

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}