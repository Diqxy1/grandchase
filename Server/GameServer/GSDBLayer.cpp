#include "GSDBLayer.h"
#include "GSDBThread.h"
#include "odbc/Odbc.h"
#include "EvtDBThread.h"
#include "Log4.h"


ImplOstmOperatorW2A( KGSDBLayer );
NiImplementRTTI( KGSDBLayer, KDBLayer );
ImplementSingleton( KGSDBLayer );

KGSDBLayer::KGSDBLayer(void)
{
}

KGSDBLayer::~KGSDBLayer(void)
{
}

ImplToStringW( KGSDBLayer )
{
    int nCommentTableSize   = -1;
    int nItemNameTableSize  = -1;

    {   // locking scope
        KLocker lock( m_kCommentTable.m_cs );
        nCommentTableSize = m_kCommentTable.m_mapComment.size();
    }   // locking scope

    {   // locking scope
        KLocker lock( m_kItemNameTable.m_cs );
        nItemNameTableSize = m_kItemNameTable.m_mapItemName.size();
    }   // locking scope

    return START_TOSTRING_PARENTW( KDBLayer )
        << TOSTRINGW( nCommentTableSize )
        << TOSTRINGW( nItemNameTableSize );

}

std::shared_ptr<KThread> KGSDBLayer::CreateThread()
{
    std::shared_ptr<KGSDBThread> spThread( new KGSDBThread );
    spThread->SetThreadManager( this );
    return spThread;
}

void KGSDBLayer::ClearCommentTable()
{
    KLocker lock( m_kCommentTable.m_cs );

    std::wcout << L"comment 테이블 초기화. size:" << m_kCommentTable.m_mapComment.size() << std::endl;

    m_kCommentTable.m_mapComment.clear();
}

bool KGSDBLayer::GetComment( IN KODBC* pkODBC_, IN int iCommentID_, OUT std::wstring& strComment_ )
{
    KLocker lock( m_kCommentTable.m_cs );

    std::map<int,std::wstring>::iterator mit;
    mit = m_kCommentTable.m_mapComment.find( iCommentID_ );

    if( mit != m_kCommentTable.m_mapComment.end() )
    {
        strComment_ = mit->second;
        return true;
    }

    // caching된 곳에는 요구하는 코멘트가 들어있지 않다. DB에 새로 쿼리해본다.
    _DBJIF( pkODBC_->Query( L"{ call dbo.sp_executesql ( "
        L"N'select Comment from dbo.EventComment with (nolock) where ID = @1 option (maxdop 1)', "
        L"N'@1 int', %d ) }",
        iCommentID_ ),
        return false, pkODBC_ );

    _DBJIF( pkODBC_->BeginFetch(), return false, pkODBC_ );

    VERIFY_QUERY( (*pkODBC_) >> strComment_ );

    pkODBC_->EndFetch();

    // 얻어온 데이터를 caching 한다.
    m_kCommentTable.m_mapComment.insert( std::make_pair( iCommentID_, strComment_ ) );

    return true;
}

bool KGSDBLayer::GetItemName(IN KODBC* pkODBC_, IN int iItemID_, OUT std::wstring& strItemName_ )
{
    KLocker lock( m_kItemNameTable.m_cs );

    std::map<int,std::wstring>::iterator mit;
    mit = m_kItemNameTable.m_mapItemName.find( iItemID_ );

    if( mit != m_kItemNameTable.m_mapItemName.end() )
    {
        strItemName_ = mit->second;
        return true;
    }

    // caching된 곳에는 요구하는 코멘트가 들어있지 않다. DB에 새로 쿼리해본다.
    _DBJIF( pkODBC_->Query( L"{ call dbo.sp_executesql ( "
        L"N'select GoodsName from dbo.GoodsInfoList with (nolock) where GoodsID = @1 option (maxdop 1)', "
        L"N'@1 int', %d ) }",
        iItemID_ ),
        return false, pkODBC_ );

    _DBJIF( pkODBC_->BeginFetch(), return false, pkODBC_ );

    VERIFY_QUERY( (*pkODBC_) >> strItemName_ );

    pkODBC_->EndFetch();

    // 얻어온 데이터를 caching 한다.
    m_kItemNameTable.m_mapItemName.insert( std::make_pair( iItemID_, strItemName_ ) );

    return true;
}

void KGSDBLayer::ClearItemNameTable()
{
    KLocker lock( m_kItemNameTable.m_cs );

    std::wcout << L"comment 테이블 초기화. size:" << m_kItemNameTable.m_mapItemName.size() << std::endl;

    m_kItemNameTable.m_mapItemName.clear();
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KGSDBLayer::RegToLua( std::shared_ptr<lua_State> sptlua )
{
    KDBLayer::RegToLua( sptlua );

    luabind::module( sptlua.get() )
    [
        //luabind::def( "Init_GSDB", KGSDBThread::InitFileDSN ),

        luabind::def( "GetDBLayer", &KGSDBLayer::GetInstance ),

        luabind::class_<KGSDBLayer, KDBLayer>( "KGSDBLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "Init_ODBC", &KGSDBLayer::Init_ODBC )
            .def( "ClearCommentTable", &KGSDBLayer::ClearCommentTable ),

        luabind::def( "Init_EvtDB", &KEvtDBThread::InitFileDSN ),
        luabind::def( "GetEvtDBLayer", &KTDBLayer<KEvtDBThread>::GetInstance ),
        luabind::class_< KTDBLayer<KEvtDBThread>, KDBLayer >( "KEvtDBLayer" )
            .def( luabind::tostring(luabind::self) )
    ];
}

void KGSDBLayer::Init_ODBC( const char* szMainODBC_, const char* szStatODBC_ )
{
    KGSDBThread::InitFileDSN( szMainODBC_ );
    KGSDBThread::InitFileDSNStat( szStatODBC_ );
}