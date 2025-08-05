#pragma once
#include "GCAgitObjectAction.h"

class KGCAgitFlagAction :
    public KGCAgitObjectAction
{
public:
    KGCAgitFlagAction(void);
    ~KGCAgitFlagAction(void);

    virtual void Create( void );
    virtual void Destroy( void );
    virtual void Enable( void ) {}
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    virtual void Render( void ) {}
    virtual void DoAction( const std::vector<DWORD>& vecParam );

    virtual void EnableLive( bool bLive );

    virtual DWORD GetUserUID() { return m_dwUserID; }


    void UpdateCheckFlag( float fElapsedTime ); //  깃발 소유권 이전 체크
    void UpdateFlag();

    void SetEffectName( const std::vector<std::string>& vecEffect );
    void SetSound( const int _nSound ) { m_nSound = _nSound; }

    void SetFlagEffect();
    void ReSetFlagEffect();

    void SetFlagState( DWORD dwUserID );

private:

    virtual GCCollisionRect<float> GetCollisionRect( void );

    std::vector<std::string>    m_vecEffect;

    int                         m_nSound;
    DWORD                       m_dwUserID;     //  Flag소유한 유저

    float                       m_fFlagTime;
    bool                        m_bActionFlag;

    DWORD                       m_dwTickCount;


    CParticleEventSeqPTR     m_pFlagParticle;

};
