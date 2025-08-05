#if !defined(AFX_COLORSTATIC_H__F35D88B3_A7BA_46D1_8FFF_AA0E973D9CC7__INCLUDED_)
#define AFX_COLORSTATIC_H__F35D88B3_A7BA_46D1_8FFF_AA0E973D9CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//
#include <string>

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();
    virtual ~CColorStatic();
    void SetTextColor ( COLORREF kTextColor_ );
	void CalcRect( CRect &rt );
	void CalcRect( CWnd* pParent_ );
	void SetWindowText( std::wstring str_ ){ m_str = str_; }
    void DrawText( CDC* pkMemDC_, CFont* pkFont_, UINT uiAlign_ = DT_CENTER );
// Attributes
public:
    COLORREF        m_kTextColor;
    RECT            m_kRect;
    RECT            m_kClientRect;
    int             m_iWidth;
    int             m_iHeight;
    std::wstring    m_str;
};
#endif // !defined(AFX_COLORSTATIC_H__F35D88B3_A7BA_46D1_8FFF_AA0E973D9CC7__INCLUDED_)
