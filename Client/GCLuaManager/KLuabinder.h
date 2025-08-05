#pragma once

//#include <windows.h>
#include "lua_tinker.h"
//#include <string>

class KLuabinder
{

public:
	KLuabinder();
    virtual ~KLuabinder();

    void Initialize();
    void Destroy();

    HRESULT DoFile(const char* strFilename);
    HRESULT DoString(const char* strBuf, int iSize);
    void Interpret();

    std::string GetLastError() const { return m_strLastErrMsg; }

	HRESULT CollectGarbage(int what,int data);
    lua_State* GetLuaState() { return m_pkLuaState; }

private:
    lua_State* m_pkLuaState;
    std::string m_strLastErrMsg;
};

extern KLuabinder g_kLuabinder;

#define KDeclareLuaInvoke \
public: \
    void to_lua(lua_State* L) { lua_tinker::push(L, this); }

#define KRegLua(x) x