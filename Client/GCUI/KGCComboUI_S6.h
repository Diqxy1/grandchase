#pragma once

class KGCComboUI_S6 : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCComboUI_S6 );
    /// Default constructor
    KGCComboUI_S6( void );
    /// Default destructor
    virtual ~KGCComboUI_S6( void );
    /// Copy constructor
    KGCComboUI_S6( const KGCComboUI_S6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCComboUI_S6& operator=( const KGCComboUI_S6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

public:
    void Render( bool bRender );
    void RenderForMatch();
    void CheckComboRenderNumber( IN int iCount, OUT std::string& strNumberName );
    void ShowEffect();
    void ShowDisappearEffect( bool bKillImgRender );
    void SetParticle();
    void ShowNumberEffect( int iNum, int iType);
    void CalculateDigit( IN int iTotalScore , OUT std::vector<int>& vecDigit );

public:
    void SetInfinityDungeonInfo( const int iPartyRank_, const int iMonsterLevel_ );
    void SetInfinityBossKillNum( const int iKillNum );
    void RenderInfinityDungeonInfo( const bool bRender_ );
    void SetInfinityDungenRankParticle( void );
    
protected:
    KD3DWnd* m_pkComboBack;
    KD3DWnd* m_pkComboImg;
    KD3DWnd* m_pkTotalScoreImg;
    KD3DWnd* m_pkKillImg;
    

    KGCUINumber* m_pkComboNum;
    KGCUINumber* m_pkTotalScore;
    KGCUINumber* m_pkKillNum;
    

    int m_iPreTotalScore;
    std::vector<std::string> vecComboCntParticle;
    std::vector<std::string> vecTotlScoreParticle;
    
    D3DXVECTOR2 vComboCntPos;
    D3DXVECTOR2 vTotalScorePos;
    bool m_bKillImageRender;

    // 차원의 문 관련 추가 
    enum 
    {
        EXP_RANK_SIZE = 6,
        PARTY_RANK_SIZE = 8,
    };

    KD3DWnd* m_pkPartyRankImg;
    KD3DWnd* m_pkMonsterLevelImg;
    KD3DWnd* m_pkExpRankImg;
    KD3DWnd* m_pkBossKillImg;

    KSafeArray< KD3DWnd*, PARTY_RANK_SIZE > m_apkPartyRank;
    KSafeArray< KD3DWnd*, EXP_RANK_SIZE >   m_apkExpRank;

    KGCUINumber* m_pkMonsterLevelNum;
    KGCUINumber* m_pkBossKillNum;

    int m_iCurrentPartyRank;
    int m_iBeforePartyRank;

};

DEFINE_WND_FACTORY( KGCComboUI_S6 );
DECLARE_WND_FACTORY( KGCComboUI_S6 );
DECLARE_WND_FACTORY_NAME( KGCComboUI_S6 );
