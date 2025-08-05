#pragma once
class KGCItemImgWnd;

class KGCAutoMatchRankingReward : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    
    enum 
    {
        EM_REWARD_NUM = 3,
        EM_REWARD_ITEM_NUM = 4,
    };


    DECLARE_CLASSNAME( KGCAutoMatchRankingReward );
    /// Default constructor
    KGCAutoMatchRankingReward( void );
    /// Default destructor
    virtual ~KGCAutoMatchRankingReward( void );
    /// Copy constructor
    KGCAutoMatchRankingReward( const KGCAutoMatchRankingReward& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAutoMatchRankingReward& operator=( const KGCAutoMatchRankingReward& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();



public:
    void CloseUI();
    void OpenCharacterSelcectUI();
    void UpdateCharacterSelect();
    void SendChangeChange(int iChar);
    void OnClickRewardBtn( int nRankType, int nRankGroup );
    
    
    void UpdateRankerReward();
    void ResetRewardItem();
    void SetRewardItem( int iCharType, int nRankType, int nRankGroup );
    void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID );

public:

    KD3DStatic*	m_pkDesc;
	KD3DWnd*	m_pkClosebtn;
    KD3DWnd*    m_pkBtnSelectCharacter;

    KSafeArray<KD3DWnd*, EM_REWARD_NUM> m_pkRPRewardBtn;
    KSafeArray<KD3DWnd*, EM_REWARD_NUM> m_pkWinRewardBtn;

    KSafeArray<KGCItemImgWnd*, EM_REWARD_ITEM_NUM> m_pkRPRewardItem;
    KSafeArray<KGCItemImgWnd*, EM_REWARD_ITEM_NUM> m_pkWinRewardItem;


    int m_nRPBtnState;
    int m_nWinBtnState;
    int m_iCharType;
};

DEFINE_WND_FACTORY( KGCAutoMatchRankingReward );
DECLARE_WND_FACTORY( KGCAutoMatchRankingReward );
DECLARE_WND_FACTORY_NAME( KGCAutoMatchRankingReward );
