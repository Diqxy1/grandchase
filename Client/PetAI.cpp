#include "stdafx.h"
// PetAI.cpp: implementation of the CPetAI class.
//
//////////////////////////////////////////////////////////////////////

//
//
//
#include "MyD3D.h"


#include "PetAI.h"
#include "DamageManager.h"
//
#include "Monster.h"
#include "Stage.h"
#include "Square/GCSquare.h"
#include "Square/GCSquarePeople.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPetAI* g_pkPetAI = NULL;

CPetAI::CPetAI()
{
	g_pkPetAI = this;
	m_fElapsedTime = 0.0f;
    m_fNowSendTime = 0.0f;
}

CPetAI::~CPetAI()
{
	Free();
}

void CPetAI::Free()
{
    for( int i = 0; i < (int)m_AISeqList.size(); i++ )
	{
		m_AISeqList[i]->magicSeqList.clear();
		m_AISeqList[i]->effectSeqList.clear();
		SAFE_DELETE( m_AISeqList[i] );
	}
	for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
	{
		DeleteCriticalSection( &m_AIObjectList[i]->criticalSection );
		SAFE_DELETE( m_AIObjectList[i] );
	}
	m_AISeqList.clear();
	m_AIObjectList.clear();
	m_ItemList.clear();
}

void CPetAI::LoadPetItemFromLuaScript( const char* strStringFile, const char* fileName )
{
    int         num;
    int         i;
    char        key[30] = {0,};
    std::string temp;
	std::string strParticleScript;
    
    
	KLuaManager m_kLuaMgr;	

    if( GCFUNC::LoadLuaScript( m_kLuaMgr , strStringFile ) == false )
		return;

    if( GCFUNC::LoadLuaScript( m_kLuaMgr , fileName ) == false )
		return;

	m_kLuaMgr.BeginTable( "PET_AI" );
    {
        m_kLuaMgr.GetValue( "NUM", num );
        for( i = 0; i < num; i++ )
        {
            sprintf( key, "AI_NAME%d", i );
			m_kLuaMgr.GetValue( key, temp );
			strParticleScript = temp + ".stg";
            LoadTempletFromLuaScript( (char*)strParticleScript.c_str(), temp.c_str() );
        }
    }
    m_kLuaMgr.EndTable();

	bool bItemID;

    m_kLuaMgr.BeginTable( "PET_ITEM" );
    {
        m_kLuaMgr.GetValue( "NUM", num );
        for( i = 0; i < num; i++ )
        {
            ItemPack	itemPack;
            sprintf( key, "PET_ITEM%d", i );
            m_kLuaMgr.BeginTable( key );
            {
				m_kLuaMgr.GetValue( "ITEM_ID",     bItemID );

				if( bItemID == true )
				{
					std::string strItemID;
					m_kLuaMgr.GetValue( "ITEM_NAME",   strItemID );
					itemPack.itemName = GCUTIL_STR::GCStrCharToWide( strItemID.c_str() );
				}
				else
				{
					int iStrID;
					m_kLuaMgr.GetValue( "ITEM_NAME",   iStrID );
					itemPack.itemName = g_pkStrLoader->GetString( iStrID );
				}
                
                m_kLuaMgr.GetValue( "PET_NAME",    itemPack.petName );
            }
            m_kLuaMgr.EndTable();
			m_ItemList.push_back( itemPack );
        }
    }
    m_kLuaMgr.EndTable();
}

std::vector< int > CPetAI::CreatePetinItem( const WCHAR* itemName, int age, int master, bool bMonsterMaster , int iItemID)
{
    std::vector< int > vecRet;
    int iSameIndexItemID = g_pItemMgr->GetSameImageIndex( iItemID );
    
    char str[15] = "";
    std::string strSameIndex = itoa(iSameIndexItemID,str,10);

    //String -> WCHAR
    LPWSTR lpszw = new WCHAR[MAX_PATH];
    int nLen = MultiByteToWideChar(CP_ACP, 0, strSameIndex.c_str(), -1 , NULL, NULL );
    MultiByteToWideChar(CP_ACP, 0, strSameIndex.c_str(), -1, lpszw, nLen );


	for( int i = 0; i < (int)m_ItemList.size(); i++ )
	{
        if( m_ItemList[i].itemName.compare(itemName) == 0 )
		{
			AIObject* pAI = AddAIObject( (int)m_AIObjectList.size(), m_ItemList[i].petName.c_str(), master, bMonsterMaster );
            vecRet.push_back( pAI->ID );
		}
        else if ( m_ItemList[i].itemName.compare( lpszw ) == 0 )
        {
            AIObject* pAI = AddAIObject( (int)m_AIObjectList.size(), m_ItemList[i].petName.c_str(), master, bMonsterMaster );
            vecRet.push_back( pAI->ID );
        }
	}

    return vecRet;
}

void CPetAI::LoadTempletFromLuaScript( const char* fileName, const char* templetName )
{
	AISequence*		aiSeq;
	MagicSequence	magicSeq;
	KLuaManager		m_kLuaMgr2;

    if( GCFUNC::LoadLuaScript( m_kLuaMgr2 , fileName ) == false )
        return;

	aiSeq = new AISequence;	

	int magicNum;
	int effectNum;
	char key[100];
	m_kLuaMgr2.BeginTable( "AI_SEQUENCE" );
    {
		aiSeq->templetName = templetName;

		//이동
		m_kLuaMgr2.GetValue( "ACTIVE",						aiSeq->bActive );
		
		m_kLuaMgr2.GetValue( "Y_GAP",						aiSeq->fYGap );
		m_kLuaMgr2.GetValue( "X_GAP",						aiSeq->fXGap );

        m_kLuaMgr2.GetValue( "FOLLOW_PLAYER_RIGHT",         aiSeq->bFollowPlayerRight );

		//m_kLuaMgr2.GetValue( "ACCEL_SPEED",					aiSeq->fAccel );
		//m_kLuaMgr2.GetValue( "MAX_SPEED",					aiSeq->fMaxSpeed );

		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_X_SCALE",					aiSeq->fMoveXScale		) ) )
			aiSeq->fMoveXScale = 0.4f;
		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_Y_SCALE",					aiSeq->fMoveYScale		) ) )
			aiSeq->fMoveYScale = 0.2f;
		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_SPEED",						aiSeq->fMoveSpeed		) ) )
			aiSeq->fMoveSpeed = 1.0f;		
		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_ROTATE",						aiSeq->fMoveRotate		) ) )
			aiSeq->fMoveRotate = 0.0f;
		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_ROTATE_SPEED",				aiSeq->fMoveRotateSpeed		) ) )
			aiSeq->fMoveRotateSpeed = 0.0f;		
		if( FAILED( m_kLuaMgr2.GetValue( "MOVE_TRACK_RATE",					aiSeq->fTrackRate		) ) )
			aiSeq->fTrackRate = 1.0f;					

		aiSeq->fMoveRotate = D3DXToRadian(aiSeq->fMoveRotate);
		

		m_kLuaMgr2.GetValue( "SIZE_X",						aiSeq->fSizeX );
		m_kLuaMgr2.GetValue( "SIZE_Y",						aiSeq->fSizeY );

		m_kLuaMgr2.GetValue( "PM_FLY1",						aiSeq->motionAnimList[PM_FLY1] );
		m_kLuaMgr2.GetValue( "PM_FLY2",						aiSeq->motionAnimList[PM_FLY2] );
		m_kLuaMgr2.GetValue( "PM_FLY3",						aiSeq->motionAnimList[PM_FLY3] );
		m_kLuaMgr2.GetValue( "PM_FLY4",						aiSeq->motionAnimList[PM_FLY4] );
		m_kLuaMgr2.GetValue( "PM_MASTER_ATTACK",			aiSeq->motionAnimList[PM_MASTER_ATTACK] );
		m_kLuaMgr2.GetValue( "PM_MASTER_ATTACKED",			aiSeq->motionAnimList[PM_MASTER_ATTACKED] );
		m_kLuaMgr2.GetValue( "PM_MAGIC1",				 	aiSeq->motionAnimList[PM_MAGIC1] );
		m_kLuaMgr2.GetValue( "PM_MAGIC2",		       		aiSeq->motionAnimList[PM_MAGIC2] );
		m_kLuaMgr2.GetValue( "PM_MAGIC3",				 	aiSeq->motionAnimList[PM_MAGIC3] );
		m_kLuaMgr2.GetValue( "PM_MAGIC4",		       		aiSeq->motionAnimList[PM_MAGIC4] );
		m_kLuaMgr2.GetValue( "PM_MAGIC5",					aiSeq->motionAnimList[PM_MAGIC5] );

		//이펙트
		m_kLuaMgr2.GetValue( "EFFECT_NUM",					effectNum );

		for( int i = 0; i < effectNum; i++ )
		{
			EffectSequence	effectSeq;
			sprintf( key, "EFFECT_SEQUENCE%d", i );

			m_kLuaMgr2.BeginTable( key );
			{
				if( FAILED(m_kLuaMgr2.GetValue( "VERSION",	effectSeq.versionNum ) ))
					effectSeq.versionNum = 1;
				m_kLuaMgr2.GetValue( "EFF_NAME",			effectSeq.effectName );
				m_kLuaMgr2.GetValue( "TRACE",				effectSeq.trace );
				m_kLuaMgr2.GetValue( "EMIT_MIN",			effectSeq.emitRate.m_Min );
				m_kLuaMgr2.GetValue( "EMIT_MAX",			effectSeq.emitRate.m_Max );
			}
			m_kLuaMgr2.EndTable();

			aiSeq->effectSeqList.push_back( effectSeq );
		}

		//마법
		m_kLuaMgr2.GetValue( "MAGIC_NUM",					magicNum );

		for( int i = 0; i < magicNum; i++ )
		{
			sprintf( key, "MAGIC_SEQUENCE%d", i );

			m_kLuaMgr2.BeginTable( key );
			{
				m_kLuaMgr2.GetValue( "MAGIC_NAME",			magicSeq.magicName );
				m_kLuaMgr2.GetValue( "MAGIC_RATE",			magicSeq.magicRate );
				m_kLuaMgr2.GetValue( "MAGIC_DELAY_MIN",		magicSeq.magicDelay.m_Min );
				m_kLuaMgr2.GetValue( "MAGIC_DELAY_MAX",		magicSeq.magicDelay.m_Max );
			}
			m_kLuaMgr2.EndTable();

			aiSeq->magicSeqList.push_back( magicSeq );
		}
	}
	m_kLuaMgr2.EndTable();

	m_AISeqList.push_back( aiSeq );
}

CPetAI::AIObject* CPetAI::AddAIObject( int ID, const char* templetName, int masterNum, bool bMonsterMaster )
{
	AIObject* ai = new AIObject();
	//ZeroMemory( ai, sizeof(AIObject) );
	ai->fMagicDelay = 5.0f;
	ai->bEnable = true;
    ai->bShow = false;

	ai->ID = ID;

    for( int i = 0; i < (int)m_AISeqList.size(); i++ )
    {
        if( m_AISeqList[i]->templetName.compare( templetName ) == 0 )
        {
            ai->AI = m_AISeqList[i];
            break;
        }
    }
	
    if( IsUsePlayer() )
    {
        if( masterNum >= 0 && masterNum < MAX_PLAYER_NUM )
        {
            ai->m_pMasterInfo = new KPetAIMasterInfo( g_MyD3D->MyPlayer[masterNum] );
        }
    }
    else
    {
        GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( masterNum );
        if( pPeople != NULL )
        {
            ai->m_pMasterInfo = new KPetAIMasterInfo( pPeople );
        }
        else
        {
            // 모든걸 로딩타이밍에 생성하다 보니 로딩 예약되어있는 녀석을 참조해야할 도 있다..
            GCSquarePeople* pReservePeople = SiGCSquare()->GetSquareReservePeople( masterNum );
            if( pReservePeople != NULL )
            {
                ai->m_pMasterInfo = new KPetAIMasterInfo( pReservePeople );
            }
        }
    }
	
	ai->iMaster			= masterNum;
	ai->bMonsterMaster	= bMonsterMaster;
	if( ai->bMonsterMaster == false )
	{
        if( ai->m_pMasterInfo != NULL )
        {
            ai->pos.x	= ai->m_pMasterInfo->vPos->x - 1.0f;
            ai->pos.y	= ai->m_pMasterInfo->vPos->y;
            ai->pos.z	= 0.8f;
        }
    }
	else
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster( masterNum);
		if ( pMonster ) {
			ai->pos.x	= pMonster->m_afX[0] - 1.0f;
			ai->pos.y	= pMonster->m_afY[0];
			ai->pos.z	= 0.8f;
		}
	}

	InitializeCriticalSection( &ai->criticalSection );

	if( ai->AI->bActive == true )
	{
		for( int i = 0; i < PM_END; i++ )
		{
			ai->motionAnimList[i] = g_MyD3D->m_pPetAnim->CreateInst( g_MyD3D->m_pPetAnim->GetSequence(ai->AI->motionAnimList[i]) );
			if( !ai->motionAnimList[i] )
			{
				delete ai;
				return NULL;
			}
			ai->motionAnimList[i]->Stop();
			ai->motionAnimList[i]->SetShow( false );
			ai->motionAnimList[i]->GetMatrix()->Scale( ai->AI->fSizeX, ai->AI->fSizeY, 1.0f );
		}
	}
	else
	{
		for( int i = 0; i < PM_MASTER_ATTACK; i++ )
		{
			ai->motionAnimList[i] = g_MyD3D->m_pPetAnim->CreateInst( g_MyD3D->m_pPetAnim->GetSequence(ai->AI->motionAnimList[i]) );
			if( !ai->motionAnimList[i] )
			{
				delete ai;
				return NULL;
			}
			ai->motionAnimList[i]->Stop();
			ai->motionAnimList[i]->SetShow( false );
			ai->motionAnimList[i]->GetMatrix()->Scale( ai->AI->fSizeX, ai->AI->fSizeY, 1.0f );
		}
	}

	ai->effSeqList2.clear();
	CParticleEventSeqPTR effseq2;
	for( int i = 0; i < (int)ai->AI->effectSeqList.size(); i++ )
	{
		effseq2 = g_ParticleManager->CreateSequence( (char*)ai->AI->effectSeqList[i].effectName.c_str(), 0,0,0 );
		effseq2->SetEmitRate(CMinMax<float>(ai->AI->effectSeqList[i].emitRate.m_Max, ai->AI->effectSeqList[i].emitRate.m_Min));

		if( ai->AI->effectSeqList[i].trace == true )
		{
			TRACE_INFO stTrace;
			stTrace.m_pvPos = &ai->pos;
            if (effseq2)
			    effseq2->SetTrace( &stTrace );
		}

		if( effseq2->GetName() == "BlackHoleJewel" )
		{
			effseq2->SetBlackHolePosition( &ai->pos );
		}

		ai->effSeqList2.push_back( effseq2 );
	}

	m_AIObjectList.push_back( ai );
	return ai;
}

void CPetAI::ClearAIObject()
{
    for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
	{
		DeleteCriticalSection( &m_AIObjectList[i]->criticalSection );
		m_AIObjectList[i]->effSeqList2.clear();
		SAFE_DELETE( m_AIObjectList[i] );
	}
	m_AIObjectList.clear();
}

void CPetAI::ClearAIObject( AIObject* aiObject )
{
	for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
	{
		if( m_AIObjectList[i] == aiObject )
		{
			DeleteCriticalSection( &m_AIObjectList[i]->criticalSection );
			m_AIObjectList[i]->effSeqList2.clear();
			SAFE_DELETE( m_AIObjectList[i] );
            m_AIObjectList.erase( m_AIObjectList.begin() + i );
            break;
		}
	}
}

CPetAI::AIObject* CPetAI::GetAIObject( int ID )
{
    for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
    {
        if( m_AIObjectList[i]->ID == ID )
        {
            return m_AIObjectList[i];
        }
    }
    return NULL;
}

void CPetAI::FrameMove( float fElapsedTime )
{
    PROFILE_SET("CPetAI::FrameMove");

    m_fElapsedTime = fElapsedTime;
    int index = 0;
	bool sendPacket = false;

	if( g_kGlobalValue.m_kUserInfo.bHost == true )
	{
		ZeroMemory( &m_MovePacketList, sizeof(LTP_PET_AI_MOVE_PACK) );
	
	    m_MovePacketList.packetType   = ELTP_PET_AI_MOVE_PACK;
		m_MovePacketList.size         = sizeof(LTP_PET_AI_MOVE_PACK);
		m_MovePacketList.latencyTime  = GC_ELAPSED_TIME;
	}

	for( std::vector<AIObject*>::iterator i = m_AIObjectList.begin(); i != m_AIObjectList.end(); i++) 
	{
        if( (*i)->bShow == false )
            continue;

        UpdateMasterInfo( (*i) );

		if( (*i)->AI->bActive == false )
		{
			if( (*i)->bMonsterMaster == false )
			{
				if( IsUsePlayer() && ( g_MyD3D->MyPlayer[(*i)->iMaster]->m_cLife <= 0 || g_MyD3D->MyPlayer[(*i)->iMaster]->m_kUserInfo.bLive == false ) )
				{
					(*i)->bDie = true;
				}
				else
				{
					(*i)->bDie = false;
				}
			}
			else
			{
				MONSTER* pMonster = g_kMonsterManager.GetMonster( (*i)->iMaster );
				if ( pMonster ) {
					if( pMonster->m_bLive == false )
					{
						(*i)->bDie = true;
					}
					else
					{
						(*i)->bDie = false;
					}
					if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon1" ) == 0
						|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon2" ) == 0 
						|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon3" ) == 0 )
					{
						if( g_MyD3D->MyStg->GetStage() != GC_GS_QUEST6_E )
							(*i)->bDie = true;
					}
				}
			}
			
			Update( (*i) );
		}
		else
		{
			EnterCriticalSection( &(*i)->criticalSection );
			if( g_kGlobalValue.m_kUserInfo.bHost == true )
			{
				if( (*i)->bMonsterMaster == false )
				{
					if( IsUsePlayer() && ( g_MyD3D->MyPlayer[(*i)->iMaster]->m_cLife <= 0 || g_MyD3D->MyPlayer[(*i)->iMaster]->m_kUserInfo.bLive == false ) )
					{
						(*i)->bDie = true;
					}
					else
					{
						(*i)->bDie = false;
					}
				}
				else
				{
					MONSTER* pMonster = g_kMonsterManager.GetMonster( (*i)->iMaster );
					if ( pMonster ) {

						if( pMonster->m_bLive == false )
						{
							(*i)->bDie = true;
						}
						else
						{
							(*i)->bDie = false;
						}
						if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon3" ) == 0
							&& pMonster->m_fHP < 0.85f )
						{
							(*i)->bDie = true;
						}
						if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon2" ) == 0
							&& pMonster->m_fHP < 0.7f )
						{
							(*i)->bDie = true;
						}
						if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon1" ) == 0
							&& pMonster->m_fHP < 0.55f )
						{
							(*i)->bDie = true;
						}
						if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon1" ) == 0
							|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon2" ) == 0 
							|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon3" ) == 0 )
						{
							if( g_MyD3D->MyStg->GetStage() != GC_GS_QUEST6_E )
								(*i)->bDie = true;
						}
					}
				}
				
				Magic( (*i) );
				sendPacket = true;
			}
			MONSTER* pMonster = g_kMonsterManager.GetMonster( (*i)->iMaster );
			if ( pMonster ) {
				if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon3" ) == 0
					&& pMonster->m_fHP < 0.85f )
				{
					(*i)->bDie = true;
				}
				if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon2" ) == 0
					&& pMonster->m_fHP < 0.7f )
				{
					(*i)->bDie = true;
				}
				if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon1" ) == 0
					&& pMonster->m_fHP < 0.55f )
				{
					(*i)->bDie = true;
				}
			}
			if( (*i)->AI->templetName.compare( "PetAIKazeazeArchon1" ) == 0
				|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon2" ) == 0 
				|| (*i)->AI->templetName.compare( "PetAIKazeazeArchon3" ) == 0 )
			{
				if( g_MyD3D->MyStg->GetStage() != GC_GS_QUEST6_E )
					(*i)->bDie = true;
			}
			Update( (*i) );
			LeaveCriticalSection( &(*i)->criticalSection );
		}        
	}

    if( g_kGlobalValue.m_kUserInfo.bHost == true && sendPacket == true )
    {
        m_fNowSendTime += m_fElapsedTime;
        if( m_fNowSendTime >= AI_SEND_GAP )
        {
            m_MovePacketList.size = sizeof(LTP_PET_AI_MOVE)*m_MovePacketList.ucActiveNum + sizeof(LTP_BASIC) + sizeof(UCHAR);
            g_LatencyTransfer.PushPacket( &m_MovePacketList );
            m_fNowSendTime = 0.0f;
        }
    }
}

void CPetAI::Magic( AIObject* aiObject )
{
    if( aiObject->motion == PM_MAGIC1
        || aiObject->motion == PM_MAGIC2
        || aiObject->motion == PM_MAGIC3
        || aiObject->motion == PM_MAGIC4
		|| aiObject->motion == PM_MAGIC5 )
    {
        return;
    }

	if( aiObject->bDie == true || aiObject->bEnable == false )
		return;
	
	if( aiObject->fMagicDelay > 0.0f )
		return;
    
    float randTemp = (float)(rand()%100);
    float magicRate = 0.0f;

    for( int i = 0; i < (int)aiObject->AI->magicSeqList.size(); i++ )       //공격 시퀀스를 돌면서
    {
        magicRate += aiObject->AI->magicSeqList[i].magicRate;
        if( magicRate > randTemp )                                    //특정 공격확률이 선택되면
        {
			m_fNowSendTime = AI_SEND_GAP;
            switch( i )
            {
                case 0:
					aiObject->motion = PM_MAGIC1;
					aiObject->fMagicDelay = aiObject->AI->magicSeqList[i].magicDelay.GetRandomNumInRange();
                    aiObject->bMagicStartThisFrame = true;
					return;

                case 1:
					aiObject->motion = PM_MAGIC2;
					aiObject->fMagicDelay = aiObject->AI->magicSeqList[i].magicDelay.GetRandomNumInRange();
                    aiObject->bMagicStartThisFrame = true;
					return;

                case 2:
					aiObject->motion = PM_MAGIC3;
					aiObject->fMagicDelay = aiObject->AI->magicSeqList[i].magicDelay.GetRandomNumInRange();
                    aiObject->bMagicStartThisFrame = true;
					return;

                case 3:
					aiObject->motion = PM_MAGIC4;
					aiObject->fMagicDelay = aiObject->AI->magicSeqList[i].magicDelay.GetRandomNumInRange();
                    aiObject->bMagicStartThisFrame = true;
					return;

                case 4:
					aiObject->motion = PM_MAGIC5;
					aiObject->fMagicDelay = aiObject->AI->magicSeqList[i].magicDelay.GetRandomNumInRange();
                    aiObject->bMagicStartThisFrame = true;
					return;
            }
        }
    }    
}

void CPetAI::ShowUpdate( AIObject* aiObject )
{
    int i = 0;

    if( aiObject->bShow == false || aiObject->bDie == true )
    {
        for( i = 0; i < PM_END; i++ )
        {
            if( aiObject->motionAnimList[i] != NULL )
            {
                aiObject->motionAnimList[i]->Stop();
                aiObject->motionAnimList[aiObject->motion]->SetShow( false );
            }
        }
        for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
        {
            if( aiObject->effSeqList2[i] != NULL )
            {
                aiObject->effSeqList2[i]->SetEmitRate( CMinMax<float>(0,0) );
            }
        }
        return;
    }
    else if( aiObject->bShow == true )
    {
        for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
        {
            if( aiObject->effSeqList2[i] != NULL )
            {
                aiObject->effSeqList2[i]->SetEmitRate(CMinMax<float>(aiObject->AI->effectSeqList[i].emitRate.m_Max, aiObject->AI->effectSeqList[i].emitRate.m_Min));
            }
        }
    }
}

void CPetAI::Update( AIObject* aiObject )
{
	int randTemp = 0;
	int i = 0;

    ShowUpdate( aiObject );

    if( aiObject->bMagicStartThisFrame == true )
	{
		for( i = 0; i < PM_END; i++ )
		{
			if( aiObject->motionAnimList[i] != NULL )
			{
				aiObject->motionAnimList[i]->Stop();
				aiObject->motionAnimList[aiObject->motion]->SetShow( false );
			}
		}
		aiObject->motionAnimList[aiObject->motion]->Start( CKTDGAnim::PT_ONE_PASS );
		aiObject->motionAnimList[aiObject->motion]->SetShow( true );
		StartUpMagic( aiObject, aiObject->motion - PM_MAGIC1 );
	}

	aiObject->fMagicDelay -= m_fElapsedTime;
	if( aiObject->fMagicDelay < 0.0f )
		aiObject->fMagicDelay = 0.0f;

    //애니메이션을 돌리고 상태를 바꾼다.
	if( aiObject->motionAnimList[aiObject->motion]->GetState() == CKTDGAnim::AS_STOP )
	{
		switch( aiObject->motion )
		{
			case PM_FLY1:
			case PM_FLY2:
			case PM_FLY3:
			case PM_FLY4:
			case PM_MASTER_ATTACK:
			case PM_MASTER_ATTACKED:
				for( i = 0; i < PM_END; i++ )
				{
					if( aiObject->motionAnimList[i] != NULL )
					{
						aiObject->motionAnimList[i]->Stop();
						aiObject->motionAnimList[aiObject->motion]->SetShow( false );
					}
				}
				randTemp = rand()%10;
				if( randTemp < 7 )
				{
					aiObject->motion = PM_FLY1;
					aiObject->motionAnimList[PM_FLY1]->Start( CKTDGAnim::PT_ONE_PASS );
					aiObject->motionAnimList[PM_FLY1]->SetShow( true );
				}
				else if( randTemp == 7 )
				{
					aiObject->motion = PM_FLY2;
					aiObject->motionAnimList[PM_FLY2]->Start( CKTDGAnim::PT_ONE_PASS );
					aiObject->motionAnimList[PM_FLY2]->SetShow( true );
				}
				else if( randTemp == 8 )
				{
					aiObject->motion = PM_FLY3;
					aiObject->motionAnimList[PM_FLY3]->Start( CKTDGAnim::PT_ONE_PASS );
					aiObject->motionAnimList[PM_FLY3]->SetShow( true );
				}
				else if( randTemp == 9 )
				{
					aiObject->motion = PM_FLY4;
					aiObject->motionAnimList[PM_FLY4]->Start( CKTDGAnim::PT_ONE_PASS );
					aiObject->motionAnimList[PM_FLY4]->SetShow( true );
				}
				break;

			case PM_MAGIC1:                           //공격 상태이면서
			case PM_MAGIC2:                           //공격 상태이면서
			case PM_MAGIC3:                           //공격 상태이면서
			case PM_MAGIC4:                           //공격 상태이면서
			case PM_MAGIC5:                           //공격 상태이면서
				for( i = 0; i < PM_END; i++ )
				{
					if( aiObject->motionAnimList[i] != NULL )
					{
						aiObject->motionAnimList[i]->Stop();
						aiObject->motionAnimList[aiObject->motion]->SetShow( false );
					}
				}
				aiObject->motion = PM_FLY1;
				aiObject->motionAnimList[PM_FLY1]->Start( CKTDGAnim::PT_ONE_PASS );
				aiObject->motionAnimList[PM_FLY1]->SetShow( true );				
				break;
		}
	}

    //상태에 맞춰 유닛을 움직인다.
    switch( aiObject->motion )
    {
		case PM_FLY1:
		case PM_FLY2:
		case PM_FLY3:
		case PM_FLY4:
		case PM_MASTER_ATTACK:
		case PM_MASTER_ATTACKED:
			{						

				aiObject->fNowMoveTime += m_fElapsedTime;
				// 움직임이.. 원을 그리면서 원을 그리는 걸로 되어있음..
				// 따라서 원 두개에 대한 옵션 설정이 필요함

				//Make Circle
				float fTempX = aiObject->AI->fMoveXScale * cos(aiObject->fNowMoveTime * aiObject->AI->fMoveSpeed);
				float fTempY = aiObject->AI->fMoveYScale * sin(aiObject->fNowMoveTime * aiObject->AI->fMoveSpeed);


				float fRotateAngle = aiObject->fNowMoveTime * aiObject->AI->fMoveRotateSpeed + aiObject->AI->fMoveRotate;

				float fNewX = cos(fRotateAngle)*fTempX - sin(fRotateAngle)*fTempY;
				float fNewY = cos(fRotateAngle)*fTempY + sin(fRotateAngle)*fTempX;
				D3DXVECTOR2 vTargetPos;
				D3DXVECTOR2 vTemp;

				if( aiObject->bMonsterMaster == false )
				{
					if( aiObject->m_pMasterInfo != NULL )
					{
						aiObject->bRight = aiObject->m_pMasterInfo->bIsRight;

                        if( aiObject->bRight )
                            vTargetPos.x = aiObject->m_pMasterInfo->m_posDesc->body.x + aiObject->AI->fXGap;
                        else
                            vTargetPos.x = aiObject->m_pMasterInfo->m_posDesc->body.x - aiObject->AI->fXGap;
                        vTargetPos.y = aiObject->m_pMasterInfo->m_posDesc->body.y + aiObject->AI->fYGap;

					}
				}
				else
				{					
					MONSTER* pMonster = g_kMonsterManager.GetMonster( aiObject->iMaster );
					if ( pMonster ) {
						aiObject->bRight = pMonster->m_abIsRight[0];
						vTargetPos.x = pMonster->m_afX[0] + aiObject->AI->fXGap - 1.0f;
						vTargetPos.y = pMonster->m_afY[0] + aiObject->AI->fYGap;
					}
				}
				vTemp = vTargetPos - aiObject->vCenterPos;
				float fLength = D3DXVec2Length( &vTemp );
				float fPercentLength = aiObject->AI->fTrackRate;

				//float fPercentLength = 0.05f;
				//if( fLength > 0.3f )
				//	fPercentLength = 0.2f;

				aiObject->vCenterPos = vTargetPos * fPercentLength + aiObject->vCenterPos * (1.0f - fPercentLength);

				aiObject->pos.x = aiObject->vCenterPos.x + fNewX;
				aiObject->pos.y = aiObject->vCenterPos.y + fNewY;
			}			
			break;

        case PM_MAGIC1:
        case PM_MAGIC2:
        case PM_MAGIC3:
        case PM_MAGIC4:
        case PM_MAGIC5:
            ProcessMagic( aiObject, (aiObject->motion - PM_MAGIC1) );
            break;
    }

	//주인이 투명일 때
	if( aiObject->bMonsterMaster == false )
	{
        if( IsUsePlayer() && g_MyD3D->MyPlayer[aiObject->iMaster]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] )
		{
			if(o3)
			{
				if( g_MyD3D->IsMyTeam( aiObject->iMaster ) || g_MyD3D->IsLocalPlayerObserver() )
				{
					if( aiObject->motionAnimList[aiObject->motion] != NULL )
					{
						aiObject->motionAnimList[aiObject->motion]->SetShow( true );
					}
					for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
					{
						if( aiObject->effSeqList2[i] != NULL )
						{
							aiObject->effSeqList2[i]->SetShow( true );
						}
					}
				}
				else
				{
					if( aiObject->motionAnimList[aiObject->motion] != NULL )
					{
						aiObject->motionAnimList[aiObject->motion]->SetShow( false );
					}
					for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
					{
						if( aiObject->effSeqList2[i] != NULL )
						{
							aiObject->effSeqList2[i]->SetShow( false );
						}
					}
				}
			}
			else
			{
				if( aiObject->motionAnimList[aiObject->motion] != NULL )
				{
					aiObject->motionAnimList[aiObject->motion]->SetShow( false );
				}
				for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
				{
					if( aiObject->effSeqList2[i] != NULL )
					{
						aiObject->effSeqList2[i]->SetShow( false );
					}
				}
			}
		}
		else
		{
			if( aiObject->motionAnimList[aiObject->motion] != NULL )
			{
				aiObject->motionAnimList[aiObject->motion]->SetShow( true );
			}
			for( i = 0; i < (int)aiObject->effSeqList2.size(); i++ )
			{
				if( aiObject->effSeqList2[i] != NULL )
				{
					aiObject->effSeqList2[i]->SetShow( true );
				}
			}
		}
	}

	aiObject->beforeFrame = aiObject->motionAnimList[aiObject->motion]->GetNowFrame();
	aiObject->motionAnimList[aiObject->motion]->GetMatrix()->Move( aiObject->pos.x, aiObject->pos.y, g_AnimManager.GetZPos() );
	if( aiObject->bRight == false )
		aiObject->motionAnimList[aiObject->motion]->GetMatrix()->Rotate( 0.0f, D3DXToRadian(180), 0.0f );
	else
		aiObject->motionAnimList[aiObject->motion]->GetMatrix()->Rotate( 0.0f, 0.0f, 0.0f );

	/*for( int k = 0; k < (int)aiObject->effSeqList2.size(); k++ )
	{
		if( aiObject->effSeqList2[k] != NULL )
		{
			aiObject->effSeqList2[k]->SetPosition( D3DXVECTOR3( aiObject->pos.x, aiObject->pos.y, 0.5f ) );
			aiObject->effSeqList2[k]->SetTrace( aiObject->AI->effectSeqList[k].trace );
			if( aiObject->effSeqList2[k]->GetName().compare( "BlackHoleJewel" ) == 0 )
				aiObject->effSeqList2[k]->SetBlackHolePosition( D3DXVECTOR3( aiObject->pos.x, aiObject->pos.y, 0.5f ) );
		}
	}*/
	
    if( g_kGlobalValue.m_kUserInfo.bHost == true && aiObject->AI->bActive == true )
    {
        //싱크를 맞추기 위해 패킷을 날린다.
        ZeroMemory( &m_MovePacket, sizeof(LTP_PET_AI_MOVE) );
        m_MovePacket.packetType				= ELTP_PET_AI_MOVE;
        m_MovePacket.size					= sizeof(LTP_PET_AI_MOVE);
        m_MovePacket.latencyTime			= GC_ELAPSED_TIME * 10.0f;

        m_MovePacket.sAI_ID					= aiObject->ID;
		m_MovePacket.bEnable				= aiObject->bEnable;
		m_MovePacket.bDie					= aiObject->bDie;
		m_MovePacket.vCenterPos				= aiObject->vCenterPos;		
		m_MovePacket.fTracking				= aiObject->fTracking;
		m_MovePacket.pos.x					= aiObject->pos.x;
        m_MovePacket.pos.y					= aiObject->pos.y;
		m_MovePacket.bMagicStartThisFrame	= aiObject->bMagicStartThisFrame;
		m_MovePacket.motion					= aiObject->motion;
		m_MovePacket.bRight					= aiObject->bRight;
        m_MovePacket.bXArrived				= aiObject->bXArrived;
        m_MovePacket.bYArrived				= aiObject->bYArrived;
		m_MovePacket.fMagicDelay			= aiObject->fMagicDelay;

		assert(m_MovePacketList.ucActiveNum < MAX_PET_NUM );
		memcpy( &m_MovePacketList.movePacket[m_MovePacketList.ucActiveNum], &m_MovePacket, sizeof(LTP_PET_AI_MOVE) );
		m_MovePacketList.ucActiveNum++;
    }
	aiObject->bMagicStartThisFrame = false;
}


void CPetAI::StartUpMagic( AIObject* aiObject, int magicNum )
{
}

void CPetAI::ProcessMagic( AIObject* aiObject, int magicNum )
{
    if( IsUsePlayer() == false )
        return;

	if( aiObject->AI->magicSeqList[magicNum].magicName.compare( "BLUE_LIGHTNING_ATTACK" ) == 0 )
	{
		g_KDSound.Play( "Lightning" );

		float tempX;
		float tempY;
		bool tempRight;

		if( aiObject->bMonsterMaster == false )
		{
			tempX		= g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.x;
			tempY		= g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.y;
			tempRight	= g_MyD3D->MyPlayer[aiObject->iMaster]->bIsRight;

			g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.x	= aiObject->pos.x + 1.0f;
			g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.y   = aiObject->pos.y + 0.4f;
			g_MyD3D->MyPlayer[aiObject->iMaster]->bIsRight    = aiObject->bRight;

			g_MyD3D->m_pDamageManager->Add( DT_LIGHTNING_GENERATE1_BLUE, aiObject->iMaster, false, -1 );
			g_MyD3D->m_pDamageManager->Add( DT_LIGHTNING_BOLT_BLUE, aiObject->iMaster, false, -1 );

			g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.x	= tempX;
			g_MyD3D->MyPlayer[aiObject->iMaster]->vPos.y	= tempY;
			g_MyD3D->MyPlayer[aiObject->iMaster]->bIsRight	= tempRight;
		}
		else
		{
			MONSTER* pMonster = g_kMonsterManager.GetMonster( aiObject->iMaster );
			if ( pMonster ) {
				tempX		= pMonster->m_afX[0];
				tempY		= pMonster->m_afY[0];
				tempRight	= pMonster->m_abIsRight[0];

				g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x	= aiObject->pos.x + 1.0f;
				g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y	= aiObject->pos.y + 0.4f;
				g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight	= aiObject->bRight;
				g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->Body_Angle	= 0;

				g_MyD3D->m_pDamageManager->Add( DT_LIGHTNING_GENERATE1_BLUE, MAX_PLAYER_NUM, false, -1 );
				g_MyD3D->m_pDamageManager->Add( DT_LIGHTNING_BOLT_BLUE, MAX_PLAYER_NUM, false, -1 );

				pMonster->SetLatencyX( 0, tempX );
				pMonster->SetLatencyY( 0, tempY );
				pMonster->m_abIsRight[0]	= tempRight;
			}
		}
	}
}

void CPetAI::PacketMove( LTP_PET_AI_MOVE* movePacket )
{
    for( std::vector<AIObject*>::iterator i = m_AIObjectList.begin(); i != m_AIObjectList.end(); i++) 
	{
        if( (*i)->ID == movePacket->sAI_ID && (*i)->AI->bActive == true )
        {
			EnterCriticalSection( &(*i)->criticalSection );
			if( (*i)->motion != (PET_MOTION)movePacket->motion )
			{
				for( int j = 0; j < PM_END; j++ )
				{
					if( j == (int)movePacket->motion )
					{
						(*i)->motionAnimList[j]->SetShow( true );
					}
					else
					{
						(*i)->motionAnimList[j]->Stop();
						(*i)->motionAnimList[j]->SetShow( false );
					}
				}
			}
			
			(*i)->bDie					= movePacket->bDie;
			(*i)->vCenterPos			= movePacket->vCenterPos;			
			(*i)->fTracking				= movePacket->fTracking;
			(*i)->bMagicStartThisFrame	= movePacket->bMagicStartThisFrame;
			(*i)->motion				= (PET_MOTION)movePacket->motion;
			if( (*i)->motion >= PM_MAGIC1 )
            {
                (*i)->pos.x				= movePacket->pos.x;
                (*i)->pos.y				= movePacket->pos.y;
            }
			(*i)->bRight				= movePacket->bRight;
			(*i)->bXArrived				= movePacket->bXArrived;
			(*i)->bYArrived				= movePacket->bYArrived;
			(*i)->fMagicDelay			= movePacket->fMagicDelay;

			(*i)->motionAnimList[(*i)->motion]->GetMatrix()->Move( movePacket->pos.x, movePacket->pos.y, 0.5f );
			if( movePacket->bRight == false )
				(*i)->motionAnimList[(*i)->motion]->GetMatrix()->Rotate( 0.0f, D3DXToRadian(180), 0.0f );
			else
				(*i)->motionAnimList[(*i)->motion]->GetMatrix()->Rotate( 0.0f, 0.0f, 0.0f );

			/*for( int j = 0; j < (int)(*i)->effSeqList2.size(); j++ )
			{
				if( (*i)->effSeqList2[j] != NULL )
				{
					(*i)->effSeqList2[j]->SetPosition( D3DXVECTOR3( (*i)->pos.x, (*i)->pos.y, 0.5f ) );
					(*i)->effSeqList2[j]->SetTrace( (*i)->AI->effectSeqList[j].trace );
				}
			}*/
            
			LeaveCriticalSection( &(*i)->criticalSection );
            return;
        }
    }
}

void CPetAI::AllPetDisable()
{
	for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
		m_AIObjectList[i]->bEnable = false;
}

void CPetAI::AllPetEnable()
{
	for( int i = 0; i < (int)m_AIObjectList.size(); i++ )
		m_AIObjectList[i]->bEnable = true;
}

void CPetAI::ClearAIObject( int iMaster )
{
	// iPlayer가 마스터인 펫들은 다 지운다
	for( int i=(int)m_AIObjectList.size()-1 ; i>=0 ; --i )
	{
		if( m_AIObjectList[i]->iMaster == iMaster )
		{
			DeleteCriticalSection( &m_AIObjectList[i]->criticalSection );

			for( int j=0 ; j<(int)m_AIObjectList[i]->effSeqList2.size() ; ++j )
			{
				g_ParticleManager->DeleteSequence( m_AIObjectList[i]->effSeqList2[j] );
			}

			m_AIObjectList[i]->effSeqList2.clear();
			SAFE_DELETE( m_AIObjectList[i] );

			m_AIObjectList.erase( m_AIObjectList.begin() + i );
		}
	}
}

void CPetAI::Show( std::vector< int > vecID, bool bShow )
{
    std::vector< int >::iterator vit;
    for( vit = vecID.begin(); vit != vecID.end(); ++vit )
    {
        vector< AIObject* >::iterator vitAi = std::find_if( m_AIObjectList.begin(), m_AIObjectList.end(), 
            boost::bind( &AIObject::ID, _1 ) == (*vit) );

        if( vitAi != m_AIObjectList.end() )
        {
            (*vitAi)->bShow = bShow;
            ShowUpdate( (*vitAi) );
        }
    }
}

bool CPetAI::IsOldPet( DWORD dwItemID )
{
    std::wstringstream strStm;

    strStm << dwItemID;

    for( int i = 0; i < (int)m_ItemList.size(); i++ )
    {
        if( m_ItemList[i].itemName == strStm.str() )
        {
            return true;
        }
    }
    return false;
}

bool CPetAI::IsUsePlayer() const
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE || g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING )
    {
        return false;
    }

    return true;
}

KPetAIMasterInfo::KPetAIMasterInfo( PLAYER* pPlayer )
{
    Update( pPlayer );
}

KPetAIMasterInfo::KPetAIMasterInfo( GCSquarePeople* pPeople )
{
    Update( pPeople );
}

void KPetAIMasterInfo::Update( PLAYER* pPlayer )
{
    bIsRight = pPlayer->GetIsRight();
    m_posDesc = &pPlayer->m_posDesc;
    vPos = &pPlayer->vPos;
}

void KPetAIMasterInfo::Update( GCSquarePeople* pPeople )
{
    bIsRight = pPeople->GetIsRight();
    m_posDesc = &pPeople->GetposDesc();
    vPos = &pPeople->GetPos();
}

void CPetAI::UpdateMasterInfo( AIObject* pAI )
{
    if( pAI->m_pMasterInfo == NULL )
        return;

    if( IsUsePlayer() )
    {
        ASSERT( pAI->iMaster >= 0 && pAI->iMaster < MAX_PLAYER_NUM );
        pAI->m_pMasterInfo->Update( g_MyD3D->MyPlayer[pAI->iMaster] );
    }
    else
    {
        GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( pAI->iMaster );
        if( pPeople != NULL )
        {
            pAI->m_pMasterInfo->Update( pPeople );
        }
    }
}
