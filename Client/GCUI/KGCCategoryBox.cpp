#include "stdafx.h"
#include "KGCCategoryBox.h"

//

IMPLEMENT_CLASSNAME( KGCCategoryBox );
IMPLEMENT_WND_FACTORY( KGCCategoryBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCategoryBox, "gc_category_box" );

KGCCategoryBox::KGCCategoryBox( void )
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
        m_apkCategoryList[i] = NULL;

    for( int i = 0; i < NUM_LEVEL; ++i )
        m_apkLevelList[i] = NULL;

    m_pkLevelText           = NULL;
    m_pkLevelBack           = NULL;
    m_pkLevelImg           = NULL;
    m_pkClearCheck          = NULL;
    m_apkEventHellMode      = NULL;

    LINK_CONTROL( "mode_all",               m_apkCategoryList[ KGCRoomListManager::ECATEGORY_ALL ] );
    LINK_CONTROL( "mode_match",             m_apkCategoryList[ KGCRoomListManager::ECATEGORY_MATCH ] );
    LINK_CONTROL( "mode_tagmatch",          m_apkCategoryList[ KGCRoomListManager::ECATEGORY_TAGMATCH ] );
    LINK_CONTROL( "mode_monstercrusader",   m_apkCategoryList[ KGCRoomListManager::ECATEGORY_MONSTERCRUSADER ] );
    LINK_CONTROL( "mode_monsterhunt",       m_apkCategoryList[ KGCRoomListManager::ECATEGORY_MONSTERHUNT ] );
    LINK_CONTROL( "mode_death",             m_apkCategoryList[ KGCRoomListManager::ECATEGORY_DEATHMATCH ] );
    LINK_CONTROL( "mode_guild",             m_apkCategoryList[ KGCRoomListManager::ECATEGORY_GUILD ] );
    LINK_CONTROL( "mode_angle_egg",         m_apkCategoryList[ KGCRoomListManager::ECATEGORY_ANGLE_EGG ] );
    LINK_CONTROL( "mode_dota",              m_apkCategoryList[ KGCRoomListManager::ECATEGORY_DOTA ] );
    LINK_CONTROL( "mode_fatal_death",       m_apkCategoryList[ KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH ] );
	
    LINK_CONTROL( "level_1",                m_apkLevelList[ LEVEL_1 ] );
    LINK_CONTROL( "level_2",                m_apkLevelList[ LEVEL_2 ] );
    LINK_CONTROL( "level_3",                m_apkLevelList[ LEVEL_3 ] );
    LINK_CONTROL( "level_4",                m_apkLevelList[ LEVEL_4 ] );
    LINK_CONTROL( "level_5",                m_apkEventHellMode );

    LINK_CONTROL( "static_level_text",  m_pkLevelText );
    LINK_CONTROL( "level_back",         m_pkLevelBack );
    LINK_CONTROL( "level_text",         m_pkLevelImg );
    LINK_CONTROL( "level_clear_check",  m_pkClearCheck );
    
    

    m_iCategoryNameID = STR_ID_GMC_MATCH;
}

KGCCategoryBox::~KGCCategoryBox(void)
{
}

void KGCCategoryBox::OnCreate()
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
    {
        if( m_apkCategoryList[i] )
        {
            m_apkCategoryList[i]->InitState( false );
        }
    }

    for( int i = 0; i < NUM_LEVEL; ++i )
    {
        if ( m_apkLevelList[i] )
        {
            m_apkLevelList[i]->InitState( false );
        }
    }

    if( m_apkEventHellMode )
        m_apkEventHellMode->InitState( false );

    if ( m_pkLevelText )
    {
        m_pkLevelText->InitState( false );
        m_pkLevelText->SetText(L"");
        m_pkLevelText->SetAlign(DT_CENTER);
        m_pkLevelText->SetFontColor(D3DCOLOR_RGBA(255,255,0,255));
    }

    if ( m_pkLevelBack )
    {   
        m_pkLevelBack->InitState( false );
    }

    if ( m_pkLevelImg ) 
    {
        m_pkLevelImg->InitState( false );
    }

    if ( m_pkClearCheck )
    {
        m_pkClearCheck->InitState( false );
    }
}

void KGCCategoryBox::FrameMoveInEnabledState()
{
}

void KGCCategoryBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCCategoryBox::UpdateCategory( char cCategory )
{
    for( int i = 0; i < NUM_LEVEL; ++i )
    {
        if ( m_apkLevelList[i] )
        {
            m_apkLevelList[i]->ToggleRender( false );
        }
    }

    if( m_apkEventHellMode )
        m_apkEventHellMode->ToggleRender( false );

    int iCurrentCategory = KGCRoomListManager::ECATEGORY_MATCH;
    m_iCategoryNameID = STR_ID_GMC_MATCH;

    switch( cCategory )
    {
        case GC_GMC_MATCH:              // 대전
            iCurrentCategory = KGCRoomListManager::ECATEGORY_MATCH;
            m_iCategoryNameID = STR_ID_GMC_MATCH;
            break;
        case GC_GMC_TAG_MATCH:          // 태그매치
            iCurrentCategory = KGCRoomListManager::ECATEGORY_TAGMATCH;
            m_iCategoryNameID = STR_ID_GMC_TAG_MATCH;
            break;
        case GC_GMC_MONSTER_CRUSADER:   // 몬스터 원정대 (보스 러쉬)
            iCurrentCategory = KGCRoomListManager::ECATEGORY_MONSTERCRUSADER;
            m_iCategoryNameID = STR_ID_GMC_MONSTER_CRUSADERS;
            break;
        case GC_GMC_MONSTER_HUNT:       // 몬스터대전
            iCurrentCategory = KGCRoomListManager::ECATEGORY_MONSTERHUNT;
            m_iCategoryNameID = STR_ID_GMC_MONSTER_HUNT;
            break;
        case GC_GMC_ANGELS_EGG:         // 천사알깨기
            iCurrentCategory = KGCRoomListManager::ECATEGORY_ANGLE_EGG;
            //m_iCategoryNameID = STR_ID_GMC_ANGELS_EGG;
            m_iCategoryNameID = STR_ID_GMC_HALLOWEEN_PUMPKIN;
            break;
        case GC_GMC_DEATHMATCH:         // 데스매치
            iCurrentCategory = KGCRoomListManager::ECATEGORY_DEATHMATCH;
            m_iCategoryNameID = STR_ID_GMC_DEATHMATCH;
            break;
        case GC_GMC_FATAL_DEATHMATCH:         // 페이탈 데스매치
            iCurrentCategory = KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH;
            m_iCategoryNameID = STR_ID_GMC_FATAL_DEATHMATCH;
            break;
        case GC_GMC_GUILD_BATTLE:       // 길드전
            iCurrentCategory = KGCRoomListManager::ECATEGORY_GUILD;
            m_iCategoryNameID = STR_ID_ROOM_FILTER_GUILD;
            break;
        case GC_GMC_DOTA:               // 점령전
            iCurrentCategory = KGCRoomListManager::ECATEGORY_DOTA;
            m_iCategoryNameID = STR_ID_GMC_DOTA;
            break;
        case GC_GMC_INVALID:            // -1 이면 전체모드..
            iCurrentCategory = KGCRoomListManager::ECATEGORY_ALL;
            m_iCategoryNameID = STR_ID_GMC_ALL;
            break;
    }

    for(int iLoop=0; iLoop<KGCRoomListManager::NUM_CATEGORY; ++iLoop)
    {
        if ( m_apkCategoryList[iLoop] ) 
        m_apkCategoryList[iLoop]->ToggleRender( ( iLoop == iCurrentCategory ) );   
    }
}

void KGCCategoryBox::UpdateLevel( int nLevel, bool bClear )
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
	{
        m_apkCategoryList[i]->ToggleRender( false );
	}

    for( int i = 0; i < NUM_LEVEL; ++i )
    {
        if ( m_apkLevelList[i] )
            m_apkLevelList[i]->ToggleRender( false );
    }   

    if( m_apkEventHellMode )
        m_apkEventHellMode->ToggleRender( false );

    int iLevel = KGCCategoryBox::LEVEL_1;

    if ( m_pkLevelText ) 
        m_pkLevelText->ToggleRender ( false );

    if ( m_pkLevelBack ) 
        m_pkLevelBack->ToggleRender ( false );

    if ( m_pkLevelImg ) 
        m_pkLevelImg->ToggleRender ( false );

    if ( m_pkClearCheck )
        m_pkClearCheck->ToggleRender( false );

    if ( SiKGCWorldMapManager()->IsRelayHeroDungeon() )
    {
        if ( m_pkLevelText ) 
        {
            m_pkLevelText->ToggleRender ( true );

            std::wstring strLevel = boost::lexical_cast<std::wstring>( nLevel+1 );

            m_pkLevelText->SetText( strLevel );
        }

        if ( m_pkLevelBack )
            m_pkLevelBack->ToggleRender( true );

        if ( m_pkLevelImg ) 
            m_pkLevelImg->ToggleRender( true );

        if ( m_pkClearCheck )
        {
            m_pkClearCheck->ToggleRender( true );

            m_pkClearCheck->SetCheck( bClear );
        }
    }
    else if ( SiKGCWorldMapManager()->IsHeroDungeon() )
    {       
    }
    else if ( nLevel == GC_DUNGEON_LEVEL_HELL && SiKGCWorldMapManager()->IsEventDungeon() )
    {       
        if( m_apkEventHellMode )
            m_apkEventHellMode->ToggleRender( true );
    }
    else if (nLevel == GC_DUNGEON_LEVEL_BREAK && SiKGCWorldMapManager()->IsEventDungeon())
    {
        if (m_apkEventHellMode)
            m_apkEventHellMode->ToggleRender(true);
    }
    else
    {
        switch( nLevel )
        {
            case GC_DUNGEON_LEVEL_NORMAL:       iLevel = KGCCategoryBox::LEVEL_1; break;
            case GC_DUNGEON_LEVEL_NORMAL+1:     iLevel = KGCCategoryBox::LEVEL_2; break;
            case GC_DUNGEON_LEVEL_NORMAL+2:     iLevel = KGCCategoryBox::LEVEL_3; break;
            case GC_DUNGEON_LEVEL_HELL:         iLevel = KGCCategoryBox::LEVEL_4; break;
            default:                            iLevel = KGCCategoryBox::LEVEL_1; break;
        }

        if ( m_apkLevelList[iLevel]  )
	    {
            m_apkLevelList[iLevel]->ToggleRender( true );
	    }
    }
}

std::wstring KGCCategoryBox::GetCategoryName()
{
    return g_pkStrLoader->GetString( m_iCategoryNameID );
}

EGCGameModeCategory KGCCategoryBox::GetGMCfromEnum( KGCRoomListManager::EFILTER_CATEGORY eGMC )
{
	switch( eGMC )
	{
	case KGCRoomListManager::ECATEGORY_ALL:             // 전체모드
		return GC_GMC_INVALID;
	case KGCRoomListManager::ECATEGORY_MATCH:           // 대전
		return GC_GMC_MATCH;
	case KGCRoomListManager::ECATEGORY_TAGMATCH:        // 태그매치
		return GC_GMC_TAG_MATCH;
	case KGCRoomListManager::ECATEGORY_MONSTERCRUSADER: // 몬스터원정대 (보스러쉬)
		return GC_GMC_MONSTER_CRUSADER;
	case KGCRoomListManager::ECATEGORY_MONSTERHUNT:     // 몬스터대전
		return GC_GMC_MONSTER_HUNT;
    case KGCRoomListManager::ECATEGORY_DEATHMATCH:      // 데스매치
        return GC_GMC_DEATHMATCH;
    case KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH:      // 데스매치
        return GC_GMC_FATAL_DEATHMATCH;
    case KGCRoomListManager::ECATEGORY_GUILD:           // 길드전
        return GC_GMC_GUILD_BATTLE;
    case KGCRoomListManager::ECATEGORY_ANGLE_EGG:       // 천사 알깨기
        return GC_GMC_ANGELS_EGG;
    case KGCRoomListManager::ECATEGORY_DOTA:            // 점령전
        return GC_GMC_DOTA;
	}
	return GC_GMC_INVALID;
}
