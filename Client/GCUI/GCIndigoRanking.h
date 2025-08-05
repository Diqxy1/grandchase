#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

//#include "GCIndigoRankingElement.h"

class KGCIndigoRankingElement;

class KGCIndigoRanking: public KD3DWnd,        // extends
						public KActionListener	// Listener
{
public:
	DECLARE_CLASSNAME( KGCIndigoRanking );
	/// Default constructor
	KGCIndigoRanking( void );
	/// Default destructor
	virtual ~KGCIndigoRanking( void );
	/// Copy constructor
	KGCIndigoRanking( const KGCIndigoRanking& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCIndigoRanking& operator=( const KGCIndigoRanking& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GC_INDIGO_RANKING_NUM = 10,
	};
public:
	void UpdateRankingData();

protected:
	virtual void OnCreate();
	virtual void ActionPerformed(const KActionEvent& event);

	void UpdateRankingData( int iPage );

	void OnPageRight( DWORD dwCode );
	void OnPageLeft( DWORD dwCode );
	void UpdatePageStatic( int iPage , int iTotalPage );

protected:
	KD3DWnd*	m_pkPageRight;
	KD3DWnd*	m_pkPageLeft;
	KD3DStatic*	m_pkPageStatic;
	KSafeArray<KGCIndigoRankingElement*,GC_INDIGO_RANKING_NUM> m_pkRankingElement;

	int		m_iPage;
	int		m_iTotalPage;
};

DEFINE_WND_FACTORY( KGCIndigoRanking );
DECLARE_WND_FACTORY( KGCIndigoRanking );
DECLARE_WND_FACTORY_NAME( KGCIndigoRanking );
