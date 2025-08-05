#include "Actor.h"
#include "GCHelper.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <algorithm>
//#include "odbc/Odbc.h"
#include "SubThread.h" // DBJIF
#include "UserEvent.h"
#include "Actor.h"
#include "GSSimLayer.h"
#include <KncUtilLua.h>
#include "Rainbow.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGCHelper );
ImplOstmOperatorW2A( KGCHelper );
NiImplementRootRTTI( KGCHelper );


std::wostream& operator<< ( std::wostream& stm_, const std::pair< int,std::set<DWORD> >& data_ )
{
    stm_ << data_.first << L" : {";
    std::copy( data_.second.begin(), data_.second.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stm_, L", " ) );
    stm_ << L"}";

    return stm_;
}

std::wostream& operator<< ( std::wostream& stm_, const KGCHelper::sBonusData& data_ )
{
    return stm_ << L"{" << data_.nBegin << L"," << data_.nEnd << L"," << data_.nValue << L"}";
}


KGCHelper::KGCHelper(void)
:m_nMaxBonusCount(0)
,m_bHellItemCheckEnable(true)
,m_dwHellItemID(82050)
,m_nMaximumTicketNum(0)
,m_nConnectionHellItemCount(1)
,m_nAttendanceHellItemcount(4)
,m_nWeekAttendHellItemcount(0)
,m_nMonthAttendHellItemcount(0)
,m_bOnEvent(false)
,m_dwSnowCoinID(0)
,m_dwSnowCoinUID(0)
,m_dwSnowCoinTickGap(0)
,m_dwSnowCoinLastTick(0)
,m_bChatRecordEnable(false)
,m_nChatLogQueLimit(100)
,m_bChatRecordUserCheck(false)
,m_tmTourDonationBegin(0)
,m_tmTourDonationEnd(0)
,m_bCheckSendHellMode(false)
,m_bDungeonFreePass(false)
,m_bCheckEnable(false)
,m_bEnableInvenOverflow(false)
,m_bEndGameHellTicketDec(false)
,m_dwTournamentTicketID(0)
,m_tmNationBegin(1)
,m_tmNationEnd(1)
,m_fExpRatio(0.f)
,m_fGPRatio(0.f)
,m_nDungeonFreePassID(0)
,m_nMaxInvenCapacity(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_tmUserAuthBegin(1)
,m_tmUserAuthEnd(1)
,m_tmUserPurchaseEnd(1)
,m_bLimitUseEnable(false)
,m_tmNewCharBegin( time_t(NULL) )
,m_tmNewCharEnd( time_t(NULL) )
,m_tmCharLevelEventBegin(1)
,m_tmCharLevelEventEnd(1)
{
    m_vecBonusTable.clear();
    m_setBonusItem.clear();
    m_mapGroupItem.clear();
    m_setNonGroup.clear();
    m_setEmoticon.clear();
    m_mapConnectionMsg.clear();
    m_setHellItemFreeMode.clear();
    m_mapInvenExtendItemInfo.clear();
    m_mapCoupon.clear();
    m_mapCharacterCard.clear();
    m_mapVipItemInfo.clear();
    m_setChatRecordUserUID.clear();
    InitIndigoRank();
    // 대회서버 시간 체크 초기화.
    InitTournament();
    m_setStringIDList.clear();
    m_mapKairoNoticeItem.clear();
    m_vecRewardItem.clear();
    m_setPresentLimitItem.clear();
    m_mapCharRewardItem.clear();
}

KGCHelper::~KGCHelper(void)
{
}

ImplToStringW( KGCHelper )
{
    KLocker lock( m_csBonusTable );
    KLocker lock1( m_csQuickSlot );
    KLocker lock2( m_csConnectionMsg );
    KLocker lock3( m_csHellItemCount );
    KLocker lock4( m_csGCClub );
    KLocker lock5( m_csDungeonTicket );

    START_TOSTRINGW
        << TOSTRINGW( m_nMaxBonusCount )
        << TOSTRINGW( m_vecBonusTable.size() )
        << TOSTRINGW( m_setBonusItem.size() )
        << TOSTRINGW( m_mapGroupItem.size() )
        << TOSTRINGW( m_setNonGroup.size() )
        << TOSTRINGW( m_setEmoticon.size() )
        << TOSTRINGW( m_ltrMixRatio.GetCaseNum() )
        << TOSTRINGW( m_mapConnectionMsg.size() )
        << TOSTRINGW( m_dwHellItemID )
        << TOSTRINGW( m_nConnectionHellItemCount )
        << TOSTRINGW( m_nAttendanceHellItemcount )
        << TOSTRINGW( m_nWeekAttendHellItemcount )
        << TOSTRINGW( m_nMonthAttendHellItemcount )
        << TOSTRINGWb( m_bHellItemCheckEnable )
        << TOSTRINGW( m_setHellItemFreeMode.size() )
        << TOSTRINGWb( m_bGCClubExtensionEnable )
        << TOSTRINGW( m_mapGCClubItemID.size() )
        << TOSTRINGW( m_vecConnectBonusItem.size() )
        << TOSTRINGW( m_vecDayAttendBonusItem.size() )
        << TOSTRINGW( m_mapDungeonTicketList.size() )
        << TOSTRINGW( m_mapTicketItem.size() )
        << TOSTRINGW( m_setPresentLimitItem.size() );

    stm_ << L"BonusTable : ";
    std::copy( m_vecBonusTable.begin(), m_vecBonusTable.end(), 
        std::ostream_iterator<std::vector< sBonusData >::value_type,wchar_t>( stm_, L", " ) );
    stm_ << std::endl;

    stm_ << L"BonusItem : ";
    std::copy( m_setBonusItem.begin(), m_setBonusItem.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stm_, L", " ) );
    stm_ << std::endl;

    //---
    stm_ << L"GroupItem : " << std::endl;
    std::map< int,std::set<DWORD> >::const_iterator mit;
    for( mit = m_mapGroupItem.begin() ; mit != m_mapGroupItem.end() ; ++mit )
        stm_ << *mit << std::endl;
    
    stm_ << L"NonGroupItem : ";
    std::copy( m_setNonGroup.begin(), m_setNonGroup.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stm_, L", " ) );
    stm_ << std::endl;

    stm_ << L"EmoticonItem : ";
    std::copy( m_setEmoticon.begin(), m_setEmoticon.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stm_, L", " ) );
    stm_ << std::endl;

    return stm_;
}

bool KGCHelper::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGCHelper::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitGCHelper.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadBonusData( kLuaMng ), return false );
    _JIF( LoadQuickSlotData( kLuaMng ), return false );
    _JIF( LoadInvenExternData( kLuaMng ), return false );
    _JIF( LoadCouponInfo( kLuaMng ), return false );
    _JIF( LoadCharacterCard( kLuaMng ), return false );
    _JIF( LoadSnowCoinScript( kLuaMng ), return false );
    _JIF( LoadHellItemInfo( kLuaMng ), return false );
    _JIF( LoadVipItemInfo(strScript_), return false );
    _JIF( LoadChatRecordUserUID(strScript_), return false );
    _JIF( LoadVirtualCashCouponInfo( kLuaMng ), return false );
    _JIF( LoadGCClubExtension( kLuaMng ), return false );
    _JIF( LoadTourDonationScript( kLuaMng ), return false );
    _JIF( LoadTournamentData( kLuaMng ), return false );
    _JIF( LoadEventInfoScript( kLuaMng ), return false );
    _JIF( LoadAdditionalMobDropRatio(kLuaMng ), return false );
    _JIF( LoadUserAuthData(kLuaMng ), return false );
    _JIF( LoadNationRewardInfo(kLuaMng ), return false );
    _JIF( LoadDungeonTicketList( kLuaMng ), return false );
    _JIF( LoadKairoNoticeInfo( kLuaMng ), return false );
    _JIF( LoadExpPotionInfo(kLuaMng), return false );
    _JIF( LoadPresentLimitItem( kLuaMng ), return false );
    _JIF( LoadNewCharData( kLuaMng ), return false );
    _JIF( LoadAttendGetChar( kLuaMng ), return false );
    _JIF( LoadCharLevelEvent( kLuaMng ), return false );
    _JIF( LoadPVPCharDisable( kLuaMng ), return false );

    return true;
}

bool KGCHelper::LoadBonusData( KLuaManager& kLuaMng_ )
{
    std::vector< sBonusData > vecBonusTable;
    vecBonusTable.clear();
    int nMaxBonusCount = 0;
    std::set<DWORD> setBonusItemList;

    _JIF( 0 == kLuaMng_.GetValue( "MaxBonusData", nMaxBonusCount ) , return false );

    _JIF( kLuaMng_.BeginTable( "BonusItemList" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwItem = 0;
        if( S_OK != kLuaMng_.GetValue( i, dwItem ) ) break;
        setBonusItemList.insert( dwItem );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "BonusData" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        sBonusData bonusData;
        _JIF( 0 == kLuaMng_.GetValue( 1, bonusData.nBegin ) , return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, bonusData.nEnd ) , return false );
        _JIF( 0 == kLuaMng_.GetValue( 3, bonusData.nValue ) , return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecBonusTable.push_back( bonusData );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"보너스 데이터 읽기 완료. Size : " << vecBonusTable.size()
        << L", MaxBonusCount : " << nMaxBonusCount ) << END_LOG;

    KLocker lock( m_csBonusTable );
    m_nMaxBonusCount    = nMaxBonusCount;
    m_vecBonusTable.swap( vecBonusTable );
    m_setBonusItem.swap( setBonusItemList );
    return true;
}

int KGCHelper::GetRefillBonusPoint( int nLevel_, int nCurrentBonus_ )
{
    sBonusData bonusData;
    if( !FindBonusData( nLevel_, bonusData ) )
        return 0;

    return std::max<int>(bonusData.nValue - nCurrentBonus_, 0 );
}

bool KGCHelper::FindBonusData( int nKey_, sBonusData& bonusData_ )
{
    KLocker lock( m_csBonusTable );
    std::vector< sBonusData >::iterator vit;
    for( vit = m_vecBonusTable.begin() ; vit != m_vecBonusTable.end() ; ++vit )
    {
        if( vit->nBegin <= nKey_ && vit->nEnd >= nKey_ )
        {
            bonusData_ = *vit;
            return true;
        }
    }

    return false;
}

int KGCHelper::GetMaxBonusCount()
{
    KLocker lock( m_csBonusTable );
    return m_nMaxBonusCount;
}

bool KGCHelper::IsBonusItem( GCITEMID dwItemId_ )
{
    KLocker lock( m_csBonusTable );
    return m_setBonusItem.find( dwItemId_ ) != m_setBonusItem.end();
}


//-------------------------------//
bool KGCHelper::LoadQuickSlotData( KLuaManager& kLuaMng_ )
{
    std::map< int,std::set<DWORD> > mapGroupItem;
    std::set<DWORD> setNonGroup;
    std::set<DWORD> setEmoticon;
    KLottery ltrEmoticonMix;

    _JIF( LoadGroupItem( kLuaMng_, mapGroupItem ),        return false );
    _JIF( LoadNonGroupItem( kLuaMng_, setNonGroup ),      return false );
    _JIF( LoadEmotionItem( kLuaMng_, setEmoticon ),       return false );
    _JIF( LoadEmoticonMixData( kLuaMng_, ltrEmoticonMix), return false );

    START_LOG( cerr, L" 퀵 슬롯/ 이모티콘 정보 로드 완료." )
        << BUILD_LOG( mapGroupItem.size() )
        << BUILD_LOG( setNonGroup.size() )
        << BUILD_LOG( setEmoticon.size() )
        << BUILD_LOG( ltrEmoticonMix.GetCaseNum() ) << END_LOG;

    KLocker lock( m_csQuickSlot );
    m_mapGroupItem.swap( mapGroupItem );
    m_setNonGroup.swap( setNonGroup );
    m_setEmoticon.swap( setEmoticon );
    m_ltrMixRatio   = ltrEmoticonMix;

    return true;

}

bool KGCHelper::LoadGroupItem( KLuaManager& kLuaMng_, OUT std::map< int,std::set<DWORD> >& mapGroupItem_ )
{
    mapGroupItem_.clear();

    _JIF( kLuaMng_.BeginTable( "GroupItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nGroupType;
        std::set<DWORD> setItems;
        _JIF( kLuaMng_.GetValue( "GroupType", nGroupType ) == S_OK, return false );
        _JIF( LoadSetItemInfo( kLuaMng_, std::string("ItemList"), setItems ), return false );
        if( mapGroupItem_.insert( std::make_pair( nGroupType, setItems ) ).second == false ) {
            START_LOG( cerr, L"포션 그룹 설정중 중복 그룹 번호 :" << nGroupType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGCHelper::LoadNonGroupItem( KLuaManager& kLuaMng_, OUT std::set<DWORD>& setNonGroup_ )
{
    setNonGroup_.clear();
    _JIF( kLuaMng_.BeginTable( "NonGroupItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng_.GetValue( i, dwItemID ) != S_OK ) break;
        setNonGroup_.insert( dwItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGCHelper::LoadEmotionItem( KLuaManager& kLuaMng_, OUT std::set<DWORD>& setEmoticon_ )
{
    setEmoticon_.clear();
    _JIF( kLuaMng_.BeginTable( "EmoticonItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng_.GetValue( i, dwItemID ) != S_OK ) break;
        setEmoticon_.insert( dwItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGCHelper::LoadEmoticonMixData( KLuaManager& kLuaMng_, KLottery& ltrRatio_ )
{
    _JIF( kLuaMng_.BeginTable( "EmoticonMixRatio" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nCase = 0;
        float fRatio = 0;
        if( kLuaMng_.GetValue( 1, nCase ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, fRatio ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        LIF(ltrRatio_.AddCase( nCase, fRatio) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGCHelper::CheckPositionGroupType( IN const int& nPositionType_, IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csQuickSlot );
    std::map< int,std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;
    mit = m_mapGroupItem.find( nPositionType_ );
    if( mit == m_mapGroupItem.end() ) {
        START_LOG( cerr, L"서버 스크립트에 포션 그룹이 설정되지 않았음")
            << BUILD_LOG( nPositionType_ )
            << END_LOG;
        return false;
    }

    sit = mit->second.find( ItemID_ );
    if( sit == mit->second.end() ) {
        START_LOG( cerr, L"서버 스크립트에 포션 그룹에 해당 아이템이 설정되지 않았음")
            << BUILD_LOG( nPositionType_ )
            << END_LOG;
        return false;
    }
    return true;
}

bool KGCHelper::IsValidateItem( IN OUT std::vector< KSlotData >& kQuickSlot_, IN const std::wstring& strLogin_ )
{
    KLocker lock( m_csQuickSlot );

    DWORD dwNonGroupCount = 0;
    std::set<int> setGroups;
    std::vector< KSlotData >::iterator vit;
    for( vit = kQuickSlot_.begin() ; vit != kQuickSlot_.end() ; ++vit )
    {
        if( m_setNonGroup.find( vit->m_kItem.m_dwID ) != m_setNonGroup.end() )
        {
            ++dwNonGroupCount;
            continue;
        }
        bool bFinded = false;
        std::map< int,std::set<DWORD> >::iterator mit;
        for( mit = m_mapGroupItem.begin() ; mit != m_mapGroupItem.end() ; ++mit )
        {
            if( mit->second.find( vit->m_kItem.m_dwID ) != mit->second.end() )
            {
                if( setGroups.insert( mit->first ).second == false )
                {
                    START_LOG( cerr, L"같은 그룹의 중첩된 퀵슬롯 아이템. 서버/클라 포션 스크립트 동기화 필요 ItemID : " << vit->m_kItem.m_dwID ) << END_LOG;
                }
                bFinded = true;
                break;
            }
        }

        if( !bFinded )
        {
            START_LOG( cerr, L"해당 사용 아이템은 등록된 아이템이 아니다. InitGCHelper.lua 확인하시오 Name : " << strLogin_ )
                << BUILD_LOG( vit->m_nSlotIndex )
                << BUILD_LOGc( vit->m_kItem.m_cItemType )
                << BUILD_LOG( vit->m_kItem.m_dwID )
                << BUILD_LOG( vit->m_kItem.m_dwUID ) << END_LOG;
            return false;
        }
    }

    if( kQuickSlot_.size()  != dwNonGroupCount + setGroups.size() )
    {
        START_LOG( cwarn, L"퀵 슬롯 장착 변경시 중첩된 아이템이 있다. Name : " << strLogin_ )
            << BUILD_LOG( kQuickSlot_.size() )
            << BUILD_LOG( dwNonGroupCount + setGroups.size() ) << END_LOG;
        return false;
    }

    return true;
}

bool KGCHelper::IsValidateEmoticon( IN OUT std::vector< KSlotData >& kQuickSlot_, IN const std::wstring& strLogin_ )
{
    KLocker lock( m_csQuickSlot );

    std::vector< KSlotData >::iterator vit;
    for( vit = kQuickSlot_.begin() ; vit != kQuickSlot_.end() ; ++vit )
    {
        if( m_setEmoticon.find( vit->m_kItem.m_dwID ) == m_setEmoticon.end() )
        {
            START_LOG( cerr, L"해당 이모티콘 아이템은 이모티콘 아이템이 아니다.. Name : " << strLogin_ )
                << BUILD_LOG( vit->m_nSlotIndex )
                << BUILD_LOGc( vit->m_kItem.m_cItemType )
                << BUILD_LOG( vit->m_kItem.m_dwID )
                << BUILD_LOG( vit->m_kItem.m_dwUID ) << END_LOG;
            return false;
        }
    }
    return true;
}

bool KGCHelper::EmotconMixResult( OUT GCITEMID& dwResult_ )
{
    int nResult = -1;
    {
        KLocker lock( m_csQuickSlot );
        nResult = m_ltrMixRatio.Decision();
    }

    dwResult_ = static_cast<GCITEMID>(nResult);
    return ( nResult != KLottery::CASE_BLANK );
}

//void KGCHelper::UpdateIndigoRank( KODBC& kODBC_ )
//{
///*
//20081114. microcat.
//
//{ call dbo.gii_indigo_island_rank ( %d ) }
//
//{ call dbo.gii_indigo_island_rank ( @1 ) }
//@1 ; Rank int ; 255를 초과할 경우 255로 고정됨
//
//n return ( @1, @2, @3, @4, @5, @6, @7, @8 )
//@1 ; Rank tinyint
//@2 ; Nick nvarchar(24)
//@3 ; CharType tinyint
//@4 ; RP int
//@5 ; Win int
//@6 ; Lose int
//@7 ; WinRate float
//@8 ; Change int
//*/
//
//    std::vector<KIndigoRankInfo>    vecIndigoRank;
//    _DBJIF( kODBC_.Query( L"{ call dbo.gii_indigo_island_rank ( 100 ) }" ),
//        return, (&kODBC_) );
//
//    while( kODBC_.Fetch() != SQL_NO_DATA )
//    {
//        KIndigoRankInfo kInfo;
//        VERIFY_QUERY( kODBC_ >> kInfo.m_nRank
//            >> kInfo.m_strNick
//            >> kInfo.m_cCharType
//            >> kInfo.m_nRP
//            >> kInfo.m_nWin
//            >> kInfo.m_nLose
//            >> kInfo.m_fWinRate
//            >> kInfo.m_nChange );
//
//        vecIndigoRank.push_back( kInfo );
//    }
//    JIF( !vecIndigoRank.empty() );
//
//    KEventPtr spEvent( new KUserEvent );
//    spEvent->m_usEventID = KUserEvent::EVENT_INDIGO_RANK_NOT;
//    KSerializer         ks;
//
//    ks.BeginWriting( &spEvent->m_kbuff );
//    ks.Put( vecIndigoRank );
//    ks.EndWriting();
//
//    spEvent->m_kbuff.Compress();
//
//    KLocker lock( m_csIndigoRank );
//    m_kserBuffIndigoRank.Clear();
//    ks.BeginWriting( &m_kserBuffIndigoRank );
//    ks.Put( *spEvent );
//    ks.EndWriting();
//
//}

void KGCHelper::GetIndigoRankInfo( KSerBuffer& serBuff_ )
{
    KLocker lock( m_csIndigoRank );
    serBuff_ = m_kserBuffIndigoRank;
}

void KGCHelper::InitIndigoRank()
{
    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = KUserEvent::EVENT_INDIGO_RANK_NOT;
    KSerializer         ks;

    std::vector<KIndigoRankInfo>    vecIndigoRank;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( vecIndigoRank );
    ks.EndWriting();

    spEvent->m_kbuff.Compress();

    KLocker lock( m_csIndigoRank );
    m_kserBuffIndigoRank.Clear();
    ks.BeginWriting( &m_kserBuffIndigoRank );
    ks.Put( *spEvent );
    ks.EndWriting();
}

//void KGCHelper::GetConnectionMsg( IN KODBC& kODBC_, IN std::vector<int>& vecMsgID_, OUT std::vector<std::wstring>& vecMessage_ )
//{
//    std::vector<int>::iterator vit;
//    for( vit = vecMsgID_.begin() ; vit != vecMsgID_.end() ; ++vit )
//    {
//        KLocker lock( m_csConnectionMsg );
//        std::map<int, std::wstring>::iterator mit;
//        mit = m_mapConnectionMsg.find( *vit );
//        if( mit == m_mapConnectionMsg.end() )
//        {
//            //call dbo.CMG_connecting_message_select
//            //( MessageID int )
//
//            //return 1 loop select
//            //( Message nvarchar(4000) )
//
//            std::wstring strMsg;
//            _DBJIF( kODBC_.Query( L"{ call dbo.CMG_connecting_message_select ( %d ) }",
//                *vit ), return, (&kODBC_) );
//            if( kODBC_.BeginFetch() )
//            {
//                VERIFY_QUERY( kODBC_ >> strMsg );
//                kODBC_.EndFetch();
//            }
//            
//            if( !strMsg.empty() )
//            {
//                m_mapConnectionMsg[*vit] = strMsg;
//                vecMessage_.push_back( strMsg );
//            }
//        }
//        else
//        {
//            vecMessage_.push_back( mit->second );
//        }
//    }
//}

void KGCHelper::ClearConnectionMsg()
{
    KLocker lock( m_csConnectionMsg );
    m_mapConnectionMsg.clear();
}

bool KGCHelper::LoadInvenExternData( KLuaManager& kLuaMng_ )
{
    int nMaxInvenCapacity = 400;
    bool bEnableInvenOverflow = false;
    std::map< DWORD, int > mapInvenExtendItemInfo;
    mapInvenExtendItemInfo.clear();

    _JIF( kLuaMng_.BeginTable( "InvenInfo" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "MaxInvenCapacity", nMaxInvenCapacity ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "InvenOverflowEnable", bEnableInvenOverflow ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "InvenExternItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nInvenCapacity = 0;
        if( kLuaMng_.GetValue( "InvenCapacity", nInvenCapacity ) != S_OK ) break;
        // 설정된 인벤토리 확장크기가 0 이하다.
        _JIF( nInvenCapacity > 0, return false );

        if( kLuaMng_.BeginTable( "ItemID" ) != S_OK ) break;

        for( int j = 1 ; ; ++j )
        {
            GCITEMID dwItemID = 0;
            if( kLuaMng_.GetValue( j, dwItemID ) != S_OK ) break;

            mapInvenExtendItemInfo[dwItemID] = nInvenCapacity;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 가방 정보 로드 완료. 최대크기:" << nMaxInvenCapacity
                  << L",초과확장여부:" << bEnableInvenOverflow
                  << L",확장아이템Size:" << mapInvenExtendItemInfo.size() ) << END_LOG;

    KLocker lock( m_csInvenExternItemInfo );
    m_nMaxInvenCapacity     = nMaxInvenCapacity;             // 인벤토리 최대 크기
    m_bEnableInvenOverflow  = bEnableInvenOverflow;          // 인벤토리 초과확장 가능여부
    m_mapInvenExtendItemInfo.swap( mapInvenExtendItemInfo ); // 인벤토리 확장아이템 정보
    return true;
}

bool KGCHelper::IsInvenExtendItem( IN GCITEMID dwItemID_, OUT int& nInvenCapacity_ )
{
    std::map< DWORD,int >::const_iterator cmit;
    nInvenCapacity_ = 0;

    KLocker lock( m_csInvenExternItemInfo );

    cmit = m_mapInvenExtendItemInfo.find( dwItemID_ );

    // 인벤토리 아이템이 아님
    if( cmit == m_mapInvenExtendItemInfo.end() )
        return false;

    // 인벤토리 확장 크기가 0 이하다.
    if( cmit->second <= 0 )
        return false;

    //인벤토리 확장 크기 넣기.
    nInvenCapacity_ = cmit->second;

    return true;
}

void KGCHelper::PrintInvenInfo()
{
    KLocker lock( m_csInvenExternItemInfo );
    std::cout << L" -- Dump Inventory Info -- " << std::endl
              << TOSTRINGA( m_nMaxInvenCapacity )
              << TOSTRINGAb( m_bEnableInvenOverflow )
              << TOSTRINGA( m_mapInvenExtendItemInfo.size() )
              << " -- Dump Inventory Info END-- " << std::endl;
}

bool KGCHelper::LoadCouponInfo( KLuaManager& kLuaMng_ )
{
    std::map<DWORD,KCouponInfo> mapCoupon;

    _JIF( kLuaMng_.BeginTable( "CouponInfo" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KCouponInfo kCouponInfo;
        int nID;
        if( kLuaMng_.GetValue( 1, nID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kCouponInfo.m_nRewardGP  ) != S_OK ) break;
        kCouponInfo.m_dwItemID = nID;
        if( !mapCoupon.insert( std::make_pair(kCouponInfo.m_dwItemID, kCouponInfo) ).second )
        {
            START_LOG( cerr, L"중복된 아이템 이 있음. ItemID : " << kCouponInfo.m_dwItemID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 쿠폰 아이템 로드 오나료. Size : " << mapCoupon.size() ) << END_LOG;

    KLocker lock( m_csCoupon );
    m_mapCoupon.swap( mapCoupon );
    return true;
}

bool KGCHelper::LoadVirtualCashCouponInfo( KLuaManager& kLuaMng_ )
{
    std::map<DWORD,KVirtualCashCouponInfo> mapCoupon;

    _JIF( kLuaMng_.BeginTable( "VirtualCashCouponInfo" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KVirtualCashCouponInfo kCouponInfo;
        int nID;
        if( kLuaMng_.GetValue( 1, nID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kCouponInfo.m_nRewardCash  ) != S_OK ) break;
        kCouponInfo.m_dwItemID = nID;
        if( !mapCoupon.insert( std::make_pair(kCouponInfo.m_dwItemID, kCouponInfo) ).second )
        {
            START_LOG( cerr, L"중복된 아이템 이 있음. ItemID : " << kCouponInfo.m_dwItemID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 가상 캐쉬 쿠폰 아이템 로드 오나료. Size : " << mapCoupon.size() ) << END_LOG;

    KLocker lock( m_csVirtualCashCoupon );
    m_mapVirtualCashCoupon.swap( mapCoupon );
    return true;
}

bool KGCHelper::GetCouponInfo( GCITEMID dwItemID_, KCouponInfo& kCouponInfo_ )
{
    std::map<DWORD,KCouponInfo>::iterator mit;
    KLocker lock( m_csCoupon );
    mit = m_mapCoupon.find( dwItemID_ );
    if( mit == m_mapCoupon.end() ) return false;

    kCouponInfo_ = mit->second;
    return true;
}

bool KGCHelper::GetCouponInfo( GCITEMID dwItemID_, KVirtualCashCouponInfo& kCouponInfo_ )
{
    std::map<DWORD,KVirtualCashCouponInfo>::iterator mit;
    KLocker lock( m_csVirtualCashCoupon );
    mit = m_mapVirtualCashCoupon.find( dwItemID_ );
    if( mit == m_mapVirtualCashCoupon.end() ) return false;

    kCouponInfo_ = mit->second;
    return true;
}

bool KGCHelper::LoadCharacterCard( KLuaManager& kLuaMng_ )
{
    std::map<GCITEMID,KCharCardInfo> mapCharacterCard;
    mapCharacterCard.clear();
    _JIF( kLuaMng_.BeginTable( "CharacterCard" ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KCharCardInfo kCharCardInfo;
        int nChar = -1;
        _JIF( kLuaMng_.GetValue( "ItemID", kCharCardInfo.m_ItmeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );

        // 완료 미션 리스트 읽어오기.
        _JIF( kLuaMng_.BeginTable( "CompleteMissionInfo" ) == S_OK, return false );
        for( int i = 1 ; ; ++i ) {
            DWORD dwMissionID;
            if( kLuaMng_.GetValue( i, dwMissionID ) != S_OK ) break;
            kCharCardInfo.m_setMissionInfo.insert( dwMissionID );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // CompleteMissionInfo

        kCharCardInfo.m_cCharType = (char)nChar;

        if( !mapCharacterCard.insert( std::make_pair( kCharCardInfo.m_ItmeID, kCharCardInfo ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 이 있음. ItemID : " << kCharCardInfo.m_ItmeID ) << END_LOG;
            return false;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 캐릭터 카드 Load 완료. Size : " << mapCharacterCard.size() ) << END_LOG;

    KLocker lock(m_csCharacterCard);
    m_mapCharacterCard.swap( mapCharacterCard );
    return true;
}

bool KGCHelper::GetCharacterCard( IN const GCITEMID itemID_, OUT char& cCharType_, OUT std::set<DWORD>& setMissionInfo_ )
{
    setMissionInfo_.clear();
    std::map<GCITEMID,KCharCardInfo>::iterator mit;

    KLocker lock(m_csCharacterCard);
    mit = m_mapCharacterCard.find( itemID_ );
    if( mit == m_mapCharacterCard.end() ) {
        return false;
    }

    cCharType_ = mit->second.m_cCharType;
    setMissionInfo_ = mit->second.m_setMissionInfo;
    return true;
}

void KGCHelper::GetCharacterCardInfo( OUT std::map<GCITEMID,KCharCardInfo>& mapCharacterCard_ )
{
    KLocker lock(m_csCharacterCard);
    mapCharacterCard_ = m_mapCharacterCard;
}

void KGCHelper::Tick()
{
    TickSnowCoin();
    TickHell();
    TickTournament();
}

bool KGCHelper::LoadSnowCoinScript( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    GCITEMID dwSnowCoinID;
    GCITEMUID dwSnowCoinUID;
    DWORD   dwSnowCoinTickGap;

    _JIF( kLuaMng_.BeginTable( "SnowCoin" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndDate"), tmEnd ), return false );
    _JIF( kLuaMng_.GetValue( "CoinID", dwSnowCoinID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CoinUID", dwSnowCoinUID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "TickGap", dwSnowCoinTickGap ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"눈꽃 코인 정보 읽기 완료." ) << END_LOG;
    {
        KLocker lock(m_csSnowCoin);
        m_tmBegin             = tmBegin;
        m_tmEnd               = tmEnd;
        m_dwSnowCoinID        = dwSnowCoinID;
        m_dwSnowCoinUID       = dwSnowCoinUID;
        m_dwSnowCoinTickGap   = dwSnowCoinTickGap;
        UpdateState();
    }
    return true;
}

bool KGCHelper::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KGCHelper::ProcessSnowCoin( IN GCITEMID dwItemID_, IN int nCount_, IN OUT std::vector<KItem>& vecInven_ )
{
    KLocker lock(m_csSnowCoin);
    if( !IsSnowCoinEvent() ) return false;
    if( !IsSnowCoin(dwItemID_) ) return false;
    if( nCount_ <= 0 ) return false;

    KItem kItem;
    MakeFakeSnowCoin( nCount_, kItem );
    vecInven_.push_back( kItem );
    return true;
}

bool KGCHelper::IsSnowCoinEvent()
{
    return m_bOnEvent;
}

bool KGCHelper::IsSnowCoin( IN GCITEMID dwItemID_ )
{
    return( dwItemID_ == m_dwSnowCoinID );
}

void KGCHelper::MakeFakeSnowCoin( IN int nCount_, OUT KItem& kItem_ )
{
    kItem_.m_ItemUID = m_dwSnowCoinUID;
    kItem_.m_ItemID = m_dwSnowCoinID;
    kItem_.m_nCount = nCount_;
    kItem_.m_nPeriod = -1;
}

void KGCHelper::TickSnowCoin()
{
    KLocker lock(m_csSnowCoin);
    if( ::GetTickCount() - m_dwSnowCoinLastTick < m_dwSnowCoinTickGap ) return;
    m_dwSnowCoinLastTick = ::GetTickCount();
    UpdateState();
}

void KGCHelper::SnowCoinInit()
{
    m_tmBegin = 1;
    m_tmEnd = 1;
    m_bOnEvent = false;
    m_dwSnowCoinID = 0;
    m_dwSnowCoinUID = 0;
    m_dwSnowCoinTickGap = 0xFFFFFFFF;
    m_dwSnowCoinLastTick    = ::GetTickCount();
}

void KGCHelper::UpdateState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    m_bOnEvent = ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

void KGCHelper::PrintCoinEvent()
{
    KLocker lock(m_csSnowCoin);
    std::cout << L" -- Dump Snow Coin -- " << std::endl
        << TOSTRINGAt( CTime(m_tmBegin) )
        << TOSTRINGAt( CTime(m_tmEnd) )
        << TOSTRINGAb( m_bOnEvent )
        << TOSTRINGA( m_dwSnowCoinID )
        << TOSTRINGA( m_dwSnowCoinUID )
        << TOSTRINGA( m_dwSnowCoinTickGap )
        << TOSTRINGA( m_dwSnowCoinLastTick )
        << " -- Dump Snow Coin END-- " << std::endl;
}

bool KGCHelper::HellItemCheckEnable( IN int nModeID_ )
{
    KLocker lock( m_csHellItemCount );

    if( m_bHellItemCheckEnable == false )
    {
        START_LOG( clog, L"헬모드 티켓체크 안함." ) << END_LOG;
        return false;
    }

    if( SiKRainbow()->GetEventByID( KRainbow::RE_CHAMP_FREE, KRainBowData() ) )
    {
        START_LOG( clog, L"레인 보우 이벤트로 인해서 헬모드 티켓체크 안함." ) << END_LOG;
        return false;
    }

    // 헬모드 티켓 소모하지 않는 모드인가?
    std::set<int>::iterator sit = m_setHellItemFreeMode.find( nModeID_ );
    if( sit != m_setHellItemFreeMode.end() )
    {
        START_LOG( clog, L"헬모드 티켓이 무료인 모드. ModeID : " << nModeID_ ) << END_LOG;
        return false;
    }

    // 헬모드 티켓 체크함.
    return true;
}

void KGCHelper::SetHellItemFreeMode( IN const std::set<int>& setModeList_ )
{
    START_LOG( clog, L"헬모드 입장권 체크안하는 모드, 읽기완료. size : " << setModeList_.size() ) << END_LOG;

    KLocker lock( m_csHellItemCount );
    if( m_setHellItemFreeMode != setModeList_ )
    {
        m_setHellItemFreeMode = setModeList_;
        m_bCheckSendHellMode = true;
    }
}

void KGCHelper::GetHellItemFreeMode( OUT std::set<int>& setModeList_ )
{
    KLocker lock( m_csHellItemCount );
    setModeList_ = m_setHellItemFreeMode;
}

void KGCHelper::TickHell()
{
    if( m_bCheckSendHellMode ) {
        std::set<int> setGameMode;
        GetHellItemFreeMode( setGameMode );
        SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_HELL_TICKET_FREE_MODE_NOT, setGameMode );
        m_bCheckSendHellMode = false;
    }
}

bool KGCHelper::LoadVipItemInfo( OUT std::string& strScript_ )
{
    std::map< int, std::map< DWORD,int > > mapVipItemInfo;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitVIPItemInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "VIPItemInfo" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        int nCharType = -1;
        std::map< DWORD,int > mapItemList;

        if( kLuaMng.BeginTable( i ) != S_OK ) break;

        if( kLuaMng.GetValue( "CharType", nCharType ) != S_OK ) break;

        if( kLuaMng.BeginTable( "ItemList" ) != S_OK ) break;
        for( int j = 1 ; ; ++j )
        {
            if( kLuaMng.BeginTable( j ) != S_OK ) break;

            std::pair<DWORD,int> prInfo;
            if( kLuaMng.GetValue( 1, prInfo.first ) != S_OK ) break;
            if( kLuaMng.GetValue( 2, prInfo.second ) != S_OK ) break;

            if( mapItemList.insert( prInfo ).second == false )
            {
                START_LOG( cerr, L"중복된 아이템 이 있음. ItemID : " << prInfo.first ) << END_LOG;
                return false;
            }
            _JIF( kLuaMng.EndTable() == S_OK, return false );
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        mapVipItemInfo[nCharType] = mapItemList;
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L" VIP Item 정보 로드 완료. Size : " << mapVipItemInfo.size() ) << END_LOG;
    //for( std::map< int, std::map< DWORD,int > >::const_iterator cmit = mapVipItemInfo.begin() ;
    //     cmit != mapVipItemInfo.end() ; ++cmit )
    //{
    //    std::cout << std::tab << L"CharType : " << cmit->first << L", ItemList Size : " << cmit->second.size() << dbg::endl;
    //}

    KLocker lock(m_csVipItemInfo);
    m_mapVipItemInfo.swap( mapVipItemInfo );
    return true;
}

void KGCHelper::GetVipItemInfo( OUT std::map< int, std::map< DWORD,int > >& mapVipItemInfo_ )
{
    KLocker lock(m_csVipItemInfo);
    mapVipItemInfo_ = m_mapVipItemInfo;
}

bool KGCHelper::IsCurrentVipItemInfo( IN const int nCharType_, IN const GCITEMID dwItemID_, IN const int nVipCount_ )
{
    KLocker lock(m_csVipItemInfo);
    std::map< int, std::map< DWORD,int > >::const_iterator cmit;
    std::map< DWORD,int >::const_iterator cmit2;

    cmit = m_mapVipItemInfo.find( nCharType_ );
    if( cmit == m_mapVipItemInfo.end() )
    {
        START_LOG( cerr, L"해당 캐릭터타입 ItemList가 없습니다. CharType : " << nCharType_ ) << END_LOG;
        return false;
    }

    cmit2 = cmit->second.find( dwItemID_ );
    if( cmit2 == cmit->second.end() )
    {
        START_LOG( cerr, L"등록되지않은 VIP Item 입니다. Item ID : " << dwItemID_ ) << END_LOG;
        return false;
    }

    if( cmit2->second != nVipCount_ )
    {
        START_LOG( cerr, L"해당 VIP Item의 VIP 가격이 다릅니다." )
            << BUILD_LOG( dwItemID_ )
            << BUILD_LOG( cmit2->second )
            << BUILD_LOG( nVipCount_ ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"정상적인 VIP Item. CharType : " << nCharType_ << L",Item ID : " << dwItemID_ << L",VipCount : " << nVipCount_ ) << END_LOG;
    return true;
}

bool KGCHelper::LoadChatRecordUserUID( OUT std::string& strScript_ )
{
    bool bChatRecordEnable = false;
    int nChatLogQueLimit = -1;
    bool bChatRecordUserCheck = false;
    std::set<DWORD> setUserUID;

    KLuaManager kLuaMng;
    const std::string strScriptName = "ChatRecUIDList.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( 0 == kLuaMng.GetValue( "ChatRecordEnable", bChatRecordEnable ) , return false );
    _JIF( 0 == kLuaMng.GetValue( "LogQueLimit", nChatLogQueLimit ) , return false );
    _JIF( 0 == kLuaMng.GetValue( "UIDCheck", bChatRecordUserCheck ) , return false );

    _JIF( kLuaMng.BeginTable( "UIDList" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwUID = 0;
        if( S_OK != kLuaMng.GetValue( i, dwUID ) ) break;

        if( setUserUID.insert( dwUID ).second == false )
        {
            START_LOG( cerr, L"채팅기록 유저 등록 중. 이미 등록된 UID : " << dwUID ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    {
        KLocker lock(m_csChatRecordUserUID);
        m_bChatRecordEnable = bChatRecordEnable;
        m_nChatLogQueLimit = nChatLogQueLimit;
        m_bChatRecordUserCheck = bChatRecordUserCheck;
        m_setChatRecordUserUID.swap( setUserUID );
    }

    START_LOG( cerr, L"채팅기록설정 스크립트 로드 완료." )
        << BUILD_LOG( bChatRecordEnable )
        << BUILD_LOG( nChatLogQueLimit )
        << BUILD_LOG( bChatRecordUserCheck )
        << BUILD_LOG( setUserUID.size() ) << END_LOG;
    return true;
}


bool KGCHelper::IsChatRecordUserUID( IN DWORD dwUID_ )
{
    KLocker lock(m_csChatRecordUserUID);

    if( m_bChatRecordUserCheck == false )
    {
        START_LOG( clog, L"유저체크 안함." ) << END_LOG;
        return true;
    }

    if( m_setChatRecordUserUID.find( dwUID_ ) == m_setChatRecordUserUID.end() )
    {
        START_LOG( clog, L"채팅기록 대상유저가 아님. UserUID : " << dwUID_ ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"채팅기록 대상유저. UserUID : " << dwUID_ ) << END_LOG;
    return true;
}

void KGCHelper::PrintChatRecord()
{
    KLocker lock(m_csChatRecordUserUID);
    std::cout << L" -- Dump ChatRecord -- " << std::endl
        << TOSTRINGAb( m_bChatRecordEnable )
        << TOSTRINGA( m_nChatLogQueLimit )
        << TOSTRINGAb( m_bChatRecordUserCheck )
        << TOSTRINGA( m_setChatRecordUserUID.size() )
        << " -- Dump ChatRecord END-- " << std::endl;
}

bool KGCHelper::LoadHellItemInfo( KLuaManager& kLuaMng_ )
{
    bool bCheckEnable = true;
    GCITEMID dwTicketID = 0;
    int nMaxTicketCount = -1;
    int nConnectGift = 0;
    int nDayAttendGift = 0;
    int nWeekAttendGift = 0;
    int nMonthAttendGift = 0;
    bool bEndGameHellTicketDec = false;
    bool bDungeonFreePass = false;
    int nDungeonFreePassID = 0;

    _JIF( kLuaMng_.BeginTable( "HellModeTicket" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CheckEnable", bCheckEnable ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "TicketID", dwTicketID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxTicketCount", nMaxTicketCount ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ConnectGift", nConnectGift ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DayAttendGift", nDayAttendGift ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "WeekAttendGift", nWeekAttendGift ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MonthAttendGift", nMonthAttendGift ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "EndGameTicketDec", bEndGameHellTicketDec ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonFreePass", bDungeonFreePass ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonFreePassID", nDungeonFreePassID ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"헬모드 입장권 정보 로드 완료." ) << END_LOG;
    {
        KLocker lock(m_csHellItemCount);
        m_bHellItemCheckEnable = bCheckEnable;
        m_dwHellItemID = dwTicketID;
        m_nMaximumTicketNum = nMaxTicketCount;
        m_nConnectionHellItemCount = nConnectGift;
        m_nAttendanceHellItemcount = nDayAttendGift;
        m_nWeekAttendHellItemcount = nWeekAttendGift;
        m_nMonthAttendHellItemcount = nMonthAttendGift;
        m_bEndGameHellTicketDec = bEndGameHellTicketDec;
        m_bDungeonFreePass = bDungeonFreePass;
        m_nDungeonFreePassID = nDungeonFreePassID;
    }

    return true;
}

void KGCHelper::PrintHellItemInfo()
{
    KLocker lock(m_csHellItemCount);
    std::cout << L" -- Dump Hell Info Info -- " << std::endl
        << TOSTRINGAb( m_bHellItemCheckEnable )
        << TOSTRINGA( m_dwHellItemID )
        << TOSTRINGA( m_nMaximumTicketNum )
        << TOSTRINGA( m_nConnectionHellItemCount )
        << TOSTRINGA( m_nAttendanceHellItemcount )
        << TOSTRINGA( m_nWeekAttendHellItemcount )
        << TOSTRINGA( m_nMonthAttendHellItemcount )
        << TOSTRINGA( m_setHellItemFreeMode.size() )
        << " -- Dump Hell Item Info END-- " << std::endl;
}

bool KGCHelper::LoadGCClubExtension( KLuaManager& kLuaMng_ )
{
    bool bExtensionEnable = false;
    std::vector<KDropItemInfo> vecConnectBonusItem;
    std::vector<KDropItemInfo> vecDayAttendBonusItem;
    std::map<GCITEMID,USHORT> mapGCClubItemID;

    _JIF( kLuaMng_.BeginTable( "GCClubExtension" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ExtensionEnable", bExtensionEnable ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "GCClubItemID" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwGCClubItemID = 0L;
        USHORT usType = GT_NONE;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, dwGCClubItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, usType ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapGCClubItemID[dwGCClubItemID] = usType;
        START_LOG(clog, L"GC클럽 아이템 추가.. ItemID : " << dwGCClubItemID << L", usType : " << usType ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // 접속시 보너스 지급 아이템 { ItemID, Factor }
    _JIF( kLuaMng_.BeginTable( "ConnectBonusItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kData;

        if( kLuaMng_.GetValue( 1, kData.m_ItemID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kData.m_nDuration ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, kData.m_nPeriod ) != S_OK ) break;

        vecConnectBonusItem.push_back( kData );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    // 일일 출석시 보너스 지급 아이템 { ItemID, Factor }
    _JIF( kLuaMng_.BeginTable( "DayAttendBonusItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kData;

        if( kLuaMng_.GetValue( 1, kData.m_ItemID ) != S_OK ) break;
        if( kLuaMng_.GetValue( 2, kData.m_nDuration ) != S_OK ) break;
        if( kLuaMng_.GetValue( 3, kData.m_nPeriod ) != S_OK ) break;

        vecDayAttendBonusItem.push_back( kData );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"GC클럽 확장기능 로드 완료. Enable:" << bExtensionEnable << L",ItemID size :" << mapGCClubItemID.size()
                    << L",Connect/Attend:" << vecConnectBonusItem.size() << L"/" << vecDayAttendBonusItem.size() ) << END_LOG;
    {
        KLocker lock( m_csGCClub );
        m_bGCClubExtensionEnable = bExtensionEnable;
        m_mapGCClubItemID.swap( mapGCClubItemID );
        m_vecConnectBonusItem.swap( vecConnectBonusItem );
        m_vecDayAttendBonusItem.swap( vecDayAttendBonusItem );
    }

    return true;
}

void KGCHelper::GetGCClubConnectBonusItem( OUT std::vector<KDropItemInfo>& vecItemList_ )
{
    KLocker lock( m_csGCClub );
    vecItemList_ = m_vecConnectBonusItem;
}

void KGCHelper::GetGCClubDayAttendBonusItem( OUT std::vector<KDropItemInfo>& vecItemList_ )
{
    KLocker lock( m_csGCClub );
    vecItemList_ = m_vecDayAttendBonusItem;
}

bool KGCHelper::LoadTourDonationScript( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin = time_t(NULL);
    time_t  tmEnd = time_t(NULL);
    GCITEMID   dwDonationItemID = 0;
    std::set<DWORD> setBuyCheckItemList;
    setBuyCheckItemList.clear();

    _JIF( kLuaMng_.BeginTable( "TourDonation" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndDate"), tmEnd ), return false );

    // 구매제한 아이템 리스트 읽어오기.
    _JIF( kLuaMng_.BeginTable( "BuyCheckItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng_.GetValue( i, dwItemID ) != S_OK ) break;
        setBuyCheckItemList.insert( dwItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // BuyCheckItem
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // TourDonation

    START_LOG( cerr, L"대만 대회참가팀 승리 정보 읽기 완료." ) << END_LOG;
    {
        KLocker lock(m_csTourDonation);
        m_tmTourDonationBegin           = tmBegin;
        m_tmTourDonationEnd             = tmEnd;
        m_setBuyCheckItemList.swap( setBuyCheckItemList );
    }
    return true;
}

bool KGCHelper::IsTourDonationEvent()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csTourDonation );
    return (m_tmTourDonationBegin <= tmCurrent && m_tmTourDonationEnd >= tmCurrent);
}

bool KGCHelper::IsDonationItemID( IN GCITEMID dwItemID_ )
{
    std::set<DWORD>::iterator sit;
    KLocker lock( m_csTourDonation );
    sit = m_setBuyCheckItemList.find( dwItemID_ );
    if( sit == m_setBuyCheckItemList.end() ) return false;
    return true;
}

void KGCHelper::GetBuyCheckItemList( OUT std::set<DWORD>& setBuyCheckItemList_ )
{
    setBuyCheckItemList_.clear();
    KLocker lock( m_csTourDonation );
    setBuyCheckItemList_ = m_setBuyCheckItemList;
}

bool KGCHelper::LoadEventInfoScript( IN KLuaManager& kLuaMng_ )
{
    bool bCheckEnable =false;
    std::set<DWORD> setStringIDList;

    _JIF( kLuaMng_.BeginTable( "EventInfo" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CheckEnable", bCheckEnable ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "StringID" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwStringID;
        if( kLuaMng_.GetValue( i, dwStringID ) != S_OK ) break;
        setStringIDList.insert( dwStringID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // StringID

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // EventInfo

    START_LOG( cerr, L"이벤트 내용 출력 여부 확인 읽기 완료." )
        << BUILD_LOG( bCheckEnable ) << END_LOG;
    {
        KLocker lock(m_csEventInfo);
        m_bCheckEnable = bCheckEnable;
        m_setStringIDList.swap( setStringIDList );
    }
    return true;
}

bool KGCHelper::CheckEventEnable()
{
    KLocker lock(m_csEventInfo);
    return m_bCheckEnable;
}

void KGCHelper::GetEventStringList( OUT std::set<DWORD>& setStringList_ )
{
    KLocker lock(m_csEventInfo);
    setStringList_ = m_setStringIDList;
}

bool KGCHelper::LoadTournamentData( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    DWORD   dwTourTickGap;
    GCITEMID dwTicketID;
    bool    bTimeCheck;
    int     nStartHour;
    int     nEndHour;

    _JIF( kLuaMng_.BeginTable( "TournamentTime" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndDate"), tmEnd ), return false );
    _JIF( kLuaMng_.GetValue( "TickGap", dwTourTickGap ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "TournamentTicketID", dwTicketID ) == S_OK, return false );
    _JIF( LoadTimeCheckData( kLuaMng_, std::string("TimeCheck"), bTimeCheck, nStartHour, nEndHour ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Tournament 시간정보 읽기 완료." )
        << BUILD_LOGtm( CTime( tmBegin ) )
        << BUILD_LOGtm( CTime( tmEnd ) )
        << BUILD_LOG( dwTourTickGap )
        << BUILD_LOG( dwTicketID )
        << BUILD_LOG( bTimeCheck )
        << BUILD_LOG( nStartHour )
        << BUILD_LOG( nEndHour ) << END_LOG;
    {
        KLocker lock(m_csTournament);
        m_tmTourBegin             = tmBegin;
        m_tmTourEnd               = tmEnd;
        m_dwTourTickGap           = dwTourTickGap;
        m_dwTournamentTicketID    = dwTicketID;
        m_bTimeCheck              = bTimeCheck;
        m_nStartHour              = nStartHour;
        m_nEndHour                = nEndHour;
        UpdateTourState();
        UpdateTimeState();
    }
    return true;
}

// 미국 대회서버 관련 변수 초기화.
void KGCHelper::InitTournament()
{
    m_tmTourBegin = 1;
    m_tmTourEnd = 1;
    m_bTourCheck = false;
    m_dwTourTickGap = 0xFFFFFFFF;
    m_dwTourLastTick    = ::GetTickCount();
    m_bTourTimeCheck = false;
    m_bTimeCheck = false;
    m_nStartHour = 0;
    m_nEndHour = 0;
}

// 미국 대회서버 입장 체크변수 갱신.
void KGCHelper::UpdateTourState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    m_bTourCheck = ( m_tmTourBegin <= tmCurrent && m_tmTourEnd >= tmCurrent );
}

// 미국 대회서버 입장 관련 Tick
void KGCHelper::TickTournament()
{
    KLocker lock(m_csTournament);
    if( ::GetTickCount() - m_dwTourLastTick < m_dwTourTickGap ) return;
    m_dwTourLastTick = ::GetTickCount();
    UpdateTourState();
    UpdateTimeState();
}

// 대회서버 입장 시간 설정 체크.
void KGCHelper::UpdateTimeState()
{
    CTime ctCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( ctCurrent );
    if( m_bTimeCheck ) {
        if( m_tmTourBegin <= tmCurrent && m_tmTourEnd >= tmCurrent && 
            ctCurrent.GetHour() >= m_nStartHour && ctCurrent.GetHour() <= m_nEndHour ) {
            m_bTourCheck = true;
        } else {
            m_bTourCheck = false;
        }
    }
}

bool KGCHelper::LoadTimeCheckData( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT bool& bTimeCheck_, OUT int& nStartHour_, OUT int& nEndHour_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "CheckEnable", bTimeCheck_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "StartHour", nStartHour_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "EndHour", nEndHour_ ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

GCITEMID KGCHelper::GetTournamentTicketID()
{
    KLocker lock(m_csTournament);
    return m_dwTournamentTicketID;
}

bool KGCHelper::LoadAdditionalMobDropRatio( IN OUT KLuaManager& kLuaMng_ )
{
    std::map<int,float> mapAdditionalMobDropRatio;

    _JIF( kLuaMng_.BeginTable( "AdditionalDropByMobGrade" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng_.BeginTable(i) != S_OK ) break;
        std::pair<int,float> prRatio;
        _JIF( kLuaMng_.GetValue( 1, prRatio.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prRatio.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        mapAdditionalMobDropRatio.insert( prRatio );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"몬스터 등급에 따른 드랍 배율 정보 로드 완료. Size : " << mapAdditionalMobDropRatio.size() ) << END_LOG;
    KLocker lock( m_csAdditionalMobDrop );
    m_mapAdditionalMobDropRatio.swap( mapAdditionalMobDropRatio );

    return true;
}

float KGCHelper::GetMobDropRatioByGrade( IN const int nMobGrade )
{
    std::map<int,float>::const_iterator mit;
    KLocker lock( m_csAdditionalMobDrop );
    mit = m_mapAdditionalMobDropRatio.find( nMobGrade );
    if ( mit == m_mapAdditionalMobDropRatio.end() )
        return 0.f;
    return mit->second;
}

void KGCHelper::SetIndigoRank( KSerBuffer& serBuffIndigoRank_ )
{
    KLocker lock( m_csIndigoRank );
    m_kserBuffIndigoRank = serBuffIndigoRank_;
}

void KGCHelper::GetConnectionMessage( std::map<int, std::wstring>& msg_ )
{
    KLocker lock( m_csConnectionMsg );
    msg_ = m_mapConnectionMsg;
}

void KGCHelper::SetConnectionMessage( std::map<int, std::wstring>& msg_ )
{
    KLocker lock( m_csConnectionMsg );
    m_mapConnectionMsg.swap( msg_ );
}

bool KGCHelper::LoadUserAuthData( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin = time_t(NULL);
    time_t  tmAuthEnd = time_t(NULL);
    time_t  tmPurchaseEnd = time_t(NULL);
    std::vector<KDropItemInfo> vecRewardItem;

    _JIF( kLuaMng_.BeginTable( "UserAuthIfno" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("AuthEndDate"), tmAuthEnd ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("PurchaseEndDate"), tmPurchaseEnd ), return false );
    _JIF( LoadRewardList( kLuaMng_, std::string("UserReward"), vecRewardItem ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"남미 미국 유저 인증 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csUserAuth );
        m_tmUserAuthBegin = tmBegin;
        m_tmUserAuthEnd = tmAuthEnd;
        m_tmUserPurchaseEnd = tmPurchaseEnd;
        m_vecRewardItem.swap( vecRewardItem );
    }
    return true;
}

bool KGCHelper::IsUserAuthEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csUserAuth );
    return (m_tmUserAuthBegin <= tmCurrent && m_tmUserAuthEnd >= tmCurrent);
}

bool KGCHelper::IsUserPurchaseEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csUserAuth );
    return (m_tmUserAuthBegin <= tmCurrent && m_tmUserPurchaseEnd >= tmCurrent);
}

bool KGCHelper::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KDropItemInfo>& vecRewardItem_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KDropItemInfo kItem;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecRewardItem_.push_back( kItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KGCHelper::GetUserRewardItem( OUT std::vector<KDropItemInfo>& vecRewardList_ )
{
    vecRewardList_.clear();

    KLocker lock( m_csUserAuth );
    vecRewardList_ = m_vecRewardItem;
}

void KGCHelper::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock(m_csUserAuth);
    tmDate_ = m_tmUserAuthBegin;
}

bool KGCHelper::IsUserVirtualPurchaseTerm( IN time_t tmDate_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csUserAuth );
    return ( tmDate_ >= tmCurrent );
}

bool KGCHelper::LoadNationRewardInfo( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    float   fExpRatio;
    float   fGPRatio;

    _JIF( kLuaMng_.BeginTable( "NationRewardInfo" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndDate"), tmEnd ), return false );
    _JIF( kLuaMng_.GetValue( "ExpReward", fExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "GPReward", fGPRatio ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"국가별 혜택 정보 읽기 완료." )
        << BUILD_LOGtm( CTime( tmBegin ) )
        << BUILD_LOGtm( CTime( tmEnd ) )
        << BUILD_LOG( fExpRatio )
        << BUILD_LOG( fGPRatio ) << END_LOG;
    {
        KLocker lock(m_csNationReward);
        m_tmNationBegin             = tmBegin;
        m_tmNationEnd               = tmEnd;
        m_fExpRatio                 = fExpRatio;
        m_fGPRatio                  = fGPRatio;
    }
    return true;
}

bool KGCHelper::IsNationRewardCheck()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csUserAuth );
    return (m_tmNationBegin <= tmCurrent && m_tmNationEnd >= tmCurrent);
}

void KGCHelper::GetNationExpGPRewardRatio( OUT float& fExpRatio, OUT float& fGPRatio )
{
    KLocker lock( m_csUserAuth );
    fExpRatio = m_fExpRatio;
    fGPRatio = m_fGPRatio;
}

bool KGCHelper::LoadItemNeedCount( KLuaManager& kLuaMng_, IN std::string& strTable_, IN int nType_, OUT std::map<GCITEMID,int>& mapList_, IN OUT std::map<int,std::set<GCITEMID> >& mapTicket_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; i += 2 ) {
        GCITEMID TicketID = 0;
        int nNeedCount = 0;
        if ( kLuaMng_.GetValue( i, TicketID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i+1, nNeedCount ) == S_OK, return false );

        mapList_[TicketID] = nNeedCount;
        mapTicket_[nType_].insert( TicketID );

        START_LOG( clog, L"티켓 소모개수 설정.. TicketID : " << TicketID << L", nNeedCount : " << nNeedCount ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGCHelper::LoadDungeonTicketList( KLuaManager& kLuaMng_ )
{
    std::map<int,KDungeonTicket> mapDungeonTicketList;
    std::map<int,std::set<GCITEMID> > mapTicketItem;

    _JIF( kLuaMng_.BeginTable( "DungeonTicket" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KDungeonTicket kInfo;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "ModeID", kInfo.m_nModeID ) == S_OK, return false );
        _JIF( LoadItemNeedCount( kLuaMng_, std::string("EnterTicket"), KPremiumInfo::TYPE_CARNIVAL, kInfo.m_mapEnterTicket, mapTicketItem ), return false );
        _JIF( LoadItemNeedCount( kLuaMng_, std::string("BossTicket"), KPremiumInfo::TYPE_GOLDSTAGE, kInfo.m_mapBossTicket, mapTicketItem ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapDungeonTicketList[kInfo.m_nModeID] = kInfo;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"던전 티켓정보 스크립트 로드 완료.. " )
        << BUILD_LOG( mapDungeonTicketList.size() )
        << BUILD_LOG( mapTicketItem.size() ) << END_LOG;

    // 접속유저에게 알림
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_DUNGEON_TICKET_LIST_NOT, mapDungeonTicketList );

    {
        KLocker lock(m_csDungeonTicket);
        m_mapDungeonTicketList.swap( mapDungeonTicketList );
        m_mapTicketItem.swap( mapTicketItem );
    }
    return true;
}

void KGCHelper::GetDungeonTicketList( OUT std::map<int,KDungeonTicket>& mapList_ )
{
    KLocker lock(m_csDungeonTicket);
    mapList_ = m_mapDungeonTicketList;
}

bool KGCHelper::GetDungeonTicketInfo( IN const int& nModeID_, OUT KDungeonTicket& kInfo_ )
{
    KLocker lock(m_csDungeonTicket);

    std::map<int,KDungeonTicket>:: iterator mit;
    mit = m_mapDungeonTicketList.find( nModeID_ );
    if ( mit == m_mapDungeonTicketList.end() ) {
        return false;
    }

    kInfo_ = mit->second;
    return true;
}

void KGCHelper::GetDungeonTicketTypeList( OUT std::map<int,std::set<GCITEMID> >& mapList_ )
{
    KLocker lock(m_csDungeonTicket);
    mapList_ = m_mapTicketItem;
}

bool KGCHelper::LoadKairoNoticeInfo( KLuaManager& kLuaMng_ )
{
    std::map<DWORD, std::set<DWORD>> mapKairoNoticeItem;
    mapKairoNoticeItem.clear();
    _JIF( kLuaMng_.BeginTable( "KairoNoticeItem" ) == S_OK, return false );

    // 공지제한 아이템 리스트 읽어오기.
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        
        GCITEMID dwItemID;
        std::set<DWORD> setItemInfo;
        setItemInfo.clear();
        _JIF( kLuaMng_.GetValue( "BoxItemID", dwItemID ) == S_OK, return false );
        _JIF( LoadSetItemInfo( kLuaMng_, std::string("PostItemList"), setItemInfo ), return false );

        if( mapKairoNoticeItem.insert( std::make_pair( dwItemID, setItemInfo ) ).second == false ) {
            START_LOG( cerr, L"중복된 박스 ItemID :" << dwItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Kairo 공지제한 아이템 정보 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csKairoNotice );
        m_mapKairoNoticeItem.swap( mapKairoNoticeItem );
    }
    return true;
}

bool KGCHelper::LoadSetItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<DWORD>& setInfo_ )
{
    setInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    DWORD dwData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, dwData ) != S_OK ) break;
        setInfo_.insert( dwData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGCHelper::CheckKairoBox( IN const GCITEMID& dwItemID_ )
{
    std::map<DWORD, std::set<DWORD>>::iterator mit;
    KLocker lock( m_csKairoNotice );
    mit = m_mapKairoNoticeItem.find( dwItemID_ );
    if( mit == m_mapKairoNoticeItem.end() ) {
        return false;
    }
    return true;
}

bool KGCHelper::CheckNoticeItem( IN const GCITEMID& dwKairoBoxItemID_, IN const GCITEMID& dwPostItemID_ )
{
    std::map<DWORD, std::set<DWORD>>::iterator mit;
    std::set<DWORD>::iterator sit;
    KLocker lock( m_csKairoNotice );
    mit = m_mapKairoNoticeItem.find( dwKairoBoxItemID_ );
    if( mit == m_mapKairoNoticeItem.end() ) {
        return false;
    }

    sit = mit->second.find( dwPostItemID_ );
    if( sit == mit->second.end() ) {
        return false;
    }
    return true;
}

bool KGCHelper::LoadExpPotionInfo( KLuaManager& kLuaMng_ )
{
    // 경험치 포션
    std::map<DWORD, KExpPotionInfo> mapPotion;

    _JIF( kLuaMng_.BeginTable( "ExpPotionInfo") == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KExpPotionInfo kExpPotionInfo;

        _JIF( kLuaMng_.GetValue( "PercentageGrow", kExpPotionInfo.m_bPercentageGrow ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ItemID", kExpPotionInfo.m_dwItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GrowRate", kExpPotionInfo.m_fGrowRate ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GrowValue", kExpPotionInfo.m_nGrowValue ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ExceedEXP", kExpPotionInfo.m_bExceedEXP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ValidMinLev", kExpPotionInfo.m_dwValidMinLev ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ValidMaxLev", kExpPotionInfo.m_dwValidMaxLev ) == S_OK, return false );

        kExpPotionInfo.m_fGrowRate *= 100.0f; // 1이 100%라서...
        m_vecExpPotionItemID.push_back( kExpPotionInfo.m_dwItemID );

        if( !mapPotion.insert( std::make_pair(kExpPotionInfo.m_dwItemID, kExpPotionInfo) ).second )
        {
            START_LOG( cerr, L"중복된 아이템 이 있음. ItemID : " << kExpPotionInfo.m_dwItemID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 경험치 포션 아이템 로드 완료. Size : " << mapPotion.size() ) << END_LOG;

    KLocker lock( m_csExpPotion );
    m_mapExpPotion.swap( mapPotion );

    return true;
}

bool KGCHelper::GetExpPotionInfo( IN const GCITEMID& dwItemID_, OUT KExpPotionInfo& KExpPotionInfo_ )
{
    std::map<DWORD,KExpPotionInfo>::iterator mit;
    KLocker lock( m_csExpPotion );
    mit = m_mapExpPotion.find( dwItemID_ );
    if( mit == m_mapExpPotion.end() ) return false;

    KExpPotionInfo_ = mit->second;

    return true;
}

bool KGCHelper::GetExpPotionInfo( OUT std::vector<GCITEMID> &vecExpPotion_ )
{
    vecExpPotion_ = m_vecExpPotionItemID;

    if ( true == vecExpPotion_.empty() ) {
        return false;
    }

    return true;
}

bool KGCHelper::LoadPresentLimitItem( KLuaManager& kLuaMng_ )
{
    std::set<DWORD> setPresentLimitItem;
    bool bLimitUseEnable;
    setPresentLimitItem.clear();
    _JIF( kLuaMng_.BeginTable( "PresentLimitItem" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "LimitUseEnable", bLimitUseEnable ) == S_OK, return false );
    _JIF( LoadSetItemInfo( kLuaMng_, std::string("LimitItemList"), setPresentLimitItem ), return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"선물하기 기능 제한 아이템 정보 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csPresentLimit );
        m_bLimitUseEnable = bLimitUseEnable;
        m_setPresentLimitItem.swap( setPresentLimitItem );
    }
    return true;
}

bool KGCHelper::CheckPresentLimitItem( IN const DWORD& dwItemID_ )
{
    std::set<DWORD>::iterator sit;
    KLocker lock( m_csPresentLimit );

    if( !m_bLimitUseEnable ) { // 선물하기 제한 기능 사용여부 체크.
        return true;
    }
    sit = m_setPresentLimitItem.find( dwItemID_ );
    if( sit == m_setPresentLimitItem.end() ) {
        return true;
    }
    return false;
}

bool KGCHelper::LoadNewCharData( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin = time_t(NULL);
    time_t  tmEnd = time_t(NULL);
    int nNewCharID = -1;
        
    _JIF( kLuaMng_.BeginTable( "NewCharEvent" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng_.GetValue( "NewCharID", nNewCharID ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"신캐릭터 지급 알림 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csNewChar );
        m_tmNewCharBegin = tmBegin;
        m_tmNewCharEnd = tmEnd;
        m_nNewCharID = nNewCharID;        
    }
    return true;
}

bool KGCHelper::IsEventNewCharTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csNewChar );
    return (m_tmNewCharBegin <= tmCurrent && m_tmNewCharEnd >= tmCurrent);
}

int KGCHelper::GetEventNewCharID()
{
    KLocker lock( m_csNewChar );
    return m_nNewCharID;
}

bool KGCHelper::LoadAttendGetChar( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin = time_t(NULL);
    time_t  tmEnd = time_t(NULL);
    std::vector<KDropItemInfo> vecRewardItem;

    _JIF( kLuaMng_.BeginTable( "AttendGetChar" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginDate"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndDate"), tmEnd ), return false );
    _JIF( LoadRewardList( kLuaMng_, std::string("Reward"), vecRewardItem ), return false );
    _JIF( kLuaMng_.GetValue( "AttendCount", m_nAttendCount ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"출석 n회 캐릭터 아이템 지급 이벤트 스크립트 로드 완료." ) << END_LOG;
    {
        KLocker lock( m_csAttendGetChar );
        m_tmAttendGetCharBegin = tmBegin;
        m_tmAttendGetCharEnd = tmEnd;
        m_vecAttendGetCharRewardItem.swap( vecRewardItem );
    }
    return true;
}

bool KGCHelper::IsAttendToGetCharTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csAttendGetChar );
    return ( m_tmAttendGetCharBegin <= tmCurrent && m_tmAttendGetCharEnd >= tmCurrent );
}

bool KGCHelper::IsAttendToGetCharDay( IN KSimpleDate& date_ )
{
    time_t tmAttendDay = KncUtil::TimeToInt( CTime ( date_.m_sYear, date_.m_cMonth, date_.m_cDay, 0, 0, 0 ) );

    KLocker lock( m_csAttendGetChar );
    return ( m_tmAttendGetCharBegin <= tmAttendDay && m_tmAttendGetCharEnd >= tmAttendDay );
}

void KGCHelper::GetAttendGetCharRewardItem( OUT std::vector<KDropItemInfo>& vecRewardList_ )
{
    vecRewardList_.clear();

    KLocker lock( m_csAttendGetChar );
    vecRewardList_ = m_vecAttendGetCharRewardItem;
}

int KGCHelper::GetAttendCountGetChar()
{
    KLocker lock( m_csAttendGetChar );
    return m_nAttendCount;
}

void KGCHelper::GetBeginDateGetCharByAttend( OUT time_t& tmDate_ )
{
    KLocker lock( m_csAttendGetChar );
    tmDate_ = m_tmAttendGetCharBegin;
}

bool KGCHelper::LoadPVPCharDisable( KLuaManager& kLuaMng_ )
{
    std::set<DWORD> setCharDisable;
    setCharDisable.clear();

    int nIndex = 0;

    _JIF( kLuaMng_.BeginTable( "PVP_Character_Disable" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        DWORD dwCharType = 0;
        if ( kLuaMng_.GetValue( i, dwCharType ) != S_OK )
            break;

        setCharDisable.insert( dwCharType );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"PVP 대전 서버 오픈 가능한 캐릭터 정보 읽기 완료") << END_LOG;
    {
        KLocker lock( m_csPVPCharDisable );
        m_setCharDisable.swap( setCharDisable );
    }

    return true;
}

bool KGCHelper::GetPVPCharacterDisableData( OUT std::set<DWORD>& setCharDisable_ )
{
    KLocker lock( m_csPVPCharDisable );

    if ( m_setCharDisable.empty() )
        return false;

    setCharDisable_ = m_setCharDisable;

    return true;
}

bool KGCHelper::LoadCharLevelEvent( KLuaManager& kLuaMng_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    DWORD dwVersion;
    std::map<std::pair<char,int>, std::vector<KDropItemInfo>> mapCharRewardItem;

    _JIF( kLuaMng_.BeginTable( "CharLevelEvent" ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Version", dwVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng_, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string("EndTime"), tmEnd ), return false );

    _JIF( kLuaMng_.BeginTable( "RewardItem" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nChar = -1;
        int nLevel = -1;
        std::pair<char,int> prData;
        VEC_REWARD vecItemInfo;
        _JIF( kLuaMng_.GetValue( "CharType", nChar ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "CharLevel", nLevel ) == S_OK, return false );
        _JIF( LoadGradeReward( kLuaMng_, std::string("ItemInfo"), vecItemInfo ), return false );

        prData.first = static_cast<char>( nChar );
        prData.second = nLevel;

        if( mapCharRewardItem.insert( std::make_pair( prData, vecItemInfo ) ).second == false ) {
            START_LOG( cerr, L"기본 복장 세팅중 중복된 캐릭터 ID. CharID :" << prData.first )
                << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // RewardItem Table
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // CharLevelEvent Table

    START_LOG( cerr, L"캐릭터 특정 레벨 아이템 지급 이벤트." ) << END_LOG;
    {
        KLocker lock( m_csCharLevelEvent );
        m_dwVersion = dwVersion;
        m_tmCharLevelEventBegin = tmBegin;
        m_tmCharLevelEventEnd = tmEnd;
        m_mapCharRewardItem.swap( mapCharRewardItem );
    }
    return true;
}

bool KGCHelper::LoadGradeReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KDropItemInfo>& vecRewardItem_ )
{

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        int nGradeID = 0;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, nGradeID ) == S_OK, return false );
        kData.m_cGradeID = static_cast<char>( nGradeID );
        vecRewardItem_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGCHelper::IsCharLevelEvent()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csCharLevelEvent );
    return (m_tmCharLevelEventBegin <= tmCurrent && m_tmCharLevelEventEnd >= tmCurrent);
}

bool KGCHelper::ServerConnectCharLevelReward( IN const char& cCharType_, IN const int& nLevel_, IN const std::set<char>& setCharInfo_, OUT std::vector<KDropItemInfo>& vecItemInfo_ )
{
    std::set<char>::const_iterator csit;
    std::pair<char, int> prData;
    prData.first = cCharType_;
    prData.second = nLevel_;
    std::map<std::pair<char,int>, std::vector<KDropItemInfo>>::iterator mit;
    std::vector<KDropItemInfo>::iterator vit;

    csit = setCharInfo_.find( cCharType_ );
    if( csit != setCharInfo_.end() ) {
        return false;
    }

    KLocker lock( m_csCharLevelEvent );
    for( mit = m_mapCharRewardItem.begin(); mit != m_mapCharRewardItem.end(); ++mit ) {
        if( mit->first.first == cCharType_ ) {
            if( mit->first.second <= nLevel_ ) {
                for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
                    vecItemInfo_.push_back( *vit );
                }
                return true;
            } else {
                return false;
            }
        }
    }

    return false;
}

bool KGCHelper::GetCharLevelRewardItem( IN const char& cCharType_, IN const int& nLevel_, IN const std::set<char>& setCharInfo_, OUT std::vector<KDropItemInfo>& vecItemInfo_ )
{
    std::set<char>::const_iterator csit;
    std::pair<char, int> prData;
    prData.first = cCharType_;
    prData.second = nLevel_;
    std::map<std::pair<char,int>, std::vector<KDropItemInfo>>::iterator mit;
    std::vector<KDropItemInfo>::iterator vit;

    csit = setCharInfo_.find( cCharType_ );
    if( csit != setCharInfo_.end() ) {
        return false;
    }

    KLocker lock( m_csCharLevelEvent );
    mit = m_mapCharRewardItem.find( prData );
    if( mit == m_mapCharRewardItem.end() ) {
        return false;
    }

    for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
        vecItemInfo_.push_back( *vit );
    }
    return true;
}
