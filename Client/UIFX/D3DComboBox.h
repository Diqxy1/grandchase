#ifndef _D3DCOMBOBOX_H_
#define _D3DCOMBOBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

//#include "D3DWnd.h"

class KD3DStatic;

class KD3DComboBox : public KD3DWnd,           // extends
						public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KD3DWnd );
    /// Default constructor
    KD3DComboBox( void );
    /// Default destructor
    virtual ~KD3DComboBox( void );
    /// Copy constructor
    KD3DComboBox( const KD3DComboBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DComboBox& operator=( const KD3DComboBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	void ActionPerformed( const KActionEvent& event );
	void OnCreate( void );
	void FrameMoveInEnabledState( void );
	void OnDestroy( void );
	void PopupHeightChange();

public:
	void SetComboBoxColor(D3DCOLOR currentColor, D3DCOLOR normalColor );
	void DirectActionPerform( const KActionEvent& event ) { ActionPerformed(event); }
	void AddString( std::wstring cName, DWORD dwAlign_ = DT_LEFT );
	void SetComboStatic( std::wstring str, DWORD dwAlign_ = DT_LEFT );
	int  GetCurSel() { return m_iCurSel; }	
	void  SetCurSel(int iSel);
	void DeleteAllItem();
    void SetAlign( DWORD dwAlign_ ){ m_dwAlign = dwAlign_; }
	KD3DWnd*	GetComboPopList() { return m_pkComboPopup; }
    void ShowComboBoxList(bool bShow);
    std::wstring GetCurStaticText(void);


protected:
	KD3DWnd*	m_pkComboMain;
	KD3DWnd*	m_pkComboPopup;
	KD3DStatic*	m_pkStaticCombo;
	D3DCOLOR   m_CurrentColor;
	D3DCOLOR   m_NormalColor;

	int			m_iCurSel;
    DWORD       m_dwAlign;
};

DEFINE_WND_FACTORY( KD3DComboBox );

#endif // _D3DCOMBOBOX_H_
