#include "stdafx.h"
//
#include "GCSKT.h"

//
//
#include "GCSkill.h"

//

void GCSKT::_D_TutorialAllTrainSkill()
{
    m_mapTrainedSkill.clear();

    std::map< EGCSkillTree, GCSkill* >::iterator vecIter = m_mapSkill.begin();
    for(; vecIter != m_mapSkill.end(); ++vecIter)
    {
        if( vecIter->second || vecIter->second->GetID() < SID_MAX )
        {
            EGCCharType eCharType = vecIter->second->GetCharType();
            int iPromotion = vecIter->second->GetPromotion();
            IterTrainedSK mit = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(eCharType, iPromotion) );        
            if( mit == m_mapTrainedSkill.end() )
            {
                SkillSet TempSet;
                TempSet.insert( vecIter->first );
                m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(eCharType, iPromotion), TempSet ) );
            }
            else
            {
                mit->second.insert( vecIter->first );
            }
        }
    }
}

void GCSKT::_D_TrainSkill( EGCCharType eCharType, int nPromotion, EGCSkillTree eSkill )
{
	IterTrainedSK mit = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(eCharType, nPromotion) );
	if( mit == m_mapTrainedSkill.end() )
	{
		SkillSet TempSet;
		TempSet.insert( eSkill );
		m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(eCharType, nPromotion), TempSet ) );
	}
	else
	{
		mit->second.insert( eSkill );
	}
}

void GCSKT::_D_UnTrainSkill( EGCCharType eCharType, int nPromotion, EGCSkillTree eSkill )
{
	IterTrainedSK mit = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(eCharType, nPromotion) );
	if( mit == m_mapTrainedSkill.end() )
	{
		return;
	}
	else
	{
		SkillSet::iterator sit = mit->second.find( eSkill );
		if( sit != mit->second.end() )
		{
			mit->second.erase( sit );
		}
	}
}

void GCSKT::_D_UnAllTrainSkill( EGCCharType eCharType, int nPromotion )
{
    IterTrainedSK mit = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(eCharType, nPromotion) );
    if( mit == m_mapTrainedSkill.end() )
    {
        return;
    }
    else
    {
        mit->second.clear();
    }
}

void GCSKT::_D_Equip_Skill( int iCharType, int nPromotion, int nSetting, int nID )
{
	GCSkill* pSkill = NULL;
	IterSkill mit = m_mapSkill.find( (EGCSkillTree)nID );
	if( mit == m_mapSkill.end() )
	{
		return;
	}
	else
	{
		pSkill = mit->second;
		if( pSkill->GetCharType() != iCharType )
		{
			return;
		}
		if( pSkill->GetPromotion() != nPromotion )
		{
			return;
		}
	}
	assert( pSkill != NULL );
	int nGroupID = (int)pSkill->GetGroupID();

	std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
	std::vector<KSPSetInfo>::iterator vit;
	vit = std::find_if( kSPInfo.begin(), kSPInfo.end(), 
		boost::bind( &KSPSetInfo::m_nSPSetID, _1 ) == nSetting );
	if( vit == kSPInfo.end() )
	{
		KSPSetInfo kEquip;        
		kSPInfo.push_back( kEquip );
	}

	EquipSkill( (EGCSkillTree)nID, nSetting );
}

void GCSKT::_D_UnEquip_Skill( int iCharType, int nPromotion, int nSetting, int nID )
{
	GCSkill* pSkill = NULL;
	IterSkill mit = m_mapSkill.find( (EGCSkillTree)nID );
	if( mit == m_mapSkill.end() )
	{
		return;
	}
	else
	{
		pSkill = mit->second;
		if( pSkill->GetCharType() != iCharType )
		{
			return;
		}
		if( pSkill->GetPromotion() != nPromotion )
		{
			return;
		}
	}
	assert( pSkill != NULL );
	int nGroupID = (int)pSkill->GetGroupID();

	std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
	std::vector<KSPSetInfo>::iterator vit;
	vit = std::find_if( kSPInfo.begin(), kSPInfo.end(), 
		boost::bind( &KSPSetInfo::m_nSPSetID, _1 ) == nSetting );
	if( vit != kSPInfo.end() )
	{
		KSPSetInfo& kEquip = (*vit);
		std::vector<KSkillSlot>::iterator vit;
		vit = std::find_if( kEquip.m_vecSkills.begin(), kEquip.m_vecSkills.end(),
			boost::bind( &KSkillSlot::m_nSlotID, _1 ) == nGroupID );
		if( vit != kEquip.m_vecSkills.end() )
		{
			kEquip.m_vecSkills.erase( vit );
		}
	}
}

void GCSKT::_D_UnAllEquip_Skill( int iCharType, int nPromotion, int nSetting )
{
    std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
    std::vector<KSPSetInfo>::iterator vecIter = kSPInfo.begin();
    for(; vecIter != kSPInfo.end(); ++vecIter)
    {
        std::vector<KSkillSlot>::iterator vecSlot = vecIter->m_vecSkills.begin();
        for(; vecSlot != vecIter->m_vecSkills.end(); ++vecSlot)
        {
            if( vecSlot->m_nSkillID )
            {
                vecIter->m_vecSkills.clear();
                break;
            }
        }
    }
}

#if defined( _DEBUG ) || !defined( __PATH__ )

void GCSKT::_D_InsertLocalPlayerSkillSet( DWORD dwUID, EGCCharType eCharType, int nPromotion )
{
    IterTrainedSK mit = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(eCharType, nPromotion) );
    if( mit == m_mapTrainedSkill.end() )
        return;
    InsertUserSkillSet( dwUID,  mit->second );
}

void GCSKT::_D_UnTrainSkillReqtoServer( int nSkill )
{
    //KP2P::GetInstance()->Send_RemoveSkill( nSkill );
}

void GCSKT::_D_TrainSkillReqtoServer( int nSkill )
{
    KP2P::GetInstance()->Send_SkillTrainingReq( nSkill );
}

void GCSKT::_D_Dump_Setting( int iCharType, int nPromotion )
{
    std::map< std::pair<char,char>, int >::iterator mit = 
        m_mapSetting.find( std::pair<char,char>((char)iCharType, (char)nPromotion) );
    if( mit == m_mapSetting.end() )
        return;

    std::wstringstream stm;
    std::vector<KSkillSlot>& vecSlot = g_kGlobalValue.m_kUserInfo.GetCurrentChar( iCharType ).m_mapEquipSkill[nPromotion];
    std::vector<KSkillSlot>::iterator vit;
    for( vit = vecSlot.begin(); vit != vecSlot.end(); ++vit )
    {
        stm << L"Set ID : " << vit->m_nSlotID << L", Skill ID : " << vit->m_nSkillID << std::endl;
    }
    START_LOG( clog, iCharType << L", " << nPromotion << L" 의 Skill세팅 : " << mit->second )
        << BUILD_LOG( stm.str() );
}

void GCSKT::_D_Dump_Skill( int iCharType, int nPromotion )
{
    std::map< std::pair<EGCCharType, char>, SkillSet >::iterator mit = 
        m_mapTrainedSkill.find( std::pair<EGCCharType, char>((EGCCharType)iCharType, (char)nPromotion) );
    if( mit == m_mapTrainedSkill.end() )
        return;

    std::wstringstream stm;
    SkillSet& setSkill = mit->second;
    SkillSet::iterator sit = setSkill.begin();
    while( sit != setSkill.end() )
    {
        stm << (*sit) << ", ";
        ++sit;
    }
    START_LOG( clog, iCharType << L", " << nPromotion << L" 의 Skill" )
        << BUILD_LOG( stm.str() );
}

void GCSKT::_D_Set_Setting( int iCharType, int nPromotion, int nSetting )
{
    m_mapSetting[ std::pair<char,char>((char)iCharType, (char)nPromotion) ] = nSetting;
}

void GCSKT::_D_Apply_SettingChangetoServer()
{
    KChangeSkillSet kChange;
    kChange.m_dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kChange.m_mapSetting = m_mapSetting;
    kChange.m_mapSkillSets = m_mapSkillSets;

    KP2P::GetInstance()->Send_SettingSkills( kChange );
}

// 튜토리얼에서 테스트하기 위한 바인드
void GCSKT::RegisterLuabind()
{
	//====================================================================================
    // 2009.01.30 : Jemitgge
    // Comment : 튜토리얼 테스트 하려면 이거 빼쟈!

	//return; //스킬 - 튜토리얼테스트
        lua_tinker::class_def<GCSKT>( KGCLuabinder::getInstance().GetLuaState(),  "TrainSkill",  &GCSKT::TrainSkill  );
        lua_tinker::class_def<GCSKT>( KGCLuabinder::getInstance().GetLuaState(),  "Launch",  &GCSKT::Launch  );
        ;

    lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "UserSkill", this );

    IterTrainedSK mit2 = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(GC_CHAR_SIEG, 0) );
	//IterTrainedSK mit2 = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(GC_CHAR_ARME, 2) );
    if( mit2 != m_mapTrainedSkill.end() )
    {
        m_mapTrainedSkill.erase( mit2 );
    }
	//GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , "Enum.stg" );
	//GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , "MotionEnum.stg" );
	//GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , "DamageEnum.stg" );
	//GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , "FireInfo.stg" );

	//std::string strInitSkillTree = "InitSkillTree";

//#ifndef NATION_KOREA
//	strInitSkillTree += "_";
//	strInitSkillTree += KncUtil::toNarrowString( NATION_CODE );
//
//	strInitSkillTree = strInitSkillTree.substr( 0, 16 );
//#endif

	//strInitSkillTree += ".lua";

    for( int i = 0; i < GC_CHAR_NUM; i++ ) {
        for( int j = 0; j < NUM_JOB_LEVEL; j++ ) {
            std::stringstream strSkillScript;
            strSkillScript << "InitSkillTree" << i << "_" << j << ".lua";
            GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strSkillScript.str() );
        }
    }
	
	m_mapSkillUsers.clear();
    GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "SkillTest.lua" );
    GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), "TestFunc()" );
}

void GCSKT::TrainSkill( int nSkill )
{
    //_D_TrainSkill( GC_CHAR_AMY, 0, (EGCSkillTree)nSkill );
    //_D_TrainSkill( GC_CHAR_ARME, 0, (EGCSkillTree)nSkill );
    _D_TrainSkill( GC_CHAR_SIEG, 0, (EGCSkillTree)nSkill );
}

void GCSKT::Launch()
{
    //_D_InsertLocalPlayerSkillSet( g_kGlobalValue.m_kUserInfo.dwUID, GC_CHAR_AMY, 0 );
	//_D_InsertLocalPlayerSkillSet( g_kGlobalValue.m_kUserInfo.dwUID, GC_CHAR_ARME, 0 );
    _D_InsertLocalPlayerSkillSet( g_kGlobalValue.m_kUserInfo.dwUID, GC_CHAR_SIEG, 0 );
    LoadingResource( g_kGlobalValue.m_kUserInfo.dwUID );
    g_KDSound.FullLoadfromLoadList();
}

bool GCSKT::_D_ReloadUIinfo( KLuaManager& luaMgr )
{
	LUA_BEGIN_TABLE( "SkillTree", return false )
	{
		for( int i = 1; ; ++i )
		{
			LUA_BEGIN_TABLE( i, break )
			{
				int nTempVal;
				LUA_GET_VALUE( "ID", nTempVal, return false );

				if( m_mapSkill.find( (EGCSkillTree)nTempVal ) == m_mapSkill.end() )
				{
					LUA_END_TABLE( return false );
					continue;
				}

				GCSkill* pNewSkill = m_mapSkill[(EGCSkillTree)nTempVal];
				pNewSkill->LoadUI( luaMgr );
			}
			LUA_END_TABLE( return false );
		}
	}
	LUA_END_TABLE( return false );

	g_pkUIScene->m_pkSkillTree->Update();
    return true;
}

void GCSKT::_D_UnlockGroupReqtoServer( int nGroup )
{
    KP2P::GetInstance()->Send_UnLockSkill( nGroup );
}
#endif
