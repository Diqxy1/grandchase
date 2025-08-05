#include "stdafx.h"
#include "GCToolTip.h"
//
//
//
//

IMPLEMENT_CLASSNAME( KGCToolTip );
IMPLEMENT_WND_FACTORY( KGCToolTip );
IMPLEMENT_WND_FACTORY_NAME( KGCToolTip, "gc_tool_tip" );

KGCToolTip::KGCToolTip(void)
{
    m_pkBack = NULL;
    m_pkTitle = NULL;
    m_ParentUID = -1;

    LINK_CONTROL( "resizebox", m_pkBack );
    LINK_CONTROL( "title_static", m_pkTitle );
}

KGCToolTip::~KGCToolTip(void)
{
}

void KGCToolTip::OnCreate()
{
    m_pkTitle->SetText( L"" );
    m_pkTitle->SetAlign( DT_CENTER );
    m_pkTitle->ToggleRender( true );
}

GCDeviceFont* KGCToolTip::GetFontMgr(int iFontSize)
{
    if(SiKGCMultipleLanguages()->GetDefualtFontSize() == m_lFontSize) 
        return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;
    else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == m_lFontSize) 
        return IsDrawFontOutline() ? g_pkFontMgrOutline14_original      : g_pkFontMgr14_original;
    else if(SiKGCMultipleLanguages()->GetLargeFontSize() == m_lFontSize) 
        return IsDrawFontOutline() ? g_pkFontMgrOutline16_original      : g_pkFontMgr16_original;
    else if(SiKGCMultipleLanguages()->GetTitleFontSize() == m_lFontSize) 
        return IsDrawFontOutline() ? g_pkFontMgrOutlineTitle_original   : g_pkFontMgrTitle_original;

    return IsDrawFontOutline() ? g_pkFontMgrOutline_original        : g_pkFontMgr_original;

}


void KGCToolTip::ShowToolTip( KD3DWnd::uid_type WndUID, std::wstring& strText, D3DXVECTOR2 vToolTipOffset, bool bMultiLine /*= false*/, ED3DWndUIAlignType eWndAlignType, ED3DWndToolTipAlignType eAlignType)
{
    KD3DWnd* pParent = g_pkUIMgr->GetWndFromID( WndUID );
    if( pParent == NULL ) {
        ToggleRender( false );
        return;
    }

    if( m_ParentUID == WndUID )
        return;

	if( (int)strText.size() == 1 || strText.empty() )
		return;

    m_ParentUID = WndUID;


	if( bMultiLine )
	{
		SetMultiText( strText );
	}
	else
	{
		SetNormalText( strText );
	}

    SetWindowZOrder(D3DWZ_TOPMOST);

    D3DXVECTOR2 vPos;
    vPos.y = pParent->GetFixedWindowPos().y - m_pkBack->GetHeight() + vToolTipOffset.y;
    
	DWORD dwBackWidth = m_pkBack->GetWidth();	DWORD dwParentWidth =  pParent->GetWidth();
	int iTmp = dwBackWidth-dwParentWidth;

	float x = static_cast<float>( ( iTmp/2.0f ) + vToolTipOffset.x );

    if( eAlignType == ED3DWND_TOOLTIP_ALIGN_LEFT || eAlignType == ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT)
        vPos.x = pParent->GetFixedWindowPos().x - ( x * 2.f );
    else if( eAlignType == ED3DWND_TOOLTIP_ALIGN_RIGHT )
        vPos.x = pParent->GetFixedWindowPos().x;
	else if( eAlignType == ED3DWND_TOOLTIP_POS_LEFT_TEXT_ALIGN_LEFT)
		vPos.x = pParent->GetFixedWindowPos().x + vToolTipOffset.x;
    else
        vPos.x = pParent->GetFixedWindowPos().x - x;

    if ( vPos.x < 0.0f ) 
        vPos.x = 0.0f;

    if ( vPos.y < 0.0f )
        vPos.y = 0.0f;

    SetWindowPosDirect( vPos );
    ToggleRender( true );
	SetAlignType(eWndAlignType);   

	if( eAlignType == ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT || eAlignType == ED3DWND_TOOLTIP_POS_LEFT_TEXT_ALIGN_LEFT){
		SetTextAlign(DT_LEFT);
		m_pkTitle->SetWindowPosX(6.0f);
	}
	else{
		SetTextAlign(DT_CENTER);
		m_pkTitle->SetWindowPosX(1.0f);
	}
}

void KGCToolTip::SetNormalText( const std::wstring& strText )
{
	int iWidth = GetFontMgr()->GetWidth( strText.c_str() ) + 14;

	m_pkTitle->SetWidth( iWidth );
	m_pkBack->SetSize( iWidth, GetHeight() );

	m_pkTitle->SetText( strText );
    m_pkTitle->SetFontOutline( true, 0xFF000000 );
}

void KGCToolTip::SetTextAlign( DWORD alignType )
{
	m_pkTitle->SetAlign( alignType );
}

void KGCToolTip::SetMultiText( const std::wstring& strText )
{
	int iWidth = 0;

	int iRowCnt = 1;			// 기본 열 1행 입니다.
	int iTempWidth = 0;
	for(int i=0;i<(int)strText.length();++i)
	{
		WCHAR wszTemp = *(strText.begin()+i);
		iTempWidth += GetFontMgr()->GetWidth( wszTemp );
		if( L'\n' == wszTemp )
		{
			if( iWidth < iTempWidth )
				iWidth = iTempWidth;

			iTempWidth = 0;
			iRowCnt++;
		}
	}
	if( iWidth < iTempWidth )
		iWidth = iTempWidth;
	iWidth += GetFontMgr()->GetFontSize();

    int iheightCorrection = 7;

	m_pkTitle->SetWidth( iWidth );
	m_pkBack->SetSize( iWidth, (GetFontMgr()->GetFontSize() * iRowCnt) + ( iheightCorrection * iRowCnt ) + 11 );

	m_pkTitle->SetLineSpace( 1.2f );
	m_pkTitle->SetTextAutoMultiline( strText );

}

GCDeviceFont* KGCToolTip::GetFontMgr(void)
{
	if( !m_pkTitle )
		return NULL;

	return GetFontMgr(m_pkTitle->GetFontSize());
}

void KGCToolTip::FrameMove()
{
    if( m_ParentUID == -1 ) {
        ToggleRender( false );
        return;
    }

    KD3DWnd* pParent = g_pkUIMgr->GetWndFromID( m_ParentUID );

    if( pParent != NULL  )
    {
        POINT       pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x / m_fWindowScaleX, (float)pt.y / m_fWindowScaleY);
        bool bCheckBound = pParent->CheckPosInWindowBound( vMousePos );

        // 툴팁이 떠 있는 상태에서 마우스가 Parent의 범위를 벗어났다면?
        if( !bCheckBound || !pParent->IsRenderOn() )
        {
            // 툴팁 보이지 않게 한다!
            ToggleRender( false );
            m_ParentUID = -1;
        }
    }
	else
	{
		ToggleRender( false );
	}
}
