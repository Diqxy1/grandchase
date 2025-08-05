#include "ItemLevelTable.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

KItemLevelTable::KItemLevelTable(void)
: m_nIndex( -1 )
,m_nTableType( -1 )
{
    m_kLottery.EnableState( KLottery::ENABLE_NO_FAILURE );
}

KItemLevelTable::~KItemLevelTable(void)
{
}

bool KItemLevelTable::IsRandomType()
{
    return m_nTableType == ILT_RANDOM;
}

bool KItemLevelTable::IsSelectiveType()
{
    return m_nTableType == ILT_SELECTIVE;
}

bool KItemLevelTable::IsFixedType()
{
    return m_nTableType == ILT_FIXED;
}

bool KItemLevelTable::LoadScript( IN int nIndex_, IN OUT KLuaManager& kLuaMng_ )
{
    _JIF( kLuaMng_.GetValue( "Type", m_nTableType ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "LevelRatioInfo" ) == S_OK, return false );

    m_nIndex = nIndex_;

    int i = 0;
    while( true ) {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        KItemLevelRatio kItemLevelRatio;

        if( kLuaMng_.GetValue( 1, kItemLevelRatio.m_nType ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 2, kItemLevelRatio.m_nAttributeNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItemLevelRatio.m_fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        m_vecLevelRatio.push_back( kItemLevelRatio );
        m_kLottery.AddCase( static_cast<int>(m_vecLevelRatio.size() - 1), kItemLevelRatio.m_fProb );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KItemLevelTable::GetLevelList( OUT std::set<int>& setLevelList_ )
{
    setLevelList_.clear();

    if( m_vecLevelRatio.empty() ) {
        return false;
    }

    for( int i = 0; i < static_cast<int>( m_vecLevelRatio.size() ); ++i ) {
        KItemLevelRatio& kLevelRatio = m_vecLevelRatio[i];
        setLevelList_.insert( kLevelRatio.m_nType );
    }
    return true;
}

bool KItemLevelTable::GetItemLevel( OUT int& nLevel_ )
{
    if( m_vecLevelRatio.empty() ) {
        return false;
    }

    if( GetItemLevelType() == ILT_FIXED ) { // 고정 레벨일 경우 100%설정된 레벨값을 반환.
        std::vector<KItemLevelRatio>::iterator vit;
        vit = m_vecLevelRatio.begin();
        nLevel_ =  vit->m_nType;
    } else {
        KLottery kLottery = m_kLottery;
        int nIndex = kLottery.Decision();
        KItemLevelRatio kLevelRatio = m_vecLevelRatio[nIndex];
        nLevel_ = kLevelRatio.m_nType;
    }
    return true;
}

bool KItemLevelTable::CheckItemLevel( IN const int nLevel_ )
{
    if( m_vecLevelRatio.empty() ) {
        return false;
    }
    std::vector<KItemLevelRatio>::iterator vit;
    for( vit = m_vecLevelRatio.begin(); vit != m_vecLevelRatio.end(); ++vit ) {
        if( vit->m_nType == nLevel_ ) {
            return true;
        }
    }
    return false;
}

bool KItemLevelTable::GetItemAttributeNum( IN const int nLevel, OUT int& nAttributeNum )
{
    std::vector<KItemLevelRatio>::iterator vit;
    for( vit = m_vecLevelRatio.begin(); vit != m_vecLevelRatio.end(); ++vit ) {
        if( nLevel == vit->m_nType ) {
            nAttributeNum = vit->m_nAttributeNum;
            return true;
        }
    }
    return false;
}