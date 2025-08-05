#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"

class KD3DStatic;

class KGCMinigameRanker : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCMinigameRanker );
	/// Default constructor
	KGCMinigameRanker( void );
	/// Default destructor
	virtual ~KGCMinigameRanker( void );
	/// Copy constructor
	KGCMinigameRanker( const KGCMinigameRanker& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMinigameRanker& operator=( const KGCMinigameRanker& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetNickName( const std::wstring& strNick ) { m_pNickname->SetText( strNick ); }
    std::wstring GetNickName( void ) const { return m_pNickname->GetText(); }
    void SetScore( const int iScore );

protected:
    virtual void OnCreate( void );

private:
    KD3DStatic*         m_pNickname;
    KD3DStatic*         m_pScore;
};

DEFINE_WND_FACTORY( KGCMinigameRanker );
DECLARE_WND_FACTORY( KGCMinigameRanker );
DECLARE_WND_FACTORY_NAME( KGCMinigameRanker );