#pragma once
#include "DlgTGBase.h"
#include "Weather.h"
#include "stdafx.h"
#include "afxwin.h"

// CDlgTGWeather 대화 상자입니다.

class CDlgTGWeather : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGWeather)

public:
	CDlgTGWeather(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGWeather();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_WEATHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CComboBox m_cbType;
    CEdit m_ebPosX;
    CEdit m_ebPosY;
    CEdit m_ebPosWidth;
    CEdit m_ebPosHeight;
    CEdit m_ebEffPosX;
    CEdit m_ebEffPosY;
    CEdit m_ebEffSequence;
    CListBox m_lbParticle;
    CEdit m_ebWindSpeedX;
    CEdit m_ebWindSpeedY;
    CEdit m_ebWindJumpRadio;
    CStatic m_stWindPropertyBox;
    CStatic m_stWindSpeedX;
    CStatic m_stWindSpeedY;
    CStatic m_stWindJumpRadio;

    CWeather m_TGWeather;
    void InitValue(void) { m_TGWeather.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CWeather*)pValue = m_TGWeather; }
    void SetValue(CBaseTGFunction *pValue) { m_TGWeather = *(CWeather*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedAddParticle();
    afx_msg void OnCbnSelchangeComboWeatherType();
    afx_msg void OnBnClickedButtonAllDel();

    void SetItemListBox();
    void CloseWeatherProperty();
    void ShowWeatherProperty( int iWeatherType );
    
};
