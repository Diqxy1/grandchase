#include "SP.h"
#include "odbc/Odbc.h"
#include <dbg/dbg.hpp>
#include "ItemManager.h"
#include "NetError.h"
#include "CenterPacket.h"
#include "LookEquipment.h"
#include "SlangFilter.h"
#include "AgitHelper.h"
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include "StrengthManager.h"
#include "Log4.h"
#include <boost/bind/bind.hpp>
#include "PetHelper.h"
#include <boost/foreach.hpp>
#include "VirtualCash.h"

KSP::KSP(void)
: m_kODBC( KODBC() )
,m_kCommonSP( KODBC() )
{
}

KSP::~KSP(void)
{
}

bool KSP::GetWaitItemFullList( IN const DWORD dwUserUID_, IN const char cCharType_, OUT std::vector< boost::tuple<__int64,KItem,int> >& vecItems_ )
{
//     mk8253. 20091117.
    /*
    대기아이템리턴, 세트인지 패키지인지 판단하지 않음
    L"{ call dbo.WIGAWaitItem_select_20130402 ( %d, %d ) }"

    { call dbo.WIGAWaitItem_select_20130402 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10 )
    @01: WIGAUID       bigint
    @02: BuyerNickname nvarchar( 24 )
    @03: ItemID        int
    @04: Grade         int
    @05: Period        int
    @06: Duration      int
    @07: GetType       int
    @08: Comment       nvarchar( 80 )
    @09: ItemLevel     int
    @10: StrengthLevel int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_select_20130402 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    KItem kItem;

    std::wstring strBuyerNick;
    std::wstring strComment;

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {

        __int64 waitItemUID = 0;
        int nGetType = 0;
        int nGradeID = 0;

        VERIFY_QUERY( m_kODBC
            >> waitItemUID
            >> strBuyerNick
            >> kItem.m_ItemID
            >> nGradeID
            >> kItem.m_nPeriod
            >> kItem.m_nCount
            >> nGetType
            >> strComment
            >> kItem.m_sEquipLevel
            >> kItem.m_cEnchantLevel );

        kItem.m_cGradeID = static_cast<char>( nGradeID );
        vecItems_.push_back( boost::tuples::make_tuple( waitItemUID, kItem, nGetType ) );
    }
    return true;
}

bool KSP::GetGiftInfoList( IN const DWORD dwUserUID_, IN const char cCharType_, std::vector<KGiftInfo>& vecGiftInfo_ )
{
    //     mk8253. 20091117.
    /*
    대기아이템리턴, 세트인지 패키지인지 판단하지 않음
    L"{ call dbo.WIGAWaitItem_select_type_20130402 ( %d, %d, %d ) }"
    { call dbo.WIGAWaitItem_select_type_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iGetType_input  int

    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09 )
    @01: WIGAUID       bigint
    @02: BuyerNickname nvarchar( 24 )
    @03: ItemID        int
    @04: GradeID       int
    @05: Period        int
    @06: Duration      int
    @07: Comment       nvarchar( 80 )
    @08: ItemLevel     int
    @09: StrengthLevel int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_select_type_20130402 ( %d, %d, %d ) }",
        dwUserUID_, cCharType_, KItemManager::EWIT_GIFT ) );

    KGiftInfo kInfo;
    int nTemp;
    __int64 tempWaitItemUID;
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        VERIFY_QUERY( m_kODBC
            >> tempWaitItemUID
            >> kInfo.m_strBuyerNick
            >> kInfo.m_dwItemID
            >> nTemp
            >> nTemp
            >> nTemp
            >> kInfo.m_strMsg
            >> nTemp
            >> nTemp );
        vecGiftInfo_.push_back( kInfo );
    }
    return true;
}

bool KSP::InsertItemToInven( IN const DWORD dwUID_, IN const char cCharType_, IN const __int64& WUID_, IN const int nGetType_, IN OUT KItem& kItem_ )
{
    int nGradeTableID = SiKItemManager()->GetGradeTableID( kItem_.m_ItemID );
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( kItem_.m_ItemID );
    int nSocketTableID = SiKItemManager()->GetSocketTableID( kItem_.m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( kItem_.m_ItemID );

    //대기 리스트에서 받아온 등급이 -1 이면 스크립트에 이미 정해진 등급을 사용.
    if( kItem_.m_cGradeID == -1 ) {
        kItem_.m_cGradeID = static_cast< char >( SiKItemManager()->m_kGradeDecider.GetGrade( nGradeTableID ) );
    }

    if( kItem_.m_cGradeID == -1 ) {
        START_LOG( cerr, L"Grade 정보가 없음" ) << END_LOG;
        return false;
    }

    // 레벨 설정이 되어 있으면 해당 속성값 사용함.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsRandomType( nLevelTableID ) && kItem_.m_sEquipLevel == 0  ) {
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_RANDOM );
    } else {
        nItemLevel = static_cast<int>( kItem_.m_sEquipLevel );
    }

    std::list<std::pair<int,float>> AttributeList;
    std::list<int> SocketList;

    // 영구 아이템인 경우에만 속성, 코디 아이템인 경우에도 속성 /
    if ( ( kItem_.m_nPeriod == KItem::UNLIMITED_ITEM && kItem_.m_nCount == KItem::UNLIMITED_ITEM ) 
        || ( SiKLookEquipment()->IsLookItemID( kItem_.m_ItemID ) && SiKLookEquipment()->GetEnableAvatar() == true ) ) {
        // TODO: Grade에서 소켓의 갯수를 가져오는 기능 필요.
        int nAttributeCount = SiKItemManager()->GetAttributeCountFromGrade( kItem_.m_cGradeID );
        SiKItemManager()->m_kAttributeDecider.GetAttributeList( nAttributeTableID, nAttributeCount, (int)kItem_.m_cGradeID, AttributeList );

        if ( nSocketTableID >= 0 ) {
            SiKItemManager()->m_kSocketDecider.GetSocketList( nSocketTableID, SocketList );
        }
        else {
            SiKItemManager()->m_kSocketDecider.GetBasicSocketList( kItem_.m_cGradeID, SocketList );
        }
    }
    // TODO : InitDuration 정보는 Duration 정보와 동일하게 맞춘다.
    
    START_LOG( clog, L"아이템 인벤토리에 삽입!!" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( kItem_.m_ItemID )
        << BUILD_LOGc( kItem_.m_cGradeID )
        << BUILD_LOG( kItem_.m_nPeriod )
        << BUILD_LOG( kItem_.m_nCount )
        << BUILD_LOG( WUID_ )
        << BUILD_LOG( nGetType_ )
        << BUILD_LOG( AttributeList.size() )
        << BUILD_LOG( nGradeTableID )
        << END_LOG;

    // 아이템 인벤토리 추가시 DB부하 줄이는 작업.
    // 기본 아이템 정보 입력.
    bool bMergedItem = false;
    if( nGetType_ != KItemManager::EWIT_PACKAGE ) { // 싼타로 획득한 캐릭터 타입이 지정되지 않은 아이템은 수량을 합치지 않는다
        if( !CheckUserItem_merge( dwUID_, kItem_.m_ItemID, kItem_.m_cGradeID, WUID_, kItem_.m_nPeriod, kItem_.m_nCount, AttributeList.size(), SocketList.size(), kItem_.m_ItemUID, cCharType_, bMergedItem ) ) {
            START_LOG( cerr, L"아이템 기본 정보 입력 실패" )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kItem_.m_ItemID )
                << BUILD_LOG( WUID_ )
                << END_LOG;
            return false;
        }
    }

    if ( bMergedItem == false ) {
        int nOK = -99;
        nOK = UIGAUserItem_merge_20130415( dwUID_, kItem_.m_ItemID, kItem_.m_cGradeID, WUID_, kItem_.m_ItemUID );

        _LOG_SUCCESS( nOK == 0, L"아이템 기본 정보 입력. Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

        if ( -1 != cCharType_ ) { // 공용(아지트) 아이템일 경우 아래 SP를 실행하지 않는다
            if ( 0 != UIGAUserItemCharacter_insert( dwUID_, cCharType_, kItem_.m_ItemUID ) ) {
                return false;
            }
        }
    }

    if( kItem_.m_nPeriod > 0 ) { // 기간제 아이템일 경우.
        if( !UserItemPeriod_merge( dwUID_, kItem_.m_ItemUID, kItem_.m_nPeriod, kItem_.m_tEndDate ) ) {
            START_LOG( cerr, L"아이템 기간 정보 입력 실패" )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( cCharType_ )
                << BUILD_LOG( kItem_.m_ItemID )
                << BUILD_LOG( kItem_.m_ItemUID )
                << END_LOG;
        }
    }
    else if ( kItem_.m_nCount > 0 ) { // 수량 아이템일 경우.
        int nCount = 0;
        if( !UserItemCount_merge( dwUID_, kItem_.m_ItemUID, kItem_.m_nCount, nCount ) ) {
            START_LOG( cerr, L"아이템 수량 정보 입력 실패" )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( cCharType_ )
                << BUILD_LOG( kItem_.m_ItemID )
                << BUILD_LOG( kItem_.m_ItemUID )
                << END_LOG;
        }
    }

    std::list<std::pair<int,float>>::iterator litAtt;
    int i;
    for( i = 0, litAtt = AttributeList.begin() ; litAtt != AttributeList.end() ; ++litAtt, ++i )
    {
        // 속성 초기값을 넣어주는 부분.
        LIF( MergeAttribute( KAttributeInfo::ATS_USING, dwUID_, i, litAtt->first, litAtt->second, kItem_, cCharType_ ) );
    }

    std::list<int>::iterator litSoc;
    for( i = 0, litSoc = SocketList.begin() ; litSoc != SocketList.end() ; ++litSoc, ++i )
    {
        int nSocState = KSocketInfo::STS_INIT;
        switch( *litSoc )
        {
        case KGCSocketTable::ST_OPENED:
            nSocState = KSocketInfo::STS_OPENED;
            break;
        case KGCSocketTable::ST_CLOSED:
            nSocState = KSocketInfo::STS_CLOSED;
            break;
        }

        LIF( UpdateSocket_merge( dwUID_, i, 0, nSocState, kItem_, cCharType_ ) );
    }

    // 아이템 레벨 설정.
    if( nItemLevel > 0 ) {
        LIF( UpdateLevel( dwUID_, nItemLevel, KItemLevelTable::IL_EQUIP_LV, kItem_, cCharType_ ) );
    }

    // 아이템 강화 레벨 설정.
    if( kItem_.m_cEnchantLevel > 0 ) {
        int nOK = UpdateStrengthItemInfo( dwUID_, kItem_.m_ItemUID, kItem_.m_cEnchantLevel, 0, kItem_.m_cGradeID, cCharType_ );
        if( nOK != 0 ) {
            START_LOG( cerr, L" 강화석 강화정보 DB 갱신시 에러" )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kItem_.m_ItemUID )
                << BUILD_LOGc( kItem_.m_cEnchantLevel )
                << BUILD_LOG( kItem_.m_cGradeID ) << END_LOG;
        }
    }

    return true;
}

bool KSP::InsertItemToInvenForInsertDepot( IN const DWORD dwUID_, IN const char cCharType_, IN const __int64& WUID_, IN const int nGetType_, IN OUT KItem& kItem_ )
{
    int nGradeTableID = SiKItemManager()->GetGradeTableID( kItem_.m_ItemID );
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( kItem_.m_ItemID );
    int nSocketTableID = SiKItemManager()->GetSocketTableID( kItem_.m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( kItem_.m_ItemID );

    //대기 리스트에서 받아온 등급이 -1이 아니면 이미 정해진 등급을 사용.
    if( kItem_.m_cGradeID == -1 ) {
        kItem_.m_cGradeID = (char)SiKItemManager()->m_kGradeDecider.GetGrade( nGradeTableID );
    }

    if( kItem_.m_cGradeID == -1 ) {
        START_LOG( cerr, L"Grade 정보가 없음" ) << END_LOG;
        return false;
    }

    // 레벨 설정이 되어 있으면 해당 속성값 사용함.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsRandomType( nLevelTableID ) && kItem_.m_sEquipLevel == 0  ) {
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_RANDOM );
    } else {
        nItemLevel = kItem_.m_sEquipLevel;
    }

    std::list<std::pair<int,float>> AttributeList;
    std::list<int> SocketList;

    // 영구 아이템인 경우에만 속성, 코디 아이템인 경우에도 속성 /
    if ( ( kItem_.m_nPeriod == KItem::UNLIMITED_ITEM && kItem_.m_nCount == KItem::UNLIMITED_ITEM ) 
        || ( SiKLookEquipment()->IsLookItemID( kItem_.m_ItemID ) && SiKLookEquipment()->GetEnableAvatar() == true ) ) {
        // TODO: Grade에서 소켓의 갯수를 가져오는 기능 필요.
        int nAttributeCount = SiKItemManager()->GetAttributeCountFromGrade( kItem_.m_cGradeID );
        SiKItemManager()->m_kAttributeDecider.GetAttributeList( nAttributeTableID, nAttributeCount, (int)kItem_.m_cGradeID, AttributeList );

        if ( nSocketTableID >= 0 ) {
            SiKItemManager()->m_kSocketDecider.GetSocketList( nSocketTableID, SocketList );
        }
        else {
            SiKItemManager()->m_kSocketDecider.GetBasicSocketList( kItem_.m_cGradeID, SocketList );
        }
    }
    // TODO : InitDuration 정보는 Duration 정보와 동일하게 맞춘다.
    START_LOG( clog, L"아이템 인벤토리에 삽입!!" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kItem_.m_ItemID )
        << BUILD_LOGc( kItem_.m_cGradeID )
        << BUILD_LOG( kItem_.m_nPeriod )
        << BUILD_LOG( kItem_.m_nCount )
        << BUILD_LOG( WUID_ )
        << BUILD_LOG( nGetType_ )
        << BUILD_LOG( AttributeList.size() )
        << BUILD_LOG( nGradeTableID )
        << END_LOG;

    // 아이템 인벤토리 추가시 DB부하 줄이는 작업.
    // 기본 아이템 정보 입력.
    //if( !UserItem_merge( dwUID_, kItem_.m_ItemID, kItem_.m_cGradeID, WUID_, kItem_.m_nPeriod, kItem_.m_nCount, AttributeList.size(), SocketList.size(), kItem_.m_ItemUID, cCharType_ ) ) {
    //    START_LOG( cerr, L"아이템 기본 정보 입력 실패" )
    //        << BUILD_LOG( dwUID_ )
    //        << BUILD_LOG( kItem_.m_ItemID )
    //        << BUILD_LOG( WUID_ )
    //        << END_LOG;
    //    return false;
    //}

    // new uid 를 발급받는다. 
    int nOK = -99;
    nOK = UIGAUserItem_merge_20130415( dwUID_, kItem_.m_ItemID, kItem_.m_cGradeID, WUID_, kItem_.m_ItemUID );

    _LOG_SUCCESS( nOK == 0, L"아이템 기본 정보 입력. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( nOK != 0 ) {
        return false;
    }

    if ( 0 != UIGAUserItemCharacter_insert( dwUID_, cCharType_, kItem_.m_ItemUID ) ) {
        return false;
    }

    if( kItem_.m_nPeriod > 0 ) { // 기간제 아이템일 경우.
        if( !UserItemPeriod_merge( dwUID_, kItem_.m_ItemUID, kItem_.m_nPeriod, kItem_.m_tEndDate ) ) {
            START_LOG( cerr, L"아이템 기간 정보 입력 실패" )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( cCharType_ )
                << BUILD_LOG( kItem_.m_ItemID )
                << BUILD_LOG( kItem_.m_ItemUID )
                << END_LOG;
        }
    }
    else if ( kItem_.m_nCount > 0 ) { // 수량 아이템일 경우.
        int nCount = 0;
        if( !UserItemCount_merge( dwUID_, kItem_.m_ItemUID, kItem_.m_nCount, nCount ) ) {
            START_LOG( cerr, L"아이템 수량 정보 입력 실패" )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( cCharType_ )
                << BUILD_LOG( kItem_.m_ItemID )
                << BUILD_LOG( kItem_.m_ItemUID )
                << END_LOG;
        }
    }

    std::list<std::pair<int,float>>::iterator litAtt;
    int i;
    for( i = 0, litAtt = AttributeList.begin() ; litAtt != AttributeList.end() ; ++litAtt, ++i )
    {
        // 속성 초기값을 넣어주는 부분.
        LIF( MergeAttribute( KAttributeInfo::ATS_USING, dwUID_, i, litAtt->first, litAtt->second, kItem_, cCharType_ ) );
    }

    std::list<int>::iterator litSoc;
    for( i = 0, litSoc = SocketList.begin() ; litSoc != SocketList.end() ; ++litSoc, ++i )
    {
        int nSocState = KSocketInfo::STS_INIT;
        switch( *litSoc )
        {
        case KGCSocketTable::ST_OPENED:
            nSocState = KSocketInfo::STS_OPENED;
            break;
        case KGCSocketTable::ST_CLOSED:
            nSocState = KSocketInfo::STS_CLOSED;
            break;
        }

        LIF( UpdateSocket_merge( dwUID_, i, 0, nSocState, kItem_, cCharType_ ) );
    }

    // 아이템 레벨 설정.
    if( nItemLevel > 0 ) {
        LIF( UpdateLevel( dwUID_, nItemLevel, KItemLevelTable::IL_EQUIP_LV, kItem_, cCharType_ ) );
    }

    // 아이템 강화 레벨 설정.
    if( kItem_.m_cEnchantLevel > 0 ) {
        int nOK = UpdateStrengthItemInfo( dwUID_, kItem_.m_ItemUID, kItem_.m_cEnchantLevel, 0, kItem_.m_cGradeID, cCharType_ );
        if( nOK != 0 ) {
            START_LOG( cerr, L" 강화석 강화정보 DB 갱신시 에러" )
                << BUILD_LOG( nOK )
                << BUILD_LOG( kItem_.m_ItemUID )
                << BUILD_LOGc( kItem_.m_cEnchantLevel )
                << BUILD_LOG( kItem_.m_cGradeID ) << END_LOG;
        }
    }

    return true;
}

bool KSP::UpdateAttribute( IN const DWORD dwUserUID_, IN const int nSlotID_, IN const int nAttType_, IN const float fAttValue_, IN OUT KItem& kItem_, IN const char cCharType_ )
{
    /*
    mk8253. 20091116.
    아이템 속성값 변경
    L"{ call dbo.UIGAUserItemAttribute_merge_20130415 ( %d, %d, %I64d, %d, %d, %d, %d  ) }"

    { call dbo.UIGAUserItemAttribute_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
    @01: @01iLoginUID_input       int
    @02: @02iCharType_input       int
    @03: @03iItemUID_input        bigint
    @04: @04iSlotID_input         tinyint
    @05: @05iTypeID_input         smallint
    @06: @06iValue_input          int
    @07: @07iAttributeState_input tinyint
        
    1 return ( @1 )
    @1: OK
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    int nAttributeState = KAttributeInfo::ATS_OPENED;
    if( nAttType_ != KGCAttributeDecider::Table::AT_INVAILD )
        nAttributeState = KAttributeInfo::ATS_USING;

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttribute_merge_20130415 ( %d, %d, %I64d, %d, %d, %d, %d  ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nSlotID_, nAttType_, (int)(fAttValue_*10000.f), nAttributeState ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 속성 설정. UID : " << dwUserUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kItem_.m_ItemUID )
        << BUILD_LOG( nSlotID_ )
        << BUILD_LOG( nAttType_ )
        << BUILD_LOG( fAttValue_ )
        << BUILD_LOG( nAttributeState )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( nOK == 0 ) {
        KAttributeInfo kAttInfo;
        kAttInfo.m_cSlotID = nSlotID_;
        kAttInfo.m_cState = nAttributeState;
        kAttInfo.m_cType = nAttType_;
        kAttInfo.m_fValue = fAttValue_;
        kItem_.m_vecAttribute.push_back( kAttInfo );
    }

    return ( nOK == 0 );
}

bool KSP::UpdateSocket( IN const DWORD dwUserUID_, 
                               IN const int nSlotID_, 
                               IN const GCITEMID CardID_, 
                               IN const int nSocState_, 
                               IN OUT KItem& kItem_,
                               IN const char cCharType_ )
{
    /*
    mk8253. 20091111.
    아이템 소켓값 변경
    /*
    L"{ call dbo.UIGAUserItemSocket_merge_20130415 ( %d, %d, %I64d, %d, %d, %d ) }"

    { call dbo.UIGAUserItemSocket_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
    @01: @01iLoginUID_input    int
    @02: @02iCharType_input    int
    @03: @03iItemUID_input     bigint
    @04: @04iSlotID_input      tinyint
    @05: @05iCardID_input      int
    @06: @06iSocketState_input tinyint
        
    1 return ( @1 )
    @1 ; OK
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemSocket_merge_20130415 ( %d, %d, %I64d, %d, %d, %d ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nSlotID_, CardID_, nSocState_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK == 0 ) {
        START_LOG( clog, L"소켓 설정 성공" ) << END_LOG;
        KSocketInfo kSocket;
        kSocket.m_CardItemID = CardID_;
        kSocket.m_cSlotID = nSlotID_;
        kSocket.m_cState = (char)nSocState_;
        kItem_.m_vecSocket.push_back( kSocket );
    }
    else if( nOK == 1 ) {
        START_LOG( clog, L"소켓 설정이 안되는 아이템" ) << END_LOG;
    }
    else {
        START_LOG( cerr, L"소켓 설정 오류, nOK : " << nOK << " Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
        return false;
    }

    return true;
}

bool KSP::GetPrice(IN const GCITEMID itemID_, IN const int nCount_, IN const int nPeriod_, OUT int& nPrice_, OUT int& nMoneyType_)
{
    DBJIF2(m_kODBC.Query(L"{ call dbo.IPGAItemPrice_select( %d, %d, %d ) }",
        itemID_, nPeriod_, nCount_));

    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC
            >> nMoneyType_
            >> nPrice_);
        m_kODBC.EndFetch();
    }

    if (nPrice_ < 0)
        nPrice_ = 0;

    START_LOG(clog, L"가격 정보 요청")
        << BUILD_LOG((int)itemID_)
        << BUILD_LOG(nMoneyType_)
        << BUILD_LOG(nPeriod_)
        << BUILD_LOG(nCount_)
        << BUILD_LOG(nPrice_) << END_LOG;

    return true;
}

int KSP::CheckSellMoneyType( IN const GCITEMID itemID_ )
{

    return 0;
}

/*bool KSP::GetPetSupplyItems( IN const DWORD dwUID_, IN const DWORD dwPetID_, IN const int nPromotion_, OUT std::vector<KItem>& vecItems_ )
{
    vecItems_.clear();

    //L"{ call dbo.PIGAPetItem_insert( %d, %d, %d ) }"
    //
    //{ call dbo.PIGAPetItem_insert ( @1, @2, @3 ) }
    //@1 ; @iLoginUID_  int
    //@2 ; @iPetID_     int
    //@3 ; @iPromotion_ tinyint
    //
    //1 return ( @1 @2 )
    //@1 ; OK
    //0        ; 성공
    //-1       ; 유저 정보가 없음
    //-2       ; 지급될 아이템이 없음
    //-101이하 ; 트랜젝션 에러
    //
    //@2 ; WUID
    //
    //이녀석을 부르면 대기아이템으로 등록되며 대기Type은 7입니다.
    //대기아이템에서 type7을 유저 인벤토리로 넣어주어야 합니다.

    int nOK = -99;
    __int64 nWUID = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPetItem_insert ( %d, %d, %d ) }",
        dwUID_, dwPetID_, nPromotion_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> nWUID );
        m_kODBC.EndFetch();
    }

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 펫 생성/전직시 지급 아이템 대기 아이템으로 넣기 실패. OK : " << nOK )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( dwPetID_ )
            << BUILD_LOG( nPromotion_ )
            << BUILD_LOG(m_kODBC.m_strLastQuery )
            << END_LOG;
        return false;
    }

    // 이아래는 바뀔수 있다..
    // 해당 대기 아이템 UID 를 통해서 아이템 목록을 가져 온다.
    if( InsertWaitItemToInven( dwUID_, KItemManager::EWIT_PET_SYSTEM, vecItems_ ) == false )
        return false;

    return true;
}

*/
bool KSP::GetWaitItemList( IN const DWORD dwUserUID_, IN const char cCharType_, IN int nWaitType_, OUT std::vector<std::pair<__int64,KItem>>& vecItems_ )
{
//     mk8253. 20091117.
    /*
    대기아이템리턴, 세트인지 패키지인지 판단하지 않음
    L"{ call dbo.WIGAWaitItem_select_type_20130402 ( %d, %d, %d ) }"
    { call dbo.WIGAWaitItem_select_type_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iGetType_input  int

    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09 )
    @01: WIGAUID       bigint
    @02: BuyerNickname nvarchar( 24 )
    @03: ItemID        int
    @04: GradeID       int
    @05: Period        int
    @06: Duration      int
    @07: Comment       nvarchar( 80 )
    @08: ItemLevel     int
    @09: StrengthLevel int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_select_type_20130402 ( %d, %d, %d ) }",
        dwUserUID_, cCharType_, nWaitType_ ) );

    __int64         waitItemUID;
    KItem           kItem;
    std::wstring    strBuyerNick;
    std::wstring    strComment;
    int nGradeID = 0;
    int nEnchantlevel = 0;

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        VERIFY_QUERY( m_kODBC
            >> waitItemUID
            >> strBuyerNick
            >> kItem.m_ItemID
            >> nGradeID
            >> kItem.m_nPeriod
            >> kItem.m_nCount
            >> strComment
            >> kItem.m_sEquipLevel
            >> nEnchantlevel );

        kItem.m_cGradeID = static_cast<char>( nGradeID );
        kItem.m_cEnchantLevel = static_cast<char>( nEnchantlevel );
        vecItems_.push_back( std::pair<__int64,KItem>( waitItemUID, kItem ) );
    }
    if ( vecItems_.empty() ) {
        START_LOG( cwarn, L"대기 아이템 없음" )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    return true;
}

bool KSP::InsertWaitItemToInven( IN const DWORD dwUserUID_, IN const char cCharType_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_ )
{
    std::vector<std::pair<__int64,KItem>> vecWaitItemList;
    if( GetWaitItemList( dwUserUID_, cCharType_, nGetType_, vecWaitItemList ) == false ) {
        return false;
    }

    std::vector<std::pair<__int64,KItem>>::iterator vit;
    for( vit = vecWaitItemList.begin() ; vit != vecWaitItemList.end() ; ++vit ) {
        InsertSingleWaitItemToInvenOrVDepot( dwUserUID_, cCharType_,vit->first, nGetType_, vecItems_ );
    }

    return true;
}


bool KSP::InsertFullWaitItemToInven( IN const DWORD dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItems_ )
{
    if ( 0 == dwUserUID_ ) {
        return false;
    }

    std::vector< boost::tuple<__int64,KItem,int> > vecWaitItemList;
    if( GetWaitItemFullList( dwUserUID_, cCharType_, vecWaitItemList ) == false ) {
        return false;
    }

    std::vector< boost::tuple<__int64,KItem,int> >::iterator vit;
    for( vit = vecWaitItemList.begin() ; vit != vecWaitItemList.end() ; ++vit ) {
        InsertSingleWaitItemToInvenOrVDepot( dwUserUID_, cCharType_, vit->get<0>(), vit->get<2>(), vecItems_ );
    }

    return true;
}

bool KSP::InsertSingleWaitItemToInvenOrVDepot( IN const DWORD dwUserUID_, IN const char cCharType_, IN const __int64 itemWUID_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_ )
{
    // 해당 대기 아이템 UID 를 통해서 아이템 목록을 가져 온다.
    std::vector< std::pair<__int64,KItem> > vecItems;
    if ( !GetWaitInnerItemList( dwUserUID_, cCharType_, itemWUID_, vecItems ) )
    {
        START_LOG( cerr, L"대기 아이템의 아이템 리스트를 가져오는데 실패 했습니다." )
            << BUILD_LOG( dwUserUID_ )
            << BUILD_LOG( itemWUID_ )
            << BUILD_LOG( nGetType_ ) << END_LOG;
        return false;
    }

    std::vector< std::pair<__int64,KItem> >::iterator vit;
    for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
        // 실제로 인벤토리에 아이템을 집어 넣는다.
        //LIF( 0 == UIGAUserItemCharacter_insert( dwUserUID_, cCharType_, vit->second.m_ItemUID ) );

        if ( InsertItemToInven( dwUserUID_, cCharType_, vit->first, nGetType_, vit->second ) ) {
            vecItems_.push_back( vit->second );
        }
        else
        {
            START_LOG( cerr, L"Inventory에 Wait 아이템을 Set하는데 실패 했습니다.")
                << BUILD_LOG( dwUserUID_ )
                << BUILD_LOG( itemWUID_ )
                << BUILD_LOG( nGetType_ )
                << BUILD_LOG( vit->second.m_ItemID ) << END_LOG;
            return false;
        }
    }

    // 대기아이템 정보에 지급 완료 플레그를 세운다.
    LIF( UpdateWaitItemToDelete( itemWUID_ ) );

    std::vector<KItem>::iterator vitItemID;
    for ( vitItemID = vecItems_.begin() ; vitItemID != vecItems_.end() ; ++vitItemID ) {
        // 룩 변환
        if ( SiKLookEquipment()->IsLookItemID( vitItemID->m_ItemID ) ) {
            int nOK = 0;
            if ( false == NormalItemToCoordiItem( dwUserUID_, vitItemID->m_ItemUID, cCharType_, nOK ) ) {
                // 이미 코디 아이템일 경우 현재 타입이 코디가 아닐 경우에 대해서만 추가 처리한다.
                if( nOK == -4 ) {
                    if ( vitItemID->m_cItemType != KItem::TYPE_LOOK ) {
                        vitItemID->AddItemType(KItem::TYPE_LOOK);
                    }
                }
                else {
                    vitItemID->DeleteItemType(KItem::TYPE_LOOK);
                }
                continue;
            }
            vitItemID->AddItemType(KItem::TYPE_LOOK);
        }

        // 아지트 오브젝트 변환
        if ( SiKAgitHelper()->IsObjectItem( vitItemID->m_ItemID ) ) {
            if ( false == AgiteObjPos_merge( dwUserUID_, vitItemID->m_ItemUID, -1, -1 ) ) {
                vitItemID->DeleteItemType(KItem::TYPE_AGIT_OBJ);
                continue;
            }
            vitItemID->AddItemType(KItem::TYPE_AGIT_OBJ);
        }

        if ( false == SiKAgitHelper()->IsAgitItem( vitItemID->m_ItemID ) && -1 == cCharType_ ) { // 아지트 아이템이 아니고 캐릭터 타입이 -1일 때 가상창고로 넣어 준다
            // 매직넘버 안쓰고싶은데... InvenType이 char형임... 기존에 Depot enum값이 -1인 이유가 이것 때문인듯
            if ( true == UpdateItemDelState( dwUserUID_, vitItemID->m_ItemUID, 254 ) ) {
                vitItemID->m_cInvenType = KItem::VIRTUAL_DEPOT;
            }
        }
    }
    return true;
}

bool KSP::InsertSingleWaitItemToInvenForDepot( IN const DWORD dwUserUID_, IN const char cCharType_, IN const __int64 itemWUID_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_ )
{
    // 해당 대기 아이템 UID 를 통해서 아이템 목록을 가져 온다.
    std::vector< std::pair<__int64,KItem> > vecItems;
    if ( !GetWaitInnerItemList( dwUserUID_, cCharType_, itemWUID_, vecItems ) )
    {
        START_LOG( cerr, L"대기 아이템의 아이템 리스트를 가져오는데 실패 했습니다." )
            << BUILD_LOG( dwUserUID_ )
            << BUILD_LOG( itemWUID_ )
            << BUILD_LOG( nGetType_ ) << END_LOG;
        return false;
    }

    std::vector< std::pair<__int64,KItem> >::iterator vit;
    for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
        // 실제로 인벤토리에 아이템을 집어 넣는다.
        if ( InsertItemToInvenForInsertDepot( dwUserUID_, cCharType_, vit->first, nGetType_, vit->second ) ) {
            vecItems_.push_back( vit->second );
        }
        else
        {
            START_LOG( cerr, L"Inventory에 Wait 아이템을 Set하는데 실패 했습니다.")
                << BUILD_LOG( dwUserUID_ )
                << BUILD_LOG( itemWUID_ )
                << BUILD_LOG( nGetType_ )
                << BUILD_LOG( vit->second.m_ItemID ) << END_LOG;
            return false;
        }
    }

    // 대기아이템 정보에 지급 완료 플레그를 세운다.
    LIF( UpdateWaitItemToDelete( itemWUID_ ) );

    std::vector<KItem>::iterator vitItemID;
    for ( vitItemID = vecItems_.begin() ; vitItemID != vecItems_.end() ; ++vitItemID ) {
        // 룩 변환
        if ( SiKLookEquipment()->IsLookItemID( vitItemID->m_ItemID ) ) {
            int nOK = 0;
            if ( false == NormalItemToCoordiItem( dwUserUID_, vitItemID->m_ItemUID, cCharType_, nOK ) ) {
                // 이미 코디 아이템일 경우 현재 타입이 코디가 아닐 경우에 대해서만 추가 처리한다.
                if( nOK == -4 ) {
                    if ( vitItemID->m_cItemType != KItem::TYPE_LOOK ) {
                        vitItemID->AddItemType(KItem::TYPE_LOOK);
                    }
                }
                else {
                    vitItemID->DeleteItemType(KItem::TYPE_LOOK);
                }
                continue;
            }
            vitItemID->AddItemType(KItem::TYPE_LOOK);
        }

        // 아지트 오브젝트 변환
        if ( SiKAgitHelper()->IsObjectItem( vitItemID->m_ItemID ) ) {
            if ( false == AgiteObjPos_merge( dwUserUID_, vitItemID->m_ItemUID, -1, -1 ) ) {
                vitItemID->DeleteItemType(KItem::TYPE_AGIT_OBJ);
                continue;
            }
            vitItemID->AddItemType(KItem::TYPE_AGIT_OBJ);
        }
    }
    return true;
}

int KSP::UIGAUserItemCharacter_insert( IN const DWORD& dwUserUID_, IN const char cCharType_, IN const __int64& WUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemCharacter_insert_20130506 ( %d, %d, %I64d ) }"

    { call dbo.UIGAUserItemCharacter_insert_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
        
    1 return ( @01 )
    @01: OK int
        0        : 성공
        -1       : 유저 정보가 없음
        -2       : 메인 아이템 정보가 없음
        -3       : 이미 캐릭터 인벤토리에 등록되어 있음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */
    int nRet = -99;
    
    DBJIF3( m_kODBC.Query( L"{ call dbo.UIGAUserItemCharacter_insert_20130506 ( %d, %d, %I64d ) }",
        dwUserUID_, cCharType_, WUID_ ), return nRet );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nRet ) );

    START_LOG( clog, L"아이템의 소유 캐릭터 정보 입력" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nRet )
        << END_LOG;

    return nRet;
}

bool KSP::UIGAUserItemCharacter_delete( IN const DWORD& dwUID_, IN const char cCharType_, IN const GCITEMUID& itemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemCharacter_delete_20130506 ( %d, %d, %I64d ) }"

    { call dbo.UIGAUserItemCharacter_delete_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
        
    1 return ( @01 )
    @01: OK int
        0        : 성공
        -1       : 유저 정보가 없음
        -2       : 메인 아이템 정보가 없음
        -3       : 캐릭터 인벤토리에 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCharacter_delete_20130506 ( %d, %d, %I64d ) }", 
        dwUID_, cCharType_, itemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    if ( nOK != 0 ) {
        return false;
    }
    return true;
}

bool KSP::InsertPetItem( IN const DWORD dwUserUID_, IN const char cCharType_, IN const int nPetID_, IN const int nPromotion_, IN OUT std::vector<KItem>& vecItems_ )
{
    //promotion에 0, 1, 2를 넣어주면 되겠삼

/*
L"{ call dbo.PIGAPetItem_insert_20130402 ( %d, %d, %d, %d ) }"

{ call dbo.PIGAPetItem_insert_20130402 ( @01, @02, @03, @04 ) }
@01: @01iLoginUID_input  int
@02: @02iCharType_input  int
@03: @03iPetID_input     int
@04: @04iPromotion_input tinyint
    
1 return ( @01, @02 )
@01: OK      int
    0       : 성공
    -1      : 유저 정보가 없음
    -2      : 지급될 아이템이 없음
    -101이하: 트랜젝션 에러
@02: WIGAUID bigint
*/
    int nOK = -99;
    __int64 nWUID = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPetItem_insert_20130402 ( %d, %d, %d, %d ) }",
        dwUserUID_, cCharType_, nPetID_, nPromotion_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> nWUID );
        m_kODBC.EndFetch();
    }

    if( nOK == 0 ) {
        if( !InsertWaitItemToInven( dwUserUID_, cCharType_, KItemManager::EWIT_PET_SYSTEM, vecItems_ ) ) {
            START_LOG( cerr, L"펫 보상 아이템의 대기 리스트를 받아오는데 실패 했습니다. WIGAUID : " << nWUID ) << END_LOG;
            return false;
        }
    }
    else if( nOK == -2 ) {
        START_LOG( clog, L"펫 진화 보상 아이템이 없음." ) << END_LOG;
        return false;
    }
    else {
        START_LOG( cerr, L"펫 진화 보상 아이템 받다가 에러 발생!!" ) << END_LOG;
        return false;
    }

    return true;
}

bool KSP::RemovePet( IN const DWORD dwUID_, IN const DWORD dwPetID_, IN const GCITEMUID dwPetUID_ )
{
    // 2007/1/13. iridology. 변경된 Pet DB 에서는 삭제를 누르면 모든 케릭터가 장착한 펫의 정보가 날아감
    //call dbo.gup_pet_remove (PetUID int, LoginUID int, PetID int)
    DBJIF2( m_kODBC.Query( L"{ call dbo.gup_pet_remove ( %d, %d, %d ) }",
        dwPetUID_, dwUID_, dwPetID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if ( nOK != 0 ) {

        switch( nOK ) {
        case -2: SET_ERROR( ERR_PET_03 ); break;    //-1(유저가 존재하지 않음)
        case -3: SET_ERROR( ERR_PET_04 ); break;    //-2(이 아이템이 펫 아이템일까요?)
        case -5: SET_ERROR( ERR_PET_06 ); break;    //-3(펫 아이템이 존재하지 않음)
        case -6: SET_ERROR( ERR_PET_07 ); break;    //-5(펫이 존재하지 않음)
        default:
            SET_ERROR( ERR_UNKNOWN ); 
        };
        START_LOG( cerr, L"펫 삭제 실패 OK : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    return (nOK == 0);
}

int KSP::BuyItemToWait( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const KBuySellItemReq& kBuyInfo_, IN const std::wstring& strComment_, OUT __int64& waitUID_, OUT std::wstring& wstrItemName_ )
{
    return BuyItemToWait( dwUserUID_, strBuyerLogin_, kBuyInfo_.m_ItemID, kBuyInfo_.m_nCount, kBuyInfo_.m_nPeriod, kBuyInfo_.m_nMoneyType, kBuyInfo_.m_nPrice, kBuyInfo_.m_cCharType, kBuyInfo_.m_nCharLevel, strComment_, kBuyInfo_.m_nGrade, waitUID_, 
        wstrItemName_ );
}

int KSP::BuyItemToWait( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const GCITEMID itemID_,
                               IN const int nCount_, IN const int nPeriod_, IN const int nMoneyType_, IN const int nPrice_,
                                    IN const int nCharType_, IN const int nCharLevel_, IN const std::wstring& strComment_, IN const int nGrade_,
                                    OUT __int64& waitUID_, OUT std::wstring& wstrItemName_ )
{
    /*  
    L"{ call dbo.BIGABuyItem_insert_20130402 ( %d, N'%s', %d, %d, %d, %d, %d, %d, %d, %d, N'%s' ) }"  
      
    { call dbo.BIGABuyItem_insert_20130402 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11 ) }  
        @01iLoginUID_input     int  
    ,   @02strBuyerLogin_input nvarchar( 40 )  
    ,   @03iItemID_input       int  
    ,   @04iGradeID_input      smallint  
    ,   @05iPeriod_input       int  
    ,   @06iDuration_input     smallint  
    ,   @07iMoneyType_input    tinyint  
    ,   @08iPrice_input        int  
    ,   @09iCharType_input     tinyint  
    ,   @10iLevel_input        tinyint  
    ,   @11strComment_input    nvarchar( 80 ) = ''  
      
    1 return ( @01, @02, @03 )  
    @01: OK       int  
        0       : 성공  
        -1      : 아이템을 보유할 유저 정보가 없음  
        -2      : 아이템을 구매한 유저 정보가 없음  
        -3      : 판매 중단된 아이템을 구매하려 함(출석 포인트 제외)  
        -101이하: 트랜젝션 에러  
    @02: WIGAUID  bigint  
    @03: ItemName nvarchar( 100 )  
    */  
      
    /*  
    @07iMoneyType_input  
        1 : Cash  
        2 : Gp  
        3 : VirtualCash  
        4 : Crystal  
        5 : Gpoint  
        11: Gift( Cash )  
    */ 

    START_LOG( clog, L"아이템 구매 요청" )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( strBuyerLogin_ )
        << BUILD_LOG( itemID_ )
        << BUILD_LOG( nPeriod_ )
        << BUILD_LOG( nCount_ )
        << BUILD_LOG( nMoneyType_ )
        << BUILD_LOG( nPrice_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( nCharLevel_ )
        << BUILD_LOG( strComment_ )
        << BUILD_LOG( nGrade_ ) << END_LOG;

    int nItemGrade = nGrade_;
    if ( nItemGrade == -1 ) {
        if( nMoneyType_ == KItemManager::EMT_CRYSTAL ) {
            nItemGrade = KItem::GRADE_NORMAL;
        }
    }

    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.BIGABuyItem_insert_20130402 ( %d, N'%s', %d, %d, %d, %d, %d, %d, %d, %d, N'%s' ) }",
        dwUserUID_, strBuyerLogin_.c_str(), itemID_, nItemGrade, nPeriod_, nCount_, nMoneyType_, nPrice_, nCharType_, nCharLevel_, strComment_.c_str() ),
        return nOK );

    waitUID_ = KItem::INVALID_ITEM;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> waitUID_ >> wstrItemName_ );
    }
    m_kODBC.EndFetch();

    switch( nOK )
    {
    case 0: START_LOG( clog, L"구매 쿼리 호출 성공" ) << END_LOG; 
        break;
    case -1: START_LOG( cerr, L"구매 쿼리 호출 실패 : 유저 정보가 없음" ) << END_LOG; 
        break;
    case -2: START_LOG( cerr, L"구매 쿼리 호출 실패 : 구매한 유저 정보가 없음" ) << END_LOG; 
        break;
    case -3: START_LOG( cerr, L"구매 쿼리 호출 실패 : 판매 중단된 아이템을 구매하려 함(출석 포인트 제외)" ) << END_LOG;
    default:
        START_LOG( cerr, L"알 수 없는 이유로 구매 쿼리 호출 실패" ) << END_LOG;
    }

    _LOG_SUCCESS( nOK == 0, L"구매 쿼리 결과. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

int KSP::BuyItemToWait_Mixed( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const GCITEMID itemID_, IN const int nGrade_, 
                                     IN const int nCount_, IN const int nPeriod_, IN const int nMoneyType_, IN const int nPrice_,
                                     IN const int nCharType_, IN const int nCharLevel_, IN const int nMoneyType2_, IN const int nPrice2_,
                                     IN const std::wstring& strComment_, OUT __int64& waitUID_ )
{
    /*  
    L"{ call dbo.BIGABuyItem_mix_insert_20130402 ( %d, N'%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N'%s' ) }"  
      
    { call dbo.BIGABuyItem_mix_insert_20130402 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11, @12, @13 ) }  
        @01iLoginUID_input     int  
    ,   @02strBuyerLogin_input nvarchar( 40 )  
    ,   @03iItemID_input       int  
    ,   @04iGradeID_input      smallint  
    ,   @05iPeriod_input       int  
    ,   @06iDuration_input     smallint  
    ,   @07iMoneyType1_input   tinyint  
    ,   @08iPrice1_input       int  
    ,   @09iCharType_input     tinyint  
    ,   @10iLevel_input        tinyint  
    ,   @11iMoneyType2_input   tinyint  
    ,   @12iPrice2_input       int  
    ,   @13strComment_input    nvarchar( 80 ) = ''  
      
    1 return ( @01, @02 )  
    @01: OK       int  
        0       : 성공  
        -1      : 아이템을 보유할 유저 정보가 없음  
        -2      : 아이템을 구매한 유저 정보가 없음  
        -3      : 판매 중단된 아이템을 구매하려 함(출석 포인트 제외)  
        -101이하: 트랜젝션 에러  
    @02: WIGAUID  bigint  
    */  
      
    /*   
    @iMoneyType_  
        1 : Cash  
        2 : Gp  
        3 : VirtualCash  
        4 : Crystal  
        5 : Gpoint  
        11: Gift( Cash )  
    */  

    START_LOG( clog, L"아이템 구매 요청" )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( strBuyerLogin_ )
        << BUILD_LOG( itemID_ )
        << BUILD_LOG( nGrade_ )
        << BUILD_LOG( nPeriod_ )
        << BUILD_LOG( nCount_ )
        << BUILD_LOG( nMoneyType_ )
        << BUILD_LOG( nPrice_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( nCharLevel_ )
        << BUILD_LOG( nMoneyType2_ )
        << BUILD_LOG( nPrice2_ )
        << BUILD_LOG( strComment_ ) << END_LOG;

    DBJIF3( m_kODBC.Query( L"{ call dbo.BIGABuyItem_mix_insert_20130402 ( %d, N'%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N'%s' ) }",
        dwUserUID_, strBuyerLogin_.c_str(), itemID_, nGrade_, nPeriod_, nCount_, nMoneyType_, nPrice_, nCharType_, nCharLevel_, nMoneyType2_, nPrice2_, strComment_.c_str() ),
        return KItem::INVALID_ITEM );

    int nOK = -99;
    waitUID_ = KItem::INVALID_ITEM;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> waitUID_ );
    }
    m_kODBC.EndFetch();

    if ( nOK != 0 ) {
        START_LOG( cerr, L"구매 쿼리 실패. nOK: " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return nOK;
}

int KSP::BuyItemToWait_Mixed( IN const DWORD dwUserUID_, IN const std::wstring& strBuyerLogin_, IN const KBuySellItemReq& kBuyInfo_, IN const int nMoneyType2_, IN const int nPrice2_, IN const std::wstring& strComment_, OUT __int64& waitUID_ )
{
    // Grade 정보 구하기.
    int nGradeTableID = SiKItemManager()->GetGradeTableID( kBuyInfo_.m_ItemID );
    int nGrade = SiKItemManager()->m_kGradeDecider.GetGrade( nGradeTableID );
    return BuyItemToWait_Mixed( dwUserUID_, strBuyerLogin_, kBuyInfo_.m_ItemID, nGrade, kBuyInfo_.m_nCount, kBuyInfo_.m_nPeriod, kBuyInfo_.m_nMoneyType,
        kBuyInfo_.m_nPrice, kBuyInfo_.m_cCharType, kBuyInfo_.m_nCharLevel, nMoneyType2_, nPrice2_, strComment_, waitUID_ );
}

bool KSP::RollbackBuyItem( IN const __int64& waitItemUID_ )
{
//     mk8253. 20091117.
//     구매실패
    /*
    L"{ call dbo.WIGAWaitItem_buy_rollback_20130402 ( %I64d ) }"
    { call dbo.WIGAWaitItem_buy_rollback_20130402 ( @01 ) }
    @01: @01iWIGAUID_input bigint

    1 return ( @1 )
    @1: OK
    0       : 성공
    -1      : 구매중인 아이템이 없음
    -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_buy_rollback_20130402 ( %I64d ) }", waitItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    switch( nOK )
    {
    case 0:
        START_LOG( clog, L"아이템 구매 롤백 막기 성공!!" )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    case -1:
        START_LOG( cerr, L"구매 중인 아이템이 없음" )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    default:
        START_LOG( cerr, L"아이템 구매 롤백 중 알 수 없는 이유로 실패")
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    }

    return nOK == 0;
}

bool KSP::UpdateWaitItemToEnabled( IN const __int64 waitItemUID_ )
{
//     mk8253. 20091117.
//     구매성공
// 
    /*
    L"{ call dbo.WIGAWaitItem_wait_update_20130402 ( %d ) }"

    { call dbo.WIGAWaitItem_wait_update_20130402 ( @01 ) }
    @01: @01iWIGAUID_input bigint
        
    1 return ( @01 )
    @01: OK int
        0       : 성공
        -1      : 구매중인 아이템이 없음
        -101이하: 트랜젝션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_wait_update_20130402 ( %I64d ) }", waitItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    switch( nOK )
    {
    case 0:
        START_LOG( clog, L"대기 리스트에서 인벤 넣을 수 있는 상태로 전이" )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    case -1:
        START_LOG( cerr, L"구매 중인 아이템이 없음" )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    default:
        START_LOG( cerr, L"아이템 상태변경 중 알 수 없는 이유로 실패 nOK : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    }

    return nOK == 0;
}

bool KSP::UpdateWaitItemToDelete( IN const __int64 waitItemUID_ )
{
    /*
    L"{ call dbo.WIGAWaitItem_delete_20130402 ( %I64d ) }"

    { call dbo.WIGAWaitItem_delete_20130402 ( @01 ) }
    @01: @01iWIGAUID_input bigint

    1 return ( @01 )
    @01: OK
    0       : 성공
    -1      : 대기중인 아이템이 없음
    -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_delete_20130402 ( %I64d ) }", waitItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    switch( nOK )
    {
    case 0:
        START_LOG( clog, L"대기 리스트에서 인벤 넣을 수 있는 상태로 전이" )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    case -1:
        START_LOG( cerr, L"구매 중인 아이템이 없음" )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << END_LOG; 
        break;

    default:
        START_LOG( cerr, L"아이템 상태변경 중 알 수 없는 이유로 실패 nOK : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
        break;
    }

    return nOK == 0;
}

bool KSP::GetWaitInnerItemList( IN const DWORD dwUserUID_, IN const char cCharType_, IN const __int64 waitItemUID_, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems_ )
{
    //mk8253. 20091117.
    /*
    대기아이템리턴, 세트인지 패키지인지 판단하지 않음
    L"{ call dbo.WIGAWaitItem_select_wigauid_20130402 ( %d, %d, %I64d ) }"
    { call dbo.WIGAWaitItem_select_wigauid_20130402 ( @01, @02, @03 ) }
    @1: @01iLoginUID_input int
    @2: @02iCharType_input int
    @3: @03iWIGAUID_input  bigint

    1 return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11 )
    @01: BuyerNickname nvarchar( 24 )
    @02: ItemID        int
    @03: GradeID       int
    @04: Period        int
    @05: CNT           int
    @06: GetType       int
    @07: Comment       nvarchar( 80 )
    @08: ItemClass     int
    0: 단품아이템
    1: 세트아이템
    2: 패키지아이템
    @09: ItemLevel     int
    @10: StrengthLevel int
    @11: CharType	   int
    */
    const int CLASS_NORMAL   = 0;
    const int CLASS_SET      = 1;
    const int CLASS_PACKAGE  = 2;

    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_select_wigauid_20130402 ( %d, %d, %I64d ) }", 
        dwUserUID_, cCharType_, waitItemUID_ ) );

    std::wstring strBuyerNick;
    KItem kItem;
    int nGetType = 0;
    int nItemClass = -99;
    std::wstring strComment;
    int nGradeID = 0;
    int nEnchantLevel = 0;
    int nCharType = -1;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC
            >> strBuyerNick
            >> kItem.m_ItemID
            >> nGradeID
            >> kItem.m_nPeriod
            >> kItem.m_nCount
            >> nGetType
            >> strComment
            >> nItemClass
            >> kItem.m_sEquipLevel
            >> nEnchantLevel
            >> nCharType );
    }
    m_kODBC.EndFetch();

    kItem.m_cEnchantLevel = static_cast<char>( nEnchantLevel );
    kItem.m_cGradeID = static_cast<char>( nGradeID );
    kItem.m_nInitCount = kItem.m_nCount;
    kItem.m_cCharType = static_cast<char>( nCharType );

    switch( nItemClass )
    {
    case CLASS_NORMAL:
        vecItems_.push_back( std::pair<__int64,KItem>( waitItemUID_, kItem ) );
        break;
    case CLASS_SET:
        GetWaitInnerItemList_SetItem( waitItemUID_, kItem, vecItems_ );
        break;
    case CLASS_PACKAGE:
        GetWaitInnerItemList_Package( waitItemUID_, kItem, vecItems_ );
        break;
    default:
        START_LOG( cerr, L"Wait Item의 Class가 비정상입니다. nItemClass : " << nItemClass )
            << BUILD_LOG( strBuyerNick )
            << BUILD_LOG( kItem.m_ItemID )
            << BUILD_LOG( kItem.m_nCount )
            << BUILD_LOG( kItem.m_nPeriod ) << END_LOG;
        break;
    }

    if( nItemClass == -99 ) { // 해당값은 DB에서 query exception으로 인해서 정보를 못가져 왔을 경우에 대한 처리이다.
        return false;
    }

    return true;
}

bool KSP::GetWaitInnerItemList_SetItem( IN const __int64 waitItemUID_, IN const KItem& kItem_, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems_ )
{
//     mk8253. 2009124.
//         세트 아이템인지 체크
// 
//         L"{ call dbo.IIGAItemInfoSet_check( %d ) }"
// 
//     { call dbo.IIGAItemInfoSet_check ( @1 ) }
//         @1 ; @iItemID_ int
// 
//         n return ( @1 )
//         @1 ; SubItemID

    DBJIF2( m_kODBC.Query( L"{ call dbo.IIGAItemInfoSet_check( %d ) }", kItem_.m_ItemID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kItem = kItem_;

        VERIFY_QUERY( m_kODBC
            >> kItem.m_ItemID );

        vecItems_.push_back( std::pair<__int64,KItem>( waitItemUID_, kItem ) );
    }
    return true;
}

bool KSP::GetWaitInnerItemList_Package( IN const __int64 waitItemUID_, IN const KItem& kItem_, IN OUT std::vector<std::pair<__int64,KItem>>& vecItems_ )
{
    /*  
    L"{ call dbo.PIGAPackageItem_1_select_20120907( %d ) }"  
      
    { call dbo.PIGAPackageItem_1_select_20120907 ( @1 ) }  
    @1: @iItemID_ int  
          
    n return ( @1, @2, @3, @4, @5, @6 )  
    @1: SubItemID     int  
    @2: SubGradeID    smallint  
    @3: SubPeriod     int  
    @4: SubDuration   smallint  
    @5: StrengthLevel int  
    @6: EquipLevel    int  
    */  

    DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPackageItem_1_select_20120907( %d ) }", kItem_.m_ItemID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kItem = kItem_;

        VERIFY_QUERY( m_kODBC
            >> kItem.m_ItemID
            >> kItem.m_cGradeID
            >> kItem.m_nPeriod
            >> kItem.m_nCount
            >> kItem.m_cEnchantLevel
            >> kItem.m_sEquipLevel );
        kItem.m_nInitCount = kItem.m_nCount;

        vecItems_.push_back( std::pair<__int64,KItem>( waitItemUID_, kItem ) );
    }
    return true;
}

bool KSP::DivideSellItem( IN const DWORD dwUID_, IN const GCITEMUID itemUID_, IN const GCITEMID itemID_, IN const int nFactor_, OUT int& nFullGP_, OUT __int64& waitUID_, IN const char cCharType_ )
{
    // 이 SP 는 GP 반환이 없으면 0 을 반환 해준다.
    // 그래서 조심 해야 한다.

    /*
    L"{ call dbo.DSG_divide_sell_item_20130415( %d, %I64d, %d, %d, %d ) }"

    { call dbo.DSG_divide_sell_item_20130415 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @03: @03iCharType_input int
    @04: @04iItemID_input   int
    @05: @05iFactor_input   int
        
    1 return ( @01, @02, @03 )
    @01: OK      int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -4      : 되팔기 가능한 아이템이 아님
        -101이하: 트랜젝션 에러
    @02: RValue  float
    @03: WIGAUID bigint
    */
    nFullGP_ = 0;
    waitUID_ = KItem::INVALID_ITEM;

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.DSG_divide_sell_item_20130415 ( %d, %I64d, %d, %d, %d ) }",
        dwUID_, itemUID_, cCharType_, itemID_, nFactor_ ) );
    if ( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK
            >> nFullGP_
            >> waitUID_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"수량 나눠 팔기, OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);
}

bool KSP::SellItem( IN const DWORD dwUID_, IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, OUT int& nFullGP_, OUT __int64& waitUID_, IN const char cCharType_ )
{
    /*
    L"{ call dbo.UIGAUserItem_sell_20130415( %d, %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_sell_20130415 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemID_input   int
    @03: @03iItemUID_input  bigint
    @04: @04iCharType_input int
        
    1 return ( @01 )
    @01: OK        int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : GoodsInfoList에 아이템 정보가 없음
        -3      : 메인 아이템 정보 없음
        -4      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -5      : 기간제 아이템
        -6      : 존재하지 않는 수량 아이템
        -101이하: 트랜젝션 에러
    @02: iReturnGP int
    @03: iWIGAUID  bigint
    */
    nFullGP_ = 0;
    waitUID_ = KItem::INVALID_ITEM;
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_sell_20130415( %d, %d, %I64d, %d ) }",
        dwUID_, itemID_, itemUID_, cCharType_ ) );
    if ( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK
            >> nFullGP_
            >> waitUID_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"수량 나눠 팔기, OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);
}

bool KSP::GetGCPointPriceAndInfo( IN const GCITEMID ItemID_, OUT int& nPeriod_, OUT int& nDuration_, OUT int& nPrice_ )
{
    //mk8253. 20100120.
    //    출석부 아이템 기간 수량 가격 정보

    //    L"{ call dbo.IPGAItemPriceGPoint_select( %d ) }"

    //{ call dbo.IPGAItemPriceGPoint_select ( @1 ) }
    //@1 ; @iItemID_ [int]

    //1 return ( @1, @2, @3, @4 )
    //    @1 ; Period
    //    @2 ; Duration
    //    @3 ; Price

    bool bRet = false;
    DBJIF2( m_kODBC.Query( L"{ call dbo.IPGAItemPriceGPoint_select( %d ) }", ItemID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nPeriod_
            >> nDuration_
            >> nPrice_ );
        bRet = (nPrice_ > 0);
    }
    m_kODBC.EndFetch();

    return bRet;
}

bool KSP::GetGCPointPriceAndInfo_CashCheck( IN const GCITEMID ItemID_, IN const bool& bIsCashItem_, OUT int& nPeriod_, OUT int& nDuration_, OUT int& nPrice_ )
{
    /*mk8253. 20100120.
    출석부 아이템 기간 수량 가격 정보

    { call dbo.IPGAItemPriceGPoint_select ( @1, @2 ) }
    @1 ; @iItemID_ [int]
    @2 ; IsCashItem bit (0:포인트, 1:캐쉬)

    1 return ( @1, @2, @3, @4 )
        @1 ; Period
        @2 ; Duration
        @3 ; Price
    */

    bool bRet = false;
    DBJIF2( m_kODBC.Query( L"{ call dbo.IPGAItemPriceGPoint_select( %d, %d ) }",
        ItemID_, bIsCashItem_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nPeriod_
            >> nDuration_
            >> nPrice_ );
        bRet = (nPrice_ > 0);
    }
    m_kODBC.EndFetch();

    return bRet;
}

bool KSP::IsGCPointItem( IN const GCITEMID ItemID_ )
{
    int nTempPeriod, nTempPrice, nTempCuration;
    return GetGCPointPriceAndInfo( ItemID_, nTempPeriod, nTempPrice, nTempCuration );
}

bool KSP::IsGCPointItem_CashCheck( IN const GCITEMID ItemID_ )
{
    int nTempPeriod, nTempPrice, nTempCuration;
    return GetGCPointPriceAndInfo_CashCheck( ItemID_, false, nTempPeriod, nTempPrice, nTempCuration );
}

int KSP::GetBreakupReelPoint( IN const DWORD dwLoginUID_ )
{
    //L"{ call dbo.UIGAUserItem_breakupdata_select( %d ) }"
    //
    //{ call dbo.UIGAUserItem_breakupdata_select ( @1 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @strLogin_  nvarchar(24)
    //
    //1 return ( @1 )
    //@1 ; Data

    int nPoint = 0;
    DBJIF3( m_kODBC.Query( L"{ call dbo.UIGAUserItem_breakupdata_select( %d ) }",
        dwLoginUID_ ), return 0 );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nPoint );
    }
    m_kODBC.EndFetch();

    return nPoint;
}

bool KSP::SetBreakupReelPoint( IN const DWORD dwLoginUID_, IN const int nReelCount_ )
{
    //L"{ call dbo.UIGAUserItem_breakupdata_merge( %d, %d ) }"
    //
    //{ call dbo.UIGAUserItem_breakupdata_merge ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iData_     int
    //
    //1 return ( @1 )
    //@1 ; OK
    //0        ; 성공
    //-1       ; 유저정보가없음
    //-101이하; 트랜젝션에러

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_breakupdata_merge( %d, %d ) }",
        dwLoginUID_, nReelCount_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"아이템 해체 정보 기록, OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KSP::EraseItemByBreakup( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const GCITEMUID itemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @04: @03iDelstate_input tinyint
        
    1 return ( @01 )
    @01: OK
        0        : 성공
        -1       : 메인 아이템 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
        -1000    : 사용 중지된 sp인데 사용하려 함
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }",
        dwLoginUID_, itemUID_, KItemManager::EDEL_DEL_BREAKUP ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"아이템 해체시 아이템 삭제, OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);

}

void KSP::GetItemBreakupExceptionList( OUT std::set<GCITEMID>& setExceptionList_ )
{
    //L"{ call dbo.IIGAItemInfobreakup_select }"
    //{ call dbo.IIGAItemInfobreakup_select }
    //n return ( @1 )
    //  @1 ; ItemID

    setExceptionList_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.IIGAItemInfobreakup_select }" ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMID itemID;
        VERIFY_QUERY( m_kODBC >> itemID );
        setExceptionList_.insert( itemID );
    }
}

bool KSP::InsertCashWaitItemToInven( IN const DWORD dwUserUID_, IN const char cCharType_, IN const std::wstring& strLogin_, IN OUT KEVENT_GET_WEB_CASH_ITEM_ACK& kPacket_ )
{
    std::vector< std::pair< __int64, KItem > > vecWaitItems;
    if ( GetWaitItemList( dwUserUID_, cCharType_,  KItemManager::EWIT_CASH_BUY, vecWaitItems ) == false ) {
        return false;
    }

    /*
    아래 코드는 InsertSingleWaitItemToInvenOrVDepot()이 실패하는 경우 문제가 있다
    아이템 획득 실패시 대기 아이템에 계속 남아있게 되고, 이럴경우 캐시백을 반복적으로 받게되는 문제가 있다
    아이템 획득 성공시에만 캐시백을 지급하도록 수정해야 한다
    */
    // vecWaitItems 루프돌면서 가상캐시 전부 계산해야 함
    typedef std::vector< std::pair< __int64, KItem > >::value_type vecWaitItemsType;
    BOOST_FOREACH( vecWaitItemsType& rWaitItemsType, vecWaitItems ) {
        KItem& rItem = rWaitItemsType.second;

        int nPrice = 0;
        int nMoneyType = 0;
        DWORD dwAddCashPoint = 0;
        DWORD dwResultCashPoint = 0;
        if ( true == GetPrice( rItem.m_ItemID, rItem.m_nCount, rItem.m_nPeriod, nPrice, nMoneyType ) ) {
            // 가상캐시 캐시백 계산
            CheckCashback( dwUserUID_, strLogin_, rItem.m_ItemID, nPrice, dwAddCashPoint, dwResultCashPoint );
        }
        else {
            // 가상캐시 관련 에러처리 할 부분이 있는가?

            START_LOG( cerr, L"웹구매 동작 중 DB에 해당 아이템 가격 정보가 없어서 캐시백을 계산하지 못함" ) << END_LOG;
        }
        kPacket_.m_kVirtualCashInfo.m_dwAddCashPoint += dwAddCashPoint; // 획득한 가상캐시는 누적
        kPacket_.m_kVirtualCashInfo.m_dwCashPoint = dwResultCashPoint; // 유저가 보유한 총 가상캐시 누적액
    }

    std::vector< std::pair< __int64, KItem > >::iterator vit;
    for ( vit = vecWaitItems.begin() ; vit != vecWaitItems.end() ; ++vit ) {
        InsertSingleWaitItemToInvenOrVDepot( dwUserUID_, cCharType_, vit->first, KItemManager::EWIT_CASH_BUY, kPacket_.m_vecGetItems );
    }

    return true;
}

bool KSP::StatrecordResolution( IN const DWORD dwLoginUID_, IN const int nWidth_, IN const int nHeight_, IN const int nMaxWidth_, IN const int nMaxHeight_ )
{
    //{ call dbo.RSGAResolutionSetting_merge ( @1, @2, @3, @4, @5 ) }
    //@1 ; @iLoginUID_  int
    //@2 ; @iOldWidth_  smallint
    //@3 ; @iOldHeight_ smallint
    //@3 ; @iNewWidth_  smallint
    //@3 ; @iNewHeight_ smallint
    //
    //@1 ; OK
    //0        ; 성공
    //-1       ; 유저 정보가 없음
    //-101이하 ; 트랜젝션 에러


    DBJIF2( m_kODBC.Query( L"{ call dbo.RSGAResolutionSetting_merge( %d, %d, %d, %d, %d ) }",
        dwLoginUID_, nWidth_, nHeight_, nMaxWidth_, nMaxHeight_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {

        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"해상도 기록.."
        << " OK : " << nOK
        << ", Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);
}

bool KSP::LowLevelCharEquip( IN const DWORD dwLoginUID_ )
{
    //접속시 저 레벨유저에게 아이템 지급
    //L"{ call dbo.LCGALowLevelChar_insert( %d ) }"
    //
    //{ call dbo.LCGALowLevelChar_insert ( @1 ) }
    //@1 ; @iLoginUID_ int
    //
    //1 return ( @1 )
    //@1 ; OK
    //0        ; 성공
    //-1       ; 유저 정보가 없음
    //-101이하 ; 트랜젝션 에러

    DBJIF2( m_kODBC.Query( L"{ call dbo.LCGALowLevelChar_insert( %d ) }",
        dwLoginUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"접속시 저 레벨유저에게 아이템 지급"
        << " OK : " << nOK
        << ", Query : " << m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KSP::InsertSingleItemJob( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const GCITEMID itemID_,
                              IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_, IN int nGrade_/*=-1*/, IN int nItemLevel_/*=-1*/, IN int nEnchantLevel_/*=0*/, IN int nStatus_/*2*/ )
{
    // 대기 아이템 리스트에 추가 한다.
    // 어느 경로로 얻었는지에 대한 자세한 Type추가.
    __int64 waitUID = 0;

    _JIF( m_kCommonSP.InsertToWaitItemJob( dwLoginUID_, cCharType_, itemID_, nCount_, nPeriod_, nGetType_, waitUID, nGrade_, nItemLevel_, nEnchantLevel_, nStatus_ ) >= 0 , return false );
    // InsertToWaitItem함수에서 리턴해준 waitUID를 가지고 진행한다.
    InsertSingleWaitItemToInvenOrVDepot( dwLoginUID_, cCharType_, waitUID, nGetType_, vecItems_ );

    return true;
}

bool KSP::InsertSingleItemJob_WaitUID( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const GCITEMID itemID_,
                              IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_, OUT __int64& waitUID_, IN int nGrade_/*=-1*/, IN int nItemLevel_/*=-1*/, IN int nEnchantLevel_/*=0*/, IN int nStatus_/*2*/ )
{
    // 대기 아이템 리스트에 추가 한다.
    // 어느 경로로 얻었는지에 대한 자세한 Type추가.
    __int64 waitUID = 0;

    _JIF( m_kCommonSP.InsertToWaitItemJob( dwLoginUID_, cCharType_, itemID_, nCount_, nPeriod_, nGetType_, waitUID, nGrade_, nItemLevel_, nEnchantLevel_, nStatus_ ) >= 0 , return false );
    // InsertToWaitItem함수에서 리턴해준 waitUID를 가지고 진행한다.
    InsertSingleWaitItemToInvenOrVDepot( dwLoginUID_, cCharType_, waitUID, nGetType_, vecItems_ );

    waitUID_ = waitUID;

    return true;
}

bool KSP::InsertSingleItemJobForDepot( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const GCITEMID itemID_,
                              IN const int nCount_, IN const int nPeriod_, IN const int nGetType_, IN OUT std::vector<KItem>& vecItems_, IN int nGrade_/*=-1*/, IN int nItemLevel_/*=-1*/, IN int nEnchantLevel_/*=0*/, IN int nStatus_/*2*/ )
{
    // 대기 아이템 리스트에 추가 한다.
    // 어느 경로로 얻었는지에 대한 자세한 Type추가.
    __int64 waitUID = 0;

    _JIF( m_kCommonSP.InsertToWaitItemJob( dwLoginUID_, cCharType_, itemID_, nCount_, nPeriod_, nGetType_, waitUID, nGrade_, nItemLevel_, nEnchantLevel_, nStatus_ ) >= 0 , return false );
    // InsertToWaitItem함수에서 리턴해준 waitUID를 가지고 진행한다.
    InsertSingleWaitItemToInvenForDepot( dwLoginUID_, cCharType_, waitUID, nGetType_, vecItems_ );

    return true;
}

bool KSP::GetCharacterInfo_1( IN const DWORD dwLoginUID_, IN const char cCharType_, OUT KCharacterInfo& kCharInfo_ )
{
    /*  
    L"{ call dbo.CIGACharacterInfo_select_1_20130625 ( %d, %d ) }"  
      
    { call dbo.CIGACharacterInfo_select_1_20130625 ( @01, @02 ) }  
        @01iLoginUID_input int  
    ,   @02iCharType_input int  
          
    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11, @12, @13 )  
    @01: SlotNo       int  
    @02: CharType     tinyint  
    @03: Promotion    tinyint  
    @04: ExpS4        bigint  
    @05: Win          int  
    @06: Lose         int  
    @07: Level        int  
    @08: Exp          bigint  
    @09: GamePoint    bigint  
    @10: DefaultBonus int  
    @11: SpecialBonus int  
    @12: InvenSize    int  
    @13: CoordiSize   int  
    */  

    DWORD dwLevel = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_select_1_20130625 ( %d, %d ) }",
        dwLoginUID_, (int)cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC 
            >> kCharInfo_.m_dwSlotNum
            >> kCharInfo_.m_cCharType
            >> kCharInfo_.m_cPromotion
            >> kCharInfo_.m_biExp
            >> kCharInfo_.m_iWin
            >> kCharInfo_.m_iLose
            >> dwLevel
            >> kCharInfo_.m_biTotalExp
            >> kCharInfo_.m_nGamePoint
            >> kCharInfo_.m_kBonusPoint.m_nBaseBonus
            >> kCharInfo_.m_kBonusPoint.m_nSpecialBonus
            >> kCharInfo_.m_nInvenCapacity
            >> kCharInfo_.m_nLookInvenCapacity );

        m_kODBC.EndFetch();

        kCharInfo_.m_cCharType = cCharType_;
        kCharInfo_.m_biInitExp = kCharInfo_.m_biExp;
        kCharInfo_.m_iInitWin = kCharInfo_.m_iWin;
        kCharInfo_.m_iInitLose = kCharInfo_.m_iLose;
        kCharInfo_.m_cCurrentPromotion = 0;
        kCharInfo_.m_kEquipPetInfo.Init();
        kCharInfo_.m_biInitTotalExp = kCharInfo_.m_biExp;
        kCharInfo_.m_biTotalExp = kCharInfo_.m_biExp;
        kCharInfo_.m_nInitGP = kCharInfo_.m_nGamePoint;
    }
    else {
        return false;
    }

    return true;
}

bool KSP::NormalItemToCoordiItem( IN const DWORD dwLoginUID_, IN const GCITEMUID itemUID_, IN const char cCharType_, OUT int& nOK_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordi_insert_20130415 ( %d, %d, %I64d ) }"

    { call dbo.UIGAUserItemCoordi_insert_20130415 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
        
    1 return ( @01 )
    @01: OK int
        0       : 코디 아이템 변경 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -4      : 이미 코디 아이템임R
        -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi_insert_20130415( %d, %d, %I64d ) }",
        dwLoginUID_, cCharType_, itemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0 || nOK == -3 || nOK == -4, L" 일반 아이템에서 룩 아이템으로 변환.. Ret : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    nOK_ = nOK; // 결과값 세팅.

    return (nOK == 0 || nOK == -3);
}

bool KSP::CoordiItemToNormalItem( IN const DWORD dwLoginUID_, IN const GCITEMUID itemUID_, IN const char cCharType_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordi_delete_20130415 ( %d, %d, %I64d ) }"

    { call dbo.UIGAUserItemCoordi_delete_20130415 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
        
    1 return ( @01 )
    @01: OK int
        0       : 일반 아이템 변경 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -4      : 코디 아이템이 아님
        -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi_delete_20130415( %d, %d, %I64d ) }",
        dwLoginUID_, cCharType_, itemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0 || nOK == -3, L" 룩 아이템에서 일반 아이템으로 변환.. Ret : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0 || nOK == -3);
}

bool KSP::UIGAUserItemCoordi_select( IN const DWORD dwLoginUID_, OUT std::vector<GCITEMUID>& vecLookUIDs_, IN const char cCharType_ )
{
    //{ call dbo.UIGAUserItemCoordi_select_20100715 ( @1 ) }
    //@1 ; @iLoginUID_ int
    //
    //n return ( @1 )
    //@1 ; ItemUID int
    /*
    L"{ call dbo.UIGAUserItemCoordi_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemCoordi_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01 )
    @01: ItemUID bigint
    */
    vecLookUIDs_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi_select_20130415( %d, %d ) }", dwLoginUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID dwItemUID = 0;
        VERIFY_QUERY( m_kODBC >> dwItemUID );
        if ( dwItemUID > 0 ) {
            vecLookUIDs_.push_back( dwItemUID );
        }
    }

    START_LOG( clog, L"룩 Item UID 얻어 옴. UID : " << dwLoginUID_
        << L", Size : " << vecLookUIDs_.size() ) << END_LOG;
    return true;
}

bool KSP::UIGAUserItemCoordi_select_20130415( IN const DWORD dwLoginUID_, IN const char cCharType_, OUT std::vector<KItem>& vecLookItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordi2_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemCoordi2_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: ItemUID bigint
    @02: ItemID  int
    */
    vecLookItem_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi2_select_20130415( %d, %d ) }", dwLoginUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        KItem kItem;

        VERIFY_QUERY( m_kODBC >> kItem.m_ItemUID >> kItem.m_CoordItemID );

        if ( kItem.m_ItemUID > 0 ) {
            vecLookItem_.push_back( kItem );
        }
    }

    START_LOG( clog, L"룩 Item 얻어 옴. UID : " << dwLoginUID_
        << L", Size : " << vecLookItem_.size() ) << END_LOG;

    return true;
}

bool KSP::UIGAUserItemCoordi_merge( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const KItem& kItem_ )
{
/*
L"{ call dbo.UIGAUserItemCoordi2_merge_20111109 ( %d, %I64d, %I64d, %d ) }"

{ call dbo.UIGAUserItemCoordi2_merge_20111109 ( @1, @2, @3, @4 ) }
@1: @iLoginUID_     int
@2: @iInnerItemUID_ bigint
@3: @iOuterItemUID_ bigint
@4: @iItemID_       int

1 return ( @1 )
@1: OK int
    0       : 성공
    -1      : 유저 정보가 없음
    -2      : 내형 아이템 정보가 없음
    -101이하: 트랜잭션 에러
*/
    /*
    L"{ call dbo.UIGAUserItemCoordi2_merge_20130415 ( %d, %d, %I64d, %I64d, %d ) }"

    { call dbo.UIGAUserItemCoordi2_merge_20130415 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input     int
    @02: @02iCharType_input     int
    @03: @03iInnerItemUID_input bigint
    @04: @04iOuterItemUID_input bigint
    @05: @05iItemID_input       int

    1 return ( @1 )
    @1: OK int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi2_merge_20130415( %d, %d, %I64d, %I64d, %d ) }", 
        dwLoginUID_, cCharType_, kItem_.m_ItemUID, kItem_.m_CoordItemUID, kItem_.m_ItemID ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"코디 아이템 합성 실패.."
        << L" UID : " << dwLoginUID_
        << L", Coordi Inner ItemID : " << kItem_.m_ItemID
        << L", Coordi Inner ItemUID : " << kItem_.m_ItemUID
        << L", Coordi Outer ItemID : " << kItem_.m_CoordItemID
        << L", Coordi Outer ItemUID : " << kItem_.m_CoordItemUID
        << L", OK : " << nOK ) << END_LOG;

    return (nOK == 0);
}

bool KSP::UIGAUserItemCoordiEquip_select( IN const DWORD dwLoginUID_, OUT std::vector< boost::tuple<char, int, GCITEMUID > >& vecLookEquips_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiEquip_select_20100715 ( %d ) }"

    { call dbo.UIGAUserItemCoordiEquip_select_20100715 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03, @04 )
    @01iItemUID     int
    @02iCharacterID int
    @03iSetID       int
    @04iItemID      int
    */

    vecLookEquips_.clear();
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiEquip_select_20100715( %d ) }", dwLoginUID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nCharType = -1;
        int nCoordiID = -1;
        GCITEMUID dwItemUID = 0;
        GCITEMID dwItemID = 0;

        VERIFY_QUERY( m_kODBC >> dwItemUID >> nCharType >> nCoordiID >> dwItemID );

        if ( dwItemUID > 0 && nCharType >= 0 && nCoordiID >= 0 ) {
            vecLookEquips_.push_back( boost::tuples::make_tuple( (char)(nCharType), nCoordiID, dwItemUID ) );
        }
    }
    return true;
}

bool KSP::SetUIGACharItemCoordiEquip( IN const DWORD dwLoginUID_, IN const std::vector<KItem>& vecInven_, IN OUT std::map<char,KCharacterInfo>& mapCharacterInfo_, IN std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiEquip_select_20100715 ( %d ) }"

    { call dbo.UIGAUserItemCoordiEquip_select_20100715 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03, @04 )
    @01iItemUID     int
    @02iCharacterID int
    @03iSetID       int
    @04iItemID      int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiEquip_select_20100715( %d ) }", dwLoginUID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        char cCharType = -1;
        int nCoordiID = -1;
        KEquipItemInfo equipElement;

        VERIFY_QUERY( m_kODBC >> equipElement.m_dwUID >> cCharType >> nCoordiID >> equipElement.m_dwID );

        // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
        std::vector<KItem>::const_iterator vit;
        vit = std::find_if( vecInven_.begin(), vecInven_.end(),
            boost::bind( &KItem::m_ItemUID, _1 ) == equipElement.m_dwUID );

        // 보유 하고 있지 않으면 장착 하지 않음..
        if ( vit != vecInven_.end() ) {
            equipElement.m_dwID = vit->m_ItemID;
            equipElement.m_vecAttribute = vit->m_vecAttribute;
            equipElement.m_vecSocket = vit->m_vecSocket;
            equipElement.m_cGradeID = vit->m_cGradeID;
            equipElement.m_nEquipLevel = static_cast<int>( vit->m_sEquipLevel );
            equipElement.m_cEnchantLevel = vit->m_cEnchantLevel;
            equipElement.m_cEnchantEquipGradeID = vit->m_cEnchantEquipGradeID;
            equipElement.m_DesignCoordiID = vit->m_DesignCoordiID;
        } else {
            std::vector<std::pair<GCITEMUID, GCITEMID>>::iterator vitDesignCoordi;
            vitDesignCoordi = std::find_if( vecDesignCoordi_.begin(), vecDesignCoordi_.end(),
                boost::bind( &std::pair<GCITEMUID, GCITEMID>::first, _1) == equipElement.m_dwUID );
            if( vitDesignCoordi != vecDesignCoordi_.end() ) {
                equipElement.m_DesignCoordiID = vitDesignCoordi->second;
            }
        }

        std::map<char, KCharacterInfo>::iterator mit;
        mit = mapCharacterInfo_.find( cCharType );
        if( mit == mapCharacterInfo_.end() ) {
            START_LOG( cwarn, L"코디 장착 정보에 맞는 캐릭터 타입 없음, UID : " << dwLoginUID_ )
                << BUILD_LOGc( cCharType )
                << BUILD_LOG( equipElement.m_dwUID )
                << BUILD_LOG( equipElement.m_dwID )
                << BUILD_LOGc( equipElement.m_cGradeID )
                << BUILD_LOGc( equipElement.m_cEnchantLevel )
                << BUILD_LOG( equipElement.m_DesignCoordiID )
                << BUILD_LOG( equipElement.m_vecAttribute.size() )
                << BUILD_LOG( equipElement.m_vecSocket.size() ) << END_LOG;

            continue;
        }

        mit->second.m_vecLookEquips.push_back( equipElement );
    }
    return true;
}

bool KSP::UIGAUserItemCoordiName_select( IN const DWORD dwLoginUID_, OUT std::vector< boost::tuple<char, int, std::wstring> >& vecCoordiName_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiName_select_20130415 ( %d ) }"

    { call dbo.UIGAUserItemCoordiName_select_20130415 ( @01 ) }
    @01: @01iLoginUID_input int

    n return ( @01, @02, @03 )
    @01: CharacterID int
    @02: SetID       int
    @03: Name        nvarchar( 6 )
    */
    vecCoordiName_.clear();
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiName_select_20130415( %d ) }", dwLoginUID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nCharType = -1;
        int nCoordiID = -1;
        std::wstring strName;

        VERIFY_QUERY( m_kODBC >> nCharType >> nCoordiID >> strName );

        if ( nCoordiID >= 0 && nCharType >= 0 ) {
            vecCoordiName_.push_back( boost::tuples::make_tuple( (char)(nCharType), nCoordiID, strName ) );
        }
    }

    START_LOG( clog, L"코디 이름 얻어 오기. UID " << dwLoginUID_
        << L", Size : " << vecCoordiName_.size() ) << END_LOG;
    return true;
}

bool KSP::UIGAUserItemCoordiEquip_delete( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const GCITEMUID itemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiEquip_delete_20130415 ( %d, %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemCoordiEquip_delete_20130415 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
    @04: @04iSetID_input    int
        
    1 return ( @01 )
    @01: OK int
        0       : 코디 아이템 해제 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -4      : 코디 아이템이 아님
        -5      : 장착 정보가 없음
        -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiEquip_delete_20130415( %d, %d, %I64d, %d ) }",
        dwLoginUID_, (int)cCharType_, itemUID_, nCoordiID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 룩 아이템장착 제거시 실패.."
            << L" UID : " << dwLoginUID_
            << L", Char Type : " << (int)cCharType_
            << L", Coordi ID : " << nCoordiID_
            << L", ItemUID : " << itemUID_
            << L", OK : " << nOK ) << END_LOG;
    }

    return (nOK == 0);

}

bool KSP::UIGAUserItemCoordiEquip_insert( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const GCITEMUID itemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiEquip_insert_20130415 ( %d, %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemCoordiEquip_insert_20130415 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
    @04: @04iSetID_input    int
        
    1 return ( @01 )
    @01: OK int
        0       : 코디 아이템 장착 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -4      : 코디 아이템이 아님
        -5      : 이미 장착 정보가 있음
        -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiEquip_insert_20130415( %d, %d, %I64d, %d ) }",
        dwLoginUID_, (int)cCharType_, itemUID_, nCoordiID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 룩 아이템 장착시 실패.."
            << L" UID : " << dwLoginUID_
            << L", Char Type : " << (int)cCharType_
            << L", Coordi ID : " << nCoordiID_
            << L", ItemUID : " << itemUID_
            << L", OK : " << nOK ) << END_LOG;
    }

    return (nOK == 0);
}

bool KSP::UIGAUserItemCoordiName_merge( IN const DWORD dwLoginUID_, IN const char cCharType_, IN const int nCoordiID_, IN const std::wstring& strCoordiName_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordiName_merge_20130415 ( %d, %d, %d, N'%s' ) }"

    { call dbo.UIGAUserItemCoordiName_merge_20130415 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iSetID_input    int
    @04: @04strName_input   nvarchar( 6 )

    1 return ( @01 )
    @01: OK int
        0       : 코디 설정 성공
        -1      : 유저가 없음
        -101이하: 트랜잭션 에러
    */
    std::wstring strCoordiName = boost::replace_all_copy( strCoordiName_, L"'", L"''" );
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordiName_merge_20130415( %d, %d, %d,  N'%s' ) }",
        dwLoginUID_, (int)cCharType_, nCoordiID_, strCoordiName.c_str() ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 룩 아이템 장착시 실패.."
            << L" UID : " << dwLoginUID_
            << L", Char Type : " << (int)cCharType_
            << L", Coordi ID : " << nCoordiID_
            << L", Coordi Name : " << strCoordiName_
            << L", Coordi Name : " << strCoordiName
            << L", OK : " << nOK ) << END_LOG;
    }

    return (nOK == 0);
}

bool KSP::GetUserHeroDungeonInfo( IN const DWORD dwUserUID_, IN const char cCharType_, IN OUT std::map< int, KHeroPlayData >& mapData_ )
{
    DBJIF2( m_kODBC.Query( L"{ call dbo.DCGADungeonClearHero_select_ticket ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nModeID = -1;
        KHeroPlayData kData;

        VERIFY_QUERY( m_kODBC
            >> nModeID
            >> kData.m_tmPlayDate
            >> kData.m_nPlayIndex
            >> kData.m_ClearCount
            >> kData.m_IsTicketUsed );

        mapData_[nModeID] = kData;

        //std::Info << L"[UserHeroDungeonInfo] " << dwUserUID_ << L" / " << nModeID << L" / " 
        //    << kData.m_tmPlayDate << L" / " << kData.m_nPlayIndex << L" / " << kData.m_ClearCount << dbg::endl;
    }

    START_LOG( clog, L"Get User HeroDungeon Clear Info. size : " << mapData_.size() ) << END_LOG;
    return true;
}

int KSP::UpdateUserHeroDungeonInfo( IN const DWORD dwUserUID_, IN const char cCharType_, IN const int nModeID_, IN const int nPlayIndex_, IN const int nClearCount_ )
{
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.DCGADungeonClearHero_merge_20130527 ( %d, %d, %d, N'%s', %d, %d ) }",
        dwUserUID_, cCharType_, nModeID_, (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nPlayIndex_, nClearCount_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"Set User HeroDungeon Clear Info.. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

bool KSP::InitAttribute( IN const DWORD dwUserUID_, IN const int nSlotID_, IN OUT KItem& kItem_ )
{
    int nAttributeState = KAttributeInfo::ATS_OPENED;
    int nAttType = -1;
    int nAttValue = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttribute_update ( %d, %I64d, %d, %d, %d, %d  ) }",
        dwUserUID_, kItem_.m_ItemUID, nSlotID_, nAttType, nAttValue, nAttributeState ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 속성 설정. UID : " << dwUserUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kItem_.m_ItemUID )
        << BUILD_LOG( nSlotID_ )
        << BUILD_LOG( nAttType )
        << BUILD_LOG( nAttValue )
        << BUILD_LOG( nAttributeState )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( nOK == 0 ) {
        KAttributeInfo kAttInfo;
        kAttInfo.m_cSlotID = nSlotID_;
        kAttInfo.m_cState = nAttributeState;
        kAttInfo.m_cType = nAttType;
        kAttInfo.m_fValue = static_cast<float>( nAttValue );
        kItem_.m_vecAttribute.push_back( kAttInfo );
    }

    return ( nOK == 0 );
}

bool KSP::GetChangeWeaponInfoFromDB( IN const DWORD& dwUserUID_, OUT std::map< char, std::pair<bool,GCITEMUID> >& mapChangeWeapon_ )
{
    /*
    mk8253. 20101228.
    무기체인지 오픈 정보.

    { call dbo.CWGAChangeWeapon_select ( @1 ) }
    @1 ; @iLoginUID_ [int]

    n return ( @1, @2, @3 )
    @1 ; CharType
    @2 ; Opened
    0 ; 잠김
    1 ; 열림
    @3 ; ItemUID
    0      ; 장착중인 무기 없음
    0 초과 ; 장착중인 무기의 ItemUID
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.CWGAChangeWeapon_select ( %d ) }",
        dwUserUID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nCharType = -1;
        bool bOpen = false;
        GCITEMUID ItemUID = 0;

        VERIFY_QUERY( m_kODBC
            >> nCharType
            >> bOpen
            >> ItemUID );

        mapChangeWeapon_[static_cast<char>(nCharType)] = std::make_pair( bOpen, ItemUID );

        START_LOG( clog, dwUserUID_ << L" : " << L", " << nCharType << L", " << (bOpen?L"true":L"false") << L", " << ItemUID  ) << END_LOG;
    }

    START_LOG( clog, L"Get ChangeWeapon Info From DB.. UserUID : " << dwUserUID_ << L"size : " << mapChangeWeapon_.size() ) << END_LOG;
    return true;
}

bool KSP::UpdateChangeWeaponInfoToDB( IN const DWORD& dwUserUID_, IN const int& nCharType_, IN const bool& bOpen_, IN const GCITEMUID& ItemUID_ )
{
    /*
    mk8253. 20101228.
    무기체인지. (장착 정보에도 써주세요.)

    { call dbo.CWGAChangeWeapon_Update ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iCharType_ [int]
    @3 ; @iOpened_   [int]
    @4 ; @iItemUID_  [bigint]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 캐릭터 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.CWGAChangeWeapon_Update ( %d, %d, %d, %I64d ) }",
        dwUserUID_, nCharType_, bOpen_, ItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"Update ChangeWeapon Info To DB. Ret : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( nOK == 0 );
}

bool KSP::QueryPCBangUser( IN const std::wstring& strIP_, IN KODBC& kODBC_ )
{
    KLocker lock( kODBC_.GetCS() );

    _DBJIF( kODBC_.Query( L"{ call dbo.sp_executesql "
        L"( N'execute dbo.ADBILLGAME_PLUS_IP_CHECK @1, @2 output, @3 output; select @2', "
        L"N'@1 nvarchar(15), @2 int, @3 int', N'%s', %d, %d ) }",
        strIP_.c_str(), 0, 0 ), return false, (&kODBC_) );

    int nRet = 0;
    if( kODBC_.BeginFetch() ) {
        VERIFY_QUERY( kODBC_ >> nRet );
    }
    kODBC_.EndFetch();

    return (nRet != 0);
}


void KSP::GetConnectionMsgSelect( IN const int msgID_, OUT std::wstring& msg_ )
{
    //call dbo.CMG_connecting_message_select
    //( MessageID int )
    //return 1 loop select
    //( Message nvarchar(4000) )
    DBJIF( m_kODBC.Query( L"{ call dbo.CMG_connecting_message_select ( %d ) }",
        msgID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> msg_ );
        m_kODBC.EndFetch();
    }
}

void KSP::GetConnectionMsgUserSelect( IN const DWORD dwUID_, OUT std::vector<int>& vecMsgID_ )
{
    //{ call dbo.CMG_connecting_message_user_select ( @1 ) }
    //@1 ; LoginUID int
    //return n loop select
    //( MessageID int )
    DBJIF( m_kODBC.Query( L"{ call dbo.CMG_connecting_message_user_select ( %d ) }", dwUID_ ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nMsgID = 0;
        VERIFY_QUERY( m_kODBC >> nMsgID );
        vecMsgID_.push_back( nMsgID );
    }
}

int KSP::UpdateUserNickname( IN const DWORD& dwUID_, IN const std::wstring& strOldNick_, IN const std::wstring& strNewNick_, IN const GCITEMUID& ItemUID_ )
{
    int nRet = -99;

    DBJIF3( m_kODBC.Query ( L"{ call dbo.UNGA_user_nickname_update ( %d, N'%s', N'%s', %I64d ) }",
        dwUID_,
        strOldNick_.c_str(),
        strNewNick_.c_str(),
        ItemUID_ ), return nRet );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_CONDITION( nRet == 0, clog, cwarn, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nRet;
}

int KSP::UpdateUserNickcolor(IN const DWORD& dwUID_, IN const std::wstring& strNewColor_, IN const GCITEMUID& ItemUID_)
{
    int nRet = -99;

    DBJIF3(m_kODBC.Query(L"{ call dbo.UNGA_user_nickcolor_update ( %d, N'%s', %I64d ) }",
        dwUID_,
        strNewColor_.c_str(),
        ItemUID_), return nRet);

    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_CONDITION(nRet == 0, clog, cwarn, L"Ret : " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

    return nRet;
}

int KSP::UpdateUserNicknameNoItem( IN const DWORD& dwUID_, IN const std::wstring& strOldNick_, IN const std::wstring& strNewNick_ )
{
    int nRet = -99;

    DBJIF3( m_kODBC.Query ( L"{ call dbo.CNGAChangeNickname_update ( %d, N'%s', N'%s' ) }",
        dwUID_,
        strOldNick_.c_str(),
        strNewNick_.c_str() ), return nRet );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_CONDITION( nRet == 0, clog, cwarn, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( nRet == -1 || nRet == -2 ) {
        return -1;
    }

    if ( nRet == -3 ) {
        return -2;
    }

    return nRet;
}

int KSP::UpdateUserNickcolorNoItem(IN const DWORD& dwUID_, IN const std::wstring& strNewColor_)
{
    int nRet = -99;

    DBJIF3(m_kODBC.Query(L"{ call dbo.CNGAChangeNickcolor_update ( %d, N'%s' ) }",
        dwUID_,
        strNewColor_.c_str()), return nRet);

    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_CONDITION(nRet == 0, clog, cwarn, L"Ret : " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

    return nRet;
}

bool KSP::LoginToUserUID( IN const std::wstring& strLogin_, OUT DWORD& dwLoginUID_ )
{
    dwLoginUID_ = 0;
    __JIF( !strLogin_.empty(), return false );
    __JIF( !SiKSlangFilter()->IsInQueryInvalidChar( strLogin_ ), return false );

    DBJIF2( m_kODBC.Query( L"{ call dbo.up_translate_login_to_loginuid ( N'%s' ) }",
        strLogin_.c_str() ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> dwLoginUID_ );
    }
    m_kODBC.EndFetch();

    _LOG_CONDITION( dwLoginUID_ > 0, clog, cwarn, L"Login to UserUID.. ret : " << (dwLoginUID_ > 0 ? L"Success" : L"Fail" ) )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( dwLoginUID_ ) << END_LOG;

    return (dwLoginUID_ > 0);
}

bool KSP::NickToUserUID( IN const std::wstring& strNick_, OUT DWORD& dwLoginUID_ )
{
    // 대표 닉네임으로 동작한다

    dwLoginUID_ = 0;
    __JIF( !strNick_.empty(), return false );
    __JIF( !SiKSlangFilter()->IsInQueryInvalidChar( strNick_ ), return false );
    __JIF( !SiKSlangFilter()->CheckUnicode( strNick_ ), return false );

    DBJIF2( m_kODBC.Query( L"{ call dbo.up_translate_nick_to_loginuid ( N'%s' )  }",
        strNick_.c_str() ) );

    int nLoginUID = -1;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nLoginUID );
    }
    m_kODBC.EndFetch();

    if ( nLoginUID == -1 ) {
        START_LOG( cwarn, L"대상이 존재하지 않음 name : " << strNick_ ) << END_LOG;
        return false;
    }
    dwLoginUID_ = static_cast<DWORD>(nLoginUID);

    _LOG_CONDITION( dwLoginUID_ > 0, clog, cwarn, L"Nick to UserUID.. ret : " << (dwLoginUID_ > 0 ? L"Success" : L"Fail" ) )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( dwLoginUID_ ) << END_LOG;

    return (dwLoginUID_ > 0);
}

bool KSP::CharNickToUserUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ )
{
    // 캐릭터 닉네임으로 동작한다

    /*
    L"{ call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( N'%s' ) }"

    { call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( @01 ) }
        @01strNickname_input nvarchar( 24 )

    1 return ( @01, @02 )
        @01iCharType int
        @02iLoginUID int
    */
    dwLoginUID_ = 0;
	/*___JIF( !strCharNick_.empty(), return false );
	___JIF( !SiKSlangFilter()->IsInQueryInvalidChar( strCharNick_ ), return false );
	___JIF( !SiKSlangFilter()->CheckUnicode( strCharNick_ ), return false );*/
	if (strCharNick_.empty() || SiKSlangFilter()->IsInQueryInvalidChar(strCharNick_) || SiKSlangFilter()->CheckUnicode(strCharNick_))
		return false;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( N'%s' )  }",
        strCharNick_.c_str() ) );

    int nLoginUID = -1;
    nCharType_ = -1;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nLoginUID >> nCharType_ );
    }
    m_kODBC.EndFetch();

    if ( -1 == nLoginUID ) {
        START_LOG( clog, L"대상 캐릭터가 존재하지 않음 name : " << strCharNick_ ) << END_LOG;

        return false;
    }

    dwLoginUID_ = static_cast< DWORD >( nLoginUID );

    START_LOG( clog, L"캐릭터 닉네임 조회 성공" )
        << BUILD_LOG( strCharNick_ )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::GetFashionCatalog( OUT std::map< int, std::pair<int,GCITEMID> >& mapItemList_ )
{
    mapItemList_.clear();

    /*
    mk8253. 20110228.
    패션카탈로그 리스트.

    { call dbo.FCGAFashionCatalog_select }

    n return ( @1, @2, @3 )
    @1 ; No
    @2 ; CharType
    @3 ; ItemID
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.FCGAFashionCatalog_select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nIndex = -1;
        int nCharType = -1;
        GCITEMID ItemID = 0;

        VERIFY_QUERY( m_kODBC >> nIndex
            >> nCharType
            >> ItemID );

        mapItemList_[nIndex] = std::make_pair( nCharType, ItemID );
    }

    START_LOG( clog, L"Fashion Catalog.. size : " << mapItemList_.size() ) << END_LOG;
    return true;
}

bool KSP::FriendFriendUserType_select( IN const DWORD& dwUserUID_, IN OUT int& nUserType_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFEVFriendFriendUserType_select_20110920 ( %d, %d, %d ) }"
    반갑다 친구야 유저 타입 정의.

    { call dbo.FFEVFriendFriendUserType_select_20110920 ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iUserType_ [tinyint]
    @3 ; @iVersion_  [tinyint] ; 버전

    1 return ( @1 )
    @1 ; UserType
    1 ; 신규유저
    2 ; 복귀유저
    4 ; 활성유저
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEVFriendFriendUserType_select_20110920 ( %d, %d, %d ) }",
        dwUserUID_, nUserType_, nVersion_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nUserType_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"Friend Type : " << nUserType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nUserType_ > 0);
}

bool KSP::SkillCount_merge( IN const std::map< std::pair<int,int>, std::map<int,int> >& mapStat_ )
{
    /*
    mk8253. 20110314. 스킬 사용에 대한 통계정보.

    { call dbo.SCSTSkillCount_merge ( @1, @2, @3, @4 ) }
    @1 ; @iModeID_       [int]
    @2 ; @iSkillID_       [int]
    @3 ; @iCharLevel_     [int]
    @4 ; @iUserCountDiff_ [int]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -101이하 ; 트랜젝션 에러
    */
    std::map< std::pair<int,int>, std::map<int,int> >::const_iterator cmit;
    for ( cmit = mapStat_.begin() ; cmit != mapStat_.end() ; ++cmit ) {
        const int& nSkillID = cmit->first.first;
        const int& nCharLv = cmit->first.second;

        std::map<int,int>::const_iterator cmitStat;
        for ( cmitStat = cmit->second.begin() ; cmitStat != cmit->second.end() ; ++cmitStat ) {
            const int& nModeType = cmitStat->first;
            const int& nUseCount = cmitStat->second;

            if ( nUseCount <= 0 ) {
                continue;
            }

            DBJIF2( m_kODBC.Query( L"{ call dbo.SCSTSkillCount_merge ( %d, %d, %d, %d ) }",
                nModeType,
                nSkillID,
                nCharLv,
                nUseCount ) );

            int nRet = 0;
            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nRet );
            }
            m_kODBC.EndFetch();

            _LOG_CONDITION( nRet == 0, clog, cwarn, L"Ret : " << nRet << L", LastQuery : " << m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    START_LOG( clog, L"DB 캐릭터 통계기록.. size : " << mapStat_.size() ) << END_LOG;
    return true;
}

bool KSP::SkillTreeID_insert( IN const DWORD& dwUserUID_, IN const int& nCharType_, IN const int& nPromotion_, IN const int& nSkillID_ )
{
    /*
    { call dbo.STGASkillTreeID_insert_20110401 ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_  int
    @2: @iCharType_  tinyint
    @3: @iPromotion_ tinyint
    @4: @iSkillID_   int

    1 select ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보 없음
    -2      : 스킬 정보 이미 있음
    -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.STGASkillTreeID_insert_20110401 ( %d, %d, %d, %d ) }",
        dwUserUID_, nCharType_, nPromotion_, nSkillID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( 0 == nRet || -2 == nRet, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (0 == nRet || -2 == nRet);
}

bool KSP::SkillTreeID_delete( IN const DWORD& dwUserUID_, IN const int& nCharType_, IN const int& nPromotion_, IN const int& nSkillID_ )
{
    /*
    { call dbo.STGASkillTreeID_delete_20110401 ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_  int
    @2: @iCharType_  tinyint
    @3: @iPromotion_ tinyint
    @4: @iSkillID_   int

    1 select ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보 없음
    -2      : 스킬 정보 없음
    -101이하: 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.STGASkillTreeID_delete_20110401 ( %d, %d, %d, %d ) }",
        dwUserUID_, nCharType_, nPromotion_, nSkillID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( 0 == nRet || -2 == nRet, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (0 == nRet || -2 == nRet);
}

bool KSP::PetInfoEquip_insert( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const GCITEMUID& dwPetUID_, IN const GCITEMUID& ItemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemPetEquip_insert_20130510 ( %d, %d, %I64d, %I64d ) }"

    { call dbo.UIGAUserItemPetEquip_insert_20130510 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input        int
    @02: @02iCharType_input        int
    @03: @03iItemUID_input         bigint
    @04: @04iEquippedItemUID_input bigint

    1 select ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 인벤 아이템 정보 없음
    -3       : 해당 캐릭터가 보유하고 있는 아이템이 아님
    -4       : 이미 장착정보가 있음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemPetEquip_insert_20130510 ( %d, %d, %I64d, %I64d ) }",
        dwUserUID_, cCharType_, dwPetUID_, ItemUID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    LIF( IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nRet == 0);
}

bool KSP::PetInfoEquip_delete( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const GCITEMUID& dwPetUID_, IN const GCITEMUID& ItemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemPetEquip_delete_20130510 ( %d, %d, %I64d, %I64d ) }"

    { call dbo.UIGAUserItemPetEquip_delete_20130510 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input        int
    @02: @02iCharType_input        int
    @03: @03iItemUID_input         bigint
    @04: @04iEquippedItemUID_input bigint

    1 select ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 인벤 아이템 정보 없음
    -3       : 해당 캐릭터가 보유하고 있는 아이템이 아님
    -4       : 펫 장착정보가 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemPetEquip_delete_20130510 ( %d, %d, %I64d, %I64d ) }",
        dwUserUID_, cCharType_, dwPetUID_, ItemUID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    LIF( IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nRet == 0);
}

bool KSP::ChoiceDrop_insert( IN const DWORD& dwUserUID_, IN const GCITEMID& BoxItemID_, IN const GCITEMID& SelectItemID_ )
{
    /*
    mk8253. 20110426.
    초이스 드랍 선택 로그 겸 통계.

    { call dbo.CDGAChoiceDrop_insert ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iItemID_   [int]
    @3 ; @iSelected_ [int]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.CDGAChoiceDrop_insert ( %d, %d, %d ) }",
        dwUserUID_, BoxItemID_, SelectItemID_ ) );

    int nRet = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"초이스 드랍 선택 로그 겸 통계.. Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KSP::QueryUserBenfitCheck( IN const std::wstring& strIP_, IN KODBC& kODBC_ )
{
    KLocker lock( kODBC_.GetCS() );

    DBJIF2( kODBC_.Query( L"{ call dbo.ipb_sp_cafeinfo_check_ip_super_bonus ( N'%s' ) }",
        strIP_.c_str() ) );

    int nRet = -1;
    if( kODBC_.BeginFetch() ) {
        VERIFY_QUERY( kODBC_ >> nRet );
    }
    kODBC_.EndFetch();

    return (nRet == 0);
}

bool KSP::UpdateLevel( IN const DWORD dwUserUID_, IN const int nLevel_, IN const int nType_, IN OUT KItem& kItem_, IN const char cCharType_ )
{
    /*
    아이템 레벨 변경
    L"{ call dbo.UIGAUserItemLevel_merge_20130415 ( %d, %d, %I64d, %d, %d ) }"

    { call dbo.UIGAUserItemLevel_merge_20130415 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input  int
    @02: @02iCharType_input  int
    @03: @03iItemUID_input   bigint
    @04: @04iType_input      tinyint
    @05: @05iItemLevel_input tinyint
        
    1 return ( @01 )
    @01: OK int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemLevel_merge_20130415 ( %d, %d, %I64d, %d, %d ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nType_, nLevel_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK == 0 ) {
        switch ( nType_ ) {
            case KItemLevelTable::IL_EQUIP_LV:
                kItem_.m_sEquipLevel = static_cast<short>( nLevel_ );
                break;
            case KItemLevelTable::IL_EQUIP_DOWN_LV:
                kItem_.m_sEquipLvDown = static_cast<short>( nLevel_ );
                break;
            default:
                START_LOG( cerr, L"정의되지 않은 아이템레벨 타입.. ItemType : " << nType_ ) << END_LOG;
                break;
        }
        START_LOG( clog, L"ItemType : " << nType_ ) << END_LOG;
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 레벨설정 DB처리 결과.. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( nOK == 0 );
}

bool KSP::VIPCustomerInfo_select( OUT std::map<USHORT,KVipEvent>& mapInfo_, IN std::map< USHORT, std::map< USHORT, USHORT > >& mapRewardByVer_ )
{
    /*
    L"{ call dbo.VCGAVIPCustomerInfo_Version_select ( %d ) }"

    { call dbo.VCGAVIPCustomerInfo_Version_select ( @1 ) }
    @1 ; @iVersion_ int

    n return ( @1, @2, @3, @4, @5 )
    @1 ; iVersion
    @2 ; sdtStartDate
    @3 ; sdtEndDate
    @4 ; bNewbie
    @5 ; bVIP
    */
    mapInfo_.clear();

    std::map< USHORT, std::map< USHORT, USHORT > >::iterator mitRewardByVer = mapRewardByVer_.begin();
    for ( ; mitRewardByVer != mapRewardByVer_.end(); ++mitRewardByVer ) {
        int nVersion = mitRewardByVer->first;
        DBJIF2( m_kODBC.Query( L"{ call dbo.VCGAVIPCustomerInfo_Version_select ( %d ) }", nVersion ) );

        KVipEvent kVipEvent;
        bool bNewbieEnable = false;
        bool bVipEnable = false;

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> kVipEvent.m_usVersion
                >> kVipEvent.m_prEventRange.first
                >> kVipEvent.m_prEventRange.second
                >> bNewbieEnable
                >> bVipEnable );

            kVipEvent.m_mapEventEnable[0] = bNewbieEnable;
            kVipEvent.m_mapEventEnable[1] = bVipEnable;
            mapInfo_[kVipEvent.m_usVersion] = kVipEvent;

            m_kODBC.EndFetch();
        }
    }

    START_LOG( clog, L"VIP Event.. size : " << mapInfo_.size() ) << END_LOG;
    return true;
}

bool KSP::VIPCustomerPoint_select( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, OUT int& nPoint_ )
{
    /*
    mk8253. 20110407.
    유저의 버전별 Point 리턴.
    { call dbo.VCGAVIPCustomerPoint_select ( @1, @2 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_  [int]

    1 return ( @1 )
    @1 ; Point
    */
    nPoint_ = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.VCGAVIPCustomerPoint_select ( %d, %d) }",
    dwUserUID_, usVer_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nPoint_ );
    }
    m_kODBC.EndFetch();

    _LOG_CONDITION( nPoint_ >= 0, clog, cwarn, L"Point : " << nPoint_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nPoint_ >= 0);
}

bool KSP::VIPCustomerReward_select( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, OUT std::map<int,bool>& mapInfo_ )
{
    /*
    mk8253. 20110407.
    보상 받아간 것들 리스트.
    { call dbo.VCGAVIPCustomerReward_select ( @1, @2 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_  [int]

    n return ( @1, @2 )
    @1 ; PayTableID
    @2 ; IsSupplied
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.VCGAVIPCustomerReward_select ( %d, %d ) }",
        dwUserUID_, usVer_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nPayTableID = -1;
        bool bIsSupplied  = false;

        VERIFY_QUERY( m_kODBC >> nPayTableID
            >> bIsSupplied );

        if ( nPayTableID > -1 ) {
            mapInfo_[nPayTableID] = bIsSupplied;
        }
    }

    START_LOG( clog, L"VIP Reward info.. size : " << mapInfo_.size() ) << END_LOG;
    return true;
}

bool KSP::VIPCustomerReward_update( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, IN const int& nPayTableID_ )
{
    /*
    mk8253. 20110407.
    보상 받아간 것 갱신.
    { call dbo.VCGAVIPCustomerReward_update ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_  [int] 
    @3 ; @iPayTableID_ [int]

    1 return ( @1 )
    @1 ; OK
    0   ; 정상
    -1   ; 유저 정보가 없음
    -2   ; 이미 받아갔음
    -101 ; 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.VCGAVIPCustomerReward_update ( %d, %d, %d ) }",
        dwUserUID_, usVer_, nPayTableID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nRet == 0, "Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nRet == 0);
}

bool KSP::MagicBox_select( IN const USHORT& usVer_, OUT std::map<GCITEMID, std::pair<KDropItemInfo,int> >& mapItemList_, OUT std::map<GCITEMID,float>& mapLuckRatio_ )
{
    /*
    버전별 등록가능한 아이템 정보 및 럭키확률.

    L"{ call dbo.MBGAMagicBox_select ( %d ) }"

    { call dbo.MBGAMagicBox_select ( @1 ) }
    @1: @iVersion_ int

    n return ( @1, @2, @3, @4, @5 )
    @1: ItemID
    @2: Period
    @3: Duration
    @4: LuckyRatio
    @5: Price int
    */
    mapItemList_.clear();
    mapLuckRatio_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.MBGAMagicBox_select ( %d ) }",
        usVer_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KDropItemInfo kItem;
        int nPrice = 0;
        float fRatio = 0.f;

        VERIFY_QUERY( m_kODBC >> kItem.m_ItemID
            >> kItem.m_nPeriod
            >> kItem.m_nDuration
            >> fRatio
            >> nPrice );

        if ( kItem.m_ItemID == 0 ) continue;
        std::pair<KDropItemInfo,int> prItemInfo = std::make_pair( kItem, nPrice );

        LIF( mapItemList_.insert( std::make_pair(kItem.m_ItemID, prItemInfo ) ).second );
        LIF( mapLuckRatio_.insert( std::make_pair(kItem.m_ItemID, fRatio) ).second );
    }

    START_LOG( clog, L"마법상자.. 유저가 획득가능한 아이템리스트 size : " << mapItemList_.size() << L", " << mapLuckRatio_.size() ) << END_LOG;
    return true;
}

bool KSP::MagicBoxRegister_update( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, IN const GCITEMID& ItemID_ )
{
    /*
    mk8253. 20110407.
    메인등록 아이템 변경.

    { call dbo.MBGAMagicBoxRegister_update ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_  [int]
    @3 ; @iItemID_   [int]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.MBGAMagicBoxRegister_update ( %d, %d, %d ) }",
        dwUserUID_, usVer_, ItemID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nRet == 0, "Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

bool KSP::MagicBoxRegister_select( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, OUT GCITEMID& ItemID_ )
{
    /*
    mk8253. 20110407.
    메인으로 등록된 아이템 보기.

    { call dbo.MBGAMagicBoxRegister_select ( @1, @2 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_ [int]

    1 return ( @1 )
    @1 ; ItemID
    */
    ItemID_ = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.MBGAMagicBoxRegister_select ( %d, %d ) }",
        dwUserUID_, usVer_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> ItemID_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"마법상자.. 메인으로 등록된 아이템 ItemID : " << ItemID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

bool KSP::MagicBoxStoneInfo_update( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, IN const GCITEMID& ItemID_, IN const int& nUseKeyDiff_, IN const bool& bClear_ )
{
    /*
    mk8253. 20110412.
    유저가 획득 가능한 아이템 정보 갱신.

    { call dbo.MBGAMagicBoxStoneInfo_update ( @1, @2, @3, @4, @5 ) }
    @1 ; @iLoginUID_       [int]
    @2 ; @iVersion_        [int]
    @3 ; @iItemID_         [int]
    @4 ; @iMagicStoneDiff_ [int] ( 마법석 사용개수 차이값 )
    @5 ; @bCleart_         [bit] ( 아이템 획득시 1, 아니면 0 )

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.MBGAMagicBoxStoneInfo_update ( %d, %d, %d, %d, %d ) }",
        dwUserUID_, usVer_, ItemID_, nUseKeyDiff_, bClear_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nRet == 0, "Ret : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

bool KSP::MagicBoxStoneInfo_select( IN const DWORD& dwUserUID_, IN const USHORT& usVer_, OUT std::map<GCITEMID,std::pair<int,int> >& mapList_ )
{
    /*
    mk8253. 20110408.
    유저가 획득 가능한 아이템 정보 보기.

    { call dbo.MBGAMagicBoxStoneInfo_select ( @1, @2 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iVersion_  [int]

    n return ( @1, @2, @3 )
    @1 ; ItemID
    @2 ; MagicStone
    @3 ; ClearCount 
    */
    mapList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.MBGAMagicBoxStoneInfo_select ( %d, %d) }",
        dwUserUID_, usVer_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMID ItemID = 0;
        std::pair<int,int> prInfo;
        int nUseKeyCount = 0;
        int nClearCount = 0;

        VERIFY_QUERY( m_kODBC >> ItemID
            >> nUseKeyCount
            >> nClearCount );

        if ( ItemID == 0 ) continue;
        prInfo.first = std::max<int>( nUseKeyCount, 0 );
        prInfo.second = std::max<int>( nClearCount, 0 );

        LIF( mapList_.insert( std::map<GCITEMID,std::pair<int,int> >::value_type( ItemID, prInfo ) ).second );
    }

    START_LOG( clog, L"마법상자.. 유저가 획득가능한 아이템리스트 size : " << mapList_.size() ) << END_LOG;
    return true;
}

bool KSP::PackageItem_select( IN const DWORD dwMaxPackageNum_, OUT std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList_ )
{
    /*
    L"{ call dbo.PIGAPackageItem_select_20120907 }"

    { call dbo.PIGAPackageItem_select_20120907 }

    n return ( @1, @2, @3, @4, @5, @6, @7 )
    @1: ItemID        int
    @2: SubItemID     int
    @3: SubGradeID    smallint
    @4: SubPeriod     int
    @5: SubDuration   smallint
    @6: StrengthLevel int
    @7: EquipLevel    int
    */
    std::map<GCITEMID,std::vector<KDropItemInfo > > mapList;
    mapList.clear();
    vecList_.clear();
    GCITEMID beforePackageItemID = 0; // 이전 패키지 ItemID 저장변수.
    DWORD dwCount = 0;                // 패키지 갯수 Count 변수.

    DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPackageItem_select_20120907 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMID packageItemID = 0;
        KDropItemInfo kPartItem;

        VERIFY_QUERY( m_kODBC >> packageItemID
            >> kPartItem.m_ItemID
            >> kPartItem.m_cGradeID
            >> kPartItem.m_nPeriod
            >> kPartItem.m_nDuration
            >> kPartItem.m_cEnchantLevel
            >> kPartItem.m_cEquipLevel );

        if ( packageItemID == 0 ) continue;

        if( beforePackageItemID != packageItemID ) {
            ++dwCount;
            beforePackageItemID = packageItemID;
        }

        if( dwCount > dwMaxPackageNum_ ) {
            vecList_.push_back( mapList );
            mapList.clear();
            dwCount = 0;
        }

        mapList[packageItemID].push_back( kPartItem );
    }

    if( !mapList.empty() ) {
        vecList_.push_back( mapList );
    }

    START_LOG( clog, L"패키지 아이템 리스트.. size : " << vecList_.size() ) << END_LOG;
    return true;
}

bool KSP::UpdateUserModeCount( IN int nModeCode_, IN int nModeCount_, IN KODBC& kODBC_ )
{
    /*
    L"{ call dbo.GMGAGameModeCount_merge_20110525 ( %d, %d ) }"

    { call dbo.GMGAGameModeCount_merge_20110525 ( @1, @2 ) }
    @1: @iModeCode_  int
    @2: @iModeCount_ int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜젝션 에러
    */
    KLocker lock( kODBC_.GetCS() );
    
    int nOK = -1;
    DBJIF2( kODBC_.Query( L"{ call dbo.GMGAGameModeCount_merge_20110525 ( %d, %d ) }",
        nModeCode_, nModeCount_ ) );

    if ( kODBC_.BeginFetch() ) {
        VERIFY_QUERY( kODBC_ >> nOK );
    }
    kODBC_.EndFetch();

    return ( nOK == 0 );

}

bool KSP::MergeAttribute( IN const int nType_, IN const DWORD dwUserUID_, IN const int nSlotID_, IN const int nAttType_, IN const float fAttValue_, IN OUT KItem& kItem_, IN const char cCharType_ )
{
    /*
    L"{ call dbo.UIGAUserItemAttribute_merge_20130415 ( %d, %d, %I64d, %d, %d, %d, %d  ) }"

    { call dbo.UIGAUserItemAttribute_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
    @01: @01iLoginUID_input       int
    @02: @02iCharType_input       int
    @03: @03iItemUID_input        bigint
    @04: @04iSlotID_input         tinyint
    @05: @05iTypeID_input         smallint
    @06: @06iValue_input          int
    @07: @07iAttributeState_input tinyint
        
    1 return ( @1 )
    @1: OK
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    int nAttributeState = KAttributeInfo::ATS_OPENED;
    if( nAttType_ != KGCAttributeDecider::Table::AT_INVAILD )
        nAttributeState = KAttributeInfo::ATS_USING;

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttribute_merge_20130415 ( %d, %d, %I64d, %d, %d, %d, %d  ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nSlotID_, nAttType_, (int)(fAttValue_*10000.f), nAttributeState ) );


    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 속성 설정. UID : " << dwUserUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kItem_.m_ItemUID )
        << BUILD_LOG( nSlotID_ )
        << BUILD_LOG( nAttType_ )
        << BUILD_LOG( fAttValue_ )
        << BUILD_LOG( nAttributeState )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( nType_ == KStrengthManager::SU_UPDATE && nOK == 0 ) {
        KAttributeInfo kAttInfo;
        kAttInfo.m_cSlotID = nSlotID_;
        kAttInfo.m_cState = nAttributeState;
        kAttInfo.m_cType = nAttType_;
        kAttInfo.m_fValue = fAttValue_;
        kItem_.m_vecAttribute.push_back( kAttInfo );
    }

    return ( nOK == 0 );
}

int KSP::UpdateStrengthItemInfo( IN const DWORD dwUID_, IN const GCITEMUID dwStrengthItemUID_, IN const int nLevel_, IN const GCITEMUID dwEquipItemUID_, IN const char cGrade_, IN const char cCharType_ )
{
    /*
    L"{ call dbo.UIGAUserItemStrength_merge_20130415 ( %d, %d, %I64d, %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemStrength_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
    @01: @01iLoginUID_input        int
    @02: @02iCharType_input        int
    @03: @03iItemUID_input         bigint
    @04: @04iStrengthLevel_input   int
    @05: @05iEquippedItemUID_input bigint
    @06: @06iGradeID_input         int
        
    1 return ( @1 )
    @1: OK int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜잭션에러
    */
    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.UIGAUserItemStrength_merge_20130415 ( %d, %d, %I64d, %d, %I64d, %d ) }",
        dwUID_, cCharType_, dwStrengthItemUID_, nLevel_, dwEquipItemUID_, (int)cGrade_ ), return nOK );
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

// 강화석 마이그레이션
bool KSP::UserItemStrength_migration011( IN const DWORD& dwUID_, IN const int& nNO_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    1.다수 레코드 리턴
    L"{ call dbo.UIMIUserItemStrength_migration011_20130415 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration011_20130415 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iNo_input       int
        
    n return ( @01, @02 )
    @01: ItemUID      bigint
    @02: EnchantLevel int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration011_20130415 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nNO_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"1.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration012( IN const DWORD& dwUID_, IN const char cCharType_, IN const int& nEnchantLv_ )
{
    /*
    L"{ call dbo.UIMIUserItemStrength_migration012_20130402 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration012_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
        
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration012_20130402 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration099( IN const DWORD& dwUID_, IN const GCITEMUID& ItemUID_, IN const int& nEnchantLv_, IN const char cCharType_ )
{
    /*
    1-2. 1에서 나온 레코드 수만큼 루프
    2-2. 2에서 나온 레코드 수만큼 루프
    3-2. 3에서 나온 레코드 수만큼 루프
    4-2. 4에서 나온 레코드 수만큼 루프
    5-2. 5에서 나온 레코드 수만큼 루프
    6-2. 6에서 나온 레코드 수만큼 루프
    7-2. 7에서 나온 레코드 수만큼 루프
    { call dbo.UIMIUserItemStrength_migration099_20130415 ( LoginUID int, CharType int, ItemUID bigint, EnchantLevel int ) }
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration099_20130415 ( %d, %d, %I64d, %d ) }",
        dwUID_, cCharType_, ItemUID_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration021( IN const DWORD& dwUID_, IN const int& nNO_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    2.다수 레코드 리턴
    L"{ call dbo.UIMIUserItemStrength_migration021_20130415 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration021_20130415 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iNo_input       int
        
    n return ( @01, @02 )
    @01: ItemUID      bigint
    @02: EnchantLevel int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration021_20130415 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nNO_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"2.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration022( IN const DWORD& dwUID_, IN const char cCharType_, IN const int& nEnchantLv_ )
{
    /*
    2-1. 2에서 나온 레코드 수만큼 루프
    L"{ call dbo.UIMIUserItemStrength_migration022_20130402 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration022_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
    
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration022_20130402 ( %d, %d, %d ) }",
        dwUID_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration031( IN const DWORD& dwUID_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    3.다수 레코드 리턴
    L"{ call dbo.UIMIUserItemStrength_migration031_20130415 ( %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration031_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: ItemUID  bigint
    @02: Duration int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration031_20130415 ( %d, %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"3.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration032( IN const DWORD& dwUID_, IN const char cCharType_,  IN const int& nEnchantLv_ )
{
    /*
    3-1. 3에서 나온 레코드 수만큼 루프
    L"{ call dbo.UIMIUserItemStrength_migration032_20130402 ( %d, %d, %d ) }"
    { call dbo.UIMIUserItemStrength_migration032_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
    
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration032_20130402 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration041( IN const DWORD& dwUID_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    4.다수 레코드 리턴
    L"{ call dbo.UIMIUserItemStrength_migration041_20130415 ( %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration041_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: ItemUID  bigint
    @02: Duration int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration041_20130415 ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"4.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration042( IN const DWORD& dwUID_, IN const char cCharType_, IN const int& nEnchantLv_ )
{
    /*
    4-1. 4에서 나온 레코드 수만큼 루프
    { call dbo.UIMIUserItemStrength_migration042_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
 
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration042_20130402 ( %d, %d, %d ) }",
        dwUID_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration051( IN const DWORD& dwUID_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    5.다수 레코드 리턴
    { call dbo.UIMIUserItemStrength_migration051_20110818 ( LoginUID int ) }

    n return ( @1, @2 )
    @1 ; ItemUID        bigint
    @2 ; EnchantLevel   int
    */
    /*
    L"{ call dbo.UIMIUserItemStrength_migration051_20130415 ( %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration051_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: ItemUID  bigint
    @02: Duration int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration051_20130415 ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"5.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration052( IN const DWORD& dwUID_, IN const char cCharType_ , IN const int& nEnchantLv_ )
{
    /*
    5-1. 5에서 나온 레코드 수만큼 루프
    L"{ call dbo.UIMIUserItemStrength_migration052_20130402 ( %d, %d, %d ) }"
    { call dbo.UIMIUserItemStrength_migration052_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
        
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration052_20130402 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration061( IN const DWORD& dwUID_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    6.다수 레코드 리턴
    { call dbo.UIMIUserItemStrength_migration061_20110818 ( LoginUID int ) }

    n return ( @1, @2 )
    @1 ; ItemUID        bigint
    @2 ; EnchantLevel   int
    */
    /*
    L"{ call dbo.UIMIUserItemStrength_migration061_20130415 ( %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration061_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: ItemUID  bigint
    @02: Duration int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration061_20130415 ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"6.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

bool KSP::UserItemStrength_migration062( IN const DWORD& dwUID_, IN const char cCharType_, IN const int& nEnchantLv_ )
{
    /*
    6-1. 6에서 나온 레코드 수만큼 루프
    L"{ call dbo.UIMIUserItemStrength_migration062_20130402 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration062_20130402 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iValue_input    int
    no return
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration062_20130402 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nEnchantLv_ ) );
    return true;
}

bool KSP::UserItemStrength_migration071( IN const DWORD& dwUID_, IN const int& nNO_, OUT std::vector< std::pair<GCITEMUID,int> >& vecItemEnchantLv_, IN const char cCharType_ )
{
    /*
    7.다수 레코드 리턴
    L"{ call dbo.UIMIUserItemStrength_migration071_20130415 ( %d, %d, %d ) }"

    { call dbo.UIMIUserItemStrength_migration071_20130415 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iNo_input       int
        
    n return ( @01, @02 )
    @01: ItemUID      bigint
    @02: Value        int
    */
    vecItemEnchantLv_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIMIUserItemStrength_migration071_20130415 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nNO_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID = 0;
        int nEnchantLv = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nEnchantLv );

        vecItemEnchantLv_.push_back( std::make_pair( ItemUID, nEnchantLv ) );
    }

    START_LOG( clog, L"7.다수 레코드 리턴.. size : " << vecItemEnchantLv_.size() ) << END_LOG;
    return true;
}

//void KSP::GetSALT( IN const std::wstring& strLogin_, OUT std::wstring& strSALT_ )
//{
//    /*
//    L"{ call dbo.USGAUserSalt_select ( %s ) }"
//
//    { call dbo.USGAUserSalt_select ( @1 ) }
//    @1 ; @iLogin_ 
//
//    1 return ( @1 )
//    @1  ; Salt [nvarchar](40)
//    */
//
//    DBJIF( m_kODBC.Query( L"{ call dbo.USGAUserSalt_select ( N'%s' ) }", strLogin_.c_str() ) );
//
//    if ( m_kODBC.BeginFetch() ) {
//        VERIFY_QUERY( m_kODBC >> strSALT_ );
//        m_kODBC.EndFetch();
//    }
//}

bool KSP::AgiteObjPos_merge( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nPosX_, IN const int& nPosY_ )
{
    /*
    mk8253. 20110419.
    오브젝트 배치.

    { call dbo.ASGAAgitObjPos_merge ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iItemUID_  [int]
    @4 ; @iPosX_     [smallint]
    @5 ; @iPosY_     [smallint]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 아이템 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.ASGAAgitObjPos_merge ( %d, %I64d, %d, %d ) }",
        dwUserUID_, ItemUID_, nPosX_, nPosY_ ) );

    int nRet = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"오브젝트 배치.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KSP::AgiteObjPos_select( IN const DWORD& dwUserUID_, OUT std::vector<KAgitObjectPos>& vecObjectPos_ )
{
    /*
    mk8253. 20110419.
    오브젝트 위치 리턴.

    { call dbo.ASGAAgitObjPos_select ( @1 ) }
    @1 ; @iLoginUID_     [int]

    n return ( @1, @2, @3, @4 )
    @1 ; ItemUID bigint
    @2 ; ItemID  int
    @3 ; PosX    smallint
    @4 ; PosY    smallint
    */
    std::set<GCITEMUID> setUID; // 중복체크용
    setUID.clear();
    vecObjectPos_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.ASGAAgitObjPos_select ( %d ) }",
        dwUserUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KAgitObjectPos kItem;

        VERIFY_QUERY( m_kODBC >> kItem.m_UID
            >> kItem.m_ID
            >> kItem.m_nPosX
            >> kItem.m_nPosY );

        if ( false == setUID.insert( kItem.m_UID ).second ) {
            START_LOG( cwarn, L"중복된 오브젝트 정보. AgitUID : " << dwUserUID_ )
                << BUILD_LOG( kItem.m_UID )
                << BUILD_LOG( kItem.m_ID )
                << BUILD_LOG( kItem.m_nPosX )
                << BUILD_LOG( kItem.m_nPosY ) << END_LOG;
            continue;
        }

        vecObjectPos_.push_back( kItem );
    }

    START_LOG( clog, L"오브젝트 아이템 리스트. size : " << vecObjectPos_.size() ) << END_LOG;
    return true;
}

bool KSP::AgitHelper_merge( IN const DWORD& dwUserUID_, IN const KBrownieInfo& kInfo_ )
{
    /*
    mk8253. 20110419.아지트 도우미 갱신.

    { call dbo.ASGAAgitHelper_merge ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_   [int]
    @2 ; @iHelperType_ [tinyint]
    @3 ; ItemUID_    [bigint]
    @4 ; @bUsed_       [bit]

    1 return ( @1 )
    @1 ; OK int
    -101이하 : 트랜잭션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.ASGAAgitHelper_merge ( %d, %d, %I64d, %d ) }",
        dwUserUID_, kInfo_.m_dwUID, kInfo_.m_EquipItemUID, kInfo_.m_bIsUsed ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"아지트 도우미 갱신.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

//bool KSP::UpdateItemGrade( IN const DWORD& dwUserUID_, IN const char cCharType_, IN const GCITEMUID& ItemUID_, IN const int& nGrade_ )
//{
//    /*
//    L"{ call dbo.UIGAUserItem_Grade_update_20130415 ( %d, %d, %I64d, %d ) }"
//
//    { call dbo.UIGAUserItem_Grade_update_20130415 ( @01, @02, @03, @04 ) }
//    @01: @01iLoginUID_input int
//    @02: @02iCharType_input int
//    @03: @03iItemUID_input  bigint
//    @04: @04iGrade_input    tinyint
//        
//    1 return ( @01 )
//    @01: OK int
//        0       : 성공
//        -1      : 유저 정보 없음
//        -2      : 메인 아이템 정보 없음
//        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
//        -101이하: 트랜잭션 에러
//    */
//
//    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_Grade_update_20130415 ( %d, %d, %I64d, %d ) }",
//        dwUserUID_, cCharType_ ItemUID_, nGrade_ ) );
//
//    int nRet = -99;
//    if ( m_kODBC.BeginFetch() ) {
//        VERIFY_QUERY( m_kODBC >> nRet );
//        m_kODBC.EndFetch();
//    }
//
//    if ( nRet != 0 ) {
//        START_LOG( cerr, L"아이템 등급 변경 실패. Result : " << nRet )
//            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
//        return false;
//    }
//
//    _LOG_SUCCESS( nRet == 0, L"아이템 등급 변경. Result : " << nRet )
//        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
//
//    return true;
//}

bool KSP::UpdateItemGrade( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nGrade_ )
{
    /*
    L"{ call dbo.UIGAUserItemGrade_update_20111104 ( %d, %I64d, %d ) }"
    { call dbo.UIGAUserItemGrade_update_20111104 ( @1, @2, @3 ) }
    @1: @iLoginUID_ int
    @2: @iItemUID_  bigint
    @3: @iGrade_    tinyint
        
    1 return ( @1 )
    @1: OK
    1: OK int
        0       : 성공
        -1      : 유저 정보가 없음
        -2      : 아이템 정보가 없음
        -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemGrade_update_20111104 ( %d, %I64d, %d ) }",
        dwUserUID_, ItemUID_, nGrade_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    if ( nRet != 0 ) {
        START_LOG( cerr, L"아이템 등급 변경 실패. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        return false;
    }

    _LOG_SUCCESS( nRet == 0, L"아이템 등급 변경. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

int KSP::Post_insert( IN const DWORD& dwToUserUID_, IN const char& cCharType_, IN const char& cScriptType_, IN const DWORD& dwFromUserUID_, IN const std::wstring& strTitle_, 
                      IN const std::wstring& strMsg_, OUT POSTUID& PostUID_, IN int nGP_/*=0*/, IN int nStorageTime_/*=-1*/ )
{
    /*
    { call dbo.PSGAPostSystem_insert_20130521 ( %d, %d, %d, %d, %d, N''%s'', N''%s'', %d ) }

    { call dbo.PSGAPostSystem_insert_20130521 ( @01, @02, @03, @04, @05, @06, @07, @08 ) }
    @01: @01iLoginUID_input          int
    @02: @02iCharType_input          int
    @03: @03iSLoginUID_input         int
    @04: @04iType_input              int
    @05: @05iMaintenancePeriod_input int
    @06: @06strTitle_input           nvarchar( 20  )
    @07: @07strContents_input        nvarchar( 600 )
    @08: @08iGamePoint_input         int

    1 return ( @01, @02 )
    @01: @iOK    int
    0        : 성공
    -1       : 유지기간이 0보다 작음
    -2       : 게임포인트가 0보다 작음
    -3       : 유저 정보 없음
    -4       : 캐릭터 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    @02: PostUID bigint
    */

    PostUID_ = 0;
    int nOK = -99;
    USHORT usScriptType = static_cast<USHORT>( cScriptType_ );

    DBJIF3( m_kODBC.Query( L"{ call dbo.PSGAPostSystem_insert_20130521 ( %d, %d, %d, %d, %d, N'%s', N'%s', %d ) }",
        dwToUserUID_, cCharType_, dwFromUserUID_, usScriptType, nStorageTime_, strTitle_.c_str(), strMsg_.c_str(), nGP_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> PostUID_ );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"우편 보내기.. 결과 : " << nOK )
        << BUILD_LOG( PostUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

int KSP::PostItem_insert( IN const DWORD& dwToUserUID_, IN const char& cCharType_, IN const POSTUID& PostUID_, IN const GCITEMUID& ItemUID_, IN const int& nCount_ )
{
    /*
    { call dbo.PSGAPostSystemItem_insert_20130521 ( %d, %d, %I64d, %I64d, %d ) }

    { call dbo.PSGAPostSystemItem_insert_20130521 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input         int
    @02: @02iCharType_input         int
    @03: @03iPostUID_input          bigint
    @04: @04iReferenceItemUID_input bigint
    @05: @05iCNT_input              int

    1 return ( @01 )
    @01: @iOK int
    0        : 성공
    -1       : 받는 유저 존재 하지 않음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.PSGAPostSystemItem_insert_20130521 ( %d, %d, %I64d, %I64d, %d ) }",
        dwToUserUID_, cCharType_, PostUID_, ItemUID_, nCount_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"우편 아이템 첨부.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return nOK;
}

int KSP::WaitItem_insert_20111028( IN const DWORD& dwToUserUID_, IN const char cCharType_, IN const GCITEMID& ItemID_, 
                                   IN const int& nGrade_, IN const int& nPeriod_, IN const int& nCount_, 
                                   IN const int& nGetType_, OUT GCITEMUID& ItemUID_, IN int nItemLevel_/*=0*/, 
                                   IN int nStrengthLV_/*=0*/, IN char cStatus_/*=12*/ )
{
    /*
    사용 : 3.시스템우편 첨부아이템_대기아이템 등록
    20111028.    jgisub.   우편함 시스템 구축과정에서 Status 컬럼 컨트롤 기능 추가.
    L"{ call dbo.WIGAWaitItem_insert_20130402 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"
    { call dbo.WIGAWaitItem_insert_20130402 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10 ) }
    @01: @01iLoginUID_input      int
    @02: @02iCharType_input      int
    @03: @03iItemID_input        int
    @04: @04iGradeID_input       smallint
    @05: @05iPeriod_input        int
    @06: @06iDuration_input      smallint
    @07: @07iGetType_input       tinyint
    @08: @08iStatus_input        tinyint
    @09: @09iItemLevel_input     smallint
    @10: @10iStrengthLevel_input int

    1 return ( @1 )
    @1: OK      int
        0       : 성공
        -1      : 유저 정보가 없음
        -2      : 아이템ID가 음수로 들어옴
        -3      : 기간이 500일을 초과함
        -4      : 수량이 2012개를 초과함
        -101이하: 트랜젝션 에러
    @2: WIGAUID bigint
    */
    ItemUID_ = 0;
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_insert_20130402 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwToUserUID_, cCharType_, ItemID_, nGrade_, nPeriod_, nCount_, nGetType_, cStatus_, nItemLevel_, nStrengthLV_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> ItemUID_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"시스템우편 첨부아이템_대기아이템 등록.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( ItemUID_ ) << END_LOG;
    return nOK;
}

bool KSP::Post_select( IN const DWORD& dwUserUID_, IN const char& cCharType_, OUT std::vector<KPostItemInfo>& vecList_ )
{
    /*
    { call dbo.PSGAPostSystem_select_20130521 ( %d, %d ) }

    { call dbo.PSGAPostSystem_select_20130521 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09 )
    @01: PostUID           bigint
    @02: SLoginUID         int
    @03: [Type]            int
    @04: [Status]          int
    0: new
    1: 읽지 않음
    2: 읽음(첨부파일미수령)
    3: 읽음(첨부파일수령)
    @05: RegDateA          datetime
    @06: MaintenancePeriod int
    @07: Title             nvarchar( 20 )
    @08: Contents          nvarchar( 600 )
    @09: GamePoint         int
    */

    vecList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.PSGAPostSystem_select_20130521 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KPostItemInfo kInfo;
        int nState = -1;
        int nScriptType = -1;

        VERIFY_QUERY( m_kODBC >> kInfo.m_PostUID
            >> kInfo.m_dwFromUserUID
            >> nScriptType
            >> nState
            >> kInfo.m_tmRegDate
            >> kInfo.m_nStorageTime
            >> kInfo.m_strTitle
            >> kInfo.m_strMessage
            >> kInfo.m_nGamePoint
            >> kInfo.m_strFromNickName );

        kInfo.m_cScriptType = static_cast<char>( nScriptType );

        // LS_NEW = 0, // 새 우편
        // LS_NOT_READ = 1, // 읽지않음
        // LS_READ = 2, // 읽음(첨부Item받지않음)
        // LS_READ_ITEM = 3, // 읽음(첨부Item받음)
        switch ( nState )
        {
        case KPostItemInfo::LS_NEW: // 새 우편
            kInfo.m_bRead = false;
            kInfo.m_bGetItemGP = false;
            break;
        case KPostItemInfo::LS_NOT_READ: // 읽지않음
            kInfo.m_bRead = false;
            kInfo.m_bGetItemGP = false;
            break;
        case KPostItemInfo::LS_READ: // 읽음(첨부받지않음)
            kInfo.m_bRead = true;
            kInfo.m_bGetItemGP = false;
            break;
        case KPostItemInfo::LS_READ_ITEM: // 읽음(첨부받음)
            kInfo.m_bRead = true;
            kInfo.m_bGetItemGP = true;
            break;
        default:
            START_LOG( cwarn, L"정의되지않은 우편 상태값 : " << nState ) << END_LOG;
            break;
        }

        vecList_.push_back( kInfo );

        //{{ 개발테스트용 로그
        START_LOG( clog, L"우편정보.. UserUID : " << dwUserUID_ )
            << BUILD_LOGc( cCharType_ )
            << BUILD_LOG( kInfo.m_PostUID )
            << BUILD_LOG( kInfo.m_dwFromUserUID )
            << BUILD_LOGc( kInfo.m_cScriptType )
            << BUILD_LOG( nState )
            << BUILD_LOGtm( CTime(kInfo.m_tmRegDate) )
            << BUILD_LOG( kInfo.m_nStorageTime )
            << BUILD_LOG( kInfo.m_strTitle )
            << BUILD_LOG( kInfo.m_strMessage )
            << BUILD_LOG( kInfo.m_bGetItemGP )
            << BUILD_LOG( kInfo.m_bRead )
            << BUILD_LOG( kInfo.m_nGamePoint ) << END_LOG;
        //}}
    }

    START_LOG( clog, L"우편함 목록 불러옴. size : " << vecList_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

bool KSP::PostItem_select( IN const DWORD& dwUserUID_, IN const char& cCharType_, OUT std::map<POSTUID,std::vector<KItem> >& mapList_ )
{
    /*
    { call dbo.PSGAPostSystemItem_select_20130521 ( %d, %d ) }

    { call dbo.PSGAPostSystemItem_select_20130521 ( @01, @02 ) }
    @01: @01iLoginUID_input         int
    @02: @02iCharType_input         int

    1 return ( @01, @02, @03, @04, @05 )
    @01: PostUID          bigint
    @02: ReferenceItemUID bigint
    @03: ItemID           int
    @04: Period           int
    @05: CNT              int
    */

    mapList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.PSGAPostSystemItem_select_20130521 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        POSTUID PostUID = 0;
        KItem kItem;

        VERIFY_QUERY( m_kODBC >> PostUID >> kItem.m_ItemUID >> kItem.m_ItemID >> kItem.m_nPeriod >> kItem.m_nCount );

        kItem.m_nInitCount = kItem.m_nCount;
        mapList_[PostUID].push_back( kItem );
        START_LOG( clog, L"PostUID : " << PostUID << L", ItemUID : " << kItem.m_ItemUID ) << END_LOG;
    }

    START_LOG( clog, L"우편함 첨부아이템 목록 불러옴. size : " << mapList_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

int KSP::PostItem_delete( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const POSTUID& PostUID_, IN const GCITEMUID& ItemUID_ )
{
    /*
    { call dbo.PSGAPostSystemItem_delete_20130521 ( %d, %d, %I64d, %I64d ) }

    { call dbo.PSGAPostSystemItem_delete_20130521 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input         int
    @02: @02iCharType_input         int
    @03: @03iPostUID_input          bigint
    @04: @04iReferenceItemUID_input bigint

    1 return ( @01 )
    @01: @iOK int
    0        : 성공
    -1       : 삭제될 우편 아이템 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.PSGAPostSystemItem_delete_20130521 ( %d, %d, %I64d, %I64d ) }",
        dwUserUID_, cCharType_, PostUID_, ItemUID_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"우편함 첨부아이템 받기.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return nOK;
}

bool KSP::Post_delete( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const POSTUID& PostUID_, IN const int& nDelState_ )
{
    /*
    { call dbo.PSGAPostSystem_delete_20130521 ( %d, %d, %I64d, %d ) }

    { call dbo.PSGAPostSystem_delete_20130521 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iPostUID_input  bigint
    @04: @04iDelstate_input tinyint

    1 return (@1)
    @1: @iOK int
    0        : 성공
    -1       : 삭제될 우편이 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.PSGAPostSystem_delete_20130521 ( %d, %d, %I64d, %d ) }",
        dwUserUID_, cCharType_, PostUID_, nDelState_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"우편 삭제.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return ( nOK == 0 );
}

int KSP::Post_State_update( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const POSTUID& PostUID_, IN const int nState_ )
{
    /*
    { call dbo.PSGAPostSystem_update_Status_20130521 ( %d, %d, %I64d, %d ) }

    { call dbo.PSGAPostSystem_update_Status_20130521 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iPostUID_input  bigint
    @04: @04iStatus_input   int

    1 return (@1)
    @1: @iOK int
    0        : 성공
    -1       : 우편 존재 하지 않음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */
    int nOK = -99;

    DBJIF3( m_kODBC.Query( L"{ call dbo.PSGAPostSystem_update_Status_20130521 ( %d, %d, %I64d, %d ) }",
        dwUserUID_, cCharType_, PostUID_, nState_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"우편함 우편상태 변경.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return nOK;
}

int KSP::WaitItem_Status_update( IN const DWORD& dwToUserUID_, IN const char cCharType_, IN const GCITEMUID& ItemUID_, IN const char& cBeforeStatus_, IN const char& cAfterStatus_ )
{
    /*
    사용 : 11.(우편첨부)대기아이템 Status 값 변경 (첨부아이템을 획득한 후에 불린다)
    유저가 첨부 파일을 보내면 아이템이 WaitItem에 Status값 3으로 기록되고 첨부파일 받기 클릭시 Status값 3 -> 2으로 변경된다.
    { call dbo.WIGAWaitItem_Status_update_20130402 ( %d, %d, %I64d, %d, %d ) }
    { call dbo.WIGAWaitItem_Status_update_20130402 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input     int
    @02: @02iCharType_input     int
    @03: @03iWIGAUID_input      bigint
    @04: @04iStatusBefore_input tinyint
    @05: @05iStatusAfter_input  tinyint

    1 return ( @1 )
    @1: @iOK int
    0       : 성공
    -1      : 조건에 맞는 WaitItem존재 하지 않음
    -101이하: 트랜잭션에러
    */
    int nOK = -99;
    USHORT usBeforeStatus = static_cast<USHORT>( cBeforeStatus_ );
    USHORT usAfterStatus = static_cast<USHORT>( cAfterStatus_ );

    DBJIF3( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_Status_update_20130402 ( %d, %d, %I64d, %d, %d ) }",
        dwToUserUID_, cCharType_, ItemUID_, usBeforeStatus, usAfterStatus ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"우편첨부 대기아이템 Status값 변경.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return nOK;
}

int KSP::WaitItem_Chang_update( IN const DWORD& dwToUserUID_, IN const GCITEMUID& ItemUID_, IN const DWORD& dwFromUserUID_, IN const char& cBeforeStatus_, IN const char& cAfterStatus_ )
{
    /*
    사용 : 12.우편보내기 실패시 첨부아이템 되살리기
    WIGAWaitItem에 LoginUID 값과 Status을 우편 보낸 사람으로 변경
    ( 변경 되는 모든 정보는 log.dbo.WILOWaitItemChang 테이블에 기록 됩니다. )

    { call dbo.WIGAWaitItem_Chang_update ( @1, @2, @3, @4, @5 ) }
    @1 ; @iLoginUID_   int     ; 유저 UID
    @2 ; @iWIGAUID_    bigint  ; 대기아이템 UID
    @3 ; @iFLoginUID_  int     ; 아이템 회수 받을 유저 UID
    @4 ; @iStatus_     tinyint ; 대기아이템 상태 
    @5 ; @iCStatus_    tinyint ; 변경될 상태값

    1 return (@1)
    @1 ; @iOK int
    0        ; 성공
    -1       ; 조건에 맞는 WaitItem존재 하지 않음
    -101이하 ; 트랜잭션에러
    */
    int nOK = -99;
    USHORT usBeforeStatus = static_cast<USHORT>( cBeforeStatus_ );
    USHORT usAfterStatus = static_cast<USHORT>( cAfterStatus_ );

    DBJIF3( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_Chang_update ( %d, %I64d, %d, %d, %d ) }",
        dwToUserUID_, ItemUID_, dwFromUserUID_, usBeforeStatus, usAfterStatus ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"우편보내기 실패시 첨부아이템 되살리기.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return nOK;
}

bool KSP::WaitItem_post_select( IN const DWORD& dwUserUID_, IN const char cCharType_, OUT std::map<GCITEMUID,KItem>& mapList_ )
{
    /*
    사용 : 13.WIGAWaitItem에 우편 아이템 목록 뽑아오기 (시스템 우편, 수량 아이템)
    L"{ call dbo.WIGAWaitItem_post_select_20130402 ( %d, %d ) }"
    { call dbo.WIGAWaitItem_post_select_20130402 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11 )
    @01: WIGAUID       bigint
    @02: BuyerNickname nvarchar( 24 )
    @03: ItemID        int
    @04: GradeID       int
    @05: Period        int
    @06: CNT           int
    @07: GetType       int
    @08: Comment       nvarchar( 80 )
    @09: ItemLevel     int
    @10: StrengthLevel int
    @11: Status        int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WIGAWaitItem_post_select_20130402 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kPostItem;
        std::wstring strBuyerNick;
        std::wstring strComment;
        int nGetType = 0;
        int nGradeID = 0;
        int nEnchantLevel = 0;
        int nStatus = 0;

        VERIFY_QUERY( m_kODBC >> kPostItem.m_ItemUID
            >> strBuyerNick
            >> kPostItem.m_ItemID
            >> nGradeID
            >> kPostItem.m_nPeriod
            >> kPostItem.m_nCount
            >> nGetType
            >> strComment
            >> kPostItem.m_sEquipLevel
            >> nEnchantLevel
            >> nStatus );

        kPostItem.m_nInitCount = kPostItem.m_nCount;
        kPostItem.m_cGradeID = static_cast<char>( nGradeID );
        kPostItem.m_cEnchantLevel = nEnchantLevel;

        mapList_[kPostItem.m_ItemUID] = kPostItem;

        //{{ 개발테스트용 로그
        START_LOG( clog, L"우편함 대기아이템 세부정보.. UserUID : " << dwUserUID_ )
            << BUILD_LOG( kPostItem.m_ItemUID )
            << BUILD_LOG( strBuyerNick )
            << BUILD_LOG( kPostItem.m_ItemID )
            << BUILD_LOG( nGradeID )
            << BUILD_LOG( kPostItem.m_nPeriod )
            << BUILD_LOG( kPostItem.m_nCount )
            << BUILD_LOG( nGetType )
            << BUILD_LOG( strComment )
            << BUILD_LOG( kPostItem.m_sEquipLevel )
            << BUILD_LOG( nEnchantLevel )
            << BUILD_LOG( nStatus ) << END_LOG;
        //}}
    }

    START_LOG( clog, L"우편함 대기아이템 세부목록 불러옴. UserUID : " << dwUserUID_ << L", size : " << mapList_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return true;
}

bool KSP::Post_NewPost_select( IN const DWORD& dwUserUID_, IN const char& cCharType_, OUT std::vector<KPostItemInfo>& vecList_ )
{
    /*
    { call dbo.PSGAPostSystem_select_20130521 ( %d, %d ) }

    { call dbo.PSGAPostSystem_select_20130521 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02, @03, @04, @05, @06, @07, @08, @09 )
    @01: PostUID           bigint
    @02: SLoginUID         int
    @03: [Type]            int
    @04: [Status]          int
    0: new
    1: 읽지 않음
    2: 읽음(첨부파일미수령)
    3: 읽음(첨부파일수령)
    @05: RegDateA          datetime
    @06: MaintenancePeriod int
    @07: Title             nvarchar( 20 )
    @08: Contents          nvarchar( 600 )
    @09: GamePoint         int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PSGAPostSystem_select_20130521 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KPostItemInfo kInfo;
        int nState = -1;
        int nScriptType = -1;

        VERIFY_QUERY( m_kODBC >> kInfo.m_PostUID
            >> kInfo.m_dwFromUserUID
            >> nScriptType
            >> nState
            >> kInfo.m_tmRegDate
            >> kInfo.m_nStorageTime
            >> kInfo.m_strTitle
            >> kInfo.m_strMessage
            >> kInfo.m_bGetItemGP );

        kInfo.m_cScriptType = static_cast<char>( nScriptType );

        switch ( nState )
        {
        case 0: // 읽지않음
            kInfo.m_bRead = false;
            kInfo.m_bGetItemGP = false;
            break;
        case 1: // 읽음(첨부받지않음)
            kInfo.m_bRead = true;
            kInfo.m_bGetItemGP = false;
            break;
        case 2: // 읽음(첨부받음)
            kInfo.m_bRead = true;
            kInfo.m_bGetItemGP = true;
            break;
        default:
            START_LOG( cwarn, L"정의되지않은 우편 상태값 : " << nState ) << END_LOG;
            break;
        }

        vecList_.push_back( kInfo );

        //{{ 개발테스트용 로그
        START_LOG( clog, L"신규 우편정보.. UserUID : " << dwUserUID_ )
            << BUILD_LOGc( cCharType_ )
            << BUILD_LOG( kInfo.m_PostUID )
            << BUILD_LOG( kInfo.m_dwFromUserUID )
            << BUILD_LOGc( kInfo.m_cScriptType )
            << BUILD_LOG( nState )
            << BUILD_LOGtm( CTime(kInfo.m_tmRegDate) )
            << BUILD_LOG( kInfo.m_nStorageTime )
            << BUILD_LOG( kInfo.m_strTitle )
            << BUILD_LOG( kInfo.m_strMessage )
            << BUILD_LOG( kInfo.m_bGetItemGP )
            << BUILD_LOG( kInfo.m_bRead )
            << BUILD_LOG( kInfo.m_bGetItemGP ) << END_LOG;
        //}}
    }

    START_LOG( clog, L"우편함 새 우편 목록만 불러옴. size : " << vecList_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KSP::PostItem_detail_select( IN const DWORD& dwUserUID_, IN const POSTUID& PostUID_, IN const char& cScriptType_, IN const GCITEMUID& ItemUID_, 
                                 OUT GCITEMID& ItemID_, OUT int& nCount_, OUT int& nPeriod_ )
{
    /*
    사용 : 15. 우편함 아이템 상세 정보 

    { call dbo.PSGAPostItem_detail_select ( @1, @2, @3, @4 ) }
    @1 ; @iSort_     tinyint ; 우편 종류( 일반:0, 시스템:1부터~쭈욱 )
    @2 ; @iLoginUID_ int     ; 유저 LoginUID
    @3 ; @iPostUID_  int     ; 우편 UID
    @4 ; @iItemUID_  bigint  ; 아이템UID / WIGAUID

    1 return (@1, @2)
    @1 ; ItemID   --아이템 ID
    @2 ; Period   --아이템 기간
    @3 ; Duration --아이템 수량
    */
    nCount_ = 0;
    nPeriod_ = 0;
    USHORT usScriptType = static_cast<USHORT>( cScriptType_ );

    DBJIF2( m_kODBC.Query( L"{ call dbo.PSGAPostItem_detail_select ( %d, %d, %d, %d ) }",
        usScriptType, dwUserUID_, PostUID_, ItemUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> ItemID_
            >> nPeriod_
            >> nCount_ );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"우편함 아이템 상세정보.. UserUID : " << dwUserUID_ )
        << BUILD_LOG( ItemUID_ )
        << BUILD_LOG( ItemID_ )
        << BUILD_LOG( nPeriod_ )
        << BUILD_LOG( nCount_ )
        << BUILD_LOG( PostUID_ )
        << BUILD_LOG( usScriptType )
        << BUILD_LOGc( cScriptType_ ) << END_LOG;

    return true;
}

bool KSP::CharacterLevelPeriod_merge_20111122( IN const DWORD& dwUserUID_, IN const int& nCharType_, IN const DWORD& dwLv_, IN const int& nTimeDiff_ )
{
    /*
    유저님께서 엘리시스 캐릭터를 5레벨 때 10분 하셨으면 종료시에 한번만 써주시면 됩니다
    { call dbo.CLGACharacterLevelPeriod_merge_20111122 ( 유저뉨, 엘리시스, 5, 10 ) }

    유저님께서 접속하셨을 때 엘리시스 캐릭터의 레벨이 5레벨이었고
        종료 전까지 6레벨이 되었는데 5레벨 때 30분, 6레벨 때 10분이라면
        이런 경우는 2번 써주셔야 합니다
    { call dbo.CLGACharacterLevelPeriod_merge_20111122 ( 유저뉨, 엘리시스, 5, 30 ) }
    { call dbo.CLGACharacterLevelPeriod_merge_20111122 ( 유저뉨, 엘리시스, 6, 10 ) }

    시간은 분단위로, 차이값만 주시면 됩니다

    쉽게 생각하시면 접속할 때부터 시간 재서 그냥 푹 꽂아주시면 됩니다
    */

    /*
    { call dbo.CLGACharacterLevelPeriod_merge_20111122 ( @1, @2, @3, @4 ) }
    @1: @i유저_           int --LoginUID
    @2: @i캐릭터_         int --CharType
    @3: @i레벨_           int --Level
    @4: @i걸리는시간Diff_ int --Period(Minute) Diff

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CLGACharacterLevelPeriod_merge_20111122 ( %d, %d, %d, %d ) }",
        dwUserUID_, nCharType_, dwLv_, nTimeDiff_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"유저 캐릭터 레벨별 플레이시간 DB통계 기록.. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);
}

bool KSP::UserGameClear2_select_20111213( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN OUT std::map<int,KDungeonUserInfo>& mapList_ )
{
    /*
    L"{ call dbo.DCGADungeonClear2_select_20130524 ( %d, %d ) }"

    { call dbo.DCGADungeonClear2_select_20130524 ( @01, @02 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int

    n return ( @01, @02, @03, @04 )
    @01: DungeonID      int
    @02: LastDifficulty int
    @03: MaxDifficulty  int
    @04: RewardCNT      int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.DCGADungeonClear2_select_20130524 ( %d, %d ) }", 
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nModeID = -1;
        KDungeonClearData kInfo;

        VERIFY_QUERY( m_kODBC >> nModeID
            >> kInfo.m_shLastDifficulty
            >> kInfo.m_shMaxDifficulty
            >> kInfo.m_usLeftRewardCount );

        if ( nModeID < 0 ) continue;

        mapList_[nModeID].m_kClearData = kInfo;

        //{{ 개발테스트용 로그
        START_LOG( clog, L"던전 클리어 정보.. UserUID : " << dwUserUID_ )
            << BUILD_LOG( nModeID )
            << BUILD_LOG( kInfo.m_bClear )
            << BUILD_LOG( kInfo.m_shLastDifficulty )
            << BUILD_LOG( kInfo.m_shMaxDifficulty )
            << BUILD_LOG( kInfo.m_usLeftRewardCount ) << END_LOG;
        //}}
    }

    START_LOG( clog, L"유저 던전 클리어 정보.. UserUID : " << dwUserUID_ )
        << BUILD_LOG( mapList_.size() ) << END_LOG;

    return true;
}

bool KSP::UserGameClear2_merge_20111213( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const int& nModeID_, IN const KDungeonClearData& kInfo_ )
{
    /*
    L"{ call dbo.DCGADungeonClear2_merge_20130524 ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.DCGADungeonClear2_merge_20130524 ( @01, @02, @03, @04, @05, @06 ) }
    @01iLoginUID_input       int
   ,@02iCharType_input       int
   ,@03iDungeonID_input      int --던전ID
   ,@04iLastDifficulty_input int --마지막으로 클리어한 난이도
   ,@05iMaxDifficulty_input  int --최대 클리어 난이도
   ,@06iRewardCNT_input      int --보상개수

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.DCGADungeonClear2_merge_20130524 ( %d, %d, %d, %d, %d, %d ) }", 
        dwUserUID_, cCharType_, nModeID_, kInfo_.m_shLastDifficulty, kInfo_.m_shMaxDifficulty, kInfo_.m_usLeftRewardCount ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"던전 클리어데이터 저장결과.. " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )  << END_LOG;

    return (nOK == 0);
}

bool KSP::UserItemLevel_select_20120103( IN const DWORD& dwUserUID_, IN OUT std::vector<KItem>& vecItem_, IN const char cCharType_ )
{
    /*
    유효한 아이템의 장착레벨값 리턴
    L"{ call dbo.UIGAUserItemLevel_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemLevel_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03 )
    @01: ItemUID   bigint
    @02: Type      int
    @03: ItemLevel int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemLevel_select_20130415 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID;
        int nItemType = 0;
        int nItemLevel = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nItemType
            >> nItemLevel );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == ItemUID );

        if ( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 UID의 아이템을 찾을 수 없음. ItemUID : " << ItemUID << L", Type : " << nItemType << L", ItemLevel : " << nItemLevel << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        switch ( nItemType ) {
            case KItemLevelTable::IL_EQUIP_LV:
                vit->m_sEquipLevel = static_cast<short>( nItemLevel );
                break;
            case KItemLevelTable::IL_EQUIP_DOWN_LV:
                vit->m_sEquipLvDown = static_cast<short>( nItemLevel );
                break;
            default:
                START_LOG( cerr, L"정의되지 않은 아이템레벨 타입.. ItemType : " << nItemType ) << END_LOG;
                break;
        }
        START_LOG( clog, L"ItemType : " << nItemType ) << END_LOG;
    }

    return true;
}

bool KSP::CheckUserItem_merge( IN const DWORD& dwUserUID_, IN const GCITEMID& ItemID_, IN const char& cGrade_, IN const GCITEMUID& WaitItemUID_,
                         IN const int nPeriod_, IN const int nDuration_, IN const USHORT usAttributeSize_, IN const USHORT usSocketSize_, OUT GCITEMUID& ItemUID_, IN const char cCharType_,
                         OUT bool& bMerged_ )
{
    if ( usAttributeSize_ == 0 && usSocketSize_ == 0 ) 
    {
        /*
        이미 존재하는 기간 아이템 찾기
        L"{ call dbo.UIGAUserItemPeriod_select_1_20130415 ( %d, %d, %d, %d ) }"

        { call dbo.UIGAUserItemPeriod_select_1_20130415 ( @01, @02, @03, @04 ) }
            @01iLoginUID_input int
        ,   @02iCharType_input int
        ,   @03iItemID_input   int
        ,   @04iGrade_input    int

        1 return ( @01 )
        @01: ItemUID bigint
        */
        if( nPeriod_ > 0 ) {
            DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemPeriod_select_1_20130415 ( %d, %d, %d, %d ) }",
                dwUserUID_,
                cCharType_,
                ItemID_,
                static_cast<UCHAR>(cGrade_) ) );

            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> ItemUID_ );
                m_kODBC.EndFetch();
            }

            if( ItemUID_ > 0 ) { // 기간 아이템을 찾으면..,
                START_LOG( clog, L"기간 아이템 리턴 ItemUID : " << ItemUID_ )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                bMerged_ = true;
                return true;
            }
        }
        /*
        이미 존재하는 수량 아이템 찾기
        mk8253. 20091124.
        수량아이템 개수 변경 획득 및 사용, 삭제
        L"{ call dbo.UIGAUserItemDuration_select_1_20130415 ( %d, %d, %d, %d ) }"

        { call dbo.UIGAUserItemDuration_select_1_20130415 ( @01, @02, @03, @04 ) }
            @01iLoginUID_input int
        ,   @02iCharType_input int
        ,   @03iItemID_input   int
        ,   @04iGrade_input    int
            
        1 return ( @01 )
        @01: ItemUID bigint
        */
        if( nDuration_ > 0 ) {
            DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemDuration_select_1_20130415 ( %d, %d, %d, %d ) }",
                dwUserUID_,
                cCharType_, 
                ItemID_,
                static_cast<UCHAR>(cGrade_) ) );

            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> ItemUID_ );
                m_kODBC.EndFetch();
            }

            if( ItemUID_ > 0 ) { // 수량 아이템을 찾으면..,
                START_LOG( clog, L"수량 아이템 리턴 ItemUID : " << ItemUID_ )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                bMerged_ = true;
                return true;
            }
        }
    }

    bMerged_ = false;
    return true;
}

int KSP::UIGAUserItem_merge_20130415( IN const DWORD& dwUserUID_, IN const GCITEMID& ItemID_, IN const char& cGrade_, IN const GCITEMUID& WaitItemUID_, OUT GCITEMUID& ItemUID_ )
{
    /*
    메인 아이템 입력
    L"{ call dbo.UIGAUserItem_merge_20130415 ( %d, %d, %d, %I64d ) }"

    { call dbo.UIGAUserItem_merge_20130415 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemID_input   int
    @03: @03iGradeID_input  tinyint
    @04: @04iWIGAUID_input  bigint
    
    1 return ( @01, @02 )
    @01: OK      int
        0       : 성공
        -101이하: 트랜젝션 에러
    @02: ItemUID bigint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_merge_20130415 ( %d, %d, %d, %I64d ) }",
        dwUserUID_,
        ItemID_,
        static_cast<UCHAR>(cGrade_),
        WaitItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> ItemUID_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"UIGAUserItem_merge_20130415 호출. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

bool KSP::UserItemPeriod_merge( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nPeriod_, OUT time_t& tmEndDate_ )
{
    /*
    L"{ call dbo.UIGAUserItemPeriod_merge_20131210 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemPeriod_merge_20131210 ( @01, @02, @03 ) }
        @01iLoginUID_input    int
    ,   @02iItemUID_input     bigint
    ,   @03iPeriod_diff_input int
        
    1 return ( @01, @02 )
    @01: OK     int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -101이하: 트랜젝션 에러
    @02: Period smalldatetime
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemPeriod_merge_20131210 ( %d, %I64d, %d ) }",
            dwUserUID_,
            ItemUID_,
            nPeriod_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> tmEndDate_ );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 기간 정보 입력. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return ( nOK == 0 );
}

bool KSP::UserItemCount_merge( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& nCountDiff_, OUT int& nCount_ )
{
    /*
    L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemDuration_merge_20131028 ( @01, @02, @03 ) }
        @01iLoginUID_input      int
    ,   @02iItemUID_input       bigint
    ,   @03iDuration_diff_input int
        
    1 return ( @01, @02 )
        @01iOK       int
            0        : 성공
            -1       : 유저 정보 없음
            -2       : 메인 아이템 정보 없음
            -801~-899: 무시가능 에러
            -901~-999: 치명적인 에러
            -1000    : 사용 중지된 sp인데 사용하려 함
        @02iDuration int
    */
    //!!주의 : nDurationDiff_ 가 음수이면 수량 아이템을 소모합니다. 양수이면 수량 증가합니다.
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }",
            dwUserUID_,
            ItemUID_,
            nCountDiff_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> nCount_ );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( 0 == nOK, L"아이템 수량 정보 입력. Result : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (0 == nOK);
}

bool KSP::UpdateSocket_merge( IN const DWORD& dwUserUID_, IN const int& nSlotID_, IN const GCITEMID& CardID_, IN const int& nSocState_, IN OUT KItem& kItem_, IN const char cCharType_ )
{
    /*
    소켓 입력
    L"{ call dbo.UIGAUserItemSocket_merge_20111226 ( %d, %d, %d, %d, %d ) }"

    { call dbo.UIGAUserItemSocket_merge_20111226 ( @1, @2, @3, @4, @5 ) }
    @1: @iLoginUID_    int
    @2: @iItemUID_     bigint
    @3: @iSlotID_      tinyint
    @4: @iCardItemID_  int
    @5: @iSocketState_ tinyint

    1 return ( @1 )
    @1 ; OK
    0       : 성공
    -1      : 유저가 없음
    -2      : 아이템이 없음
    -101이하: 트랜젝션 에러
    */
    /*
    L"{ call dbo.UIGAUserItemSocket_merge_20130415 ( %d, %d, %I64d, %d, %d, %d ) }"

    { call dbo.UIGAUserItemSocket_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
    @01: @01iLoginUID_input    int
    @02: @02iCharType_input    int
    @03: @03iItemUID_input     bigint
    @04: @04iSlotID_input      tinyint
    @05: @05iCardID_input      int
    @06: @06iSocketState_input tinyint
        
    1 return ( @1 )
    @1 ; OK
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemSocket_merge_20130415 ( %d, %d, %I64d, %d, %d, %d ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nSlotID_, CardID_, nSocState_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK == 0 ) {
        START_LOG( clog, L"소켓 설정 성공" ) << END_LOG;
        KSocketInfo kSocket;
        kSocket.m_CardItemID = CardID_;
        kSocket.m_cSlotID = static_cast<char>(nSlotID_);
        kSocket.m_cState = static_cast<char>(nSocState_);
        kItem_.m_vecSocket.push_back( kSocket );
    }
    else {
        START_LOG( cerr, L"소켓 설정 오류, nOK : " << nOK << " Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
        return false;
    }

    return true;
}

bool KSP::UpdateLevel_merge( IN const DWORD dwUserUID_, IN const int nType_, IN const int nLevel_, IN OUT KItem& kItem_, IN const char cCharType_ )
{
    /*
    아이템레벨 입력
    L"{ call dbo.UIGAUserItemLevel_merge_20130415 ( %d, %d, %I64d, %d, %d ) }"

    { call dbo.UIGAUserItemLevel_merge_20130415 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input  int
    @02: @02iCharType_input  int
    @03: @03iItemUID_input   bigint
    @04: @04iType_input      tinyint
    @05: @05iItemLevel_input tinyint
        
    1 return ( @01 )
    @01: OK int
        0       : 성공
        -1      : 유저 정보 없음
        -2      : 메인 아이템 정보 없음
        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
        -101이하: 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemLevel_merge_20130415 ( %d, %d, %I64d, %d, %d ) }",
        dwUserUID_, cCharType_, kItem_.m_ItemUID, nType_, nLevel_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( nOK == 0 ) {
        START_LOG( clog, L"레벨 설정 성공" ) << END_LOG;
        kItem_.m_sEquipLevel = static_cast<short>( nLevel_ );
    }
    else {
        START_LOG( cerr, L"레벨 설정 오류, nOK : " << nOK << " Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
        return false;
    }

    return true;
}


//bool KSP::UpdateItemTotalAttributeMerge( IN const std::wstringstream& stm_ )
//{
//    /*
//    아이템 속성 입력하거나 갱신하거나
//    L"{ call dbo.UIGAUserItemAttribute_merge_20130415 ( %d, %d, %I64d, %d, %d, %d, %d  ) }"
//
//    { call dbo.UIGAUserItemAttribute_merge_20130415 ( @01, @02, @03, @04, @05, @06 ) }
//    @01: @01iLoginUID_input       int
//    @02: @02iCharType_input       int
//    @03: @03iItemUID_input        bigint
//    @04: @04iSlotID_input         tinyint
//    @05: @05iTypeID_input         smallint
//    @06: @06iValue_input          int
//    @07: @07iAttributeState_input tinyint
//        
//    1 return ( @1 )
//    @1: OK
//        0       : 성공
//        -1      : 유저 정보 없음
//        -2      : 메인 아이템 정보 없음
//        -3      : 해당 캐릭터가 보유하고 있는 아이템이 아님
//        -101이하: 트랜젝션 에러
//    */
//    int nOK = -99;
//    DBJIF2( m_kODBC.Query( stm_.str().c_str() ) );
//    if( m_kODBC.BeginFetch() )
//    {
//        VERIFY_QUERY( m_kODBC >> nOK );
//        m_kODBC.EndFetch();
//    }
//
//    return ( nOK == 0 );
//}

bool KSP::UserStorage_merge(IN const DWORD& dwUserUID_, IN const KDepotInfo& kDepot_)
{
    /*
    �������(â��) ���/����
    L"{ call dbo.USGAUserStorage_Merge ( %d, %d, %d, %d ) }"

    { call dbo.USGAUserStorage_Merge ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iCharID_   tinyint
    @3 ; @iTabID_    tinyint
    @4 ; @iSizeDiff_ int

    1 return ( @1 )
    @1: OK int
    0       : ����
    -1      : ���� ������ ����
    -101����: Ʈ����� ����
*/
    DBJIF2(m_kODBC.Query(L"{ call dbo.USGAUserStorage_Merge ( %d, %d, %d, %d ) }",
        dwUserUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID, kDepot_.m_nSlotSize));

    int nRet = -99;
    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS(nRet == 0, L"â�� ���/����. Result : " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    if (nRet != 0) {
        return false;
    }

    return true;
}

//  2012-1-13 woosh. �Ʒ� sp �� ����Ϸ��� sp �̸� �����ؾ� ��. DBA ���� �ʿ���.
bool KSP::UserStorage_select_by_char_tab(IN const DWORD& dwUserUID_, IN OUT KDepotInfo& kDepot_)
{
    /*
    ������� Tab�� ������ �˻�
    L"{ call dbo.USGAUserStorage_select ( %d, %d, %d ) }"

    { call dbo.USGAUserStorage_select ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iCharID_   tinyint
    @3 ; @iTabID_    tinyint

    1 return ( @1 )
    @1 ; Size    int
    */
    DBJIF2(m_kODBC.Query(L"{ call dbo.USGAUserStorage_alone_select ( %d, %d, %d ) }",
        dwUserUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID));

    int nRetSize = -99;
    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRetSize);
        m_kODBC.EndFetch();
    }

    kDepot_.m_nSlotSize = nRetSize;

    _LOG_SUCCESS(nRetSize >= 0, L"â�� Tab �� ������ �˻�. Tab : " << static_cast<int>(kDepot_.m_cTabID) << L" Size : " << nRetSize)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return true;
}

bool KSP::UserStorage_select(IN const DWORD& dwUserUID_, OUT std::map<char, KDepotInfo >& mapDepotInfo_)
{
    /*
    ������� Tab�� ������ �˻�
    L"{ call dbo.USGAUserStorage_select ( %d ) }"

    { call dbo.USGAUserStorage_select ( @1 ) }
    @1 ; @iLoginUID_ int

    n return ( @1, @2, @3 )
    @1 ; CharID   tinyint
    @2 ; TabID    tinyint
    @3 ; Size     int
    */
    //std::map<char, KDepotInfo > mapDepotInfo;

    DBJIF2(m_kODBC.Query(L"{ call dbo.USGAUserStorage_select ( %d ) }", dwUserUID_));

    while (m_kODBC.Fetch() != SQL_NO_DATA) {

        KDepotInfo depotInfo;
        UCHAR ucCharType = 255;
        UCHAR ucTabID = 0;
        int nSlotSize = -1;

        VERIFY_QUERY(m_kODBC >> ucCharType
            >> ucTabID
            >> nSlotSize);

        depotInfo.m_cTabID = ucTabID;
        depotInfo.m_nSlotSize = nSlotSize;

        if (ucCharType == 255) {
            mapDepotInfo_.insert(std::make_pair(depotInfo.m_cTabID, depotInfo));
        }

    }

    START_LOG(clog, L"���� â�� ĳ����/��/���� ������ ��������. UserUID : " << dwUserUID_ << L" Size : " << mapDepotInfo_.size())
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return true;
}


bool KSP::WareHouse_Merge(IN const DWORD& dwUserUID_, IN const KDepotItem& kDepot_)
{
    /*
    â���� Item ����/�����̵�
    L"{ call dbo.WHGAWareHouse_merge_20130506 ( %d, %I64d, %d, %d, %d ) }"
    { call dbo.WHGAWareHouse_merge_20130506 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @03: @03iCharID_input   tinyint
    @04: @04iTabID_input    tinyint
    @05: @05iSlotID_input   tinyint

    1 return ( @01 )
    @01: OK int
        0        : ����
        -1       : ���� ������ ����
        -2       : â�� ������ ������ ����
        -801~-899: ���ð��� ����
        -901~-999: ġ������ ����
    */
    DBJIF2(m_kODBC.Query(L"{ call dbo.WHGAWareHouse_merge_20130506 ( %d, %I64d, %d, %d, %d) }",
        dwUserUID_,
        kDepot_.m_ItemUID,
        static_cast<UCHAR>(255),
        kDepot_.m_kDepotKey.m_cTabID,
        kDepot_.m_kDepotKey.m_cSlotID));

    int nRet = -99;
    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    LIF(IgnorableErrorProcessingAfterCallSP(nRet));

    _LOG_SUCCESS(nRet == 0, L"â�� ������ ����/�����̵�. Result : " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    if (nRet != 0) {
        return false;
    }

    return true;
}

bool KSP::WareHouse_select(IN const DWORD& dwUserUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_)
{
    /*
    â�� Tab������ Item��� �̾Ƴ���.
    L"{ call dbo.WHGAWareHouse_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouse_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint

    n return ( @01, @02 )
    @01: ItemUID bigint
    @02: ItemID  int
    @03: GradeID tinyint
    @04: SlotID  tinyint
    */

    DBJIF2(m_kODBC.Query(L"{ call dbo.WHGAWareHouse_select_20130506 ( %d, %d, %d ) }",
        dwUserUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID));

    std::vector<KItem> vecItem;
    while (m_kODBC.Fetch() != SQL_NO_DATA) {
        KItem kItem;
        VERIFY_QUERY(m_kODBC >> kItem.m_ItemUID
            >> kItem.m_ItemID
            >> kItem.m_cGradeID
            >> kItem.m_cSlotID);

        // InvenType�� DB sp �� ���� select ���� �ʰ�, â�� ������ �ҷ����� ������ InvenType ������.
        kItem.m_cInitInvenType = kItem.m_cInvenType = KItem::DEPOT;
        kItem.m_cInitSlotID = kItem.m_cSlotID;
        kItem.m_cInitTabID = kItem.m_cTabID = kDepot_.m_cTabID;
        vecItem.push_back(kItem);
    }

    vecItem_.swap(vecItem);

    _LOG_CONDITION(!vecItem.empty(), clog, cwarn, L"â�� Tab �� ������ , size : " << vecItem.size())
        << BUILD_LOG(dwUserUID_)
        //<< BUILD_LOGc( kDepot_.m_cCharType )
        << BUILD_LOGc(kDepot_.m_cTabID)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return true;
}

bool KSP::WareHouse_Delete(IN const DWORD& dwUserUID_, IN const KDepotItem& kDepot_, OUT int& nCheckRet)
{
    /*
    â���� ��������Item �κ����� �̵�.
    L"{ call dbo.WHGAWareHouse_delete_20130506 ( %d, %I64d, %d, %d) }"

    { call dbo.WHGAWareHouse_delete_20130506 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @03: @03iCharID_input   tinyint
    @04: @04iTabID_input    tinyint

    1 return ( @1 )
    @1: OK int
        0        : ����
        -1       : ���� ������ ����
        -2       : â�� ������ ������ ����
        -3       : â���� ����
        -801~-899: ���ð��� ����
        -901~-999: ġ������ ����
    */

    DBJIF2(m_kODBC.Query(L"{ call dbo.WHGAWareHouse_delete_20130506 ( %d, %I64d, %d, %d) }",
        dwUserUID_,
        kDepot_.m_ItemUID,
        static_cast<UCHAR>(255),
        kDepot_.m_kDepotKey.m_cTabID));

    int nRet = -99;
    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    if (nRet == -21)
    {
        nCheckRet = -21;
    }

    LIF(IgnorableErrorProcessingAfterCallSP(nRet));

    _LOG_SUCCESS(nRet == 0, L"â������ �κ��丮�� ������ �̵�. UserUID : " << dwUserUID_)
        << BUILD_LOGc(kDepot_.m_kDepotKey.m_cTabID)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return true;
}


bool KSP::UserItem_DelState_Update( IN const DWORD& dwUserUID_, IN const GCITEMUID& ItemUID_, IN const int& Inven_OR_Depot_, IN const char cCharType_ )
{
    // 아이템의 DelState값 변경
    /*
    L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @04: @03iDelstate_input tinyint
        
    1 return ( @01 )
    @01: OK
        0        : 성공
        -1       : 메인 아이템 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
        -1000    : 사용 중지된 sp인데 사용하려 함
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }",
        dwUserUID_, ItemUID_, static_cast<UCHAR>( Inven_OR_Depot_ ) ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"창고<->인벤토리 아이템 이동에 따른 스테이트 조정. UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::GoodsInfoList_CharType_select( const IN GCITEMID& ItemID_, OUT char& charType_ )
{
    /*
    아이템의 케릭터타입 검색
    L"{ call dbo.GoodsInfoList_CharType_select ( %d ) }"
    
    { call dbo.GoodsInfoList_CharType_select ( @1 ) }
    @1 ; @iGoodsID_ int
     
    1 return ( @1 )
    @1 ; CharType    smallint
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.GoodsInfoList_CharType_select ( %d ) }", ItemID_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    charType_ = static_cast<char>(nRet);

    _LOG_SUCCESS( ( nRet >= 0 && nRet <= 255 ), L"아이템의 캐릭터 타입. ItemID " << ItemID_ )
        << BUILD_LOG( nRet )
        << BUILD_LOGc( charType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::WareHouse_Count_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
    /*
    창고 Tab단위로 유효한 아이템의 수량값 리턴
    L"{ call dbo.WHGAWareHouseDuration_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouseDuration_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03 )
    @01: ItemUID  bigint
    @02: Duration int
    @03: SlotID   tinyint
    */
    /*
    L"{ call dbo.WHGAWareHouseDuration_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouseDuration_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
 
    n return ( @01, @02, @03 )
    @01: ItemUID  bigint
    @02: Duration int
    @03: SlotID   tinyint
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseDuration_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    std::wstringstream stmMinus;
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID;
        int nCount;
        char slotID;
        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> nCount
            >> slotID );

        if ( nCount == 0 ) {
            continue;
        }

        if ( nCount < 0 ) {
            stmMinus << L"[" << itemUID << L"," << nCount << L"]";
            continue;
        }

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if ( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 수량을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_nCount = nCount;
        vit->m_nInitCount = nCount;
    }

    if ( !stmMinus.str().empty() ) {
        START_LOG( cerr, L"수량이 음수인 아이템 UserUID : " << dwUID_ << L" , "
            << stmMinus.str() ) << END_LOG;
    }

    return true;
}

bool KSP::WareHouse_Attribute_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
    /*
    창고 Tab단위로 유효한 아이템의 속성값 리턴
    L"{ call dbo.WHGAWareHouseAttribute_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouseAttribute_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04, @05, @06 )
    @01: ItemUID        bigint
    @02: SlotID         tinyint
    @03: TypeID         smallint
    @04: Value          int
    @05: AttributeState tinyint
        0: 초기화 필요
        1: 사용할 수 있음
        2: 사용중
        3: 사용할 수 없음
    @06: SlotID         tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseAttribute_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID       itemUID;
        KAttributeInfo kAttInfo;
        char slotID;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kAttInfo.m_cSlotID
            >> kAttInfo.m_cType
            >> kAttInfo.m_fValue
            >> kAttInfo.m_cState
            >> slotID );

        kAttInfo.m_fValue /= 10000.f;

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 속성을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        KItem& kItem = *vit;
        kItem.m_vecAttribute.push_back( kAttInfo );
    }

    return true;
}

bool KSP::WareHouse_Enchant_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
    /*
    창고 Tab단위로 유효한 아이템의 룬강화정보 리턴
    L"{ call dbo.WHGAWareHouseEnchant_select_20130506 ( %d, %d, %d ) }"
    { call dbo.WHGAWareHouseEnchant_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03 )
    @01: ItemUID     bigint
    @02: StrongLevel int
    @03: SlotID      tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseEnchant_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID itemUID;
        char cStrongLevel;
        char slotID;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> cStrongLevel 
            >> slotID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if ( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 강화값을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_cEnchantLevel = cStrongLevel;
    }

    return true;
}

bool KSP::WareHouse_Level_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
/*
    창고 Tab단위로 유효한 아이템의 레벨정보 리턴
    L"{ call dbo.WHGAWareHouseLevel_select_20130506 ( %d, %d, %d ) }"
    { call dbo.WHGAWareHouseLevel_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04 )
    @01: ItemUID   bigint
    @02: Type      int
        0 : 아이템 장착레벨
        1 : 감소되는 장착레벨 (초기값:0)
    @03: ItemLevel int
    @04: SlotID    tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseLevel_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID = 0;
        int ItemLevel = 0;
        int ItemType = 0;
        char slotID = 0;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> ItemType
            >> ItemLevel
            >> slotID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if ( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 레벨을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        switch ( ItemType ) {
            case KItemLevelTable::IL_EQUIP_LV:
                vit->m_sEquipLevel = static_cast<short>( ItemLevel );
                break;
            case KItemLevelTable::IL_EQUIP_DOWN_LV:
                vit->m_sEquipLvDown = static_cast<short>( ItemLevel );
                break;
            default:
                START_LOG( cerr, L"정의되지 않은 아이템레벨 타입.. ItemType : " << ItemType ) << END_LOG;
                break;
        }

    }
    return true;
}

bool KSP::WareHouse_Strength_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, IN OUT std::vector<KItem>& vecItem_, OUT std::map<GCITEMUID, GCITEMUID>& mapStrengthItem_ )
{
    //mapStrengthItem_.clear();
    /*
    창고 Tab단위로 유효한 아이템의 강화정보 리턴
    L"{ call dbo.WHGAWareHouseStrength_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouseStrength_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04 )
    @01: ItemUID         bigint
    @02: StrengthLevel   int
    @03: EquippedItemUID bigint
    @04: SlotID          tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseStrength_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID StrengthitemUID;
        char cStrongLevel;
        GCITEMUID EquipitemUID;
        char slotID;

        VERIFY_QUERY( m_kODBC
            >> StrengthitemUID
            >> cStrongLevel
            >> EquipitemUID
            >> slotID );

        // 강화석 정보 갱신.
        std::vector<KItem>::iterator vitStrengthItem = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == StrengthitemUID );
        if ( vitStrengthItem != vecItem_.end() ) {
            vitStrengthItem->m_cEnchantLevel = cStrongLevel;
        }

        // 강화석에 연결된 장비의 정보 갱신.
        std::vector<KItem>::iterator vitItem = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == EquipitemUID );
        if ( vitItem == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 강화석 연계 아이템을 찾을 수 없습니다. ItemUID : "<< StrengthitemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vitItem->m_EnchantEquipItemUID = StrengthitemUID;
        vitItem->m_cEnchantLevel = cStrongLevel;
        mapStrengthItem_[EquipitemUID] = StrengthitemUID;
    }

    return true;
}

bool KSP::UserItem_LoginUID_CharType_select( IN const DWORD dwUID_, IN const char cCharType_, OUT std::vector<KItem>& vecItem )
{
    /*
    유저의 아이템 리스트의 케릭터타입 검색.
    L"{ call dbo.UIGAUserItem_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItem_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04, @05 )
    @01: ItemUID  bigint
    @02: ItemID   int
    @03: GradeID  tinyint
    @04: CharType int --해당 아이템을 보유하고 있는 캐릭터
    @05: CharType int --아이템 종속 캐릭터
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_select_20130415 ( %d, %d ) }", dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        GCITEMID itemID;
        char cGrade;
        int nCharType;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> itemID
            >> cGrade
            >> nCharType );

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() ) {
            START_LOG( cwarn, L"해당 캐릭터의 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_cCharType = static_cast<char>(nCharType); // nCharType 이 0~255 범위로 가정하고 있음.

    }

    return true;
}

bool KSP::WareHouse_Period_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem )

{
    /*
    창고 Tab단위로 유효한 아이템의 기간정보 리턴
    L"{ call dbo.WHGAWareHousePeriod_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHousePeriod_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04 )
    @01: ItemUID bigint
    @02: Period  int
    @03: EndDate smalldatetime
    @04: SlotID  tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHousePeriod_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID;
        int nPeriod;
        time_t tEndDate;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> nPeriod
            >> tEndDate );

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() )
        {
            START_LOG( cwarn, L"해당 기간을 가진 창고 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_nPeriod = nPeriod;
        vit->m_tEndDate = tEndDate;
    }

    return true;
}

bool KSP::WareHouse_Socket_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
    /*
    창고 Tab단위로 유효한 아이템의 속성값 리턴
    L"{ call dbo.WHGAWareHouseSocket_select_20130506 ( %d, %d, %d ) }"
    { call dbo.WHGAWareHouseSocket_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04, @05 )
    @01: ItemUID   bigint
    @02: SlotID    tinyint
    @03: CardID    int
    @04: SockState tinyint
        0: 초기화 필요
        1: 사용할 수 있음
        2: 사용중
        3: 사용할 수 없음
    @05: SlotID    tinyint

    @04번 왠지 저게 아닌듯하여 여기에 기록합니다
    확인된것만
        2: 비어있는 소켓(이때 CardID는 0임)
        3: 몬스터 카드가 장착된 소켓(이때 CardID는 0이 아님)
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseSocket_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID itemUID;
        KSocketInfo kSocketInfo;
        char slotID;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kSocketInfo.m_cSlotID
            >> kSocketInfo.m_CardItemID
            >> kSocketInfo.m_cState
            >> slotID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 소켓을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_vecSocket.push_back( kSocketInfo );
    }

    return true;
}

bool KSP::Item_DelState_Select( IN const DWORD dwUID_, IN const GCITEMUID& ItemUID_, OUT char delState_ )
{
/*  
    아이템의 DelState값 검색.
    L"{ call dbo.UIGAUserItem_DelState_Select ( %d, %d ) }"  
      
    { call dbo.UIGAUserItem_DelState_Select ( @1, @2 ) }  
    @1 ; @iLoginUID_                int  
    @2 ; @iItemUID_                 bigint  
       
    1 return ( @1 )  
    @1: Delstate    tinyint

*/
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_DelState_Select ( %d, %d ) }", dwUID_, ItemUID_ ) );

    char delState = -1;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> delState );
        m_kODBC.EndFetch();
    }
    delState_ = delState;

    return true;
}

bool KSP::WareHouse_Coordi_select( IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::vector<KItem>& vecItem_ )
{
    /*  
    창고 Tab단위로 코디 아이템인지 확인.
    L"{ call dbo.WHGAWareHouseCoordi_select_20130506 ( %d, %d, %d ) }"

    { call dbo.WHGAWareHouseCoordi_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
 
    n return ( @01, @02 )
    @01: ItemUID bigint
    @02: SlotID  tinyint
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHouseCoordi_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID itemUID;
        char slotID;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> slotID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 코디 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->AddItemType(KItem::TYPE_LOOK);
    }

    return true;

}

bool KSP::WareHouse_Pet_select(IN const DWORD dwUID_, IN const KDepotInfo& kDepot_, OUT std::map< GCITEMUID, KPetInfo >& mapPetInfo_ )
{
    /*
    창고 Tab단위로 유효한 펫의 정보를 가저 온다.
    L"{ call dbo.WHGAWareHousePet_select_20130506 ( %d, %d, %d ) }"
    { call dbo.WHGAWareHousePet_select_20130506 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharID_input   tinyint
    @03: @03iTabID_input    tinyint
     
    n return ( @01, @02, @03, @04, @05, @06 )
    @01: PetUID    bigint
    @02: PetID     int
    @03: PetName   nvarchar( 20 )
    @04: Promotion tinyint
    @05: FullTime  int
    @06: SlotID    tinyint
    */
    //mapPetInfo_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.WHGAWareHousePet_select_20130506 ( %d, %d, %d ) }", dwUID_, static_cast<UCHAR>(255), kDepot_.m_cTabID ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        //GCITEMUID itemUID;
        char slotID;

        KPetInfo kPetInfo;
        kPetInfo.Init();
        kPetInfo.m_vecInitEquipItem.clear();
        kPetInfo.m_vecEquipItem.clear();

        VERIFY_QUERY( m_kODBC
            >> kPetInfo.m_dwUID         // 펫 UID
            >> kPetInfo.m_dwID          // 펫 아이템 아이디
            >> kPetInfo.m_strName       // 펫 이름
            >> kPetInfo.m_cPromotion    // 진화
            >> kPetInfo.m_iSatiation
            >> slotID );  // 만복도

        if ( mapPetInfo_.find( kPetInfo.m_dwUID) == mapPetInfo_.end() ) {
            // 초기 상태 설정.
            kPetInfo.m_iInitSatiation = kPetInfo.m_iSatiation;
            kPetInfo.m_nHatchingID = SiKPetHelper()->GetNextID( kPetInfo.m_dwID );            

            // 펫의 레벨 세팅
            mapPetInfo_.insert( std::make_pair( kPetInfo.m_dwUID, kPetInfo ) );
        }
        else {
            START_LOG( cerr, L"펫 정보 가져오는중 중복 데이터 있음." << L" User UID : " << dwUID_ << L", Pet UID : " << kPetInfo.m_dwUID ) << END_LOG;
        }    
    }
    return true;
}

bool KSP::PTEVPlayTimeEvent( IN const DWORD dwUID_, IN const std::wstring& strStartDate_, IN const std::wstring& strEndDate_ )
{
    /*
    유저 플레이타임 등록
    L"{ call dbo.PTEVPlayTimeEvent_insert ( %d, N'%s', N'%s' ) }"

    { call dbo.PTEVPlayTimeEvent_insert ( @1, @2, @3 ) }
    @1: @iLoginUID_input     int
    @2: @sdtLoginDate_input  smalldatetime
    @3: @sdtLogoutDate_input smalldatetime

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가 없음
        -901이하: 트랜젝션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PTEVPlayTimeEvent_insert ( %d, N'%s', N'%s' ) }", dwUID_, strStartDate_.c_str(), strEndDate_.c_str() ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }
    
    if ( nOK != 0 ) {
        START_LOG( cerr, L"플레이타임 이벤트 SP 리턴값 실패 " )
            << BUILD_LOG( nOK )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;

        return false;
    }

    return true;
}

void KSP::UIGAUserItemDuration_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem )
{
    /*
    유효한 아이템의 수량값 리턴
    L"{ call dbo.UIGAUserItemDuration_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemDuration_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02 )
    @01: ItemUID  bigint
    @02: Duration int
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemDuration_select_20130415 ( %d, %d ) }", dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID;
        int nDuration;
        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> nDuration );

        if( nDuration == 0 ) {
            continue;
        }

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() ) {
            START_LOG( cwarn, L"해당 수량을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        vit->m_nCount = nDuration;
        vit->m_nInitCount = nDuration;
    }
}

void KSP::UIGAUserItemPeriod_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem )
{
    /*
    유효한 아이템의 기간 리턴
    L"{ call dbo.UIGAUserItemPeriod_select_20130415 ( %d, %d ) }"
    { call dbo.UIGAUserItemPeriod_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03 )
    @01: ItemUID bigint
    @02: Period  int
    @03: EndDate smalldatetime
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemPeriod_select_20130415 ( %d, %d ) }", dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID;
        int nPeriod;
        time_t tEndDate;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> nPeriod
            >> tEndDate );

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() )
        {
            START_LOG( cwarn, L"해당 기간을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        vit->m_nPeriod = nPeriod;
        vit->m_tEndDate = tEndDate;
    }
}

void KSP::UIGAUserItemAttribute_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem )
{
    /*
    유효한 아이템의 속성값 리턴
    L"{ call dbo.UIGAUserItemAttribute_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemAttribute_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04, @05 )
    @01: ItemUID        bigint
    @02: SlotID         tinyint
    @03: TypeID         smallint
    @04: Value          int
    @05: AttributeState tinyint
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttribute_select_20130415 ( %d, %d ) }", dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID       itemUID;
        KAttributeInfo kAttInfo;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kAttInfo.m_cSlotID
            >> kAttInfo.m_cType
            >> kAttInfo.m_fValue
            >> kAttInfo.m_cState );

        kAttInfo.m_fValue /= 10000.f;

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() )
        {
            START_LOG( cwarn, L"해당 속성을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        KItem& kItem = *vit;
        kItem.m_vecAttribute.push_back( kAttInfo );
    }
}

void KSP::UIGAUserItemSocket_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem )
{
    /*
    L"{ call dbo.UIGAUserItemSocket_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItemSocket_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04 )
    @01: ItemUID
    @02: SlotID
    @03: CardID
    @04: SockState
        0: 초기화 필요
        1: 사용할 수 있음
        2: 사용중
        3: 사용할 수 없음

    @04번 왠지 저게 아닌듯하여 여기에 기록합니다
    확인된것만
        2: 비어있는 소켓(이때 CardID는 0임)
        3: 몬스터 카드가 장착된 소켓(이때 CardID는 0이 아님)
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemSocket_select_20130415 ( %d, %d ) }", dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID itemUID;
        KSocketInfo kSocketInfo;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kSocketInfo.m_cSlotID
            >> kSocketInfo.m_CardItemID
            >> kSocketInfo.m_cState );

        std::vector<KItem>::iterator vit = std::find_if( vecItem.begin(), vecItem.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem.end() )
        {
            START_LOG( cwarn, L"해당 소켓을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        vit->m_vecSocket.push_back( kSocketInfo );
    }
}

void KSP::UIGAUserItem_select( IN const DWORD& dwUserUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem )
{
    /*
    L"{ call dbo.UIGAUserItem_select_20130415 ( %d, %d ) }"

    { call dbo.UIGAUserItem_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03, @04, @05 )
    @01: ItemUID  bigint
    @02: ItemID   int
    @03: GradeID  tinyint
    @04: CharType int --해당 아이템을 보유하고 있는 캐릭터
    @05: CharType int --아이템 종속 캐릭터
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItem_select_20130415 ( %d, %d ) }", dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItem kItem;
        VERIFY_QUERY( m_kODBC
            >> kItem.m_ItemUID
            >> kItem.m_ItemID
            >> kItem.m_cGradeID
            >> kItem.m_cCharType );

        vecItem.push_back( kItem );
    }
}

void KSP::UserWareHouseTotalselect( IN const DWORD& dwUID_, OUT std::vector<GCITEMUID>& vecWareHouseItem_ )
{
    vecWareHouseItem_.clear();
    /*
    L"{ call dbo.WHGAWareHouse_select_ItemUID_20130506 ( %d ) }"

    { call dbo.WHGAWareHouse_select_ItemUID_20130506 ( @1 ) }
    @01 : @iLoginUID_input int

    n return ( @1 )
    @01: ItemUID bigint
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.WHGAWareHouse_select_ItemUID_20130506 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID WareHouseitemUID;

        VERIFY_QUERY( m_kODBC
            >> WareHouseitemUID );

        vecWareHouseItem_.push_back( WareHouseitemUID );
    }
}

void KSP::UserItemStrengthInfo( IN const DWORD& dwUID_, IN const char cCharType_, IN std::vector<GCITEMUID>& vecWareHouseItem_, IN OUT std::vector<KItem>& vecItem_, OUT std::vector<GCITEMUID>& vecRemoveItem_ )
{
    vecRemoveItem_.clear();

    /*
    유효한 아이템의 강화값 리턴
    L"{ call dbo.UIGAUserItemStrength_select_20130415 ( %d, %d ) }"
    { call dbo.UIGAUserItemStrength_select_20130415 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
        
    n return ( @01, @02, @03 )
    @01: ItemUID         bigint
    @02: StrengthLevel   int
    @03: EquippedItemUID bigint
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemStrength_select_20130415 ( %d, %d ) }", dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID StrengthitemUID;
        char cStrongLevel;
        GCITEMUID EquipitemUID;

        VERIFY_QUERY( m_kODBC
            >> StrengthitemUID
            >> cStrongLevel
            >> EquipitemUID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == StrengthitemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 강화석 아이템을 찾을 수 없습니다. ItemUID : "<< StrengthitemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_cEnchantLevel = cStrongLevel;
        vit->m_EnchantEquipItemUID = EquipitemUID;

        // 강화석 아이템이 장착되지 않았을 경우에 대해서는 아래의 갱신 처리 하지 않는다.
        if( EquipitemUID == 0 ) {
            continue;
        }

        // 강화석에 연결된 장비의 정보 갱신.
        std::vector<KItem>::iterator vitItem = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == EquipitemUID );
        if( vitItem == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 강화석 연계 아이템을 찾을 수 없습니다. ItemUID : "<< StrengthitemUID << L", UserUID : " << dwUID_ ) << END_LOG;

            std::vector<GCITEMUID>::iterator vitWareHouse = std::find( vecWareHouseItem_.begin(), vecWareHouseItem_.end(), EquipitemUID );
            if( vitWareHouse == vecWareHouseItem_.end() ) { // 창고에도 연계된 아이템이 없을 경우..,
                START_LOG( cwarn, L"해당 강화석 연계 아이템을 창고에서도 찾을 수 없습니다. " ) << END_LOG;
            }
            continue;
        }
        vitItem->m_EnchantEquipItemUID = StrengthitemUID;
        vitItem->m_cEnchantLevel = vit->m_cEnchantLevel;
        vitItem->m_cEnchantEquipGradeID = vit->m_cGradeID;
    }
}

bool KSP::CheckCashback( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN const GCITEMID dwItemID_, IN const int nPrice_, OUT DWORD& dwAddCashPoint_, OUT DWORD& dwResultCashPoint_ )
{
    enum ECashbackRatioType {
        ECT_NONE        = -1,
        ECT_FIRST_BUY   = 0,
        ECT_EVENT       = 1,
        ECT_DEFAULT     = 2,

        ECT_MAX,
    };

    int nCashbackType = ECT_NONE;
    int nVersion = -1;
    int nCashbackRatio = 0; // 캐시백 비율은 DB에 0 ~ 100 로 설정한다

    if ( true == CheckCashbackFirstBuyRatio( dwUID_, nVersion, nCashbackRatio ) ) { // 첫 구매 대상 유저인 경우 첫 구매 캐시백 비율을 적용한다
        nCashbackType = ECT_FIRST_BUY;
        START_LOG( clog, L"첫 구매 유저로 확인됨. 첫 구매 캐시백 비율 최우선 적용" ) << END_LOG;
    }
    else {
        if ( true == CheckCashbackEventRatio( dwItemID_, nVersion, nCashbackRatio ) ) { // 기간별 캐시백 비율을 우선하여 적용한다
            nCashbackType = ECT_EVENT;
            START_LOG( clog, L"기간별 캐시백 비율 우선 적용" ) << END_LOG;
        }
        else if ( true == CheckCashbackDefaultRatio( dwItemID_, nCashbackRatio ) ) { // 기간별 설정이 없는 경우 기본 캐시백 비율을 적용한다
            nCashbackType = ECT_DEFAULT;
            START_LOG( clog, L"기본 캐시백 비율 적용" ) << END_LOG;
        }
        else { // 설정된 내용이 없다면 캐시백 비율은 0%를 기본값으로 적용한다
            nCashbackRatio = 0;
            nCashbackType = ECT_NONE;

            START_LOG( clog, L"캐시백 비율 설정되지 않음" ) << END_LOG;
        }
    }

    DWORD dwAddCashPoint = 0;
    DWORD dwResultCashPoint = 0;
    // 캐시백으로 충전할 가상캐시 계산 후 DB에 기록함
    // min( 아이템 가격, ( 아이템 가격 * 캐시백 비율 ) )
    if ( 0 < nCashbackRatio && nCashbackRatio <= 100 && 0 < nPrice_ ) {
        dwAddCashPoint = static_cast< DWORD >( std::ceil( static_cast< double >( nPrice_ ) * ( nCashbackRatio / 100.0f ) ) );
        dwAddCashPoint = std::min< DWORD >( nPrice_, dwAddCashPoint );

        VirtualCashMergeWithType( dwUID_, dwItemID_, KVirtualCash::EVCMT_ADD_CASHBACK, dwAddCashPoint, 0 );
    }
    dwResultCashPoint = static_cast< DWORD >( GetCurrentVirtualCash( dwUID_, strLogin_ ) );

    dwAddCashPoint_ = dwAddCashPoint;
    dwResultCashPoint_ = dwResultCashPoint;

    // 로그 기록
    switch( nCashbackType ) {
        case ECT_FIRST_BUY:
            Insert_CashBack_FirstBuyUser_ToDB( dwUID_, nVersion, dwItemID_ );

            START_LOG( clog, L"캐시백 첫 구매 유저 로그 기록" ) << END_LOG;

            break;
        case ECT_EVENT:
            Insert_CashBack_EventUser_ToDB( dwUID_, nVersion, dwItemID_, nCashbackRatio );

            START_LOG( clog, L"캐시백 이벤트 유저 로그 기록" ) << END_LOG;

            break;
        case ECT_DEFAULT:
            // 캐시백 기본 비율 적용시에는 VirtualCashMergeWithType() 에서 이미 로그를 남겼다

            break;
        case ECT_NONE:
        default:
            START_LOG( clog, L"캐시백 비율 설정되지 않아서 로그 기록하지 않음" ) << END_LOG;
            break;
    }

    START_LOG( clog, L"캐시백 확인 로그. userUID: " << dwUID_
        << L", itemID: " << dwItemID_
        << L", 가격: " << nPrice_
        << L", 캐시백 비율: " << nCashbackRatio
        << L", 충전된 가상 캐시: " << dwAddCashPoint_
        << L", 최종 가상 캐시 잔액: " << dwResultCashPoint_ )
        << END_LOG;

    return true;
}

bool KSP::CheckCashbackFirstBuyRatio( IN const DWORD dwUID_, OUT int& nVersion_, OUT int& nCashbackRatio_ )
{
    /*
    캐시백 첫구매 비율
    L"{ call dbo.CBEVCashBackFirstBuyEvent_select ( %d ) }"

    { call dbo.CBEVCashBackFirstBuyEvent_select ( @1 ) }
    @1 ; @bAll_ bit
        0 : 갱신된데이터만읽음
        1 : 전체데이터읽음

    n return ( @1, @2, @3, @4 )
    @1 ; Version        int
    @2 ; StartDateD     smalldatetime
    @3 ; EndDateD       smalldatetime
    @4 ; Rate           int
    */
    nVersion_ = -1;
    nCashbackRatio_ = 0;

    int nVersion = -1;
    time_t tmStart = LONG_MAX;
    time_t tmEnd = LONG_MAX;
    int nRate = 0;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBEVCashBackFirstBuyEvent_select ( %d ) }", 1 ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nVersion
            >> tmStart
            >> tmEnd
            >> nRate );
        m_kODBC.EndFetch();
    }

    if ( -1 == nVersion ) {
        START_LOG( clog, L"캐시백 첫 구매 비율 이벤트 진행하지 않음" ) << END_LOG;

        return false;
    }

    START_LOG( clog, L"캐시백 첫 구매 비율 정보 가져옴" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOGtm( CTime( tmStart ) )
        << BUILD_LOGtm( CTime( tmEnd ) )
        << BUILD_LOG( nRate )
        << END_LOG;

    /*
    캐시백 첫구매 유저 데이터 검색(받았는지 여부 확인)
    L"{ call dbo.CBGACashBackFirstBuyEventUser_select ( %d, %d ) }"

    { call dbo.CBGACashBackFirstBuyEventUser_select ( @1, @2 ) }
    @1 : @iLoginUID_     int
    @2 : @iVersion_      int

    1 return ( @1 )
    @1 ; @iOK   // 데이터있으면1, 아니면0
    */
    nVersion_ = nVersion;

    int nOK = -1;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGACashBackFirstBuyEventUser_select ( %d, %d ) }", dwUID_, nVersion ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( -1 == nOK ) {  // DB 오류
        START_LOG( cerr, L"캐시백 첫 구매 여부 확인 실패. LoginUID: " ) << dwUID_ << END_LOG;
        return false;
    }

    if ( 1 == nOK ) {   // 이미 받아갔음
        START_LOG( clog, L"첫 구매로 이미 받아간 유저. LoginUID: " ) << dwUID_ << END_LOG;
        return false;
    }

    if ( 0 == nOK ) {
        // 첫구매 비율 적용 대상자일 경우에 비율을 리턴한다
        nCashbackRatio_ = nRate;
    }

    START_LOG( clog, L"캐시백 첫 구매 대상 유저 확인 완료" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( nCashbackRatio_ )
        << END_LOG;

    return true;
}

bool KSP::CheckCashbackEventRatio( IN const GCITEMID dwItemID_, OUT int& nVersion_, OUT int& nCashbackRatio_ )
{
    /*
    캐시백 이벤트 검색
    L"{ call dbo.CBEVCashBackEvent_select ( %d ) }"

    { call dbo.CBEVCashBackEvent_select ( @1 ) }
    @1 ; @bAll_ bit
        0 : 갱신된데이터만읽음
        1 : 전체데이터읽음

    n return ( @1, @2, @3 )
    @1 ; Version        int
    @2 ; StartDateD     smalldatetime
    @3 ; EndDateD       smalldatetime
    */
    nVersion_ = -1;
    nCashbackRatio_ = 0;

    int nVersion = -1;
    time_t tmStart = LONG_MAX;
    time_t tmEnd = LONG_MAX;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBEVCashBackEvent_select ( %d ) }", 1 ) );

    // SP에서는 n return 이지만 기간이 중복되도록 설정하지 않는 가정을 하여 1 return 처럼 처리한다
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nVersion
            >> tmStart
            >> tmEnd );
        m_kODBC.EndFetch();
    }

    if ( -1 == nVersion ) {
        START_LOG( clog, L"캐시백 이벤트 비율 진행하지 않음" ) << END_LOG;

        return false;
    }

    START_LOG( clog, L"캐시백 이벤트 비율, 버전 정보 가져옴" )
        << BUILD_LOG( dwItemID_ )
        << BUILD_LOG( nVersion )
        << BUILD_LOGtm( CTime( tmStart ) )
        << BUILD_LOGtm( CTime( tmEnd ) )
        << END_LOG;

    /*
    캐시백 이벤트 비율(아이템별)
    L"{ call dbo.CBEVCashBackEventItem_select ( %d, %d ) }"

    { call dbo.CBEVCashBackEventItem_select ( @1, @2 ) }
    @1 ; @iVersion_ int
    @2 ; @iItemID_  int

    1 return ( @1, @2 )
    @1 ; Rate       int
    */
    nVersion_ = nVersion;

    int nRate = -1;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBEVCashBackEventItem_select ( %d, %d ) }", nVersion, dwItemID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRate );
        m_kODBC.EndFetch();
    }

    if ( -1 == nRate ) {
        nCashbackRatio_ = 0;

        START_LOG( clog, L"캐시백 이벤트 비율로 등록되지 않은 아이템. nVersion: " << nVersion << ", itemID: " << dwItemID_ ) << END_LOG;
    }
    else {
        nCashbackRatio_ = nRate;
    }

    START_LOG( clog, L"캐시백 이벤트 비율 적용확인 완료" )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwItemID_ )
        << BUILD_LOG( nCashbackRatio_ )
        << END_LOG;

    return true;
}

bool KSP::CheckCashbackDefaultRatio( IN const GCITEMID dwItemID_, OUT int& nCashbackRatio_ )
{
    /*
    캐시백 비율(아이템별)
    L"{ call dbo.CBGACashBackRate_select ( %d ) }"

    { call dbo.CBGACashBackRate_select ( @1 ) }
    @1 ; @iItemID_ int

    1 return ( @1 )
    @1 ; Rate       int
    */
    nCashbackRatio_ = 0;

    int nRate = -1;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGACashBackRate_select ( %d ) }", dwItemID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRate );
        m_kODBC.EndFetch();
    }

    if ( -1 == nRate ) {
        nCashbackRatio_ = 0;

        START_LOG( clog, L"캐시백 기본 비율로 등록되지 않은 아이템. itemID: " << dwItemID_ ) << END_LOG;
    }
    else {
        nCashbackRatio_ = nRate;
    }

    START_LOG( clog, L"캐시백 기본 비율 적용확인 완료" )
        << BUILD_LOG( dwItemID_ )
        << BUILD_LOG( nCashbackRatio_ )
        << END_LOG;

    return true;
}

bool KSP::Insert_CashBack_FirstBuyUser_ToDB( IN const DWORD dwUID_, IN const int nVersion_, IN const GCITEMID dwItemID_ )
{
    /*
    캐시백 첫구매 유저 데이터 저장(받았는지 여부 저장)
    L"{ call dbo.CBGACashBackFirstBuyEventUser_insert ( %d, %d, %d ) }"

    { call dbo.CBGACashBackFirstBuyEventUser_insert ( @1, @2, @3 ) }
    @1 : @iLoginUID_     int
    @2 : @iVersion_      int
    @3 : @iItemID_       int

    1 return ( @1 )
    @1: OK
        0       : 성공
        -101이하: 트랜젝션에러
    */
    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGACashBackFirstBuyEventUser_insert ( %d, %d, %d ) }",
        dwUID_, nVersion_, dwItemID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( 0 == nOK )
        << BUILD_LOG( nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwItemID_ )
        << END_LOG;

    return true;
}

bool KSP::Insert_CashBack_EventUser_ToDB( IN const DWORD dwUID_, IN const int nVersion_, IN const GCITEMID dwItemID_, IN const int nRate_ )
{
    /*
    캐시백 이벤트 유저 로그 기록
    L"{ call dbo.CBGACashBackEventUser_insert ( @1, @2, @3, @4 ) }"

    { call dbo.CBGACashBackEventUser_insert ( %d, %d, %d, %d ) }
    @1 : @iLoginUID_     int
    @2 : @iVersion_      int
    @3 : @iItemID_       int
    @4 : @iRate_         int

    1 return ( @1 )
    @1: OK
        0       : 성공
        -101이하: 트랜젝션에러
    */
    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGACashBackEventUser_insert ( @1, @2, @3, @4 ) }",
        dwUID_, nVersion_, dwItemID_, nRate_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( 0 == nOK )
        << BUILD_LOG( nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwItemID_ )
        << BUILD_LOG( nRate_ )
        << END_LOG;

    return true;
}

int KSP::GetCurrentVirtualCash( IN const DWORD dwUID_, IN const std::wstring& strLogin_ )
{
    //{ call dbo.VCGA_virtual_cash_select ( @1 ) }
    //@1 ; @iLoginUID_ int
    //
    //1 return ( @1 )
    //@1 ; VCPoint int ; 리턴값이 없으면 0으로 취급

    int nBalance = 0;
    _DBJIF( m_kODBC.Query( L"{ call dbo.VCGA_virtual_cash_select ( %d ) }", dwUID_ ),
        return 0, (&m_kODBC) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nBalance );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( nBalance >= 0 )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nBalance ) << END_LOG;

    return nBalance;
}

bool KSP::VirtualCashMergeWithType( IN const DWORD dwUserUID_, IN const DWORD dwItemID_, IN const int nType_, IN const int nAddPoint_, IN const int nSubtractPoint_ )
{
    /*
    L"{ call dbo.VCGAVirtualCash_merge ( %d, %d, %d, %d, %d ) }"

    { call dbo.VCGAVirtualCash_merge ( @1, @2, @3, @4, @5 ) }
    @1 ; @iLoginUID_        int
    @2 ; @iItemID_          int
    @3 ; @iType_            int
    @4 ; @iPlusPriceDiff_   int    //0 또는양수값
    @5 ; @iMinusPriceDiff_  int    //0 또는음수값

    1 return ( @1 )
    @1 ; OK int
        0        ; 성공
        -1       ; 유저정보가없음
        -2       ; @iPlusPriceDiff_ 값이음수임.
        -3       ; @iMinusPriceDiff_값이양수임.
        -101이하; 트랜잭션에러
    */

    int nSubtractPoint = 0;
    if ( nSubtractPoint_ > 0 ) {
        nSubtractPoint = -nSubtractPoint_;
    }

    DBJIF2( m_kODBC.Query( L"{ call dbo.VCGAVirtualCash_merge ( %d, %d, %d, %d, %d ) }",
        dwUserUID_, dwItemID_, nType_, nAddPoint_, nSubtractPoint ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"가상 캐시 가산/차감. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( nOK == 0 );
}

int KSP::UpdateMoneyConsumptionStat( IN const DWORD& dwUID_, IN const int& nMoneyType_, 
                                            IN const int& nAmount_, IN const __int64& biWaitItemUID_ )
{
    /*
    L"{ call dbo.MCGAMoneyConsumption_insert_20130619 ( %d, %d, %d, %I64d  ) }"
    { call dbo.MCGAMoneyConsumption_insert_20130619 ( @01, @02, @03, @04 ) }
        @01iLoginUID_input int
    ,   @02iType_input     int
        1: 캐쉬웹구매

        이 구매타입은 서버에서 지정하시면 됩니다
        10000부터 시작하시면 됩니다
        소비 로그이므로 통상적으로 Cash는 10000, GP는 10001 이런 식으로 하셔도 됩니다만
        필요에 따라서 10000은 A경로를 통해서 구매한 캐쉬 아이템, 10001는 B경로를 통해서 구매한 캐쉬 아이템
        으로 하셔도 됩니다
        가능하다면 숫자 그룹을 묶어주시면 좋겠습니다(예:캐쉬는 10000부터 10099까지, GP는 10100부터 10199까지 ..)
        지정된 값을 알려주시면 데이터 제공에 도움이 됩니다
        게임 서버류 외의 다른 인터페이스는 1~9999사이를 기재합니다

    ,   @03iAmount_input   int
    ,   @04iWIGAUID_input  bigint

        모든 아이템은 대기 아이템으로 일단 들어가게 되므로 WIGAUID값을 가질 수 있을 겁니다
        이 값을 기재해주시면 됩니다

        1 return ( @1 )
        @1: OK int
        0       : 성공
        -1      : 유저 정보가 업음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF3( m_kODBC.Query( L"{ call dbo.MCGAMoneyConsumption_insert_20130619 ( %d, %d, %d, %I64d ) }", 
        dwUID_, nMoneyType_, nAmount_, biWaitItemUID_ ), return nOK );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK  );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    return nOK;
}

bool KSP::User_auth_update( IN const DWORD& dwUserUID_, IN const bool& bChatEnable_ )
{
    /*
    { call dbo.UAGTUserAuth_update_20130722 ( %d, %d, %d ) }

    { call dbo.UAGTUserAuth_update_20130722 ( @01, @02, @04 ) }
    @01iLoginUID_input   int
   ,@02iAuthLevel_input  int
       0 : 정상유저
       -4: 채팅 금지
   ,@04iPeriodMinute_input int
    @02iAuthLevel_input가 0인 경우 0
    @02iAuthLevel_input가 -4인 경우 -1 또는 0보다 큰 값

    1 return ( @01 )
    @01iOK int
        0        : 성공
        -1       : 유저 정보 없음
        -2       : 지정된 AuthLevel이 아님(0 또는 -4)
        -3       : 채팅 금지인데 기간이 0이 옴
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    int nChangeType = 0;
    int nAuthLevel = 0;
    if( !bChatEnable_ ) {
        nChangeType = -4;
        nAuthLevel = -1;
    }

    DBJIF2( m_kODBC.Query( L"{ call dbo.UAGTUserAuth_update_20130722 ( %d, %d, %d ) }",
        dwUserUID_, nChangeType, nAuthLevel ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"유저 등급 변경.. UserUID : " << dwUserUID_ )
        << BUILD_LOGc( bChatEnable_ )
        << BUILD_LOG( nChangeType ) << END_LOG;

    return ( nOK == 0 );
}

int KSP::GetTodayEventCount( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_ )
{
    return m_kCommonSP.GetTodayEventCount( dwUID_, nEventType_, tmDate_ );
}

bool KSP::SetTodayEventDoneDiff( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ )
{
    return m_kCommonSP.SetTodayEventDoneDiff( dwUID_, nEventType_, tmDate_, nData_ );
}

bool KSP::SetTodayEventDone( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ )
{
    return m_kCommonSP.SetTodayEventDone( dwUID_, nEventType_, tmDate_, nData_ );
}

bool KSP::GetUserItemReceiveDate( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemDelDateA_select ( %d ) }"

    { call dbo.UIGAUserItemDelDateA_select ( @1 ) }
    @1: @iLoginUID_

    n return ( @1, @2 )
    @1: ItemUID  bigint
    @2: DelDateA tinyint
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemDelDateA_select ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        time_t tmDate;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> tmDate );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 유저의 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_tRegDate = tmDate;
    }

    return true;
}

bool KSP::GetUserAttributeMigrationInfo( IN const DWORD dwUID_, OUT std::map<GCITEMUID, DWORD>& mapAttributeMigrationItemInfo_ )
{
    /*
    L"{ call dbo.UIGAUserItemAttributeResetCount_select ( %d ) }"

    { call dbo.UIGAUserItemAttributeResetCount_select ( @1 ) }
    @1: @iLoginUID_     int

    n return ( @1, @2 )
    @1: ItemUID         bigint
    @2: ResetCount      int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttributeResetCount_select ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        int nResetCount;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> nResetCount );

        if( mapAttributeMigrationItemInfo_.insert( std::make_pair( itemUID, nResetCount ) ).second == false ) {
            START_LOG( cerr, L"중복된 ItemUID : " << itemUID << L", ResetCount : " << nResetCount ) << END_LOG;
        }
    }

    return true;
}

bool KSP::UserItemAttributeResetCountMerge( IN const DWORD dwUserUID_, IN const GCITEMUID dwItemUID_ )
{
    /*
    L"{ call dbo.UIGAUserItemAttributeResetCount_merge ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemAttributeResetCount_merge ( @1, @2, @3 ) }
    @1: @iLoginUID_          int
    @2: @iItemUID_           bigint
    @3: @iResetCount_Diff_   int

    1 return ( @1 )
    @1: OK       int
    0       : 성공
    -1      : 유저가없음.
    -2      : 유저에게아이템없음.(삭제된아이템도포함)
    -801    ; 로그기록실패
    -901    ; 속성재설정횟수갱신실패
    -902    ; 속성재설정횟수등록실패
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttributeResetCount_merge ( %d, %I64d, %d ) }",
        dwUserUID_, dwItemUID_, 1 ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"유저 속성 재설정 값 수정. OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( nOK == 0 );
}

bool KSP::WareHouse_ReceiveDate_select( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemWarehouseDelDateA_select ( %d ) }"  

    { call dbo.UIGAUserItemWarehouseDelDateA_select ( @1 ) }  
    @1: @iLoginUID_  
      
    n return ( @1, @2 )  
    @1: ItemUID  bigint  
    @2: DelDateA tinyint  
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemWarehouseDelDateA_select ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        time_t tmDate;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> tmDate );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"창고에서 기간 세팅중 해당 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_tRegDate = tmDate;
    }

    return true;
}

bool KSP::SetCooriComposeInfo( IN const DWORD& dwUID_, IN const GCITEMUID& AbilityItemUID_, IN const GCITEMUID& DesignItemUID_, IN const GCITEMID& DesignCoordiItemID_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordi2_merge_20111109 ( %d, %I64d, %I64d, %d ) }"

    { call dbo.UIGAUserItemCoordi2_merge_20111109 ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_     int
    @2: @iInnerItemUID_ bigint
    @3: @iOuterItemUID_ bigint
    @4: @iItemID_       int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 없음
    -2      : 내형 아이템 정보가 없음
    -3      : 외형 아이템 정보가 없음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi2_merge_20111109 ( %d, %I64d, %I64d, %d ) }",
        dwUID_,
        AbilityItemUID_,
        DesignItemUID_,
        DesignCoordiItemID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"코디 합성정보 기록 UserUID : " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( AbilityItemUID_ )
        << BUILD_LOG( DesignItemUID_ )
        << BUILD_LOG( DesignCoordiItemID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
};

bool KSP::GetUserItemCooriCompose( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_, OUT std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi_ )
{
    vecDesignCoordi_.clear();
    /*
    L"{ call dbo.UIGAUserItemCoordi2_select_20111109 ( %d ) }"

    { call dbo.UIGAUserItemCoordi2_select_20111109 ( @1 ) }
    @1: @iLoginUID_ int

    n return ( @1, @2 )
    @1: ItemUID bigint
    @2: ItemID  int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi2_select_20111109 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        GCITEMID itemID;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> itemID );
        vecDesignCoordi_.push_back( std::make_pair(itemUID, itemID) );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 유저의 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_DesignCoordiID = itemID;
    }

    return true;
}

bool KSP::WareHouse_ItemCooriCompose( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemCoordi2_warehouse_select_20130904 ( %d ) }"

    { call dbo.UIGAUserItemCoordi2_warehouse_select_20130904 ( @1 ) }
    @1: @iLoginUID_ int

    n return ( @1, @2 )
    @1: ItemUID bigint
    @2: ItemID  int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemCoordi2_warehouse_select_20130904 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        GCITEMID itemID;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> itemID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"창고에서 해당 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_DesignCoordiID = itemID;
    }

    return true;
}

bool KSP::UpdateItemDelState( IN const DWORD dwUID_, IN const GCITEMUID ItemUID_, IN const DWORD dwDelState_ )
{
/*
    L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @04: @03iDelstate_input tinyint
        
    1 return ( @01 )
    @01: OK
        0        : 성공
        -1       : 메인 아이템 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
        -1000    : 사용 중지된 sp인데 사용하려 함
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }", dwUID_, ItemUID_, dwDelState_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"아이템 delstate 업데이트" << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return (nOK == 0);
}

bool KSP::InsertClientNetworkStat( IN const std::wstring& strIP_, IN const int& nID_, IN const int& nCount_ )
{
    /*
    L"{ call dbo.UCSTUDPClientNetworkError_merge ( N'%s', %d, %I64d ) }"

    { call dbo.UCSTUDPClientNetworkError_merge ( @1, @2, @3 ) }
    @1 ; @strIP_        nvarchar( 15 )
    @2 ; @iType_        int
    @3 ; @iData_Diff_   bigint

    1 select ( @1 )
    @1  ; OK int
    0        ; 성공
    -901     ; Data 값 변경 실패
    -902     ; Data 값 등록 실패
    */

    int nOK = -99;
    __int64 biCount = static_cast<__int64>( nCount_ );
    DBJIF2( m_kODBC.Query( L"{ call dbo.UCSTUDPClientNetworkError_merge ( N'%s', %d, %I64d ) }",
        strIP_.c_str(),
        nID_,
        biCount ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"클라이언트 네트워크 통계" )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nID_ )
        << BUILD_LOG( nCount_ )
        << END_LOG;

    return true;
}

bool KSP::DeleteAttribute( IN const DWORD dwUserUID_, IN const GCITEMUID& ItemUID_ )
{
    /*
    아이템 Attribute 정보 삭제

    L"{ call dbo.UIGAUserItemAttribute_delete_20111212 ( %d, %I64d ) }"

    { call dbo.UIGAUserItemAttribute_delete_20111212 ( @1, @2 ) }
    @1: @iLoginUID_ int
    @2: @iItemUID_  bigint

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 없음
    -2      : Attribute 정보가 없음
    -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemAttribute_delete_20111212 ( %d, %I64d ) }",
        dwUserUID_, ItemUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"아이템 속성 삭제. UID : " << dwUserUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( ItemUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( nOK == 0 );
}

bool KSP::GetSellCountByEventUID_SocialCommerce( IN const DWORD& dwEventUID_, IN int& nSellCount_ )
{
/*
    발렌타인 보부상 이벤트별 누적 판매 개수 조회

    L"{ call dbo.SCGASocialCommerceEventSellCount_Select ( %d ) }"

    { call dbo.SCGASocialCommerceEventSellCount_Select ( @1 ) }
    @1 ; @iEventUID_ int

    1 return ( @1 )
    @1 ; SellCount   int    --누적 판매 개수
*/   


    DBJIF2( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventSellCount_Select ( %d ) }",
        dwEventUID_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC 
            >> nSellCount_ );
    }
    m_kODBC.EndFetch();

    return true;
}

void KSP::VirtualDepotItemAttribute_select( IN const DWORD& dwUserUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseAttribute_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseAttribute_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03, @04, @05 )
    @01iItemUID        bigint
    @02iSlotID         tinyint
    @03iTypeID         smallint
    @04iValue          int
    @05iAttributeState tinyint
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseAttribute_select_20131119 ( %d ) }", dwUserUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        GCITEMUID       itemUID;
        KAttributeInfo kAttInfo;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kAttInfo.m_cSlotID
            >> kAttInfo.m_cType
            >> kAttInfo.m_fValue
            >> kAttInfo.m_cState );

        kAttInfo.m_fValue /= 10000.f;

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 속성을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        KItem& kItem = *vit;
        kItem.m_vecAttribute.push_back( kAttInfo );
    }
}

void KSP::VirtualDepotItemSocket_select( IN const DWORD& dwUserUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseSocket_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseSocket_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03, @04 )
    @01iItemUID   bigint
    @02iSlotID    tinyint
    @03iCardID    int
    @04iSockState tinyint
        0: 초기화 필요
        1: 사용할 수 있음
        2: 사용중
        3: 사용할 수 없음

    @04번 왠지 저게 아닌듯하여 여기에 기록합니다
    확인된것만
        2: 비어있는 소켓(이때 CardID는 0임)
        3: 몬스터 카드가 장착된 소켓(이때 CardID는 0이 아님)
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseSocket_select_20131119 ( %d ) }", dwUserUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID itemUID;
        KSocketInfo kSocketInfo;

        VERIFY_QUERY( m_kODBC
            >> itemUID
            >> kSocketInfo.m_cSlotID
            >> kSocketInfo.m_CardItemID
            >> kSocketInfo.m_cState );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 소켓을 가진 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        vit->m_vecSocket.push_back( kSocketInfo );
    }
}

bool KSP::VirtualDepotLevel_select( IN const DWORD& dwUserUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseLevel_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseLevel_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03 )
    @01iItemUID   bigint
    @02iType      int
    @03iItemLevel int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseLevel_select_20131119 ( %d ) }",
        dwUserUID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID ItemUID;
        int nItemType = 0;
        int nItemLevel = 0;

        VERIFY_QUERY( m_kODBC >> ItemUID
            >> nItemType
            >> nItemLevel );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == ItemUID );

        if ( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 UID의 아이템을 찾을 수 없음. ItemUID : " << ItemUID << L", Type : " << nItemType << L", ItemLevel : " << nItemLevel << L", UserUID : " << dwUserUID_ ) << END_LOG;
            continue;
        }

        switch ( nItemType ) {
            case KItemLevelTable::IL_EQUIP_LV:
                vit->m_sEquipLevel = static_cast<short>( nItemLevel );
                break;
            case KItemLevelTable::IL_EQUIP_DOWN_LV:
                vit->m_sEquipLvDown = static_cast<short>( nItemLevel );
                break;
            default:
                START_LOG( cerr, L"정의되지 않은 아이템레벨 타입.. ItemType : " << nItemType ) << END_LOG;
                break;
        }
        START_LOG( clog, L"ItemType : " << nItemType ) << END_LOG;
    }

    return true;
}

void KSP::VirtualDepotStrengthInfo_select( IN const DWORD& dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseStrength_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseStrength_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03 )
    @01iItemUID         bigint
    @02iStrengthLevel   int
    @03iEquippedItemUID bigint
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseStrength_select_20131119 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID StrengthitemUID;
        char cStrongLevel;
        GCITEMUID EquipitemUID;

        VERIFY_QUERY( m_kODBC
            >> StrengthitemUID
            >> cStrongLevel
            >> EquipitemUID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == StrengthitemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 강화석 아이템을 찾을 수 없습니다. ItemUID : "<< StrengthitemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }

        vit->m_cEnchantLevel = cStrongLevel;
    }
}

void KSP::VirtualDepotItemReceiveDate_select( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseDelDateA_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseDelDateA_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02 )
    @01iItemUID    bigint
    @02sdtDelDateA datetime
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseDelDateA_select_20131119 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        time_t tmDate;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> tmDate );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 기간 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_tRegDate = tmDate;
    }
}

void KSP::VirtualDepotItemCooriCompose_select( IN const DWORD dwUID_, IN OUT std::vector<KItem>& vecItem_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseCoordi2_select_20131119 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouseCoordi2_select_20131119 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02 )
    @01iItemUID bigint
    @02iItemID  int
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseCoordi2_select_20131119 ( %d ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {

        GCITEMUID itemUID;
        GCITEMID itemID;
        VERIFY_QUERY( m_kODBC 
            >> itemUID
            >> itemID );

        std::vector<KItem>::iterator vit = std::find_if( vecItem_.begin(), vecItem_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == itemUID );

        if( vit == vecItem_.end() ) {
            START_LOG( cwarn, L"해당 코디 합성 아이템을 찾을 수 없습니다. ItemUID : "<< itemUID << L", UserUID : " << dwUID_ ) << END_LOG;
            continue;
        }
        vit->m_DesignCoordiID = itemID;
    }
}

bool KSP::CharNickToLoginUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ )
{
    // 캐릭터 닉네임으로 동작한다

    /*
    L"{ call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( N'%s' ) }"

    { call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( @01 ) }
        @01strNickname_input nvarchar( 24 )

    1 return ( @01, @02 )
        @01iCharType int
        @02iLoginUID int
    */

    dwLoginUID_ = 0;
	//___JIF( !strCharNick_.empty(), return false );
	if (strCharNick_.empty())
		return false;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( N'%s' ) }",
        strCharNick_.c_str() ) );

    int nLoginUID = -1;
    nCharType_ = -1;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nCharType_ >> nLoginUID );
    }        
    m_kODBC.EndFetch();

    if ( -1 == nLoginUID ) {
        START_LOG( clog, L"대상 캐릭터가 존재하지 않음 name : " << strCharNick_ ) << END_LOG;

        return false;
    }

    dwLoginUID_ = static_cast< DWORD >( nLoginUID );

    START_LOG( clog, L"캐릭터 닉네임 조회 성공" )
        << BUILD_LOG( strCharNick_ )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( nCharType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::LoginUIDToLogin( IN const DWORD dwLoginUID_, OUT std::wstring& strCharNick_ )
{
    /*
    L"{ call dbo.ULWBUser_select ( %d ) }"

    { call dbo.ULWBUser_select ( @1 ) }
    @1 ; @iLoginUID_    int

    1 return ( @1 )
    @1 ; Login         nvarchar(80)
    */

    strCharNick_.clear();
    DBJIF2( m_kODBC.Query( L"{ call dbo.ULWBUser_select ( %d ) }", dwLoginUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strCharNick_ );

        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"로그인 조회 성공" )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOG( strCharNick_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::InsertInvenDepotMoveLog( IN const KInvenDepotMoveLog& kInvenDepotMoveLog_ )
{
    /*
    L"{ call dbo.IWGLInvenWarehouseItemMoving_insert_20131204 ( %d, %d, %d, %I64d, %d, %d, %I64d, %d, %d, %d ) }"

    { call dbo.IWGLInvenWarehouseItemMoving_insert_20131204 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10 ) }
        @01iLoginUID_input      int
    ,   @02iCharType_input      int
    ,   @03iInputType_input     int
    ,   @04iItemUIDFrom_input   bigint
    ,   @05iItemIDFrom_input    int
    ,   @06iItemCNTFrom_input   int
    ,   @07iItemUIDTo_input     bigint
    ,   @08iItemIDTo_input      int
    ,   @09iItemCNTTo_input     int
    ,   @10iItemCNTMoving_input int

    1 select ( @01 )
    @01iOK int
        0        : 성공
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
        -1000    : 사용 중지된 sp인데 사용하려 함
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.IWGLInvenWarehouseItemMoving_insert_20131204 ( %d, %d, %d, %I64d, %d, %d, %I64d, %d, %d, %d ) }",
        kInvenDepotMoveLog_.m_dwUID,
        kInvenDepotMoveLog_.m_cCharType,
        kInvenDepotMoveLog_.m_nType,
        kInvenDepotMoveLog_.m_ItemUIDFrom,
        kInvenDepotMoveLog_.m_ItemIDFrom,
        kInvenDepotMoveLog_.m_nCountFrom,
        kInvenDepotMoveLog_.m_ItemUIDTo,
        kInvenDepotMoveLog_.m_ItemIDTo,
        kInvenDepotMoveLog_.m_nCountTo,
        kInvenDepotMoveLog_.m_nRequestCount ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"유저 공용창고 수량 아이템 통계" )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

bool KSP::IgnorableErrorProcessingAfterCallSP( IN OUT int& nReturnValue_ )
{
    // -801~-899: 무시가능 에러
    if ( -899 <= nReturnValue_ && -801 >= nReturnValue_ ) {
        nReturnValue_ = 0;

        return false;
    }

    return true;
}

bool KSP::InsertCharMissionInfo( IN const std::map< std::pair<char,char>,std::vector<DWORD> > mapCharMissionList_, IN const int nType_ )
{
    /*
    L"{ call dbo.MCGAMissionControl_insert ( %d, %d, %d, %d ) }"

    { call dbo.MCGAMissionControl_insert ( @1, @2, @3, @4 ) }
    @1: @iMissionID_    int
    @2: @iCharType_     int
    @3: @iLevel_        int    
    @4: @iType_         int

    1 return ( @1 )
    @1: OK  int
        0        : 성공
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    std::map<std::pair<char,char>,std::vector<DWORD>>::const_iterator cmit;
    std::vector<DWORD>::const_iterator cvit;
    for( cmit = mapCharMissionList_.begin(); cmit != mapCharMissionList_.end(); ++cmit ) {
        for( cvit = cmit->second.begin(); cvit != cmit->second.end(); ++cvit ) {
            DBJIF2( m_kODBC.Query( L"{ call dbo.MCGAMissionControl_insert ( %d, %d, %d, %d ) }",
                *cvit,
                cmit->first.first,
                cmit->first.second,
                nType_ ) );

            int nRet = -99;
            if ( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nRet );
                m_kODBC.EndFetch();
            }

            LIF( IgnorableErrorProcessingAfterCallSP( nRet ) );

            _LOG_SUCCESS( nRet == 0, L"전용던전 미션 정보 DB기록.. Result : " << nRet )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    return true;
}