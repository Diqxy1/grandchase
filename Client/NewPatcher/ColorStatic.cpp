// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
    m_kTextColor = RGB (0, 0, 0);
}

CColorStatic::~CColorStatic()
{
}

void CColorStatic::SetTextColor ( COLORREF kTextColor_ )
{
    m_kTextColor = kTextColor_;
}

void CColorStatic::CalcRect( CWnd* pParent_ )
{
#if defined( _CHINA ) || defined(_CHINA_TEST)
    pParent_->GetClientRect(&m_kRect);    
    this->GetClientRect(&m_kClientRect);    
    this->ClientToScreen(&m_kRect);
    this->ScreenToClient(&m_kRect);
    m_iWidth = m_kRect.right - m_kRect.left;
    m_iHeight = m_kRect.bottom - m_kRect.top;
#else
    this->GetClientRect(&m_kRect);    
    this->GetClientRect(&m_kClientRect);    
    this->ClientToScreen(&m_kRect);
    pParent_->ScreenToClient(&m_kRect);
    m_iWidth = m_kRect.right - m_kRect.left;
    m_iHeight = m_kRect.bottom - m_kRect.top;
#endif
}

void CColorStatic::CalcRect( CRect &rt )
{
	m_iWidth = rt.right - rt.left;
	m_iHeight = rt.bottom - rt.top;
	m_kRect = rt;
}


void CColorStatic::DrawText( CDC* pkMemDC_, CFont* pkFont_, UINT uiAlign_ )
{   
	pkMemDC_->SetBkMode(TRANSPARENT);
	CFont * pOldFont =  pkMemDC_->SelectObject(pkFont_);
	pkMemDC_->SetTextColor( m_kTextColor );    
	pkMemDC_->DrawTextW(m_str.c_str(), m_str.length(), &m_kRect, uiAlign_);
	pkMemDC_->SelectObject(pOldFont);
}