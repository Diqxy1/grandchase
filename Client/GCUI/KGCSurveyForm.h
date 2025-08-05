#pragma once

class KGCSurveyExample;

class KGCSurveyForm : public KD3DWnd,
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSurveyForm );
    /// Default constructor
    KGCSurveyForm( void );
    /// Default destructor
    virtual ~KGCSurveyForm( void );
    /// Copy constructor
    KGCSurveyForm( const KGCSurveyForm& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSurveyForm& operator=( const KGCSurveyForm& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DStatic*     m_pkStaticIntroduce;
    KD3DStatic*     m_pkStaticReward;
    KD3DStatic*     m_pkStaticPage;
    KD3DStatic*     m_pkBtnNext;
    KD3DStatic*     m_pkBtnDone;
    
    KSafeArray< KGCSurveyExample*, KGCSurveyExample::ESURVEY_EXAMPLE_NUM > m_pkExample;
    KSafeArray< KGCSurveyExample*, KGCSurveyExample::ESURVEY_EXAMPLE_NUM > m_pkExampleImg;

    KGCSurveyExample* m_pkExampleInput;

    int m_iFormType;

    int m_iSurveyType;
    int m_iDataFirst;
    int m_iDataSecond;
    std::map< int, std::vector< int > > m_mapNumberAnswer;
    std::map< int, std::wstring >       m_mapStringAnswer;

    int m_iMaxNumOfAnswer;
    int m_iCurQuestionNum; // 현재 보여주고 있는 문제번호
    int m_iQuestionPage; // 현재 문제 페이지 번호

    std::deque< std::wstring >          m_deqAnswer;
    std::map< int, KSurveyQuestion >    m_mapSurveyQuestion;

public:
    void SetFormType( int iType_, unsigned int unExampleSize_ );
    void SetNumOfAnswer( int iNum_ ) { m_iMaxNumOfAnswer = iNum_; }    
    void SetSurveyData( int iSurveyType_, int iDataFirst_, int iDataSecond_, std::map< int, KSurveyQuestion >& mapSurveyQuestion_ );  

    void SetQuestionText( std::wstring& wsQuestion_ );
    void SetQuestionNumberText( std::wstring wsNumberText_ );
    void SetPageNumberText( int iCurrentPage_, int iLastPage_ );
    void SetExampleText( int iExampleCnt_, std::map< int, std::wstring >& mapExampleText_ );

    void GetUserAnswers( std::map< int, std::vector< int > >& mapNumberAnswer_, std::map< int, std::wstring >& mapStringAnswer_ );

    void RenderExampleBox( KGCSurveyExample* pkExample_, const D3DXVECTOR2& vMousePos_, int iIdx_ );
    void RenderEditBox();
    void RenderQuestion( int iNum_ ); // 파라미터는 문제번호
    int GetFirstQuestionNumber();

    void InitExampleInfo();

    void OnNext();
    void OnDone();

    void ClearData();
};

DEFINE_WND_FACTORY( KGCSurveyForm );
DECLARE_WND_FACTORY( KGCSurveyForm );
DECLARE_WND_FACTORY_NAME( KGCSurveyForm );
