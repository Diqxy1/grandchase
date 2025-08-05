// DlgTGWeather.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGSubject.h"
#include "ObjectDataManager.h"

// CDlgTGWeather 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGSubject, CDlgTGBase)
CDlgTGSubject::CDlgTGSubject(CWnd* pParent /*=NULL*/)
{
}

CDlgTGSubject::~CDlgTGSubject()
{
}

void CDlgTGSubject::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_SUBJECT_TYPE, m_ebSubjectType);
    DDX_Control(pDX, IDC_EDIT_SUBJECT_RADIO, m_ebSubjectRadio);
    DDX_Control(pDX, IDC_EDIT_SUBJECT_CHARTYPE, m_ebCharType);
    DDX_Control(pDX, IDC_EDIT_SUBJECT_LEVEL, m_ebLevel);
    DDX_Control(pDX, IDC_EDIT_SUBJECT_JOBLEVEL, m_ebJobLevel);

    DDX_Control(pDX, IDC_BUTTON_SUBJECT, m_bbAddSubject);
    DDX_Control(pDX, IDC_BUTTON_SUBJECT_CHARTYPE, m_bbAddCharType);
    DDX_Control(pDX, IDC_BUTTON_SUBJECT_LEVEL, m_bbAddLevel);
    DDX_Control(pDX, IDC_BUTTON_SUBJECT_JOBLEVEL, m_bbAddJobLevel);

    DDX_Control(pDX, IDC_LIST_SUBJECT, m_lcSubject);
    DDX_Control(pDX, IDC_LIST_SUBJECT_CHARTYPE, m_lbCharType);
    DDX_Control(pDX, IDC_LIST_SUBJECT_LEVEL, m_lbLevel);
    DDX_Control(pDX, IDC_LIST_SUBJECT_JOBLEVEL, m_lbJobLevel);

    CString strTemp;
    strTemp.Format(L"%d", 0);
    m_ebSubjectType.SetWindowText( strTemp );
    m_ebSubjectRadio.SetWindowText( strTemp );
    m_ebCharType.SetWindowText( strTemp );
    m_ebLevel.SetWindowText( strTemp );
    m_ebJobLevel.SetWindowText( strTemp );

    {
        m_lcSubject.DeleteAllItems();
        m_lcSubject.DeleteColumn( 0 );
        m_lcSubject.DeleteColumn( 1 );
        m_lcSubject.InsertColumn(0, L"과제ID", LVCFMT_CENTER, 60, 0);
        m_lcSubject.InsertColumn(1, L"확  률", LVCFMT_CENTER, 60, 0);
        std::vector<std::pair<int, int>>::iterator vecIter = m_TGSubject.m_vecSubject.begin();
        for(int iLoop=0; vecIter != m_TGSubject.m_vecSubject.end(); ++iLoop, ++vecIter)
        {
            strTemp.Format(L"%d", vecIter->first );
            m_lcSubject.InsertItem(iLoop, strTemp);
            strTemp.Format(L"%d", vecIter->second );
            m_lcSubject.SetItemText(iLoop, 1, strTemp);
        }
    }

    {
        m_lbCharType.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConCharType.begin();
        for(int iLoop=0; vecIter != m_TGSubject.m_vecConCharType.end(); ++iLoop, ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbCharType.InsertString(iLoop, strTemp);
        }
    }
    
    {
        m_lbLevel.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConLevel.begin();
        for(int iLoop=0; vecIter != m_TGSubject.m_vecConLevel.end(); ++iLoop, ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbLevel.InsertString(iLoop, strTemp);
        }
    }

    {
        m_lbJobLevel.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConJobLevel.begin();
        for(int iLoop=0; vecIter != m_TGSubject.m_vecConJobLevel.end(); ++iLoop, ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbJobLevel.InsertString(iLoop, strTemp);
        }
    }
}

BEGIN_MESSAGE_MAP(CDlgTGSubject, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_SUBJECT, OnBnClickedButtonAddSubject)
    ON_BN_CLICKED(IDC_BUTTON_SUBJECT_CHARTYPE, OnBnClickedButtonAddCharType)
    ON_BN_CLICKED(IDC_BUTTON_SUBJECT_LEVEL, OnBnClickedButtonAddLevel)
    ON_BN_CLICKED(IDC_BUTTON_SUBJECT_JOBLEVEL, OnBnClickedButtonAddJobLevel)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUBJECT, OnNmItemdblclickListSubject)
    ON_LBN_DBLCLK(IDC_LIST_SUBJECT_CHARTYPE, OnLbDBClickedCharType)
    ON_LBN_DBLCLK(IDC_LIST_SUBJECT_LEVEL, OnLbDBClickedLevel)
    ON_LBN_DBLCLK(IDC_LIST_SUBJECT_JOBLEVEL, OnLbDBClickedJobLevel)
    
END_MESSAGE_MAP()

// CDlgTGSubject 메시지 처리기
void CDlgTGSubject::Refresh(void)
{
    CString strTemp;
    strTemp.Format(L"%d", 0);
    m_ebSubjectType.SetWindowText( strTemp );
    m_ebSubjectRadio.SetWindowText( strTemp );
    m_ebCharType.SetWindowText( strTemp );
    m_ebLevel.SetWindowText( strTemp );
    m_ebJobLevel.SetWindowText( strTemp );

    {
        m_lcSubject.DeleteAllItems();
        std::vector<std::pair<int, int>>::iterator vecIter = m_TGSubject.m_vecSubject.begin();
        for(int iLoop=0; vecIter != m_TGSubject.m_vecSubject.end(); ++iLoop, ++vecIter)
        {
            strTemp.Format(L"%d", vecIter->first );
            m_lcSubject.InsertItem(iLoop, strTemp);
            strTemp.Format(L"%d", vecIter->second );
            m_lcSubject.SetItemText(iLoop, 1, strTemp);
        }
    }

    {
        m_lbCharType.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConCharType.begin();
        for(;vecIter != m_TGSubject.m_vecConCharType.end(); ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbCharType.AddString(strTemp);
        }
    }

    {
        m_lbLevel.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConLevel.begin();
        for(; vecIter != m_TGSubject.m_vecConLevel.end(); ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbLevel.AddString(strTemp);
        }
    }

    {
        m_lbJobLevel.ResetContent();
        std::vector<int>::iterator vecIter = m_TGSubject.m_vecConJobLevel.begin();
        for(; vecIter != m_TGSubject.m_vecConJobLevel.end(); ++vecIter)
        {
            strTemp.Format(L"%d", (*vecIter) );
            m_lbJobLevel.AddString(strTemp);
        }
    }
}

// 값 저장
void CDlgTGSubject::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        m_strDesc =  L"달성과제를 생성합니다.";
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGSubject.m_vecSubject.clear();
        for(int iLoop=0; iLoop<m_lcSubject.GetItemCount(); ++iLoop)
        {
            CString strSubjectType = m_lcSubject.GetItemText(iLoop, 0);
            int iSubjectType = _wtoi(strSubjectType);
            CString strSubjectRadio = m_lcSubject.GetItemText(iLoop, 1);
            int iSubjectRadio = _wtoi(strSubjectRadio);
            m_TGSubject.m_vecSubject.push_back( std::make_pair(iSubjectType, iSubjectRadio) );
        }

        m_TGSubject.m_vecConCharType.clear();
        for(int iLoop=0; iLoop<m_lbCharType.GetCount(); ++iLoop)
        {
            CString strCharType;
            m_lbCharType.GetText(iLoop, strCharType);
            int iCharType = _wtoi(strCharType);
            m_TGSubject.m_vecConCharType.push_back( iCharType );
        }

        m_TGSubject.m_vecConLevel.clear();
        for(int iLoop=0; iLoop<m_lbLevel.GetCount(); ++iLoop)
        {
            CString strLevel;
            m_lbLevel.GetText(iLoop, strLevel);
            int iLevel = _wtoi(strLevel);
            m_TGSubject.m_vecConLevel.push_back( iLevel );
        }

        m_TGSubject.m_vecConJobLevel.clear();
        for(int iLoop=0; iLoop<m_lbJobLevel.GetCount(); ++iLoop)
        {
            CString strJobLevel;
            m_lbJobLevel.GetText(iLoop, strJobLevel);
            int iJobLevel = _wtoi(strJobLevel);
            m_TGSubject.m_vecConJobLevel.push_back( iJobLevel );
        }

        m_TGSubject.m_strDesc = GetDesc();
    }        
}

void CDlgTGSubject::OnBnClickedButtonAddSubject()
{
    CString strSubjectType;
    m_ebSubjectType.GetWindowText( strSubjectType );
    int iSubjectType = _wtoi(strSubjectType);
    int iItemCnt = m_lcSubject.GetItemCount();

    if( 0 == iSubjectType )
    {
        ::MessageBoxW( NULL, L"과제ID는 0이상입니다.\nEnum.stg를 참조하세요.", L"SubjectInsertError", MB_OK);
        return;
    }

    for(int iLoop=0; iLoop<m_lcSubject.GetItemCount(); ++iLoop)
    {
        CString strSubjectType = m_lcSubject.GetItemText(iLoop, 0);
        int iType = _wtoi(strSubjectType);
        if( iSubjectType == iType )
        {
            iItemCnt = iLoop;
            break;
        }
    }

    CString strSubjectRadio;
    m_ebSubjectRadio.GetWindowText( strSubjectRadio );
    int iSubjectRadio = _wtoi(strSubjectRadio);
    if( 0 == iSubjectRadio )
    {
        ::MessageBoxW( NULL, L"확률이 0%면 없는거나 마찬가지 잖아요!!", L"SubjectInsertError", MB_OK);
        return;
    }

    CString strTemp; 
    strTemp.Format(L"%d", iSubjectType );
    if( iItemCnt == m_lcSubject.GetItemCount() )
        m_lcSubject.InsertItem(iItemCnt, strTemp);
    else
        m_lcSubject.SetItemText(iItemCnt, 0, strTemp);

    strTemp.Format(L"%d", iSubjectRadio );
    m_lcSubject.SetItemText(iItemCnt, 1, strTemp);
}

void CDlgTGSubject::OnBnClickedButtonAddCharType()
{
    CString strTemp;
    CString strCharType;
    m_ebCharType.GetWindowText( strCharType );
    for(int iLoop=0; iLoop<m_lbCharType.GetCount(); ++iLoop)
    {
        m_lbCharType.GetText(iLoop, strTemp);
        if( strCharType == strTemp )
        {
            return;
        }
    }
    m_lbCharType.AddString( strCharType );
}

void CDlgTGSubject::OnBnClickedButtonAddLevel()
{
    CString strTemp;
    CString strLevel;
    m_ebLevel.GetWindowText( strLevel );
    for(int iLoop=0; iLoop<m_lbLevel.GetCount(); ++iLoop)
    {
        m_lbLevel.GetText(iLoop, strTemp);
        if( strLevel == strTemp )
        {
            return;
        }
    }
    m_lbLevel.AddString( strLevel );
}

void CDlgTGSubject::OnBnClickedButtonAddJobLevel()
{
    CString strTemp;
    CString strJobLevel;
    m_ebJobLevel.GetWindowText( strJobLevel );
    for(int iLoop=0; iLoop<m_lbJobLevel.GetCount(); ++iLoop)
    {
        m_lbJobLevel.GetText(iLoop, strTemp);
        if( strJobLevel == strTemp )
        {
            return;
        }
    }
    m_lbJobLevel.AddString( strJobLevel );
}

void CDlgTGSubject::OnNmItemdblclickListSubject(NMHDR *pNMHDR, LRESULT *pResult)
{
    int iItem = -1;
    POSITION pSelPos = m_lcSubject.GetFirstSelectedItemPosition();
    if( pSelPos )
        iItem = m_lcSubject.GetNextSelectedItem( pSelPos );

    if( -1 >= iItem || iItem >= m_lcSubject.GetItemCount() )
        ::MessageBox(NULL, L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lcSubject.DeleteItem( iItem );
}

void CDlgTGSubject::OnLbDBClickedCharType()
{
    int iItem = m_lbCharType.GetCurSel();

    if( -1 >= iItem || iItem > m_lbCharType.GetCount() )
        ::MessageBox(NULL, L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lbCharType.DeleteString( iItem );
}

void CDlgTGSubject::OnLbDBClickedLevel()
{
    int iItem = m_lbLevel.GetCurSel();

    if( -1 >= iItem || iItem > m_lbLevel.GetCount() )
        ::MessageBox(NULL, L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lbLevel.DeleteString( iItem );
}

void CDlgTGSubject::OnLbDBClickedJobLevel()
{
    int iItem = m_lbJobLevel.GetCurSel();

    if( -1 >= iItem || iItem > m_lbJobLevel.GetCount() )
        ::MessageBox(NULL, L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lbJobLevel.DeleteString( iItem );
}