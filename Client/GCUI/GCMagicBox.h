#pragma once

/*
560320	고대의 마력석               //판매안함  , 아이템 소모시키기
560330	코우나트의 마력석
560340	고대의 마력석 패키지        //판매함    , 구입누르면 뜨개하기
560350	코우나트의 마력석 패키지


ITEM_ID_ANCIENT_STONE_USE           (56032) //고대의 마력석 사용
ITEM_ID_ANCIENT_STONE_BUY           (56034) //고대의 마력석 구입(패키지)
ITEM_ID_KONAUT_STONE_USE            (56033) //코나우트 마력석 사용
ITEM_ID_KONAUT_STONE_BUY            (56035) //코나우트 마력석 구입(패키지)

*/
class KGCMagicBoxItemWnd;



class KGCMagicBox : public KD3DDialog                   // extends
                            ,public KActionListener     // implements
{
public:
    DECLARE_CLASSNAME( KGCMagicBox );
    /// Default constructor
    KGCMagicBox( void );
    /// Default destructor
    virtual ~KGCMagicBox( void );
    /// Copy constructor
    KGCMagicBox( const KGCMagicBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMagicBox& operator=( const KGCMagicBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnInitialize( void );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void SetChar(int iCharType);
    void OnClose();
    bool OnClickBuyBtn( const KActionEvent &event );
    void BuyItem( DWORD& dwBuyItemID );
    void ClickNextPage( );
    void ClickPrePage( );
    void SetItemPage( int _pageIndex );
    void InitButtonPos();
    void SetSelectableList( std::map<GCITEMID, std::pair<int,int> >& _mapItemInfo );
    void IsValidItem( GCITEMID _selectItemID );
    void CalculateLimitPageByCharacter( OUT int& _itemPageLimit );
    void InitVariable();
    //void DrawItemInfoBox( DWORD dwGoodsID, D3DXVECTOR2 vPos );
    void SetSelectPageIndex( int _nIndex );
    void ClickKounatStone( );
    void ClickAncientStone( );
    void ClickItemList( GCITEMID _selectItemID );
    void ResultEvent( KEVENT_USE_MAGIC_BOX_ACK result_ );
    void ClearItemList() { m_mapItemList.clear(); };
private:

    enum {
        NUM_MAX_ITEM = 10,
        NUM_MAX_PAGE_INDEX = 10,
    };

    struct SMagicBoxInfo {
        GCITEMID    m_gcItemID;
        int         m_iExtraKeyCount;
        int         m_iGetItemCount;
    };

    KD3DWnd*                            m_pkBtnClose;
    KD3DWnd*                            m_pkBtnBuyAcientStone;
    KD3DWnd*                            m_pkBtnbuyKounatStone;
    KD3DWnd*                            m_pkBtnUseAncientStone;
    KD3DWnd*                            m_pkBtnUseKounatStone;
    KD3DWnd*                            m_pkBtnUseSelectAcientStone;
    KD3DWnd*                            m_pkBtnUseSelectKounatStone;


    KSafeArray<KD3DWnd*,GC_CHAR_NUM>    m_apkCharName;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM>    m_apkCharFace;
    KD3DWnd*                            m_pkBtnRightPage;
    KD3DWnd*                            m_pkBtnLeftPage;



    KD3DStatic*                         m_pkStaticAcientStone;
    KD3DStatic*                         m_pkStaticKounatStone;
    KD3DStatic*                         m_pkStaticCurrentHaveCount;


    KD3DStatic*                         m_pkStaticEventGuide;
    KGCUINumber*                        m_pkStaticAcientCount;
    KGCUINumber*                        m_pkStaticKounatcount;


    KSafeArray<KGCUINumber*,NUM_MAX_PAGE_INDEX> m_apkStaticPageIndex;
    int             m_iItemListPageLimit;
    int             m_iSelectCharacter;


    GCITEMID                                    m_selectItemID; //잠금 깍을 아이템
    std::map<GCITEMID, std::pair<int,int> >     m_mapItemInfo;  //출력할 아이템 목록 ( 서버로부터 받는다 )
    std::map<int, std::vector<SMagicBoxInfo>>   m_mapItemList;
    int             m_iRealPageIndex;

    KSafeArray<KGCMagicBoxItemWnd*,NUM_MAX_ITEM>   m_apkMagicBoxItemWnd;
    KGCMagicBoxItemWnd*                             m_pkSelectItemWnd;



    DWORD           m_dwMyAncientStoneCount;
    DWORD           m_dwMyKonautStoneCount;

    int             m_nViewIndexCount;
    int             m_nViewMaxIndex;
    int             m_nViewMinIndex;

    bool            m_bCheckCanBuy;     // 구매제한 아이템인가 체크 true면 구매 가능 false면 불가능 
    bool            m_bCheckInventory;  // 인벤토이레 여유 있는지 체크 true면 여유 있음  false면 없음 
};

DEFINE_WND_FACTORY( KGCMagicBox );
DECLARE_WND_FACTORY( KGCMagicBox );
DECLARE_WND_FACTORY_NAME( KGCMagicBox );
