#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../GCD3DWndEffect.h"

class KD3DSizingBox;

#define MAX_QUEST_RANK_TOTAL_SCORE  5

class KGCQuestRankTotalScore : public KD3DWnd, public KActionListener
{
public:
    KGCQuestRankTotalScore(void);
    virtual ~KGCQuestRankTotalScore(void);

    DECLARE_CLASSNAME( KGCQuestRankTotalScore );

    /// Copy constructor
    KGCQuestRankTotalScore( const KGCQuestRankTotalScore& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestRankTotalScore& operator=( const KGCQuestRankTotalScore& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    //KD3DSizingBox*      m_pkBackGround;
    KSafeArray<KD3DWnd*,MAX_QUEST_RANK_TOTAL_SCORE> m_pkStar;
    KSafeArray<KGCD3DWndEffect,MAX_QUEST_RANK_TOTAL_SCORE> m_Star;

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& ) {}

    void SetTotalScore(int iNumOfStars);

    // 이 함수들은 보간함수들을 이용하여 고정된 윈도우 창을 재미있게 움직이는 역할을 합니다.
    void InitShowGradeStarEffect(void);
    void SetShowGradeStarEffect(int iGrade, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent);
    void FrameMove_GradeStarEffect(KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow);

    bool IsValidQuestRankTotalScoreIndex( int iIndex );

protected:
    bool                m_bInitValue;
    int                 m_iNumOfStars;

};

DEFINE_WND_FACTORY( KGCQuestRankTotalScore );
DECLARE_WND_FACTORY( KGCQuestRankTotalScore );
DECLARE_WND_FACTORY_NAME( KGCQuestRankTotalScore );
