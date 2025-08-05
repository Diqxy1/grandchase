#pragma once

// [12/27/2007] breadceo. 
// 광장에 서있는 사람 클래스!

//#include "GCGlobal.h"
#include "IGCSquareObj.h"
#include "GCSquarePeopleFrameInfo.h"
#include "KGCEmoticon.h"
#include "PlayerComp.h"
#include "KGCObj.h"
#include "KGCObjectEgo.h"
#include "../GCObjectAndMotion.h"
#include "GCFaceOffManager.h"


class KGC3DObject;
// [1/16/2008] breadceo. 일단은 벗기기로..
//class GCDeviceTexture;
class GCDeviceMotionFRM;
class KGCItemManager;



#define WorldCoordTOScreenCoord(wx ,wy ,sx ,sy) \
    sx = (int)( ( wx + 1.0f ) * 400.0f + 0.5f );\
    sy = (int)( ( -wy + 0.75f ) * 400.0f + 0.5f );\

struct SQUARE_PEOPLE_ACTIVITY_SAMPLE;
class GCSquarePeopleActivitySensor;
class KGCEmoticonInstance;

struct SQUARE_PEOPLE_TEMPLATE
{
    UINT WAIT;
    UINT WALK;
    UINT JUMP_READY;
    UINT JUMP;
    UINT DASH;

    SQUARE_PEOPLE_TEMPLATE()
    {
        WAIT = MID_ELESIS_WAIT;
        WALK = MID_ELESIS_WALK;
        JUMP_READY = MID_ELESIS_JUMPREADY;
        JUMP = MID_ELESIS_JUMP;
        DASH = MID_ELESIS_DASH;
    }
};

struct LOADING_STATE
{
    bool m_bMotionLoad;
    bool m_bModelLoad;
    bool m_bEffectLoad;
    bool m_bEffectLoad2;
    bool m_bCreateEffect;
    int m_iPrevLoadMotion;
    std::vector< DWORD > m_vecModelID;
    std::vector< UINT > m_vecMotionID;
    std::vector< int > m_vecEffectID;
    int m_iLoadedItemEffIdx;
    DWORD m_dwElapsedTime;

    LOADING_STATE()
    {
        Init();
    }

    void Init()
    {
        m_bMotionLoad = false;
        m_bModelLoad = false;
        m_bEffectLoad = false;
        m_bEffectLoad2 = false;
        m_bCreateEffect = false;
        m_iPrevLoadMotion = 0;
        m_vecModelID.clear();
        m_vecMotionID.clear();
        m_vecEffectID.clear();
        m_iLoadedItemEffIdx = 0;
        m_dwElapsedTime = 0;
        RefreshTime();
    }
    bool IsComplete() const { 
        return m_bMotionLoad && m_bModelLoad 
            && m_bEffectLoad && m_bEffectLoad2 
            && m_bCreateEffect ;
    }
    void RefreshTime() { m_dwElapsedTime = ::timeGetTime(); }
    bool IsTimeOver() const { return ::timeGetTime() - m_dwElapsedTime > (DWORD)100; }
};

class GCSquarePeople : public IGCSquareObj                     
                     , public GCSquarePeopleFrameInfo
{
public:
    enum PEOPLE_STATE
    {
        INVALID_PEOPLE = -1,
        WAIT_PEOPLE,
        WALK_PEOPLE,
        JUMP_READY_PEOPLE,
        JUMP_PEOPLE,
        DASH_PEOPLE,
        DASH_JUMP_PEOPLE,
        SOCIAL_PEOPLE,

        NUM_PEOPLE_STATE,
    };

private:
    GCSquarePeople(void);

public:
    GCSquarePeople( DWORD dwUID );
    virtual ~GCSquarePeople(void);

    virtual void FrameMove();
    virtual void Render();
    virtual bool Load();
    virtual bool TimeShareLoading();
    
    void UpdateFootHoldPosition();
    virtual void ShadowRender();

    virtual bool Picking( const D3DXVECTOR2& vMousePos, const int iMouseBtn );

    // getter / setter
    void SetPeopleState( PEOPLE_STATE eState );
    PEOPLE_STATE GetPeopleState() const { return m_eState; }
    PEOPLE_STATE GetOldPeopleState() const { return m_eOldState; }
    UINT GetMotionID( PEOPLE_STATE eState );
    bool GetContact() const { return m_bIsContact; }
    void SetContact( bool bContact ) { m_bIsContact = bContact; }
    float GetXSpeed() const { return m_fXSpeed; }
    void SetXSpeed( float fSpeed ) { m_fXSpeed = fSpeed; }
    float GetYSpeed() const { return m_fYSpeed; }
    void SetYSpeed( float fSpeed ) { m_fYSpeed = fSpeed; }
    unsigned char GetNoCheckContact() const { return m_cNoCheckContact; }
    void SetNoCheckContact( unsigned char ucContact ) { m_cNoCheckContact = ucContact; }
    DWORD GetUID() const { return m_dwUID; }
    void SetPeopleInfo( const KSquareUserInfo& sInfo_ );
    const KSquareUserInfo& GetPeopleInfo() { return m_kPeopleInfo; }
    GCDeviceMotionFRM* GetMotionPtr() const;
    void Get_BasicMatrix( D3DXMATRIX* pMat, bool bCamera, bool bForNickName );
    void SetFrameLock( bool bFrameLock ) { m_bFrameLock = bFrameLock; }
    bool GetFrameLock() const { return m_bFrameLock; }
    virtual void SetRender( bool bRender );
    bool GetDownJump() const { return m_bDownJump; }
    void SetDownJump(bool val) { m_bDownJump = val; }
    char GetExtraCharNum() const { return Extra_Char_Num; }
    PositionDesc& GetposDesc() { return m_posDesc; }
    PLAYER_TEMPLATE& GetPlayerTemplate() { return m_PlayerTemplate; }
    GCObject* GetGCObject();
    DWORD& GetElemental() { return m_Elemental; }
    int& GetStrongLevel() { return m_StrongLevel; }
    MAP_OVERLAPPED_PAIR& GetOvelappedPair() { return m_mapOverlappedPair; }
    std::vector< ITEM_EFFECT >& GetItemEff() { return m_vecItemEff; }
    // [5/6/2008] breadceo. 호환성을 위해 놔두는거지 광장에서 이거 나오면 -_-;;
    std::vector< ITEM_EFFECT >& GetDamageEff() { return m_vecDamageEff; }
    std::vector< ITEM_EFFECT >& GetSkillEff() { return m_vecSkillEff; }
    std::vector< KGCAnimManager::ItemAnim >& GetItemAnim() { return m_ItemAnim; }
    // [5/6/2008] breadceo. 호환성을 위해 놔두는거지 광장에서 이거 나오면 -_-;;
    std::vector< KGCAnimManager::DamageAnim >& GetDamageAnim() { return m_DamageAnim; }
    std::vector< KGCAnimManager::SkillAnim* >& GetSkillAnim() { return m_SkillAnim; }

    // functions
    void FrameMoveLocalPlayer();
    void FrameMoveRemotePlayer();
    void MovingProcess();
    void MoveXInSky();
    void MoveXInSkyRemote();
    void CheckContact();
    void EffectDust();
    void SquareBoundCheck();
    void SetBasicDataEqual() { m_OldBasicData = m_BasicData; }
    bool IsLocalPlayer();
    void DoLastFrame( bool bFrameOver );
    void DoNextFrame();
    //void DoFrameInfoProc();
    void Jump();
    void DownJump();
    void DashJump();
    void SetJumpDir();
    void EquipBaseItem( std::vector<DWORD>& vecItem );
    void NotifyPositionToServer();
    void SetTemplate();
    void DrawNickName();
	void DrawGCClubMark();
	void DrawPetName(int x,int y);
	void DrawGuildMark();
	void SetPetName(std::wstring str);
	void ResetPetName();

    void CreateObject();
    void MakeModelLoadList();
    void MakeLowModelLoadList();
    void MakeMotionLoadList();
    void MakeEffectLoadList();
    void CheckEquipItemState();

    void LoadItemModel( DWORD dwItemID );
    void LoadETCModel();
    void LoadFaceTexture( GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition );
    std::string GetDefaultMeshName( DWORD dwSlotPosition );
    inline bool IsJumpState()
    {
        if( m_eState == GCSquarePeople::JUMP_PEOPLE || 
            m_eState == GCSquarePeople::DASH_JUMP_PEOPLE )
        {
            return true;
        }

        return false;
    }
    void UpdateBonePosition();
    std::vector< int >& GetPetAiID() { return m_vecPetAiID; }
    void LoadOneEffect( int iItemID );
    void OnLoadingComplete();
	void SetPlayerName();
    void ToggleEffect( bool bRender );
    bool NoForceEffect( EMIT_POSITION_TYPE positionType );
	bool NoForceEffect( std::vector<EMIT_POSITION_TYPE>& positionType );
    // for debug
    void DrawDumpData();
    std::wstring GetPeopleStateStr();
    static std::wstring GetPeopleStateStr( PEOPLE_STATE eState );
    GCSquarePeopleActivitySensor* GetSensor() { return m_pSensor; }
    void DrawBodyRect();

    //for SayBalloon
    void DrawSquarePeople();

    void SetEmoticon( KGCEmoticonInstance* pEmoticonInstance_ );

	CParticleEventSeqPTR AddTraceParticle( char* strSequence, float fOffsetX, float fOffsetY );

protected:
    KRenderData m_OldBasicData;

private:
    PEOPLE_STATE m_eState;
    PEOPLE_STATE m_eOldState;
    bool m_bIsContact;
    bool m_bOldIsContact;
    float m_fXSpeed;
    float m_fYSpeed;
    unsigned char m_cNoCheckContact;
    bool m_bFrameLock;
    GCSquarePeopleActivitySensor* m_pSensor;
    DWORD m_dwUID;
    KSquareUserInfo m_kPeopleInfo;
    KGC3DObject* m_pObject;
    int m_nNotifyPositionToServer;
    SQUARE_PEOPLE_TEMPLATE m_kSpTemplate;
    LOADING_STATE m_kLoadedState;
    KGCEmoticonInstance* m_pEmoticon;
    bool m_bDownJump;
    void MakeOneFrameSample( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kOneFrame );
    PositionDesc m_posDesc;
    void ReadyToRender();
    void EgoItemInitialize();
    void EgoItemFrameMove();
    void SetEgoItemOrderPlayer(char cConditionIdx, DWORD dwUnique);
    PLAYER_TEMPLATE m_PlayerTemplate;
    char Extra_Char_Num;
    MAP_OVERLAPPED_PAIR m_mapOverlappedPair;
    std::vector< ITEM_EFFECT > m_vecItemEff;
    std::vector< ITEM_EFFECT > m_vecDamageEff;
    std::vector< ITEM_EFFECT > m_vecSkillEff;
    std::vector< KGCAnimManager::ItemAnim > m_ItemAnim;
    std::vector< KGCAnimManager::DamageAnim > m_DamageAnim;
    std::vector< KGCAnimManager::SkillAnim* > m_SkillAnim;
    DWORD m_Elemental;
    int m_StrongLevel;
    std::vector< int > m_vecPetAiID;
    bool m_bBufferEmpty;
	GCDeviceTexture*	m_pGCClubTexture;
	GCImageIconTexture* m_pGuildMarkTexture;

	LPDIRECT3DTEXTURE9 m_pTexture;
	std::pair< std::wstring, D3DCOLOR > m_prPetNameWithColor;
        std::vector< KGCEgoItemInfo > m_pObjectEgo;

    bool m_bIsEquipCoordiSetItem;
    KCoordiSetTemplet m_kCoordiSetTemplet;
    bool m_bLoadDownHelmetNFL;
    bool m_bEquipBigHead;
    bool m_bShadowHead;
    GCMeshObject* m_pNonChangeFaceLift;

    bool m_bEquipFaceOffHead;

public:
    bool IsEquipCoordiSetItem() { return m_bIsEquipCoordiSetItem; }
    bool IsRemovePartsItem( REMOVEPARTS removeParts);
};
