#include "ChoiceDrop.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KChoiceDrop );
ImplOstmOperatorW2A( KChoiceDrop );
NiImplementRootRTTI( KChoiceDrop );

KChoiceDrop::KChoiceDrop(void)
{
}

KChoiceDrop::~KChoiceDrop(void)
{
}

ImplToStringW( KChoiceDrop )
{
    KLocker lock( m_csChoiceDrop );

    START_TOSTRINGW
        << TOSTRINGW( m_setBoxItemID.size() )
        << TOSTRINGW( m_mapeBoxList.size() );
    return stm_;
}

bool KChoiceDrop::LoadScript()
{
    return _LoadScript(std::string());
}

bool KChoiceDrop::_LoadScript( OUT std::string& strScript_ )
{
    std::set<GCITEMID> setBoxItemID;
    std::map<GCITEMID,KChoiceBox> mapBoxList;
    KLuaManager kLuaMng;

    const std::string strScriptName = "InitChoiceDrop.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "BoxList" ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {
        KChoiceBox kBox;
        kBox.m_BoxItemID = 0;
        kBox.m_usDropCount = 0;

        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "BoxItemID", kBox.m_BoxItemID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "DropCount", kBox.m_usDropCount ) == S_OK, return false );
        _JIF( LoadBoxInfo( kLuaMng, kBox.m_mapMaterial ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if ( kBox.m_BoxItemID == 0 ) continue;
        if ( kBox.m_usDropCount == 0 ) continue;
        if ( kBox.m_usDropCount > kBox.m_mapMaterial.size() ) {
            START_LOG( cwarn, L"드랍개수가 최대 마테리얼 수보다 많음.. BoxID : " << kBox.m_BoxItemID )
                << BUILD_LOG( kBox.m_usDropCount )
                << BUILD_LOG( kBox.m_mapMaterial.size() ) << END_LOG;

            kBox.m_usDropCount = kBox.m_mapMaterial.size();
        }

        _JIF( setBoxItemID.insert( kBox.m_BoxItemID ).second, return false );
        _JIF( mapBoxList.insert( std::map<GCITEMID,KChoiceBox>::value_type(kBox.m_BoxItemID,kBox) ).second, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load Choice Drop script." )
        << BUILD_LOG( setBoxItemID.size() )
        << BUILD_LOG( mapBoxList.size() ) << END_LOG;

    KLocker lock( m_csChoiceDrop );
    m_setBoxItemID.swap( setBoxItemID );
    m_mapeBoxList.swap( mapBoxList );
    return true;
}

bool KChoiceDrop::LoadBoxInfo( KLuaManager& kLuaMng_, OUT std::map<USHORT, KDropItemInfo>& mapList_ )
{
    mapList_.clear();
    int nIndex = 0;

    _JIF( kLuaMng_.BeginTable( "DropItem" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KDropItemInfo kItem;
        int nGradeID = -1;
        nIndex = mapList_.size();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        kItem.m_cGradeID = static_cast< char >( nGradeID );

        _JIF( kItem.m_ItemID > 0, return false );
        _JIF( mapList_.insert( std::map<USHORT, KDropItemInfo>::value_type( nIndex, kItem ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KChoiceDrop::DumpInfo()
{
    //KLocker lock( m_csChoiceDrop );

    //std::cout << L"-- Dump Choice Drop (size:" << m_mapeBoxList.size() << L") --" << dbg::endl;

    //std::map<GCITEMID,KChoiceBox>::const_iterator cmit;
    //for ( cmit = m_mapeBoxList.begin() ; cmit != m_mapeBoxList.end() ; ++cmit ) {

    //    std::cout << L"BoxID : " << cmit->second.m_BoxItemID
    //              << L", DropCount : " << cmit->second.m_usDropCount
    //              << L", MaterialCount : " << cmit->second.m_mapMaterial.size() << dbg::endl;

    //    std::map<USHORT, KDropItemInfo>::const_iterator cmitMaterial;
    //    for ( cmitMaterial = cmit->second.m_mapMaterial.begin() ; cmitMaterial != cmit->second.m_mapMaterial.end() ; ++cmitMaterial ) {
    //        std::cout << std::tab << cmitMaterial->first
    //            << L" : " << cmitMaterial->second.m_ItemID
    //            << L", " << cmitMaterial->second.m_nDuration
    //            << L", " << cmitMaterial->second.m_nPeriod << dbg::endl;
    //    }
    //    std::cout << dbg::endl;
    //}

    //std::cout << L"-- End Dump --" << dbg::endl;
}

void KChoiceDrop::GetBoxList( OUT std::set<GCITEMID>& setList_ )
{
    KLocker lock( m_csChoiceDrop );
    setList_ = m_setBoxItemID;
}

bool KChoiceDrop::GetBoxInfo( IN const GCITEMID& BoxItemID_, OUT KChoiceBox& kInfo_ )
{
    KLocker lock( m_csChoiceDrop );

    std::map<GCITEMID,KChoiceBox>::const_iterator cmit;
    cmit = m_mapeBoxList.find( BoxItemID_ );
    _JIF( cmit != m_mapeBoxList.end(), return false );

    kInfo_ = cmit->second;
    return true;
}