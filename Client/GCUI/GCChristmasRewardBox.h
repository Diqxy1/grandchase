#pragma once


enum {
    CRRS_NOT_ENOUGH_POINT = 0,
    CRRS_GET_REWARD,
    CRRS_ALREADY_GOT_REWARD,
};


class KGCChristmasRewardBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasRewardBox );
    /// Default constructor
    KGCChristmasRewardBox( void );
    /// Default destructor
    virtual ~KGCChristmasRewardBox( void );
    /// Copy constructor
    KGCChristmasRewardBox( const KGCChristmasRewardBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasRewardBox& operator=( const KGCChristmasRewardBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }



private:
    KD3DWnd*                m_pkBtnGetReward;
    KD3DWnd*                m_pkBtnRewardText;
    KD3DWnd*                m_pkBtnInvisible;
    KD3DWnd*                m_pkFrame;
    KD3DWnd*                m_pkRewardItemImgOver;
    
    KGCItemImgWnd*          m_pkRewardItemImgWnd;

    // data
    int                     m_nState;        // 누적포인트에 따른 현재 상태
    VEC_REWARD              m_vecRewardList; // 보상리스트
    
    DWORD                   m_dwGrade;        // 이 버튼은 몇 단계 버튼인가



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickGetReward();

    void SetRewardInfo( DWORD dwGrade, VEC_REWARD& vecReward );
    void UpdateRewardBox();
    void InitRewardBox();
    void SetRewardState( int nState ) { m_nState = nState; }
    void DrawInfoWindow();


};

DEFINE_WND_FACTORY( KGCChristmasRewardBox );
DECLARE_WND_FACTORY( KGCChristmasRewardBox );
DECLARE_WND_FACTORY_NAME( KGCChristmasRewardBox );
