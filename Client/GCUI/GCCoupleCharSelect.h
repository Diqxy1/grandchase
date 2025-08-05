#pragma once
#include "GCEnum.h"

class KGCCoupleCharSelect : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleCharSelect );

	KGCCoupleCharSelect(void);
	virtual ~KGCCoupleCharSelect(void);
	/// Copy constructor
	KGCCoupleCharSelect( const KGCCoupleCharSelect& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleCharSelect& operator=( const KGCCoupleCharSelect& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GCCCS_CHAR_BTN_CLICK	= 200001,
	};

public:
	virtual void OnCreate( void );
	//virtual void FrameMoveInEnabledState( void );
	//virtual void PostDraw( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void ExcludeCharacter( int iCharType );
	void IncludeCharacter( int iCharType );
	void Update();

private:
	void FixCharacterRange( int& iCharType );
	void InitCharacterSet();
	void InitPosition();
	void OnClickCharBtn( int i );
	void OnClickUpDownBtn( bool bUp );

private:
	bool		m_bFirst;
	int			m_iCurIndex;
	KSafeArray<D3DXVECTOR2,3> m_vPos;

	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkChar;
	KD3DWnd*	m_pkBtnUp;
	KD3DWnd*	m_pkBtnDown;
	std::set<int> m_setCharacters;
};

DEFINE_WND_FACTORY( KGCCoupleCharSelect );
DECLARE_WND_FACTORY( KGCCoupleCharSelect );
DECLARE_WND_FACTORY_NAME( KGCCoupleCharSelect );