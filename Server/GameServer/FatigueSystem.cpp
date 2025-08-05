#include "FatigueSystem.h"
#include <KncUtil.h>
#include <KncUtilLua.h> // luabind::object
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KFatigueSystem )
ImplToStringW( KFatigueSystem )
{
    
    stm_    << TOSTRINGWb( m_bEnableRewardLimit )
            << TOSTRINGW( m_nClearAccOfflineTime )
            << TOSTRINGW( m_nThresholdTime )
            << TOSTRINGW( m_nUnlimitedTime )
            << TOSTRINGW( m_nLimitedTime );

    stm_ << L"--- PenaltyTable --- " << std::endl << L"{" << std::endl;
    std::map< int, KFatigueTable >::const_iterator mit;

    {// locking scope
        KLocker lock( m_csPenaltyTable );
        for( mit = m_mapFatigueTable.begin() ; mit != m_mapFatigueTable.end() ; ++mit )
        {
            stm_ << L"   { " << mit->second.m_nBeginMin << L", " << mit->second.m_nEndMin << L", " << mit->second.m_nRatio << L" }" << std::endl;        
        }
    }

    stm_ << L"}" << std::endl;
    return stm_;
}

KFatigueSystem::KFatigueSystem(void)
:
m_bEnableRewardLimit( false ),
m_nClearAccOfflineTime( 0 ),
m_nThresholdTime( 0 ),
m_nUnlimitedTime( 0 ),
m_nLimitedTime( 0 )
{
    m_mapFatigueTable.clear();
}

KFatigueSystem::~KFatigueSystem(void)
{
    m_mapFatigueTable.clear();
}

void KFatigueSystem::SetRewardLimit(  bool bEnableRewardLimit_,
                                    int nClearAccOfflineTime_,
                                    int nThresholdTime_,
                                    int nUnlimitedTime_,
                                    int nLimitedTime_,
                                    const luabind::object& table_ )
{
    JIF( table_.type() == LUA_TTABLE );

    m_bEnableRewardLimit    = bEnableRewardLimit_;
    m_nClearAccOfflineTime  = nClearAccOfflineTime_;
    m_nThresholdTime        = nThresholdTime_;
    m_nUnlimitedTime        = nUnlimitedTime_;
    m_nLimitedTime          = nLimitedTime_;

    KLocker lock( m_csPenaltyTable );

    m_mapFatigueTable.clear();
    luabind::object::iterator it;
    for( it = table_.begin(); it != table_.end(); ++it )
    {
        KFatigueTable fatigueTable;
        luabind::object dataTable = *it;
        JIF( dataTable.type() == LUA_TTABLE );
        fatigueTable.m_nType     = KncUtil::GetLuaData( dataTable, 1, 0 );
        fatigueTable.m_nBeginMin = KncUtil::GetLuaData( dataTable, 2, 0 );
        fatigueTable.m_nEndMin   = KncUtil::GetLuaData( dataTable, 3, 0 );
        fatigueTable.m_nRatio    = KncUtil::GetLuaData( dataTable, 4, 0 );
        fatigueTable.m_nPeriod   = KncUtil::GetLuaData( dataTable, 5, 0 );

        //m_vecPenaltyTable.push_back( fatigueTable );
        m_mapFatigueTable[fatigueTable.m_nType] = fatigueTable;
    }
}

void KFatigueSystem::GetInfo( OUT std::map< int, KFatigueTable >& mapTable_, 
                             OUT int& nMaxOnlineTime_,
                             OUT int& nResetOfflineTime_ )
{
    KLocker lock( m_csPenaltyTable );
    mapTable_ = m_mapFatigueTable;
    nMaxOnlineTime_ = m_nLimitedTime;
    nResetOfflineTime_ = m_nClearAccOfflineTime;
}



int KFatigueSystem::GetRewardRatio( int m_nAccOnlineTime_, const std::wstring& strLogin_ )
{
    if( !m_bEnableRewardLimit || m_nAccOnlineTime_ < m_nUnlimitedTime )
        return 100;

    if( m_nLimitedTime <= m_nAccOnlineTime_ )
        return 0;
    
    std::map< int, KFatigueTable >::iterator mit;
    KLocker lock( m_csPenaltyTable );
    for( mit = m_mapFatigueTable.begin() ; mit != m_mapFatigueTable.end() ; ++mit )
    {
        // Ex. m_nBeginMin < CurrentAccMin <= m_nEndMin
        if( mit->second.m_nBeginMin < m_nAccOnlineTime_ && m_nAccOnlineTime_ <= mit->second.m_nEndMin )
            return mit->second.m_nRatio;
    }

    //이상황에서는 어떻게 해야 할까..........? 로그 찍고 패널티를 주지 말자..

    std::wstringstream stm;

    stm << TOSTRINGW( strLogin_ )
        << TOSTRINGW( m_nAccOnlineTime_ )
        << TOSTRINGWb( m_bEnableRewardLimit )
        << TOSTRINGW( m_nClearAccOfflineTime )
        << TOSTRINGW( m_nThresholdTime )
        << TOSTRINGW( m_nUnlimitedTime )
        << TOSTRINGW( m_nLimitedTime );

    stm << L"--- PenaltyTable --- " << std::endl << L"{" << std::endl;
    for( mit = m_mapFatigueTable.begin() ; mit != m_mapFatigueTable.end() ; ++mit )
    {
        stm << L"   { " << mit->second.m_nBeginMin << L", " << mit->second.m_nEndMin << L", " << mit->second.m_nRatio << L" }" << std::endl;        
    }

    stm << L"}" << std::endl;

    START_LOG( cerr,L" 게임 제한 오류" )
        << stm.str() << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return 100;
}

ImplOstmOperatorW2A( KFatigueSystem );