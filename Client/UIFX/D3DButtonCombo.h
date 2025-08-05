#ifndef _KD3DButtonCombo_H_
#define _KD3DButtonCombo_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
#define MAX_COMBOBUTTON_NUM 20
class KD3DButtonCombo : public KD3DWnd,           // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KD3DButtonCombo );
	/// Default constructor
	KD3DButtonCombo( void );
	/// Default destructor
	virtual ~KD3DButtonCombo( void );
	/// Copy constructor
	KD3DButtonCombo( const KD3DButtonCombo& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DButtonCombo& operator=( const KD3DButtonCombo& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );

	std::vector<KD3DWnd*> m_vecComponent;

public:
	void ToggleRender( bool bRender , unsigned int iComponent );
	void Lock( bool bLock, unsigned int iComponent );
	void SetData( DWORD dwData, unsigned int iComponent );
	DWORD GetData( unsigned int iComponent ) const;
	D3DXVECTOR2 GetCurrentBlockWindowPos( unsigned int iComponent ) const;

	KD3DWnd::ED3DWndMode GetWndMode( unsigned int iComponent ) const;

	KD3DWnd* GetChild( UINT index );
};

DEFINE_WND_FACTORY( KD3DButtonCombo );
DECLARE_WND_FACTORY( KD3DButtonCombo );
DECLARE_WND_FACTORY_NAME( KD3DButtonCombo );

#endif // _KD3DButtonCombo_H_

