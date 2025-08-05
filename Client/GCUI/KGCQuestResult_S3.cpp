#include "StdAfx.h"
#include "KGCQuestResult_S3.h"

#include "GCBonusBoxSelect_S4.h"

#include "KDSound.h"

#include <boost/lexical_cast.hpp>

IMPLEMENT_CLASSNAME( KGCQuestResult_S3 );
IMPLEMENT_WND_FACTORY( KGCQuestResult_S3 );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestResult_S3, "gc_quest_result_s3" );

KGCQuestResult_S3::KGCQuestResult_S3(void)
:m_pkBackgroundMain(NULL)
,m_pkQuestResultTitle(NULL)
,m_pkQuestObtainGPEXP(NULL)
,m_pkMinuteSign(NULL)
,m_pkMinuteSignBest(NULL)
,m_pkBonusAllClear(NULL)
,m_pkBonusItem(NULL)
,m_pkBonusEvent(NULL)
,m_pkBonusPCRoom(NULL)
,m_pkBonusServer(NULL)
,m_pkBounusAnimAllClear(NULL)
,m_pkBonusAnimItem(NULL)
,m_pkBonusAnimEvent(NULL)
,m_pkBonusAnimPCRoom(NULL)
,m_pkBonusAnimServer(NULL)
,m_pkQuestLevelHell(NULL)
,m_pkQuestLevelEventHell(NULL)
,m_pkQuestLevelTime(NULL)
,m_pkQuestLevelEventTime(NULL)
,m_pkTimeOver(NULL)
,m_eAnimScene(EANIM_SCENE_STOP)
,m_iFrameCount(-1)
,m_iAlpha(0)
,m_iStageBonusExp(0)
,m_iStageBonusGp(0)
,m_iTotalExp(0)
,m_iTotalGP(0)
,m_pkBonusBoxSelect( NULL )
,m_dwInitHeight(607)
,m_pkBonusAnimGuild(NULL)
,m_pkBonusGuild(NULL)
,m_bRenderLevelNum(false)
,m_nDungeonLevel(0)
,m_pkLevleText(NULL)
,m_pkBonusBoxS6(NULL)
,m_nRewardType(0)
,m_pkZkey(NULL)
,m_bNewRecordPlayTime(false)
,m_pkClearNewImg(NULL)
,m_pkDamageRankNewImg(NULL)
,m_ClearNewRecordAlpha(0)
,m_DamageNewRecordAlpha(0)
,m_pkBonusAnimRank(NULL)
{
    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EMAX_CONTENTS_BG; ++i )
    {
        m_pkContentsBG[i] = NULL;
        sprintf_s( str, "contents_background%d", i );
        LINK_CONTROL( str, m_pkContentsBG[i] );

        m_pkContentsTitle[i] = NULL;
        sprintf_s( str, "contents_title%d", i );
        LINK_CONTROL( str, m_pkContentsTitle[i] );
    }

    for ( int i = 0; i < EMAX_CONTENTS_SUB_BG; ++i )
    {
        m_pkContentsSubBG[i] = NULL;
        sprintf_s( str, "contents_back_sub%d", i );
        LINK_CONTROL( str, m_pkContentsSubBG[i] );
    }

    for( int i = 0; i < EMAX_LEVEL_STAR; i++ )
    {
        m_pkLevelStarBG[i] = NULL;
        sprintf_s( str, "quest_rank_star_bg%d", i );
        LINK_CONTROL( str, m_pkLevelStarBG[i] );
    }

    LINK_CONTROL( "quest_result_title",         m_pkQuestResultTitle);
    LINK_CONTROL( "background_main",            m_pkBackgroundMain );    
    LINK_CONTROL( "gp_exp",                     m_pkQuestObtainGPEXP );
    LINK_CONTROL( "time_minute_sign",           m_pkMinuteSign );
    LINK_CONTROL( "time_minute_sign_best",      m_pkMinuteSignBest);
    LINK_CONTROL( "quest_result_end_back",      m_pkBackgroundEnd );

    LINK_CONTROL( "bonus_all_stage_clear",      m_pkBonusAllClear );
    LINK_CONTROL( "bonus_item",                 m_pkBonusItem );
    LINK_CONTROL( "bonus_event",                m_pkBonusEvent );
    LINK_CONTROL( "bonus_pc_room",              m_pkBonusPCRoom );
	LINK_CONTROL( "bonus_server",				m_pkBonusServer );	

    LINK_CONTROL( "bonus_anim_stage_clear",     m_pkBounusAnimAllClear );
    LINK_CONTROL( "bonus_anim_item",            m_pkBonusAnimItem );
    LINK_CONTROL( "bonus_anim_event",           m_pkBonusAnimEvent );
    LINK_CONTROL( "bonus_anim_pc_room",         m_pkBonusAnimPCRoom );
    LINK_CONTROL( "bonus_anim_server",          m_pkBonusAnimServer );
#if defined (GUILD_RENEW_S4)
	LINK_CONTROL( "bonus_anim_guild",          m_pkBonusAnimGuild );
    LINK_CONTROL( "bonus_guild",				m_pkBonusGuild );
#endif
    LINK_CONTROL( "bonus_anim_rank",            m_pkBonusAnimRank);

    for( int i = 0; i < EMAX_BONUS_VITALITY; i++ )
    {
        m_pkBonusVitality[i] = NULL;
        sprintf_s( str, "bonus_vitality%d", i );
        LINK_CONTROL( str, m_pkBonusVitality[i] );
    }
    LINK_CONTROL( "bonus_box_select_s4",           m_pkBonusBoxSelect );

    LINK_CONTROL( "time_over",                  m_pkTimeOver );
    

    LINK_CONTROL( "quest_level_hell",           m_pkQuestLevelHell );
    LINK_CONTROL( "quest_level_event_hell",     m_pkQuestLevelEventHell );
    LINK_CONTROL( "quest_level_time",           m_pkQuestLevelTime );
    LINK_CONTROL( "quest_level_event_time",     m_pkQuestLevelEventTime );
    
    LINK_CONTROL( "level_text",                 m_pkLevleText );

	m_pkStaticSkip = NULL;
	LINK_CONTROL( "static_skip",                 m_pkStaticSkip );
	LINK_CONTROL( "bonus_box_s6",m_pkBonusBoxS6 );
    LINK_CONTROL( "z_key", m_pkZkey );
    LINK_CONTROL( "clear_new_record", m_pkClearNewImg );
    LINK_CONTROL( "damage_rank_new_record", m_pkDamageRankNewImg );
}

KGCQuestResult_S3::~KGCQuestResult_S3(void)
{
}

void KGCQuestResult_S3::OnCreate(void)
{
    m_sBGMoveProperty.Init();
    m_sPlayTime.Init();
    m_sPrevPlayTime.Init();
    m_iOriginalExp = 0;
    m_iOriginalGP = 0;
    m_iAlpha = 0;
    m_iBonusAnimCount = 0;
    m_iFrameCount = -1;
    m_iEndDelayTime = EDELAY_END_TIME;
    m_mapBonusExpInfo.clear();
    m_mapBonusGPInfo.clear();
    m_iOriginalSPExp = 0;
    m_mapBonusSPExpInfo.clear();

    m_pkBackgroundMain->InitState(true);
    m_pkBackgroundMain->SetWindowPosX( 800 * GC_SCREEN_DIV_WIDTH );        
    m_pkBackgroundEnd->InitState(false);
    m_pkQuestObtainGPEXP->InitState(false);

    m_pkBounusAnimAllClear->InitState(true);
    m_pkBonusAnimItem->InitState(true);
    m_pkBonusAnimEvent->InitState(true);
    m_pkBonusAnimPCRoom->InitState(true);
#if !defined( NATION_IDN )
	m_pkBonusAnimServer->InitState(true);
#endif
#ifdef GUILD_RENEW_S4
    m_pkBonusAnimGuild->InitState(true);
    m_pkBonusGuild->InitState(false);
    m_pkBonusGuild->SetWndMode(D3DWM_DEFAULT);
#endif




    m_pkBonusAllClear->InitState(false);
    m_pkBonusAllClear->SetWndMode(D3DWM_DEFAULT);
    m_pkBonusItem->InitState(false);
    m_pkBonusItem->SetWndMode(D3DWM_DEFAULT);
    m_pkBonusEvent->InitState(false);
    m_pkBonusEvent->SetWndMode(D3DWM_DEFAULT);
    m_pkBonusPCRoom->InitState(false);
#if !defined( NATION_USA ) && !defined( NATION_EU )
    m_pkBonusPCRoom->SetWndMode(D3DWM_DEFAULT);
#endif

	m_pkBonusServer->InitState(false);
	m_pkBonusServer->SetWndMode(D3DWM_DEFAULT);

    for( int i = 0; i < EMAX_BONUS_VITALITY; i++ )
    {
        if ( m_pkBonusVitality[i] != NULL )
        {
            m_pkBonusVitality[i]->InitState(false);
            m_pkBonusVitality[i]->SetWndMode(D3DWM_DEFAULT);
        }
    }
    
    m_pkQuestResultTitle->InitState(false);
    m_pkMinuteSign->InitState(false);
    m_pkMinuteSignBest->InitState(false);

    m_pkQuestLevelHell->InitState( false );
    m_pkQuestLevelEventHell->InitState( false );
    m_pkQuestLevelTime->InitState( false );
    m_pkQuestLevelEventTime->InitState( false );

    for( int i = 0; i < EMAX_CONTENTS_BG; i++ )
    {
        m_pkContentsBG[i]->InitState(false);
        m_pkContentsTitle[i]->InitState(false);
    }

    for( int i = 0; i < EMAX_CONTENTS_SUB_BG; i++ )
    {
        if ( m_pkContentsSubBG[i] != NULL )
            m_pkContentsSubBG[i]->InitState(false);
    }

    for( int i = 0; i < EMAX_LEVEL_STAR; i++ )
        m_pkLevelStarBG[i]->InitState(false);

    //초기 알파값 셋팅
    SetWndAlpha(0);
    InitQuestLevel();    

    m_pkBonusBoxSelect->InitState( false, true, this );

	//static bool bFirst = true;
	//if ( bFirst )
	//{
 //       m_dwInitHeight = GetChildByName("background_main")->GetHeight();
	//	GetChildByName("contents_background4")->SetHeight( GetChildByName("contents_background4")->GetHeight() - 31 );
	//	bFirst = false;
	//}

 //   GetChildByName("background_main")->SetHeight( m_dwInitHeight - 31 );

    if( m_pkTimeOver != NULL )
        m_pkTimeOver->ToggleRender(false);

    if ( m_pkLevleText ) 
        m_pkLevleText->InitState(false);

	if (m_pkStaticSkip)
	{
		m_pkStaticSkip->InitState(true);
		m_pkStaticSkip->SetText(g_pkStrLoader->GetString( STR_ID_PUSH_Z_FOR_SKIP ));
	}

	if ( m_pkBonusBoxS6 ) {
		m_pkBonusBoxS6->InitState(false,false,NULL);
	}

	m_nRewardType = 0;
    m_pkZkey->InitState(false, false, NULL);
    m_pkClearNewImg->InitState(false, false, NULL);
    m_pkDamageRankNewImg->InitState(false, false, NULL);
    
    m_pkBonusAnimRank->InitState(false);
    m_pkBonusAnimRank->SetWndMode(D3DWM_DEFAULT);

    m_ClearNewRecordAlpha = 0;
    m_DamageNewRecordAlpha = 0;
    m_bNewRecordPlayTime = false;
}

void KGCQuestResult_S3::OnCreateComplete(void)
{
}

void KGCQuestResult_S3::ActionPerformed( const KActionEvent& event )
{
}

void KGCQuestResult_S3::OnDestroy( void )
{
}

void KGCQuestResult_S3::OnDestroyComplete(void)
{
}

void KGCQuestResult_S3::FrameMoveInEnabledState(void)
{
    if( m_eAnimScene == EANIM_SCENE_STOP )
        return;

#if defined( USE_ONGAME_INVENTORY )
    g_MyD3D->MyCtrl->k_Game_Inventory = false;
#endif

    static int iDelayTime = 0;

	SkipSet();

    switch(m_eAnimScene)
    {
    case EANIM_SCENE_BACKGROUND:
        AnimationBackground(EANIM_SCENE_MAIN_CONTENTS, true);
        break;
    case EANIM_SCENE_MAIN_CONTENTS:
        AnimationMainContents(true);
        break;
    case EANIM_SCENE_RECORD_CHECK:
        {
            if ( m_bNewRecordPlayTime )
                AnimationNewRecord(0, EALPHA_WEIGHT);

            int iRank = SiKGCWorldMapManager()->GetCurrentDungeonRank(true);
            if (iRank <= 0 )
                iRank = 0;

            if ( g_pkQuestManager->GetRankNum() > iRank )
                AnimationNewRecord(1, EALPHA_WEIGHT);

            if ( CanMoveNextScene() )
                m_eAnimScene = EANIM_SCENE_BONUS;
        }
        break;
    case EANIM_SCENE_BONUS:
        if( IsCompleteBonusAnim() && m_pkQuestObtainGPEXP->isComplete() )
        {
            if( !g_pkQuestManager->m_bWin )
            {
                m_iAlpha = 0;
                m_pkBackgroundEnd->SetAllWindowAlpha(0);
                m_pkBackgroundEnd->ToggleRender(true);
                m_eAnimScene = EANIM_SCENE_END_GAME;
            }
            else
            {
                m_iAlpha = 255;
                SetMoveBackground(GC_SCREEN_DIV_SIZE_INT(420), 0, 0, 0, 10, m_eAnimScene);
                m_eAnimScene = EANIM_SCENE_RESULT_END;
            }
        }
        break;
    case EANIM_SCENE_RESULT_END:       
		if ( m_nRewardType == KEndGameAck::RESULT_REWARD )
        {
            m_pkBackgroundMain->ToggleRender(false);
			m_pkBackgroundEnd->ToggleRender(false);
			RenderContents(false);

            m_pkClearNewImg->ToggleRender(false);
            m_pkDamageRankNewImg->ToggleRender(false);

			m_eAnimScene = EANIM_SCENE_SELECT_BONUS_BOX;
		}
        else if ( m_nRewardType == KEndGameAck::NONE_REWARD || m_nRewardType == KEndGameAck::STAGE_REWARD )
        {
            m_pkBackgroundMain->ToggleRender(false);
            m_pkBackgroundEnd->ToggleRender(false);
            RenderContents(false);

            m_pkClearNewImg->ToggleRender(false);
            m_pkDamageRankNewImg->ToggleRender(false);

            m_eAnimScene = EANIM_SCENE_END_GAME;
            m_iEndDelayTime = 55;
        }
        else
        {
			iDelayTime++;
			if( iDelayTime <= m_iEndDelayTime )
				break;

			iDelayTime = 0;
			m_iEndDelayTime = -1;

			AnimEndResultWnd(EALPHA_WEIGHT * 2);
			AnimationBackground(EANIM_SCENE_SELECT_BONUS_BOX, false);
		}
        break;
    case EANIM_SCENE_SELECT_BONUS_BOX:
        {
            m_pkClearNewImg->ToggleRender(false);
            m_pkDamageRankNewImg->ToggleRender(false);
#if defined( USE_NEWBIE_GUIDE )
            if ( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL && SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST0 ) {
                g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(false);
                g_pkGameOverlayUI->m_pkAddtionGuide->EndTrialForestDungeon();
            }
#endif
            if ( m_nRewardType == KEndGameAck::RESULT_REWARD ){
				if( false == m_pkBonusBoxS6->IsRenderOn() ) {
					m_pkBonusBoxS6->SetTime();
					m_pkBonusBoxS6->ToggleRender( true );
				}

				if ( m_pkBonusBoxS6->IsComplete() ) {
					m_iAlpha = 0;
					m_pkBackgroundEnd->SetAllWindowAlpha(0);
					m_pkBackgroundEnd->ToggleRender(false);
					m_pkBonusBoxS6->ToggleRender( false );
					m_eAnimScene = EANIM_SCENE_END_GAME;
					m_pkBonusBoxS6->SetComplete(false);
					m_iEndDelayTime = 55;
				}

			}
			else{
                if ( m_nRewardType == KEndGameAck::NONE_REWARD || m_nRewardType == KEndGameAck::STAGE_REWARD )
                {
                    m_iAlpha = 0;
                    m_pkBackgroundEnd->SetAllWindowAlpha(0);
                    m_pkBackgroundEnd->ToggleRender(true);
                    m_pkBonusBoxSelect->ToggleRender(false);
                    m_eAnimScene = EANIM_SCENE_END_GAME;
                }
                else
                {
                    if (false == m_pkBonusBoxSelect->IsRenderOn()) {
                        m_pkBonusBoxSelect->ToggleRender(true);
                    }

                    if (m_pkBonusBoxSelect->IsAllProcessComplete())
                    {
                        m_iAlpha = 0;
                        m_pkBackgroundEnd->SetAllWindowAlpha(0);
                        m_pkBackgroundEnd->ToggleRender(true);
                        m_pkBonusBoxSelect->ToggleRender(false);
                        m_eAnimScene = EANIM_SCENE_END_GAME;
                    }
                }
			}


        }
        break;
    case EANIM_SCENE_END_GAME:
#if !defined( NO_USE_ADDITION_GUIDE )
		if (g_pkGameOverlayUI->m_pkAddtionGuide)
			g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(false);
#endif
        AnimationEndBackground( EALPHA_WEIGHT );
        break;
    }    
}

bool KGCQuestResult_S3::IsCompleteBonusAnim()
{
    std::stringstream str;    

    std::map<int, float>::iterator mit;

    bool bJump=true;
    bool bCheck=false;
    switch( m_iBonusAnimCount )
    {
    case 0: //스테이지 보너스
        bCheck=true;
        if( m_iStageBonusExp != 0 )
            m_pkQuestObtainGPEXP->SetEXP( m_pkQuestObtainGPEXP->GetExp() + m_iStageBonusExp );
        if( m_iStageBonusGp != 0 )
            m_pkQuestObtainGPEXP->SetGP( m_pkQuestObtainGPEXP->GetGp() + m_iStageBonusGp );

        if( g_pkQuestManager->GetTotalStageNum() == g_pkQuestManager->GetNumOfClearStage() )
        {
            m_pkBounusAnimAllClear->SetLetter( " EXP\n+10%", 0.5f);
            m_pkBounusAnimAllClear->SetAnimMoveY( -20, 1, true );
            //m_iEndDelayTime += 20;
            m_pkBounusAnimAllClear->SetLetter( " GP\n+10%", 0.5f);
            m_pkBounusAnimAllClear->SetAnimMoveY( -20, 1, true );
            bJump = false;
        }

        if( g_pkQuestManager->m_bWin )
            m_pkBonusAllClear->SetWndMode(D3DWM_SELECT);

        break;
	case 20: //서버
        bCheck=true;
		mit = m_mapBonusExpInfo.find((int)KCharacterExpInfoOut::BT_SERVER);

		if( mit != m_mapBonusExpInfo.end() )
		{
			if( mit->second != 0.0f )
			{
				str.str("");
				str<<" EXP\n+"<<(int)(mit->second * 100.f)<<"%";

				m_pkBonusAnimServer->SetLetter( str.str(), 0.5f);
				m_pkBonusAnimServer->SetAnimMoveY( -20, 1, true );
				bJump = false;

				m_pkBonusServer->SetWndMode(D3DWM_SELECT);

				GCEXPTYPE iCurExp  = m_pkQuestObtainGPEXP->GetExp();
				if( iCurExp < m_iOriginalExp )
					iCurExp = m_iOriginalExp;
				m_pkQuestObtainGPEXP->SetEXP( iCurExp + static_cast<int>(m_iOriginalExp * mit->second) );
			}
		}

		mit = m_mapBonusGPInfo.find((int)KCharacterExpInfoOut::BT_SERVER);

		if( mit != m_mapBonusGPInfo.end() )
		{
			if( mit->second != 0.0f )
			{
				str.str("");
				str<<" GP\n+"<<(int)(mit->second * 100.f)<<"%";

				m_pkBonusAnimServer->SetLetter( str.str(), 0.5f);
				m_pkBonusAnimServer->SetAnimMoveY( -20, 1, true );
				bJump = false;

				m_pkBonusServer->SetWndMode(D3DWM_SELECT);

				int iCurGP = m_pkQuestObtainGPEXP->GetGp();
				m_pkQuestObtainGPEXP->SetGP( iCurGP + static_cast<int>(m_iOriginalGP * mit->second) );
			}
		}

		mit = m_mapBonusSPExpInfo.find((int)KCharacterExpInfoOut::BT_SERVER);

		if( mit != m_mapBonusSPExpInfo.end() )
		{
			if( mit->second != 0.0f )
			{
				str.str("");
				str<<"SPEXP\n+%d%"<<(int)(mit->second * 100.f);

				m_pkBonusAnimServer->SetLetter( str.str(), 0.5f);
				m_pkBonusAnimServer->SetAnimMoveY( -20, 1, true );
				bJump = false;

				m_pkBonusServer->SetWndMode(D3DWM_SELECT);
			}
		}
		break;
    case 40: //아이템 보너스
        bCheck=true;
        mit = m_mapBonusExpInfo.find((int)KCharacterExpInfoOut::BT_ITEM);

        if( mit != m_mapBonusExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" EXP\n+"<<(int)(mit->second * 100.f)<<"%";
                m_pkBonusAnimItem->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimItem->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusItem->SetWndMode(D3DWM_SELECT);

                GCEXPTYPE iCurExp  = m_pkQuestObtainGPEXP->GetExp();
                m_pkQuestObtainGPEXP->SetEXP( static_cast<GCEXPTYPE>(iCurExp + (m_iOriginalExp * mit->second)) );
            }
        }

        mit = m_mapBonusGPInfo.find((int)KCharacterExpInfoOut::BT_ITEM);

        if( mit != m_mapBonusGPInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" GP\n+"<<(int)(mit->second * 100.f)<<"%";

                m_pkBonusAnimItem->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimItem->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusItem->SetWndMode(D3DWM_SELECT);

                int iCurGP = m_pkQuestObtainGPEXP->GetGp();
                m_pkQuestObtainGPEXP->SetGP( iCurGP + static_cast<int>(m_iOriginalGP * mit->second) );
            }
        }

        mit = m_mapBonusSPExpInfo.find((int)KCharacterExpInfoOut::BT_ITEM);

        if( mit != m_mapBonusSPExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<"SPEXP\n+%d%"<<(int)(mit->second * 100.f);

                m_pkBonusAnimItem->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimItem->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusItem->SetWndMode(D3DWM_SELECT);
            }
        }
        break;
    case 60: //이벤트 보너스
        bCheck=true;
        mit = m_mapBonusExpInfo.find((int)KCharacterExpInfoOut::BT_EVENT);

        if( mit != m_mapBonusExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                float itemp = mit->second * 100.f;
                str.str("");
                str<<" EXP\n+"<< ROUND_OFF (itemp)<<"%";

                m_pkBonusAnimEvent->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimEvent->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusEvent->SetWndMode(D3DWM_SELECT);
                GCEXPTYPE iCurExp  = m_pkQuestObtainGPEXP->GetExp();
                m_pkQuestObtainGPEXP->SetEXP( iCurExp + static_cast<int>(m_iOriginalExp * mit->second) );
            }
        }

        mit = m_mapBonusGPInfo.find((int)KCharacterExpInfoOut::BT_EVENT);

        if( mit != m_mapBonusGPInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" GP\n+"<<(int)(mit->second * 100.f)<<"%";

                m_pkBonusAnimEvent->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimEvent->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusEvent->SetWndMode(D3DWM_SELECT);

                int iCurGP = m_pkQuestObtainGPEXP->GetGp();
                m_pkQuestObtainGPEXP->SetGP( iCurGP + static_cast<int>(m_iOriginalGP * mit->second) );
            }
        }

        mit = m_mapBonusSPExpInfo.find((int)KCharacterExpInfoOut::BT_EVENT);

        if( mit != m_mapBonusSPExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<"SPEXP\n+%d%"<<(int)(mit->second * 100.f);

                m_pkBonusAnimItem->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimItem->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusItem->SetWndMode(D3DWM_SELECT);
            }
        }
        break;
#if !defined( NATION_USA ) && !defined( NATION_EU )
    case 80: //PC방 보너스
        bCheck=true;


		//////////////////////////////////////////////////////////////////////////
		//	old
         mit = m_mapBonusExpInfo.find((int)KCharacterExpInfoOut::BT_PC);
 
         if( mit != m_mapBonusExpInfo.end() )
         {
             if( mit->second != 0.0f )
             {
                 str.str("");
                 str<<" EXP\n+"<<(int)(mit->second * 100.f)<<"%";
 
                 m_pkBonusAnimPCRoom->SetLetter( str.str(), 0.5f);
                 m_pkBonusAnimPCRoom->SetAnimMoveY( -20, 1, true );
                 bJump = false;
 
                 m_pkBonusPCRoom->SetWndMode(D3DWM_SELECT);
 
                 GCEXPTYPE iCurExp  = m_pkQuestObtainGPEXP->GetExp();
                 m_pkQuestObtainGPEXP->SetEXP( iCurExp + static_cast<int>(m_iOriginalExp * mit->second) );
             }
         }
		//////////////////////////////////////////////////////////////////////////
		//	new
		//	private: void SetBonusAnimationString

		//	argument
// 		{
// 			std::map<int, float>& mapBonusInfo_				=	m_mapBonusExpInfo;
// 			KCharacterExpInfoOut::BonusType enBonusType_	=	KCharacterExpInfoOut::BT_PC;
// 			KGCScoreAnimUI*&	scoreAnimUI_				=	m_pkBonusAnimPCRoom;
// 			KD3DWnd*&			outWindow_					=	m_pkBonusPCRoom;
// 			std::string			strText_("What the hell");
// 			bool&				bJump_						=	bJump;
// 
// 			typedef GCEXPTYPE (KGCQuestObtainGPEXP::*GetValueFunction)();
// 			typedef void (KGCQuestObtainGPEXP::*SetValueFunction)( GCEXPTYPE, bool );
// 
// 			GetValueFunction	GetFunc	=	&KGCQuestObtainGPEXP::GetExp;
// 			SetValueFunction	SetFunc	=	&KGCQuestObtainGPEXP::SetEXP;
// 
// 			//	procedure
// 			std::map<int, float>::iterator& itrBonusInfo	=	mapBonusInfo_.find( enBonusType_ );
// 			if(		( itrBonusInfo			!= mapBonusInfo_.end() )
// 				&&	( itrBonusInfo->second	!= 0.0f )
// 				){
// 					static const float	fScale		= 0.5f;
// 					static const float	fMovePosY	= -20.0f;
// 					static const float	fWeight		= 1.0f;
// 					static const bool	bAlphaAnim	= true;
// 
// 					scoreAnimUI_->SetLetter( strText_, fScale );
// 					scoreAnimUI_->SetAnimMoveY( fMovePosY, fWeight, bAlphaAnim );
// 
// 					outWindow_->SetWndMode( D3DWM_SELECT );
// 
// 					bJump_	=	false;
// 
// 					int iCurrentValue = static_cast<int>( ((*m_pkQuestObtainGPEXP).*GetFunc)() );
// 					iCurrentValue += static_cast<int>(m_iOriginalExp * itrBonusInfo->second );
// 					((*m_pkQuestObtainGPEXP).*SetFunc)( iCurrentValue, false );
// 			}
// 
// 		}
		//////////////////////////////////////////////////////////////////////////




        mit = m_mapBonusGPInfo.find((int)KCharacterExpInfoOut::BT_PC);

        if( mit != m_mapBonusGPInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" GP\n+"<<(int)(mit->second * 100.f)<<"%";

                m_pkBonusAnimPCRoom->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimPCRoom->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusPCRoom->SetWndMode(D3DWM_SELECT);

                int iCurGP = m_pkQuestObtainGPEXP->GetGp();
                m_pkQuestObtainGPEXP->SetGP( iCurGP + static_cast<int>(m_iOriginalGP * mit->second) );
            }
        }

		
        mit = m_mapBonusSPExpInfo.find((int)KCharacterExpInfoOut::BT_PC);

        if( mit != m_mapBonusSPExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<"SPEXP\n+%d%"<<(int)(mit->second * 100.f);

                m_pkBonusAnimItem->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimItem->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusItem->SetWndMode(D3DWM_SELECT);
            }
        }
		break;
#endif

	case 100:
        if ( g_pMagicEffect->IsMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_VITALITY_BUFF ) )
        {
            for( int i = 0; i < EMAX_BONUS_VITALITY; i++ )
            {
                if ( m_pkBonusVitality[i] != NULL )
                    m_pkBonusVitality[i]->SetWndMode(D3DWM_SELECT);
            }
        }
#if defined ( GUILD_RENEW_S4 )
        bCheck=true;
        
		if ( g_pGameBoard->m_dwContributionPoint > 0 )
        {
            str.str("");
            str<< "+" << static_cast <int> (g_pGameBoard->m_dwContributionPoint);

		    m_pkBonusAnimGuild->SetLetter( str.str(), 0.5f);
		    m_pkBonusAnimGuild->SetAnimMoveY( -20, 1, true );
		    bJump = false;

		    m_pkBonusGuild->SetWndMode(D3DWM_SELECT);
        }

        mit = m_mapBonusExpInfo.find((int)KCharacterExpInfoOut::BT_GUILD);

        if( mit != m_mapBonusExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" EXP\n+"<<(int)(mit->second * 100.f)<<"%";

                m_pkBonusAnimGuild->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimGuild->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusGuild->SetWndMode(D3DWM_SELECT);

                GCEXPTYPE iCurExp  = m_pkQuestObtainGPEXP->GetExp();
                m_pkQuestObtainGPEXP->SetEXP( iCurExp + static_cast<int>(m_iOriginalExp * mit->second) );
            }
        }

        mit = m_mapBonusGPInfo.find((int)KCharacterExpInfoOut::BT_GUILD);

        if( mit != m_mapBonusGPInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<" GP\n+"<<(int)(mit->second * 100.f)<<"%";

                m_pkBonusAnimGuild->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimGuild->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusGuild->SetWndMode(D3DWM_SELECT);

                int iCurGP = m_pkQuestObtainGPEXP->GetGp();
                m_pkQuestObtainGPEXP->SetGP( iCurGP + static_cast<int>(m_iOriginalGP * mit->second) );
            }
        }


        mit = m_mapBonusSPExpInfo.find((int)KCharacterExpInfoOut::BT_GUILD);

        if( mit != m_mapBonusSPExpInfo.end() )
        {
            if( mit->second != 0.0f )
            {
                str.str("");
                str<<"SPEXP\n+%d%"<<(int)(mit->second * 100.f);

                m_pkBonusAnimGuild->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimGuild->SetAnimMoveY( -20, 1, true );
                bJump = false;

                m_pkBonusGuild->SetWndMode(D3DWM_SELECT);
            }
        }
#endif
        break;
    case 120:
        {
            // 에휴.. 소수점 오류때문에 제대로 안나온다... 마지막에 보정 해주쟈;;
            m_pkQuestObtainGPEXP->SetGP( m_iTotalGP);
            m_pkQuestObtainGPEXP->SetEXP( m_iTotalExp );
        }
        break;

    case 140:
        {
            int iPercent = 0;
            GetExpPercentForRank( g_pkQuestManager->GetRankNum() , iPercent);

            if ( iPercent != 0) {
                str.str("");
                str<<"EXP\n+"<<(int)(iPercent) <<"%";

                m_pkBonusAnimRank->SetLetter( str.str(), 0.5f);
                m_pkBonusAnimRank->SetAnimMoveY( -20, 1, true );
                bJump = false;
            }
        }
        break;

    }    

    if (bCheck==true && bJump==true)
    {
        m_iBonusAnimCount+=20;
    }
    else
        m_iBonusAnimCount++;

    if( m_iEndDelayTime <= m_iBonusAnimCount )
        return true;
    else
        return false;
}

void KGCQuestResult_S3::SetQuestResult()
{
    m_nDungeonLevel = SiKGCRoomManager()->GetDungeonLevel();
    SetQuestLevel( m_nDungeonLevel );    
    m_iStageBonusExp = 0;
    m_iStageBonusGp = 0;

    g_KMci.Stop();
    g_KMci.Play(GC_BGM_GAME_WIN, false);

    g_pkQuestManager->m_bSendRouletteList = true;
    g_pkQuestManager->m_dwRouletteWaitDelay = timeGetTime();        

    CountPlayTime();
}

void KGCQuestResult_S3::SetMoveBackground( int iStartX, int iStartY, int iEndX, int iEndY, int iSpeed, int iSceneType )
{
    //현재는 x이동밖에 쓰지않지만, 혹시나 해서 y이동에 관련한 변수들도 추가함.
    m_sBGMoveProperty.m_iStartX = iStartX;
    m_sBGMoveProperty.m_iStartY = iStartY;
    m_sBGMoveProperty.m_iEndX   = iEndX;
    m_sBGMoveProperty.m_iEndY   = iEndY;
    m_sBGMoveProperty.m_iSpeed  = iSpeed;
    m_sBGMoveProperty.m_iMoveX  = iStartX;
    m_sBGMoveProperty.m_iMoveY  = iStartY;
    m_eAnimScene = (EAnimScene)iSceneType;    
}

void KGCQuestResult_S3::InitQuestLevel()
{
    for ( int i = 0; i < EMAX_LEVEL_STAR; ++i ) 
    {
        m_pkLevelStarBG[i]->ToggleRender(false);
        m_pkLevelStarBG[i]->SetWndMode( D3DWM_DEFAULT );
    }

    if( m_pkLevleText )
        m_pkLevleText->ToggleRender( false );

    m_bRenderLevelNum = false;
}


void KGCQuestResult_S3::SetQuestLevel( int nDungeonLevel )
{

    EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();
    // 릴레이 영웅던전 이면....
    if ( SiKGCWorldMapManager()->IsRelayHeroDungeon( emGameMode ) )
    {
        m_bRenderLevelNum = true;


        m_pkQuestLevelHell->ToggleRender(false);
        m_pkQuestLevelEventHell->ToggleRender(false);
        m_pkQuestLevelTime->ToggleRender(false);
        m_pkQuestLevelEventTime->ToggleRender(false);
        for ( int i = 0; i < EMAX_LEVEL_STAR; ++i ) 
        {
            m_pkLevelStarBG[i]->ToggleRender(false);
        }
    }
    else if ( SiKGCWorldMapManager()->IsInfinityDungeon( emGameMode ) )
    {
        m_bRenderLevelNum = true;

        m_pkQuestLevelHell->ToggleRender(false);
        m_pkQuestLevelEventHell->ToggleRender(false);
        m_pkQuestLevelTime->ToggleRender(false);
        m_pkQuestLevelEventTime->ToggleRender(false);
        for ( int i = 0; i < EMAX_LEVEL_STAR; ++i ) 
        {
            m_pkLevelStarBG[i]->ToggleRender(false);
        }
    }
    //  일반 던전
    else
    {
        int iMaxNum = 0;
        ED3DWndMode eWndMode = D3DWM_DEFAULT;
        m_bRenderLevelNum = false;

        switch( nDungeonLevel )
        {
        case GC_DUNGEON_LEVEL_HELL:
            iMaxNum = 2;
            eWndMode = D3DWM_SELECT;
			m_bRenderLevelNum = true;
            if( SiKGCWorldMapManager()->IsEventDungeon() )
            {
                m_pkQuestLevelEventHell->ToggleRender(true);
            }
            else
            {
                //난이도ui 챔피언으로 변경 해줄 구문
                m_pkQuestLevelHell->ToggleRender(true);
            }
            break;
        case GC_DUNGEON_LEVEL_BREAK:
            iMaxNum = 2;
            eWndMode = D3DWM_SELECT;
            m_bRenderLevelNum = true;
            if (SiKGCWorldMapManager()->IsEventDungeon())
            {
                m_pkQuestLevelEventTime->ToggleRender(true);
            }
            else
            {
                //난이도ui 챔피언으로 변경 해줄 구문
                m_pkQuestLevelTime->ToggleRender(true);
            }
            break;
        default:
            iMaxNum = nDungeonLevel;
            eWndMode = D3DWM_SELECT;
            m_pkQuestLevelHell->ToggleRender(false);
            m_pkQuestLevelEventHell->ToggleRender(false);
            m_pkQuestLevelTime->ToggleRender(false);
            m_pkQuestLevelEventTime->ToggleRender(false);
            break;
        }
		m_pkQuestLevelHell->SetAllWindowAlpha(0);
		m_pkQuestLevelEventHell->SetAllWindowAlpha(0);

        for ( int i = 0; i < EMAX_LEVEL_STAR; ++i ) 
        {
            m_pkLevelStarBG[i]->ToggleRender(!m_bRenderLevelNum);
			m_pkLevelStarBG[i]->SetAllWindowAlpha(0);

            if ( i <= iMaxNum ) 
            {
                m_pkLevelStarBG[i]->SetWndMode(eWndMode);                
            }
        }

        if( m_pkLevleText )
            m_pkLevleText->ToggleRender( false );
    }
}

void KGCQuestResult_S3::SetWndAlpha( int iAlpha_ )
{
    m_pkQuestObtainGPEXP->SetAllWindowAlpha(iAlpha_);
    m_pkQuestObtainGPEXP->SetExpGpColor(D3DCOLOR_RGBA( 255, 255, 255, iAlpha_ ));
    m_pkQuestResultTitle->SetAllWindowAlpha(iAlpha_);
	m_pkStaticSkip->SetAllWindowAlpha(iAlpha_);
    m_pkMinuteSign->SetAllWindowAlpha(iAlpha_);
    m_pkMinuteSignBest->SetAllWindowAlpha(iAlpha_);
    if( m_pkTimeOver != NULL )
        m_pkTimeOver->SetAllWindowAlpha(iAlpha_);
    m_pkBonusAllClear->SetAllWindowAlpha(iAlpha_);
    m_pkBonusAnimRank->SetAllWindowAlpha(iAlpha_);
    m_pkBonusItem->SetAllWindowAlpha(iAlpha_);
    m_pkBonusEvent->SetAllWindowAlpha(iAlpha_);
#ifdef GUILD_RENEW_S4
    m_pkBonusGuild->SetAllWindowAlpha(iAlpha_);
#endif


#if !defined( NATION_USA ) && !defined( NATION_EU )
    m_pkBonusPCRoom->SetAllWindowAlpha(iAlpha_);
#endif
	m_pkBonusServer->SetAllWindowAlpha(iAlpha_);

    for( int i = 0; i < EMAX_BONUS_VITALITY; i++ )
    {
        if ( m_pkBonusVitality[i] != NULL )
            m_pkBonusVitality[i]->SetAllWindowAlpha(iAlpha_);
    }

    m_pkQuestLevelHell->SetAllWindowAlpha(iAlpha_);
    m_pkQuestLevelEventHell->SetAllWindowAlpha(iAlpha_);
    m_pkQuestLevelTime->SetAllWindowAlpha(iAlpha_);
    m_pkQuestLevelEventTime->SetAllWindowAlpha(iAlpha_);
    
    for( int i = 0; i < EMAX_CONTENTS_BG; i++ )
    {
        m_pkContentsBG[i]->SetAllWindowAlpha(iAlpha_);
        m_pkContentsTitle[i]->SetAllWindowAlpha(iAlpha_);
    }

    for( int i = 0; i < EMAX_CONTENTS_SUB_BG; i++ )
    {
        if ( m_pkContentsSubBG[i] != NULL )
            m_pkContentsSubBG[i]->SetAllWindowAlpha(iAlpha_);
    }

    for( int i = 0; i < EMAX_LEVEL_STAR; i++ )
        m_pkLevelStarBG[i]->SetAllWindowAlpha(iAlpha_);


    if( m_pkLevleText )
        m_pkLevleText->SetAllWindowAlpha( iAlpha_ );
    m_pkZkey->SetAllWindowAlpha(iAlpha_);
    m_pkClearNewImg->SetAllWindowAlpha(iAlpha_);
    m_pkDamageRankNewImg->SetAllWindowAlpha(iAlpha_);
}

// exp 정보 셋팅
void KGCQuestResult_S3::SetExp( DWORD dwPlayerUID, GCEXPTYPE biExp )
{
    m_iOriginalExp = biExp;

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if( dwPlayerUID == g_kGlobalValue.m_kUserInfo.dwUID )
        {
            m_pkQuestObtainGPEXP->SetEXP( biExp, true );
        }
    }
}

void KGCQuestResult_S3::SetLocalUserStageBonusExp( GCEXPTYPE biExp )
{
    m_iStageBonusExp = biExp;
}

void KGCQuestResult_S3::SetLocalUserBonusExpInfo( std::map<int, float>& mapInfo_ )
{
    m_mapBonusExpInfo = mapInfo_;
}

// gp 정보 셋팅
void KGCQuestResult_S3::SetGp( DWORD dwPlayerUID, int iGp )
{
    m_iOriginalGP = iGp;

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        if( dwPlayerUID == g_kGlobalValue.m_kUserInfo.dwUID )
        {
            m_pkQuestObtainGPEXP->SetGP( iGp, true );
        }
    }
}

void KGCQuestResult_S3::SetLocalUserStageBonusGp( int iGp )
{
    m_iStageBonusGp = iGp;
}

void KGCQuestResult_S3::SetLocalUserBonusGpInfo( std::map<int, float>& mapInfo_ )
{
    m_mapBonusGPInfo = mapInfo_;
}

void KGCQuestResult_S3::SetLocalUserBonusSPExpInfo( std::map<int, float>& mapInfo_ )
{
    m_mapBonusSPExpInfo = mapInfo_;
}

void KGCQuestResult_S3::CountPlayTime()
{
    DWORD dwTime = g_pGameBoard->GetPlayTimeInFrame();

    dwTime/=55;
    DWORD minute=(dwTime/60);
    DWORD sec=dwTime%60;

    m_sPlayTime.m_iMinute1 = (int)(minute / 10);
    m_sPlayTime.m_iMinute2 = (int)(minute % 10);
    m_sPlayTime.m_iSecond1 = (int)(sec / 10);
    m_sPlayTime.m_iSecond2 = (int)(sec % 10);

    DWORD dwPrevTime = GetDungeonPrevClearRecord();
    
    if ( dwPrevTime > 6000 ) {
        m_sPrevPlayTime.m_iMinute1 = 9;
        m_sPrevPlayTime.m_iMinute2 = 9;
        m_sPrevPlayTime.m_iSecond1 = 9;
        m_sPrevPlayTime.m_iSecond2 = 9;
    }
    else {
        minute=(dwPrevTime/60);
        sec=dwPrevTime%60;

        m_sPrevPlayTime.m_iMinute1 = (int)(minute / 10);
        m_sPrevPlayTime.m_iMinute2 = (int)(minute % 10);
        m_sPrevPlayTime.m_iSecond1 = (int)(sec / 10);
        m_sPrevPlayTime.m_iSecond2 = (int)(sec % 10);
    }

    //비교
    if ( dwTime < dwPrevTime || dwPrevTime == 0)
        m_bNewRecordPlayTime = true;
}

void KGCQuestResult_S3::RenderContents( bool bRender_ )
{
    m_pkMinuteSign->ToggleRender(bRender_);
    m_pkMinuteSignBest->ToggleRender(bRender_);
    m_pkQuestObtainGPEXP->ToggleRender(bRender_);
    m_pkQuestResultTitle->ToggleRender(bRender_);
	m_pkStaticSkip->ToggleRender(bRender_);
    m_pkBonusAllClear->ToggleRender(bRender_);
    m_pkBonusAnimRank->ToggleRender(bRender_);
    m_pkBonusItem->ToggleRender(bRender_);
    m_pkBonusEvent->ToggleRender(bRender_);
#ifdef GUILD_RENEW_S4
	m_pkBonusGuild->ToggleRender(bRender_);
#endif
#if !defined( NATION_USA ) && !defined( NATION_EU )
	m_pkBonusPCRoom->ToggleRender(bRender_);
#endif
	m_pkBonusServer->ToggleRender(bRender_);

    for( int i = 0; i < EMAX_BONUS_VITALITY; i++ )
    {
        if ( m_pkBonusVitality[i] != NULL )
            m_pkBonusVitality[i]->ToggleRender(bRender_);
    }

    for( int i = 0; i < EMAX_CONTENTS_BG; i++ )
    {
        m_pkContentsBG[i]->ToggleRender(bRender_);
        m_pkContentsTitle[i]->ToggleRender(bRender_);
    }

    for( int i = 0; i < EMAX_CONTENTS_SUB_BG; i++ )
    {
        if ( m_pkContentsSubBG[i] != NULL )
            m_pkContentsSubBG[i]->ToggleRender(bRender_);
    }

	if( m_pkLevleText )
	{
		EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();
		if ( SiKGCWorldMapManager()->IsRelayHeroDungeon( emGameMode ) && bRender_)
		{
			m_pkLevleText->ToggleRender( true );
		}
		else
		{
			m_pkLevleText->ToggleRender( false );
		}
	}
	
    m_pkZkey->ToggleRender(bRender_);
    SetLevelBGRender( bRender_ );
}

void KGCQuestResult_S3::SetLevelBGRender( bool bRender_ )
{
    if ( m_bRenderLevelNum ) 
        bRender_ = false;

    for( int i = 0; i < EMAX_LEVEL_STAR; i++ )    
        m_pkLevelStarBG[i]->ToggleRender(bRender_);
}

void KGCQuestResult_S3::AnimationBackground( int iNextSceneType, bool bWndRender )
{
    //시작지점이 도착지점보다 작을 때로 일반적으로 위치가 늘어날 때
    if( m_sBGMoveProperty.m_iStartX < m_sBGMoveProperty.m_iEndX )
    {
        m_sBGMoveProperty.m_iMoveX += m_sBGMoveProperty.m_iSpeed;

        // 스피드를 더한값이 목적지위치를 넘어서면 안된다.
        if( m_sBGMoveProperty.m_iMoveX > m_sBGMoveProperty.m_iEndX )
           m_sBGMoveProperty.m_iMoveX = m_sBGMoveProperty.m_iEndX;
    }
    else if( m_sBGMoveProperty.m_iStartX > m_sBGMoveProperty.m_iEndX )
    {
        m_sBGMoveProperty.m_iMoveX -= m_sBGMoveProperty.m_iSpeed;

        // 스피드를 더한값이 목적지위치를 넘어서면 안된다.
        if( m_sBGMoveProperty.m_iMoveX < m_sBGMoveProperty.m_iEndX )
            m_sBGMoveProperty.m_iMoveX = m_sBGMoveProperty.m_iEndX;
    }
    
    if( m_sBGMoveProperty.m_iMoveX == m_sBGMoveProperty.m_iEndX )
    {
        m_eAnimScene = (EAnimScene)iNextSceneType;     
        SetWndAlpha(0);
        RenderContents(bWndRender);
    }

    m_pkBackgroundMain->SetWindowPosX( (float)m_sBGMoveProperty.m_iMoveX );

    if ( iNextSceneType == EANIM_SCENE_SELECT_BONUS_BOX)
        AdjustWnd();
}

void KGCQuestResult_S3::AnimationMainContents(bool bIncreaseAlpha_ )
{
    if( bIncreaseAlpha_ )
        m_iAlpha += (int)EALPHA_WEIGHT;
    else
        m_iAlpha -= (int)EALPHA_WEIGHT;

    if( m_iAlpha > EMAX_ALPHA_VALUE )
    {
        m_iAlpha = (int)EMAX_ALPHA_VALUE;
        g_MyD3D->WaitForServerAck(Result_EndGame, INT_MAX, 3000, TIME_OUT_VALUE );
        g_KDSound.Play("Dungeon_result_Pop_up_back");
        //다음씬으로
        m_eAnimScene = EANIM_SCENE_PLAY_TIME;
        m_iFrameCount = 0;
    }
    else if( m_iAlpha < 0 )
    {
        m_iAlpha = 0;
        //다음씬으로
        g_MyD3D->WaitForServerAck(Result_EndGame, INT_MAX, 3000, TIME_OUT_VALUE );
        g_KDSound.Play("Dungeon_result_Pop_up_back");
        m_eAnimScene = EANIM_SCENE_PLAY_TIME;
        m_iFrameCount = 0;
    }

    SetWndAlpha(m_iAlpha);
}

void KGCQuestResult_S3::AnimEndResultWnd( int iWeight )
{
    m_iAlpha -= iWeight;

    if( m_iAlpha < 0 )
    {
        m_iAlpha = 0;        
    }

    SetWndAlpha(m_iAlpha);
}

void KGCQuestResult_S3::AnimationEndBackground( int iWeight )
{    
    static int iDelayTime = 0;
    iDelayTime++;
    if( iDelayTime <= m_iEndDelayTime )
        return;

    m_iAlpha += iWeight;
    // 알파값이 다 채워지면 게임을 끝내자.
    if( m_iAlpha > EMAX_ALPHA_VALUE || m_nRewardType == KEndGameAck::RESULT_REWARD || m_nRewardType == KEndGameAck::NONE_REWARD || m_nRewardType == KEndGameAck::STAGE_REWARD )
    {
        m_iAlpha = (int)EMAX_ALPHA_VALUE;        
        iDelayTime = 0;
        m_eAnimScene = EANIM_SCENE_STOP;
		if ( m_nRewardType == KEndGameAck::RESULT_REWARD ) {
			m_pkBonusBoxS6->ToggleRender(false);		
		}
		else {
			m_pkBonusBoxSelect->InitUIs();
		}
        g_pStateGame->EndGame();
    }
    m_pkBackgroundEnd->SetAllWindowAlpha(m_iAlpha);
}

void KGCQuestResult_S3::PostChildDraw(void)
{
    if( m_eAnimScene == EANIM_SCENE_STOP )
        return;

    float fPosY;
#if defined(NATION_CHINA)
    fPosY = -33.0f;
#else
    fPosY = 0.0f;
#endif

    D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();
    D3DXVECTOR2 vPos1( 670 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 207 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos2( 690 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 207 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos3( 735 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 207 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos4( 755 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 207 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );

    D3DXVECTOR2 vPos5( 670 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 240 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos6( 690 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 240 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos7( 735 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 240 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos8( 755 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 240 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );

    D3DXVECTOR2 vPos9( 750 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 285 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
    D3DXVECTOR2 vPos10( 750 * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 320 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );

    static int iAlpha = (int)EMAX_ALPHA_VALUE;
    if( m_eAnimScene == EANIM_SCENE_RESULT_END && iAlpha > 0 )
    {
        iAlpha = m_iAlpha;
        if( iAlpha < 0 )
            iAlpha = 0;
    }

	if( m_eAnimScene >= EANIM_SCENE_SELECT_BONUS_BOX )
		iAlpha = 0;

    if( m_eAnimScene == EANIM_SCENE_PLAY_TIME )
    {
        iAlpha = (int)EMAX_ALPHA_VALUE;
        m_iFrameCount++;    
    }

    static DWORD dwMinute = 0;
    if( m_iFrameCount == 0 )
    {
        dwMinute = g_pGameBoard->GetPlayTimeInFrame()/55/60;
    }
        
    if( dwMinute > 60 )
    {
        m_pkMinuteSign->ToggleRender(false);
        if( m_iFrameCount == 20 )
        {
            if( m_pkTimeOver != NULL )
                m_pkTimeOver->ToggleRender(true);
        }
    }
    else
    {
        //현재 시간
        if( m_iFrameCount >= 1 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos1, m_sPlayTime.m_iMinute1, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount >= 10 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos2, m_sPlayTime.m_iMinute2, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount == 20 )
            m_pkMinuteSign->ToggleRender(true);
        if( m_iFrameCount >= 30 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos3, m_sPlayTime.m_iSecond1, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount >= 40 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos4, m_sPlayTime.m_iSecond2, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        //과거시간 기록 표기
        if( m_iFrameCount >= 50 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos5, m_sPrevPlayTime.m_iMinute1, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount >= 60 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos6, m_sPrevPlayTime.m_iMinute2, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount == 70 )
            m_pkMinuteSignBest->ToggleRender(true);
        if( m_iFrameCount >= 80 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos7, m_sPrevPlayTime.m_iSecond1, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount >= 90 )
            g_pkUIMgr->RenderNumber( "orange_gradation_num", vPos + vPos8, m_sPrevPlayTime.m_iSecond2, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);

        //랭크 표기
        if( m_iFrameCount >= 100 )
            g_pkUIMgr->RenderNumber( "dungeon_damage_rank_num", vPos + vPos9, g_pkQuestManager->GetRankNum(), D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        if( m_iFrameCount >= 110 ) {
            int iRank = SiKGCWorldMapManager()->GetCurrentDungeonRank(true);

            if ( iRank < 0 )
                iRank = 0;

            g_pkUIMgr->RenderNumber( "dungeon_damage_rank_num", vPos + vPos10, iRank , D3DCOLOR_RGBA( 255, 255, 255, iAlpha ),true, false, -1.0f, true);
        }
    }    

    if( m_iFrameCount == 115 && m_eAnimScene == EANIM_SCENE_PLAY_TIME )
    {
        m_eAnimScene = EANIM_SCENE_RECORD_CHECK;
    }

    if ( m_bRenderLevelNum ) 
    {
		if (m_pkLevleText->IsRenderOn())
		{
			int nLevel = m_nDungeonLevel + 1;
			int nXpos = 740;

			std::string strNumber = boost::lexical_cast<string>( nLevel );
			int nLen = strNumber.length();

			nXpos -= ( nLen * 30 ) / 2;


			D3DXVECTOR2 vPosNum( nXpos * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 160 * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH );
			g_pkUIMgr->RenderNumberScale( "orange_gradation_num", vPos + vPosNum, nLevel, 0.75f, D3DCOLOR_RGBA( 255, 255, 255, iAlpha ), false );
		}  
    }
}

void KGCQuestResult_S3::AdjustWnd( void )
{
    GetChildByName("background_main")->SetHeight( m_dwInitHeight + ADD_HEIGHT_RESULT_END );
}

void KGCQuestResult_S3::SkipSet()
{
	if ( g_pkQuestManager->IsNextSceneKeyUp() )
	{
		switch(m_eAnimScene)
		{
		case EANIM_SCENE_BACKGROUND:
			m_sBGMoveProperty.m_iMoveX = m_sBGMoveProperty.m_iEndX;
			break;
		case EANIM_SCENE_MAIN_CONTENTS:
			m_iAlpha = EMAX_ALPHA_VALUE;
			break;
		case EANIM_SCENE_PLAY_TIME:
			m_iFrameCount = 114;
			break;
        case EANIM_SCENE_RECORD_CHECK:
            SkipNewRecord();
            m_eAnimScene = EANIM_SCENE_BONUS;
            break;
		case EANIM_SCENE_BONUS:
			while( m_iBonusAnimCount < m_iEndDelayTime )
            {
				IsCompleteBonusAnim();
            }

            if( !g_pkQuestManager->m_bWin )
            {
                m_iAlpha = 0;
                m_pkBackgroundEnd->SetAllWindowAlpha(0);
                m_pkBackgroundEnd->ToggleRender(true);
                m_eAnimScene = EANIM_SCENE_END_GAME;
            }
            else
            {
				m_iAlpha = 255;
				SetMoveBackground(GC_SCREEN_DIV_SIZE_INT(420), 0, 0, 0, 10, m_eAnimScene);
				m_eAnimScene = EANIM_SCENE_RESULT_END;
            }
			break;
		case EANIM_SCENE_RESULT_END:
			m_iEndDelayTime = 0;
			m_iAlpha = -1;
			m_sBGMoveProperty.m_iMoveX = m_sBGMoveProperty.m_iEndX - m_sBGMoveProperty.m_iSpeed -1;
			GetChildByName("background_main")->SetHeight(m_dwInitHeight + ADD_HEIGHT_RESULT_END);
			break;
		case EANIM_SCENE_SELECT_BONUS_BOX:
			if ( m_nRewardType == KEndGameAck::RESULT_REWARD ){
				m_pkBonusBoxS6->SetComplete( true );
			}
			else {
				m_pkBonusBoxSelect->SetEnd();
			}
			break;
		case EANIM_SCENE_END_GAME:
			break;
		}
	}
	
}

void KGCQuestResult_S3::SetRewardItem( std::map<int, KDropItemInfo> mapRewardItem, int iChar, std::wstring strNickName )
{
    m_pkBonusBoxS6->SetRewardItem( mapRewardItem, iChar, strNickName );
}

DWORD KGCQuestResult_S3::GetDungeonPrevClearRecord()
{

    EGCGameMode eGameMode = SiKGCRoomManager()->GetGameMode();

    std::map<int,KDungeonUserInfo>::iterator mIter  =  g_kGlobalValue.m_kUserInfo.mapStagePlayable.find( eGameMode );

    if ( mIter != g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ) {
        std::map< int, KUsersDungeonRecordData >::iterator mRecordIter = mIter->second.m_kClearData.m_mapBestRecord.find( SiKGCRoomListManager()->GetCurrentLevel() );

        if ( mRecordIter != mIter->second.m_kClearData.m_mapBestRecord.end() ) {
            return mRecordIter->second.m_dwPrevBestClearTime;
        }
    }

    return 0;
}

void KGCQuestResult_S3::AnimationNewRecord( int iType , int iWeight )
{
    if ( iType == 0 && g_pkQuestManager->m_bWin) {
        m_ClearNewRecordAlpha += iWeight;
        m_pkClearNewImg->ToggleRender(true);
        m_pkClearNewImg->SetAllWindowAlpha( m_ClearNewRecordAlpha);
    }
    else if (iType == 1 && g_pkQuestManager->m_bWin){
        m_DamageNewRecordAlpha += iWeight;
        m_pkDamageRankNewImg->ToggleRender(true);
        m_pkDamageRankNewImg->SetAllWindowAlpha( m_DamageNewRecordAlpha);
    }
}

bool KGCQuestResult_S3::CanMoveNextScene()
{
    bool bRankRecord = false;
    bRankRecord  = g_pkQuestManager->GetRankNum() > SiKGCWorldMapManager()->GetCurrentDungeonRank(true) ? true : false;

    if ( !g_pkQuestManager->m_bWin )
        return true;

    if ( m_bNewRecordPlayTime && bRankRecord ) {
        if ( m_ClearNewRecordAlpha >= EMAX_ALPHA_VALUE && m_DamageNewRecordAlpha >= EMAX_ALPHA_VALUE) 
            return true;
        else 
            return false;
    }

    if ( (m_bNewRecordPlayTime && m_ClearNewRecordAlpha >= EMAX_ALPHA_VALUE ) && !bRankRecord)
        return true;

    if ( (bRankRecord && m_DamageNewRecordAlpha >= EMAX_ALPHA_VALUE ) && !m_bNewRecordPlayTime)
        return true;

    if ( !m_bNewRecordPlayTime && !bRankRecord )
        return true;

    return false;
}

void KGCQuestResult_S3::SkipNewRecord( )
{
    if ( m_bNewRecordPlayTime ) {
        m_pkClearNewImg->ToggleRender(true);
        m_pkClearNewImg->SetAllWindowAlpha( EMAX_ALPHA_VALUE);
    }
    if ( g_pkQuestManager->GetRankNum() > SiKGCWorldMapManager()->GetCurrentDungeonRank(true) ) {
        m_pkDamageRankNewImg->ToggleRender(true);
        m_pkDamageRankNewImg->SetAllWindowAlpha( EMAX_ALPHA_VALUE);
    }
}

void KGCQuestResult_S3::GetExpPercentForRank(IN int iRankIndex , OUT int& iPercent)
{
    switch (iRankIndex)
    {
    case 4:
        iPercent = 3;
        break;
    case 5:
        iPercent = 6;
        break;
    case 6:
        iPercent = 10;
        break;
    case 7:
        iPercent = 15;
        break;
    default:
        iPercent = 0;
        break;
    }
}