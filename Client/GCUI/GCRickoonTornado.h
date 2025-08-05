#pragma once

#include "GCUI/GCItemImgWnd.h" 
class KD3DStatic;
class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KGCPetMessage;
class KPet;
class KGCCharacterUI;
class KGCUINumber;
class KGC2DObject;
class KGCCharacterSelectBox;

#define RK_COIN_ID  58243
#define RK_LUCKY_COIN_ID  58244
#define SLOT_SWAP_TIME 0.1

#if defined( NATION_KOREA )
#define REWARD_SWAP_TIME 0.7
#define SLOT_ROLL_TIME 0.5
#else
#define REWARD_SWAP_TIME 7.0
#define SLOT_ROLL_TIME 5
#endif

class KGCRickoonTornado : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRickoonTornado );
    /// Default constructor
    KGCRickoonTornado( void );
    /// Default destructor
    virtual ~KGCRickoonTornado( void );
    /// Copy constructor
    KGCRickoonTornado( const KGCRickoonTornado& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRickoonTornado& operator=( const KGCRickoonTornado& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        VIEWPORT_OFFSET_X   = -15,
        VIEWPORT_OFFSET_Y   = 118,
        VIEWPORT_WIDTH      = 364,
        VIEWPORT_HEIGHT     = 250,
    };

    enum
    {
        ESLOT_ITEM_NUM      = 3,
        EREWARD_ITEM_NUM    = 3,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void OnDestroy( void );

private:
    KD3DWnd*			m_pkBtnClose;
    KD3DWnd*            m_pkBtnCharRight;
    KD3DWnd*            m_pkBtnStart;
    KD3DWnd*            m_pkBtnStop;
    KD3DWnd*            m_pkBtnBuy;
    KD3DWnd*            m_pkBtnRewardListLeft;
    KD3DWnd*            m_pkBtnRewardListRight;
    KD3DWnd*            m_pkBtnLuckyStart;

    KD3DWnd*            m_pkLuckyItemSlot;
    KD3DWnd*            m_pkLuckyItemSlotOne;
    KD3DWnd*            m_pkImgLuckyCoin;

    KD3DStatic*         m_pkStaticCoinName;
    KD3DStatic*         m_pkStaticLuckyCoinName;
    KD3DStatic*         m_pkStaticCoinCount;
    KD3DStatic*         m_pkStaticLuckyCount;

    KSafeArray<KD3DWnd*,GC_CHAR_NUM>    m_pkSelectCharFace;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM>    m_pkSelectCharName;

    KSafeArray<KGCItemImgWnd*,ESLOT_ITEM_NUM>       m_pkImgItem;
    KSafeArray<KGCItemImgWnd*,EREWARD_ITEM_NUM>     m_pkImgRewardItem;
    KSafeArray<bool,ESLOT_ITEM_NUM>                 m_bSlotEffect;

    D3DVIEWPORT9        m_viewport;
    D3DXVECTOR3         m_vPetAxisCorrection;

    float				m_fZoom;
    float               m_fPosX;
    float               m_fPosY;
    float               m_fPosZ;
    float               m_fStartAngle;

    float               m_fRewardSwapTime;
    float               m_fSlotSwapTime;
    float               m_fReawdLastTime;
    float               m_fSlotLastTime;
    float               m_fSlotRollTime;
    float               m_fRewardDely;
    float               m_fRewardDelyLast;
    float               m_fParticleLife;
    float               m_fParticleX;

    bool				m_bMouseCatchStart;
    bool                m_bRickoonOn;
    bool                m_bLuckyTornado;
    bool                m_bRewardDelay;
    bool                m_bRool;

    KPet*				m_pkPet;

    int                 m_iSelectedChar;
    int                 m_iCoinCount;
    int                 m_iLuckyCount;
    int                 m_iLuckyEnableCount;
    int                 m_iRewardItemListIndex;
    int                 m_iRewardItemListSize;

    DWORD               m_dwType;
    DWORD               m_dwRewardType;
    DWORD               m_dwRewardItemID;

    std::map< DWORD, std::vector< DWORD > > m_mapRewardList;
    std::vector< KItem >    m_vecRewardItem;
    std::vector<KItem>      m_vecUseItem;
public:
    bool                m_bIsComplete;
    bool                m_bEnableEvent;
    GCITEMID            m_dwExposeItemToUser;

public:
    void SetRickoon();
    void OnClickCharMove( KD3DWnd* pWnd, DWORD dwCode );
    void OpenCharacterSelcectUI();
    void UpdateCharacterSelect();
    void SetSelectedChar( int iCharType, int iPromotion);
    void SetInitInfo( DWORD dwLuckyEnableCount, std::map< DWORD, std::map< DWORD, std::set<DWORD> > >  mapNonFailRewardList, GCITEMID dwExposeItemToUser );
    void UpdateRewardItemList();
    void UpdateSlotItemImg( int iSlotIdx = 0 );
    void ButtonLock();
    void ButtonUnLock();
    void CalTime();
    void OnBuyBtn();
    void RollStart();
    void RollStop();
    void SetRewardItemList( DWORD dwRewardType, std::vector<KItem> vecUseItem, std::vector<KItem> vecItem, DWORD dwRewardItemID );
    void SetCoinCount();
    void RewardItem();

    void InitRollState();

    void CheckLucky();
    void ShowslotEffect(int slotNum);
    void UpdateCoinCount();
    bool GetCreateComplete();
    void SetEnableEvent( bool bEnableEvent );
    bool GetEnableEvent();

    void Close();

    const D3DXVECTOR3 AddVector( const D3DXVECTOR3& first, const D3DXVECTOR3& second );
};

DEFINE_WND_FACTORY( KGCRickoonTornado );
DECLARE_WND_FACTORY( KGCRickoonTornado );
DECLARE_WND_FACTORY_NAME( KGCRickoonTornado );
