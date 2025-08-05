#include "StdAfx.h"
#include "KGCGuildInfoDlg.h"

IMPLEMENT_CLASSNAME( KGCGuildInfoDlg );
IMPLEMENT_WND_FACTORY( KGCGuildInfoDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildInfoDlg, "gc_guild_info_dlg" );

KGCGuildInfoDlg::KGCGuildInfoDlg(void)
:m_pGuildMarkTexture(NULL)
,m_pkGuildName(NULL)
,m_pkGuildBirtyday(NULL)
,m_pkGuildPoint(NULL)
,m_pkGuildActivity(NULL)
,m_pkGuildMasterName(NULL)
,m_pkGeneralListBox(NULL)
,m_pkGeneralListScroll(NULL)
,m_pkGuildNotice1(NULL)
,m_pkGuildNotice2(NULL)
,m_pkGuildIntroduce(NULL)
,m_pkGuildUrl(NULL)
,m_pkGeneralListBG(NULL)
,m_pkGuildMarkIcon(NULL)
,m_pkGuildGradeNew(NULL)
,m_pkGuildGradeRegular(NULL)
,m_pkUpIcon(NULL)
,m_pkGuildLevelIcon(NULL)
,m_pkGaugeBackground(NULL)
,m_pkGaugeBar(NULL)
,m_pkBattlePoint(NULL)
,m_pkGuildLevelStr(NULL)
,iGlevel(0)
{
    LINK_CONTROL("guild_name",						m_pkGuildName );
    LINK_CONTROL("guild_birthday",					m_pkGuildBirtyday );    
	LINK_CONTROL("guild_gpoint_static",				m_pkGuildPoint );
	LINK_CONTROL("guild_active_rate_static",		m_pkGuildActivity );
	LINK_CONTROL("guild_master_name_static",		m_pkGuildMasterName );
	LINK_CONTROL("guild_url_static",				m_pkGuildUrl );

	LINK_CONTROL("guild_general_list_static",		m_pkGeneralListBox );
	LINK_CONTROL("guild_general_list_scroll",		m_pkGeneralListScroll );

	LINK_CONTROL("guild_introduce",					m_pkGuildIntroduce );

	LINK_CONTROL("guild_notice_01",					m_pkGuildNotice1 );
	LINK_CONTROL("guild_notice_02",					m_pkGuildNotice2 );

	LINK_CONTROL("genernal_list_back",				m_pkGeneralListBG );

	LINK_CONTROL("guild_grade_mark_icon",			m_pkGuildMarkIcon );

	LINK_CONTROL("guild_grade_new",					m_pkGuildGradeNew );
	LINK_CONTROL("guild_grade_regular",				m_pkGuildGradeRegular );
    LINK_CONTROL("guild_level_icon" ,               m_pkGuildLevelIcon);
    LINK_CONTROL("guild_gauge_frame" ,              m_pkGaugeBackground);
    LINK_CONTROL("guild_gauge_bar" ,                m_pkGaugeBar);
    LINK_CONTROL("guild_level_up_icon" ,            m_pkUpIcon );
    LINK_CONTROL("battle_point_static",             m_pkBattlePoint );
    LINK_CONTROL("guild_level_static",              m_pkGuildLevelStr);
    
}

KGCGuildInfoDlg::~KGCGuildInfoDlg(void)
{
}


void KGCGuildInfoDlg::OnCreate( void )
{
    m_pkGuildName->InitState(true);
	m_pkGuildName->SetAlign( DT_LEFT );
	m_pkGuildBirtyday->InitState(true);
	m_pkGuildBirtyday->SetAlign( DT_LEFT );
	m_pkGuildPoint->InitState(true);
	m_pkGuildPoint->SetAlign( DT_LEFT );
	m_pkGuildActivity->InitState(true);
	m_pkGuildActivity->SetAlign( DT_LEFT );
    if ( m_pkBattlePoint ) {
        m_pkBattlePoint->InitState(true);
        m_pkBattlePoint->SetAlign( DT_LEFT );
    }
	m_pkGuildMasterName->InitState(true);
	m_pkGuildMasterName->SetAlign( DT_LEFT );
	m_pkGuildUrl->InitState(true);
	m_pkGuildUrl->SetFontColor(D3DCOLOR_RGBA(0,0,0,255));
	m_pkGuildUrl->SetAlign( DT_LEFT );

	m_pkGeneralListBox->InitState( true, true, this );
	m_pkGeneralListBox->SetLineSpace( 1.2f );
	m_pkGeneralListBox->Clear();
	m_pkGeneralListScroll->InitState(true, true, this);
	m_pkGeneralListScroll->SetScrollPageSize( m_pkGeneralListBox->GetDisplayLimitLine() );
	m_pkGeneralListScroll->SetScrollRangeMax( 0 );

	//길드소개 에디트박스
	m_pkGuildIntroduce->InitState(true, true, this);
	m_pkGuildIntroduce->SetEditBoxLimitLine(4);	
	m_pkGuildIntroduce->SetEditBoxLimitText(NEW_GUILD_NOTICE_LIMIT);
	m_pkGuildIntroduce->SetUseScrollBar(false);
	m_pkGuildIntroduce->SetCommonEditType((int)KNGuildNotice::GN_COMMENT);
	m_pkGuildIntroduce->SetTextColor(D3DCOLOR_RGBA(0,0,0,255));
	m_pkGuildIntroduce->SetTitleText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_INTRODUCE_TITLE), DT_CENTER, D3DCOLOR_RGBA(184,134,11,255));
	//공지1 에디트박스
	m_pkGuildNotice1->InitState(true, true, this);
    m_pkGuildNotice1->SetEditBoxLimitLine(70);	
	m_pkGuildNotice1->SetEditBoxLimitText(NEW_GUILD_NOTICE_LIMIT);
	m_pkGuildNotice1->SetCommonEditType((int)KNGuildNotice::GN_NOTICE_1);
	m_pkGuildNotice1->SetTextColor(D3DCOLOR_RGBA(0,0,0,255));
	m_pkGuildNotice1->SetTitleText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NOTICE1_TITLE), DT_CENTER, D3DCOLOR_RGBA(184,134,11,255));
	//공지2 에디트박스
	m_pkGuildNotice2->InitState(true, true, this);
    m_pkGuildNotice2->SetEditBoxLimitLine(70);	
	m_pkGuildNotice2->SetEditBoxLimitText(NEW_GUILD_NOTICE_LIMIT);
	m_pkGuildNotice2->SetCommonEditType((int)KNGuildNotice::GN_NOTICE_2);
	m_pkGuildNotice2->SetTextColor(D3DCOLOR_RGBA(0,0,0,255));
	m_pkGuildNotice2->SetTitleText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_NOTICE2_TITLE), DT_CENTER, D3DCOLOR_RGBA(184,134,11,255));
	
	m_pkGeneralListBG->InitState(true);

	m_pkGuildGradeNew->InitState(false);
	m_pkGuildGradeNew->SetToolTip(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_NEW));
	m_pkGuildGradeRegular->InitState(false);
	m_pkGuildGradeRegular->SetToolTip(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_REGULAR));

    //길드 레벨 , 게이지 바 
    if ( m_pkGuildLevelIcon )
        m_pkGuildLevelIcon->InitState( true );
    if ( m_pkGuildLevelIcon )
        m_pkGaugeBackground->InitState( true );
    if (  m_pkGaugeBar )
        m_pkGaugeBar->InitState( true );
    if (  m_pkUpIcon )
        m_pkUpIcon->InitState( true );
    if ( m_pkGuildLevelStr)
        m_pkGuildLevelStr->InitState( true );
}

void KGCGuildInfoDlg::OnCreateComplete( void )
{	
	//길드마크
	SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);	
	m_pkGuildMarkIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);

	if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER )
		m_pkGuildIntroduce->RenderEditBtn(false);

	if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER && 
		g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_GENERAL )
	{
		m_pkGuildNotice1->RenderEditBtn(false);
		m_pkGuildNotice2->RenderEditBtn(false);
	}
}

void KGCGuildInfoDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkGeneralListScroll )
	{
		if( m_pkGeneralListBox->GetTopIndex() != m_pkGeneralListScroll->GetScrollPos() )
		{
			g_KDSound.Play( "73" );
			m_pkGeneralListBox->SetTopIndex( m_pkGeneralListScroll->GetScrollPos() );
		}																								
	}	
}

void KGCGuildInfoDlg::OnDestroyComplete( void )
{
}

void KGCGuildInfoDlg::FrameMoveInEnabledState( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkGuildUrl->CheckPosInWindowBound(vMousePos))
	{
		m_pkGuildUrl->SetFontColor(D3DCOLOR_RGBA(0,0,255,255));
		if( g_pkInput->BtnDown( KInput::MBLEFT ) && g_kGlobalValue.m_kGuildInfo.m_strURL != L"" )
        {
			PopUpWebPage( (LPTSTR)(LPCTSTR)g_kGlobalValue.m_kGuildInfo.m_strURL.c_str() );
		}
	}
	else
		m_pkGuildUrl->SetFontColor(D3DCOLOR_RGBA(0,0,0,255));

	if( m_pkGeneralListBG->CheckPosInWindowBound(vMousePos) )
	{
		if( g_pkInput->ISWheelUp() )
		{
			if( m_pkGeneralListScroll->GetScrollPos() > 0 )
			{
				m_pkGeneralListScroll->SetScrollPos(m_pkGeneralListScroll->GetScrollPos() - 1);
				g_KDSound.Play( "73" );
				m_pkGeneralListBox->SetTopIndex( m_pkGeneralListScroll->GetScrollPos() );
			}
		}
		else if( g_pkInput->ISWheelDown() )
		{
			if( m_pkGeneralListScroll->GetScrollPos() < m_pkGeneralListScroll->GetScrollRangeMax() )
			{
				m_pkGeneralListScroll->SetScrollPos(m_pkGeneralListScroll->GetScrollPos() + 1);
				g_KDSound.Play( "73" );
				m_pkGeneralListBox->SetTopIndex( m_pkGeneralListScroll->GetScrollPos() );
			}
		}	
	}
}

void KGCGuildInfoDlg::PostChildDraw( void )
{
}

void KGCGuildInfoDlg::OnClose( void )
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCGuildInfoDlg::SetGuildInfo()
{
	m_pkGuildName->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);
	m_pkGuildBirtyday->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_FOUNDING_DAY, "iii", g_kGlobalValue.m_kGuildInfo.m_kFoundingDate.m_sYear,
		(int)g_kGlobalValue.m_kGuildInfo.m_kFoundingDate.m_cMonth, (int)g_kGlobalValue.m_kGuildInfo.m_kFoundingDate.m_cDay));
	m_pkGuildPoint->SetText((int)g_kGlobalValue.m_kGuildInfo.m_dwPoint);
	m_pkGuildActivity->SetText((int)g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint);
    
#if defined ( GUILD_RENEW_S4 )
    m_pkBattlePoint->SetText( (int)g_kGlobalValue.m_kGuildInfo.m_GuildBattlePoint );
    iGlevel = g_kGlobalValue.GetGuildLevel();
    m_pkGuildLevelStr->SetText( iGlevel );
    m_pkGuildLevelStr->SetFontColor( D3DCOLOR_ARGB ( 255, 255, 243, 201 ) );
    m_pkGuildLevelStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 33, 20, 5) );
    m_pkGuildLevelStr->SetFontSize( 16 );
#endif

	m_pkGuildUrl->SetForceWordWrapText(g_kGlobalValue.m_kGuildInfo.m_strURL, true);
	m_pkGuildUrl->SetToolTip(g_kGlobalValue.m_kGuildInfo.m_strURL);
	m_pkGuildIntroduce->SetDefaultText(g_kGlobalValue.m_kGuildInfo.m_strComment);

	if( g_kGlobalValue.m_kGuildInfo.m_ucGrade == KNGuildInfo::GG_NEWBIE )
	{
		m_pkGuildGradeNew->ToggleRender(true);
		m_pkGuildGradeRegular->ToggleRender(false);
	}
	else
	{
		m_pkGuildGradeNew->ToggleRender(false);
		m_pkGuildGradeRegular->ToggleRender(true);		
	}

	std::map<DWORD, KNGuildUserInfo>::iterator mit;
	std::wstring strGeneralName, temp;
	m_pkGeneralListBox->Clear();
	for( mit = g_kGlobalValue.m_mapNGuildUserInfo.begin(); mit != g_kGlobalValue.m_mapNGuildUserInfo.end(); ++mit)
	{
		std::wstring strNickName = mit->second.m_strNickName;
		std::wstring strNickColor = mit->second.m_strNickColor;
		if (strNickColor == L"")
			strNickColor = L"FFFFFF";
		GCUTIL_STR::Trim( strNickName );
		if( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER )
			m_pkGuildMasterName->SetText( L"#c" + strNickColor + strNickName + L"#cx" );
		else if( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL )
			m_pkGeneralListBox->AddString( L"#c" + strNickColor + strNickName + L"#cx" );
	}	
	m_pkGeneralListScroll->SetScrollPageSize( m_pkGeneralListBox->GetDisplayLimitLine() );
	m_pkGeneralListScroll->SetScrollRangeMax( m_pkGeneralListBox->GetViewContentSize() );
	m_pkGeneralListScroll->SetScrollPos( 0 );
	m_pkGeneralListBox->SetTopIndex(0);
}

void KGCGuildInfoDlg::SetGuildNotice( std::wstring strGuildNotice, int iType_)
{
	if( iType_ == KNGuildNotice::GN_COMMENT )
		m_pkGuildIntroduce->SetDefaultText(g_kGlobalValue.m_kGuildInfo.m_strComment);
	else if( iType_ == KNGuildNotice::GN_NOTICE_1 )
		m_pkGuildNotice1->SetDefaultText(strGuildNotice);
	else if( iType_ == KNGuildNotice::GN_NOTICE_2 )
		m_pkGuildNotice2->SetDefaultText(strGuildNotice);
}

void KGCGuildInfoDlg::UpdateGuildMark()
{
	SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);	
	m_pkGuildMarkIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);
    }
    
void KGCGuildInfoDlg::UpdateGuildLevel( int iGPoint)
{
    float fBarPercent			=	0.0f;
    float fFullGuageWidth		=	108.0f;
    int iCount = 0;

    //길드 레벨 구하기 
    //각 레벨 마다 해당 요구 Gpoint가 있는데, 레벨별 차이를 구해 컨테이너 A에 담아둔후 -> 이값은 LevelList에서 m_mapGpointDiff으로 담아 둔다. (Levellist의 GpointDiff함수 참조)
    //( 자신 길드의 Gpoint  - 이전 레벨의 요구 Gpoint ) 값으로 나누면 Bar에 표시해줄 퍼센테이지 값이 나온다. 이 나온 퍼센테이지를 근거로 경험치 획득 양을 표기
    std::map <int , int>::iterator mit = g_kGlobalValue.m_mapGpointDiff.begin();
    std::map <int , int>::iterator preMit;

    int iTotalPoint = 0;
    for( ; mit != g_kGlobalValue.m_mapGpointDiff.end(); ++mit)
    {
        preMit = mit;
        if ( iCount == 0 ) {
            fBarPercent = ( static_cast<float>( (preMit)->first) - static_cast<float>(iGPoint) ) / static_cast<float>(mit->second);
            fBarPercent = 1.0f - fBarPercent;
            iTotalPoint = mit->first;
        }
        else if ( iCount == iGlevel || iGlevel == GUILD_MAX_LEVEL ) {
            fBarPercent = (static_cast<float>(iGPoint) - static_cast<float>( (--preMit)->first)) / static_cast<float>(mit->second );
            iTotalPoint = mit->first;
        }
        iCount++;
    }
    
    if( fBarPercent > 1.0f ) fBarPercent = 1.0f;
        m_dwGuageWidth		=	static_cast<DWORD>( fFullGuageWidth * fBarPercent );

    if ( m_pkGaugeBar )
    {
        m_pkGaugeBar->SetWidth( m_dwGuageWidth );
        WCHAR szTemp[256];
        wsprintf( szTemp , L"%d / %d", iGPoint, iTotalPoint );
        m_pkGaugeBar->SetToolTip( szTemp );
    }

}

