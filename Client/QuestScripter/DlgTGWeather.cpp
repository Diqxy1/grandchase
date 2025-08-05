// DlgTGWeather.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGWeather.h"
#include ".\dlgtgweather.h"

// CDlgTGWeather 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGWeather, CDlgTGBase)
CDlgTGWeather::CDlgTGWeather(CWnd* pParent /*=NULL*/)
{
}

CDlgTGWeather::~CDlgTGWeather()
{
}

void CDlgTGWeather::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_WEATHERTYPE, m_cbType);
    DDX_Control(pDX, IDC_EDIT_WEATHER_POS_X, m_ebPosX);
    DDX_Control(pDX, IDC_EDIT_WEATHER_POS_Y, m_ebPosY);
    DDX_Control(pDX, IDC_EDIT_WEATHER_POS_WIDTH, m_ebPosWidth);
    DDX_Control(pDX, IDC_EDIT_WEATHER_POS_HEIGHT, m_ebPosHeight);
    DDX_Control(pDX, IDC_EDIT_WEATHER_EFF_SEQ, m_ebEffSequence);
    DDX_Control(pDX, IDC_EDIT_WEATHER_EFF_X, m_ebEffPosX);
    DDX_Control(pDX, IDC_EDIT_WEATHER_EFF_Y, m_ebEffPosY);
    DDX_Control(pDX, IDC_LIST_EFF, m_lbParticle);
    DDX_Control(pDX, IDC_EDIT_WEATHER_WIND_SPEED_X, m_ebWindSpeedX);
    DDX_Control(pDX, IDC_EDIT_WEATHER_WIND_SPEED_Y, m_ebWindSpeedY);
    DDX_Control(pDX, IDC_EDIT_WEATHER_WIND_JUMPRADIO, m_ebWindJumpRadio);
    DDX_Control(pDX, IDC_GROUP_WIND, m_stWindPropertyBox);
    DDX_Control(pDX, IDC_STATIC_WIND_X, m_stWindSpeedX);
    DDX_Control(pDX, IDC_STATIC_WIND_Y, m_stWindSpeedY);
    DDX_Control(pDX, IDC_STATIC_WIND_JUMPRADIO, m_stWindJumpRadio);

    m_cbType.ResetContent();
    m_cbType.InsertString(CWeather::WEATHERTYPE_NONE, L"효과없음");
    m_cbType.InsertString(CWeather::WEATHERTYPE_WIND, L"바람저항");
    if( m_cbType.GetCount() > 1 ) m_cbType.SetCurSel(1);

    m_ebPosX.SetWindowText(L"");
    m_ebPosY.SetWindowText(L"");
    m_ebPosWidth.SetWindowText(L"");
    m_ebPosHeight.SetWindowText(L"");
    m_ebEffPosX.SetWindowText(L"");
    m_ebEffPosY.SetWindowText(L"");
    m_ebEffSequence.SetWindowText(L"");
    m_lbParticle.ResetContent();
    m_ebWindSpeedX.SetWindowText(L"");
    m_ebWindSpeedY.SetWindowText(L"");
    m_ebWindJumpRadio.SetWindowText(L"");
}


BEGIN_MESSAGE_MAP(CDlgTGWeather, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_ADD_EFF, &CDlgTGWeather::OnBnClickedAddParticle)
    ON_CBN_SELCHANGE(IDC_COMBO_WEATHERTYPE, &CDlgTGWeather::OnCbnSelchangeComboWeatherType)
    ON_BN_CLICKED(IDC_BUTTON_ALL_DEL, &CDlgTGWeather::OnBnClickedButtonAllDel)
END_MESSAGE_MAP()


// CDlgTGWeather 메시지 처리기입니다.

void CDlgTGWeather::Refresh(void)
{
    if( m_cbType.GetCount() > m_TGWeather.m_iType ) m_cbType.SetCurSel( m_TGWeather.m_iType );

    CString strTemp;
    strTemp.Format(L"%f", m_TGWeather.m_vRect.x);
    m_ebPosX.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWeather.m_vRect.y);
    m_ebPosY.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWeather.m_vRect.z);
    m_ebPosWidth.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWeather.m_vRect.w);
    m_ebPosHeight.SetWindowText( strTemp );
    strTemp.Format(L"%f", 0.0);
    m_ebEffPosX.SetWindowText( strTemp );
    strTemp.Format(L"%f", 0.0);
    m_ebEffPosY.SetWindowText( strTemp );
    strTemp.Format(L"%s", L"");
    m_ebEffSequence.SetWindowText( strTemp );

    strTemp.Format(L"%f", m_TGWeather.m_vWindProperty.m_vSpeed.x);
    m_ebWindSpeedX.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWeather.m_vWindProperty.m_vSpeed.y);
    m_ebWindSpeedY.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWeather.m_vWindProperty.m_fJumpRadio);
    m_ebWindJumpRadio.SetWindowText( strTemp );

    SetItemListBox();
    ShowWeatherProperty( m_cbType.GetCurSel() );
}

void CDlgTGWeather::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strType = L"맑은";
        if( m_cbType.GetCount() > m_cbType.GetCurSel() ) m_cbType.GetLBText(m_cbType.GetCurSel(), strType);
        m_strDesc = MiddleBraceSidebyString( strType ) + L"날씨를 설치한다.";
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGWeather.m_iType = m_cbType.GetCurSel();

        CString strTemp;
        m_ebPosX.GetWindowText(strTemp);
        m_TGWeather.m_vRect.x = (float)_wtof(strTemp);
        m_ebPosY.GetWindowText(strTemp);
        m_TGWeather.m_vRect.y = (float)_wtof(strTemp);
        m_ebPosWidth.GetWindowText(strTemp);
        m_TGWeather.m_vRect.z = (float)_wtof(strTemp);
        m_ebPosHeight.GetWindowText(strTemp);
        m_TGWeather.m_vRect.w = (float)_wtof(strTemp);

        m_ebWindSpeedX.GetWindowText(strTemp);
        m_TGWeather.m_vWindProperty.m_vSpeed.x = (float)_wtof(strTemp);
        m_ebWindSpeedY.GetWindowText(strTemp);
        m_TGWeather.m_vWindProperty.m_vSpeed.y = (float)_wtof(strTemp);
        m_ebWindJumpRadio.GetWindowText(strTemp);
        m_TGWeather.m_vWindProperty.m_fJumpRadio = (float)_wtof(strTemp);

        m_TGWeather.m_strDesc = GetDesc();
    }
}

void CDlgTGWeather::OnBnClickedAddParticle()
{
    CString strX, strY, strSeq;
    m_ebEffPosX.GetWindowText(strX);
    m_ebEffPosY.GetWindowText(strY);
    m_ebEffSequence.GetWindowText(strSeq);

    if( 0 == strSeq.GetLength() || 0 == strX.GetLength() || 0 == strY.GetLength() )
    {
        MessageBoxA(NULL, "파티클 정보가 입력되지 않았습니다.", "InsertError", MB_OK);
        return;
    }

    PARTICLE_WEATHER particle;
    particle.m_strSeq = GCUTIL_STR::GCStrWideToChar( strSeq.GetString() );
    particle.m_vPos.x = (float)_wtof(strX);
    particle.m_vPos.y = (float)_wtof(strY);
    m_TGWeather.m_vecParticle.push_back( particle );

    SetItemListBox();
}

void CDlgTGWeather::OnCbnSelchangeComboWeatherType()
{
    ShowWeatherProperty( m_cbType.GetCurSel() );
}

void CDlgTGWeather::SetItemListBox()
{
    CString strTemp;
    m_lbParticle.ResetContent();
    std::vector<PARTICLE_WEATHER>::iterator vecIter = m_TGWeather.m_vecParticle.begin();
    for(; vecIter != m_TGWeather.m_vecParticle.end(); ++vecIter)
    {
        strTemp.Format(L"%s, %.4f, %.4f", GCUTIL_STR::GCStrCharToWide(vecIter->m_strSeq.c_str()), vecIter->m_vPos.x, vecIter->m_vPos.y);
        m_lbParticle.AddString( strTemp );
    }
}

void CDlgTGWeather::CloseWeatherProperty()
{
    m_ebWindSpeedX.ShowWindow( false );
    m_ebWindSpeedY.ShowWindow( false );
    m_ebWindJumpRadio.ShowWindow( false );
    m_stWindPropertyBox.ShowWindow( false );
    m_stWindSpeedX.ShowWindow( false );
    m_stWindSpeedY.ShowWindow( false );
    m_stWindJumpRadio.ShowWindow( false );
}

void CDlgTGWeather::ShowWeatherProperty( int iWeatherType )
{
    CloseWeatherProperty();
    switch( iWeatherType )
    {
    case CWeather::WEATHERTYPE_WIND:
        m_ebWindSpeedX.ShowWindow( true );
        m_ebWindSpeedY.ShowWindow( true );
        m_ebWindJumpRadio.ShowWindow( true );
        m_stWindPropertyBox.ShowWindow( true );
        m_stWindSpeedX.ShowWindow( true );
        m_stWindSpeedY.ShowWindow( true );
        m_stWindJumpRadio.ShowWindow( true );
        break;
    }
}
void CDlgTGWeather::OnBnClickedButtonAllDel()
{
    m_lbParticle.ResetContent();
    m_TGWeather.m_vecParticle.clear();
}
