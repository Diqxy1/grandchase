#ifndef _GCMYINFOSCENE_H_
#define _GCMYINFOSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"
#include "GCUI/GCUserEquipBox.h"
#include "GCUI/KGCBillBoardChatBoard.h"

class KGCUserItemBox;
class KGCUserEquipBox;

class KGCShopTab;
class KGCInfoPanel;
class KGCStatusInfoPanel;
class KGCCharWnd;
class KD3DComboBox;
class KGCMissionDlg;
class KGCInventoryCapacity;
class KGCEmoticonListBox;
class KGCEmoticonWindow;
class KGCSellInfo;
class KGCSearchBar;
class KGCMyInfoSceneSubMenu;
class KGCPopupAttributeSelect;
class KGCItemPopupMenu;

class KGCMyInfoScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCMyInfoScene );
    /// Default constructor
    KGCMyInfoScene( void );
    /// Default destructor
    virtual ~KGCMyInfoScene( void );
    /// Copy constructor
    KGCMyInfoScene( const KGCMyInfoScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMyInfoScene& operator=( const KGCMyInfoScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        TAB_INVENTORY,
        TAB_OTHERITEM,
        TAB_EXTRAITEM,
		TAB_OTHERITEM2,
        TAB_NUM,
    };

    void ActionPerformed( const KActionEvent& event );	
    void SetInvenWndCategory( USER_INVEN_CATEGORY ItemCategory );
    void OnInitialize( void );
    //////////////////////////////////////////////////////////////////////////
    // 오후 2:57 2008-03-17 by jemitgge
    // 이모티콘 클릭하면 바로 장비창의 이모티콘 탭으로 이동하기 위해서 탭의 스크로 포즈를 받는다 -_-;;
    // 두 함수 변경되었습니다.
    void SetEquipWndCategory( USER_EQUIP_CATEGORY ItemCategory, int iScrollPos = 0 );
    void UpdateGameData(bool bSaveEquip = false, USER_EQUIP_CATEGORY eEquipCategory_ = UEC_NUM, int iScrollPos = 0/*, USER_INVEN_CATEGORY eInvenCategory_ = UIC_NUM*/);
    //////////////////////////////////////////////////////////////////////////
    void OnEnterScene( void );
    void OnLeaveScene( void );
    void UpdateData();
    void SetTitleItemToCharWnd( int iItemID );
    void UpdateCharInfo();
    void FindItemFromName( const WCHAR* strName_ );
    void UpdateInventoryCapacity( bool bCoordi_ = false );

	void UpdateCharacterSelect( int _iCharType );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    void OnChangeChar();
    void SetComboPopupList();
    void ChangeInventoryItem( int iGroupType );
    void InitInventoryItem();
    void DrawInfoWindow();
    void UpdatePetWnd();
    void SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_);

    void ActiveInventoryTab( int iTabID );
    int  GetNowInventoryTab();

    bool OnClickSell_OpenSellUI( const KActionEvent& kEvent_ );


	bool OnClickSell_CloseSellUI( const KActionEvent& kEvent_ );


	void OnClickSellItem( const KActionEvent& kEvent_ );
    void OnClickSellBtn();
    bool OnRecvSellPacket( const KActionEvent& kEvent_ );
    void OnRefreshSellItemList();

    void OnClickInventoryItem( const KActionEvent& kEvent_ );
    void OnRClickInventoryItem( const KActionEvent& kEvent_ );
    void OnClickInventoryItem_ForSell( const KActionEvent& kEvent_ );
    bool OnClickLookItem( GCITEMUID UID_ );
    bool OnClickPetItem( GCITEMUID UID_ );
    bool OnClickPetEquipItem( GCITEMUID UID_ );
    bool OnClickMission( GCITEMUID UID_ );
    bool OnClickShortCutItem( GCITEMUID UID_ );
    bool OnClickEmoticonItem( GCITEMUID UID_ );
    bool OnClickSkillItem( GCITEMUID UID_ );
    bool OnClickEquipment( GCITEMUID UID_ );
    bool OnClickItemWnd( GCITEMUID UID_ );
    bool OnClickPVPDisabledItem( GCITEMUID UID_ );
    bool OnClickNotExistCharItem( GCITEMUID UID_ );

    void OnRecommend();

public:
    void SellInvenItem( const KItem& kItem );  
    void RecieveSellInvenItem();

public:
	void OnClickOpenSellUI( const KActionEvent &kEvent_, KD3DWnd *pkBtn_Sell=NULL ); 
	void OnClickCloseSellUI(); 
    KGCInfoPanel*		m_pkInfoPanel;
    KGCStatusInfoPanel* m_pkStatusPanel;
	USER_EQUIP_CATEGORY GetEquipCategory(){ return m_EquipCategory; }
    USER_INVEN_CATEGORY GetInvenCategory(){ return m_InvenCategory;  }
	KGCUserEquipBox*	m_pkUserEquipBox;

    KGCPopupAttributeSelect*    m_pkPopopAttributeSelect;
    USER_INVEN_CATEGORY	m_InvenCategory;
    USER_EQUIP_CATEGORY	m_EquipCategory;
protected:	
    KGCSellInfo*        m_pkSellInfo;
	KGCUserItemBox*		m_pkUserItemBox;
    KGCUserItemBox*     m_pkSellItemBox;

	KSafeArray<KD3DWnd*,UEC_NUM> m_apEquipTab;
    KSafeArray<KD3DWnd*,UIC_NUM> m_apInvenTab;
    KSafeArray<KD3DWnd*,TAB_NUM> m_pkInventoryTab;
	//KGCMissionDlg*	m_pkMissionDlg;

    int                 m_iItemGroupType;

    KD3DWnd*            m_pkEmoticonWindowButton;
    KGCEmoticonWindow*  m_pkEmoticonWindow;

	KD3DComboBox*		m_pkItemGroupCombo;

    KGCInventoryCapacity*   m_pkInventoryCapacity;

    KD3DWnd*            m_pkWelcomeBack_btn;
    KD3DWnd*            m_pkBtn_Sell;
    KD3DWnd*            m_pkBtn_Ordering_Level;
    KD3DWnd*            m_pkBtn_Ordering_Parts;
    KD3DWnd*            m_pkBtnLevelDown;
    KD3DWnd*            m_pkBtnLevelDownOn;

    //Sistema de Skin de Dano
    KD3DWnd* m_pkBtnFontSkin;

    bool m_bShowUserInfo;
    bool m_bShowStatusInfo;

    bool m_bSortByLevel;

	int  m_iItemBtnState;// 아이템에 붙을 버튼이 무엇인지 나타낸다.(소켓, 속성초기화)
    bool m_bLevelDown;  // 레벨 다운 상태 인가...

	KGCMyInfoSceneSubMenu *m_pkMyinfoSceneSubMenu;

	KD3DWnd* m_pkBntUserInfo;
	KD3DWnd* m_pkBntUserStatus;
    KGCCharWnd* m_pkCharWnd;

    KGCItemPopupMenu* m_pkItemPopupMenu;
    KItem             m_kSellItem;

    KD3DWnd*            m_pkBtnExitFromMyinfoRoom;

	KGCBillBoardChatBoard*	m_pkBillBoardChatBoard; // ???

public:
    KGCSearchBar*		m_pkSearchBar;
    std::wstring        m_strFilter;

    void UpdateLevelDown( bool bLevelDown );
	void SetItemBtnState(const KActionEvent &kEvent_){ m_iItemBtnState = static_cast<int>(kEvent_.m_dwlParam); }
    int  GetItemBtnState(){return m_iItemBtnState;}

    void SetLevelDownState( bool bLevelDown ) { m_bLevelDown = bLevelDown; }
    bool GetLevelDownState() { return m_bLevelDown; }

    void ShowItemEffect( bool _bShow );
    void UpdateHotItemCheck();

    void OnBindPet( const GCITEMUID itemUID_ );

    bool MakeItemOnFirstSlot( IN GCITEMID dwItemID );

    //Sistema de Skin de Dano
    void OnClickBtnFontSkin();
};

DEFINE_WND_FACTORY( KGCMyInfoScene );
DECLARE_WND_FACTORY( KGCMyInfoScene );
DECLARE_WND_FACTORY_NAME( KGCMyInfoScene );

#endif // _GCMYINFOSCENE_H_
