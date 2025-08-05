#include "StdAfx.h"
#include "KGCObjectEgo.h"
#include "GCObjectAndMotion.h"

ImplementSingleton( KGCObjectEgoMgr )

KGCObjectEgoMgr::KGCObjectEgoMgr()
{

}

KGCObjectEgoMgr::~KGCObjectEgoMgr()
{

}

void KGCObjectEgoMgr::LoadFromLuaScript( char* strFilename_ )
{
    KLuaManager kLuaMgr;
    if ( !GCFUNC::LoadLuaScript( kLuaMgr, "Enum.stg" ) )
    {
        return;
    }
    if ( !GCFUNC::LoadLuaScript( kLuaMgr, "DamageEnum.stg" ) )
    {
        return;
    }
    if ( !GCFUNC::LoadLuaScript( kLuaMgr, "MotionEnum.stg" ) )
    {
        return;
    }
	if ( !GCFUNC::LoadLuaScript( kLuaMgr, strFilename_ ) )
	{
		return;
	}
	kLuaMgr.BeginTable("EgoItem_Data");

	for ( int i = 1 ; kLuaMgr.BeginTable( i ) == S_OK; ++i )
	{
		std::stack<DWORD> stkGoodsID;
        if ( kLuaMgr.BeginTable( "GoodsID") == S_OK )
        {
            DWORD goodID;
            for( int iIndex = 1; kLuaMgr.GetValue( iIndex, goodID ) == S_OK; ++iIndex )
            {
                stkGoodsID.push( goodID );
            }
            kLuaMgr.EndTable();
        }

        std::list<KGCObjectEgo*> listObjEgo;
        if( kLuaMgr.BeginTable( "Object" ) == S_OK ) {

            for( int j = 1 ; kLuaMgr.BeginTable( j ) == S_OK ; ++j ) {
                KGCObjectEgo* pEgo = new KGCObjectEgo();

                pEgo->Load( &kLuaMgr, (i+j) );

                listObjEgo.push_back( pEgo );

                kLuaMgr.EndTable();

            }

            kLuaMgr.EndTable();
        }

		while( !stkGoodsID.empty() )
		{
			m_mapObjectEgo[stkGoodsID.top()] = listObjEgo;
			stkGoodsID.pop();
		}
		kLuaMgr.EndTable();
	}
	kLuaMgr.EndTable();
}

std::list<KGCObjectEgo*> KGCObjectEgoMgr::GetObjectEgo( DWORD dwGoodsID )
{
	if ( m_mapObjectEgo.find(dwGoodsID) == m_mapObjectEgo.end() )
	{
		return std::list<KGCObjectEgo*>();
	}
	return m_mapObjectEgo[dwGoodsID];

}

void KGCObjectEgoMgr::ClearObjectStateInfo()
{
    m_mapObjectStateInfo.clear();
}

void KGCObjectEgoMgr::AddStateInfo( const std::string& strUniqueID, std::string strState )
{
    std::map< std::string, std::string >::iterator mit = m_mapObjectStateInfo.find( strUniqueID );
    if( mit == m_mapObjectStateInfo.end() )
    {
        m_mapObjectStateInfo.insert( std::make_pair(strUniqueID, strState) );
        return;
    }
    mit->second = strState;
}

bool KGCObjectEgoMgr::IsEgoItem( GCITEMID itemID )
{
    std::map< DWORD, std::list<KGCObjectEgo*> >::iterator mapIter = m_mapObjectEgo.find( itemID );
    if( mapIter == m_mapObjectEgo.end() )
        return false;
    return true;
}

const std::string KGCObjectEgoMgr::GetStateInfo( const std::string& strUniqueID )
{
    std::map< std::string, std::string >::iterator mit = m_mapObjectStateInfo.find( strUniqueID );
    if( mit == m_mapObjectStateInfo.end() ) {
        return std::string("");
    }
    return mit->second;
}

void KGCObjectEgo::Load( KLuaManager* pLuaMgr_, DWORD dwUniqueID )
{
    if ( pLuaMgr_->BeginTable( "Model") == S_OK )
    {
        for( int iIndex = 1; pLuaMgr_->BeginTable( iIndex ) == S_OK; ++iIndex )
        {
            KGCObjectEgoModel model;

            pLuaMgr_->GetValue( 1, model.m_strP3M );
            pLuaMgr_->GetValue( 2, model.m_strTex );
            pLuaMgr_->GetValue( 3, model.m_iBoneIdx );
            model.m_dwUniqueID = iIndex;

            m_vecModels.push_back( model );

            pLuaMgr_->EndTable();
        }
        pLuaMgr_->EndTable();
    }
	if ( pLuaMgr_->BeginTable( "State") == S_OK )
	{
		for( int iIndex = 1; pLuaMgr_->BeginTable( iIndex ) == S_OK; ++iIndex )
		{
			KGCObjectEgoState state;
			pLuaMgr_->GetValue( "ID", state.m_strID );
			pLuaMgr_->GetValue( "REPEAT", state.m_iRepeat );
			pLuaMgr_->GetValue( "AFTER", state.m_strAfterState );
            pLuaMgr_->GetValue( "MOTION", state.m_strMotion );
            pLuaMgr_->GetValue( "SOUND", state.m_iSoundID );
            pLuaMgr_->GetValue( "DAMAGE", state.m_iDamageID );

			m_vecState.push_back( state );

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
    }
    if ( pLuaMgr_->BeginTable( "Condition") == S_OK )
    {
        for( int iIndex = 1; pLuaMgr_->BeginTable( iIndex ) == S_OK; ++iIndex )
        {
            KGCObjectEgoCondition condition;
            pLuaMgr_->GetValue( 1, condition.m_strPossibleID );
            pLuaMgr_->GetValue( 2, condition.m_iConditionID );
            pLuaMgr_->GetValue( 3, condition.m_iLparam );
            pLuaMgr_->GetValue( 4, condition.m_iWparam );
            pLuaMgr_->GetValue( 5, condition.m_strMoveID );
            condition.m_dwUniqueID =( dwUniqueID + iIndex);

            m_vecCondition.push_back( condition );

            pLuaMgr_->EndTable();
        }
        pLuaMgr_->EndTable();
    }
    pLuaMgr_->GetValue( "DefaultState", m_strDefaultStateID );
}

const KGCObjectEgoState* KGCObjectEgo::GetEgoStateFromID( std::string strID )
{
    BOOST_FOREACH( const KGCObjectEgoState& state, m_vecState ) {
        if( state.m_strID == strID )
        {
            return &state;
        }
    }
    return NULL;
}

const KGCObjectEgoState* KGCObjectEgo::GetEgoState( std::string strCurState, PLAYER *pPlayer, USHORT ucFrame, OUT int& iConditionIdx )
{
    if( pPlayer )
    {
        int iIndex=0;
        BOOST_FOREACH( KGCObjectEgoCondition& condition, m_vecCondition ) {
            if( CheckEgoCondition( condition, strCurState, pPlayer, ucFrame ) )
            {
                BOOST_FOREACH( const KGCObjectEgoState& state, m_vecState ) {
                    if( state.m_strID == condition.m_strMoveID )
                    {
                        iConditionIdx = iIndex;
                        return &state;
                        }
                }
            }
            iIndex++;
        }
    }
	return NULL;
}

const KGCObjectEgoState* KGCObjectEgo::GetEgoStateNotCheckCondition( int iConditionIdx )
{
    if( 0 <= iConditionIdx && iConditionIdx < int(m_vecCondition.size()) )
    {
        BOOST_FOREACH( const KGCObjectEgoState& state, m_vecState ) {
            if( state.m_strID == m_vecCondition[ iConditionIdx ].m_strMoveID )
                return &state;
        }
    }
    return NULL;
}

bool KGCObjectEgo::CheckEgoCondition( KGCObjectEgoCondition& condition, std::string strCurState, PLAYER *pPlayer, USHORT ucFrame )
{
    if( pPlayer && 
        (strCurState == condition.m_strPossibleID) )
    {
        switch( condition.m_iConditionID )
        {
        case E_MOTION_START:
            if( (condition.m_iLparam == pPlayer->uiMotion) && (condition.m_iWparam == ucFrame) )
                return true;
            break;
        case E_COMMON_MOTION:
            if( (pPlayer->GetCurFormTemplate().GetMotionIDByEnum(condition.m_iLparam) == pPlayer->uiMotion) && (condition.m_iWparam == ucFrame) )
                return true;
            break;
        case E_GAME_RANDOM:
            if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME  ) {
                if( ( (rand()%100) < condition.m_iLparam) && (condition.m_iWparam == ucFrame) )
                    return true;
            }
            break;
        case E_SHOWROOM_RANDOM:
            if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME  ) {
                if( ( (rand()%100) < condition.m_iLparam) && (condition.m_iWparam == ucFrame) )
                    return true;
            }
            break;
        }
    }

	return false;
}

void KGCObjectEgo::AddMesh( GCObject* pObj, GCITEMID gcItemID, int iPlayerIdx, std::vector< KGCEgoItemInfo >* pmapObj )
{
    GCITEMID gcTextureID = gcItemID;

    if( !pObj )
        return;

    if( m_vecModels.empty() )
        return;

    const KGCObjectEgoState *state = GetEgoStateFromID( m_strDefaultStateID );
    if( NULL == state )
    {
        ASSERT( !"왜!! 기본상태가 없어! 스크립트 체크해보세요!");
        return;
    }

    int iBoneIdx = 0;
    int iChildIndex = 0;
    std::stringstream strModelName;
    std::stringstream strModelTex;
    BOOST_FOREACH( KGCObjectEgoModel model, m_vecModels ) {

        iBoneIdx = model.m_iBoneIdx;
        strModelName.str("");
        strModelTex.str("");

        if( "GoosIDBasicModel" == model.m_strP3M )
        {
            gcItemID = g_pItemMgr->GetSameImageIndex( gcItemID );
            gcTextureID = g_pItemMgr->GetSameTextureIndex( gcItemID );
            strModelName << "egoabta" << gcItemID << "_" << iBoneIdx << ".p3m";
            strModelTex << "egoabta" << gcTextureID << "_" << iBoneIdx << ".dds";
        }
        else
        {
            strModelName << model.m_strP3M;
            strModelTex << model.m_strTex;
        }

        KGCObjectAndMotion* pRet = (KGCObjectAndMotion*)g_pGCDeviceManager2->CreateLiveModel( strModelName.str(), strModelTex.str(), state->m_strMotion, iBoneIdx, pObj );
        if( NULL == pRet )
            continue;

        std::stringstream stm;
        stm << std::setw(2) << std::setfill('0') << iPlayerIdx << std::setw(6) << gcItemID << std::setw(2) << model.m_dwUniqueID;
        SiKGCObjectEgoMgr()->AddStateInfo( stm.str(), m_strDefaultStateID );

        pRet->m_strName = stm.str();
        pRet->dwGoodsID = gcItemID;

        KGCEgoItemInfo egoinfo;
        egoinfo.m_bCheckCondition = false;
        egoinfo.m_iConditionID = -1;
        egoinfo.m_pEgoObj = this;
        egoinfo.m_strUniqueID = stm.str();
        egoinfo.m_dwUModelniqueID = atoi( stm.str().c_str() );

        if( pmapObj )
            pmapObj->push_back( egoinfo );
    }
}