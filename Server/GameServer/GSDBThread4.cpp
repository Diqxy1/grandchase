#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include <iomanip> // std::setfill
#include "Socket/NetCommon.h"
#include "NetError.h"
#include "support_User.h"
#include "DepotManager.h"
#include "ItemManager.h"

#define CLASS_TYPE KGSDBThread

_INT_IMPL_ON_FUNC( EVENT_QUERY_USER_INFO2_REQ, KPacket2Name )
{
    KEVENT_QUERY_USER_INFO2_ACK kPacket;
    kPacket.m_cOK = -99;
    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        kPacket.m_kUserInfo.m_bIsNickname = true;
        kPacket.m_kUserInfo.m_strNickName = kPacket_.m_strName;

        DBJIF(m_kODBC.Query(L"{ call dbo.up_query_user_nick_color ( N'%s' ) }", kPacket_.m_strName));
        if (m_kODBC.BeginFetch())
        {
            VERIFY_QUERY(m_kODBC >> kPacket.m_kUserInfo.m_strNickColor);
            m_kODBC.EndFetch();
        }
        //Login을 얻어온다.
        if( TranslateNicknameToLogin( kPacket.m_kUserInfo.m_strNickName, kPacket.m_kUserInfo.m_strName ) != 0 )
        {
            START_LOG( cwarn, L"닉네임으로 로그인 정보 얻어오기 실패 : " << kPacket.m_kUserInfo.m_strNickName ) << END_LOG;
            goto quit;
        }
        //DB에서 받아온 nickname을 얻어온다.
        if( GetNickName( kPacket.m_kUserInfo.m_strName, kPacket.m_kUserInfo.m_strNickName ) != 0 )
        {
            START_LOG( cwarn, L"로그인으로 닉네임 정보 얻어오기 실패 : " << kPacket.m_kUserInfo.m_strName ) << END_LOG;
            goto quit;
        }
        //DB에서 유저UID를 구한다.
        if( NickToUID( kPacket.m_kUserInfo.m_strNickName, kPacket.m_kUserInfo.m_dwUID ) == false )
        {
            START_LOG( cwarn, L"유저UID 얻어오기 실패 : " << kPacket_.m_strName ) << END_LOG;
            goto quit;
        }
        //DB에서 받아온 ID를 넣어둔다.
        kPacket_.m_strName = kPacket.m_kUserInfo.m_strName;
    }
    else
    {
        kPacket.m_kUserInfo.m_bIsNickname = false;
        kPacket.m_kUserInfo.m_strName = kPacket_.m_strName;

        DBJIF(m_kODBC.Query(L"{ call dbo.up_query_user_nick_color ( N'%s' ) }", kPacket_.m_strName));
        if (m_kODBC.BeginFetch())
        {
            VERIFY_QUERY(m_kODBC >> kPacket.m_kUserInfo.m_strNickColor);
            m_kODBC.EndFetch();
        }

        //DB에서 받아온 nickname을 얻어온다.
        if( GetNickName( kPacket.m_kUserInfo.m_strName, kPacket.m_kUserInfo.m_strNickName ) != 0 )
        {
            START_LOG( cwarn, L"로그인으로 닉네임 정보 얻어오기 실패 : " << kPacket.m_kUserInfo.m_strName ) << END_LOG;
            goto quit;
        }
        //DB에서 받아온 Login을 얻어온다.
        if( TranslateNicknameToLogin( kPacket.m_kUserInfo.m_strNickName, kPacket.m_kUserInfo.m_strName ) != 0 )
        {
            START_LOG( cwarn, L"닉네임으로 로그인 정보 얻어오기 실패 : " << kPacket.m_kUserInfo.m_strNickName ) << END_LOG;
            goto quit;
        }
        //DB에서 유저UID를 구한다.
        if( LoginToUID( kPacket.m_kUserInfo.m_strName , kPacket.m_kUserInfo.m_dwUID ) == false )
        {
            START_LOG( cwarn, L"유저UID 얻어오기 실패 : " << kPacket.m_kUserInfo.m_strName ) << END_LOG;
            goto quit;
        }
    }
    /*
    call dbo.MTM_manager_tool_user_info_query
    ( Login nvarchar(20) )

    return 1 loop select
    ( OK int )
    ( GamePoint int )
    ( AuthLevel int )
    ( Status int )

    OK
    0 : 리턴 성공, 다음 데이터를 신뢰해도 됨
    -2 : 해당 유저가 존재하지 않음, 다음 데이터를 신뢰하지 마삼
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.MTM_manager_tool_user_info_query ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;
        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );

        if( kPacket.m_cOK == 0 )
        {
            kPacket.m_kUserInfo.m_mapCharacter.clear();

            VERIFY_QUERY( m_kODBC
                >> kPacket.m_kUserInfo.m_iGP
                >> kPacket.m_kUserInfo.m_cAuthLevel
                >> kPacket.m_kUserInfo.m_cWarning );

            START_LOG( clog, L"사용자 정보 얻어오기" )
                << BUILD_LOGc( kPacket.m_cOK )
                << BUILD_LOG( kPacket.m_kUserInfo.m_iGP )
                << BUILD_LOGc( kPacket.m_kUserInfo.m_cAuthLevel)
                << BUILD_LOGc( kPacket.m_kUserInfo.m_cWarning ) << END_LOG;
        }
        else if( kPacket.m_cOK == -2 )
        {
            START_LOG( cerr, L"해당 유저가 존재하지 않음" )
                << BUILD_LOGc( kPacket.m_cOK )
                << BUILD_LOG( kPacket_.m_strName ) << END_LOG;
        }
        else
        {
            START_LOG( cerr, L"알 수 없는 오류" )
                << BUILD_LOGc( kPacket.m_cOK )
                << BUILD_LOG( kPacket_.m_strName )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }

        m_kODBC.EndFetch();
    }

    /*
    call dbo.MTM_manager_tool_character_info_query
    ( Login nvarchar(20) )

    return n loop select
    ( CharType tinyint )
    ( Promotion tinyint )
    ( [Exp] bigint )
    ( Level int )
    ( Win int )
    ( Lose int )
    */
    if( kPacket.m_cOK == 0 )
    {
        int nCharCount = 0;

        DBJIF( m_kODBC.Query( L"{ call dbo.MTM_manager_tool_character_info_query ( N'%s' ) }",
            kPacket_.m_strName.c_str() ) );

        while( m_kODBC.Fetch() != SQL_NO_DATA )
        {
            KCharacterInfo kCharTemp;
            kCharTemp.m_strCharName = kPacket_.m_strName.c_str();
            kCharTemp.m_vecEquipItems.clear();

            VERIFY_QUERY( m_kODBC
                >> kCharTemp.m_cCharType
                >> kCharTemp.m_cPromotion
                >> kCharTemp.m_biExp
                >> kCharTemp.m_dwLevel
                >> kCharTemp.m_iWin
                >> kCharTemp.m_iLose );

            kPacket.m_kUserInfo.m_mapCharacter[ kCharTemp.m_cCharType ] = kCharTemp;

            START_LOG( clog, L"      CharName : " <<  kCharTemp.m_strCharName
                << L", Promotion : " << static_cast<int>(kCharTemp.m_cPromotion)
                << L", Exp : " << kCharTemp.m_biExp
                << L", Level : " << kCharTemp.m_dwLevel ) << END_LOG;
        }

        m_kODBC.EndFetch();
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_USER_INFO2_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_REMOVE_USER_INFO_REQ, KPacket2Name )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_drop_user_info ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    if( m_kODBC.BeginFetch() ) 
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_REMOVE_USER_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_INSERT_P2PFAILRATE_DATA, std::vector<int> )
{
    KLocker lock( m_kODBC.GetCS() );

    /*
    L"{ call dbo.PFGAStatP2PFailRate_merge ( N'%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.PFGAStatP2PFailRate_merge ( @1, @2 , @3 , @4 , .. , @99 , @100 , @101 ) }
    @1  : @nServerIP_ nvarchar(16)
    @2  : @icount_00_ int
    @3  : @icount_01_ int
    @4  : @icount_02_ int
    @5  : @icount_03_ int
    ..
    @99 : @icount_97_ int
    @100 : @icount_98_ int
    @101 : @icount_99_ int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션에러
    */

    std::wstringstream stmQuery;

    stmQuery << L"{ call dbo.PFGAStatP2PFailRate_merge ( ";
    stmQuery << L" N'" << NetCommon::GetLocalIP() << L"'";
    std::vector<int>::iterator vit;
    for( vit = kPacket_.begin(); vit != kPacket_.end(); vit++ ) {
        stmQuery << L", " << *vit;
    }
    stmQuery << L" ) }";

    DBJIF( m_kODBC.QueryToWString( stmQuery.str() ) );

    if( m_kODBC.BeginFetch() ) {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    stmQuery.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmQuery.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}


_INT_IMPL_ON_FUNC( DB_EVENT_INSERT_CLIENTERR_DATA, std::vector<int> )
{
    /* 
    L"{ call dbo.SCGAStatClientErrReport_merge ( N'%s', %d, %d, ... , %d ) }" 

    { call dbo.SCGAStatClientErrReport_merge ( @1, @2 , @3 , ... , @101 ) } 
    @1 : @nServerIP_ nvarchar(16) 
    @2 : @icount_00_ int 
    ...(00 부터99 까지100개있습니다) 
    @101 : @icount_99_ int 

    1 return ( @1 ) 
    @1: OK int 
    0 : 성공 
    -101이하: 트랜잭션에러 
    */
    std::wstringstream stmQuery;

    stmQuery << L"{ call dbo.SCGAStatClientErrReport_merge ( ";
    stmQuery << L" N'" << NetCommon::GetLocalIP() << L"'";
    std::vector<int>::iterator vit;
    for( vit = kPacket_.begin(); vit != kPacket_.end(); vit++ ) {
        stmQuery << L", " << *vit;
    }
    stmQuery << L" ) }";

    DBJIF( m_kODBC.QueryToWString( stmQuery.str() ) );

    if( m_kODBC.BeginFetch() ) {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    stmQuery.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmQuery.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

INT_IMPL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_USE_CHANGE_NICKNAME_REQ );

    KEVENT_USE_CHANGE_NICKNAME_REQ kPacket;
    kPacket.m_nOK               = -99;
    kPacket.m_strNewNickName    = L"";
    kPacket.m_ItemID = kPacket_.m_ItemID;
    kPacket.m_ItemUID = kPacket_.m_ItemUID;
    kPacket.m_bUseItem = kPacket_.m_bUseItem;

    int nRet = -99;

    if ( kPacket_.m_bUseItem ) {
        nRet = m_kHelper.UpdateUserNickname( dwUID_, kPacket_.m_strOldNickName, kPacket_.m_strNewNickName, kPacket_.m_ItemUID );
    }
    else {
        // 닉네임변경카드를 사용하지 않는 SP를 콜한다.
        nRet = m_kHelper.UpdateUserNicknameNoItem( dwUID_, kPacket_.m_strOldNickName, kPacket_.m_strNewNickName );
    }

    if ( nRet != 0 ) {
        switch ( nRet )
        {
        case -1: SET_ERR_GOTO( ERR_NICK_04, END_PROC ); // 아이디와 기존 닉네임으로 해당 유저를 찾을 수 없음.
        case -2: SET_ERR_GOTO( ERR_NICK_05, END_PROC ); // 이미 사용중인 닉네임 입니다.
        case -3: SET_ERR_GOTO( ERR_NICK_06, END_PROC ); // 닉네임변경 아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음.
        default: SET_ERR_GOTO( ERR_NICK_07, END_PROC ); // DB처리 실패
        }
    }

    kPacket.m_strNewNickName = kPacket_.m_strNewNickName;
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_USE_CHANGE_NICKNAME_ACK );

    _LOG_CONDITION( NetError::NET_OK == NetError::GetLastNetError(), clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_strNewNickName )
        << BUILD_LOG( kPacket_.m_strOldNickName )
        << BUILD_LOG( kPacket_.m_strNewNickName )
        << BUILD_LOG( kPacket_.m_bUseItem ) << END_LOG;
}

INT_IMPL_ON_FUNC(EVENT_USE_CHANGE_NICKCOLOR_REQ)
{
    SET_ERROR(ERR_UNKNOWN);
    EVENT_CHECK_OUT(EVENT_USE_CHANGE_NICKCOLOR_REQ);

    KEVENT_USE_CHANGE_NICKCOLOR_REQ kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_strNewNickColor = L"";
    kPacket.m_ItemID = kPacket_.m_ItemID;
    kPacket.m_ItemUID = kPacket_.m_ItemUID;
    kPacket.m_bUseItem = false;

    int nRet = m_kHelper.UpdateUserNickcolorNoItem(dwUID_, kPacket_.m_strNewNickColor);

    if (nRet != 0) {
        switch (nRet)
        {
        case -1: SET_ERR_GOTO(ERR_NICK_04, END_PROC); // 아이디와 기존 닉네임으로 해당 유저를 찾을 수 없음.
        case -2: SET_ERR_GOTO(ERR_NICK_05, END_PROC); // 이미 사용중인 닉네임 입니다.
        case -3: SET_ERR_GOTO(ERR_NICK_06, END_PROC); // 닉네임변경 아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음.
        default: SET_ERR_GOTO(ERR_NICK_07, END_PROC); // DB처리 실패
        }
    }

    kPacket.m_strNewNickColor = kPacket_.m_strNewNickColor;
    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER(EVENT_USE_CHANGE_NICKNAME_ACK);

    _LOG_CONDITION(NetError::NET_OK == NetError::GetLastNetError(), clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket.m_strNewNickColor)
        << BUILD_LOG(kPacket_.m_strNewNickColor)
        << BUILD_LOG(kPacket_.m_bUseItem) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_DEPOT_CHAR_TAB_INFO_REQ );
    KEVENT_DEPOT_CHAR_TAB_INFO_ACK kPacket;

    std::map< char, KDepotInfo >::iterator mit;
    
    for ( mit = kPacket_.begin(); mit != kPacket_.end() ; ++mit ) {
        m_kHelper.UserStorage_select_by_char_tab( dwUID_, mit->second );

        if ( mit->second.m_nSlotSize <= 0 ) { // 창고가 없는 경우 생성하는 코드
            KDepotInfo depotInfo;

            depotInfo.m_cTabID = SiKDepotManager()->GetInitTabID();
            depotInfo.m_nSlotSize = SiKDepotManager()->GetInitSlotSize();
            depotInfo.m_bDBSelectItem = true;

            m_kHelper.UserStorage_merge( dwUID_, depotInfo );

            kPacket.m_mapDepotInfo[depotInfo.m_cTabID] = depotInfo;

        }
        else {
            GetDepotItemListFromDB( dwUID_, mit->second, kPacket.m_vecItem, kPacket.m_mapStrengthItem );
            mit->second.m_bDBSelectItem = true;
            kPacket.m_mapDepotInfo[mit->first] = mit->second;

            m_kHelper.WareHouse_Pet_select( dwUID_, mit->second, kPacket.m_mapPetInfo );
            GetPetExp_Depot( dwUID_, mit->second, kPacket.m_mapPetInfo );
        }
        
    }

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER_UID( EVENT_DEPOT_CHAR_TAB_INFO_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_DEPOT_CREATE_REQ, KDepotInfo )
{
    EVENT_CHECK_OUT( EVENT_DEPOT_CREATE_REQ );

    KEVENT_DEPOT_CHAR_TAB_INFO_ACK kPacket;
    m_kHelper.UserStorage_select_by_char_tab( dwUID_, kPacket_ );

    if ( kPacket_.m_nSlotSize <= 0 ) { // 창고가 없는 경우 생성하는 코드
        KDepotInfo depotInfo;

        depotInfo.m_cTabID = SiKDepotManager()->GetInitTabID();
        depotInfo.m_nSlotSize = SiKDepotManager()->GetInitSlotSize();
        depotInfo.m_bDBSelectItem = true;

        if ( false == m_kHelper.UserStorage_merge( dwUID_, depotInfo ) ) {
            SET_ERR_GOTO( ERR_DEPOT_14, END_PROC );
        }

        kPacket.m_mapDepotInfo[depotInfo.m_cTabID] = depotInfo;
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER_UID( EVENT_DEPOT_CREATE_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_DEPOT_INFO_REQ )
{
    EVENT_CHECK_OUT( EVENT_DEPOT_INFO_REQ );

    KEVENT_DEPOT_INFO_ACK kPacket;

    m_kHelper.UserStorage_select( dwUID_, kPacket.m_mapDepotInfo );

    QUEUING_ACK_TO_USER_UID( EVENT_DEPOT_INFO_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_DEPOT_DATA_REQ, KDepotUpdateData )
{
    EVENT_CHECK_OUT( DB_EVENT_UPDATE_DEPOT_DATA_REQ );

    if ( !kPacket_.m_vecInvenTypeItemInv.empty() ) { // 영구아이템, 인벤토리-> 창고로 옮기는 경우
        LIF( UpdateInvenTypeItem( dwUID_, kPacket_.m_vecInvenTypeItemInv, std::vector<KItem>(), cCharType_ ) ); // 아이템 인벤토리/창고  state 변경.
        LIF( MergeDepotItem( dwUID_, kPacket_.m_vecInvenTypeItemInv ) );
        LIF( DeleteUserItemCharacter( dwUID_, kPacket_.m_vecInvenTypeItemInv, cCharType_ ) );
    } 

    if( !kPacket_.m_vecInvenCountItem.empty() ) {
        // KItem::m_bReserveNewUID = true 인 아이템이 있다면, DB 로부터 UID를 발급받자.(수량성 아이템 중 일부 수량을 창고로 넣은 경우)
        std::vector<KItem> vecNewUIDItem;
        // 함수 타입 추가 필요. 이쪽은 창고에서 새로 생긴 수량 아이템 집어 넣는 부분
        LIF( 0 == CheckReserveNewUID( dwUID_, cCharType_, kPacket_.m_vecInvenCountItem, KItemManager::EWIT_INVEN_TO_DEPOT, vecNewUIDItem ) );
        LIF( vecNewUIDItem.size() == SetNewUID( vecNewUIDItem, kPacket_.m_vecInvenCountItem ) );

        LIF( UpdateInvenTypeItem( dwUID_, kPacket_.m_vecInvenCountItem, std::vector<KItem>(), cCharType_ ) );
        // 아래 함수 호출 전에 item uid 를 받아와야 함. 
        
        LIF( MergeDepotItem( dwUID_, kPacket_.m_vecInvenCountItem ) );

        UpdateCountItems( dwUID_, kPacket_.m_vecInvenCountItem );
        DeleteDepotZeroCountItem( dwUID_, cCharType_, kPacket_.m_vecInvenCountItem );

        LIF( DeleteUserItemCharacter( dwUID_, kPacket_.m_vecInvenCountItem, cCharType_ ) );
    }

    if( !kPacket_.m_vecDeleteDepotItem.empty() ) {
        LIF( DeleteDepotItem( dwUID_, kPacket_.m_vecDeleteDepotItem ) );
        LIF( InsertUserItemCharacter( dwUID_, kPacket_.m_vecDeleteDepotItem, cCharType_ ) );
        LIF( UpdateInvenTypeItem( dwUID_, std::vector<KItem>(), kPacket_.m_vecDeleteDepotItem, cCharType_ ) );
    }

    if ( !kPacket_.m_vecDeleteDepotCountItem.empty() ) {
        std::vector<KItem> vecNewUIDItem;
        // 함수 타입 추가 필요. 이쪽은 인벤에서 새로 생긴 수량 아이템 집어 넣는 부분
        LIF( 0 == CheckReserveNewUID( dwUID_, cCharType_, kPacket_.m_vecDeleteDepotCountItem, KItemManager::EWIT_DEPOT_TO_INVEN, vecNewUIDItem ) );
        LIF( vecNewUIDItem.size() == SetNewUID( vecNewUIDItem, kPacket_.m_vecDeleteDepotCountItem ) );

        DeleteDepotCountItem( dwUID_, cCharType_, kPacket_.m_vecDeleteDepotCountItem );

        LIF( InsertUserItemCharacter( dwUID_, kPacket_.m_vecDeleteDepotCountItem, cCharType_ ) );
        LIF( UpdateInvenTypeItem( dwUID_, std::vector<KItem>(), kPacket_.m_vecDeleteDepotCountItem, cCharType_ ) );
    }

    if ( kPacket_.m_bAckToUser == true ) {
        _QUEUING_ACK_TO_USER( EVENT_UPDATE_DEPOT_DATA_ACK, kPacket_ );
    }
}

_INT_IMPL_ON_FUNC( DB_EVENT_DELETE_DEPOT_DATA_REQ, KDepotUpdateData )
{
    EVENT_CHECK_OUT( DB_EVENT_DELETE_DEPOT_DATA_REQ );

    if( !kPacket_.m_vecDeleteDepotItem.empty() ) {
        LIF( DeleteDepotItem( dwUID_, kPacket_.m_vecDeleteDepotItem ) );
    }

}

_INT_IMPL_ON_FUNC( EVENT_DEPOT_EXTEND_REQ, KDepotExtend )
{
    EVENT_CHECK_OUT( EVENT_DEPOT_EXTEND_REQ );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_DEPOT_EXTEND_ACK kPacket;

    if ( false == m_kHelper.UserStorage_merge( dwUID_, kPacket_.m_kDepot ) ) {
       SET_ERR_GOTO( ERR_DEPOT_14, END_PROC );
    }
    
    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"창고 확장 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, 1 ) ) {
       SET_ERR_GOTO( ERR_DEPOT_12, END_PROC );
    }
    kPacket_.m_kItem.m_nCount -= 1;
    kPacket_.m_kItem.m_nCount = std::max<int>( kPacket_.m_kItem.m_nCount, 0 );

    kPacket.m_kUseItem = kPacket_.m_kItem;
    kPacket.m_kDepot = kPacket_.m_kDepot;
    kPacket.m_nExtendType = kPacket_.m_nExtendType;

    SET_ERROR( NET_OK );
    
END_PROC:
    if ( IS_CORRECT( ERR_DEPOT_12 ) ) {
        kPacket.m_kUseItem.m_nCount = 0;
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER_UID( EVENT_DEPOT_EXTEND_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_GP_ATTRIBUTE_RANDOM_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_GP_ATTRIBUTE_RANDOM_REQ );
    KEVENT_GP_ATTRIBUTE_RANDOM_ACK kPacket;    
    kPacket.m_nOK = -99;
    std::list<std::pair<int,float>>::iterator litAtt;

    KItem kItem = kPacket_.m_kItem;
    kItem.m_vecAttribute.clear();
    KItem kUseItem = kPacket_.m_kUseItem;

    // 아이템 랜덤 속성 적용.
    bool bUpdateSuccess = true;
    int i;
    for( i = 0, litAtt = kPacket_.m_listAttributeList.begin() ; litAtt != kPacket_.m_listAttributeList.end() ; ++litAtt, ++i )
    {
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateAttribute(dwUID_, i, litAtt->first, litAtt->second, kItem, cCharType_ ) );
    }

    if( bUpdateSuccess == false ) {
        kPacket.m_nOK = NetError::ERR_RANDOM_ATTRIBUTE_SELECT_03;
    }
    else {
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kItem = kItem;
        kPacket.m_ItemID = kPacket_.m_ItemID;
        kPacket.m_bInitItem = kPacket_.m_bInitItem;
        kPacket.m_kUseItem = kPacket_.m_kUseItem;

        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket.m_kUseItem.m_ItemUID, kPacket.m_kUseItem.m_nInitCount, kPacket.m_kUseItem.m_nCount ) ) {
            START_LOG( clog, L"GP랜덤속성 주문서 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket.m_kUseItem.m_ItemUID )
                << BUILD_LOG( kPacket.m_kUseItem.m_nInitCount )
                << BUILD_LOG( kPacket.m_kUseItem.m_nCount )
                << END_LOG;
        }

        if ( UseCountItem( dwUID_, kUseItem.m_ItemUID, 1 ) ) { // 성공
            kPacket.m_kUseItem.m_nCount -= 1;
            kPacket.m_kUseItem.m_nCount = std::max<int>( kPacket.m_kUseItem.m_nCount, 0 );
        }
        else { // 실패
            kPacket.m_kUseItem.m_nCount = 0;
            kPacket.m_nOK = NetError::ERR_RANDOM_ATTRIBUTE_SELECT_05;            
        }
    }

    QUEUING_ACK_TO_USER( EVENT_GP_ATTRIBUTE_RANDOM_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ )
{
    KEVENT_CASHBACK_EXTRA_RATIO_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    int nCashbackRatio = 0;
    int nVersion = 0;

    SET_ERROR( ERR_UNKNOWN );

    // 캐시백 첫 구매 비율 조회
    m_kHelper.CheckCashbackFirstBuyRatio( dwUID_, nVersion, nCashbackRatio );

    kPacket.m_nVersion = nVersion;
    kPacket.m_nFirstBuyRatio = nCashbackRatio;

    SET_ERROR( NET_OK );

//END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket.m_nFirstBuyRatio )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CASHBACK_EXTRA_RATIO_INFO_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ );

    KEVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecGetRewards.clear();
    kPacket.m_nAccPlayTime = 0;
    kPacket.m_vecNextGift.clear();

    SET_ERROR( ERR_UNKNOWN );

    // 선물 받을시 접속 선물상자 유저 데이터 기록
    if ( false == Update_ConnectionGiftBox_EventUserData_ToDB( dwUID_, static_cast< int >( cCharType_ ), kPacket_.m_nVersion, kPacket_.m_nCurrentGrade ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_03, END_PROC );
    }

    // 신규유저 대상 선물상자 보상을 지급한다
    if ( false == kPacket_.m_vecRewards.empty() ) {
        std::vector< KDropItemInfo >::iterator vitRewards;
        for ( vitRewards = kPacket_.m_vecRewards.begin(); vitRewards != kPacket_.m_vecRewards.end(); ++vitRewards ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
                cCharType_,
                vitRewards->m_ItemID,
                vitRewards->m_nDuration,
                vitRewards->m_nPeriod,
                KItemManager::EWIT_CONNECTION_GIFTBOX_NEW_USER_REWARD,
                kPacket.m_vecGetRewards ) ) {
                    START_LOG( cerr, L"신규유저 접속 선물상자 시스템 보상 아이템 지급 실패 UID : " << dwUID_ )
                        << BUILD_LOG( vitRewards->m_ItemID )
                        << BUILD_LOG( vitRewards->m_nDuration )
                        << BUILD_LOG( vitRewards->m_nPeriod )
                        << END_LOG;
                    continue;
            }
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecGetRewards.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ );

    KEVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecGetRewards.clear();
    kPacket.m_nAccPlayTime = 0;
    kPacket.m_vecNextGift.clear();


    SET_ERROR( ERR_UNKNOWN );

    // 선물 받을시 접속 선물상자 유저 데이터 기록
    if ( false == Update_ConnectionGiftBox_EventUserData_ToDB( dwUID_, static_cast< int >( cCharType_ ), kPacket_.m_nVersion, kPacket_.m_nCurrentGrade ) ) {
        SET_ERR_GOTO( ERR_CONNECTION_GIFTBOX_ERROR_03, END_PROC );
    }

    // 이벤트유저 대상 선물상자 보상을 지급한다
    if ( false == kPacket_.m_vecRewards.empty() ) {
        std::vector< KDropItemInfo >::iterator vitRewards;
        for ( vitRewards = kPacket_.m_vecRewards.begin(); vitRewards != kPacket_.m_vecRewards.end(); ++vitRewards ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_,
                cCharType_,
                vitRewards->m_ItemID,
                vitRewards->m_nDuration,
                vitRewards->m_nPeriod,
                KItemManager::EWIT_CONNECTION_GIFTBOX_EVENT_USER_REWARD,
                kPacket.m_vecGetRewards ) ) {
                    START_LOG( cerr, L"이벤트유저 접속 선물상자 시스템 보상 아이템 지급 실패 UID : " << dwUID_ )
                        << BUILD_LOG( vitRewards->m_ItemID )
                        << BUILD_LOG( vitRewards->m_nDuration )
                        << BUILD_LOG( vitRewards->m_nPeriod )
                        << END_LOG;
                    continue;
            }
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecGetRewards.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_INFINITY_DUNGEON_REWARD_ITEM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwRecvUserUID = kPacket_.m_dwRecvUserUID;
    kPacket.m_vecDropItem.clear();
    kPacket.m_mapUserDice = kPacket_.m_mapUserDice;

    if ( false == m_kHelper.InsertSingleItemJob( kPacket_.m_dwRecvUserUID, kPacket_.m_cRecvUserCharType, kPacket_.m_kDropItem.m_ItemID, kPacket_.m_kDropItem.m_nDuration, kPacket_.m_kDropItem.m_nPeriod, KItemManager::EWIT_INFINITY_DUNGEON_DROP, kPacket.m_vecDropItem ) ) {
        START_LOG( cerr, L"무한던전 아이템 획득 DB작업 실패. UserUID : " << kPacket_.m_dwRecvUserUID )
            << BUILD_LOGc( kPacket_.m_cRecvUserCharType )
            << BUILD_LOG( kPacket_.m_kDropItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kDropItem.m_nDuration )
            << BUILD_LOG( kPacket_.m_kDropItem.m_nPeriod ) << END_LOG;

        SET_ERR_GOTO( ERR_INFINITY_DUNGEON_03, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK, L"Ret: " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwRecvUserUID )
        << BUILD_LOG( kPacket_.m_cRecvUserCharType )
        << BUILD_LOG( kPacket.m_vecDropItem.size() )
        << BUILD_LOG( kPacket.m_mapUserDice.size() )
        << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_INFINITY_DUNGEON_REWARD_ITEM_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_INFINITY_DUNGEON_STAT_NOT )
{
    LIF( Update_InfinityDungeon_Stats_ToDB( kPacket_ ) );

    START_LOG( clog, L"무한던전 통계 기록" ) << END_LOG;
}
