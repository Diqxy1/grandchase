#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "GCSayBalloon.h"
//#include "GCHighLight.h"


////////////// GCGuide로 대체합니다.2013-07-09
//
class KGCSayBalloon;
class KGCHighLight;

class KGCGuideNewbie:    public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuideNewbie );
    /// Default constructor
    KGCGuideNewbie( void );
    /// Default destructor
    virtual ~KGCGuideNewbie( void );
    /// Copy constructor
    KGCGuideNewbie( const KGCGuideNewbie& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuideNewbie& operator=( const KGCGuideNewbie& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        GCGN_LEFT_KNIGHT = 0,
        GCGN_RIGHT_KNIGHT = 1,
        GCGN_KNIGHT_NUM,
    };
public:
    void SetPointPos( float x, float y, bool bIsUp );
    void SetHighLightPos( float fx, float fy, DWORD dwHeight, DWORD dwWidth );
    void SetDesc( const std::wstring& strDesc );
    void SetSide( bool bIsRight );
    void SetParticle(std::vector<SGuidePaticleInfo>& vecParticle);
    bool GoNextGuide();
	void SetMouseClickPos(D3DXVECTOR2 vPos_);

    virtual void ToggleRender(bool bRender);

    void SetCompleateActionNum(int num){m_iCompleateAction = num;}

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event);

    void RefreshSayBalloonPos();
    void RefreshPointPos( int iFrame );
    void RefreshMent();
    bool CheckGoNextGuide();
    void ClearPaticle();

    void SetView(bool arg_);

protected:
    KSafeArray<KD3DWnd*,GCGN_KNIGHT_NUM> m_pkKnight;
    KD3DWnd*	m_pkUpPoint;
    KD3DWnd*	m_pkDownPoint;
    KD3DStatic*	m_pkMent;
    KSafeArray<KGCSayBalloon*,GCGN_KNIGHT_NUM> m_pkSayBalloon;
    KGCHighLight*	m_pkHighLight;

    std::wstring	m_strOriginalMent;
    std::wstring	m_strMentAnim;

    float	m_fPointOffset;
    bool	m_bIsUpPoint;
    bool	m_bIsRight;
    D3DXVECTOR2	m_vPointPos;


	DWORD		m_dwAnimTime;
	bool		m_bShowMouseClick;
	KD3DWnd*	m_pkMouseClick1;
	KD3DWnd*	m_pkMouseClick2;
    int         m_iCompleateAction;

    std::vector<CParticleEventSeqPTR> m_vecParticle;

};

DEFINE_WND_FACTORY( KGCGuideNewbie );
DECLARE_WND_FACTORY( KGCGuideNewbie );
DECLARE_WND_FACTORY_NAME( KGCGuideNewbie );
