#pragma once
// �˼��մϴ�.. ó�� ��ȹ�ߴ� �༮�� ������ ������ ���̶� �̷��� �̸��� �����µ�....
// �ٸ��뵵�ε� ���� ���� �Ǿ����ϴ�.
class KGCGawibawiboGameLose : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGawibawiboGameLose );
    /// Default constructor
    KGCGawibawiboGameLose( void );
    /// Default destructor
    virtual ~KGCGawibawiboGameLose( void );
    /// Copy constructor
    KGCGawibawiboGameLose( const KGCGawibawiboGameLose& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGawibawiboGameLose& operator=( const KGCGawibawiboGameLose& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum GAWIBAWIBO_RESULT{
		GR_LOSE =	0,
		GR_WIN =	1,
		GR_DRAW =	2,
	};
    virtual void	ActionPerformed( const KActionEvent& event );
    virtual void	OnCreate( void );
	void			SetRetryCoin(int iCoin_);
	void			SetActionFunc(boost::function0<void> a_Func){m_ActionClickFunc = a_Func;}
	void			SetResultInfo();
	void			SetResult(int result_){m_iResult = result_;SetResultInfo();}
	

private:
	KD3DWnd		*m_pkBtnOK;
	KD3DWnd		*m_pkBtnRetry;
	KD3DWnd		*m_pkBtnCheck;

	KD3DStatic*	m_pkStaticTitle1;
	KD3DStatic*	m_pkStaticTitle2;
	KD3DStatic*	m_pkStaticClear;
	KD3DStatic*	m_pkStaticRetry;

	int			m_iRetryCoin;
	int			m_iResult;

	boost::function0<void>					m_ActionClickFunc;
};

DEFINE_WND_FACTORY( KGCGawibawiboGameLose );
DECLARE_WND_FACTORY( KGCGawibawiboGameLose );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGameLose );
