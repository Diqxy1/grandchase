#include "stdafx.h"
#include "KGCPrivateDungeonManager.h"
#include "KGCWorldMapManager.h"
#include "GCStateRoom.h"
#include "gcui/KGCWorldMapContainer.h"
#include "KGCRoomSceneS6.h"

ImplementSingleton( KGCPrivateDungeonManager )

KGCPrivateDungeonManager::KGCPrivateDungeonManager()
{
    Init();
}

KGCPrivateDungeonManager::~KGCPrivateDungeonManager()
{
}


void KGCPrivateDungeonManager::Init()
{
    m_mapPrivateDungeonInfo.clear();
    m_mapPrivateDungeonList.clear();
    m_emState = EM_MOVE_STATE_NONE;
    m_bNextLevel = false;
}

bool KGCPrivateDungeonManager::LoadScript( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("PrivateDungeonInfo",return false)
    {
        for(int i = 1; ; i++)
        {
            LUA_BEGIN_TABLE(i,break)
            {
                int iCharID = GC_CHAR_INVALID;
                int iStartDungeon = GC_GM_INVALID;
                int iWorldMap = NUM_WORLDMAP;

                
                LUA_GET_VALUE( "CHAR_ID", iCharID, return false );
                LUA_GET_VALUE( "START_DUNGEON", iStartDungeon, return false );
                LUA_GET_VALUE( "MOVE_WORLDMAP", iWorldMap, return false );


                SCharPrivateDungeon sPrivateDungeon;
                sPrivateDungeon.m_nCharType = iCharID;
                sPrivateDungeon.m_emStartDungeon = static_cast<EGCGameMode>( iStartDungeon );
                sPrivateDungeon.m_emWorldMap = static_cast<EGCWorldMap>( iWorldMap );

                LUA_BEGIN_TABLE("DUNGEON_LIST",return false)
                {                    

                    for(int i = 1; ; i++)
                    {
                        LUA_BEGIN_TABLE(i,break)
                        {

                            int iGameMode = GC_GM_INVALID;
                            LUA_GET_VALUE( "DUNGEON_ID", iGameMode, return false );

                            std::vector<int> vecLevel;

                            LUA_BEGIN_TABLE_RAII("DUNGEON_LEVEL")
                            {
                                int iLoop = 1, iTemp;
                                while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                                {
                                    vecLevel.push_back( iTemp );
                                }   
                            }

                            EGCGameMode eGameMode = static_cast<EGCGameMode>(iGameMode);

                            std::map<EGCGameMode, std::vector<int>>::iterator iter = sPrivateDungeon.m_mapPrivateDungeon.find( eGameMode );

                            // 중복된 데이터가 있음.
                            if ( iter != sPrivateDungeon.m_mapPrivateDungeon.end() || vecLevel.empty() )
                            {
                                MessageBox( NULL, L"PrivateDungeonInfo Duplication DungeonID", L"", MB_OK );
                            }
                            else
                            {
                                sPrivateDungeon.m_mapPrivateDungeon.insert( std::pair<EGCGameMode, std::vector<int>>( eGameMode, vecLevel ));
                            }


                            std::map<EGCGameMode, int>::iterator iter2 = m_mapPrivateDungeonList.find( eGameMode );
                            if ( iter2 != m_mapPrivateDungeonList.end() )
                            {
                                MessageBox( NULL, L"PrivateDungeonInfo Duplication DungeonID", L"", MB_OK );
                            }
                            else
                            {
                                m_mapPrivateDungeonList.insert( std::pair<EGCGameMode,int>( eGameMode, iCharID ) );
                            }                            

                        }
                        LUA_END_TABLE(return false)
                    }
                }
                LUA_END_TABLE(return false)

                std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );

                // 중복된 데이터가 있음.
                if ( iter != m_mapPrivateDungeonInfo.end() )
                {
                    MessageBox( NULL, L"PrivateDungeonInfo Duplication CharID", L"", MB_OK );
                }
                else
                {
                    m_mapPrivateDungeonInfo.insert( std::pair<int, SCharPrivateDungeon>( iCharID, sPrivateDungeon ));
                }

            }
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)


    return true;
}


//  캐릭터 전용 던전인가?
bool KGCPrivateDungeonManager::IsPrivateDungeon( EGCGameMode eGameMode_ )
{
    std::map<EGCGameMode, int>::iterator iter = m_mapPrivateDungeonList.find( eGameMode_ );

    if ( iter != m_mapPrivateDungeonList.end() )
        return true;
    else 
        return false;


    return false;
}

// 누구의 전용던전인지 확인
int KGCPrivateDungeonManager::GetPrivateDungeonChar( EGCGameMode eGameMode_ )
{
    std::map<EGCGameMode, int>::iterator iter = m_mapPrivateDungeonList.find( eGameMode_ );
    if ( iter == m_mapPrivateDungeonList.end() )
        return GC_CHAR_INVALID;
    else 
        return iter->second;
}

//  해당 캐릭터가 전용 던전이 있는 캐릭터 인지 체크    
bool KGCPrivateDungeonManager::IsHavePrivateDungeon( int iCharID )
{
    std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );
    if ( iter != m_mapPrivateDungeonInfo.end() )
        return true;
    else 
        return false;
}

//  해당 캐릭터가 전용 던전을 졸업했는지 체크.( 임시로 쓸 예정. 나중에 서버 올릴 내용 )
bool KGCPrivateDungeonManager::IsClearPrivateDungeon( int iCharID )
{
    std::set<int>::iterator iter_set = m_setNoClearChar.find( iCharID );
    if ( iter_set != m_setNoClearChar.end() )
        return false;

    return true;

//  서버에서 준걸로 체크한다. 나중에 혹시 문제 생기면 해당 코드 사용하자.
//     std::map<int,KDungeonUserInfo>& mapStagePlayable_ = g_kGlobalValue.m_kUserInfo.mapStagePlayable;
// 
//     std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );
// 
//     if ( iter == m_mapPrivateDungeonInfo.end() )
//         return true;
// 
//     std::map<EGCGameMode, std::vector<int>>& mapClearDungeon = iter->second.m_mapPrivateDungeon;
//     std::map<EGCGameMode, std::vector<int>>::iterator iter_dun = mapClearDungeon.begin();
// 
//     for ( ; iter_dun != mapClearDungeon.end(); iter_dun++ )
//     {
// 
//         std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(iter_dun->first) );
//         if ( pos_map == mapStagePlayable_.end() ) 
//             return false;        
// 
//         std::vector<int>& vecLevel = iter_dun->second;
//         std::vector<int>::iterator iter_level = vecLevel.begin();
// 
//         for ( ; iter_level != vecLevel.end();  iter_level++ )
//         {
//             KDungeonUserInfo& kDungeonUserInfo = pos_map->second;
// 
//             if ( !kDungeonUserInfo.m_kModeDifficulty.Get( *iter_level ) )
//                 return false;
//         }
//     }
// 
// 
//     return true;
}


bool KGCPrivateDungeonManager::IsFirstDungeonChar( int iCharID )
{
    std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );

    if ( iter == m_mapPrivateDungeonInfo.end() )
        return false;

    return iter->second.m_bStartDungeonCheck;
}

void KGCPrivateDungeonManager::SetFirstDungeonChar( int iCharID, bool bSet )
{
    std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );

    if ( iter == m_mapPrivateDungeonInfo.end() )
        return;

    iter->second.m_bStartDungeonCheck = bSet;    
}


void KGCPrivateDungeonManager::CheckFirstDungeon( int iCharID )
{
    if( iCharID == GC_CHAR_INVALID )
    {
        std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.begin();
        std::map<int, SCharPrivateDungeon>::iterator iter_end = m_mapPrivateDungeonInfo.end();

        for ( ; iter != m_mapPrivateDungeonInfo.end(); ++iter )
        {
            SCharPrivateDungeon& sPrivateDungeon = iter->second;

            //  최초 던전 플레이 기록이 없다.
            if ( !SiKGCWorldMapManager()->IsPlayedDungeon( sPrivateDungeon.m_emStartDungeon, -1, g_kGlobalValue.m_kUserInfo.mapStagePlayable ) )
            {
                sPrivateDungeon.m_bStartDungeonCheck = true;
            }
            else
            {
                sPrivateDungeon.m_bStartDungeonCheck = false;
            }
        }
    }
    else
    {
        std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find(iCharID);
        if ( iter == m_mapPrivateDungeonInfo.end() )
            return;

        SCharPrivateDungeon& sPrivateDungeon = iter->second;

        //  최초 던전 플레이 기록이 없다.
        if ( !SiKGCWorldMapManager()->IsPlayedDungeon( sPrivateDungeon.m_emStartDungeon, -1, g_kGlobalValue.m_kUserInfo.mapStagePlayable ) )
        {
            sPrivateDungeon.m_bStartDungeonCheck = true;
        }
        else
        {
            sPrivateDungeon.m_bStartDungeonCheck = false;
        }

    }
}

EGCGameMode KGCPrivateDungeonManager::GetStartDungeon( int iCharID )
{
    std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );
    
    if ( iter == m_mapPrivateDungeonInfo.end() )
        return GC_GM_INVALID;

    return iter->second.m_emStartDungeon;
}

EGCWorldMap KGCPrivateDungeonManager::GetMoveWorldMap( int iCharID )
{
    std::map<int, SCharPrivateDungeon>::iterator iter = m_mapPrivateDungeonInfo.find( iCharID );

    if ( iter == m_mapPrivateDungeonInfo.end() )
        return NUM_WORLDMAP;

    return iter->second.m_emWorldMap;
}

void KGCPrivateDungeonManager::MoveStateCondition( EMMoveStateType emState )
{
    int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();
    if ( !IsHavePrivateDungeon( iCharType ) )
        return;

    if ( IsFirstDungeonChar( iCharType ) )
        m_emState = EM_CREATE_ROOM_TYPE;
    else
        m_emState = EM_MOVE_STATE_NONE;
}


void KGCPrivateDungeonManager::MoveStateRun( EMMoveStateType emState )
{
    switch ( emState )
    {
    case EM_CREATE_ROOM_TYPE:
        {
            int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();

            //  특정 조건에서만 발동해야된다. 즉, 캐릭터 선택해야할때만 이코드가 실행되야 된다.
            if ( m_emState == emState )
            {
                if ( IsFirstDungeonChar( iCharType ) )
                {
                    // 자동 방생성 해야됨.
                    std::wstring strRoomName = g_pkStrLoader->GetString( STR_ID_CREATE_DUNGEON_ROOM_TITLE1 );
                    EGCGameMode emGameMode = GetStartDungeon( iCharType );
                    
                    if ( emGameMode == GC_GM_INVALID )
                        return;

                    m_emState = EM_START_GAME_TYPE;

//                     //  월드맵 매니저에 현재 선택한 던전을 셋팅한다.
//                     int nIconMode = SiKGCWorldMapManager()->GetWorldMapIconNum( SiKGCWorldMapManager()->GetCurrentWorldMap(), emGameMode );
//                     SiKGCWorldMapManager()->m_iCurrentSelectDungeon = nIconMode;
                    
                    //  방생성 한다.
                    SiKGCRoomManager()->CreateRoomParty( strRoomName, L"",  emGameMode, 0, GC_GMC_DUNGEON , 0 , GC_GS_FOREST_OF_ELF ,  true );                    
                    g_pkUIScene->ToggleMessageBoxBack( false, true );
                }
            }

        }
        break;
    case EM_START_GAME_TYPE:
        {
            int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();

            if ( m_emState == emState )
            {
                // 게임 자동 시작해야됨.
                if ( IsFirstDungeonChar( iCharType ) )
                {
                    m_emState = EM_EXIT_ROOM_TYPE;
                    g_pStateRoom->OnGameStart();
                    g_pkUIScene->ToggleMessageBoxBack( false, true );
                }
            }
        }
        break;
    case EM_EXIT_ROOM_TYPE:
        {
            int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();

            if ( m_emState == emState )
            {                
                if ( IsFirstDungeonChar( iCharType ) )
                {                  
                    SetFirstDungeonChar( iCharType, false );

                    g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, false );
                }

                m_emState = EM_MOVE_STATE_NONE;
            }
            else if ( m_emState == EM_CLEAR_DUNGEON ) 
            {
                g_MyD3D->m_pStateMachine->ExitRoom( true, KFailRate::P2P_NO_ERROR, false );
            }
        }
        break;
    case EM_CLEAR_DUNGEON:
        {
            int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentCharType();

            if ( m_emState == emState )
            {                
                if ( IsClearPrivateDungeon( iCharType ) )
                {                  
                    //  졸업 처리
                    GraduateProcess();
                }

                m_emState = EM_MOVE_STATE_NONE;
            }
        }
        break;
    default:
        break;
    }
}


void KGCPrivateDungeonManager::SetClearRewardItem( std::vector<KItem>& vecRewardItem )
{
    m_vecRewardItem = vecRewardItem;
}

void KGCPrivateDungeonManager::GraduateProcess()
{
    // 졸업한 캐릭터 졸업 처리
    SiKGCWorldMapManager()->SetCurrentWorldMap( SiKGCPrivateDungeonManager()->GetMoveWorldMap( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) );
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    
    g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
    SiKGCWorldMapManager()->UpdateWorldMap();       


    g_MyD3D->m_OriginalEquip = g_MyD3D->m_TempPlayer.m_kUserInfo;

    // 장비 해제
    // 기본장비 먼저 벗김.
    g_pItemMgr->UnEquipBaseItem( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar() );            
    g_pItemMgr->UnEquipItem( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar() );
    // 장비 장착
    for(std::vector<KItem>::iterator vit = m_vecRewardItem.begin(); vit != m_vecRewardItem.end(); ++vit )
    {
        //여기로 UID가 들어옴
        GCItem* kItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10  );
        g_pItemMgr->EquipInventoryItem(&g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), vit->m_ItemUID );
    }

    g_MyD3D->SendEquipItemList( &g_MyD3D->m_TempPlayer );

    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

    m_vecRewardItem.clear();
}

void KGCPrivateDungeonManager::CheckNextLevel()
{
    if ( IsNextLevel() )
    {
        if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM && g_kGlobalValue.m_kUserInfo.bHost )
        {
            int nLevel = SiKGCRoomManager()->GetDungeonLevel();
            int nLastLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( SiKGCRoomManager()->GetGameMode(), g_kGlobalValue.m_kUserInfo.mapStagePlayable );

            if ( nLevel < nLastLevel ) 
            {
                SiKGCRoomManager()->ChangeDungeonGameMode( nLevel + 1 );
            }           
        }

        m_bNextLevel = false;
    }
}

void KGCPrivateDungeonManager::DelNoClearChar( int nCharType )
{
    std::set<int>::iterator iter = m_setNoClearChar.find( nCharType );
    if ( iter != m_setNoClearChar.end() )
    {
        m_setNoClearChar.erase( iter );
    }
}