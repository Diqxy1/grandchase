// DlgTGGateObjectGenerater.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGGateObjectGenerater.h"
#include "ObjectDataManager.h"
#include "struct.h"
#include ".\dlgtggateobjectgenerater.h"

// CDlgTGGateObjectGenerater 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGGateObjectGenerater, CDlgTGBase)
CDlgTGGateObjectGenerater::CDlgTGGateObjectGenerater(CWnd* pParent /*=NULL*/)
{
}

CDlgTGGateObjectGenerater::~CDlgTGGateObjectGenerater()
{
}

void CDlgTGGateObjectGenerater::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_COMBO_OBJ_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_OBJ_TYPE, m_cbType);
    DDX_Control(pDX, IDC_COMBO_OBJ_RIGHT, m_cbRight);
    DDX_Control(pDX, IDC_EDIT_OBJ_X, m_ebEntranceX);
    DDX_Control(pDX, IDC_EDIT_OBJ_Y, m_ebEntranceY);
    DDX_Control(pDX, IDC_COMBO_OBJ_GEN, m_cbGen);
    DDX_Control(pDX, IDC_STATIC_OBJGEN_1, m_stObjGen1);
    DDX_Control(pDX, IDC_STATIC_OBJGEN_2, m_stObjGen2);
    DDX_Control(pDX, IDC_STATIC_OBJGEN_3, m_stObjGen3);
    DDX_Control(pDX, IDC_STATIC_OBJGEN_4, m_stObjGen4);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, m_cbDirection);
    DDX_Control(pDX, IDC_EDIT_DELAY, m_ebDelay);
    DDX_Control(pDX, IDC_STATIC_OBJGEN_5, m_stObjGen5);
    DDX_Control(pDX, IDC_EDIT_USE_SELECT, m_ebUseSelect);
    DDX_Control(pDX, IDC_EDIT_DOTA_STAGE, m_ebDotaStage);
    

    m_cbType.Init();

    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_MONSTER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

    m_cbType.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_DOOR ).size();++i)
    {
        m_cbType.InsertString( i, SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_DOOR )[i] );
        m_cbType.AddSearchString( SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_DOOR )[i] );
    }
    if( m_cbType.GetCount() > 0 ) m_cbType.SetCurSel(0);

    m_cbRight.ResetContent();
    m_cbRight.InsertString(0, L"Esquerda");
    m_cbRight.InsertString(1, L"Direita");
    if( m_cbRight.GetCount() > 0 ) m_cbRight.SetCurSel(0);

    m_ebEntranceX.SetWindowText(L"");
    m_ebEntranceY.SetWindowText(L"");
    m_ebDelay.SetWindowText(L"");
    m_ebUseSelect.SetWindowText(L"");
    m_ebDotaStage.SetWindowText(L"");

    m_cbDirection.ResetContent();
    m_cbDirection.InsertString(0, L"Mover pra porta da Esquerda");
    m_cbDirection.InsertString(1, L"Mover pra porta de Cima");
    m_cbDirection.InsertString(2, L"Mover pra porta da Direita");
    m_cbDirection.InsertString(3, L"Mover pra porta de Baixo");
    m_cbDirection.InsertString(4, L"Personalizado");
    if( m_cbDirection.GetCount() > 1 ) m_cbDirection.SetCurSel(0);

    m_cbGen.ResetContent();
    m_cbGen.InsertString(0, L"Remover.");
    m_cbGen.InsertString(1, L"Criar.");
    if( m_cbGen.GetCount() > 1 ) m_cbGen.SetCurSel(1);
}

BEGIN_MESSAGE_MAP(CDlgTGGateObjectGenerater, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, OnCbnSelchangeComboDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_TYPE, OnCbnSelchangeComboObjType)
    ON_STN_CLICKED(IDC_STATIC_OBJGEN_7, &CDlgTGGateObjectGenerater::OnStnClickedStaticObjgen7)
END_MESSAGE_MAP()


// CDlgTGGateObjectGenerater 메시지 처리기입니다.
void CDlgTGGateObjectGenerater::Refresh(void)
{
    if( m_cbRight.GetCount() > (int)m_TGGateObjectGenerater.m_bRight ) m_cbRight.SetCurSel(m_TGGateObjectGenerater.m_bRight);
    if( m_cbSlot.GetCount() > m_TGGateObjectGenerater.m_iSlot ) m_cbSlot.SetCurSel(m_TGGateObjectGenerater.m_iSlot);
    if( m_cbType.GetCount() > m_TGGateObjectGenerater.m_iType ) m_cbType.SetCurSel(m_TGGateObjectGenerater.m_iType);
    if( m_cbGen.GetCount() > (int)m_TGGateObjectGenerater.m_bGenerate ) m_cbGen.SetCurSel((int)m_TGGateObjectGenerater.m_bGenerate);
    if( m_cbDirection.GetCount() > (int)ConvertToComboBoxNumber(m_TGGateObjectGenerater.m_iDirection) ) 
        m_cbDirection.SetCurSel( ConvertToComboBoxNumber(m_TGGateObjectGenerater.m_iDirection) );

    CString strX, strY;
    strX.Format(L"%f", m_TGGateObjectGenerater.m_fEntranceX);
    strY.Format(L"%f", m_TGGateObjectGenerater.m_fEntranceY);
    m_ebEntranceX.SetWindowText(strX);
    m_ebEntranceY.SetWindowText(strY);

    CString strDelay;
    strDelay.Format(L"%d", m_TGGateObjectGenerater.m_iDelayTime);
    m_ebDelay.SetWindowText(strDelay);

    CString strUseSelectDirection;
    strUseSelectDirection.Format(L"%d", m_TGGateObjectGenerater.m_iDirection);
    m_ebUseSelect.SetWindowText(strUseSelectDirection);

    CString strDotaStage;
    strDotaStage.Format(L"%d", m_TGGateObjectGenerater.m_iDotaStage);
    m_ebDotaStage.SetWindowText(strDotaStage);

    OnCbnSelchangeComboDirection();
}

void CDlgTGGateObjectGenerater::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strSlot = L"X";
        CString strObj = L"X";
        if( m_cbSlot.GetCurSel() >= 0 )
            m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

        m_cbType.GetWindowText(strObj);        
        int index = m_cbType.FindStringExact(0, strObj);
        if( CB_ERR != index )
            m_cbType.SetCurSel(index);
        else
            strObj = L"X";

        CString strRight;
        if( m_cbRight.GetCount() > m_cbRight.GetCurSel() ) m_cbRight.GetLBText(m_cbRight.GetCurSel(), strRight);

        CString strX, strY;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);

        CString strGen;
        m_cbGen.GetWindowText(strGen);        
        index = m_cbGen.FindStringExact(0, strGen);
        if( CB_ERR != index )
            m_cbGen.SetCurSel(index);
        else
            strGen = "X";

        CString strDirection;
        m_cbDirection.GetWindowText(strDirection);
        index = m_cbDirection.FindStringExact(0, strDirection);
        if( CB_ERR != index )
            m_cbDirection.SetCurSel(index);
        else
            strDirection = "X";

        CString strDelay;
        m_ebDelay.GetWindowText(strDelay);

        CString strText1, strText2, strText3, strText4, strText5, strText6;
        m_stObjGen1.GetWindowText(strText1);
        m_stObjGen2.GetWindowText(strText2);
        m_stObjGen3.GetWindowText(strText3);
        m_stObjGen4.GetWindowText(strText4);
        m_stObjGen5.GetWindowText(strText5);

        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strDelay)
            + L" " + strText5 + L" " + MiddleBraceSidebyString(strDirection)
            + MiddleBraceSidebyString(strObj) + L" " + strText2 
            + L" " + MiddleBraceSidebyString(strRight) + strText3
            + L" (" + MiddleBraceSidebyString(strX) + L"," + MiddleBraceSidebyString(strY) + L") " + strText4 + L" " +
            + L" " + MiddleBraceSidebyString(strGen);
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGGateObjectGenerater.m_bRight = (bool)m_cbRight.GetCurSel();
        m_TGGateObjectGenerater.m_iSlot = m_cbSlot.GetCurSel();
        m_TGGateObjectGenerater.m_iType = m_cbType.GetCurSel();
        m_TGGateObjectGenerater.m_bGenerate = m_cbGen.GetCurSel();
        m_TGGateObjectGenerater.m_iDirection = ConvertToImageNumber( m_cbDirection.GetCurSel() );

        CString strUseSelectDirection;
        if( DIRECTION_USE_SELECT == m_TGGateObjectGenerater.m_iDirection )
        {
            m_ebUseSelect.GetWindowText(strUseSelectDirection);
            m_TGGateObjectGenerater.m_iDirection = (int)_wtoi(strUseSelectDirection);
        }

        CString strX, strY, strDelay, strDotaStage;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);
        m_ebDelay.GetWindowText(strDelay);
        m_ebDotaStage.GetWindowText(strDotaStage);
        m_TGGateObjectGenerater.m_fEntranceX = (float)_wtof(strX);
        m_TGGateObjectGenerater.m_fEntranceY = (float)_wtof(strY);
        m_TGGateObjectGenerater.m_iDelayTime = (int)_wtoi(strDelay);
        m_TGGateObjectGenerater.m_iDotaStage = (int)_wtoi(strDotaStage);
        m_TGGateObjectGenerater.m_strDesc = GetDesc();
    }
}


void CDlgTGGateObjectGenerater::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGGateObjectGenerater::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGGateObjectGenerater::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        AfxGetMainWnd()->SendMessage(WM_GATEOBJECT_ADD_WINDOW, 0, 0);
        SetFocus();
    }
}

// 이미지 넘버를 넣으면 콤보박스 번호로 리턴해준다.
int CDlgTGGateObjectGenerater::ConvertToComboBoxNumber(int iGateNum)
{
    switch(iGateNum)
    {
    case DIRECTION_LEFT:             // left
        return 0;
        break;
    case DIRECTION_UP:               // top
        return 1;
        break;
    case DIRECTION_RIGHT:            // right
        return 2;
        break;
    case DIRECTION_DOWN:             // bottom
        return 3;
        break;
    case DIRECTION_USE_SELECT:       // 사용자 지정
        return 4;
        break;
    }

    return 0;
}


// 콤보박스의 0, 1, 2, 3을 기준으로 left right up down 값을 판단하여 리턴
int CDlgTGGateObjectGenerater::ConvertToImageNumber(int iGateNum)
{
    switch(iGateNum)
    {
    case 0:             // left
        return DIRECTION_LEFT;
        break;
    case 1:             // top
        return DIRECTION_UP;
        break;
    case 2:             // right
        return DIRECTION_RIGHT;
        break;
    case 3:             // bottom
        return DIRECTION_DOWN;
        break;
    case 4:
        return DIRECTION_USE_SELECT;// 사용자 지정
        break;
    }

    return 0;
}
void CDlgTGGateObjectGenerater::OnCbnSelchangeComboObjType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgTGGateObjectGenerater::OnCbnSelchangeComboDirection()
{
    if( DIRECTION_USE_SELECT == ConvertToImageNumber( m_cbDirection.GetCurSel() ) )
    {
        m_ebUseSelect.EnableWindow( TRUE );
    }
    else
    {
        m_ebUseSelect.EnableWindow( FALSE );
    }
}

void CDlgTGGateObjectGenerater::OnStnClickedStaticObjgen7()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
