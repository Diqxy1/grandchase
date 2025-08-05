//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TB_DBG_PROFILER_HPP
#define TB_DBG_PROFILER_HPP

// Comment this out if you want the calls to the profiler to do nothing. You'd want this behavior
// in release mode.
//#define TB_USE_PROFILER


#include <hash_map>


namespace dbg
{
    typedef __int64 i64;

    struct _char_hash_compare
    {
	    size_t operator ()( const wchar_t* str ) const
	    {
            register size_t ret = 0;

            while( *str )
            {
                ret ^= *str << 1;
                str++;
            }

            return ret;
	    }

	    bool operator ()( const wchar_t* left, const wchar_t* right ) const
	    {
            while( *left && *right )
            {
                if( *left < *right )
                {
                    return true;
                }
                
                if( *left > *right )
                {
                    return false;
                }

                ++left;
                ++right;
            }

            return false;
	    }
    };

    typedef stdext::hash_compare<const wchar_t*, _char_hash_compare> char_hash_compare;

    class profiler
    {
        struct profile_info
        {
            float total_time;
            float max_time;
            float min_time;
            float start_time;
            i64 num_profiles;
            profile_info* parent;
            typedef stdext::hash_map<const wchar_t*, profile_info*, char_hash_compare> hash_t;
            hash_t children;
        };

        static profile_info* m_root;
        static profile_info* m_current;
        static i64 m_frequency;

        static void kill_hash_map( profile_info::hash_t* pmap );
        static void output_profile_info( profile_info::hash_t* pmap, std::ofstream& outfile, profile_info* parent );

    public:

        profiler( const wchar_t* name );
        ~profiler();

        static void start( const wchar_t* name ); // throws std::runtime_error if cant initialize
        static void end();

        static void init_profiler();
        static void kill_profiler( const wchar_t* output_file );
    };

} // dbg


// NOTES:
//
// To use this macro, you will need to turn off the edit and continue compiler debug option from
// your project settings. Otherwise you will get unwanted behaviour beause of how the __LINE__ macro
// is used in MSVC. 

#ifdef TB_USE_PROFILER
#   define tb_profiler_help( name, l ) tb_profiler_help2( l )(name)
#   define tb_profiler_help2( li ) tb_profile_block ## li
#   define tbProfile( name ) ::dbg::profiler tb_profiler_help( name, __LINE__ )
#else
#   define tbProfile( name ) ((void*)0)
#endif


#endif // TB_DBG_PROFILER_HPP