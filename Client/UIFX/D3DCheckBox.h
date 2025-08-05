#ifndef _D3DCHECKBOX_H_
#define _D3DCHECKBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"
class KD3DStatic;
class KD3DCheckBox : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DCheckBox );
    /// Default constructor
    KD3DCheckBox( void );
    /// Default destructor
    virtual ~KD3DCheckBox( void );
    /// Copy constructor
    KD3DCheckBox( const KD3DCheckBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DCheckBox& operator=( const KD3DCheckBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	void SetCheck(bool bCheck = true);
	bool GetCheck();
	void ToggleCheck() { SetCheck( !GetCheck() ); }
    void SetStatic( const std::wstring& str );
	void SetStaticColor( D3DCOLOR color);
	void SetTwoStatic( const std::wstring& str, const std::wstring& str_2);
	void ToggleCheckBox( bool render);
    bool IsCheckBackRenderOn( void ) { return m_pWndCheckBox->IsRenderOn(); }
    void SetWordWrap( const std::wstring& str);
    void CheckBoxLock( bool bLock );

protected:
	KD3DWnd*	m_pWndCheck;
	KD3DWnd*	m_pWndCheckBox;
	KD3DStatic* m_pkStatic;
};

DEFINE_WND_FACTORY( KD3DCheckBox );

#endif // _D3DCHECKBOX_H_