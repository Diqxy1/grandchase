#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../GCD3DWndEffect.h"

#define QUEST_RANK_SCALE_FRAME  (GC_FPS_LIMIT*1.3f)
#define QUEST_RANK_SHOW_FRAME   (GC_FPS_LIMIT*3.6f)
#define QUEST_RANK_MAX_SCALE    3000
#define QUEST_RANK_MIN_SCALE    100

class KD3DNumber;
class KD3DSizingBox;
class KGCQuestRankTotalScore;

class KGCMinigameResult : public KD3DWnd,
	                      public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCMinigameResult );
	/// Default constructor
	KGCMinigameResult( void );
	/// Default destructor
	virtual ~KGCMinigameResult( void );
	/// Copy constructor
	KGCMinigameResult( const KGCMinigameResult& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMinigameResult& operator=( const KGCMinigameResult& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum QUEST_RANK_GRADE { QUEST_RG_FAIL, QUEST_RG_BAD, QUEST_RG_GOOD, QUEST_RG_PERFECT, NUM_OF_QUEST_RG };

public:
    void InitNumberTextureCoordinate( void );
    void SetMyRanking( const int iRank );
    void SetPlayTime( void );
    void SetSec( const int iSec );
    void SetMinute( const int iMinute );
    void SetHighScore( const int iHighScore );
    void SetCurrentScore( const int iCurrentScore );
    int GetDigit( const int iValue );
    int GetDigitNumber( const int iValue, const int iOffset );
    void InitData( void );
    void SetTotalStar( const int iNumOfStars );
    void SetRankLowDesc( bool bToggle );

    // 이 함수들은 보간함수들을 이용하여 윈도우(KD3DWnd) 창을 재미있게 움직이는 역할을 합니다.
    void InitShowGradeEffect(void);
    void FrameMove_GradeEffect( KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow, bool bEvent = false );        // 순차적으로 스케일, 블랜딩이 진행되는 함수
    void SetTotalScoreRankImage( int iRank );
    void SetShowGradeEffect( int iGrade, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent );

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

private:
	KD3DWnd*					m_pkRankDesc;
    KD3DWnd*                    m_pkRestartBtn;
    KD3DWnd*                    m_pkEndGameBtn;
    KD3DWnd*                    m_pkRankLowDesc;
    KSafeArray<KD3DWnd*,NUM_OF_QUEST_RG> m_pkRankGrade;
    KGCQuestRankTotalScore*     m_pkRankStarScore;
    KSafeArray<KD3DNumber*,4> m_pkMyCurrentRanking;
    KSafeArray<KD3DNumber*,4> m_pkHighScore;
    KSafeArray<KD3DNumber*,4> m_pkPlayTime;
    KSafeArray<KD3DNumber*,4> m_pkCurrentScore;
    KSafeArray<KGCD3DWndEffect,NUM_OF_QUEST_RG> m_GradeInfo;       // 등급 KD3dWnd의 애니매이션 정보 (Scale, Blend, Position 등등의 정보가 있음)

    bool                        m_bIniValue;            // 윈도우 기본위치 저장 여부 (KD3DWnd의 초기 위치 - 크기가 변경되며 애니매이션 되기 때문)
    int                         m_iGrade;               // 등급    
    DWORD                       m_dwStartTime;
};

DEFINE_WND_FACTORY( KGCMinigameResult );
DECLARE_WND_FACTORY( KGCMinigameResult );
DECLARE_WND_FACTORY_NAME( KGCMinigameResult );
