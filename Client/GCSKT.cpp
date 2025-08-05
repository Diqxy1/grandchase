#include "stdafx.h"
//
#include ".\gcskt.h"
#include "GCSkill.h"
#include "GCSkillResource.h"
#include "../gcui/GCUIScene.h"
#include "../gcui/GCSkillTreeDlg.h"
//
#include "GCUI/GCToolTip.h"

//
//

//

#include "Replay.h"

bool bReloadSKT = false;
namespace {
	class FindSkillInGroup{
	private:
		EGCSkillGroup m_iGroupID;
		FindSkillInGroup() {}
	public:
		FindSkillInGroup( EGCSkillGroup iGroup ) { m_iGroupID = iGroup; }
		~FindSkillInGroup() {}

		bool operator() ( const KSkillSlot& arg )
		{
			const GCSkill* pSkill = SiGCSKT()->GetSkill( (EGCSkillTree)arg.m_nSkillID );

			if( pSkill == NULL )
				return false;

			if( pSkill->GetGroupID() == m_iGroupID )
				return true;

			return false;
		}
	};

	class FindSkill{
	private:
		EGCSkillTree m_eSkillID;
		FindSkill() {}
	public:
		FindSkill( EGCSkillTree eSkillID ) { m_eSkillID = eSkillID; }
		~FindSkill() {}

		bool operator() ( const KSkillSlot& arg )
		{
			const GCSkill* pSkill = SiGCSKT()->GetSkill( (EGCSkillTree)arg.m_nSkillID );

			if( pSkill == NULL )
				return false;

			if( pSkill->GetID() == m_eSkillID )
				return true;

			return false;
		}
	};

	class SortSkillByLevel{
	public:
		SortSkillByLevel() {}
		~SortSkillByLevel() {}

		bool operator() ( const GCSkill* left, const GCSkill* right )
		{
			return left->GetLv() < right->GetLv();
		}
	};
}

ImplementSingleton( GCSKT );


#include "GCUtil/GCFileMoniter.h"

void InitSkillTreeFileMoniter()
{
    bReloadSKT = true;    
}

GCSKT::GCSKT(void)
{
    if( g_pGCDeviceManager2 ) {
        for( int i = 0; i < GC_CHAR_NUM; i++ ) {
            for( int j = 0; j < NUM_JOB_LEVEL; j++ ) {
                std::stringstream strSkillScript;
                strSkillScript << "InitSkillTree" << i << "_" << j << ".lua";
                SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( strSkillScript.str() ) , InitSkillTreeFileMoniter );
            }
        }		
    }

    clear();

	m_cCurUICharType = 0;
	m_cCurUIPromotion = 0;

	//m_mapCashSkillPosition[SG_ELESIS1_CASH_SPECIAL1] = ESP_SKC_WHITE_ONE;
	//m_mapCashSkillPosition[SG_ELESIS1_CASH_SPECIAL2] = ESP_SKC_WHITE_TWO;
	//m_mapCashSkillPosition[SG_ELESIS1_CASH_SPECIAL3] = ESP_SKC_WHITE_THREE;
	m_mapCashSkillPosition[SG_ARME_WHITE_SPECIAL1_CASH] = ESP_SKC_WHITE_ONE;
	m_mapCashSkillPosition[SG_ARME_WHITE_SPECIAL2_CASH] = ESP_SKC_WHITE_TWO;
	m_mapCashSkillPosition[SG_ARME_WHITE_SPECIAL3_CASH] = ESP_SKC_WHITE_THREE;
	m_mapCashSkillPosition[SG_ARME_BLACK_SPECIAL1_CASH] = ESP_SKC_DARK_ONE;
	m_mapCashSkillPosition[SG_ARME_BLACK_SPECIAL2_CASH] = ESP_SKC_DARK_TWO;
	m_mapCashSkillPosition[SG_ARME_BLACK_SPECIAL3_CASH] = ESP_SKC_DARK_THREE;
    m_mapCashSkillPosition[SG_LIRE1_CASH_SPECIAL1] = ESP_SKC_WHITE_ONE;
    m_mapCashSkillPosition[SG_LIRE1_CASH_SPECIAL2] = ESP_SKC_WHITE_TWO;
    m_mapCashSkillPosition[SG_LIRE1_CASH_SPECIAL3] = ESP_SKC_WHITE_THREE;
    m_mapCashSkillPosition[SG_LAS1_CASH_SPECIAL1] = ESP_SKC_WHITE_ONE;
    m_mapCashSkillPosition[SG_LAS1_CASH_SPECIAL2] = ESP_SKC_WHITE_TWO;
    m_mapCashSkillPosition[SG_LAS1_CASH_SPECIAL3] = ESP_SKC_WHITE_THREE;

	m_mapCashSkillPosition[SG_RYAN1_CASH_SPECIAL1] = ESP_SKC_WHITE_ONE;
	m_mapCashSkillPosition[SG_RYAN1_CASH_SPECIAL2] = ESP_SKC_WHITE_TWO;
	m_mapCashSkillPosition[SG_RYAN1_CASH_SPECIAL3] = ESP_SKC_WHITE_THREE;

	m_bNewSkillMsgRender = false;

    m_iMaxSID = 0;
    BindToLua();

	m_pLocalPlayer = &g_MyD3D->m_TempPlayer;

	m_bCharacterSelectable = true;
	m_bPromotionSelectable = true;
	m_bUnlockAll = false;

#if defined( USE_AP_SLOT_CHANGE ) 
    for( int i = 0; i < KSkillSlot::INDEX_MAX; i++ ) {
        m_iHotKeyIndex[i] = 0;
    }
#else
    m_iHotKeyIndex = 0;
#endif

    m_iCurSlotType = KSkillSlot::INDEX_A;

}

GCSKT::~GCSKT(void)
{
    clear();
}

const std::wstring& GCSKT::GetString( int iID )
{
    return m_kSkillStr.GetString( iID );
}

bool GCSKT::LoadSkillScript( KLuaManager& luaMgr )
{
    LUA_GET_VALUE( "SID_MAX", m_iMaxSID, return false );

	m_mapOrderByLevel.clear();
    LUA_BEGIN_TABLE( "SkillTree", return false )
    {
        for( int i = 1; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                GCSkill* pNewSkill = new GCSkill();
                if( false == pNewSkill->Load( luaMgr ) )
                {
                    assert( !"failed to load skill" );
                    return false;
                }
				if ( !pNewSkill->IsEnableSkill() )				
				{
					delete pNewSkill;
					LUA_END_TABLE( return false );
					continue;
				}


                if( false == m_mapSkill.insert( SKTValue( pNewSkill->GetID(), pNewSkill ) ).second )
                {
                    assert( !"failed to insert skill" );
                    return false;
                }
				if( pNewSkill->IsBase() == true )
				{
					m_mapBaseSkill.insert( SKTBaseSkill::value_type( pNewSkill->GetGroupID() , pNewSkill ) );
				}
				else 
                {
                    if( pNewSkill->GetPromotion() == -1 )
                    {
                        for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
                            m_mapOrderByLevel[pNewSkill->GetCharType() * NUM_JOB_LEVEL + i ].push_back( pNewSkill );
                    }
                    else
                        m_mapOrderByLevel[pNewSkill->GetCharType() * NUM_JOB_LEVEL + pNewSkill->GetPromotion() ].push_back( pNewSkill );
                }
				
					

            }
            LUA_END_TABLE( return false );
        }
    }
    LUA_END_TABLE( return false );


	std::map<int, std::vector< GCSkill* > >::iterator mit;
	for( mit = m_mapOrderByLevel.begin() ; mit != m_mapOrderByLevel.end() ; ++mit )
	{
		std::sort( mit->second.begin(), mit->second.end(), SortSkillByLevel() );
	}

    FillSkillTree();
    return true;
}

bool GCSKT::LoadSkillString( const std::string& strSkill )
{
    m_kSkillStr.Clear();
    return m_kSkillStr.LoadStringFile( strSkill );
}

bool GCSKT::LoadSkillStringFromKOM( const std::string& strSkill )
{
    m_kSkillStr.Clear();
    return m_kSkillStr.LoadStringFromKOM( strSkill );
}


bool GCSKT::LoadSkillResource( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE( "SkillResource", return false )
    {
        for( int i = 1; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                GCSkillResource* pNewRes = new GCSkillResource();
                if( false == pNewRes->Load( luaMgr ) )
                {
                    assert( !"failed to load skillRes" );
                    return false;
                }

                IterSkill mit = m_mapSkill.find( pNewRes->GetID() );
                if( mit != m_mapSkill.end() )
					pNewRes->SetSkill( mit->second );
				else
				{
					delete pNewRes;
					LUA_END_TABLE( return false );
					continue;
				}

                if( false == m_mapSkillRes.insert( SKTResValue( pNewRes->GetID(), pNewRes ) ).second )
                {
                    assert( !"failed to insert skillRes" );
                    return false;
                }
            }
            LUA_END_TABLE( return false );
        }
    }
    LUA_END_TABLE( return false );

	IterSkill mit = m_mapSkill.begin();
	while( mit != m_mapSkill.end() )
	{
		if ( mit->second->m_pHotKeySkill )
		{
			if ( m_mapSkillRes.find( mit->first ) == m_mapSkillRes.end() )
			{
				GCSkillResource* pNewRes = new GCSkillResource();
				pNewRes->SetSkill( mit->second );
				m_mapSkillRes.insert( SKTResValue( pNewRes->GetID(), pNewRes ) );
			}
		}
		mit++;
	}

    return true;
}

bool GCSKT::LoadSkillGroupScript( KLuaManager& luaMgr )
{
	std::map<int,std::vector<int>> mapTemp;
	std::vector<int> vecTemp;
	LUA_BEGIN_TABLE( "SkillGroupTable", return false )
	{
		int iChar;
		for (int i=1 ; ; i++)
		{
			LUA_BEGIN_TABLE( i, break;)
			{
				LUA_GET_VALUE_DEF( "CHAR_ID", iChar, -1 );
				int iPromotion;
				mapTemp.clear();
				//전진 넘버
				LUA_BEGIN_TABLE( "LIST", break; )
				{
					for (int j=1 ;; j++)
					{
						LUA_BEGIN_TABLE( j, break;)
						{
							LUA_GET_VALUE_DEF( "PROMOTION_ID", iPromotion, -1 );
							int iTemp;
							vecTemp.clear();
							LUA_BEGIN_TABLE( "SKILL_LIST", break; )
							{
								for (int k=1 ;;k++)
								{
									LUA_GET_VALUE_DEF(k, iTemp, -1);
									if (iTemp == -1)
										break;
									vecTemp.push_back(iTemp);
								}
								if (iPromotion != -1)
									mapTemp.insert(std::make_pair(iPromotion,vecTemp));
							}
							LUA_END_TABLE( ;)	
						}
						LUA_END_TABLE( ;)
					}
					
				}
				LUA_END_TABLE( ;)
				if (iChar != -1)
					m_mapSkillGroup.insert(std::make_pair(iChar , mapTemp));
			}
			LUA_END_TABLE( ;)
		}

	}
	LUA_END_TABLE( return true )

	return true;
}

bool GCSKT::CheckPosibleSkill(IN const GCSkill* pGCSkill_ )
{
	int iSkillID = pGCSkill_->GetID();

	int iChar = m_cCurUICharType;

    int iPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iPromotionLevel;

    std::map< int , std::map<int,std::vector<int>> >::iterator mapIter = m_mapSkillGroup.find(iChar);

    if (mapIter != m_mapSkillGroup.end())
    {
        std::map< int , std::vector<int> >::iterator mapIterIn;

        for (int i=0 ; i <= iPromotion ; i++)
        {
            mapIterIn = mapIter->second.find(i);
            if (mapIterIn != mapIter->second.end())
            {
                std::vector<int>::iterator vecIter = mapIterIn->second.begin();
                for (vecIter ; vecIter!=mapIterIn->second.end() ; ++vecIter )
                {
                    if ( *vecIter == iSkillID)
                        return true;
                }

            }
        }
    }

    return false;
	
}


void GCSKT::clear()
{
    m_kSkillStr.Clear();
    for( IterSkillRes mit = m_mapSkillRes.begin(); mit != m_mapSkillRes.end(); ++mit )
    {
        SAFE_DELETE( mit->second );
    }
    m_mapSkillRes.clear();

    for( IterSkill mit = m_mapSkill.begin(); mit != m_mapSkill.end(); ++mit )
    {
        SAFE_DELETE( mit->second );
    }
    m_mapSkill.clear();
    m_mapTrainedSkill.clear();
    m_mapSkillUsers.clear();
    m_mapSetting.clear();
	m_mapCashSkillPosition.clear();
	m_mapBaseSkill.clear();
	m_mapSkillItem.clear();
    m_mapUnLockGroupID.clear();
    m_nBuyingReqGroup = -1;

	m_mapSkillGroup.clear();
}

void GCSKT::FillSkillTree()
{
    IterSkill mit = m_mapSkill.begin();
    while( mit != m_mapSkill.end() )
    {
        GCSkill* pSkill = mit->second;
        // prev
        SkillPtrList lTempList;
        GetSKillPtr( pSkill->GetPrevSet(), lTempList );
        pSkill->SetPrevList( lTempList );

#ifndef __PATH__
        for( SkillPtrList::iterator lit = lTempList.begin(); lit != lTempList.end(); ++lit )
        {
            GCSkill* pPrevSkill = (*lit);
            const SkillSet& setSkill = pPrevSkill->GetNextSet();
            //assert( "PreSkill의 NextList에 해당 스킬이 없음" &&
            //    setSkill.end() != setSkill.find( pSkill->GetID() ) );
        }
#endif

        lTempList.clear();
        // next
        GetSKillPtr( pSkill->GetNextSet(), lTempList );
        pSkill->SetNextList( lTempList );

#ifndef __PATH__
        for( SkillPtrList::iterator lit = lTempList.begin(); lit != lTempList.end(); ++lit )
        {
            GCSkill* pNextSkill = (*lit);
            const SkillSet& setSkill = pNextSkill->GetPrevSet();
//          assert( "NextSkill의 PreList에 해당 스킬이 없음" &&
//                 ( setSkill.end() != setSkill.find( pSkill->GetID() ) || pSkill->IsBase() ) );
        }
#endif
		std::set<int> setGoods = pSkill->GetGoodsList();
		std::set<int>::iterator sit;
		for( sit = setGoods.begin(); sit != setGoods.end() ; ++sit )
		{
			m_mapSkillItem.insert( std::map< DWORD, EGCSkillTree >::value_type( (DWORD)(*sit), pSkill->GetID() ) );
		}

        lTempList.clear();
        mit++;
    }
}

// ID에 대한 Skill*를 담아줌.
void GCSKT::GetSKillPtr( IN const SkillSet& setID, OUT SkillPtrList& lSkill )
{
    lSkill.clear();

    SkillSet::const_iterator lit;
    for( lit = setID.begin(); lit != setID.end(); ++lit )
    {
        const EGCSkillTree& eSkill = (*lit);
        IterSkill mit = m_mapSkill.find( eSkill );
//        assert( mit != m_mapSkill.end() );
        if( mit != m_mapSkill.end() )
        {
            lSkill.push_back( mit->second );
        }
    }
}

// Skill ID를 넣으면 해당 스킬을 익히기 위해 필요한 이전 스킬들을 담아준다. recursive
void GCSKT::GetPrecedeSkill( EGCSkillTree eID, OUT SkillSet& setPrecede )
{
    IterSkill mit = m_mapSkill.find( eID );
    assert( mit != m_mapSkill.end() );
    if( mit != m_mapSkill.end() )
    {
        const SkillSet& setPrev = mit->second->GetPrevSet();
        if( setPrev.empty() )
        {
            return;
        }
        SkillSet setMerge = setPrev;
        // 두 셋에 합집합을 만든다.
        std::set_union( setMerge.begin(), setMerge.end(), setPrecede.begin(), setPrecede.end(),
            std::inserter( setMerge, setMerge.begin() ) );
        setPrecede.swap( setMerge );
        SkillSet::const_iterator itSkill = setPrev.begin();
        while( itSkill != setPrev.end() )
        {
            GetPrecedeSkill( (*itSkill), setPrecede );
            itSkill++;
        }
        mit++;
    }
}

// local유저가 스킬을 사용.
bool GCSKT::CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill )
{
    if(pPlayer == NULL) return false;
    //리플레이 재생중일때에는 체크 안함
    if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
        return true;

    IterSkill itSkill = m_mapSkill.find( eSkill );

	if( itSkill == m_mapSkill.end() )
		return false;

    assert( itSkill->second != NULL );
    GCSkill* pSkill = itSkill->second;
    // enable check
    if( !pSkill->IsEnableSkill() )
    {
        return false;
    }

	if( -1 == pPlayer->m_kUserInfo.cCharIndex )
		return false;

    EGCCharType eType = (EGCCharType)pPlayer->GetCurrentChar().iCharType;
    // charType check
    if( eType != pSkill->GetCharType() )
    {
        return false;
    }

    int nPromotion = ( pPlayer->Extra_Char_Num - pPlayer->GetCurrentChar().iCharType ) / GC_CHAR_NUM;
    // Promotion Check
    if( -1 != pSkill->GetPromotion() && nPromotion != pSkill->GetPromotion() )
    {
        return false;
    }

    // lv check
    if( pSkill->GetLv() > pPlayer->GetCurrentChar().iLevel )
    {
        return false;
    }

    // item check
    if( !pSkill->CheckItem( pPlayer ) )
    {
        return false;
    }

    if( IsSkillMissionSkillID( eSkill ) && IsLearnedSkill( eSkill ) ) {
        return true;
    }

    ItserSkillUsers it = m_mapSkillUsers.find( eSkill );
    if( it == m_mapSkillUsers.end() )
    {
        return false;
    }
    std::set<DWORD>& setUID = it->second;
    std::set<DWORD>::iterator sit = setUID.find( pPlayer->m_kUserInfo.dwUID );
    if( sit != setUID.end() )
    {

#if defined( CHARACTER_STATS )
        // SID통계
        //SCharInfo sCharInfo = pPlayer->m_kUserInfo.GetCurrentChar( pPlayer->m_kUserInfo.cCharIndex );
        //EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
        //if( (-1 < sCharInfo.iCharType) && ((GC_GMC_MATCH == eGameModeCategory) ||(GC_GMC_DUNGEON == eGameModeCategory)) )
        //{
        //    std::pair<int, int> prKey(int(eSkill), sCharInfo.iLevel);
        //    std::map< std::pair<int, int>, KGCGlobal::CHARACTERSTATS >::iterator mapIter = g_kGlobalValue.m_mapCharacterStats.find( prKey );
        //    if( mapIter == g_kGlobalValue.m_mapCharacterStats.end() )
        //    {
        //        KGCGlobal::CHARACTERSTATS StatsData;
        //        switch( eGameModeCategory )
        //        {
        //        case GC_GMC_DUNGEON:            // 퀘스트
        //            StatsData.iGameMode = 0;
        //            break;
        //        case GC_GMC_MATCH:              // 대전
        //            if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT)
        //                StatsData.iGameMode = 2;
        //            else
        //                StatsData.iGameMode = 1;
        //            break;
        //        }
        //        StatsData.iAccCount = 1;
        //        g_kGlobalValue.m_mapCharacterStats.insert( make_pair(prKey, StatsData) );
        //    }
        //    else
        //    {
        //        ++mapIter->second.iAccCount;
        //    }
        //}
#endif

        // i trained skill.
        return true;
    }
    return false;

}

bool GCSKT::IsSkillTrainable( EGCSkillTree eSkill )
{
    if( GetOwnerPlayer()->m_kUserInfo.vecCharInfo.empty() )
        return false;

	if( SiGCSKT()->IsLearnedSkill( eSkill ) )
		return false;

    IterSkill mit = m_mapSkill.find( eSkill );
    if( mit == m_mapSkill.end() )
        return false;

    assert( mit != m_mapSkill.end() );
    SkillSet setPrev;
    GetPrecedeSkill( eSkill, setPrev );

	GCSkill* pSkill = mit->second;

	SCharInfo& sChar = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( pSkill->GetCharType() );

	if( pSkill->GetLv() > sChar.iLevel )
		return false;

    if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ||
        g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL ) {
        if( -1 != pSkill->GetPromotion() && pSkill->GetPromotion() > g_kGlobalValue.GetOpenedPromotionNum( pSkill->GetCharType() ) )
            return false;
    } else {
	    if( -1 != pSkill->GetPromotion() && pSkill->GetPromotion() > sChar.iPromotionLevel )
		    return false;
    }

	if( pSkill->GetLearnSP() > GetExtraSP( pSkill->GetCharType() ) )
    {
        // 연습모드 캐릭터 창일 때 스테이트가 GS_SERVER 임 
        if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ||
            g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL ) {
                return true;
        }
		return false;
    }

	if( !m_bUnlockAll )
	{
		if( pSkill->GetBlockGroup() != -1)
		{
			if( m_mapUnLockGroupID.find( (char)pSkill->GetCharType() ) == m_mapUnLockGroupID.end() )
				return false;

			if( m_mapUnLockGroupID[(char)pSkill->GetCharType()].find( pSkill->GetBlockGroup() ) == m_mapUnLockGroupID[(char)pSkill->GetCharType()].end() )
				return false;
		}
	}

	// 선행 스킬에서 Base 스킬은 제외한다.
	SkillSet::iterator sit;
	for( sit = setPrev.begin() ; sit != setPrev.end() ; ++sit )
	{
		const GCSkill* pSkill = GetSkill( *sit );
		if( pSkill->IsBase() == true )
		{
			//베이스 스킬을 하나 이상 가질 수 없으니 하나 찾으면 끝
			setPrev.erase( sit );
			break;
		}
	}

	// 선행스킬이 없다면..
	if( setPrev.empty() )
	{
		return true;
	}

    IterTrainedSK itTRSk = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion()) );
    if( itTRSk == m_mapTrainedSkill.end() )
        return false;    
    // 내가 배운 스킬들이 이 스킬을 배우기 위한 선행스킬에 부분집합인지 확인

    // 선행 스킬 중 하나라도 배우고 있을 시
    if( pSkill->m_bOrCheckPreList ) {
        const SkillSet& setPrevID = pSkill->GetPrevSet();
        for( std::set< EGCSkillTree >::const_iterator sit = setPrevID.begin(); sit != setPrevID.end(); ++sit ) {
            if( IsLearnedSkill( *sit ) ) {
                return true;
            }
        }
    }

    SkillSet& setTR = itTRSk->second;
    return std::includes( setTR.begin(), setTR.end(), setPrev.begin(), setPrev.end() );
}

void GCSKT::InsertUserSkillSet( DWORD dwUserUID, const SkillSet& setSKill )
{
    SkillSet::const_iterator sit = setSKill.begin();
    while( sit != setSKill.end() )
    {
        ItserSkillUsers mit = m_mapSkillUsers.find( (*sit) );
        if( mit == m_mapSkillUsers.end() )
        {
            std::set<DWORD> tmpSet;
            tmpSet.insert( dwUserUID );
            m_mapSkillUsers.insert( SkillUsersValue( (*sit), tmpSet ) );
        }
        else
        {
            mit->second.insert( dwUserUID );
        }
        ++sit;
    }
}

void GCSKT::RemoveUserSkillSet( DWORD dwUserUID )
{
    ItserSkillUsers mit = m_mapSkillUsers.begin();
    while( mit != m_mapSkillUsers.end() )
    {
        std::set<DWORD>& setUsers = mit->second;
        std::set<DWORD>::iterator sit = setUsers.find( dwUserUID );
        if( sit != setUsers.end() )
        {
            setUsers.erase( sit );
        }

        if( setUsers.empty() )
        {
            mit = m_mapSkillUsers.erase( mit );
            continue;
        }

        ++mit;
    }
}

void GCSKT::LoadingResource( DWORD dwUID )
{
    ItserSkillUsers mit = m_mapSkillUsers.begin();
    while( mit != m_mapSkillUsers.end() )
    {
        std::set<DWORD>& setUsers = mit->second;
        std::set<DWORD>::iterator sit = setUsers.find( dwUID );
        if( sit != setUsers.end() )
        {
            IterSkillRes it = m_mapSkillRes.find( mit->first );
            if( it != m_mapSkillRes.end() )
            {
                GCSkillResource* pRes = it->second;
                // ref count....

				pRes->LoadRes( dwUID );
            }
        }
        ++mit;
    }
}

void GCSKT::ReleaseResource( DWORD dwUID )
{
    ItserSkillUsers mit = m_mapSkillUsers.begin();
    while( mit != m_mapSkillUsers.end() )
    {
        std::set<DWORD>& setUsers = mit->second;
        std::set<DWORD>::iterator sit = setUsers.find( dwUID );
        if( sit != setUsers.end() )
        {
            IterSkillRes it = m_mapSkillRes.find( mit->first );
            if( it != m_mapSkillRes.end() )
            {
                GCSkillResource* pRes = it->second;
                // ref count....
                pRes->ReleaseRes( dwUID );
            }
        }
        ++mit;
    }
}

void GCSKT::Server_SetTrainedSkill( const std::map<std::pair<char,char>, KSPInfo>& kFullInfo )
{
    m_mapTrainedSkill.clear();

	if (g_kGlobalValue.ServerInfo.CheckServerType( ST_LOCK_SKILL_TREE ) == true )
	{
		return;
	}


    std::map<std::pair<char,char>, KSPInfo>::const_iterator mit = kFullInfo.begin();
    while( mit != kFullInfo.end() )
    {
        const std::pair<char,char>& pairInfo = mit->first;
        std::pair<EGCCharType, char> kKey( (EGCCharType)pairInfo.first, pairInfo.second );
        IterTrainedSK iterSk = m_mapTrainedSkill.find( kKey );

        if( iterSk != m_mapTrainedSkill.end() )
        {
            SkillSet& setSkill = iterSk->second;
            setSkill.clear();
            const KSPInfo& kSPInfo = mit->second;
            TransformSkillInfo( kSPInfo, setSkill );
        }
        else
        {
            SkillSet setSkill;
            const KSPInfo& kSPInfo = mit->second;
            TransformSkillInfo( kSPInfo, setSkill );
            m_mapTrainedSkill.insert( TrainedSKValue( kKey, setSkill ) );
        }
        ++mit;
    }
}

void GCSKT::TransformSkillInfo( const KSPInfo& kSPInfo, OUT SkillSet& setSkill )
{
    std::vector<int>::const_iterator vit = kSPInfo.m_vecSkills.begin();
    while( vit != kSPInfo.m_vecSkills.end() )
    {
        setSkill.insert( (EGCSkillTree)(*vit) );
        ++vit;
    }
}

SkillSet* GCSKT::GetTrainedSkillSet( EGCSkillTree eSkillID )
{
	IterSkill mit = m_mapSkill.find( eSkillID );
	if( mit == m_mapSkill.end() )
	{
		assert( mit != m_mapSkill.end() );
		return NULL;
	}

	GCSkill* pSkill = mit->second;
	IterTrainedSK itSk = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion()) );
	if( itSk == m_mapTrainedSkill.end() )
	{
        if( g_kGlobalValue.m_kUserInfo.IsCharExist( pSkill->GetCharType() ) ) {
            itSk = m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion()) , SkillSet() )).first;
            m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(pSkill->GetCharType(), -1) , SkillSet() )).first;

            if( itSk != m_mapTrainedSkill.end() ) {
                std::vector<KSPSetInfo> vecSkillSets;
				KSPSetInfo kspsetinfo1,kspsetinfo2;
				kspsetinfo1.m_nSPSetID = 0;
				kspsetinfo2.m_nSPSetID = 1;

				kspsetinfo1.m_vecSkills.clear();
				kspsetinfo2.m_vecSkills.clear();

                vecSkillSets.push_back(kspsetinfo1 );
                vecSkillSets.push_back(kspsetinfo2 );
                m_mapSkillSets.insert( std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::value_type( std::pair<char,char>(pSkill->GetCharType(), pSkill->GetPromotion()) , vecSkillSets ));
                m_mapSetting[std::pair<char,char>(pSkill->GetCharType(), pSkill->GetPromotion())] = 0;
                return &itSk->second;
            }
        }
		assert( itSk != m_mapTrainedSkill.end() );
		return NULL;
	}

	return &itSk->second;
}

bool GCSKT::Server_TrainSkill( int nSkillID )
{
    SkillSet* setSkill = GetTrainedSkillSet( (EGCSkillTree)nSkillID );

	if( setSkill == NULL )
		return false;

    (*setSkill).insert( (EGCSkillTree)nSkillID );
    return true;
}

bool GCSKT::Server_UnTrainSkill( int nSkillID )
{
	// 장착하고 있던 것들도 다 빼자!!!
	std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mitEquiped;
	for( mitEquiped = m_mapSkillSets.begin() ; mitEquiped != m_mapSkillSets.end() ; ++mitEquiped )
	{
		for( int i = 0 ; i < (int)(mitEquiped->second).size() ; ++i )
		{
			std::vector<KSkillSlot>::iterator vit = std::remove_if( (mitEquiped->second)[i].m_vecSkills.begin(), (mitEquiped->second)[i].m_vecSkills.end(), FindSkill( (EGCSkillTree)nSkillID ) );
			(mitEquiped->second)[i].m_vecSkills.erase( vit, (mitEquiped->second)[i].m_vecSkills.end() );
		}
	}

	SkillSet* setSkill = GetTrainedSkillSet( (EGCSkillTree)nSkillID );

	if( setSkill == NULL )
		return false;

	(*setSkill).erase( (EGCSkillTree)nSkillID );
    return true;
}

const GCSkill* GCSKT::GetBaseSkill( EGCSkillGroup eGroupID )
{
	if( m_mapBaseSkill.find( eGroupID ) == m_mapBaseSkill.end() )
		return NULL;

	return m_mapBaseSkill[eGroupID];
}

const GCSkill* GCSKT::GetSkill( EGCSkillTree eID )
{
	if( m_mapSkill.find( eID ) == m_mapSkill.end() )
		return NULL;

	return m_mapSkill[eID];
}

void GCSKT::GetSkillSet( int iCharType, int nPromotion, OUT std::vector< EGCSkillTree >& setSkill )
{
	std::pair<char,char> key = std::make_pair(iCharType,nPromotion);

	if ( m_mapSkillSets.find( key ) == m_mapSkillSets.end() )
	{
		return;
	}
	
	std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
	if ( kSPInfo.empty() )
	{
		return;
	}

	std::vector<KSkillSlot>& vecSkills = kSPInfo[GetSetting(iCharType,nPromotion)].m_vecSkills;

	for ( unsigned int i = 0; i < vecSkills.size(); i++ )
	{
		setSkill.push_back( (EGCSkillTree)vecSkills[i].m_nSkillID );
	}

}

std::vector<EGCSkillTree> GCSKT::GetHotKeySkillVector( int iCharType, int nPromotion )
{
	std::pair<char,char> key = std::make_pair(iCharType,nPromotion);

	std::vector<EGCSkillTree> vecRet;

	if ( m_mapSkillSets.find( key ) == m_mapSkillSets.end() )
	{
		return vecRet;
	}

	std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
	if ( kSPInfo.empty() )
	{
		return vecRet;
	}

	std::vector<KSkillSlot>& vecSkills = kSPInfo[GetSetting(iCharType,nPromotion)].m_vecSkills;
	
#if defined( USE_AP_SLOT_CHANGE )
	for ( unsigned int i = 0; i < vecSkills.size(); i++ )
	{
		if( vecSkills[i].m_nSlotIndex == m_iCurSlotType && GetSkill( (EGCSkillTree)vecSkills[i].m_nSkillID )->m_pHotKeySkill )
			vecRet.push_back( (EGCSkillTree)vecSkills[i].m_nSkillID );
	}
#else
    for ( unsigned int i = 0; i < vecSkills.size(); i++ )
    {
        if( GetSkill( (EGCSkillTree)vecSkills[i].m_nSkillID )->m_pHotKeySkill && 
            GetSkill( (EGCSkillTree)vecSkills[i].m_nSkillID )->m_pHotKeySkill->m_bNoEquipSlot == false ) {
            vecRet.push_back( (EGCSkillTree)vecSkills[i].m_nSkillID );
        }            
    }
#endif

	return vecRet;
}

std::vector<EGCSkillTree> GCSKT::GetHotKeySkillVectorByType( int iCharType, int nPromotion, int iSlotType_ )
{
    std::pair<char,char> key = std::make_pair(iCharType,nPromotion);

    std::vector<EGCSkillTree> vecRet;

    if ( m_mapSkillSets.find( key ) == m_mapSkillSets.end() )
    {
        return vecRet;
    }

    std::vector<KSPSetInfo>& kSPInfo = m_mapSkillSets[ std::pair<char,char>((char)iCharType, (char)nPromotion) ];
    if ( kSPInfo.empty() )
    {
        return vecRet;
    }

    std::vector<KSkillSlot>& vecSkills = kSPInfo[GetSetting(iCharType,nPromotion)].m_vecSkills;

    for ( unsigned int i = 0; i < vecSkills.size(); i++ )
    {
        if( vecSkills[i].m_nSlotIndex == iSlotType_ && GetSkill( (EGCSkillTree)vecSkills[i].m_nSkillID )->m_pHotKeySkill &&
            GetSkill( (EGCSkillTree)vecSkills[i].m_nSkillID )->m_pHotKeySkill->m_bNoEquipSlot == false ) {
            vecRet.push_back( (EGCSkillTree)vecSkills[i].m_nSkillID );
        }
    }

    return vecRet;
}

void GCSKT::Server_SetSkillSetting(std::map< std::pair<char, char>, int >& kSetting)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_LOCK_SKILL_TREE) == true)
    {
        m_mapSetting.clear();
        return;
    }

    m_mapSetting = kSetting;
}

void GCSKT::Server_SetSkillSet(std::map< std::pair<char, char>, std::vector<KSPSetInfo> >& kSkillSet)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_LOCK_SKILL_TREE))
    {
        m_mapSkillSets.clear();
        return;
    }

    std::map< std::pair<char, char>, std::vector<KSPSetInfo> >::iterator mit;

    for (int k = 0; k < GC_CHAR_NUM; k++)
    {
        if (g_kGlobalValue.IsQuickSkillPlayer(k) ||
            g_kGlobalValue.IsHybridPlayer(k))
        {
            for (int i = -1; i < 4; i++)
            {
                mit = kSkillSet.find(std::pair<char, char>((char)k, (char)i));

                if (mit == kSkillSet.end())
                {
                    continue;
                }

                std::vector<KSPSetInfo>& vecSetInfo = mit->second;
                std::vector<KSPSetInfo>::iterator vitSetInfo = vecSetInfo.begin();
                while (vitSetInfo != vecSetInfo.end())
                {
                    std::vector<KSkillSlot>& vec = vitSetInfo->m_vecSkills;
                    std::vector<KSkillSlot> vecTemp;
                    KSkillSlot skill;
                    skill.m_nSkillID = SID_DIO1_EMPTY_SLOT;
                    skill.m_nSlotID = SG_DIO1_EMPTY_SLOT;

                    for (unsigned int i = 0; i < (vec.size() + 5); i++)
                    {
                        vecTemp.push_back(skill);
                    }

                    for (unsigned int i = 0; i < vec.size(); i++)
                    {
                        if (vec[i].m_nSlotID > (int)vecTemp.size())
                        {
                            vecTemp[i] = vec[i];
                        }
                        else
                        {
                            vecTemp[vec[i].m_nSlotID - 1] = vec[i];
                        }
                    }

                    vecTemp.swap(vec);

                    std::vector<KSkillSlot>::iterator vit = vec.begin();
                    while (vit != vec.end())
                    {
                        if (GetSkill((EGCSkillTree)vit->m_nSkillID) == NULL) {
                            vit++;
                            continue;
                        }
                        vit->m_nSlotID = GetSkill((EGCSkillTree)vit->m_nSkillID)->GetGroupID();
                        vit++;
                    }

                    vitSetInfo++;
                }
            }
        }
    }

    m_mapSkillSets = kSkillSet;
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_LOCK_SKILL_TREE) == true)
    {
        m_mapSkillSets.clear();
        return;
    }
}

std::set< EGCSkillTree > GCSKT::GetSkillIDsInGroup( EGCSkillGroup eGroupID )
{
	IterSkill mit = m_mapSkill.begin();
	std::set< EGCSkillTree > setGroupSkill;
	while( mit != m_mapSkill.end() )
	{
		GCSkill* pSkill = mit->second;

		if( pSkill->GetGroupID() == eGroupID )
			setGroupSkill.insert( pSkill->GetID() );

		mit++;
	}

	return setGroupSkill;
}

bool GCSKT::IsLearnedSkill( EGCSkillTree eSkill )
{
	const GCSkill* pSkill = GetSkill( eSkill );

	if( pSkill == NULL )
		return false;

	if( pSkill->IsBase() == true )
		return true;

	if( pSkill->IsCash() == true )
	{
		std::set< int > m_setGoods = pSkill->GetGoodsList();
		std::set< int >::iterator sit;
		for( sit = m_setGoods.begin() ; sit != m_setGoods.end() ; ++sit )
		{
			if( g_pItemMgr->FindInventoryForItemID( *sit ) == true )
				return true;
		}
	}

	IterTrainedSK itTRSkill = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion() ) );
	if( itTRSkill == m_mapTrainedSkill.end() )
	{
		return false;
	}
	SkillSet& setSkill = itTRSkill->second;
	IterSkillSet itList = std::find( setSkill.begin(), setSkill.end(), eSkill );
	if( itList != setSkill.end() )
	{
		// i trained skill.
		return true;
	}
	return false;
}

const GCSkill* GCSKT::GetEquipedSkill( EGCSkillGroup eGroupID, int iSetID )
{
	std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;


//		SG_ELESIS1_CASH_SPECIAL1,
//		SG_ELESIS1_CASH_SPECIAL2,
//		SG_ELESIS1_CASH_SPECIAL3,
	// 엘리시스는 백마법 1,2,3 단필 이다.
	// 캐시 스킬에 대한 다른 처리가 나오지 않는 이상 이런 하드 코딩외 방법이 떠오르지 않는다.
	// 다음번 스킬 트리 작업시 반드시 캐시 스킬에 대한 처리를 해주길 바란다.
	if( m_mapCashSkillPosition.find( eGroupID ) != m_mapCashSkillPosition.end() )
	{
		KItem* pItem = g_pItemMgr->FindEquipedSkill( &GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(m_cCurUICharType), (ESKILLPOSITION)m_mapCashSkillPosition[eGroupID] );
		if( pItem == NULL )
			return GetBaseSkill( eGroupID );

        return GetSkillFromItemID( pItem->m_ItemID );	
	}

	for( mit = m_mapSkillSets.begin() ; mit != m_mapSkillSets.end() ; ++mit )
	{		
		if ((int)mit->second.size()<=iSetID)
			continue;

		std::vector<KSkillSlot>::iterator vit;
		vit = std::find_if( (mit->second)[iSetID].m_vecSkills.begin(), (mit->second)[iSetID].m_vecSkills.end(), FindSkillInGroup( eGroupID ) );

		if( vit != (mit->second)[iSetID].m_vecSkills.end() )
		{          
			return GetSkill( (EGCSkillTree)(*vit).m_nSkillID );
		}
	}

	return GetBaseSkill( eGroupID );
}

void GCSKT::EquipSkill( EGCSkillTree eSkillID, int iSetID  )
{
    typedef std::pair<char,char> SetKey;
    const GCSkill* pSkill = GetSkill( eSkillID );

    if( pSkill == NULL )
    {
        ASSERT( !"No skill Info" );
        return;
    }

    if( pSkill->IsCash() == true && !IsUnlockAll() )
    {
        std::set<int> setGoods = pSkill->GetGoodsList();
        std::set<int>::iterator sit;

        for( sit = setGoods.begin() ; sit != setGoods.end() ; ++sit )
        {
            KItem* pkItem = g_pItemMgr->m_kInventory.FindItemByItemID( *sit );
            if( pkItem == NULL )
                continue;

            g_pItemMgr->EquipInventoryItem( &GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( pSkill->GetCharType() ), pkItem->m_ItemUID );
            g_pItemMgr->EquipInventoryItem( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar( pSkill->GetCharType() ), pkItem->m_ItemUID );
        }
        return;
    }

    char iCharType = (char)pSkill->GetCharType();
    char iPromotion = (char)pSkill->GetPromotion();

    if( m_mapSkillSets.find( SetKey( iCharType, iPromotion ) ) == m_mapSkillSets.end() )
    {
        iPromotion = m_cCurUIPromotion;
    }

    if( m_mapSkillSets.find( SetKey( iCharType, iPromotion ) ) == m_mapSkillSets.end() )
    {
        return;
    }

    if( pSkill->GetPromotion() == -1 ) 
    {
        for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
        {
            EquipSkill( pSkill, i, iSetID );
        }
    }
    else
    {
		EquipSkill( pSkill, iPromotion, iSetID );
    }		
    //std::map<std::pair<char,char>, std::vector<KSPSetInfo> > m_mapSkillSets; // 내 세트

    
}

void GCSKT::EquipSkill( const GCSkill* pSkill, int iPromotion, int iSetID )
{
	typedef std::pair<char,char> SetKey;
	char iCharType = (char)pSkill->GetCharType();
		
	if( m_mapSkillSets.find( SetKey( iCharType, iPromotion ) ) == m_mapSkillSets.end() )
		return;

	int iSetSize = m_mapSkillSets[SetKey(iCharType, iPromotion )].size();
	if( iSetSize == 0 || iSetSize < iSetID || iSetID < 0 )
		return;

	std::vector<KSkillSlot>& setInfo = m_mapSkillSets[ SetKey( iCharType, iPromotion ) ][iSetID].m_vecSkills;

	bool bSlotSkillUnequipped;
	UnEquipSkillGroup( setInfo, pSkill->GetGroupID(), &bSlotSkillUnequipped );
	if ( bSlotSkillUnequipped && g_kGlobalValue.IsCharUseSpecial4( pSkill->GetCharType() ) == false )
	{
		SetHotKeyIndexToEmpty( iCharType, iPromotion );
	}
	
	
	if ( pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_bNoEquipSlot == false && g_kGlobalValue.IsCharUseSpecial4( pSkill->GetCharType() ) == false )
	{
		EquipHotKeySkill( setInfo, pSkill );		
	}
	else
	{
		KSkillSlot skill;
		skill.m_nSkillID = pSkill->GetID();
		skill.m_nSlotID = pSkill->GetGroupID();
        skill.m_nSlotIndex = m_iCurSlotType;
		setInfo.push_back( skill );		
	}
	
	START_LOG( clog, L"스킬 장착 : " << pSkill->GetID() );
}

void GCSKT::EquipHotKeySkill( std::vector<KSkillSlot>& setInfo, const GCSkill* pSkill )
{
#if defined( USE_AP_SLOT_CHANGE ) 
    if( m_iCurSlotType >= KSkillSlot::INDEX_MAX ) {
        m_iCurSlotType = KSkillSlot::INDEX_A;
    }

	std::vector<KSkillSlot>::iterator vit = setInfo.begin();

	int iSlotSkillIndex = 0;

	while ( vit != setInfo.end() )
	{

		if ( GetSkill( (EGCSkillTree)vit->m_nSkillID )->m_pHotKeySkill && vit->m_nSlotIndex == m_iCurSlotType )
		{
			if ( m_iHotKeyIndex[m_iCurSlotType] == iSlotSkillIndex )
			{
				vit->m_nSkillID = pSkill->GetID();
				vit->m_nSlotID = pSkill->GetGroupID();
                vit->m_nSlotIndex = m_iCurSlotType;
				return;
			}

			if ( iSlotSkillIndex >= MAX_SLOT_COUNT )
			{
				return;
			}
			iSlotSkillIndex++;
		}		
		vit++;
	}

	while ( iSlotSkillIndex < MAX_SLOT_COUNT )
	{
		KSkillSlot skill;
		if ( iSlotSkillIndex == m_iHotKeyIndex[m_iCurSlotType] )
		{
			skill.m_nSkillID = pSkill->GetID();
			skill.m_nSlotID = pSkill->GetGroupID();
            skill.m_nSlotIndex = m_iCurSlotType;
		}
		else
		{
		    skill.m_nSkillID = SID_DIO1_EMPTY_SLOT;
		    skill.m_nSlotID = SG_DIO1_EMPTY_SLOT;
            skill.m_nSlotIndex = KSkillSlot::INDEX_A;
		}
		setInfo.push_back( skill );				
		iSlotSkillIndex++;
	}
#else
    std::vector<KSkillSlot>::iterator vit = setInfo.begin();

    int iSlotSkillIndex = 0;

    int iMaxSlotCnt = MAX_SLOT_COUNT;

    while ( vit != setInfo.end() )
    {
        if ( GetSkill( (EGCSkillTree)vit->m_nSkillID )->m_pHotKeySkill )
        {
            if ( m_iHotKeyIndex == iSlotSkillIndex )
            {
                vit->m_nSkillID = pSkill->GetID();
                vit->m_nSlotID = pSkill->GetGroupID();
                return;
            }

            if ( iSlotSkillIndex >= iMaxSlotCnt )
            {
                return;
            }
            iSlotSkillIndex++;
        }		
        vit++;
    }

    while ( iSlotSkillIndex < iMaxSlotCnt )
    {
        KSkillSlot skill;
        if ( iSlotSkillIndex == m_iHotKeyIndex )
        {
            skill.m_nSkillID = pSkill->GetID();
            skill.m_nSlotID = pSkill->GetGroupID();
        }
        else
        {
            skill.m_nSkillID = SID_DIO1_EMPTY_SLOT;
            skill.m_nSlotID = SG_DIO1_EMPTY_SLOT;
        }
        setInfo.push_back( skill );				
        iSlotSkillIndex++;
    }
#endif

}

bool GCSKT::EquipSkill( DWORD dwItemID )
{
	if( m_mapSkillItem.find( dwItemID ) == m_mapSkillItem.end() )
	{
		//ASSERT( !"스킬 트리에 없는 아이템이여!!" );
		return false;
	}

    const GCSkill* pSkill = GetSkill( m_mapSkillItem[dwItemID] );

	int iCharType = (int)( pSkill->GetCharType() );
	int iPromotion = (int)( pSkill->GetPromotion() );

	if( m_mapSetting.find( std::pair<int,int>(iCharType,iPromotion) ) == m_mapSetting.end() )
	{
		EquipSkill( pSkill->GetID(), 0 );
	}
	else
		EquipSkill( pSkill->GetID(), m_mapSetting[ std::pair<int,int>(iCharType,iPromotion) ] );

	return true;
}

void GCSKT::UnequipSkill( EGCSkillGroup eGroup, int iSetID )
{
	if( m_mapCashSkillPosition.find( eGroup ) != m_mapCashSkillPosition.end() )
	{
		KItem* pItem = g_pItemMgr->FindEquipedSkill( &GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(m_cCurUICharType), (ESKILLPOSITION)m_mapCashSkillPosition[eGroup] );

		if( pItem == NULL )
			return;

		g_pItemMgr->UnequipItemForItemID( &GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(), pItem->m_ItemID );
		g_pItemMgr->UnequipItemForItemID( &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(), pItem->m_ItemID );
		return;
	}

	std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
	for( mit = m_mapSkillSets.begin() ; mit != m_mapSkillSets.end() ; ++mit )
	{
        int iSize = mit->second.size();
        if( iSize == 0 || iSize <= iSetID || iSetID < 0 ) 
            continue;

		bool bSlotSkillUnequipped;
        std::vector<KSkillSlot>& vecSetInfo = (mit->second)[iSetID].m_vecSkills;
		UnEquipSkillGroup( vecSetInfo, eGroup, &bSlotSkillUnequipped );
		if ( bSlotSkillUnequipped )
		{
			SetHotKeyIndexToEmpty(mit->first.first,mit->first.second );
		}
	}

    START_LOG( clog, L"스킬 해제 : " << eGroup );
}

void GCSKT::ApplySettingToServer()
{
    KChangeSkillSet kChange;
    kChange.m_dwUID = GetOwnerPlayer()->m_kUserInfo.dwUID;
    kChange.m_mapSetting = m_mapSetting;

    kChange.m_mapSkillSets = m_mapSkillSets;
    for (int k = 0; k < GC_CHAR_NUM; k++)
    {
        if (g_kGlobalValue.IsQuickSkillPlayer(k) ||
            g_kGlobalValue.IsHybridPlayer(k))
        {
            for (int i = -1; i < 4; i++)
            {
                std::map<std::pair<char, char>, std::vector<KSPSetInfo> >::iterator mit;

                mit = kChange.m_mapSkillSets.find(std::pair<char, char>((char)k, (char)i));

                if (mit != kChange.m_mapSkillSets.end())
                {
                    std::vector<KSPSetInfo>& vecSetInfo = mit->second;
                    std::vector<KSPSetInfo>::iterator vitSetInfo = vecSetInfo.begin();
                    while (vitSetInfo != vecSetInfo.end())
                    {
                        std::vector<KSkillSlot>& vec = vitSetInfo->m_vecSkills;
                        std::vector<KSkillSlot> vecExtract;
                        std::vector<KSkillSlot>::iterator vit = vec.begin();
                        int iIncresingNum = 1;
                        while (vit != vec.end())
                        {
                            vit->m_nSlotID = iIncresingNum;
                            iIncresingNum++;
                            if (vit->m_nSkillID != SID_DIO1_EMPTY_SLOT)
                            {
                                vecExtract.push_back(*vit);
                            }
                            vit++;
                        }

                        vec.swap(vecExtract);

                        vitSetInfo++;
                    }
                }
            }
        }
    }

    KP2P::GetInstance()->Send_SettingSkills(kChange);
}

void GCSKT::ChangeSetting( int iCharType, int iPromotion, int iSetID )
{
	if( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) ||
        g_kGlobalValue.IsHybridPlayer( iCharType ) )
	{
        // 전직이 -1(공용)이면 우짜라고??ㅠㅠㅠ 
        // 반복횟수는 늘어나지만 아래 코드로 바꿉니다.
        //std::pair<char,char> key;
        //key.first = (char)iCharType;
        //for ( char i = 0; i < 4; i++ )
        //{
        //	key.second = i;
        //	if ( m_mapSetting.find( key ) != m_mapSetting.end() )
        //	{
        //		m_mapSetting[ key ] = iSetID;
        //	}
        //}

        std::map< std::pair<char,char>, int >::iterator mapIter = m_mapSetting.begin();
        for(; mapIter != m_mapSetting.end(); ++mapIter)
        {
            if( iCharType == mapIter->first.first )
            {
                mapIter->second = iSetID;
            }
        }
	}
    m_mapSetting[ std::pair<char,char>((char)iCharType, (char)iPromotion) ] = iSetID;
}

int GCSKT::GetSetting( int iCharType, int iPromotion )
{
	if( m_mapSetting.find( std::pair<char,char>( (char)iCharType, (char)iPromotion ) ) == m_mapSetting.end() )
		return 0;

	return m_mapSetting[ std::pair<char,char>((char)iCharType, (char)iPromotion) ];
}

int GCSKT::GetCurrentSP( int iCharType )
{
    return GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).kSkillInfo.m_nLvSPPoint;
}

int GCSKT::GetExtraSP( int iCharType )
{
    //std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill; // 내가배운스킬
    //typedef std::set< EGCSkillTree > SkillSet

	int iUsedSP = 0;
	if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) || g_kGlobalValue.IsHybridPlayer( iCharType ) )
	{
		iUsedSP = GetUsedSPinPromotion( iCharType, -1 );

	}
	else
	{
		for( int i = -1 ; i < NUM_JOB_LEVEL ; ++i )
		{
			int iTemp = GetUsedSPinPromotion( iCharType, i );
			iUsedSP += iTemp;
		}
	}

    int iExtraSP = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).kSkillInfo.m_nLvSPPoint - iUsedSP;
    return (iExtraSP<0?0:iExtraSP) ;
}

int GCSKT::GetLeftSPExcludingCommonSkill( int iCharType )
{
    int iUsedSP = 0;
    if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) || g_kGlobalValue.IsHybridPlayer( iCharType ) )
    {
        iUsedSP = GetUsedSPinPromotion( iCharType, -1 );

    }
    else
    {
        for( int i = -1 ; i < NUM_JOB_LEVEL ; ++i )
        {
            int iTemp = GetUsedSPinPromotion( iCharType, i );
            iUsedSP += iTemp;
        }
    }

    // 남은 SP량 = 해당 전직에서 사용할 수 있는 SP량 - 사용한 SP량
    int iPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( iCharType ).iPromotionLevel;
    int iMaxSP = SiGCSKT()->GetMaxSPinPromotion( iCharType, iPromotion );                                   // 현재 전직에서 사용가능한 최대 스킬포인트량
    int iObtainedSP = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).kSkillInfo.m_nLvSPPoint;    // 현재 레벨에서 획득한 스킬포인트량

    // 남은 SP를 계산해보자
    int iLeftSP = iObtainedSP;

    // MP케릭은 공용스킬을 사용량을 구함.
    int iUsedCommonSP = 0;
    if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) == false && g_kGlobalValue.IsHybridPlayer( iCharType ) == false ){
        iUsedCommonSP = GetUsedSPinPromotion( iCharType, -1 );
        iLeftSP -= iUsedCommonSP;
        iUsedSP -= iUsedCommonSP; 
    }

    // 전직에서는 많이 쓸 수 있는데, 레벨이 낮아서 얻은 SP가 적은 경우 
    if( iLeftSP < iMaxSP )
        iLeftSP = iLeftSP - iUsedSP;
    else
        iLeftSP = iMaxSP - iUsedSP;

    return (iLeftSP<0?0:iLeftSP);
}

int GCSKT::GetMaxSP( int iCharType ,int iPromotion)
{
    // 내가 획득한 양으로 수정 2012. 06. 25
    int iObtainedSP = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).kSkillInfo.m_nLvSPPoint;    // 현재 레벨에서 획득한 스킬포인트량

    return iObtainedSP;

    // 서버에서 받는값으로 고치자!
    //return 150;

    //KEVENT_MAX_CHAR_SP_LEVEL_NOT::iterator mIter = m_mapMaxSkillInfo.find(std::pair<char,char>(iCharType, iPromotion));
    //if(mIter == m_mapMaxSkillInfo.end())
    //        return 0;
    //return mIter->second.second;
}

int GCSKT::GetUsedSPinPromotion( int iCharType, int iPromotion )
{
	int iUsedSP = 0;
    if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) ||  g_kGlobalValue.IsHybridPlayer( iCharType ) )            
	{
		iPromotion = -1;
	}

	if( m_mapTrainedSkill.find( std::pair<EGCCharType, char>( (EGCCharType)iCharType, iPromotion ) ) == m_mapTrainedSkill.end() )
		return 0;

    SkillSet& TrainedSet = m_mapTrainedSkill[ std::pair<EGCCharType, char>( (EGCCharType)iCharType, iPromotion ) ];
    SkillSet::iterator sit;
    for( sit = TrainedSet.begin(); sit != TrainedSet.end() ; ++sit )
    {
        const GCSkill* pSkill = GetSkill( *sit );

        if( pSkill == NULL )
            continue;

        iUsedSP += pSkill->GetLearnSP();
    }

    return iUsedSP;
}

int GCSKT::GetMaxSPinPromotion( int iCharType , int iPromotion )
{
    int iMaxSP = 35;
	if( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) ||
        g_kGlobalValue.IsHybridPlayer( iCharType ) )
	{
        //if ( SiGCSKT()->IsUnlockAll() )
        //{
        //    return 60;
        //}

        KEVENT_MAX_CHAR_SP_LEVEL_NOT::iterator mapIter = m_mapMaxSkillInfo.find( std::make_pair( iCharType, GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).iPromotionLevel ) );
        if( mapIter != m_mapMaxSkillInfo.end() )
        {
            iMaxSP = mapIter->second.second;
        }
        // 2011-12-04 shmhlove, 위 코드(map에서 찾아서 돌려주는)로 변경하기 이전에 하드코드로 박혀있던 내용을 아래에 유지시켜봅니다.
        // 게임서버 안 붙고 연습모드 바로 들어오면 무조건 이리로 옴... 위에 map에 내용 채울려면 로긴서버에서 처리를 하든 바꿔야 하는데....
        // 여기서 이상 하드코드가 늘어날 가능성이 있으면 스크립트로 옮기도록 하겠음~
        else
        {
            int iCurPromotion = static_cast< int >( GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).iPromotionLevel );
            if( g_kGlobalValue.IsTutorialMode() ) {
                iCurPromotion = static_cast< int >( m_cCurUIPromotion );
            }

            if( iCurPromotion == 0 )
            {
                if( GC_CHAR_LUPUS == iCharType || GC_CHAR_RIN == iCharType || GC_CHAR_EDEL == iCharType || GC_CHAR_BEIGAS == iCharType )
                    iMaxSP = 40;
#ifdef DECANEE 
                else if( GC_CHAR_ASIN == iCharType || GC_CHAR_LIME == iCharType || GC_CHAR_DECANEE == iCharType)
#else
                else if (GC_CHAR_ASIN == iCharType || GC_CHAR_LIME == iCharType)
#endif
                    iMaxSP = 160;                        
                else
                    iMaxSP = 30;
            }
            else if( iCurPromotion == 1 )
            {
                if( GC_CHAR_LUPUS == iCharType || GC_CHAR_RIN == iCharType || GC_CHAR_EDEL == iCharType || GC_CHAR_BEIGAS == iCharType )
                    iMaxSP = 80;
                else
                    iMaxSP = 60;
            }
            else if( iCurPromotion == 2 )
            {
                if( GC_CHAR_LUPUS == iCharType || GC_CHAR_RIN == iCharType || GC_CHAR_EDEL == iCharType || GC_CHAR_BEIGAS == iCharType )
                    iMaxSP = 120;
                else
                    iMaxSP = 90;
            }
            else if( iCurPromotion == 3 )
            {
                if( GC_CHAR_LUPUS == iCharType || GC_CHAR_RIN == iCharType || GC_CHAR_EDEL == iCharType || GC_CHAR_BEIGAS == iCharType )
                    iMaxSP = 160;
                else
                    iMaxSP = 120;
            }
        }
	}

	return iMaxSP;
}

void GCSKT::UnequipAllSkill( int iCharType, int iPromotion, int iSetID )
{
	std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
	if( m_mapSkillSets.find( std::pair<char,char>((char)iCharType, (char)iPromotion) ) == m_mapSkillSets.end() )
		return;

	if( iSetID < 0 || iSetID > (int)m_mapSkillSets[std::pair<char,char>((char)iCharType, (char)iPromotion)].size() )
		return;

	m_mapSkillSets[std::pair<char,char>((char)iCharType, (char)iPromotion)][iSetID].m_vecSkills.clear();
}

const GCSkill* GCSKT::GetSkillFromItemID( DWORD dwItemID )
{
	if( m_mapSkillItem.find( dwItemID ) == m_mapSkillItem.end() )
		return NULL;

	return GetSkill( m_mapSkillItem[dwItemID] );
}

void GCSKT::Server_SetUnlockGroup( const std::map<char, std::set<int> >& mapUnLockGroup )
{
    m_mapUnLockGroupID = mapUnLockGroup;
}

bool GCSKT::IsBlockedSkill( EGCCharType eType, int nGroup )
{
	if ( m_bUnlockAll )
	{
		return true;
	}

    std::map<char, std::set<int> >::iterator mit2 = m_mapUnLockGroupID.find( (char)eType );
    if( mit2 == m_mapUnLockGroupID.end() )
    {
        return true;
    }
    std::set<int>& setUnblockedID = mit2->second;
    std::set<int>::iterator sit = setUnblockedID.find( nGroup  );
    if( sit != setUnblockedID.end() )
    {
        return false;
    }
    return true;
}

bool GCSKT::IsBlockedSkill( int nGroup )
{
    if ( m_bUnlockAll )
    {
        return true;
    }

    std::map<char, std::set<int> >::iterator mit2 = m_mapUnLockGroupID.begin();
    for(; mit2 != m_mapUnLockGroupID.end(); ++mit2)
    {
        std::set<int>& setUnblockedID = mit2->second;
        std::set<int>::iterator sit = setUnblockedID.find( nGroup  );
        if( sit != setUnblockedID.end() )
        {
            return false;
        }
    }

    return true;
}

void GCSKT::Server_UnlockGroup( char cCharType, int nUnLockGroupID )
{
    std::map<char, std::set<int> >::iterator mit = m_mapUnLockGroupID.find( cCharType );
    if( mit == m_mapUnLockGroupID.end() )
    {
        std::set<int> kInsert;
        kInsert.insert( nUnLockGroupID );
        m_mapUnLockGroupID.insert( std::map<char, std::set<int> >::value_type( cCharType, kInsert ) );
    }
    else
    {
        mit->second.insert( nUnLockGroupID );
    }
	
    // 스킬 오픈하면 나는 크래시 때문에
	g_pkUIScene->m_pkToolTip->DeleteParent();
}

bool GCSKT::Process_UnlockGroupItem( int nGroup, bool bBrowserClose )
{
    SetBuyingReqGroup( bBrowserClose ? -1 : nGroup );

#ifdef __OPEN_TEST__
    // 인벤토리를 디분다 있으면
    if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_OPEN_BLOCKSKILLGROUP ) == true )
    {
        return KP2P::GetInstance()->Send_UnLockSkill( nGroup );
    }
    return false;
#endif

    // 인벤토리를 디분다 있으면
    if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_OPEN_BLOCKSKILLGROUP ) == true )
    {
        return KP2P::GetInstance()->Send_UnLockSkill( nGroup );
    }
    else // 없으면 산다
    {
        if( bBrowserClose )
            return false;

#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( ITEM_ID_OPEN_BLOCKSKILLGROUP );
#else
    #ifdef GAME_CASH
    	g_pkUIScene->EnableBuyCashItemWnd( ITEM_ID_OPEN_BLOCKSKILLGROUP, false, true );
    #else
        g_pItemMgr->BuyItem( ITEM_ID_OPEN_BLOCKSKILLGROUP, 1, false );
    #endif
#endif
		
        // 산거다
        if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_OPEN_BLOCKSKILLGROUP ) == true )
        {
            return KP2P::GetInstance()->Send_UnLockSkill( nGroup );
        }
        else // 실패
        {
            return true;
        }
    }
    return true;
}

void GCSKT::CheckNewSkill()
{
	m_bNewSkillMsgRender = false;
	if ( GetOwnerPlayer()->m_kUserInfo.GetCharSize() == 0 )
	{
		return;
	}

	std::map< EGCSkillTree, GCSkill* >::iterator mit;
	for( mit = m_mapSkill.begin() ; mit != m_mapSkill.end() ; ++mit )
	{
		mit->second->SetNew( true );
		if( mit->second->IsBase() == true )
		{
			mit->second->SetNew( false );
			continue;
		}

		if( mit->second->GetLv() <= GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(mit->second->GetCharType() ).iLevel )
		{
			mit->second->SetNew( false );
		}
	}
}

bool GCSKT::IsNewSkill( EGCSkillTree eSkillID )
{
	if( m_mapSkill.find( eSkillID ) == m_mapSkill.end() )
		return false;

	if( m_mapSkill[eSkillID] == NULL )
		return false;

	bool bNew = m_mapSkill[eSkillID]->isNew() && m_mapSkill[eSkillID]->GetLv() > GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(m_mapSkill[eSkillID]->GetCharType() ).iLevel;
	m_mapSkill[eSkillID]->SetNew( false );
	return bNew;
}

int GCSKT::GetNewSkillNum()
{
	int iCounter = 0;

	const SCharInfo& charInfo = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar();

	if (g_kGlobalValue.IsSkillTreeOpenChar(charInfo.iCharType)==false)
	{
		return iCounter;
	}

	std::map< EGCSkillTree, GCSkill* >::iterator mit;
	for( mit = m_mapSkill.begin() ; mit != m_mapSkill.end() ; ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetCharType() != charInfo.iCharType )
			continue;

		if( mit->second->GetLv() <= charInfo.iLevel && mit->second->isNew() )
		{
			++iCounter;
		}
	}

	return iCounter;
}

bool GCSKT::IsEquipedSkill( EGCSkillTree eSkillID, int iSetID )
{
	std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;

	for( mit = m_mapSkillSets.begin() ; mit != m_mapSkillSets.end() ; ++mit )
	{
		if(static_cast<int>(mit->second.size()) <= iSetID) return false; 
		//(*mit->second)[iSetID].m_vecSkills;
		//std::find( (mit->second)[iSetID].m_vecSkills.begin(), (mit->second)[iSetID].m_vecSkills.end(), FindSkillInGroup( eGroupID ) );
		std::vector<KSkillSlot>::iterator vit;
		vit = std::find_if( (mit->second)[iSetID].m_vecSkills.begin(), (mit->second)[iSetID].m_vecSkills.end(), FindSkill( eSkillID ) );
		if( vit != (mit->second)[iSetID].m_vecSkills.end() )
		{
			return true;
		}
	}

	return false;
}

const GCSkill* GCSKT::GetTutorialSimmilarLevelSkill( int iLevel )
{
    std::vector< GCSkill* >::iterator vit;

    for( vit = m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].begin() ; vit != m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() ; ++vit )
    {
        if( (*vit)->IsBase() )
            continue;

        if( (*vit)->IsCash() )
            continue;

        if( (*vit)->GetID() == SID_DIO1_EMPTY_SLOT )
            continue;

        if( m_cCurUICharType == GC_CHAR_DIO && m_cCurUIPromotion == 0 )
        {
            if( (*vit)->GetID() > SID_DIO1_HAUNTINGSHOCK_LV1 )
                continue;
        }

        return (*vit);
    }

    return NULL;
}

const GCSkill* GCSKT::GetSimmilarLevelSkill( int iLevel )
{
	//std::vector< GCSkill* > m_mapOrderByLevel;

	std::vector< GCSkill* >::iterator vit;

	for( vit = m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].begin() ; vit != m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() ; ++vit )
	{
		if( (*vit)->IsBase() )
			continue;

		if( (*vit)->IsCash() )
			continue;

		if( (*vit)->GetID() == SID_DIO1_EMPTY_SLOT )
			continue;

		if( m_cCurUICharType == GC_CHAR_DIO && m_cCurUIPromotion == 0 )
		{
			if( (*vit)->GetID() > SID_DIO1_HAUNTINGSHOCK_LV1 )
				continue;
		}

		if( IsLearnedSkill( (*vit)->GetID() ) == false || (*vit)->GetLv() >= iLevel )
		{
			return (*vit);
		}
	}

	return NULL;
}

const GCSkill* GCSKT::GetNextLevelSkill( const GCSkill* pSkill )
{
	std::vector< GCSkill* >::iterator vit;

	vit = std::find( m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].begin(), m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() , pSkill );

	if( vit == m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() )
		return NULL;

	if( vit + 1 == m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() )
		return NULL;

	if( IsLearnedSkill((*(vit+1 ))->GetID()))
		return GetNextLevelSkill( *(vit+1 ) );

	if( (*(vit+1 ))->IsCash() )
		return GetNextLevelSkill( *(vit+1 ) );

	if( (*(vit + 1))->GetID() == SID_DIO1_EMPTY_SLOT )
		return GetNextLevelSkill( *(vit+1 ) );

	if( m_cCurUICharType == GC_CHAR_DIO && m_cCurUIPromotion == 0 )
	{
		if( (*(vit+1 ))->GetID() > SID_DIO1_HAUNTINGSHOCK_LV1 )
			return GetNextLevelSkill( *(vit+1 ) );
	}

    if( m_cCurUICharType == GC_CHAR_ZERO && m_cCurUIPromotion == 0 )
    {
        if( (*(vit+1 ))->GetID() > SID_ZERO1_DS_BACKATK_LV1 )
            return GetNextLevelSkill( *(vit+1 ) );
    }

    if( m_cCurUICharType == GC_CHAR_LEY && m_cCurUIPromotion == 0 )
    {
        if( (*(vit+1 ))->GetID() > SID_LEY01_CRITICAL_LV3 )
            return GetNextLevelSkill( *(vit+1 ) );
    }

	return *(vit + 1);
}

const GCSkill* GCSKT::GetPrevLevelSkill( const GCSkill* pSkill )
{

	std::vector< GCSkill* >::iterator vit;

	vit = std::find( m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].begin(), m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() , pSkill );

	if( vit == m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].end() )
		return NULL;

	if( m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].empty() )
		return NULL;

	if( vit == m_mapOrderByLevel[m_cCurUICharType*NUM_JOB_LEVEL+m_cCurUIPromotion].begin() )
		return NULL;

	if( IsLearnedSkill((*(vit -1 ))->GetID()))
		return GetPrevLevelSkill( *(vit-1 ) );

	if( (*(vit-1 ))->IsCash() )
		return GetPrevLevelSkill( *(vit-1 ) );

	if( m_cCurUICharType == GC_CHAR_DIO && m_cCurUIPromotion == 0 )
	{
		if( (*(vit-1 ))->GetID() > SID_DIO1_HAUNTINGSHOCK_LV1 )
			return GetPrevLevelSkill( *(vit-1 ) );
	}

	if( (*(vit - 1))->GetID() == SID_DIO1_EMPTY_SLOT )
		return GetPrevLevelSkill( *(vit-1 ) );

    if( m_cCurUICharType == GC_CHAR_ZERO && m_cCurUIPromotion == 0 )
    {
        if( (*(vit-1 ))->GetID() > SID_ZERO1_DS_BACKATK_LV1 )
            return GetPrevLevelSkill( *(vit-1 ) );
    }

    if( m_cCurUICharType == GC_CHAR_LEY && m_cCurUIPromotion == 0 )
    {
        if( (*(vit-1 ))->GetID() > SID_LEY01_CRITICAL_LV3 )
            return GetPrevLevelSkill( *(vit-1 ) );
    }

	return *(vit - 1);
}

namespace{
	class SortSkillIDByLevel{
	public:
		SortSkillIDByLevel() {}
		~SortSkillIDByLevel() {}

		bool operator() ( const EGCSkillTree& left, const EGCSkillTree& right )
		{
			const GCSkill* pLeft = SiGCSKT()->GetSkill( left );
			const GCSkill* pRight = SiGCSKT()->GetSkill( right );
			return pLeft->GetLv() < pRight->GetLv();
		}
	};
}

std::vector< EGCSkillTree > GCSKT::GetSkillIDsInGroupInVector( EGCSkillGroup eGroupID )
{
	IterSkill mit = m_mapSkill.begin();
	std::vector< EGCSkillTree > vecGroupSkill;
	vecGroupSkill.reserve( 10 );

	while( mit != m_mapSkill.end() )
	{
		GCSkill* pSkill = mit->second;

		if( pSkill != NULL && pSkill->GetGroupID() == eGroupID )
			vecGroupSkill.push_back( pSkill->GetID() );

		mit++;
	}

	std::sort( vecGroupSkill.begin(), vecGroupSkill.end(), SortSkillIDByLevel() );

	return vecGroupSkill;
}

std::pair<bool,DWORD> GCSKT::IsExistSkillUntrainItem()
{
	std::pair<bool,DWORD> UntraindItem( false, 0 );
	int iDuration = INT_MAX;

	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD, iDuration, UntraindItem );
	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD2, iDuration, UntraindItem );
	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD3, iDuration, UntraindItem );

	return UntraindItem;
}

void GCSKT::CheckUntrainItem( IN DWORD dwItemID, IN OUT int& iDuration , OUT std::pair<bool,DWORD>& ret )
{
	if( true == g_pItemMgr->FindInventoryForItemID( dwItemID ) &&
		iDuration > g_pItemMgr->GetInventoryItemDuration( dwItemID ) )
	{
		ret.first = true;
		ret.second = dwItemID;
		iDuration = g_pItemMgr->GetInventoryItemDuration( dwItemID );
	}
}

void GCSKT::UnEquipSkillGroup( std::vector<KSkillSlot>& setInfo, EGCSkillGroup eGroup, OUT bool* pbSlotSkillUnequipped )
{
	if ( pbSlotSkillUnequipped )
	{
		*pbSlotSkillUnequipped = false;
	}
	std::vector<KSkillSlot>::iterator vit = setInfo.begin();
	while( vit != setInfo.end() )
	{
		if( vit->m_nSlotID == eGroup && vit->m_nSlotIndex == m_iCurSlotType )
		{
			if( GetSkill( (EGCSkillTree)vit->m_nSkillID )->m_pHotKeySkill && GetSkill( (EGCSkillTree)vit->m_nSkillID )->m_pHotKeySkill->m_bNoEquipSlot == false &&
                g_kGlobalValue.IsCharUseSpecial4( GetSkill( (EGCSkillTree)vit->m_nSkillID )->GetCharType() ) == false )
			{
				vit->m_nSkillID = SID_DIO1_EMPTY_SLOT;                
				vit->m_nSlotID = SG_DIO1_EMPTY_SLOT;
				if ( pbSlotSkillUnequipped )
				{
					*pbSlotSkillUnequipped = true;
				}
			}
			else
			{
				vit = setInfo.erase( vit );
				continue;
			}
		}
		vit++;
	}	
}

EGCSkillTree GCSKT::GetMaxSID()
{
    return (EGCSkillTree)m_iMaxSID;
}

void GCSKT::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<GCSKT>( L, "GCSKT" );
    lua_tinker::class_def<GCSKT>( L,  "GetNewSkillNum",  &GCSKT::GetNewSkillNum  );
    lua_tinker::class_def<GCSKT>( L,  "IsNewSkillMsgRender",  &GCSKT::IsNewSkillMsgRender  );

    lua_tinker::decl( L, "SkillMgr", this );
}

void GCSKT::SetHotKeyIndexToEmpty( int iChar, int iPromotion )
{
#if defined( USE_AP_SLOT_CHANGE )
    if( m_iCurSlotType >= KSkillSlot::INDEX_MAX ) {
        m_iCurSlotType = KSkillSlot::INDEX_A;
    }

	std::vector<EGCSkillTree> vec = GetHotKeySkillVector(iChar, iPromotion);

	std::vector<EGCSkillTree>::size_type i;
	for( i = 0; i < vec.size(); i++)
	{
		if ( vec[i] == SID_DIO1_EMPTY_SLOT )
		{
            if( i >= MAX_SLOT_COUNT ) {
                m_iHotKeyIndex[m_iCurSlotType] = MAX_SLOT_COUNT -1;
            } else {
			    m_iHotKeyIndex[m_iCurSlotType] = i;
            }
			return;
		}		
	}

	if ( i < MAX_SLOT_COUNT )
	{
		m_iHotKeyIndex[m_iCurSlotType] = i;
	}
#else
    std::vector<EGCSkillTree> vec = GetHotKeySkillVector(iChar, iPromotion);

    std::vector<EGCSkillTree>::size_type i;
    for( i = 0; i < vec.size(); i++)
    {
        if ( vec[i] == SID_DIO1_EMPTY_SLOT )
        {
            if( i >= MAX_SLOT_COUNT ) {
                m_iHotKeyIndex  = MAX_SLOT_COUNT -1;
            } else {
                m_iHotKeyIndex  = i;
            }
            return;
        }		
    }

    if ( i < MAX_SLOT_COUNT )
    {
        m_iHotKeyIndex = i;
    }
#endif
}

void GCSKT::SetHotKeyIndexToEmpty()
{
	SetHotKeyIndexToEmpty( m_cCurUICharType, m_cCurUIPromotion );
}
void GCSKT::Server_SetFullSPInfo( KFullSPInfo& kRecv )
{	
	Server_SetTrainedSkill( kRecv.m_mapTrainedSkill );
	Server_SetSkillSet( kRecv.m_mapSkillSets );
	Server_SetSkillSetting( kRecv.m_mapSetting );
	Server_SetUnlockGroup( kRecv.m_mapUnLockGroupID );
	g_kGlobalValue.m_kUserInfo.UpdateSkillSet( kRecv.m_mapSetting, kRecv.m_mapSkillSets );	
	CheckNewSkill();

    m_mapSlotOpenInfo = kRecv.m_mapSlotOpenInfo;

    //스킬미션 작업
    m_mapSkillMissionInfo = kRecv.m_mapSkillMissionInfo;
}

void GCSKT::SetUnlockAll( bool bUnlock )
{
	if ( bUnlock )
	{
		if ( m_bUnlockAll == false )
		{
			m_mapSetting2 = m_mapSetting;
			m_mapSkillSets2 = m_mapSkillSets;
			m_mapTrainedSkill2 = m_mapTrainedSkill;
			m_mapUnLockGroupID2 = m_mapUnLockGroupID;
		}
		
		m_mapSetting.clear();
		m_mapSkillSets.clear();
		m_mapTrainedSkill.clear();
		m_mapUnLockGroupID.clear();

		m_mapTrainedSkill;
		for ( int i = 0; i < GC_CHAR_NUM ; i++ )
		{
			for ( int j = -1; j < NUM_JOB_LEVEL; j++ )
			{
				SkillSet set;
				m_mapTrainedSkill[std::make_pair((EGCCharType)i,j)] = set;
				std::vector<KSPSetInfo> vec;
				KSPSetInfo temp;
				vec.push_back( temp );
				m_mapSkillSets[std::make_pair(i,j)] = vec;
			}
		}

	}
	else 
	{
		m_mapSetting = m_mapSetting2;
		m_mapSkillSets = m_mapSkillSets2;
		m_mapTrainedSkill = m_mapTrainedSkill2;
		m_mapUnLockGroupID = m_mapUnLockGroupID2;
	}


	m_bUnlockAll = bUnlock;
}

void GCSKT::SetOrigSkillTree()
{
	SetOwnerPlayer( &g_MyD3D->m_TempPlayer );
	SetUnlockAll( false );
	SetCharacterSelectable( true );	
	m_bPromotionSelectable = true;
}

int GCSKT::GetOpenSkillSlotIndex( char cCharType_ )
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ||
        g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL ||
        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL ) {
        return KSkillSlot::INDEX_MAX;
    }
    std::map< char, int >::iterator mit = m_mapSlotOpenInfo.find( cCharType_ );
    if( mit != m_mapSlotOpenInfo.end() ) {
        return mit->second;
    }
    return KSkillSlot::INDEX_A;
}

bool GCSKT::IsSkillMissionSkillID( EGCSkillTree eSkillID_ )
{
#if defined( LOCK_ASIN_HIDDEN_SKILL )
    switch( eSkillID_ )
    {
    case SID_ASIN_SUPER_HIDDEN_MP4:
    case SID_ASIN_SUPER_HIDDEN_UNBEATABLE_BUFF:
    case SID_ASIN_SUPER_HIDDEN_SPECIAL_4:
        return true;
    default:
        return false;
    }
#else
    std::map< DWORD, int >::iterator mit = m_mapSkillMissionInfo.begin();

    for( mit; mit != m_mapSkillMissionInfo.end(); ++mit ) {
        if( mit->second == static_cast< int >( eSkillID_ ) ) {
            return true;
        }
    }
    return false;
#endif
}

int GCSKT::GetCurrentSkillSetSize()
{
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit = m_mapSkillSets.find( std::make_pair( m_cCurUICharType, m_cCurUIPromotion ) );
    if( mit != m_mapSkillSets.end() ) {
        return static_cast< int >( mit->second.size() );
    }

    return 0;
}

void GCSKT::SetNewCharacterTrainedSkill( const KSPInfo& info_ )
{
    std::pair< EGCCharType, char > prKey( static_cast< EGCCharType >( info_.m_cCharType ), info_.m_cPromotion );
    IterTrainedSK itTrainSkill = m_mapTrainedSkill.find( prKey );

    // 배운 스킬 정보 있으면 신규 캐릭터 스틸 정보가 아님
    if( itTrainSkill != m_mapTrainedSkill.end() ) {     
        return;
    }

    SkillSet setSkill;
    TransformSkillInfo( info_, setSkill );
    m_mapTrainedSkill.insert( std::make_pair( prKey, setSkill ) );
}

void GCSKT::SetUpdateTrainedSkill( const KSPInfo& info_ )
{
    std::pair< EGCCharType, char > prKey( static_cast< EGCCharType >( info_.m_cCharType ), info_.m_cPromotion );
    IterTrainedSK itTrainSkill = m_mapTrainedSkill.find( prKey );

    // 이미 배운 스킬 정보가 있으면 갱신한다.
    if( itTrainSkill != m_mapTrainedSkill.end() ) 
    {     
        SkillSet setSkill;
        TransformSkillInfo( info_, setSkill );
        itTrainSkill->second = setSkill;
    }
    else
    {
        SkillSet setSkill;
        TransformSkillInfo( info_, setSkill );
        m_mapTrainedSkill.insert( std::make_pair( prKey, setSkill ) );
    }
}

GCSkillResource* GCSKT::GetSkillResource( EGCSkillTree eSID_ )
{
    std::map< EGCSkillTree, GCSkillResource* >::iterator mit = m_mapSkillRes.find( eSID_ );

    if( mit != m_mapSkillRes.end() ) {
        return mit->second;
    }
    return NULL;
}
