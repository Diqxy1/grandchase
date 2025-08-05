#ifndef _KGCSHORTCUTITEMDEFINE_H_
#define _KGCSHORTCUTITEMDEFINE_H_

//#include <map>
//#include <GCLuaManager/KLuaManager.h>
//#include "KGCLuabinder.h"
#include <ProtectedType.h>

// 플레이어 정보를 얻어볼까 ~ ♬
// 얘가 여기 있는 이유는 편의상~~~
struct PLAYER_INFO
{
	// Level
	int		iLevel;			// 내 캐릭터의 레베루가 얼마인가
	// HP 관련 내용
	KProtectedType<float>*	pfHP;			// 현재 남은 피가 얼마인가
	float	fHP_Point;		// 피통이 만땅으로 차면 얼마인가
	bool	bIncHP;			// 피가 만땅인가 아닌가

	// MP 관련 내용
	KProtectedType<float>*	pfMP;			// 플레이어의 현재 MP 만땅은 3.0 (마법이 3레벨까지 있어서 그런 듯)

	// 공격력
	KProtectedType<float>*	pfAtk_Point;		// 플레이어 공격력을 올려볼까 ~ ♬

	// 방어력
	KProtectedType<float>*	pfDef_Point;		// 플레이어 방어력을 올려볼까 ~ ♬
};

// POTION이나 BUFF같이 능력치에 영향을 끼치는 Item 정보 구조체
struct SHORTCUT_ITEM
{
    int			iItemType;			    // Item 구분 Type
    int			iRemainTime;		    // Item 효과의 지속시간
    int			iCoolTime;			    // Item 사용 후 다음에 사용 가능할 때까지 걸리는 시간.
    float		fHP_RecoveryRate;	    // Item이 HP를 회복/감소 시키는 수치
    float		fMP_RecoveryRate;	    // Item이 MP를 회복/감소 시키는 수치
    float       fPetMp;                 // Item이 Pet의 MP를 적용 시키는 수치
    float		fAttack;			    // Item이 공격력을 증가/감소 시키는 수치
    float		fDefense;			    // Item이 방어력을 증가/감소 시키는 수치
    int         iSystemMessage;         //
    bool    	bCancelation;           // Item이 상태이상을 회복 시켜 주는가?
	bool		bPercentageRecovery;    // Item이 HP/MP 회복을 비율로 할 것인가?
    bool        bLevelDesignRecovery;   // Item이 회복량이 절대 수치인가?
    int         iEmoticonGoodsID;       // Item사용시 표시되는 이모티콘의 굿스 아이디
    int         iDamageEnumRight;       // Item 사용시 추가되는 데미지 Enum( 오른쪽 ) 
    int         iDamageEnumLeft;        // Item 사용시 추가되는 데미지 Enum( 왼쪽 ) 
    DWORD       dwSummonMonsterID;      // 소환될 몬스터 ID 
    
    float        fDamageOREmoticonAppearanceRate;

	std::string strSound;		// Item이 사용될 때 재생되는 사운드
	std::string strSoundAll;
	int		iBuffIndex;			//.Item이 버프를 발동시키는가? 시간이 없어서 이렇게 함..이해해주셈염
	float	fBuffTime;			// Item의 버프 지속 시간
    std::map< int, int> m_mapSkillMotion;
	std::set< EGCGameMode > m_setGameModes;
	int iEquipLimit;

	SHORTCUT_ITEM()
	{
		iItemType			=	0;
		iRemainTime			=	0;
		iCoolTime			=	0;
		fHP_RecoveryRate	=	0.0f;
		fMP_RecoveryRate	=	0.0f;
        fPetMp              =   0.0f;
		fAttack				=	0.0f;
		fDefense			=	0.0f;
		bCancelation		=	false;
		bPercentageRecovery	=	false;
		iBuffIndex			= 0;
		fBuffTime			= 0.0f;
        iEmoticonGoodsID    = 0;
        iSystemMessage      = -1;
        bLevelDesignRecovery = false;
        iDamageEnumRight    = -1;
        iDamageEnumLeft     = -1;
        iDamageEnumLeft     = -1;
        dwSummonMonsterID   = 0;
        fDamageOREmoticonAppearanceRate = -1.f;
		m_setGameModes.clear();
		iEquipLimit			= -1;
	}
	SHORTCUT_ITEM( const SHORTCUT_ITEM& arg_ )
	{
		iItemType			=	arg_.iItemType;
		iRemainTime			=	arg_.iRemainTime;
		iCoolTime			=	arg_.iCoolTime;
		fHP_RecoveryRate	=	arg_.fHP_RecoveryRate;
		fMP_RecoveryRate	=	arg_.fMP_RecoveryRate;
        fPetMp              =   arg_.fPetMp;
		fAttack				=	arg_.fAttack;
		fDefense			=	arg_.fDefense;
		bCancelation		=	arg_.bCancelation;
		strSound			=	arg_.strSound;
		bPercentageRecovery	=	arg_.bPercentageRecovery;
		iBuffIndex			=	arg_.iBuffIndex;
		fBuffTime			=	arg_.fBuffTime;
        m_mapSkillMotion    =   arg_.m_mapSkillMotion;
        iEmoticonGoodsID    =   arg_.iEmoticonGoodsID;
        iSystemMessage      =   arg_.iSystemMessage;
        bLevelDesignRecovery = arg_.bLevelDesignRecovery;
        iDamageEnumRight    = arg_.iDamageEnumRight;
        iDamageEnumLeft     = arg_.iDamageEnumLeft;
        dwSummonMonsterID   = arg_.dwSummonMonsterID;
        fDamageOREmoticonAppearanceRate = arg_.fDamageOREmoticonAppearanceRate;
		m_setGameModes		= arg_.m_setGameModes;
		iEquipLimit			= arg_.iEquipLimit;
	}

    void clear()
    {
        iItemType			= 0;
        iRemainTime			= 0;
        iCoolTime			= 0;
        fHP_RecoveryRate	= 0.0;
        fMP_RecoveryRate	= 0.0;
        fPetMp              = 0.0;
        fAttack				= 0.0;
        fDefense			= 0.0;
		bCancelation        = false;
		bPercentageRecovery	=	false;
		iBuffIndex			= 0;
		fBuffTime			= 0.0f;
        iEmoticonGoodsID    =   0;
        iSystemMessage      = -1;
        bLevelDesignRecovery = false;
        iDamageEnumRight    = -1;
        iDamageEnumLeft     = -1;
        dwSummonMonsterID   = 0;
        fDamageOREmoticonAppearanceRate = -1.f;
        m_mapSkillMotion.clear();
		m_setGameModes.clear();
		iEquipLimit			= -1;
	}
};


class KGCShortCutItemDefine
{
public:
	KGCShortCutItemDefine();
	~KGCShortCutItemDefine();

public:
	void LoadItem();
	int	 GetItemKind( DWORD ItemID );
	bool GetStructure( int ItemID, SHORTCUT_ITEM* &structure );	// Key(ItemID)값을 받아 해당 Item 정보를 가진 구조체를 반환.

protected:
	void initItemTable();

protected:
	int		m_iItemCount;
	std::map<int,SHORTCUT_ITEM> 	m_Map_Item;
};

#endif	//_KPOTION_BUFF_H_
