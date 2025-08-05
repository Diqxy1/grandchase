#include "stdafx.h"
#include "KGCRoomBar.h"

//
#include "KGCCategoryBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "gcui/KGCCountryGroup.h"

IMPLEMENT_CLASSNAME( KGCRoomBar );
IMPLEMENT_WND_FACTORY( KGCRoomBar );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomBar, "gc_room_bar" );

KGCRoomBar::KGCRoomBar(void)
:	m_pkPremiumBlog(NULL)
{
    m_pkRoomNumber  = NULL;
    m_pkRoomName    = NULL;
    m_pkNumOfPeople = NULL;

    m_pkKeyIcon     = NULL;
    m_pkStatePlay   = NULL;
    m_pkStateWait   = NULL;
    m_pkStateFull   = NULL;

    m_pkStateJoinable = NULL;

    m_pkCategoryBox = NULL;

    m_pkNoEnter = NULL;
    m_pkGuildBorderR = NULL;
    m_pkGuildBorderL = NULL;
    
    m_pkQuestionIconR = NULL;
    m_pkQuestionIconL = NULL;
    
    m_pkGuildmarkIconR = NULL;
    m_pkGuildmarkIconL = NULL;
    m_pkVersusIcon = NULL;

    m_pkGuildImage_R = NULL;
    m_pkGuildImage_L = NULL;
    m_pkCountryGroup = NULL;

    m_pkPremiumGoldStage = NULL;

    LINK_CONTROL( "room_number",        m_pkRoomNumber );
    LINK_CONTROL( "room_name",          m_pkRoomName );
    LINK_CONTROL( "number_of_people",   m_pkNumOfPeople );

    LINK_CONTROL( "key_icon",           m_pkKeyIcon );
    LINK_CONTROL( "state_play",         m_pkStatePlay );
    LINK_CONTROL( "state_wait",         m_pkStateWait );
    LINK_CONTROL( "state_full",         m_pkStateFull );

    LINK_CONTROL( "state_play_joinable",         m_pkStateJoinable );

    LINK_CONTROL( "category_box",       m_pkCategoryBox );

    m_dwPremium = KPremiumInfo::TYPE_NORMAL;
    m_pkPremiumTitle = NULL;

	LINK_CONTROL( "room_premium_gc_club", m_pkPremiumTitle );
	LINK_CONTROL( "room_premium_gc_blog", m_pkPremiumBlog );   
    LINK_CONTROL( "room_premium_gold_stage", m_pkPremiumGoldStage );

    LINK_CONTROL( "state_no_enter" , m_pkNoEnter );
    LINK_CONTROL( "guild_icon_border_right" , m_pkGuildBorderR );
    LINK_CONTROL( "guild_icon_border_left" , m_pkGuildBorderL );
    LINK_CONTROL( "question_icon_r" , m_pkQuestionIconR );
    LINK_CONTROL( "question_icon_l", m_pkQuestionIconL );
    LINK_CONTROL( "guildmark_icon_left" , m_pkGuildmarkIconL );
    LINK_CONTROL( "guildmark_icon_right" , m_pkGuildmarkIconR );
    LINK_CONTROL( "versus_icon" , m_pkVersusIcon );
    LINK_CONTROL( "guildmark_image_right", m_pkGuildImage_R );
    LINK_CONTROL( "guildmark_image_left", m_pkGuildImage_L );

    LINK_CONTROL( "country_group", m_pkCountryGroup );
} 

KGCRoomBar::~KGCRoomBar(void)
{
}

void KGCRoomBar::OnCreate()
{
    m_pkRoomNumber->InitState( true );
    m_pkRoomName->InitState( true );
    m_pkNumOfPeople->InitState( true );

    m_pkKeyIcon->InitState( true );
    m_pkStatePlay->InitState( true );
    m_pkStateWait->InitState( true );
    m_pkStateFull->InitState( true );
    m_pkCategoryBox->InitState( true );
    if( m_pkStateJoinable )
    {
        m_pkStateJoinable->InitState( true );
    }

    if ( m_pkNoEnter )
        m_pkNoEnter->InitState( true );
    if ( m_pkGuildBorderR )
        m_pkGuildBorderR->InitState( true );
    if ( m_pkGuildBorderL )
        m_pkGuildBorderL->InitState( true );
    if ( m_pkQuestionIconR )
        m_pkQuestionIconR->InitState( true );
    if ( m_pkQuestionIconL )
        m_pkQuestionIconL->InitState( true );
    if ( m_pkGuildmarkIconR )
        m_pkGuildmarkIconR->InitState( true );
    if ( m_pkGuildmarkIconL )
        m_pkGuildmarkIconL->InitState( true );
    if ( m_pkVersusIcon )
        m_pkVersusIcon->InitState( true );
    if ( m_pkGuildImage_R )
        m_pkGuildImage_R->InitState( true );
    if ( m_pkGuildImage_L )
        m_pkGuildImage_L->InitState( true );


    m_pkRoomNumber->SetAlign( DT_CENTER );
    m_pkNumOfPeople->SetAlign( DT_CENTER );

    m_pkRoomNumber->SetFontOutline( true );
    m_pkRoomName->SetFontOutline( true );
    m_pkNumOfPeople->SetFontOutline( true );   

    if( m_pkCountryGroup != NULL )
        m_pkCountryGroup->InitState(true);

    if( m_pkPremiumGoldStage )
        m_pkPremiumGoldStage->InitState( false );

}
void KGCRoomBar::TurnOnCategoryBox( bool m_bUse)
{
    if ( m_pkCategoryBox )
        m_pkCategoryBox->ToggleRender( m_bUse );
    if ( m_pkNoEnter )
        m_pkNoEnter->ToggleRender( !m_bUse ? true : false );
    if ( m_pkGuildBorderR )
        m_pkGuildBorderR->ToggleRender( !m_bUse ? true : false );
    if ( m_pkGuildBorderL )
        m_pkGuildBorderL->ToggleRender( !m_bUse ? true : false );
    if ( m_pkVersusIcon )
        m_pkVersusIcon->ToggleRender( !m_bUse ? true : false );
    if ( m_pkGuildImage_R )
        m_pkGuildImage_R->ToggleRender( !m_bUse ? true : false );
    if ( m_pkGuildImage_L )
        m_pkGuildImage_L->ToggleRender( !m_bUse ? true : false );

    //이것들은 기본적으로 꺼준다.
    if ( m_pkQuestionIconR )
        m_pkQuestionIconR->ToggleRender( false );
    if ( m_pkQuestionIconL )
        m_pkQuestionIconL->ToggleRender( false );
    if ( m_pkGuildmarkIconR )
        m_pkGuildmarkIconR->ToggleRender( false );
    if ( m_pkGuildmarkIconL )
        m_pkGuildmarkIconL->ToggleRender( false );
}

void KGCRoomBar::FrameMoveInEnabledState()
{

}

void KGCRoomBar::ActionPerformed( const KActionEvent& event )
{
}

void KGCRoomBar::SetRoomInfo( KSimpleRoomInfo kRoomInfo )
{
    m_kRoomInfo = kRoomInfo;

    m_pkRoomName->SetText( m_kRoomInfo.m_strRoomName );

    WCHAR strText[20];
    swprintf( strText, 20, L"%d", m_kRoomInfo.m_usRoomID + 1 );
    m_pkRoomNumber->SetText( strText );

	if( m_kRoomInfo.m_bObserverInRoom )
	{
		m_kRoomInfo.m_usMaxUsers--;
		m_kRoomInfo.m_usUsers--;
	}

    swprintf( strText, 20, L"%d/%d", m_kRoomInfo.m_usUsers, m_kRoomInfo.m_usMaxUsers );
    m_pkNumOfPeople->SetText( strText );

    m_pkKeyIcon->ToggleRender( !m_kRoomInfo.m_bPublic );
    // 플레이 중인가?
    m_pkStatePlay->ToggleRender( m_kRoomInfo.m_bPlaying);
    // 플레이 중이 아니어야 하고.. 최대 유져보다 적은 유져가 들어 있으면..
    m_pkStateWait->ToggleRender( !m_kRoomInfo.m_bPlaying && m_kRoomInfo.m_usUsers < m_kRoomInfo.m_usMaxUsers  );
    // 플레이 중이 아니어야 하고.. 최대 유져보다 이상 유져가 들어 있으면..
    m_pkStateFull->ToggleRender( !m_kRoomInfo.m_bPlaying && m_kRoomInfo.m_usUsers >= m_kRoomInfo.m_usMaxUsers );    

    if( m_pkCountryGroup != NULL )
        m_pkCountryGroup->SetCountryGroup(kRoomInfo.m_wstrCCode, true, STR_ID_PLAY_SAME_COUNTRY_MESSAGE0, ED3DWND_TOOLTIP_ALIGN_RIGHT);


#if defined ( GUILD_RENEW_S4 )

    //입장불가인 방은. false 로 세팅해서 보내줍니다..  기본적으로 true
    if(g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD )){
        if( !m_pkStateFull->IsRenderOn())
        {
            m_pkStateWait->ToggleRender( m_kRoomInfo.m_bGuildPermitEnter && m_pkStateWait->IsRenderOn() );
            if ( m_pkNoEnter )
                m_pkNoEnter->ToggleRender( !m_kRoomInfo.m_bGuildPermitEnter );

            if ( !m_kRoomInfo.m_bGuildPermitEnter )
            {
                m_pkStateWait->ToggleRender( false );
                m_pkStatePlay->ToggleRender( false );
                m_pkStateFull->ToggleRender( false );    
            }
        }
        SetGuildMarkTexture(m_kRoomInfo.m_pairGuildMarkName, m_kRoomInfo.m_pairGuildName, m_kRoomInfo.m_pairBattlePoint );
    }
#endif


    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
        m_pkCategoryBox->UpdateLevel( m_kRoomInfo.m_nDifficulty, m_kRoomInfo.m_bClearLastDifficult );
    else
        m_pkCategoryBox->UpdateCategory( m_kRoomInfo.m_cGameCategory );

    if( m_pkStateJoinable != NULL )
    {
        m_pkStateJoinable->ToggleRender( false );
    }

    if( m_pkStateJoinable != NULL )
    {
        if( m_pkStatePlay->IsRenderOn() && m_pkStateJoinable )
            //게임중을 보여주고 있는데 데쓰매치면서 게임에 참가할 수 있는 경우 joinable를 render on!
            if( m_pkStatePlay->IsRenderOn() && m_pkStateJoinable && m_kRoomInfo.m_bDeathMatchIntrudable )
            {
                if ( m_kRoomInfo.m_cGameCategory == GC_GMC_DEATHMATCH || 
                     m_kRoomInfo.m_cGameCategory == GC_GMC_FATAL_DEATHMATCH )

                m_pkStatePlay->ToggleRender( false );
                m_pkStateJoinable->ToggleRender( true );
            }
            else
            {
                m_pkStateJoinable->ToggleRender( false );
            }
    }
}


void KGCRoomBar::SetPremium()
{
    // [8/10/2007 breadceo] 다 끄고
    if( m_pkPremiumTitle )
	    m_pkPremiumTitle->ToggleRender( false );
	if( m_pkPremiumBlog )
		m_pkPremiumBlog->ToggleRender( false );
    if( m_pkPremiumGoldStage )
        m_pkPremiumGoldStage->ToggleRender( false );

    D3DXVECTOR2 vPos;
    vPos = m_pkPremiumTitle->GetCurrentWindowLocalPos( );
    bool bCheckGuildServer = g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD );

    bool bGoldStage = false;
    if( (m_dwPremium & KPremiumInfo::TYPE_GOLDSTAGE) &&
        KGCRoomManager::NOT_ENTER_CONDITION != SiKGCRoomManager()->CheckTicketToDungeonEnter( false, m_kRoomInfo.m_nGameMode, &bGoldStage ) &&
        true == bGoldStage )
    {
        if( m_pkPremiumGoldStage )
        {
            m_pkPremiumGoldStage->ToggleRender( true );
        }
    }
	else if( m_dwPremium & KPremiumInfo::TYPE_GCCLUB )
	{
        if( m_pkPremiumTitle )
        {
            if(bCheckGuildServer)
                m_pkPremiumTitle->SetWindowPosX(vPos.x - 36);
            m_pkPremiumTitle->ToggleRender( true );
        }
    }
    else if( m_dwPremium & KPremiumInfo::TYPE_GCBLOG )
    {   
        //길드 서버일때 x:271위치에 blog와 club을 표시해 준다. Nomal X pos : 307, guildwar X pos : 271
        if(m_pkPremiumBlog) 
        {
            if(bCheckGuildServer)
                m_pkPremiumBlog->SetWindowPosX( vPos.x - 36);
            m_pkPremiumBlog->ToggleRender( true );            
        }
    }
}

void KGCRoomBar::SetGuildMarkTexture(std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_, std::pair< DWORD, DWORD > m_pairBattlePoint_ )
{
    SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.first);	
    SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.second);	

    if ( prGuildMarkName_.first != L""  && prGuildMarkName_.second !=L"" )
    {
        m_pkGuildImage_L->SetTextureName(prGuildMarkName_.first);
        m_pkGuildImage_R->SetTextureName(prGuildMarkName_.second);
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.first.c_str(), m_pairBattlePoint_.first );
        m_pkGuildImage_L->SetToolTip( szTemp );
        wsprintf( szTemp , L"%s\n%d", prGuildName_.second.c_str(), m_pairBattlePoint_.second);
        m_pkGuildImage_R->SetToolTip( szTemp );
        CheckGuildMark( prGuildMarkName_ , prGuildName_ , m_pairBattlePoint_ );
    }
    else if ( prGuildName_.second == L"" &&  prGuildName_.first != L"" )
    {
        m_pkGuildImage_L->SetTextureName(prGuildMarkName_.first);
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.first.c_str(), m_pairBattlePoint_.first );
        m_pkGuildImage_L->SetToolTip( szTemp );

        m_pkGuildImage_R->ToggleRender( false );
        m_pkQuestionIconR->ToggleRender( true );
        m_pkQuestionIconR->SetToolTip( g_pkStrLoader->GetString( STR_ID_GUILD_EMPTY ) );
        CheckGuildMark( prGuildMarkName_ , prGuildName_ , m_pairBattlePoint_ );
    }
    else if (prGuildName_.first == L"" && prGuildName_.second != L"")
    {
        m_pkGuildImage_R->SetTextureName(prGuildMarkName_.second);
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.second.c_str(), m_pairBattlePoint_.second );
        m_pkGuildImage_R->SetToolTip( szTemp );

        m_pkGuildImage_L->ToggleRender( false );
        m_pkQuestionIconL->ToggleRender( true );
        m_pkQuestionIconL->SetToolTip( g_pkStrLoader->GetString( STR_ID_GUILD_EMPTY ) );
        CheckGuildMark( prGuildMarkName_ ,prGuildName_ , m_pairBattlePoint_ );
    }
    else
    {
        m_pkGuildmarkIconL->ToggleRender( true );
        m_pkGuildmarkIconR->ToggleRender( true );
        m_pkGuildImage_L->ToggleRender( false );
        m_pkGuildImage_R->ToggleRender( false );
        
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.first.c_str(), m_pairBattlePoint_.first );
        m_pkGuildmarkIconL->SetToolTip( szTemp );
        wsprintf( szTemp , L"%s\n%d", prGuildName_.second.c_str(), m_pairBattlePoint_.second);
        m_pkGuildmarkIconR->SetToolTip( szTemp );
        
        //CheckGuildMark( prGuildMarkName_ );
    }
}

void KGCRoomBar::CheckGuildMark( std::pair<std::wstring, std::wstring>& prGuildMarkName_ , std::pair<std::wstring, std::wstring>& prGuildName_, std::pair< DWORD, DWORD > m_pairBattlePoint_ )
{
    if (prGuildMarkName_.first == L"defaultmark.dds" || ( prGuildName_.first != L"" && prGuildMarkName_.first == L"") ) {
        m_pkGuildmarkIconL->ToggleRender( true );
        m_pkGuildImage_L->ToggleRender( false );
        
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.first.c_str(), m_pairBattlePoint_.first );
        m_pkGuildmarkIconL->SetToolTip( szTemp );
    }
    else if (prGuildMarkName_.second == L"defaultmark.dds" || ( prGuildName_.second != L"" && prGuildMarkName_.second == L"") ){
        m_pkGuildmarkIconR->ToggleRender( true );
        m_pkGuildImage_R->ToggleRender( false );

        WCHAR szTemp[256];
        wsprintf( szTemp , L"%s\n%d", prGuildName_.second.c_str(), m_pairBattlePoint_.second );
        m_pkGuildmarkIconR->SetToolTip( szTemp );
    }
}