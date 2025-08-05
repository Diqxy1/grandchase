#pragma once

class KGCSystemGateCount : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSystemGateCount );
    /// Default constructor
    KGCSystemGateCount( void );
    /// Default destructor
    virtual ~KGCSystemGateCount( void );
    /// Copy constructor
    KGCSystemGateCount( const KGCSystemGateCount& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSystemGateCount& operator=( const KGCSystemGateCount& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnEndCount( void );
	void OnSetCount( int second );
	void FrameMoveInEnabledState( );
	void SetNumberTex( int index, KD3DWnd *pWnd );
	void SetNumber( void );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
private:
	KD3DStatic *m_pkStaticDesc;
	KD3DStatic *m_pkStaticEsc;

	KD3DWnd *m_pkNumber;
	KD3DWnd *m_pkNumber10;

	int m_iCount;

	int m_ulTime;

};

DEFINE_WND_FACTORY( KGCSystemGateCount );
DECLARE_WND_FACTORY( KGCSystemGateCount );
DECLARE_WND_FACTORY_NAME( KGCSystemGateCount );
