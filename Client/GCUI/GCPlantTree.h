#pragma once
#include "stdafx.h"

class KGCPlantTreeMap;
//class KGCPlantTreeRewardItemListView;

class KGCPlantTree : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPlantTree );
    /// Default constructor
    KGCPlantTree( void );
    /// Default destructor
    virtual ~KGCPlantTree( void );
    /// Copy constructor
    KGCPlantTree( const KGCPlantTree& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlantTree& operator=( const KGCPlantTree& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EPLANT_TREE_STATE{
        EPTS_EARTH = 0,
        EPTS_GREEN_TREE_1 = 1,
        EPTS_GREEN_TREE_2 = 2,
        EPTS_GREEN_TREE_3 = 3,
        EPTS_GREEN_TREE_4 = 4,
        EPTS_GOLD_TREE_1 = 5,
        EPTS_GOLD_TREE_2 = 6,
        EPTS_GOLD_TREE_3 = 7,
        EPTS_GOLD_TREE_4 = 8,
        EPTS_DEATH = 9,
        EPTS_NUM
    };

    enum EPLANT_TREE_BTN {
        EPTB_WATER_ITEM = 0,
        EPTB_FERTILIZERS_ITEM = 1,
        EPTB_GREEN_TREE_ITEM = 2,
        EPTB_GOLD_TREE_ITEM = 3,
        EPTB_GET_TREE_ITEM = 4,
        EPTB_GET_WORLD_ITEM = 5,
        EPTB_NUM,
    };

    enum EPLANT_TREE_STATIC {
        EPTSTR_WATER_ITEM_TITLE = 0,
        EPTSTR_FERTILIZERS_ITEM_TITLE = 1,
        EPTSTR_GREEN_TREE_ITEM_TITLE = 2,
        EPTSTR_GOLD_TREE_ITEM_TITLE = 3,
        EPTSTR_WATER_ITEM = 4,
        EPTSTR_FERTILIZERS_ITEM = 5,
        EPTSTR_GREEN_TREE_ITEM = 6,
        EPTSTR_GOLD_TREE_ITEM = 7,
        EPTSTR_DESCRIPTION = 8,
        EPTSTR_NUM,
    };


    virtual void ActionPerformed( const KActionEvent& event );

    void OnClickBtn( const KActionEvent &event ); 

    virtual void OnCreate( void );
    void SetPlantTreeInfo( DWORD dwContinentType, DWORD dwCurrentPosition, DWORD dwTreeStatus);
    void SetPlantTreeRewardInfo( KEVENT_PLANT_TREE_MAP_NOT &kRecv );
    void SetTree(int iContinentType, int iCurrentPos, int iTreeStatus);
    void InitPlantTree( void );
    int TransferStateInClient( int iTreeStatus );

    void ShowRewardParticle( int iTreeStatus ); 

    void SetObjectPos( int iContinentType , int iCurrentPos ,int iTreeStatus);
    std::vector<D3DXVECTOR2> GetObjectPos( int iContinentType );
    void ClearTreeState( );
    void ClearEarth( );
    void ClearTree( );
    void SetBtn( int iTreeStatus);
    void SetInfoWindow( );
    void SetInfoWinowStatic( int itemID, int staticIndex );
    bool OnClickUseItemBtn( const KActionEvent &event,int iBtnType, int iActionType, DWORD dwItemID );
    bool OnClickGetItemBtn( const KActionEvent &event,int iBtnType);
    void On_ActionAck( KEVENT_PLANT_TREE_ACTION_ACK &kRecv );

    void RecvItemList( std::vector<KItem> &kRecvItem , bool bViewItem);
    void On_RewardAck( KEVENT_PLANT_TREE_REWARD_ACK &kRecv );
    void FrameMoveInEnabledState();
    void SetTreeProgressBar(int iContinentType, int iCurrentPos);
    void SetWaterProgressBar(int iContinentType, int iNum);
    bool OnClickMoveWorldBtn( const KActionEvent &event);
    bool OnClickOpenWorldBtn( const KActionEvent &event);
    void ToggleWorldmapBtn( void );
    void BuyItem( DWORD& dwBuyItemID );
    bool OnClickBuyBtn( const KActionEvent &event );
private:

    KD3DWnd     *m_pkBtnViewRewardItem; // 보상 보기 버튼
    KD3DWnd     *m_pkClose; // Close

    KD3DWnd     *m_pkTotalProgressbarL; // 나무 프로그레스바 좌측 우측 센터 ㅋ
    KD3DWnd     *m_pkTotalProgressbarR; // 
    KD3DWnd     *m_pkTotalProgressbarC; // 

    KD3DWnd     *m_pkWaterProgressbarL; // 물 프로그레스바 좌측 우측 센터 ㅋ
    KD3DWnd     *m_pkWaterProgressbarR; // 
    KD3DWnd     *m_pkWaterProgressbarC; // 

    KD3DSizingBox*			m_pkBkWorldBtn;

    KD3DWnd     *m_pkBtnMoveWorld; // 

    KD3DWnd     *m_pkBtnBuyWaterItem;       // 물 사기 
    KD3DWnd     *m_pkBtnBuyGoldTreeItem;    // 황금나무 사기
    KD3DWnd     *m_pkBtnBuyFertilizersItem; // 비료 사기

    std::map<int, KD3DWnd*> m_mapBtnWorldList;

    KGCPlantTreeMap *m_pkWoldMapList;
//    KGCPlantTreeRewardItemListView *m_pkReardItemListView;
    KEVENT_PLANT_TREE_MAP_NOT m_mapPlantTreeInfo;

    std::vector<KD3DWnd*> m_vecTreeList;
    std::vector<KD3DWnd*> m_vecEarthList;
    std::vector<KD3DWnd*> m_vecTreeStateList;
    std::vector<KD3DWnd*> m_vecBtnList;
    std::vector<KD3DStatic*> m_vecStaticList;

    int m_iContinentType;
    int m_iCurrentPos;

    int m_iRealContinentType;
    int m_iRealCurrentPos;
    int m_iRealCurrentState;
    bool  m_bRenderProgressBar;

    std::map<int,std::vector<std::string>> m_mapParticle;
};

DEFINE_WND_FACTORY( KGCPlantTree );
DECLARE_WND_FACTORY( KGCPlantTree );
DECLARE_WND_FACTORY_NAME( KGCPlantTree );
