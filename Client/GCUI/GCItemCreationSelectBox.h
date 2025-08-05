#pragma once

class KGCItemCreationSelectBox : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemCreationSelectBox );
    /// Default constructor
    KGCItemCreationSelectBox( void );
    /// Default destructor
    virtual ~KGCItemCreationSelectBox( void );
    /// Copy constructor
    KGCItemCreationSelectBox( const KGCItemCreationSelectBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemCreationSelectBox& operator=( const KGCItemCreationSelectBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void FrameMoveInEnabledState( void );

private:
    enum {
        NUM_SLOT_PER_LINE = 4,      // 한줄에 아이템 4개 
        NUM_MAX_SLOT      = 80,     // 슬롯 최대 80개 

        BACK_START_X = 16,           // 아이템 배경 시작 위치 
        BACK_START_Y = 32,

        IMG_START_X = 16,           // 아이템 이미지 시작 위치 
        IMG_START_Y = 33,

        SLOT_OFFSET = 12,            // 각 상자 좌우 간격 
        SLOT_ROUND_OFFSET = 11,      // 강 상자와 배경 간격 / 강 상자 상하 간격

        IMG_WIDTH  = 50,            // 아이템 이미지 사이즈 
        IMG_HEIGHT = 50,
        IMG_BACK_WIDTH  = 51,       // 아이템 배경 사이즈 
        IMG_BACK_HEIGHT = 51,

        BACKGROUND_START_HEIGHT = 94,   // 배경 시작 높이 - 아이템 수에 따라 사이즈 조정 
    };

private:
    D3DXVECTOR2           m_dxvPos;
    KD3DSizingBox*        m_pkBackGround;

    GCItem*               m_pkItemData;
    std::vector< KItem* > m_vecInvenItem;

    int                   m_iItemCount;
    int                   m_iSlotLineCount;

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >         m_vecItemBack;
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >   m_vecItemImg;

public:
    void OnClose( void );
    void OnClickItem( const KActionEvent& event );
    void SetPos( const D3DXVECTOR2 dxvPos_ ) { m_dxvPos = dxvPos_; }

private:
    void InitUI( void );
    void MakeItemSlotList( void );
    void SetBackGroundHeight( void );
    void SetItemSlotPos( void );
    void ShowItemInfo( void );
    void ClearData( void );

};

DEFINE_WND_FACTORY( KGCItemCreationSelectBox );
DECLARE_WND_FACTORY( KGCItemCreationSelectBox );
DECLARE_WND_FACTORY_NAME( KGCItemCreationSelectBox );
