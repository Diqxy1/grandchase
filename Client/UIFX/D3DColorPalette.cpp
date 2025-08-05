#include "stdafx.h"
//#include "./D3DColorPalette.h"
//
//
//
//
//
//
#include "../GCItemManager.h"
#include "GCTextColorManager.h"
//


IMPLEMENT_CLASSNAME( KD3DColorPalette );
IMPLEMENT_WND_FACTORY( KD3DColorPalette );
IMPLEMENT_WND_FACTORY_NAME( KD3DColorPalette, "gc_color_palette" );

KD3DColorPalette::KD3DColorPalette(void)
{
    for( int i = 0; i < MAX_COLOR_BUTTON; ++i )
    {
        m_apkColor[i] = NULL;

        char szText[128] = {0, };
        sprintf( szText, "paint_color%d", i );
        LINK_CONTROL( szText, m_apkColor[i] );
    }

    m_pkCheck = NULL;
    LINK_CONTROL( "check", m_pkCheck );

    m_iColorIndex   = DEFAULT_COLOR_INDEX;
    m_bButtonsCheck = false;
    m_bEnable       = false;

}

KD3DColorPalette::~KD3DColorPalette(void)
{
}

void KD3DColorPalette::OnCreate( void )
{
    // 캐쉬 아이템이냐? GP 아이템이냐에 따라서 팔레트 색상이 달라진다.
    SiGCTextColorManager()->SetColorNum( CASH_ITEM_COLOR_NUM + 1 );
    if( !g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH ) && 
        !( g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB || g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG || g_kGlobalValue.m_kUserInfo.cIsNetmarblePCRoom == 1 ) )              //GC CLUB 인 경우 추가
    {
        SiGCTextColorManager()->SetColorNum( GP_ITEM_COLOR_NUM + 1 );
        if( !g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_ROOM_NAME_COLORING_GP ) )
        {
            SiGCTextColorManager()->SetColorNum( 0 );
        }
    }

	CheckPaletteButtons( false );
	EnablePaletteButtons( false );
    
}




void KD3DColorPalette::FrameMoveInEnabledState( void )
{
}

void KD3DColorPalette::ActionPerformed( const KActionEvent& event )
{
	if( !m_bButtonsCheck )
		return;

	if( !m_bEnable )
		return;

    switch( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); return;
    }

    if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    for( int i = 0; i < SiGCTextColorManager()->GetMaxColor(); ++i )
    {
        if( event.m_pWnd == m_apkColor[i] )
        {
            SetColorIndex(i);
            m_apkColor[i]->ParentRendeOff();
        }
    }
}

void KD3DColorPalette::SetColorIndex( int iIndex )
{
    m_iColorIndex = iIndex;

    // 인덱스 범위를 넘지는 않나 체크
    if( ( SiGCTextColorManager()->GetMaxColor() <= m_iColorIndex || m_iColorIndex < 0 ) && ( m_iColorIndex != DEFAULT_COLOR_INDEX ) )
    {
        m_iColorIndex = DEFAULT_COLOR_INDEX;
        SpeakToActionListener( KActionEvent( this, ON_NO_PAINT_MASSAGEBOX ) );
    }

    // 버튼들 상태를 세팅한다
    for( int i = 0; i < MAX_COLOR_BUTTON; ++i )
    {
        if( i == iIndex )
            continue;

        bool bEnableButton = m_bEnable && ( SiGCTextColorManager()->GetMaxColor() > i );

        m_apkColor[i]->InitState( true, bEnableButton, this );
        if( bEnableButton )
            m_apkColor[i]->SetWndMode( D3DWM_DEFAULT );
        else
            m_apkColor[i]->SetWndMode( D3DWM_LOCK );
        m_apkColor[i]->SetWindowColor( SiGCTextColorManager()->GetColor(i ) );
    }
    m_apkColor[DEFAULT_COLOR_INDEX]->Lock( false );

    m_apkColor[iIndex]->SetWndMode( D3DWM_SELECT );
    D3DXVECTOR2 vPos = m_apkColor[iIndex]->GetFixedWindowLocalPos();
    vPos.x -= 1.0f;
    vPos.y -= 5.0f;
    m_pkCheck->SetFixedWindowLocalPos( vPos );
}

int KD3DColorPalette::GetColorIndex(void)
{
    if( SiGCTextColorManager()->GetMaxColor() <= m_iColorIndex || m_iColorIndex < 0 )
        return DEFAULT_COLOR_INDEX;

    return m_iColorIndex;
}

D3DCOLOR KD3DColorPalette::GetColor(void)
{
    return SiGCTextColorManager()->GetColor(m_iColorIndex); // 없으면 흰색이다.
}

std::wstring KD3DColorPalette::GetColorString(void)
{
    return SiGCTextColorManager()->GetColorString(m_iColorIndex);
}

bool KD3DColorPalette::IsCashColor(std::wstring strColor)
{
	return SiGCTextColorManager()->IsCashColor(strColor);
}

void KD3DColorPalette::EnablePaletteButtons( bool bEnable )
{
    m_bEnable = bEnable;
    SetColorIndex( DEFAULT_COLOR_INDEX );
}

bool KD3DColorPalette::IsHaveColorPaint()
{ 
	return ( SiGCTextColorManager()->GetMaxColor() > 1 ); 
}


void KD3DColorPalette::SetColorString( std::wstring strColor )
{
	m_iColorIndex = SiGCTextColorManager()->GetColorIndex(strColor);
	if( m_iColorIndex == -1 ) {
		m_iColorIndex = DEFAULT_COLOR_INDEX;
		return;
	}
	SetColorIndex(m_iColorIndex);
}

void KD3DColorPalette::SetColorNum(int iNum)
{
	SiGCTextColorManager()->SetColorNum(iNum);
}
