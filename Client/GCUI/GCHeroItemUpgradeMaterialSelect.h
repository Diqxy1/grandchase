#pragma once

class KGCHeroItemUpgradeMaterialSelect : public KD3DWnd    // extends
                                         ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroItemUpgradeMaterialSelect );
    /// Default constructor
    KGCHeroItemUpgradeMaterialSelect( void );
    /// Default destructor
    virtual ~KGCHeroItemUpgradeMaterialSelect( void );
    /// Copy constructor
    KGCHeroItemUpgradeMaterialSelect( const KGCHeroItemUpgradeMaterialSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroItemUpgradeMaterialSelect& operator=( const KGCHeroItemUpgradeMaterialSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    enum { 
        NUM_SLOT_LINE = 4,  // 한줄에 아이템 4개 
        NUM_MAX_SLOT  = 80, // 슬롯 최대 80개 

        START_POS_X = 5,    // 시작 위치 
        START_POS_Y = 5,

        SLOT_OFFEST = 2,    // 슬롯 간격
        ROUND_OFFSET =5,    // 슬롯과 배경 사이 간격 

        IMG_WIDTH  = 30,
        IMG_HEIGHT = 30,
        BOX_WIDTH  = 30,
        BOX_HEIGHT = 30,

        BACKGROUND_START_HEIGHT = 40,   // 배경 시작 높이 
    };

private:
    D3DXVECTOR2     m_dxvPos;
    KD3DSizingBox*  m_pkBackGround;
    
    GCItem*                 m_pItemData;
    std::vector< KItem* >   m_vecInvenItem;
    GCITEMUID               m_SelectItemUID;

    int m_iItemCount;
    int m_iSlotLineCount;

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >         m_vecItemBox;
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >   m_vecItemImg;

    int m_iUpgradeSlot;
    int m_iMaterialSlot;

public:
    void SetInfo( int iUpgradeSlot_, int iMaterialSlot_, GCITEMID itemID_, int iGrade_ );
    void OnClose( void );
    void OnClickItem( const KActionEvent& event );
    void SetPos( D3DXVECTOR2 dxvPos_ ) { m_dxvPos = dxvPos_; }

private:
    void InitUI( void );
    void ClearData( void );
    void MakeItemList( void );
    void SetBackGroundHeigt( void );
    void SetItemPos( void );
    void ShowItemInfo( void );
};

DEFINE_WND_FACTORY( KGCHeroItemUpgradeMaterialSelect );
DECLARE_WND_FACTORY( KGCHeroItemUpgradeMaterialSelect );
DECLARE_WND_FACTORY_NAME( KGCHeroItemUpgradeMaterialSelect );
