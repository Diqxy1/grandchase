#include "stdafx.h"
#include "KGCComboUI_S6.h"

IMPLEMENT_CLASSNAME( KGCComboUI_S6 );
IMPLEMENT_WND_FACTORY( KGCComboUI_S6 );
IMPLEMENT_WND_FACTORY_NAME( KGCComboUI_S6, "gc_combo_ui_s6" );

KGCComboUI_S6::KGCComboUI_S6( void )
: m_pkComboBack(NULL)
, m_pkComboImg(NULL)
, m_pkTotalScoreImg(NULL)
, m_pkKillImg(NULL)
, m_pkComboNum(NULL)
, m_pkTotalScore(NULL)
, m_pkKillNum(NULL)
, m_pkPartyRankImg( NULL )
, m_pkMonsterLevelImg( NULL )
, m_pkExpRankImg( NULL )
, m_pkBossKillImg(NULL)
, m_pkMonsterLevelNum( NULL )
, m_pkBossKillNum(NULL)
, m_bKillImageRender(false)
{
    LINK_CONTROL("combo_back", m_pkComboBack );
    LINK_CONTROL("combo" , m_pkComboImg );
    LINK_CONTROL("combo_ui_num" , m_pkComboNum );
    LINK_CONTROL("total_score_img" , m_pkTotalScoreImg );
    LINK_CONTROL("total_score" , m_pkTotalScore );
    LINK_CONTROL("kill_img" , m_pkKillImg );
    LINK_CONTROL("kill_num" , m_pkKillNum );

    LINK_CONTROL("party_rank_img" ,     m_pkPartyRankImg );
    LINK_CONTROL("monster_level_img",   m_pkMonsterLevelImg );
    LINK_CONTROL("exp_img",             m_pkExpRankImg );
    LINK_CONTROL("boss_kill_img",       m_pkBossKillImg );

    LINK_CONTROL("monster_level_num",   m_pkMonsterLevelNum );
    LINK_CONTROL("boss_kill_num",       m_pkBossKillNum );

    for( int i=0 ; i<PARTY_RANK_SIZE ; ++i ) { 
        m_apkPartyRank[i] = NULL;

        char temp[128];
        sprintf_s( temp, 127, "party_rank%d", i );
        LINK_CONTROL( temp, m_apkPartyRank[i] );
    }

    for( int i=0 ; i<EXP_RANK_SIZE ; ++i ) { 
        m_apkExpRank[i] = NULL;

        char temp[128];
        sprintf_s( temp, 127, "exp_rank%d", i );
        LINK_CONTROL( temp, m_apkExpRank[i] );
    }

    vecTotlScoreParticle.clear();
    vecComboCntParticle.clear();

    m_iPreTotalScore = 0;
    m_iCurrentPartyRank = 0;
    m_iBeforePartyRank = 0;
}

KGCComboUI_S6::~KGCComboUI_S6( void )
{
}

void KGCComboUI_S6::OnCreate()
{
    m_pkComboBack->InitState(false, true , this);
    m_pkComboImg->InitState(false, true , this);
    m_pkTotalScoreImg->InitState(false, true , this);
    m_pkKillImg->InitState(false, true , this);

    m_pkComboNum->InitState(false, true, this);
    m_pkTotalScore->InitState(false, true, this);
    m_pkKillNum->InitState(false, true, this);

    m_pkComboNum->SetWriteAlign(ALIGN_RIGHT_SIDE);
    m_pkComboNum->SetNumber( 0 );
    m_pkComboNum->SetStringType( "combo_0_to_5" );

    m_pkTotalScore->SetWriteAlign(ALIGN_RIGHT_SIDE);
    m_pkTotalScore->SetNumber( 0 );
    m_pkTotalScore->SetStringType( "dungeon_kill_totalscore_num" );

    m_pkKillNum->SetWriteAlign(ALIGN_RIGHT_SIDE);
    m_pkKillNum->SetNumber( 0 );
    m_pkKillNum->SetStringType( "dungeon_kill_totalscore_num" );

    m_pkBossKillNum->SetWriteAlign(ALIGN_RIGHT_SIDE);
    m_pkBossKillNum->SetNumber( 0 );
    m_pkBossKillNum->SetStringType( "dungeon_kill_totalscore_num" );    

    m_pkPartyRankImg->InitState( false, true, this );
    m_pkMonsterLevelImg->InitState( false, true, this );
    m_pkExpRankImg->InitState( false, true, this );
    m_pkBossKillImg->InitState(false, true, this);
    
    m_pkMonsterLevelNum->InitState( false, true, this );
    m_pkBossKillNum->InitState(false, true, this);

    m_pkMonsterLevelNum->SetWriteAlign( ALIGN_CENTER_SIDE );
    m_pkMonsterLevelNum->SetNumber( 0 );
    m_pkMonsterLevelNum->SetStringType( "infinity_dungeon_rank_num" );

    m_pkBossKillNum->SetWriteAlign( ALIGN_CENTER_SIDE );
    m_pkBossKillNum->SetNumber( 0 );
    m_pkBossKillNum->SetStringType( "infinity_dungeon_rank_num" );

    for( int i=0 ; i<PARTY_RANK_SIZE ; ++i ) { 
        m_apkPartyRank[i]->InitState( false, true, this );
    }

    for( int i=0 ; i<EXP_RANK_SIZE ; ++i )  {
        m_apkExpRank[i]->InitState( false, true, this );
    }

    SetParticle();

    m_iPreTotalScore = 0;
    m_iCurrentPartyRank = 0;
    m_iBeforePartyRank = 0;
}

void KGCComboUI_S6::ActionPerformed(const KActionEvent& event )
{

}

void KGCComboUI_S6::FrameMoveInEnabledState()
{
    if( g_MyD3D->MyHead->Combo_Count < 2 && !SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
    {
        Render(false);
        return;
    }

    if (g_pkQuestManager->GetComboBreakState() && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if ( m_pkKillImg->IsRenderOn() && !SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
            m_bKillImageRender = true;
        else
            m_bKillImageRender = false;

        if ( g_MyD3D->MyHead->bDisappearComboCheck )
            ShowDisappearEffect( m_bKillImageRender );

        Render(false);
        return;
    }

    std::string strRenderNumber;
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
        int iTotalScore = 0;
        int iKillNum = 0;
        int iBossKillNum = 0;

        g_pkQuestManager->GetComboDamage( iTotalScore );
        g_pkQuestManager->GetComboKillPoint( iKillNum );
        g_pkQuestManager->GetComboBossKillPoint( iBossKillNum );

        if ( ( g_MyD3D->MyHead->Combo_Count >= 2 && iTotalScore > 0 ) ||
            SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) 
        {
            CheckComboRenderNumber( g_MyD3D->MyHead->Combo_Count ,strRenderNumber );
            m_pkComboNum->SetStringType(strRenderNumber);
            m_pkComboNum->SetNumber( g_MyD3D->MyHead->Combo_Count );
            m_pkTotalScore->SetNumber( iTotalScore );

            if ( g_MyD3D->MyHead->bEventComboCheck ) {
                ShowEffect();
                ShowNumberEffect( g_MyD3D->MyHead->Combo_Count , 0);
            }

            if ( m_iPreTotalScore != iTotalScore && g_MyD3D->MyHead->bTotalScoreEffect) {
                m_iPreTotalScore = iTotalScore;
                ShowNumberEffect( iTotalScore , 1);
            }

            Render(true);
            

            if ( iKillNum <= 0 )
            {
                m_pkKillNum->ToggleRender(false);
                m_pkKillImg->ToggleRender(false);
            }
            else
            {
                m_pkKillNum->ToggleRender(true);
                m_pkKillImg->ToggleRender(true);
                m_pkKillNum->SetNumber( iKillNum );
            }

            if ( iBossKillNum <= 0 )
            {
                m_pkBossKillNum->ToggleRender(false);
                m_pkBossKillImg->ToggleRender(false);
            }
            else
            {
                m_pkBossKillNum->ToggleRender(true);
                m_pkBossKillImg->ToggleRender(true);
                m_pkBossKillNum->SetNumber( iBossKillNum );
            }
        }
        else
        {
            Render(false);
        }
    }
    else
    {
        int iTotalScore = 0;
        iTotalScore = static_cast<int> ( g_pGameBoard->m_pStartingMember[ g_MyD3D->GetMyPlayer()->m_iPlayerIndex].fComboDamage);
        if( ( g_MyD3D->MyHead->Combo_Count >= 2 && !(g_pGameBoard->GetComboBreak() )) && iTotalScore > 0 )
        {
            CheckComboRenderNumber( g_MyD3D->MyHead->Combo_Count ,strRenderNumber );
            m_pkComboNum->SetStringType(strRenderNumber);
            m_pkComboNum->SetNumber( g_MyD3D->MyHead->Combo_Count );
            m_pkTotalScore->SetNumber( iTotalScore );
            
            if ( g_MyD3D->MyHead->bEventComboCheck ) {
                ShowEffect();
                ShowNumberEffect( g_MyD3D->MyHead->Combo_Count , 0);
            }

            if ( m_iPreTotalScore != iTotalScore && g_MyD3D->MyHead->bTotalScoreEffect) {
                m_iPreTotalScore = iTotalScore;
                ShowNumberEffect( iTotalScore , 1);
            }

            RenderForMatch();
        }
        else
        {
            g_pGameBoard->m_pStartingMember[ g_MyD3D->GetMyPlayer()->m_iPlayerIndex].fComboDamage = 0;
            
            if ( g_MyD3D->MyHead->bDisappearComboCheck )
                ShowDisappearEffect( false );

            Render(false);
        }
    }
}

void KGCComboUI_S6::Render( bool bRender )
{
    m_pkComboBack->ToggleRender( bRender);
    m_pkComboImg->ToggleRender( bRender);
    m_pkTotalScoreImg->ToggleRender( bRender);
    m_pkKillImg->ToggleRender( bRender);

    m_pkComboNum->ToggleRender( bRender);
    m_pkTotalScore->ToggleRender( bRender);
    m_pkKillNum->ToggleRender( bRender);

    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
    {
        if ( !bRender )
        {
            if ( m_pkKillNum->GetNumber() > 0 )
            {
                m_pkKillImg->ToggleRender( true );
                m_pkKillNum->ToggleRender( true );
            }
        }

        bool bRenderState = g_MyD3D->GetMyPlayer()->GetcomboState();
        m_pkComboBack->ToggleRender( bRenderState );
        m_pkComboImg->ToggleRender( bRenderState );
        m_pkComboNum->ToggleRender( bRenderState );
        m_pkTotalScoreImg->ToggleRender( bRenderState );
        m_pkTotalScore->ToggleRender( bRenderState );

        RenderInfinityDungeonInfo( true );
    }
    else { 
        RenderInfinityDungeonInfo( false );
    }
}

void KGCComboUI_S6::RenderForMatch( )
{
    m_pkComboBack->ToggleRender(true);
    m_pkComboImg->ToggleRender( true);
    m_pkTotalScoreImg->ToggleRender( true);
    m_pkKillImg->ToggleRender( false);

    m_pkComboNum->ToggleRender( true);
    m_pkTotalScore->ToggleRender( true);
    m_pkKillNum->ToggleRender( false);

    m_pkBossKillImg->ToggleRender( false);
    m_pkBossKillNum->ToggleRender( false);
}

void KGCComboUI_S6::CheckComboRenderNumber( IN int iCount, OUT std::string& strNumberName )
{
    if ( iCount > 0 && iCount < 6)
        strNumberName = "combo_0_to_5";
    else if ( iCount >= 6 && iCount < 11)
        strNumberName = "combo_6_to_10";
    else if ( iCount >= 11 && iCount < 26)
        strNumberName = "combo_11_to_25";
    else if ( iCount >= 26 && iCount < 51)
        strNumberName = "combo_26_to_50";
    else if ( iCount >= 51 )
        strNumberName = "combo_51_to_100";
}

void KGCComboUI_S6::ShowEffect()
{
    D3DXVECTOR2 vPos = m_pkComboImg->GetCurrentWindowPos();

    vPos.x += static_cast<float>( ( m_pkComboImg->GetWidth()/2 ) + 12.f );
    vPos.y += static_cast<float>( ( m_pkComboImg->GetHeight()/2 ) + 0.2f );

    float fPosX = PARTICLEPOS_X(vPos.x);
    float fPosY = PARTICLEPOS_Y(vPos.y);
    
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_Combo_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
    
    g_MyD3D->MyHead->bEventComboCheck = false;
}

void KGCComboUI_S6::ShowDisappearEffect( bool bKillImgRender )
{
    D3DXVECTOR2 vPos1 = m_pkComboImg->GetCurrentWindowPos();
    D3DXVECTOR2 vPos2 = m_pkTotalScoreImg->GetCurrentWindowPos();
    D3DXVECTOR2 vPos3 = m_pkKillImg->GetCurrentWindowPos();

    vPos1.x += static_cast<float>( ( m_pkComboImg->GetWidth()/2 ) );
    vPos1.y += static_cast<float>( ( m_pkComboImg->GetHeight()/2 ) );
    vPos2.x += static_cast<float>( ( m_pkTotalScoreImg->GetWidth()/2 ) );
    vPos2.y += static_cast<float>( ( m_pkTotalScoreImg->GetHeight()/2 ) );
    vPos3.x += static_cast<float>( ( m_pkKillImg->GetWidth()/2 ) );
    vPos3.y += static_cast<float>( ( m_pkKillImg->GetHeight()/2 ) );

    float fPosX1 = PARTICLEPOS_X(vPos1.x);
    float fPosY1 = PARTICLEPOS_Y(vPos1.y);
    float fPosX2 = PARTICLEPOS_X(vPos2.x);
    float fPosY2 = PARTICLEPOS_Y(vPos2.y);
    float fPosX3 = PARTICLEPOS_X(vPos3.x);
    float fPosY3 = PARTICLEPOS_Y(vPos3.y);

    int iKillNum = 0;
    g_pkQuestManager->GetComboKillPoint( iKillNum );

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_Combo_Hide",fPosX1  , fPosY1 , 0.5f) , GC_LAYER_CUSTOM);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_Score_Hide",fPosX2  , fPosY2 , 0.5f) , GC_LAYER_CUSTOM);

    if ( bKillImgRender )
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Rank_Kill_Hide",fPosX3 , fPosY3 , 0.5f) , GC_LAYER_CUSTOM);

    g_MyD3D->MyHead->bDisappearComboCheck = false;
}

void KGCComboUI_S6::SetParticle()
{
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    //float x1 = 0.f, y1 = 0.f;
    //float x2 = 0.f, y2 = 0.f;

    //LUA_GET_VALUE_DEF( "x1", x1, 0.0f );
    //LUA_GET_VALUE_DEF( "y1", y1, 0.0f );
    //LUA_GET_VALUE_DEF( "x2", x2, 0.0f );
    //LUA_GET_VALUE_DEF( "y2", y2, 0.0f );


    vComboCntPos = GetFixedWindowLocalPos();
    vComboCntPos.x += m_pkComboImg->GetFixedWindowLocalPos().x;
    vComboCntPos.y += m_pkComboImg->GetFixedWindowLocalPos().y;

    vComboCntPos.x = PARTICLEPOS_X(vComboCntPos.x);
    vComboCntPos.y = PARTICLEPOS_Y(vComboCntPos.y);

    vComboCntPos.x -= 0.026f;
    vComboCntPos.y -= 0.015f;

    vTotalScorePos = GetFixedWindowLocalPos();
    vTotalScorePos.x += m_pkTotalScoreImg->GetFixedWindowLocalPos().x;
    vTotalScorePos.y += m_pkTotalScoreImg->GetFixedWindowLocalPos().y;

    vTotalScorePos.x = PARTICLEPOS_X(vTotalScorePos.x);
    vTotalScorePos.y = PARTICLEPOS_Y(vTotalScorePos.y);

    vTotalScorePos.x -= 0.02f;
    vTotalScorePos.y -= 0.017f;


    if( vecComboCntParticle.empty() )
    {
        vecComboCntParticle.push_back("UI_Rank_Count_00");
        vecComboCntParticle.push_back("UI_Rank_Count_01");
        vecComboCntParticle.push_back("UI_Rank_Count_02");
        vecComboCntParticle.push_back("UI_Rank_Count_03");
        vecComboCntParticle.push_back("UI_Rank_Count_04");
        vecComboCntParticle.push_back("UI_Rank_Count_05");
        vecComboCntParticle.push_back("UI_Rank_Count_06");
        vecComboCntParticle.push_back("UI_Rank_Count_07");
        vecComboCntParticle.push_back("UI_Rank_Count_08");
        vecComboCntParticle.push_back("UI_Rank_Count_09");
    }

    if( vecTotlScoreParticle.empty() )
    {
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_00");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_01");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_02");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_03");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_04");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_05");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_06");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_07");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_08");
        vecTotlScoreParticle.push_back("UI_Rank_Count_S_09");
    }
}


void KGCComboUI_S6::ShowNumberEffect( int iNum, int iType)
{
    std::vector<int> vecDigit;
    CalculateDigit( iNum , vecDigit);

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    //float x = 0.f, y = 0.f;
    //LUA_GET_VALUE_DEF( "x", x, 0.0f );
    //LUA_GET_VALUE_DEF( "y", y, 0.0f );
    float x = 0.07f;
    float y = 0.04f;

    std::vector<int>::iterator vit = vecDigit.begin();
    int iCnt = 0;
    if ( vit != vecDigit.end() )
    {
        for ( ; vit != vecDigit.end() ; vit++)
        {
            if ( iType == 0)
                g_ParticleManager->CreateSequenceReceiveName( vecComboCntParticle[*vit] , vComboCntPos.x - (iCnt * x), vComboCntPos.y );
            else if (iType == 1)
                g_ParticleManager->CreateSequenceReceiveName( vecTotlScoreParticle[*vit] , vTotalScorePos.x - (iCnt * y), vTotalScorePos.y );
            ++iCnt;
        }
    }

    if ( iType == 1)
        g_MyD3D->MyHead->bTotalScoreEffect = false;
}

void KGCComboUI_S6::CalculateDigit( IN int iNum , OUT std::vector<int>& vecDigit )
{
    CString strTemp;
    strTemp.Format(_T("%d"), iNum);
    int len = strTemp.GetLength();

    int iDigit =0;
    for (int i = 0; i < len ; i++)
    {
        iDigit = iNum % 10;
        vecDigit.push_back(iDigit);
        iNum = iNum / 10;
    }
}

void KGCComboUI_S6::RenderInfinityDungeonInfo( const bool bRender_ )
{
    m_pkPartyRankImg->ToggleRender( bRender_ );
    m_pkMonsterLevelImg->ToggleRender( bRender_ );
    m_pkExpRankImg->ToggleRender( bRender_ );
    m_pkBossKillImg->ToggleRender( bRender_ );

    m_pkMonsterLevelNum->ToggleRender( bRender_ );
    m_pkBossKillNum->ToggleRender( bRender_ );

    for( int i=0 ; i<PARTY_RANK_SIZE ; ++i ) { 
        m_apkPartyRank[i]->ToggleRender( bRender_ );
    }

    for( int i=0 ; i<EXP_RANK_SIZE ; ++i )  {
        m_apkExpRank[i]->ToggleRender( bRender_ );
    }

    if( bRender_ ) { 
        SetInfinityDungeonInfo( g_pkQuestManager->GetInfinityDungeonPartyRank(), g_pkQuestManager->GetInfinityDungeonMonsterLevel() );
    }
}

void KGCComboUI_S6::SetInfinityDungeonInfo( const int iPartyRank_, const int iMonsterLevel_ )
{
    if( !SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        RenderInfinityDungeonInfo( false );
        return;
    }

    if( 0 > iPartyRank_ || iPartyRank_ >= PARTY_RANK_SIZE ) { 
        return;
    }

    int iExpRank = SiKGCInfinityDungeonManager()->GetExpRate( SiKGCRoomManager()->GetGameMode(), iMonsterLevel_ );
    if( 0 > iExpRank || iExpRank >= EXP_RANK_SIZE ) { 
        return;
    }

    // 파티 랭크
    m_iBeforePartyRank = m_iCurrentPartyRank;
    m_iCurrentPartyRank = iPartyRank_;
    for( int i=0 ; i<PARTY_RANK_SIZE ; ++i ) { 
        if( iPartyRank_ == i )
            m_apkPartyRank[i]->ToggleRender( true );
        else 
            m_apkPartyRank[i]->ToggleRender( false );
    }
    SetInfinityDungenRankParticle();
    

    // 몬스터 레벨 
    m_pkMonsterLevelNum->SetNumber( iMonsterLevel_ );

    // 경험치 효율 
    for( int i=0 ; i<EXP_RANK_SIZE ; ++i ) { 
        if( iExpRank == i ) 
            m_apkExpRank[i]->ToggleRender( true );
        else
            m_apkExpRank[i]->ToggleRender( false );
    }

    // 보스 킬
    int iBossKillNum = 0; 
    g_pkQuestManager->GetComboBossKillPoint( iBossKillNum );
    m_pkBossKillImg->ToggleRender( iBossKillNum > 0 ? true : false );
    m_pkBossKillNum->ToggleRender( iBossKillNum > 0 ? true : false );
}

void KGCComboUI_S6::SetInfinityDungenRankParticle( void )
{
    // 위치 
    D3DXVECTOR2 dxvParticlePos = GetFixedWindowLocalPos();
    dxvParticlePos += m_pkPartyRankImg->GetFixedWindowLocalPos();

    dxvParticlePos.x = PARTICLEPOS_X( dxvParticlePos.x );
    dxvParticlePos.y = PARTICLEPOS_Y( dxvParticlePos.y );

    dxvParticlePos.x += 0.253f;
    dxvParticlePos.y -= 0.020f;

    std::vector< std::string > vecPartiRankParticle;
    if( m_iCurrentPartyRank > m_iBeforePartyRank ) { 
        // 랭크 상승      
        switch( m_iCurrentPartyRank ) { // 랭크따라 이펙트 다르다
            case 1: // E
                vecPartiRankParticle.push_back( "UI_PartyRank_E_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_E_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 2: // D
                vecPartiRankParticle.push_back( "UI_PartyRank_D_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_D_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 3: // C
                vecPartiRankParticle.push_back( "UI_PartyRank_C_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_C_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 4: // B
                vecPartiRankParticle.push_back( "UI_PartyRank_B_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_B_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 5: // A
                vecPartiRankParticle.push_back( "UI_PartyRank_A_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_A_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 6: // S
                vecPartiRankParticle.push_back( "UI_PartyRank_S_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_S_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            case 7: // SS
                vecPartiRankParticle.push_back( "UI_PartyRank_SS_01" );
                vecPartiRankParticle.push_back( "UI_PartyRank_SS_02" );
                vecPartiRankParticle.push_back( "UI_PartyRank_Up" );
                break;
            default:
                break;
        }
    }
    else if( m_iCurrentPartyRank < m_iBeforePartyRank ) { 
        // 랭크 하락 - 하락 이펙트는 한개 
        vecPartiRankParticle.push_back( "UI_PartyRank_Down_01" );
        vecPartiRankParticle.push_back( "UI_PartyRank_Down_02" );
    }

    if( !vecPartiRankParticle.empty() ) { 
        std::vector< std::string >::const_iterator it = vecPartiRankParticle.begin();
        for( ; it != vecPartiRankParticle.end() ; ++it ) { 
            g_ParticleManager->CreateSequenceReceiveName( *it , dxvParticlePos.x, dxvParticlePos.y );
        }
    }
}