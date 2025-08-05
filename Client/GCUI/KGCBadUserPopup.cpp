#include "stdafx.h"
#include ".\KGCBadUserPopup.h"
//

//

//
//



IMPLEMENT_CLASSNAME( KGCBadUserInfo );
IMPLEMENT_WND_FACTORY( KGCBadUserInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCBadUserInfo, "gc_baduser_info_popup" );

KGCBadUserInfo::KGCBadUserInfo(void)
:m_pkInfoTitle( NULL )
,m_pkInfoDesc( NULL )
,m_pkBar_Extend( NULL )
,m_pkInfoDesc_Extend( NULL )
,m_pkBar_Extend_Time( NULL )
,m_pkTimeInfo( NULL )
,m_pkBack( NULL )
{
    LINK_CONTROL( "info_static",        m_pkInfoTitle );
    LINK_CONTROL( "info_desc",          m_pkInfoDesc );

    LINK_CONTROL( "bar_extend",         m_pkBar_Extend );
    LINK_CONTROL( "info_desc_extend",   m_pkInfoDesc_Extend );

    LINK_CONTROL( "bar_extend_time",    m_pkBar_Extend_Time );
    LINK_CONTROL( "time_extend",        m_pkTimeInfo );
    LINK_CONTROL( "item_decs_back",     m_pkBack );
}

KGCBadUserInfo::~KGCBadUserInfo(void)
{
}

void KGCBadUserInfo::OnCreateComplete( void )
{
    m_pkInfoTitle->InitState( true );
    m_pkInfoTitle->SetAlign( DT_CENTER );
    
    m_pkInfoTitle->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkInfoTitle->SetFontColor( D3DCOLOR_XRGB( 255, 255, 0 ) );

    m_pkInfoDesc->InitState( true );
    m_pkInfoDesc->SetMultiLine( true );
    m_pkInfoDesc->SetAlign( DT_CENTER );
    m_pkInfoDesc->SetLineSpace( 1.5f );

    m_pkBar_Extend->InitState( false );

    m_pkInfoDesc_Extend->InitState( false );
    m_pkInfoDesc_Extend->SetMultiLine( true );
    m_pkInfoDesc_Extend->SetLineSpace( 1.5f );

    m_pkBar_Extend_Time->InitState( false );

    m_pkTimeInfo->InitState( false );

    m_pkBack->InitState( false );

    //SetInfoText( Result_BadUser_Info );
}

void KGCBadUserInfo::SetInfoText( KEVENT_BAD_USER_INFO_NOT kRecv )
{
    /*
    m_cLastGrade, m_cCurrentGrade 설명
    0 : 일반 유저
    1, 2 : 랭커 (랭킹등급이 높은 일반 유저 )
    3 : 운영자
    -1 : 경고
    -2 : 불량 유저
    -3 : 블럭 유저
    */

    // 남은 시간
    CTime time ( kRecv.m_tmLimit );
    CTimeSpan tmReMaind = ( time - g_kGlobalValue.m_tmServerTime );
    std::wstring strTime = L"";
    if ( tmReMaind.GetDays() != 0 )
        //strTime = tmReMaind.Format( g_pkStrLoader->GetReplacedString( STR_ID_TIME_DDMMSS, "sss", L"%D", "%H", "%M" ).c_str() ).GetBuffer();
        strTime = g_pkStrLoader->GetReplacedString( STR_ID_TIME_DDMMSS, "iii", static_cast<int>(tmReMaind.GetDays()), static_cast<int>(tmReMaind.GetHours()), static_cast<int>(tmReMaind.GetMinutes()) );
    else
        //strTime = tmReMaind.Format( g_pkStrLoader->GetReplacedString( STR_ID_TIME_MMSS, "ss", "%H", "%M" ).c_str() ).GetBuffer();
        strTime = g_pkStrLoader->GetReplacedString( STR_ID_TIME_MMSS, "ii", static_cast<int>(tmReMaind.GetHours()), static_cast<int>(tmReMaind.GetMinutes()) );

    switch ( static_cast<int>( kRecv.m_cCurrentGrade ) )
    {
    case -1:
        {
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_ALERT0 ) );         // 1차경고
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_DESC0, "i", 1 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC1 ) +
                g_pkStrLoader->GetString( STR_ID_BADUSER_DESC4 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkTimeInfo->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_RELEASE1, "l", strTime ) );
        }
        break;
    case -2:
        {
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_ALERT1 ) );         // 2차경고
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_DESC0, "i", 2 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC2 ) + 
                g_pkStrLoader->GetString( STR_ID_BADUSER_DESC4 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkTimeInfo->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_RELEASE1, "l", strTime ) );
        }
        break;
    }
    
    /*
    m_cCause 변수 설명
    0 : 해당 없음.
    1 : 욕설
    2 : 도배
    3 : 허위 신고
    4 : 해킹툴 사용
    5 : 운영자 사칭
    6 : 캐시 사기
    */
    switch ( static_cast<int>( kRecv.m_cCause ) )
    {
    case 1:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON0 ) );
        break;
    case 2:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON1 ) );
        break;
    case 3:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON2 ) );
        break;
    case 4:
    case 5:
    case 6:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON4 ) );
        break;
    }

    

    // 마우스 오버시 사용하는 UI모음!
    m_pkBar_Extend->ToggleRender( true );
    m_pkInfoDesc_Extend->ToggleRender( true );
    m_pkBar_Extend_Time->InitState( true );
    m_pkTimeInfo->InitState( true );
    m_pkBack->ToggleRender( true );
}
