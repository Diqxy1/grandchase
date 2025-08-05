#pragma once

class KGCGawibawiboGamePlay : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGawibawiboGamePlay );
    /// Default constructor
    KGCGawibawiboGamePlay( void );
    /// Default destructor
    virtual ~KGCGawibawiboGamePlay( void );
    /// Copy constructor
    KGCGawibawiboGamePlay( const KGCGawibawiboGamePlay& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGawibawiboGamePlay& operator=( const KGCGawibawiboGamePlay& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void FrameMoveInEnabledState( void );
	enum	GAWIBAWIBO_STATE{
		GAWIBAWIBO_STATE_DEFAULT	=0, //아무것도 선택 하지 않았을때.
		GAWIBAWIBO_STATE_GAWI		=1, //가위
		GAWIBAWIBO_STATE_BAWI		=2, //바위
		GAWIBAWIBO_STATE_BO			=3, //보
		GAWIBAWIBO_STATE_RANDOM		=4 //랜덤.
	};
	void ResultGawiBawiBoGame(const KEVENT_GAWIBAWIBO_ACK &recvInfo_);
	void SetActionFunc(boost::function0<void> a_Func){m_ActionClickFunc = a_Func;}
	void Destroy();
	void InitGamePlay();
	void TogglRenderBtn(bool arg_);
	void PostChildDraw();
	void SetClearPaticleEff(bool arg_){m_bClearPaticleEff = arg_;}

    void SetRollTimeDeg( bool bRoll_ )  { m_bRollTimeDeg = bRoll_; }

private:
	std::map<int, KD3DWnd*>		m_mapLeftBalloonGBB;
	std::map<int, KD3DWnd*>		m_mapRightBalloonGBB;

	KD3DWnd						*m_pkBtnGawi;
	KD3DWnd						*m_pkBtnBawi;
	KD3DWnd						*m_pkBtnBo;
	KD3DWnd						*m_pkBtnRandom;

	int							m_iSelectBtn;
	bool						m_bStop;
	int							m_iRightGawibawibo;
	int							m_iResult;
	int							m_iPreResult;
	int							m_iDegTime;
	bool						m_bEndPlay;
	bool						m_bClearPaticleEff;

	int							m_iRollFrame;
	void						RollGawiBawiBo();
	void						ClickGawiBawiBoBtn();
	bool						m_bRollTimeDeg;							
	boost::function0<void>		m_ActionClickFunc;
	void ViewResultGawiBawiBoGame(int gamestate_);
	void ClearEffect();
	void RenderPaticle();
	std::vector<CParticleEventSeqPTR> m_vecPaticle;
};

DEFINE_WND_FACTORY( KGCGawibawiboGamePlay );
DECLARE_WND_FACTORY( KGCGawibawiboGamePlay );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGamePlay );
