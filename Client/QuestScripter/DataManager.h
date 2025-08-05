#pragma once
#include "struct.h"
//#include <vector>

#define MAX_UNDO 100

class CDataManager
{
public:
	CDataManager(void);
	~CDataManager(void);

	int m_iCnt;                         // 백업한 총 갯수.
	int m_index;                        // 인덱스

	std::vector<Quest*> m_vtQuestHistory;

	void AddQuest(Quest *quest);        // 데이터를 백업해둔다.
	bool NextQuest(Quest *quest);       // 벡터에서 다음인덱스의 데이터를 가져온다.
	bool PrevQuest(Quest *quest);       // 벡터에서 이전인덱스의 데이터를 가져온다.
	void Clear(void);                   // 벡터의 백업데이터를 모두 날린다.
};
