#pragma once
#include "GCAgitObjectAction.h"

class KGCAgitDiceAction :
    public KGCAgitObjectAction
{
public:
    KGCAgitDiceAction(void);
    ~KGCAgitDiceAction(void);

    virtual void Create( void );
    virtual void Destroy( void );
    virtual void Enable( void ) {}
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    virtual void Render( void ) {}
    
    virtual void DoAction( const std::vector<DWORD>& vecParam );

    virtual void EnableLive( bool bLive );

    void SetEffectName( const std::vector<std::string>& vecEffect );
    void SetSound( const int _nSound ) { m_nSound = _nSound; }

    void SetEffctNum( const int _nBasicEffectNum ) { m_nBasicEffectNum = _nBasicEffectNum; }

    void SetBasicEffect();
    void ResetBasicEffect();
    

private:

    virtual GCCollisionRect<float> GetCollisionRect( void );

    std::vector<std::string>    m_vecEffect;

    int                         m_nSound;
    int                         m_nDiceNum1;
    int                         m_nDiceNum2;

    float                       m_fDiceTime;
    bool                        m_bActionDice;

    int                         m_nBasicEffectNum;

    std::vector<CParticleEventSeqPTR>     m_vecBasicParticle;

};
