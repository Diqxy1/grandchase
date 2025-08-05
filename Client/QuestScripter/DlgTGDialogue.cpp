// DlgTGDialogue.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGDialogue.h"
//#include "../GCLuaManager/KLuaManager.h"
#include "FunctionToScriptFile.h"
#include "DlgDirectorySetting.h"

// CDlgTGDialogue 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGDialogue, CDlgTGBase)
CDlgTGDialogue::CDlgTGDialogue(CWnd* pParent /*=NULL*/)
{
}

CDlgTGDialogue::~CDlgTGDialogue()
{
}

void CDlgTGDialogue::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_TG_DIALOGUE, m_cbDialogue);
    DDX_Control(pDX, IDC_COMBO_TG_ONOFF, m_cbOnOff);
    DDX_Control(pDX, IDC_EDIT_CHAR, m_ceChar);
    

    m_cbDialogue.Init();
    m_cbDialogue.ResetContent();

    KLuaManager luaMgr;
    std::string strQuestDialogueFile = GCUTIL_STR::GCStrWideToChar( SiCDlgDirectorySetting()->m_strQuestScriptPath );
    strQuestDialogueFile += "/QuestDialogue.stg";
    if( SUCCEEDED(luaMgr.DoFile(strQuestDialogueFile.c_str())) )
    {
        if( SUCCEEDED(luaMgr.BeginTable("dialogue")) )
        {
            for(int i=1;;++i)
            {
                if( SUCCEEDED(luaMgr.BeginTable(i)) )
                {
                    std::string strName;
                    luaMgr.GetValue("name", strName);

                    std::wstring strName2 = GCUTIL_STR::GCStrCharToWide( strName.c_str() );
                    
                    m_cbDialogue.AddSearchString(strName2.c_str());
                    m_cbDialogue.InsertString(m_cbDialogue.GetCount(), strName2.c_str());
                    for(int j=1;;++j)
                    {
                        if( SUCCEEDED(luaMgr.BeginTable(j)) )
                        {
                            bool bRight;
                            int iChar, iString;
                            luaMgr.GetValue(1, bRight);
                            luaMgr.GetValue(2, iChar);
                            luaMgr.GetValue(3, iString);

                            luaMgr.EndTable();
                        }
                        else
                        {
                            break;
                        }
                    }

                    luaMgr.EndTable();
                }
                else
                {
                    break;
                }
            }
            luaMgr.EndTable();
        }
    }

    if( m_cbDialogue.GetCount() > 0 ) m_cbDialogue.SetCurSel(0);
    m_cbOnOff.ResetContent();
    m_cbOnOff.InsertString(0, L"Ocultar Dialogo");
    m_cbOnOff.InsertString(1, L"Mostrar Dialogo");
    m_cbOnOff.SetCurSel(0);

    CString strChar("-1");
    m_ceChar.SetWindowText(strChar);
}


BEGIN_MESSAGE_MAP(CDlgTGDialogue, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGDialogue 메시지 처리기입니다.

void CDlgTGDialogue::Refresh(void)
{
    int index = m_cbDialogue.FindStringExact(0, m_TGDialogue.m_strDialogue.c_str());
    if( CB_ERR != index )
        m_cbDialogue.SetCurSel(index);

    if( m_cbOnOff.GetCount() > (int)m_TGDialogue.m_bOnOff ) m_cbOnOff.SetCurSel((int)m_TGDialogue.m_bOnOff);

    if( -1 < m_TGDialogue.m_iChar )
    {
        CString strChar;
        strChar.Format(L"%d", m_TGDialogue.m_iChar);
        m_ceChar.SetWindowText(strChar);
    }
}

void CDlgTGDialogue::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strDialogue;
    if( m_cbDialogue.GetCurSel() >= 0 && m_cbDialogue.GetCount() > m_cbDialogue.GetCurSel() )
        m_cbDialogue.GetLBText(m_cbDialogue.GetCurSel(), strDialogue);

    CString strOnOff;
    if( m_cbOnOff.GetCurSel() >= 0 && m_cbOnOff.GetCount() > m_cbOnOff.GetCurSel() )
        m_cbOnOff.GetLBText(m_cbOnOff.GetCurSel(), strOnOff);

    m_strDesc = MiddleBraceSidebyString(strDialogue) + L" " + MiddleBraceSidebyString(strOnOff);

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGDialogue.m_bOnOff           = (bool)m_cbOnOff.GetCurSel();
    m_TGDialogue.m_strDialogue      = strDialogue.GetBuffer();
    m_TGDialogue.m_strDesc = GetDesc();

    CString strChar;
    m_ceChar.GetWindowText( strChar );
    m_TGDialogue.m_iChar = _wtoi(strChar);
}

void CDlgTGDialogue::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGDialogue::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}