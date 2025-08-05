#pragma once

// [12/27/2007] breadceo. 
// 광장 오브젝트 관리
// - 기본적인 리스트 관리
// - 리소스 순차 로딩, 로딩 완료시 처리
// 서버 통신
// - 광장 입장, 퇴장 같은 것들

#ifdef _DEBUG
//#define ENABLE_SQUARE_LOGGING
#define TIME_SHARE_LOADING_TEST
#endif
#if defined(PERFORMANCE_TEST)
#define TIME_SHARE_LOADING_TEST
#endif

#include "Packet.h"
#include <KNCSingleton.h>
#include <wtypes.h>
#include "GCStateMachine.h"
#include "SquareEnum.h"
#include "ParticleLib/KTDGParticleSystem.h"
#include "GCEventManager.h"
#include "GCBuffDonationManager.h"

class IGCSquareObj;
class GCSquarePeople;
class KGCSquare2DObj;
class GCSquarePet;
class KGCSquare3DObj;
struct MESH_INFO;

struct KSquareBaseEquip
{
    int                 iCharNum;
    std::vector<int>    vecParts;

    void Init()
    {
        iCharNum = -1;
        vecParts.clear();
    }
};

class GCSquare
{
    DeclareSingleton( GCSquare );

public:
    GCSquare(void);
    virtual ~GCSquare(void);

	// enums
	enum ESquareLayer{
		SL_BEFORE_PEOPLE,
		SL_AFTER_PEOPLE,
	};

	enum{
		MAX_GUILD_SQUARE_NUM = 4,
	};

    // static functions
    static void SquareInit();

    KItemIDVector GetEquipmentForLoadModel( KSquareUserInfo& sInfo_ );

    // functions
    void AddSquarePeople( DWORD dwPeopleUID, const KSquareUserInfo& sInfo_ );
	void AddSquareObject( ESquareLayer eLayer, IGCSquareObj* pObj );
	void AddSquarePeopleReserve( DWORD dwPeopleUID, const KSquareUserInfo& sInfo_ );
	void AddParticle( char* pParticleName, float fPosX, float fPosY, float fPosZ );

    void RemoveSquarePeople( DWORD dwPeopleUID );

    //void ChangePeopleInfo( DWORD dwPeopleUID, const KSquareUserInfo& sInfo_ );
    void SquareLoading();
	bool SquareObjInit();
	bool SquarePeopleInit();
    bool SquarePetInit();
	bool SquareReservePeopleLoading();

	void ClearSquareObj();
	void CloseSquare();
	void ClearResourceOnSquare();

    void FrameMove();
	void FrameMove_People();
	void FrameMove_Object();
    void FrameMove_Effect();
    void FrameMove_Pet();

    void Render();
    void ObjectRender();
	void Render_Peoples();
	void Render_Object( ESquareLayer eLayer );
    void Render_PetAnim();
    void Render_Damage();
    void Render_Effect();
    void Render_EtcParticle();
    void Render_Text();
	void ResetText();	

    void ShadowRender();
    void SparkRender();

    void UpdateSquarePeoplePosition( const std::vector<KPosUID>& vecPosInfo );
    void Connect();
    bool ConnectTRServer();
    void CheckPerformance();
    void SortingbyDistance();
    void PerformanceUp();

    // getter/setter
    bool IsLoadingComplete() const { return m_bLoadingComplete; }
    void LoadingComplete( bool bComplete );
    DWORD GetNextSquarePeopleUID( DWORD dwUID ) const;
    GCSquarePeople* GetSquarePeople( DWORD dwUID ) const;
    GCSquarePeople* GetSquarePeopleByChracterNickName( const std::wstring& strCharNickName_ ) const;
    GCSquarePeople* GetSquareReservePeople( DWORD dwUID ) const;
    std::vector< DWORD > GetUIDVector() const { return m_vecPeopleUID; }
    void SetSquareInfo( const std::vector< KSquareInfo >& vecInfo );
	std::vector< KSquareInfo > GetSquareInfo() const { return m_vecSquareInfo; }
    void SetRelayServer( DWORD dwIP, USHORT usPort )
    {
        m_dwRelayServerIP = dwIP;
        m_usRelayServerPort = usPort;
    }
    DWORD GetRelayServerIP() const { return m_dwRelayServerIP; }
    USHORT GetRelayServerPort() const { return m_usRelayServerPort; }
    std::vector< DWORD > GetNearPeopleUIDVector() const { return m_vecNearPeopleUID; }
    void SetNearPeopleUIDVector( const std::vector< DWORD >& vecUID_ ) { m_vecNearPeopleUID = vecUID_; }
    void SetChannelID( DWORD dwID ) { m_dwPrevChannelID = dwID; }
    DWORD GetChannelID() const { return m_dwPrevChannelID; }
    void SetPrevState( GAME_STATE eState ) { m_ePrevState = eState; }
    GAME_STATE GetPrevState() const { return m_ePrevState; }
    float GetLoadPercent() const;
    void SetSquareID( DWORD dwID ) { m_dwSquareID = dwID; }
    DWORD GetSquareID() const { return m_dwSquareID; }
    void SetFPS( int iFPS ) { m_iCurFPS = iFPS; }
    int GetFPS() const { return m_iCurFPS; }
    void SetJoinedChannel( bool bJoinedChannel ) { m_bPrevJoinedChannel = bJoinedChannel; }
    void SetJoinedGuild( bool bJoinedGuild ) { m_bPrevJoinedGuild = bJoinedGuild; }
    bool GetJoinedChannel() const { return m_bPrevJoinedChannel; }
    bool GetJoinedGuild() const { return m_bPrevJoinedGuild; }
    KSquareInfo GetCurrentSquareInfo();
    size_t GetSize() const { return m_mapPeople.size(); }
    void SetSavedState( const std::list<GAME_STATE>& lState ) { m_lPrevSavedState = lState; }
    std::list<GAME_STATE> GetSavedState() const { return m_lPrevSavedState; }
    void ClearSavedState() { m_lPrevSavedState.clear(); }
    bool SavedStateEmpty() { return m_lPrevSavedState.empty(); }
    GCSquarePeople GetAddSquarePeople();
    void Picking( OUT std::vector< KSquareUserInfo >& vecRet , const int iMouseBtn );
    void RefreshRenderTech();
	void CreateCampFire();
	void CreateFireWorks();
	void DeleteCampFire();
	void DeleteFireWorks();
	void ChangeFireWorksPos();
	void LoadingFireWorks();

	// lua functions
	bool LoadObjectScript();
    void Load3DObjcetScript( void );
    void LoadMeshInfo( KLuaManager& luaMgr, MESH_INFO& meshInfo );

	KGCSquare2DObj*	Create2DObject();
	IGCSquareObj*	GetLocalPeople();
	D3DXVECTOR3*	GetWorldMousePos();
    int             GetGuildLevel();
    void            ShowMessageBox();

    void SetNPCSpeechList( SQUARE_OBJ_SUBTYPE eNPC_Subtype, std::wstring strSpeech );
    void ReloadNPCSpeechList( SQUARE_OBJ_SUBTYPE eNPC_Subtype );
    D3DXVECTOR3     Get3DObjectPos( SQUARE_OBJ_SUBTYPE eNPC_Subtype );
    void SetConnectSkip( bool bSkip ) { m_bConnectToSquare = bSkip; }

    GCSquarePet* GetSquarePetPtr( DWORD dwUID );

	void SetGPCItemID( DWORD dwItemID_, DWORD dwUID_, int iTime_ );
	void RenderGPCItem();

	void SetSquareType( int iType_ ) { m_iSquareType = iType_; }
	int GetSquareType() { return m_iSquareType; }

	void SetGuildSquareIndex( int iIndex_ ) { m_iGuildSquareIndex = iIndex_; }
	int GetGuildSquareIndex() { return m_iGuildSquareIndex; }

private:
    // typedefs
    typedef std::map< DWORD, IGCSquareObj* >::iterator IterPeople;
    typedef std::map< DWORD, IGCSquareObj* >::const_iterator ConstIterPeople;
    std::map< DWORD, IGCSquareObj* > m_mapPeople;

    typedef std::map< DWORD, GCSquarePet* >::iterator IterPet;
    typedef std::map< DWORD, GCSquarePet* >::const_iterator ConstIterPet;
    std::map< DWORD, GCSquarePet* > m_mapPet;

	typedef std::multimap< ESquareLayer, IGCSquareObj* > SquareObjMap;
	std::multimap< ESquareLayer, IGCSquareObj* >	m_mapSquareObject;

    typedef std::vector< std::pair< DWORD, IGCSquareObj* > >::iterator IterReservePeople;
    typedef std::vector< std::pair< DWORD, IGCSquareObj* > >::const_iterator ConstIterReservePeople;
    std::vector< std::pair< DWORD, IGCSquareObj* > > m_vecPeopleReserve;

    typedef std::vector< std::pair< DWORD, GCSquarePet* > >::iterator IterReservePet;
    typedef std::vector< std::pair< DWORD, GCSquarePet* > >::const_iterator ConstIterReservePet;
    std::vector< std::pair< DWORD, GCSquarePet* > > m_vecPetReserve;

    bool m_bLoadingComplete;
    bool m_bSparkLoading;
    bool m_bSoundLoading;
    bool m_bConnectToSquare;
    bool m_bSendReq;
    std::vector< DWORD > m_vecPeopleUID;
    std::vector< KSquareInfo > m_vecSquareInfo;
    DWORD m_dwRelayServerIP;
    USHORT m_usRelayServerPort;
    std::vector< DWORD > m_vecNearPeopleUID;
    DWORD m_dwPrevChannelID;
    GAME_STATE m_ePrevState;
    DWORD m_dwSquareID;
    DWORD m_dwPrevCheckTime;
    DWORD m_dwSumOfFrame;
    DWORD m_dwSumCount;
    std::vector< DWORD > m_vecSortedPeoplebyDistance;
    DWORD m_dwMaxRenderCount;
    int m_iCurFPS;
    bool m_bPrevJoinedGuild;
    bool m_bPrevJoinedChannel;
    std::list<GAME_STATE> m_lPrevSavedState; // 샵이나 장비창을 위함
    typedef std::vector<KGCSquare3DObj*>::iterator Iter3DObject;
    std::vector<KGCSquare3DObj*> m_vec3DObjcet;

	//브라질 축제를 위한 준비작업
	//캠프파이어 파티클
	int		m_iResetTime;
	bool	m_bCreateCampFire;
	bool	m_bCreateFireWorks;						//
		
	KSafeArray<KSafeArray<CParticleEventSeqPTR,4>,2> m_pCampFireParticle;
	
    //불꽃놀이 파티클
    std::vector< CParticleEventSeqPTR > m_vecFireWorksBigParticle1;
    std::vector< CParticleEventSeqPTR > m_vecFireWorksBigParticle2;
    std::vector< CParticleEventSeqPTR > m_vecFireWorksMiddleParticle;
    std::vector< CParticleEventSeqPTR > m_vecFireWorksSmallParticle;
    std::vector< CParticleEventSeqPTR > m_vecFireWorksSmallBgParticle;
    std::vector< CParticleEventSeqPTR > m_vecFireWorksMiddleBgParticle;

	//gp캡슐
	GCDeviceTexture* m_pTextureGPCItem;
	D3DXVECTOR3	m_vGPCPlayerPos;
	int m_iGPCRenderTime;
	int m_iSquareType;
	int m_iGuildSquareIndex;

    void SendEventBannerType();
    int m_iEventBannerType;

public:
    void SetEventBannerType(int iEventBannerType_) { m_iEventBannerType = iEventBannerType_; }
    void LoadSquareBaseEquipScript();
    std::vector<KSquareBaseEquip> GetSquareBaseEquipContainer();
    void SendGwcBoardReq();
    void Send_NewYearDonationInfoReq( IN const DWORD dwType_ );
public:
        //광장 기본 복장들 ID값 담는 컨테이너
        std::vector<KSquareBaseEquip> m_vecSquareBaseEquip;
};

DefSingletonInline( GCSquare );
