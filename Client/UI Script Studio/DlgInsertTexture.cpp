// DlgInsertTexture.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "DlgInsertTexture.h"
#include "KWndManager.h"
#include ".\dlginserttexture.h"

extern KWndManager				*g_WndMgr;

// DlgInsertTexture 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgInsertTexture, CDialog)
DlgInsertTexture::DlgInsertTexture(CWnd* pParent /*=NULL*/)
	: CDialog(DlgInsertTexture::IDD, pParent)
{
}

DlgInsertTexture::~DlgInsertTexture()
{
}

void DlgInsertTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgInsertTexture, CDialog)
//	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// DlgInsertTexture 메시지 처리기입니다.

BOOL DlgInsertTexture::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pKomFileList = (CComboBox*)GetDlgItem(IDC_FILELISTINKOM);

	if( !g_WndMgr->m_KOMFileName.compare("") )
	{
		::MessageBox( this->GetSafeHwnd(), _T("KOM File이  로드되지 않았습니다."), NULL, MB_OK );
		OnCancel();
	}
	else
	{
		m_MassFile.LoadMassFile( g_WndMgr->m_KOMFileName.c_str() );
		ShowWindow(SW_SHOW);

		GCMassFile::MASSFILE_MEMBERFILEINFO Info;
		for( int i = 0; i < m_MassFile.GetFileCount(); i++ )
		{
			m_MassFile.GetMemberFile(i, &Info);

			m_pKomFileList->InsertString(i, Info.strFileName );
			m_pKomFileList->SetItemData(i, (DWORD)new CString(Info.strFileName));
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DlgInsertTexture::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	

	// KOM안에 있는 파일을 UI Script에 단지 추가하기 위함이므로 m_vecStrTexFileName에 추가만 해주면 됨.
	if( m_pKomFileList->GetItemData(m_pKomFileList->GetCurSel()) != LB_ERR )
	{
		CString FileName = *(CString*)(m_pKomFileList->GetItemData(m_pKomFileList->GetCurSel()));
		std::string strFileName = ToCharStr( FileName.GetBuffer() );

		for( int i = 0; i < (int)g_WndMgr->m_vecStrTexFileName.size(); )
		{
			if( !g_WndMgr->m_vecStrTexFileName[i].compare( strFileName.c_str() ) )
			{
				::MessageBox( this->GetSafeHwnd(), _T("파일이 이미 있습니다."), NULL, MB_OK | MB_ICONERROR );
				break;
			}

			if( ++i == (int)g_WndMgr->m_vecStrTexFileName.size() )
			{
				::MessageBox( this->GetSafeHwnd(), _T("파일 추가 성공"), NULL, MB_OK | MB_ICONINFORMATION );
				g_WndMgr->m_vecStrTexFileName.push_back( strFileName );				
				break;
			}
		}
	}

	OnOK();
}
