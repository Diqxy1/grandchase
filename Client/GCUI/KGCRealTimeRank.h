#pragma once

class KD3DSizingBox;

class KGCRealTimeRank :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRealTimeRank );
    /// Default constructor
    KGCRealTimeRank( void );
    /// Default destructor
    virtual ~KGCRealTimeRank( void );
    /// Copy constructor
    KGCRealTimeRank( const KGCRealTimeRank& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRealTimeRank& operator=( const KGCRealTimeRank& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EQuestRankSing 
    { 
        QUEST_RG_DEFAULT = -1,
        QUEST_RG_BAD = 0, 
        QUEST_RG_GOOD, 
        QUEST_RG_GREAT,
        QUEST_RG_PERFECT, 
        NUM_OF_QUEST_RG 
    };

    enum EQuestRankScore
    {
        S3_RANK_PERFECT_POINT = 95,
        S3_RANK_GREAT_POINT = 80,
        S3_RANK_GOOD_POINT = 60,
    };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event ) {}
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    UINT            m_uiPointControl;
    UINT            m_uiPointStageClear;
    UINT            m_uiPointRank;
    UINT            m_uiMovePointRank;

    float           m_fWidthRatio;

    int             m_iPlusAlpha;
    int             m_iMinusAlpha;
    int             m_iWndAlpha;
    bool            m_bEndAnim;
    bool            m_bShowState;

    KD3DSizingBox*  m_pkRankGuage;

    EQuestRankSing  m_eSignType;
    EQuestRankSing  m_ePrevSignType;

    KSafeArray<KD3DWnd*, NUM_OF_QUEST_RG> m_pkRankSign;

public:
    void SetControlPoint( UINT uiPoint_ );
    void SetRankPoint( UINT uiPoint_ );
    void SetStageClearPoint( UINT uiPoint_ ) { m_uiPointStageClear = uiPoint_; }    

    void AnimationRankGurage();
    void AnimationRankSign();
    void SetRankSign();

    void SetShowState( bool bShow );

    void ShowWndAnim();
};

DEFINE_WND_FACTORY( KGCRealTimeRank );
DECLARE_WND_FACTORY( KGCRealTimeRank );
DECLARE_WND_FACTORY_NAME( KGCRealTimeRank );