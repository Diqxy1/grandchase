#pragma once

//#include "../uifx/D3DWnd.h"

class KGCUINumber;

class KGCMinigameScoreBoard : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCMinigameScoreBoard );
	/// Default constructor
	KGCMinigameScoreBoard( void );
	/// Default destructor
	virtual ~KGCMinigameScoreBoard( void );
	/// Copy constructor
	KGCMinigameScoreBoard( const KGCMinigameScoreBoard& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMinigameScoreBoard& operator=( const KGCMinigameScoreBoard& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetHighScore( const int iHighScore );
    void SetCurrentScore( const int iCurrentScore );
    void SetGP( const int iGp );
    int GetDigit( const int iValue );
    int GetDigitNumber( const int iValue, const int iOffset );
    void InitData( void );
    void InitNumberTextureCoordinate( void );

protected:
    virtual void OnCreate( void );

private:
    //KSafeArray<KD3DNumber*,4> m_pkHighScore;
    KSafeArray<KD3DNumber*,4> m_pkCurrentScore;
    //KSafeArray<KD3DNumber*,3> m_pkGP;
    KGCUINumber*    m_pkScore;
};

DEFINE_WND_FACTORY( KGCMinigameScoreBoard );
DECLARE_WND_FACTORY( KGCMinigameScoreBoard );
DECLARE_WND_FACTORY_NAME( KGCMinigameScoreBoard );
