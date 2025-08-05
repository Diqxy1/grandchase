#include "stdafx.h"
#include "KGCTutorialMap.h"

IMPLEMENT_CLASSNAME( KGCTutorialMap );
IMPLEMENT_WND_FACTORY( KGCTutorialMap );
IMPLEMENT_WND_FACTORY_NAME( KGCTutorialMap, "gc_tutorial_map" );


KGCTutorialMap::KGCTutorialMap( void )
{
    m_iCurMapIndex = 0;
    
    LINK_CONTROL( "map_change_box", m_pkMapChangeBox );
}

KGCTutorialMap::~KGCTutorialMap( void ){}



void KGCTutorialMap::OnCreateComplete( void )
{
    m_mapWndMap.clear();
    
    KGCModeTutorial* pMode = GetTutorialMode();
    
    if ( pMode ) 
    {
        std::map< EGCGameStage, KTutorialMapInfo > mapTutorialInfo;
        pMode->GetTutoriaInfo( mapTutorialInfo );

        std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = mapTutorialInfo.begin();
        std::map< EGCGameStage, KTutorialMapInfo >::iterator pos_end = mapTutorialInfo.end();

        char szTemp[128];
        for ( ; pos != pos_end; ++pos )
        {
            sprintf(szTemp, "map_image%d", pos->second.nMapIndex);
            KD3DWnd* pWnd = GetChildByName( szTemp );
            
            if ( !pWnd )
                continue;

            pWnd->InitState( true, false, this );
            m_mapWndMap.insert( std::make_pair( pos->second.emGameStage, pWnd ) );

        }
    }  

    m_pkMapChangeBox->InitState( true, true, this );

    SelectMap( m_iCurMapIndex );
}

void KGCTutorialMap::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == this )
    {  return; }

    // 롤오버소리
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {  g_KDSound.Play( "30" ); }

    // 클릭소리
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN ||
        event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_DOWN )
    {  g_KDSound.Play( "31" ); }

    // 맵 변경 박스 이벤트
    if( event.m_pWnd == m_pkMapChangeBox )
    {
        int nMaxTutorialMap = GetTutorialMapSize();

        switch ( event.m_dwCode )
        {
        case KGCChangeBox::RIGHT_BUTTON:
            {
                m_iCurMapIndex++;
                if( m_iCurMapIndex >= nMaxTutorialMap )
                { m_iCurMapIndex = 0; }
                g_KDSound.Play( "31" );
                break;
            }
        case KGCChangeBox::LEFT_BUTTON:
            {
                m_iCurMapIndex--;
                if( m_iCurMapIndex < 0 )
                { m_iCurMapIndex = nMaxTutorialMap-1; }
                g_KDSound.Play( "31" );
                break;
            }
        }

        SelectMap( m_iCurMapIndex );
    }
}

KGCModeTutorial* KGCTutorialMap::GetTutorialMode()
{
    return static_cast<KGCModeTutorial*>( g_pStateGame->GetGameModePtr( GC_GMC_TUTORIAL ) );
}


// ¸E ¼±AA
void KGCTutorialMap::SelectMap(int iIndex)
{
	EGCGameStage emGameStage = GetSeqMapStage( iIndex );
    int nStringIdx = GetSeqMapStringID( iIndex );

    m_pkMapChangeBox->SetText( g_pkStrLoader->GetString( nStringIdx ) );

    std::map<EGCGameStage, KD3DWnd*>::iterator pos = m_mapWndMap.begin();
    std::map<EGCGameStage, KD3DWnd*>::iterator pos_end = m_mapWndMap.end();

    for( ; pos != pos_end; ++pos )
    {       
        pos->second->ToggleRender( pos->first == emGameStage );
    }
}

EGCGameStage KGCTutorialMap::GetSeqMapStage( int nIndex )
{
    KGCModeTutorial* pMode = GetTutorialMode();

    if ( !pMode ) 
        return GC_GS_FOREST_OF_ELF;

	return pMode->GetSeqMagStage( nIndex );
}

int KGCTutorialMap::GetSeqMapStringID( int nIndex )
{
    KGCModeTutorial* pMode = GetTutorialMode();

    if ( !pMode ) 
        return 0;

    return pMode->GetSeqMapStringID( nIndex );
}

int KGCTutorialMap::GetTutorialMapSize()
{
    KGCModeTutorial* pMode = GetTutorialMode();

    if ( !pMode ) 
        return 0;

    return pMode->GetTutorialMapSize();
}

EGCGameStage KGCTutorialMap::GetMapStage()
{   
	return GetSeqMapStage( m_iCurMapIndex );
};

int KGCTutorialMap::GetMapStringID()
{
	return GetSeqMapStringID( m_iCurMapIndex );
};