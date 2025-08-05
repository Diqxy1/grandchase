#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

//#include "../uifx/D3DStatic.h"
//#include "GCCharacterUI.h"
//#include "UserPacket.h"

class KGCCharacterUI;

class KGCIndigoRankingElement: public KD3DWnd        // extends
{
public:
	DECLARE_CLASSNAME( KGCIndigoRankingElement );
	/// Default constructor
	KGCIndigoRankingElement( void );
	/// Default destructor
	virtual ~KGCIndigoRankingElement( void );
	/// Copy constructor
	KGCIndigoRankingElement( const KGCIndigoRankingElement& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCIndigoRankingElement& operator=( const KGCIndigoRankingElement& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void SetElementData( int iRank, const std::wstring& strNickName, int iCharType, const std::wstring& strData);
	void SetElementData( const KIndigoRankInfo& kRankInfo );
	void SetEmpty();
protected:
	virtual void OnCreate( void );

protected:
	KD3DStatic*		m_pkRank;
	KD3DStatic*		m_pkNickName;
	KD3DStatic*		m_pkIndigoData;
	KGCCharacterUI*	m_pkCharacter;
};

DEFINE_WND_FACTORY( KGCIndigoRankingElement );
DECLARE_WND_FACTORY( KGCIndigoRankingElement );
DECLARE_WND_FACTORY_NAME( KGCIndigoRankingElement );
