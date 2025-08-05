#pragma once
//#include "GCDeviceManager2.h"
//#include "GCLuaManager/KLuabinder.h"
//#include "GCLuaManager/KLuaManager.h"
//#include <string>
//#include <map>

using namespace std;

#define CHECK_STACK( L ) \
int iStackTop = lua_gettop(L);\
BOOST_SCOPE_EXIT( (&iStackTop)(&L) ){\
    if( iStackTop != lua_gettop(L) ) {\
        ASSERT_MBOX( "before : " << iStackTop <<"\nafter : "<<lua_gettop(L) );\
    }\
} BOOST_SCOPE_EXIT_END;

class KGCLuabinder : public KLuabinder
{
private:
    map<string, string> m_mRequireMap;	//이전에 요청된 파일들의 내용을 저장합니다. 다음번에 다시 로드되지 않게.. 사용비추

	static KGCLuabinder* m_pKGCLuabinder;

public:
	KGCLuabinder(void);
	~KGCLuabinder(void);
	static KGCLuabinder& getInstance();
	static void			 release();

	HRESULT DoFile(string _strFilename, string _strRequire);
	HRESULT DoFile(const char* strFilename);
	bool	RequireFile(const char* _strFileName );
};

class KGCLuaManager : public KLuaManager
{
private:
	map<string, string> m_mRequireMap;	//이전에 요청된 파일들의 정보를 저장합니다.
	static KGCLuaManager* m_pKGCLuaManager;

public:
	KGCLuaManager(void);
	KGCLuaManager(lua_State* pLuaState);
	~KGCLuaManager(void);
	static KGCLuaManager& getInstance();
    //static KGCLuaManager& getInstance(lua_State* pLuaState);
    static void release();

	HRESULT DoFile(string _strFilename, string _strRequire);
	void	RequireFile(const char* _strFileName );
};
