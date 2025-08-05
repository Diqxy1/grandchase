#pragma once
#include "gcgameobject.h"

class CParticleEventSeqPTR;

class KGCJungSu :
    public KGCGameObject
{
public:
    KGCJungSu(void);
    virtual ~KGCJungSu(void);

public:
    virtual void Create( void );
    virtual void Destroy( void );
    virtual void Enable( void );
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    virtual void Render( void );

    virtual bool IsCrashCheck(void);	
    virtual GCCollisionRect<float> GetCollisionRect( void );

    GCDeviceTexture* m_pTexBase;
    DWORD m_JungSuItemID;
    int m_JungSuType;
    int m_CoolTime;
    int m_RemainCoolTime;

	std::vector< std::string > m_vecParticleStart;
    std::vector< CParticleEventSeqPTR > m_vecParticleActive;
    std::vector< CParticleEventSeqPTR > m_vecParticleCoolTime;
    std::vector< CParticleEventSeqPTR > m_vecParticleHover;

    virtual void Activate(int CharIndex);
    virtual void ResetCoolTime();
    virtual void ActiveAction(int CharIndex){};

    virtual void EnableLive( bool bLive );
};

class KGCJungSuCirculation : public KGCJungSu
{
public:
    KGCJungSuCirculation(void);
    virtual ~KGCJungSuCirculation(void);
    virtual void Create( void );
    virtual void ActiveAction(int CharIndex);

public:
};

class KGCJungSuInferno : public KGCJungSu
{
public:
    KGCJungSuInferno(void);
    virtual ~KGCJungSuInferno(void);
    virtual void Create( void );
    virtual void ActiveAction(int CharIndex);

public:
};

class KGCJungSuControl : public KGCJungSu
{
public:
    KGCJungSuControl(void);
    virtual ~KGCJungSuControl(void);
    virtual void Create( void );
    virtual void ActiveAction(int CharIndex);

public:
};

class KGCJungSuDestruction : public KGCJungSu
{
public:
    KGCJungSuDestruction(void);
    virtual ~KGCJungSuDestruction(void);
    virtual void Create( void );
    virtual void ActiveAction(int CharIndex);

public:
};

class KGCJungSuResurrection : public KGCJungSu
{
public:
    KGCJungSuResurrection(void);
    virtual ~KGCJungSuResurrection(void);
    virtual void Create( void );
    virtual void ActiveAction(int CharIndex);

public:
};


