#pragma once
#include "..\GCGuideMgr.h"
#include "GCHighLight.h"


enum { // GCGuide NPC Enum
    GNPCT_KNIGHT_0 = 0,     // 기사단장 큰거 왼쪽
    GNPCT_KNIGHT_1 = 1,     // 기사단장 큰거 오른쪽
    GNPCT_KNIGHT_2 = 2,     // 기사단장 작은거 왼쪽
    GNPCT_KNIGHT_3 = 3,     // 기사단장 작은거 왼쪽
    GNPCT_MAX
};

enum { // GCGuide Arrow Enum
    GAT_NORMAL_0 = 0,     // 11시 방향 화살표
    GAT_NORMAL_1 = 1,     // 7시 방향 화살표
    GAT_MAX
};

enum { //Highlight Type
    GHT_NORMAL = 0,
    GHT_NORMAL_WITHOUT_RECT,
    GHT_NO_BLIND_WITHOUT_RECT,
};

const DWORD dwHighlightDefaultSize = 34; // 사이징 박스 기본 크기

class KGCHighLight;
class KGCGuideNPC;
class KGCGuideArrow;

struct SStep;
struct SAction;
struct SHighlightInfo;
struct SNPCInfo;
struct SArrowInfo;
struct SGuidePaticleInfo;

class KGCGuide:    public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuide );
    /// Default constructor
    KGCGuide( void );
    /// Default destructor
    virtual ~KGCGuide( void );
    /// Copy constructor
    KGCGuide( const KGCGuide& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuide& operator=( const KGCGuide& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetCurrentGuide( IN int nGuideType, IN std::vector<SStep> vecStep );
    void SetActive();
    void FrameMove();
    void SetCompleteAction( IN bool bAction );
    int GetCurrStep(){ return m_nCurrentStep; }

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

    void SetAction( IN SAction& kAction );
    void SetHighlightRect( IN SHighlightInfo& kHighlightInfo );
    void SetNPCInfo( IN SNPCInfo& kNPCInfo );
    void SetArrowInfo( IN SArrowInfo& kArrowInfo );
    void SetParticle( IN std::vector<SGuidePaticleInfo>& vecParticle );
    void ClearPaticle();
    void GoNextStep();
    void EndCurrentStep();
    void UseExtraFunction();

protected:
    // 하이라이트 영역 저장용
    float m_fx_hl;
    float m_fy_hl;
    DWORD m_dwWidth_hl;
    DWORD m_dwHeight_hl;
    bool m_bUseHighlightClick;

    // 시간
	DWORD m_dwTime;
    DWORD m_dwStartTime;

    // NPC
    int m_nNPCType;

    // 아이템 수령
    bool m_bItemRecieve;

    // 스텝 정보
    bool m_bCompleteAction;
    int m_nCurrentGuideType;
    int m_nCurrentStep;
    std::vector<SStep> m_vecStep; 

    KGCHighLight* m_pkHighlight;
    KGCHighLight* m_pkNoHighlight;
    KGCHighLight* m_pkNoBlindHighlight;
    KGCGuideNPC* m_pkGuideNPC[GNPCT_MAX];
    KGCGuideArrow* m_pkGiudeArrow;
    std::vector<CParticleEventSeqPTR> m_vecParticle;

};

DEFINE_WND_FACTORY( KGCGuide );
DECLARE_WND_FACTORY( KGCGuide );
DECLARE_WND_FACTORY_NAME( KGCGuide );


/////////////////////////// GCGuide End



class KGCGuideNPC:    public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuideNPC );
    /// Default constructor
    KGCGuideNPC( void );
    /// Default destructor
    virtual ~KGCGuideNPC( void );
    /// Copy constructor
    KGCGuideNPC( const KGCGuideNPC& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuideNPC& operator=( const KGCGuideNPC& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetPosition( IN float fx, IN float fy );
    void SetDesc( IN int nDescStrID );
    void FrameMoveMent();

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event);

    KD3DWnd*            m_pkNPC;
    KD3DWnd*            m_pkBalloon;
    KD3DStatic*         m_pkDesc;

    std::wstring        m_strOriginalMent;
    std::wstring        m_strMentAnim;


};

DEFINE_WND_FACTORY( KGCGuideNPC );
DECLARE_WND_FACTORY( KGCGuideNPC );
DECLARE_WND_FACTORY_NAME( KGCGuideNPC );


///////////////////// Guide NPC end


const DWORD dwFlipTime = 200; // 화살표 깜빡이는 주기 1000 = 1sec

class KGCGuideArrow:    public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuideArrow );
    /// Default constructor
    KGCGuideArrow( void );
    /// Default destructor
    virtual ~KGCGuideArrow( void );
    /// Copy constructor
    KGCGuideArrow( const KGCGuideArrow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuideArrow& operator=( const KGCGuideArrow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetArrowType( IN int nType );
    void SetPosition( IN float fx, IN float fy );

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    KD3DWnd* m_pkArrow[GAT_MAX];

    int nCurrentArrow;
    DWORD m_dwAnimTime;
    ED3DWndMode m_eWndState;
};

DEFINE_WND_FACTORY( KGCGuideArrow );
DECLARE_WND_FACTORY( KGCGuideArrow );
DECLARE_WND_FACTORY_NAME( KGCGuideArrow );



///////////////////// Guide Arrow end


