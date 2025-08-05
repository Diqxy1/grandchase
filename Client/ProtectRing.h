#ifndef _PROTECTRING_H_
#define _PROTECTRING_H_

#include "GCShortCutItem.h"
#include "GCShortCutItemDefine.h"
//#include "GCItem.h"
//#include "GCItemManager.h"


class KProtectRing : public KGCShortCutSlotItem
{
public :
	KProtectRing( KItem*, PLAYER_INFO*, SHORTCUT_ITEM* );
	virtual ~KProtectRing();

public:
	virtual void Set();
	virtual bool Do();

	virtual void SetItemID(INT _itemID){kItem.m_ItemID = _itemID;}
	virtual void SetItemUID(INT _itemUID){kItem.m_ItemUID = _itemUID;}
	virtual void SetItemKind(INT _itemKind){itemKind = _itemKind;}
	virtual void SetQuantity(INT _quantity){kItem.m_nCount = _quantity;}
	virtual void SetCoolTime(INT _CoolTime = -1);

	virtual void DecreaseCoolTime(INT Num=1);
	virtual bool DecreaseQuantity();

    virtual std::string Dump() const;

protected :
	// Item 사용에 필요한 값을 가진 structure들
	SHORTCUT_ITEM* m_ItemStruct;
	PLAYER_INFO* m_MyPlayer;
};


#endif	// _POTION_BUFF_H_
