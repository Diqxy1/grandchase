#include "StdAfx.h"
#include "KGCRealTimeRank.h"

#define RTR_MAX_RANK_GUAGE      100
#define RTR_ALPHA_WEIGHT        5
#define RTR_MAX_ALPHA           200

IMPLEMENT_CLASSNAME( KGCRealTimeRank );
IMPLEMENT_WND_FACTORY( KGCRealTimeRank );
IMPLEMENT_WND_FACTORY_NAME( KGCRealTimeRank, "gc_real_time_rank" );

KGCRealTimeRank::KGCRealTimeRank(void)
{
    m_pkRankGuage = NULL;
    for( int i = 0; i < NUM_OF_QUEST_RG; i++ )
        m_pkRankSign[i] = NULL;

    LINK_CONTROL( "rank_guage",     m_pkRankGuage);

    LINK_CONTROL( "rank_bad",       m_pkRankSign[QUEST_RG_BAD]);
    LINK_CONTROL( "rank_good",      m_pkRankSign[QUEST_RG_GOOD]);
    LINK_CONTROL( "rank_great",     m_pkRankSign[QUEST_RG_GREAT]);
    LINK_CONTROL( "rank_perfect",   m_pkRankSign[QUEST_RG_PERFECT]);
}

KGCRealTimeRank::~KGCRealTimeRank(void)
{
}

void KGCRealTimeRank::OnCreate( void )
{
    m_uiPointControl = 0;
    m_uiPointStageClear = 0;
    m_uiPointRank = 0;
    m_uiMovePointRank = 0;
    m_iPlusAlpha = -1;
    m_iMinusAlpha = -1;
    m_iWndAlpha = 255;
    m_eSignType = QUEST_RG_DEFAULT;
    m_ePrevSignType = QUEST_RG_DEFAULT;
    m_bEndAnim = true;
    m_bShowState = true;

    //게이지의 최대넓이가 달라질 수도 있기 때문에 비율값을 둔다.
    m_fWidthRatio = (float)RTR_MAX_RANK_GUAGE / 100.f;

    m_pkRankGuage->InitState(false);

    for( int i = 0; i < NUM_OF_QUEST_RG; i++ )
    {
        m_pkRankSign[i]->InitState(false);
        m_pkRankSign[i]->SetAllWindowAlpha(0);
    }
}

void KGCRealTimeRank::OnCreateComplete( void )
{
}

void KGCRealTimeRank::OnDestroyComplete( void )
{
}

void KGCRealTimeRank::FrameMoveInEnabledState( void )
{
    ShowWndAnim();

    if( m_bShowState )
    {
        AnimationRankGurage();
        AnimationRankSign();
    }   
}

void KGCRealTimeRank::PostChildDraw( void )
{
    // #if defined( _DEBUG )
    //	KLuaManager luaMgr;
    //	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //
    //	float x1 = 0.0f, y1 = 1.0f;
    //	LUA_GET_VALUE_DEF( "x1", x1, 1.0f );
    //	LUA_GET_VALUE_DEF( "y1", y1, 1.0f );
    //	float x2 = 0.0f, y2 = 1.0f;
    //	LUA_GET_VALUE_DEF( "x2", x2, 1.0f );
    //	LUA_GET_VALUE_DEF( "y2", y2, 1.0f );
    //float fScale =0.0f;
    //LUA_GET_VALUE_DEF( "scale", fScale, 1.0f );
    //#endif

    D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();

    //wow magic number!!!
    //D3DXVECTOR2 vPos1( 115 , 9 );
    //D3DXVECTOR2 vPos2( 115 , 31 );
    D3DXVECTOR2 vPos1( 124 * m_fWindowScaleX, 9 * m_fWindowScaleY);
    D3DXVECTOR2 vPos2( 124 * m_fWindowScaleX , 31 * m_fWindowScaleY);
    //D3DXVECTOR2 vPos3( 152 , 58 );

    g_pkUIMgr->RenderNumberScale("orange_gradation_num", vPos + vPos1,(DWORD)m_uiPointControl, 0.4f * m_fWindowScaleY, D3DCOLOR_RGBA( 255, 255, 255, m_iWndAlpha ), false, false, false);
    g_pkUIMgr->RenderNumberScale("orange_gradation_num", vPos + vPos2,(DWORD)m_uiPointStageClear, 0.4f * m_fWindowScaleY, D3DCOLOR_RGBA( 255, 255, 255, m_iWndAlpha ),false, false, false);
    //g_pkUIMgr->RenderNumberScale("orange_gradation_num", vPos + vPos3,(DWORD)m_uiPointRank, 0.4f);
}

void KGCRealTimeRank::SetControlPoint( UINT uiPoint_ ) 
{ 
    if( uiPoint_ > 100 )
        uiPoint_ = 100;

    m_uiPointControl = uiPoint_; 
}

void KGCRealTimeRank::SetRankPoint( UINT uiPoint_ ) 
{ 
    if( uiPoint_ > 100 )
        uiPoint_ = 100;

    m_pkRankGuage->ToggleRender(true);

    m_uiPointRank = uiPoint_; 

    m_bEndAnim = true;
}

void KGCRealTimeRank::AnimationRankGurage()
{
    if( m_uiPointRank > m_uiMovePointRank )
    {
        m_uiMovePointRank++;
        m_pkRankGuage->SetWidth( static_cast<int>(m_uiMovePointRank * m_fWidthRatio));
    }
    else if( m_uiPointRank < m_uiMovePointRank)
    {
        m_uiMovePointRank--;
        m_pkRankGuage->SetWidth( static_cast<int>(m_uiMovePointRank * m_fWidthRatio));
    }
    else
    {
        //여기서 랭크글자 처리
        if( m_bEndAnim  && m_uiPointRank != 0 )
            SetRankSign();

        if( m_uiMovePointRank == 0 )
            m_pkRankGuage->ToggleRender(false);
    }
}

void KGCRealTimeRank::AnimationRankSign()
{
    if( m_iPlusAlpha == -1 && m_iMinusAlpha == -1 )
        return;

    m_iPlusAlpha += RTR_ALPHA_WEIGHT;
    m_iMinusAlpha -= RTR_ALPHA_WEIGHT;

    if( m_iPlusAlpha > RTR_MAX_ALPHA)
        m_iPlusAlpha = RTR_MAX_ALPHA;

    if( m_iMinusAlpha <= 0 )
    {
        m_iMinusAlpha = 0;
        if( m_ePrevSignType != QUEST_RG_DEFAULT && m_ePrevSignType != m_eSignType )
            m_pkRankSign[m_ePrevSignType]->ToggleRender(false);
    }

    m_pkRankSign[m_eSignType]->SetAllWindowAlpha(m_iPlusAlpha);
    
    if( m_ePrevSignType != QUEST_RG_DEFAULT && m_ePrevSignType != m_eSignType )
        m_pkRankSign[m_ePrevSignType]->SetAllWindowAlpha(m_iMinusAlpha);

    if( m_iPlusAlpha == RTR_MAX_ALPHA && m_iMinusAlpha == 0 )
    {
        m_iPlusAlpha = -1;
        m_iMinusAlpha = -1;        
    }
}

void KGCRealTimeRank::SetRankSign()
{
    m_bEndAnim = false;
    m_iPlusAlpha = 0;
    m_iMinusAlpha = RTR_MAX_ALPHA;
    m_ePrevSignType = m_eSignType;

    if( m_uiPointRank < S3_RANK_GOOD_POINT )
        m_eSignType = QUEST_RG_BAD;        
    else if( S3_RANK_GOOD_POINT <= m_uiPointRank && m_uiPointRank < S3_RANK_GREAT_POINT )
        m_eSignType = QUEST_RG_GOOD;
    else if( S3_RANK_GREAT_POINT <= m_uiPointRank && m_uiPointRank < S3_RANK_PERFECT_POINT )
        m_eSignType = QUEST_RG_GREAT;
    else
        m_eSignType = QUEST_RG_PERFECT;
    

    if( m_eSignType != QUEST_RG_DEFAULT )
        m_pkRankSign[m_eSignType]->ToggleRender(true);
}

void KGCRealTimeRank::SetShowState( bool bShow )
{
    m_bShowState = bShow;

    if( m_bShowState )
        m_iWndAlpha = 0;
    else
        m_iWndAlpha = 255;
}

void KGCRealTimeRank::ShowWndAnim()
{
    if( m_bShowState )
    {
        m_iWndAlpha += ( RTR_ALPHA_WEIGHT * 2 );

        if( m_iWndAlpha > 255 )
            m_iWndAlpha = 255;        
    }
    else
    {
        m_iWndAlpha -= ( RTR_ALPHA_WEIGHT * 2 );

        if( m_iWndAlpha < 0 )
            m_iWndAlpha = 0;
    }

    this->SetAllWindowAlpha(m_iWndAlpha);
}