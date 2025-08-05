#include "CharDefault.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCharDefault );
ImplOstmOperatorW2A( KCharDefault );
NiImplementRootRTTI( KCharDefault );

KCharDefault::KCharDefault(void)
{
    m_mapCharDefaultDungeon.clear();
    m_mapCharDefaultItemInfo.clear();
    m_mapCharDefaultSkill.clear();
    m_mapCharDefaultSlotItemInfo.clear();
    m_mapCharCreateLimitLevel.clear();
}

KCharDefault::~KCharDefault(void)
{
}

ImplToStringW( KCharDefault )
{
    KLocker lock( m_csCharDefault );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapCharDefaultDungeon.size() )
        << TOSTRINGW( m_mapCharDefaultItemInfo.size() )
        << TOSTRINGW( m_mapCharDefaultSkill.size() )
        << TOSTRINGW( m_mapCharDefaultSlotItemInfo.size() )
        << TOSTRINGW( m_mapCharCreateLimitLevel.size() );
}

bool KCharDefault::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharDefault::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "CharDefaultInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    std::map< char, std::set<int> > mapCharDefaultDungeon;
    _JIF( LoadDefaultDengeon( kLuaMng, mapCharDefaultDungeon ), return false );
    std::map< char, VEC_REWARD > mapCharDefaultItemInfo;
    _JIF( LoadDefaultItemInfo( kLuaMng, mapCharDefaultItemInfo ), return false );
    std::map< char, std::set<int> > mapCharDefaultSkill;
    _JIF( LoadDefaultSkill( kLuaMng, mapCharDefaultSkill ), return false );

    std::map< char, VEC_REWARD > mapCharDefaultSlotItemInfo;
    _JIF( LoadDefaultSlotItemInfo( kLuaMng, mapCharDefaultSlotItemInfo ), return false );

    std::map< char, DWORD > mapCharCreateLimitLevel;
    _JIF( LoadCreateLimitLevel( kLuaMng, mapCharCreateLimitLevel ), return false );

    {
        KLocker lock( m_csCharDefault );
        m_mapCharDefaultDungeon.swap( mapCharDefaultDungeon );
        m_mapCharDefaultItemInfo.swap( mapCharDefaultItemInfo );
        m_mapCharDefaultSkill.swap( mapCharDefaultSkill );
        m_mapCharDefaultSlotItemInfo.swap( mapCharDefaultSlotItemInfo );
        m_mapCharCreateLimitLevel.swap( mapCharCreateLimitLevel );
    }

    START_LOG( cerr, L"KCharDefault 정보 읽기 완료." )
        << BUILD_LOG( m_mapCharDefaultDungeon.size() ) 
        << BUILD_LOG( m_mapCharDefaultItemInfo.size() ) 
        << BUILD_LOG( m_mapCharDefaultSkill.size() )
        << BUILD_LOG( m_mapCharDefaultSlotItemInfo.size() )
        << BUILD_LOG( m_mapCharCreateLimitLevel.size() )
        << END_LOG;

    return true;
}

bool KCharDefault::LoadDefaultDengeon( IN KLuaManager& kLuaMng_, OUT std::map< char, std::set<int> >& mapCharDefaultDungeon_ )
{
    mapCharDefaultDungeon_.clear();
    _JIF( kLuaMng_.BeginTable( "CharDefaultDungeon" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::set<int> setDungeonInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadSetInfo( kLuaMng_, std::string("DefaultDungeon"), setDungeonInfo ), return false );

        if( mapCharDefaultDungeon_.insert( std::make_pair( (char)nChar, setDungeonInfo ) ).second == false ) {
            START_LOG( cerr, L"기본 던전 세팅중 중복된 캐릭터 ID. CharID :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharDefault::LoadSetInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setInfo_ )
{
    setInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setInfo_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharDefault::CheckCharDungeonInfo( IN const char cChar_, OUT std::set<int>& setDungeonInfo_ )
{
    setDungeonInfo_.clear();

    KLocker lock( m_csCharDefault );
    std::map<char, std::set<int>>::iterator mit;
    mit = m_mapCharDefaultDungeon.find( cChar_ );
    if( mit == m_mapCharDefaultDungeon.end() ) return false;
    setDungeonInfo_ = mit->second;
    return true;
}

bool KCharDefault::LoadDefaultItemInfo( IN KLuaManager& kLuaMng_, OUT std::map< char, VEC_REWARD >& mapCharDefaultItemInfo_ )
{
    mapCharDefaultItemInfo_.clear();
    _JIF( kLuaMng_.BeginTable( "CharDefaultEquipItemInfo" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        VEC_REWARD vecItemInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("DefaultItem"), vecItemInfo ), return false );

        if( mapCharDefaultItemInfo_.insert( std::make_pair( (char)nChar, vecItemInfo ) ).second == false ) {
            START_LOG( cerr, L"기본 복장 세팅중 중복된 캐릭터 ID. CharID :" << nChar )
                << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharDefault::LoadDefaultSlotItemInfo( IN KLuaManager& kLuaMng_, OUT std::map< char, VEC_REWARD >& mapCharSlotItemInfo_ )
{
    mapCharSlotItemInfo_.clear();
    _JIF( kLuaMng_.BeginTable( "CharDefaultSlotItemInfo" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        VEC_REWARD vecItemInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("DefaultItem"), vecItemInfo ), return false );

        if( mapCharSlotItemInfo_.insert( std::make_pair( (char)nChar, vecItemInfo ) ).second == false ) {
            START_LOG( cerr, L"기본 슬롯 아이템 세팅중 중복된 캐릭터 ID. CharID :" << nChar )
                << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharDefault::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KCharDefault::GetDefaultItemInfo( IN const char& cCharType_, OUT VEC_REWARD& vecItemInfo_ )
{
    vecItemInfo_.clear();
    std::map< char, VEC_REWARD >::iterator mit;

    KLocker lock( m_csCharDefault );
    mit = m_mapCharDefaultItemInfo.find( cCharType_ );
    if( mit != m_mapCharDefaultItemInfo.end() ) {
        vecItemInfo_ = mit->second;
    }
}

void KCharDefault::GetDefaultSlotItemInfo( IN const char& cCharType_, IN OUT VEC_REWARD& vecItemInfo_ )
{
    vecItemInfo_.clear();
    std::map< char, VEC_REWARD >::iterator mit;

    KLocker lock( m_csCharDefault );
    mit = m_mapCharDefaultSlotItemInfo.find( cCharType_ );
    if ( mit != m_mapCharDefaultSlotItemInfo.end() ) {
        vecItemInfo_ = mit->second;
    }
}

bool KCharDefault::LoadDefaultSkill( IN KLuaManager& kLuaMng_, OUT std::map< char, std::set<int> >& mapCharDefaultSkill_ )
{
    mapCharDefaultSkill_.clear();
    _JIF( kLuaMng_.BeginTable( "CharDefaultSkillInfo" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        std::set<int> setInfo;
        _JIF( kLuaMng_.GetValue( "Char", nChar ) == S_OK, return false );
        _JIF( LoadSetInfo( kLuaMng_, std::string("DefaultSkillInfo"), setInfo ), return false );

        if( mapCharDefaultSkill_.insert( std::make_pair( (char)nChar, setInfo ) ).second == false ) {
            START_LOG( cerr, L"기본 스킬 세팅중 중복된 캐릭터 ID. CharID :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharDefault::CheckCharSkillInfo( IN const char cChar_, OUT std::set<int>& setSkillInfo_ )
{
    setSkillInfo_.clear();

    KLocker lock( m_csCharDefault );
    std::map<char, std::set<int>>::iterator mit;
    mit = m_mapCharDefaultSkill.find( cChar_ );
    if( mit == m_mapCharDefaultSkill.end() ) return false;
    setSkillInfo_ = mit->second;
    return true;
}

bool KCharDefault::LoadCreateLimitLevel( IN KLuaManager& kLuaMng_, OUT std::map< char, DWORD >& mapCharCreateLimitLevel_ )
{
    mapCharCreateLimitLevel_.clear();
    _JIF( kLuaMng_.BeginTable( "CharCreateLimitLevel" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        DWORD dwLimitLevel = 0;
        std::set<int> setDungeonInfo;
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "LimitLevel", dwLimitLevel ) == S_OK, return false );

        if( mapCharCreateLimitLevel_.insert( std::make_pair( (char)nChar, dwLimitLevel ) ).second == false ) {
            START_LOG( cerr, L"특정 레벨 캐릭터 보유하고 있어야 생성가능한 캐릭터 중복된 ID. CharID :" << nChar ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharDefault::CheckCharCreateEnable( IN const char cChar_, OUT DWORD& dwLevel_ )
{
    std::map< char, DWORD >::iterator mit;

    KLocker lock( m_csCharDefault );
    mit = m_mapCharCreateLimitLevel.find( cChar_ );
    if( mit == m_mapCharCreateLimitLevel.end() ) {
        return false;
    }

    dwLevel_ = mit->second;
    return true;
}
