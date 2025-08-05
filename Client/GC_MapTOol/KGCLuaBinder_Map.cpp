#include "stdafx.h"
//#include ".\KGCLuabinder_Map.h"

KGCLuabinder_Map* KGCLuabinder_Map::m_pKGCLuabinder = NULL;

KGCLuabinder_Map::KGCLuabinder_Map(void)
{
	lua_tinker::class_add<KGCLuabinder_Map>(GetLuaState(), "KGCLuabinder");
	lua_tinker::class_def<KGCLuabinder_Map>(GetLuaState(), "RequireFile", &KGCLuabinder_Map::RequireFile);;

	lua_tinker::decl(GetLuaState(), "KGCLuabinder", this);
}

KGCLuabinder_Map::~KGCLuabinder_Map(void)
{
}

KGCLuabinder_Map& KGCLuabinder_Map::getInstance()
{
	if (m_pKGCLuabinder == NULL)
		m_pKGCLuabinder = new KGCLuabinder_Map();

	return *m_pKGCLuabinder;
}

void KGCLuabinder_Map::release()
{
	SAFE_DELETE(m_pKGCLuabinder);
}

HRESULT KGCLuabinder_Map::DoFile(const char* strFilename)
{
	if (luaL_dofile(GetLuaState(), strFilename) == 0)
		return S_OK;

	return E_FAIL;
}

HRESULT KGCLuabinder_Map::DoFile(string _strFilename, string _strRequire)
{
	map< string, string > ::const_iterator m_iter_IsExist;

	m_iter_IsExist = m_mRequireMap.find(_strFilename);

	if (m_iter_IsExist == m_mRequireMap.end())
	{
		//스트링의 덩어리를 만듭니다.
		string strTotal;
		string strTemp;

		//init
		strTotal.clear();
		strTemp.clear();

		GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), _strFilename);

		if (!memfile.IsLoaded())
			return E_FAIL;

		strTotal = (const char*)memfile.GetDataPointer();

		strTotal.insert(0, strTemp);

		//Require들 부터 차례로 호출해 줍니다.
		GCMemoryFile memfile2(g_pGCDeviceManager->GetMassFileManager(), _strRequire);

		if (!memfile2.IsLoaded())
			return E_FAIL;

		strTotal.insert(0, (const char*)memfile2.GetDataPointer());

		//스트링의 덩어리를 저장합니다.
		m_mRequireMap[_strFilename] = strTotal;

		//뭉쳐서 루아를 실행시켜 줍니다.
		_strFilename.append(".txt");
		FILE* temp = fopen(_strFilename.c_str(), "w");

		fprintf(temp, strTotal.c_str());

		fclose(temp);

		return DoString(strTotal.c_str());
	}
	else
	{
		//이전에 저장된 내용이 있다는 뜻이므로 실행시켜 줍니다.
		return DoString(m_mRequireMap[_strFilename].c_str());
	}
}

void KGCLuabinder_Map::RequireFile(const char* _strFileName)
{
	//GCFUNC::LoadLuaScript( KGCLuabinder_Map::getInstance(), _strFileName );
	GCMemoryFile MemFile(g_pGCDeviceManager->GetMassFileManager(), _strFileName);

	if (!MemFile.IsLoaded())
		return;

	if (FAILED(KGCLuabinder_Map::getInstance().DoString(MemFile.GetDataPointer())))
	{
		MessageBoxA(NULL, "RequireFile Load Failed!", NULL, MB_OK);
		exit(0);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//#include ".\KGCLuabinder_Map.h"

KGCLuaManager* KGCLuaManager::m_pKGCLuaManager = NULL;

KGCLuaManager::KGCLuaManager(void)
{
}
KGCLuaManager::KGCLuaManager(lua_State* pLuaState)
	:KLuaManager(pLuaState)
{
}

KGCLuaManager::~KGCLuaManager(void)
{
}

KGCLuaManager& KGCLuaManager::getInstance()
{
	if (m_pKGCLuaManager == NULL)
		m_pKGCLuaManager = new KGCLuaManager();

	return *m_pKGCLuaManager;
}

//KGCLuaManager& KGCLuaManager::getInstance(lua_State* pLuaState)
//{
//	if( m_pKGCLuaManager == NULL )
//		m_pKGCLuaManager = new KGCLuaManager(pLuaState);
//
//	return *m_pKGCLuaManager;
//}

void KGCLuaManager::release()
{
	SAFE_DELETE(m_pKGCLuaManager);
}

HRESULT KGCLuaManager::DoFile(string _strFilename, string _strRequire)
{
	map< string, string > ::const_iterator m_iter_IsExist;

	m_iter_IsExist = m_mRequireMap.find(_strFilename);

	if (m_iter_IsExist == m_mRequireMap.end())
	{
		//스트링의 덩어리를 만듭니다.
		string strTotal;
		string strTemp;

		//init
		strTotal.clear();
		strTemp.clear();

		GCMemoryFile Memfile(g_pGCDeviceManager->GetMassFileManager(), _strFilename);

		if (Memfile.IsLoaded())
			strTotal = Memfile.GetDataPointer();

		strTotal.insert(0, strTemp);

		//Require들 부터 차례로 호출해 줍니다.
		GCMemoryFile Memfile2(g_pGCDeviceManager->GetMassFileManager(), _strRequire);

		if (Memfile2.IsLoaded())
			strTemp = Memfile2.GetDataPointer();

		strTotal.insert(0, strTemp);

		//스트링의 덩어리를 저장합니다.
		m_mRequireMap[_strFilename] = strTotal;

		//뭉쳐서 루아를 실행시켜 줍니다.
		return DoString(strTotal.c_str());
	}
	else
	{
		//이전에 저장된 내용이 있다는 뜻이므로 실행시켜 줍니다.
		return DoString(m_mRequireMap[_strFilename].c_str());
	}

	return S_OK;
}

void KGCLuaManager::RequireFile(const char* _strFileName)
{
	GCMemoryFile MemFile(g_pGCDeviceManager->GetMassFileManager(), _strFileName);

	if (!MemFile.IsLoaded())
		return;

	if (FAILED(KGCLuabinder_Map::getInstance().DoString(MemFile.GetDataPointer())))
	{
		MessageBoxA(NULL, "RequireFile Load Failed!", NULL, MB_OK);
		exit(0);
	}
}