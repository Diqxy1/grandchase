#include "stdafx.h"
#include "GCResultDamage.h"

IMPLEMENT_CLASSNAME( GCResultDamage );
IMPLEMENT_WND_FACTORY( GCResultDamage );
IMPLEMENT_WND_FACTORY_NAME( GCResultDamage, "gc_result_damage" );

GCResultDamage::GCResultDamage( void )
: m_pkDamageStatic(NULL)
, m_pkSerdinBack(NULL)
, m_pkCanabanBack(NULL)
, m_pkTotalDmgImg(NULL)
{
    LINK_CONTROL( "back_serdin", m_pkSerdinBack );
    LINK_CONTROL( "back_canaban", m_pkCanabanBack );
    LINK_CONTROL("total_damage_img", m_pkTotalDmgImg);

    LINK_CONTROL("damage_static" , m_pkDamageStatic);
}

GCResultDamage::~GCResultDamage( void )
{
}

void GCResultDamage::OnCreate()
{
    m_pkSerdinBack->InitState(false, true, this);
    m_pkCanabanBack->InitState(false, true, this);
    m_pkTotalDmgImg->InitState(false, true, this);

    m_pkDamageStatic->InitState(false, true, this);
    m_pkDamageStatic->SetText(L"0");
    m_pkDamageStatic->SetFontSize( 13 );
    m_pkDamageStatic->SetAlign( DT_RIGHT );
    m_pkDamageStatic->SetFontOutline( true);
    m_pkDamageStatic->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
}

void GCResultDamage::ActionPerformed(const KActionEvent& event )
{

}

void GCResultDamage::FrameMoveInEnabledState()
{

}

void GCResultDamage::Init()
{
    m_pkSerdinBack->ToggleRender(false);
    m_pkCanabanBack->ToggleRender(false);
    m_pkDamageStatic->ToggleRender( false );
    m_pkDamageStatic->SetText(L"0");
    m_pkTotalDmgImg->ToggleRender(false);
}

void GCResultDamage::SetDamage(int iDamage,int iTeam)
{
    if ( iTeam == TEAM_SERDIN) {
        m_pkSerdinBack->ToggleRender(true);
        m_pkCanabanBack->ToggleRender(false);
    }
    else {
        m_pkSerdinBack->ToggleRender(false);
        m_pkCanabanBack->ToggleRender(true);
    }

    std::wstring wstrDamage = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iDamage);

    m_pkDamageStatic->SetText(wstrDamage);
    m_pkDamageStatic->ToggleRender(true);
    
    m_pkTotalDmgImg->ToggleRender(true);
};