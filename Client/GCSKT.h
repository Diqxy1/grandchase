#pragma once

#include <KNCSingleton.h>
#include "./mission/KGCMissionString.h"
//#include "KLuaManager.h"
//#include <map>
//#include <list>
//#include "GCEnum.h"
#include "GCSkillEnum.h"
//#include <UserPacket.h>

class GCSkill;
class GCSkillResource;
class PLAYER;
class KGCToolTip;
struct PLAYER_TEMPLATE;

#define MAX_SLOT_COUNT 5
class GCSKT
{
    DeclareSingleton( GCSKT );

public:
    GCSKT(void);
    virtual ~GCSKT(void);

    // typedefs
    typedef std::map< EGCSkillTree, GCSkill* >::iterator IterSkill;
    typedef std::map< EGCSkillTree, GCSkill* >::value_type SKTValue;
    typedef std::map< EGCSkillTree, GCSkillResource* >::iterator IterSkillRes;
    typedef std::map< EGCSkillTree, GCSkillResource* >::value_type SKTResValue;
    typedef std::map< std::pair<EGCCharType, char>, SkillSet >::value_type TrainedSKValue;
    typedef std::map< std::pair<EGCCharType, char>, SkillSet >::iterator IterTrainedSK;
    typedef std::map< EGCSkillTree, std::set<DWORD> >::iterator ItserSkillUsers;
    typedef std::map< EGCSkillTree, std::set<DWORD> >::value_type SkillUsersValue;
	typedef std::map< EGCSkillGroup, GCSkill* > SKTBaseSkill;
	typedef std::map< EGCSkillGroup, GCSkill* >::iterator IterBaseSkill;

    // functions
    const std::wstring& GetString( int iID );
    bool LoadSkillScript( KLuaManager& luaMgr );
    bool LoadSkillString( const std::string& strSkill );
    bool LoadSkillResource( KLuaManager& luaMgr );
	bool LoadSkillGroupScript( KLuaManager& luaMgr );
	bool CheckPosibleSkill(IN const GCSkill* pGCSkill_ );
    void clear();
    void FillSkillTree();
    void GetSKillPtr( IN const SkillSet& setID, OUT SkillPtrList& lSkill );
    void GetPrecedeSkill( EGCSkillTree eID, OUT SkillSet& setPrecede );
    bool CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill );
    bool IsSkillTrainable( EGCSkillTree eSkill );
	bool IsLearnedSkill( EGCSkillTree eSkill );
    void InsertUserSkillSet( DWORD dwUserUID, const SkillSet& setSKill );
    void RemoveUserSkillSet( DWORD dwUserUID );
    void LoadingResource( DWORD dwUID );
    void ReleaseResource( DWORD dwUID );
	void GetSkillSet( int iCharType, int nPromotion, OUT std::vector< EGCSkillTree >& setSkill );
    std::vector<EGCSkillTree> GetHotKeySkillVector( int iCharType, int nPromotion );
    std::vector<EGCSkillTree> GetHotKeySkillVectorByType( int iCharType, int nPromotion, int iSlotType_ );

	void ChangeSetting( int iCharType, int iPromotion, int iSetID );
	int GetSetting( int iCharType, int iPromotion );
	void EquipSkill( EGCSkillTree eSkillID, int iSetID );
	void EquipSkill( const GCSkill* pSkill,int iPromotion, int iSetID );
	void EquipHotKeySkill(std::vector<KSkillSlot>& setInfo, const GCSkill* pSkill );
	void UnequipSkill( EGCSkillGroup eGroup, int iSetID );
	void UnequipAllSkill( int iCharType, int iPromotion, int iSetID );
	bool EquipSkill( DWORD dwItemID );

	const GCSkill*	GetBaseSkill( EGCSkillGroup eGroupID );
	const GCSkill*	GetSkill( EGCSkillTree eID );
	const GCSkill*	GetSkillFromItemID( DWORD dwItemID );
	const GCSkill*	GetEquipedSkill( EGCSkillGroup eGroupID, int iSetID );
	const GCSkill*	GetSimmilarLevelSkill( int iLevel );
    const GCSkill*  GetTutorialSimmilarLevelSkill( int iLevel );
	const GCSkill*	GetNextLevelSkill( const GCSkill* pSkill );
	const GCSkill*	GetPrevLevelSkill( const GCSkill* pSkill );
	bool IsEquipedSkill( EGCSkillTree eSkillID, int iSetID );
	std::set< EGCSkillTree > GetSkillIDsInGroup( EGCSkillGroup eGroupID );
	std::vector< EGCSkillTree > GetSkillIDsInGroupInVector( EGCSkillGroup eGroupID );
	std::pair<bool,DWORD> IsExistSkillUntrainItem();
	void ApplySettingToServer();

    int GetCurrentSP( int iCharType );
	int GetExtraSP( int iCharType );
    int GetLeftSPExcludingCommonSkill( int iCharType );
	int GetMaxSP( int iCharType , int iPromotion);
	int GetUsedSPinPromotion( int iCharType, int iPromotion );
	int GetMaxSPinPromotion( int iCharType , int iPromotion );

    bool IsBlockedSkill( EGCCharType eType, int nGroup );
    bool IsBlockedSkill( int nGroup );
    bool Process_UnlockGroupItem( int nGroup, bool bBrowserClose );
    int GetBuyingReqGroup() const { return m_nBuyingReqGroup; }
    void SetBuyingReqGroup(int val) { m_nBuyingReqGroup = val; }

	void CheckNewSkill();
	bool IsNewSkill( EGCSkillTree eSkillID );
	int	GetNewSkillNum();

    void _D_TutorialAllTrainSkill();
	void _D_Equip_Skill( int iCharType, int nPromotion, int nSetting, int nID );
	void _D_UnEquip_Skill( int iCharType, int nPromotion, int nSetting, int nID );
	void _D_UnAllEquip_Skill( int iCharType, int nPromotion, int nSetting );
	void _D_TrainSkill( EGCCharType eCharType, int nPromotion, EGCSkillTree eSkill );
	void _D_UnTrainSkill( EGCCharType eCharType, int nPromotion, EGCSkillTree eSkill );
    void _D_UnAllTrainSkill( EGCCharType eCharType, int nPromotion );
    // for test func GCSKTDebug.cpp 에 있다.
#if defined( _DEBUG ) || !defined( __PATH__ )
	bool _D_ReloadUIinfo( KLuaManager& luaMgr );
    
    void _D_InsertLocalPlayerSkillSet( DWORD dwUID, EGCCharType eCharType, int nPromotion );
    void _D_TrainSkillReqtoServer( int nSkill );
    void _D_UnTrainSkillReqtoServer( int nSkill );
    void _D_Dump_Skill( int iCharType, int nPromotion );
    void _D_Dump_Setting( int iCharType, int nPromotion );
    void _D_Set_Setting( int iCharType, int nPromotion, int nSetting );
    void _D_Apply_SettingChangetoServer();
    
    void _D_UnlockGroupReqtoServer( int nGroup );

    // for lua
    void RegisterLuabind();
    void TrainSkill( int nSkill );
    void Launch();
#endif
    void BindToLua();

    // packet processing
	void Server_SetFullSPInfo( KFullSPInfo& kRecv );	
    void Server_SetTrainedSkill( const std::map<std::pair<char,char>, KSPInfo>& kFullInfo );

    void Server_SetSkillSet( std::map< std::pair<char,char>, std::vector<KSPSetInfo> >& kSkillSet );
    void Server_SetSkillSetting( std::map< std::pair<char,char>, int >& kSetting );
    void TransformSkillInfo( const KSPInfo& kSPInfo, OUT SkillSet& setSkill );
    bool Server_TrainSkill( int nSkillID );
    bool Server_UnTrainSkill( int nSkillID );
    void Server_SetUnlockGroup( const std::map<char, std::set<int> >& mapUnLockGroup );
	
    void Server_UnlockGroup( char cCharType, int nUnLockGroupID );

    char GetCurUICharType() { return m_cCurUICharType; }
	void SetCurUICharType( char cCharType ) { m_cCurUICharType = cCharType; }
	void SetCurUIPromotion( char cPromotion) { m_cCurUIPromotion = cPromotion; }

    std::map< std::pair<char,char>, int >& GetSkillSettings() { return m_mapSetting; }
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& GetSkillSets() { return m_mapSkillSets; }

private:
	void CheckUntrainItem( IN DWORD dwItemID, IN OUT int& iDuration , OUT std::pair<bool,DWORD>& ret);
	void UnEquipSkillGroup( std::vector<KSkillSlot>& setInfo, EGCSkillGroup eGroup, OUT bool* pbSlotSkillUnequipped = NULL );
	SkillSet* GetTrainedSkillSet( EGCSkillTree eSkillID );    

private:
    KGCMissionString m_kSkillStr;
    std::map< EGCSkillTree, GCSkill* > m_mapSkill;
    std::map< EGCSkillTree, GCSkillResource* > m_mapSkillRes;

    std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill; // 내가배운스킬
    std::map< std::pair<char,char>, int >  m_mapSetting; // 내 세팅
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> > m_mapSkillSets; // 내 세트
	std::map< DWORD, EGCSkillTree > m_mapSkillItem;

	std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill2; // 내가배운스킬
	std::map< std::pair<char,char>, int >  m_mapSetting2; // 내 세팅
	std::map<std::pair<char,char>, std::vector<KSPSetInfo> > m_mapSkillSets2; // 내 세트
	


    std::map< EGCSkillTree, std::set<DWORD> > m_mapSkillUsers; // 스킬1 : 유저n
	std::map< EGCSkillGroup, GCSkill* > m_mapBaseSkill;

	std::map< EGCSkillGroup, int > m_mapCashSkillPosition;
    std::map<char, std::set<int> > m_mapUnLockGroupID;
	std::map<char, std::set<int> > m_mapUnLockGroupID2;
	std::map<int, std::vector< GCSkill* > > m_mapOrderByLevel;

	std::map<int , std::map<int , std::vector<int>> > m_mapSkillGroup;
    int m_nBuyingReqGroup;
	char m_cCurUICharType;
	char m_cCurUIPromotion;	// UI에서 선택한 프로모션 정보입니다. 현재 유저가 사용중인 케릭터 정보가 아닙니다.
    
    int m_iMaxSID;

	PLAYER* m_pLocalPlayer;
	bool m_bUnlockAll;
	bool m_bCharacterSelectable;
    KEVENT_MAX_CHAR_SP_LEVEL_NOT m_mapMaxSkillInfo;

	
public:	
    GCSkillResource* GetSkillResource( EGCSkillTree eSID_ );

	bool m_bNewSkillMsgRender;

	bool IsNewSkillMsgRender() { return m_bNewSkillMsgRender; }
	void SetNewSkillMsgRender(bool bShow) { m_bNewSkillMsgRender = bShow; }

	PLAYER* GetOwnerPlayer() {return m_pLocalPlayer; }
	void SetOwnerPlayer( PLAYER* pPlayer ){ m_pLocalPlayer = pPlayer; }
	void SetOrigSkillTree();

    EGCSkillTree GetMaxSID();

	void SetUnlockAll( bool bUnlock );
	
	bool IsUnlockAll(){ return m_bUnlockAll; }
	void SetCharacterSelectable( bool bSelectable )
	{
		m_bCharacterSelectable = bSelectable;
	}
	bool IsCharacterSelectable(){ return m_bCharacterSelectable; }

	bool m_bPromotionSelectable;
#if defined( USE_AP_SLOT_CHANGE ) 
    KSafeArray< int, KSkillSlot::INDEX_MAX > m_iHotKeyIndex;
#else
    int m_iHotKeyIndex;
#endif

	void SetHotKeyIndexToEmpty( int iChar, int iPromotion );
	void SetHotKeyIndexToEmpty();
    void SetSkillMaxSPInfo(KEVENT_MAX_CHAR_SP_LEVEL_NOT &recv){m_mapMaxSkillInfo = recv;}
    bool LoadSkillStringFromKOM( const std::string& strSkill );
private:
    int m_iCurSlotType;
    std::map< char, int > m_mapSlotOpenInfo;

public:
    void SetCurrentSlotType( int iSlotType_ ) { m_iCurSlotType = iSlotType_; }
    int GetCurrentSlotType() { return m_iCurSlotType; }
    
    int GetOpenSkillSlotIndex( char cCharType_ );
    void SetOpenSkillSlotIndex( std::map< char, int >& mapSlotOpenInfo_ ) { m_mapSlotOpenInfo = mapSlotOpenInfo_; }

private:
    std::map< DWORD, int > m_mapSkillMissionInfo;
public:
    void GetSkillMissionInfo( std::map< DWORD, int >& mapSkillMissionInfo_ ) { mapSkillMissionInfo_ = m_mapSkillMissionInfo; }
    bool IsSkillMissionSkillID( EGCSkillTree eSkillID_ );

    int GetCurrentSkillSetSize();

public:
    void SetNewCharacterTrainedSkill( const KSPInfo& info_ );
    void SetUpdateTrainedSkill( const KSPInfo& info_ );
};

extern bool bReloadSKT;

DefSingletonInline( GCSKT );
