#include "stdafx.h"
#include "KLuabinder.h"

KLuabinder g_kLuabinder;

//---------------------------------------------------------------------------
KLuabinder::KLuabinder()
{
	Initialize();
}

//---------------------------------------------------------------------------
KLuabinder::~KLuabinder()
{
    if(m_pkLuaState)
	    Destroy();// 이 함수를 호출하는 부분이 없어서 메모리 누수. 해서 호출
}
//---------------------------------------------------------------------------
void KLuabinder::Initialize()
{
    m_pkLuaState = lua_open();

    // init Lua
    luaL_openlibs(m_pkLuaState);

    lua_settop(m_pkLuaState, 0);
}
//---------------------------------------------------------------------------
void KLuabinder::Destroy()
{
    // close Lua
    lua_close(m_pkLuaState);
}
//---------------------------------------------------------------------------
HRESULT KLuabinder::DoFile(const char* strFilename)
{
    if( luaL_dofile(m_pkLuaState, strFilename) == 0 )
		return S_OK;

    m_strLastErrMsg = lua_tostring( m_pkLuaState, -1 );
    lua_pop( m_pkLuaState, 1 );
	return E_FAIL;
}
//--------------------- ------------------------------------------------------
HRESULT KLuabinder::DoString(const char* strBuf, int iSize)
{
	if( luaL_dostring(m_pkLuaState, strBuf, iSize) == 0 )
		return S_OK;

    m_strLastErrMsg = lua_tostring( m_pkLuaState, -1 );
    lua_pop( m_pkLuaState, 1 );
	return E_FAIL;
    
}
//---------------------------------------------------------------------------
void KLuabinder::Interpret()
{    
	/*
    while (true)
    {
        KCon << "> ";
        std::string strBuf = KCon.GetString();
        if (strBuf == "exit")
            break;

        DoString(strBuf);
    }
	*/
}

HRESULT KLuabinder::CollectGarbage( int what,int data )
{
	return lua_gc(m_pkLuaState,what,data);
}
//---------------------------------------------------------------------------
