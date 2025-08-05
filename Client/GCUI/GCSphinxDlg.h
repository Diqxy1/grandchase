#pragma once

class KGCSphinxInfoView;
class KGCSphinxQuiz;
class KGCSphinxChallenge;
class KGCSphinxResultItemView;
class KGCSphinxFace;

class KGCSphinxDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxDlg );
    /// Default constructor
    KGCSphinxDlg( void );
    /// Default destructor
    virtual ~KGCSphinxDlg( void );
    /// Copy constructor
    KGCSphinxDlg( const KGCSphinxDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxDlg& operator=( const KGCSphinxDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState();

    enum SPHINX_EVENT_DLG_STATE {
        SEDS_CHALLENGE = 0, // 도전 상태
        SEDS_QUIZ = 1, // 퀴즈
        SEDS_RESULT_ITEM_VIEW = 2, // 보상 아이템 뷰 - 이벤트 정보 보지 않음.
    };

    enum SPHINX_EVENT_FACE_STATE {
        SEFS_NORMAL = 1, // 스핑크스 평소 표정
        SEFS_ANGRY = 2, // 스핑크스 열받은 표정
        SEFS_MOCK = 3, // 스핑크스 비웃는 표정
    };


    void InitSphinxInfo(int iTotalAnswer);
    void SetSphinxState();
    void SetSphinxState(int iState, bool update = true);
    void SetSphinxFace( int iState );
    void SetSphinxResult( int iState , std::vector<KItem> vecItem);
    void SetResultItemList(KEVENT_SPHINX_ITEM_LIST_NOT mapItemList);
    void SetResultItemList( );
    void SetQuizInfo(int iSecondCount, KSphinxQuestionInfo &kQuestionInfo);
    void OnCloseEventBoard( );
    void CreatePaticle();
    void ClearPaticle();
    void UpdateCoin();
    bool GetCreateComplete();

    void close();

private:
    KGCSphinxFace	*m_pkSphinxFace; //스핑크스 이미지

    KD3DWnd *m_pkEndBtn;
    
    KGCSphinxInfoView *m_pkSphinxInfoView; // 코인 / 난이도 / 정답개수
    KGCSphinxQuiz *m_pkSphinxQuiz; // 퀴즈 
    KGCSphinxChallenge *m_pkSphinxChallenge; // 도전 
    KGCSphinxResultItemView *m_pkSphinxResultItemView; // 보상 아이템 뷰

    int m_iState; // 이벤트 DLG 상황 , SPHINX_EVENT_DLG_STATE

    std::vector<CParticleEventSeqPTR> m_vecParticle;

public:
    bool m_bCreateComplete;

};

DEFINE_WND_FACTORY( KGCSphinxDlg );
DECLARE_WND_FACTORY( KGCSphinxDlg );
DECLARE_WND_FACTORY_NAME( KGCSphinxDlg );
