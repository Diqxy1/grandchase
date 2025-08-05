#include "CnSocketConsumeItem.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KCnSocketConsumeItem::KCnSocketConsumeItem(void)
:m_SocketOpenItem(0)
{}

KCnSocketConsumeItem::~KCnSocketConsumeItem(void)
{
}

void KCnSocketConsumeItem::GetTableInfo( OUT KENU_SOCKET_TABLE_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csConsume );

    kPacket_.m_mapCashItemUseCount = m_mapItemUseCount;
    kPacket_.m_mapConsumeGP = m_mapConsumeGP;
    kPacket_.m_SocketOpenItemID = m_SocketOpenItem;
}

void KCnSocketConsumeItem::GetTableInfo( OUT KECN_SOCKET_ITEM_GP_TABLE_NOT& kPacket_ )
{
    KLocker lock( m_csConsume );

    kPacket_.m_mapItemUseCount = m_mapItemUseCount;
    kPacket_.m_mapConsumeGP = m_mapConsumeGP;
}

bool KCnSocketConsumeItem::LoadScript_Item()
{
    KLocker lock( m_csConsume );

    KLuaManager luaMgr;

    _JIF( luaMgr.DoFile( "ItemSocketFomula.lua" ) == S_OK, return false );
    _JIF( luaMgr.GetValue( "SocketOpenItem", m_SocketOpenItem ) == S_OK, return false );

    return true;
}

bool KCnSocketConsumeItem::LoadScript()
{
    KLuaManager luaMgr;
    std::map<DWORD, KSocketConsumeInfo > mapCashItemUseCount;
    _JIF( luaMgr.DoFile( "ItemSocketFomula.lua" ) == S_OK, return false );

    _JIF( luaMgr.BeginTable( "SocketOpenItemUseNum" ) == S_OK, return false );

    int iIndex = 0;
    while ( ++iIndex ) {
        if ( luaMgr.BeginTable( iIndex ) != S_OK ) {
            break;
        }

        DWORD Level = 0;
        _JIF( luaMgr.GetValue( 1, Level ) == S_OK, return false );

        DWORD NormalCount = 0;
        DWORD RareCount = 0;
        DWORD EpicCount = 0;
        DWORD LegendCount = 0;
        DWORD MysticCount = 0;

        _JIF( luaMgr.GetValue( 2, NormalCount ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 3, RareCount ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 4, EpicCount ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 5, LegendCount ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 6, MysticCount ) == S_OK, return false );

        KSocketConsumeInfo sItemCount;
        sItemCount.m_dwNormal = NormalCount;
        sItemCount.m_dwRare = RareCount;
        sItemCount.m_dwEpic = EpicCount;
        sItemCount.m_dwLegend = LegendCount;
        sItemCount.m_dwMystic = MysticCount;

        mapCashItemUseCount.insert( std::map<DWORD, KSocketConsumeInfo>::value_type( Level, sItemCount ) );

        luaMgr.EndTable();
    }
    luaMgr.EndTable();

    KLocker lock( m_csConsume );
    m_mapItemUseCount.swap( mapCashItemUseCount );

    return true;
}

void KCnSocketConsumeItem::GetCashItemNum( IN int EquipLevel_, IN int ItemLevel_, OUT int& ItemCount_ )
{
    std::map<DWORD, KSocketConsumeInfo>::iterator mit;

    KLocker lock( m_csConsume );
    mit = m_mapItemUseCount.find( EquipLevel_ );

    if ( mit == m_mapItemUseCount.end() ) {
        return;    
    }

    KSocketConsumeInfo sItemCount = mit->second;

    switch( ItemLevel_ ) {
        case GRADE_NORMAL:
            ItemCount_ = sItemCount.m_dwNormal;
            break;
        case GRADE_RARE:
            ItemCount_ = sItemCount.m_dwRare;
            break;
        case GRADE_EPIC:
            ItemCount_ = sItemCount.m_dwEpic;
            break;
        case GRADE_LEGEND:
            ItemCount_ = sItemCount.m_dwLegend;
            break;
        case GRADE_MYSTIC:
            ItemCount_ = sItemCount.m_dwMystic;
            break;
        default:
            ItemCount_ = 0;
    }

}

bool KCnSocketConsumeItem::LoadScript_GP()
{
    KLuaManager luaMgr;
    std::map< DWORD, KSocketConsumeInfo > mapConsumeGP;

    _JIF( luaMgr.DoFile( "ItemSocketFomula.lua" ) == S_OK, return false );

    _JIF( luaMgr.BeginTable( "GPConsume" ) == S_OK, return false );

    int iIndex = 0;
    while( ++iIndex ) {
        if( luaMgr.BeginTable( iIndex ) != S_OK ) {
            break;
        }

        DWORD Level = 0;
        _JIF( luaMgr.GetValue( 1, Level ) == S_OK, return false );

        DWORD NormalPrice = 0;
        DWORD RarePrice = 0;
        DWORD EpicPrice = 0;
        DWORD LegendPrice = 0;
        DWORD MysticPrice = 0;

        _JIF( luaMgr.GetValue( 2, NormalPrice  ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 3, RarePrice ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 4, EpicPrice ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 5, LegendPrice ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 6, MysticPrice ) == S_OK, return false );

        KSocketConsumeInfo sGP;
        sGP.m_dwNormal = NormalPrice;
        sGP.m_dwRare = RarePrice;
        sGP.m_dwEpic = EpicPrice;
        sGP.m_dwLegend = LegendPrice;
        sGP.m_dwMystic = MysticPrice;

        mapConsumeGP.insert( std::map<DWORD, KSocketConsumeInfo>::value_type( Level, sGP ) );

        luaMgr.EndTable();
    }

    luaMgr.EndTable();

    KLocker lock( m_csConsume );
    m_mapConsumeGP.swap( mapConsumeGP );
    return true;
}


void KCnSocketConsumeItem::GetConsumeGP( IN int EquipLevel_, IN int ItemLevel_, OUT int& GP_ )
{
    KLocker lock( m_csConsume );
    std::map<DWORD, KSocketConsumeInfo>::iterator mit;
    
    mit = m_mapConsumeGP.find( EquipLevel_ );

    if ( mit == m_mapConsumeGP.end() ) {
        return;    
    }

    KSocketConsumeInfo sGP = mit->second;

    switch( ItemLevel_ ) {
        case GRADE_NORMAL:
            GP_ = sGP.m_dwNormal;
            break;
        case GRADE_RARE:
            GP_ = sGP.m_dwRare;
            break;
        case GRADE_EPIC:
            GP_ = sGP.m_dwEpic;
            break;
        case GRADE_LEGEND:
            GP_ = sGP.m_dwLegend;
            break;
        case GRADE_MYSTIC:
            GP_ = sGP.m_dwMystic;
            break;
        default:
            GP_ = 0;
    }    
}
