#ifndef _GCINFOPANEL_H_
#define _GCINFOPANEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../GCItem.h"
//#include <sstream>

class KGCUserItemBox;
class KD3DStatic;
class KGCQuestRankScore;
class KGCBadUserInfo;
class KD3DImageIcon;
class KGCCharWnd;

class KGCMatchInfoPanelS6 : public KD3DWnd,        // extends
	public KActionListener // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchInfoPanelS6 );
	/// Default constructor
	KGCMatchInfoPanelS6( void );
	/// Default destructor
	virtual ~KGCMatchInfoPanelS6( void );
	/// Copy constructor
	KGCMatchInfoPanelS6( const KGCMatchInfoPanelS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchInfoPanelS6& operator=( const KGCMatchInfoPanelS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void UpdateInfo( void );
	void UpdateMark( void );
    void UpdateRank( void );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
	void SetGradeTex( int index, KD3DWnd *pWnd );
    void OnRankBtn();


private:
	KD3DStatic* m_pkstAutoInfo;
	KD3DStatic* m_pkstAutoRankInfo;
	KD3DStatic* m_pkstAutoValInfo;

	KD3DStatic* m_pkstAutoWin;
	KD3DStatic* m_pkstAutoLose;
	KD3DStatic* m_pkstAutoRait;

	KD3DStatic* m_pkstFreeWin;
	KD3DStatic* m_pkstFreeLose;
	KD3DStatic* m_pkstFreeRait;
	KD3DStatic* m_pkstRank;

	KD3DWnd* m_pkRank;
	KD3DWnd* m_pkRankBack;
    KD3DWnd* m_pkRankBtn;

    KD3DSizingBox* m_pkAutotitleBack;
    KD3DSizingBox* m_pkAutoInfoBack;
};

DEFINE_WND_FACTORY( KGCMatchInfoPanelS6 );
DECLARE_WND_FACTORY( KGCMatchInfoPanelS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchInfoPanelS6 );

class KGCInfoPanel : public KD3DWnd,        // extends
                     public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCInfoPanel );
    /// Default constructor
    KGCInfoPanel( void );
    /// Default destructor
    virtual ~KGCInfoPanel( void );
    /// Copy constructor
    KGCInfoPanel( const KGCInfoPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInfoPanel& operator=( const KGCInfoPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
	
	void OnPostChildInitialize();
	void SetUserInfo();
	void PostChildDraw( void );

    bool IsSelectVirtualCash(void);
    void HideAllCashGashInfo(void);

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
	
	void UpdateCharMark();
	void UpdateGrade();

protected:
    KD3DStatic*			m_pkTeamID;
	KD3DStatic*			m_pkLevel;

    KD3DStatic*			m_pkWin;
	KD3DStatic*			m_pkLose;
	KD3DStatic*			m_pkWinPercent;
	KD3DStatic*			m_pkGuildName;
	
    KD3DStatic*         m_pkGP;
	KD3DStatic*			m_pkGem;
	KD3DStatic*			m_pkCrystal;
	KD3DStatic*			m_pkChange;
    KD3DStatic*			m_pkPoint;
    KD3DStatic*			m_pkHeroCoin;

	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkTopChar;
	KSafeArray<KD3DWnd*,GC_GRADE_NUM> m_pkGrade;

	KD3DWnd*			m_pkGCCoinBackground;
	KD3DWnd*			m_pkCash_img;
    KD3DWnd*            m_pkChargeCashBtn;
    KD3DStatic*         m_pkStaticCash;
    KD3DWnd*            m_pkChinaWebCashImg;
    KD3DStatic*         m_pkStaticChinaWebCash;

	KD3DWnd*			m_pkGashBackground;
	KD3DWnd*			m_pkGGCoin_img;
	KD3DStatic*         m_pkStaticGCCoin;

    KD3DWnd*            m_pkPointMark;
    KD3DWnd*            m_pkBonusMark;
    KGCQuestRankScore*  m_pkBonus;

    KGCBadUserInfo*         m_pkBadUserInfo;
    KD3DWnd*                m_pkYellowCard;
    KD3DWnd*                m_pkRedCard;

    KD3DWnd*                m_pkShowVirtualCashBtn;

	KD3DWnd*            m_pkBtnGacha;
	KD3DWnd*            m_pkWndGachaCoin;
	KD3DStatic*         m_pkStaticGachaCoin;
	KD3DWnd*            m_pkWndGachaVip;
	KD3DStatic*         m_pkStaticGachaVip;

	GCImageIconTexture*			m_pkTexGuildMark;

	int 	m_iLevel;

	int		m_iWin;
	int		m_iLose;

	int		m_iBlessing;
	int	    m_iGP;
	int		m_iGem;
	int		m_iCrystal;
    int     m_iHeroCoin;
	int*	m_iChange;
    int		m_iSelectCharIndex;
    DWORD   m_dwCashPoint;
    int     m_iGCPoint;
    DWORD   m_dwVirtualCash;

	KD3DWnd* m_pkExpGaugeRect;
	KD3DWnd* m_pkExpGauge;
	KD3DStatic* m_pkStaticExpValue;

    KD3DWnd* m_pkGuildIconBack;
	KD3DWnd* m_pkGuildIconDrawRect;
	KGCMatchInfoPanelS6* m_pkMatchInfo;
};

DEFINE_WND_FACTORY( KGCInfoPanel );
DECLARE_WND_FACTORY( KGCInfoPanel );
DECLARE_WND_FACTORY_NAME( KGCInfoPanel );

#endif // _GCINFOPANEL_H_
