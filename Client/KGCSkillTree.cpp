#include "stdafx.h"
#include ".\KGCSkillTree.h"
#include "GCSkill.h"
#include "GCSkillResource.h"
#include "../gcui/GCUIScene.h"
#include "../gcui/GCSkillTreeDlg.h"
#include "GCUI/GCToolTip.h"
#include "Replay.h"

bool bReloadSkillTree = false;

ImplementSingleton( KGCSkillTree );


#include "GCUtil/GCFileMoniter.h"

void InitSkillTreeFileMoniterNew()
{
    bReloadSkillTree = true;    
}

KGCSkillTree::KGCSkillTree(void)
{
	if( g_pGCDeviceManager2 )
		SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( "InitSkillTree.lua" ) , InitSkillTreeFileMoniterNew );

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

	m_bNewSkillMsgRender = true;

    m_iMaxSID = 0;
    BindToLua();

	m_pLocalPlayer = &g_MyD3D->m_TempPlayer;

	m_bCharacterSelectable = true;
	m_bPromotionSelectable = true;
	m_bUnlockAll = false;

    for( int i = 0; i < KSkillSlot::INDEX_MAX; i++ ) {
        m_iHotKeyIndex[i] = 0;
    }

    m_iCurSlotType = KSkillSlot::INDEX_A;

}

KGCSkillTree::~KGCSkillTree(void)
{
    clear();
}

const std::wstring& KGCSkillTree::GetString( int iID )
{
    return m_kSkillStr.GetString( iID );
}

bool KGCSkillTree::LoadSkillScript( KLuaManager& luaMgr )
{
    LUA_GET_VALUE( "SID_MAX", m_iMaxSID, return false );

	m_mapOrderSkillByLevel.clear();
    LUA_BEGIN_TABLE( "SkillTreeSkills", return false )
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

                m_mapOrderSkillByLevel[ std::make_pair( pNewSkill->GetCharType(), pNewSkill->GetPromotion() ) ].push_back( pNewSkill );
            }
            LUA_END_TABLE( return false );
        }
    }
    LUA_END_TABLE( return false );


	std::map< std::pair< char, char >, std::vector< GCSkill* > >::iterator mit;
	for( mit = m_mapOrderSkillByLevel.begin() ; mit != m_mapOrderSkillByLevel.end() ; ++mit )
	{
		std::sort( mit->second.begin(), mit->second.end(), SortByLevelToSkill() );
	}

    FillSkillTree();
    return true;
}

bool KGCSkillTree::LoadSkillString( const std::string& strSkill )
{
    m_kSkillStr.Clear();
    return m_kSkillStr.LoadStringFile( strSkill );
}

bool KGCSkillTree::LoadSkillStringFromKOM( const std::string& strSkill )
{
    m_kSkillStr.Clear();
    return m_kSkillStr.LoadStringFromKOM( strSkill );
}

void KGCSkillTree::clear()
{
    m_kSkillStr.Clear();

    for( IterSkill mit = m_mapSkill.begin(); mit != m_mapSkill.end(); ++mit )
    {
        SAFE_DELETE( mit->second );
    }
    m_mapSkill.clear();
    m_mapTrainedSkill.clear();
    m_mapSkillUsers.clear();
	m_mapCashSkillPosition.clear();
	m_mapSkillItem.clear();
    m_nBuyingReqGroup = -1;
}

void KGCSkillTree::FillSkillTree()
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
            assert( "NextSkill의 PreList에 해당 스킬이 없음" &&
                ( setSkill.end() != setSkill.find( pSkill->GetID() ) ) );
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
void KGCSkillTree::GetSKillPtr( IN const SkillSet& setID, OUT SkillPtrList& lSkill )
{
    lSkill.clear();

    SkillSet::const_iterator lit;
    for( lit = setID.begin(); lit != setID.end(); ++lit )
    {
        const EGCSkillTree& eSkill = (*lit);
        IterSkill mit = m_mapSkill.find( eSkill );
        assert( mit != m_mapSkill.end() );
        if( mit != m_mapSkill.end() )
        {
            lSkill.push_back( mit->second );
        }
    }
}

// Skill ID를 넣으면 해당 스킬을 익히기 위해 필요한 이전 스킬들을 담아준다. recursive
void KGCSkillTree::GetPrecedeSkill( EGCSkillTree eID, OUT SkillSet& setPrecede )
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
bool KGCSkillTree::CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill )
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

    ItserSkillUsers it = m_mapSkillUsers.find( eSkill );
    if( it == m_mapSkillUsers.end() )
    {
        return false;
    }
    std::set<DWORD>& setUID = it->second;
    std::set<DWORD>::iterator sit = setUID.find( pPlayer->m_kUserInfo.dwUID );
    if( sit != setUID.end() )
    {
        return true;
    }
    return false;

}

bool KGCSkillTree::IsSkillTrainable( EGCSkillTree eSkill )
{       
	if( SiKGCSkillTree()->IsLearnedSkill( eSkill ) )
		return false;

    if( g_kGlobalValue.IsTutorialMode() ) {
            return true;
    }

    if( GetOwnerPlayer()->m_kUserInfo.vecCharInfo.empty() )
        return false;

    IterSkill mit = m_mapSkill.find( eSkill );
    assert( mit != m_mapSkill.end() );
    SkillSet setPrev;
    GetPrecedeSkill( eSkill, setPrev );

	GCSkill* pSkill = mit->second;

	SCharInfo& sChar = GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( pSkill->GetCharType() );

	if( pSkill->GetLv() > sChar.iLevel )
		return false;
    
	if( pSkill->GetPromotion() != -1 && sChar.IsAcquiredPromotion( pSkill->GetPromotion() ) == false )
		return false;

	if( pSkill->GetLearnSP() > GetExtraSP( pSkill->GetCharType() ) )
    {
        if( g_kGlobalValue.IsTutorialMode() ) {
                return true;
        }
		return false;
    }

	// 선행스킬이 없다면..
	if( setPrev.empty() )
	{
		return true;
	}

    IterTrainedSK itTRSk = m_mapTrainedSkill.find( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion()) );
    if( itTRSk == m_mapTrainedSkill.end() )
        return false;
    SkillSet& setTR = itTRSk->second;
    // 내가 배운 스킬들이 이 스킬을 배우기 위한 선행스킬에 부분집합인지 확인
    return std::includes( setTR.begin(), setTR.end(), setPrev.begin(), setPrev.end() );
}

void KGCSkillTree::InsertUserSkillSet( DWORD dwUserUID, const SkillSet& setSKill )
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

void KGCSkillTree::RemoveUserSkillSet( DWORD dwUserUID )
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

void KGCSkillTree::Server_SetTrainedSkill( const std::map<std::pair<char,char>, KSPInfo>& kFullInfo )
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

void KGCSkillTree::TransformSkillInfo( const KSPInfo& kSPInfo, OUT SkillSet& setSkill )
{
    std::vector<int>::const_iterator vit = kSPInfo.m_vecSkills.begin();
    while( vit != kSPInfo.m_vecSkills.end() )
    {
        setSkill.insert( (EGCSkillTree)(*vit) );
        ++vit;
    }
}

SkillSet* KGCSkillTree::GetTrainedSkillInfo( EGCSkillTree eSkillID )
{
	IterSkill mit = m_mapSkill.find( eSkillID );
	if( mit == m_mapSkill.end() ) {
		assert( mit != m_mapSkill.end() );
		return NULL;
	}

	GCSkill* pSkill = mit->second;
    std::map< std::pair<EGCCharType, char>, SkillSet >::iterator itSk = m_mapTrainedSkill.find( std::make_pair(pSkill->GetCharType(), pSkill->GetPromotion()) );

    if( itSk != m_mapTrainedSkill.end() ) {
        return &itSk->second;
    } else {
        if( g_kGlobalValue.m_kUserInfo.IsCharExist( pSkill->GetCharType() ) ) {
            itSk = m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion()) , SkillSet() )).first;
            m_mapTrainedSkill.insert( TrainedSKValue( std::pair<EGCCharType, char>(pSkill->GetCharType(), -1) , SkillSet() ));

            if( itSk != m_mapTrainedSkill.end() ) {                
                return &itSk->second;
            }
        }
		assert( itSk != m_mapTrainedSkill.end() );
		return NULL;
	}	
}

bool KGCSkillTree::Server_TrainSkill( int nSkillID )
{
    SkillSet* setSkill = GetTrainedSkillInfo( (EGCSkillTree)nSkillID );

	if( setSkill == NULL )
		return false;

    (*setSkill).insert( (EGCSkillTree)nSkillID );
    return true;
}

bool KGCSkillTree::Server_UnTrainSkill( int nSkillID )
{
	// 장착하고 있던 것들도 다 빼자!!!
	std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mitEquiped;
	for( mitEquiped = m_mapSkillEquipInfo.begin() ; mitEquiped != m_mapSkillEquipInfo.end() ; ++mitEquiped ) {
		std::vector<KSkillSlot>::iterator vit = std::remove_if( mitEquiped->second.begin(), mitEquiped->second.end(), FindEquipedSkill( (EGCSkillTree)nSkillID ) );
		mitEquiped->second.erase( vit, mitEquiped->second.end() );
	}

	SkillSet* setSkill = GetTrainedSkillInfo( (EGCSkillTree)nSkillID );

	if( setSkill == NULL )
		return false;

	(*setSkill).erase( (EGCSkillTree)nSkillID );
    return true;
}

const GCSkill* KGCSkillTree::GetSkill( EGCSkillTree eID )
{
	if( m_mapSkill.find( eID ) == m_mapSkill.end() )
		return NULL;

	return m_mapSkill[eID];
}

void KGCSkillTree::GetSkillSet( int iCharType, int nPromotion, OUT std::vector< EGCSkillTree >& setSkill )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( iCharType, nPromotion ) );

    if( mit != m_mapSkillEquipInfo.end() ) {
        for( int i = 0; i < static_cast< int >( mit->second.size() ); i++ ) {
            setSkill.push_back( static_cast< EGCSkillTree >( mit->second[i].m_nSkillID ) );
        }
    }
}

std::vector<EGCSkillTree> KGCSkillTree::GetHotKeySkillVector( int iCharType, int nPromotion )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( iCharType, nPromotion ) );
    std::vector<EGCSkillTree> vecRet;

    if( mit != m_mapSkillEquipInfo.end() ) {
        for( int i = 0; i < static_cast< int >( mit->second.size() ); i++ ) {
            if( mit->second[i].m_nSlotIndex == m_iCurSlotType && 
                GetSkill( (EGCSkillTree)mit->second[i].m_nSkillID )->m_pHotKeySkill && 
                GetSkill( (EGCSkillTree)mit->second[i].m_nSkillID )->m_pHotKeySkill->m_bNoEquipSlot == false ) {
                vecRet.push_back( (EGCSkillTree)mit->second[i].m_nSkillID );
            }
        }
    }

    return vecRet;
}

std::vector<EGCSkillTree> KGCSkillTree::GetHotKeySkillVectorByType( int iCharType, int nPromotion, int iSlotType_ )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( iCharType, nPromotion ) );
    std::vector<EGCSkillTree> vecRet;

    if( mit != m_mapSkillEquipInfo.end() ) {
        for( int i = 0; i < static_cast< int >( mit->second.size() ); i++ ) {
            if( mit->second[i].m_nSlotIndex == iSlotType_ && 
                GetSkill( (EGCSkillTree)mit->second[i].m_nSkillID )->m_pHotKeySkill && 
                GetSkill( (EGCSkillTree)mit->second[i].m_nSkillID )->m_pHotKeySkill->m_bNoEquipSlot == false ) {
                vecRet.push_back( (EGCSkillTree)mit->second[i].m_nSkillID );
            }
        }
    }

    return vecRet;
}

void KGCSkillTree::Server_SetSkillEquipInfo( std::map< std::pair<char,char>, std::vector<KSkillSlot> > kSkillEquiptInfo_ )
{
    std::map< std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit;
    

    for ( int k = 0; k < GC_CHAR_NUM; k++ )
    {
        for ( int i = 0 ; i < NUM_JOB_LEVEL ; i++ )
        {
            mit = kSkillEquiptInfo_.find( std::pair<char,char>((char)k, (char)i) );

            if( mit == kSkillEquiptInfo_.end() ) {
                continue;
            }

            std::vector<KSkillSlot> &vec = mit->second;
            std::vector<KSkillSlot> vecTemp;
            KSkillSlot skill;
            skill.m_nSkillID = SID_DIO1_EMPTY_SLOT;
            skill.m_nSlotID = SG_DIO1_EMPTY_SLOT;

            for ( unsigned int j = 0; j < (vec.size() + 5); j++ )   
            {
                vecTemp.push_back(skill);
            }

            for ( unsigned int j = 0; j < vec.size(); j++ )
            {
                if ( vec[j].m_nSlotID > (int)vecTemp.size() )
                {
                    vecTemp[j] = vec[j];
                }
                else
                {
                    if( vec[j].m_nSlotID > 0 ) {
                        vecTemp[vec[j].m_nSlotID-1] = vec[j];
                    }                    
                }

            }

            vecTemp.swap( vec );

            std::vector<KSkillSlot>::iterator vit = vec.begin();
            while( vit != vec.end() )
            {
                if (GetSkill( (EGCSkillTree)vit->m_nSkillID ) == NULL ){
                    vit++;
                    continue;
                }
                vit->m_nSlotID = GetSkill( (EGCSkillTree)vit->m_nSkillID )->GetGroupID();
                vit++;

            }
        }
    }
        
    m_mapSkillEquipInfo = kSkillEquiptInfo_;

    if (g_kGlobalValue.ServerInfo.CheckServerType( ST_LOCK_SKILL_TREE ) == true )
    {
        m_mapSkillEquipInfo.clear();
        return;
    }
}

std::set< EGCSkillTree > KGCSkillTree::GetSkillIDsInGroup( EGCSkillGroup eGroupID )
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

bool KGCSkillTree::IsLearnedSkill( EGCSkillTree eSkill )
{    
	const GCSkill* pSkill = GetSkill( eSkill );

	if( pSkill == NULL )
		return false;

    // 연습모드에서 공용스킬이 아니면 무조건 배울걸로 해주자.
    if ( g_kGlobalValue.IsTutorialMode() ) {
        if( pSkill->GetPromotion() != -1 ) {
            return true;
        } else {
            if( pSkill->m_iAwakeningLv != -1 ) {
                return true;
            }

            IterTrainedSK mit = m_mapTutorialTrainedSkill.find( std::pair<EGCCharType, char>(pSkill->GetCharType(), pSkill->GetPromotion() ) );
            
            if( mit != m_mapTutorialTrainedSkill.end() ) {
                SkillSet& setSkill = mit->second;
                IterSkillSet itList = std::find( setSkill.begin(), setSkill.end(), eSkill );
                if( itList != setSkill.end() )
                {
                    return true;
                }
            }    
            return false;
        }
    }

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

const GCSkill* KGCSkillTree::GetEquipedSkill( EGCSkillGroup eGroupID )
{	
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
			return NULL;

        return GetSkillFromItemID( pItem->m_ItemID );	
	}

    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit;

	for( mit = m_mapSkillEquipInfo.begin() ; mit != m_mapSkillEquipInfo.end() ; ++mit )
	{		
		std::vector<KSkillSlot>::iterator vit;
		vit = std::find_if( mit->second.begin(), mit->second.end(), FindEquipedSkillInGroup( eGroupID ) );

		if( vit != mit->second.end() )
		{          
			return GetSkill( (EGCSkillTree)(*vit).m_nSkillID );
		}
	}

	return NULL;
}

void KGCSkillTree::EquipSkill( EGCSkillTree eSkillID )
{
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

    //if( m_mapSkillEquipInfo.find( std::make_pair( iCharType, iPromotion ) ) == m_mapSkillEquipInfo.end() ) {
    //    iPromotion = m_cCurUIPromotion;
    //}

    //if( m_mapSkillEquipInfo.find( std::make_pair( iCharType, iPromotion ) ) == m_mapSkillEquipInfo.end() ) {
    //    return;
    //}

    if( m_mapSkillEquipInfo.find( std::make_pair( iCharType, iPromotion ) ) == m_mapSkillEquipInfo.end() ) {
        std::vector<KSkillSlot> vecSkill;
        m_mapSkillEquipInfo.insert( std::make_pair( std::make_pair(pSkill->GetCharType(), pSkill->GetPromotion()) , vecSkill ) );
    }

    //if( pSkill->GetPromotion() == -1 ) 
    //{
    //    for( int i = 0 ; i < NUM_JOB_LEVEL ; ++i )
    //    {
    //        EquipSkill( pSkill, i );
    //    }
    //}
    //else
    {
		EquipSkill( pSkill, iPromotion );
    }		   
}

void KGCSkillTree::EquipSkill( const GCSkill* pSkill, int iPromotion )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( pSkill->GetCharType(), iPromotion ) );

    if( mit != m_mapSkillEquipInfo.end() ) {
        bool bSlotSkillUnequipped;
        UnEquipSkillGroup( mit->second, pSkill->GetGroupID(), &bSlotSkillUnequipped );
        //if ( bSlotSkillUnequipped )
        //{
        //    SetHotKeyIndexToEmpty( pSkill->GetCharType(), iPromotion );
        //}

        if ( pSkill->m_pHotKeySkill )
        {
            EquipHotKeySkill( mit->second, pSkill );		
        }
        else
        {
            KSkillSlot skill;
            skill.m_nSkillID = pSkill->GetID();
            skill.m_nSlotID = pSkill->GetGroupID();

            mit->second.push_back( skill );		
        }

        START_LOG( clog, L"스킬 장착 : " << pSkill->GetID() );
    }
}

void KGCSkillTree::EquipHotKeySkill( std::vector<KSkillSlot>& setInfo, const GCSkill* pSkill )
{
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
            skill.m_nSlotIndex = m_iCurSlotType;
		}
		setInfo.push_back( skill );				
		iSlotSkillIndex++;
	}
}

bool KGCSkillTree::EquipItemSkill( DWORD dwItemID )
{
	if( m_mapSkillItem.find( dwItemID ) == m_mapSkillItem.end() )
	{
		//ASSERT( !"스킬 트리에 없는 아이템이여!!" );
		return false;
	}

    const GCSkill* pSkill = GetSkill( m_mapSkillItem[dwItemID] );

	int iCharType = (int)( pSkill->GetCharType() );
	int iPromotion = (int)( pSkill->GetPromotion() );


    EquipSkill( pSkill->GetID() );

	return true;
}

void KGCSkillTree::UnequipSkill( EGCSkillGroup eGroup )
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

    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.begin();

    for( mit; mit != m_mapSkillEquipInfo.end(); ++mit ) {
        bool bSlotSkillUnequipped;        
        UnEquipSkillGroup( mit->second, eGroup, &bSlotSkillUnequipped );
        //if ( bSlotSkillUnequipped )
        //{
        //    SetHotKeyIndexToEmpty( mit->first.first, mit->first.second );
        //}
    }

    START_LOG( clog, L"스킬 해제 : " << eGroup );
}

void KGCSkillTree::ApplySettingToServer()
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> > mapSkillEquipInfo = m_mapSkillEquipInfo;
	for ( int k = 0; k < GC_CHAR_NUM; k++ )
	{
        if( g_kGlobalValue.IsNewSkillTreePlayer(k) ) {
		    for( int i = -1; i < NUM_JOB_LEVEL; i++ )
		    {
			    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit;
    			
			    mit = mapSkillEquipInfo.find( std::pair<char,char>((char)k, (char)i) );

                if( mit != mapSkillEquipInfo.end() ) {
                    std::vector<KSkillSlot> vecExtract;
                    std::vector<KSkillSlot>::iterator vit = mit->second.begin();
                    int iIncresingNum = 1;
                    while( vit != mit->second.end() )
                    {
                        vit->m_nSlotID = iIncresingNum;
                        iIncresingNum++;
                        if ( vit->m_nSkillID != SID_DIO1_EMPTY_SLOT )
                        {
                            vecExtract.push_back( *vit );
                        }
                        vit++;
                    }	

                    mit->second.swap( vecExtract );
                }
            }
        }
	}

    // ui에서 스킬 배우고, 제거한 내용들의 패킷을 보내자!!
    std::set< int > setTrainSkillID;
    for( std::set< EGCSkillTree >::iterator sit = m_setSaveTrainSkill.begin(); sit != m_setSaveTrainSkill.end(); ++sit ) {
        setTrainSkillID.insert( static_cast< int >( *sit ) );
    }

    std::set< int > setRemoveSkillID;
    std::pair<bool,DWORD> UntrainItem = SiKGCSkillTree()->IsExistSkillUntrainItem();
    for( std::set< EGCSkillTree >::iterator sit = m_setSaveRemoveSkill.begin(); sit != m_setSaveRemoveSkill.end(); ++sit ) {
        setRemoveSkillID.insert( static_cast< int >( *sit ) );
    }

    KEVENT_CHANGE_SKILL_TREE_REQ kPacket;
    kPacket.m_setTrainSkill = setTrainSkillID;
    kPacket.m_dwItemID = UntrainItem.second;
    kPacket.m_setRemoveSkill = setRemoveSkillID;
    kPacket.m_mapSkillEquipInfo = mapSkillEquipInfo;   
    KP2P::GetInstance()->Send_ChangeSkillInfo( kPacket );

    // 패킷 다보내고 나면 설정 저장했던 내용은 지워주자.
    ClearTrainRemoveSkill();
}

int KGCSkillTree::GetCurrentSP( int iCharType )
{
    return GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).kSkillInfo.m_nLvSPPoint;
}

int KGCSkillTree::GetExtraSP( int iCharType )
{
    //std::map< std::pair<EGCCharType, char>, SkillSet > m_mapTrainedSkill; // 내가배운스킬
    //typedef std::set< EGCSkillTree > SkillSet

	int iUsedSP = 0;
	if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) || g_kGlobalValue.IsHybridPlayer( iCharType ) || g_kGlobalValue.IsNewSkillTreePlayer( iCharType ) )
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

int KGCSkillTree::GetMaxSP( int iCharType ,int iPromotion)
{
    // 서버에서 받는값으로 고치자!
	//return 150;

    KEVENT_MAX_CHAR_SP_LEVEL_NOT::iterator mIter = m_mapMaxSkillInfo.find(std::pair<char,char>(iCharType, iPromotion));

    if(mIter == m_mapMaxSkillInfo.end())
        return 0;

    return mIter->second.second;
    
}

int KGCSkillTree::GetUsedSPinPromotion( int iCharType, int iPromotion )
{
	int iUsedSP = 0;
    if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) ||  g_kGlobalValue.IsHybridPlayer( iCharType ) || g_kGlobalValue.IsNewSkillTreePlayer( iCharType ) )            
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

int KGCSkillTree::GetMaxSPinPromotion( int iCharType , int iPromotion )
{
    int iMaxSP = 35;
	if( g_kGlobalValue.IsQuickSkillPlayer( iCharType ) ||
        g_kGlobalValue.IsHybridPlayer( iCharType ) ||
        g_kGlobalValue.IsNewSkillTreePlayer( iCharType ) )
	{
        if ( SiKGCSkillTree()->IsUnlockAll() )
        {
            return 60;
        }

        KEVENT_MAX_CHAR_SP_LEVEL_NOT::iterator mapIter = m_mapMaxSkillInfo.find( std::make_pair( iCharType, GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).GetMaxPromotionNum() ) );
        if( mapIter != m_mapMaxSkillInfo.end() )
        {
            iMaxSP = mapIter->second.second;
        }
        // 2011-12-04 shmhlove, 위 코드(map에서 찾아서 돌려주는)로 변경하기 이전에 하드코드로 박혀있던 내용을 아래에 유지시켜봅니다.
        else
        {
            if ( GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).GetMaxPromotionNum() == 0 )
            {
                if( GC_CHAR_LUPUS == iCharType )
                    iMaxSP = 40;
                else
                    iMaxSP = 30;
            }
            else if ( GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).GetMaxPromotionNum() == 1 )
            {
                if( GC_CHAR_LUPUS == iCharType )
                    iMaxSP = 80;
                else
                    iMaxSP = 60;
            }
            else if ( GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).GetMaxPromotionNum() == 2 )
            {
                if( GC_CHAR_LUPUS == iCharType )
                    iMaxSP = 120;
                else
                    iMaxSP = 90;
            }
            else if ( GetOwnerPlayer()->m_kUserInfo.GetCurrentChar( iCharType ).GetMaxPromotionNum() == 3 )
            {
                if( GC_CHAR_LUPUS == iCharType )
                    iMaxSP = 160;
                else
                    iMaxSP = 120;
            }
        }
	}

	return iMaxSP;
}

void KGCSkillTree::UnequipAllSkill( int iCharType, int iPromotion )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( iCharType, iPromotion ) );

    if( mit != m_mapSkillEquipInfo.end() ) {
        mit->second.clear();
    }
}

const GCSkill* KGCSkillTree::GetSkillFromItemID( DWORD dwItemID )
{
	if( m_mapSkillItem.find( dwItemID ) == m_mapSkillItem.end() )
		return NULL;

	return GetSkill( m_mapSkillItem[dwItemID] );
}

bool KGCSkillTree::IsBlockedSkill( EGCCharType eType, int nGroup )
{
    return m_bUnlockAll;
}

bool KGCSkillTree::Process_UnlockGroupItem( int nGroup, bool bBrowserClose )
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

		#ifdef GAME_CASH
				g_pkUIScene->EnableBuyCashItemWnd( ITEM_ID_OPEN_BLOCKSKILLGROUP, false, true );
		#else    
        g_pItemMgr->BuyItem( ITEM_ID_OPEN_BLOCKSKILLGROUP, 1, false );
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

void KGCSkillTree::CheckNewSkill()
{
	m_bNewSkillMsgRender = true;
	if ( GetOwnerPlayer()->m_kUserInfo.GetCharSize() == 0 )
	{
		return;
	}

	std::map< EGCSkillTree, GCSkill* >::iterator mit;
	for( mit = m_mapSkill.begin() ; mit != m_mapSkill.end() ; ++mit )
	{
		mit->second->SetNew( true );

		if( mit->second->GetLv() <= GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(mit->second->GetCharType() ).iLevel )
		{
			mit->second->SetNew( false );
		}
	}
}

bool KGCSkillTree::IsNewSkill( EGCSkillTree eSkillID )
{
	if( m_mapSkill.find( eSkillID ) == m_mapSkill.end() )
		return false;

	if( m_mapSkill[eSkillID] == NULL )
		return false;

	bool bNew = m_mapSkill[eSkillID]->isNew() && m_mapSkill[eSkillID]->GetLv() > GetOwnerPlayer()->m_kUserInfo.GetCurrentChar(m_mapSkill[eSkillID]->GetCharType() ).iLevel;
	m_mapSkill[eSkillID]->SetNew( false );
	return bNew;
}

int KGCSkillTree::GetNewSkillNum()
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

bool KGCSkillTree::IsEquipedSkill( EGCSkillTree eSkillID )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.begin();

    for( mit; mit != m_mapSkillEquipInfo.end(); ++mit ) {
        std::vector<KSkillSlot>::iterator vit = std::find_if( mit->second.begin(), mit->second.end(), FindEquipedSkill( eSkillID ) );
        if( vit != mit->second.end() ) {
            return true;
        }
    }

    return false;
}

bool KGCSkillTree::IsEquipedSkill( EGCSkillTree eSkillID, char cCharType_, char cPromotion_ )
{
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mit = m_mapSkillEquipInfo.find( std::make_pair( cCharType_, cPromotion_ ) );

    if( mit != m_mapSkillEquipInfo.end() ) {
        std::vector<KSkillSlot>::iterator vit = std::find_if( mit->second.begin(), mit->second.end(), FindEquipedSkill( eSkillID ) );
        if( vit != mit->second.end() ) {
            return true;
        }
    }
    return false;
}

const GCSkill* KGCSkillTree::GetTutorialSimmilarLevelSkill( int iLevel )
{
    std::vector< GCSkill* >::iterator vit;

    std::pair< char, char > prKey = std::make_pair( m_cCurUICharType, m_cCurUIPromotion );
    for( vit = m_mapOrderSkillByLevel[ prKey ].begin() ; vit != m_mapOrderSkillByLevel[ prKey ].end() ; ++vit )
    {
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

const GCSkill* KGCSkillTree::GetSimmilarLevelSkill( int iLevel )
{
	//std::vector< GCSkill* > m_mapOrderByLevel;

	std::vector< GCSkill* >::iterator vit;
    std::pair< char, char > prKey = std::make_pair( m_cCurUICharType, m_cCurUIPromotion );

	for( vit = m_mapOrderSkillByLevel[ prKey ].begin() ; vit != m_mapOrderSkillByLevel[ prKey ].end() ; ++vit )
	{
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

const GCSkill* KGCSkillTree::GetNextLevelSkill( const GCSkill* pSkill )
{
	std::vector< GCSkill* >::iterator vit;
    std::pair< char, char > prKey = std::make_pair( m_cCurUICharType, m_cCurUIPromotion );

	vit = std::find( m_mapOrderSkillByLevel[ prKey ].begin(), m_mapOrderSkillByLevel[ prKey ].end() , pSkill );

	if( vit == m_mapOrderSkillByLevel[ prKey ].end() )
		return NULL;

	if( vit + 1 == m_mapOrderSkillByLevel[ prKey ].end() )
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

const GCSkill* KGCSkillTree::GetPrevLevelSkill( const GCSkill* pSkill )
{

	std::vector< GCSkill* >::iterator vit;
    std::pair< char, char > prKey = std::make_pair( m_cCurUICharType, m_cCurUIPromotion );

	vit = std::find( m_mapOrderSkillByLevel[ prKey ].begin(), m_mapOrderSkillByLevel[ prKey ].end() , pSkill );

	if( vit == m_mapOrderSkillByLevel[ prKey ].end() )
		return NULL;

	if( m_mapOrderSkillByLevel[ prKey ].empty() )
		return NULL;

	if( vit == m_mapOrderSkillByLevel[ prKey ].begin() )
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

std::vector< EGCSkillTree > KGCSkillTree::GetSkillIDsInGroupInVector( EGCSkillGroup eGroupID )
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

	std::sort( vecGroupSkill.begin(), vecGroupSkill.end(), SortByLevelToSkillID() );

	return vecGroupSkill;
}

std::pair<bool,DWORD> KGCSkillTree::IsExistSkillUntrainItem()
{
	std::pair<bool,DWORD> UntraindItem( false, 0 );
	int iDuration = INT_MAX;

	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD, iDuration, UntraindItem );
	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD2, iDuration, UntraindItem );
	CheckUntrainItem( ITEM_ID_SKILLUNTRAIN_CARD3, iDuration, UntraindItem );

	return UntraindItem;
}

void KGCSkillTree::CheckUntrainItem( IN DWORD dwItemID, IN OUT int& iDuration , OUT std::pair<bool,DWORD>& ret )
{
	if( true == g_pItemMgr->FindInventoryForItemID( dwItemID ) &&
		iDuration > g_pItemMgr->GetInventoryItemDuration( dwItemID ) )
	{
		ret.first = true;
		ret.second = dwItemID;
		iDuration = g_pItemMgr->GetInventoryItemDuration( dwItemID );
	}
}

void KGCSkillTree::UnEquipSkillGroup( std::vector<KSkillSlot>& setInfo, EGCSkillGroup eGroup, OUT bool* pbSlotSkillUnequipped )
{
	if ( pbSlotSkillUnequipped )
	{
		*pbSlotSkillUnequipped = false;
	}
	std::vector<KSkillSlot>::iterator vit = setInfo.begin();
	while( vit != setInfo.end() )
	{
        if( vit->m_nSlotID == eGroup )
		{
			if( GetSkill( (EGCSkillTree)vit->m_nSkillID )->m_pHotKeySkill )
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

EGCSkillTree KGCSkillTree::GetMaxSID()
{
    return (EGCSkillTree)m_iMaxSID;
}

void KGCSkillTree::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCSkillTree>( L, "KGCSkillTree" );
    lua_tinker::class_def<KGCSkillTree>( L,  "GetNewSkillNum",  &KGCSkillTree::GetNewSkillNum  );
    lua_tinker::class_def<KGCSkillTree>( L,  "IsNewSkillMsgRender",  &KGCSkillTree::IsNewSkillMsgRender  );

    lua_tinker::decl( L, "SkillMgr", this );
}

void KGCSkillTree::SetHotKeyIndexToEmpty( int iChar, int iPromotion )
{
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
}

void KGCSkillTree::SetHotKeyIndexToEmpty()
{
	SetHotKeyIndexToEmpty( m_cCurUICharType, m_cCurUIPromotion );
}
void KGCSkillTree::Server_SetFullSPInfo( KSkillTreeFullInfo& kRecv )
{	
	Server_SetTrainedSkill( kRecv.m_mapTrainedSkill );
	Server_SetSkillEquipInfo( kRecv.m_mapSkillEquipInfo ); // 일반 장착스킬 정보 셋팅하도록 바꿔주자

    // 유저가 보유한 캐릭터 별 스킬 장착 정보를 갱신한다.
	g_kGlobalValue.m_kUserInfo.UpdateSkillEquipInfo( kRecv.m_mapSkillEquipInfo );
	CheckNewSkill();

    //스킬미션 작업
    m_mapSlotOpenInfo = kRecv.m_mapSlotOpenInfo;
    m_mapSkillMissionInfo = kRecv.m_mapSkillMissionInfo;
}

void KGCSkillTree::SetUnlockAll( bool bUnlock )
{
	if ( bUnlock )
	{
		if ( m_bUnlockAll == false )
		{
            m_mapSkillEquipInfo2 = m_mapSkillEquipInfo;
			m_mapTrainedSkill2 = m_mapTrainedSkill;
		}
		
		m_mapSkillEquipInfo.clear();
		m_mapTrainedSkill.clear();

		for ( int i = 0; i < GC_CHAR_NUM ; i++ )
		{
			for ( int j = -1; j < 4; j++ )
			{
				SkillSet set;
				m_mapTrainedSkill[std::make_pair((EGCCharType)i,j)] = set;
				std::vector<KSkillSlot> vec;
				m_mapSkillEquipInfo[std::make_pair(i,j)] = vec;
			}
		}

	}
	else 
	{
		m_mapSkillEquipInfo = m_mapSkillEquipInfo2;
		m_mapTrainedSkill = m_mapTrainedSkill2;
	}       

    m_mapTutorialTrainedSkill.clear();

	m_bUnlockAll = bUnlock;
}

void KGCSkillTree::SetOrigSkillTree()
{
	SetOwnerPlayer( &g_MyD3D->m_TempPlayer );
	SetUnlockAll( false );
	SetCharacterSelectable( true );	
	m_bPromotionSelectable = true;
}

int KGCSkillTree::GetOpenSkillSlotIndex( char cCharType_ )
{
    if( g_kGlobalValue.IsTutorialMode() ) {
        return KSkillSlot::INDEX_MAX;
    }
    std::map< char, int >::iterator mit = m_mapSlotOpenInfo.find( cCharType_ );
    if( mit != m_mapSlotOpenInfo.end() ) {
        return mit->second;
    }
    return KSkillSlot::INDEX_A;
}

bool KGCSkillTree::IsSkillMissionSkillID( EGCSkillTree eSkillID_ )
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

std::set< EGCSkillTree > KGCSkillTree::GetHotKeySkillSetByChar( char cCharType_, char cPromotion_ )
{
    std::set< EGCSkillTree > setSkill;
    std::map< std::pair< EGCCharType, char >, SkillSet >::iterator mit = m_mapTrainedSkill.find( std::make_pair( ( EGCCharType )cCharType_, cPromotion_ ) );
    if( mit != m_mapTrainedSkill.end() ) {        
        for( std::set< EGCSkillTree >::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit ) {
            const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( *sit );
            if( pSkill != NULL ) {
                if( pSkill->m_pHotKeySkill ) {
                    setSkill.insert( pSkill->GetID() );
                }
            }
        }
    }
    return setSkill;
}

std::set< EGCSkillTree > KGCSkillTree::GetTrainedSkillSetByChar( char cCharType_, char cPromotion_ )
{
    std::set< EGCSkillTree > setSkill;
    std::map< std::pair< EGCCharType, char >, SkillSet >::iterator mit = m_mapTrainedSkill.find( std::make_pair( ( EGCCharType )cCharType_, cPromotion_ ) );

    if( mit != m_mapTrainedSkill.end() ) {
        setSkill = mit->second;
    }

    return setSkill;
}

std::vector< EGCSkillTree > KGCSkillTree::GetSkillsPerClass( char cCharType_, char cPromotion_ )
{
    std::pair< char, char > prKey = std::make_pair( cCharType_, cPromotion_ );
    if( cCharType_ == -1 ) {
        prKey = std::make_pair( m_cCurUICharType, m_cCurUIPromotion );
    }

    std::vector< GCSkill* > vecSkills;
    std::map< std::pair< char, char >, std::vector< GCSkill* > >::iterator mit = m_mapOrderSkillByLevel.find( prKey );
    if( mit != m_mapOrderSkillByLevel.end() ) {
        vecSkills = mit->second;
    }

    std::vector< EGCSkillTree > vecSkillID;
    for( int i = 0; i < static_cast< int >( vecSkills.size() ); i++ ) {
        vecSkillID.push_back( vecSkills[i]->GetID() );
    }
    return vecSkillID;
}

void KGCSkillTree::TutorialTrainingSkill( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = GetSkill( eSkillID_ );

    if( pSkill ) {
        std::pair<EGCCharType, char> prKey = std::make_pair( ( EGCCharType )GetCurUICharType(), pSkill->GetPromotion() );
        std::map< std::pair<EGCCharType, char>, SkillSet >::iterator mit = m_mapTutorialTrainedSkill.find( prKey );
        if( mit != m_mapTutorialTrainedSkill.end() ) {
            mit->second.insert( eSkillID_ );
        } else {
            SkillSet setSkillSet;
            setSkillSet.insert( eSkillID_ );
            m_mapTutorialTrainedSkill.insert( std::make_pair( prKey, setSkillSet ) );
        }
        EquipSkill( eSkillID_ );
    }   
}

void KGCSkillTree::TutorialUnTrainingSkill( EGCSkillTree eSkillID_ )
{    
    // 착용 중 이던 스킬 빼자.
    std::map<std::pair<char,char>, std::vector<KSkillSlot> >::iterator mitEquiped;
    for( mitEquiped = m_mapSkillEquipInfo.begin() ; mitEquiped != m_mapSkillEquipInfo.end() ; ++mitEquiped ) {
        std::vector<KSkillSlot>::iterator vit = std::remove_if( mitEquiped->second.begin(), mitEquiped->second.end(), FindEquipedSkill( eSkillID_ ) );
        mitEquiped->second.erase( vit, mitEquiped->second.end() );
    }

    const GCSkill* pSkill = GetSkill( eSkillID_ );

    if( pSkill ) {
        std::pair<EGCCharType, char> prKey = std::make_pair( ( EGCCharType )GetCurUICharType(), pSkill->GetPromotion() );
        std::map< std::pair<EGCCharType, char>, SkillSet >::iterator mit = m_mapTutorialTrainedSkill.find( prKey );
        if( mit != m_mapTutorialTrainedSkill.end() ) {
            mit->second.erase( eSkillID_ );
        }
    }
}

void KGCSkillTree::SaveTrainSkill( EGCSkillTree eSkillID_ )
{
    std::set< EGCSkillTree >::iterator sit = m_setSaveRemoveSkill.find( eSkillID_ );

    if( sit != m_setSaveRemoveSkill.end() ) {
        m_setSaveRemoveSkill.erase( *sit );
        return;
    }
    
    m_setSaveTrainSkill.insert( eSkillID_ );
}

void KGCSkillTree::SaveRemoveSkill( EGCSkillTree eSkillID_ )
{
    std::set< EGCSkillTree >::iterator sit = m_setSaveTrainSkill.find( eSkillID_ );

    if( sit != m_setSaveTrainSkill.end() ) {
        m_setSaveTrainSkill.erase( *sit );
        return;
    }

    m_setSaveRemoveSkill.insert( eSkillID_ );
}

void KGCSkillTree::ClearTrainRemoveSkill()
{
    m_setSaveTrainSkill.clear();
    m_setSaveRemoveSkill.clear();
}