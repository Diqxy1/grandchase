#include "stdafx.h"
#include "KGCTutorialMonsterSetting.h"
//
//
//
//

IMPLEMENT_CLASSNAME( KGCTutorialMonsterSetting );
IMPLEMENT_WND_FACTORY( KGCTutorialMonsterSetting );
IMPLEMENT_WND_FACTORY_NAME( KGCTutorialMonsterSetting, "gc_tutorial_monster_setting" );

KGCTutorialMonsterSetting::KGCTutorialMonsterSetting( void )
{
    // 디버그 모드나 사내테섭일때만.. 연습모드에서 몬스터를 등장시킬 수 있다!
    m_pkMonsterChangeLeft       = NULL;
    m_pkMonsterChangeRight      = NULL;
    m_pkMonsterLevelChangeLeft  = NULL;
    m_pkMonsterLevelChangeRight = NULL;
    m_pMonsterName              = NULL;
    m_pMonsterAiFileName        = NULL;
    m_pMonsterLevel             = NULL;
    m_pMonsterIndex             = NULL;

    LINK_CONTROL( "monster_change_left",        m_pkMonsterChangeLeft );
    LINK_CONTROL( "monster_change_right",       m_pkMonsterChangeRight );
    LINK_CONTROL( "monster_level_change_left",  m_pkMonsterLevelChangeLeft );
    LINK_CONTROL( "monster_level_change_right", m_pkMonsterLevelChangeRight );
    LINK_CONTROL( "monster_name",               m_pMonsterName );
    LINK_CONTROL( "monster_ai_file_name",       m_pMonsterAiFileName );
    LINK_CONTROL( "monster_level",              m_pMonsterLevel );
    LINK_CONTROL( "monster_index",              m_pMonsterIndex );

    m_iSelectMonsterIndex = 0;
    m_iMonsterLevel = 0;

    // 챔피언 속성관련..
    DWORD dwChampionProperty = 0x00000001;
    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
    {
        m_apkCheckBox[i]    = NULL;
        m_apkCheckText[i]   = NULL;
        m_apkCheck[i]       = NULL;

        sprintf( strWndName, "checkbox%02d", i );
        LINK_CONTROL( strWndName,   m_apkCheckBox[i] );
        sprintf( strWndName, "check_text%02d", i );
        LINK_CONTROL( strWndName,   m_apkCheckText[i] );
        sprintf( strWndName, "check%02d", i );
        LINK_CONTROL( strWndName,   m_apkCheck[i] );

        m_adwProperty[i] = dwChampionProperty;
        dwChampionProperty *= 2;
    }

    m_pkCheckBoxAll     = NULL;
    m_pkCheckTextAll    = NULL;
    m_pkCheckAll        = NULL;
    LINK_CONTROL( "checkbox_all",   m_pkCheckBoxAll );
    LINK_CONTROL( "check_text_all", m_pkCheckTextAll );
    LINK_CONTROL( "check_all",      m_pkCheckAll );
}

KGCTutorialMonsterSetting::~KGCTutorialMonsterSetting( void )
{
}

void KGCTutorialMonsterSetting::OnCreate( void )
{
    m_pkMonsterChangeLeft->InitState( true, true, this );
    m_pkMonsterChangeRight->InitState( true, true, this );
    m_pkMonsterLevelChangeLeft->InitState( true, true, this );
    m_pkMonsterLevelChangeRight->InitState( true, true, this );
    m_pMonsterName->InitState( true );
    m_pMonsterAiFileName->InitState( true );
    m_pMonsterLevel->InitState( true );
    m_pMonsterIndex->InitState( true );

    m_pMonsterName->SetAlign( DT_CENTER );
    m_pMonsterAiFileName->SetAlign( DT_CENTER );
    m_pMonsterLevel->SetAlign( DT_CENTER );
    m_pMonsterIndex->SetAlign( DT_CENTER );

    m_pMonsterName->SetFontOutline( true );
    m_pMonsterAiFileName->SetFontOutline( true );
    m_pMonsterLevel->SetFontOutline( true );
    m_pMonsterIndex->SetFontOutline( true );

    for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
    {
        m_apkCheckBox[i]->InitState( true, true, this );
        m_apkCheckText[i]->InitState( true, true, this );
        m_apkCheck[i]->InitState( false, true, this );

        m_apkCheckText[i]->SetFontOutline( true );
    }

    m_pkCheckBoxAll->InitState( true, true, this );
    m_pkCheckTextAll->InitState( true, true, this );
    m_pkCheckAll->InitState( false, true, this );

    m_pkCheckTextAll->SetText( L"일괄선택/해제" );
    m_apkCheckText[0]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY01 ) );
    m_apkCheckText[1]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY02 ) );
    m_apkCheckText[2]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY03 ) );
    m_apkCheckText[3]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY04 ) );
    m_apkCheckText[4]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY05 ) );
    m_apkCheckText[5]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY06 ) );
    m_apkCheckText[6]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY07 ) );
    m_apkCheckText[7]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY08 ) );
	m_apkCheckText[8]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY08 ) );
	m_apkCheckText[9]->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION_PROPERTY08 ) );

    //m_pkMonsterChangeLeft->SetHotKey( DIK_LEFT );
    //m_pkMonsterChangeRight->SetHotKey( DIK_RIGHT );
    //m_pkMonsterLevelChangeLeft->SetHotKey( DIK_PGDN );
    //m_pkMonsterLevelChangeRight->SetHotKey( DIK_PGUP );
}

#define MONSTER_BUTTON_LOCK_FRAME (8)
void KGCTutorialMonsterSetting::FrameMoveInEnabledState( void )
{
    static int siCount = MONSTER_BUTTON_LOCK_FRAME;
    if( siCount <= 0 )
    {
        if( g_pkInput->IsPressed( DIK_LEFT ) )
        {
            OnMonsterChangeLeft();
            siCount = MONSTER_BUTTON_LOCK_FRAME;
        }
        else if( g_pkInput->IsPressed( DIK_RIGHT ) )
        {
            OnMonsterChangeRight();
            siCount = MONSTER_BUTTON_LOCK_FRAME;
        }
        else if( g_pkInput->IsPressed( DIK_PGDN ) )
        {
            OnMonsterLevelChangeLeft();
            siCount = MONSTER_BUTTON_LOCK_FRAME;
        }
        else if( g_pkInput->IsPressed( DIK_PGUP ) )
        {
            OnMonsterLevelChangeRight();
            siCount = MONSTER_BUTTON_LOCK_FRAME;
        }
    }

    if( siCount > 0 )
        --siCount;
}

void KGCTutorialMonsterSetting::ActionPerformed( const KActionEvent& event )
{
    // 몬스터 이름/레벨 바꾸기
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkMonsterChangeLeft )         { OnMonsterChangeLeft(); return; }
        if( event.m_pWnd == m_pkMonsterChangeRight )        { OnMonsterChangeRight(); return; }
        if( event.m_pWnd == m_pkMonsterLevelChangeLeft )    { OnMonsterLevelChangeLeft(); return; }
        if( event.m_pWnd == m_pkMonsterLevelChangeRight )   { OnMonsterLevelChangeRight(); return; }

        if( event.m_pWnd == m_pkCheckBoxAll || 
            event.m_pWnd == m_pkCheckTextAll ||
            event.m_pWnd == m_pkCheckAll )
        {
            m_pkCheckAll->ToggleRender( !m_pkCheckAll->IsRenderOn() );
            for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
                m_apkCheck[i]->ToggleRender( m_pkCheckAll->IsRenderOn() );

            return;
        }

        for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
        {
            // 체크 하면?
            if( event.m_pWnd == m_apkCheckBox[i] || 
                event.m_pWnd == m_apkCheckText[i] ||
                event.m_pWnd == m_apkCheck[i] )
            {
                m_apkCheck[i]->ToggleRender( !m_apkCheck[i]->IsRenderOn() );

                int iCountCheck = 0;
                for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
                    iCountCheck += m_apkCheck[i]->IsRenderOn() ? 1 : 0;

                m_pkCheckAll->ToggleRender( iCountCheck == 8 );

                return;
            }
        }
    }
}

void KGCTutorialMonsterSetting::OnMonsterChangeLeft()
{
    int iMonsterSize = (int)g_kMonsterManager.m_vecMonsterTable.size() + 1;
    m_iSelectMonsterIndex = ( m_iSelectMonsterIndex - 1 + iMonsterSize ) % iMonsterSize;
    m_iMonsterLevel = g_kMonsterManager.m_vecMonsterTable[m_iSelectMonsterIndex - 1].iBaseLevel;
    UpdateMonsterPanel();
}

void KGCTutorialMonsterSetting::OnMonsterChangeRight()
{
    int iMonsterSize = (int)g_kMonsterManager.m_vecMonsterTable.size() + 1;
    m_iSelectMonsterIndex = ( m_iSelectMonsterIndex + 1 ) % iMonsterSize;
    m_iMonsterLevel = g_kMonsterManager.m_vecMonsterTable[m_iSelectMonsterIndex - 1].iBaseLevel;
    UpdateMonsterPanel();
}

void KGCTutorialMonsterSetting::OnMonsterLevelChangeLeft()
{
    if( m_iMonsterLevel <= 0 )
        m_iMonsterLevel = 100;
    else
        --m_iMonsterLevel;
    UpdateMonsterPanel();
}

void KGCTutorialMonsterSetting::OnMonsterLevelChangeRight()
{
    if( m_iMonsterLevel >= 100 )
        m_iMonsterLevel = 0;
    else
        ++m_iMonsterLevel;
    UpdateMonsterPanel();
}

void KGCTutorialMonsterSetting::UpdateMonsterPanel()
{
    #if defined( _DEBUG ) || !defined( __PATH__ )
    {
        if( m_iSelectMonsterIndex == 0 )
        {
            std::wstringstream strIndex;
            strIndex << L"몬스터선택    " << m_iSelectMonsterIndex  << "/" << (int)g_kMonsterManager.m_vecMonsterTable.size();

            m_pMonsterName->SetText( L"선택 안함" );
            m_pMonsterAiFileName->SetText( L"선택 안함" );
            m_pMonsterLevel->SetText( L"선택 안함" );
            m_pMonsterIndex->SetText( strIndex.str() );
        }
        else
        {
            const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[m_iSelectMonsterIndex - 1];

            std::wstringstream strLevel;
            std::wstringstream strIndex;
            std::wstringstream strAiFileName;
            strLevel << m_iMonsterLevel << L" (BaseLV:" << kMonsterTemplate.iBaseLevel << L")" ;
            strIndex << L"몬스터선택    " << m_iSelectMonsterIndex << "/" << (int)g_kMonsterManager.m_vecMonsterTable.size();
            strAiFileName << kMonsterTemplate.strAIFileName.c_str();

            m_pMonsterName->SetText( g_pkStrLoader->GetString( kMonsterTemplate.iNameID ) );
            m_pMonsterAiFileName->SetText( strAiFileName.str() );
            m_pMonsterLevel->SetText( strLevel.str() );
            m_pMonsterIndex->SetText( strIndex.str() );
        }
    }
    #endif
}

DWORD KGCTutorialMonsterSetting::GetChampionProperty()
{
    DWORD dwChampionPropery = 0x00000000;
    for( int i = 0; i < CHAMPION_PROPERTY_SIZE; ++i )
    {
        if( m_apkCheck[i]->IsRenderOn() )
        {
            dwChampionPropery = dwChampionPropery | m_adwProperty[i];
        }
    }
    return dwChampionPropery;
}
