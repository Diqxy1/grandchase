#include "ItemBreakUp.h"
#include "CommonPacket.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "ItemManager.h"
#include <boost/random.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KItemBreakUp::KItemBreakUp(void)
:m_fPriceRatio(0)
,m_fMaterialRatioMin(1.f)
,m_fMaterialRatioMax(1.f)
{
}

KItemBreakUp::~KItemBreakUp(void)
{
}


bool KItemBreakUp::LoadScript()
{
    return _LoadScript(std::string());
}

bool KItemBreakUp::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitBreakUp.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    float fMaterialRatioMin = 1.f;
    float fMaterialRatioMax = 1.f;
    std::map<int, GCITEMID> mapMaterialItemID;
    std::map<PAIR_INT, std::map<int,int> > mapMaterialInfo;
    std::map< PAIR_INT, std::vector< KDropItemInfo > >  mapSpecificBreakupInfo;

    _JIF( kLuaMng.GetValue( "MaterialRatioMin", fMaterialRatioMin ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaterialRatioMax", fMaterialRatioMax ) == S_OK, return false );
    _JIF( LoadMaterialItemID( kLuaMng, mapMaterialItemID ), return false );
    _JIF( LoadMaterialInfo( kLuaMng, mapMaterialInfo ), return false );
    _JIF( LoadSpecificBreakupInfo( kLuaMng, mapSpecificBreakupInfo ), return false );

    START_LOG( cerr, L"아이템 해체 정보 로드 완료." )
        << BUILD_LOG( fMaterialRatioMin )
        << BUILD_LOG( fMaterialRatioMax )
        << BUILD_LOG( mapMaterialItemID.size() )
        << BUILD_LOG( mapMaterialInfo.size() )
        << BUILD_LOG( mapSpecificBreakupInfo.size() ) << END_LOG;

    {
        KLocker lock( m_csBreakUp );
        m_fMaterialRatioMin = fMaterialRatioMin;
        m_fMaterialRatioMax = fMaterialRatioMax;
        m_mapMaterialItemID.swap( mapMaterialItemID );
        m_mapMaterialInfo.swap( mapMaterialInfo );
        m_mapSpecificBreakupInfo.swap( mapSpecificBreakupInfo );
    }
    return true;
}

void KItemBreakUp::SetExceptListFromDB( IN OUT std::set<GCITEMID>& setExceptList_ )
{
    START_LOG( cerr, L"아이템 해체 제외 아이템 리스트 Size : " << setExceptList_.size() ) << END_LOG;
    KLocker lock( m_csBreakUp );
    m_setExceptList.swap( setExceptList_ );
}

bool KItemBreakUp::LoadMaterialItemID( IN OUT KLuaManager& kLuaMng_, OUT std::map<int, GCITEMID>& mapMaterialItemID_ )
{
    mapMaterialItemID_.clear();

    _JIF( kLuaMng_.BeginTable( "MaterialItemID" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nIndex = -1;
        GCITEMID itemID = 0;

        if ( kLuaMng_.GetValue( i, itemID ) != S_OK ) {
            break;
        }

        if ( itemID <= 0 ) {
            continue;
        }

        nIndex = mapMaterialItemID_.size();
        if ( false == mapMaterialItemID_.insert( std::make_pair( nIndex, itemID ) ).second ) {
            START_LOG( cerr, L" 재료 번호에 중복이 있습니다. Material Index: " << nIndex ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KItemBreakUp::LoadCountRatio( IN OUT KLuaManager& kLuaMng_, OUT std::map<int,int>& mapCountRatio_ )
{
    mapCountRatio_.clear();

    for ( int i = 3 ; ; ++i ) {

        int nCountRatio = -1;
        if ( kLuaMng_.GetValue( i, nCountRatio ) != S_OK ) {
            break;
        }

        if ( nCountRatio > 0 ) {
            int nIndex = i - 3;
            mapCountRatio_.insert( std::make_pair( nIndex, nCountRatio ) );
        }
    }
}

bool KItemBreakUp::LoadMaterialInfo( IN OUT KLuaManager& kLuaMng_, OUT std::map<PAIR_INT, std::map<int,int> >& mapMaterial_ )
{
    mapMaterial_.clear();

    _JIF( kLuaMng_.BeginTable( "MaterialCountRatio" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {

        if ( kLuaMng_.BeginTable( i ) != S_OK ) {
            break;
        }

        PAIR_INT prItemLvGrade;
        prItemLvGrade.first = -1; // 아이템 레벨
        prItemLvGrade.second = -1; // 아이템 등급

        if ( kLuaMng_.GetValue( 1, prItemLvGrade.first ) != S_OK ) {
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            continue;
        }

        if ( kLuaMng_.GetValue( 2, prItemLvGrade.second ) != S_OK ) {
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            continue;
        }

        std::map<int,int> mapCountRatio;
        LoadCountRatio( kLuaMng_, mapCountRatio );

        if ( prItemLvGrade.first < 0 || prItemLvGrade.second < 0 ) {
            continue;
        }

        if ( false == mapMaterial_.insert( std::make_pair( prItemLvGrade, mapCountRatio ) ).second ) {
            START_LOG( cerr, L" 아이템 레벨/등급에 중복이 있습니다. ItemLv : " << prItemLvGrade.first << L", ItemGrade : " << prItemLvGrade.second ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}


bool KItemBreakUp::CheckBreakUpEnable( IN const KItem& kItem_ )
{
    if ( kItem_.m_nPeriod > 0 ) return false;
    if ( kItem_.m_nCount > 1 ) return false;

    return !IsInExceptList( kItem_.m_ItemID );
}

bool KItemBreakUp::BreakUpItem( IN const KItem& kItem_, OUT std::vector<KDropItemInfo>& vecDropMaterial_ )
{
    vecDropMaterial_.clear();

    DWORD dwEquipLevel = 0;
    _JIF( SiKItemManager()->GetItemEquipLevel( kItem_.m_ItemID, dwEquipLevel ), return false );
    if( dwEquipLevel == 0 ) {
        dwEquipLevel = static_cast<DWORD>( kItem_.m_sEquipLevel );
    }

    // GetSpecificDrop()에서 해체 리스트를 찾은 경우 GetMaterialDrop()은 처리하지 않는다
    if ( false == GetSpecificDrop( static_cast< int >( kItem_.m_ItemID ), static_cast< int >( kItem_.m_cGradeID ), vecDropMaterial_ ) ) {
        _JIF( GetMaterialDrop( static_cast< int >( dwEquipLevel ), static_cast< int >( kItem_.m_cGradeID ), vecDropMaterial_ ), return false );
    }
    return true;
}

bool KItemBreakUp::IsInExceptList( IN const GCITEMID iteID_ )
{
    KLocker lock( m_csBreakUp );
    return (m_setExceptList.find( iteID_ ) != m_setExceptList.end());
}

bool KItemBreakUp::GetMaterialDrop( IN const int& nItemEquipLv_, IN const int& nItemGrade_, OUT std::vector<KDropItemInfo>& vecDropMaterial_ )
{
    PAIR_INT prKey( nItemEquipLv_, nItemGrade_ );

    KLocker lock( m_csBreakUp );

    std::map< PAIR_INT, std::map<int,int> >::const_iterator cmitLvGrade;
    cmitLvGrade = m_mapMaterialInfo.find( prKey );
    if ( cmitLvGrade == m_mapMaterialInfo.end() ) {
        START_LOG( cwarn, L"아이템 레벨/등급에 해당하는 해체 정보가 없음. (" << prKey.first << L"," << prKey.second << L")" )
            << BUILD_LOG( nItemEquipLv_ )
            << BUILD_LOG( nItemGrade_ ) << END_LOG;
        return false;
    }

    std::map<int,int>::const_iterator cmit;
    for ( cmit = cmitLvGrade->second.begin() ; cmit != cmitLvGrade->second.end() ; ++cmit ) {
        float fCount = std::ceil( cmit->second * GetRandomNum( m_fMaterialRatioMin, m_fMaterialRatioMax ) );
        fCount = std::max<float>(0,fCount);

        if ( fCount <= 0 ) {
            continue;
        }

        GCITEMID MaterialItemID = GetMaterialItemID( cmit->first );
        if ( MaterialItemID <= 0 ) {
            continue;
        }

        KDropItemInfo kItem;
        kItem.m_ItemID = MaterialItemID;
        kItem.m_nDuration = static_cast<int>( fCount );
        kItem.m_nPeriod = KItem::UNLIMITED_ITEM;

        vecDropMaterial_.push_back( kItem );

        if ( vecDropMaterial_.size() >= MATERIAL_MAX ) {
            break;
        }
    }

    return true;
}

GCITEMID KItemBreakUp::GetMaterialItemID( IN const int& nIndex_ )
{
    std::map<int, GCITEMID>::const_iterator cmit;
    cmit = m_mapMaterialItemID.find( nIndex_ );
    if ( cmit == m_mapMaterialItemID.end() ) {
        return 0;
    }

    return cmit->second;
}

float KItemBreakUp::GetRandomNum( IN const float& fMin_, IN const float& fMax_ )
{
    static boost::lagged_fibonacci607 generator;
    static boost::uniform_real<float> uni_dist( fMin_, fMax_ );
    static boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_real<float> > uni(generator, uni_dist);

    return uni();
}

void KItemBreakUp::DumpMaterialList()
{
    //KLocker lock( m_csBreakUp );

    //std::cout << L"--- Dump Material List (size:" << m_mapMaterialInfo.size() << L") ---" << dbg::endl;
    //std::map< PAIR_INT, std::map<int,int> >::const_iterator cmitLvGrade;
    //for ( cmitLvGrade = m_mapMaterialInfo.begin() ; cmitLvGrade != m_mapMaterialInfo.end() ; ++cmitLvGrade ) {
    //    std::cout << std::tab << L"Lv : " << cmitLvGrade->first.first << L", Grade : " << cmitLvGrade->first.second << L", Material size : " << cmitLvGrade->second.size() << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KItemBreakUp::DumpMaterialInfo( IN const int nItemLv_, IN const int nItemGrade_ )
{
    PAIR_INT prKey( nItemLv_, nItemGrade_ );

    KLocker lock( m_csBreakUp );

    std::map< PAIR_INT, std::map<int,int> >::const_iterator cmitLvGrade;
    cmitLvGrade = m_mapMaterialInfo.find( prKey );
    if ( cmitLvGrade == m_mapMaterialInfo.end() ) {
        START_LOG( cerr, L"아이템 레벨/등급에 해당하는 해체 정보가 없음. (" << prKey.first << L"," << prKey.second << L")" )
            << BUILD_LOG( nItemLv_ )
            << BUILD_LOG( nItemGrade_ ) << END_LOG;
        return;
    }

    //std::cout << L"--- Dump Material Info (" << prKey.first << L"," << prKey.second << L") (size:" << cmitLvGrade->second.size() << L") ---" << dbg::endl;
    //std::map<int,int>::const_iterator cmitMaterial;
    //for ( cmitMaterial = cmitLvGrade->second.begin() ; cmitMaterial != cmitLvGrade->second.end() ; ++cmitMaterial ) {
    //    std::cout << std::tab << L"Material Index : " << cmitMaterial->first << L", Count : " << cmitMaterial->second << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

bool KItemBreakUp::LoadSpecificBreakupInfo( IN KLuaManager& kLuaMng_, OUT std::map< PAIR_INT, std::vector< KDropItemInfo > >& mapSpecificBreakupInfo_ )
{
    mapSpecificBreakupInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "SpecificBreakup" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {

        if ( kLuaMng_.BeginTable( i ) != S_OK ) {
            break;
        }

        PAIR_INT prItemIDGrade;
        prItemIDGrade.first = -1; // 아이템 ID
        prItemIDGrade.second = -1; // 아이템 등급

        _JIF( kLuaMng_.BeginTable( "Input" ) == S_OK, return false );

        _JIF( kLuaMng_.GetValue( 1, prItemIDGrade.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prItemIDGrade.second ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // Input table


        std::vector< KDropItemInfo > vecResultMaterial;
        vecResultMaterial.clear();

        _JIF( kLuaMng_.BeginTable( "Output" ) == S_OK, return false );

        for ( int j = 1; ; ++j ) {
            if ( kLuaMng_.BeginTable( j ) != S_OK ) {
                break;
            }

            int nGradeID = -1;
            KDropItemInfo kDropItemInfo;
            _JIF( kLuaMng_.GetValue( 1, kDropItemInfo.m_ItemID ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( 2, kDropItemInfo.m_nPeriod ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( 3, kDropItemInfo.m_nDuration ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( 4, nGradeID ) == S_OK, return false );

            kDropItemInfo.m_cGradeID = static_cast< int >( nGradeID );

            vecResultMaterial.push_back( kDropItemInfo );

            _JIF( kLuaMng_.EndTable() == S_OK, return false ); // j table
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // Output table

        if ( false == mapSpecificBreakupInfo_.insert( std::make_pair( prItemIDGrade, vecResultMaterial ) ).second ) {
            START_LOG( cerr, L"아이템 아이디/등급에 중복이 있습니다. ItemID: " << prItemIDGrade.first << L", ItemGrade: " << prItemIDGrade.second ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // i table
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // SpecificBreakup table

    return true;
}

bool KItemBreakUp::GetSpecificDrop( IN const int& nItemID_, IN const int& nItemGrade_, OUT std::vector< KDropItemInfo >& vecDropMaterial_ )
{
    KLocker lock( m_csBreakUp );

    PAIR_INT prKey( nItemID_, nItemGrade_ );

    std::map< PAIR_INT, std::vector< KDropItemInfo > >::iterator mitItemIDGrade;
    mitItemIDGrade = m_mapSpecificBreakupInfo.find( prKey );
    if ( mitItemIDGrade == m_mapSpecificBreakupInfo.end() ) {
        START_LOG( clog, L"아이템 아이디/등급에 해당하는 해체 정보가 없습니다. ItemID: " << prKey.first << L", ItemGrade: " << prKey.second )
            << BUILD_LOG( nItemID_ )
            << BUILD_LOG( nItemGrade_ ) << END_LOG;
        return false;
    }

    vecDropMaterial_ = mitItemIDGrade->second;

    return true;
}

void KItemBreakUp::GetItemBreakupInfoAll( OUT KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kPacket_ )
{
    KLocker lock( m_csBreakUp );

    kPacket_.m_fMaterialRatioMin = m_fMaterialRatioMin;
    kPacket_.m_fMaterialRatioMax = m_fMaterialRatioMax;
    kPacket_.m_mapMaterialItemID = m_mapMaterialItemID;
    kPacket_.m_mapMaterialInfo = m_mapMaterialInfo;
    kPacket_.m_mapSpecificBreakupInfo = m_mapSpecificBreakupInfo;
}
