#pragma once

#include "KTDGParticleSystem.h"
#include "./GridCtrl/GridCtrl.h"
//#include <vector>
//#include <map>

struct SKeyFrame
{
	CString m_strFrameData;
	bool	m_bFade;

	SKeyFrame()
	{
		m_bFade = true;
	}

	SKeyFrame( const SKeyFrame& src )
	{
		*this = src;
	}

	const SKeyFrame& operator = ( const SKeyFrame& src )
	{
		m_strFrameData  = src.m_strFrameData;
		m_bFade			= src.m_bFade;
		return *this;
	}
};

class CTimeLineEditor : public CGridCtrl
{
public:
	CTimeLineEditor();
	~CTimeLineEditor(void);
public:
	void InitTimeLine();
	void SetMaxTime(DWORD dwMaxTime = 10);
	bool AddKeyFrame(SKeyFrame& KeyFrame, DWORD dwTime, CKTDGParticleSystem::EventType eEventType);
	bool AddEvent(CKTDGParticleSystem::CParticleEvent* pEvent);
	void RemoveAllKeyFrame(int iLimitCol = 0);
	void Display();
	void KeyFrameToParticleEvent(CKTDGParticleSystem::CParticleEventSequence* pWorkParticle);
	CKTDGParticleSystem::CParticleEvent* ScriptToEvent(CString strScript);
protected:
	int  FindFadeFrame(CKTDGParticleSystem::EventType eEventType, int iStartCol);
	void OnEditCell(int nRow, int nCol, UINT nChar);
	bool EditCell(int nRow, int nCol);	
	void DeleteKeyFrame(int nRow, int nCol, bool bUpdateNDisplay = true);

	CCellID m_cellDrag;

	//============================Clip Board
	void CutSelectedText();
	COleDataSource* CopyTextFromGrid();
	BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);

	void OnEditCut();
	void OnEditCopy();
	bool EditPaste( int iRow, int iCol );
	void OnEditPaste();
	int	 m_iClipBoardRow;
	SKeyFrame m_keyClipboard;
	//============================
private:
	DWORD			m_dwMaxTime;		// 1초가 10으로 표시됩니다.
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
