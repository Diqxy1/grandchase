#pragma once

#include "KNCSingleton.h"

struct SDungeonItemInfo
{
	int m_iDungeonID;
	int m_iDifficult;
	std::vector<GCITEMID> m_vecItemInfo;
	SDungeonItemInfo(){
		m_iDungeonID = -1;
		m_iDifficult = 0;
	}
};
class KGCDungeonInfoManager
{
    DeclareSingleton( KGCDungeonInfoManager );

public:
    KGCDungeonInfoManager(void);
    ~KGCDungeonInfoManager(void);

	void LoadStringFile();
	void LoadScriptFile();

	SDungeonItemInfo GetDungeonInfo( int dungeonID,int dungeonDiff );
private:
	std::map<DWORD,std::wstring> m_mapDropItemName;
	//key<던전번호,난이도>, Item 스트링번호 m_mapDropItemName
	std::map<std::pair<int,int>,SDungeonItemInfo> m_mapDropItem;
	
};

DefSingletonInline( KGCDungeonInfoManager );
