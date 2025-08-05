#include "stdafx.h"
#include "GCMonsterCardManager.h"

KGCMonsterCardMix::KGCMonsterCardMix()
{
	m_mapGradePoint.clear();
	m_mapPointReward.clear();
	m_mapRewardGroup.clear();
}

KGCMonsterCardMix::~KGCMonsterCardMix()
{

}

int KGCMonsterCardMix::GetGradePoint( int _nGrade )
{
	MAP_GRADE_POINT_ITER pos = m_mapGradePoint.find( _nGrade );
	if ( pos != m_mapGradePoint.end() ) 
		return pos->second;

	return 0;
}

void KGCMonsterCardMix::GetPointReward( int nPoint, std::vector<USHORT>& vecRewardGroup )
{
	vecRewardGroup.clear();

	MAP_POINT_REWARD_ITER pos = m_mapPointReward.begin();
	MAP_POINT_REWARD_ITER pos_end = m_mapPointReward.end();

	for ( ; pos != pos_end; ++pos )
	{
		PAIR_USHORT pair = pos->first;

		if ( pair.first <= nPoint && pair.second >= nPoint )
		{
			vecRewardGroup = pos->second;
			break;
		}
	}
}

void KGCMonsterCardMix::GetMonsterCardRewardList( const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward )
{
	int nPoint = 0;

	_vecReward.clear();

	for ( int i = 0; i < (int)_vecMaterial.size(); ++i ) 
	{
		GCItem* pItem = g_pItemMgr->GetItemData( _vecMaterial[i] );
		if ( !pItem ) 
			continue;

		nPoint += GetGradePoint( (int)pItem->cItemGrade );
	}

	std::vector<USHORT> vecRewardGroup;
	GetPointReward( nPoint, vecRewardGroup );


	for ( int i = 0; i < (int)vecRewardGroup.size(); ++i ) 
	{
		USHORT usGroup = vecRewardGroup[i];

		MAP_REWARD_GROUP_ITER pos = m_mapRewardGroup.find( usGroup );
		if ( pos != m_mapRewardGroup.end() )
		{
			std::vector<GCITEMID>& vecItem = pos->second;

			for ( int i = 0; i < (int)vecItem.size(); ++i ) 
			{
				_vecReward.push_back( vecItem[i] );
			}
		}
	}
}


KGCMonsterCardManager::KGCMonsterCardManager()
{
	m_vecMonsterCardList.clear();
}

KGCMonsterCardManager::~KGCMonsterCardManager()
{
}

void KGCMonsterCardManager::InitMonsterCard()
{
	LoadMonsterCardList();
}

void KGCMonsterCardManager::LoadMonsterCardList()
{
	KLuaManager luaMgr;
	
	if( GCFUNC::LoadLuaScript( luaMgr , "monster_card_list.lua" ) == false )
		return ;

	m_vecMonsterCardList.clear();

	LUA_BEGIN_TABLE( "MONSTER_CARD_LIST", return );
	{
		for(int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE( i, break )
			{
				int iItemID = -1;
				int iShow = 0;
				LUA_GET_VALUE_DEF( "GOODSID", iItemID, -1 );
				LUA_GET_VALUE_DEF( "SHOW", iShow, 0 );

				GCItem* pItem = g_pItemMgr->GetItemData( iItemID );
				if( pItem && 1 == iShow )
				{
					m_vecMonsterCardList.push_back( pItem );
				}
			}
			LUA_END_TABLE( break )
		}
	}
	LUA_END_TABLE( return )
}

void KGCMonsterCardManager::SetMonsterCardInfo( KEVENT_MONSTER_CARD_MIX_INFO_ACK kPacket )
{
    m_kMonsterCardMix.m_mapGradePoint = kPacket.m_mapGradePoint;
    m_kMonsterCardMix.m_mapPointReward = kPacket.m_mapPointReward;
    m_kMonsterCardMix.m_mapRewardGroup = kPacket.m_mapRewardGroup;
}

void KGCMonsterCardManager::GetMonsterCardRewardList( const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward )
{
	m_kMonsterCardMix.GetMonsterCardRewardList( _vecMaterial, _vecReward );
}