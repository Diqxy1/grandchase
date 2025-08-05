#pragma once
#include "StdAfx.h"
#include "QuestScripter.h"
#include "DlgTGBase.h"
#include "SoundOnOff.h"
#include "ACEdit.h"
#include "DlgDirectorySetting.h"
#include "DlgMapToObject.h"
#include "ReadOnlyEdit.h"

// CDlgTGSoundOnOff 대화 상자입니다.

class CDlgTGSoundOnOff : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGSoundOnOff)

public:
	CDlgTGSoundOnOff(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGSoundOnOff();

    CSoundOnOff m_SoundOnOff;
    void InitValue(void) { m_SoundOnOff.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CSoundOnOff*)pValue = m_SoundOnOff; }
    void SetValue(CBaseTGFunction *pValue) { m_SoundOnOff = *(CSoundOnOff*)pValue; }

    void Refresh(void);
    void CalcResult(void);
    void RefreshSoundDirectory(void);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_SOUNDONOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CComboBox m_cbOnOff;
    //CReadOnlyEdit m_ebSoundFileName;
    afx_msg void OnBnClickedButtonMusicFile();
    CACEdit m_cbFileName;
    CStatic m_stSoundOnOff1;
    CStatic m_stSoundOnOff2;
};
