
#ifndef DISABLE_GACHA_PON

#pragma once

class KGCGachaPongDlg : public KD3DDialog, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGachaPongDlg );
	/// Default constructor
	KGCGachaPongDlg( void );
	/// Default destructor
	virtual ~KGCGachaPongDlg( void );
	/// Copy constructor
	KGCGachaPongDlg( const KGCGachaPongDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGachaPongDlg& operator=( const KGCGachaPongDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
	virtual void OnCreate( void );
	virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState();

public:
	void UpdateMachine(DWORD dwMachineNumber);
	void UpdateCapsuleNumber();
	void ShowBallRollingEffect();
	void RestartMachine();
	void FrameMove_ItemImage();

private:
	void OnClose();
	void OnOK();
	void OnBuy();
	void OnMachineChange();
	void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID dwItemID_ );
	void OnClickAuto();
	void Init_ItemImage();

private:
	enum
	{
		MAX_MACHINE_COLORS = 3,
		MAX_BUTTON_OK_STATE = 3,
		RED_CAPSULE = 0,
		BLUE_CAPSULE = 1,
		YELLOW_CAPSULE = 2,
	};

	KSafeArray<KD3DWnd*, MAX_MACHINE_COLORS>	m_arrMachine;
	KSafeArray<KD3DWnd*, MAX_BUTTON_OK_STATE>	m_arrOKBtn;
	KD3DWnd*									m_btnOK;
	KD3DWnd*									m_btnClose;

	//Tooltip
	KD3DWnd*									m_pkTooltipImg;
	KD3DStatic*									m_pkTooltipText;

	//Machine Info
	KD3DStatic*									m_pkTotalCapsule;
	KD3DStatic*									m_pkRewardNumber;
	KGCItemImgWnd*								m_pkRewardImg;

	//Coin
	KGCItemImgWnd*								m_pkCoinImg;
	KD3DStatic*									m_pkCoinNumber;
	KD3DWnd*									m_btnBuy;

	KD3DComboBox*								m_cbRewardList;

	KD3DCheckBox*								m_pkCheckBoxAutoClear;

private:
	enum eSTATE
	{
		STATE_INIT = 0,
		STATE_TURN_LEVER = 1,
		STATE_SHOW_EFFECT = 2,
		STATE_END = 3,
	};

	eSTATE										m_eMachineState;
	std::map<int, DWORD>						m_mapMachineIds;
	DWORD										m_dwCurMachine;

	DWORD										m_dwParticleLife;
	DWORD										m_dwFramesAfterTurnLever;
	int											m_iLastMachineColor;
	int											m_iFrameCount_ItemImage;
	bool										m_bAutoMode;
	bool										m_bGachaProccessed;
	GCITEMID									m_iCapsuleControl;
	KGC2DObject*								m_pkGetItemImage;
};

DEFINE_WND_FACTORY( KGCGachaPongDlg );
DECLARE_WND_FACTORY( KGCGachaPongDlg );
DECLARE_WND_FACTORY_NAME( KGCGachaPongDlg );

#endif