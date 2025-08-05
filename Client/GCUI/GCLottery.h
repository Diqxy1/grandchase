#pragma once


class KGCLotteryRewardBox;
class KGCLottery : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCLottery );
    /// Default constructor
    KGCLottery( void );
    /// Default destructor
    virtual ~KGCLottery( void );
    /// Copy constructor
    KGCLottery( const KGCLottery& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLottery& operator=( const KGCLottery& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

#define APHADOT	D3DCOLOR_ARGB(0,0,0,0)

	enum {
		RAN_RANGE1 = 10,
		RAN_RANGE2 = 15,
		RAN_RANGE3 = 20,
	};

	enum eSTATE{
		STATE_INIT = 0,
		STATE_MANUAL = 1,
		STATE_AUTO = 2,
		STATE_SHOW_EFFECT = 3,
		STATE_END = 4,
	};

	enum {
		NUM_SHOW_ITEM_IMG = 3,
		NUM_REWARD_DISPLAY_BOX = 5
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void PostChildDraw();
	virtual void FrameMoveInEnabledState();

	void ScratchOnBoard(POINT ptPoint_ );
	
	void SetShowImgAtSuccess(GCITEMID gcItemID_ );
	void SetShowImgAtFail();
	void SetLotteryDuration(int iDurantion_);
	void SetSuccessVar(bool bVar_ )				{ m_bSuccescc = bVar_; }

	void SetTempRewardItem(std::vector<KItem>* vecRewardItem_ );

private:

	void SetRewardDisplay_();
	void OnExit();
	void OnAuto();
	void OnYes();
	void OnLeft();
	void OnRight();

	void SetInitVailue();

	void CreateRewardDisplayBox_();
	void SetCharSelectBox_();
	void FrameMoveAutoScratch_();
	void MakeAutoScratchPos_();
	void FrameMoveShowEffect_();

	void OnClickAuto();
	void OnStopAuto();

	KD3DWnd*	m_pkImgRect[NUM_SHOW_ITEM_IMG];
	GCDeviceTexture*	m_pTexShowImg[NUM_SHOW_ITEM_IMG];

	KD3DWnd*	m_pkOverBlindRect;
	KD3DWnd*	m_pkCheckRect;
	GCDeviceTexture*	m_pTexOverBlind;

	KD3DWnd*	m_btnExit;
	KD3DWnd*	m_btnAuto;
	KD3DWnd*	m_btnYes;
	KD3DWnd*	m_btnLeft;
	KD3DWnd*	m_btnRight;

	KD3DWnd*	m_pkTitleQuestion;
	KD3DWnd*	m_pkTitleRejection;
	KD3DWnd*	m_pkTitleGuide;

	int			m_iCharType;

	eSTATE		m_eLotteryState;

	DWORD		m_dwFrameCnt;
	float		m_fAutoScratchCnt;
	int			m_iScratchType;

	bool		m_bHaveLottery;
	bool		m_bNoTouch;

	KD3DComboBox*			m_pkCharSelectBox;
	std::map<int,char>		m_mapCharType;

	KD3DWnd*				m_pkRewardDisplayRect;
	KGCLotteryRewardBox*	m_pkRewardDisplayBox[NUM_REWARD_DISPLAY_BOX];
	KD3DStatic*				m_pkStaticGuide;
	KD3DStatic*				m_pkStaticExplain0;
	KD3DStatic*				m_pkStaticExplain1;
	KD3DStatic*				m_pkStaticExplain2;
	KD3DStatic*				m_pkStaticNumLottery;
	KD3DCheckBox*			m_pkCheckBoxAutoClear;

	int		m_iFirstIndex;
	bool	m_bSuccescc;
	bool    m_bAutoMode;

	DWORD	m_dwFrammCntAtShowEffect;
	std::vector<KItem>	m_vecTempRewardItem;
	std::map<int,std::vector<D3DXVECTOR2>>	m_mapAutoScratchPos;
};

DEFINE_WND_FACTORY( KGCLottery );
DECLARE_WND_FACTORY( KGCLottery );
DECLARE_WND_FACTORY_NAME( KGCLottery );
