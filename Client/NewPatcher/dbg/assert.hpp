//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TB_DBG_ASSERT_HPP
#define TB_DBG_ASSERT_HPP

#include "logfile.hpp"

// comment this out if you want the calls to the cassert macro to do nothing. This is probably what
// you want when building in release mode.
#define TB_ASSERT

namespace dbg
{
    class custom_assert
    {
        std::wstring m_expression;
        std::wstring m_filename;
        unsigned int m_line;

    public:
        
        custom_assert& m_custom_assert_a;
        custom_assert& m_custom_assert_b;

        custom_assert();

        template< typename T0 > custom_assert& print_val( const wchar_t* str, T0 val )
        {
            cerr<< tab<< str<< L" = "<< val<< endl;
            return *this;
        }

        custom_assert& msg( const wchar_t* msg );
        custom_assert& msg( const char* msg );

        void fatal();

        custom_assert& operator () ( const wchar_t* expr, unsigned int line, const wchar_t* func, const wchar_t* file );
        custom_assert& operator () ( const char* expr, unsigned int line, const wchar_t* func, const wchar_t* file );
        
    }; // custom_assert

    extern custom_assert cassert;

} // dbg


#define WIDEN(x) L ## x
#define WIDENM(x) WIDEN(x)
#define __WFILE__ WIDENM(__FILE__)
#define __WFUNCTION__ WIDENM(__FUNCTION__)

#define WIDENE(x) L ## # x

#define m_custom_assert_a(x) m_custom_assert_op( x, b )
#define m_custom_assert_b(x) m_custom_assert_op( x, a )

#define m_custom_assert_op( x, next ) m_custom_assert_a.print_val( WIDENE(x), (x) ).m_custom_assert_ ## next

#ifdef TB_ASSERT

#define cassert( expr ) \
    if( (expr) ) ; \
    else ::dbg::cassert( WIDENE(expr) , __LINE__, __WFUNCTION__, __WFILE__ ).m_custom_assert_a 

#else

#define cassert( expr ) \
    if( true ) ; \
    else ::dbg::cassert( WIDENE(expr) , __LINE__, __WFUNCTION__, __WFILE__ ).m_custom_assert_a 

#endif // TB_ASSERT




#endif // TB_DBG_ASSERT_HPP