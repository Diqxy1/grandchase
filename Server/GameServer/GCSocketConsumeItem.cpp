#include "GCSocketConsumeItem.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KGCSocketConsumeItem::KGCSocketConsumeItem(void)
{}

KGCSocketConsumeItem::~KGCSocketConsumeItem(void)
{
}

void KGCSocketConsumeItem::GetCashItemNum( IN DWORD EquipLevel_, IN int ItemLevel_, OUT int& ItemCount_ )
{
    std::map<DWORD, KSocketConsumeInfo>::iterator mit;
    KLocker lock( m_csSocketInfo );

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


bool KGCSocketConsumeItem::GetConsumeGP( IN DWORD EquipLevel_, IN int ItemLevel_, OUT int& GP_ )
{
    std::map<DWORD, KSocketConsumeInfo>::iterator mit;
    KLocker lock( m_csSocketInfo );

    mit = m_mapConsumeGP.find( EquipLevel_ );

    if ( mit == m_mapConsumeGP.end() ) {
        return false;
    }

    switch( ItemLevel_ ) {
        case GRADE_NORMAL:
            GP_ = mit->second.m_dwNormal;
            break;
        case GRADE_RARE:
            GP_ = mit->second.m_dwRare;
            break;
        case GRADE_EPIC:
            GP_ = mit->second.m_dwEpic;
            break;
        case GRADE_LEGEND:
        case GRADE_MYSTIC:
            GP_ = mit->second.m_dwLegend;
            break;
        default:
            GP_ = 0;
            return false;
    }

    return true;
}

void KGCSocketConsumeItem::UpdateItemTable( IN KECN_SOCKET_ITEM_GP_TABLE_NOT& kPacket_ )
{
    KLocker lock( m_csSocketInfo );
    m_mapItemUseCount = kPacket_.m_mapItemUseCount;
    m_mapConsumeGP = kPacket_.m_mapConsumeGP;

    START_LOG( clog, L"Table Size :" << m_mapItemUseCount.size() ) << END_LOG;
}