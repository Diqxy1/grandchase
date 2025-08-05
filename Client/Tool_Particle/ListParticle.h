#pragma once

#include "KTDGParticleSystem.h"
// CListParticle

class CListParticle : public CListCtrl
{
	DECLARE_DYNAMIC(CListParticle)

public:
	CListParticle();
	virtual ~CListParticle();

	void InitList();
	void OnDelete();
	void OnCopy();
	void OnPaste();

	void CreateSampleParticle(std::vector<CString>& vecList, CKTDGParticleSystem::CParticleEventSequence* pWorkParticle);
	void SetSelectItemText(CString strName);
	void SetSelectItemData(DWORD dwValue);
private:
	CString m_strClipboard;
	CString m_strClipboardParticleName;
public:
	bool	m_bLock;	//Update해도 적용안되게...
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


