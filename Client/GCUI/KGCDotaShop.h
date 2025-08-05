#ifndef _GCKGCDOTASHOP_H_
#define _GCKGCDOTASHOP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCItemImgWnd.h"
#include "KGCDPointItemBox.h"

#define  MAX_ITEMBOX 8

class KGCDPointItemBox;
class KGCDotaShop : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCDotaShop );
    /// Default constructor
    KGCDotaShop( void );
    /// Default destructor
    virtual ~KGCDotaShop( void );
    /// Copy constructor
    KGCDotaShop( const KGCDotaShop& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDotaShop& operator=( const KGCDotaShop& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void InitData();
    void CreateItemBox();
    void CreateItemImg();
    void RefreshItemBox();
    void RefreshMyPoint();
    void RefreshSelectItem();
    inline bool IsSelectItem()
    {
        return ( 0 <= m_iSelectItem && m_iSelectItem < int(m_sDSItemInfo.vecItemList.size()) );
    }

public:
    void UseItem();
    bool KeyControll();
    int GetItemName( int iGameItemIdx );
    void CloseUI();

public:
    void OnClickBuy();
    void OnClickItemBox( int iBoxIdx );
    void OnRollOverItemBox( int iBoxIdx );
    void OnClickClsoe();
    void OnEnterCursor();

private:
    KD3DWnd               *m_pkBack;
    KGCItemImgWnd         *m_pkImgSelectItem;
    KD3DScrollbar         *m_pkScroll;
    KD3DWnd               *m_pkBtnClose;
    KD3DWnd               *m_pkBtnBuy;
    KD3DStatic            *m_pkStaticMyPoint;
    KD3DStatic            *m_pkStaticItemPoint;
    KD3DStatic            *m_pkStaticPoint;
    KD3DStatic            *m_pkStaticItem;
    KD3DStatic            *m_pkStaticItemName;

    int                    m_iScrollIdx;
    int                    m_iSelectItem;
    SDota_D_PointShopItemInfo m_sDSItemInfo;
    std::vector<KGCDPointItemBox*> m_vecItemBox;
    std::vector<GCDeviceTexture*>  m_vecpItemImg;

private:
    int m_iAlpha;
    int m_iViewCoolTime;
    KGCItemImgWnd         *m_pkShowBuyItem;
    KD3DStatic            *m_pkShowBuyItemDesc;
    KD3DStatic            *m_pkShopDesc;

public:
    void SetBuyItem( int iGameItemIdx, GCDeviceTexture* pTexture, int iAlpha, int iViewCoolTime );
};

DEFINE_WND_FACTORY( KGCDotaShop );
DECLARE_WND_FACTORY( KGCDotaShop );
DECLARE_WND_FACTORY_NAME( KGCDotaShop );

#endif  // _GCKGCDOTASHOP_H_