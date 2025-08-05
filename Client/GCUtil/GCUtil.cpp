#include "stdafx.h"
#include "GCUtil.h"
//

#include "KncUtil.h"


namespace GCUTIL_MATH
{

}

namespace GCUTIL_STR
{
    void RTrim( std::string& str_ )
    {
        str_.erase( str_.find_last_not_of( " \t\n") + 1 );
    }

    void RTrim( std::wstring& str_ )
    {
        str_.erase( str_.find_last_not_of( L" \t\n") + 1 );
    }

    void LTrim( std::string& str_ )
    {
        str_.erase( 0, str_.find_first_not_of( " \t\n" ) );
    }

    void LTrim( std::wstring& str_ )
    {
        str_.erase( 0, str_.find_first_not_of( L" \t\n" ) );
    }

    void Trim( std::string& str_ )
    {
        LTrim( str_ );
        RTrim( str_ );
    }

    void Trim( std::wstring& str_ )
    {
        LTrim( str_ );
        RTrim( str_ );
    }

	//태국어에서 오동작하여 Boost라이브러리로 수정함..

    void ToUpper( std::string& str_ )
    {
        //std::transform( str_.begin(), str_.end(), str_.begin(), std::toupper );
		boost::to_upper(str_);
    }

    void ToUpper( std::wstring& str_ )
    {
        //std::transform( str_.begin(), str_.end(), str_.begin(), std::toupper );
		boost::to_upper(str_);
    }

    void ToLower( std::string & str_ )
    {
        //std::transform( str_.begin(), str_.end(), str_.begin(), std::tolower );
		boost::to_lower(str_);
    }

    void ToLower( std::wstring & str_ )
    {
        //std::transform( str_.begin(), str_.end(), str_.begin(), std::tolower );
		boost::to_lower(str_);
    }

    const char*	GCStrWideToChar(const WCHAR* strSrc)
    {
        static char strTemp[4096];
        memset( strTemp, 0, sizeof(char)*4096 );
        std::wstring strWide = strSrc;
        std::string strNarrow = KncUtil::toNarrowString( strWide );
        lstrcpyA( strTemp, strNarrow.c_str() );

        return strTemp;
    }

    const WCHAR* GCStrCharToWide(const char* strSrc)
    {
        static WCHAR strTemp[4096];
        memset( strTemp, 0, sizeof(WCHAR)*4096 );
        std::string strNarrow = strSrc;
        std::wstring strWide = KncUtil::toWideString( strNarrow );
        lstrcpyW( strTemp, strWide.c_str() );

        return strTemp;
    }

    size_t GetStringBytes( const std::wstring& str_ )
    {
        size_t sizeofbyte = 0;
        for( int i = 0; i < (int)str_.size(); ++i )
        {
            WCHAR wChar = str_[i];
            if( wChar < L' ' )
            {
                continue;
            }
            else if( wChar >= L' ' && wChar <= L'~' )
            {
                sizeofbyte += 1;
            }
            else
            {
                sizeofbyte += 2;
            }
        }
        return sizeofbyte;
    }

    bool CheckInvalidCharacters( const std::wstring& str_ )
    {
        for( int i = 0; i < (int)str_.size(); ++i )
        {
            WCHAR wChar = str_[i];
            if( wChar < L'!' )
            {
                return false;
            }
        }
        return true;
    }

    LPCTSTR GCStrToDefined( const wchar_t* strSrc )
    {
#if defined( UNICODE )
        return strSrc;
#else
        return GCStrWideToChar( strSrc );
#endif
    }

    LPCTSTR GCStrToDefined( const char* strSrc )
    {
#if defined( UNICODE )
        return GCStrCharToWide( strSrc );
#else
        return strSrc;
#endif
    }

    void Tokenizer( const std::wstring& string, const wchar_t* wSep, std::vector<std::wstring>& vecString )
    {
        typedef boost::tokenizer< boost::char_separator< WCHAR >, std::wstring::const_iterator, std::wstring > tokenizer;

        std::wstring strTemp = string;

        boost::char_separator<WCHAR> sep(wSep);
        tokenizer tokens( strTemp, sep );

        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
        {
            vecString.push_back( *tok_iter );
        }

        return;

    }

    std::string InsertCommaNumber( int nNumber )
    {
        std::string strNumber = boost::lexical_cast<std::string>( nNumber );
        char szTemp[MAX_PATH];

        NUMBERFMTA fmt = { 0,0,3, ".", ",", 1 };
        ::GetNumberFormatA( NULL, NULL, strNumber.c_str(), &fmt, szTemp, MAX_PATH );

        strNumber = szTemp;
        return strNumber;
    }
}

namespace GCUTIL_GRAPHICS
{
}

namespace GCUTIL_TIME
{
    int GetYear( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetYear();
    }
    int GetMonth( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetMonth();
    }
    int GetDay( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetDay();
    }
    int GetHour( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetHour();
    }
    int GetMinute( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetMinute();
    }
    int GetSecond( time_t tTime_ )
    {
        CTime kTime(tTime_);
        return kTime.GetSecond();
    }
    time_t GetCurTime()
    {
        return static_cast<time_t>(CTime::GetCurrentTime().GetTime());
    }
    bool IsFuture( time_t tTime_ )
    {
        CTime kCurtime = CTime::GetCurrentTime();
        CTime kCheckTime = tTime_;
        if ( kCurtime <= kCheckTime )
            return true;
        return false;
    }
    bool IsPast( time_t tTime_ )
    {
        return !IsFuture( tTime_);
    }
}


namespace GCUTIL_STR_THAI
{
	enum CLASS {	
		CTRL= 0,
		NON= 1,
		CONS= 2,
		LV= 3,
		FV1= 4,
		FV2= 5,
		FV3= 6,
		BV1= 7,
		BV2= 8,
		BD= 9,
		TONE=10,
		AD1=11,
		AD2=12,
		AD3=13,
		AV1=14,
		AV2=15,
		AV3=16,
	};

	CLASS	CLASS_TABLE[256] = {
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		NON, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, //E01
		CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, //E02
		CONS, CONS, CONS, CONS,  FV3, CONS,  FV3, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS,  NON, //E02
		FV1,  AV2,  FV1,  FV1,  AV1,  AV3,  AV2,  AV3,  BV1,  BV2,   BD,  NON,  NON,  NON,  NON,  NON, //E03
		LV,   LV,   LV,   LV,   LV,   FV2,  NON,  AD2, TONE, TONE, TONE, TONE,  AD1,  AD1,  AD3,  NON,  //E04
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, CTRL, 
	//   0     1     2     3     4     5     6     7     8     9     10
	};

	namespace INPUT
	{
		enum TYPE { A, C, S, R, X };

		bool COMPOSIBLE[3][5] = {
			{ true, true, true, true, true },
			{ true, true, true,false, true },
			{ true, true,false,false, true },
		};

		TYPE STATE_CHECK[17][17] = {
			{ X,A,A,A,R,R,R,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,R,R,R,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,C,C,C,C,C,C,C,C,C,C },
			{ X,S,A,S,S,S,S,R,R,R,R,R,R,R,R,R,R },
			{ X,S,A,S,A,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,A,S,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,R,R,R,C,C,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,A,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,C,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,C,R,R,R,R },
		};
	}

	namespace OUTPUT
	{
		enum TYPE { N, C, X };

		TYPE STATE_CHECK[17][17] = {
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 0
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 1
			{ X,N,N,N,C,C,C,C,C,C,C,C,C,C,C,C,C }, // 2
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 3
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 4
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 5
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 6
			{ X,N,N,N,N,N,N,N,N,N,C,C,N,N,N,N,N }, // 7
			{ X,N,N,N,N,N,N,N,N,N,C,N,N,N,N,N,N }, // 8
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 9
			{ X,N,N,N,C,N,N,N,N,N,N,N,N,N,N,N,N }, // 10
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 11
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 12
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 13
			{ X,N,N,N,N,N,N,N,N,N,C,C,N,N,N,N,N }, // 14
			{ X,N,N,N,N,N,N,N,N,N,C,N,N,N,N,N,N }, // 15
			{ X,N,N,N,N,N,N,N,N,N,C,N,C,N,N,N,N }, // 16
		};
	}


	bool IsComposibleA(BYTE prev, BYTE curr, int mode)
	{
		using namespace GCUTIL_STR_THAI::INPUT;

		if(mode > 2) {
			return false;
		} else {
			CLASS prev_class = CLASS_TABLE[prev];
			CLASS curr_class = CLASS_TABLE[curr];
			TYPE state_type = STATE_CHECK[prev_class][curr_class];

			return COMPOSIBLE[mode][state_type];
		}
	}

	// Unicode of thai : 0x0E00~0x0E7F (Direct Mapping)
	// Thai Character -> Unicode    (char+0x0E00)-0xA0 =    char+0x0D60
	// Unicode -> Thai Character (wchar_t-0x0E00)+0xA0 = wchar_t-0x0D60
	// 0x0D60 = 3424

	bool CheckClassTableBound(int sum)
	{
		if (sum < 0 || sum > 255) return false;
		return true;
	}

	const WCHAR* CharNextTh(const WCHAR* lpsz)
	{
		using namespace GCUTIL_STR_THAI::OUTPUT;

		const WCHAR* stream = (const WCHAR*)(lpsz);
		while(CheckClassTableBound(stream[0]-3424)==true && 
			CheckClassTableBound(stream[1]-3424)==true &&
			STATE_CHECK[CLASS_TABLE[stream[0]-3424]][CLASS_TABLE[stream[1]-3424]] == C) 
		{
			++stream;
		}

		return stream+1;
	}

	const WCHAR* CharPrevTh(const WCHAR* lpszStart, const WCHAR* lpszCurrent)
	{
		while(lpszStart < lpszCurrent) {

			const WCHAR* lpszNext = CharNextTh(lpszStart);

			if(lpszNext < lpszCurrent) {
				lpszStart = lpszNext;
			} else {
				return lpszStart;
			}
		}

		return lpszStart;
	}


	const char* CharNextThA(const char* lpsz)
	{
		using namespace GCUTIL_STR_THAI::OUTPUT;

		const BYTE* stream = (const BYTE*)(lpsz);

		while(CheckClassTableBound(stream[0])==true && 
			CheckClassTableBound(stream[1])==true &&
			STATE_CHECK[CLASS_TABLE[stream[0]]][CLASS_TABLE[stream[1]]] == C) ++stream;

		return (const char*)(stream+1);
	}

	const char* CharPrevThA(const char* lpszStart, const char* lpszCurrent)
	{
		while(lpszStart < lpszCurrent) {

			const char* lpszNext = CharNextThA(lpszStart);

			if(lpszNext < lpszCurrent) {
				lpszStart = lpszNext;
			} else {
				return lpszStart;
			}
		}

		return lpszStart;
	}


}
