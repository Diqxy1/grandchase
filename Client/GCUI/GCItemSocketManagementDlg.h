#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KD3DStatic;
class KGCItemImgWnd;
class KGCSocketIcon;
class KD3DScrollbar;

class KGCItemSocketManagementDlg : public KD3DDialog,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemSocketManagementDlg );
    /// Default constructor
    KGCItemSocketManagementDlg( void );
    /// Default destructor
    virtual ~KGCItemSocketManagementDlg( void );
    /// Copy constructor
    KGCItemSocketManagementDlg( const KGCItemSocketManagementDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemSocketManagementDlg& operator=( const KGCItemSocketManagementDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    void OnCreateComplete( void );
    void PostChildDraw( void );
    void ActionPerformed( const KActionEvent& event_);
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

    enum USE_MATERIAL_KIND {
        UMK_GAMEPOINT,
        UMK_CASHITEM,
        UMK_MAX,
    };
private:
    enum EITEMSOCKETINDEX
    {   
        EISI_UNKNOWN = -1,
        EISI_ONE_LINE_COUNT = 2,
        EISI_MAX = 4,
    };

    //탭과 소켓상태는 동일하게 쓰일 수 있지만.. 현재로선 탭수가 상태수보다 적다.
    KSocketInfo::SOCKET_STATE        m_eSelectedTab;

    KD3DWnd* m_pkSocketManageMentTab[ KSocketInfo::STS_NUM ];
    KD3DWnd* m_pkSocketManageMentSelectedTab[ KSocketInfo::STS_NUM ];
    KD3DWnd* m_pkActionBtn[ KSocketInfo::STS_NUM ];
    GCITEMID    m_iMaterialUnit[ KSocketInfo::STS_NUM ];
    int         m_iDescriptionStringID[ KSocketInfo::STS_NUM ];
    int         m_iConditionInfoStringID[ KSocketInfo::STS_NUM ];

    KGCSocketIcon* m_pkSocketBtn[EISI_MAX];
    KD3DSizingBox* m_pkSocketActiveEdge[ EISI_MAX ];
    KGCItemImgWnd* m_pkMonsterCardImg[ EISI_MAX ];

    KGCItemImgWnd*  m_pkItemImg;
    KD3DStatic* m_pkDescription;
    KD3DWnd*    m_pkCloseBtn;
    KD3DScrollbar* m_pkScrollBar;

    KD3DStatic* m_pkTextNeed;
    KD3DStatic* m_pkTextOwn;
    KD3DStatic* m_pkNeedCount;
    KD3DStatic* m_pkOwnCount;

    int         m_iSelectedIndex;
    GCITEMID    m_SelectedMonsterCardID;
    GCITEMUID   m_ItemUID;
    GCITEMID    m_UseCashItemID;
    std::vector< KSocketInfo > m_vecSocketInfo;
    
    USE_MATERIAL_KIND       m_eUseMaterialKind;

    void SetSelectedItem();
    void OnClickActionButton( KSocketInfo::SOCKET_STATE eItemSocketManagementTab_);
    void OnClose();
    void InitTabInfo( KSocketInfo::SOCKET_STATE eSelectTab_ );
    void SelectSocket( int iSocketIdex_ );
    void SetCondition();
    void SetScoketView();
    void RefreshSocketActiveEdge();
    void RefreshSelectedMonsterCard();
    void OnScroll();
    bool IsEnableSocketIndex( int iSocketIndex_);
public:
    void SetSelectedItemUID( GCITEMUID ItemUID_ ){ m_ItemUID = ItemUID_; }
    int GetScrollTopIndex() { return m_pkScrollBar->GetScrollPos() * EISI_ONE_LINE_COUNT; }
    std::wstring GetDescString();
    KSocketInfo GetSocketInfo(int iIndex_);

};

DEFINE_WND_FACTORY( KGCItemSocketManagementDlg );
DECLARE_WND_FACTORY( KGCItemSocketManagementDlg );
DECLARE_WND_FACTORY_NAME( KGCItemSocketManagementDlg );


