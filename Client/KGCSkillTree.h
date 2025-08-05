#pragma once

#include <KNCSingleton.h>
#include "./mission/KGCMissionString.h"
#include "GCSkillEnum.h"

class GCSkill;
class GCSkillResource;
class PLAYER;
class KGCToolTip;
struct PLAYER_TEMPLATE;

#define MAX_SLOT_COUNT 5
class KGCSkillTree
{
    DeclareSingleton( KGCSkillTree );

public:
    KGCSkillTree(void);
    virtual ~KGCSkillTree(void);

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
    void clear();
    void FillSkillTree();
    void GetSKillPtr( IN const SkillSet& setID, OUT SkillPtrList& lSkill );
    void GetPrecedeSkill( EGCSkillTree eID, OUT SkillSet& setPrecede );
    bool CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill );
    bool IsSkillTrainable( EGCSkillTree eSkill );
	bool IsLearnedSkill( EGCSkillTree eSkill );
    void InsertUserSkillSet( DWORD dwUserUID, const SkillSet& setSKill );
    void RemoveUserSkillSet( DWORD dwUserUID );
	void GetSkillSet( int iCharType, int nPromotion, OUT std::vector< EGCSkillTree >& setSkill );
    std::vector<EGCSkillTree> GetHotKeySkillVector( int iCharType, int nPromotion );
    std::vector<EGCSkillTree> GetHotKeySkillVectorByType( int iCharType, int nPromotion, int iSlotType_ );

	void EquipSkill( EGCSkillTree eSkillID );
	void EquipSkill( const GCSkill* pSkill,int iPromotion );
    bool EquipItemSkill( DWORD dwItemID );
	void EquipHotKeySkill(std::vector<KSkillSlot>& setInfo, const GCSkill* pSkill );
	void UnequipSkill( EGCSkillGroup eGroup );
	void UnequipAllSkill( int iCharType, int iPromotion );	
	
	const GCSkill*	GetSkill( EGCSkillTree eID );
	const GCSkill*	GetSkillFromItemID( DWORD dwItemID );
	const GCSkill*	GetEquipedSkill( EGCSkillGroup eGroupID );
	const GCSkill*	GetSimmilarLevelSkill( int iLevel );
    const GCSkill*  GetTutorialSimmilarLevelSkill( int iLevel );
	const GCSkill*	GetNextLevelSkill( const GCSkill* pSkill );
	const GCSkill*	GetPrevLevelSkill( const GCSkill* pSkill );
	bool IsEquipedSkill( EGCSkillTree eSkillID );
    bool IsEquipedSkill( EGCSkillTree eSkillID, char cCharType_, char cPromotion_ );
	std::set< EGCSkillTree > GetSkillIDsInGroup( EGCSkillGroup eGroupID );
	std::vector< EGCSkillTree > GetSkillIDsInGroupInVector( EGCSkillGroup eGroupID );
	std::pair<bool,DWORD> IsExistSkillUntrainItem();
	void ApplySettingToServer();

    int GetCurrentSP( int iCharType );
	int GetExtraSP( int iCharType );
	int GetMaxSP( int iCharType , int iPromotion);
	int GetUsedSPinPromotion( int iCharType, int iPromotion );
	int GetMaxSPinPromotion( int iCharType , int iPromotion );

    bool IsBlockedSkill( EGCCharType eType, int nGroup );
    bool Process_UnlockGroupItem( int nGroup, bool bBrowserClose );
    int GetBuyingReqGroup() const { return m_nBuyingReqGroup; }
    void SetBuyingReqGroup(int val) { m_nBuyingReqGroup = val; }

	void CheckNewSkill();
	bool IsNewSkill( EGCSkillTree eSkillID );
	int	GetNewSkillNum();

    void BindToLua();

    // packet processing
	void Server_SetFullSPInfo( KSkillTreeFullInfo& kRecv );	
    void Server_SetTrainedSkill( const std::map<std::pair<char,char>, KSPInfo>& kFullInfo );
    void Server_SetSkillEquipInfo( std::map< std::pair<char,char>, std::vector<KSkillSlot> > kSkillEquiptInfo_ );    
    void TransformSkillInfo( const KSPInfo& kSPInfo, OUT SkillSet& setSkill );
    bool Server_TrainSkill( int nSkillID );
    bool Server_UnTrainSkill( int nSkillID );
	void Server_UnlockGroup( const KEVENT_UNLOCK_SKILL_ACK& kPacket );

    char GetCurUICharType() { return m_cCurUICharType; }
	void SetCurUICharType( char cCharType ) { m_cCurUICharType = cCharType; }
    char GetCurUIPromotion() { return m_cCurUIPromotion; }
	void SetCurUIPromotion( char cPromotion) { m_cCurUIPromotion = cPromotion; }

private:
	void CheckUntrainItem( IN DWORD dwItemID, IN OUT int& iDuration , OUT std::pair<bool,DWORD>& ret);
	void UnEquipSkillGroup( std::vector<KSkillSlot>& setInfo, EGCSkillGroup eGroup, OUT bool* pbSlotSkillUnequipped = NULL );
	SkillSet* GetTrainedSkillInfo( EGCSkillTree eSkillID );

private:
    KGCMissionString m_kSkillStr;
    std::map< EGCSkillTree, GCSkill* > m_mapSkill;

    std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill; // 내가배운스킬    
    std::map<std::pair<char,char>, std::vector<KSkillSlot> > m_mapSkillEquipInfo; // 내 세트
	std::map< DWORD, EGCSkillTree > m_mapSkillItem;

	std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill2; // 내가배운스킬
	std::map<std::pair<char,char>, std::vector<KSkillSlot> > m_mapSkillEquipInfo2; // 내 세트


    std::map< EGCSkillTree, std::set<DWORD> > m_mapSkillUsers; // 스킬1 : 유저n

	std::map< EGCSkillGroup, int > m_mapCashSkillPosition;
    std::map< std::pair< char, char >, std::vector< GCSkill* > > m_mapOrderSkillByLevel;

    int m_nBuyingReqGroup;
	char m_cCurUICharType;
	char m_cCurUIPromotion;	// UI에서 선택한 프로모션 정보입니다. 현재 유저가 사용중인 케릭터 정보가 아닙니다.
    
    int m_iMaxSID;

	PLAYER* m_pLocalPlayer;
	bool m_bUnlockAll;
	bool m_bCharacterSelectable;
    KEVENT_MAX_CHAR_SP_LEVEL_NOT m_mapMaxSkillInfo;

	
public:	
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
    KSafeArray< int, KSkillSlot::INDEX_MAX > m_iHotKeyIndex;

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

    std::set< EGCSkillTree > GetHotKeySkillSetByChar( char cCharType_, char cPromotion_ );
    std::set< EGCSkillTree > GetTrainedSkillSetByChar( char cCharType_, char cPromotion_ );
    
    std::vector< EGCSkillTree > GetSkillsPerClass( char cCharType_ = -1, char cPromotion_ = -1 );

    void GetAllEquipedSkillInfo( std::map<std::pair<char,char>, std::vector<KSkillSlot> >& mapEquipedSkillInfo_ ) { mapEquipedSkillInfo_ = m_mapSkillEquipInfo; }
    void SetAllEquipedSkillInfo( std::map<std::pair<char,char>, std::vector<KSkillSlot> >& mapEquipedSkillInfo_ ) { m_mapSkillEquipInfo = mapEquipedSkillInfo_; }

private:
    std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTutorialTrainedSkill; // 연습모드에서 배운 스킬 - 임시 저장용

public:
    void TutorialTrainingSkill( EGCSkillTree eSkillID_ );
    void TutorialUnTrainingSkill( EGCSkillTree eSkillID_ );
    void GetTutorialTrainingSkill( std::map< std::pair<EGCCharType, char>, SkillSet >& mapTutorialTrainedSkill_ ) { mapTutorialTrainedSkill_ = m_mapTutorialTrainedSkill; }
    void SetTutorialTrainingSkill( std::map< std::pair<EGCCharType, char>, SkillSet >& mapTutorialTrainedSkill_ ) { m_mapTutorialTrainedSkill = mapTutorialTrainedSkill_; }

private:
    std::set< EGCSkillTree > m_setSaveTrainSkill;
    std::set< EGCSkillTree > m_setSaveRemoveSkill;

public:
    void GetAllTrainedSkillInfo( std::map< std::pair<EGCCharType, char>, SkillSet >& mapTrainedSkill_ ) { mapTrainedSkill_ = m_mapTrainedSkill; }
    void SetAllTrainedSkillInfo( std::map< std::pair<EGCCharType, char>, SkillSet >& mapTrainedSkill_ ) { m_mapTrainedSkill = mapTrainedSkill_; }
    void SaveTrainSkill( EGCSkillTree eSkillID_ );
    void SaveRemoveSkill( EGCSkillTree eSkillID_ );
    void ClearTrainRemoveSkill();

};

extern bool bReloadSkillTree;

DefSingletonInline( KGCSkillTree );

class FindEquipedSkillInGroup{
private:
    EGCSkillGroup m_iGroupID;
    FindEquipedSkillInGroup() {}
public:
    FindEquipedSkillInGroup( EGCSkillGroup iGroup ) { m_iGroupID = iGroup; }
    ~FindEquipedSkillInGroup() {}

    bool operator() ( const KSkillSlot& arg )
    {
        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( (EGCSkillTree)arg.m_nSkillID );

        if( pSkill == NULL )
            return false;

        if( pSkill->GetGroupID() == m_iGroupID )
            return true;

        return false;
    }
};

class FindEquipedSkill{
private:
    EGCSkillTree m_eSkillID;
    FindEquipedSkill() {}
public:
    FindEquipedSkill( EGCSkillTree eSkillID ) { m_eSkillID = eSkillID; }
    ~FindEquipedSkill() {}

    bool operator() ( const KSkillSlot& arg )
    {
        const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( (EGCSkillTree)arg.m_nSkillID );

        if( pSkill == NULL )
            return false;

        if( pSkill->GetID() == m_eSkillID )
            return true;

        return false;
    }
};

class SortByLevelToSkill{
public:
    SortByLevelToSkill() {}
    ~SortByLevelToSkill() {}

    bool operator() ( const GCSkill* left, const GCSkill* right )
    {
        return left->GetLv() < right->GetLv();
    }
};

class DescendingSortByLevel{
public:
    DescendingSortByLevel() {}
    ~DescendingSortByLevel() {}

    bool operator() ( const GCSkill* left, const GCSkill* right )
    {
        return left->GetLv() > right->GetLv();
    }
};

class SortByLevelToSkillID{
public:
    SortByLevelToSkillID() {}
    ~SortByLevelToSkillID() {}

    bool operator() ( const EGCSkillTree& left, const EGCSkillTree& right )
    {
        const GCSkill* pLeft = SiKGCSkillTree()->GetSkill( left );
        const GCSkill* pRight = SiKGCSkillTree()->GetSkill( right );
        return pLeft->GetLv() < pRight->GetLv();
    }
};

class DescendingSortByLevelToSkillID{
public:
    DescendingSortByLevelToSkillID() {}
    ~DescendingSortByLevelToSkillID() {}

    bool operator() ( const EGCSkillTree& left, const EGCSkillTree& right )
    {
        const GCSkill* pLeft = SiKGCSkillTree()->GetSkill( left );
        const GCSkill* pRight = SiKGCSkillTree()->GetSkill( right );
        return pLeft->GetLv() > pRight->GetLv();
    }
};