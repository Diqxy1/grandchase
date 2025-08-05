/** @file
@author cruise@kogsoft.com
*/

#include <sstream>
#include "KLuaManager.h"

/** Default constructor. */
KLuaManager::KLuaManager(void)
	: m_iTableDepth(0), m_bLuaTinkerHelper(false)
{
	//m_pkLuaState = lua_open(0);
	m_pkLuaState = lua_open();

	luaopen_base(m_pkLuaState);
}

/** Constructor with initial internal stack size.
@param iStackSize [in] LUA stack의 초기 크기
*/
KLuaManager::KLuaManager(int iStackSize)
	: m_iTableDepth(0), m_bLuaTinkerHelper(false)
{
	//m_pkLuaState = lua_open(iStackSize);
	m_pkLuaState = lua_open();
}

KLuaManager::KLuaManager(lua_State* pLuaState)
{
	m_pkLuaState = pLuaState;
	m_iTableDepth = 0;
	m_bLuaTinkerHelper = true;
}

/** Destructor. */
KLuaManager::~KLuaManager(void)
{
	if (!m_bLuaTinkerHelper)
	{
		if (m_pkLuaState != NULL)
		{
			lua_close(m_pkLuaState);
			m_pkLuaState = NULL;
		}
	}
	m_iTableDepth = 0;
}

/** String representation. */
std::string KLuaManager::ToString(void) const
{
	std::ostringstream s;
	s << "KLuaManager (luaState=" << m_pkLuaState;
	s << ",tableDepth=" << m_iTableDepth << ")";
	return s.str();
}

/** string으로 주어진 LUA 명령을 실행한다.
@param pszStr [in] LUA 명령을 담고 있는 string
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure
*/
HRESULT KLuaManager::DoString(const char* pszStr, bool bLogOutput)
{
	if (lua_dostring(m_pkLuaState, pszStr) == 0)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

/** file에 주어진 LUA 명령을 실행한다.
@param pszFileName [in] LUA 명령을 담고 있는 file의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure.
*/
HRESULT KLuaManager::DoFile(const char* pszFileName, bool bLogOutput)
{
	if (lua_dofile(m_pkLuaState, pszFileName) == 0)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

/** Internal function: pszName의 이름을 가지는 변수/table을 LUA stack으로 가져온다.
@param pszName [in] 변수/table의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure.
*/
HRESULT KLuaManager::Get(const char* pszName)
{
	if (m_iTableDepth > 0 || m_bLuaTinkerHelper)
	{
		lua_pushstring(m_pkLuaState, pszName);
		lua_gettable(m_pkLuaState, -2);
	}
	else
	{
		lua_getglobal(m_pkLuaState, pszName);
	}
	return S_OK;
}

/** Internal function: (table 내에서) iIndex번째 변수/table을 LUA stack으로 가져온다.
@param iIndex [in] 변수/table의 index (1부터 시작함)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (현재 table 안에 들어와 있지 않은
경우).
*/
HRESULT KLuaManager::Get(int iIndex)
{
	if (m_iTableDepth <= 0 && !m_bLuaTinkerHelper) return E_FAIL;
	lua_pushnumber(m_pkLuaState, iIndex);
	lua_gettable(m_pkLuaState, -2);
	return S_OK;
}

/** pszName의 이름을 가지는 table을 LUA stack으로 가져온다.
@param pszName [in] table의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 table이 없는 경우).
*/
HRESULT KLuaManager::BeginTable(const char* pszName, bool bLogOutput)
{
	if (SUCCEEDED(Get(pszName)) && lua_istable(m_pkLuaState, -1) != 0)
	{
		m_iTableDepth++;
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** (table 내에서) iIndex번째 table을 LUA stack으로 가져온다.
@param iIndex [in] table의 index (1부터 시작함)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (현재 table 안에 들어와 있지 않은
경우, 해당 table이 없는 경우).
*/
HRESULT KLuaManager::BeginTable(int iIndex, bool bLogOutput)
{
	if (SUCCEEDED(Get(iIndex)) && lua_istable(m_pkLuaState, -1) != 0)
	{
		m_iTableDepth++;
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** LUA stack에서 가장 최근에 사용한 table을 제거한다.
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (table의 사용이 없던 경우).
*/
HRESULT KLuaManager::EndTable(bool bLogOutput)
{
	if (m_iTableDepth > 0)
	{
		lua_pop(m_pkLuaState, 1);
		m_iTableDepth--;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param bValue [out] 숫자 변수가 0이면 false, 0이 아니면 true (실패 시는 변화
없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, bool& bValue, bool bLogOutput)
{
	if (SUCCEEDED(Get(pszName)) && lua_isnumber(m_pkLuaState, -1) != 0)
	{
		bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param bValue [out] 숫자 변수가 0이면 false, 0이 아니면 true (실패 시는 변화
없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, bool& bValue, bool bLogOutput)
{
	if (SUCCEEDED(Get(iIndex)) && lua_isnumber(m_pkLuaState, -1) != 0)
	{
		bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

#define GET_BY_NAME(var,type) \
    if (SUCCEEDED(Get(pszName)) && lua_isnumber(m_pkLuaState, -1) != 0) \
    { \
        var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
        lua_pop(m_pkLuaState, 1); \
        return S_OK; \
    } \
    else \
    { \
        lua_pop(m_pkLuaState, 1); \
        return E_FAIL; \
    }

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param sValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, short& sValue, bool bLogOutput)
{
	GET_BY_NAME(sValue, short);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, int& iValue, bool bLogOutput)
{
	GET_BY_NAME(iValue, int);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param lValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, long& lValue, bool bLogOutput)
{
	GET_BY_NAME(lValue, long);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param usValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput)
{
	GET_BY_NAME(usValue, unsigned short);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param uiValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput)
{
	GET_BY_NAME(uiValue, unsigned int);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param ulValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned long& ulValue, bool bLogOutput)
{
	GET_BY_NAME(ulValue, unsigned long);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param fValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, float& fValue, bool bLogOutput)
{
	GET_BY_NAME(fValue, float);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param dValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, double& dValue, bool bLogOutput)
{
	GET_BY_NAME(dValue, double);
}

#define GET_BY_NUM(var,type) \
    if (SUCCEEDED(Get(iIndex)) && lua_isnumber(m_pkLuaState, -1) != 0) \
    { \
        var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
        lua_pop(m_pkLuaState, 1); \
        return S_OK; \
    } \
    else \
    { \
        lua_pop(m_pkLuaState, 1); \
        return E_FAIL; \
    }

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, __int64& iValue, bool bLogOutput)
{
	GET_BY_NAME(iValue, __int64);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param sValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, short& sValue, bool bLogOutput)
{
	GET_BY_NUM(sValue, short);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, int& iValue, bool bLogOutput)
{
	GET_BY_NUM(iValue, int);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param lValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, long& lValue, bool bLogOutput)
{
	GET_BY_NUM(lValue, long);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param usValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned short& usValue, bool bLogOutput)
{
	GET_BY_NUM(usValue, unsigned short);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param uiValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput)
{
	GET_BY_NUM(uiValue, unsigned int);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param ulValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput)
{
	GET_BY_NUM(ulValue, unsigned long);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param fValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, float& fValue, bool bLogOutput)
{
	GET_BY_NUM(fValue, float);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param dValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, double& dValue, bool bLogOutput)
{
	GET_BY_NUM(dValue, double);
}

/** pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
@param pszName [in] 문자열 변수의 이름
@param strValue [out] 문자열 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, std::string& strValue, bool bLogOutput)
{
	if (SUCCEEDED(Get(pszName)) && lua_isstring(m_pkLuaState, -1) != 0)
	{
		strValue = lua_tostring(m_pkLuaState, -1);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param strValue [out] 문자열 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, std::string& strValue, bool bLogOutput)
{
	if (SUCCEEDED(Get(iIndex)) && lua_isstring(m_pkLuaState, -1) != 0)
	{
		strValue = lua_tostring(m_pkLuaState, -1);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
@param pszName [in] 문자열 변수의 이름
@param pszValue [out] 문자열 변수의 값이 저장될 주소 (실패시는 변화 없음)
@param iMaxLen [in] pszValue에 최대 저장 가능한 문자의 수
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput)
{
	if (SUCCEEDED(Get(pszName)) && lua_isstring(m_pkLuaState, -1) != 0)
	{
		strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param pszValue [out] 문자열 변수의 값이 저장될 주소 (실패시는 변화 없음)
@param iMaxLen [in] pszValue에 최대 저장 가능한 문자의 수
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput)
{
	if (SUCCEEDED(Get(iIndex)) && lua_isstring(m_pkLuaState, -1) != 0)
	{
		strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
		lua_pop(m_pkLuaState, 1);
		return S_OK;
	}
	else
	{
		lua_pop(m_pkLuaState, 1);
		return E_FAIL;
	}
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return S_OK for success, E_FAIL for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
HRESULT KLuaManager::GetValue(int iIndex, __int64& iValue, bool bLogOutput)
{
	GET_BY_NUM(iValue, __int64);
}