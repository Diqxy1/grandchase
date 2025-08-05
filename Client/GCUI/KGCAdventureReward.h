#pragma once
#include "GCAdventureManager.h"

class KGCAdventureRewardItemWnd;

class KGCAdventureReward : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    
    DECLARE_CLASSNAME( KGCAdventureReward );
    /// Default constructor
    KGCAdventureReward( void );
    /// Default destructor
    virtual ~KGCAdventureReward( void );
    /// Copy constructor
    KGCAdventureReward( const KGCAdventureReward& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAdventureReward& operator=( const KGCAdventureReward& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void ShowItemInfo( void );
    void OnMouseEnter( void );
    void OnMouseLeave( void );
    void SetRewardItemBoxInfo( std::vector< KDropItemInfo > vecContinentReward  );
    void OnClickRewardBtn();
    void SetContinentInfo( int iIndex );
    void CheckContinentRewardBtn ( int iContinentID );
    void WholeContinentRender( bool bRender );
    void CheckContinentReward();
    void SetFinalRewardItemBoxInfo();
    void OffCheckContinentImage();
    void RewardBtnWndModeLock();
private:
    KD3DWnd*    m_pkRewardName;
    KD3DWnd*    m_pkFinalRewardName;
    KD3DWnd*    m_pkRewardBtn;
    KD3DWnd*    m_pkWholeContinentImage;
    KD3DStatic* m_pkStaticContinentDesc;
    KGCItemImgWnd*      m_pkContinentRewardImgWnd;
    KSafeArray<KGCAdventureRewardItemWnd*, MAX_CONTINENT_REWARD_ITEM_NUM >   m_apkAdventureRewardItemBox;
    KSafeArray<KD3DWnd*,GCAdventureManager::CONTINENT_MAX>   m_saCheckContinent;

    bool m_bMouseBound;
    GCITEMID m_dwItemID;
    std::map<DWORD, KCollectItem > m_mapCollectItems;
    std::map <DWORD, int> m_mapCollectData;

};

DEFINE_WND_FACTORY( KGCAdventureReward );
DECLARE_WND_FACTORY( KGCAdventureReward );
DECLARE_WND_FACTORY_NAME( KGCAdventureReward );
