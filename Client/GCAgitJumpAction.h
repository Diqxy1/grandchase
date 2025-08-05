#pragma once
#include "GCAgitObjectAction.h"

class KGCAgitJumpAction :
    public KGCAgitObjectAction
{
public:
    KGCAgitJumpAction(void);
    ~KGCAgitJumpAction(void);

    virtual void Create( void );
    virtual void Destroy( void );
    virtual void Enable( void ) {}
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    virtual void Render( void ) {}
    virtual void DoAction( const std::vector<DWORD>& vecParam ) {}
    virtual void EnableLive( bool bLive );

    void SetJump( float fJumpX, float fJumpY ) { m_fJumpX = fJumpX; m_fJumpY = fJumpY; }
    void SetEffectName( const std::vector<std::string>& vecEffect );
    void SetSound( const int _nSound ) { m_nSound = _nSound; }
    void SetEffctNum( const int _nBasicEffectNum ) { m_nBasicEffectNum = _nBasicEffectNum; }

    void SetBasicEffect();
    void ResetBasicEffect();

private:

    virtual GCCollisionRect<float> GetCollisionRect( void );
    GCCollisionRect<float> GetPlayerCollisionRect( int i );
    
    std::vector<std::string>    m_vecEffect;

    float				    	m_fJumpX;
    float						m_fJumpY;

    int                         m_nSound;
    int                         m_nBasicEffectNum;

    std::vector<CParticleEventSeqPTR>     m_vecBasicParticle;

};
