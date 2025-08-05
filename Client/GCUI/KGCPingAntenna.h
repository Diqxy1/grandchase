#pragma once

class KGCPingAntenna :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCPingAntenna );
    /// Default constructor
    KGCPingAntenna( void );
    /// Default destructor
    virtual ~KGCPingAntenna( void );
    /// Copy constructor
    KGCPingAntenna( const KGCPingAntenna& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPingAntenna& operator=( const KGCPingAntenna& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    struct SPingStateClass{
        DWORD dwVeryBest;
        DWORD dwBest;
        DWORD dwGood;
        DWORD dwNormal;
        DWORD dwBad;
        DWORD dwVeryBad;
    };

public:
    void SetPingState( DWORD iPing_ = 0 );
    void SetPingStateClass();

    DWORD m_dwOriginalWidth;
    SPingStateClass m_sPingStateClass;

protected:

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
private:
    D3DXVECTOR4 m_vAntennaCoords;
    KD3DWnd*    m_pkAntenna;
    KD3DWnd*    m_pkWorstPing;
};

DEFINE_WND_FACTORY( KGCPingAntenna );
DECLARE_WND_FACTORY( KGCPingAntenna );
DECLARE_WND_FACTORY_NAME( KGCPingAntenna );