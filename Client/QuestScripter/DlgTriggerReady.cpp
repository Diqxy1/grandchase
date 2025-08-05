// DlgTriggerReady.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTriggerReady.h"
#include "DlgTrigger.h"
#include "QuestScripterDlg.h"

// CDlgTriggerReady 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTriggerReady, CDlgTGBase)
CDlgTriggerReady::CDlgTriggerReady(CWnd* pParent /*=NULL*/)
{
}

CDlgTriggerReady::~CDlgTriggerReady()
{
}

void CDlgTriggerReady::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_COMBO_TGREADY_NAME, m_cbName);
    m_cbName.Init();

    InitTGName();

    Refresh();    
    DDX_Control(pDX, IDC_STATIC_TRIGGER_READY_1, m_stTGReady);
}


BEGIN_MESSAGE_MAP(CDlgTriggerReady, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTriggerReady 메시지 처리기입니다.
void CDlgTriggerReady::InitTGName(void)
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
                    std::wstring strTGName = pCategory->m_vtBaseTrigger[k]->m_strTGName;                       
                    m_cbName.InsertString(m_cbName.GetCount(), strTGName.c_str());
                    m_cbName.AddSearchString( strTGName.c_str() );
                }
            }
        }
    }
    if( m_cbName.GetCount() ) m_cbName.SetCurSel(0);
}


void CDlgTriggerReady::Refresh(void)
{
    for(int i=0;i<(int)m_cbName.GetCount();++i)
    {
        CString strText;
        m_cbName.GetLBText(i, strText);

        CString strTrigger = m_TGReady.m_szTriggerName;

        
        if( strText == strTrigger )
        {
            m_cbName.SetCurSel(i);
            break;
        }
    }
}

void CDlgTriggerReady::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strTGName = L"X";
    m_cbName.GetWindowText(strTGName);

    CString strText1, strText2;
    m_stTGReady.GetWindowText(strText1);

    m_strDesc = MiddleBraceSidebyString(strTGName) + L" " + strText1;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    //m_ebName.GetWindowText(m_TGOnOff.m_szTriggerName, sizeof(m_TGOnOff.m_szTriggerName));
    if( strTGName.GetLength() >= MAX_TG_STR_LENGTH )
    {
        CString strMsg;
        strMsg.Format(L"트리거명의 길이가 최대치(%d)를 넘었습니다. 트리거명을 저장 할 수 없습니다.", MAX_TG_STR_LENGTH);
        MessageBox(strMsg, L"알림", MB_OK);
        ZeroMemory(m_TGReady.m_szTriggerName, sizeof(m_TGReady.m_szTriggerName));
    }
    else
    {
        StringCchCopyN( m_TGReady.m_szTriggerName, sizeof( m_TGReady.m_szTriggerName ),
                        strTGName.GetBuffer(), strTGName.GetLength() );
    }

    m_TGReady.m_strDesc = GetDesc();
}

void CDlgTriggerReady::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTriggerReady::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}