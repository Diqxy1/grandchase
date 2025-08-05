#pragma once

typedef std::map<int,USHORT>		MAP_GRADE_POINT;
typedef MAP_GRADE_POINT::iterator	MAP_GRADE_POINT_ITER;

typedef std::map<PAIR_USHORT,std::vector<USHORT>>		MAP_POINT_REWARD;
typedef MAP_POINT_REWARD::iterator						MAP_POINT_REWARD_ITER;

typedef std::map<USHORT,std::vector<GCITEMID>>			MAP_REWARD_GROUP;
typedef MAP_REWARD_GROUP::iterator						MAP_REWARD_GROUP_ITER;

class KGCMonsterCardMix
{
public:
	KGCMonsterCardMix();
	~KGCMonsterCardMix();

public:
	int GetGradePoint( int _nGrade );
	
	void GetPointReward( int nPoint, std::vector<USHORT>& vecRewardGroup );

	void GetMonsterCardRewardList( const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward );
	
public:
	MAP_GRADE_POINT		m_mapGradePoint; // 카드등급별 포인트  map[등급,포인트]
	MAP_POINT_REWARD	m_mapPointReward; // 획득 카드 그룹 확률  map[포인트범위,보상확률]
	MAP_REWARD_GROUP    m_mapRewardGroup; // 그룹별 카드 구성  map[그룹넘버,보상확률]
};


class KGCMonsterCardManager
{
public:
	KGCMonsterCardManager();
	~KGCMonsterCardManager();

public:
	void InitMonsterCard();
	void LoadMonsterCardList();

    void SetMonsterCardInfo( KEVENT_MONSTER_CARD_MIX_INFO_ACK kPacket );

	std::vector< GCItem* > GetMonsterCardList() { return m_vecMonsterCardList; }
	void GetMonsterCardRewardList( const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward );

public:
	std::vector< GCItem* > m_vecMonsterCardList;

	KGCMonsterCardMix	   m_kMonsterCardMix;
};