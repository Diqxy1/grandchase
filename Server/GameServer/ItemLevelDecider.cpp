#include "ItemLevelDecider.h"
#include <dbg/dbg.hpp>
#include "ItemLevelTable.h"
#include "Log4.h"

KItemLevelDecider::KItemLevelDecider(void)
{
}

KItemLevelDecider::~KItemLevelDecider(void)
{
}

bool KItemLevelDecider::LoadScript()
{
    return _LoadScript(std::string());
}

bool KItemLevelDecider::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    const std::string strScriptName = "ItemLevelTable.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    std::map< int, LevelTable >    mapItemLevelInfo;
    _JIF( LoadItemLevelTable( kLuaMng, mapItemLevelInfo ), return false );

    START_LOG( cerr, L"아이템 레벨 스크립트 로드 완료. size " << mapItemLevelInfo.size() ) << END_LOG;
    {
        KLocker lock( m_csLevelDecider );
        m_mapItemLevelInfo.swap( mapItemLevelInfo );

        return true;
    }
}

bool KItemLevelDecider::LoadItemLevelTable( IN KLuaManager& kLuaMng_, OUT std::map< int, LevelTable >& mapItemLevelInfo_ )
{
    mapItemLevelInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "ItemLevelTable" ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        LevelTable kLevelTable;
        int nLevelTypeNum;
        _JIF( kLuaMng_.GetValue( "LevelTypeNum", nLevelTypeNum ) == S_OK, return false );

        if ( kLevelTable.LoadScript( nLevelTypeNum, kLuaMng_ ) == false ) {
            break;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapItemLevelInfo_.insert( std::make_pair( nLevelTypeNum, kLevelTable) ).second ) {
            START_LOG( cerr, L"중복된 레벨 Type 있음.. Type : " << nLevelTypeNum ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KItemLevelDecider::GetItemLevelTable( IN int nTableIndex_, OUT LevelTable& kItemLevelTable_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nTableIndex_ );

    if( mit == m_mapItemLevelInfo.end() ) return false;
    kItemLevelTable_ = mit->second;

    return true;
}

bool KItemLevelDecider::GetItemLevelList( IN int nTableIndex_, OUT std::set<int>& setLevelList_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nTableIndex_ );

    if( mit == m_mapItemLevelInfo.end() ) return false;

    return mit->second.GetLevelList( setLevelList_ );
}

KItemLevelDecider::LevelTable::ItemLevelTableType KItemLevelDecider::GetTableType( IN int nTableIndex_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nTableIndex_ );

    if( mit == m_mapItemLevelInfo.end() ) {
        return KItemLevelDecider::LevelTable::ILT_INVAILD;
    }

    return mit->second.GetItemLevelType();
}

bool KItemLevelDecider::GetItemLevel( IN int nTableIndex_, OUT int& nLevel_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nTableIndex_ );

    if( mit == m_mapItemLevelInfo.end() ) {
        return false;
    }

    return mit->second.GetItemLevel( nLevel_ );
}

bool KItemLevelDecider::CheckItemLevel( IN const int nTableIndex_, IN const int nLevel_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nTableIndex_ );

    if( mit == m_mapItemLevelInfo.end() ) {
        return false;
    }

    return mit->second.CheckItemLevel( nLevel_ );
}

bool KItemLevelDecider::CheckItemLevelAttribute( IN const int nLevelTableID_, IN OUT int& nAttributeID_, IN OUT int& nItemLevel_, IN const int nLevelTableType_ )
{
    if( KItemLevelDecider::LevelTable::ILT_INVAILD == nLevelTableID_ ) {
        return true;
    }
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nLevelTableID_ );
    if( mit == m_mapItemLevelInfo.end() ) {
        START_LOG( cerr, L"LevelTableID가 스크립트에 없음. nLevelTableID" << nLevelTableID_ ) << END_LOG;
        return false;
    }

    // 설정된 ItemLV값 가져오기.
    // RandomType과 SelectType의 구분이 필요하다.
    // RandomType은 확률설정한 대로 Level값 계산.
    // SelectType은 인자로 받아온 Level값 사용.
    if( KItemLevelDecider::LevelTable::ILT_RANDOM == nLevelTableType_ ) {
        if( !GetItemLevel( nLevelTableID_, nItemLevel_ ) ) {
            START_LOG( cerr, L"LevelTableID에 레벨 설정 없음. nLevelTableID" << nLevelTableID_ ) << END_LOG;
            return false;
        }
    }

    // 설정된 ItemTable 정보 가져오기.
    LevelTable kLevelTable;
    if( !GetItemLevelTable( nLevelTableID_, kLevelTable ) ) {
        START_LOG( cerr, L"LevelTable정보 읽어오기 실패. nLevelTableID" << nLevelTableID_ ) << END_LOG;
        return false;
    }

    if( !kLevelTable.GetItemAttributeNum( nItemLevel_, nAttributeID_ ) ) {
        START_LOG( cerr, L"해당 Level의 속성정보가 없네." )
            << BUILD_LOG( nLevelTableID_ )
            << BUILD_LOG( nItemLevel_ ) << END_LOG;
        return false;
    }
    return true;
}

bool KItemLevelDecider::IsSelectiveType( IN const int nLevelTableID_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nLevelTableID_ );

    if( mit == m_mapItemLevelInfo.end() ) {
        return false;
    }

    return mit->second.IsSelectiveType();
}

bool KItemLevelDecider::IsRandomType( IN const int nLevelTableID_ )
{
    KLocker lock( m_csLevelDecider );

    std::map< int, LevelTable >::iterator mit;
    mit = m_mapItemLevelInfo.find( nLevelTableID_ );

    if( mit == m_mapItemLevelInfo.end() ) {
        return false;
    }

    return mit->second.IsRandomType();
}