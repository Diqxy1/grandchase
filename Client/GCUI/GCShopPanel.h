#ifndef _GCSHOPPANEL_H_
#define _GCSHOPPANEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../GCItem.h"
#include "GCShopItemBox.h"
//#include <map>

class KGCShopTab;

class KGCShopPanel : public KD3DWnd,        // extends
                     public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCShopPanel );
    /// Default constructor
    KGCShopPanel( void );
    /// Default destructor
    virtual ~KGCShopPanel( void );
    /// Copy constructor
    KGCShopPanel( const KGCShopPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCShopPanel& operator=( const KGCShopPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EShopTapType
    {
        STT_ARMOR,
        STT_SET,
        STT_WEAPON,
        STT_ACCESSORY,
        STT_TRAINING,
        NUM_SHOP_TAB,
    };

public:
    inline EShopTapType GetShopTabType( void ) const { return m_eShopTabType; }
    void FilterGoodsItem( EShopTapType eShopTabType );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnActivateComplete( void );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    void SetShopClerkStr( const GCItem* item );
    void SetActiveShopTab( KGCShopTab* pkActiveTab );

protected:
    KGCShopTab* m_pkTabArmor;
    KGCShopTab* m_pkTabSet;
    KGCShopTab* m_pkTabWeapon;
    KGCShopTab* m_pkTabAccessory;
    KGCShopTab* m_pkTabTraining;

    KGCShopTab* m_pkCurActiveTab;
    KSafeArray<KGCShopTab*,NUM_SHOP_TAB> m_apkShopTabList;

    KGCShopItemBox* m_pkItemBox;
    KSafeArray<KD3DWnd*,KGCShopItemBox::NUM_ITEM_WND> m_apkNewGoods;

    UCHAR           m_ucCharNum;
    EShopTapType    m_eShopTabType;

    std::map<KGCShopTab*, EShopTapType> m_mapTabType;
};

DEFINE_WND_FACTORY( KGCShopPanel );
DECLARE_WND_FACTORY( KGCShopPanel );
DECLARE_WND_FACTORY_NAME( KGCShopPanel );

extern KGCShopPanel* g_pkShopPanel;

#endif // _GCSHOPPANEL_H_