#include "stdafx.h"
#include ".\gcongameexpbar.h"
#include "GCOnGameExpGauge.h"
//




#include "GCGameResult.h"
//

IMPLEMENT_CLASSNAME( GCOnGameExpBar );
IMPLEMENT_WND_FACTORY( GCOnGameExpBar );
IMPLEMENT_WND_FACTORY_NAME( GCOnGameExpBar, "gc_on_game_exp_bar" );

GCOnGameExpBar::GCOnGameExpBar(void)
{
    m_pExpGauge         = NULL;
    m_fCurExp           = 0.0f;
    m_fVirtualExp       = 0.0f;
    m_pkExpPercent      = NULL;

    LINK_CONTROL( "exp_gauge", m_pExpGauge );
    LINK_CONTROL( "exp_percent_static", m_pkExpPercent );
}

GCOnGameExpBar::~GCOnGameExpBar(void)
{
}

void GCOnGameExpBar::OnCreate( void )
{
    m_pkExpPercent->InitState( true );
    m_pkExpPercent->SetFontColor( 0xCCFFFFFF );
    m_pkExpPercent->SetShadow( true );
    m_pkExpPercent->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
}
void GCOnGameExpBar::OnDestroy( void )
{

}

void GCOnGameExpBar::InitGauge()
{
    m_pExpGauge->InitGauge();
}

void GCOnGameExpBar::SetExp( float fExp_ )
{ 
    m_pExpGauge->SetExp( fExp_ );

    m_fCurExp = fExp_;
    SetCurrentExpPercent();
}

void GCOnGameExpBar::AddExp( float fExp_ )
{
    m_pExpGauge->AddExp( fExp_ );

    m_fCurExp += fExp_;
    SetCurrentExpPercent();
}

void GCOnGameExpBar::SetCurrentExpPercent()
{
    float fCurrentExp = GetCurrentExpPercent( (GCEXPTYPE)m_fCurExp );
    int iLevel = Exp_2_Level( (GCEXPTYPE)m_fCurExp );

    //int iTargetExp = (int)Level_2_Exp( iLevel + 1 ) - (int)Level_2_Exp( iLevel );
    //int iRemainExp = (int)Exp_For_Next( (DWORD)m_fCurExp );

    WCHAR strTemp[100];
    swprintf( strTemp, 100, L"%s %i   %3.2f%%", g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str(), iLevel, fCurrentExp );
    m_pkExpPercent->SetText( strTemp );
}

void GCOnGameExpBar::SetVirtualExp( float fExp_ )
{ 
    m_pExpGauge->SetVirtualExp( fExp_ );

    m_fVirtualExp = fExp_;
    SetVirtualExpPercent();
}

void GCOnGameExpBar::AddVirtualExp( float fExp_ )
{
    m_pExpGauge->AddVirtualExp( fExp_ );

    m_fVirtualExp += fExp_;
    SetVirtualExpPercent();
}

void GCOnGameExpBar::SetVirtualExpPercent()
{
    // 레벨도 표시해 주는게..
    float fExp = GetCurrentExpPercent( (GCEXPTYPE)m_fVirtualExp );
    int iLevel = Exp_2_Level( (GCEXPTYPE)m_fVirtualExp );

    //int iTargetExp = (int)Level_2_Exp( iLevel + 1 ) - (int)Level_2_Exp( iLevel );
    //int iRemainExp = (int)Exp_For_Next( (DWORD)m_fVirtualExp );

    WCHAR strTemp[100];
    swprintf( strTemp, 100, L"%s %i   %3.2f%%", g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str(), iLevel, fExp );
    m_pkExpPercent->SetText( strTemp );
}
