#ifndef _KGCWELCOMEBACK_H_
#define _KGCWELCOMEBACK_H_

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

class KGCWelcomeBack : public KD3DDialog,
	                   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCWelcomeBack );
	/// Default constructor
	KGCWelcomeBack( void );
	/// Default destructor
	virtual ~KGCWelcomeBack( void );
	/// Copy constructor
	KGCWelcomeBack( const KGCWelcomeBack& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCWelcomeBack& operator=( const KGCWelcomeBack& )
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
    KSafeArray<KD3DStatic*,2> m_pkStaticDesc;       //기존에 줄 바꿈 방식에 버그가 있어서 [1]번째 배열 쓰지 않도록 코드 수정했음
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

DEFINE_WND_FACTORY( KGCWelcomeBack );
DECLARE_WND_FACTORY( KGCWelcomeBack );
DECLARE_WND_FACTORY_NAME( KGCWelcomeBack );

#endif