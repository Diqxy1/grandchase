#ifndef _KGCSHORTCUTITEMDEFINE_H_
#define _KGCSHORTCUTITEMDEFINE_H_

//#include <map>
//#include <GCLuaManager/KLuaManager.h>
//#include "KGCLuabinder.h"
#include <ProtectedType.h>

// �÷��̾� ������ ���� ~ ��
// �갡 ���� �ִ� ������ ���ǻ�~~~
struct PLAYER_INFO
{
	// Level
	int		iLevel;			// �� ĳ������ �����簡 ���ΰ�
	// HP ���� ����
	KProtectedType<float>*	pfHP;			// ���� ���� �ǰ� ���ΰ�
	float	fHP_Point;		// ������ �������� ���� ���ΰ�
	bool	bIncHP;			// �ǰ� �����ΰ� �ƴѰ�

	// MP ���� ����
	KProtectedType<float>*	pfMP;			// �÷��̾��� ���� MP ������ 3.0 (������ 3�������� �־ �׷� ��)

	// ���ݷ�
	KProtectedType<float>*	pfAtk_Point;		// �÷��̾� ���ݷ��� �÷����� ~ ��

	// ����
	KProtectedType<float>*	pfDef_Point;		// �÷��̾� ������ �÷����� ~ ��
};

// POTION�̳� BUFF���� �ɷ�ġ�� ������ ��ġ�� Item ���� ����ü
struct SHORTCUT_ITEM
{
    int			iItemType;			    // Item ���� Type
    int			iRemainTime;		    // Item ȿ���� ���ӽð�
    int			iCoolTime;			    // Item ��� �� ������ ��� ������ ������ �ɸ��� �ð�.
    float		fHP_RecoveryRate;	    // Item�� HP�� ȸ��/���� ��Ű�� ��ġ
    float		fMP_RecoveryRate;	    // Item�� MP�� ȸ��/���� ��Ű�� ��ġ
    float       fPetMp;                 // Item�� Pet�� MP�� ���� ��Ű�� ��ġ
    float		fAttack;			    // Item�� ���ݷ��� ����/���� ��Ű�� ��ġ
    float		fDefense;			    // Item�� ������ ����/���� ��Ű�� ��ġ
    int         iSystemMessage;         //
    bool    	bCancelation;           // Item�� �����̻��� ȸ�� ���� �ִ°�?
	bool		bPercentageRecovery;    // Item�� HP/MP ȸ���� ������ �� ���ΰ�?
    bool        bLevelDesignRecovery;   // Item�� ȸ������ ���� ��ġ�ΰ�?
    int         iEmoticonGoodsID;       // Item���� ǥ�õǴ� �̸�Ƽ���� �½� ���̵�
    int         iDamageEnumRight;       // Item ���� �߰��Ǵ� ������ Enum( ������ ) 
    int         iDamageEnumLeft;        // Item ���� �߰��Ǵ� ������ Enum( ���� ) 
    DWORD       dwSummonMonsterID;      // ��ȯ�� ���� ID 
    
    float        fDamageOREmoticonAppearanceRate;

	std::string strSound;		// Item�� ���� �� ����Ǵ� ����
	std::string strSoundAll;
	int		iBuffIndex;			//.Item�� ������ �ߵ���Ű�°�? �ð��� ��� �̷��� ��..�������ּ���
	float	fBuffTime;			// Item�� ���� ���� �ð�
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
	bool GetStructure( int ItemID, SHORTCUT_ITEM* &structure );	// Key(ItemID)���� �޾� �ش� Item ������ ���� ����ü�� ��ȯ.

protected:
	void initItemTable();

protected:
	int		m_iItemCount;
	std::map<int,SHORTCUT_ITEM> 	m_Map_Item;
};

#endif	//_KPOTION_BUFF_H_
