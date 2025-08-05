#include "stdafx.h"
#include "KGCRoomFilterList.h"

//
//
//
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCRoomFilterList );
IMPLEMENT_WND_FACTORY( KGCRoomFilterList );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomFilterList, "gc_room_filter_list" );

KGCRoomFilterList::KGCRoomFilterList(void)
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
    {
        m_apkIcon[i] = NULL;
        m_apkName[i] = NULL;
        m_abEnabled[i] = false;
    }

    LINK_CONTROL( "mode_all",                   m_apkIcon[ KGCRoomListManager::ECATEGORY_ALL ] );
    LINK_CONTROL( "mode_match",                 m_apkIcon[ KGCRoomListManager::ECATEGORY_MATCH ] );
    LINK_CONTROL( "mode_tagmatch",              m_apkIcon[ KGCRoomListManager::ECATEGORY_TAGMATCH ] );
    LINK_CONTROL( "mode_monstercrusader",       m_apkIcon[ KGCRoomListManager::ECATEGORY_MONSTERCRUSADER ] );
    LINK_CONTROL( "mode_monsterhunt",           m_apkIcon[ KGCRoomListManager::ECATEGORY_MONSTERHUNT ] );
    LINK_CONTROL( "mode_death",                 m_apkIcon[ KGCRoomListManager::ECATEGORY_DEATHMATCH ] );
    LINK_CONTROL( "mode_guild",                 m_apkIcon[ KGCRoomListManager::ECATEGORY_GUILD ] );
	LINK_CONTROL( "mode_angle_egg",             m_apkIcon[ KGCCategoryBox::ECATEGORY_ANGLE_EGG ] );
    LINK_CONTROL( "mode_dota",                  m_apkIcon[ KGCCategoryBox::ECATEGORY_DOTA ] );
    LINK_CONTROL( "mode_fatal_death",           m_apkIcon[ KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH ] );

    LINK_CONTROL( "mode_all_name",              m_apkName[ KGCRoomListManager::ECATEGORY_ALL ] );
    LINK_CONTROL( "mode_match_name",            m_apkName[ KGCRoomListManager::ECATEGORY_MATCH ] );
    LINK_CONTROL( "mode_tagmatch_name",         m_apkName[ KGCRoomListManager::ECATEGORY_TAGMATCH ] );
    LINK_CONTROL( "mode_monstercrusader_name",  m_apkName[ KGCRoomListManager::ECATEGORY_MONSTERCRUSADER ] );
    LINK_CONTROL( "mode_monsterhunt_name",      m_apkName[ KGCRoomListManager::ECATEGORY_MONSTERHUNT ] );
    LINK_CONTROL( "mode_death_name",            m_apkName[ KGCRoomListManager::ECATEGORY_DEATHMATCH ] );
    LINK_CONTROL( "mode_guild_name",            m_apkName[ KGCRoomListManager::ECATEGORY_GUILD ] );
	LINK_CONTROL( "mode_angle_egg_name",        m_apkName[ KGCCategoryBox::ECATEGORY_ANGLE_EGG ] );
    LINK_CONTROL( "mode_dota_name",             m_apkName[ KGCCategoryBox::ECATEGORY_DOTA ] );
    LINK_CONTROL( "mode_fatal_death_name",      m_apkName[ KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH ] );


	LINK_CONTROL( "back",						m_pkSizingBox );

    m_iSelectCategoryIndex = KGCRoomListManager::ECATEGORY_ALL;
    m_bInputCheck = false;
    m_bClick = false;
}

KGCRoomFilterList::~KGCRoomFilterList(void)
{
}

void KGCRoomFilterList::OnCreate()
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
    {
        m_apkIcon[i]->InitState( true, true, this );
        m_apkName[i]->InitState( true );
        m_apkName[i]->SetAutoScroll(true);
    }

    m_apkName[ KGCRoomListManager::ECATEGORY_ALL ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_ALL ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_MATCH ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_MATCH ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_TAGMATCH ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_TAG_MATCH ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_MONSTERCRUSADER ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_MONSTER_CRUSADERS ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_MONSTERHUNT ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_MONSTER_HUNT ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_DEATHMATCH ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_DEATHMATCH ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_GUILD ]->SetText( g_pkStrLoader->GetString( STR_ID_ROOM_FILTER_GUILD ) );
	//m_apkName[ KGCCategoryBox::ECATEGORY_ANGLE_EGG ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_ANGELS_EGG ) );
    m_apkName[ KGCCategoryBox::ECATEGORY_ANGLE_EGG ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_HALLOWEEN_PUMPKIN ) );
    m_apkName[ KGCCategoryBox::ECATEGORY_DOTA ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_DOTA ) );
    m_apkName[ KGCRoomListManager::ECATEGORY_FATAL_DEATHMATCH ]->SetText( g_pkStrLoader->GetString( STR_ID_GMC_FATAL_DEATHMATCH ) );

	m_pkSizingBox->InitState( true );
}

void KGCRoomFilterList::FrameMoveInEnabledState()
{
}

void KGCRoomFilterList::ActionPerformed( const KActionEvent& event )
{
    switch( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            // 마우스가 올라간 녀석의 글자 색깔을 노란색으로 바꾼다, 평소엔 흰색
            for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
            {
                if( event.m_pWnd == m_apkIcon[i] )
                    m_apkName[i]->SetFontColor( D3DCOLOR_ARGB( m_byteFontAlpha, 0xFF, 0xAA, 0x00 ) );
                else
                    m_apkName[i]->SetFontColor( D3DCOLOR_ARGB( m_byteFontAlpha, 0xFF, 0xFF, 0xFF ) );
            }
            g_KDSound.Play( "30" ); return;
        }
        case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); return;
    }

    if( !m_bInputCheck )
        return;

    if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    // 어느 녀석이 클릭 됐는지?
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
    {
        if( event.m_pWnd == m_apkIcon[i] )
        {
            m_bClick = true;
            m_iSelectCategoryIndex = i;
            ToggleRender( false );
        }
    }
}

void KGCRoomFilterList::InitTextColor()
{
    for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
        m_apkName[i]->SetFontColor( 0xFFFFFFFF );
}

void KGCRoomFilterList::Update()
{
	const int iTopButtomGap = GC_SCREEN_DIV_SIZE_INT(5);	// 윗부분의 갭입니다.
	const int iBetweenGap = GC_SCREEN_DIV_SIZE_INT(3); // 아이콘 사이의 갭입니다.
	const int iIconTextGap = GC_SCREEN_DIV_SIZE_INT(3); //아이콘과 
    int iEnabledCount = 0; //활성화된 필터의 갯수를 세어서 BackFrame 크기를 조절합니다.
	for( int i = 0; i < KGCRoomListManager::NUM_CATEGORY; ++i )
	{
		if( SiKGCRoomListManager()->IsFilterEnabled( (KGCRoomListManager::EFILTER_CATEGORY)i ) == true 
			|| KGCRoomListManager::ECATEGORY_ALL == (KGCRoomListManager::EFILTER_CATEGORY)i  )
		{
			m_apkName[i]->ToggleRender( true );
			m_apkIcon[i]->ToggleRender( true );
			++iEnabledCount;
		}
		else
		{
			m_apkName[i]->ToggleRender( false );
			m_apkIcon[i]->ToggleRender( false );
		}
	}

	int iIconWidthHeight = GC_SCREEN_DIV_SIZE_INT(26);	//m_apkIcon UI의 사이즈인데, 하드코딩으로 해결 합니다. 원망은... 시간에게...
	int iFrameHeight = iTopButtomGap + iEnabledCount * ( iIconWidthHeight + iBetweenGap );
    
	D3DXVECTOR2 vPosParent = GetFixedWindowLocalPos();
	vPosParent.y += (float)GetHeight() - (float)iFrameHeight;
	SetWindowPosDirect( vPosParent );

	m_pkSizingBox->SetHeight( iFrameHeight );
	SetHeight( m_pkSizingBox->GetHeight() );
	SetWidth( m_pkSizingBox->GetWidth() );
	

	iEnabledCount = 0;
	D3DXVECTOR2 vPosFrame = m_pkSizingBox->GetFixedWindowLocalPos();
	vPosFrame.y += (float)iTopButtomGap;
	for( int i = 0 ; i < KGCRoomListManager::NUM_CATEGORY  ; ++i )
	{
		if( m_apkIcon[i]->IsRenderOn() == false )
			continue;

		D3DXVECTOR2 vPosIcon = m_apkIcon[i]->GetCurrentWindowLocalPos();
		vPosIcon.y = vPosFrame.y + ( iEnabledCount * ( iIconWidthHeight + iBetweenGap ));
		m_apkIcon[i]->SetWindowPosDirect( vPosIcon );

		D3DXVECTOR2 vPosText = m_apkName[i]->GetCurrentWindowLocalPos();
		vPosText.x = (float)(vPosIcon.x + iIconWidthHeight + iIconTextGap);
		vPosText.y = vPosIcon.y + iIconTextGap + GC_SCREEN_DIV_SIZE_INT(4);
		m_apkName[i]->SetWindowPosDirect( vPosText );

		++iEnabledCount;
	}
}

void KGCRoomFilterList::SetChildFontAlpha( BYTE byteFontAlpha )
{
    m_byteFontAlpha = byteFontAlpha;
}
