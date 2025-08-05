#include "StdAfx.h"
#include "KGCStageLoadCompleteSign.h"


IMPLEMENT_CLASSNAME( KGCStageLoadCompleteSign );
IMPLEMENT_WND_FACTORY( KGCStageLoadCompleteSign );
IMPLEMENT_WND_FACTORY_NAME( KGCStageLoadCompleteSign, "gc_stage_load_complete_sign" );

KGCStageLoadCompleteSign::KGCStageLoadCompleteSign(void)
{
    char str[MAX_PATH] = {0,};
    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
    {                
        m_pkCompleteSign[i] = NULL;
        sprintf_s( str, "load_complete_sign%d", i );
        LINK_CONTROL( str, m_pkCompleteSign[i] );

        m_pkUserNickName[i] = NULL;
        sprintf_s( str, "player_login_id%d", i );
        LINK_CONTROL( str, m_pkUserNickName[i] );

        m_pkBack[i] = NULL;
        sprintf_s( str, "background%d", i );
        LINK_CONTROL( str, m_pkBack[i] );
    } 
}

KGCStageLoadCompleteSign::~KGCStageLoadCompleteSign(void)
{
}

void KGCStageLoadCompleteSign::OnCreate( void )
{
    InitInfo();
}

void KGCStageLoadCompleteSign::InitInfo()
{
    m_vecPlayerUID.clear();
    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
    {
        m_pkBack[i]->InitState(false);
        m_pkCompleteSign[i]->InitState(false);        
        m_pkCompleteSign[i]->Lock(false);
        m_pkUserNickName[i]->SetAlign(DT_CENTER);
        m_pkUserNickName[i]->SetText(L"");
    }
}

void KGCStageLoadCompleteSign::FrameMoveInEnabledState( void )
{

}

void KGCStageLoadCompleteSign::CheckPlayerLoadComplete( DWORD dwUID_ )
{
    int iIndex = -1;
    for( int i = 0; i < static_cast<int>(m_vecPlayerUID.size()); i++ )
    {
        if( m_vecPlayerUID[i] == dwUID_ )
        {
            iIndex = i;
            break;
        }
    }

    if( iIndex == -1 || iIndex >= MAX_DUNGEON_PLAYER_NUM )
        return;

    m_pkCompleteSign[iIndex]->Lock(true);    
}

void KGCStageLoadCompleteSign::SetUserInfo( DWORD dwUID_ )
{
    m_vecPlayerUID.push_back(dwUID_);

    int index = m_vecPlayerUID.size() - 1;
    if( index >= 0 && index < MAX_DUNGEON_PLAYER_NUM )
    {
        m_pkBack[index]->ToggleRender(true);
        m_pkCompleteSign[index]->ToggleRender(true);
        m_pkCompleteSign[index]->Lock(false);

        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(dwUID_);   
        m_pkUserNickName[index]->SetText(L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx");
    }
}

void KGCStageLoadCompleteSign::CheckAllPlayerLoadComplete()
{
    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; i++ )
    {
        m_pkCompleteSign[i]->Lock(true);        
    }
}