#ifndef _GCPLAYERSTATUSBAR_H_
#define _GCPLAYERSTATUSBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "GCSingleMenu.h"

class KD3DStatic;
class KGCSingleMenu;
class KGCPingAntenna;

class KGCPlayerStatusBar : public KD3DWnd // extends
	, public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCPlayerStatusBar);
	/// Default constructor
	KGCPlayerStatusBar(void);
	/// Default destructor
	virtual ~KGCPlayerStatusBar(void);
	/// Copy constructor
	KGCPlayerStatusBar(const KGCPlayerStatusBar&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCPlayerStatusBar& operator=(const KGCPlayerStatusBar&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	void SetPlayerIndex(int iIndex);
	int  GetPlayerIndex() { return m_iPlayerIdx; }

protected:
	enum
	{
		MAX_LEN_HP_GAUGE = 121,
	};

protected:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);
	virtual void ActionPerformed(const KActionEvent& event);

	void SetHP(float fHP);
	void SetChar(int iChar);
	void SetTagChar(int iChar);
	void SetName(const WCHAR* szName, bool bPosChange);
	void SetPing(int iPing);

protected:
	int         m_iPlayerIdx;
	D3DXVECTOR2 m_vBarPos;
	KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkChar;
	KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_pkTagChar;

	// HP
	D3DXVECTOR4 m_vHPGaugeTexCoords;
	KD3DWnd*    m_pkHPGauge;

	// Name
	KD3DStatic* m_pkName;

	//Ping
	KD3DWnd*	m_pkPingBg;
	KD3DWnd*	m_pkRedPing;
	KD3DWnd*	m_pkYellowPing;
	KD3DWnd*	m_pkGreenPing;

	//Rank
	KD3DWnd* m_pkRank;

	//Report
	KGCSingleMenu* m_pkFairPlayReport;

	UCHAR		m_ucFrmCount;

	KGCPingAntenna* m_pkPingAntenna;
	D3DXVECTOR2 m_vNickNameBasePos;

	KD3DWnd*        m_pkDeathKill;
	KGCUINumber*    m_pkDeathKillNumber;

public:
	void SetRoomMemberPingInfo(std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_);
	void UpdateMark(void);
	void SetGradeTex(int index, KD3DWnd *pWnd);
	void ChangeUVtoTexel(D3DXVECTOR4& vUV);
};

DEFINE_WND_FACTORY(KGCPlayerStatusBar);
DECLARE_WND_FACTORY(KGCPlayerStatusBar);
DECLARE_WND_FACTORY_NAME(KGCPlayerStatusBar);

#endif // _GCPLAYERSTATUSBAR_H_
