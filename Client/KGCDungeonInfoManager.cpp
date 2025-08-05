#include "KGCDungeonInfoManager.h"

ImplementSingleton( KGCDungeonInfoManager )

KGCDungeonInfoManager::KGCDungeonInfoManager(void)
{
}

KGCDungeonInfoManager::~KGCDungeonInfoManager(void)
{
}

void KGCDungeonInfoManager::LoadStringFile()
{
	SiKGCMultipleLanguages()->LoadStringFile_AutoNationFileName("str_dungeon_drop_item",m_mapDropItemName);

	std::map<DWORD,std::wstring>::iterator mit = m_mapDropItemName.begin();
	for(;mit != m_mapDropItemName.end();++mit){
		if(mit->second.empty()) continue;
		int index = mit->second.size() - 1;

		if(mit->second[index] == L'\n' ){
			mit->second.erase(index);
		}
	}
}

void KGCDungeonInfoManager::LoadScriptFile()
{
	KLuaManager luaMgr;

	std::vector<std::string> vecFileList;
	//대륙별 파일리스트 가져오기	
	if( GCFUNC::LoadLuaScript( luaMgr , "DungeonDropItemList.stg" ) != false )
	{
		std::string strFileName;
		LUA_BEGIN_TABLE( "Table", return;)
		{
			for( int i = 1; ; i++ ){
				strFileName.clear();
				LUA_GET_VALUE_NOASSERT(i,strFileName,break);
				vecFileList.push_back(strFileName);
			}
		}
		LUA_END_TABLE(/* Table */;);
	}

	//대륙별 던전 아이템 정보 가져오기
	for(std::vector<std::string>::iterator vIt = vecFileList.begin(); vIt != vecFileList.end(); ++vIt){
		if( GCFUNC::LoadLuaScript( luaMgr , vIt->c_str() ) != false )
		{
			std::string strFileName;
			LUA_BEGIN_TABLE( "Table", return;)
			{
				for( int i = 1; ; i++ ){
					SDungeonItemInfo temp;
					LUA_BEGIN_TABLE( i, break;)
					{
						LUA_GET_VALUE_NOASSERT("DUNGEON_ID",temp.m_iDungeonID,break);
						LUA_GET_VALUE_NOASSERT("DIFFICULT",temp.m_iDifficult,break);


						LUA_BEGIN_TABLE( "ITEM_ID" , ; )
						{
							for( int j = 1; ; j++ ){
								GCITEMID giItemID = 0;
								LUA_GET_VALUE_NOASSERT(j, giItemID, break;);
								temp.m_vecItemInfo.push_back(giItemID/10);
							}
						}
						LUA_END_TABLE(/* ITEM_STR_ID */;);
					}
					LUA_END_TABLE(/* i */;);
					m_mapDropItem.insert(std::pair<std::pair<int,int>,SDungeonItemInfo>(std::pair<int,int>(temp.m_iDungeonID, temp.m_iDifficult),temp));
				}
			}
			LUA_END_TABLE(/* Table */;);
		}
	}
}

SDungeonItemInfo KGCDungeonInfoManager::GetDungeonInfo( int dungeonID,int dungeonDiff )
{
	SDungeonItemInfo result;
	std::map<std::pair<int,int>,SDungeonItemInfo>::iterator mit = m_mapDropItem.find(std::pair<int,int>(dungeonID,dungeonDiff));
	if ( mit == m_mapDropItem.end() ) {
		return result;
	}

	return mit->second;
}


