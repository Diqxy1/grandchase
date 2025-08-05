#include "stdafx.h"
#include "GCVitalityWorldmap.h"

IMPLEMENT_CLASSNAME( KGCVitalityWorldmap );
IMPLEMENT_WND_FACTORY( KGCVitalityWorldmap );
IMPLEMENT_WND_FACTORY_NAME( KGCVitalityWorldmap, "gc_worldmap_vitality" );

KGCVitalityWorldmap::KGCVitalityWorldmap( void )
{
    m_pkWndTooltip = NULL;
    m_pkGuageVitality = NULL;
    m_pkGuageExp = NULL;
    m_pkBtnRecharge = NULL;
    m_pkLevel = NULL;
    m_pkGuageVitalityRect = NULL;
    m_pkGuageExpRect = NULL;
    LINK_CONTROL( "wnd_tooltip", m_pkWndTooltip );
    LINK_CONTROL( "vitality_guage", m_pkGuageVitality );
    LINK_CONTROL( "vitality_guage_rect", m_pkGuageVitalityRect );
    LINK_CONTROL( "exp_guage", m_pkGuageExp );
    LINK_CONTROL( "exp_guage_rect", m_pkGuageExpRect );
    LINK_CONTROL( "btn_recharge", m_pkBtnRecharge );
    LINK_CONTROL( "lv", m_pkLevel );

    m_pkStaticVitality = NULL;
    m_pkStaticExp = NULL;
    LINK_CONTROL( "static_vitality", m_pkStaticVitality );
    LINK_CONTROL( "static_exp", m_pkStaticExp );

    m_pkCharImg = NULL;
    LINK_CONTROL( "char_img", m_pkCharImg );

    char str[MAX_PATH] = {0,};

    for( int i = 0; i < KGCRoomUserSlotS6::EM_LEVEL_NUM; ++i ) {
        m_apkUserLevel[i] = NULL;

        sprintf( str, "level_num%d", i );
        LINK_CONTROL( str,  m_apkUserLevel[i] );
    }

}

KGCVitalityWorldmap::~KGCVitalityWorldmap( void )
{

}

void KGCVitalityWorldmap::OnCreate( void )
{
    m_pkWndTooltip->InitState( true, true, this );
    m_pkGuageVitality->InitState( true, true, this );
    m_pkGuageVitalityRect->InitState( true );
    m_pkGuageExp->InitState( true, true, this );
    m_pkGuageExpRect->InitState( false );
    m_pkBtnRecharge->InitState( true, true, this );
    m_pkLevel->InitState( true, true, this );
    m_pkLevel->SetWindowColor( 0xff88dd00 ); // 풀색

    m_pkStaticVitality->InitState( true );
    m_pkStaticVitality->SetFontSize( 6 );
    m_pkStaticVitality->SetFontOutline( true );
    m_pkStaticVitality->SetAlign( DT_CENTER );

    m_pkStaticExp->InitState( true );
    m_pkStaticExp->SetFontSize( 6 );
    m_pkStaticExp->SetFontOutline( true );
    m_pkStaticExp->SetAlign( DT_CENTER );

    m_pkCharImg->InitState( true, true, this );

    for( int i = 0; i < KGCRoomUserSlotS6::EM_LEVEL_NUM; ++i ) {
        m_apkUserLevel[i]->InitState( true );
        m_apkUserLevel[i]->SetWindowColor( 0xff88dd00 ); // 풀색
    }
}

void KGCVitalityWorldmap::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnRecharge,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRechargeBtn );
}

void KGCVitalityWorldmap::OnClickRechargeBtn()
{
    int nRemainRechargePoint = g_kGlobalValue.GetVitalityRemainRechargePoint() ;

    if( nRemainRechargePoint > 0 ) {
        SCharInfo* pCharInfo = &g_kGlobalValue.m_kUserInfo.GetCurrentChar();

        int nVitality = g_kGlobalValue.GetVitality( pCharInfo->iCharType );
        int nMaxVitality = g_kGlobalValue.GetMaxVitality();

        if( nVitality >= nMaxVitality )
            return;

        int nRechargeable = nMaxVitality - nVitality;

        if( nRechargeable > nRemainRechargePoint )
            nRechargeable = nRemainRechargePoint;

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_VITALITY_STR_4, "i", nRechargeable ), 
            g_pkStrLoader->GetString( STR_VITALITY_STR_5 ), KGCUIScene::GC_MBOX_USE_RECHARGE_VITALITY, 0, 0, false, true);
    }
}

void KGCVitalityWorldmap::SetCurrCharInfo()
{
    SCharInfo* pCharInfo = &g_kGlobalValue.m_kUserInfo.GetCurrentChar();

    if( pCharInfo->iCharType == -1 )
        return;

    // 케릭터 얼굴
    m_pkCharImg->SetChar( pCharInfo->iCharType );

    // 케릭터 레벨
    int nLevel = Exp_2_Level( pCharInfo->biExp );

    if ( (nLevel / 10) > 0 ) {
        SetNumberTex((nLevel / 10) , m_apkUserLevel[0]);
        m_apkUserLevel[0]->ToggleRender( true );
    } else {
        m_apkUserLevel[0]->ToggleRender( false );
    }

    SetNumberTex((nLevel % 10 ), m_apkUserLevel[1]);   

    // 활력
    D3DXVECTOR2 vGaugePos = m_pkGuageVitalityRect->GetFixedWindowLocalPos();
    int nCurrVitality = g_kGlobalValue.GetVitality( pCharInfo->iCharType );
    int nMaxVitality = g_kGlobalValue.GetMaxVitality();

    // 잔여 활력이 MaxVitality 보다 많을 경우 계정 첫 캐릭터 생성용 기준으로 표시한다
    if ( nCurrVitality > nMaxVitality ) {
        nMaxVitality = g_kGlobalValue.m_kVitalityInfo.m_nMaxVitalityForFirstCharacter;
    }

    float fPercent;
    if( nMaxVitality == 0 ) {
        fPercent = 0.0f;
    } else {
        fPercent = static_cast<float>( nCurrVitality ) / static_cast<float>( nMaxVitality );
    }

    if( fPercent > 1.0f )   fPercent = 1.0f;

    DWORD dwLength = static_cast<DWORD>(m_pkGuageVitalityRect->GetWidth() * fPercent );

    m_pkGuageVitality->SetWidth( dwLength );
    m_pkGuageVitality->SetFixedWindowLocalPos(vGaugePos);

    WCHAR strTemp[10];
    swprintf(strTemp, 10, L"%d/%d", nCurrVitality, nMaxVitality );
    m_pkStaticVitality->SetText( strTemp );

    // 경험치
    vGaugePos = m_pkGuageExpRect->GetFixedWindowLocalPos();
    fPercent = GetCurrentExpPercent( pCharInfo->biExp );
    dwLength = static_cast<DWORD>(m_pkGuageExpRect->GetWidth() * fPercent / 100);

    m_pkGuageExp->SetWidth( dwLength );
    m_pkGuageExp->SetFixedWindowLocalPos(vGaugePos);

    swprintf(strTemp, 10, L"%3.1f%%", fPercent);
    m_pkStaticExp->SetText( strTemp );

    // 툴팁
    int nVitalityRemainRechargePoint = g_kGlobalValue.GetVitalityRemainRechargePoint();
    m_pkGuageVitalityRect->SetToolTip( g_pkStrLoader->GetReplacedString( STR_VITALITY_STR_2, "ii", nVitalityRemainRechargePoint, nMaxVitality ) );
    m_pkWndTooltip->SetToolTip(  g_pkStrLoader->GetString( STR_VITALITY_STR_3 ) );

    // 충전버튼
    if( nCurrVitality >= nMaxVitality || nVitalityRemainRechargePoint == 0 ) {
        m_pkBtnRecharge->SetWndMode( D3DWM_LOCK );
        m_pkBtnRecharge->SetToolTip( g_pkStrLoader->GetString( STR_VITALITY_STR_1 ), 0.0f, 0.0f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT );
    } else {
        m_pkBtnRecharge->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnRecharge->SetToolTip( L"" );
    }
}

void KGCVitalityWorldmap::SetNumberTex( IN int index, IN KD3DWnd *pWnd )
{
    if(pWnd == NULL) return;

    int x_index = index % 10 ; 

    float tu = static_cast<float>(x_index) * 17.0f + 281.f;
    float tv = 73.f;
    float tu1 = static_cast<float>(x_index) * 17.0f + 295.f ;
    float tv1 = 87.f;
    D3DXVECTOR4 vec(tu,tv,tu1,tv1);
    ChangeUVtoTexel(vec);

    pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}

void KGCVitalityWorldmap::ChangeUVtoTexel( OUT D3DXVECTOR4& vUV )
{
    float fMagicNumber=0.5f;
    if (vUV.x>vUV.z)
    {
        fMagicNumber*=-1.0f;
    }
    vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
    vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

    fMagicNumber=0.5f;
    if (vUV.y>vUV.w)
    {
        fMagicNumber*=-1.0f;
    }
    vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
    vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

    TEXEL_BOUND(vUV.x);
    TEXEL_BOUND(vUV.y);
    TEXEL_BOUND(vUV.z);
    TEXEL_BOUND(vUV.w);
}
