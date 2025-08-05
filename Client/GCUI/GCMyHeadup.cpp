#include "stdafx.h"
#include "GCMyHeadup.h"
//
#include "../MyD3D.h"

#include "../Controls.h"
#include "../Headup Display.h"
#include "../Item.h"

//

#include "Buff/KGCBuffManager.h"
#include "KGCIndigoSelect.h"
//#include "KGCTagMatchSelect.h"
#include "KGCCharDependentGauge.h"
#include "KGCRoomManager.h"
#include "gcui/GCGameSkillBox.h"
#include "KGCMultipleLanguages.h"
#include "KGCSavingGauge.h"

IMPLEMENT_CLASSNAME( KGCMyHeadup );
IMPLEMENT_WND_FACTORY( KGCMyHeadup );
IMPLEMENT_WND_FACTORY_NAME( KGCMyHeadup, "gc_my_head" );

KGCMyHeadup::KGCMyHeadup( void ) :  m_vHUDPos( 0.0f, 0.0f ),
                                    m_vHPGaugeTexCoords( 0.0f, 0.0f, 0.0f, 0.0f ),
                                    m_vMPGaugeTexCoords( 0.0f, 0.0f, 0.0f, 0.0f ),
                                    m_vMPGatherTexCoords( 0.0f, 0.0f, 0.0f, 0.0f ),
                                    m_vPetMPGaugeTexCoords( 0.0f, 0.0f, 0.0f, 0.0f )
{
    m_iPlayerIdx        = 0;
    m_iMPGatherLevel    = 0;

    m_pkBlackMP         = NULL;
    m_pkWhiteMPGauge    = NULL;
    m_pkWhiteMP4Gauge   = NULL;
    m_pkMPFull          = NULL;
    m_pkHPGauge         = NULL;
    m_pkMPGauge         = NULL;
	m_pkAPGauge         = NULL;
    m_pkPetMPBack       = NULL;
    m_pkPetMPGauge      = NULL;
    m_pkPetMPGather     = NULL;
    m_pkBlackGather     = NULL;
    m_pkPetLogo         = NULL;
    m_pkPetMPGageDisable= NULL;
    m_pkMPGaugeEx       = NULL;
    m_pkMPGatherEx      = NULL;
    m_pkHUDMainPart4    = NULL;
    m_pkAPGaugeSP4      = NULL;
    m_pkAPGatherSP4     = NULL;
    m_pkAPBack          = NULL;
    m_pkLockMP4Mark     = NULL;
    m_pkHUDChatBar      = NULL;


    m_pkRemainSpecial	= NULL;
    //m_pkBgReMain = NULL;
    m_pkDun64CharLife = NULL;
    m_pkDun64GameItem = NULL;
    m_pkDun64X = NULL;

    m_pkSavingGauge = NULL;

    for( int i = 0 ; i < NUM_TAKE_ITEM ; ++i )
    {
        m_dwNextTagTime[i] = 0;
    }

    for( int i = 0 ; i < (int)m_apkItemCursor.size() ; ++i )
    {
        m_apkItemCursor[i] = NULL;
    }
    for( int i = 0 ; i < (int)m_apkCharFace.size() ; ++i )
    {
        m_apkCharFace[i] = NULL;
    }

    m_pkDeathKill = NULL;
    m_pkDeathKillNumber = NULL;

    LINK_CONTROL( "headup_display_main_part",           m_pkHUDMainPart );
    LINK_CONTROL( "headup_display_main_part_4",         m_pkHUDMainPart4 );
    
    LINK_CONTROL( "headup_display_sub_part",            m_pkHUDSubPart );
    LINK_CONTROL( "headup_display_chat_bar",            m_pkHUDChatBar );
    
    LINK_CONTROL( "headup_display_hp_gauge",            m_pkHPGauge );
    LINK_CONTROL( "headup_display_mp_gauge",            m_pkMPGauge );
    LINK_CONTROL( "headup_display_mp_gather_gauge",     m_pkMPGather );
    LINK_CONTROL( "headup_display_mp_gauge_ex",         m_pkMPGaugeEx );
    LINK_CONTROL( "headup_display_mp_gather_gauge_ex",  m_pkMPGatherEx );
	LINK_CONTROL( "headup_display_ap_gauge",            m_pkAPGauge );
	LINK_CONTROL( "headup_display_ap_gather_gauge",     m_pkAPGather );
    LINK_CONTROL( "headup_display_ap_gauge_sp4",        m_pkAPGaugeSP4 );
    LINK_CONTROL( "headup_display_ap_gather_gauge_sp4", m_pkAPGatherSP4 );
    LINK_CONTROL( "headup_display_ap_back",             m_pkAPBack );
    LINK_CONTROL( "headup_display_mp_1",                m_pkMP1st );
    LINK_CONTROL( "headup_display_mp_2",                m_pkMP2nd );
    LINK_CONTROL( "headup_display_mp_3",                m_pkMP3rd );
    LINK_CONTROL( "headup_black_mp",                    m_pkBlackMP );
    LINK_CONTROL( "headup_white_mp_gauge",              m_pkWhiteMPGauge );
    LINK_CONTROL( "headup_white_mp4_gauge",             m_pkWhiteMP4Gauge );
    LINK_CONTROL( "headup_mp_full",                     m_pkMPFull );
    LINK_CONTROL( "headup_item_cursor0",                m_apkItemCursor[0] );
    LINK_CONTROL( "headup_item_cursor1",                m_apkItemCursor[1] );
    LINK_CONTROL( "headup_item_cursor2",                m_apkItemCursor[2] );
    LINK_CONTROL( "headup_display_mp_ex_1",             m_pkMP1stEx );
    LINK_CONTROL( "headup_display_mp_ex_2",             m_pkMP2ndEx );
    LINK_CONTROL( "headup_display_mp_ex_3",             m_pkMP3rdEx );
    LINK_CONTROL( "headup_display_mp_ex_4",             m_pkMP4thEx );
    LINK_CONTROL( "headup_display_black_gather_gauge",  m_pkBlackGather );
    LINK_CONTROL( "lock_mp4_mark",                      m_pkLockMP4Mark  );

    for(int i=0;i<GC_CHAR_NUM;++i)
    {
        char strTemp[MAX_PATH];
        sprintf( strTemp, "char_face%d", i+1);
        LINK_CONTROL( strTemp,           m_apkCharFace[i] );

        sprintf( strTemp, "headup_display_char%d", i);
        LINK_CONTROL( strTemp,           m_pkChar[i] );
    }

    LINK_CONTROL( "headup_display_hp_stop",         m_pkHPStop );

    LINK_CONTROL( "headup_display_pet_mp_back",            m_pkPetMPBack );
    LINK_CONTROL( "headup_display_pet_mp_gauge",        m_pkPetMPGauge );
    LINK_CONTROL( "headup_display_pet_mp_gather_gauge",    m_pkPetMPGather );
    LINK_CONTROL( "headup_display_pet_logo",            m_pkPetLogo );
    LINK_CONTROL( "headup_disable_pet_mp_gage",         m_pkPetMPGageDisable );

    LINK_CONTROL( "skill_type0",            m_pkSkillType[0] );
    LINK_CONTROL( "skill_type1",            m_pkSkillType[1] );

    LINK_CONTROL( "skill_box",                m_pGameSkillBox );
	
    LINK_CONTROL( "text_remain_special",    m_pkRemainSpecial );
    //LINK_CONTROL( "bg_remain_special",      m_pkBgReMain );

    LINK_CONTROL( "text_next_tag_time_1",      m_pkNextTagTime[0] );
    LINK_CONTROL( "text_next_tag_time_2",      m_pkNextTagTime[1] );
    LINK_CONTROL( "text_next_tag_time_3",      m_pkNextTagTime[2] );

    LINK_CONTROL( "SpGauge",                 m_pkSpGauge );
    LINK_CONTROL( "saving_gauge",            m_pkSavingGauge );

    LINK_CONTROL( "death_kill",            m_pkDeathKill );
    LINK_CONTROL( "death_kill_number",     m_pkDeathKillNumber );

    m_strRemainSpecial = L"";
}

KGCMyHeadup::~KGCMyHeadup( void )
{
    if( m_pkDun64CharLife )
        SAFE_RELEASE(m_pkDun64CharLife);
    if( m_pkDun64GameItem )
        SAFE_RELEASE(m_pkDun64GameItem);
    if( m_pkDun64X )
        SAFE_RELEASE(m_pkDun64X);
}

void KGCMyHeadup::OnCreate( void )
{    
    //m_iPlayerIdx = g_MyD3D->Get_MyPlayer();
    m_vHUDPos = GetFixedWindowPos();

    m_vHPGaugeTexCoords     = m_pkHPGauge->GetWndTexCoords();

    // 마나 4칸
    m_vMPGaugeTexCoordsEx   = m_pkMPGaugeEx->GetWndTexCoords();
    m_vMPGatherTexCoordsEx  = m_pkMPGatherEx->GetWndTexCoords();
    // 마나 3칸
    m_vMPGaugeTexCoords     = m_pkMPGauge->GetWndTexCoords();
    m_vMPGatherTexCoords    = m_pkMPGather->GetWndTexCoords();
    
    m_vPetMPGaugeTexCoords    = m_pkPetMPGauge->GetWndTexCoords();	

    // AP 3칸
	m_vAPGaugeTexCoords     = m_pkAPGauge->GetWndTexCoords();
	m_vAPGatherTexCoords    = m_pkAPGather->GetWndTexCoords();
    // AP 4칸
    m_vAPGaugeTexCoordsEx   = m_pkAPGaugeSP4->GetWndTexCoords();
    m_vAPGatherTexCoordsEx  = m_pkAPGatherSP4->GetWndTexCoords();

    m_pkHUDMainPart->SetSelfInputCheck( false );
    m_pkHUDMainPart4->SetSelfInputCheck( false );
    m_pkHUDMainPart4->ToggleRender( false );
    m_pkHUDSubPart->SetSelfInputCheck( false );	
    m_pkHUDChatBar->ToggleRender( false );

    for(int i = 0; i < GC_CHAR_NUM; i++)
    {
        m_pkChar[i]->SetSelfInputCheck(false);
    }

    m_pkBlackMP->SetSelfInputCheck( false );
    m_pkWhiteMPGauge->SetSelfInputCheck( false );
    m_pkWhiteMP4Gauge->SetSelfInputCheck( false );
    m_pkMPFull->SetSelfInputCheck( false );

    m_pkBlackMP->ToggleRender( false );
    m_pkWhiteMPGauge->ToggleRender( false );
    m_pkWhiteMP4Gauge->ToggleRender( false );
    m_pkMPFull->ToggleRender( false );

    m_pkHPGauge->SetSelfInputCheck( false );
    m_pkMPGauge->SetSelfInputCheck( false );
    m_pkMPGather->SetSelfInputCheck( false );
    m_pkBlackGather->SetSelfInputCheck( false );
    ToggleBlackGauge( false );

    m_pkPetMPBack->SetSelfInputCheck( false );
    m_pkPetMPGauge->SetSelfInputCheck( false );
    m_pkPetMPGather->SetSelfInputCheck( false );
    m_pkPetLogo->SetSelfInputCheck( false );
    m_pkPetMPGageDisable->SetSelfInputCheck( false );
    m_pkRemainSpecial->SetSelfInputCheck( false );
    m_pkRemainSpecial->SetShadow( true );
    //m_pkBgReMain->SetSelfInputCheck( false );

    m_pkMP1st->SetSelfInputCheck( false );
    m_pkMP2nd->SetSelfInputCheck( false );
    m_pkMP3rd->SetSelfInputCheck( false );

    m_pkMP1st->ToggleRender( false );
    m_pkMP2nd->ToggleRender( false );
    m_pkMP3rd->ToggleRender( false );

    m_pkMP1stEx->InitState( false, false );
    m_pkMP2ndEx->InitState( false, false );
    m_pkMP3rdEx->InitState( false, false );
    m_pkMP4thEx->InitState( false, false );

    m_pkLockMP4Mark->ToggleRender( false ) ;

    m_pkHPStop->ToggleRender( false );
    m_pkRemainSpecial->ToggleRender( false );
    //m_pkBgReMain->ToggleRender( false );

    m_pkSavingGauge->InitState( true, false );


    for ( int i = 0; i < NUM_TAKE_ITEM; ++i )
    {
        m_apkItemCursor[i]->SetSelfInputCheck( false );
        m_apkItemCursor[i]->ToggleRender( false );
    }

    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
    {
        m_apkCharFace[i]->InitState( false, true, NULL );
        m_apkCharFace[i]->Lock( false );
    }

    g_MyD3D->MyItms->SetItemCursorPos(-1);

    for( int i = 0; i < NUM_TAKE_ITEM; ++i )
    {
        m_pkNextTagTime[i]->SetFontSize( SiKGCMultipleLanguages()->GetTitleFontSize() );
        m_pkNextTagTime[i]->SetFontColor( 0xFFFFFFFF );
        m_pkNextTagTime[i]->SetShadow( true );
    }

    if( GC_GM_QUEST64  == SiKGCRoomManager()->GetGameMode() )
    {
        if( m_pkDun64CharLife )
            SAFE_RELEASE(m_pkDun64CharLife);
        m_pkDun64CharLife = g_pGCDeviceManager2->CreateTexture("Dun63CharLife.dds");
        if( m_pkDun64GameItem )
            SAFE_RELEASE(m_pkDun64GameItem);
        m_pkDun64GameItem = g_pGCDeviceManager2->CreateTexture("Dun63GameItem.dds");
        if( m_pkDun64X )
            SAFE_RELEASE(m_pkDun64X);
        m_pkDun64X = g_pGCDeviceManager2->CreateTexture("ui_dungeon00.dds");
    }

    m_pkDeathKill->InitState( false );
    m_pkDeathKillNumber->InitState( false );
    m_pkDeathKillNumber->SetStringType("epic_num_enchant");
    m_pkDeathKillNumber->SetWriteToRight(false);
    m_pkDeathKillNumber->SetInterval( 12.0f );
    m_pkDeathKillNumber->SetWriteAlign( ALIGN_RIGHT_SIDE );

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
       m_pkDeathKill->ToggleRender( true ); 
       m_pkDeathKillNumber->ToggleRender( true ); 
    }
}

void KGCMyHeadup::OnInitialize()
{   
	SetAlignType(WNDUIAT_POSZEROBASE);

	m_fWindowScaleX  = 1.0f;
	m_fWindowScaleY  = 1.0f;
	// 게임내 UI크기는  1024x768베이스로 맞춘다. 
	if (g_pGCDeviceManager2->GetWindowScaleX() >= 1024.0f ) 
		m_fWindowScaleX = g_pGCDeviceManager2->GetWindowScaleX() / (1024.0f);

	// 게임내 UI크기는 1024x768베이스로 맞춘다. 
	if (g_pGCDeviceManager2->GetWindowScaleY() >= 768.0f ) 
		m_fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY() / (768.0f );


	SetWindowScale(D3DXVECTOR2(m_fWindowScaleX,m_fWindowScaleY));

    if( g_kGlobalValue.IsTutorialMode() ) {
        m_iPlayerIdx = 0;
    } else {
        m_iPlayerIdx = g_MyD3D->Get_MyPlayer();
    }    

    if( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.cCharIndex == -1 )
        return;

    if( m_iPlayerIdx < MAX_PLAYER_NUM )
    {    
        m_iMPGatherLevel = 0;
        SetHP( 1.0f );
        SetMP( 1.0f );
        SetChar( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.GetCurrentChar().iCharType );
    }

    //if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
    //{
    //    // UI에 세팅
    //    char* pSlot = g_pIndigoSelect->GetSlot();

    //    for( int i=0 ; i<3 ; ++i )
    //    {
    //        m_apkCharFace[pSlot[i]]->ToggleRender( true );

    //        D3DXVECTOR2 vPos = m_apkItemCursor[i]->GetFixedWindowLocalPos();
    //        vPos.x += 4;
    //        vPos.y += 4;

    //        m_apkCharFace[pSlot[i]]->SetFixedWindowLocalPos( vPos );
    //    }
    //}
    //2013-08 매치 개선 UI 들어가면서 tagmatch 작업 고려 안함 (뺀 컨텐츠)
    //  else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
  //  {
  //      m_vecCharSlot.clear();
  //      // UI에 세팅
  //      const KSafeArray<int,TAG_CHAR_NUM>& aiCharSlot = g_pTagMatchSelect->GetSlot();
  //      std::vector< int >::iterator vit;
  //      for( int i = 0; i < TAG_CHAR_NUM; ++i )
  //      {
  //          vit = std::find( m_vecCharSlot.begin(), m_vecCharSlot.end(), aiCharSlot[i] );
  //          if( vit == m_vecCharSlot.end() )
  //              m_vecCharSlot.push_back( aiCharSlot[i] );
  //      }

		//vit = m_vecCharSlot.begin();
  //      for( int i = 0; vit != m_vecCharSlot.end(); ++vit, ++i )
  //      {
  //          m_apkCharFace[(*vit)]->ToggleRender( true );

  //          D3DXVECTOR2 vPos = m_apkItemCursor[i]->GetFixedWindowLocalPos();
  //          vPos.x += 4;
  //          vPos.y += 4;

  //          m_apkCharFace[(*vit)]->SetFixedWindowLocalPos( vPos );
  //      }
  //  }

    m_pkPetMPBack->ToggleRender( false );
    m_pkPetMPGauge->ToggleRender( false );
    m_pkPetMPGather->ToggleRender( false );
    m_pkPetLogo->ToggleRender( false );
    m_pkPetMPGageDisable->ToggleRender( false );
    m_pkRemainSpecial->ToggleRender( false );
    //m_pkBgReMain->ToggleRender( false );

    m_apkItemCursor[0]->ToggleRender( false );
    m_apkItemCursor[1]->ToggleRender( false );
    m_apkItemCursor[2]->ToggleRender( false );

    if( SiKGCRoomManager()->IsMonsterGameMode() && 
        SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST64 &&
        g_MyD3D->GetMyPlayer()->IsLearnedSkill( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SAVING_SKILL_ID ) ) {
            m_pkSavingGauge->SetUseGauge( true );
    } else {
        m_pkSavingGauge->SetUseGauge( false );
    }
}

void KGCMyHeadup::OnDestroy( void )
{
    SetHP( 1.0f );
    SetMP( 1.0f );

    if( m_pkDun64CharLife )
        SAFE_RELEASE(m_pkDun64CharLife);
    if( m_pkDun64GameItem )
        SAFE_RELEASE(m_pkDun64GameItem);
    if( m_pkDun64X )
        SAFE_RELEASE(m_pkDun64X);
}

void KGCMyHeadup::FrameMoveInEnabledState( void )
{
    if( !IsRenderOn() )
        return;

    if ( m_iPlayerIdx < 0 || MAX_PLAYER_NUM <= m_iPlayerIdx ) 
        return;    

    if( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.cCharIndex == -1 )
        return;

    SetChar( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.GetCurrentChar().iCharType );

    // HP
    float fHP = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetHP();
    SetHP( fHP / g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fAbility[ABILITY_HP], 0 != g_pMagicEffect->IsMagicEffect(m_iPlayerIdx, EGC_EFFECT_BLOOD_BASH) );

    // MP
    //float fMP = g_MyD3D->MyHead->Level / g_kGlobalValue.GetMaxMP();
    float fMP = g_MyD3D->MyHead->Level / 4;
    SetMP( fMP );

    // MP 모으기
    float fMPGather = g_MyD3D->MyCtrl->Energy / 4;
    SetMPGather( fMPGather );
    if ( 0.0f <= g_MyD3D->MyCtrl->Energy && g_MyD3D->MyCtrl->Energy < 1.0f )
    {
        m_iMPGatherLevel = 0;
    }
    else if ( 1.0f <= g_MyD3D->MyCtrl->Energy && g_MyD3D->MyCtrl->Energy < 2.0f )
    {
        if ( m_iMPGatherLevel != 1 )
        {
            //g_KDSound.Play( "73" );
            //g_KDSound.Play( "MP" );
        }
        m_iMPGatherLevel = 1;
    }
    else if ( 2.0f <= g_MyD3D->MyCtrl->Energy && g_MyD3D->MyCtrl->Energy < 4 )
    {
        if ( m_iMPGatherLevel != 2 )
        {
            //g_KDSound.Play( "73" );
            //g_KDSound.Play( "MP" );
        }
        m_iMPGatherLevel = 2;
    }
    else if ( 4 <= g_MyD3D->MyCtrl->Energy )
    {
        if ( m_iMPGatherLevel != 3 )
        {
            //g_KDSound.Play( "73" );
            //g_KDSound.Play( "MP" );
        }
        m_iMPGatherLevel = 3;
    }

    int iCharType = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetCurrentChar().iCharType;
    int iGameMode = SiKGCRoomManager()->GetGameMode();
    if( iGameMode != GC_GM_QUEST64 )
    {
#if defined( NATION_KOREA )
        m_pkMP1stEx->ToggleRender( false );
        m_pkMP2ndEx->ToggleRender( false );
        m_pkMP3rdEx->ToggleRender( false );
        m_pkMP4thEx->ToggleRender( false );
#else
        m_pkMP1stEx->ToggleRender( g_MyD3D->MyHead->Level >= 1.0f && o10 < 10 );
        m_pkMP2ndEx->ToggleRender( g_MyD3D->MyHead->Level >= 2.0f && o10 < 10 );
        m_pkMP3rdEx->ToggleRender( g_MyD3D->MyHead->Level >= 3.0f && o10 < 10 );
        m_pkMP4thEx->ToggleRender( g_MyD3D->MyHead->Level >= 4.0f && o10 < 10 );
#endif
        m_pkMP1st->ToggleRender( false );
        m_pkMP2nd->ToggleRender( false );
        m_pkMP3rd->ToggleRender( false );

        // 흑마법/백마법 표시
        if ( iCharType == GC_CHAR_ARME || iCharType == GC_CHAR_RONAN )
        {
            m_pkBlackMP->ToggleRender( g_MyD3D->MyHead->IsBlackMagic );
            m_pkWhiteMP4Gauge->ToggleRender( !g_MyD3D->MyHead->IsBlackMagic );
            m_pkWhiteMPGauge->ToggleRender( false );
        }
        else
        {
            m_pkBlackMP->ToggleRender( false );
            m_pkWhiteMPGauge->ToggleRender( false );
            m_pkWhiteMP4Gauge->ToggleRender( false );
        }

        // MP 모으기 풀 표시
        m_pkMPFull->ToggleRender( g_MyD3D->MyCtrl->Energy >= g_kGlobalValue.GetMaxMP() && o10 < 10 );

        if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH )
        {
            // 아이템 커서
            for ( int i = 0; i < NUM_TAKE_ITEM; ++i )
            {
                m_apkItemCursor[i]->ToggleRender( i == g_MyD3D->MyItms->GetItemCursorPos() );
            }
        }
        else
        {
            // 아이템 커서
            for ( int i = 0; i < TAG_CHAR_NUM; ++i )
            {
                m_apkItemCursor[i]->ToggleRender( false );
                if( i >= (int)m_vecCharSlot.size() )
                {
                    continue;
                }
                m_apkItemCursor[i]->ToggleRender( iCharType == m_vecCharSlot[i] );
            }

            for( int it = 0; it < NUM_TAKE_ITEM; ++it )
            {
                if( m_dwNextTagTime[it] != 0 )
                {
                    std::wstringstream wstm;
                    wstm << GetNextTagTimeSec( m_dwNextTagTime[it] );
                    m_pkNextTagTime[it]->ToggleRender( true );
                    m_pkNextTagTime[it]->SetText( wstm.str() );
                    m_dwNextTagTime[it]--;
                }
                else
                {
                    m_pkNextTagTime[it]->ToggleRender( false );
                    m_pkNextTagTime[it]->SetText( L"" );
                }
            }
        }

        // 펫 : 스킬사용, MP
        // 펫을 가지고 있지 않다면 MP바가 안 보이도록 한다
        if( g_MyD3D->MyPlayer[ m_iPlayerIdx ]->GetCurrentChar().kPetInfo.m_dwUID == 0 )
        {
            m_pkPetMPBack->ToggleRender( false );
            m_pkPetMPGauge->ToggleRender( false );
            m_pkPetLogo->ToggleRender( false );
            m_pkPetMPGageDisable->ToggleRender( false );
            m_strRemainSpecial = L"";
            //m_pkBgReMain->ToggleRender( false );
            m_pkRemainSpecial->SetText( m_strRemainSpecial );
            m_pkPetMPGather->ToggleRender( false );
        }
        else
        {
            m_pkPetMPBack->ToggleRender( true );
            m_pkPetMPGauge->ToggleRender( true );
            m_pkPetLogo->ToggleRender( true );
            m_pkPetMPGageDisable->ToggleRender( false );
            m_pkRemainSpecial->ToggleRender( true );
            //m_pkBgReMain->ToggleRender( true );
            m_pkRemainSpecial->SetText( m_strRemainSpecial );

            // 스킬 사용 가능.
            if( IsEnableUsePetSkill( m_iPlayerIdx ) == true )
            {
                float fPetMP = g_MyD3D->m_akPet[m_iPlayerIdx]->m_fCurrentMP;
                SetPetMP( fPetMP );

                // MP 꽉 찼을때 깜빡깜빡
                if( fPetMP >= 1.0f )
                    m_pkPetMPGather->ToggleRender( o10 < 10 );
                else
                    m_pkPetMPGather->ToggleRender( false );
            }
            else // 스킬 사용 불가.
            {
                m_pkPetMPGageDisable->ToggleRender( true );
                m_pkPetMPGauge->ToggleRender( false );
                m_pkPetMPBack->ToggleRender( false );
                m_strRemainSpecial = L"";
                m_pkRemainSpecial->SetText( m_strRemainSpecial );
                m_pkRemainSpecial->ToggleRender( false );
                //m_pkBgReMain->ToggleRender( false );
                m_pkPetMPGather->ToggleRender( false );
            }
        }

        if( g_MyD3D->GetMyPlayer()->GetMyCharString() == CID_MARI2 ) {
            m_pkSpGauge->SetAmount( g_MyD3D->GetMyPlayer()->GetHittingPoint() ); 
            m_pkSpGauge->SetBlink( g_MyD3D->GetMyPlayer()->GetHittingState() );
        } else if( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_QUANTITY_POINT && 
            ( !g_MyD3D->GetMyPlayer()->IsLearnedSkill( SID_RIN2_SELECT_EVIL ) || g_MyD3D->GetMyPlayer()->GetCurrentChar().iPromotionLevel == 0 ) ) {
            float fQPAmount = 0.f;
            if( g_MyD3D->GetMyPlayer()->GetQPChargeFrame() > 0 ) {
                fQPAmount = static_cast<float>( g_MyD3D->GetMyPlayer()->GetQPChargeFrame() ) / static_cast< float >( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iQPChargeFrame );
            }
            m_pkSpGauge->SetAmount( fQPAmount ); 
        } else if( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() ) {
#ifdef DECANEE
            if ( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_DECANEE )
            {
                float fMarbleNum = g_MyD3D->GetMyPlayer()->GetCurMarbleNum() / 3.0;
                m_pkSpGauge->SetAmount(fMarbleNum);
                m_pkSpGauge->SetBlink(g_MyD3D->GetMyPlayer()->GetIsMarbleRun());
            }
            else
#endif
            {
                float fMarbleNum = g_MyD3D->GetMyPlayer()->GetCurMarbleNum() / g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fMaxMarbleAmount;
                m_pkSpGauge->SetAmount(fMarbleNum);
                m_pkSpGauge->SetBlink(g_MyD3D->GetMyPlayer()->CheckRunMarbleMode());
            }
        } else {
            m_pkSpGauge->SetAmount( g_MyD3D->GetMyPlayer()->m_fBurnningPoint / 3.0f ); 
            m_pkSpGauge->SetBlink( g_MyD3D->GetMyPlayer()->m_bIsBurnningMode );
        }

	    if( g_kGlobalValue.IsQuickSkillPlayer( g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType ) ||
            g_kGlobalValue.IsHybridPlayer( g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType ) ||
            g_kGlobalValue.IsCharUseSpecial4( g_MyD3D->GetMyPlayer()->m_kUserInfo.GetCurrentChar().iCharType ) )	
	    {
		    m_pkSkillType[0]->ToggleRender( false );
		    m_pkSkillType[1]->ToggleRender( false );
	    }
	    else
	    {
            SetPlayerSkillType( g_MyD3D->MyPlayer[m_iPlayerIdx]->m_iSkillType );
	    }
    }


    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        m_pkDeathKillNumber->SetNumber( g_pGameBoard->GetHowKill(m_iPlayerIdx) );
    }
}
bool KGCMyHeadup::IsEnableUsePetSkill( int iIndex_ )
{
    if ( g_MyD3D->m_akPet[iIndex_]->m_bEnableUseSkill &&
         !g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) &&
		 SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_INDIGO &&
		 !g_kGlobalValue.ServerInfo.CheckServerType( ST_CREATE_LIMIT ))
         return true;
    return false;
}

void KGCMyHeadup::PostChildDraw( void )
{
    if ( m_iPlayerIdx < 0 || MAX_PLAYER_NUM <= m_iPlayerIdx || false == IsRenderOn() ) 
		return;

    int iGameMode = SiKGCRoomManager()->GetGameMode();
    if( iGameMode == GC_GM_QUEST64 )
    {
        g_pGCDeviceManager2->DrawInScreen(m_pkDun64CharLife,
            (10.0f * m_fWindowScaleX), (50.0f * m_fWindowScaleY),
            (10.0f + m_pkDun64CharLife->GetWidth()) * m_fWindowScaleX,
            (50.0f + m_pkDun64CharLife->GetHeight()) * m_fWindowScaleY,
            0.0f,0.0f,1.0f,1.0f);

        g_pGCDeviceManager2->DrawInScreen(m_pkDun64GameItem,
             (7.0f * m_fWindowScaleX), (100.0f * m_fWindowScaleY),
             (7.0f + m_pkDun64GameItem->GetWidth()) * m_fWindowScaleX,
             (100.0f + m_pkDun64GameItem->GetHeight()) * m_fWindowScaleY,
             0.0f,0.0f,1.0f,1.0f);

        g_pGCDeviceManager2->DrawInScreen(m_pkDun64X,
            (75.0f * m_fWindowScaleX),
            (80.0f * m_fWindowScaleY),
            (98.0f * m_fWindowScaleX),
            (100.0f * m_fWindowScaleY),
            (135.0f/m_pkDun64X->GetWidth()),
            (46.0f/m_pkDun64X->GetHeight()),
            (158.0f/m_pkDun64X->GetWidth()),
            (66.0f/m_pkDun64X->GetHeight()));

        g_pGCDeviceManager2->DrawInScreen(m_pkDun64X,
            (75.0f * m_fWindowScaleX),
            (125.0f * m_fWindowScaleY),
            (98.0f * m_fWindowScaleX),
            (145.0f * m_fWindowScaleY),
            (135.0f/m_pkDun64X->GetWidth()),
            (46.0f/m_pkDun64X->GetHeight()),
            (158.0f/m_pkDun64X->GetWidth()),
            (66.0f/m_pkDun64X->GetHeight()));

        // 아레나 수
        D3DXVECTOR2 vCharLifeNumPos(95.0f, 55.0f);
        DWORD dwLife = (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_cLife);
        if( 0 < dwLife ) dwLife -= 1;
        g_pkUIMgr->RenderNumber( "9th_event_num", vCharLifeNumPos, dwLife );

        // 게임 아이템 수
        D3DXVECTOR2 vGameItemNumPos(95.0f, 105.0f);
        DWORD dwGameItem = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetNumGameItem();
        g_pkUIMgr->RenderNumber( "9th_event_num", vGameItemNumPos, dwGameItem );
    }
    else
    {
	    // 아레나 수
        int iLife = g_MyD3D->MyPlayer[m_iPlayerIdx]->m_cLife;
        if(g_kGlobalValue.m_bMUHANLife == true) {
        }
	    else if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL && iLife < 10 )
	    {
		    g_pkUIMgr->RenderNumber( "orange_num",  D3DXVECTOR2((m_vHUDPos.x + 370.0f) * m_fWindowScaleX,( m_vHUDPos.y + 40.0f ) * m_fWindowScaleY), iLife, true, false,-1.0f,false );
	    }

        // 내 피 게이지 수치
        D3DXVECTOR2 vPos = this->GetFixedWindowPos();
        D3DXVECTOR2 vecPos = this->GetRelocatedWindowPos();
        vecPos.x += (this->GetParent()->GetWidth() + 130) * m_fWindowScaleX;
        vecPos.y += (42.0f * m_fWindowScaleY);

        float fScreenX = vecPos.x;
        float fScreenY = vecPos.y;

#ifdef LEVEL_DESIGN_STAT_CALC
        int iHP = static_cast<int>(g_MyD3D->MyPlayer[m_iPlayerIdx]->GetHP());
#else
        int iHP = static_cast<int>(g_MyD3D->MyPlayer[m_iPlayerIdx]->GetHP() * 100);
#endif

        std::wstringstream stm;
        stm.str(L"");

#ifdef LEVEL_DESIGN_STAT_CALC
        stm << iHP << L"/" << static_cast<int>(g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fAbility[ABILITY_HP]);
#else
        stm << iHP << L"/" << static_cast<int>(g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fAbility[ABILITY_HP] * 100);
#endif

        D3DCOLOR Color = 0xFFFFFFFF;
        g_pkFontMgr14->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_RIGHT | DT_TOP );

        // 게임 아이템
        static D3DXVECTOR4 s_avGameItemPos[NUM_TAKE_ITEM] =
        {
            D3DXVECTOR4( 132.0f, 38.0f, 174.0f, 80.0f ),
            D3DXVECTOR4( 102.0f, 77.0f, 144.0f, 119.0f ),
            D3DXVECTOR4( 60.0f, 105.0f, 102.0f, 147.0f ),
        };

        for ( int i = 0; i < NUM_TAKE_ITEM; ++i )
        {
            int ItemNo = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetGameItem( i );
            if ( ItemNo < 0 )
                continue;

            g_pGCDeviceManager2->DrawInScreen(g_MyD3D->MyItms->m_pTexItem[ItemNo],
                s_avGameItemPos[i].x * m_fWindowScaleX,
                s_avGameItemPos[i].y * m_fWindowScaleY,
                s_avGameItemPos[i].z * m_fWindowScaleX,
                s_avGameItemPos[i].w * m_fWindowScaleY,	
                0.0f,0.0f,1.0f,1.0f);
        }

        // 채팅모드라면 채팅 에디트가 깜빡인다.
        if ( o10 < 10 && g_MyD3D->MyHead->IsCheatMode )
        {
            g_pGCDeviceManager2->PushState();
            {
                g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
                g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );

                g_pGCDeviceManager2->DrawInScreen(NULL,
                    204.f * m_fWindowScaleX, 13.0f * m_fWindowScaleY, (144.f + 440.0f) * m_fWindowScaleX, (18.0f + 16.0f) * m_fWindowScaleY,
                    0.0f, 0.0f, 1.0f, 1.0f,
                    false, 10.0f, D3DCOLOR_ARGB(255, 100, 100, 100));		
            }
            g_pGCDeviceManager2->PopState();
        }
    }
}

void KGCMyHeadup::SetHP( float fHP, bool bHPStop )
{
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;

    m_pkHPGauge->SetWndTexCoords( D3DXVECTOR4( m_vHPGaugeTexCoords.x, m_vHPGaugeTexCoords.y,
        m_vHPGaugeTexCoords.x + fHP * ( m_vHPGaugeTexCoords.z - m_vHPGaugeTexCoords.x ),
        m_vHPGaugeTexCoords.w ) );
    m_pkHPGauge->SetWidth( static_cast<int>( fHP * MAX_LEN_HP_GAUGE ) );    

    m_pkHPStop->ToggleRender( bHPStop );
}

void KGCMyHeadup::SetMP( float fMP )
{   
    if( g_MyD3D->MyHead->Level > 3 && g_kGlobalValue.GetMaxMP() == 3 ) {
        return;
    }

    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;
    
    float fTU2;
    fTU2 = m_vMPGaugeTexCoordsEx.x + fMP * ( m_vMPGaugeTexCoordsEx.z - m_vMPGaugeTexCoordsEx.x );
    //fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

    m_pkMPGaugeEx->SetWndTexCoords( D3DXVECTOR4( m_vMPGaugeTexCoordsEx.x,
        m_vMPGaugeTexCoordsEx.y, fTU2,
        m_vMPGaugeTexCoordsEx.w ) );
    m_pkMPGaugeEx->SetWidth( static_cast<int>( fMP * MAX_LEN_MP_GAUGE ) );

    fTU2 = m_vAPGaugeTexCoordsEx.x + fMP * ( m_vAPGaugeTexCoordsEx.z - m_vAPGaugeTexCoordsEx.x );
    //fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

    m_pkAPGaugeSP4->SetWndTexCoords( D3DXVECTOR4( m_vAPGaugeTexCoordsEx.x,
        m_vAPGaugeTexCoordsEx.y, fTU2,
        m_vAPGaugeTexCoordsEx.w ) );
    m_pkAPGaugeSP4->SetWidth( static_cast<int>( fMP * MAX_LEN_MP_GAUGE ) );
}

void KGCMyHeadup::SetMPGather( float fMPGather )
{
    if( g_MyD3D->MyHead->Level > 3 && g_kGlobalValue.GetMaxMP() == 3 ) {
        return;
    }

    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;

    float fTU2;
    fTU2 = m_vMPGatherTexCoordsEx.x + fMPGather * ( m_vMPGatherTexCoordsEx.z - m_vMPGatherTexCoordsEx.x );
    //fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;
    m_pkMPGatherEx->SetWndTexCoords( D3DXVECTOR4( m_vMPGatherTexCoordsEx.x,
        m_vMPGatherTexCoordsEx.y, fTU2,
        m_vMPGatherTexCoordsEx.w ) );
    m_pkMPGatherEx->SetWidth( static_cast<int>( fMPGather * MAX_LEN_MP_GAUGE ) );

    fTU2 = m_vAPGatherTexCoordsEx.x + fMPGather * ( m_vAPGatherTexCoordsEx.z - m_vAPGatherTexCoordsEx.x );
    //fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;
    m_pkAPGatherSP4->SetWndTexCoords( D3DXVECTOR4( m_vAPGatherTexCoordsEx.x,
        m_vAPGatherTexCoordsEx.y, fTU2,
        m_vAPGatherTexCoordsEx.w ) );
    m_pkAPGatherSP4->SetWidth( static_cast<int>( fMPGather * MAX_LEN_MP_GAUGE ) );
}

void KGCMyHeadup::SetPetMP( float fMP )
{
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;

    float fTU2 = m_vPetMPGaugeTexCoords.x + fMP * ( m_vPetMPGaugeTexCoords.z - m_vPetMPGaugeTexCoords.x );
    fTU2 = ( static_cast<int>( fTU2 * 512.0f + 0.5f ) + 0.5f ) / 512.0f;

    m_pkPetMPGauge->SetWndTexCoords( D3DXVECTOR4( m_vPetMPGaugeTexCoords.x,
        m_vPetMPGaugeTexCoords.y, fTU2,
        m_vPetMPGaugeTexCoords.w ) );

    m_pkPetMPGauge->SetWidth( static_cast<int>( fMP * MAX_LEN_PET_MP_GAUGE ) );
}

void KGCMyHeadup::SetChar( int iChar )
{
    int iGameMode = SiKGCRoomManager()->GetGameMode();
    switch( iGameMode )
    {
    case GC_GM_QUEST64:
        ToggleAllUI_Dungeon64( iChar );
        break;
    default:
        ToggleAllUI_Base( iChar );
    }
}

void KGCMyHeadup::ToggleAllUI_Base( int iChar )
{
    for(int i = 0; i < GC_CHAR_NUM; i++)
    {
        m_pkChar[i]->ToggleRender( i == iChar );
    }

    bool bTemp = g_MyD3D->GetMyPlayer()->IsUsableMarbleState();
    float fTemp = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fMaxMarbleAmount;

    if( g_MyD3D->GetMyPlayer()->GetMyCharString() == CID_MARI2 ||
        g_MyD3D->GetMyPlayer()->GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_QUANTITY_POINT ||
        ( g_MyD3D->GetMyPlayer()->IsUsableMarbleState() && g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fMaxMarbleAmount != 0.f ) ) {
            m_pkSpGauge->ToggleRender(true);
    } else {
        m_pkSpGauge->ToggleRender( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().BURNNING_ACTIVE != -1 );
    }

    //if( g_MyD3D->GetMyPlayer()->IsLearnedSkill( SID_RIN2_SELECT_EVIL ) && g_MyD3D->GetMyPlayer()->GetCurrentChar().iPromotionLevel > 0 ) {
    //    m_pkSpGauge->ToggleRender(false);
    //}

    m_pkHPGauge->ToggleRender( true );
    m_pkAPGaugeSP4->ToggleRender( g_kGlobalValue.IsQuickSkillPlayer( g_MyD3D->MyPlayer[ m_iPlayerIdx ]->m_kUserInfo.GetCurrentChar().iCharType ) );
    m_pkAPGatherSP4->ToggleRender( g_kGlobalValue.IsQuickSkillPlayer( g_MyD3D->MyPlayer[ m_iPlayerIdx ]->m_kUserInfo.GetCurrentChar().iCharType ) );

    m_pkMPGaugeEx->ToggleRender( !m_pkAPGaugeSP4->IsRenderOn() );
    m_pkMPGatherEx->ToggleRender( !m_pkAPGatherSP4->IsRenderOn() );
    m_pkMPGauge->ToggleRender( false );
    m_pkMPGather->ToggleRender( false );
    m_pkAPGauge->ToggleRender( false );
    m_pkAPGather->ToggleRender( false );

    if( g_kGlobalValue.IsQuickSkillPlayer( iChar ) )
        m_pkAPBack->ToggleRender( true );
    else
        m_pkAPBack->ToggleRender( false );

    m_pkHUDMainPart->ToggleRender( false );
    m_pkHUDMainPart4->ToggleRender( true );

    if( g_kGlobalValue.GetMaxMP() == 4.f ) {
        m_pkLockMP4Mark->ToggleRender( false );
    } else {
        m_pkLockMP4Mark->ToggleRender( true );
    }
    m_pkHUDSubPart->ToggleRender( true );
    m_pkHUDChatBar->ToggleRender( false );
}

void KGCMyHeadup::ToggleAllUI_Dungeon64( int iChar )
{
    for(int i = 0; i < GC_CHAR_NUM; i++)
    {
        m_pkChar[i]->ToggleRender( false );
    }

    m_pkSkillType[0]->ToggleRender( false );
    m_pkSkillType[1]->ToggleRender( false );
    m_pkBlackMP->ToggleRender( false );
    m_pkWhiteMPGauge->ToggleRender( false );
    m_pkWhiteMP4Gauge->ToggleRender( false );
    m_pkMPFull->ToggleRender( false );
    m_pkHPGauge->ToggleRender( false );
    m_pkMPGauge->ToggleRender( false );
    m_pkAPGauge->ToggleRender( false );
    m_pkMPGather->ToggleRender( false );
    m_pkAPGather->ToggleRender( false );
    m_pkMPGaugeEx->ToggleRender( false );
    m_pkMPGatherEx->ToggleRender( false );
    m_pkBlackGather->ToggleRender( false );
    m_pkAPGaugeSP4->ToggleRender( false );
    m_pkAPGatherSP4->ToggleRender( false );
    m_pkAPBack->ToggleRender( false );
    m_pkMP1st->ToggleRender( false );
    m_pkMP2nd->ToggleRender( false );
    m_pkMP3rd->ToggleRender( false );
    m_pkMP1stEx->ToggleRender( false );
    m_pkMP2ndEx->ToggleRender( false );
    m_pkMP3rdEx->ToggleRender( false );
    m_pkMP4thEx->ToggleRender( false );
    m_pkLockMP4Mark->ToggleRender( false );
    m_pkHPStop->ToggleRender( false );
    m_pkSpGauge->ToggleRender(false);
    m_pkSpGauge->ToggleRender(false);
    m_pGameSkillBox->ToggleRender(false);
    m_pkHUDMainPart->ToggleRender( false );
    m_pkHUDMainPart4->ToggleRender( false );
    m_pkHUDSubPart->ToggleRender( false );

    D3DXVECTOR2 vPos(15.0f, 5.0f);
    D3DXVECTOR2 vSize(380.0f, 35.0f);
    m_pkHUDChatBar->ToggleRender( true );
    m_pkHUDChatBar->SetWindowPosDirect( vPos );
    m_pkHUDChatBar->SetSize( static_cast<DWORD>(vSize.x), static_cast<DWORD>(vSize.y) );
}

void KGCMyHeadup::ToggleBlackGauge( bool bShow )
{
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;

    if( m_pkBlackGather )
        m_pkBlackGather->ToggleRender( bShow );
}

void KGCMyHeadup::SetPlayerSkillType( int iType )
{
    if( GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode() )
        return;

    for(int i = 0; i < 2; i++)
    {
        m_pkSkillType[i]->ToggleRender(i == iType);
    }
}

void KGCMyHeadup::SetRemainString( std::wstring& strRemain )
{
    m_strRemainSpecial = strRemain;
}

void KGCMyHeadup::SetNextTagTime( int iCharType, DWORD dwTIme )
{
 //   const KSafeArray<int,TAG_CHAR_NUM>& aiCharSlot = g_pTagMatchSelect->GetSlot();

	//int i;
 //   for( i = 0; i < (int)m_vecCharSlot.size(); ++i ) 
 //   {
 //       if( m_vecCharSlot[i] == iCharType )
 //           break;
 //   }

 //   if( m_vecCharSlot.empty() || i == (int)m_vecCharSlot.size() )
 //   {
 //       ASSERT( "!케릭터 없다" );
 //       return;
 //   }

 //   for( int it = 0; it < NUM_TAKE_ITEM; ++it )
 //   {
 //       m_pkNextTagTime[it]->ToggleRender( false );
 //       m_pkNextTagTime[it]->SetText( L"" );
 //       m_dwNextTagTime[it] = 0;
 //   }

 //   m_dwNextTagTime[i] = dwTIme;
}

int KGCMyHeadup::GetNextTagTimeSec( DWORD dwTime )
{
    return (dwTime / GC_FPS_LIMIT) + 1;
}

void KGCMyHeadup::SetSPAmount( float fAmount )
{
	 m_pkSpGauge->SetAmount(fAmount);
}