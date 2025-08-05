#include "StdAfx.h"
#include "GCAchieveManager.h"

ImplementSingleton(KGCAchieveManager);

KGCAchieveManager::KGCAchieveManager(void)
{
}

KGCAchieveManager::~KGCAchieveManager(void)
{
}

void KGCAchieveManager::OccurAction(const SAchieve& arg_)
{
#if defined( ACHEIVE_SYSTEM )
	return;
#endif

#ifndef __PATH__
#undef _ENUM
#define _ENUM( e, desc ) \
//     case (SAchieve::e): LOG_TRACE( "log", #desc << LOG_VALUE( arg_.m_iCharType ) << LOG_VALUE(arg_.m_iParam) << LOG_VALUE(arg_.m_iParam2) ); break;
	//
	//
	//     switch( arg_.m_iActionID ) {
	// #include "GCAchieveDef.h"
	//     default:
	//         break;
	//     }
#endif

	lua_State* L = KGCLuabinder::getInstance().GetLuaState();
	CHECK_STACK(L);
	lua_checkstack(L, 10);                    // 스택 사이즈를 체크 합니다. ( 20보다 작으면 여유공간을 확보 합니다. )
	lua_pushstring(L, "AchieveProc");         // 함수 이름을 Push 합니다.
	lua_rawget(L, LUA_GLOBALSINDEX);          // 글로벌 테이블에서 함수 키를 찾습니다.

	if (lua_isfunction(L, -1) == false) {
		lua_pop(L, 1);
		return;
	}

	lua_tinker::push(L, arg_);
	if (lua_pcall(L, 1, LUA_MULTRET, 0) != 0) {
		std::string strError = lua_tostring(L, -1);
		START_LOG(cerr, strError);
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
}

void KGCAchieveManager::OccurAction(int iActionID_, int iCharType_, int iParam_ /*= -1*/, int iParam2_ /*= -1 */)
{
#if defined( ACHEIVE_SYSTEM )
	return;
#endif
	SAchieve achieve;
	achieve.m_iActionID = iActionID_;
	achieve.m_iParam = iParam_;
	achieve.m_iParam2 = iParam2_;
	achieve.m_iCharType = iCharType_;
	OccurAction(achieve);
}

void KGCAchieveManager::LoadScript()
{
#if defined( ACHEIVE_SYSTEM )
	return;
#endif
	BindToLua(KGCLuabinder::getInstance().GetLuaState());

	GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), "AchieveFunc.lua");
}

void KGCAchieveManager::BindToLua(lua_State* L)
{
#if defined( ACHEIVE_SYSTEM )
	return;
#endif
	lua_tinker::class_add<SAchieve>(L, "SAchieve");
	lua_tinker::class_mem<SAchieve>(L, "action", &SAchieve::m_iActionID);
	lua_tinker::class_mem<SAchieve>(L, "chartype", &SAchieve::m_iCharType);
	lua_tinker::class_mem<SAchieve>(L, "param", &SAchieve::m_iParam);
	lua_tinker::class_mem<SAchieve>(L, "param2", &SAchieve::m_iParam2);
	lua_tinker::class_con<SAchieve>(L, lua_tinker::constructor<>());

	lua_tinker::class_add<KGCAchieveManager>(L, "KGCAchieveManager_Class");
	lua_tinker::class_def<KGCAchieveManager>(L, "AddToCounter", &KGCAchieveManager::AddToCounter);
#ifndef __PATH__
	lua_tinker::class_def<KGCAchieveManager>(L, "GetActionDesc", &KGCAchieveManager::GetActionDesc);
#endif
	lua_tinker::decl(L, "AchieveMgr", this);
}

void KGCAchieveManager::AddToCounter(int iCounterID_)
{
	START_LOG(clog, "Achieve Counted : " << iCounterID_);
}

#ifndef __PATH__
const char* KGCAchieveManager::GetActionDesc(int iActionID_)
{
#undef _ENUM
#define _ENUM( e, desc ) \
    case (SAchieve::e): return #desc;

	switch (iActionID_) {
#include "GCAchieveDef.h"
	default:
		break;
	}

	return "There is no description for this action";
}
#endif