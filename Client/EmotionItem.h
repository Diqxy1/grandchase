#pragma once
#include "gcshortcutitem.h"
#include "GCShortCutItemDefine.h"

struct PLAYER_INFO;
struct KInventoryItem;

class EmotionItem :
    public KGCShortCutSlotItem
{
public:
    EmotionItem(  KItem* _kInven, PLAYER_INFO* _pMyPlayer, SHORTCUT_ITEM* _pStruct );
    virtual ~EmotionItem(void);

public:
    virtual void Set();
    virtual bool Do();

    virtual void SetItemID(INT _itemID){kItem.m_ItemID = _itemID;}
    virtual void SetItemUID(INT _itemUID){kItem.m_ItemUID = _itemUID;}
    virtual void SetItemKind(INT _itemKind){itemKind = _itemKind;}
    virtual void SetQuantity(INT _quantity){kItem.m_nCount = 1;}
    virtual void SetCoolTime(INT _CoolTime = -1);

    virtual void DecreaseCoolTime(INT Num=1);
    virtual bool DecreaseQuantity();

    //virtual std::string Dump() const;

protected:
    SHORTCUT_ITEM* m_ItemStruct;
    PLAYER_INFO* m_MyPlayer;

};
