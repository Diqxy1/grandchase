#ifndef _POTION_BUFF_H_
#define _POTION_BUFF_H_

#include "GCShortCutItem.h"
#include "GCShortCutItemDefine.h"

class KPotion_Buff : public KGCShortCutSlotItem
{
public:
	KPotion_Buff(KItem*, PLAYER_INFO*, SHORTCUT_ITEM*);
	virtual ~KPotion_Buff();

public:
	virtual bool Do();

	virtual void SetItemID(INT _itemID) { kItem.m_ItemID = _itemID; }
	virtual void SetItemUID(INT _itemUID) { kItem.m_ItemUID = _itemUID; }
	virtual void SetItemKind(INT _itemKind) { itemKind = _itemKind; }
	virtual void SetQuantity(INT _quantity) { kItem.m_nCount = _quantity; }
	virtual void SetCoolTime(INT _CoolTime = -1);

	virtual void DecreaseCoolTime(INT Num = 1);
	virtual bool DecreaseQuantity();
	virtual void SetDamageOREmoticonAppearance(int iRenderDamage);

	virtual std::string Dump() const;

protected:
	bool HP();
	bool MP();
	bool Attack();
	bool Defense();
	bool Cancelation();
	bool SetBuff();
	bool UseSkill();
	bool Use4SPSkill(void);
	bool UseDamagePotion(void);
	bool UseSummonPotion(void);


	void PlaySoundAll();
	void PlaySound();
	bool PetMP();

protected:
	SHORTCUT_ITEM* m_ItemStruct;
	PLAYER_INFO* m_MyPlayer;

private:

	bool bRenderDamage;

};


#endif