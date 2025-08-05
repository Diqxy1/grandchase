// DlgTGSoundOnOff.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DlgTGSoundOnOff.h"


// CDlgTGSoundOnOff 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGSoundOnOff, CDlgTGBase)
CDlgTGSoundOnOff::CDlgTGSoundOnOff(CWnd* pParent /*=NULL*/)
{
}

CDlgTGSoundOnOff::~CDlgTGSoundOnOff()
{
}

void CDlgTGSoundOnOff::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_MUSIC_ONOFF, m_cbOnOff);
    //DDX_Control(pDX, IDC_EDIT_MUSIC_FILENAME, m_ebSoundFileName);

    //m_ebSoundFileName.SetReadOnly(1);
    //m_ebSoundFileName.SetBackColor(RGB(255, 255, 255));
    //m_ebSoundFileName.SetTextColor(RGB(0, 0, 0));
    m_cbOnOff.ResetContent();
    m_cbOnOff.InsertString(0, L"멈춤");
    m_cbOnOff.InsertString(1, L"재생");
    if( m_cbOnOff.GetCount() ) m_cbOnOff.SetCurSel(0);

    Refresh();
    DDX_Control(pDX, IDC_COMBO_MUSIC_FILENAME, m_cbFileName);
    m_cbFileName.Init();

    RefreshSoundDirectory();
    DDX_Control(pDX, IDC_STATIC_SOUNDONOFF_1, m_stSoundOnOff1);
    DDX_Control(pDX, IDC_STATIC_SOUNDONOFF_2, m_stSoundOnOff2);
}


BEGIN_MESSAGE_MAP(CDlgTGSoundOnOff, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_MUSIC_FILE, OnBnClickedButtonMusicFile)
END_MESSAGE_MAP()


void CDlgTGSoundOnOff::RefreshSoundDirectory(void)
{
    m_cbFileName.ResetContent();
    //CString strFilePath = g_Path;
    //strFilePath += "/Data/Music/*.*";

    CString strSoundPath = SiCDlgDirectorySetting()->m_strSoundPath;
    strSoundPath += "\\*.*";

    CFileFind cFileFind;
    if( cFileFind.FindFile(strSoundPath) )
    {
        while(1)
        {
            if( !cFileFind.FindNextFile() ) break;

            if( !cFileFind.IsDirectory()
                && !cFileFind.IsDots() )
            {
                m_cbFileName.InsertString(m_cbFileName.GetCount(), cFileFind.GetFileName());
                m_cbFileName.AddSearchString(cFileFind.GetFileName());
            }
        }
    }
    if( m_cbFileName.GetCount() > 0 ) m_cbFileName.SetCurSel(0);
}

// CDlgTGSoundOnOff 메시지 처리기입니다.
void CDlgTGSoundOnOff::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGSoundOnOff::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGSoundOnOff::Refresh(void)
{
    int index = m_cbFileName.FindStringExact(0, m_SoundOnOff.m_szSoundFileName);
    if( CB_ERR != index )
        m_cbFileName.SetCurSel(index);
    if( m_cbOnOff.GetCount() > (int)m_SoundOnOff.m_bSoundOn ) m_cbOnOff.SetCurSel(m_SoundOnOff.m_bSoundOn);
}

void CDlgTGSoundOnOff::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strSoundFileName;
    if( m_cbFileName.GetCurSel() >= 0 )
        m_cbFileName.GetLBText(m_cbFileName.GetCurSel(), strSoundFileName);
    m_cbFileName.GetWindowText(strSoundFileName);
    int index = m_cbFileName.FindStringExact(0, strSoundFileName);
    if( CB_ERR != index )
        m_cbFileName.SetCurSel(index);
    else
        strSoundFileName = L"X";        

    CString strOnOff = L"";
    if( m_cbOnOff.GetCurSel() >= 0 )
        m_cbOnOff.GetLBText(m_cbOnOff.GetCurSel(), strOnOff);

    CString strText1, strText2;
    m_stSoundOnOff1.GetWindowText(strText1);
    m_stSoundOnOff2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strSoundFileName) + L" " + strText1 + L" " + MiddleBraceSidebyString(strOnOff) + L" " + strText2;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    if( strSoundFileName.GetLength() >= MAX_TG_STR_LENGTH )
    {
        CString strMsg;
        strMsg.Format(L"음악파일의 길이가 최대치(%d)를 넘었습니다. 파일명을 저장 할 수 없습니다.", MAX_TG_STR_LENGTH);
        MessageBox(strMsg, L"알림", MB_OK);
        ZeroMemory(m_SoundOnOff.m_szSoundFileName, sizeof(m_SoundOnOff.m_szSoundFileName));
    }
    else
    {
        StringCchCopyN( m_SoundOnOff.m_szSoundFileName, sizeof( m_SoundOnOff.m_szSoundFileName ),
            strSoundFileName.GetBuffer(), strSoundFileName.GetLength() );
    }
    m_SoundOnOff.m_bSoundOn = m_cbOnOff.GetCurSel();
    m_SoundOnOff.m_strDesc = GetDesc();
}
void CDlgTGSoundOnOff::OnBnClickedButtonMusicFile()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //char Filter[] = "배경음악(*.mp3)|*.mp3|";
    //CFileDialog dlg(TRUE, "*.mp3(*.mp3)", "*.mp3", OFN_HIDEREADONLY | 
    //    OFN_FILEMUSTEXIST, Filter, NULL);

    //CString strPath = g_Path;
    //strPath += "\\Data\\Music";
    //dlg.m_ofn.lpstrInitialDir = (LPSTR)(strPath.GetBuffer());

    //if( IDOK == dlg.DoModal() )
    //{
    //    char szFileName[512];
    //    char szExt[256];
    //    _splitpath(dlg.GetPathName(), 0, 0, szFileName, szExt);
    //    strcat(szFileName, szExt);
    //    m_ebSoundFileName.SetWindowText(szFileName);
    //}
}
