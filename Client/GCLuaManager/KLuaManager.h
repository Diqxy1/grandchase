/** @file
@author cruise@kogsoft.com
*/

#ifndef _KLUAMANAGER_H_
#define _KLUAMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include <string>
extern "C"{
#include "LUA/lua.h"
#include "LUA/lauxlib.h"
#include "LUA/lualib.h"
};
//#include <windows.h>
////#include <assert.h>

/** LUA manager class.
LUA언어로 작성된 configuration file을 읽어 오고, 그 내용을 C++
프로그램으로 가져오기 위한 class. LUA의 기본형은 number (float 형태로
저장), string 2가지 뿐이고, 이들을 table (structure 개념과 유사)로
저장할 수 있다. table 내의 table도 지원된다. 자세한 내용은 LUA manual을
참고하기 바란다.
 
@verbinclude testKLuaManager.cpp
*/

#define LUA_GET_VALUE(name, value, ret)	\
	if( FAILED( luaMgr.GetValue( name, value ) ) )\
	{\
	assert(!name);\
	ret;\
	}

#define LUA_GET_VALUE_NOASSERT(name, value, ret) \
	if( FAILED( luaMgr.GetValue( name, value ) ) )\
	{\
	ret;\
	}

#define LUA_GET_VALUE_DEF(name, value, def )	\
	if( FAILED( luaMgr.GetValue( name, value ) ) )\
	{\
	value = def;\
	}

#define LUA_GET_VALUE_RAII( name, value )   \
    if( SUCCEEDED( luaMgr.GetValue( name, value ) ) )

#define LUA_BEGIN_TABLE(name, ret)		\
	if( FAILED( luaMgr.BeginTable( name ) ) )\
	{\
	ret;\
	}

#define LUA_END_TABLE(ret)					\
	if ( FAILED( luaMgr.EndTable() ) )\
	{\
	assert(!"EndTable");\
	ret;\
	}

#define LUA_GET_BIG_INT(name, value, ret) \
    if( FAILED( luaMgr.GetBigINT( name, value ) ) )\
    {\
    ret;\
    }

#define LUA_BEGIN_TABLE_RAII( name )   \
    if( KLuaTable temptable__ = KLuaTable( &luaMgr, name, false ) )

//typedef long HRESULT;

class KLuaManager
{
public:
    /// Default constructor.
    KLuaManager(void);
    /// Constructor with initial internal stack size.
    KLuaManager(int iStackSize);
	/// Constructor for LuaTinker Helper
	KLuaManager(lua_State* pLuaState);
    /// Destructor.
    ~KLuaManager(void);
private:
    /// Copy constructor: DISABLED.
    KLuaManager(const KLuaManager&);
    /// Assignment operator: DISABLED.
    void operator=(const KLuaManager&);
public:
    /// String representation.
    std::string ToString(void) const;
    /// Get the low-level LUA state.
    inline lua_State* GetLuaState(void) { return m_pkLuaState; }

    // COMMAND EXECUTION ***********************************************

    /// string으로 주어진 LUA 명령을 실행한다.
    HRESULT DoString(const char* pszStr, int iSize);
    /// file에 주어진 LUA 명령을 실행한다.
    HRESULT DoFile(const char* pszFileName, bool bLogOutput = true);

    // TABLE HANDLING **************************************************

    /// pszName의 이름을 가지는 table을 LUA stack으로 가져온다.
    HRESULT BeginTable(const char* pszName, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 table을 LUA stack으로 가져온다.
    HRESULT BeginTable(int iIndex, bool bLogOutput = true);
    /// LUA stack에서 가장 최근에 사용한 table을 제거한다.
    HRESULT EndTable(bool bLogOutput = true);

    // GET BOOL-TYPE VALUE *********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, bool& bValue, bool bLogOutput = true);

    // GET INTEGER-TYPE VALUE ******************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, short& sValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, short& sValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, int& iValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, int& iValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* szName, long& lValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, long& lValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, unsigned short& usValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* szName, unsigned long& ulValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput = true);

    // GET FLOAT-TYPE VALUE ********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, float& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, float& fValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, double& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, double& fValue, bool bLogOutput = true);

    // GET STRING-TYPE VALUE *******************************************

    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, std::string& strValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true);

    HRESULT GetValue( const char* pszName, UCHAR& ucValue, bool bLogOutput = true );
    HRESULT GetValue( int iIndex, UCHAR& ucValue, bool bLogOutput = true );

    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetBigINT(const char* pszName, __int64& i64Value, bool bLogOutput = true);
    HRESULT GetBigINT(int iIndex, __int64& i64Value, bool bLogOutput = true);

    // 마지막으로 발생한 에러를 메시지를 리턴해 준다.
    const std::string& GetLastError() { return m_strLastErrMsg; }

private:
    /// Internal function: pszName의 이름을 가지는 변수/table을 LUA stack으로 가져온다.
    HRESULT Get(const char* pszName);
    /// Internal function: (table 내에서) iIndex번째 변수/table을 LUA stack으로 가져온다.
    HRESULT Get(int iIndex);

private:
    lua_State* m_pkLuaState; ///< LUA state: 실제 동작을 담당.
    int m_iTableDepth; ///< LUA stack에 올라간 table의 갯수.
	bool m_bLuaTinkerHelper;	// Lua Tinker를 사용하기 위한거라면...m_pkLuaState를 만들지도 지우지도 않고, 사용만 함
    std::string m_strLastErrMsg;

private:    
    int m_iLogLevel;
};

class KLuaTable {
private:
    KLuaTable();
    KLuaManager* m_luaMgr;
    bool m_bTableOpenSuccess;
public:
    KLuaTable( KLuaManager* kLuaMgr_, const char* strTableName, bool bOutputLog = false )
        : m_luaMgr( kLuaMgr_ )
        , m_bTableOpenSuccess( false )
    {
            m_bTableOpenSuccess = SUCCEEDED(kLuaMgr_->BeginTable( strTableName, bOutputLog ));
    }
    KLuaTable( KLuaManager* kLuaMgr_, int iTableIndex, bool bOutputLog = false )
        : m_luaMgr( kLuaMgr_ )
        , m_bTableOpenSuccess( false )
    {
        m_bTableOpenSuccess = SUCCEEDED(kLuaMgr_->BeginTable( iTableIndex, bOutputLog ));
    }
    ~KLuaTable() {
        if( m_bTableOpenSuccess )
            m_luaMgr->EndTable();
    }
    operator bool() {
        return m_bTableOpenSuccess;
    }
};

#endif // _KLUAMANAGER_H_
