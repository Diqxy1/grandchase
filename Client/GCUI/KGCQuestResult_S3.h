#pragma once

#define ROUND_OFF(f) (int((f)+0.5))

class KGCQuestObtainGPEXP;
class KGCScoreAnimUI;
class KGCBonusBoxSelect_S4;
class KGCBonusBoxS6;

class KGCQuestResult_S3 : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCQuestResult_S3 );
    /// Default constructor
    KGCQuestResult_S3( void );
    /// Default destructor
    virtual ~KGCQuestResult_S3( void );
    /// Copy constructor
    KGCQuestResult_S3( const KGCQuestResult_S3& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestResult_S3& operator=( const KGCQuestResult_S3& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }    

    struct KMoveProperty
    {
        int m_iStartX; 
        int m_iStartY;
        int m_iEndX;
        int m_iEndY;
        int m_iMoveX;
        int m_iMoveY;
        int m_iSpeed;

        void Init()
        {
            m_iStartX   = 0;
            m_iStartY   = 0;
            m_iEndX     = 0;
            m_iEndY     = 0;
            m_iMoveX    = 0;
            m_iMoveY    = 0;
            m_iSpeed    = 0;
        }
    };

    struct KPlayTime
    {
        int m_iMinute1;
        int m_iMinute2;
        int m_iSecond1;
        int m_iSecond2;

        void Init()
        {
            m_iMinute1 = 0;
            m_iMinute2 = 0;
            m_iSecond1 = 0;
            m_iSecond2 = 0;
        }
    };

    enum EAnimScene
    {
        EANIM_SCENE_STOP = -1,
        EANIM_SCENE_BACKGROUND = 0, //백그라운드 애님 씬.
        EANIM_SCENE_MAIN_CONTENTS,
        EANIM_SCENE_PLAY_TIME,
        EANIM_SCENE_RANK_SCORE,
        EANIM_SCENE_RECORD_CHECK,
        EANIM_SCENE_BONUS,
        EANIM_SCENE_RESULT_END,
        EANIM_SCENE_SELECT_BONUS_BOX,
        EANIM_SCENE_END_GAME,
    };

    enum
    {
        EALPHA_WEIGHT = 5,
        EMAX_CONTENTS_BG = 5,
        EMAX_CONTENTS_SUB_BG = 8,
        EMAX_LEVEL_STAR = 3,
        EDELAY_END_TIME = 315,
        EMAX_ALPHA_VALUE = 255,
        EADD_DELAY_END_TIME = 25,
        EMAX_BONUS_VITALITY = 2,
        ADD_HEIGHT_RESULT_END = 25,
    };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroy( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );


	void SkipSet();

private:
    KSafeArray<KD3DSizingBox*, EMAX_CONTENTS_BG>        m_pkContentsBG;
    KSafeArray<KD3DWnd*, EMAX_CONTENTS_BG>              m_pkContentsTitle;
    KSafeArray<KD3DSizingBox*, EMAX_CONTENTS_SUB_BG>    m_pkContentsSubBG;
    KSafeArray<KD3DWnd*, EMAX_LEVEL_STAR>               m_pkLevelStarBG;

    KGCScoreAnimUI*                                     m_pkBounusAnimAllClear;
    KGCScoreAnimUI*                                     m_pkBonusAnimItem;
    KGCScoreAnimUI*                                     m_pkBonusAnimEvent;
    KGCScoreAnimUI*                                     m_pkBonusAnimPCRoom;
	KGCScoreAnimUI*                                     m_pkBonusAnimServer;
	KGCScoreAnimUI*                                     m_pkBonusAnimGuild;
    KGCScoreAnimUI*                                     m_pkBonusAnimRank;

    KGCQuestObtainGPEXP*                                m_pkQuestObtainGPEXP;    

    KD3DWnd*                                            m_pkQuestLevelHell;
    KD3DWnd*                                            m_pkQuestLevelEventHell;
    KD3DWnd*                                            m_pkQuestLevelTime;
    KD3DWnd*                                            m_pkQuestLevelEventTime;

    KD3DWnd*                                            m_pkBonusAllClear;
    KD3DWnd*                                            m_pkBonusItem;
    KD3DWnd*                                            m_pkBonusEvent;
    KD3DWnd*                                            m_pkBonusPCRoom;
	KD3DWnd*											m_pkBonusServer;
	KD3DWnd*											m_pkBonusGuild;
    KSafeArray<KD3DWnd*, EMAX_BONUS_VITALITY>			m_pkBonusVitality;

    KD3DWnd*                                            m_pkQuestResultTitle;
    KD3DWnd*                                            m_pkMinuteSign;
    KD3DWnd*                                            m_pkMinuteSignBest;
    KD3DSizingBox*                                      m_pkBackgroundMain;
    KD3DSizingBox*                                      m_pkBackgroundEnd;

    KD3DWnd*											m_pkTimeOver;
    KD3DWnd*											m_pkLevleText;

    KGCBonusBoxSelect_S4*								m_pkBonusBoxSelect;

    KMoveProperty                                       m_sBGMoveProperty;
    KPlayTime                                           m_sPlayTime;
    KPlayTime                                           m_sPrevPlayTime;
    EAnimScene                                          m_eAnimScene;

    GCEXPTYPE                                           m_iOriginalExp;
    GCEXPTYPE		                                    m_iStageBonusExp;
    GCEXPTYPE		                                    m_iRankBonusExp;
    GCEXPTYPE		                                    m_iRankBonusGp;
    GCEXPTYPE		                                    m_iTotalExp;
    int                                                 m_iTotalGP;
    std::map<int, float>                                m_mapBonusExpInfo;

    int                                                 m_iOriginalGP;    
    int                                                 m_iStageBonusGp;
    std::map<int, float>                                m_mapBonusGPInfo;

    int                                                 m_iOriginalSPExp;    
    std::map<int, float>                                m_mapBonusSPExpInfo;

    int                                                 m_iAlpha;
    int                                                 m_iFrameCount;
    int                                                 m_iBonusAnimCount;
    int                                                 m_iEndDelayTime;

    bool                                                m_bRenderLevelNum;     //  난이도를 숫자로 표시
    int                                                 m_nDungeonLevel;

	KD3DStatic*											m_pkStaticSkip;
	KGCBonusBoxS6*										m_pkBonusBoxS6;
	int													m_nRewardType;
    KD3DWnd*                                            m_pkZkey;
    bool                                                m_bNewRecordPlayTime;
    KD3DWnd*                                            m_pkClearNewImg;
    KD3DWnd*                                            m_pkDamageRankNewImg;
    int                                                 m_ClearNewRecordAlpha;
    int                                                 m_DamageNewRecordAlpha;

public:        
    void SetQuestResult();
    void SetMoveBackground( int iStartX = 0, int iStartY = 0, int iEndX = 0, int iEndY = 0, int iSpeed = 0, int iSceneType = EANIM_SCENE_STOP );
    void SetWndAlpha( int iAlpha_ );
    void SetQuestLevel( int nDungeonLevel );
    void InitQuestLevel();

    void SetExp( DWORD dwPlayerUID, GCEXPTYPE biExp );
    void SetLocalUserStageBonusExp( GCEXPTYPE biExp );
    void SetLocalUserRankBonusExp( GCEXPTYPE biExp ) { m_iRankBonusExp = biExp; }
    void SetLocalUserRankBonusGp( GCEXPTYPE biExp ) { m_iRankBonusGp = biExp; }
    void SetLocalUserBonusExpInfo( std::map<int, float>& mapInfo_ );

    void SetTotalExp( GCEXPTYPE biExp ) { m_iTotalExp = biExp; }
    void SetTotalGp( int iGp ) { m_iTotalGP = iGp; }
    void SetGp( DWORD dwPlayerUID, int iGp );    
    void SetLocalUserStageBonusGp( int iGp );
    void SetLocalUserBonusGpInfo( std::map<int, float>& mapInfo_ );


    void SetLocalUserBonusSPExpInfo( std::map<int, float>& mapInfo_ );

    int GetLocalUserRankBonusExp() { return (int)m_iRankBonusExp; }
    int GetLocalUserRankBonusGp() { return (int)m_iRankBonusGp; }
    int GetLocalUserBaseEXP() { return (int)m_iOriginalExp; }
    int GetLocalUserBaseGP() { return m_iOriginalGP; }
       
    bool IsCompleteBonusAnim();

    void CountPlayTime();
    void RenderContents( bool bRender_ );
    
    void AnimationBackground( int iNextSceneType, bool bWndRender );
    void AnimationMainContents(bool bIncreaseAlpha_ );    
    void AnimEndResultWnd( int iWeight );
    void AnimationEndBackground( int iWeight );

    KGCQuestObtainGPEXP* GetQuestObtainGPEXP() { return m_pkQuestObtainGPEXP; }
    void AdjustWnd( void );
    void SetLevelBGRender( bool bRender_ );


    DWORD m_dwInitHeight;
public:
    KGCBonusBoxSelect_S4* GetBonusBoxSelect(){ return m_pkBonusBoxSelect; } 
	void SetRewardType( int nType ) { m_nRewardType = nType; }
	void SetRewardItem( std::map<int, KDropItemInfo> mapRewardItem, int iChar, std::wstring strNickName );
    DWORD GetDungeonPrevClearRecord();
    void AnimationNewRecord( int iType , int iWeight );
    bool CanMoveNextScene();
    void SkipNewRecord( );
    void GetExpPercentForRank(IN int iRankIndex , OUT int& iPercent);
};

DEFINE_WND_FACTORY( KGCQuestResult_S3 );
DECLARE_WND_FACTORY( KGCQuestResult_S3 );
DECLARE_WND_FACTORY_NAME( KGCQuestResult_S3 );
