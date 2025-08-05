#pragma once
// 죄송합니다.. 처음 계획했던 녀석이 질때만 나오는 것이라 이렇게 이름을 지었는데....
// 다른용도로도 같이 쓰게 되었습니다.
class KGCOlympicGawibawiboGameLose : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicGawibawiboGameLose );
    /// Default constructor
    KGCOlympicGawibawiboGameLose( void );
    /// Default destructor
    virtual ~KGCOlympicGawibawiboGameLose( void );
    /// Copy constructor
    KGCOlympicGawibawiboGameLose( const KGCOlympicGawibawiboGameLose& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicGawibawiboGameLose& operator=( const KGCOlympicGawibawiboGameLose& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum GAWIBAWIBO_RESULT{
        ER_WIN     = 1,
        ER_LOSE    = 2,
        ER_DRAW    = 3,
    };
    virtual void	ActionPerformed( const KActionEvent& event );
    virtual void	OnCreate( void );
    void			SetRetryCoin(int iCoin_);
    void			SetActionFunc(boost::function0<void> a_Func){m_ActionClickFunc = a_Func;}
    void			SetResultInfo();
    void            SetReqInfo( time_t  tmTodayDate, int nGameType, int nTeamType );
    void			SetResult(int result_){m_iResult = result_;SetResultInfo();}

private:
    KD3DWnd		*m_pkBtnOK;
    KD3DWnd		*m_pkBtnRetry;
    KD3DWnd		*m_pkBtnCheck;

    KD3DStatic*	m_pkStaticTitle1;
    KD3DStatic*	m_pkStaticTitle2;
    KD3DStatic*	m_pkStaticClear;
    KD3DStatic*	m_pkStaticRetry;

    int			m_iCoinCount;
    int			m_iResult;

    boost::function0<void>					m_ActionClickFunc;
	
};

DEFINE_WND_FACTORY( KGCOlympicGawibawiboGameLose );
DECLARE_WND_FACTORY( KGCOlympicGawibawiboGameLose );
DECLARE_WND_FACTORY_NAME( KGCOlympicGawibawiboGameLose );
