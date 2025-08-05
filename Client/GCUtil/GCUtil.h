#pragma once
//#include <windows.h>
//#include <string>


namespace GCUTIL_MATH
{
#define SETVECTOR4(vector,_x,_y,_z,_w) {vector.x = _x; vector.y = _y; vector.z = _z; vector.w = _w;}
#define SETVECTOR3(vector,_x,_y,_z) {vector.x = _x; vector.y = _y; vector.z = _z;}
#define SETVECTOR2(vector,_x,_y) {vector.x = _x; vector.y = _y;}

    static inline USHORT floatToHalfI(UINT i)
    {
        register int s =  (i >> 16) & 0x00008000;
        register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
        register int m =   i        & 0x007fffff;

        if (e <= 0)
        {
            if (e < -10)
            {
                return 0;
            }
            m = (m | 0x00800000) >> (1 - e);

            return s | (m >> 13);
        }
        else if (e == 0xff - (127 - 15))
        {
            if (m == 0) // Inf
            {
                return s | 0x7c00;
            } 
            else    // NAN
            {
                m >>= 13;
                return s | 0x7c00 | m | (m == 0);
            }
        }
        else
        {
            if (e > 30) // Overflow
            {
                return s | 0x7c00;
            }

            return s | (e << 10) | (m >> 13);
        }
    }
    static inline USHORT floatToHalf(float i)
    {
        return floatToHalfI(*reinterpret_cast<UINT*>(&i));
    }

    static inline UINT halfToFloatI(USHORT y)
    {
        register int s = (y >> 15) & 0x00000001;
        register int e = (y >> 10) & 0x0000001f;
        register int m =  y        & 0x000003ff;

        if (e == 0)
        {
            if (m == 0) // Plus or minus zero
            {
                return s << 31;
            }
            else // Denormalized number -- renormalize it
            {
                while (!(m & 0x00000400))
                {
                    m <<= 1;
                    e -=  1;
                }
                e += 1;
                m &= ~0x00000400;
            }
        }
        else if (e == 31)
        {
            if (m == 0) // Inf
            {
                return (s << 31) | 0x7f800000;
            }
            else // NaN
            {
                return (s << 31) | 0x7f800000 | (m << 13);
            }
        }
        e = e + (127 - 15);
        m = m << 13;
        return (s << 31) | (e << 23) | m;
    }

    static inline float halfToFloat(USHORT y)
    {
        UINT r = halfToFloatI(y);
        return *reinterpret_cast<float*>(&r);
    }
}

namespace GCUTIL_STR
{

#if defined( UNICODE )
#define ToWideStr(val) val
#define ToCharStr(val) GCUTIL_STR::GCStrWideToChar(val)
#else
#define ToWideStr(val) GCUTIL_STR::GCStrCharToWide(val)
#define ToCharStr(val) val
#endif

    // Trim계열 함수들.
    void RTrim( std::string& str_ );
    void RTrim( std::wstring& str_ );
    void LTrim( std::string& str_ );
    void LTrim( std::wstring& str_ );
    void Trim( std::string& str_ );
    void Trim( std::wstring& str_ );
    void ToUpper( std::string& str_ );
    void ToUpper( std::wstring& str_ );
    void ToLower( std::string & str_ );
    void ToLower( std::wstring & str_ );

    const char*	GCStrWideToChar(const WCHAR* strSrc);
    const WCHAR* GCStrCharToWide(const char* strSrc);
    LPCTSTR GCStrToDefined( const wchar_t* strSrc );
    LPCTSTR GCStrToDefined( const char* strSrc );

    size_t GetStringBytes( const std::wstring& str_ );
    bool CheckInvalidCharacters( const std::wstring& str_ );

    void Tokenizer( const std::wstring& string, const wchar_t* wSep,  std::vector<std::wstring>& vecString );
    std::string InsertCommaNumber( int nNumber );

	// 밑으로는 펑터 등록.
	class KStringSort
	{
	public:
		bool operator()( std::string& lhs_, std::string& rhs_ ) const
		{
			return lhs_ < rhs_;
		}
	};
} 

namespace GCUTIL_GRAPHICS
{
#define SETCOLOR(color,_r,_g,_b,_a) { color.r = _r; color.g = _g; color.b = _b; color.a = _a;}
}

namespace GCUTIL_TIME
{
    int         GetYear( time_t tTime_ );
    int         GetMonth( time_t tTime_ );
    int         GetDay( time_t tTime_ );
    int         GetHour( time_t tTime_ );
    int         GetMinute( time_t tTime_ );
    int         GetSecond( time_t tTime_ );
    time_t      GetCurTime();
    bool        IsFuture( time_t tTime_ );
    bool        IsPast( time_t tTime_ );
}

namespace GCUTIL_STR_THAI
{
	const WCHAR* CharNextTh(const WCHAR* lpsz);
	const WCHAR* CharPrevTh(const WCHAR* lpszStart, const WCHAR* lpszCurrent);
	const char* CharNextThA(const char* lpsz);
	const char* CharPrevThA(const char* lpszStart, const char* lpszCurrent);
	bool IsComposibleA(BYTE prev, BYTE curr, int mode);
}