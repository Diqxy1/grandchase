#pragma once

class KGCItemImgWnd;
class KD3DStatic;
class KGCSurveyForm;

class KGCSurveyDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSurveyDlg );
    /// Default constructor
    KGCSurveyDlg( void );
    /// Default destructor
    virtual ~KGCSurveyDlg( void );
    /// Copy constructor
    KGCSurveyDlg( const KGCSurveyDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSurveyDlg& operator=( const KGCSurveyDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    enum {
        EITEM_WND_NUM = 5,
    };

    enum {
        QUESTION_NORMAL     = 0,
        QUESTION_IMAGE      = 1,
        QUESTION_INPUT      = 2,
    };
    struct sQuestion 
    {
        int                                           m_iType;         // 0: 게임접속시, 1: 던전클리어, 2: 캐릭터 레벨업시
        int                                           m_iDataFirst;    // 던전 ID, CharType
        int                                           m_iDataSecond;   // 던전 난위도, CharLevel
        std::map< int, KSurveyQuestion >              m_mapSurveyInfo;
    };

    void OnYes();
    void OnNo();

    void SetQuestionList( int iType_, int iDataFirst_, int iDataSecond_, std::map< int, KSurveyQuestion >& mapSurveyInfo_ );
    void SetRewardList( std::map< int, std::vector< KDropItemInfo > >& mapNormalReward_ );

    void SetQuestionData();

    void RenderRewardItem();

    void OnSurveyRewardAck( std::vector< KItem >& vecReward_ );

    bool IsReserveQuestion() { return m_bReserveQuestion; }
    bool IsHaveQuestion();
    void SetHaveQuestion( bool bHaveQuestion_ );

    void AddQuestion( sQuestion& sQuestion_ );
    void GetQuestion( sQuestion& sQuestion_ );

    void OnEventSurveyListNot( KEVENT_SURVEY_LIST_NOT& kRecv, bool bReserve_ = false );


private:
    KD3DWnd* m_pkBtnYes;
    KD3DWnd* m_pkBtnNo;

    KD3DStatic* m_pkStaticIntroduce;
    KD3DStatic* m_pkStaticRewardDesc;
    KD3DStatic* m_pkStaticReward;

    KGCSurveyForm* m_pkSurveyForm;

    KSafeArray<KGCItemImgWnd*, EITEM_WND_NUM>     m_pkImgNormalReward;

    std::vector< sQuestion >                         m_vecQuestionList;

    int                                           m_iType;         // 0: 게임접속시, 1: 던전클리어, 2: 캐릭터 레벨업시
    int                                           m_iDataFirst;    // 던전 ID, CharType
    int                                           m_iDataSecond;   // 던전 난위도, CharLevel
    std::map< int, KSurveyQuestion >              m_mapSurveyInfo;

    std::map< int, std::vector< KDropItemInfo > > m_mapNormalReward;    // 보상 리스트
    
    bool                                          m_bResolvedFirstQuestion;

    bool                                          m_bReserveQuestion;
};

DEFINE_WND_FACTORY( KGCSurveyDlg );
DECLARE_WND_FACTORY( KGCSurveyDlg );
DECLARE_WND_FACTORY_NAME( KGCSurveyDlg );
