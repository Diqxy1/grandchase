#include "BillDB.h"
#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

std::wstring KBillDB::ms_wstrFileDSN;
KBillDB::KBillDB(void)
: m_kHelper( m_kODBC )
{
}

KBillDB::~KBillDB(void)
{
}

void KBillDB::InitFileDSN( const char* szFileDSN_ )
{
    ms_wstrFileDSN = KncUtil::toWideString( szFileDSN_ );
}

bool KBillDB::DriveConnect()
{
    return m_kODBC.DriverConnect(  ms_wstrFileDSN.c_str() );
}

bool KBillDB::CheckBuyLimitItem( IN const DWORD dwUID_, IN const GCITEMID dwItemID_ )
{
    //{ call dbo.ILGAItemLimitUser_check ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iItemID_   int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0 ; 구매 승인(해당 아이템은 최대 구매횟수 제한 있음)
    //-1 ; 구매 승인(해당 아이템은 최대 구매횟수 제한 없음)
    //-2 ; 구매 불가(제한 횟수를 초과하였음)

    DBJIF2( m_kODBC.Query( L"{ call dbo.ILGAItemLimitUser_check ( %d, %d )  }", dwUID_, dwItemID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch()  )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0 || nOK == -1 || nOK == -2, L"구매 횟수 제한 체크. UID :" << dwUID_ << L", Item ID : " << dwItemID_ <<L", OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return ( nOK != -2 );
}

void KBillDB::CommitBuyLimitItem( IN const DWORD dwUID_, IN const GCITEMID dwItemID_ )
{
    //{ call dbo.ILGAItemLimitUser_merge ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iItemID_   int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0       ; 성공(해당 아이템에 대한 구매횟수 카운팅)
    //-1       ; 성공(해당 아이템은 최대 구매횟수 제한 없음)
    //-101이하 ; 트랜잭션 에러

    DBJIF( m_kODBC.Query( L"{ call dbo.ILGAItemLimitUser_merge ( %d, %d )  }", dwUID_, dwItemID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch()  )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0 || nOK == -1, L"구매 횟수 제한 적용. UID :" << dwUID_ << L", Item ID : " << dwItemID_ <<L", OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KBillDB::NickToLoginUID( IN const std::wstring& strNick_, OUT DWORD& dwLoginUID_ )
{
    // 대표 닉네임으로 동작한다

    dwLoginUID_ = 0;
    __JIF( !strNick_.empty(), return false );
    DBJIF2( m_kODBC.Query( L"{ call dbo.up_translate_nick_to_loginuid ( N'%s' )  }",
        strNick_.c_str() ) );

    int nLoginUID = -1;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nLoginUID );
    }        
    m_kODBC.EndFetch();

    if ( nLoginUID == -1 ) {
        START_LOG( cwarn, L"대상이 존재하지 않음 name : " << strNick_ ) << END_LOG;
        return false;
    }
    dwLoginUID_ = static_cast<DWORD>(nLoginUID);
    return true;
}

bool KBillDB::CharNickToLoginUID( IN const std::wstring& strCharNick_, OUT DWORD& dwLoginUID_, OUT int& nCharType_ )
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

	// is this basically an assertion but not terminating?
	//___JIF( !strCharNick_.empty(), return false );
	if (strCharNick_.empty())
		return false;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CNGACharacterNickname_select_iNickname_1oCharType_1oLoginUID_20131114 ( N'%s' ) }",
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

bool KBillDB::SetTimeOutMark( IN __int64 dwItemSetUID_ )
{
    //  용도 : 해당 아이템의 FromWeb필드값을 2로 설정함
    //  call dbo.up_buy_cash_item_mark ( ItemUID bigint )
    //  ItemUID는 up_buy_cash_item_rollback 저장 프로시저에 입력되는 값과 동일한 값을 취하면 됨
    //  select 1 loop return ( OK int )
    //  OK : 0(성공), -1(CashItemDisplayList 테이블에 아이템 정보가 없음), -101이하(트랜잭션 에러)
    DBJIF2( m_kODBC.Query( L"{ call dbo.up_buy_cash_item_mark ( %I64d ) }",
        dwItemSetUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    return (nOK == 0);
}

bool KBillDB::SetBillingKey( IN const __int64 WaitItemUID_, IN __int64& BillingKey_ )
{
    /*
    mk8253. 20100517.
    WIGAUID에 해당하는 빌링 키 참조 데이터 삽입

    L"{ call dbo.BKGABillingKey_insert ( %I64d, %I64d ) }"

    { call dbo.BKGABillingKey_insert ( @1, @2 ) }
    @1 ; @iWIGAUID_ [bigint]
    @2 ; @iBillingKey_ [bigint]

    1 return ( @1 )
    @1 ; OK
    0         ; 성공
    -1        ; 해당 정보가 이미 있음
    -101이하  ; 트랜젝션 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.BKGABillingKey_insert ( %I64d, %I64d ) }",
        WaitItemUID_, BillingKey_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"BillingKey" )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KBillDB::NickNameToLogin( IN const std::wstring& strNickName_, OUT std::wstring& strLogin_ )
{
    //{ call dbo.up_translate_nickname_to_login ( @1 ) }
    //@1 ; @nickname nvarchar(24)
    //
    //1 return ( @1 )
    //@1 ; @Login nvarchar(40);
    //
    strLogin_ = L"";
    DBJIF2( m_kODBC.Query( L"{ call dbo.up_translate_nickname_to_login ( N'%s' )  }",
        strNickName_.c_str() ) );

    //int nLoginUID = -1;
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> strLogin_ );
    }
    m_kODBC.EndFetch();

    if ( strLogin_.size() == 0 ) {
        START_LOG( cwarn, L"대상이 존재하지 않음 name : " << strNickName_ ) << END_LOG;
        return false;
    }
    return true;
}
