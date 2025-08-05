#pragma once

#include "KNCSingleton.h"
class KLuaManager;

class KGCObjectEgo;
struct KGCObjectEgoState;

class KGCObjectEgoMgr : public KGCObj
{
	DeclareSingleton( KGCObjectEgoMgr );
public:
	KGCObjectEgoMgr();
	~KGCObjectEgoMgr();
	void LoadFromLuaScript( char* strFilename_ );
	std::list<KGCObjectEgo*> GetObjectEgo( DWORD dwGoodsID );

public:
    void ClearObjectStateInfo();
    void AddStateInfo( const std::string& strUniqueID, std::string strState );
    const std::string GetStateInfo( const std::string& strUniqueID );
    bool IsEgoItem( GCITEMID itemID );

private:
    std::map< DWORD, std::list<KGCObjectEgo*> > m_mapObjectEgo;
    std::map< std::string, std::string > m_mapObjectStateInfo;
};

DefSingletonInline( KGCObjectEgoMgr );

struct KGCObjectEgoState
{
    int m_iSoundID;
    int m_iRepeat;
    int m_iDamageID;
    std::string m_strID;
    std::string m_strAfterState;
    std::string m_strMotion;
    KGCObjectEgoState() 
        : m_iSoundID(-1)
        , m_iDamageID(-1)
        , m_iRepeat(-1)
    {}
};

struct KGCObjectEgoCondition
{
    int m_iLparam;
    int m_iWparam;
    int m_iConditionID;
    DWORD m_dwUniqueID;
    std::string m_strPossibleID;
    std::string m_strMoveID;
};

struct KGCObjectEgoModel
{
    int m_iBoneIdx;
    DWORD m_dwUniqueID;
    std::string m_strP3M;
    std::string m_strTex;
};

struct KGCEgoItemInfo
{
    int m_bCheckCondition;
    int m_iConditionID;
    DWORD m_dwUModelniqueID;
    std::string m_strUniqueID;
    KGCObjectEgo *m_pEgoObj;
};

class KGCObjectEgo
{
public:
    KGCObjectEgo(void){};
    virtual ~KGCObjectEgo(void){};

    enum ECondition {
        E_MOTION_START = 0,
        E_COMMON_MOTION,
        E_GAME_RANDOM,
        E_SHOWROOM_RANDOM,
    };

public:
	void Load( KLuaManager* pLuaMgr_, DWORD dwUniqueID );
    void AddMesh( GCObject* pObj, GCITEMID gcItemID, int iPlayerIdx, std::vector< KGCEgoItemInfo >* pmapObj );

public:
    const KGCObjectEgoState* GetEgoStateFromID( std::string strID );
    const KGCObjectEgoState* GetEgoState( std::string strCurState, PLAYER *pPlayer, USHORT ucFrame, OUT int& iConditionIdx );
    const KGCObjectEgoState* GetEgoStateNotCheckCondition( int iConditionIdx );

private:
    static int ms_iUniqueID;
    std::string m_strName;
    std::string m_strDefaultStateID;
    std::vector<KGCObjectEgoCondition> m_vecCondition;	
    std::vector<KGCObjectEgoState> m_vecState;
    std::vector<KGCObjectEgoModel> m_vecModels;

    bool CheckEgoCondition( KGCObjectEgoCondition& condition, std::string strCurState, PLAYER *pPlayer, USHORT ucFrame );
};