#pragma once

class KGCPlantTreeRewardItemListView : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPlantTreeRewardItemListView );
    /// Default constructor
    KGCPlantTreeRewardItemListView( void );
    /// Default destructor
    virtual ~KGCPlantTreeRewardItemListView( void );
    /// Copy constructor
    KGCPlantTreeRewardItemListView( const KGCPlantTreeRewardItemListView& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlantTreeRewardItemListView& operator=( const KGCPlantTreeRewardItemListView& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void InitBtnState();
    void SetRewardSetting();
    void SetRewardSetting(KEVENT_PLANT_TREE_MAP_NOT mapInfo);
    void ClearItemImage();
    bool OnClickBtn( const KActionEvent& event );
    
    void SetWorldBtnState(int index){m_iWorldBtnState = index;}
    void PostChildDraw();
    void DrawItemInfo();

    enum PTRLV_BTN_LIST{
        PTRLV_GREEN_TREE =0,
        PTRLV_GOLD_TREE ,
        PTRLV_WORLD,
        PTRLV_BERMESIAH,
        PTRLV_SILVERLAND,
        PTRLV_ELLIA,
        PTRLV_ARCHIMEDIA,
    };

    enum { 
        IMG_START_X = 244,
        IMG_START_Y = 233,
        IMG_SIZE = 52,
    };

private:
    std::map<int, KD3DWnd*>     m_mapBtn;
    KD3DWnd                     *m_pkClose;
    std::vector<KGC2DObject*>	m_vecItemImage; //아이템 이미지들

    int                         m_iBtnState;
    int                         m_iWorldBtnState;

    KEVENT_PLANT_TREE_MAP_NOT   m_mapInfo;
    std::vector<std::pair<D3DXVECTOR2, KDropItemInfo >> m_vecRewardItem;
};

DEFINE_WND_FACTORY( KGCPlantTreeRewardItemListView );
DECLARE_WND_FACTORY( KGCPlantTreeRewardItemListView );
DECLARE_WND_FACTORY_NAME( KGCPlantTreeRewardItemListView );
