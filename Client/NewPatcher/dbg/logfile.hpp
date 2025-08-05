//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TB_DBG_LOGFILE_HPP
#define TB_DBG_LOGFILE_HPP

#include <fstream>
//{{ florist
#include <iostream>     // std::cout
#include <windows.h>    // OutputDebugString
//}} florist

// NOTES:
//
// You shouldnt create any logfiles. There are already 4 precreated logfiles that you can use
// to log stuff. 
//
// If you want to change the color that the logfiles log stuff in then go to the .cpp file and change
// the html constants at teh top of the file. Also if you want to change the title that is displayed
// in the logfile then goto the .cpp file and check the init() member function. You'll find what you 
// want to change in there.

#include  "KncUtil.h"
namespace dbg
{
    enum e_log_mode
    {
        lm_comment, ///< Logs in comment mode
        lm_error,   ///< Logs in error mode
        lm_warning, ///< Logs in warning mode
        lm_info,    ///< Logs in info mode
        lm_comment2,
        lm_error2,
        lm_warning2,
        lm_info2,
        lm_number_of_modes,
    };

    class logfile
    {
		void start();
		void end();
        void set_mode();
        void init();

        static std::wofstream m_file;
		static bool m_initialized;

        static bool m_abCurMode[lm_number_of_modes];
		//static bool m_was_error;
		//static bool m_was_info;
		//static bool m_was_comm;
		//static bool m_was_warn;
        
        e_log_mode m_mode;
        //{{ florist
        static bool             ms_bFileNameAtTime;
        static short            ms_sLevel;
        short                   m_sLevel;
        static std::string      ms_strFileName;
        //}} florist

    public:

        //{{ florist
        logfile( e_log_mode lm, unsigned short usLevel );
        // original below
        //logfile( e_log_mode lm );
        //}} florist
        ~logfile();

        logfile& line_file_func( const unsigned int line, const wchar_t* file, const wchar_t* func );

        template<class T0> logfile& operator << ( T0 data )
        {
            //{{ florist
            if( m_sLevel > ms_sLevel ) return *this;
            //}} florist

            this->set_mode();
            if( m_initialized )
            {   
                m_file<< data;                
                std::wcout << data;

                //::OutputDebugString( data );
            }

            return *this;
        }

        logfile& operator << ( const std::wstring& szStr )  
        {    
            return operator<< ( szStr.c_str() );
        }

        logfile& operator << ( const std::string& szStr )  
        {    
            return operator<< ( szStr.c_str() );
        }

        logfile& operator << ( const char* szData )  
        {    
            //{{ florist
            if( m_sLevel > ms_sLevel ) return *this;
            //}} florist

            this->set_mode();
            if( m_initialized )
            {
                std::wstring strTmp = KncUtil::toWideString( szData );
                m_file << strTmp.c_str();
                std::wcout << strTmp.c_str();
            }

            return *this;  
        }
        
        template<> logfile& operator << ( bool b )  
        {    
            //{{ florist
            if( m_sLevel > ms_sLevel ) return *this;
            //}} florist

            this->set_mode();
	        if( m_initialized )
            {
		        m_file << ( ( b == true ) ? L"true" : L"false" );
                std::wcout << ( ( b == true ) ? L"true" : L"false" );
            }

	        return *this;  
        }  


        template<> logfile& operator << ( logfile& (*_manip)(logfile&) )
        {
            //{{ florist
            if( m_sLevel > ms_sLevel ) return *this;
            //}} florist

            return _manip(*this);
        }

	    friend logfile& endl( logfile& );
	    friend logfile& endp( logfile& );
	    friend logfile& tab( logfile& );
	    friend logfile& lbreak( logfile& );


        //{{ florist.
        static void CloseFile();
        static void SetLogLevel( short sLevel );
        static void SetFileNameAtTime( bool bFileNameAtTime );
        static void SetLogFileTitle( const wchar_t* szFileTitle );
        static const char* GetLogFileName() { return ms_strFileName.c_str(); }
        //}} florist

    private:
        void OpenLogFile(); // 050202. florist. 설정에 맞춰 로그파일을 생성한다.

    }; // logfile

    extern logfile cout;
    extern logfile cerr;
    extern logfile cwarn;
    extern logfile clog;

    extern logfile cout2;
    extern logfile cerr2;
    extern logfile cwarn2;
    extern logfile clog2;

} // dbg


#endif // TB_DBG_LOGFILE_HPP