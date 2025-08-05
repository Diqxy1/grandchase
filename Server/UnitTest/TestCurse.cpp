
#pragma warning(push, 2)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include "utf16_cvt.h"
#include <set>
#include <string>
#include <fstream>
#include <iostream>
extern std::set<std::wstring>   g_setNickName;
void Curse()
{

    //std::wcout.imbue( std::locale("KOR") );
    //BOOST_CHECK_EQUAL(g_setNickName.empty(), true); // 커스 로드 전

    //LoadNickNameFilter( "Curse_jp.txt" );
    //BOOST_CHECK_EQUAL(g_setNickName.empty(), false); // 커스 로드 후

    //std::set<std::wstring>::const_iterator   sit = g_setNickName.begin();
    //while( sit != g_setNickName.end() )
    //{
    //    std::wcout << (*sit) << "  ";
    //    ++sit;
    //}

    //printf("길이 = %d",g_setNickName.size() );
    //wchar_t jpHalfManco[] = 
    //{
    //    0xff8f,
    //    0xff9d,
    //    0xff7a,
    //    L'\0'
    //};
    //BOOST_CHECK_EQUAL(IsNickNameString(jpHalfManco), true);

    //BOOST_CHECK_EQUAL(IsNickNameString(L"개새끼"), false);
    //BOOST_CHECK_EQUAL(IsNickNameString(L"ㅇㅐㅈㅏ"), false);
    //BOOST_CHECK_EQUAL(IsNickNameString(L"ㅇ"), false);
    //

    //// 셋을 비운후 정상적으로 동작하는지.
    //g_setNickName.clear(); // 비우기

    //BOOST_CHECK_EQUAL(IsNickNameString(L"사랑해"), false);
    //BOOST_CHECK_EQUAL(IsNickNameString(L"개새끼"), false);

    //BOOST_CHECK_EQUAL(g_setNickName.empty(), false); // 로드가 잘되었는지.
    //g_setNickName.clear();

    //LoadNickNameFilter( "Curse_kr.txt" );

    //BOOST_CHECK_EQUAL(IsNickNameString(L"사랑해"), false);
    //BOOST_CHECK_EQUAL(IsNickNameString(L"개새끼"), true);
    //g_setNickName.clear();

    /*LoadNickNameFilter( "Curse.txt" );

    BOOST_CHECK_EQUAL(IsNickNameString(L"　랑해"), true);

    g_setNickName.clear();*/

    std::wifstream      m_wFile;
    IMBUE_NULL_CODECVT( m_wFile );
    m_wFile.open( "./Curse.txt", std::ios::binary | std::ios::in );
    
    std::wstring strOut;
    while( !m_wFile.eof() )
    {
        m_wFile >> strOut;
        //m_wFile.getline(strOut);
    }    
    m_wFile.close();
}