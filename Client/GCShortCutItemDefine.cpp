#include "stdafx.h"
//
#include "GCShortCutItemDefine.h"

//////////////////////////////////////////////////////////////////////////
//					   포션 & 버프 계열 Item의 정보						//
//////////////////////////////////////////////////////////////////////////
KGCShortCutItemDefine::KGCShortCutItemDefine()
{
	initItemTable();
}

KGCShortCutItemDefine::~KGCShortCutItemDefine()
{
	initItemTable();
}

void KGCShortCutItemDefine::LoadItem()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
		return;

	if( GCFUNC::LoadLuaScript( luaMgr, "ShortCutItem.stg" ) == false )
		return;

	initItemTable();

	luaMgr.BeginTable( "POTION_LIST" );
	{
		int iIndex = 1;

		int	 iItemID = 0;
		SHORTCUT_ITEM PotionBuff_Structure;
		while( true )
		{
			if( FAILED( luaMgr.BeginTable( iIndex ) ) )	break;
			{
				PotionBuff_Structure.clear();

				luaMgr.GetValue("ItemID",					iItemID);
				luaMgr.GetValue("ItemType",			        PotionBuff_Structure.iItemType);
				luaMgr.GetValue("RemainTime",		    	PotionBuff_Structure.iRemainTime);
				luaMgr.GetValue("CoolTime",			    	PotionBuff_Structure.iCoolTime);
				luaMgr.GetValue("HP_RecoveryRate",	    	PotionBuff_Structure.fHP_RecoveryRate);
                luaMgr.GetValue("MP_RecoveryRate",	    	PotionBuff_Structure.fMP_RecoveryRate);
                luaMgr.GetValue("PetMp",			        PotionBuff_Structure.fPetMp);
                luaMgr.GetValue("Attack",			    	PotionBuff_Structure.fAttack);
                luaMgr.GetValue("Defense",			    	PotionBuff_Structure.fDefense);
                luaMgr.GetValue("SystemMessage",			PotionBuff_Structure.iSystemMessage);
				LUA_GET_VALUE_DEF( "Cancelation",           PotionBuff_Structure.bCancelation, false );
				LUA_GET_VALUE_DEF( "BuffIndex",             PotionBuff_Structure.iBuffIndex, 0 );
				LUA_GET_VALUE_DEF( "BuffTime",              PotionBuff_Structure.fBuffTime, 0.0f );
				LUA_GET_VALUE_DEF( "PercentageRecovery",    PotionBuff_Structure.bPercentageRecovery, false );
                LUA_GET_VALUE_DEF( "EmoticonGoodsID",       PotionBuff_Structure.iEmoticonGoodsID, 0 );
				LUA_GET_VALUE_DEF( "LevelDesignRecovery",   PotionBuff_Structure.bLevelDesignRecovery, false );
                LUA_GET_VALUE_DEF( "DamageEnumRight",       PotionBuff_Structure.iDamageEnumRight, -1 );
                LUA_GET_VALUE_DEF( "DamageEnumLeft",        PotionBuff_Structure.iDamageEnumLeft, -1 );
                LUA_GET_VALUE_DEF( "SummonMonsterID",       PotionBuff_Structure.dwSummonMonsterID, 0 );
                LUA_GET_VALUE_DEF( "DamageOREmoticonAppearanceRate",PotionBuff_Structure.fDamageOREmoticonAppearanceRate, -1.f);
				LUA_GET_VALUE_DEF( "EquipLimit",			PotionBuff_Structure.iEquipLimit, -1 );

				if( SUCCEEDED( luaMgr.BeginTable( "SkillMotionID" ) ) )
				{
					for(int i = 1; SUCCEEDED( luaMgr.BeginTable( i )) ; ++i)
					{
						int iFormID, iMotionID;
						LUA_GET_VALUE_DEF( "FormID", iFormID, -1);
						LUA_GET_VALUE_DEF( "MotionID", iMotionID , -1);

						if( -1 != iFormID && -1 != iMotionID)
							PotionBuff_Structure.m_mapSkillMotion.insert( std::map<int, int>::value_type(iFormID, iMotionID));
						luaMgr.EndTable();
					}
					luaMgr.EndTable();
				}

				if( FAILED( luaMgr.GetValue( "Sound", PotionBuff_Structure.strSound ) ) )
					PotionBuff_Structure.strSound.clear();

				if (FAILED(luaMgr.GetValue("SoundAll", PotionBuff_Structure.strSoundAll)))
					PotionBuff_Structure.strSoundAll.clear();

				if( SUCCEEDED( luaMgr.BeginTable( "UseGameMode" ) ) )
				{
					for(int i = 1; ; ++i)
					{
						int iGameMode;
						if(FAILED( luaMgr.GetValue( i, iGameMode ) ) )
						{
							break;
						}

						PotionBuff_Structure.m_setGameModes.insert( (EGCGameMode)iGameMode );
					}
					luaMgr.EndTable();
				}

				// Insert to Map container
				m_Map_Item[iItemID] = PotionBuff_Structure;
			}
			luaMgr.EndTable();
			
			iIndex++;
		}
	}
	luaMgr.EndTable();
}
void KGCShortCutItemDefine::initItemTable()
{
	m_Map_Item.clear();
}


int KGCShortCutItemDefine::GetItemKind( DWORD _dwItemID )
{
	std::map<int,SHORTCUT_ITEM>::iterator map_Iter;

	map_Iter = m_Map_Item.find( static_cast<int>(_dwItemID) );
	// Key값에 맞는 정보가 없다면
	if( map_Iter == m_Map_Item.end() )
	{
		return (-1);
	}

	int a = ((*map_Iter).second).iItemType;
	return ((*map_Iter).second).iItemType;
}

// Key(ItemID)값을 받아 해당 Item 정보를 가진 구조체를 반환.
bool KGCShortCutItemDefine::GetStructure( int _iItemID, SHORTCUT_ITEM* &_structure )
{
	std::map<int,SHORTCUT_ITEM>::iterator map_Iter;
	
	map_Iter = m_Map_Item.find( _iItemID );
	// Key값에 맞는 정보가 없다면
	if( map_Iter == m_Map_Item.end() )
	{
		return false;
	}
	
	_structure = &((*map_Iter).second);
	return true;
}
