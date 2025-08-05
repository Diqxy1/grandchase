#pragma once

class KGCGawibawiboGameInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGawibawiboGameInfo );
    /// Default constructor
    KGCGawibawiboGameInfo( void );
    /// Default destructor
    virtual ~KGCGawibawiboGameInfo( void );
    /// Copy constructor
    KGCGawibawiboGameInfo( const KGCGawibawiboGameInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
	const KGCGawibawiboGameInfo& operator=( const KGCGawibawiboGameInfo& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	
	void	SetGameInfo(const KEVENT_GAWIBAWIBO_INFO_ACK &recvInfo_);
	void	FrameMoveInEnabledState( void );
	void	SetActionFunc(boost::function0<void> a_Func){m_ActionClickFunc = a_Func;}
	void	SetMaxTryPointTime(int iTime_){m_iMaxTryPointTime = iTime_*60;}
	void	SetTryPointInfo(DWORD recvInfo_ );
	int		GetTurnCount(){return m_iPlayTurnCount;}
private:
	KD3DWnd*	m_pkBtnChallenge;
	
	KD3DStatic*	m_pkStaticChallengePoint;
	KD3DStatic*	m_pkStaticReChallengePoint;
	KD3DStatic*	m_pkStaticClearCnt;
	KD3DStatic*	m_pkStaticChargeTime;

	int			m_iTryPoint;
	int			m_iSyncPlayTime;
	int			m_iPlayTurnCount;
	int			m_iMaxTryPointTime;
	void		SetTime();
	boost::function0<void>					m_ActionClickFunc;

	int			m_iViewPlayTime; // 보여주기 위한 시간 카운트


};

DEFINE_WND_FACTORY( KGCGawibawiboGameInfo );
DECLARE_WND_FACTORY( KGCGawibawiboGameInfo );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGameInfo );
