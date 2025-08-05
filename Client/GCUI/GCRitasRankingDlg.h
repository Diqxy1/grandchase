#pragma once


class KGCRitasHelp;
class KGCRitasRankList;

class KGCRitasRankingDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
	enum{
		MAX_COIN_NUM = 5,
		MAX_NUM_RANK = 10
	};
    DECLARE_CLASSNAME( KGCRitasRankingDlg );
    /// Default constructor
    KGCRitasRankingDlg( void );
    /// Default destructor
    virtual ~KGCRitasRankingDlg( void );
    /// Copy constructor
    KGCRitasRankingDlg( const KGCRitasRankingDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasRankingDlg& operator=( const KGCRitasRankingDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState();


	void OnClose();
	void OnStart();
	void OnHelp();

	void OnMyRank();
	void OnAllRnak();
	void OnOpenBox();


	void SetStartCoin();

	void UpDatePrimiumInfo();
	void UpDateRankInfo();

	KGCRitasHelp* GetRitasHelp()		{ return m_pkHelpPlanCard; }

private:

	KD3DWnd*	m_pkBtnStart;
	KD3DWnd*	m_pkBtnClose;
	KD3DWnd*	m_pkBtnHelp;

	KD3DWnd*	m_pkBtnMyRank;
	KD3DWnd*	m_pkMyRankOver;
	KD3DWnd*	m_pkBtnAllRank;
	KD3DWnd*	m_pkAllRankOver;

	KD3DWnd*	m_pkBtnOpenBox;

	KGCRitasHelp* m_pkHelpPlanCard;
	KGCRitasRankList* m_pkRankList;

	KD3DStatic*	m_pkStaticTodayScore;
	KD3DStatic*	m_pkStaticTodayScoreValue;

	KD3DStatic*	m_pkStaticOutOfRank;

	KD3DStatic*	m_pkStaticPrimium;
	KD3DStatic*	m_pkStaticSpecialKey;
	KD3DStatic*	m_pkStaticPrimiumValue;
	KD3DStatic*	m_pkStaticSpecialKeyValue;

	KD3DStatic*	m_pkStaticLeftCoin;
	KD3DStatic*	m_pkStaticLeftCoinValue;

	KD3DStatic*	m_pkStaticRankGuide;

	KGCUINumber*	m_pkMyRank;
	KD3DWnd*		m_pkLevelImg;
};

DEFINE_WND_FACTORY( KGCRitasRankingDlg );
DECLARE_WND_FACTORY( KGCRitasRankingDlg );
DECLARE_WND_FACTORY_NAME( KGCRitasRankingDlg );
