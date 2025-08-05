#include "stdafx.h"
#include "GCPromotionChangePanel.h"

IMPLEMENT_CLASSNAME( KGCPromotionChangePanel );
IMPLEMENT_WND_FACTORY( KGCPromotionChangePanel );
IMPLEMENT_WND_FACTORY_NAME( KGCPromotionChangePanel, "gc_promotionchangepanel" );

KGCPromotionChangePanel::KGCPromotionChangePanel( void )
{
    m_pkAtk = NULL;
    m_pkDef = NULL;
    m_pkHP  = NULL;
    m_pkCri = NULL;
    LINK_CONTROL( "info_atk", m_pkAtk );
    LINK_CONTROL( "info_def", m_pkDef );
    LINK_CONTROL( "info_hp", m_pkHP );
    LINK_CONTROL( "info_cri", m_pkCri );

    m_pkAtkBack = NULL;
    m_pkDefBack = NULL;
    m_pkHPBack  = NULL;
    m_pkCriBack = NULL;
    LINK_CONTROL( "atk_back", m_pkAtkBack );
    LINK_CONTROL( "def_back", m_pkDefBack );
    LINK_CONTROL( "hp_back", m_pkHPBack );
    LINK_CONTROL( "cri_back", m_pkCriBack );

    m_pkStaticAtk = NULL;
    m_pkStaticDef = NULL;
    m_pkStaticHP  = NULL;
    m_pkStaticCri = NULL;
    LINK_CONTROL( "static_atk", m_pkStaticAtk );
    LINK_CONTROL( "static_def", m_pkStaticDef );
    LINK_CONTROL( "static_hp", m_pkStaticHP );
    LINK_CONTROL( "static_cri", m_pkStaticCri );
}

KGCPromotionChangePanel::~KGCPromotionChangePanel( void )
{
}

void KGCPromotionChangePanel::OnCreate( void )
{
    m_pkAtk->ToggleRender( false );
    m_pkDef->ToggleRender( false );
    m_pkHP->ToggleRender( false );
    m_pkCri->ToggleRender( false );

    m_pkAtkBack->ToggleRender( false );
    m_pkDefBack->ToggleRender( false );
    m_pkHPBack->ToggleRender( false );
    m_pkCriBack->ToggleRender( false );

    m_pkStaticAtk->SetFontColor(0xffffffff);
    m_pkStaticAtk->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticAtk->SetShadow(true);
    m_pkStaticAtk->ToggleRender(false);
    m_pkStaticAtk->SetAlign(DT_CENTER);

    m_pkStaticDef->SetFontColor(0xffffffff);
    m_pkStaticDef->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticDef->SetShadow(true);
    m_pkStaticDef->ToggleRender(false);
    m_pkStaticDef->SetAlign(DT_CENTER);

    m_pkStaticHP->SetFontColor(0xffffffff);
    m_pkStaticHP->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticHP->SetShadow(true);
    m_pkStaticHP->ToggleRender(false);
    m_pkStaticHP->SetAlign(DT_CENTER);

    m_pkStaticCri->SetFontColor(0xffffffff);
    m_pkStaticCri->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticCri->SetShadow(true);
    m_pkStaticCri->ToggleRender(false);
    m_pkStaticCri->SetAlign(DT_CENTER);
}

void KGCPromotionChangePanel::SetPromotionInfo(int iPromotion)
{
    if( iPromotion < 0 || iPromotion > NUM_JOB_LEVEL-1 )
        return;
    
    int iBonusStatus = 0;
    int iBonusCritical = 0;

    switch( iPromotion )
    {
    case 1:
#ifdef NATION_KOREA
        iBonusStatus = g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
#else
        iBonusStatus = 100;
#endif
        break;

    case 2:
#ifdef NATION_KOREA
        iBonusStatus = g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
        iBonusCritical = static_cast<int>(g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical * 100.0f);
#else
        iBonusStatus = 250;
        iBonusCritical = 1;
 
#endif
        break;

    case 3:
#ifdef NATION_KOREA
        iBonusStatus = g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
        iBonusCritical = static_cast<int>(g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical * 100.0f);
#else
        iBonusStatus = 500;
        iBonusCritical = 2;
#endif
        break;

    default:
        break;
    }

    std::wostringstream strmStatus;
    std::wostringstream strmCritical;

    strmStatus << L"+" << iBonusStatus ;
    strmCritical << L"+" << iBonusCritical << L"%";

    m_pkStaticAtk->SetText(strmStatus.str());
    m_pkStaticDef->SetText(strmStatus.str());
    m_pkStaticHP->SetText(strmStatus.str());
    m_pkStaticCri->SetText(strmCritical.str());
}

void KGCPromotionChangePanel::SetShow(bool bShow)
{
    this->ToggleRender( bShow );

    m_pkAtk->ToggleRender( bShow );
    m_pkDef->ToggleRender( bShow );
    m_pkHP->ToggleRender( bShow );
    m_pkCri->ToggleRender( bShow );

    m_pkAtkBack->ToggleRender( bShow );
    m_pkDefBack->ToggleRender( bShow );
    m_pkHPBack->ToggleRender( bShow );
    m_pkCriBack->ToggleRender( bShow );

    m_pkStaticAtk->ToggleRender( bShow );
    m_pkStaticDef->ToggleRender( bShow );
    m_pkStaticHP->ToggleRender( bShow );
    m_pkStaticCri->ToggleRender( bShow );
}