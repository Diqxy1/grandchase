#pragma once

class KGCVirtualDepotChar;

class KGCVirtualDepotDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVirtualDepotDlg );
    /// Default constructor
    KGCVirtualDepotDlg( void );
    /// Default destructor
    virtual ~KGCVirtualDepotDlg( void );
    /// Copy constructor
    KGCVirtualDepotDlg( const KGCVirtualDepotDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVirtualDepotDlg& operator=( const KGCVirtualDepotDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


    enum {
        EVDEPOT_COMMONTAB               = 1,
        EVDEPOT_EXCLUSIVETAB            = 2,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroy();
public:
    void BackGroundEnable( bool bRender = true );
    void InitScroll( void );
    void OnClose( void );
    void OnClickClose( void );
    void OnClickCharacterChange( void );
    void OnDepotScrollPos( void );
    void OnInvenScrollPos( void );

    void CreateVDepotUI( void );
    void SetItemBoxPosition( std::vector<KD3DWnd*> &vecBox, int With,D3DXVECTOR2 pos_ );
    void SetNotMoveImg( std::vector<KD3DWnd*> &vecBox, int With,D3DXVECTOR2 pos_ );

    void ClearDepotItemImgListVD( void );
    void ClearInvenItemImgListVD( void );

    void OnClickMoveItems( void );

    // 가상 창고 탭에 따른 아이템 리스트 보여주기
    void SetDepotCommonItemList( void );
    void SetShowDepotCommonItemList( void );

    void SetDepotExclusiveItemList( void );
    void SetShowDepotExclusiveItemList( void );
    void ActiveCommonTab( bool bActive );

    void SetInvenItemList( void );
    void SetShowInvenItemList( void );
    void UpdateCharacterSelect( void );
    void SetCharacter( int iChar );

    void SetItemImg( KItem* kItem, KGCCommonItemWnd* pkItemImg);
 
    void ActionRbtnClick( GCITEMUID dwItemUID );
    void ActionMoveInvenToDepotRbtnClick( GCITEMUID dwItemUID );
    void ActionMoveDepotToInvenRbtnClick( GCITEMUID dwItemUID );

    void ActionMoveDepotToInvenDrag( GCITEMUID dwItemUID, KGCCommonItemWnd* pImage );
    void ActionMoveInvenToDepotDrag( GCITEMUID dwItemUID, KGCCommonItemWnd* pImage );

    int ProgressDepotToInvenDrag(bool btnUp );
    int ProgressInvenToDepotDrag(bool btnUp );
    void EndlDrag( void );

    void UpdateShowList( void );
    void OnMoveComplate( void );

    void CheckDrowItemInfo( void );
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);

    void SetCheckMark( KD3DWnd *pkBaseWnd );

private:

    KD3DWnd*                m_pkCloseBtn;
    KD3DWnd*                m_pkCharacterBtn;
    KGCVirtualDepotChar*    m_pkCharacterInfoVD;

    int                     m_iCurrentTab;

    KD3DScrollbar*          m_pkDepotScrollbar;
    int                     m_iDepotScrollPos;
    KD3DWnd*                m_pkDepotScrollTaget;

    KD3DScrollbar*          m_pkInvenScrollbar;
    int                     m_iInvenScrollPos;
    KD3DWnd*                m_pkInvenScrollTaget;

    KD3DWnd*                m_pkClipRangeVD;
    std::vector<KD3DWnd*>   m_vecDepotBackBoxVD;
    std::vector<KD3DWnd*>   m_vecInvenBackBoxVD;

    KD3DWnd*                m_pkDepotCommonTabBtn;
    KD3DWnd*                m_pkDepotExclusiveTabBtn;

    std::vector<GCITEMUID>  m_vecShowDepotCommonItemList;
    std::vector<GCITEMUID>  m_vecShowDepotExclusiveItemList;

    std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>    m_vecDepotItemImgListVD;
    std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>>    m_vecInvenItemImgListVD;

    std::vector<KD3DWnd*>       m_NotMoveImg;

    KGCCommonItemWnd*  m_pkCurrentDragImage;
    KItem*          m_pkCurrentDragItem;

    KD3DSizingBox*  m_pkInven_Back;
    KD3DSizingBox*  m_pkVDepot_Back;

    KD3DWnd*        m_pkMoveComplate;
    KD3DStatic*     m_pkMent;
    KD3DStatic*     m_pkNpcMent;

    KD3DWnd*        m_pkMoveITems;
    KD3DWnd*        m_pkCheck;

    KD3DWnd*        m_pkWhiteBackground;

    bool            m_bMovesItem;
};

DEFINE_WND_FACTORY( KGCVirtualDepotDlg );
DECLARE_WND_FACTORY( KGCVirtualDepotDlg );
DECLARE_WND_FACTORY_NAME( KGCVirtualDepotDlg );