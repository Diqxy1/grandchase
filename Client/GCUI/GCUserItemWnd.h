#ifndef _GCUSERITEMWND_H_
#define _GCUSERITEMWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"
#include "GCUserItemWndInterface.h"

class KD3DStatic;
class KGCItemBar;
class GCDeviceTexture;
class KGCUINumber;
class KGCItemImgWnd;
struct KInventoryItem;

class KGCUserItemWnd : public KGCUserItemWndInterface,          // extends
                       public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCUserItemWnd );
    /// Default constructor
    KGCUserItemWnd( void );
    /// Default destructor
    virtual ~KGCUserItemWnd( void );
    /// Copy constructor
    KGCUserItemWnd( const KGCUserItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUserItemWnd& operator=( const KGCUserItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_LAYER_WND           = 9,
        NUM_ACTIVE_FRAME        = 8,
        ENCHANT_BOX_OFFSET_X    = 155,
        ENCHANT_BOX_OFFSET_Y    = -80,
    };
	
	enum ITEM_BTN_STATE
	{
		IBS_DEFAULT			=0,
		IBS_ATTRIBUTE		=1,
		IBS_SOCKET			=2,
		IBS_RANDOM_INIT_ATTRIBUTE	=3,
        IBS_LEVEL_DOWN      = 4,
	};

public:
    GCITEMID GetItemID() { return m_iItemID; }
    void OnSetItem();
    void OnSetItem( KItem* pInventoryItem);
    bool IsCursorRollover();
	//bool IsTreasureBox( int iType );
	void SetUpdateBtn( const KActionEvent& event );
    void OnCoordiUndoButton();
	GCITEMUID               m_dwUID;
	void SetItemDuration( int iDuration );
	int GetItemDuration() { return m_iDuration; };
	void OnSellStart();
	void OnSellEnd();
    bool GetSellState(){ return m_bSellState; };
    void SetSelect(bool _select){ m_bSelectState = _select; }; 
    bool IsSelect(){return m_bSelectState;};
    void UpdateSelectIcon();
protected:
    void ShowItem(bool bShow);
    void UpdateButton( bool bShow );

	void SetItemBtn(int state_ = IBS_DEFAULT); 

	bool SetSocketBtn( KItem* pInven );
	bool SetAttributeBtn( KItem* pInven ); 
	bool SetRandomInitAttributeBtn( KItem* pInven,int state_ ); 
    bool SetLevelDownBtn( KItem* pInven );
	bool SetEquipBtn(KItem* pInven);


	void RegisterMission();
    std::wstring GetPetState( int iSatiation );
    bool IsShowConfirmButton( int iItemID );
    bool IsShowCraftingButton( int iItemID );
    KItem* GetItemInfo( void );
    GCItem* GetItemData() { return m_ItemInfo; }
    void UpdateSellIcons();
    void SetGradeFrame( IN char cGradeID_ );
    void UsePromotionMagic(GCITEMID ItemID_, int iPromotionIdx);
    void OnConfirmButton();
//  void OnSellButton();
    void OnEnchantButton();
    void OnDeleteButton();
    void OnCraftingButton();
    void OnDeletePointItemButton();
    void OnEatFoodButton();
    void OnAttributeButton();
    void OnPlayButton();
    void OnSocketButton();

    virtual void OnCreate( void );
	virtual void OnCreateComplete(void);
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

    bool SetEnterCursor();
    bool SetLeaveCursor();
	void OnRandomInitAttributeButton(const KActionEvent& event);
    void OnItemLevelDownClick();
public:
	void OffItemBtn( );

protected:

    int                 m_iItemID;
    int                 m_iStrong;
    int					m_iDuration;
    bool                m_bSellState;
    bool                m_bSelectState;

    enum {
        ITEM_UNIQUE         = 0x00000001,
    };

    bool                m_bUnique;
    bool                m_bPeriodItem;
    bool                m_bShow;
    bool                m_bEnchant;
    bool                m_bEnchantBreak;
    bool                m_bEvent;
    bool                m_bPointItem;
    bool                m_bNotSellItem;

    KD3DSizingBox*      m_pkFrame;
    KD3DSizingBox*      m_pkGradeFrame[KItem::GRADE_NUM];
    KD3DSizingBox*      m_pkActiveFrame;

    KD3DStatic*         m_pkItemName;
    KD3DStatic*         m_pkRemainTime;
    KD3DStatic*         m_pkItemLimit;
    KGCItemBar*         m_pkItemBar;

    KD3DWnd*            m_pkDeleteButton;
    KD3DWnd*            m_pkSellButton;
    KD3DWnd*            m_pkEnchantButton;
    KD3DWnd*            m_pkConfirmButton;
    KD3DWnd*            m_pkEatFoodButton;
	KD3DWnd*            m_pkAttributeButton;
	KD3DWnd*            m_pkRandomInitAttributeButton;
    KD3DWnd*            m_pkCraftingButton;

    KD3DWnd*            m_pkLevelDownButton;
    KD3DWnd*            m_pkLevelDownOver;

	KD3DWnd*			m_pkPlayButton;
    KD3DWnd*            m_pkSocketButton;

    KD3DWnd*            m_pkLimitIcon;
    KD3DWnd*            m_pkPocketIcon;
    KD3DWnd*            m_pkTimeRed;
    KD3DWnd*            m_pkTimeYellow;
    KD3DWnd*            m_pkTimeGreen;

    KD3DWnd*            m_pkNowSelling;
    KD3DWnd*            m_pkNowRemove;
    KD3DWnd*            m_pkCantSell;
    KD3DWnd*            m_pkNowEquip;
    KD3DWnd*            m_pkSelected;

    KD3DWnd*            m_pkImgCoordiCompose;
	KD3DWnd*			m_pkBtnEquip;

    KD3DWnd*            m_pkImgCoordiUndo;

    KGCUINumber*        m_pkDurationNum;
    KGCUINumber*        m_pkSmallDurationNum;
    KGCUINumber*        m_pkStrongNum;
    KGCUINumber*        m_pkPriceNum;
    KGCUINumber*        m_pkEquipEnchantItemLv;

    int                 m_iPromotionLevel;
    enum
    {
        PROMOTION_MAGIC_1=1,
        PROMOTION_MAGIC_2,
        PROMOTION_MAGIC_3,
    };
};

DEFINE_WND_FACTORY( KGCUserItemWnd );
DECLARE_WND_FACTORY( KGCUserItemWnd );
DECLARE_WND_FACTORY_NAME( KGCUserItemWnd );

#endif // _GCUSERITEMWND_H_
