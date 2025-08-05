/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 11월 15일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 탑승물 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once
#include "KGCEmbarkObject.h"

class KGCEmbarkSettingKey
{
public:
    KGCEmbarkSettingKey()    {   Init();     };
    ~KGCEmbarkSettingKey()   {               };
    bool operator < ( const KGCEmbarkSettingKey& right ) const
    {
        if( iGameMode > right.iGameMode )
            return false;
        return true;
    }

private:
    bool operator == ( const KGCEmbarkSettingKey& right ) const;
    bool operator >= ( const KGCEmbarkSettingKey& right ) const;
    bool operator <= ( const KGCEmbarkSettingKey& right ) const;
    bool operator >  ( const KGCEmbarkSettingKey& right ) const;

public:
    void Init()
    {
        iGameMode = -1;
        iStage = -1;
        iLevel = -1;
    }

public:
    int iGameMode;
    int iStage;
    int iLevel;
};

class KGCEmbarkSettingValue
{
public:
    KGCEmbarkSettingValue()    {   Init();     };
    ~KGCEmbarkSettingValue()   {               };

public:
    void Init()
    {
        iType = 0;
        bIsRight = true;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
    }

public:
    int iType;
    bool bIsRight;
    D3DXVECTOR2 vPos;
};

struct KGCEmbarkSettingKeyFinder
{
private:
    KGCEmbarkSettingKey m_kFindKey;
    KGCEmbarkSettingKeyFinder(){};

public:
    KGCEmbarkSettingKeyFinder( KGCEmbarkSettingKey kFindKey )
    {
        m_kFindKey = kFindKey;
    }
    bool operator()( const std::pair<KGCEmbarkSettingKey, std::vector<KGCEmbarkSettingValue> >& kArg )
    {
        if( m_kFindKey.iGameMode != kArg.first.iGameMode )
            return false;
        if( m_kFindKey.iStage != kArg.first.iStage )
            return false;
        if( m_kFindKey.iLevel != kArg.first.iLevel )
            return false;
        return true;
    }
};

class KGCEmbarkManager
{
    DeclareSingleton( KGCEmbarkManager );
public:
    KGCEmbarkManager(void);
    ~KGCEmbarkManager(void);

#if !defined( __PATH__ )
    void CheckFileMonitor();
    void RegisterFileMoniter( const char *szFileName );
#endif

public:
    void LoadScript();
    void LoadEmbarkTemplate( KLuaManager &luaMgr );
    void LoadActionScript( KLuaManager &luaMgr, KGCEmbarkTemplate &kEmbarkTemplate );

public:
    void RegisterLuaBind();

public:
    void SetCountUID(DWORD dwUID, bool bNoCheckHost = false);
    DWORD GetCountUID()   {   return m_dwUIDCount;    };
    DWORD GetCountUIDWithAutoCount();
    KGCEmbarkObject* CreateEmbarkObject( int iType, DWORD dwUID );

public:// 탑승물 소환 과정 : AddEmbark -> 패킷REQ -> 방장(UIDCounting) -> 패킷ACK -> SummonEmbark()
    void SummonEmbark( KGC_PID_BROAD_EMBARK_SUMMON_ACK *pData );
    void AddEmbark( int iType, float fPosX, float fPosY, bool bIsRight, bool bEmbakation = false, DWORD dwPlayerUID = 0 );

public:
    void DestoryAllInstance( bool bDirectDelete = false );
    void ClearInstance( DWORD dwUID );
    void ClearInstanceByLocal( DWORD dwUID, bool bDirectDelete = false );

public:
    void FrameMove();
    void FrameMove_DeleteList();
    void FrameMove_AddList();

public:
    void StartEmbark();
    void TextRender();
    void CheckStageMoveEmbarkation();

public:
    const KGCEmbarkTemplate* GetEmbarkTemplate( int iType );
    KGCEmbarkObject* GetEmbarkInstance( DWORD dwUID );
    void SetCharMotion( int iMotionID );
    std::vector<int> GetCharMotionList() {  return m_vecCharMotionList; };
    int GetEmbarkationCount();

public:
    void SendEmbarkUIDSync();

private:
    DWORD                               m_dwUIDCount;
    std::map<DWORD, KGCEmbarkObject*>   m_mapEmbarkInstance;
    std::map<DWORD, KGCEmbarkTemplate>  m_mapEmbarkTemplate;
    
private:
    std::vector<DWORD>                  m_vecDeleteList;
    std::vector<KGCEmbarkObject*>       m_vecAddList;
    std::vector<int>                    m_vecCharMotionList;

private:
    int                                 m_iUIDLatencyCnt;
};

DefSingletonInline( KGCEmbarkManager );