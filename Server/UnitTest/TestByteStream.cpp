#include <boost/test/unit_test.hpp>

#include <iostream>
#include <Serializer/Serializer.h>
#include <Serializer/SerMap.h>
#include "KncBitStream.h"

void TestBitStream_Simulation()
{
    std::map< char, KncBitStream > mapClear;

    KncBitStream bsPlayable;
    bsPlayable.Set( 3 );
    bsPlayable.Set( 15 );

    std::copy( bsPlayable.begin(), bsPlayable.end(), std::ostream_iterator<int>( std::cout, " " ) );

    std::cout << std::endl
        << "vector size : " << bsPlayable.size() << std::endl
        << "bit size    : " << bsPlayable.GetBitSize() << std::endl
        << "byte size   : " << bsPlayable.GetByteSize() << std::endl;

    mapClear.insert( std::make_pair( (char)12, bsPlayable ) );
    mapClear.insert( std::make_pair( (char)5, bsPlayable ) );
    mapClear.insert( std::make_pair( (char)53, bsPlayable ) );
    mapClear.insert( std::make_pair( (char)100, bsPlayable ) );

    bsPlayable.Set( 10 );  // 8bit 초과. 1바이트가 더 추가된다.
    mapClear.insert( std::make_pair( (char)9, bsPlayable ) );

    bsPlayable.Set( 100 ); // 몇 바이트 추가??

    mapClear.insert( std::make_pair( (char)77, bsPlayable ) );

    std::map<char,KncBitStream>::iterator mit;
    for( mit = mapClear.begin(); mit != mapClear.end(); mit++ )
    {
        std::cout << "key : " << (int)mit->first << ", size : " << mit->second.size() << std::endl << "  ";
        mit->second.Dump( std::cout );
    }

    KSerializer ks;
    KSerBuffer  buff;

    ks.BeginWriting( &buff );
    ks.Put( mapClear );
    ks.EndWriting();

    std::cout << "size : " << buff.GetLength() << std::endl;

    mapClear.clear();

    ks.BeginReading( &buff );
    ks.Get( mapClear );
    ks.EndReading();

    std::cout << "size after serialize : " << mapClear.size() << std::endl;

    for( mit = mapClear.begin(); mit != mapClear.end(); mit++ )
    {
        std::cout << "key : " << (int)mit->first << ", size : " << mit->second.size() << std::endl << "  ";
        mit->second.Dump( std::cout );
    }
}

void TestBitStream_impl()
{
    KncBitStream bsPlayable;

    // simple set / get / reset test
    for( int i = 0; i < 327; i++ )
    {
        BOOST_CHECK( !bsPlayable.Get( i ) );

        bsPlayable.Set( i );
        BOOST_CHECK( bsPlayable.Get( i ) );

        bsPlayable.Reset( i );
        BOOST_CHECK( !bsPlayable.Get( i ) );
    }

    bsPlayable.clear();

    bsPlayable.Set( 123 );

    BOOST_CHECK_EQUAL( bsPlayable.size(), 16 );
    BOOST_CHECK_EQUAL( bsPlayable.GetBitSize(), 128 );
    BOOST_CHECK_EQUAL( bsPlayable.GetByteSize(), 16 );

    bsPlayable.Set( 256 );

    BOOST_CHECK_EQUAL( bsPlayable.size(), 33 );
    BOOST_CHECK_EQUAL( bsPlayable.GetBitSize(), 264 );
    BOOST_CHECK_EQUAL( bsPlayable.GetByteSize(), 33 );

    // test for compaction ( after reset )
    // reset 후 뒤에 붙은 0 비트값들은 모두 없어져야 한다.

    bsPlayable.Reset( 256 );

    BOOST_CHECK_EQUAL( bsPlayable.size(), 16 );
    BOOST_CHECK_EQUAL( bsPlayable.GetBitSize(), 128 );
    BOOST_CHECK_EQUAL( bsPlayable.GetByteSize(), 16 );

    bsPlayable.Reset( 123 );

    BOOST_CHECK_EQUAL( bsPlayable.size(), 0 );
    BOOST_CHECK_EQUAL( bsPlayable.GetBitSize(), 0 );
    BOOST_CHECK_EQUAL( bsPlayable.GetByteSize(), 0 );

}

void TestBitStream()
{
    TestBitStream_Simulation();
    TestBitStream_impl();
}
