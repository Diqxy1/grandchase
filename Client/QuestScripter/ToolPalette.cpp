// ToolPalette.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ToolPalette.h"
#include ".\toolpalette.h"
#include "PaletteStruct.h"
// CToolPalette 대화 상자입니다.

IMPLEMENT_DYNAMIC(CToolPalette, CDialog)
CToolPalette::CToolPalette(CWnd* pParent /*=NULL*/)
	: CDialog(CToolPalette::IDD, pParent)
{
    m_Type          = TG_TYPE_EACTION;
    m_bObject       = false;
    m_bPlayer       = false;
    m_bMonster      = false;
    m_bDoor         = false;
    m_bDamage       = false;

    m_dlgPaletteMon.SetObjectType(OBJECTTYPE_EACTION_MON);
    m_dlgPalettePlayer.SetObjectType(OBJECTTYPE_EACTION_PLAYER);
    m_dlgPaletteObject.SetObjectType(OBJECTTYPE_EACTION_OBJECT);
    m_dlgPaletteDoor.SetObjectType(OBJECTTYPE_EACTION_DOOR);
    m_dlgPaletteDamage.SetObjectType(OBJECTTYPE_EACTION_DAMAGE);
}

CToolPalette::~CToolPalette()
{

}
    
void CToolPalette::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_TYPE_EACTION, m_cbTypeEAction);
    DDX_Control(pDX, IDC_COMBO_TYPE_CONDITION, m_cbTypeCondition);
    m_cbTypeEAction.Init();
    m_cbTypeCondition.Init();

    m_cbTypeEAction.ResetContent();
    m_cbTypeEAction.AddSearchString(L"PLAYER");
    m_cbTypeEAction.InsertString(0, L"PLAYER");
    m_cbTypeEAction.AddSearchString(L"MONSTER");
    m_cbTypeEAction.InsertString(1, L"MONSTER");
    m_cbTypeEAction.AddSearchString(L"OBJECT");
    m_cbTypeEAction.InsertString(2, L"OBJECT");
    m_cbTypeEAction.AddSearchString(L"DOOR");
    m_cbTypeEAction.InsertString(3, L"DOOR");
    m_cbTypeEAction.AddSearchString(L"DAMAGE");
    m_cbTypeEAction.InsertString(4, L"DAMAGE");
    if( m_cbTypeEAction.GetCount() ) m_cbTypeEAction.SetCurSel(0);

    m_cbTypeCondition.AddSearchString(L"MONSTER_SELMODE");
    m_cbTypeCondition.InsertString(0, L"MONSTER_SELMODE");
    m_cbTypeCondition.AddSearchString(L"AREA_SELMODE");
    m_cbTypeCondition.InsertString(1, L"AREA_SELMODE");
    m_cbTypeCondition.AddSearchString(L"POS_SELMODE");
    m_cbTypeCondition.InsertString(2, L"POS_SELMODE");
    if( m_cbTypeCondition.GetCount() ) m_cbTypeCondition.SetCurSel(0);

    OnCbnSelchangeComboType();
    DDX_Control(pDX, IDC_CHECK_OBJECT, m_ckObject);
    DDX_Control(pDX, IDC_CHECK_PLAYER, m_ckPlayer);
    DDX_Control(pDX, IDC_CHECK_MONSTER, m_ckMonster);
    DDX_Control(pDX, IDC_CHECK_DAMAGE, m_ckDamage);

    m_ckObject.SetCheck( m_bObject || m_bDoor );
    m_ckPlayer.SetCheck( m_bPlayer );
    m_ckMonster.SetCheck( m_bMonster );
    m_ckDamage.SetCheck( m_bDamage );
}

BEGIN_MESSAGE_MAP(CToolPalette, CDialog)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE_EACTION, OnCbnSelchangeComboType)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE_CONDITION, OnCbnSelchangeComboType)
    ON_MESSAGE(WM_CLICK_TOOL_BUTTON, OnClickToolButton)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_CHECK_OBJECT, OnBnClickedCheckObject)
    ON_BN_CLICKED(IDC_CHECK_PLAYER, OnBnClickedCheckPlayer)
    ON_BN_CLICKED(IDC_CHECK_MONSTER, OnBnClickedCheckMonster)
    ON_BN_CLICKED(IDC_CHECK_DAMAGE, OnBnClickedCheckDamage)
END_MESSAGE_MAP()


// CToolPalette 메시지 처리기입니다.

void CToolPalette::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();

}

void CToolPalette::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}


void CToolPalette::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    CRect rtClient;
    GetClientRect(rtClient);
    rtClient.top += 140;

    if( m_dlgPaletteMon.GetSafeHwnd() )
        m_dlgPaletteMon.MoveWindow(rtClient, 1);

    if( m_dlgPalettePlayer.GetSafeHwnd() )
        m_dlgPalettePlayer.MoveWindow(rtClient, 1);

    if( m_dlgPaletteObject.GetSafeHwnd() )
        m_dlgPaletteObject.MoveWindow(rtClient, 1);

    if( m_dlgPaletteDoor.GetSafeHwnd() )
        m_dlgPaletteDoor.MoveWindow(rtClient, 1);

    if( m_dlgPaletteDamage.GetSafeHwnd() )
        m_dlgPaletteDamage.MoveWindow(rtClient, 1);
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CToolPalette::OnCbnSelchangeComboType()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int iDetailType = 0;

    switch( (int)m_Type )
    {
        case TG_TYPE_EACTION:
            {
                iDetailType = m_cbTypeEAction.GetCurSel();
                if( m_cbTypeEAction.GetCurSel() < (int)OBJECTTYPE_EACTION_PLAYER
                    || m_cbTypeEAction.GetCurSel() > (int)OBJECTTYPE_EACTION_MAXNUM 
                    )
                    iDetailType = 0;

                m_bPlayer   = (OBJECTTYPE_EACTION_PLAYER == iDetailType);
                m_bMonster  = (OBJECTTYPE_EACTION_MON == iDetailType);
                m_bObject   = (OBJECTTYPE_EACTION_OBJECT == iDetailType);
                m_bDoor     = (OBJECTTYPE_EACTION_DOOR == iDetailType);
                m_bDamage   = (OBJECTTYPE_EACTION_DAMAGE == iDetailType);
                RefreshCheckBoxs();

                m_dlgPalettePlayer.ShowWindow( m_bPlayer );
                m_dlgPaletteMon.ShowWindow( m_bMonster );
                m_dlgPaletteObject.ShowWindow( m_bObject );
                m_dlgPaletteDoor.ShowWindow( m_bDoor );
                m_dlgPaletteDamage.ShowWindow( m_bDamage );
            }
            break;
        case TG_TYPE_CONDITION:
            {
                iDetailType = m_cbTypeCondition.GetCurSel();
                if( m_cbTypeCondition.GetCurSel() < (int)OBJECTTYPE_CONDITION_SELECT_MON
                    || m_cbTypeCondition.GetCurSel() > (int)OBJECTTYPE_CONDITION_MAXNUM 
                    )
                    iDetailType = 0;

                m_bPlayer   = false;
                m_bMonster  = (OBJECTTYPE_CONDITION_SELECT_MON == iDetailType);
                m_bObject   = (OBJECTTYPE_CONDITION_SELECT_AREA == iDetailType);
                RefreshCheckBoxs();

                m_dlgPalettePlayer.ShowWindow( SW_HIDE );
                m_dlgPaletteMon.ShowWindow( SW_HIDE );
                m_dlgPaletteObject.ShowWindow( SW_HIDE );
                m_dlgPaletteDoor.ShowWindow( SW_HIDE );
                m_dlgPaletteDamage.ShowWindow( SW_HIDE );
            }
            break;
    }

    if( GetParent() )
    {
        GetParent()->SendMessage(WM_CLICK_TOOL_TYPE, (int)m_Type, iDetailType);

        if( m_dlgPalettePlayer.IsWindowVisible() ) m_dlgPalettePlayer.OnCbnSelchangeComboId();
        if( m_dlgPaletteMon.IsWindowVisible() ) m_dlgPaletteMon.OnCbnSelchangeComboId();
        if( m_dlgPaletteObject.IsWindowVisible() ) m_dlgPaletteObject.OnCbnSelchangeComboId();
        if( m_dlgPaletteDoor.IsWindowVisible() ) m_dlgPaletteDoor.OnCbnSelchangeComboId();
        if( m_dlgPaletteDamage.IsWindowVisible() ) m_dlgPaletteDamage.OnCbnSelchangeComboId();
    }
}

LRESULT CToolPalette::OnClickToolButton(WPARAM wParam, LPARAM lParam)
{
    // wParam은 [ObjectType], lParam [Button 포인터]
    if( GetParent() )
        GetParent()->PostMessage(WM_CLICK_TOOL_BUTTON, wParam, lParam);

    return S_OK;
}

int CToolPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
    if( -1 == m_dlgPaletteMon.Create(CDlgPaletteContent::IDD, this) 
        || -1 == m_dlgPalettePlayer.Create(CDlgPaletteContent::IDD, this)
        || -1 == m_dlgPaletteObject.Create(CDlgPaletteContent::IDD, this)
        || -1 == m_dlgPaletteDoor.Create(CDlgPaletteContent::IDD, this)
        || -1 == m_dlgPaletteDamage.Create(CDlgPaletteContent::IDD, this)
        )
    {
        MessageBox(L"도구 팔레트 생성에 문제가 발생되었습니다.", L"알림", MB_OK);
        return -1;
    }

    return 0;
}

BOOL CToolPalette::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CToolPalette::SetType(TG_TYPE enType, int iFuncType)
{
    m_Type = enType;
    switch( enType )
    {
        case TG_TYPE_EACTION:
            if( m_cbTypeCondition.GetSafeHwnd() ) m_cbTypeCondition.EnableWindow(false);
            if( m_cbTypeEAction.GetSafeHwnd() )
            {
                m_cbTypeEAction.EnableWindow(true);
                if( m_cbTypeEAction.GetCount() > (int)iFuncType )
                    m_cbTypeEAction.SetCurSel((int)iFuncType);
            }
            break;
        case TG_TYPE_CONDITION:
            if( m_cbTypeEAction.GetSafeHwnd() ) m_cbTypeEAction.EnableWindow(false);
            if( m_cbTypeCondition.GetSafeHwnd() )
            {                
                m_cbTypeCondition.EnableWindow(true);
                if( m_cbTypeCondition.GetSafeHwnd() )
                {
                    if( m_cbTypeCondition.GetCount() > (int)iFuncType )
                        m_cbTypeCondition.SetCurSel((int)iFuncType);
                }
            }
            break;
    }
}

void CToolPalette::OnBnClickedCheckObject()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_ckObject.GetSafeHwnd() )
        m_bDoor = m_bObject = m_ckObject.GetCheck();
}

void CToolPalette::OnBnClickedCheckPlayer()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_ckPlayer.GetSafeHwnd() ) m_bPlayer = m_ckPlayer.GetCheck();
}

void CToolPalette::OnBnClickedCheckMonster()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_ckMonster.GetSafeHwnd() ) m_bMonster = m_ckMonster.GetCheck();
}

BOOL CToolPalette::ShowWindow(int nCmdShow)
{
    RefreshCheckBoxs();

    return CDialog::ShowWindow(nCmdShow);
}

void CToolPalette::RefreshCheckBoxs(void)
{
    if( m_ckObject.GetSafeHwnd() )      m_ckObject.SetCheck(m_bObject || m_bDoor);
    if( m_ckPlayer.GetSafeHwnd() )      m_ckPlayer.SetCheck(m_bPlayer);
    if( m_ckMonster.GetSafeHwnd() )     m_ckMonster.SetCheck(m_bMonster);
    if( m_ckDamage.GetSafeHwnd() )      m_ckDamage.SetCheck(m_bDamage);
}
void CToolPalette::OnBnClickedCheckDamage()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_ckDamage.GetSafeHwnd() ) m_bDamage = m_ckDamage.GetCheck();
}
