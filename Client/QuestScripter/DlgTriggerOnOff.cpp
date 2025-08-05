// DlgTriggerOnOff.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTriggerOnOff.h"

// CDlgTriggerOnOff 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTriggerOnOff, CDlgTGBase)
CDlgTriggerOnOff::CDlgTriggerOnOff(CWnd* pParent /*=NULL*/)
{
}

CDlgTriggerOnOff::~CDlgTriggerOnOff()
{
}

void CDlgTriggerOnOff::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_TGONOFF_NAME, m_cbName);
    DDX_Control(pDX, IDC_COMBO_TGONOFF_ONOFF, m_cbOnOff);

    DDX_Control(pDX, IDC_EDIT_TG_TG_ADDCHARTYPE, m_ebCharType);
    DDX_Control(pDX, IDC_BUTTON_TG_TG_ADDCHAR, m_bbAddCharType);
    DDX_Control(pDX, IDC_LIST_TG_TG_CHARTYPE, m_lbCharType);
    DDX_Control(pDX, IDC_COMBO_TGONOFF_CHAR, m_cbHasChar);    

    m_cbName.Init();

    m_cbOnOff.InsertString(0, L"끕니다");
    m_cbOnOff.InsertString(1, L"켭니다");
    if( m_cbOnOff.GetCount() ) m_cbOnOff.SetCurSel(0);

    m_cbHasChar.InsertString(0, L"모든 파티원들이 포함되지 않아야 동작한다!!");
    m_cbHasChar.InsertString(1, L"모든 파티원들이 포함되어야 동작한다!!");
    if( m_TGOnOff.m_bHasChar ) m_cbHasChar.SetCurSel(0);

    CString strTemp;
    strTemp.Format(L"%d", 0);
    m_ebCharType.SetWindowText( L"" );
    m_lbCharType.ResetContent();
    std::vector<int>::iterator vecIter = m_TGOnOff.m_vecConCharType.begin();
    for(int iLoop=0; vecIter != m_TGOnOff.m_vecConCharType.end(); ++iLoop, ++vecIter)
    {
        strTemp.Format(L"%d", (*vecIter) );
        m_lbCharType.InsertString(iLoop, strTemp);
    }

    InitTGName();

    Refresh();    
    DDX_Control(pDX, IDC_STATIC_TRIGGER_ONOFF_1, m_stTGOnOff1);
}

BEGIN_MESSAGE_MAP(CDlgTriggerOnOff, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_TG_TG_ADDCHAR, OnBnClickedAddCharType)
    ON_LBN_DBLCLK(IDC_LIST_TG_TG_CHARTYPE, OnLbDBClickedCharType)
    ON_STN_CLICKED(IDC_COMBO_TGONOFF_CHAR, &CDlgTriggerOnOff::OnStnClickedStaticTriggerOnoff3)
END_MESSAGE_MAP()


// CDlgTriggerOnOff 메시지 처리기입니다.

void CDlgTriggerOnOff::InitTGName(void)
{
    CQuestScripterDlg *pDlgScript = (CQuestScripterDlg*)AfxGetMainWnd();

    m_cbName.ResetContent();
    if( pDlgScript )
    {
        CDlgTrigger *pDlgTrigger = &pDlgScript->m_dlgTrigger;
        for(int i=0;i<(int)pDlgScript->m_Quest.vtStage.size();++i)
        {
            for(int j=0;j<(int)pDlgScript->m_Quest.vtStage[i]->vtCategory.size();++j)
            {
                CCategory *pCategory = pDlgScript->m_Quest.vtStage[i]->vtCategory[j];                
                for(int k=0;k<(int)pCategory->m_vtBaseTrigger.size();++k)
                {                
                    m_cbName.InsertString(m_cbName.GetCount(), pCategory->m_vtBaseTrigger[k]->m_strTGName.c_str());
                    m_cbName.AddSearchString(pCategory->m_vtBaseTrigger[k]->m_strTGName.c_str());
                }
            }
        }
    }
    if( m_cbName.GetCount() ) m_cbName.SetCurSel(0);
}

void CDlgTriggerOnOff::Refresh(void)
{
    for(int i=0;i<(int)m_cbName.GetCount();++i)
    {
        CString strText;
        m_cbName.GetLBText(i, strText);
        if( !_wcsicmp(m_TGOnOff.m_szTriggerName, strText) )
        {
            m_cbName.SetCurSel(i);
            break;
        }
    }
    if( m_cbOnOff.GetCount() > (int)m_TGOnOff.m_bOnOff ) m_cbOnOff.SetCurSel(m_TGOnOff.m_bOnOff);

    CString strTemp;
    m_lbCharType.ResetContent();
    std::vector<int>::iterator vecIter = m_TGOnOff.m_vecConCharType.begin();
    for(;vecIter != m_TGOnOff.m_vecConCharType.end(); ++vecIter)
    {
        strTemp.Format(L"%d", (*vecIter) );
        m_lbCharType.AddString(strTemp);
    }

    if( m_TGOnOff.m_bHasChar ) 
        m_cbHasChar.SetCurSel(1);
    else 
        m_cbHasChar.SetCurSel(0);
}

void CDlgTriggerOnOff::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strTGName = L"X";
    //if( m_cbName.GetCurSel() >= 0 )
    //    m_cbName.GetLBText(m_cbName.GetCurSel(), strTGName);
    m_cbName.GetWindowText(strTGName);

    CString strOnOff = L"";
    if( m_cbOnOff.GetCurSel() >= 0 )
        m_cbOnOff.GetLBText(m_cbOnOff.GetCurSel(), strOnOff);

    CString strText1, strText2, strText3;
    m_stTGOnOff1.GetWindowText(strText1);

    strText3 = L"";
    m_TGOnOff.m_vecConCharType.clear();
    for(int iLoop=0; iLoop<m_lbCharType.GetCount(); ++iLoop)
    {
        CString strCharType;
        m_lbCharType.GetText(iLoop, strCharType);
        if( (iLoop+1) < m_lbCharType.GetCount() )
            strText3 += strCharType + L", ";
        else
            strText3 += strCharType;
        int iCharType = _wtoi(strCharType);
        m_TGOnOff.m_vecConCharType.push_back( iCharType );
    }

    if( 0 < strText3.GetLength() )
    {
        CString strHasChar = L"";
        if( m_cbHasChar.GetCurSel() > 0 )
            strText3 += L"번 캐릭터가 있어야(파티전원)";
        else
            strText3 += L"번 캐릭터가 없어야(파티전원)";
        m_strDesc = MiddleBraceSidebyString(strTGName) + L" " + strText1 + L" " + MiddleBraceSidebyString(strText3) + MiddleBraceSidebyString(strOnOff);
    }
    else
        m_strDesc = MiddleBraceSidebyString(strTGName) + L" " + strText1 + L" " + MiddleBraceSidebyString(strOnOff);

    //************************************************************************
    // 설명 생성
    //************************************************************************
    //m_ebName.GetWindowText(m_TGOnOff.m_szTriggerName, sizeof(m_TGOnOff.m_szTriggerName));
    if( strTGName.GetLength() >= MAX_TG_STR_LENGTH )
    {
        CString strMsg;
        strMsg.Format(L"트리거명의 길이가 최대치(%d)를 넘었습니다. 트리거명을 저장 할 수 없습니다.", MAX_TG_STR_LENGTH);
        MessageBox(strMsg, L"알림", MB_OK);
        ZeroMemory(m_TGOnOff.m_szTriggerName, sizeof(m_TGOnOff.m_szTriggerName));
    }
    else
    {
        StringCchCopyN( m_TGOnOff.m_szTriggerName, sizeof( m_TGOnOff.m_szTriggerName ),
                        strTGName.GetBuffer(), strTGName.GetLength() );

    }

    m_TGOnOff.m_bOnOff = m_cbOnOff.GetCurSel();
    m_TGOnOff.m_bHasChar = m_cbHasChar.GetCurSel();
    m_TGOnOff.m_strDesc = GetDesc();
}

void CDlgTriggerOnOff::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTriggerOnOff::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTriggerOnOff::OnBnClickedAddCharType()
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
    m_lbCharType.SetCurSel( m_lbCharType.GetCount()-1 );
    m_ebCharType.SetWindowText( L"" );
    m_ebCharType.SetFocus();
}

void CDlgTriggerOnOff::OnLbDBClickedCharType()
{
    int iItem = m_lbCharType.GetCurSel();

    if( -1 >= iItem || iItem > m_lbCharType.GetCount() )
        ::MessageBox(NULL, L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lbCharType.DeleteString( iItem );
}
void CDlgTriggerOnOff::OnStnClickedStaticTriggerOnoff3()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
