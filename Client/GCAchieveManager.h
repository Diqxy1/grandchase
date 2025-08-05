#pragma once
#include <KNCSingleton.h>

struct SAchieve {
	enum {
		INVALID = 0,
#undef _ENUM
#include "GCAchieveDef.h"
	};

	SAchieve() {
		m_iActionID = INVALID;
		m_iCharType = GC_CHAR_INVALID;
		m_iParam = -1;
		m_iParam2 = -1;
	}

	int m_iActionID;
	int m_iCharType;
	int m_iParam;
	int m_iParam2;
};

class KGCAchieveManager
{
	DeclareSingleton(KGCAchieveManager);
public:
	KGCAchieveManager(void);
	~KGCAchieveManager(void);

	enum TEST_ACHIEVE {
	};

	void OccurAction(int iActionID_, int iCharType_, int iParam_ = -1, int iParam2_ = -1);
	void OccurAction(const SAchieve& arg_);

	void LoadScript();

#ifndef __PATH__
	const char* GetActionDesc(int iActionID_);
#endif

	struct SAchieveReward {
		int     m_iTagID;
		int     m_iGP;
		KBuySellItemReq     m_Item;
	};
	struct SAchieveInfo {
		int     m_iAchieveID;
		int     m_iCompleteCount;
		int     m_iCounterID;
		std::vector<SAchieveReward>     m_vecReward;
	};
private:
	void BindToLua(lua_State* L);
	void AddToCounter(int iCounterID_);

	std::map<int, int>       m_mapAchieveCounter;
};

DefSingletonInline(KGCAchieveManager);
