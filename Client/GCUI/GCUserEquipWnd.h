#ifndef _GCUSEREQUIPWND_H_
#define _GCUSEREQUIPWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"
#include "KGCSocketWnd.h"

class KD3DStatic;
class KGCItemBar;
class GCDeviceTexture;
struct KInventoryItem;
class KGCUINumber;
class KGCItemImgWnd;

class KGCUserEquipWnd : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCUserEquipWnd );
	/// Default constructor
	KGCUserEquipWnd( void );
	/// Default destructor
	virtual ~KGCUserEquipWnd( void );
	/// Copy constructor
	KGCUserEquipWnd( const KGCUserEquipWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCUserEquipWnd& operator=( const KGCUserEquipWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum
	{
		NUM_LAYER_WND           = 9,
		NUM_ACTIVE_FRAME        = 8,
		ENCHANT_BOX_OFFSET_X    = 197,
		ENCHANT_BOX_OFFSET_Y    = -59,
	};

public:
	KItem* GetItemData( void );
	GCITEMID GetItemID() const { return m_iItemID; }
    GCITEMUID GetItemUID() const { return m_dwUID; }
	GCItem*& GetItemInfo() { return m_kItemInfo;	}
	virtual void ActionPerformed( const KActionEvent& event );
	void SetItem(KItem*& pInventoryItem);
    void SetItem();
	// ShortCutSlot을 위한 함수 오버로딩 : 모드가 true면 Item false면 Emoticon
	void SetItem( int index, bool bMode );
    void SetSelect( bool bSelect ) { m_bSelect = bSelect; }

protected:
	void ShowItem( bool bShow );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );

protected:

	int         m_iItemID;
	GCItem*		    m_kItemInfo;

	bool        m_bShow;
	bool        m_bMouseHoverPrev;
	bool        m_bAlreadyPatternUp;

	bool        m_bEnchant;
	int			m_iStrong;
    int         m_iDuration;
	bool		m_bUnique;
	bool		m_bPeriodItem;
    std::wstring m_strRemainTime;

	bool        m_bEnchantBreak;
	bool        m_bNotSaleForLevel;
	bool        m_bEvent;

	bool		m_bShortCut;
	int			m_iMyIndex;
    bool        m_bEmoticon;
    bool        m_bSelect;

    KD3DSizingBox*  m_pkActiveFrame;

    KGCItemImgWnd*  m_pkItemImg;
    KGCUINumber*    m_pkDuration;
    KGCUINumber*    m_pkStrong;
	KD3DStatic* m_pkItemName;
	KD3DStatic* m_pkRemainTime;
	GCDeviceTexture* m_pTextureItem;

    KD3DWnd*        m_pkImgCoordiCompose;

    GCITEMUID       m_dwUID;

    KGCUINumber*  m_pkEquipEnchantItemLv;

	KD3DWnd*    m_pkImgDrawRect;
public:
	
	/*KD3DWnd*    m_pkSellButton;
	KD3DWnd*    m_pkPatternButton;
	KD3DWnd*    m_pkEnchantButton;
	KD3DWnd*    m_pkRepairButton;
	KD3DWnd*    m_pkConfirmButton;*/
};

DEFINE_WND_FACTORY( KGCUserEquipWnd );
DECLARE_WND_FACTORY( KGCUserEquipWnd );
DECLARE_WND_FACTORY_NAME( KGCUserEquipWnd );

#endif // _GCUSEREQUIPWND_H_
