#pragma once

class KD3DWnd;
class KGCParticleUI : public KD3DWnd

{
public:
    DECLARE_CLASSNAME( KGCParticleUI );
    /// Default constructor
    KGCParticleUI( void );
    /// Default destructor
    virtual ~KGCParticleUI( void );
    /// Copy constructor
    KGCParticleUI( const KGCParticleUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCParticleUI& operator=( const KGCParticleUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
private:
    static int ms_iLayer;
    static int GetOwnLayer();

private:
    int m_iLayer;
    std::list<CParticleEventSeqPTR> m_listParticles;

public:
    void BindToLua();
    void PostChildDraw();
    void FrameMoveInEnabledState();
    void AddParticle( const char* strName, float fOffsetX, float fOffsetY );
    void OnDestroy();
};

DEFINE_WND_FACTORY( KGCParticleUI );
DECLARE_WND_FACTORY( KGCParticleUI );
DECLARE_WND_FACTORY_NAME( KGCParticleUI );
