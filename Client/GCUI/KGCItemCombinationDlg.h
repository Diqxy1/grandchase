#pragma once

typedef std::pair< std::pair<GCITEMID, GCITEMUID>, bool >     PAIR_STUFF_LIST;
typedef std::pair< GCITEMID, int >      PAIR_STUFF_AND_INDEX;
class KGCItemCombinationStuff;

class KGCItemCombinationDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemCombinationDlg );
    /// Default constructor
    KGCItemCombinationDlg( void );
    /// Default destructor
    virtual ~KGCItemCombinationDlg( void );
    /// Copy constructor
    KGCItemCombinationDlg( const KGCItemCombinationDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemCombinationDlg& operator=( const KGCItemCombinationDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_STUFF_SLOT_SIZE       = 4,
        EM_STUFF_LIST_SLOT_SIZE  = 5,
        EM_REWARD_SLOT_SIZE      = 6,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void OnClickClose();
    void OnClickAssembleBtn();
    void OnRewawrdPrevMove();
    void OnRewardNextMove();
    void OnStuffPrevMove();
    void OnStuffNextMove();
    void UpdateStuffList( void );
    void OnClickStuffListSlot( int nSlot );
    void UpdateStuff();
    void OnClickStuffSlot( int nSlot );
    void InitData( void );
    void UpdateRewardItemList( void );
    void ButtonLock(bool bBtnLock);
    void ShowEffect();
    void CheckParticleTime();
public:
    KD3DWnd* m_pkBtnClose;
    KD3DWnd* m_pkAssembleBtn;
    KD3DWnd* m_pkBtnRewardPre;
    KD3DWnd* m_pkBtnRewardNext;
    KD3DWnd* m_pkBtnStuffPre;
    KD3DWnd* m_pkBtnStuffNext;
    KD3DStatic* m_pkDescStatic;
    KD3DStatic* m_pkBaloonDesc;

    int m_nIndex;
    int m_RewardItemIndex;
    int m_iParticleTime;

    std::vector<GCITEMID>  m_vecRewardItemList;
    std::vector<PAIR_STUFF_LIST>  m_vecStuffItemList;
    std::vector<PAIR_STUFF_AND_INDEX> m_vecStuffSlot;
    std::map<int , GCITEMUID>   m_mapStuffItemUIDList;

    KSafeArray<KGCItemCombinationStuff*,EM_STUFF_SLOT_SIZE>		    m_pkStuffSlot;
    KSafeArray<KGCItemCombinationStuff*,EM_STUFF_LIST_SLOT_SIZE>    m_pkStuffListSlot;
    KSafeArray<KGCItemImgWnd*,EM_REWARD_SLOT_SIZE>                  m_pkRewardSlot;

};

DEFINE_WND_FACTORY( KGCItemCombinationDlg );
DECLARE_WND_FACTORY( KGCItemCombinationDlg );
DECLARE_WND_FACTORY_NAME( KGCItemCombinationDlg );
