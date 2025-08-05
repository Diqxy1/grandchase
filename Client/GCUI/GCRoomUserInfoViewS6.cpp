#include "stdafx.h"
#include "GCRoomUserInfoViewS6.h"

IMPLEMENT_CLASSNAME( KGCRoomUserInfoViewS6 );
IMPLEMENT_WND_FACTORY( KGCRoomUserInfoViewS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomUserInfoViewS6, "gc_room_user_info_view_s6" );

KGCRoomUserInfoViewS6::KGCRoomUserInfoViewS6( void )
: m_pkID( NULL ) 
, m_pkLevel( NULL ) 
, m_pkWin( NULL ) 
, m_pkGuildName( NULL ) 
, m_pkExpGaugeRect( NULL ) 
, m_pkExpGauge( NULL ) 
, m_pkStaticExpValue( NULL ) 
, m_pkGuildIconDrawRect( NULL ) 
, m_pkTexGuildMark( NULL ) 
, m_pkClassName( NULL ) 
, m_pkJobName( NULL )
, m_pkCoupleName( NULL )
, m_pkBonusText( NULL ) 
{
    for(int i = 0 ; i < GC_GRADE_NUM; ++i)
    {
        m_pkGrade[i] = NULL;
    }

    LINK_CONTROL( "id_static",				m_pkID);
    LINK_CONTROL( "level_static",			m_pkLevel);

    LINK_CONTROL( "class_name_static",			m_pkClassName);
    LINK_CONTROL( "job_name_static",			m_pkJobName);
    LINK_CONTROL( "couple_name_static",			m_pkCoupleName);
    LINK_CONTROL( "bonus_static",			    m_pkBonusText);
    

    LINK_CONTROL( "win_static",				m_pkWin);

    LINK_CONTROL( "guildname_static",		m_pkGuildName);

    LINK_CONTROL( "exp_gauge_rect", m_pkExpGaugeRect );
    LINK_CONTROL( "exp_gauge", m_pkExpGauge );
    LINK_CONTROL( "exp_value", m_pkStaticExpValue );

	LINK_CONTROL( "guild_icon_draw_rect", m_pkGuildIconDrawRect );


    char strCharName[MAX_PATH];
    for( int i = 0; i < GC_GRADE_NUM; i++ ) {
        sprintf(strCharName,"player_grade%d",i);
        LINK_CONTROL( strCharName, m_pkGrade[i] );
    }
}

KGCRoomUserInfoViewS6::~KGCRoomUserInfoViewS6( void )
{
}

void KGCRoomUserInfoViewS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCRoomUserInfoViewS6::OnCreate( void )
{
    m_pkLevel->InitState( true, false, this );
    m_pkID->InitState( true, false, this );
    m_pkGuildName->InitState( true, false, this );
    m_pkWin->InitState( true, false, this );

    m_pkClassName->InitState( true, false, this );
    m_pkJobName->InitState( true, false, this );
    m_pkCoupleName->InitState( true, false, this );    
    m_pkBonusText->InitState( true, false, this );

    for(int i = 0; i < GC_GRADE_NUM; i++)
    {
        m_pkGrade[i]->InitState( true, false, this );
    }


    m_pkLevel->SetFontColor( 0xFFFFFFFF );
    m_pkID->SetFontColor( 0xFFFFFFFF );
    m_pkGuildName->SetFontColor( 0xFFFFFFFF );
    m_pkClassName->SetFontColor( 0xFFFFFFFF );
    m_pkJobName->SetFontColor( 0xFFFFFFFF );
    m_pkCoupleName->SetFontColor( 0xFFFFFFFF );
    m_pkBonusText->SetFontColor( 0xFFFFFFFF );

    m_pkWin->SetFontColor( 0xFFFFFFFF );

#if defined( NATION_KOREA )
    m_pkID->SetFontSize( 14 );
    m_pkLevel->SetFontSize( 14 );
    m_pkClassName->SetFontSize( 14 );
    m_pkJobName->SetFontSize( 14 );
#endif

    m_pkExpGaugeRect->InitState(false);
    m_pkExpGauge->InitState(true);
    m_pkStaticExpValue->InitState(true);
    m_pkStaticExpValue->SetFontSize(9);
    m_pkStaticExpValue->SetAlign(DT_CENTER);
    m_pkStaticExpValue->SetFontOutline( true );

    m_pkGuildIconDrawRect->InitState(false);
}

void KGCRoomUserInfoViewS6::FrameMoveInEnabledState(void)
{
}

void KGCRoomUserInfoViewS6::SetUserInfo( PLAYER* pPlayer )
{
    UpdateUserInfo( pPlayer );
}

void KGCRoomUserInfoViewS6::UpdateUserInfo( PLAYER* pPlayer )
{
    if ( !pPlayer ) 
    {
        pPlayer = &g_MyD3D->m_TempPlayer;
    }

	std::wostringstream strWin;
	std::wostringstream strmLV;
	int iWinRate = 0;

    GCEXPTYPE biExp = pPlayer->m_kUserInfo.GetCurrentChar().biExp;
	int iLevel = static_cast<int>(Exp_2_Level( biExp ));
    int iWin = 0;
    int iLose = 0;


	iWin = pPlayer->m_kUserInfo.GetCurrentChar().iWin;
	iLose = pPlayer->m_kUserInfo.GetCurrentChar().iLose;

	
   
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
    {
        iWin = pPlayer->m_kUserInfo.iIndigoWin;
        iLose = pPlayer->m_kUserInfo.iIndigoLose;
    }

	if( iWin + iLose > 0 )
		iWinRate = ( iWin * 100 ) / ( iWin + iLose );

	strWin << iWin << g_pkStrLoader->GetString( STR_ID_WIN ).c_str();
    strWin << L" " << iLose << g_pkStrLoader->GetString( STR_ID_LOSE ).c_str();
	strWin << L" " << g_pkStrLoader->GetString( STR_ID_WINRATE ).c_str() << iWinRate << '%';

    std::wstring tempSlInfoViewName = L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx";

	m_pkID->SetText( tempSlInfoViewName );

	m_pkLevel->SetText( iLevel );
    m_pkClassName->SetText( GCFUNC::GetCharName(pPlayer->GetCurrentChar().iCharType) );
    m_pkJobName->SetText( g_kGlobalValue.GetJopName(pPlayer->GetCurrentChar().iCharType , pPlayer->GetCurrentChar().iPromotionLevel) );


	KInDoorUserInfo kIndoorUserInfo;
	pPlayer->m_kUserInfo.SetIndoorUserInfo(&kIndoorUserInfo);

    DWORD dwUID = pPlayer->m_kUserInfo.dwUID;
    bool bSolo = SiKGCCoupleSystem()->Are_you_Solo( dwUID );

    if ( !bSolo )
    {
        m_pkCoupleName->SetText( L"#c" + SiKGCCoupleSystem()->GetNickColor_of_your_honey(dwUID) + SiKGCCoupleSystem()->GetNickName_of_your_honey(dwUID) + L"#cx" );
    }
    else
    {
        m_pkCoupleName->SetText( L"" );
    }

	SAFE_RELEASE(m_pkTexGuildMark);
    if( kIndoorUserInfo.m_dwGuildUID == 0)
	{
		m_pkGuildName->SetText(g_pkStrLoader->GetString( STR_ID_NOT_JOIN_GUILD ));		
	}
	else
	{
		m_pkGuildName->ToggleRender(true);
		m_pkGuildName->SetText( kIndoorUserInfo.m_strGuildName );	

		m_pkTexGuildMark = SiKGCIconImageManager()->GetTexture(kIndoorUserInfo.m_strMarkName);
		if (m_pkTexGuildMark == NULL)
		{
			SiKGCIconImageManager()->LoadIconImageFromHTTP(kIndoorUserInfo.m_strMarkName);
			m_pkTexGuildMark = SiKGCIconImageManager()->GetTexture(kIndoorUserInfo.m_strMarkName); 
		}
	}

	m_pkWin->SetText(strWin.str());

	UpdateCharMark( pPlayer );
	UpdateGrade( pPlayer );

	float fPercent = GetCurrentExpPercent( pPlayer->GetCurrentChar().biExp );
	DWORD dwPercentWidth = static_cast<DWORD>(m_pkExpGaugeRect->GetWidth() * fPercent / 100);
	D3DXVECTOR2 vGaugePos = m_pkExpGaugeRect->GetFixedWindowLocalPos();

	m_pkExpGauge->SetWidth(dwPercentWidth);
	m_pkExpGauge->SetFixedWindowLocalPos(vGaugePos);

	WCHAR strTemp[10];
	swprintf(strTemp, 10, L"%3.1f%%", fPercent);
	m_pkStaticExpValue->SetText(strTemp);

    std::wostringstream strBonus;
    
    //  pc방 체크
    if ( pPlayer->m_kUserInfo.cIsNetmarblePCRoom == 1 )
    {
        strBonus << g_pkStrLoader->GetString( STR_ID_ROOM_BONUS_PCBANG ).c_str() << L"  ";
    }
    
    if ( pPlayer->m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB ) 
    {
        strBonus << g_pkStrLoader->GetString( STR_ID_ROOM_BONUS_GCCLUB ).c_str() << L"  ";
    }

    if ( pPlayer->m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG ) 
    {
        strBonus << g_pkStrLoader->GetString( STR_ID_ROOM_BONUS_GCBLOG ).c_str();
    }

    if ( pPlayer->m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM )
    {
        strBonus << g_pkStrLoader->GetString( STR_ID_ROOM_BONUS_GCBLOG ).c_str();
    }

    m_pkBonusText->SetText( strBonus.str() );
}

void KGCRoomUserInfoViewS6::UpdateCharMark( PLAYER* pPlayer )
{
}

void KGCRoomUserInfoViewS6::UpdateGrade( PLAYER* pPlayer )
{

    g_pkUIHelper->SetCharacterGrade( m_pkGrade, pPlayer->m_kUserInfo.GetCharacterGrade() );

}

void KGCRoomUserInfoViewS6::PostChildDraw( void )
{
    if (m_pkTexGuildMark == NULL)
        return;

    D3DXVECTOR2 vPos = GetRelocatedWindowPos();
    //vPos += m_pkGuildMarkRect->GetFixedWindowLocalPos();
    vPos.x += m_pkGuildIconDrawRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
    vPos.y += m_pkGuildIconDrawRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;

    g_pGCDeviceManager2->DrawInScreen(m_pkTexGuildMark,
        vPos.x , vPos.y, 
        vPos.x + m_pkGuildIconDrawRect->GetWidth() * m_fWindowScaleX, vPos.y + m_pkGuildIconDrawRect->GetHeight() * m_fWindowScaleY,
        0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
}

