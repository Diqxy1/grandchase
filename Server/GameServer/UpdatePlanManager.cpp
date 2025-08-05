#include "UpdatePlanManager.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "Lua/KLuaManager.h"
#include "UpdatePlanScriptID.h"
#include "GameServer.h"
#include "GameDrop.h"
#include "Gawibawibo.h"
#include "ErrandManager.h"
#include "ItemManager.h"
#include "EclipseTimeEvent.h"
#include "RKTornado.h"
#include "Songkran.h"
#include "AccumulateAttendance.h"
#include "StrengthManager.h"
#include "GachaManager.h"
#include "DicePlay.h"
#include "HeroDungeonManager.h"

ImplementSingleton( KUpdatePlanManager );
ImplOstmOperatorW2A( KUpdatePlanManager );
NiImplementRootRTTI( KUpdatePlanManager );

KUpdatePlanManager::KUpdatePlanManager( void )
{
}

KUpdatePlanManager::~KUpdatePlanManager( void )
{
}

ImplToStringW( KUpdatePlanManager )
{
    //KLocker lock( m_csICCUpdatePlan );

    // 출력 해볼만한 정보가 있는가?
    return START_TOSTRINGW;
        //<< TOSTRINGW( m_mapUpdatePlan.size() );
}

bool KUpdatePlanManager::UpdateClientContentsOpenInfoNotFromCenter( IN KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csICCUpdatePlan );

    if ( false == kPacket_.m_prGameCategoryInfo.first.empty() ) {
        m_kEventClientContentsOpenInfoNot.m_prGameCategoryInfo = kPacket_.m_prGameCategoryInfo;
    }

    if ( false == kPacket_.m_prGameCharInfo.first.empty() ) {
        m_kEventClientContentsOpenInfoNot.m_prGameCharInfo = kPacket_.m_prGameCharInfo;
    }

    if ( false == kPacket_.m_prMonsterInfo.first.empty() ) {
        m_kEventClientContentsOpenInfoNot.m_prMonsterInfo = kPacket_.m_prMonsterInfo;
    }

    if ( false == kPacket_.m_prMiniEventInfo.first.empty() ) {
        m_kEventClientContentsOpenInfoNot.m_prMiniEventInfo = kPacket_.m_prMiniEventInfo;
    }

    return true;
}

bool KUpdatePlanManager::UpdateClientContentsFirstInitInfoNotFromCenter( IN KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csICCUpdatePlan );

    if ( false == kPacket_.m_prUsingFilesInfo.first.empty() ) {
        m_kEventClientContentsFirstInitInfoNot.m_prUsingFilesInfo = kPacket_.m_prUsingFilesInfo;
    }

    if ( false == kPacket_.m_prHackCheckInfo.first.empty() ) {
        m_kEventClientContentsFirstInitInfoNot.m_prHackCheckInfo = kPacket_.m_prHackCheckInfo;
    }

    return true;
}

bool KUpdatePlanManager::UpdateItemBuyInfoNotFromCenter( IN KItemBuyInfo& kPacket_ )
{
    KLocker lock( m_csItemBuyInfo );

    m_kItemBuyInfo = kPacket_;

    return true;
}

bool KUpdatePlanManager::UpdateEventStateListFromCenter( IN KEVENT_UPDATE_EVENT_STATE_NOT& kPacket_ )
{
    KLocker lock( m_csEventUpdatePlan );

    m_vecEventUpdatePlan = kPacket_;

    return true;
}

bool KUpdatePlanManager::UpdateScriptFilesFromCenter( IN KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_, OUT std::vector< int >& vecEventID_ )
{
    KECN_PLAN_SCRIPT_UPDATE_NOT::iterator vitScript;
    vecEventID_.clear();

    for ( vitScript = kPacket_.begin(); vitScript != kPacket_.end(); vitScript++ ) {
        switch ( vitScript->first ) {
            case SCI_INITDROP:
                _JIF( 0 == lua_dofile( SiKGameServer()->GetLuaState(), vitScript->second.c_str() ), return false );
                break;
            case SCI_GAMEDROP:
                _JIF( true == SiKGameDrop()->LoadMonsterScript( vitScript->second ), return false );
                break;
            case SCI_GAWIBAWIBO:
                _JIF( true == SiKGawibawibo()->Get_Gawibawibo_UserData_FromDB( vitScript->second ), return false );
                break;
            case SCI_MANUFACTURE:
                _JIF( true == SiKItemManager()->m_kItemManufacture.LoadScript( vitScript->second ), return false );
                vecEventID_.push_back( SCI_MANUFACTURE );
                break;
            case SCI_ERRAND:
                _JIF( true == SiKErrandManager()->Get_Errand_UserData_FromDB( vitScript->second ), return false );
                break;
            case SCI_ECLIPSE_TIME:
                _JIF( true == SiKEclipseTimeEvent()->SendEclipseTimeEventInfoNot( vitScript->second ), return false );
                break;
            case SCI_RK_TORNADO:
                _JIF( true == SiKRKTornado()->SendRKTornadoItemListNot(), return false );
                break;
            case SCI_SONGKRAN:
                _JIF( true == SiKSongkran()->Get_Songkran_UserData_FromDB( vitScript->second ), return false );
                break;
            case SCI_ACCUMULATE_ATTENDANCE:
                _JIF( true == SiKAccumulateAttendanceManager()->GetAccumulateAttendanceUserData_FromDB( vitScript->second ), return false );
                break;
            case SCI_ENCHANT:
                _JIF( 0 == lua_dofile( SiKGameServer()->GetLuaState(), vitScript->second.c_str() ), return false );
                break;
            case SCI_STRENGTH:
                _JIF( true == SiKStrengthManager()->_LoadScript( vitScript->second ), return false );
                break;
            case SCI_GACHA:
                _JIF( true == SiKGachaManager()->_LoadScript( vitScript->second ), return false );
                _JIF( true == SiKGachaManager()->SendUserGachaDataGetFromDBNot( vitScript->second ), return false );
                _JIF( true == SiKGachaManager()->SendGachaDataChangeNot( vitScript->second ), return false );
                break;
            case SCI_DICE_BOARD:
                _JIF( true == SiKDicePlay()->Get_DicePlay_UserData_FromDB( vitScript->second ), return false );
                break;
            case SCI_HERO_DUNGEON:
                _JIF( SiKHeroDungeonManager()->_LoadScript( vitScript->second ), return false );
                break;
            default:
                START_LOG( cerr, "EScriptID 에 설정 된 enum값이 아니다" )
                    << BUILD_LOG( vitScript->first )
                    << END_LOG;
                break;
        }
    }

    return true;
}