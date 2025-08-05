#include "stdafx.h"
#include "KGCStatusInfoPanel.h"

//
//
#include "../MyD3D.h"


//
#include "Procedure.h"
//

//


#include "KGCUIHelper.h"
#include "GCQuestRankScore.h"
#include "KGCElvis.h"
#include "GCIconImageManager.h"
#include "GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCStatusInfoPanel );
IMPLEMENT_WND_FACTORY( KGCStatusInfoPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCStatusInfoPanel, "gc_status_info_panel" );

KGCStatusInfoPanel::KGCStatusInfoPanel()
:	m_pkScrollbar(NULL)
,	m_pkListDesc(NULL)
,	m_iStartAtk(0)
,	m_iChangeAtk(0)
,   m_iStartTotalAtk(0)
,   m_iChangeTotalAtk(0)
,	m_iStartDef(0)
,	m_iChangeDef(0)
,	m_iStartHP(0)
,	m_iChangeHP(0)
,	m_iStartSPAtk(0)
,	m_iChangeSPAtk(0)
,	m_iStartSPDef(0)
,	m_iChangeSPDef(0)
,	m_fStartCritical(0.0f)
,	m_fChangeCritical(0.0f)
,	m_fStartCriticalDamage(0.0f)
,	m_fChangeCriticalDamage(0.0f)
,	m_fStartCounterDef(0.0f)
,	m_fChangeCounterDef(0.0f)
,	m_fStartHPRecovery(0.0f)
,	m_fChangeHPRecovery(0.0f)
,	m_fStartMPRecovery(0.0f)
,	m_fChangeMPRecovery(0.0f)
,	m_fStartGPAdd(0.0f)
,	m_fChangeGPAdd(0.0f)
,	m_fStartEXPAdd(0.0f)
,	m_fChangeEXPAdd(0.0f)
,	m_iStartDarkAtkDamage(0)
,	m_iChangeDarkAtkDamage(0)
,   m_iChangePetMPRecUp( 0.0f )
,   m_fStartCounterDefMatch( 0.0f )
,   m_fChangeCounterDefMatch( 0.0f )
,   m_fStartMPRecoveryMatch( 0.0f )
,   m_fChangeMPRecoveryMatch( 0.0f )
,   m_iElyosResistBuff(0)
,   m_iChangeElyosResistBuff(0)
,   m_bRoomUserInfo( false )
{

    LINK_CONTROL("status_desc", m_pkListDesc);
    LINK_CONTROL("status_status", m_pkListStatus);

	LINK_CONTROL("scroll", m_pkScrollbar);
		
    m_iStartGrade = -1;
    m_iChangeGrade = -1;
	m_mapListIndexAutoDivideString.clear();

	m_iStartHPDrain = m_iChangeHPDrain = 0;
	m_iStartMPDrain = m_iChangeMPDrain = 0;

	for (int i=0; i<MAX_NUM_GRADE ; i++)
	{
		m_pkGrade[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "grade%d", i );
		LINK_CONTROL( temp, m_pkGrade[i] );
	}
}

void KGCStatusInfoPanel::OnCreate()
{
    DWORD dwColorA;
    DWORD dwColorB;

    if ( m_bRoomUserInfo ) 
    {
        dwColorA = 0xff88c343;
        dwColorB = 0xffffffff;        
    }
    else
    {
        dwColorA = 0xffffffff;
        dwColorB = 0xffffffff;
    }

    m_pkListDesc->Clear();

    m_pkListDesc->SetFontColor(0xffffffff);

	m_pkListDesc->InitState(true,false,this);
	m_pkListStatus->InitState(true,false,this);

#if defined(NATION_USA) ||  defined(NATION_PHILIPPINE) || defined(NATION_THAILAND) || defined( NATION_EU ) || defined( NATION_IDN)

	m_pkListDesc->SetLineSpace(1.0f);
	m_pkListStatus->SetLineSpace(1.0f);
#elif defined(NATION_KOREA)

	m_pkListDesc->SetLineSpace(1.2f);
	m_pkListStatus->SetLineSpace(1.2f);
#else
	m_pkListDesc->SetLineSpace(1.3f);
	m_pkListStatus->SetLineSpace(1.3f);
#endif
    m_pkListDesc->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkListDesc->SetShadow(true);

    m_pkListStatus->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());


	if(m_pkScrollbar){
#if defined( USE_MATCH_STATUS_RATE )
		m_pkScrollbar->InitState(true,false,this);
#else 
        m_pkScrollbar->InitState(false,false,this);
#endif
		m_pkScrollbar->SetScrollPageSize( m_pkListDesc->GetDisplayLimitLine() );
		m_pkScrollbar->SetScrollRangeMax( m_pkListDesc->GetCount() );
        m_pkScrollbar->SetThumbXGab(1);
	}
	
	int index = 0;
	int AutoDevideStringCnt = 0;

    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_TOTAL_ATTACK), dwColorA);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_DESC_PLAYER_INFO3), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_DESC_PLAYER_INFO4), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_DESC_PLAYER_INFO5), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_SP_ATTACK_POWER), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_SP_DEFENCE_POWER), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_CRITICAL_RATE), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_CRITICAL_DAMAGE), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_COUNTER_DEFENCE_RATE), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
#if defined( USE_MATCH_STATUS_RATE )
    std::wostringstream stmCounterMatch;
#if defined (NATION_BRAZIL)
    stmCounterMatch << g_pkStrLoader->GetString( STR_ID_COUNTER_RESIST_IN_PVP );
#else
    stmCounterMatch << g_pkStrLoader->GetString( STR_ID_GMC_MATCH ) << g_pkStrLoader->GetString( STR_ID_COUNTER_DEFENCE_RATE ) ;
#endif
    m_pkListDesc->AddString(stmCounterMatch.str(), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
#endif
    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_HP_RECOVERY_RATE), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_MP_RECOVERY_RATE), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
#if defined( USE_MATCH_STATUS_RATE )
    std::wostringstream stmMPRMatch;
#if defined (NATION_BRAZIL)
    stmMPRMatch << g_pkStrLoader->GetString( STR_ID_MP_RECOVERY_IN_PVP );
#else
    stmMPRMatch << g_pkStrLoader->GetString( STR_ID_GMC_MATCH ) << g_pkStrLoader->GetString( STR_ID_MP_RECOVERY_RATE ) ;
#endif
    m_pkListDesc->AddString(stmMPRMatch.str(), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
#endif
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_GP_PLUS_RATE), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_EXP_PLUS_RATE), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_RANDOM_ATTRIBUTE_9 ), dwColorB);
	SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ID_STR_VELPETMPREC), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
    m_pkListDesc->AddString(g_pkStrLoader->GetString(STR_ELYOS_RESISTANCE), dwColorB);
    SetListAutoDevideStringCnt(index, AutoDevideStringCnt);
	m_pkScrollbar->SetScrollRangeMax( m_pkListDesc->GetCount() - 12);
	m_pkScrollbar->SetScrollPageSize( m_pkListDesc->GetCount() - 12);
	
	m_pkListDesc->SetCheckList(true);
	m_pkListStatus->SetCheckList(true);

	for (int i=0 ; i<MAX_NUM_GRADE ; i++)
	{
		if (m_pkGrade[i])
			m_pkGrade[i]->InitState(false);
	}

}

void KGCStatusInfoPanel::SetStartUserInfo( PLAYER* pPlayer, bool bRoom )
{
    if ( !pPlayer ) 
    {
        pPlayer = &g_MyD3D->m_TempPlayer;
    }   

    // 기본능력치 얻기
    EAbilityData stAbilityData;
    pPlayer->GetCharAbility( stAbilityData );

    // 필살기 공.방어력
    m_iStartSPAtk = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_SPECIAL_ATK]);
    m_iStartSPDef = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_SPECIAL_DEF]);

    // 치명타. 카운터 저항
    m_fStartCritical = stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] * 100.0f;
    m_fStartCriticalDamage = stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP];
    m_fStartCounterDef = stAbilityData.fEquipItemAbility[ABILITY_COUNTER_DEF] * 100.0f; 

    if( m_fStartCounterDef >= 100.0f )
        m_fStartCounterDef = 100.0f;

    // 카운터 저항 대전 적용
#if defined( USE_MATCH_STATUS_RATE )
    m_fStartCounterDefMatch = m_fStartCounterDef * COUNTER_DEF_PERCENT_MATCH;
    if( m_fStartCounterDefMatch > COUNTER_DEF_MAX_VALUE_MATCH * 100.0f) { 
        m_fStartCounterDefMatch = COUNTER_DEF_MAX_VALUE_MATCH * 100.0f;
    }
#endif

    // HP, MP 회복
    m_fStartHPRecovery = (stAbilityData.fEquipItemAbility[ABILITY_HP_RECOVERY_RATE] * 100.0f); 
    m_fStartMPRecovery = (stAbilityData.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE] * 100.0f);

    // MP회복 대전적용
#if defined( USE_MATCH_STATUS_RATE )
    m_fStartMPRecoveryMatch = m_fStartMPRecovery * MP_RECOVERY_PERCENT_MATCH;
    if( m_fStartMPRecoveryMatch > MP_RECOVERY_MAX_VALUE_MATCH * 100.0f ) { 
        m_fStartMPRecoveryMatch = MP_RECOVERY_MAX_VALUE_MATCH * 100.0f;
    }
#endif

    // HP, MP 흡수
    m_iStartHPDrain = ( static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_HP_DRAIN_RATE]) );
    m_iStartMPDrain = ( static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_MP_DRAIN_RATE]) );

    // GP, EXP 추가 획득 정보 가져온다. ( 캐릭터 능력치에는 해당 정보가 없다. )
    EItemAbilityData stItemAttribute;
    g_MyD3D->m_kItemMgr.GetItemAttribute( pPlayer, pPlayer->GetCurrentChar().iCharType, false, stItemAttribute);
    m_fStartGPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE];
    m_fStartEXPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE];

	m_iStartDarkAtkDamage = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_DARK_DAMAGE_ATK_POINT]);   

    m_iStartPetMPRecUp = stAbilityData.fEquipItemAbility[ABILITY_RECOVERY_UP_PET_MP];

    m_iStartAtk = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_ATK]);
    m_iStartDef = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_DEF]);
    m_iStartHP = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_HP]);

    m_iElyosResistBuff = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_ELYOS_RESIST_BUFF]);

    m_iStartTotalAtk = static_cast<int>(pPlayer->GetCharTotalAtk( stAbilityData ));

    // 등급
	m_iStartGrade = GCFUNC::GetStatusGradeByTotalAtk( m_iStartTotalAtk );
}

void KGCStatusInfoPanel::SetChangeUserInfo( PLAYER* pPlayer, bool bRoom )
{
    if ( !pPlayer ) 
    {
        pPlayer = &g_MyD3D->m_TempPlayer;
    }

    // 기본능력치 얻기
    EAbilityData stAbilityData;
    pPlayer->GetCharAbility( stAbilityData );


    // 필살기 공.방어력
    m_iChangeSPAtk = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_SPECIAL_ATK]);
    m_iChangeSPDef = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_SPECIAL_DEF]);

    // 치명타. 카운터 저항
    m_fChangeCritical = (stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE]) * 100.0f;
    m_fChangeCriticalDamage = (stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP]);
    m_fChangeCounterDef = (stAbilityData.fEquipItemAbility[ABILITY_COUNTER_DEF] * 100.0f); 

    if( m_fChangeCounterDef >= 100.0f )
        m_fChangeCounterDef = 100.0f;

    // 카운터 저항 대전 적용
#if defined( USE_MATCH_STATUS_RATE )
    m_fChangeCounterDefMatch = m_fChangeCounterDef * COUNTER_DEF_PERCENT_MATCH;
    if( m_fChangeCounterDefMatch > COUNTER_DEF_MAX_VALUE_MATCH * 100.0f) { 
        m_fChangeCounterDefMatch = COUNTER_DEF_MAX_VALUE_MATCH * 100.0f;
    }
#endif

    // HP, MP 회복
    m_fChangeHPRecovery = (stAbilityData.fEquipItemAbility[ABILITY_HP_RECOVERY_RATE] * 100.0f); 
    m_fChangeMPRecovery = (stAbilityData.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE] * 100.0f);

    // MP회복 대전적용
#if defined( USE_MATCH_STATUS_RATE )
    m_fChangeMPRecoveryMatch = m_fChangeMPRecovery * MP_RECOVERY_PERCENT_MATCH;
    if( m_fChangeMPRecoveryMatch > MP_RECOVERY_MAX_VALUE_MATCH * 100.0f ) { 
        m_fChangeMPRecoveryMatch = MP_RECOVERY_MAX_VALUE_MATCH * 100.0f;
    }
#endif

    // HP, MP 흡수
    m_iChangeHPDrain = ( static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_HP_DRAIN_RATE]) );
    m_iChangeMPDrain = ( static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_MP_DRAIN_RATE]) );

    // GP, EXP 추가 획득
    EItemAbilityData stItemAttribute;
    g_MyD3D->m_kItemMgr.GetItemAttribute( pPlayer, pPlayer->GetCurrentChar().iCharType, false, stItemAttribute);
    m_fChangeGPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE];
    m_fChangeEXPAdd = stItemAttribute.fItemAttribute[ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE];   

	m_iChangeDarkAtkDamage = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_DARK_DAMAGE_ATK_POINT]);   

    m_iChangePetMPRecUp = stAbilityData.fEquipItemAbility[ABILITY_RECOVERY_UP_PET_MP];

    m_iChangeAtk = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_ATK]);
    m_iChangeDef = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_DEF]);
    m_iChangeHP = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_HP]);

    m_iChangeElyosResistBuff = static_cast<int>(stAbilityData.fEquipItemAbility[ABILITY_ELYOS_RESIST_BUFF]);

    m_iChangeTotalAtk = static_cast<int>(pPlayer->GetCharTotalAtk( stAbilityData ));

    // 등급
	m_iChangeGrade = GCFUNC::GetStatusGradeByTotalAtk( m_iChangeTotalAtk );
}

void KGCStatusInfoPanel::ResetInfo()
{
    m_iStartGrade = -1;
    m_iChangeGrade = -1;
    m_iStartAtk = m_iChangeAtk = 0;
    m_iStartTotalAtk = m_iChangeTotalAtk = 0;
    m_iStartDef = m_iChangeDef = 0;
    m_iStartHP = m_iChangeHP = 0;
    m_iStartSPAtk = m_iChangeSPAtk = 0;
    m_iStartSPDef = m_iChangeSPDef = 0;

    m_iStartHPDrain = m_iChangeHPDrain = 0;
    m_iStartMPDrain = m_iChangeMPDrain = 0;

    m_fStartCounterDef = m_fChangeCounterDef = 0.0f;

    m_fStartCritical = m_fChangeCritical = 0.0f;
    m_fStartCriticalDamage = m_fChangeCriticalDamage = 0.0f;
    m_fStartHPRecovery = m_fChangeHPRecovery =  0.0f;
    m_fStartMPRecovery = m_fChangeMPRecovery = 0.0f;

    m_fStartGPAdd = m_fChangeGPAdd = 0.0f;
    m_fStartEXPAdd = m_fChangeEXPAdd = 0.0f;

    m_iElyosResistBuff = m_iChangeElyosResistBuff = 0;

	m_pkListStatus->Clear();

    //for(int i=0 ; i<Num_Status ; ++i)
    //    m_apkStaticStatus[i]->SetText(L"");
}

void KGCStatusInfoPanel::SetText()
{
    m_pkListStatus->Clear();

    if(SiKGCChannelManager()->IsPVPBallanceServer()
		|| SiKGCRoomManager()->IsAngelEggModeAbility()
		|| SiKGCRoomManager()->IsDotaModeAbility() )

    {
        m_pkListStatus->AddString(L"-");
        SetListAutoDevideString(static_cast<int>(Atk));
        m_pkListStatus->AddString(L"-");
        SetListAutoDevideString(static_cast<int>(Def));
        m_pkListStatus->AddString(L"-");
        SetListAutoDevideString(static_cast<int>(HP));
    }
    else    
    {
        m_pkListStatus->AddString(GetStatusString(TotalAck, m_iChangeTotalAtk, m_iStartTotalAtk));
        SetListAutoDevideString(static_cast<int>(TotalAck));
        m_pkListStatus->AddString(GetStatusString(Atk, m_iChangeAtk, m_iStartAtk));
        SetListAutoDevideString(static_cast<int>(Atk));
        m_pkListStatus->AddString(GetStatusString(Def, m_iChangeDef, m_iStartDef));
        SetListAutoDevideString(static_cast<int>(Def));
        m_pkListStatus->AddString(GetStatusString(HP, m_iChangeHP, m_iStartHP));
        SetListAutoDevideString(static_cast<int>(HP));
    }

    m_pkListStatus->AddString(GetStatusString(SPAtk, m_iChangeSPAtk, m_iStartSPAtk));
    SetListAutoDevideString(static_cast<int>(SPAtk));
    m_pkListStatus->AddString(GetStatusString(SPDef, m_iChangeSPDef, m_iStartSPDef));
    SetListAutoDevideString(static_cast<int>(SPDef));

    if(SiKGCChannelManager()->IsPVPBallanceServer() ||
        SiKGCRoomManager()->IsAngelEggModeAbility() ||
        SiKGCRoomManager()->IsDotaModeAbility() )
    {
        m_pkListStatus->AddString(L"0.0%");
        SetListAutoDevideString(static_cast<int>(Critical));
        m_pkListStatus->AddString(L"0.0%");
    }
    else 
    {
        m_pkListStatus->AddString(GetStatusString(Critical, m_fChangeCritical, m_fStartCritical, true));
        SetListAutoDevideString(static_cast<int>(Critical));
        m_pkListStatus->AddString(GetStatusString(CriticalDamage, m_fChangeCriticalDamage, m_fStartCriticalDamage));
    }
    SetListAutoDevideString(static_cast<int>(CriticalDamage));
    SetListAutoDevideString(static_cast<int>(Critical));
    m_pkListStatus->AddString(GetStatusString(CounterDef, m_fChangeCounterDef, m_fStartCounterDef));
    SetListAutoDevideString(static_cast<int>(CounterDef));
#if defined( USE_MATCH_STATUS_RATE )
    m_pkListStatus->AddString(GetStatusString(CounterDefMatch, m_fChangeCounterDefMatch, m_fStartCounterDefMatch));
    SetListAutoDevideString(static_cast<int>(CounterDefMatch));
#endif
    m_pkListStatus->AddString(GetStatusString(HPRecovery, m_fChangeHPRecovery, m_fStartHPRecovery));
    SetListAutoDevideString(static_cast<int>(HPRecovery));
    m_pkListStatus->AddString(GetStatusString(MPRecovery, m_fChangeMPRecovery, m_fStartMPRecovery));
    SetListAutoDevideString(static_cast<int>(MPRecovery));
#if defined( USE_MATCH_STATUS_RATE )
    m_pkListStatus->AddString(GetStatusString(MPRecoveryMatch, m_fChangeMPRecoveryMatch, m_fStartMPRecoveryMatch));
    SetListAutoDevideString(static_cast<int>(MPRecoveryMatch));
#endif
    m_pkListStatus->AddString(GetStatusString(GPAdd, m_fChangeGPAdd, m_fStartGPAdd));
    SetListAutoDevideString(static_cast<int>(GPAdd));
    m_pkListStatus->AddString(GetStatusString(EXPAdd, m_fChangeEXPAdd, m_fStartEXPAdd));
    SetListAutoDevideString(static_cast<int>(EXPAdd));
    m_pkListStatus->AddString(GetStatusString(DarkDamage, m_iChangeDarkAtkDamage, m_iStartDarkAtkDamage));
    SetListAutoDevideString(static_cast<int>(DarkDamage));

    m_pkListStatus->AddString(GetStatusString(PetMPRecUp, m_iChangePetMPRecUp, m_iStartPetMPRecUp));
    SetListAutoDevideString(static_cast<int>(PetMPRecUp));

    m_pkListStatus->AddString(GetStatusString(ElyosResist, m_iChangeElyosResistBuff, m_iElyosResistBuff));
    SetListAutoDevideString(static_cast<int>(ElyosResist));

    m_pkScrollbar->SetScrollPos( 0 );
    m_pkListDesc->SetTopIndex( m_pkScrollbar->GetScrollPos());
    m_pkListStatus->SetTopIndex( m_pkScrollbar->GetScrollPos());
}

std::wstring KGCStatusInfoPanel::GetDifString(int iDiff_)
{
    std::wostringstream strm;
    strm.setf(std::ios_base::right);
    std::wstring strColorKey;
    std::wstring strColorKeyEnd = L"#cX";    
     
    if( iDiff_ == 0 ) {
        strm << L"";
        return strm.str();
    }

    if( iDiff_ < 0 )  
        strColorKey = L"#cff0000(↓";
    else if( iDiff_ > 0 ) 
        strColorKey = L"#c00ff00(↑";
    
    strm << strColorKey << abs(iDiff_) << L")" << strColorKeyEnd;
    return strm.str();
}

std::wstring KGCStatusInfoPanel::GetDifString(float fDiff_)
{
    std::wostringstream strm;
    strm.setf(std::ios_base::right | std::ios_base::fixed);

    std::wstring strColorKey;
    std::wstring strColorKeyEnd = L"#cX";    

    if( fDiff_ == 0.0f ) {
        strm << L"";
        return strm.str();
    }

    if( fDiff_ < 0.0f )  
        strColorKey = L"#cff0000(↓";
    else if( fDiff_ > 0.0f ) 
        strColorKey = L"#c00ff00(↑";

    strm << strColorKey << setprecision(1) << ASMfabs(fDiff_) << L"%)" << strColorKeyEnd;
    return strm.str();
}

std::wstring KGCStatusInfoPanel::GetOverString(float fOver_)
{
    std::wostringstream strm;
    strm.setf(std::ios_base::right | std::ios_base::fixed);

    std::wstring strColorKey;
    std::wstring strColorKeyEnd = L"#cX";

    if (fOver_ == 0.0f) {
        strm << L"";
        return strm.str();
    }

    strColorKey = L"#cff0000(";

    strm << strColorKey << setprecision(1) << ASMfabs(fOver_) << L"%)" << strColorKeyEnd;
    return strm.str();
}

std::wstring KGCStatusInfoPanel::GetStatusString(int iIndex_, int iChangeVal_, int iStartVal_)
{  
    std::wostringstream strm; 
    strm.setf(std::ios_base::right);

    int iVal = iChangeVal_ - iStartVal_;

    strm << GetDifString(iVal) << iChangeVal_;
    return strm.str();
}

std::wstring KGCStatusInfoPanel::GetStatusString(int iIndex_, float fChangeVal_, float fStartVal_, bool isOver)
{
    std::wostringstream strm;
    strm.setf(std::ios_base::right | std::ios_base::fixed);

    float fVal = fChangeVal_ - fStartVal_;
    float fFinalVal = fChangeVal_ - fStartVal_;
    std::wstring overValue = L"";

    if ( isOver )
    {
        if ( fChangeVal_ > 100 )
        {
            fFinalVal = 100;
            overValue = GetOverString( fChangeVal_ - fFinalVal );
        }
        else
        {
            fFinalVal = fChangeVal_;
            overValue = L"";
        }
    }
    else
    {
        fFinalVal = fChangeVal_;
        overValue = L"";
    }

    strm << GetDifString( fVal ) << setprecision(1) << fFinalVal << L"% " << overValue;
    return strm.str();
}

std::wstring KGCStatusInfoPanel::GetStatusString(int iIndex_, float fChangeVal0_, float fStartVal0_, float fChangeVal1_, float fStartVal1_)
{
    std::wostringstream strm;
    strm.setf(std::ios_base::right | std::ios_base::fixed);
    float fVal0 = fChangeVal0_ - fStartVal0_;
    float fVal1 = fChangeVal1_ - fStartVal1_;

    strm << GetDifString(fVal0) << setprecision(1) << fChangeVal0_ << L"%," << GetDifString(fVal1) << setprecision(1) << fChangeVal1_ << L"%";
    return strm.str();
}

void KGCStatusInfoPanel::SetPromotionStatus(int iPromotion, int& iAtk, int& iDef, int& iHP, float &fCritical)
{
    if(SiKGCChannelManager()->IsPVPBallanceServer())
        return;

    for(int i = 0 ; i <= iPromotion ; ++i )
    {
        switch( i )
        {
        case 1:
            iAtk += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
            iDef += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
            iHP += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
            break;

        case 2:
            iAtk += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
            iDef += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;    
            iHP += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status; 
            fCritical += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
            break;

        case 3:
            iAtk += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
            iDef += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
            iHP += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status; 
            fCritical += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
            break;
        }
    }
}

void KGCStatusInfoPanel::OnScroll(const KActionEvent& event_)
{
	if ( m_pkListDesc->GetTopIndex() != m_pkScrollbar->GetScrollPos())
	{
		g_KDSound.Play( "73" );
		m_pkListDesc->SetTopIndex( m_pkScrollbar->GetScrollPos() );
	}
	if ( m_pkListStatus->GetTopIndex() != m_pkScrollbar->GetScrollPos())
	{
		g_KDSound.Play( "73" );
		m_pkListStatus->SetTopIndex( m_pkScrollbar->GetScrollPos() );
	}
}

void KGCStatusInfoPanel::SetListAutoDevideStringCnt( int &index, int &AutoDevideStringCnt )
{
	index++;
	m_mapListIndexAutoDivideString.insert(std::pair<int,int>(index - 1, m_pkListDesc->GetCount() - (index + AutoDevideStringCnt)));
	AutoDevideStringCnt += m_pkListDesc->GetCount() - (index + AutoDevideStringCnt);
}

void KGCStatusInfoPanel::SetListAutoDevideString( int index)
{
	std::map<int,int>::iterator mit = m_mapListIndexAutoDivideString.find(index);

	if(mit != m_mapListIndexAutoDivideString.end())
	{
		for(int i = 0; i < mit->second ; i++)
		{
			m_pkListStatus->AddString(L" ");
		}
	}
}

void KGCStatusInfoPanel::ActionPerformed( const KActionEvent& event_ )
{
	if(event_.m_pWnd == m_pkScrollbar)
	{
		OnScroll(event_);
	}
}

void KGCStatusInfoPanel::FrameMoveInEnabledState()
{
	for (int i=0 ; i<MAX_NUM_GRADE ; i++)
	{
		if (m_pkGrade[i])
			m_pkGrade[i]->ToggleRender(false);
	}

	if ( m_iChangeGrade>=0 && m_iChangeGrade<MAX_NUM_GRADE )
		if (m_pkGrade[m_iChangeGrade])
			m_pkGrade[m_iChangeGrade]->ToggleRender(true);

}
