// DlgTGWeather.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGEmbark.h"
#include "ObjectDataManager.h"

// CDlgTGWeather 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGEmbark, CDlgTGBase)
CDlgTGEmbark::CDlgTGEmbark(CWnd* pParent /*=NULL*/)
{
}

CDlgTGEmbark::~CDlgTGEmbark()
{
}

void CDlgTGEmbark::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_EMBARK_TYPE, m_cbEmbarkType);
    DDX_Control(pDX, IDC_COMBO_EMBARK_RIGHT, m_cbIsRight);
    DDX_Control(pDX, IDC_EDIT_EMBARK_X, m_ebPosX);
    DDX_Control(pDX, IDC_EDIT_EMBARK_Y, m_ebPosY);
    DDX_Control(pDX, IDC_EDIT_EMBARK_SLOT, m_ebEmbarkSlot);
    DDX_Control(pDX, IDC_COMBO_EMBARK_CHAR_SLOT, m_cbEmbarkationCharSlot);

    m_cbEmbarkType.ResetContent();
    std::vector<CString>& vecEmbarkList = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_EMBARK);
    std::vector<CString>::iterator vecIter = vecEmbarkList.begin();
    for(int iLoop=0; vecIter != vecEmbarkList.end(); ++iLoop, ++vecIter)
    {
        m_cbEmbarkType.InsertString( iLoop, *vecIter);
    }
    m_cbEmbarkType.SetCurSel( 0 );

    m_cbIsRight.ResetContent();
    m_cbIsRight.InsertString( 0, L"Esquerda");
    m_cbIsRight.InsertString( 1, L"Direita");
    m_cbIsRight.SetCurSel( 0 );

    m_cbEmbarkationCharSlot.ResetContent();
    m_cbEmbarkationCharSlot.InsertString(0, L"Vazio");
    for(int i=1;i<(int)MAX_SLOT_PLAYER+1;++i)
    {
        CString strIndex;
        strIndex.Format(L"%d Personagens", (i-1));
        m_cbEmbarkationCharSlot.InsertString(i, strIndex);
    }
    m_cbEmbarkationCharSlot.SetCurSel( 0 );

    m_ebPosX.SetWindowText(L"0.0");
    m_ebPosY.SetWindowText(L"0.0");
    m_ebEmbarkSlot.SetWindowText(L"0");
}


BEGIN_MESSAGE_MAP(CDlgTGEmbark, CDialog)
END_MESSAGE_MAP()


// CDlgTGEmbark 메시지 처리기
void CDlgTGEmbark::Refresh(void)
{
    if( m_cbEmbarkType.GetCount() > m_TGEmbark.m_iEmbarkType ) 
    {
        m_cbEmbarkType.SetCurSel( m_TGEmbark.m_iEmbarkType );
    }

    if( m_cbEmbarkationCharSlot.GetCount() > m_TGEmbark.m_iEmbarkationCharSlot ) 
    {
        m_cbEmbarkationCharSlot.SetCurSel( m_TGEmbark.m_iEmbarkationCharSlot );
    }

    if( m_cbIsRight.GetCount() > 1 ) 
    {
        m_cbIsRight.SetCurSel( (m_TGEmbark.m_bIsRight?1:0) );
    }

    CString strTemp;
    strTemp.Format(L"%f", m_TGEmbark.m_vPos.x);
    m_ebPosX.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGEmbark.m_vPos.y);
    m_ebPosY.SetWindowText( strTemp );
    strTemp.Format(L"%d", m_TGEmbark.m_iEmbarkSlot);
    m_ebEmbarkSlot.SetWindowText( strTemp );
}

// 값 저장
void CDlgTGEmbark::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strType, strChar=L"";
        if( m_cbEmbarkType.GetCount() > m_cbEmbarkType.GetCurSel() ) m_cbEmbarkType.GetLBText(m_cbEmbarkType.GetCurSel(), strType);

        int iCharIndex = m_cbEmbarkationCharSlot.GetCurSel();
        if( 0 < iCharIndex )
        {
            m_cbEmbarkationCharSlot.GetLBText(iCharIndex, strChar);
            strChar += L" a bordo ";
        }

        m_strDesc = MiddleBraceSidebyString( strType ) + L"embarcando "  + strChar + L"lugar.";
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGEmbark.m_iEmbarkType = m_cbEmbarkType.GetCurSel();

        CString strTemp;
        m_ebPosX.GetWindowText(strTemp);
        m_TGEmbark.m_vPos.x = (float)_wtof(strTemp);
        m_ebPosY.GetWindowText(strTemp);
        m_TGEmbark.m_vPos.y = (float)_wtof(strTemp);
        m_ebEmbarkSlot.GetWindowText(strTemp);
        m_TGEmbark.m_iEmbarkSlot = (int)_wtof(strTemp);
        m_TGEmbark.m_bIsRight = m_cbIsRight.GetCurSel();
        m_TGEmbark.m_iEmbarkationCharSlot = m_cbEmbarkationCharSlot.GetCurSel();
        m_TGEmbark.m_strDesc = GetDesc();
    }
}