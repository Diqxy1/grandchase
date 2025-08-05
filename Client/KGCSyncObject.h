/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 04월 22일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 오브젝트 싱크 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

/*
    < 클래스 설계이유 >
        던전 개발 중 충돌가능한 오브젝트 개체처리가 필요해서
        데미지 시스템을 이용해서 개발을 했습니다.
        그런데 
        데미지는 로컬계산을 하고있기 때문에 네트워크 싱크가 100% 정확하지 않았습니다.
        또 데미지는 같은 ID의 데미지라도 여러 개의 인스턴스를 생성할 수 있지만 UID가 없기 때문에
        네트워크 플레이어들과 인스턴스에 대한 구분을 할 수 없어
        방장이 어떤 데미지 인스턴스를 지워라고 명령할 수 없습니다.

    < 클래스 역할 >
        그래서 싱크오브젝트 클래스를 만들었는데
        이 싱크 오브젝트 클래스는 SyncObjectInfo.lua에 명시된 객체의 모양대로(파티클이나 2D오브젝트, 메쉬, 등등.. ) 오브젝트를 생성하고, 배치해주고,
        이 오브젝트가 생성될때 인스턴스별 UID를 기록하고 있습니다.
        또한 생성 / 삭제시 옵션에 따라 패킷을 보내어 네트워크 플레이어들과 오브젝트 상태를 
        100% 똑같게 관리할 수 있도록 하는것이 이 클래스의 역할 입니다.

    < 주의사항 >
        1. 이 오브젝트에 이동싱크 기능은 작업하지 않았습니다.
           만약 이동싱크가 필요하다면 레이턴시를 활용하는 방법을 사용합시다.

        2. 패킷을 보낸 후 생성/제거하기 때문에 오브젝트가 생성/제거되기까지 
           약간의 딜타임이 있습니다.
           방장도 보낸 패킷을 받았을때 생성합니다.( 딜타임을 최대한 비슷하게 맞추기 위해 )
*/

#ifndef _KGCSYNCOBJECT_H_
#define _KGCSYNCOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stage.h"
#include "KNCSingleton.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 싱크오브젝트에서 사용하는 파티클 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SyncParticleInfo
{
    std::string  m_strName;
    float        m_fXOffset;
    float        m_fYOffset;
    float        m_fTraceTime;
    SyncParticleInfo() : m_strName(""), m_fXOffset(0.0f), m_fYOffset(0.0f), m_fTraceTime(0.5f){};
    void Init()
    {
        m_strName.clear();
        m_fXOffset = 0.0f;
        m_fYOffset = 0.0f;
        m_fTraceTime = 0.5f;
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 싱크오브젝트 속성 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////
class SyncObjectOriginal
{
public:
    SyncObjectOriginal() {  Init(); }
    SyncObjectOriginal( const SyncObjectOriginal &sSyncObject )
    {
        m_iObjectID                     = sSyncObject.m_iObjectID;
        m_iChange_Life                  = sSyncObject.m_iChange_Life;
        m_iStart_Life                   = sSyncObject.m_iStart_Life;
        m_iPlayerMagicEff               = sSyncObject.m_iPlayerMagicEff;
        m_iMonsterMagicEff              = sSyncObject.m_iMonsterMagicEff;
        m_iMagicEffLevel                = sSyncObject.m_iMagicEffLevel;
        m_iNoCheckGroundFrame           = sSyncObject.m_iNoCheckGroundFrame;
        m_fMagicEffTime                 = sSyncObject.m_fMagicEffTime;
        m_fOffsetX                      = sSyncObject.m_fOffsetX;
        m_fOffsetY                      = sSyncObject.m_fOffsetY;
        m_fHalfWidth                    = sSyncObject.m_fHalfWidth;
        m_fHalfHeight                   = sSyncObject.m_fHalfHeight;
        m_fGravity                      = sSyncObject.m_fGravity;
        m_fGroundOffsetX                = sSyncObject.m_fGroundOffsetX;
        m_fGroundOffsetY                = sSyncObject.m_fGroundOffsetY;
        m_bCheckGround                  = sSyncObject.m_bCheckGround;
        m_bNoCrashPlayer                = sSyncObject.m_bNoCrashPlayer;
        m_bNoCrashMonster               = sSyncObject.m_bNoCrashMonster;
        m_bMonsterTargettingRelease     = sSyncObject.m_bMonsterTargettingRelease;
        m_bClearCrash                   = sSyncObject.m_bClearCrash;
        m_bOnlyOneCrash                 = sSyncObject.m_bOnlyOneCrash;
        m_bTraceParticle                = sSyncObject.m_bTraceParticle;
        m_bNoCrashCharSummonMonster     = sSyncObject.m_bNoCrashCharSummonMonster;
        m_vecNoCrashBuff                = sSyncObject.m_vecNoCrashBuff;
        m_vecNoCrashMonsterType         = sSyncObject.m_vecNoCrashMonsterType;
        m_vecParticle                   = sSyncObject.m_vecParticle;
    }
    ~SyncObjectOriginal(){}

public:
    inline void Init()
    {
        m_iObjectID = 0;
        m_iChange_Life = -1;
        m_iStart_Life = 0;
        m_iPlayerMagicEff = -1;
        m_iMonsterMagicEff = -1;
        m_iMagicEffLevel = 0;
        m_iNoCheckGroundFrame = 0;
        m_fMagicEffTime = 0.0f;
        m_fOffsetX = 0.0f;
        m_fOffsetY = 0.0f;
        m_fHalfWidth = 0.1f;
        m_fHalfHeight = 0.1f;
        m_fGravity = 0.0f;
        m_fGroundOffsetX = 0.0f;
        m_fGroundOffsetY = 0.0f;
        m_bCheckGround = false;
        m_bNoCrashPlayer = false;
        m_bNoCrashMonster = false;
        m_bMonsterTargettingRelease = false;
        m_bClearCrash = false;
        m_bOnlyOneCrash = false;
        m_bTraceParticle = false;
        m_vecNoCrashBuff.clear();
        m_vecNoCrashMonsterType.clear();
        m_vecParticle.clear();
    }

public:
    int                               m_iObjectID;
    int                               m_iChange_Life;
    int                               m_iStart_Life;
    int                               m_iPlayerMagicEff;
    int                               m_iMonsterMagicEff;
    int                               m_iMagicEffLevel;
    int                               m_iNoCheckGroundFrame;
    float                             m_fMagicEffTime;
    float                             m_fOffsetX;
    float                             m_fOffsetY;
    float                             m_fHalfWidth;
    float                             m_fHalfHeight;
    float                             m_fGravity;
    float                             m_fGroundOffsetX;
    float                             m_fGroundOffsetY;
    bool                              m_bCheckGround;
    bool                              m_bNoCrashPlayer;
    bool                              m_bNoCrashMonster;
    bool                              m_bMonsterTargettingRelease;
    bool                              m_bClearCrash;
    bool                              m_bOnlyOneCrash;
    bool                              m_bTraceParticle;
    bool                              m_bNoCrashCharSummonMonster;
    std::vector<int>                  m_vecNoCrashBuff;
    std::vector<int>                  m_vecNoCrashMonsterType;
    std::vector<SyncParticleInfo>     m_vecParticle;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 싱크오브젝트 인스턴스 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////
class SyncObjectInstance
{
public:
    SyncObjectInstance( DWORD dwUID, int iObjectID, float fX, float fY, SyncObjectOriginal* pOriginalObject );
    SyncObjectInstance( const SyncObjectInstance &sObjectInstance );
    ~SyncObjectInstance();

public:
    void CreateParticle();
    void ClearParticle();

public:
    void SettingAttribute();

public:
    DWORD m_dwUID;
    int m_iObjectID;
    int m_iLife;
    int m_iNoCheckGroundFrame;
    bool m_bIsGroundCrash;
    D3DXVECTOR3 m_vPos;
    const SyncObjectOriginal* m_pOriginalObject;
    std::vector<CParticleEventSeqPTR> m_vecParticleSeq;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 싱크오브젝트 매니져 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////
class KGCSyncObjectManager
{
    DeclareSingleton( KGCSyncObjectManager );

public:
    friend void ON_GC_PID_BROAD_SYNCOBJECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

public:
	KGCSyncObjectManager(void);
	~KGCSyncObjectManager(void);

#if !defined( __PATH__ )
    void CheckFileMonitor();
    void RegisterFileMoniter();
#endif

public:
    void BindLua();
    bool LoadScript();

public:
    void InitCalss();
    void DestroyCalss();

private:
    template<typename T>
    void ClearObject_Private( T &mapObject );

public:
    void FrameMove();
    void Render();

private:
    SyncObjectOriginal* GetSyncObjectInfo_Private( int iObjectID );

public:
    DWORD AddSyncObject_BroadCast( int iObjectID, float fX, float fY );
    DWORD DelSyncObject_BroadCast(DWORD dwUID);
    DWORD AddSyncObject_NotMeBroad( int iObjectID, float fX, float fY );
    DWORD DelSyncObject_NotMeCast(DWORD dwUID);

private:
    DWORD AddSyncObject_Packet_Private(DWORD dwUID, int iObjectID, float fX, float fY);
    DWORD DelSyncObject_Packet_Private(DWORD dwUID);

private:
    bool CheckCrashPlayer_Private( SyncObjectInstance *pInstance );
    bool CheckCrashMonster_Private( SyncObjectInstance *pInstance );

public:
    void CheckCrashGround( SyncObjectInstance *pInstance, float fSpeedX, float fSpeedY );
    float GetGravityPos( SyncObjectInstance* pInstance );

public:
    const std::map<DWORD, SyncObjectInstance*>* GetObjectInstancePool() { return &m_mapObjectInstance; }
    int GetInstanceCount() { return int(m_mapObjectInstance.size()); }
    int GetInstanceCountToID( int iID );
    D3DXVECTOR2 GetInstancePos(DWORD dwUID);

public:
    enum SyncPacketMode
    {
        SYNC_MODE_ADD,
        SYNC_MODE_DEL,
    };

private:
    std::map<int, SyncObjectOriginal*>      m_mapObjectInfo;            // 오브젝트 속성( map<오브젝트ID, 속성> )
    std::map<DWORD, SyncObjectInstance*>    m_mapObjectInstance;        // 오브젝트 인스턴스( map<오브젝트UID, 인스턴스> )
    std::map<int, int>                      m_mapObjectInstanceCount;   // 오브젝트 ID별 인스턴스 생성갯수( map<오브젝트ID, 갯수> )

private:
    DWORD m_dwUIDCount;
};

DefSingletonInline( KGCSyncObjectManager );

#endif // _KGCSYNCOBJECT_H_