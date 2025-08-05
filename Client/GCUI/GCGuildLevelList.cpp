#include "StdAfx.h"
#include "KGCGuildDlg.h"
#include "GCGuildLevelList.h"
#include "GCGuildBenefitListBar.h"

IMPLEMENT_CLASSNAME( KGCGuildLevelList );
IMPLEMENT_WND_FACTORY( KGCGuildLevelList );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildLevelList, "gc_guild_level_list" );

KGCGuildLevelList::KGCGuildLevelList(void)
:m_pkScrollBar(NULL)
,m_pkGuildLevelBack(NULL)
,m_pkGuildBenefit(NULL)
,m_pkGuildPoint(NULL)
,m_pkRequareContribution(NULL)
,m_pkContributionStr(NULL)
,m_pkContributionScore(NULL)
,m_pkGuildLevelStr(NULL)
,m_pkGuildLevelIcon(NULL)
,m_pkListBack(NULL)
,m_iBarIdx ( 0 )
,iGlevel( 0 )
{
    memset(m_pkGuildBenifitList, 0, sizeof(KGCGuildBenefitListBar*) * MAX_GUILD_LIST);

    LINK_CONTROL("scroll",				m_pkScrollBar );
    LINK_CONTROL("guild_level_background" , m_pkGuildLevelBack );    
    LINK_CONTROL("guild_benefit" , m_pkGuildBenefit );
    LINK_CONTROL("guild_point" ,  m_pkGuildPoint );
    LINK_CONTROL("require_contribution" , m_pkRequareContribution );
    LINK_CONTROL("contribution_static" , m_pkContributionStr );
    LINK_CONTROL("contribution_score", m_pkContributionScore );
    LINK_CONTROL("guild_level_static" , m_pkGuildLevelStr );
    LINK_CONTROL("guild_level_icon" , m_pkGuildLevelIcon );
    LINK_CONTROL("background_person_3",	m_pkListBack );
    
    char szTemp[128];
    for(int iLoop=0; iLoop< MAX_GUILD_LIST; ++iLoop)
    {
        sprintf(szTemp, "guild_benefit_list_bar%d", iLoop);
        LINK_CONTROL( szTemp, m_pkGuildBenifitList[iLoop] );
    }
    
    //길드 레벨 탭의 정보들을 lua 파일에서 읽어 들어 컨테이너에 저장한다. 
    LoadGuildlBenefitList();
    GpointDiff();
}

KGCGuildLevelList::~KGCGuildLevelList(void)
{
    
}

void KGCGuildLevelList::OnCreateComplete( void )
{
    if ( m_pkScrollBar)
        m_pkScrollBar->InitState(true, true, this );
    if ( m_pkGuildLevelBack )
        m_pkGuildLevelBack->InitState(true, true, this );    
    if ( m_pkGuildBenefit )
        m_pkGuildBenefit->InitState(true, true, this );
    if ( m_pkGuildPoint )
        m_pkGuildPoint->InitState(true, true, this );
    if ( m_pkRequareContribution )
        m_pkRequareContribution->InitState(true, true, this );
    if ( m_pkGuildLevelIcon )
        m_pkGuildLevelIcon->InitState(true, true, this );
    if ( m_pkListBack )
        m_pkListBack->InitState(true, true, this );    
    // 리스트바 셋팅
    for(int iLoop=0; iLoop<MAX_GUILD_LIST; ++iLoop)
    {
        if( m_pkGuildBenifitList[iLoop] )
        {
            m_pkGuildBenifitList[ iLoop ]->InitState( true, true, this);
        }    
    }

    
    if ( m_pkContributionStr ) {
        m_pkContributionStr->InitState(true, true, this );
        m_pkContributionStr->SetAlign( DT_LEFT );
    }
    if ( m_pkGuildLevelStr ) {
        m_pkGuildLevelStr->InitState(true, true, this );
        m_pkGuildLevelStr->SetAlign( DT_LEFT );
    }
    if ( m_pkContributionScore ) {
        m_pkContributionScore->InitState(true, true, this );
        m_pkContributionScore->SetAlign( DT_LEFT );
    }

    InitScroll();    
#if defined ( GUILD_RENEW_S4 )
    ShowList();
#endif
}

void KGCGuildLevelList::FrameMoveInEnabledState( void )
{
}

void KGCGuildLevelList::ActionPerformed( const KActionEvent& event )
{
}

void KGCGuildLevelList::InitScroll( void )
{
    if (m_pkScrollBar) {
        m_pkScrollBar->SetScrollPos( 0 );
        m_pkScrollBar->SetScrollRangeMin( 0 );
        m_pkScrollBar->SetScrollPageSize( 0 );   
        m_pkScrollBar->SetScrollRangeMax( 0 );
    }
}

void KGCGuildLevelList::ShowList()
{
    int iContribution = (int)g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint;
    int iGpoint = (int)g_kGlobalValue.m_kGuildInfo.m_dwPoint;
    int iGLevel = GetGuildLevel( g_kGlobalValue.m_mapGuildLevelTable );

    m_pkContributionStr->SetText( g_pkStrLoader->GetString( STR_ID_GUILD_CONTRIBUTION ) );
    m_pkContributionScore->SetText( (int)g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint );

    m_pkGuildLevelStr->SetText( iGLevel );
    m_pkGuildLevelStr->SetFontColor( D3DCOLOR_ARGB(255, 255, 243, 201) );
    m_pkGuildLevelStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 33, 20, 5) );
    m_pkGuildLevelStr->SetFontSize( 16 );
    
    for(int iLoop=0; iLoop<MAX_GUILD_LIST; ++iLoop)
    {
        if( m_pkGuildBenifitList[ iLoop ] )
        {
            // 정보초기화
            m_pkGuildBenifitList[ iLoop ]->ClearText();            
            m_pkGuildBenifitList[ iLoop ]->SetLevelText( m_vecGuildBenefitList[ iLoop ].strLevel  );
            m_pkGuildBenifitList[ iLoop ]->SetStrNumberText ( m_vecGuildBenefitList[iLoop].iNumber );
            m_pkGuildBenifitList[ iLoop ]->SetGpointText (m_vecGuildBenefitList[ iLoop ].iGpoint );
            m_pkGuildBenifitList[ iLoop ]->SetContributionText (m_vecGuildBenefitList[ iLoop ].iNeedContribution );
            
            bool bFontOn = ( ( iContribution >= m_vecGuildBenefitList[ iLoop ].iNeedContribution ) && iGLevel > iLoop )? true : false ;
                        
            m_pkGuildBenifitList[ iLoop ]->SetListFontColor( bFontOn );
        }
    }    
}

void KGCGuildLevelList::LoadGuildlBenefitList()
{
    USES_CONVERSION;

    m_vecGuildBenefitList.clear();
    KLuaManager luaMgr; 

    if( GCFUNC::LoadLuaScript( luaMgr, "GuildlevelBenefit.lua") == false)
        return;

    if ( luaMgr.BeginTable( "GuildBenefit") == S_OK )
    {
        for( int i = 1; luaMgr.BeginTable( i ) == S_OK; ++i )
        {
            KGuildBenefitList kGuildBenefitList;

            std::string strLevel;
            if ( luaMgr.GetValue( "LevelString", strLevel ) == S_OK )
            {
                std::wstring strLevel_( A2W( strLevel.c_str() ) );
                kGuildBenefitList.strLevel= strLevel_;
            }

            int iBenefitStr;
            if ( luaMgr.GetValue( "BenefitString", iBenefitStr ) == S_OK )
            {
                kGuildBenefitList.iNumber = iBenefitStr;
            }

            int iGpoint_;
            if ( luaMgr.GetValue( "GPoint", iGpoint_ ) == S_OK )
            {
                kGuildBenefitList.iGpoint = iGpoint_;
            }

            int iContribution;
            if ( luaMgr.GetValue( "Contribution", iContribution ) == S_OK )
            {
                kGuildBenefitList.iNeedContribution = iContribution;
            }

            m_vecGuildBenefitList.push_back(kGuildBenefitList);

            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }
}

int KGCGuildLevelList::GetGuildLevel( std::map < USHORT, int> m_mapGuildLevelTable )
{
    int iGpoint = (int)g_kGlobalValue.m_kGuildInfo.m_dwPoint;

    std::map< USHORT, int >::iterator mit;
    for (mit = m_mapGuildLevelTable.begin() ; mit != m_mapGuildLevelTable.end(); ++mit )
    {
        if (mit->second > iGpoint)
            break;
        else
            iGlevel = mit->first;
    }

    return iGlevel;
}

void KGCGuildLevelList::GpointDiff( void )
{
    std::vector<KGuildBenefitList>::iterator it;
    std::vector<KGuildBenefitList>::iterator itPre;

    for (it = m_vecGuildBenefitList.begin(); it != m_vecGuildBenefitList.end(); ++it)
    {
        if ( !( it == m_vecGuildBenefitList.begin() ) ) {
            itPre = it;
            int iPreNumber = (--itPre)->iGpoint;
            int iNowNumber = it->iGpoint;
            int iTemp = 0;
            iTemp = iNowNumber - iPreNumber;
            g_kGlobalValue.m_mapGpointDiff.insert(pair<int, int>(it->iGpoint, iTemp ));
        }
        else {
            g_kGlobalValue.m_mapGpointDiff.insert(pair<int, int>(it->iGpoint, it->iGpoint ));
        }
    }    
}