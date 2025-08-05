#include "stdafx.h"
#include "GCOption.h"

#include "GCOptionSound.h"
#include "GCOptionMusic.h"
//
//
//
#include "../MyD3D.h"



//

//
#include "GCChatBox.h"
#include "GCCashShopScene.h"
#include "GCGPShopScene.h"
#include "GCSearchBar.h"
#include "KGCInviteManager.h"
#include "GCCheckBox.h"
#include "../GCStateMachine.h"
#include "../GCRenderManager.h"
#include "gcui/GCKeySettingWnd.h"
#include "KDInput.h"

IMPLEMENT_CLASSNAME( KGCOption );
IMPLEMENT_WND_FACTORY( KGCOption );
IMPLEMENT_WND_FACTORY_NAME( KGCOption, "gc_option" );

KGCOption::KGCOption( void )
{
#if defined( USE_JOYSTICK )
	m_pkJoystickBtn         = NULL;
	m_pkKeyBoardBtn         = NULL;
#endif
    m_pkGraphicQuality      = NULL;
    m_pkSound               = NULL;
    m_pkMusic               = NULL;
    m_pkWhisperCheckBox     = NULL;
    m_pkWhisperCheck        = NULL;
    m_pkHitEffectCheckBox   = NULL;
    m_pkHitEffectCheck      = NULL;
    m_pkMotionBlurCheck     = NULL;
    m_pkMotionBlurCheckBox  = NULL;
    m_pkAutoGraphicCheckBox = NULL;
    m_pkAutoGraphicCheck    = NULL;
    m_pkClose               = NULL;
#if defined( USE_JOYSTICK )
	m_pkControllStr         = NULL;
#endif
    m_pkTitleStr            = NULL;
    m_pkGraphicStr          = NULL;
    m_pkAudioStr            = NULL;
    m_pkSettingStr          = NULL;
    m_pkWhisperStr          = NULL;
    m_pkHitEffectStr        = NULL;
    m_pkMotionBlurStr       = NULL;
    m_pkAutoGraphicStr      = NULL;
    m_pkKeySettingStr       = NULL;
    m_pkKeySettingWnd       = NULL;
	m_pkSkillKeySettingWnd  = NULL;
    m_pkSkillEffectStr      = NULL;
    m_pkSkillEffectCheck    = NULL;
    m_pkSkillEffectCheckBox = NULL;
    m_pkUsingFPCheck        = NULL;
    m_pkUsingVSCheck        = NULL;
    m_pkUsingFPCheckBox     = NULL;
    m_pkUsingVSCheckBox     = NULL;
    m_pkVertexShaderStr     = NULL;
    m_pkFixedPipeStr        = NULL;
    m_pkDenyInviteCheckBox  = NULL;
    m_pkDenyInviteStr       = NULL;
    m_pkVideoComboBox       = NULL;
	m_pkMultiSamplingComboBox = NULL;
	m_pkVideoStr            = NULL;
	m_pkMultiSamplingStr    = NULL;
    m_pkItemBoxAlphaAniCheckBox = NULL;
    m_pkItemBoxAlphaAniStr  = NULL;
    m_pkPetViewStr = NULL;
    m_pkPetViewCheckBox = NULL;
    m_pkBasicEquipStr = NULL;
    m_pkBasicEquipCheckBox = NULL;
    m_pkRoomAnimationStr = NULL;
    m_pkRoomAnimationCheckBox = NULL;

    LINK_CONTROL( "graphic_quality",        m_pkGraphicQuality );
    LINK_CONTROL( "sound",                  m_pkSound );
    LINK_CONTROL( "music",                  m_pkMusic );
    LINK_CONTROL( "option_close",           m_pkClose );
    LINK_CONTROL( "title",					m_pkTitleStr );
    LINK_CONTROL( "graphic_title",         m_pkGraphicStr );
    LINK_CONTROL( "audio_title",           m_pkAudioStr );
    LINK_CONTROL( "setting_title",         m_pkSettingStr );
    LINK_CONTROL( "whisper_checkbox",       m_pkWhisperCheckBox );
    LINK_CONTROL( "whisper_check",          m_pkWhisperCheck );
    LINK_CONTROL( "whisper_str",            m_pkWhisperStr );
    LINK_CONTROL( "hiteffect_checkbox",     m_pkHitEffectCheckBox );
    LINK_CONTROL( "hiteffect_check",        m_pkHitEffectCheck );
    LINK_CONTROL( "hiteffect_str",          m_pkHitEffectStr );
    LINK_CONTROL( "auto_graphic_checkbox",  m_pkAutoGraphicCheckBox );
    LINK_CONTROL( "auto_graphic_check",     m_pkAutoGraphicCheck );
    LINK_CONTROL( "auto_graphic_str",       m_pkAutoGraphicStr );
    LINK_CONTROL( "motionblur_checkbox",    m_pkMotionBlurCheckBox );
    LINK_CONTROL( "motionblur_check",       m_pkMotionBlurCheck );
    LINK_CONTROL( "motionblur_str",         m_pkMotionBlurStr );
    LINK_CONTROL( "keysetting_title",      m_pkKeySettingStr );
    LINK_CONTROL( "key_setting_wnd",        m_pkKeySettingWnd );
	LINK_CONTROL( "key_setting_wnd2",        m_pkSkillKeySettingWnd );
    LINK_CONTROL( "skill_effect_str",       m_pkSkillEffectStr );
    LINK_CONTROL( "skill_effect_check",     m_pkSkillEffectCheck );
    LINK_CONTROL( "skill_effect_checkbox",  m_pkSkillEffectCheckBox );
    LINK_CONTROL( "fixed_pipe_check",       m_pkUsingFPCheck );
    LINK_CONTROL( "vertex_shader_check",    m_pkUsingVSCheck );
    LINK_CONTROL( "fixed_pipe_checkbox",    m_pkUsingFPCheckBox );
    LINK_CONTROL( "vertex_shader_checkbox", m_pkUsingVSCheckBox );
    LINK_CONTROL( "vertex_shader_str",      m_pkVertexShaderStr );
    LINK_CONTROL( "fixed_pipe_str",         m_pkFixedPipeStr );
    LINK_CONTROL( "deny_invite_str",        m_pkDenyInviteStr );
    LINK_CONTROL( "deny_invite_check",      m_pkDenyInviteCheckBox );
    LINK_CONTROL( "video_static",           m_pkVideoStr       );
    LINK_CONTROL( "video_combobox",         m_pkVideoComboBox  );
	LINK_CONTROL( "sampling_static",        m_pkMultiSamplingStr       );
	LINK_CONTROL( "sampling_combobox",      m_pkMultiSamplingComboBox  );
#if defined( ITEMBOX_ANI_TOGGLE )
    LINK_CONTROL( "itembox_animaion_str",   m_pkItemBoxAlphaAniStr );
    LINK_CONTROL( "itembox_animaion_check", m_pkItemBoxAlphaAniCheckBox );
#endif
#if defined( USE_JOYSTICK )
	LINK_CONTROL( "joystic_btn",            m_pkJoystickBtn );
	LINK_CONTROL( "keyboard_btn",           m_pkKeyBoardBtn );
	LINK_CONTROL( "controll_title",        m_pkControllStr );
#endif
	LINK_CONTROL( "petview_str", m_pkPetViewStr );
	LINK_CONTROL( "petview_check", m_pkPetViewCheckBox );
    LINK_CONTROL( "baseequip_str", m_pkBasicEquipStr );
    LINK_CONTROL( "baseequip_check", m_pkBasicEquipCheckBox );
    LINK_CONTROL( "roomanimation_str", m_pkRoomAnimationStr );
    LINK_CONTROL( "roomanimation_check", m_pkRoomAnimationCheckBox );

	m_bRFirst=false;
}

KGCOption::~KGCOption( void )
{
    // empty
}


void KGCOption::OnCreate( void )
{
#if defined( USE_JOYSTICK )
	bool bJoystick = IsUseJoystic;
    m_pkJoystickBtn->ToggleRender( true );
	m_pkJoystickBtn->SetSelfInputCheck( true );
	m_pkJoystickBtn->Lock( bJoystick );

    m_pkKeyBoardBtn->ToggleRender( true );
	m_pkKeyBoardBtn->SetSelfInputCheck( true );
	m_pkKeyBoardBtn->Lock( true );

    m_pkControllStr->ToggleRender( true );

    KD3DWnd* pOptionBack = GetChildByName("option_back");
    if(pOptionBack)
    {
        KD3DWnd* pControllBack = pOptionBack->GetChildByName("controll_back");
        if(pControllBack)
            pControllBack->ToggleRender( true );

        KD3DWnd* pSeperator4 = pOptionBack->GetChildByName("seperator4");
        if(pSeperator4)
            pSeperator4->ToggleRender( true );
    }
#endif
	m_pkClose->InitState( true, true, this );
	m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkClose->SetHotKey( DIK_RETURN );

	// 자동 그래픽 퀄리티 켜기 / 끄기
	m_pkAutoGraphicCheckBox->InitState( true, true, this );
	m_pkAutoGraphicCheck->InitState( g_MyD3D->m_KGCOption.IsAutoGraphicQuality(), true, this );
	m_pkAutoGraphicStr->InitState( true, true, this );
	m_pkAutoGraphicStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION_AUTO_GRAPHIC_QUALITY ) );

	// 귓속말 거부 켜기 / 끄기
	m_pkWhisperCheckBox->InitState( true, true, this );
	m_pkWhisperCheck->InitState( g_pkChatManager->GetWhisperFilter(), true, this );
	m_pkWhisperStr->InitState( true, true, this );
	m_pkWhisperStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION_WHISPER_FILTER ) );

	m_pkMotionBlurCheckBox->SetSelfInputCheck( true );
    m_pkMotionBlurCheckBox->AddActionListener( this );
    m_pkMotionBlurCheckBox->ToggleRender( true );
    m_pkMotionBlurCheck->SetSelfInputCheck( true );
    m_pkMotionBlurCheck->AddActionListener( this );
    m_pkMotionBlurCheck->ToggleRender( g_MyD3D->m_KGCOption.IsMotionBlur() );

    // 2007/2/12. iridology. 모션블러가 불가능하면?
    if( !g_MyD3D->m_KGCBlurMgr.IsCapable() )
    {
        m_pkMotionBlurCheckBox->SetSelfInputCheck( false );
        m_pkMotionBlurCheckBox->AddActionListener( NULL );
        m_pkMotionBlurCheck->SetSelfInputCheck( false );
        m_pkMotionBlurCheck->AddActionListener( NULL );
        m_pkMotionBlurCheck->ToggleRender( false );
        g_MyD3D->m_KGCOption.SetMotionBlur( false );
    }

    m_pkMotionBlurStr->InitState( true, true, this );
    m_pkMotionBlurStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION_MOTION_BLUR ) );
    m_pkMotionBlurStr->ToggleRender( true );

	// 타격 이펙트 켜기 / 끄기
	m_pkHitEffectCheckBox->InitState( true, true, this );
	m_pkHitEffectCheck->InitState( g_MyD3D->m_KGCOption.IsHiteffect(), true, this );
	m_pkHitEffectStr->InitState( true, true, this );
	m_pkHitEffectStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION_HITEFFECT ) );

    //m_pkTitleStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION ) );
    //m_pkGraphicStr->SetText( g_pkStrLoader->GetString( STR_ID_GRAPHIC ) );
    //m_pkGraphicStr->SetAlign( DT_CENTER );

    //m_pkAudioStr->SetText( g_pkStrLoader->GetString( STR_ID_AUDIO ) );
    //m_pkAudioStr->SetAlign( DT_CENTER );

	if( m_pkVideoComboBox && m_pkMultiSamplingComboBox)
    {
        m_pkVideoComboBox->InitState( true, true, this );
        m_pkVideoComboBox->SetAlign( DT_CENTER );
        m_pkVideoComboBox->DeleteAllItem();

		m_pkMultiSamplingComboBox->InitState(true, true, this);
		m_pkMultiSamplingComboBox->SetAlign( DT_CENTER );
		m_pkMultiSamplingComboBox->DeleteAllItem();

        DWORD dwNumModes = g_MyD3D->m_KGCOption.GetNumOfResolution();
        for (unsigned int i=0;i<dwNumModes;i++)
        {
			std::wstringstream strResolution;
			D3DModeInfo sResolution;
            if( g_MyD3D->m_KGCOption.GetResolution( i, sResolution ) == false )
                continue;
#ifdef ENABLE_RESOLUTION
            if(g_MyD3D->m_KGCOption.IsEnableResolution(sResolution.Width, sResolution.Height) == false)
                continue;
#endif
			strResolution <<static_cast<int>(sResolution.Width)<<L" x "<<static_cast<int>(sResolution.Height);      
           
			m_pkVideoComboBox->AddString( strResolution.str(), DT_CENTER );
            
		}

		for (unsigned int j = 0 ;j < 5; j++) //4배 셈플링 까지 검사
		{
			std::wstring strSampleInfo = g_MyD3D->m_KGCOption.GetStringSampleInfo((D3DMULTISAMPLE_TYPE)j);
			if (strSampleInfo==L"") continue;

			m_pkMultiSamplingComboBox->AddString(strSampleInfo);
		}

		m_pkVideoComboBox->SetCurSel( g_MyD3D->m_KGCOption.GetCurrentMode());
		m_pkMultiSamplingComboBox->SetCurSel(g_MyD3D->m_KGCOption.GetSampleInfoNum());
		m_bRFirst=true;
   }

    if( m_pkVideoStr )
    {
        m_pkVideoStr->SetText( g_pkStrLoader->GetString( STR_ID_RESOLUTION ) );
        m_pkVideoStr->SetAlign( DT_CENTER );
    }

	if( m_pkMultiSamplingStr )
	{
		m_pkMultiSamplingStr->SetText( g_pkStrLoader->GetString( STR_ID_ANTIALISING ) );
		m_pkMultiSamplingStr->SetAlign( DT_CENTER );
	}

	// #if defined( USE_JOYSTICK )
	// 	m_pkControllStr->SetText( g_pkStrLoader->GetString( STR_ID_CONTROL ) );
	// 	m_pkControllStr->SetAlign( DT_CENTER );
	// #endif

    //m_pkSettingStr->SetText( g_pkStrLoader->GetString( STR_ID_GAME_OPTION ) );
    //m_pkSettingStr->SetAlign( DT_CENTER );

    // 키설정
    //m_pkKeySettingStr->SetText( g_pkStrLoader->GetString( STR_ID_KEY_SETTING ) );
    //m_pkKeySettingStr->SetAlign( DT_CENTER );

    // 2007/5/18. daeuk. for 조선컴
    m_pkSkillEffectStr->InitState( true, true, this );
    m_pkSkillEffectStr->SetText( g_pkStrLoader->GetString( STR_ID_OPTION_SKILL_EFFECT ) );
    m_pkSkillEffectStr->ToggleRender( true );

    m_pkSkillEffectCheckBox->SetSelfInputCheck( true );
    m_pkSkillEffectCheckBox->AddActionListener( this );
    m_pkSkillEffectCheckBox->ToggleRender( true );
    m_pkSkillEffectCheck->SetSelfInputCheck( true );
    m_pkSkillEffectCheck->AddActionListener( this );
    m_pkSkillEffectCheck->ToggleRender( g_MyD3D->m_KGCOption.IsSkillEffect() );

#ifdef _ENABLE_FIXED_PIPELINE
	m_pkUsingVSCheckBox->InitState( true, true, this );
	m_pkUsingFPCheckBox->InitState( true, true, this );
	m_pkVertexShaderStr->InitState( true, true, this );
	m_pkFixedPipeStr->InitState( true, true, this );
    m_pkUsingVSCheck->InitState( g_RenderManager->GetUsingVS(), true, this );
    m_pkUsingFPCheck->InitState( !g_RenderManager->GetUsingVS(), true, this );
#else
	m_pkUsingVSCheckBox->InitState( false );
	m_pkUsingFPCheckBox->InitState( false );
	m_pkVertexShaderStr->InitState( false );
	m_pkFixedPipeStr->InitState( false );
	m_pkUsingVSCheck->InitState( false );
	m_pkUsingFPCheck->InitState( false );
#endif

    m_pkVertexShaderStr->SetText( g_pkStrLoader->GetString( STR_ID_USING_VERTEX_SHADER ) );
    //m_pkVertexShaderStr->SetAlign( DT_CENTER );
    m_pkFixedPipeStr->SetText( g_pkStrLoader->GetString( STR_ID_USING_FIXED_PIPE ) );
    //m_pkFixedPipeStr->SetAlign( DT_CENTER );

    m_pkDenyInviteStr->InitState( true, true, this );
    m_pkDenyInviteStr->SetText( g_pkStrLoader->GetString( STR_ID_DENY_INVITE ) );
    m_pkDenyInviteCheckBox->InitState( true, true, this );


#if defined( ITEMBOX_ANI_TOGGLE )
    m_pkItemBoxAlphaAniStr->InitState( true, true, this );
    m_pkItemBoxAlphaAniStr->SetText( g_pkStrLoader->GetString( STR_ID_ITEMBOX_ANI ) );
    m_pkItemBoxAlphaAniCheckBox->InitState( true, true, this );
#endif

 //   if( g_kGlobalValue.IsOpenedChar( GC_CHAR_DIO ) )
	//{
    m_pkSkillKeySettingWnd->AddActionListener( this );
	//}
	//else
	//{
 //       if( m_pkSkillKeySettingWnd )
	//	    m_pkSkillKeySettingWnd->ToggleRender(false);
	//}

	m_pkPetViewStr->InitState( true, true, this );
	m_pkPetViewStr->SetText( g_pkStrLoader->GetString( STR_ID_PETVIEW_HIDING ) );

	m_pkPetViewCheckBox->SetSelfInputCheck( true );
	m_pkPetViewCheckBox->AddActionListener( this );
	m_pkPetViewCheckBox->ToggleRender( true );
	m_pkPetViewCheckBox->SetCheck( g_MyD3D->m_KGCOption.GetPetViewHide() );

    m_pkBasicEquipStr->InitState( true, true, this );
    m_pkBasicEquipStr->SetText( g_pkStrLoader->GetString( STR_ID_BASICEQUIP_ENABLE ) );

    m_pkBasicEquipCheckBox->SetSelfInputCheck( true );
    m_pkBasicEquipCheckBox->AddActionListener( this );
    m_pkBasicEquipCheckBox->ToggleRender( true );
    m_pkBasicEquipCheckBox->SetCheck( g_MyD3D->m_KGCOption.GetBasicEquipEnable() );

    m_pkRoomAnimationStr->InitState( true, true, this );
    m_pkRoomAnimationStr->SetText(g_pkStrLoader->GetString( STR_ID_ROOMANIMATION_DISABLE ) );

    m_pkRoomAnimationCheckBox->SetSelfInputCheck( true );
    m_pkRoomAnimationCheckBox->AddActionListener( this );
    m_pkRoomAnimationCheckBox->ToggleRender( true );
    m_pkRoomAnimationCheckBox->SetCheck( g_MyD3D->m_KGCOption.GetRoomAnimationEnable() );

    static bool bFirst = true;
    if ( bFirst )
    {
        bFirst = false;
        m_pkSkillKeySettingWnd->ToggleRender( true );
        SetWindowPosX( GetFixedWindowPos().x - 60.f );
    }
}

void KGCOption::ActionPerformed( const KActionEvent& event )
{
	if((m_pkVideoComboBox != NULL && m_pkVideoComboBox == event.m_pWnd) ||
		(m_pkMultiSamplingComboBox != NULL && m_pkMultiSamplingComboBox == event.m_pWnd))
    {
        if( event.m_dwCode == D3DWE_COMBO_SELCHANGE )
        {
			if (m_bRFirst==false) return;
			D3DMULTISAMPLE_TYPE eMultiSample = g_MyD3D->m_KGCOption.GetSampleInfoFromNum(m_pkMultiSamplingComboBox->GetCurSel());

			BOOL bResoultion = g_MyD3D->m_KGCOption.SetResolution( m_pkVideoComboBox->GetCurSel());            
            
            if( bResoultion == false )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_USE_RESOLUTION ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                m_pkVideoComboBox->SetCurSel( g_MyD3D->m_KGCOption.GetCurrentMode());
            }
            
			BOOL bSampling = g_MyD3D->m_KGCOption.SetSampleing(eMultiSample);
			if (bSampling == TRUE || bResoultion == TRUE)				
			{
				g_MyD3D->SetResolution(TRUE); //옵션 변경하면 그 즉시 해상도를 변경한다.
				g_KDSound.Play( "31" );
			}		
        }
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkWhisperCheckBox || event.m_pWnd == m_pkWhisperCheck || event.m_pWnd == m_pkWhisperStr )
        {
            g_pkChatManager->SetWhisperFilter( !g_pkChatManager->GetWhisperFilter() );

            if( !g_pkChatManager->GetWhisperFilter() )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_OFF), KGCChatManager::CHAT_TYPE_ALERT );
            else
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_ON), KGCChatManager::CHAT_TYPE_ALERT );

            m_pkWhisperCheck->ToggleRender( g_pkChatManager->GetWhisperFilter() );

            g_KDSound.Play( "31" );
        }
        else if( event.m_pWnd == m_pkHitEffectCheckBox || event.m_pWnd == m_pkHitEffectCheck || event.m_pWnd == m_pkHitEffectStr )
        {
            g_MyD3D->m_KGCOption.SetHitEffect( !g_MyD3D->m_KGCOption.IsHiteffect() );

            if( !g_MyD3D->m_KGCOption.IsHiteffect() )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_HITEFFECT_OFF), KGCChatManager::CHAT_TYPE_ALERT );
            else
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_HITEFFECT_ON), KGCChatManager::CHAT_TYPE_ALERT );

            m_pkHitEffectCheck->ToggleRender( g_MyD3D->m_KGCOption.IsHiteffect() );

            g_KDSound.Play( "31" );
        }
        else if( event.m_pWnd == m_pkMotionBlurCheck || event.m_pWnd == m_pkMotionBlurCheckBox || event.m_pWnd == m_pkMotionBlurStr )
        {
            // 2007/2/12. iridology. 가능??
            if( g_MyD3D->m_KGCBlurMgr.IsCapable() && g_MyD3D->m_KGCOption.GetQualityCount() > 5 )
            {
                g_MyD3D->m_KGCOption.SetMotionBlur( !g_MyD3D->m_KGCOption.IsMotionBlur() );

                if( !g_MyD3D->m_KGCOption.IsMotionBlur() )
                    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_MOTION_BLUR_OFF), KGCChatManager::CHAT_TYPE_ALERT );
                else
                    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_MOTION_BLUR_ON), KGCChatManager::CHAT_TYPE_ALERT );

                m_pkMotionBlurCheck->ToggleRender( g_MyD3D->m_KGCOption.IsMotionBlur() );

                g_KDSound.Play( "31" );
            }
        }
		else if( event.m_pWnd == m_pkAutoGraphicCheckBox || event.m_pWnd == m_pkAutoGraphicCheck || event.m_pWnd == m_pkAutoGraphicStr )
		{
			g_MyD3D->m_KGCOption.SetAutoGraphicQuality( !g_MyD3D->m_KGCOption.IsAutoGraphicQuality() );

			if( !g_MyD3D->m_KGCOption.IsAutoGraphicQuality() )
				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_NOTIFY_AUTO_GRAPHIC_QUALITY_OFF ), KGCChatManager::CHAT_TYPE_ALERT );
			else
				g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_NOTIFY_AUTO_GRAPHIC_QUALITY_ON ), KGCChatManager::CHAT_TYPE_ALERT );

			m_pkAutoGraphicCheck->ToggleRender( g_MyD3D->m_KGCOption.IsAutoGraphicQuality() );
			g_KDSound.Play( "31" );
		}
        else if( event.m_pWnd == m_pkSkillEffectCheck || event.m_pWnd == m_pkSkillEffectCheckBox || event.m_pWnd == m_pkSkillEffectStr )
        {
            g_MyD3D->m_KGCOption.SetSkillEffect( !g_MyD3D->m_KGCOption.IsSkillEffect() );

            if( !g_MyD3D->m_KGCOption.IsSkillEffect() )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_SKILL_EFFECT_OFF), KGCChatManager::CHAT_TYPE_ALERT );
            else
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_SKILL_EFFECT_ON), KGCChatManager::CHAT_TYPE_ALERT );

            m_pkSkillEffectCheck->ToggleRender( g_MyD3D->m_KGCOption.IsSkillEffect() );

            g_KDSound.Play( "31" );
        }
        else if( event.m_pWnd == m_pkClose || event.m_pWnd == m_pkSkillKeySettingWnd->m_pkClose )
        {
			m_bRFirst=false;
			g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );

            if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
                g_pkWorldmapChatBox->SetFocus();
            if( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH )
                g_pkLobbyChatBox->SetFocus();
            if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
                g_pkRoomChatBox->SetFocus();
            if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH )
                g_pkUIScene->m_pkCashShopScene->m_pkSearchBar->SetFocus( true );
#ifndef DISABLE_MAP_SHOP // Kawan>
            if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
                g_pkUIScene->m_pkGPShopScene->m_pkSearchBar->SetFocus( true );
#endif
        } 
        else if( event.m_pWnd  == m_pkUsingVSCheck || event.m_pWnd  == m_pkUsingVSCheckBox || event.m_pWnd == m_pkVertexShaderStr )
        {
            g_KDSound.Play( "31" );
            m_pkUsingVSCheck->ToggleRender( true );
            m_pkUsingFPCheck->ToggleRender( false );
            g_RenderManager->SetUsingVS( true );
        }
        else if( event.m_pWnd  == m_pkUsingFPCheck || event.m_pWnd  == m_pkUsingFPCheckBox || event.m_pWnd == m_pkFixedPipeStr )
        {
            g_KDSound.Play( "31" );
            m_pkUsingVSCheck->ToggleRender( false );
            m_pkUsingFPCheck->ToggleRender( true );
			g_RenderManager->SetUsingVS( false );
        }
        else if( event.m_pWnd == m_pkDenyInviteCheckBox || event.m_pWnd == m_pkDenyInviteStr )
        {
            g_KDSound.Play( "31" );
            m_pkDenyInviteCheckBox->ToggleCheck();
        }
#if defined( ITEMBOX_ANI_TOGGLE )
        else if( event.m_pWnd == m_pkItemBoxAlphaAniCheckBox || event.m_pWnd == m_pkItemBoxAlphaAniStr )
        {
            g_KDSound.Play( "31" );
            m_pkItemBoxAlphaAniCheckBox->ToggleCheck();
        }
#endif
		else if(event.m_pWnd == m_pkPetViewCheckBox || event.m_pWnd == m_pkPetViewStr)
		{
			g_KDSound.Play("31");
			m_pkPetViewCheckBox->ToggleCheck();
		}
        else if (event.m_pWnd == m_pkBasicEquipCheckBox || event.m_pWnd == m_pkBasicEquipStr)
        {
            g_KDSound.Play("31");
            m_pkBasicEquipCheckBox->ToggleCheck();
        }
        else if (event.m_pWnd == m_pkRoomAnimationCheckBox || event.m_pWnd == m_pkRoomAnimationStr)
        {
            g_KDSound.Play("31");
            m_pkRoomAnimationCheckBox->ToggleCheck();
        }
    }
}

void KGCOption::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );

    // [2/26/2008] breadceo. 닫을때 적용..
    SiKGCInviteManager()->SetDenyInviteMsg( m_pkDenyInviteCheckBox->GetCheck() );
    g_MyD3D->m_KGCOption.SetDenyInviteMsg( m_pkDenyInviteCheckBox->GetCheck() );
#if defined( ITEMBOX_ANI_TOGGLE )
    g_MyD3D->m_KGCOption.SetAlphaAnimation( m_pkItemBoxAlphaAniCheckBox->GetCheck() );
#endif
	g_MyD3D->m_KGCOption.SetPetViewHide( m_pkPetViewCheckBox->GetCheck() );
    g_MyD3D->m_KGCOption.SetBasicEquipEnable(m_pkBasicEquipCheckBox->GetCheck());
    g_MyD3D->m_KGCOption.SetRoomAnimationEnable(m_pkRoomAnimationCheckBox->GetCheck());
}

void KGCOption::OnCreateComplete()
{
    m_pkDenyInviteCheckBox->SetCheck( g_MyD3D->m_KGCOption.IsDenyInviteMsg() );
#if defined( ITEMBOX_ANI_TOGGLE )
    m_pkItemBoxAlphaAniCheckBox->SetCheck( g_MyD3D->m_KGCOption.GetAlphaAnimation() );
#endif
	m_pkPetViewCheckBox->SetCheck( g_MyD3D->m_KGCOption.GetPetViewHide() );
    g_MyD3D->m_KGCOption.SetBasicEquipEnable(m_pkBasicEquipCheckBox->GetCheck());
    g_MyD3D->m_KGCOption.SetRoomAnimationEnable(m_pkRoomAnimationCheckBox->GetCheck());
}
