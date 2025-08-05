#include "stdafx.h"
//
#include ".\gcskilltreedlg.h"

//
//
#include "GCSKT.h"
#include "gcskill.h"
//
//
//

//
//
#include "gcui/GCMyInfoScene.h"
#include "GCSkillBoxLock.h"
#include "MyD3D.h"
#include "../GCStateMachine.h"
#include "GCSkilltreeField.h"
#include "GCCharacterUI.h"
#include "GCDescBox.h"
#include "gcui/GCCashShopScene.h"
#include "gcui/GCGPShopScene.h"

#include "Player.h"
#include "gcui/GCSkillSlotBox.h"
#include "GCSkillResource.h"


IMPLEMENT_CLASSNAME( KGCSkillTreeDlg );
IMPLEMENT_WND_FACTORY( KGCSkillTreeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillTreeDlg, "gc_skill_tree" );

KGCSkillTreeDlg::KGCSkillTreeDlg(void)
: m_iSelectedChar( 0 )
, m_iPromotion( 0 )
, m_iSetID( 0 )
, m_bFirstCreate( true )
, m_bFieldFlip( false )
, m_bEnable( false )
, m_bProcedureUpdateCall( false )
, m_pkPreviewSkill( NULL )
, m_pkPreviewSkillBox( NULL )
, m_bActivePassive( false )
, m_bIsSelectedSkillB( false )
{
	m_pkBtnSkillLeft				= NULL;
	m_pkBtnSkillRight				= NULL;
	m_pkBtnReset 					= NULL;
	m_pkBtnClose					= NULL;
	m_pkCharMaxSp 					= NULL;
	m_pkCharMaxSpText 				= NULL;
	m_pkCurrentSkillLevel 			= NULL;
	m_pkCurrentSp 					= NULL;
	m_pkCurrentSpText 				= NULL;
	m_pkMaxSp 						= NULL;
	m_pkMaxSpText 					= NULL;
	m_pkBtnModeChange 				= NULL;
	m_pkTabNormalSkill 				= NULL;
	m_pkTabNormalSkillAct 			= NULL;
	m_pkTabSpecial 					= NULL;
	m_pkTabSpecialAct 				= NULL;
	m_pkTabCommonSkill				= NULL;
	m_pkTabCommonSkillAct			= NULL;
	m_pkTabTransform				= NULL;
	m_pkTabTransformAct				= NULL;
	m_pkUsingSp 					= NULL;
	m_pkUsingSpText 				= NULL;
	m_pkCharacterName 				= NULL;
	m_pkComboSkillSet				= NULL;
	m_pkSkilltreeField[0]			= NULL;
	m_pkSkilltreeField[1]			= NULL;
	m_pkSkillLevelGuage				= NULL;
	m_pkSkillLevelGuageBack			= NULL;
	m_pkDescBox						= NULL;
	m_pkLeftField					= NULL;
	m_pkRightField					= NULL;
	m_pkSkillRemovePoint			= NULL;
	m_pkSkillRemovePointText		= NULL;
	m_pkLockerField					= NULL;
	m_pkCharacterLevel				= NULL;
	m_pkAlertCommon					= NULL;
	m_pkAlertNormal					= NULL;
	m_pkAlertSpecial				= NULL;
	m_pkCurrentSkillExpPercent		= NULL;
	m_pkNextSkillDesc				= NULL;
	m_pkPreviewSkillField			= NULL;
	m_pkFlipButton[0]				= NULL;
	m_pkFlipButton[1]				= NULL;
    m_pkTypeSwapBtn                 = NULL;
	m_pkTabActiveSkill				= NULL;
	m_pkTabActiveSkillAct			= NULL;
	m_pkTabPassiveSkill				= NULL;
	m_pkTabPassiveSkillAct			= NULL;
    m_pkBackAsin                    = NULL;
    m_pkStaticAsin                  = NULL;
    m_pkCommingSoon                 = NULL;


	// ��ũ��Ʈ�� ������ ��ųƮ���� �ִ� �ɸ��͸� �˾Ƴ��ϴ�.
	InitOpenChar();

    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        m_pkPromotion[i] = NULL;
        char temp[MAX_PATH];

        if( i<NUM_JOB_LEVEL ) { 
            sprintf_s( temp, MAX_PATH-1, "btn_promotion%d", i );
            LINK_CONTROL( temp, m_pkPromotion[i] );
        }
        else { 
            LINK_CONTROL( "btn_promotion1_2", m_pkPromotion[i] );
        }
    }

    m_bOpenNewChar = false;

	LINK_CONTROL( "btn_skill_left",				m_pkBtnSkillLeft );
	LINK_CONTROL( "btn_skill_right",			m_pkBtnSkillRight );
	LINK_CONTROL( "btn_reset", 					m_pkBtnReset );
	LINK_CONTROL( "btn_mode_change", 			m_pkBtnModeChange );
	LINK_CONTROL( "btn_close",					m_pkBtnClose );
	LINK_CONTROL( "tab_normalskill", 			m_pkTabNormalSkill );
	LINK_CONTROL( "tab_normalskill_act", 		m_pkTabNormalSkillAct );
	LINK_CONTROL( "tab_special", 				m_pkTabSpecial );
	LINK_CONTROL( "tab_special_act", 			m_pkTabSpecialAct );
	LINK_CONTROL( "tab_commonskill", 			m_pkTabCommonSkill );
	LINK_CONTROL( "tab_commonskill_act", 		m_pkTabCommonSkillAct );
	LINK_CONTROL( "tab_transform", 				m_pkTabTransform );
	LINK_CONTROL( "tab_transform_act", 			m_pkTabTransformAct );
	LINK_CONTROL( "transform_background", 		m_pkTransFormMenu );
	
	
	LINK_CONTROL( "static_char_maxsp", 			m_pkCharMaxSp );
	LINK_CONTROL( "static_char_maxsp_text", 	m_pkCharMaxSpText );
	LINK_CONTROL( "static_currentskill_level", 	m_pkCurrentSkillLevel );
	LINK_CONTROL( "static_current_sp", 			m_pkCurrentSp );
	LINK_CONTROL( "static_current_sp_text", 	m_pkCurrentSpText );
	LINK_CONTROL( "static_maxsp", 				m_pkMaxSp );
	LINK_CONTROL( "static_maxsp_text", 			m_pkMaxSpText );
	LINK_CONTROL( "static_usingsp", 			m_pkUsingSp );
	LINK_CONTROL( "static_usingsp_text", 		m_pkUsingSpText );
	LINK_CONTROL( "static_current_skill_exp",	m_pkCurrentSkillExpPercent );
	LINK_CONTROL( "static_nextskill_desc",		m_pkNextSkillDesc );
	LINK_CONTROL( "caracter_name", 				m_pkCharacterName );
	LINK_CONTROL( "combo_skillset",				m_pkComboSkillSet );
	LINK_CONTROL( "field_skilltree1",			m_pkSkilltreeField[0] );
	LINK_CONTROL( "field_skilltree2",			m_pkSkilltreeField[1] );
	LINK_CONTROL( "skill_level_guage",			m_pkSkillLevelGuage );
	LINK_CONTROL( "skill_level_guage_back",		m_pkSkillLevelGuageBack );
	LINK_CONTROL( "skill_info_box",				m_pkDescBox );
	LINK_CONTROL( "frame_left_lower",			m_pkLeftField );
	LINK_CONTROL( "frame_right_lower",			m_pkRightField );
	LINK_CONTROL( "frame_between_lower",		m_pkbetweenField );
	LINK_CONTROL( "static_reset_sp",			m_pkSkillRemovePoint );
	LINK_CONTROL( "static_reset_sp_text",		m_pkSkillRemovePointText );
	LINK_CONTROL( "field_locker",				m_pkLockerField );
	LINK_CONTROL( "static_character_level", 	m_pkCharacterLevel );
	LINK_CONTROL( "alert_normal",				m_pkAlertNormal );
	LINK_CONTROL( "alert_special",				m_pkAlertSpecial );
	LINK_CONTROL( "alert_etc",					m_pkAlertCommon );
	LINK_CONTROL( "field_preview_skill",		m_pkPreviewSkillField );
	LINK_CONTROL( "btn_flip_right",				m_pkFlipButton[0] );
	LINK_CONTROL( "btn_flip_left",				m_pkFlipButton[1] );

	LINK_CONTROL( "btnTypeSwap",				m_pkTypeSwapBtn );


	LINK_CONTROL( "skillSlotBox",				m_pkSkillSlotBox );

	
	LINK_CONTROL( "tab_active_skill",			m_pkTabActiveSkill );
	LINK_CONTROL( "tab_active_skill_act",		m_pkTabActiveSkillAct );
	LINK_CONTROL( "tab_passive_skill",			m_pkTabPassiveSkill );
	LINK_CONTROL( "tab_passive_skill_act",		m_pkTabPassiveSkillAct );

    LINK_CONTROL( "back_asin_only",		        m_pkBackAsin );
    LINK_CONTROL( "static_asin_title",		    m_pkStaticAsin );
    LINK_CONTROL( "comming_soon",		        m_pkCommingSoon );

    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) { 
        m_apkWaitImg[i] = NULL;

        char szTemp[128];
        sprintf_s( szTemp, 127, "img_wait_%d", i );
        LINK_CONTROL( szTemp, m_apkWaitImg[i] );
    }

	m_pkLevelDrawPos = NULL;
	LINK_CONTROL( "level_draw_rect",		    m_pkLevelDrawPos );

}

KGCSkillTreeDlg::~KGCSkillTreeDlg(void)
{
}

void KGCSkillTreeDlg::OnCreate( void )
{
	m_pkTransFormMenu->InitState(false,true,this);
	m_pkTabNormalSkill->InitState( false, true, this );
	m_pkTabNormalSkillAct->InitState( true, true, this );
	m_pkTabSpecial->InitState( true, true, this );
	m_pkTabSpecialAct->InitState( false, true, this );
	m_pkTabTransform->InitState(false,true,this);
	m_pkTabTransformAct->InitState(false,true,this);
	m_pkTabCommonSkill->InitState( true, true, this );
	m_pkTabCommonSkillAct->InitState( false, true, this );
	m_pkBtnSkillLeft->InitState( true, true, this );
	m_pkBtnSkillRight->InitState( true, true, this );
	m_pkBtnReset->InitState( false, true, this );
	m_pkBtnModeChange->InitState( true, true, this );
	m_pkBtnClose->InitState( true, true, this );
	m_pkCharMaxSp->InitState( true );
	m_pkCharMaxSpText->InitState( true );
	m_pkCurrentSkillLevel->InitState( true );
	m_pkCurrentSp->InitState( true );
	m_pkCurrentSpText->InitState( true );
	m_pkMaxSp->InitState( true );
	m_pkMaxSpText->InitState( true );
	m_pkUsingSp->InitState( true );
	m_pkUsingSpText->InitState( true );
	m_pkCharacterName->InitState( true, true );
	m_pkComboSkillSet->InitState( true, true, this );
	m_pkSkillLevelGuage->InitState( true );
	m_pkSkillLevelGuageBack->InitState( true );
	m_pkDescBox->InitState( false, true );
	m_pkCharacterLevel->InitState( true );
	m_pkCurrentSkillExpPercent->InitState( true );
	m_pkNextSkillDesc->InitState( true );

	m_pkTabActiveSkill->InitState( true, true, this );
	m_pkTabActiveSkillAct->InitState( true, true, this );
	m_pkTabPassiveSkill->InitState( true, true, this );
	m_pkTabPassiveSkillAct->InitState( true, true, this );

	m_pkAlertNormal->InitState( false );
	m_pkAlertSpecial->InitState( false );
	m_pkAlertCommon->InitState( false );
	m_pkFlipButton[0]->InitState( false, true, this );
	m_pkFlipButton[0]->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SWAPBTN1 ) );
	m_pkFlipButton[1]->InitState( false, true, this );
	m_pkFlipButton[1]->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SWAPBTN2 ) );
    m_pkTypeSwapBtn->InitState( false, true, this );
    m_pkTypeSwapBtn->SetToolTip( g_pkStrLoader->GetString( 5382 ) );
	m_pkPreviewSkillField->InitState( true );

	m_pkCurrentSp->SetAlign( DT_CENTER );
	m_pkCurrentSpText->SetAlign( DT_CENTER );
	m_pkMaxSp->SetAlign( DT_CENTER );
	m_pkMaxSpText->SetAlign( DT_CENTER );
	m_pkUsingSp->SetAlign( DT_CENTER );
	m_pkUsingSpText->SetAlign( DT_CENTER );
	m_pkCharMaxSp->SetAlign( DT_CENTER );
	m_pkCharMaxSpText->SetAlign( DT_CENTER );
	m_pkCharacterLevel->SetAlign( DT_CENTER );

    if( m_pkBackAsin != NULL && m_pkStaticAsin != NULL ) {
        m_pkBackAsin->ToggleRender( false );
        m_pkStaticAsin->ToggleRender( false );
        m_pkStaticAsin->SetAlign( DT_CENTER );        
    }    

    if( m_pkPreviewSkillBox == NULL )
    {
        m_pkPreviewSkillBox = (KGCSkillBox*)g_pkUIMgr->CreateTemplateUI( m_pkPreviewSkillField, "gc_skillbox", "ui_skillbox.stg", "skillbox", true, true );
        m_pkPreviewSkillBox->Create( m_pkPreviewSkillField->IsAutoCreate() );
		m_pkPreviewSkillBox->InitState( true, true, this );
    }

	
	//*/

    m_pkCharMaxSpText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SKILLTREE_CURRENT_MAX_SP ));
    m_pkCurrentSpText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SKILLTREE_CURRENT_SP ));
    m_pkMaxSpText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SKILLTREE_MAX_SP ));
    m_pkUsingSpText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SKILLTREE_USING_SP ));
	m_pkCharacterLevel->SetText(g_pkStrLoader->GetString( STR_ID_LEVEL ));

	m_pkCharMaxSpText->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE19 ) );
	m_pkCurrentSpText->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE16 ) );
	m_pkMaxSpText->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE17 ) );
	m_pkUsingSpText->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE18 ) );

	m_pkSkillRemovePoint->SetAlign( DT_CENTER );
	m_pkSkillRemovePointText->SetAlign( DT_CENTER );
	m_pkSkillRemovePointText->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_SKILLTREE_RESET_POINT) );

	m_pkComboSkillSet->DeleteAllItem();
	m_pkComboSkillSet->AddString( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SKILLSET1 ) );
	m_pkComboSkillSet->AddString( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SKILLSET2 ) );

	m_pkBtnModeChange->Lock( true );
	m_pkBtnModeChange->ToggleRender( false );

	m_pkPreviewSkillField->SetWidth( m_pkPreviewSkillBox->GetWidth() );
	m_pkPreviewSkillField->SetHeight( m_pkPreviewSkillBox->GetHeight() );

	m_pkCommingSoon->ToggleRender( false );



	m_pkSkillSlotBox->InitState(false,true,this);

	static bool bFirst = true;
	if ( bFirst )
	{
		KD3DWnd* pWnd = NULL;
		int i = 0;
		while( pWnd = GetChild( i ) )		
		{
			if ( pWnd->GetFixedWindowLocalPos().y < 125.0f )
			{
				pWnd->SetFixedWindowLocalPos( D3DXVECTOR2( pWnd->GetFixedWindowLocalPos().x, pWnd->GetFixedWindowLocalPos().y + 20.0f ));
			}
			i++;
		}
		bFirst = false;
		GetChildByName("upper_frame")->SetHeight( GetChildByName("upper_frame")->GetHeight() - 20 );

		m_pkSkilltreeField[0]->SetFixedWindowLocalPos( D3DXVECTOR2(0.0f,0.0f ));
		m_pkSkilltreeField[1]->SetFixedWindowLocalPos( D3DXVECTOR2(0.0f,0.0f ));


	}

	

	GetChildByName("skill_level_guage_back")->ToggleRender( false );
	GetChildByName("skill_level_guage")->ToggleRender( false );
	GetChildByName("upper_skilllevel_frame")->ToggleRender( false );
	
	m_pkCurrentSkillExpPercent->ToggleRender( false );
	m_pkCurrentSkillLevel->ToggleRender( false );    

    GetChildByName( "tab_ultimate_skill" )->InitState( false, true, this );
    GetChildByName( "tab_ultimate_skill_act" )->InitState( false, true, this );
    GetChildByName( "tab_fightstance_skill" )->InitState( false, true, this );
    GetChildByName( "tab_fightstance_skill_act" )->InitState( false, true, this );

    GetChildByName( "tab_aura_skill" )->InitState( false, true, this );
    GetChildByName( "tab_aura_skill_act" )->InitState( false, true, this );
    GetChildByName( "tab_selfaura_skill" )->InitState( false, true, this );
    GetChildByName( "tab_selfaura_skill_act" )->InitState( false, true, this );

    GetChildByName( "tab_weapon_skill" )->InitState( false, true, this );
    GetChildByName( "tab_weapon_skill_act" )->InitState( false, true, this );

    GetChildByName( "tab_devil_skill" )->InitState( false, true, this );
    GetChildByName( "tab_devil_skill_act" )->InitState( false, true, this );

	if ( GetChildByName( "wnd_dio_devil" ) )
		GetChildByName( "wnd_dio_devil" )->SetToolTip( g_pkStrLoader->GetString( STR_ID_DIO_DEVIL_SKILL_DESC ) );
	if (GetChildByName( "wnd_dio_weapon" ))
		GetChildByName( "wnd_dio_weapon" )->SetToolTip( g_pkStrLoader->GetString( STR_ID_DIO_WEAPON_SKILL_DESC ) );

    GetChildByName( "tab_goddess_skill" )->InitState( false, true, this );
    GetChildByName( "tab_goddess_skill_act" )->InitState( false, true, this );

    GetChildByName( "tab_evil_skill" )->InitState( false, true, this );
    GetChildByName( "tab_evil_skill_act" )->InitState( false, true, this );

    GetChildByName( "tab_rin_transform" )->InitState( false, true, this );
    GetChildByName( "tab_rin_transform_act" )->InitState( false, true, this );

    GetChildByName( "tab_special_type2" )->InitState( false, true, this );
    GetChildByName( "tab_special_type2_act" )->InitState( false, true, this );

	m_pkLevelDrawPos->InitState(false);

// 	if (g_MyD3D->GetMyPlayer())
// 	{
// 		g_MyD3D->GetMyPlayer()->UpdateTempPlayerUserInfo();
// 	}
}

void KGCSkillTreeDlg::FrameMoveInEnabledState( void )
{
	if(g_pkInput->IsDown( DIK_ESCAPE ))
	{
        if( g_pkUIScene->m_pkNewTutorialOption != NULL ){
            if( g_pkUIScene->m_pkNewTutorialOption->IsRenderOn() ){
                g_pkUIScene->m_pkNewTutorialOption->SetCancle( true );
            }
        }
		OnClickClose();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	}

	FrameMove_FlipCheck();	
	FrameMove_BlinkAlert();

	if( m_bProcedureUpdateCall )
	{
		Update();
		m_bProcedureUpdateCall = false;
	}

}

void KGCSkillTreeDlg::ActionPerformed( const KActionEvent& event )
{
    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        if( m_mapOpenCharIndex.find( m_iSelectedChar ) == m_mapOpenCharIndex.end() ) { 
            continue;
        }
        if( m_pkPromotion[i] == NULL ) { 
            continue;
        }

        GCWND_WND_CODE_MAP( m_pkPromotion[i], OnClickCharPromotion );
    }

    GCWND_MSG_MAP( m_pkTabSpecial,		KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickSpeicalTab );
	GCWND_MSG_MAP( m_pkTabNormalSkill,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickNormalSkillTab );
	GCWND_MSG_MAP( m_pkTabCommonSkill,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickCommonSkillTab );
	GCWND_MSG_MAP( m_pkTabTransform,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickTransformSkillTab );
	
	GCWND_MSG_MAP( m_pkTabActiveSkill,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickActiveTab );
	GCWND_MSG_MAP( m_pkTabPassiveSkill,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickPassiveTab );

	GCWND_MSG_MAP( m_pkBtnClose,		KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickClose );
	GCWND_MSG_MAP( m_pkBtnReset,		KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickReset );
	GCWND_MSG_MAP( m_pkBtnSkillLeft,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickSkillLeft );
	GCWND_MSG_MAP( m_pkBtnSkillRight,	KD3DWnd::D3DWE_BUTTON_CLICK,	OnClickSkillRight );
	GCWND_MSG_MAP( m_pkComboSkillSet,	KD3DWnd::D3DWE_COMBO_SELCHANGE, OnChangeSkillSet );

	GCWND_WND_CODE_MAP( m_pkFlipButton[0],	OnClickFlipBtn );
	GCWND_WND_CODE_MAP( m_pkFlipButton[1],	OnClickFlipBtn );

    if( event.m_pWnd == m_pkTypeSwapBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnClickSwapBtn();

	if( event.m_pWnd == (KD3DWnd*)m_pkPreviewSkillBox )
	{
		if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
		{
			if ( m_pkPreviewSkillBox && m_pkPreviewSkill )
			{
				KGCSkilltreeField::OnSetDescBoxInfo( m_pkPreviewSkill->GetID(), m_pkPreviewSkillBox );	
			}
			else
			{
				m_pkDescBox->ToggleRender( false );
			}
			
		}
		else if( event.m_dwCode ==KD3DWnd::D3DWE_CURSOR_LEAVE )
			m_pkDescBox->ToggleRender( false );
		
	}
    
}


void KGCSkillTreeDlg::OnClickTransformSkillTab()
{
	OnTransformTabUI();

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
    
	SetPassiveTabClick( false );
    ReloadSkillTree( "Transform" );
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();

}


void KGCSkillTreeDlg::OnClickNormalSkillTab()
{  
	if( g_kGlobalValue.IsQuickSkillPlayer( m_iSelectedChar ) || 
        g_kGlobalValue.IsHybridPlayer( m_iSelectedChar ) )	
	{
        // �� ��ų �и��Ǳ� ������ ������忡���� �� �� ���� �ϵ��ڵ� �ؾߵȴ�.
        // �ƴ� ���� ���� ��...
        if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER || g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL || 
            ( SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL && g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) ) {
            if( m_iSelectedChar == GC_CHAR_RIN && m_iPromotion > 0 && m_iPromotion < NUM_JOB_LEVEL - 1  ) { // �� 3�� ������ ����
                if( m_bIsSelectedSkillB ) {
                    OnClickPassiveTab();
                    m_pkTabActiveSkill->ToggleRender( false );
                } else {
                    OnClickActiveTab();
                    m_pkTabPassiveSkill->ToggleRender( false );
                }
                return;
            }
        }    

        // ��� 3�� �̸鼭 �̺� �� ������ ��ų�� �����ٸ� �̺���(�нú�)�� Ȱ��ȭ ���� ����.
        if( ( m_iSelectedChar == GC_CHAR_DIO && m_iPromotion == 3 && SiGCSKT()->IsLearnedSkill( SID_DIO3_STICK_LANCE_SPECIAL_LV3 ) ) ||
            ( m_iSelectedChar == GC_CHAR_RIN && m_iPromotion > 0 && m_iPromotion < NUM_JOB_LEVEL - 1 && SiGCSKT()->IsLearnedSkill( SID_RIN2_SELECT_EVIL ) ) ) {
            OnClickPassiveTab();
		}
		else if ( m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE )
		{
			OnClickCommonSkillTab();
        } else {
		    OnClickActiveTab();
        }
		return;
	}
	

	OnNormalTabUI();

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
	m_pkbetweenField->ToggleRender(false);

    CheckDioTab();
	SetPassiveTabClick( false );
    ReloadSkillTree( "NormalSkill" );
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();
}

void KGCSkillTreeDlg::OnClickSpeicalTab()
{
	OnSepcialTabUI();

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
	m_pkbetweenField->ToggleRender(false);
    if( m_iSelectedChar == GC_CHAR_AMY )
    {
        m_pkFlipButton[0]->SetToolTip( L"" );
        m_pkFlipButton[1]->SetToolTip( L"" );                
        m_pkTypeSwapBtn->ToggleRender( true );
    }
	else if( m_iSelectedChar == GC_CHAR_RONAN )
	{
		m_pkFlipButton[0]->SetToolTip(  g_pkStrLoader->GetString( STR_ID_SKILLTREE_RONAN_SWAPBTN1 ) );
		m_pkFlipButton[1]->SetToolTip(  g_pkStrLoader->GetString( STR_ID_SKILLTREE_SWAPBTN2 ) );
		m_pkTypeSwapBtn->ToggleRender( false );
	}
    else
    {
        m_pkFlipButton[0]->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SWAPBTN1 ) );        
        m_pkFlipButton[1]->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE_SWAPBTN2 ) );        
        m_pkTypeSwapBtn->ToggleRender( false );
    }
	SetPassiveTabClick( false );
    CheckDioTab();
	ReloadSkillTree( "Special" );

	
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();
}

void KGCSkillTreeDlg::OnClickCommonSkillTab()
{
	OnCommonTabUI();

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
	m_pkbetweenField->ToggleRender(false);
    
	SetPassiveTabClick( false );
    CheckDioTab();
	ReloadSkillTree( "CommonSkill" );
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();
}

void KGCSkillTreeDlg::SetSelectedChar( int iCharType, int iPromotion)
{
	m_iSelectedChar = iCharType;
	m_iPromotion = iPromotion;
	m_pkCharacterName->SetChar( m_iSelectedChar );
	SiGCSKT()->SetCurUICharType( (char)m_iSelectedChar );	
	SiGCSKT()->SetCurUIPromotion( (char)m_iPromotion );	

	m_pkPreviewSkill = NULL;

	SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.SetCurrentChar( iCharType );
	//SiGCSKT()->GetOwnerPlayer()->SetMyPlayer();
	if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
	{
		KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo, KChangeRoomUserInfo::RUI_CHAR, g_MyD3D->Get_MyPlayer() );
	}
	//���̾��϶� ���� ���� ����Ѵ�.
	if (m_iSelectedChar==GC_CHAR_RYAN)
	{
		m_pkTabTransform->ToggleRender(true);
		m_pkTabTransformAct->ToggleRender(false);

		//���̾��̸� ũ�� �������� ���Դϴ�-_-;
		//m_pkTabTansform->SetWidth(37); m_pkTabTansform->SetHeight(90);
		m_pkTabSpecial->SetWidth(31); m_pkTabSpecial->SetHeight(90);
		m_pkTabNormalSkill->SetWidth(31); m_pkTabNormalSkill->SetHeight(90);
		m_pkTabCommonSkill->SetWidth(31); m_pkTabCommonSkill->SetHeight(90);

		m_pkTabSpecialAct->SetWidth(40); m_pkTabSpecialAct->SetHeight(100);
		m_pkTabNormalSkillAct->SetWidth(40); m_pkTabNormalSkillAct->SetHeight(100);
		m_pkTabCommonSkillAct->SetWidth(40); m_pkTabCommonSkillAct->SetHeight(100);

		m_pkTabNormalSkillAct->SetWindowPos(D3DXVECTOR2(36,149));
		m_pkTabNormalSkill->SetWindowPos(D3DXVECTOR2(45,154));
		m_pkTabSpecialAct->SetWindowPos(D3DXVECTOR2(36,251));
		m_pkTabSpecial->SetWindowPos(D3DXVECTOR2(45,256));
		m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(36,356));
		m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(45,361));

	}
	else if ( m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE )
	{
        m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
        m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,158));
        m_pkTabActiveSkillAct->SetWindowPos(D3DXVECTOR2(30,283));
        m_pkTabActiveSkill->SetWindowPos(D3DXVECTOR2(35,289));
        m_pkTabPassiveSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
        m_pkTabPassiveSkill->SetWindowPos(D3DXVECTOR2(34,420));

        m_pkTabSpecial->SetWidth(41); m_pkTabSpecial->SetHeight(119);
        m_pkTabNormalSkill->SetWidth(41); m_pkTabNormalSkill->SetHeight(119);
        m_pkTabCommonSkill->SetWidth(41); m_pkTabCommonSkill->SetHeight(119);

        m_pkTabSpecialAct->SetWidth(46); m_pkTabSpecialAct->SetHeight(130);
        m_pkTabNormalSkillAct->SetWidth(46); m_pkTabNormalSkillAct->SetHeight(130);
        m_pkTabCommonSkillAct->SetWidth(46); m_pkTabCommonSkillAct->SetHeight(130);
    }
	else
	{
		m_pkTabTransform->ToggleRender(false);
		m_pkTabTransformAct->ToggleRender(false);

		//�ƴϸ� ���� �������
		m_pkTabSpecial->SetWidth(41); m_pkTabSpecial->SetHeight(119);
		m_pkTabNormalSkill->SetWidth(41); m_pkTabNormalSkill->SetHeight(119);
		m_pkTabCommonSkill->SetWidth(41); m_pkTabCommonSkill->SetHeight(119);

		m_pkTabSpecialAct->SetWidth(46); m_pkTabSpecialAct->SetHeight(130);
		m_pkTabNormalSkillAct->SetWidth(46); m_pkTabNormalSkillAct->SetHeight(130);
		m_pkTabCommonSkillAct->SetWidth(46); m_pkTabCommonSkillAct->SetHeight(130);

		m_pkTabNormalSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
		m_pkTabNormalSkill->SetWindowPos(D3DXVECTOR2(35,158));
		m_pkTabSpecialAct->SetWindowPos(D3DXVECTOR2(30,283));
		m_pkTabSpecial->SetWindowPos(D3DXVECTOR2(35,289));
		m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
		m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,420));
		m_pkTransFormMenu->ToggleRender(false);
        m_pkTabActiveSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
        m_pkTabActiveSkill->SetWindowPos(D3DXVECTOR2(35,158));
        m_pkTabPassiveSkillAct->SetWindowPos(D3DXVECTOR2(30,283));
        m_pkTabPassiveSkill->SetWindowPos(D3DXVECTOR2(34,289));
	}

    UpdateSkillTab();    
	m_pkCommingSoon->ToggleRender( false );
}

void KGCSkillTreeDlg::SetSelectedChar( int iCharType, int iPromotion, bool bChangeRight )
{
    // check character index range
	if( iCharType < 0 )
		iCharType = GC_CHAR_NUM - 1;
	else if( iCharType >= GC_CHAR_NUM )
		iCharType = 0;

	if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.vecCharInfo.empty() || m_mapOpenCharIndex.empty())
        return;

	if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.IsCharExist( iCharType ) == false )
	{
		return SetSelectedChar( iCharType + (bChangeRight?1:-1), iPromotion, bChangeRight );        
	}
	else if( m_mapOpenCharIndex.find( iCharType ) == m_mapOpenCharIndex.end() )
	{
//#if defined( NATION_KOREA )
//        return SetSelectedChar( iCharType );
//#else
		return SetSelectedChar( iCharType + (bChangeRight?1:-1), iPromotion, bChangeRight );        
	}
	else
	{
		SetSelectedChar(iCharType, iPromotion);
	}
}

void KGCSkillTreeDlg::SetSelectedChar( int iCharType_ )
{
    m_iSelectedChar = iCharType_;
    m_iPromotion = 0;
    m_pkCharacterName->SetChar( m_iSelectedChar );
    SiGCSKT()->SetCurUICharType( (char)m_iSelectedChar );	
    SiGCSKT()->SetCurUIPromotion( (char)m_iPromotion );	

    m_pkPreviewSkill = NULL;

    SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.SetCurrentChar( m_iSelectedChar );

    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
    {
        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo, KChangeRoomUserInfo::RUI_CHAR, g_MyD3D->Get_MyPlayer() );
    }

    m_pkTabTransform->ToggleRender(false);
    m_pkTabTransformAct->ToggleRender(false);

    //�ƴϸ� ���� �������
    m_pkTabSpecial->SetWidth(41); m_pkTabSpecial->SetHeight(119);
    m_pkTabNormalSkill->SetWidth(41); m_pkTabNormalSkill->SetHeight(119);
    m_pkTabCommonSkill->SetWidth(41); m_pkTabCommonSkill->SetHeight(119);

    m_pkTabSpecialAct->SetWidth(46); m_pkTabSpecialAct->SetHeight(130);
    m_pkTabNormalSkillAct->SetWidth(46); m_pkTabNormalSkillAct->SetHeight(130);
    m_pkTabCommonSkillAct->SetWidth(46); m_pkTabCommonSkillAct->SetHeight(130);

    m_pkTabNormalSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
    m_pkTabNormalSkill->SetWindowPos(D3DXVECTOR2(35,158));
    m_pkTabSpecialAct->SetWindowPos(D3DXVECTOR2(30,283));
    m_pkTabSpecial->SetWindowPos(D3DXVECTOR2(35,289));
    m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
    m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,420));
    m_pkTransFormMenu->ToggleRender(false);

    GetChildByName( "tab_passive_skill" )->ToggleRender( false );
    GetChildByName( "tab_passive_skill_act" )->ToggleRender( false );
    m_pkTabPassiveSkill = GetChildByName( "tab_passive_skill" );
    m_pkTabPassiveSkillAct = GetChildByName( "tab_passive_skill_act" );        
    GetChildByName( "tab_ultimate_skill" )->ToggleRender( false );
    GetChildByName( "tab_ultimate_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_fightstance_skill" )->ToggleRender( false );
    GetChildByName( "tab_fightstance_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_selfaura_skill" )->ToggleRender( false );
    GetChildByName( "tab_selfaura_skill_act" )->ToggleRender( false );

    m_pkCommingSoon->ToggleRender( false );
}

void KGCSkillTreeDlg::PreDraw()
{
	//���̾��϶� ����ϹǷ� ���� ó�� ��
	if (m_iSelectedChar == GC_CHAR_RYAN)
	{
		g_pGCDeviceManager->PushState();
		g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	}
	
}
void KGCSkillTreeDlg::OnChangeSkillSet()
{
	m_iSetID = m_pkComboSkillSet->GetCurSel();
	
	if(m_iSetID == -1)
		m_iSetID = 0;

	SiGCSKT()->ChangeSetting( m_iSelectedChar, m_iPromotion, m_iSetID );

	Update();
}

bool KGCSkillTreeDlg::ReloadSkillTree( std::string strTabname )
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return false;

	std::string strSkillTreeUI = "SkillTreeUI_kr.lua";
	if( GCFUNC::LoadLuaScript( luaMgr, strSkillTreeUI ) == false )
		return false ;


	m_strCurrentTab = strTabname;
	LUA_BEGIN_TABLE( "SkillTreeUI", return false )
	{
		m_pkSkilltreeField[0]->ClearAllChild();
		m_pkSkilltreeField[1]->ClearAllChild();
		LUA_BEGIN_TABLE( m_iSelectedChar + m_iPromotion * GC_CHAR_NUM, return false)
		{
			if( LoadSkillTreeTab( luaMgr, strTabname ) == false )
				return false;
		}
		LUA_END_TABLE( return false )
	}
	LUA_END_TABLE( return false )

	Update();
	return true;
}

bool KGCSkillTreeDlg::LoadSkillLocker( KLuaManager& luaMgr )
{
	m_pkLockerField->RemoveAllChild();
	LUA_BEGIN_TABLE( "Lock", return false )
	{
        for( int i = 1 ; ; ++i )
		{
			LUA_BEGIN_TABLE( i, break )
			{
				int LockID = 0, Height = 0, Width = 0, UnlockPos = 0;
				float x = 0.0f, y = 0.0f;
				LUA_GET_VALUE( "LockerID",	LockID,		return false );
				LUA_GET_VALUE( "x",			x,			return false );
				LUA_GET_VALUE( "y",			y,			return false );
				LUA_GET_VALUE( "Height",	Height,		return false );
				LUA_GET_VALUE( "Width",		Width,		return false );
				LUA_GET_VALUE_DEF( "UnlockPos", UnlockPos, 0 );

				AddLocker( LockID, D3DXVECTOR2( x, y ), Width, Height, UnlockPos );
			}
			LUA_END_TABLE( break )
		}
	}
	LUA_END_TABLE( return false )

	return true;
}

bool KGCSkillTreeDlg::LoadSkillTreeTab( KLuaManager& luaMgr, std::string strTable )
{
	// ������ ���� : 337
	// �̰Ŵ� UI�� ��! ���ϱ� ������ �ڵ忡 ����Ѽ� �ۿ���.... ��,.��
	if( SUCCEEDED( luaMgr.BeginTable( strTable.c_str() ) ) )
	{
		bool bFlip = false;
		LUA_GET_VALUE_DEF( "flip", bFlip, false );

		bool bSlot = false;
		LUA_GET_VALUE_DEF( "HotKeySlot", bSlot, false );

		m_pkSkillSlotBox->ToggleRender( bSlot );

		m_bFieldFlip = bFlip;
		if( bFlip == false )
		{
			m_pkFlipButton[0]->ToggleRender( false );
			m_pkFlipButton[1]->ToggleRender( false );
            m_pkTypeSwapBtn->ToggleRender( false );
		}

		if( SUCCEEDED( luaMgr.BeginTable( "Left") ) )
		{
			if( LoadSkilltreeField( luaMgr, 0 ) == false )
			{
				ASSERT( !"SkillTreeUI.stg ���Ͽ� Left ���̺��� ���� ����." );
				return false;
			}

			if( !m_bFieldFlip || m_pkFlipButton[1]->IsRenderOn() )
				LoadSkillLocker( luaMgr );

			LUA_END_TABLE( return false );

			if( SUCCEEDED( luaMgr.BeginTable( "Right" ) ) )
			{
				if( LoadSkilltreeField( luaMgr, 1 ) == false )
				{
					ASSERT( !"SkillTreeUI.stg ���Ͽ� Right ���̺��� ���� ����." );
					return false;
				}
				else
					m_pkRightField->ToggleRender( true );

				if( !m_bFieldFlip || m_pkFlipButton[0]->IsRenderOn() )
					LoadSkillLocker( luaMgr );
				LUA_END_TABLE( return false );
			}
			luaMgr.EndTable();
		}
		else if( SUCCEEDED( luaMgr.BeginTable( "LeftGroup" ) ) )
		{
			if( LoadSkillGroup( luaMgr, 0 ) == false )
			{
				ASSERT( !"SkillTreeUI.stg ���Ͽ� LeftGroup ���̺��� ���� ����." );
				return false;
			}
			if( !m_bFieldFlip || m_pkFlipButton[1]->IsRenderOn() )
				LoadSkillLocker( luaMgr );

			m_pkLeftField->SetWidth( 337 );
			luaMgr.EndTable();

			if( SUCCEEDED( luaMgr.BeginTable( "RightGroup" ) ) )
			{
				if( LoadSkillGroup( luaMgr, 1 ) == false )
				{
					ASSERT( !"SkillTreeUI.stg ���Ͽ� RightGroup ���̺��� ���� ����." );
					return false;
				}
				else
					m_pkRightField->ToggleRender( true );
				if( !m_bFieldFlip || m_pkFlipButton[0]->IsRenderOn() )
					LoadSkillLocker( luaMgr );
				luaMgr.EndTable();
			}
			else
			{
				m_pkLeftField->SetWidth( 337 * 2 + 4 );
				m_pkRightField->ToggleRender( false );
			}

			//��ҹ��� �����ϼ���-0-
			if( SUCCEEDED( luaMgr.BeginTable( "BetWeenGroup" ) ) )
			{
				if( LoadSkillGroup( luaMgr, 1 ) == false )
				{
					ASSERT( !"SkillTreeUI.stg ���Ͽ� BetWeenGroup ���̺��� ���� ����." );
					return false;
				}
				else
				{
					int width = 223, height = 427;
					int x = 80, y = 148;
					m_pkLeftField->SetWidth( width );
					m_pkLeftField->SetHeight( height );
					m_pkRightField->SetWidth( width );
					m_pkRightField->SetHeight( height );
					m_pkbetweenField->SetWidth( width + 1 );
					m_pkbetweenField->SetHeight( height );

					m_pkLeftField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>(x) , static_cast<float>(y)) );
					m_pkRightField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>(x + width + 4), static_cast<float>(y)) );
					m_pkbetweenField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>( x + width*2 + 8 ), static_cast<float>(y) ) );
					m_pkbetweenField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, 11, 53, 69 ) );
					m_pkbetweenField->ToggleRender( true );	
				}
				//if( !m_bFieldFlip || m_pkFlipButton[0]->IsRenderOn() )
				//	LoadSkillLocker( luaMgr );
				luaMgr.EndTable();
			}
			else
			{
				m_pkbetweenField->ToggleRender(false);
			}

            if( SUCCEEDED( luaMgr.BeginTable( "Between" ) ) )
            {
                if( LoadSkilltreeField( luaMgr, 1 ) == false )
                {
                    ASSERT( !"SkillTreeUI.stg ���Ͽ� Right ���̺��� ���� ����." );
                    return false;
                }

                if( !m_bFieldFlip || m_pkFlipButton[0]->IsRenderOn() )
                    LoadSkillLocker( luaMgr );
                LUA_END_TABLE( return false );
            }
            else
            {
                m_pkbetweenField->ToggleRender(false);
            }
            luaMgr.EndTable();
		}
	}
	UpdateFieldSize( strTable );

	return true;
}

bool KGCSkillTreeDlg::LoadSkillGroup( KLuaManager& luaMgr, int iFieldIndex )
{
	for( int i = 1 ; ; ++i )
	{
		LUA_BEGIN_TABLE( i , break )
		{
			int iGroup = 0, iTitleID = 0, width = 0, height = 0;
			float x = 0.0f, y = 0.0f;
			LUA_GET_VALUE( "SkillGroup", iGroup, return false );
			LUA_GET_VALUE( "x", x, return false );
			LUA_GET_VALUE( "y", y, return false );
			LUA_GET_VALUE_DEF( "width", width , 0 );
			LUA_GET_VALUE_DEF( "height", height, 0 );
			LUA_GET_VALUE( "Title", iTitleID, return false );
            
            if( width == -1 && height == -1 ) {
                m_pkSkilltreeField[iFieldIndex]->CreateMainSkillBox( (EGCSkillGroup)iGroup, x, y, iTitleID, !m_bFieldFlip||!m_pkFlipButton[iFieldIndex]->IsRenderOn(), m_iSelectedChar, m_iPromotion );
            } else {
                m_pkSkilltreeField[iFieldIndex]->CreateSkillBoxGroup( (EGCSkillGroup)iGroup, x, y, (DWORD)width, (DWORD)height, iTitleID );
            }			
		}
		LUA_END_TABLE( break )
	}

	return true;
}

bool KGCSkillTreeDlg::LoadSkilltreeField( KLuaManager& luaMgr, int iFieldIndex )
{
	for( int i = 1 ; ; ++i )
	{
		LUA_BEGIN_TABLE( i , break )
		{
			int iGroup = 0, iTitleID = 0;
			float x = 0.0f, y = 0.0f;
			LUA_GET_VALUE( "SkillGroup", iGroup, return false );
			if( m_bFieldFlip && m_pkFlipButton[iFieldIndex]->IsRenderOn() )
				LUA_BEGIN_TABLE( "FlipPos", ASSERT(!"���̺��� ����!!") );
			LUA_GET_VALUE( "x", x, return false );
			LUA_GET_VALUE( "y", y, return false );
			if( m_bFieldFlip && m_pkFlipButton[iFieldIndex]->IsRenderOn() )
				LUA_END_TABLE( ASSERT( !"���� �̻���" ));
			LUA_GET_VALUE( "Title", iTitleID, return false );

			m_pkSkilltreeField[iFieldIndex]->CreateMainSkillBox( (EGCSkillGroup)iGroup, x, y, iTitleID, !m_bFieldFlip||!m_pkFlipButton[iFieldIndex]->IsRenderOn(), m_iSelectedChar, m_iPromotion );
		}
		LUA_END_TABLE( break )
	}

	return true;
}

void KGCSkillTreeDlg::OnClickClose()
{

	if( !SiGCSKT()->IsUnlockAll() )
	{
		SiGCSKT()->ApplySettingToServer();

		switch(g_MyD3D->m_pStateMachine->GetState())
		{
		case GS_MY_INFO:
		case GS_MY_INFO_FROM_ROOM:
		case GS_MATCH:
			g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
			break;
		case GS_SHOP_CASH:
			g_pkUIScene->m_pkCashShopScene->SetCharItem( (EGCCharType)((int)SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType) );
			break;
#ifndef DISABLE_MAP_SHOP // Kawan>
        case GS_SHOP_GP:
            g_pkUIScene->m_pkGPShopScene->SetCharItem( (EGCCharType)((int)SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType) );
            break;
#endif
#if defined USE_COORDI_SHOP
		case GS_COORDI_SHOP:
			g_pkUIScene->m_pkCoordiShopScene->SetChangeCharacter( static_cast<int>(SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType) );
			break;
#endif
		}
// 		g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
// 		if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH || g_MyD3D->m_pStateMachine->GetState() == GS_COORDI_SHOP)
// 		{
// 			g_pkUIScene->m_pkCashShopScene->SetCharItem( (EGCCharType)((int)SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType) );
// 		}
	}

	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();

	SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ));

#if !defined(__PATH__)
    // ���� ������ ġƮ�� ��ų ���� �����ϱ� ���� ��� - ������ ����� ���� �ʱ� ������ ���� ������ ���� ������ ���� ���������� ��ų ������ ���� ����.
    if( g_MyD3D->GetCheatOpenSkillUI() && GS_GAME == g_MyD3D->m_pStateMachine->GetState()) {
        //ó��
        if( g_kGlobalValue.IsTutorialMode() ) {
            g_MyD3D->MyPlayer[0]->m_kUserInfo.UpdateSkillSet( SiGCSKT()->GetSkillSettings(), SiGCSKT()->GetSkillSets() );
            g_MyD3D->MyPlayer[0]->LoadPlayerTemplate();
        } else {
            g_MyD3D->GetMyPlayer()->m_kUserInfo.UpdateSkillSet( SiGCSKT()->GetSkillSettings(), SiGCSKT()->GetSkillSets() );
            g_MyD3D->GetMyPlayer()->LoadPlayerTemplate();
        }                
        g_MyD3D->SetCheatOpenSkillUI( false );
    }
#endif
}

void KGCSkillTreeDlg::OnClickReset()
{
	//SiGCSKT()->UnequipAllSkill( m_iSelectedChar, m_iPromotion, m_pkComboSkillSet->GetCurSel() );
	m_pkSkilltreeField[0]->UnEquipMainSkills();
	m_pkSkilltreeField[1]->UnEquipMainSkills();
	g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
	Update();
}

void KGCSkillTreeDlg::Update()
{
	if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.vecCharInfo.empty() )
		return;

	SiGCSKT()->SetCurUICharType( (char)m_iSelectedChar );
	SiGCSKT()->SetCurUIPromotion( (char)m_iPromotion );

	const KSkillInfo& SkillInfo = SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).kSkillInfo;
	int iLv = SkillInfo.m_nLvSPPoint;


	m_pkCurrentSkillLevel->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE_CURRENT_SKILL_LEVEL, "i", iLv ) );
	
	float fGuagePercent = GetCurrentExpPercent( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).biExp ); 
	if ( fGuagePercent > 1.0f )
	{
		fGuagePercent = 1.0f;
	}
	m_pkSkillLevelGuage->SetWidth( (DWORD)((float)m_pkSkillLevelGuageBack->GetWidth() * fGuagePercent) );

    if( g_kGlobalValue.IsTutorialMode() ) {
	    m_pkCurrentSp->SetText( L"��" );
	    m_pkMaxSp->SetText( L"��" );
        m_pkUsingSp->SetText( L"��" );
	    m_pkCharMaxSp->SetText( L"��" );
    } else {
        m_pkCurrentSp->SetText( SiGCSKT()->GetLeftSPExcludingCommonSkill( m_iSelectedChar ) );
        m_pkMaxSp->SetText( SiGCSKT()->GetMaxSP( m_iSelectedChar,m_iPromotion ) );

        int iUsedSP = SiGCSKT()->GetUsedSPinPromotion( m_iSelectedChar, m_iPromotion );

        m_pkUsingSp->SetText( iUsedSP );

        m_pkCharMaxSp->SetText( SiGCSKT()->GetMaxSPinPromotion( m_iSelectedChar, m_iPromotion ) );
    }

	m_pkSkillRemovePoint->SetText( GetRemovePoint() );

	m_pkSkilltreeField[0]->Update( m_iSetID );
	m_pkSkilltreeField[1]->Update( m_iSetID );

	UpdateLocker();    
#if defined ( NATION_LATINAMERICA )
    if( g_kGlobalValue.IsTempUseSpecial4Char( m_iSelectedChar ) || g_kGlobalValue.IsHybridPlayer( m_iSelectedChar ) ) {
        m_pkCommingSoon->ToggleRender( false );
    } else {
        m_pkCommingSoon->ToggleRender( true );
    }
#else
	m_pkCommingSoon->ToggleRender( false );
#endif
	UpdatePromotionBtn();

    m_pkSkillSlotBox->Update();    
}

void KGCSkillTreeDlg::OnCreateComplete()
{

    if( KP2P::GetInstance()->IsConnectedToGameServer() == true && m_bOpenNewChar == false )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
            KP2P::GetInstance()->Send_Into_Myinfo( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.dwUID, SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.strLogin, GC_RUS_SKILL );
        }

        std::map<int, std::set<int> >::iterator mit;
        for( mit = m_mapOpenCharIndex.begin() ; mit != m_mapOpenCharIndex.end() ; ++mit )
        {
            int iCharType = mit->first;
            int iPromotion = mit->second.size() - 1;
            if( iCharType == SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar().iCharType )
            {
				if ( m_iPromotion > iPromotion )
				{
					m_iPromotion = iPromotion;
				}
                SetSelectedChar( iCharType ,m_iPromotion ,true);                
                break;
            }
        }
    }
    else
        SetSelectedChar( m_iSelectedChar, m_iPromotion ,true );

	int iSetID = m_iSetID;

	iSetID = SiGCSKT()->GetSetting(m_iSelectedChar, SiGCSKT()->GetOwnerPlayer()->GetPromotion(m_iSelectedChar) ) ;
	
	m_pkComboSkillSet->SetCurSel( iSetID );
	if (m_iSelectedChar==GC_CHAR_RYAN)
	{
		m_pkTabTransform->ToggleRender(true);
		m_pkTabTransformAct->ToggleRender(false);
		m_pkTransFormMenu->ToggleRender(false);
	}
	m_pkComboSkillSet->LockAll( SiGCSKT()->IsUnlockAll() );


    if( TABOPEN_NORMALSKILL == ( TABOPEN_NORMALSKILL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ||
        TABOPEN_ACTIVE == ( TABOPEN_ACTIVE & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ) {
            OnClickNormalSkillTab();
            if( m_iSelectedChar == GC_CHAR_RYAN ) {
                m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(36,356));
                m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(45,361));
			}
			else if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE)
			{
				m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
                m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,158));
            } else {
                m_pkTabCommonSkill->SetWindowPos( D3DXVECTOR2( 35, 420 ) );
                m_pkTabCommonSkillAct->SetWindowPos( D3DXVECTOR2( 30, 414 ) );
            }            
    } else {
        OnClickCommonSkillTab();
        if( m_iSelectedChar == GC_CHAR_RYAN ) {
            m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(36,158));
            m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(45,152));

            m_pkTabTransform->ToggleRender(false);
            m_pkTabTransformAct->ToggleRender(false);
		}
		else if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) 
		{
			m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
            m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,158));
        } else {
            m_pkTabCommonSkill->SetWindowPos( D3DXVECTOR2( 35, 158 ) );
            m_pkTabCommonSkillAct->SetWindowPos( D3DXVECTOR2( 30, 152 ) );
        }        
    }
	Update();

    m_bEnable = true;
}

void KGCSkillTreeDlg::OnDestroyComplete()
{
    if( KP2P::GetInstance()->IsConnectedToGameServer() == true )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
            KP2P::GetInstance()->Send_Into_Myinfo( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.dwUID, SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.strLogin, GC_RUS_WAIT );
        }
    }

	// ���� �� TabUI ���¸� ���� SkillTree�� �ε��մϴ�.
	// ������ �⺻ ������ �����ϹǷ� ���� �� �⺻ ������ �����մϴ�.
	OnNormalTabUI();

    m_bEnable = false;
    m_bOpenNewChar = false;
}

std::wstring KGCSkillTreeDlg::GetMiddleDesc( const GCSkill* pSkill )
{
	std::wstringstream str;

	int iCharType = pSkill->GetCharType();
	int iPromotion = pSkill->GetPromotion();

	// �ʿ� ����
#ifdef PERYTON // Kawan>
	if (!g_kGlobalValue.IsQuickSkillPlayer(pSkill->GetCharType()) || pSkill->GetCharType() == GC_CHAR_DIO || pSkill->GetCharType() == GC_CHAR_BEIGAS || pSkill->GetCharType() == GC_CHAR_PERYTON)
#else
	if( !g_kGlobalValue.IsQuickSkillPlayer( pSkill->GetCharType() ) || pSkill->GetCharType() == GC_CHAR_DIO || pSkill->GetCharType() == GC_CHAR_BEIGAS )
#endif
	{
		str<<( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).iLevel >= pSkill->GetLv() ? L"#c00FF00" : L"#cFF0000" );
		str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE6, "i", pSkill->GetLv() )<<L"#CX\n";
	}	

	// �ʿ� SP
    bool bNeedSP = SiGCSKT()->GetExtraSP(iCharType) >= pSkill->GetLearnSP() ||SiGCSKT()->IsLearnedSkill( pSkill->GetID() ) == true ;
    str<<( bNeedSP ? L"#c00FF00" : L"#cFF0000" );
	str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE12, "i", pSkill->GetLearnSP() )<<L"#CX\n";

#if !defined(__PATH__) && defined(NATION_KOREA)
    str << L"��ųID : " << pSkill->GetID() <<L"#CX\n";

    GCSkillResource* pSkillResource = SiGCSKT()->GetSkillResource( pSkill->GetID() );    
    std::set< MOTIONID > setMotionID;
    if( pSkillResource != NULL ) {        
        pSkillResource->GetMotionID( setMotionID );

        int iLoopCnt = 0;
        for( std::set< MOTIONID >::iterator sit = setMotionID.begin(); sit != setMotionID.end(); ++sit ) {
            if( iLoopCnt == 0 ) {
                str << L"���ID : " << *sit;
            } else {
                str << L" " << *sit;
            }

            iLoopCnt++;

            if( iLoopCnt > 3 ) {
                str << L"\n";
                iLoopCnt = 0;
            }
        }
        str <<L"#CX\n";
    }   
#endif

	// ���� ��ų
	const SkillSet prevSkill = pSkill->GetPrevSet();
	SkillSet::const_iterator csit;
	std::wstringstream prevList;
	bool bPrevSkillOk = true;
	for( csit = prevSkill.begin() ; csit != prevSkill.end() ; ++csit )
	{
		const GCSkill* pPrev = SiGCSKT()->GetSkill( *csit );
		if( csit != prevSkill.begin() )
			prevList<<L", ";
		prevList<<SiGCSKT()->GetString( pPrev->GetTitleID() );

		if( SiGCSKT()->IsLearnedSkill( pPrev->GetID() ) == false )
			bPrevSkillOk = false;
	}

	if( prevSkill.empty() == true )
	{
		prevList<<g_pkStrLoader->GetString(STR_ID_SKILLTREE14);
	}

	str<<( bPrevSkillOk ? L"#c00FF00" : L"#cFF0000" );
	str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE13, "l", prevList.str() )<<L"#CX\n";

	if ( pSkill->m_pHotKeySkill )
	{
        float fMana = pSkill->m_pHotKeySkill->m_fMana;
        float fCoolTime = pSkill->m_pHotKeySkill->m_fCoolTime;
#if defined( MATCH_BALANCE_2011_8 )
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            if( 0.0f < pSkill->m_pHotKeySkill->m_fMonsterModeMana )
                fMana = pSkill->m_pHotKeySkill->m_fMonsterModeMana;
            if( 0.0f < pSkill->m_pHotKeySkill->m_fMonsterModeCoolTime )
                fCoolTime = pSkill->m_pHotKeySkill->m_fMonsterModeCoolTime;
	    }
#endif
		str<<g_pkStrLoader->GetReplacedString( STR_ID_SKILL_INFOBOX_DESC_COOLTIME, "ii", 
            static_cast< int >( pSkill->m_pHotKeySkill->m_fMonsterModeCoolTime ), static_cast< int >( pSkill->m_pHotKeySkill->m_fCoolTime ) ) << L"\n";
        if( g_kGlobalValue.IsQuickSkillPlayer( m_iSelectedChar ) ) {
		    str<<g_pkStrLoader->GetReplacedString( STR_ID_SKILL_INFOBOX_DESC_AP, "i", static_cast< int >(( fMana / 3.0f ) * 100.0f));		
        } else {
            str<<g_pkStrLoader->GetReplacedString( STR_ID_SKILL_INFOBOX_DESC_MP, "i", static_cast< int >(( fMana / 3.0f ) * 100.0f));		
        }
	}
	else if( !pSkill->m_bPassive )
	{
		str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE8, "l", SiGCSKT()->GetString( pSkill->GetControlDescID() ) );
	}
	

	return str.str();
}

std::wstring KGCSkillTreeDlg::GetSkillTitle( const GCSkill* pSkill )
{
	std::wstringstream title;

    int iCharType = pSkill->GetCharType();
    bool bNeedSP = SiGCSKT()->GetExtraSP(iCharType) >= pSkill->GetLearnSP() ||SiGCSKT()->IsLearnedSkill( pSkill->GetID() ) == true ;

    title<<(bNeedSP? L"#c00FF00" : L"#cFF0000" )<<SiGCSKT()->GetString( pSkill->GetTitleID() )<<L"#CX";
	return title.str();
}

bool KGCSkillTreeDlg::AddLocker( int iLockID, D3DXVECTOR2 vPos, DWORD dwWidth, DWORD dwHeight, int iUnlockPos )
{
    if( SiGCSKT()->IsBlockedSkill( iLockID ) == false )
        return false;

	KGCSkillBoxLock* pWnd = (KGCSkillBoxLock*)g_pkUIMgr->CreateTemplateUI( m_pkLockerField, "gc_skillbox_lock", "ui_skillbox_lock.stg", "skillbox_lock", true, true );

	if( pWnd == NULL )
	{
		ASSERT( !"���� �̻��ϰԵ� Wnd ������ ���� ����" );
		return false;
	}
	pWnd->SetLockID( iLockID );
	pWnd->SetWindowPosDirect( vPos );
	pWnd->SetSize( dwWidth, dwHeight );
	pWnd->SetUnlockButtonPosition( iUnlockPos );

	if ( SiGCSKT()->IsUnlockAll() )
	{
		pWnd->ToggleRender( false );
	}

	return true;
}

void KGCSkillTreeDlg::UpdateLocker()
{
	g_pkUIScene->m_pkToolTip->DeleteParent();

	for( int i = 0 ; i < (int)m_pkLockerField->GetNumChild() ; ++i )
	{
		KGCSkillBoxLock* pWnd = (KGCSkillBoxLock*)m_pkLockerField->GetChild( i );
		if( SiGCSKT()->IsBlockedSkill( (EGCCharType)m_iSelectedChar, pWnd->GetLockID() ) == false )
		{
			m_pkLockerField->DeleteChild( i );
			--i;
		}
	}
}

void KGCSkillTreeDlg::PostChildDraw()
{
	D3DXVECTOR2 vPos(GetRelocatedWindowPos());
//	vPos += m_pkLevelDrawPos->GetFixedWindowLocalPos();
	vPos.x += m_pkLevelDrawPos->GetFixedWindowLocalPos().x * m_fWindowScaleX;
	vPos.y += m_pkLevelDrawPos->GetFixedWindowLocalPos().y * m_fWindowScaleY;

	g_pkUIMgr->RenderNumber("very_small_num", vPos , 
		(DWORD)SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(m_iSelectedChar).iLevel, 0xffffffff, true, false, -1.0f,true);
	//���̾��϶� ����ϹǷ� ���� ó�� ��
	if (m_iSelectedChar == GC_CHAR_RYAN)
	{
		g_pGCDeviceManager->PopState();
	}
}

void KGCSkillTreeDlg::FrameMove_BlinkAlert()
{
	static BYTE alpha = 0xFF;
	static bool bIncrease = false;

	if( alpha>=0xFF - 6 )
		bIncrease = false;

	if( alpha<=0x00 + 6 )
		bIncrease = true;

	bIncrease?alpha+=5:alpha-=5;

	if( m_pkAlertNormal->IsRenderOn() )
	{
		m_pkAlertNormal->ForceSetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
	}
	if( m_pkAlertSpecial->IsRenderOn() )
	{
		m_pkAlertSpecial->ForceSetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
	}
	if( m_pkAlertCommon->IsRenderOn() )
	{
		m_pkAlertSpecial->ForceSetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
	}
}

void KGCSkillTreeDlg::OnClickSkillLeft()
{
	const GCSkill* pTemp = SiGCSKT()->GetPrevLevelSkill( m_pkPreviewSkill );

	if (pTemp == NULL || SiGCSKT()->CheckPosibleSkill(pTemp) == false)
		return;

	m_pkPreviewSkill = pTemp;
	m_pkCommingSoon->ToggleRender( false );
}

void KGCSkillTreeDlg::OnClickSkillRight()
{
	const GCSkill* pTemp = SiGCSKT()->GetNextLevelSkill( m_pkPreviewSkill );

	if (pTemp == NULL || SiGCSKT()->CheckPosibleSkill(pTemp) == false)
		return;

#if defined( LOCK_ASIN_HIDDEN_SKILL )
    if( SiGCSKT()->IsSkillMissionSkillID( pTemp->GetID() ) ) {
        return;
    }
#endif

	m_pkPreviewSkill = pTemp;
	UpdatePreviewSkill();
}

void KGCSkillTreeDlg::UpdatePreviewSkill( bool bTutorial )
{
    if( g_kGlobalValue.IsTempUseSpecial4Char( m_iSelectedChar ) || g_kGlobalValue.IsHybridPlayer( m_iSelectedChar ) ) {
        m_pkCommingSoon->ToggleRender( false );
    } else {
        m_pkCommingSoon->ToggleRender( true );
    }

	if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.vecCharInfo.empty() )
		return;

    if( bTutorial )
    {
    	if( m_pkPreviewSkill == NULL )
	    	m_pkPreviewSkill = SiGCSKT()->GetSimmilarLevelSkill( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).iLevel );
        if( m_pkPreviewSkill == NULL )
		    return;

    }
    else
    {
        if( m_pkPreviewSkill == NULL )
            m_pkPreviewSkill = SiGCSKT()->GetTutorialSimmilarLevelSkill( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).iLevel );
        if( m_pkPreviewSkill == NULL )
            return;
    }

	if( SiGCSKT()->IsLearnedSkill( m_pkPreviewSkill->GetID() ) )
		OnClickSkillRight();
	m_pkPreviewSkillBox->SetSkill( m_pkPreviewSkill->GetID() );
	m_pkNextSkillDesc->SetTextAutoMultiline( GetSimpleDesc( m_pkPreviewSkill ) );
}

std::wstring KGCSkillTreeDlg::GetSimpleDesc( const GCSkill* pSkill )
{
	std::wstringstream str;

	int iCharType = pSkill->GetCharType();
	int iPromotion = pSkill->GetPromotion();

    // ��� SID_DIO1_EMPTY_SLOT�̸� ����ִ� ��ų��.
    // ���Ͼ����� Ȯ�ιް� �Ʒ� �ϵ��ڵ����� ��ư Ȱ��ȭ�� ���Ƶ�.
    //m_pkBtnSkillLeft->Lock( NULL == SiGCSKT()->GetPrevLevelSkill( m_pkPreviewSkill ) );
    //m_pkBtnSkillRight->Lock( NULL == SiGCSKT()->GetNextLevelSkill( m_pkPreviewSkill ) );

    bool bSkillLocker;
    const GCSkill *pSkillTemp = SiGCSKT()->GetPrevLevelSkill( m_pkPreviewSkill );
    if( pSkillTemp && SID_DIO1_EMPTY_SLOT == pSkillTemp->GetID() )  
	{   
		bSkillLocker = true;    
	}
    else if( NULL == pSkillTemp )                                   
	{   
		bSkillLocker = true;    
	}
	else if (SiGCSKT()->CheckPosibleSkill(pSkillTemp) == false)
	{
		bSkillLocker = true;
	}
    else                                                            
	{   
		bSkillLocker = false;   
	}
    m_pkBtnSkillLeft->Lock( bSkillLocker );

    pSkillTemp = SiGCSKT()->GetNextLevelSkill( m_pkPreviewSkill );
    if( pSkillTemp && SID_DIO1_EMPTY_SLOT == pSkillTemp->GetID() )  
	{   
		bSkillLocker = true;    
	}
    else if( NULL == pSkillTemp )                                   
	{   
		bSkillLocker = true;    
	}
	else if (SiGCSKT()->CheckPosibleSkill(pSkillTemp) == false)
	{
		bSkillLocker = true;
	}
    else                                                            
	{   
		bSkillLocker = false;   
	}
    m_pkBtnSkillRight->Lock( bSkillLocker );

	// �Ҽ� �׷�(���~ �׷��̸��� ����... )
	str<<L"\n";
	
	// ��ų �̸�
	//str<< GetSkillTitle(pSkill)<< L"\n";

	// �ʿ� ����
	str<<( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).iLevel >= pSkill->GetLv() ? L"#c00FF00" : L"#cFF0000" );
	str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE21, "i", pSkill->GetLv() )<<L"#CX\n";

	// �ʿ� SP
	str<<( SiGCSKT()->GetExtraSP(iCharType) >= pSkill->GetLearnSP() ? L"#c00FF00" : L"#cFF0000" );
	str<<g_pkStrLoader->GetReplacedString(STR_ID_SKILLTREE22, "i", pSkill->GetLearnSP() )<<L"#CX\n";

	return str.str();
}

void KGCSkillTreeDlg::OnTransformTabUI()
{
	m_pkTabSpecialAct->ToggleRender( false );
	m_pkTabSpecial->ToggleRender( true );
	m_pkTabNormalSkill->ToggleRender( true );
	m_pkTabNormalSkillAct->ToggleRender( false );
	m_pkTabCommonSkill->ToggleRender( true );
	m_pkTabCommonSkillAct->ToggleRender( false );
	m_pkTabTransform->ToggleRender(false);
	m_pkTabTransformAct->ToggleRender(true);
	m_pkTransFormMenu->ToggleRender(true);
}

void KGCSkillTreeDlg::OnSepcialTabUI()
{	
    if( TABOPEN_SPECIAL == ( TABOPEN_SPECIAL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ) {
        m_pkTabSpecialAct->ToggleRender( true );
        m_pkTabSpecial->ToggleRender( false );
        m_pkTabNormalSkill->ToggleRender( true );
        m_pkTabNormalSkillAct->ToggleRender( false );
        m_pkTabCommonSkill->ToggleRender( true );
        m_pkTabCommonSkillAct->ToggleRender( false );
    } else {
        m_pkTabSpecialAct->ToggleRender( false );
        m_pkTabSpecial->ToggleRender( false );
        m_pkTabNormalSkill->ToggleRender( false );
        m_pkTabNormalSkillAct->ToggleRender( false );
    }

	if (m_iSelectedChar==GC_CHAR_RYAN)
	{
		m_pkTabTransform->ToggleRender(true);
		m_pkTabTransformAct->ToggleRender(false);
		m_pkTransFormMenu->ToggleRender(false);
	}
}

void KGCSkillTreeDlg::OnNormalTabUI()
{
    if( TABOPEN_NORMALSKILL == ( TABOPEN_NORMALSKILL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ) {
	    m_pkTabSpecialAct->ToggleRender( false );
	    m_pkTabSpecial->ToggleRender( true );
	    m_pkTabNormalSkill->ToggleRender( false );
	    m_pkTabNormalSkillAct->ToggleRender( true );
	    m_pkTabCommonSkill->ToggleRender( true );
	    m_pkTabCommonSkillAct->ToggleRender( false );
    } else {
        m_pkTabSpecialAct->ToggleRender( false );
        m_pkTabSpecial->ToggleRender( false );
        m_pkTabNormalSkill->ToggleRender( false );
        m_pkTabNormalSkillAct->ToggleRender( false );
    }

	m_pkTabPassiveSkillAct->ToggleRender( false );
	m_pkTabPassiveSkill->ToggleRender( false );
	m_pkTabActiveSkill->ToggleRender( false );
	m_pkTabActiveSkillAct->ToggleRender( false );
	if (m_iSelectedChar==GC_CHAR_RYAN)
	{
		m_pkTabTransform->ToggleRender(true);
		m_pkTabTransformAct->ToggleRender(false);
		m_pkTransFormMenu->ToggleRender(false);
	}
}

void KGCSkillTreeDlg::OnCommonTabUI()
{
	m_pkTabCommonSkill->ToggleRender( false );	
	m_pkTabCommonSkillAct->ToggleRender( true );

	m_pkTabSpecial->ToggleRender( false );
	m_pkTabSpecialAct->ToggleRender( false );
	m_pkTabNormalSkill->ToggleRender( false );
	m_pkTabNormalSkillAct->ToggleRender( false );

	m_pkTabPassiveSkillAct->ToggleRender( false );
	m_pkTabPassiveSkill->ToggleRender( false );
	m_pkTabActiveSkill->ToggleRender( false );
	m_pkTabActiveSkillAct->ToggleRender( false );

	m_pkTabTransform->ToggleRender(false);
	m_pkTabTransformAct->ToggleRender(false);
	m_pkTransFormMenu->ToggleRender(false);

    m_pkBackAsin->ToggleRender( false );
    m_pkStaticAsin->ToggleRender( false );

    if( m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar==GC_CHAR_LIME || m_iSelectedChar==GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE)
	{
		m_pkTabPassiveSkill->ToggleRender( true );
        m_pkTabActiveSkill->ToggleRender( true );
    }

    if( m_iSelectedChar == GC_CHAR_ASIN ) {
        m_pkBackAsin->ToggleRender( true );
        m_pkStaticAsin->ToggleRender( true );
        m_pkStaticAsin->SetText( g_pkStrLoader->GetString( STR_ID_ASIN_HIDDEN_SKILL_TITLE ) );
    }

	if ( TABOPEN_NORMALSKILL == ( TABOPEN_NORMALSKILL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) )
	{
		m_pkTabSpecial->ToggleRender( true );
	}

	if ( TABOPEN_SPECIAL == ( TABOPEN_SPECIAL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ) 
	{
		m_pkTabNormalSkill->ToggleRender( true );
	}

	if ( TABOPEN_TRANSFORM == ( TABOPEN_TRANSFORM & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) )
	{
		m_pkTabTransform->ToggleRender(true);
	}

	
}

int KGCSkillTreeDlg::GetRemovePoint()
{
	if ( SiGCSKT()->IsUnlockAll() )
	{
		return 0;
	}
	return g_pItemMgr->GetInventoryItemDuration( ITEM_ID_SKILLUNTRAIN_CARD ) +
		g_pItemMgr->GetInventoryItemDuration( ITEM_ID_SKILLUNTRAIN_CARD2 ) + 
		g_pItemMgr->GetInventoryItemDuration( ITEM_ID_SKILLUNTRAIN_CARD3 );
}

void KGCSkillTreeDlg::UpdateTab()
{
	if( m_pkTabNormalSkillAct->IsRenderOn() )
	{
		ReloadSkillTree( "NormalSkill" );
	}
	else if( m_pkTabSpecialAct->IsRenderOn() )
	{
		ReloadSkillTree( "Special" );
	}
	else if (m_pkTabCommonSkillAct->IsRenderOn())
	{
		ReloadSkillTree( "CommonSkill" );
	}
	else if (m_pkTabActiveSkillAct->IsRenderOn())
	{
		ReloadSkillTree( "Active" );
	}
	else if (m_pkTabPassiveSkillAct->IsRenderOn())
	{
		ReloadSkillTree( "Passive" );        
	}
	else
	{
		ReloadSkillTree("Transform");
	}
}

void KGCSkillTreeDlg::InitOpenChar()
{
	int i = 0, j = 0;
	KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

	m_mapOpenCharIndex.clear();

	std::string strSkillTreeUI = "SkillTreeUI_kr.lua";

	if( GCFUNC::LoadLuaScript( luaMgr, strSkillTreeUI ) == false )
		return;

	LUA_BEGIN_TABLE( "SkillTreeUI", ASSERT(!"���̺��� �̻���!!!" ) )
	{
		for( i = 0 ; i < GC_CHAR_NUM ; ++i )
		{

			for( j = 0 ; j < NUM_JOB_LEVEL ; ++j )
			{
				if( SUCCEEDED( luaMgr.BeginTable( i + j * GC_CHAR_NUM ) ) )
				{
					m_mapOpenCharIndex[i].insert( j );

                    UCHAR ucOpneTab = 0;
                    if( SUCCEEDED( luaMgr.BeginTable( "NormalSkill" ) ) )
                    {
                        ucOpneTab = TABOPEN_NORMALSKILL;
                        luaMgr.EndTable();
                    }
                    if( SUCCEEDED( luaMgr.BeginTable( "Special" ) ) )
                    {
                        ucOpneTab |= TABOPEN_SPECIAL;
                        luaMgr.EndTable();
                    }
                    if( SUCCEEDED( luaMgr.BeginTable( "CommonSkill" ) ) )
                    {
                        ucOpneTab |= TABOPEN_COMMONSKILL;
                        luaMgr.EndTable();
                    }
                    if( SUCCEEDED( luaMgr.BeginTable( "Active" ) ) )
                    {
                        ucOpneTab |= TABOPEN_ACTIVE;
                        luaMgr.EndTable();
                    }
                    if( SUCCEEDED( luaMgr.BeginTable( "Passive" ) ) )
                    {
                        ucOpneTab |= TABOPEN_PASSIVE;
                        luaMgr.EndTable();
                    }
                    if( SUCCEEDED( luaMgr.BeginTable( "Transform" ) ) )
                    {
                        ucOpneTab |= TABOPEN_TRANSFORM;
                        luaMgr.EndTable();
                    }
                    m_mapOpenTab[i][j] = ucOpneTab;

					luaMgr.EndTable();
				}
			}
		}
	}
	LUA_END_TABLE( ASSERT(!"���̺� ��뿡 ������ ����") );
}

void KGCSkillTreeDlg::UpdatePromotionBtn()
{
    std::map< int, std::set< int > >::const_iterator mit = m_mapOpenCharIndex.find( m_iSelectedChar );
    if( mit == m_mapOpenCharIndex.end() ) {             // �ȿ��� ĳ���̸� ������ �� �� 
        for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
            m_pkPromotion[i]->Lock( true );
            m_pkPromotion[i]->InitState( false );
        }
        return;
    }

#if defined(SKILL_TREE_OPEN_BY_SERVERSCRIPT)
    for( int i=0 ; i<NUM_JOB_LEVEL ; ++i ) { 
	std::set<DWORD> setSkillEnable = g_kGlobalValue.GetCharSkillEnable();

#if defined(NATION_IDN)
        if( mit->second.find( i ) == mit->second.end() ) { 
            m_pkPromotion[i]->TogggleRender( false );
                continue;
            }
#endif

        if( setSkillEnable.find( m_iSelectedChar + ( GC_CHAR_NUM * i ) ) != setSkillEnable.end() ) { 
            if( (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_DECANEE) && i > 0 ) {
                m_pkPromotion[NUM_JOB_LEVEL]->Lock( false );
                m_pkPromotion[NUM_JOB_LEVEL]->InitState( true, SiGCSKT()->m_bPromotionSelectable, this );
                m_pkPromotion[NUM_JOB_LEVEL]->Activate( m_iPromotion == i );
				}
            else { 
                m_pkPromotion[i]->Lock( false );
                m_pkPromotion[i]->InitState( true, SiGCSKT()->m_bPromotionSelectable, this );
                m_pkPromotion[i]->Activate( m_iPromotion == i );
			}

            if( !SiGCSKT()->IsUnlockAll() ) { 
                if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).iPromotionLevel < i ) { 
                    m_pkPromotion[i]->Lock( true );
			}
		}
	}
        else { 
            m_pkPromotion[i]->Lock( true );
            m_pkPromotion[i]->InitState( true, true, this );
        }

        if( (m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_DECANEE)  && i > 1 ) {
            m_pkPromotion[i]->ToggleRender( false );
        }
    }
#else
    for( int i=0 ; i<NUM_JOB_LEVEL ; ++i ) { 
        if( mit->second.find(i) != mit->second.end() ) {
            if( (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_DECANEE) && i>0 ) { 
                m_pkPromotion[NUM_JOB_LEVEL]->Lock( false );
                m_pkPromotion[NUM_JOB_LEVEL]->InitState( true, SiGCSKT()->m_bPromotionSelectable, this );
                m_pkPromotion[NUM_JOB_LEVEL]->Activate( m_iPromotion == i );
            }
            else { 
                m_pkPromotion[i]->Lock( false );
                m_pkPromotion[i]->InitState( true, SiGCSKT()->m_bPromotionSelectable, this );
                m_pkPromotion[i]->Activate( m_iPromotion == i );
            }

            if( !SiGCSKT()->IsUnlockAll() ) { 
                if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).iPromotionLevel < i ) { 
                    m_pkPromotion[i]->Lock( true );
                }
            }
        }
        else { 
            m_pkPromotion[i]->Lock( true );
            m_pkPromotion[i]->InitState( true, true, this );
        }

        if( (m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_DECANEE)  && i > 1 ) { 
            m_pkPromotion[i]->ToggleRender( false );
        }
    }
#endif

    if( !SiGCSKT()->IsUnlockAll() ) { 
        if( m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_DECANEE ) {
            if( SiGCSKT()->GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( m_iSelectedChar ).iPromotionLevel < 1 ) { 
                m_pkPromotion[ NUM_JOB_LEVEL ]->Lock( true );
            }
        }
    }

    if( m_iSelectedChar != GC_CHAR_ASIN && m_iSelectedChar != GC_CHAR_LIME && m_iSelectedChar != GC_CHAR_DECANEE ) {
        m_pkPromotion[NUM_JOB_LEVEL]->ToggleRender( false );
    }

}

void KGCSkillTreeDlg::OnClickCharPromotion( KD3DWnd* pWnd, DWORD dwCode )
{
	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;

	ASSERT( pWnd != NULL );    

    for( int i=0 ; i<=NUM_JOB_LEVEL ; ++i ) { 
        if( m_pkPromotion[i] == pWnd ) { 
            if( (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_DECANEE) && i == NUM_JOB_LEVEL ) {
                m_iPromotion = 1;
            }
            else { 
                m_iPromotion = i;
            }
            m_pkPromotion[i]->Activate( true );
        }
        else { 
            m_pkPromotion[i]->Activate( false );
        }
    }

    UpdateSkillTab();

    if( TABOPEN_NORMALSKILL == ( TABOPEN_NORMALSKILL & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ||
        TABOPEN_ACTIVE == ( TABOPEN_ACTIVE & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) ) {
            OnClickNormalSkillTab();
            if( m_iSelectedChar == GC_CHAR_RYAN ) {
                m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(36,356));
                m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(45,361));
			} else if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) {
				m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
                m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,158));
            } else {
                m_pkTabCommonSkill->SetWindowPos( D3DXVECTOR2( 35, 420 ) );
                m_pkTabCommonSkillAct->SetWindowPos( D3DXVECTOR2( 30, 414 ) );
            }            
    } else {
        OnClickCommonSkillTab();
        if( m_iSelectedChar == GC_CHAR_RYAN ) {
            m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(36,158));
            m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(45,152));
            
            m_pkTabTransform->ToggleRender(false);
            m_pkTabTransformAct->ToggleRender(false);
		} else if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) {
            m_pkTabCommonSkillAct->SetWindowPos(D3DXVECTOR2(30,152));
            m_pkTabCommonSkill->SetWindowPos(D3DXVECTOR2(35,158));
        } else {
            m_pkTabCommonSkill->SetWindowPos( D3DXVECTOR2( 35, 158 ) );
            m_pkTabCommonSkillAct->SetWindowPos( D3DXVECTOR2( 30, 152 ) );
        }        
    } 

	Update();	
}

void KGCSkillTreeDlg::OnClickFlipBtn( KD3DWnd* pWnd, DWORD dwCode )
{
	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;

	if( pWnd == m_pkFlipButton[0] )
	{
		m_pkFlipButton[1]->ToggleRender( true );
		m_pkFlipButton[0]->ToggleRender( false );
		HideSubSkillBox( 1 );
		ShowSubSkillBox( 0 );
	}
	else if( pWnd == m_pkFlipButton[1] )
	{
		m_pkFlipButton[1]->ToggleRender( false );
		m_pkFlipButton[0]->ToggleRender( true );
		HideSubSkillBox( 0 );
		ShowSubSkillBox( 1 );
	}
	UpdateFieldSize( m_strCurrentTab );
	ReloadSkillTree( m_strCurrentTab );
}

void KGCSkillTreeDlg::HideSubSkillBox( int iFieldIndex )
{
	m_pkSkilltreeField[iFieldIndex]->HideSubSkillBox();
}

void KGCSkillTreeDlg::ShowSubSkillBox( int iFieldIndex )
{
	m_pkSkilltreeField[iFieldIndex]->ShowSubSkillBox();
}

bool KGCSkillTreeDlg::UpdateFieldSize( std::string strTable )
{

	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return false;

	std::string strSkillTreeUI = "SkillTreeUI_kr.lua";
	if( GCFUNC::LoadLuaScript( luaMgr, strSkillTreeUI ) == false )
		return false ;

	LUA_BEGIN_TABLE( "SkillTreeUI", return false )
	{
		LUA_BEGIN_TABLE( m_iSelectedChar + m_iPromotion * GC_CHAR_NUM, return false)
		{
			if( SUCCEEDED( luaMgr.BeginTable( strTable.c_str() ) ) )
			{
				if( SUCCEEDED( luaMgr.BeginTable( "Left") ) )
				{

					int iTitleID = 0, width = 0, height = 0;
					float x = 0.0f, y = 0.0f;
					int R = 0, G = 0, B = 0;
					if( SUCCEEDED( luaMgr.BeginTable( m_pkFlipButton[0]->IsRenderOn() ? "FlipedState":"NormalState" ) ) )
					{
						LUA_GET_VALUE( "x", x, return false );
						LUA_GET_VALUE( "y", y, return false );
						LUA_GET_VALUE( "width", width , return false );
						LUA_GET_VALUE( "height", height, return false );
						LUA_GET_VALUE_DEF( "Red", R, 11 );
						LUA_GET_VALUE_DEF( "Green", G, 53 );
						LUA_GET_VALUE_DEF( "Blue", B, 69 );
						luaMgr.EndTable();
					}

					m_pkLeftField->SetWidth( width );
					m_pkLeftField->SetHeight( height );
					m_pkLeftField->SetWindowPosDirect( D3DXVECTOR2(x, y) );
					m_pkLeftField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );

					if( SUCCEEDED( luaMgr.BeginTable( "FlipBtnPos" ) ) )
					{
						LUA_GET_VALUE( "x", x, return false );
						LUA_GET_VALUE( "y", y, return false );
						m_pkFlipButton[0]->SetWindowPosDirect( D3DXVECTOR2(x,y) );
                        m_pkTypeSwapBtn->SetWindowPosDirect( D3DXVECTOR2(x,y) );
						luaMgr.EndTable();
					}

					LUA_END_TABLE( return false );

					if( SUCCEEDED( luaMgr.BeginTable( "Right" ) ) )
					{

						int iTitleID = 0, width = 0, height = 0;
						float x = 0.0f, y = 0.0f;
						int R = 0, G = 0, B = 0;
						if( SUCCEEDED( luaMgr.BeginTable( m_pkFlipButton[1]->IsRenderOn() ? "FlipedState":"NormalState" ) ) )
						{
							LUA_GET_VALUE( "x", x, return false );
							LUA_GET_VALUE( "y", y, return false );
							LUA_GET_VALUE( "width", width , return false );
							LUA_GET_VALUE( "height", height, return false );
							LUA_GET_VALUE_DEF( "Red", R, 11 );
							LUA_GET_VALUE_DEF( "Green", G, 53 );
							LUA_GET_VALUE_DEF( "Blue", B, 69 );
							luaMgr.EndTable();
						}

						m_pkRightField->SetWidth( width );
						m_pkRightField->SetHeight( height );
						m_pkRightField->SetWindowPosDirect( D3DXVECTOR2(x, y) );
						m_pkRightField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );

						if( SUCCEEDED( luaMgr.BeginTable( "FlipBtnPos" ) ) )
						{
							LUA_GET_VALUE( "x", x, return false );
							LUA_GET_VALUE( "y", y, return false );
							m_pkFlipButton[1]->SetWindowPosDirect( D3DXVECTOR2(x,y) );
//                            m_pkTypeSwapBtn->SetWindowPosDirect( D3DXVECTOR2(x,y) );
							luaMgr.EndTable();
						}

						LUA_END_TABLE( return false );
					}
					else
					{
						if ( true == IsClickPassiveTab() ) {
                            m_pkLeftField->ForceSetWindowColor( D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
                        }
                        else {
                            m_pkLeftField->SetWidth( 337 * 2 + 4 );
                        }
                        
						m_pkRightField->ToggleRender( false );
					}
				}
				else if( SUCCEEDED( luaMgr.BeginTable( "LeftGroup" ) ) )
				{
					int iTitleID = 0, width = 0, height = 0;
					float x = 0.0f, y = 0.0f;
					int R = 0, G = 0, B = 0;
					if( SUCCEEDED( luaMgr.BeginTable( "NormalState" ) ) )
					{
						LUA_GET_VALUE( "x", x, return false );
						LUA_GET_VALUE( "y", y, return false );
						LUA_GET_VALUE( "width", width , return false );
						LUA_GET_VALUE( "height", height, return false );
						LUA_GET_VALUE_DEF( "Red", R, 11 );
						LUA_GET_VALUE_DEF( "Green", G, 53 );
						LUA_GET_VALUE_DEF( "Blue", B, 69 );
						luaMgr.EndTable();
					}

					m_pkLeftField->SetWidth( width );
					m_pkLeftField->SetHeight( height );
					m_pkLeftField->SetWindowPosDirect( D3DXVECTOR2(x, y) );
					m_pkLeftField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );

					luaMgr.EndTable();

					if( SUCCEEDED( luaMgr.BeginTable( "RightGroup" ) ) )
					{
						int iTitleID = 0, width = 0, height = 0;
						float x = 0.0f, y = 0.0f;
						int R = 0, G = 0, B = 0;
						if( SUCCEEDED( luaMgr.BeginTable( "NormalState" ) ) )
						{
							LUA_GET_VALUE( "x", x, return false );
							LUA_GET_VALUE( "y", y, return false );
							LUA_GET_VALUE( "width", width , return false );
							LUA_GET_VALUE( "height", height, return false );
							LUA_GET_VALUE_DEF( "Red", R, 11 );
							LUA_GET_VALUE_DEF( "Green", G, 53 );
							LUA_GET_VALUE_DEF( "Blue", B, 69 );
							luaMgr.EndTable();
						}

						m_pkRightField->SetWidth( width );
						m_pkRightField->SetHeight( height );
						m_pkRightField->SetWindowPosDirect( D3DXVECTOR2(x, y) );
						m_pkRightField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );

						luaMgr.EndTable();
					}
					else
					{
						m_pkLeftField->SetWidth( 337 * 2 + 4 );
						m_pkRightField->ToggleRender( false );
					}


					if(SUCCEEDED( luaMgr.BeginTable( "BetWeenGroup" ) ))
					{
						int width = 223, height = 427;
						int x = 80, y = 148;
                        int R = 0, G = 0, B = 0;
                        bool bForcedValue = true;
                        if( SUCCEEDED( luaMgr.BeginTable( "NormalState" ) ) )
						{
                            bForcedValue = false;
                            LUA_GET_VALUE( "x", x, return false );
                            LUA_GET_VALUE( "y", y, return false );
                            LUA_GET_VALUE( "width", width , return false );
                            LUA_GET_VALUE( "height", height, return false );
                            LUA_GET_VALUE_DEF( "Red", R, 11 );
                            LUA_GET_VALUE_DEF( "Green", G, 53 );
                            LUA_GET_VALUE_DEF( "Blue", B, 69 );
                            luaMgr.EndTable();
                        }
                        
                        if( bForcedValue )
                        {
                            m_pkLeftField->SetWidth( width );
                            m_pkLeftField->SetHeight( height );
                            m_pkRightField->SetWidth( width );
                            m_pkRightField->SetHeight( height );
                            m_pkbetweenField->SetWidth( width + 1 );
                            m_pkbetweenField->SetHeight( height );

                            m_pkLeftField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>(x ), static_cast<float>(y) ) );
                            m_pkRightField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>(x + width + 4), static_cast<float>(y) ) );
                            m_pkbetweenField->SetWindowPosDirect( D3DXVECTOR2( static_cast<float>(x + width*2 + 8), static_cast<float>(y) ) );
                            m_pkbetweenField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, 11, 53, 69 ) );
                            m_pkbetweenField->ToggleRender( true );
                        }
                        else
                        {
                            m_pkbetweenField->SetWidth( width );
                            m_pkbetweenField->SetHeight( height );
                            m_pkbetweenField->SetWindowPosDirect( D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y)) );
                            m_pkbetweenField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );
                        }

						luaMgr.EndTable();						
					}
					else
					{
						m_pkbetweenField->ToggleRender( false );
					}

                    if( SUCCEEDED( luaMgr.BeginTable( "Between" ) ) )
                    {

                        int iTitleID = 0, width = 0, height = 0;
                        float x = 0.0f, y = 0.0f;
                        int R = 0, G = 0, B = 0;
                        if( SUCCEEDED( luaMgr.BeginTable( m_pkFlipButton[1]->IsRenderOn() ? "FlipedState":"NormalState" ) ) )
                        {
                            LUA_GET_VALUE( "x", x, return false );
                            LUA_GET_VALUE( "y", y, return false );
                            LUA_GET_VALUE( "width", width , return false );
                            LUA_GET_VALUE( "height", height, return false );
                            LUA_GET_VALUE_DEF( "Red", R, 11 );
                            LUA_GET_VALUE_DEF( "Green", G, 53 );
                            LUA_GET_VALUE_DEF( "Blue", B, 69 );
                            luaMgr.EndTable();
                        }

                        m_pkbetweenField->SetWidth( width );
                        m_pkbetweenField->SetHeight( height );
                        m_pkbetweenField->SetWindowPosDirect( D3DXVECTOR2(x, y) );
                        m_pkbetweenField->ForceSetWindowColor( D3DCOLOR_ARGB( 255, R, G, B ) );
                        m_pkbetweenField->ToggleRender( true );

                        if( SUCCEEDED( luaMgr.BeginTable( "FlipBtnPos" ) ) )
                        {
                            LUA_GET_VALUE( "x", x, return false );
                            LUA_GET_VALUE( "y", y, return false );
                            m_pkFlipButton[1]->SetWindowPosDirect( D3DXVECTOR2(x,y) );
                            //                            m_pkTypeSwapBtn->SetWindowPosDirect( D3DXVECTOR2(x,y) );
                            luaMgr.EndTable();
                        }

                        LUA_END_TABLE( return false );
                    }
                    else
                    {
                        m_pkbetweenField->ToggleRender( false );
                    }
					
				}
				luaMgr.EndTable();
			}
		}
		LUA_END_TABLE( return false )
	}
	LUA_END_TABLE( return false )

	return true;
}

void KGCSkillTreeDlg::FrameMove_FlipCheck()
{
	if( m_bFieldFlip == false )
		return;

	ASSERT( g_pkInput != NULL );

	if( CheckFlipCondition() == false )
		return;


	if( m_pkFlipButton[0]->IsRenderOn() == true )
	{
		OnClickFlipBtn( m_pkFlipButton[0], KD3DWnd::D3DWE_BUTTON_CLICK );
	}
	else
	{
		OnClickFlipBtn( m_pkFlipButton[1], KD3DWnd::D3DWE_BUTTON_CLICK );
	}
}

bool KGCSkillTreeDlg::CheckFlipCondition()
{
	D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
	if( g_pkInput->BtnUp( KInput::MBLEFT ) == TRUE )
	{
		if( (m_pkFlipButton[0]->CheckPosInWindowBound( vMousePos ) == true) || 
			(m_pkFlipButton[1]->CheckPosInWindowBound( vMousePos ) == true) )
			return false;

		if( m_pkFlipButton[0]->IsRenderOn() == true && m_pkLeftField->CheckPosInWindowBound( vMousePos ) == true )
			return true;

		if( m_pkFlipButton[1]->IsRenderOn() == true && m_pkRightField->CheckPosInWindowBound( vMousePos ) == true )
			return true;
	}


	if( g_pkInput->IsUp(DIK_X) == false )
		return false;

	return true;
}

void KGCSkillTreeDlg::ThreadSafeUpdate()
{
	m_bProcedureUpdateCall = true;
}

void KGCSkillTreeDlg::OnClickSwapBtn()
{   
    if( m_pkFlipButton[0]->IsRenderOn() == true )
    {
        OnClickFlipBtn( m_pkFlipButton[0], KD3DWnd::D3DWE_BUTTON_CLICK );
    }
    else
    {
        OnClickFlipBtn( m_pkFlipButton[1], KD3DWnd::D3DWE_BUTTON_CLICK );
    }
}

bool KGCSkillTreeDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    if( kInfo_.m_lParam == OPEN_NEW_CHAR )
    {
        m_iSelectedChar = static_cast<int>(kInfo_.m_lParam2);
        m_iPromotion = 0;
        m_bOpenNewChar = true;
    }
    
    this->Update();    
    this->UpdateTab();
    return true;
}

void KGCSkillTreeDlg::OnClickActiveTab()
{    
	m_pkTabSpecialAct->ToggleRender( false );
	m_pkTabSpecial->ToggleRender( false );
	m_pkTabNormalSkill->ToggleRender( false );
	m_pkTabNormalSkillAct->ToggleRender( false );
	m_pkTabCommonSkill->ToggleRender( false );
	m_pkTabCommonSkillAct->ToggleRender( false );

    m_pkTabPassiveSkillAct->ToggleRender( false );
    if( TABOPEN_PASSIVE == (TABOPEN_PASSIVE & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) )
	    m_pkTabPassiveSkill->ToggleRender( true );
    else
        m_pkTabPassiveSkill->ToggleRender( false );
	m_pkTabActiveSkill->ToggleRender( false );
	m_pkTabActiveSkillAct->ToggleRender( true );

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
	m_pkbetweenField->ToggleRender(false);

    m_pkBackAsin->ToggleRender( false );
    m_pkStaticAsin->ToggleRender( false );

	if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_LIME || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) {
        m_pkTabCommonSkill->ToggleRender( true );
        m_pkTabCommonSkillAct->ToggleRender( false );
    }
    
    CheckDioTab();
	SetPassiveTabClick( false );
    ReloadSkillTree( "Active" );
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();
}

void KGCSkillTreeDlg::OnClickPassiveTab()
{
	m_pkTabSpecialAct->ToggleRender( false );
	m_pkTabSpecial->ToggleRender( false );
	m_pkTabNormalSkill->ToggleRender( false );
	m_pkTabNormalSkillAct->ToggleRender( false );
	m_pkTabCommonSkill->ToggleRender( false );
	m_pkTabCommonSkillAct->ToggleRender( false );

    if( TABOPEN_PASSIVE == (TABOPEN_PASSIVE & m_mapOpenTab[m_iSelectedChar][m_iPromotion] ) )
	    m_pkTabPassiveSkillAct->ToggleRender( true );
    else
        m_pkTabPassiveSkillAct->ToggleRender( false );
	m_pkTabPassiveSkill->ToggleRender( false );
	m_pkTabActiveSkill->ToggleRender( true );
	m_pkTabActiveSkillAct->ToggleRender( false );	

	m_pkFlipButton[0]->ToggleRender( true );
	m_pkFlipButton[1]->ToggleRender( false );
	m_pkbetweenField->ToggleRender(false);

    m_pkBackAsin->ToggleRender( false );
    m_pkStaticAsin->ToggleRender( false );

	if (m_iSelectedChar == GC_CHAR_ASIN || m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) {
        m_pkTabCommonSkill->ToggleRender( true );
        m_pkTabCommonSkillAct->ToggleRender( false );
    } else if( m_iSelectedChar == GC_CHAR_LIME ) {
        m_pkTabCommonSkill->ToggleRender( true );
        m_pkTabCommonSkillAct->ToggleRender( false );
        m_pkBackAsin->ToggleRender( true );
        m_pkStaticAsin->ToggleRender( true );
        m_pkStaticAsin->SetText( g_pkStrLoader->GetString( STR_ID_LIME_HIDDEN_SKILL_TITLE ) );
    }

    GetChildByName( "frame_back_dio" )->ToggleRender( false );
    GetChildByName( "frame_dio4" )->ToggleRender( false );
	if ( GetChildByName( "wnd_dio_devil" ) )
		GetChildByName( "wnd_dio_devil" )->ToggleRender( false );
	if ( GetChildByName( "wnd_dio_weapon" ) )
		GetChildByName( "wnd_dio_weapon" )->ToggleRender( false );
	SetPassiveTabClick( true );
	ReloadSkillTree( "Passive" );
	for (int i=0 ; i<2 ; i++)
		m_pkSkilltreeField[i]->ClearParticle();

}

bool KGCSkillTreeDlg::CheckSkillOpenChar( int iChar )
{
	return m_mapOpenCharIndex.find(iChar) != m_mapOpenCharIndex.end();
}

int KGCSkillTreeDlg::GetEndSkillOpenPromotion( int iChar, int iPromotion )
{
    int iReturn = 0;

	std::set<DWORD>::iterator sit = g_kGlobalValue.GetCharSkillEnable().find(iChar + GC_CHAR_NUM * iPromotion);
	if (sit != g_kGlobalValue.GetCharSkillEnable().end())
		return iPromotion;
//     std::map<int, std::set<int> >::iterator mit = m_mapOpenCharIndex.find(iChar);
//     if( mit != m_mapOpenCharIndex.end() )
//     {
//         std::set<int>::iterator sit = mit->second.find( iPromotion );
//         if( sit != mit->second.end() )
//         {
//             iReturn = *sit;
//         }
//         else
//         {
//             iReturn = max(int(mit->second.size()) - 1, 0);
//         }
//     }
    return iReturn;
}

void KGCSkillTreeDlg::UpdateSkillTab()
{
    // Ư�� �ǵ� ���� ��Ű�� ���� �ϵ��ڵ�    
    GetChildByName( "frame_dio4" )->ToggleRender( false );
    GetChildByName( "frame_back_dio" )->ToggleRender( false );

    GetChildByName( "tab_active_skill" )->ToggleRender( false );
    GetChildByName( "tab_active_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_passive_skill" )->ToggleRender( false );
    GetChildByName( "tab_passive_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_ultimate_skill" )->ToggleRender( false );
    GetChildByName( "tab_ultimate_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_fightstance_skill" )->ToggleRender( false );
    GetChildByName( "tab_fightstance_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_aura_skill" )->ToggleRender( false );
    GetChildByName( "tab_aura_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_selfaura_skill" )->ToggleRender( false );
    GetChildByName( "tab_selfaura_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_weapon_skill" )->ToggleRender( false );
    GetChildByName( "tab_weapon_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_devil_skill" )->ToggleRender( false );
    GetChildByName( "tab_devil_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_goddess_skill" )->ToggleRender( false );
    GetChildByName( "tab_goddess_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_evil_skill" )->ToggleRender( false );
    GetChildByName( "tab_evil_skill_act" )->ToggleRender( false );
    GetChildByName( "tab_rin_transform" )->ToggleRender( false );
    GetChildByName( "tab_rin_transform_act" )->ToggleRender( false );
    GetChildByName( "tab_special_type2" )->ToggleRender( false );
    GetChildByName( "tab_special_type2_act" )->ToggleRender( false );

	if (GetChildByName( "wnd_dio_weapon" ))
		GetChildByName( "wnd_dio_weapon" )->ToggleRender( false );

    if( m_iSelectedChar == GC_CHAR_ASIN ) {
        GetChildByName( "tab_active_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_active_skill" );
        m_pkTabActiveSkillAct =GetChildByName( "tab_active_skill_act" );
        if( m_iPromotion == 1 ) {
            GetChildByName( "tab_fightstance_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_fightstance_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_fightstance_skill" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_fightstance_skill_act" );
        } else {
            GetChildByName( "tab_ultimate_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_ultimate_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_ultimate_skill" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_ultimate_skill_act" );            
        }
        m_pkTabPassiveSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
        m_pkTabPassiveSkill->SetWindowPos(D3DXVECTOR2(34,420));
    } else if( m_iSelectedChar == GC_CHAR_LIME ) {
        GetChildByName( "tab_active_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_active_skill" );
        m_pkTabActiveSkillAct =GetChildByName( "tab_active_skill_act" );
        if( m_iPromotion == 1 ) {
            GetChildByName( "tab_selfaura_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_selfaura_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_selfaura_skill" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_selfaura_skill_act" );
        } else {
            GetChildByName( "tab_aura_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_aura_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_aura_skill" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_aura_skill_act" );
        }
        m_pkTabPassiveSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
        m_pkTabPassiveSkill->SetWindowPos(D3DXVECTOR2(34,420));
    } else if( m_iSelectedChar == GC_CHAR_EDEL || m_iSelectedChar == GC_CHAR_BEIGAS || m_iSelectedChar == GC_CHAR_PERYTON || m_iSelectedChar == GC_CHAR_DECANEE) {
        GetChildByName( "tab_active_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_active_skill" );
        m_pkTabActiveSkillAct =GetChildByName( "tab_active_skill_act" );
        GetChildByName( "tab_passive_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
        GetChildByName( "tab_passive_skill_act" )->ToggleRender( m_pkTabPassiveSkillAct->IsRenderOn() );
        m_pkTabPassiveSkill = GetChildByName( "tab_passive_skill" );
        m_pkTabPassiveSkillAct = GetChildByName( "tab_passive_skill_act" );
        m_pkTabPassiveSkillAct->SetWindowPos(D3DXVECTOR2(30,414));
        m_pkTabPassiveSkill->SetWindowPos(D3DXVECTOR2(34,420));
        GetChildByName( "tab_commonskill" )->ToggleRender( m_pkTabCommonSkill->IsRenderOn() );
        GetChildByName( "tab_commonskill_act" )->ToggleRender( m_pkTabCommonSkillAct->IsRenderOn() );
        m_pkTabCommonSkill = GetChildByName( "tab_special_type2" );
        m_pkTabCommonSkillAct = GetChildByName( "tab_special_type2_act" );
    } else if( m_iSelectedChar == GC_CHAR_DIO && m_iPromotion == 3 ) {
        GetChildByName( "tab_devil_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_devil_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_devil_skill" );
        m_pkTabActiveSkillAct = GetChildByName( "tab_devil_skill_act" );        
        GetChildByName( "tab_weapon_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
        GetChildByName( "tab_weapon_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
        m_pkTabPassiveSkill = GetChildByName( "tab_weapon_skill" );
        m_pkTabPassiveSkillAct = GetChildByName( "tab_weapon_skill_act" );

        GetChildByName( "tab_active_skill" )->ToggleRender( false );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( false );

    } else if( m_iSelectedChar == GC_CHAR_RIN && m_iPromotion > 0 && m_iPromotion < 3 ) {
        GetChildByName( "tab_goddess_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_goddess_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_goddess_skill" );
        m_pkTabActiveSkillAct = GetChildByName( "tab_goddess_skill_act" );        
        GetChildByName( "tab_evil_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
        GetChildByName( "tab_evil_skill_act" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
        m_pkTabPassiveSkill = GetChildByName( "tab_evil_skill" );
        m_pkTabPassiveSkillAct = GetChildByName( "tab_evil_skill_act" );

        GetChildByName( "tab_active_skill" )->ToggleRender( false );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( false );
    } else {
        if( m_iSelectedChar == GC_CHAR_RIN && m_iPromotion == 3 ) {
            GetChildByName( "tab_rin_transform" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_rin_transform_act" )->ToggleRender( m_pkTabPassiveSkillAct->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_rin_transform" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_rin_transform_act" );        
        } else {
            GetChildByName( "tab_passive_skill" )->ToggleRender( m_pkTabPassiveSkill->IsRenderOn() );
            GetChildByName( "tab_passive_skill_act" )->ToggleRender( m_pkTabPassiveSkillAct->IsRenderOn() );
            m_pkTabPassiveSkill = GetChildByName( "tab_passive_skill" );
            m_pkTabPassiveSkillAct = GetChildByName( "tab_passive_skill_act" );        
        }        

        GetChildByName( "tab_active_skill" )->ToggleRender( m_pkTabActiveSkill->IsRenderOn() );
        GetChildByName( "tab_active_skill_act" )->ToggleRender( m_pkTabActiveSkillAct->IsRenderOn() );
        m_pkTabActiveSkill = GetChildByName( "tab_active_skill" );
        m_pkTabActiveSkillAct = GetChildByName( "tab_active_skill_act" );

        GetChildByName( "tab_commonskill" )->ToggleRender( m_pkTabCommonSkill->IsRenderOn() );
        GetChildByName( "tab_commonskill_act" )->ToggleRender( m_pkTabCommonSkillAct->IsRenderOn() );
        m_pkTabCommonSkill = GetChildByName( "tab_commonskill" );
        m_pkTabCommonSkillAct = GetChildByName( "tab_commonskill_act" );

        GetChildByName( "tab_goddess_skill" )->ToggleRender( false );
        GetChildByName( "tab_goddess_skill_act" )->ToggleRender( false );
        GetChildByName( "tab_evil_skill" )->ToggleRender( false );
        GetChildByName( "tab_evil_skill_act" )->ToggleRender( false );
    }
}

void KGCSkillTreeDlg::SetPassiveTabClick( bool bIsActivePassiveTab )
{
    m_bActivePassive = bIsActivePassiveTab;
}

void KGCSkillTreeDlg::CheckDioTab()
{
    if( m_iSelectedChar == GC_CHAR_DIO ) {
        if( m_iPromotion < 3 ) {
            GetChildByName( "frame_back_dio" )->ToggleRender( true );
			if ( GetChildByName( "wnd_dio_devil" ) )
				GetChildByName( "wnd_dio_devil" )->ToggleRender( true );
			if ( GetChildByName( "wnd_dio_weapon" ) )
				GetChildByName( "wnd_dio_weapon" )->ToggleRender( true );
        } else {
            GetChildByName( "frame_dio4" )->ToggleRender( true );
			if ( GetChildByName( "wnd_dio_devil" ) )
				GetChildByName( "wnd_dio_devil" )->ToggleRender( false );
			if ( GetChildByName( "wnd_dio_weapon" ) )
				GetChildByName( "wnd_dio_weapon" )->ToggleRender( false );
        }
    } else {
		if ( GetChildByName( "wnd_dio_devil" ) )
			GetChildByName( "wnd_dio_devil" )->ToggleRender( false );
		if ( GetChildByName( "wnd_dio_weapon" ) )
			GetChildByName( "wnd_dio_weapon" )->ToggleRender( false );
    }
}

KD3DWnd* KGCSkillTreeDlg::GetTrainableSkillBox()
{
    // 
    KD3DWnd* pkWnd = NULL;

    // �ܺ��� �˻�
    if( m_pkTabNormalSkill->IsRenderOn() || m_pkTabNormalSkillAct->IsRenderOn() ){
        OnClickNormalSkillTab();
        pkWnd = FindTrainableSkillBox();

        if( pkWnd !=  NULL )
            return pkWnd;
    } 

    if ( m_pkTabSpecial->IsRenderOn() || m_pkTabSpecialAct->IsRenderOn() ) {
        OnClickSpeicalTab();
        pkWnd = FindTrainableSkillBox();

        if( pkWnd !=  NULL )
            return pkWnd;
    }

    if ( m_pkTabCommonSkill->IsRenderOn() || m_pkTabCommonSkillAct->IsRenderOn() ) {
        OnClickCommonSkillTab();
        pkWnd = FindTrainableSkillBox();

        if( pkWnd !=  NULL )
            return pkWnd;
    }

    if ( m_pkTabActiveSkill->IsRenderOn() ||m_pkTabActiveSkillAct->IsRenderOn() ) {
        OnClickActiveTab();
        pkWnd = FindTrainableSkillBox();

        if( pkWnd !=  NULL )
            return pkWnd;
    }

    if ( m_pkTabPassiveSkill->IsRenderOn() || m_pkTabPassiveSkillAct->IsRenderOn() ) {
        OnClickPassiveTab();
        pkWnd = FindTrainableSkillBox();

        if( pkWnd !=  NULL )
            return pkWnd;
    }

    return pkWnd;
}

KD3DWnd* KGCSkillTreeDlg::FindTrainableSkillBox()
{
    //
    KD3DWnd* pkWnd = NULL;

    // KGCSkillBox
    pkWnd = m_pkSkilltreeField[0]->GetTrainableSkillBox();

    if( pkWnd == NULL )
        pkWnd = m_pkSkilltreeField[1]->GetTrainableSkillBox();

    if( pkWnd != NULL ) {
        return pkWnd;
    }

    // KGCSkillBoxSub
    pkWnd = m_pkSkilltreeField[0]->GetTrainableSubSkillBox();

    if( pkWnd == NULL )
        pkWnd = m_pkSkilltreeField[1]->GetTrainableSubSkillBox();

    if( pkWnd != NULL ) {
        return pkWnd;
    }

    return pkWnd;
}