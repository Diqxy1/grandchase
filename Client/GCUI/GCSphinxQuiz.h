#pragma once

class KGCSphinxQuiz : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxQuiz );
    /// Default constructor
    KGCSphinxQuiz( void );
    /// Default destructor
    virtual ~KGCSphinxQuiz( void );
    /// Copy constructor
    KGCSphinxQuiz( const KGCSphinxQuiz& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxQuiz& operator=( const KGCSphinxQuiz& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void SetQuestion(KSphinxQuestionInfo &kQuestionInfo, int nTime);
    void StartQuestion();
    void EndQuestion();
    void DelayEndState();
    void SetResultItem(std::vector<KItem> vecResultItem);

    void CreateSecondUI(int nSecond);
    void UpdateSecondUI();

    void OnClickO();
    void OnClickX();
    void OnCreateComplete( void );
    enum SPHINX_QUIZ_ANSWER{
        SQA_X = 0,
        SQA_O = 1,
        SQA_TIME_OVER =2,
    };

private:
    KD3DWnd *m_pkBtnO;
    KD3DWnd *m_pkBtnX;
    KD3DWnd *m_pkSecond;
    KD3DWnd *m_pkClipRange;
    std::vector<KD3DWnd*> m_vecCreatedList;
    
    KD3DStatic *m_pkStaticSecond;
    KD3DStatic *m_pkStaticQuestion;
    KD3DStatic *m_pkStaticPreQuestion;

    DWORD m_dwDelayTime;
    DWORD m_dwQuestionTime;
    DWORD m_dwDelayEndQuestionTime;
    int   m_iQuestionID;

    std::vector<KItem> m_vecResultItem;
};

DEFINE_WND_FACTORY( KGCSphinxQuiz );
DECLARE_WND_FACTORY( KGCSphinxQuiz );
DECLARE_WND_FACTORY_NAME( KGCSphinxQuiz );
