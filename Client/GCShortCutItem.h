#ifndef _GCSHORTCUTSLOTITEM_H_
#define _GCSHORTCUTSLOTITEM_H_

typedef int INT;
typedef unsigned int UINT;
typedef unsigned long DWORD;

#define SECOND(X)  (60*X)

//#include <sstream>
#include "GCItemManager.h"
class KGCShortCutSlotItem
{
public :
	KGCShortCutSlotItem(void)
	{
        kItem.m_ItemID = KItem::INVALID_ITEM;
        kItem.m_ItemUID = KItem::INVALID_ITEM;
		itemKind = -1;
		kItem.m_nCount = 0;
        kItem.m_nPeriod = 0;
		nowCoolTime = 0;
		iUsedCount = 0;
	}
	virtual ~KGCShortCutSlotItem(void){}

	virtual void Set(void){return;}
	virtual bool Do(void){return false;}

	virtual	INT  GetItemID(void){return kItem.m_ItemID;}
	virtual GCITEMUID GetItemUID(void){return kItem.m_ItemUID;}
	virtual INT  GetItemKind(void){return itemKind;}
	virtual INT  GetQuantity(void){return kItem.m_nCount;}
	virtual INT  GetCoolTime(void){return nowCoolTime;}
	virtual INT	 GetUsedTimeCount(void) {return iUsedCount;}

	virtual void SetItemID(INT ID){kItem.m_ItemID = KItem::INVALID_ITEM; }
    virtual void SetItemUID(GCITEMUID UID){kItem.m_ItemUID = KItem::INVALID_ITEM; }
	virtual void SetItemKind(INT Kind){itemKind = -1;}
	virtual void SetQuantity(INT Quantity){kItem.m_nCount = 0;}
	virtual void SetCoolTime(INT Cooltime=0){nowCoolTime = 0;}

	virtual void DecreaseCoolTime(INT Num=1){;}
	virtual bool DecreaseQuantity(){return true;}

    virtual void SetDamageOREmoticonAppearance(int iRenderDamage){}

    virtual std::string Dump() const
    {
        std::stringstream stm;
        stm << "ItemID : " << kItem.m_ItemID << " ItemUID : " << kItem.m_ItemUID
            << " ItemKind : " << itemKind << " count : " << kItem.m_nCount;
        return stm.str();
    }

protected:
	KItem	kItem;
	INT itemKind;
	INT nowCoolTime;
	int iUsedCount;
};

#endif // _GCSHORTCUTSLOTITEM_H_
