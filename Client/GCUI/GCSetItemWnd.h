#ifndef _GCSETITEMWND_H_
#define _GCSETITEMWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class KD3DStatic;
class KGCItemBar;
class GCDeviceTexture;
class KGCUINumber;

class KGCSetItemWnd : public KD3DWnd,           // extends
                      public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCSetItemWnd );
    /// Default constructor
    KGCSetItemWnd( void );
    /// Default destructor
    virtual ~KGCSetItemWnd( void );
    /// Copy constructor
    KGCSetItemWnd( const KGCSetItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSetItemWnd& operator=( const KGCSetItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_LAYER_WND       = 9,
        NUM_ACTIVE_FRAME    = 8,
    };

public:
    void SetIndex( int iIndex );
    bool IsEquip( void ) const;
    void Equip( bool bEquip );
    GCItem*& GetItemInfo( void );
	DWORD GetItemKey( void )
	{
		return m_iItemID;
	}
	int GetItemID() { return m_iItemID; }
    virtual void ActionPerformed( const KActionEvent& event );
	void SetItemInfo(int iShopItem);
	void SetMask(const int& RecomLevel_);
    void SetVIP(bool bVIP);
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
	D3DXVECTOR4 GetTextureCoords( int iSetNum );

protected:
    int         m_iIndex;
	int			m_iItemID;
	int			m_iPeriod;
	bool		m_bEquip;
	bool		m_bShow;
	GCItem*		m_kItemInfo;

    KD3DStatic* m_pkItemName;
    KD3DStatic* m_pkItemLimit;
    KGCItemBar* m_pkItemBar;
    KD3DWnd*    m_pkEquip;
	GCDeviceTexture* m_pTextureItem;
    KGCUINumber*    m_pkPriceNum;

	KD3DSizingBox	*m_pkLayer;
	KD3DWnd*    m_pkImgDrawRect;
	KD3DSizingBox	*m_pkActiveLayer;

public:
    KD3DWnd*    m_pkBuyButton;
    KD3DWnd*    m_pkPresentButton;
	KD3DWnd*    m_pkGambleButton;

	KD3DSizingBox*    m_pkNew;
	KD3DSizingBox*    m_pkFit;
	KD3DSizingBox*    m_pkHot;
    KD3DSizingBox*    m_pkVIP;
	KD3DWnd*          m_pkSale;
};

DEFINE_WND_FACTORY( KGCSetItemWnd );
DECLARE_WND_FACTORY( KGCSetItemWnd );
DECLARE_WND_FACTORY_NAME( KGCSetItemWnd );

#endif // _GCSETITEMWND_H_
