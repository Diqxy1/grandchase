#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../GCUtil/GCInterpolate.h"
#include "GCCameraOrtho.h"
//#include "../GCDeviceManager2.h"
#include "TypeDef.h"

class KD3DSizingBox;
class KGCNumberControl;

#define CONVERT_PIXEL_TO_TEX_SIZE(Size, TSize)  ( (float)Size/(float)TSize )
#define MAX_QUEST_RANK_SCORE_NUMBER             11
#define MAX_NUM_OF_NUMBER                       10
#define MAX_NUM_OF_NUMBER_DIVIDE                1000000000      // 1까지 포함해서 8자리 수를 만들어준다.
#define CHANGE_SCORE_FRAME                      GC_FPS_LIMIT*2

class KGCQuestRankScore : public KD3DWnd, public KActionListener
{
public:
    KGCQuestRankScore(void);
    virtual ~KGCQuestRankScore(void);

    DECLARE_CLASSNAME( KGCQuestRankScore );

    /// Copy constructor
    KGCQuestRankScore( const KGCQuestRankScore& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestRankScore& operator=( const KGCQuestRankScore& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    //KD3DSizingBox*      m_pkBackGround;

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& ) {}

    GCEXPTYPE              m_biScore;               // 점수
    KGCLinearInterpolate   m_stScore;              // SetLinearScore 함수를 사용하여 점수를 설정할 시 
                                                   // 이 보간 함수를 이용하여 점수를 천천히 올린다.
   
public:
    void SetNumOfNumber(int iMaxNumOfNumber);      // 숫자 최대 자리 수 설정
    void SetScore( GCEXPTYPE biScore_ );                     // 점수 설정
    void SetLinearScore( GCEXPTYPE biScore_ );               // 천천히 올라가는 점수 설정
	GCEXPTYPE GetScore()	{ return m_biScore; }

	bool IsScoreAnimComplete()	{ return m_stScore.IsComplete(); }
	void SetEndScore()			{ m_stScore.SetEnd(); }

    // 숫자를 찍을 때 사용하는 것들
    void SetNumPos(D3DXVECTOR2 vPos);
    void SetNumWidth(float fWidth);
    void SetNumHeight(float fHeight);
    void SetNumInterval(float fInterval);
    void DrawNumber(D3DXVECTOR2 vPos, int iNum, DWORD dwColor_, float fScale = 1.0f);

    void SetColor( DWORD dwColor_ ) { m_dwColor = dwColor_; }

protected:
    int                     m_iMaxNumOfNumber;      // 최대 숫자 자리 수
    GCDeviceTexture*        m_pDungeon;                                    // 던젼 미니맵용 텍스쳐
    KSafeArray<FRect,MAX_QUEST_RANK_SCORE_NUMBER> m_rtNumber;        // 숫자의 UV 좌표를 저장해둔다.

    D3DXVECTOR2             m_vPos;                 // 숫자의 위치
    float                   m_fNumWidth;            // 숫자의 넓이
    float                   m_fNumHeight;           // 숫자의 높이
    float                   m_fNumInterval;         // 숫자간 간격

    float                   m_fCurScore;
    float                   m_fPrevScore;

    DWORD                   m_dwColor;
};

DEFINE_WND_FACTORY( KGCQuestRankScore );
DECLARE_WND_FACTORY( KGCQuestRankScore );
DECLARE_WND_FACTORY_NAME( KGCQuestRankScore );
