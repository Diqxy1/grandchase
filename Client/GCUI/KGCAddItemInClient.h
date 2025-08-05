

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "KGCAddItemInClientItemList.h"


class KGCAddItemInClientItemList;
class KD3DComboBox;
class KGCCheckBox;

class KGCAddItemInClient : public KD3DDialog,
    public KActionListener

{
public:
    DECLARE_CLASSNAME( KGCAddItemInClient );
    /// Default constructor
    KGCAddItemInClient( void );
    /// Default destructor
    virtual ~KGCAddItemInClient( void );
    /// Copy constructor
    KGCAddItemInClient( const KGCAddItemInClient& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAddItemInClient& operator=( const KGCAddItemInClient& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

#ifndef __PATH__

private:
    enum BTN_TYPE { BTN_FIRST = 0, BTN_PREV = 1, BTN_NEXT = 2, BTN_END = 3, NUM_BTN = 4 };

    KGCAddItemInClientItemList* m_pkItemList;
    KD3DWnd*    m_pkSearchBack;
    KD3DWnd*    m_pkSearchButton;
    KD3DWnd*    m_pkAddButton;
    KD3DEdit*   m_pkEditSearch;
    KSafeArray< KD3DWnd*, NUM_BTN >     m_apkBtn;
    KD3DWnd*    m_pkPageBack;
    KD3DStatic*    m_pkStaticPage;
    KD3DComboBox*		m_pkCharTypeCombo;
    KGCCheckBox*        m_pkAllItemListCheckBox;
    KD3DWnd*           m_pkAddBookMark;
    KGCCheckBox*        m_pkAllItemListBookMark;
    KD3DWnd*           m_pkDeleteBookMark;

    KD3DWnd*    m_pkBookMarkBack;
    KD3DStatic* m_pkStaticBookMark;   
    KD3DStatic* m_pkStaticCheck;
    KD3DStatic* m_pkStaticNameID;
    KD3DStatic* m_pkStaticCharType;
    KD3DStatic* m_pkStaticSearch;
    KD3DStatic* m_pkStaticAdd;

    KD3DStatic* m_pkStaticBookMarkTip;
private:
    
    USHORT m_usCurrentPage;
    USHORT m_usMaxPage;
    std::vector<GCItem*> m_vecItemDataList;
    std::vector<GCItem*> m_vectoritemlistStorage;
    std::vector<GCItem*> m_vecBookMarkItemDataList;
    std::vector<int> m_vecBookMarkItemInfo;
    unsigned int m_usDisplayMinItemNum;
    unsigned int m_usDisplayMaxItemNum;
    int m_icount;
    bool m_bLoaded;
    bool m_bDeleteBookMark;
    bool m_bSearch;
    int m_iColumn;
    int m_iRow;

    std::vector<const WCHAR*> m_vecBookMarkItemData;
    std::vector<GCItem*> m_vecBookMarkItemList;
public:
    void Init();
    void SetFocus( bool bFocus );
    void Refreshpage( BTN_TYPE emBtnType);
    void RefreshItemList();
    void OnPrintBookMarkItemInfo();
    void LoadBookMark();
    void OnRenderUpLine(bool bRender);
    void OnRenderDownLine(bool bRender);
    void AllRender(bool bRender);
    void OnKeyDown( const KActionEvent& event );
    void OnKeyBookMark();
    void ClearEdit();
    void ActionSearch();
    void ActionAdd();
#endif // __PATH__
};

DEFINE_WND_FACTORY( KGCAddItemInClient );
DECLARE_WND_FACTORY( KGCAddItemInClient );
DECLARE_WND_FACTORY_NAME( KGCAddItemInClient );


