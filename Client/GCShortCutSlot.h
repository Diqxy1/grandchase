//////////////////////////////////////////////////////////////////////////
//							crey21	20071119							//
//////////////////////////////////////////////////////////////////////////

#ifndef _GCSHORTCUTSLOT_H_
#define _GCSHORTCUTSLOT_H_

//#include "GCItem.h"
//#include "GCItemManager.h"
//#include "GCShortCutItem.h"			// Slot에 들어가는 Item의 \Virtual Fucntion들을 정의하고 있오.
#include "GCShortCutItemDefine.h"	// Slot에 들어가는 Item의 정보 구조체를 관리하고 있오.

class KGCShortCutSlotItem;

// ShortCutSlot에 장착할 수 있는 ItemID 목록
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


// 값을 넣어주는 곳은 GCUserItemBox
// 값을 빼는 경우는 GCUserEquipBox 혹은 게임 내에서 아이템이 사용되는 경우이다.
class KGCShortCutSlot
{
public :
	KGCShortCutSlot();
	~KGCShortCutSlot();

	void initialization();									// Normal Item Slot과 Emoticon Slot을 초기화 한다.
	void Clear();											// 클리어라고
	void ClearItem();										// Normal Item Slot을 초기화 한다.
	void ClearEmoticon();									// Emoticon Slot을 초기화 한다.

	void SwapItem( INT& indexFrom, INT& indexTo );			// Slot의 두 Index를 입력받아 서로 위치를 바꿔준다.

	bool UseItem( INT index );								// 해당 슬롯에 있는 Item을 사용한다.
	void UseEmot( INT index );								// 해당 슬롯에 있는 Emoticon을 사용한다

	KGCShortCutSlotItem* FindItem( INT index );				// Item Slot Index를 받아서 해당 Index의 Item 객체 포인터를 반환.
	KGCShortCutSlotItem* FindEmot( INT index );				// Emot Slot Index를 받아서 해당 Index의 Item 객체 포인터를 반환.
	INT  FindItemIndex( DWORD itemID );						// ItemID를 받아 해당 Item을 가지고 있는 Slot을 찾는다.
	INT  FindEmotIndex( DWORD itemID );						// ItemID를 받아 해당 Emot을 가지고 있는 Slot을 찾는다.

	INT	 InsertItem( KItem* );						// Item을 Slot에 삽입하고 Index를 반환한다.
	void DischargeItem( DWORD itemID );						// ItemID에 해당하는 Item을 Slot에서 제거하고 수량을 돌려준다.
	bool ExchangeItem( IN KItem* kInven );					// 입력된 Item과 같은 Type의 Item을 DisCharge 시키고 입력된 Item을 Insert.
	bool SlotRefill();										// Slot에서 사용된 아이템을 자동으로 리필해준다.

    INT	 InsertEmot( KItem* );						// Emoticon을 Slot에 삽입하고 Index를 반환한다.
    void DischargeEmot( DWORD itemID );						// ItemID에 해당하는 이모티콘을 Slot에서 제거하고 수량을 돌려준다.
        


	INT  GetItemID( INT index );							// 해당 슬롯의 Item이 가진 ID값을 반환.
	GCITEMUID  GetItemUID( INT index );							// 해당 슬롯의 Item이 가진 UID값을 반환.
	INT  GetItemQuantity( INT index );						// 해당 슬롯의 Item 수량을 반환.
	INT	 GetItemCoolTime( INT index );						// 해당 슬롯의 Item에 CoolTime이 얼마나 남았는지 반환.
	bool isItemCoolTime( INT index );						// 해당 슬롯의 Item이 CoolTime 상태인지 아닌지를 반환.

	INT  GetEmotID( INT index );								// 해당 슬롯의 Emot이 가진 ID값을 반환.
	GCITEMUID  GetEmotUID( INT index );							// 해당 슬롯의 Emot이 가진 UID값을 반환.
	INT  GetEmotQuantity( INT index );						// 해당 슬롯의 Emot 수량을 반환.
	INT  GetEmotCoolTime( INT index );						// 해당 슬롯의 Emot에 CoolTime이 얼마나 남았는지 반환.
	bool isEmotCoolTime( INT index );						// 해당 슬롯의 Emot이 CoolTime 상태인지 아닌지를 반환.

	bool isExist( DWORD item );								// ShortCutSlot 안에 해당 Item이 존재하는지 검사.
	bool isProtectRing( INT index );						// 해당 index의 Item이 수호의 반지 계열인지 검사
	INT  FindItemType( INT itemType );						// ItemType을 받아 해당 Type을 가지고 있는 Slot을 찾는다.
	INT  FindEmotType( INT itemType );						// ItemType을 받아 해당 Type을 가지고 있는 Slot을 찾는다.

    void FrameMove();

	bool IsGameModeAllowed( EGCGameMode eGameMode, int index);
	bool HasAllowedItem(EGCGameMode eGameMode);

	bool CanUseItem(int iIndex);
private :
/////////////////////
// Member Variables
	KSafeArray<KGCShortCutSlotItem*,ITEM_SLOT_SIZE> m_ItemSlot;		// Slot Item의 객체 포인터 배열
	KSafeArray<KGCShortCutSlotItem*,EMOT_SLOT_SIZE> m_EmotSlot;		// Slot Emoticon의 객체 포인터 배열

	KSafeArray<INT,ITEM_SLOT_SIZE> m_ItemSlotIndex;					// ItemSlot List의 각 객체에 indirect 접근하기 위한 Index 배열
	KSafeArray<INT,EMOT_SLOT_SIZE> m_EmotSlotIndex;					// EmoticonSlot List의 각 객체에 indirect 접근하기 위한 Index 배열

	PLAYER_INFO	m_MyPlayer;									// Item 사용에 필요한 내 플레이어의 정보 구조체
	KGCShortCutItemDefine m_kItemDefine;					// ItemID로 적절한 Item 정보 구조체를 찾을 수 있는 class 객체

	INT m_Packet_OK;										// 패킷 전송결과가 무엇인지


/////////////////////
// Member Functions
public:
	void DeleteItembyIndex( INT index );					// 해당 Index에 있는 Item을 삭제한다.
	void DeleteEmotbyIndex( INT index );					// 해당 Index에 있는 Emot을 삭제한다.
	void DeleteItembyItemID( GCITEMID itemID );				// 해당 ItemID를 가진 Item을 삭제한다.
	void DeleteEmotbyItemID( GCITEMID itemID );				// 해당 ItemID를 가진 Emot을 삭제한다.

	bool FindSameItemType( GCITEMID itemID );					// POTION 같이 종류별로 묶이는 아이템인 경우 같은 종류가 있는지 찾는다.

	INT  CanbeInsertItem( GCITEMID itemID );					// ItemSlot에 새로운 Item이 들어갈 수 있는 Index를 반환
	INT  CanbeInsertEmot( GCITEMID itemID );					// EmotSlot에 새로운 Item이 들어갈 수 있는 Index를 반환

	// public InsertItem 에 죽 달아서 쓰니 보기 싫어서 밖으로 뺀 것
	bool InsertItem( int index, SCS_ITEM_KIND itemType, KItem* kInventoryItem );

	
	// 내 플레이어의 정보를 수집한다.
	void SetMyPlayerInfo();


	// 패킷 관련 정보
public:
	void initQuickSlot(KQuickSlot Src);						// 서버로부터 QuickSlot의 정보를 받아온다.
	void SendQuickSlotinfo();								// 단축슬롯의 정보를 서버로 전송한다.

    std::string Dump() const;                               // 크래쉬로그

protected:
	KQuickSlot m_kQuickSlot;
public:
    INT FindItemIndexByItemUID( GCITEMUID itemUID_ );
    INT FindEmotIndexByItemUID( GCITEMUID itemUID_ );
    bool isExistByItemUID( GCITEMUID itemUID_ );

    int GetCurEquipItemNum(DWORD dwitemID_ );				// 현재장착하고있는 숏컷아이템을 수를 리턴한다.

    bool IsSpecialSkillItem( GCITEMID dwItemID_ );
    bool IsEquipSpecialSkillItem( int iIndex_ );

    int InsertSpecialSkill( KItem* kItem_ );
    bool UseSpecialSkillItem( INT index );

    int GetSpecialSkillItemCoolTime( GCITEMID dwItemID_ );

    int SetDamageOREmoticonAppearanceState(SHORTCUT_ITEM* pItemInfo );

};

#endif // _GCSHORTCUTSLOT_H_
