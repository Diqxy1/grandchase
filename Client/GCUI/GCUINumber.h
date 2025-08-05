#pragma once

//class D3DUIManager;
class KD3DWnd;
class KGCUINumber : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCUINumber );
	/// Default constructor
	KGCUINumber( void );
	/// Default destructor
	virtual ~KGCUINumber( void );
	/// Copy constructor
	KGCUINumber( const KGCUINumber& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCUINumber& operator=( const KGCUINumber& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void PostChildDraw( void );

    void SetStringType( std::string strType, bool bShadow = true );
    
	void SetNumber( int iNumber );
    int GetNumber() const { return m_iNumber; }
    
	void SetWriteToRight( bool bEnable );
	void SetWriteAlign( EGCAlignType eAlignType);
    
	bool GetShadow( );
    void SetShadow( bool bEnable );
    
	bool IsSelcted( );
    void SetSelcted( bool bSelecte );
    
	void SetPrecision( const int iPrecision_ ) { m_iPrecision = iPrecision_; }
    int  GetPrecision( void ) { return m_iPrecision; }
	
	void SetInterval( const float fInterval) { m_fInterval = fInterval;}
	float GetInterval( void ) { return m_fInterval; }

	void SetScale( const float fScale) { m_fScale = fScale;}
	float GetScale( void ) { return m_fScale; }
	
	void CalculatePos();
	int GetNumWidth();

public:
	bool m_bMinus;
    bool m_bWriteRight;
    bool m_bAnim;
	int	m_iNumber;
    int m_iTargetNumber;
	std::string	m_strType;
    bool m_bShadow;
    bool m_bSelected;
    int  m_iPrecision;
	int  m_iNumSize;
	float m_fInterval;
	float m_fScale;
	D3DXVECTOR2 m_vStandardPos;
	EGCAlignType m_eAlignType;

};

DEFINE_WND_FACTORY( KGCUINumber );
DECLARE_WND_FACTORY( KGCUINumber );
DECLARE_WND_FACTORY_NAME( KGCUINumber );
