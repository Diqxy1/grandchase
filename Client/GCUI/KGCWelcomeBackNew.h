#ifndef _KGCWELCOMEBACKNEW_H_
#define _KGCWELCOMEBACKNEW_H_

#if defined( WELCOMEBACK_FRIEND_NEW ) 
#define RECOMMENDER_MAX_NUM     4
#define REWARD_MAX_NUM          4
#else
#define RECOMMENDER_MAX_NUM     12
#define REWARD_MAX_NUM          4
#endif

class KD3DStatic;
class KGCWelcomeBackRecom;
class KGCWelcomeBackReward;
class KGCWelcomeBackItemList;

class KGCWelcomeBackNew : public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCWelcomeBackNew );
    /// Default constructor
    KGCWelcomeBackNew( void );
    /// Default destructor
    virtual ~KGCWelcomeBackNew( void );
    /// Copy constructor
    KGCWelcomeBackNew( const KGCWelcomeBackNew& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWelcomeBackNew& operator=( const KGCWelcomeBackNew& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void UpdateState( void );
    void SetItemInfo( KEVENT_CHECK_RECOMMEND_ENABLE_ACK kRecv );
    void UpdateRewardState( void );
    void SetRewardState( int nState);
    void UpdateMyState( int _state);

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DStatic*             m_pkStaticDesc;
    KD3DStatic*             m_pkStaticMyState;
    KD3DWnd*                m_pkCloseBtn;
    KD3DWnd*                m_pkMyStateBackground;

    KSafeArray<KGCWelcomeBackRecom*,RECOMMENDER_MAX_NUM> m_pkRecommender;
    KSafeArray<KGCWelcomeBackReward*,REWARD_MAX_NUM> m_pkReward;     // 보상버튼
    KSafeArray<KGCWelcomeBackReward*,REWARD_MAX_NUM> m_pkDescReward; // 
    KSafeArray<KGCWelcomeBackItemList*,REWARD_MAX_NUM> m_pkItemList;   // 버튼 누르면 받을 아이템 리스트 

    int                     m_nState;

    std::map<int,std::vector<KDropItemInfo> >  m_mapRecomBonus;
};

DEFINE_WND_FACTORY( KGCWelcomeBackNew );
DECLARE_WND_FACTORY( KGCWelcomeBackNew );
DECLARE_WND_FACTORY_NAME( KGCWelcomeBackNew );
#endif