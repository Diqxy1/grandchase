#include "stdafx.h"
#include "GCDungeonRankingClearData.h"

IMPLEMENT_CLASSNAME( KGCDungeonRankingClearData );
IMPLEMENT_WND_FACTORY( KGCDungeonRankingClearData );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonRankingClearData, "gc_dungeonrankingcleardata" );

KGCDungeonRankingClearData::KGCDungeonRankingClearData( void )
{
    m_pkClearNoData = NULL;
    m_pkClearBack = NULL;

    LINK_CONTROL( "no_data", m_pkClearNoData );
    LINK_CONTROL( "data_back", m_pkClearBack );

    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EM_STAGE_NUM; ++i ) 
    {
        m_pkStageClear[i] = NULL;
        m_pkStageClearText[i] = NULL;

        sprintf( str, "top%d", i );  
        LINK_CONTROL( str, m_pkStageClear[i] );


        sprintf( str, "static_time%d", i );  
        LINK_CONTROL( str, m_pkStageClearText[i] );
    }
}

KGCDungeonRankingClearData::~KGCDungeonRankingClearData( void )
{
}

void KGCDungeonRankingClearData::OnCreate( void )
{
    m_pkClearNoData->InitState( true );
    m_pkClearBack->InitState( false );

    for ( int i = 0; i < EM_STAGE_NUM; ++i ) 
    {
        m_pkStageClear[i]->InitState( false );
        m_pkStageClearText[i]->InitState( true, true, this );
        m_pkStageClearText[i]->SetAlign( DT_CENTER );
    }
}


void KGCDungeonRankingClearData::ActionPerformed( const KActionEvent& event )
{
}

void KGCDungeonRankingClearData::UpdateUI()
{
    std::map< int, KDungeonPersonalRecord > mapDungeonPersonalRecord;

    g_kGlobalValue.m_kHeroDungeonInfo.GetDungeonPersonalRecord( GC_GM_QUEST81, mapDungeonPersonalRecord );

    if( !mapDungeonPersonalRecord.size() )
    {
        m_pkClearBack->ToggleRender( false );
        m_pkClearNoData->ToggleRender( true );        

        for ( int i = 0; i < EM_STAGE_NUM; ++i )
        {
            m_pkStageClear[i]->ToggleRender( false );
            m_pkStageClearText[i]->SetText( L"" );
        }
    }
    else
    {
        m_pkClearBack->ToggleRender( true );
        m_pkClearNoData->ToggleRender( false );

        for ( int i = 0; i < EM_STAGE_NUM; ++i ) 
        {
            std::map< int, KDungeonPersonalRecord >::iterator iter = mapDungeonPersonalRecord.find( i );

            if ( iter == mapDungeonPersonalRecord.end() )
            {
                m_pkStageClear[i]->ToggleRender( false );
                m_pkStageClearText[i]->SetText( L"" );
            }
            else
            {
                KDungeonPersonalRecord kRecord = iter->second;

                m_pkStageClear[i]->ToggleRender( true );                

                std::wstring wstrGrade, wstrClearData;
                GCFUNC::GetRankGradeText( kRecord.m_nControlRank, wstrGrade );                
                wstrGrade = g_pkStrLoader->GetReplacedString( STR_ID_RANK_TEXT, "l", wstrGrade );

                int nMin = ( kRecord.m_dwStageClearTime / 1000 ) / 60;
                int nSec = ( kRecord.m_dwStageClearTime / 1000 ) % 60;

                wstrClearData = boost::str( boost::wformat( L"%s (%02d : %02d)" )%wstrGrade%nMin%nSec );                
                m_pkStageClearText[i]->SetText( wstrClearData );
            }
        }
    }
}

