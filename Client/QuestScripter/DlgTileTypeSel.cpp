// DlgTileTypeSel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTileTypeSel.h"
#include ".\dlgtiletypesel.h"
#include "DlgMapToObject.h"

// CDlgTileTypeSel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTileTypeSel, CDialog)
CDlgTileTypeSel::CDlgTileTypeSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTileTypeSel::IDD, pParent)
{
	m_iCurIndex = 0;

	CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
	m_mapBmpFilePath[0] = strPath + L"/Data/Texture/0.bmp";
	m_mapBmpFilePath[2] = strPath + L"/Data/Texture/2.bmp";
	m_mapBmpFilePath[4] = strPath + L"/Data/Texture/4.bmp";
	m_mapBmpFilePath[6] = strPath + L"/Data/Texture/6.bmp";
	m_mapBmpFilePath[8] = strPath + L"/Data/Texture/8.bmp";
	m_mapBmpFilePath[10] = strPath + L"/Data/Texture/10.bmp";
	m_mapBmpFilePath[12] = strPath + L"/Data/Texture/12.bmp";
	m_mapBmpFilePath[14] = strPath + L"/Data/Texture/14.bmp";
	m_mapBmpFilePath[16] = strPath + L"/Data/Texture/16.bmp";
	m_mapBmpFilePath[18] = strPath + L"/Data/Texture/18.bmp";
	m_mapBmpFilePath[20] = strPath + L"/Data/Texture/20.bmp";
	m_mapBmpFilePath[22] = strPath + L"/Data/Texture/22.bmp";
	m_mapBmpFilePath[24] = strPath + L"/Data/Texture/24.bmp";
	m_mapBmpFilePath[26] = strPath + L"/Data/Texture/26.bmp";
	m_mapBmpFilePath[28] = strPath + L"/Data/Texture/28.bmp";
	m_mapBmpFilePath[30] = strPath + L"/Data/Texture/30.bmp";

	m_vtSequence.push_back(0);
	m_vtSequence.push_back(2);
	m_vtSequence.push_back(4);
	m_vtSequence.push_back(6);
	m_vtSequence.push_back(8);
	m_vtSequence.push_back(10);
	m_vtSequence.push_back(12);
	m_vtSequence.push_back(14);
	m_vtSequence.push_back(16);
	m_vtSequence.push_back(18);
	m_vtSequence.push_back(20);
	m_vtSequence.push_back(22);
	m_vtSequence.push_back(24);
	m_vtSequence.push_back(26);
	m_vtSequence.push_back(28);
	m_vtSequence.push_back(30);
}

CDlgTileTypeSel::~CDlgTileTypeSel()
{
}

void CDlgTileTypeSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_spinType);
	DDX_Control(pDX, IDC_BUTTON_SEL, m_btSkinButton);
}

BEGIN_MESSAGE_MAP(CDlgTileTypeSel, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
END_MESSAGE_MAP()

// CDlgTileTypeSel 메시지 처리기입니다.

void CDlgTileTypeSel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CDlgTileTypeSel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CDlgTileTypeSel::OnBnClickedButtonSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NextIndex();
	SetButtonImage();
}

BOOL CDlgTileTypeSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetButtonImage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTileTypeSel::NextIndex(void)
{
	if ((m_iCurIndex + 1) >= (int)m_vtSequence.size())
		m_iCurIndex = 0;
	else
		m_iCurIndex++;
}

void CDlgTileTypeSel::PrevIndex(void)
{
	if (!m_vtSequence.size())
	{
		m_iCurIndex = 0;
		return;
	}

	if ((m_iCurIndex - 1) < 0)
		m_iCurIndex = (int)m_vtSequence.size() - 1;
	else
		m_iCurIndex--;
}

void CDlgTileTypeSel::SetButtonImage(void)
{
	int iRealIndex = m_vtSequence[m_iCurIndex];
	CString strFilePath = m_mapBmpFilePath[iRealIndex];
	m_btSkinButton.SetSkin(strFilePath, strFilePath, strFilePath, strFilePath, strFilePath, L"", 1, 1, 0);
	m_btSkinButton.Invalidate(1);
}

void CDlgTileTypeSel::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (-1 == pNMUpDown->iDelta)
		NextIndex();
	else
		PrevIndex();
	SetButtonImage();
	*pResult = 0;
}

BOOL CDlgTileTypeSel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_PRIOR:
			PrevIndex();
			SetButtonImage();
			break;
		case VK_NEXT:
			NextIndex();
			SetButtonImage();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}