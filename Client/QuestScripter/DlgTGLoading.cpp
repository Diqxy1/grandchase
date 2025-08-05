// DlgTGLoading.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGLoading.h"
#include ".\dlgtgloading.h"


// CDlgTGLoading 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGLoading, CDlgTGBase)
CDlgTGLoading::CDlgTGLoading(CWnd* pParent /*=NULL*/)
{
}

CDlgTGLoading::~CDlgTGLoading()
{
}

void CDlgTGLoading::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_LOADING_ONOFF, m_cbOnOff);
    //DDX_Control(pDX, IDC_EDIT_LOAD_FILENAME, m_ebLoadingTitleFileName);
    DDX_Control(pDX, IDC_COMBO_LOAD_ENUM, m_cbLoad_Enum);
    m_cbLoad_Enum.Init();

    m_cbOnOff.ResetContent();
    m_cbOnOff.InsertString(0, L"Sair");
    m_cbOnOff.InsertString(1, L"Marcar");
    m_cbOnOff.SetCurSel(0);

    m_mapQuestEnum.clear();
    m_mapQuestEnum[L"GC_GM_QUEST1"]          = 10;
    m_mapQuestEnum[L"GC_GM_QUEST2"]          = 11;
    m_mapQuestEnum[L"GC_GM_QUEST3"]          = 12;
    m_mapQuestEnum[L"GC_GM_QUEST4"]          = 13;
    m_mapQuestEnum[L"GC_GM_QUEST5"]          = 14;
    m_mapQuestEnum[L"GC_GM_QUEST6"]          = 15;
    //m_mapQuestEnum[16]          = "GC_GM_QUEST1_1_VVVIC";
    //m_mapQuestEnum[17]          = "GC_GM_QUEST1_2_VVVIC";
    //m_mapQuestEnum[18]          = "GC_GM_QUEST2_1_VVVIC";
    //m_mapQuestEnum[19]          = "GC_GM_QUEST2_2_VVVIC";
    //m_mapQuestEnum[20]          = "GC_GM_QUEST3_1_VVVIC";
    //m_mapQuestEnum[21]          = "GC_GM_QUEST3_2_VVVIC";
    //m_mapQuestEnum[22]          = "GC_GM_QUEST4_1_VVVIC";
    //m_mapQuestEnum[23]          = "GC_GM_QUEST4_2_VVVIC";
    //m_mapQuestEnum[24]          = "GC_GM_QUEST6_1_VVVIC";
    //m_mapQuestEnum[25]          = "GC_GM_QUEST6_2_VVVIC";
    m_mapQuestEnum[L"GC_GM_QUEST_ZERO"]      = 26;
    m_mapQuestEnum[L"GC_GM_QUEST_ZERO_2"]    = 27;
    //m_mapQuestEnum[28]          = "GC_GM_INDIGO_TEAM";
    //m_mapQuestEnum[29]          = "GC_GM_TUTORIAL";
    m_mapQuestEnum[L"GC_GM_QUEST_EPISODE1"]  = 30;
    m_mapQuestEnum[L"GC_GM_QUEST_EPISODE2"]  = 31;
    //m_mapQuestEnum[32]          = "GC_GM_TAG_TEAM";
    //m_mapQuestEnum[33]          = "GC_GM_TAG_SURVIVAL";
    //m_mapQuestEnum[34]          = "GC_GM_ONE_ON_ONE";
    m_mapQuestEnum[L"GC_GM_QUEST_BERMESIA_FINAL"] = 35;

    m_cbLoad_Enum.ResetContent();
    std::map<CString, int>::iterator itQEnum = m_mapQuestEnum.begin();
    for(;m_mapQuestEnum.end() != itQEnum;itQEnum++)
    {
        m_cbLoad_Enum.InsertString(m_cbLoad_Enum.GetCount(), itQEnum->first);
        m_cbLoad_Enum.AddSearchString(itQEnum->first);
    }
    if( m_cbLoad_Enum.GetCount() ) m_cbLoad_Enum.SetCurSel(0);
    DDX_Control(pDX, IDC_STATIC_LOADING_1, m_stLoading1);
    DDX_Control(pDX, IDC_STATIC_LOADING_2, m_stLoading2);
    DDX_Control(pDX, IDC_STATIC_LOADING_3, m_stLoading3);
}


BEGIN_MESSAGE_MAP(CDlgTGLoading, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGLoading 메시지 처리기입니다.
void CDlgTGLoading::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGLoading::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGLoading::Refresh(void)
{
    for(int i=0;i<(int)m_cbLoad_Enum.GetCount();++i)
    {
        CString strText;
        m_cbLoad_Enum.GetLBText(i, strText);
        if( m_Loading.m_iTexID == m_mapQuestEnum[strText] )
        {
            m_cbLoad_Enum.SetCurSel(i);
            break;
        }
    }
    if( m_cbOnOff.GetCount() > (int)m_Loading.m_bLoad ) m_cbOnOff.SetCurSel((int)m_Loading.m_bLoad);
}

void CDlgTGLoading::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    // CString strLoadingTitleFileName;
    // m_ebLoadingTitleFileName.GetWindowText(strLoadingTitleFileName);
    CString strLoadingTitleName = L"X";
    m_cbLoad_Enum.GetWindowText(strLoadingTitleName);

    CString strOnOff;
    if( m_cbOnOff.GetCurSel() >= 0 )
        m_cbOnOff.GetLBText(m_cbOnOff.GetCurSel(), strOnOff);

    CString strText1, strText2, strText3;
    m_stLoading1.GetWindowText(strText1);
    m_stLoading2.GetWindowText(strText2);
    m_stLoading3.GetWindowText(strText3);

    m_strDesc = MiddleBraceSidebyString(strLoadingTitleName) + L" " + strText1 + L" " + strText2 
                + L" " + MiddleBraceSidebyString(strOnOff) + L" " + strText3;


    //************************************************************************
    // 값 저장
    //************************************************************************    
    int iTextID = 10;
    for( std::map<CString, int>::iterator itQE = m_mapQuestEnum.begin();m_mapQuestEnum.end() != itQE; itQE++)
    {
        if(itQE->first == strLoadingTitleName )
        {
            iTextID = itQE->second;
            break;
        }
    }

    m_Loading.SetValue(m_cbOnOff.GetCurSel(), iTextID);
    m_Loading.m_strDesc     = GetDesc();
}