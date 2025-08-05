#include "stdafx.h"
#include "KGCPlayerRank.h"

IMPLEMENT_CLASSNAME( KGCPlayerRank );
IMPLEMENT_WND_FACTORY( KGCPlayerRank );
IMPLEMENT_WND_FACTORY_NAME( KGCPlayerRank, "gc_player_rank" );

KGCPlayerRank::KGCPlayerRank( void )
:m_vRankTexCoords( 0.0f, 0.0f, 0.0f, 0.0f )
, m_vRankWndPos( 0.0f, 0.0f )
, m_pRankBack(NULL)
, m_pRankShow(NULL)
, m_vBaseTexStartAxis(3.f,3.f)
, m_vTexSize(120.f, 83.f)
, m_vTextureFullSize(512.f,512.f)
, m_vSecondTexStartAxis(128.f, 91.f)
, m_iPreRank(0)
, m_pkRankImg(NULL)
{
    LINK_CONTROL("rank_back", m_pRankBack);
    LINK_CONTROL("rank_show", m_pRankShow);
    LINK_CONTROL("rank_img", m_pkRankImg);
}

KGCPlayerRank::~KGCPlayerRank( void )
{
}

void KGCPlayerRank::OnCreate()
{
    m_pRankBack->InitState(false, false, this);
    m_pRankShow->InitState(false, false, this);
    m_pkRankImg->InitState(false, false, this);
    m_vRankTexCoords = m_pRankBack->GetWndTexCoords();
    m_vRankWndPos = D3DXVECTOR2(0.f, 0.f);
    m_iPreRank = 0;
}

void KGCPlayerRank::ActionPerformed(const KActionEvent& event )
{

}

void KGCPlayerRank::FrameMoveInEnabledState()
{

}

void KGCPlayerRank::SetPlayerRank( float fRankPoint )
{
    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON ) {
        m_pRankBack->ToggleRender(false);
        m_pRankShow->ToggleRender(false);
        m_pkRankImg->ToggleRender(false);
        return;
    }

    float fPercent = 0.0f;
    float fDiff = 0.0f;
    int iIndex = 0;

    m_pRankShow->ToggleRender(true);
    m_pkRankImg->ToggleRender(true);
    PercentForDamage(fRankPoint, fPercent, iIndex );
    fDiff = (RANK_HEIGHT * fPercent)  / 512 ;

    int nX = iIndex / 5;
    int nY = iIndex % 5;

    m_vImageTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vImageTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;

    if ( m_vImageTexEndtAxis.y > fDiff)
        m_pRankShow->SetWndTexCoords( D3DXVECTOR4( m_vImageTexStartAxis.x, m_vImageTexEndtAxis.y - (fDiff - RANK_CORRECTION_NUM), m_vImageTexEndtAxis.x , m_vImageTexEndtAxis.y ) );
    
    m_pRankShow->SetWindowPosY( m_vRankWndPos.y + (RANK_HEIGHT - static_cast<int>(RANK_HEIGHT * fPercent ) ) );
    m_pRankShow->SetHeight( static_cast<int>( fPercent * RANK_HEIGHT ) );
}

void KGCPlayerRank::PercentForDamage( IN float fRankPoint, OUT float& fPercent, OUT int& iIndex )
{
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );


    if (fRankPoint >= fDungeonMaxScore ) {
        fRankPoint = fDungeonMaxScore;
    }
    else if (fRankPoint < 0 ) {
        fRankPoint = 0;
    }

    iIndex = g_pkQuestManager->GetRankNum();
    if ( iIndex != 0 && iIndex > m_iPreRank ) {
        ShowEffect( iIndex );
        m_iPreRank = iIndex;
    }
    else {
        m_iPreRank = iIndex;
    }

    g_pkQuestManager->GetRankScroeTermPercent(iIndex , fRankPoint , fPercent );

    if ( fPercent > 1.0f)
        fPercent = 1.0f;

    SetBackgroundImage( iIndex);
}

void KGCPlayerRank::SetBackgroundImage( int iIndex )
{
    int nX = iIndex / 5;
    int nY = iIndex % 5;

    m_vBackImgTexStartAxis.x = ( m_vBaseTexStartAxis.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vBackImgTexStartAxis.y = ( m_vBaseTexStartAxis.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    m_vBackImgTexEndtAxis.x = ( m_vBaseTexStartAxis.x + m_vTexSize.x + ((m_vSecondTexStartAxis.x - m_vBaseTexStartAxis.x) *static_cast<float>(nX) ) ) / m_vTextureFullSize.x;
    m_vBackImgTexEndtAxis.y = ( m_vBaseTexStartAxis.y + m_vTexSize.y + ((m_vSecondTexStartAxis.y - m_vBaseTexStartAxis.y) *static_cast<float>(nY) ) ) / m_vTextureFullSize.y;
    
    m_pRankBack->ToggleRender(true);
    m_pRankBack->SetWndTexCoords( D3DXVECTOR4( m_vBackImgTexStartAxis.x, m_vBackImgTexStartAxis.y , m_vBackImgTexEndtAxis.x, m_vBackImgTexEndtAxis.y ) );
}

void KGCPlayerRank::ShowEffect( int iIndex )
{
    if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
        return;

    D3DXVECTOR2 vPos = m_pRankBack->GetCurrentWindowPos();
    vPos.x += static_cast<float>(m_pRankBack->GetWidth()/2);
    vPos.y += static_cast<float>(m_pRankBack->GetHeight()/2);

    float fPosX = PARTICLEPOS_X(vPos.x);
    float fPosY = PARTICLEPOS_Y(vPos.y);
    
    switch(iIndex)
    {
    case 1:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_E_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_E_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_07",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_E_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 2:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_D_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_D_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_06",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_D_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 3:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_C_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_C_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_06",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_C_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 4:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_B_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_B_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_B_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 5:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_A_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_A_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_A_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 6:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_S_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_S_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_S_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 7:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_SS_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_SS_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_SS_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    }

    g_KDSound.Play( "972014" );
    ShowExpEffect( iIndex );
}



void KGCPlayerRank::ShowExpEffect(int iIndex)
{
    if (iIndex < 4 )
        return;

    D3DXVECTOR2 vPos = m_pRankBack->GetCurrentWindowPos();
    vPos.x += static_cast<float>(m_pRankBack->GetWidth()/2);
    vPos.y += static_cast<float>(m_pRankBack->GetHeight() -100.f);

    float fPosX = PARTICLEPOS_X(vPos.x);
    float fPosY = PARTICLEPOS_Y(vPos.y);

    switch(iIndex)
    {
    case 4:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_EXP_3",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 5:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_EXP_6",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 6:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_EXP_10",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    case 7:
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_EXP_15",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        break;
    }
}