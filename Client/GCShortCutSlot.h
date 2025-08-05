//////////////////////////////////////////////////////////////////////////
//							crey21	20071119							//
//////////////////////////////////////////////////////////////////////////

#ifndef _GCSHORTCUTSLOT_H_
#define _GCSHORTCUTSLOT_H_

//#include "GCItem.h"
//#include "GCItemManager.h"
//#include "GCShortCutItem.h"			// Slot�� ���� Item�� \Virtual Fucntion���� �����ϰ� �ֿ�.
#include "GCShortCutItemDefine.h"	// Slot�� ���� Item�� ���� ����ü�� �����ϰ� �ֿ�.

class KGCShortCutSlotItem;

// ShortCutSlot�� ������ �� �ִ� ItemID ���
enum SCS_ITEM_KIND
{
	HP_POTION = 0,
	MP_POTION,
	PROTECT_RING,
	REJUVENATION_POTION,
	CANCELATION_POTION,
	BUFF_POTION,
    SKILL_POTION,
    SKILL_POTION2,
    EVENT_POTION,
    LV1_SPECIAL_SKILL_POTION,
    LV2_SPECIAL_SKILL_POTION,
    LV3_SPECIAL_SKILL_POTION,
    LV4_SPECIAL_SKILL_POTION,
    DAMAGE_POTION,
    SUMMON_POTION,
	LISNAR_PRAY,
};

const INT ITEM_SLOT_SIZE = 5;
const INT EMOT_SLOT_SIZE = 10;
const INT MCOMMU_SLOT_SIZE = 10;
const INT SPECIAL_SKILL_SIZE = 3;


// ���� �־��ִ� ���� GCUserItemBox
// ���� ���� ���� GCUserEquipBox Ȥ�� ���� ������ �������� ���Ǵ� ����̴�.
class KGCShortCutSlot
{
public :
	KGCShortCutSlot();
	~KGCShortCutSlot();

	void initialization();									// Normal Item Slot�� Emoticon Slot�� �ʱ�ȭ �Ѵ�.
	void Clear();											// Ŭ������
	void ClearItem();										// Normal Item Slot�� �ʱ�ȭ �Ѵ�.
	void ClearEmoticon();									// Emoticon Slot�� �ʱ�ȭ �Ѵ�.

	void SwapItem( INT& indexFrom, INT& indexTo );			// Slot�� �� Index�� �Է¹޾� ���� ��ġ�� �ٲ��ش�.

	bool UseItem( INT index );								// �ش� ���Կ� �ִ� Item�� ����Ѵ�.
	void UseEmot( INT index );								// �ش� ���Կ� �ִ� Emoticon�� ����Ѵ�

	KGCShortCutSlotItem* FindItem( INT index );				// Item Slot Index�� �޾Ƽ� �ش� Index�� Item ��ü �����͸� ��ȯ.
	KGCShortCutSlotItem* FindEmot( INT index );				// Emot Slot Index�� �޾Ƽ� �ش� Index�� Item ��ü �����͸� ��ȯ.
	INT  FindItemIndex( DWORD itemID );						// ItemID�� �޾� �ش� Item�� ������ �ִ� Slot�� ã�´�.
	INT  FindEmotIndex( DWORD itemID );						// ItemID�� �޾� �ش� Emot�� ������ �ִ� Slot�� ã�´�.

	INT	 InsertItem( KItem* );						// Item�� Slot�� �����ϰ� Index�� ��ȯ�Ѵ�.
	void DischargeItem( DWORD itemID );						// ItemID�� �ش��ϴ� Item�� Slot���� �����ϰ� ������ �����ش�.
	bool ExchangeItem( IN KItem* kInven );					// �Էµ� Item�� ���� Type�� Item�� DisCharge ��Ű�� �Էµ� Item�� Insert.
	bool SlotRefill();										// Slot���� ���� �������� �ڵ����� �������ش�.

    INT	 InsertEmot( KItem* );						// Emoticon�� Slot�� �����ϰ� Index�� ��ȯ�Ѵ�.
    void DischargeEmot( DWORD itemID );						// ItemID�� �ش��ϴ� �̸�Ƽ���� Slot���� �����ϰ� ������ �����ش�.
        


	INT  GetItemID( INT index );							// �ش� ������ Item�� ���� ID���� ��ȯ.
	GCITEMUID  GetItemUID( INT index );							// �ش� ������ Item�� ���� UID���� ��ȯ.
	INT  GetItemQuantity( INT index );						// �ش� ������ Item ������ ��ȯ.
	INT	 GetItemCoolTime( INT index );						// �ش� ������ Item�� CoolTime�� �󸶳� ���Ҵ��� ��ȯ.
	bool isItemCoolTime( INT index );						// �ش� ������ Item�� CoolTime �������� �ƴ����� ��ȯ.

	INT  GetEmotID( INT index );								// �ش� ������ Emot�� ���� ID���� ��ȯ.
	GCITEMUID  GetEmotUID( INT index );							// �ش� ������ Emot�� ���� UID���� ��ȯ.
	INT  GetEmotQuantity( INT index );						// �ش� ������ Emot ������ ��ȯ.
	INT  GetEmotCoolTime( INT index );						// �ش� ������ Emot�� CoolTime�� �󸶳� ���Ҵ��� ��ȯ.
	bool isEmotCoolTime( INT index );						// �ش� ������ Emot�� CoolTime �������� �ƴ����� ��ȯ.

	bool isExist( DWORD item );								// ShortCutSlot �ȿ� �ش� Item�� �����ϴ��� �˻�.
	bool isProtectRing( INT index );						// �ش� index�� Item�� ��ȣ�� ���� �迭���� �˻�
	INT  FindItemType( INT itemType );						// ItemType�� �޾� �ش� Type�� ������ �ִ� Slot�� ã�´�.
	INT  FindEmotType( INT itemType );						// ItemType�� �޾� �ش� Type�� ������ �ִ� Slot�� ã�´�.

    void FrameMove();

	bool IsGameModeAllowed( EGCGameMode eGameMode, int index);
	bool HasAllowedItem(EGCGameMode eGameMode);

	bool CanUseItem(int iIndex);
private :
/////////////////////
// Member Variables
	KSafeArray<KGCShortCutSlotItem*,ITEM_SLOT_SIZE> m_ItemSlot;		// Slot Item�� ��ü ������ �迭
	KSafeArray<KGCShortCutSlotItem*,EMOT_SLOT_SIZE> m_EmotSlot;		// Slot Emoticon�� ��ü ������ �迭

	KSafeArray<INT,ITEM_SLOT_SIZE> m_ItemSlotIndex;					// ItemSlot List�� �� ��ü�� indirect �����ϱ� ���� Index �迭
	KSafeArray<INT,EMOT_SLOT_SIZE> m_EmotSlotIndex;					// EmoticonSlot List�� �� ��ü�� indirect �����ϱ� ���� Index �迭

	PLAYER_INFO	m_MyPlayer;									// Item ��뿡 �ʿ��� �� �÷��̾��� ���� ����ü
	KGCShortCutItemDefine m_kItemDefine;					// ItemID�� ������ Item ���� ����ü�� ã�� �� �ִ� class ��ü

	INT m_Packet_OK;										// ��Ŷ ���۰���� ��������


/////////////////////
// Member Functions
public:
	void DeleteItembyIndex( INT index );					// �ش� Index�� �ִ� Item�� �����Ѵ�.
	void DeleteEmotbyIndex( INT index );					// �ش� Index�� �ִ� Emot�� �����Ѵ�.
	void DeleteItembyItemID( GCITEMID itemID );				// �ش� ItemID�� ���� Item�� �����Ѵ�.
	void DeleteEmotbyItemID( GCITEMID itemID );				// �ش� ItemID�� ���� Emot�� �����Ѵ�.

	bool FindSameItemType( GCITEMID itemID );					// POTION ���� �������� ���̴� �������� ��� ���� ������ �ִ��� ã�´�.

	INT  CanbeInsertItem( GCITEMID itemID );					// ItemSlot�� ���ο� Item�� �� �� �ִ� Index�� ��ȯ
	INT  CanbeInsertEmot( GCITEMID itemID );					// EmotSlot�� ���ο� Item�� �� �� �ִ� Index�� ��ȯ

	// public InsertItem �� �� �޾Ƽ� ���� ���� �Ⱦ ������ �� ��
	bool InsertItem( int index, SCS_ITEM_KIND itemType, KItem* kInventoryItem );

	
	// �� �÷��̾��� ������ �����Ѵ�.
	void SetMyPlayerInfo();


	// ��Ŷ ���� ����
public:
	void initQuickSlot(KQuickSlot Src);						// �����κ��� QuickSlot�� ������ �޾ƿ´�.
	void SendQuickSlotinfo();								// ���ཽ���� ������ ������ �����Ѵ�.

    std::string Dump() const;                               // ũ�����α�

protected:
	KQuickSlot m_kQuickSlot;
public:
    INT FindItemIndexByItemUID( GCITEMUID itemUID_ );
    INT FindEmotIndexByItemUID( GCITEMUID itemUID_ );
    bool isExistByItemUID( GCITEMUID itemUID_ );

    int GetCurEquipItemNum(DWORD dwitemID_ );				// ���������ϰ��ִ� ���ƾ������� ���� �����Ѵ�.

    bool IsSpecialSkillItem( GCITEMID dwItemID_ );
    bool IsEquipSpecialSkillItem( int iIndex_ );

    int InsertSpecialSkill( KItem* kItem_ );
    bool UseSpecialSkillItem( INT index );

    int GetSpecialSkillItemCoolTime( GCITEMID dwItemID_ );

    int SetDamageOREmoticonAppearanceState(SHORTCUT_ITEM* pItemInfo );

};

#endif // _GCSHORTCUTSLOT_H_
