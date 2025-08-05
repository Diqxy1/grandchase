#include <WinSock2.h>
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include "odbc/odbc.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

#include <Serializer/Serializer.h>
#include "SubThread.h"
#include <boost/tuple/tuple.hpp>
#include "Lua/KLuaManager.h"

//FILE_NAME_FOR_LOG

void InsertSingleItem( KODBC& m_kODBC, DWORD dwItemID, int nFactor, bool bCount )
{
    // m_kODBC 라고 쓴거는 매크로 쓰기 위해.
    if( bCount )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.GIA_game_item_duration_single_insert ( 13171, %d, %d ) }",
            dwItemID, nFactor ) );
    }
    else
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.GIA_game_item_normal_single_insert ( 13171, %d, %d ) }",
            dwItemID, nFactor ) );
    }

    m_kODBC.BeginFetch();
    m_kODBC.EndFetch();

}

void InsertItem( KODBC& m_kODBC, std::wstringstream& stm )
{
    DBJIF( m_kODBC.Query( L"{ call dbo.up_insert_item ( N'kkurrung,', N'%s' ) }",
        stm.str().c_str() ) );
}

void ReadyItems( std::vector< boost::tuple<DWORD,int,bool> >& vecDatas )
{
    std::cout << "Item Load Start..." << std::endl;
    KLuaManager kLuaMng;
    JIF( kLuaMng.DoFile( "TestODBC.lua" ) == S_OK );
    JIF( kLuaMng.BeginTable("Items") == S_OK );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        int nItemID;
        int nFactor;
        bool bCount;
        JIF( kLuaMng.GetValue( 1, nItemID ) == S_OK );
        JIF( kLuaMng.GetValue( 2, nFactor ) == S_OK );
        JIF( kLuaMng.GetValue( 3, bCount ) == S_OK );

        vecDatas.push_back( boost::make_tuple( (DWORD)(nItemID), nFactor, bCount ) );

        JIF( kLuaMng.EndTable() == S_OK );
    }

    JIF( kLuaMng.EndTable() == S_OK );
    std::cout << "Item Load Complete : " << vecDatas.size() << std::endl;
}

void TestODBC()
{
    std::cout << "ODBC Test..."  << std::endl;
    KODBC kOdbc;
    BOOST_CHECK( kOdbc.DriverConnect( L"odbc_internal.dsn" ) );

    std::cout << " Test Start " << std::endl << std::endl;
    std::vector< boost::tuple<DWORD,int,bool> > vecDatas;
    ReadyItems( vecDatas );

    DWORD dwSingle = ::GetTickCount();
    std::vector< boost::tuple<DWORD,int,bool> >::iterator vit;
    for( vit = vecDatas.begin() ; vit != vecDatas.end() ; ++vit )
    {
        InsertSingleItem( kOdbc, vit->get<0>(), vit->get<1>(), vit->get<2>() );
    }
    dwSingle = ::GetTickCount() - dwSingle;
    std::wstringstream stm;
    for( vit = vecDatas.begin() ; vit != vecDatas.end() ; ++vit )
    {
        stm<< vit->get<0>() << L"," << vit->get<1>() << L",";
    }

    DWORD dwMulti = ::GetTickCount();
    InsertItem( kOdbc, stm );
    dwMulti = ::GetTickCount() - dwMulti;

    std::cout << "elapsed Time Multi : " << dwMulti << std::endl;
    std::cout << "elapsed Time Single : " << dwSingle << std::endl;

}
