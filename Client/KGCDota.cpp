#include "stdafx.h"
#include "KGCDota.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"
#include "KStats.h"
#include "Message.h"

#include "KGCGameBoard.h"

#include "KGCRoomManager.h"
#include "KGCElvis.h"

#include "../gcui/GCGameOverlayUI.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCDota::KGCDota(void)
: m_iCanabanCoreDPointCool( 0 )
, m_iSerdinCoreDPointCool( 0 )
, m_iDefaultDPointCool( 0 )
, m_iPlayerGameStartCoolTime( -1 )
, m_iArenaCoolTime( -1 )
, m_iGameStartCoolTime( -1 )
, m_iSerdinCoreHp( 0 )
, m_iCanabanCoreHp( 0 )
, m_iEndGameCoolTime( -1 )
, m_bGameEnd( false )
{}
KGCDota::~KGCDota(void){}

HRESULT KGCDota::InitAtGame()
{
#if defined( NATION_KOREA )
    {
        m_fAccumulateTime = 0.0f;

#ifndef __PATH__
        {
            g_kStatsManager.InsertPage( L"GAME TIME" );
            g_kStatsManager.InsertValue( L"AccumulateTime", &m_fAccumulateTime );
        }
#endif
    }
#endif

    m_dwLastRebirthTime = 0;

    m_bRequestRebirth = false;
    SiKGCElvis()->SetRevivalCount(0);
    g_pGameBoard->InitPlayTime();

    // 카메라 위치 초기화
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayerIdx ) )
    {
        g_kCamera.SetDirectCameraPosition( g_MyD3D->MyPlayer[iPlayerIdx]->vPos.x, g_MyD3D->MyPlayer[iPlayerIdx]->vPos.y, KGCCameraOrtho::POS_MODE_DIRECT );
    }

    // 도타정보 얻어두기
    m_pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();

    // 쿨타임 초기화
    m_iCanabanCoreDPointCool = 0;
    m_iSerdinCoreDPointCool = 0;
    m_iDefaultDPointCool = 0;
    m_iPlayerGameStartCoolTime = m_pCurDotaInfo->iGameStartCoolTime;
    m_iGameStartCoolTime = 275;
    m_iEndGameCoolTime = -1;
    m_bGameEnd = false;

    // 도타코어 DPoint지급 메시지 처리 초기화
    m_bFirstMsg_Serdin = false;
    m_bFirstMsg_Canaban = false;


    // 아이템 사용 정보 초기화
    m_pCurDotaInfo->sDPointItemInfo.InitCnt();
    m_pCurDotaInfo->sDPointItemInfo.InitNPCAcc();



    return S_OK;
}

HRESULT KGCDota::InitAtLoadMonster() 
{
	// 고정형 몬스터 설치
	std::vector<SDotaStaticMonsterInfo>::iterator vecStaticMonsterIter = m_pCurDotaInfo->vecsStaticMonsterInfo.begin();
	for(; vecStaticMonsterIter != m_pCurDotaInfo->vecsStaticMonsterInfo.end(); ++vecStaticMonsterIter)
	{
		g_MyD3D->AddMonster( vecStaticMonsterIter->iMonsterID, (vecStaticMonsterIter->vPos.x/SiKGCFantasticMap()->GetPixelWidth()), vecStaticMonsterIter->vPos.y, vecStaticMonsterIter->bIsRight, false,
			0, -1, 0x00000000, false, false, vecStaticMonsterIter->iSlotID );

		MONSTER *pMonster = g_kMonsterManager.GetMonster(vecStaticMonsterIter->iSlotID);

		if(pMonster == NULL)
			continue;

		pMonster->m_iTeam = vecStaticMonsterIter->iTeam;
		pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
		pMonster->m_dwOwnerUID = MAX_PLAYER_NUM;
		pMonster->m_bLive = true;
		pMonster->m_bRender = true;
		pMonster->SetImitSummon( false );
		pMonster->ToggleMonsterInfo( true );
		pMonster->m_fHPPoint = float(vecStaticMonsterIter->iHP);
		pMonster->m_fHP = float(vecStaticMonsterIter->iHP);
		pMonster->SetMovableMonster( false );
		pMonster->SetTargetingDelayTime( GC_FPS_LIMIT * 6 );
	}

	// 코어 HP저장
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_pCurDotaInfo->iCoreSerdin_MonSlotID);
	if ( pMonster != NULL )
		m_iSerdinCoreHp = int( pMonster->GetMaxHP() * 0.9f );
	pMonster = g_kMonsterManager.GetMonster(m_pCurDotaInfo->iCoreCanaban_MonSlotID);
	if ( pMonster != NULL )
		m_iCanabanCoreHp = int(pMonster->GetMaxHP() * 0.9f);


	return S_OK;

}


HRESULT KGCDota::InitAtLoading()
{
    if( g_pkUIScene->m_pkGameOverlayUI )
        g_pkUIScene->m_pkGameOverlayUI->m_pkDotaItemImg->CreateItemImg();

    return S_OK;
}

HRESULT KGCDota::Render()
{
    if( -1 == m_iGameStartCoolTime )
    {
        KGCGameModeInterface::Render();
    }

    return S_OK;
}

HRESULT KGCDota::CustomFrameMove( float fElapsedTime )
{

    PROFILE_SET("KGCDota::CustomFrameMove");

    SendP2PPacket( GC_PID_RELAY_PING );
    g_MyD3D->Story++;

    //퀘스트 컨트롤
    if (g_kEveryTriggerManager.MovetoNextStage() == true)
    {
		InitAtLoadMonster();
        //게임서버로 자신의 로딩이 끝났음을 알린다.
        KP2P::GetInstance()->Send_StageLoadCompleteNot();
        SendNetworkData( GC_PID_UDP_RELAYSERVER_HEART_BEAT, 0 );        
        SendP2PPacket( GC_PID_RELAY_PING );

#if defined(USE_PING_STATE_UI)
        KP2P::GetInstance()->Send_RoomMemberPingInfoReq();

    #if defined(USE_ONLY_TCP_RELAY)
        KP2P::GetInstance()->Send_PingInfoNot(g_kGlobalValue.GetTRAvgPing());
    #else
        SendPingToTCPRelay();
    #endif
#endif

        // 로딩단계를 18단계로 구분하기 때문에 18을 넣어줌
        g_pkUIScene->m_pkGameOverlayUI->DungeonMyLoadingState(18);    
        g_pkUIScene->m_pkGameOverlayUI->RenderDungeonLoadingState( false );
        KP2P::GetInstance()->Send_RelayLoadingState(std::make_pair( g_kGlobalValue.m_kUserInfo.dwUID, 18));

        g_pkQuestManager->SetEnableMovement(true);

        return S_OK;
    }

    // 스테이지 로딩이 완료되었나 확인해본다.
    if( !g_pkQuestManager->IsStageLoadComplete() )
    {
        // 30초가 지나도 로딩이 끝나지 않는 경우를 체크해서 서버로 리포팅한다. 30초동안 스테이지로딩이 끝나지 않으면 분명 문제가 있다.
        if( g_pkQuestManager->m_kStageLoadingStopWatch.IsStop() )
        {
            g_pkQuestManager->m_kStageLoadingStopWatch.SetStopCheck(true);
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_STAGE_LOADING_OVER_TIME );
        }

        return S_OK;
    }

    // 퀘스트트리거를 구동한다.
    g_kEveryTriggerManager.FrameMove();

    // 트리거가 매치가 안되었을 경우 타이머(10초) 후 퀘스트에서 나간다.
    if( g_kEveryTriggerManager.FrameMove_NotMatchTrigger() )
        return E_FAIL;

#if defined(ENABLE_DUGEON_HACK_CHECKING)
    // 해킹체크
    g_pkQuestManager->CheckHackAction();
#endif

    // 쿨타임 처리
    if( E_FAIL == CalcCoolTime() )
        return S_OK;

    SiKGCEmbarkManager()->FrameMove();

    // 플레이어에 대한 프레임 업데이트 및 충돌처리용 영역을 계산한다.
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Calculate_Body_Rect();
        g_MyD3D->MyPlayer[i]->Frame_Move();
    }

    // 몬스터에 대한 프레임 업데이트
    g_kMonsterManager.FrameMove();	

    // 플레이어의 충돌을 체크한다.
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Crash_Check();
    }

    // 펫에 대한 프레임 업데이트
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        g_MyD3D->m_akPet[i]->FrameMove();
    }

    // 오브젝트 프레임 업데이트
    for(int i=0;i<MAX_OBJECT;i++)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            g_MyD3D->m_pObject[i]->FrameMove();			
        }
    }

    // 디폴트 D-Point처리
    CalcDefault_D_Point();

    // 코어몬스터 HP상태에 따른 D-Point처리
    CalcCore_D_Point();

    // 코어몬스터 HP상태에 따른 메시지처리
    CalcCore_Message();

    //목숨처리( 부활처리 )
    CheckRebirth();

    //아이템 처리
    FrameMove_Item();

    // 게임종료 체크
    CheckDotaEnd();

    //g_ParticleManager->CreateSequence( "Portal_Zone_Effect01", 
    //    m_pTeleporter->GetMatrix()->GetXPos(),
    //    m_pTeleporter->GetMatrix()->GetYPos(), 1.9f );
    //g_ParticleManager->CreateSequence( "Portal_Zone_Effect02", 
    //    m_pTeleporter->GetMatrix()->GetXPos(),
    //    m_pTeleporter->GetMatrix()->GetYPos(), 1.9f );

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}

void KGCDota::CalcCore_Message()
{
    if( m_pCurDotaInfo )
    {
        int iAttackTeam = -1;
        int iPlayerIdx = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iPlayerIdx) && g_MyD3D->MyPlayer[iPlayerIdx]->IsLocalPlayer() &&
			g_kMonsterManager.IsMonsterIndex(m_pCurDotaInfo->iCoreSerdin_MonSlotID) &&
			g_kMonsterManager.IsMonsterIndex(m_pCurDotaInfo->iCoreCanaban_MonSlotID))
        {

            if( TEAM_SERDIN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
            {
                if( m_iSerdinCoreHp > g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreSerdin_MonSlotID ]->GetHP() )
                {
                    m_iSerdinCoreHp = int(m_iSerdinCoreHp - (m_iSerdinCoreHp * 0.1f));
                    iAttackTeam = STR_TEAM_SERDIN;
                }
            }
            else if( TEAM_CANABAN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
            {
                if( m_iCanabanCoreHp > g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreCanaban_MonSlotID ]->GetHP() )
                {
                    m_iCanabanCoreHp = int(m_iCanabanCoreHp - (m_iCanabanCoreHp * 0.1f));
                    iAttackTeam = STR_TEAM_CANABAN;
                }
            }

            if( -1 < iAttackTeam )
            {
                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_HP_CORE_RADIO].first;
                kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_HP_CORE_RADIO].second;
                kMessage.strValue1 = const_cast<WCHAR*>(g_pkStrLoader->GetString( iAttackTeam ).c_str());
                pMessage->SetMessage( &kMessage );
                if( iAttackTeam != g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
                    g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_ATTACK_CORE );
            }
        }
    }
}

void KGCDota::CalcDefault_D_Point()
{
    if( m_pCurDotaInfo )
    {
        if( m_pCurDotaInfo->sDefault_D_Point.iCoolTime < m_iDefaultDPointCool )
        {
            bool bSendOk=false;
            int iMyPlayerID = g_MyD3D->Get_MyPlayer();
            int iDPoint[MAX_PLAYER_NUM]={0,};
            for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( ( g_MyD3D->MyPlayer[ iLoop ]->IsLive() ) &&
                    ( g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point() < g_MyD3D->MyPlayer[ iLoop ]->Get_MAX_D_Point() ))
                {
                    if( g_kGlobalValue.m_kUserInfo.bHost )
                    {
                        bSendOk = true;
                        g_MyD3D->MyPlayer[ iLoop ]->Set_D_Point( g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point() + m_pCurDotaInfo->sDefault_D_Point.iDPoint );
                        iDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
                    }

                    // 같은팀원이면 킬러의 획득한 DPoint를 표현해주자!!
                    if( g_MyD3D->IsPlayerIndex(iMyPlayerID) && g_MyD3D->IsSameTeam(iMyPlayerID, iLoop) )
                    {
                        g_MyD3D->MyPlayer[ iLoop ]->AddParticleUINumber( m_pCurDotaInfo->sDefault_D_Point.iDPoint, D3DXVECTOR2(-1.05f, -0.05f), true );
                    }
                }
                else
                    iDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
            }

            if( g_kGlobalValue.m_kUserInfo.bHost && bSendOk )
            {
                SendCore_D_Point( iDPoint );
                g_pkUIScene->m_pkDotaShop->RefreshMyPoint();
            }
            m_iDefaultDPointCool=0;
        }
        ++m_iDefaultDPointCool;
    }
}

void KGCDota::CalcCore_D_Point() 
{
    if( m_pCurDotaInfo )
    {
        bool bSendSerdinDPoint=false;
        bool bSendCanabanDPoint=false;

		if ( g_kMonsterManager.IsMonsterIndex( m_pCurDotaInfo->iCoreSerdin_MonSlotID ) == false )
			return;
		if ( g_kMonsterManager.IsMonsterIndex( m_pCurDotaInfo->iCoreCanaban_MonSlotID ) == false )
			return;

        float fSerdinLimitHP = g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreSerdin_MonSlotID ]->GetMaxHP() * m_pCurDotaInfo->sCore_D_Point.fHpRadio;
        float fCanabanLimitHP = g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreCanaban_MonSlotID ]->GetMaxHP() * m_pCurDotaInfo->sCore_D_Point.fHpRadio;

        int iDPoint[MAX_PLAYER_NUM]={0,};

        // 세르딘 포인트 획득처리
        if( g_kMonsterManager.m_mapMonster[m_pCurDotaInfo->iCoreSerdin_MonSlotID]->GetHP() <= fSerdinLimitHP )
        {
            if( m_pCurDotaInfo->sCore_D_Point.iCoolTime < m_iSerdinCoreDPointCool)
            {
                bSendSerdinDPoint = SetCore_D_Point( m_pCurDotaInfo->iCoreSerdin_MonSlotID, g_kGlobalValue.GetDotaCore_D_Point(), iDPoint );
                m_iSerdinCoreDPointCool=0;
            }
            ++m_iSerdinCoreDPointCool;
        }

        // 카나반 포인트 획득처리
        if( g_kMonsterManager.m_mapMonster[m_pCurDotaInfo->iCoreCanaban_MonSlotID]->GetHP() <= fCanabanLimitHP )
        {
            if( m_pCurDotaInfo->sCore_D_Point.iCoolTime < m_iCanabanCoreDPointCool)
            {
                bSendCanabanDPoint = SetCore_D_Point( m_pCurDotaInfo->iCoreCanaban_MonSlotID, g_kGlobalValue.GetDotaCore_D_Point(), iDPoint );
                m_iCanabanCoreDPointCool=0;
            }
            ++m_iCanabanCoreDPointCool;
        }

        // 포인트 획득 메시지 전송
        if( g_kGlobalValue.m_kUserInfo.bHost && (bSendSerdinDPoint || bSendCanabanDPoint) )
        {
            SendCore_D_Point( iDPoint );
        }

        // DPoint지급 알림 메시지 처리 : 최초한번만 m_pCurDotaInfo->sCore_D_Point.fHpRadio * 100 값을 출력하도록 String처리
        //                                  내 코어 에너지가 달았을때
        int iMyPlayer = g_MyD3D->Get_MyPlayer();
        if( (false == m_bFirstMsg_Serdin) && (false == m_bFirstMsg_Canaban) &&
            g_MyD3D->IsPlayerIndex(iMyPlayer) )
        {
            if( ((bSendSerdinDPoint && (int(TEAM_SERDIN) == g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam)) &&
                  g_kMonsterManager.m_mapMonster[m_pCurDotaInfo->iCoreSerdin_MonSlotID]->IsLive()) ||
                ((bSendCanabanDPoint && (int(TEAM_CANABAN) == g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam)) &&
                  g_kMonsterManager.m_mapMonster[m_pCurDotaInfo->iCoreCanaban_MonSlotID]->IsLive()) )
            {
                m_bFirstMsg_Serdin = bSendSerdinDPoint;
                m_bFirstMsg_Canaban = bSendCanabanDPoint;
                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_HPCORE].first;
                kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_HPCORE].second;
                kMessage.iValue1 = int(m_pCurDotaInfo->sCore_D_Point.fHpRadio * 100.0f);
                pMessage->SetMessage( &kMessage );
            }
        }
    }
}

void KGCDota::FrameMove_Item() 
{
    if( m_pCurDotaInfo )
    {
        int iMyPlayerIdx = g_MyD3D->Get_MyPlayer();
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
                continue;

            // 체력증가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_HPUP] > 0 )
            {
                g_MyD3D->MyPlayer[iLoop]->SetHP( g_MyD3D->MyPlayer[iLoop]->GetHP() + m_pCurDotaInfo->sDPointItemInfo.fHp );
                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_HPUP] = 0;
            }
            // 공격력 중가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_ATK] > 0 )
            {
                g_MyD3D->MyPlayer[iLoop]->SetAttack( g_MyD3D->MyPlayer[iLoop]->GetAttack() + m_pCurDotaInfo->sDPointItemInfo.fAtk );
                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_ATK] = 0;

                if( iMyPlayerIdx == iLoop)
                    ++m_pCurDotaInfo->sDPointItemInfo.iUseCntAtk;
            }
            // 최대 생명치 증가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_FULLHPUP] > 0 )
            {
                g_MyD3D->MyPlayer[iLoop]->m_fAbility[ABILITY_HP] += m_pCurDotaInfo->sDPointItemInfo.fFullHp;
                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_FULLHPUP] = 0;

                if( iMyPlayerIdx == iLoop)
                    ++m_pCurDotaInfo->sDPointItemInfo.iUseCntFullHp;
            }
            // 크리티컬확률 증가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_CRITICAL] > 0 )
            {
                g_MyD3D->MyPlayer[iLoop]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += m_pCurDotaInfo->sDPointItemInfo.fCritical;
                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_CRITICAL] = 0;

                if( iMyPlayerIdx == iLoop)
                    ++m_pCurDotaInfo->sDPointItemInfo.iUseCntCri;
            }
            // 아래나 위치로 이동
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_MOVE_ARENA] > 0 )
            {
                g_MyD3D->MyPlayer[iLoop]->Calculate_Arena_Pos();
                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_MOVE_ARENA] = 0;
            }
            // NPC몬스터 공격력 증가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_ATTACKUP] > 0 )
            {
                if( TEAM_SERDIN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    m_pCurDotaInfo->sDPointItemInfo.fAccSerdinNPCAtk += m_pCurDotaInfo->sDPointItemInfo.fNPCAtk;
                else
                    m_pCurDotaInfo->sDPointItemInfo.fAccCanabanNPCAtk += m_pCurDotaInfo->sDPointItemInfo.fNPCAtk;

                if( g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.iTeam == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    ++m_pCurDotaInfo->sDPointItemInfo.iUseCntNPCAtk;

                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_ATTACKUP] = 0;
            }
            // NPC몬스터 HP 증가
            if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_HPUP] > 0 )
            {
                if( TEAM_SERDIN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    m_pCurDotaInfo->sDPointItemInfo.fAccSerdinNPCHp += m_pCurDotaInfo->sDPointItemInfo.fNPCHp;
                else
                    m_pCurDotaInfo->sDPointItemInfo.fAccCanabanNPCHp += m_pCurDotaInfo->sDPointItemInfo.fNPCHp;

                if( g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.iTeam == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    ++m_pCurDotaInfo->sDPointItemInfo.iUseCntNPCHp;

                g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_HPUP] = 0;
            }
        }
    }
}

HRESULT KGCDota::CalcExpAndGP()
{ return S_OK; }

void KGCDota::CheckDotaEnd() 
{
    if( m_pCurDotaInfo )
    {
        // 코어 인덱스 검사
        if( false == g_kMonsterManager.IsMonsterIndex( m_pCurDotaInfo->iCoreSerdin_MonSlotID ) ||
            false == g_kMonsterManager.IsMonsterIndex( m_pCurDotaInfo->iCoreCanaban_MonSlotID ) )
        {
            // 코어 몬스터 인덱스 오류
#if !defined(__PATH__)
            char szTemp[1024];
            sprintf(szTemp, "DotaTemplate.stg 오류 : 코어몬스터 인덱스 : 세르딘(%d), 카나반(%d)", m_pCurDotaInfo->iCoreSerdin_MonSlotID, m_pCurDotaInfo->iCoreCanaban_MonSlotID );
            MessageBoxA( NULL, szTemp, "DotaError", MB_OK );
#endif
            m_bGameEnd = true;
            return;
        }

        if( -1 == m_iEndGameCoolTime )
        {
            int iSerdin = 0;
            int iCanaban = 0;
#if defined(__PATH__)
            // 팀별 플레이어가 없을때 종료
            for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( g_MyD3D->MyPlayer[iLoop]->IsLive() )
                {
                    if( TEAM_SERDIN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam ) ++iSerdin;
                    if( TEAM_CANABAN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam ) ++iCanaban;
                }
            }
            if( 0 == iSerdin || 0 == iCanaban )
            {
                IsBlueWin = (0 != iSerdin);

                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_EMPTY_TEAM].first;
                kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_EMPTY_TEAM].second;
                pMessage->SetMessage( &kMessage );

                m_iEndGameCoolTime = m_pCurDotaInfo->iGameEndCoolTime;
            }
#endif
            // 코어 둘 중 하나가 죽었을때 게임종료
            if( (false == g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreSerdin_MonSlotID ]->IsLive()) ||
                (false == g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreCanaban_MonSlotID ]->IsLive()) )
            {
                int iMyPlayer = g_MyD3D->Get_MyPlayer();
                if( g_MyD3D->IsPlayerIndex(iMyPlayer) && g_MyD3D->MyPlayer[iMyPlayer]->IsLocalPlayer() )
                {
                    int iKiller;
                    int iKillTeam;
                    IsBlueWin = g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreSerdin_MonSlotID ]->IsLive();
                    if( IsBlueWin )
                    {
                        iKiller = g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreCanaban_MonSlotID ]->GetLastAttackedMe();
                        iKillTeam = STR_TEAM_CANABAN;
                    }
                    else
                    {
                        iKiller = g_kMonsterManager.m_mapMonster[ m_pCurDotaInfo->iCoreSerdin_MonSlotID ]->GetLastAttackedMe();
                        iKillTeam = STR_TEAM_SERDIN;
                    }

                    std::wstring wstrKiller;
                    if( g_MyD3D->IsPlayerIndex(iKiller) )
                        wstrKiller = g_MyD3D->MyPlayer[iKiller]->GetStrUserName().c_str();
                    else if( g_kMonsterManager.IsMonsterIndex(iKiller) )
                        wstrKiller = g_kMonsterManager.m_mapMonster[iKiller]->GetMonsterName();
                    KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                    KInGameMessageAttribute kMessage;
                    kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_BREAK_CORE].first;
                    kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_BREAK_CORE].second;
                    kMessage.strValue1 = const_cast<WCHAR*>(wstrKiller.c_str());
                    kMessage.strValue2 = const_cast<WCHAR*>(g_pkStrLoader->GetString( iKillTeam ).c_str());
                    pMessage->SetMessage( &kMessage );
                }
                m_iEndGameCoolTime = m_pCurDotaInfo->iGameEndCoolTime;
            }
        }
        else if( 0 == m_iEndGameCoolTime )
        {
            for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( TEAM_SERDIN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    g_pGameBoard->m_pStartingMember[iLoop].IsWin = IsBlueWin;
                else if( TEAM_CANABAN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    g_pGameBoard->m_pStartingMember[iLoop].IsWin = !IsBlueWin;
            }
            m_bGameEnd = true;
        }
        else
            --m_iEndGameCoolTime;
    }
}

bool KGCDota::CheckGameEnd()
{
    return m_bGameEnd;
}

void KGCDota::GameEnd()
{
    DestroyDialogUI();
}

void KGCDota::DestroyDialogUI()
{
    // 대기중인 도타메시지 박스 모두 제거
    while( g_pkUIScene->DeleteWaitMsgBoxforUse(KGCUIScene::GC_MBOX_USE_DOTA_GAME) );
    while( g_pkUIScene->DeleteWaitMsgBoxforUse(KGCUIScene::GC_MBOX_USE_MAX) );

    // 대기중인 상점 UI 닫기처리
    while( g_pkUIScene->RemoveWaitMessageBox( KGCUIScene::GC_MBOX_DOTA_SHOP ) );

    // 현재열린 상점 UI 닫기처리
    if( g_pkUIScene->m_pkDotaShop )
        g_pkUIScene->m_pkDotaShop->OnClickClsoe();
}

// 코어 D-Point지급 시스템
bool KGCDota::SetCore_D_Point(int iMonsterID, int iPoint, int *piDPoint)
{
    bool bResult = false;

    if( NULL == piDPoint || false == g_kMonsterManager.IsMonsterIndex( iMonsterID ) )
        return bResult;

    int iMyPlayerID = g_MyD3D->Get_MyPlayer();
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( ( g_MyD3D->MyPlayer[ iLoop ]->IsLive() ) &&
            ( g_MyD3D->MyPlayer[ iLoop ]->m_kUserInfo.iTeam == g_kMonsterManager.m_mapMonster[iMonsterID]->m_iTeam ) &&
            ( g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point() < g_MyD3D->MyPlayer[ iLoop ]->Get_MAX_D_Point() ) )
        {
            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
                g_MyD3D->MyPlayer[ iLoop ]->Set_D_Point( g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point() + iPoint );
                piDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
            }

            // 내가 획득한 DPoint는 표현해주자!!
            if( g_MyD3D->IsPlayerIndex(iMyPlayerID) && g_MyD3D->IsSameTeam(iMyPlayerID, iLoop) )
            {
                g_MyD3D->MyPlayer[ iLoop ]->AddParticleUINumber( g_kGlobalValue.GetDotaCore_D_Point(), D3DXVECTOR2(-1.05f, -0.05f), true );
            }

            bResult = true;
        }
        else
            piDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
    }
    return bResult;
}

void KGCDota::SendCore_D_Point( int *piDPoint )
{
    if( NULL == piDPoint )
        return;

    int iHostID = g_MyD3D->Get_MyPlayer();
    if( iHostID >= MAX_PLAYER_NUM ) iHostID = 0;
    KGC_PID_BROAD_DOTA_POINT kCustom( GC_PID_BROAD_DOTA_POINT, g_MyD3D->MyPlayer[ iHostID ]->m_kUserInfo.dwUID, piDPoint );
    SendP2PPacket( &kCustom, sizeof(kCustom), KNC_BROADCAST_NOT_ME, _RELIABLE );
    KGCPC_MEMO( "GC_PID_BROAD_DOTA_POINT" );
}

HRESULT KGCDota::CalcCoolTime()
{
    // 게임시작 쿨타임 처리
    if( 0 < m_iGameStartCoolTime )
    {
        --m_iGameStartCoolTime;
        return E_FAIL;
    }
    else
        m_iGameStartCoolTime = -1;

    KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();

    // 게임방법 메시지 처리
    if( m_iPlayerGameStartCoolTime == m_pCurDotaInfo->iGameStartCoolTime )
    {
        --m_iPlayerGameStartCoolTime;
        KInGameMessageAttribute kMessage;
        kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTGAME].first;
        kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTGAME].second;
        pMessage->SetMessage( &kMessage );
        g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_STARTGAME );
    }

    // 게임 시작 알림 메시지
    else if( 0 == pMessage->IsRenderToString( m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTGAME].first ) )
    {
        KInGameMessageAttribute kMessage;
        kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTPLAYE].first;
        kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTPLAYE].second;
        pMessage->SetMessage( &kMessage );
        g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_MINIMAP );

        // 미니맵 활성화
        g_pkUIScene->m_pkGameOverlayUI->ToggleDotaTileMap( true );
    }

    // 아레나 알림 메시지
    else if( m_iArenaCoolTime == m_pCurDotaInfo->iArenaCoolTime )
    {
        --m_iArenaCoolTime;
        KInGameMessageAttribute kMessage;
        kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_ARENA].first;
        kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_ARENA].second;
        pMessage->SetMessage( &kMessage );
    }

    // 플레이어 게임시작 쿨타임 처리
    if( 0 < m_iPlayerGameStartCoolTime )
    {
        --m_iPlayerGameStartCoolTime;

        // 카운터 숫자랜더링 셋팅( 아래주석은 알림문 출력 후 숫자렌더링 되도록 제어한것 )
        //if( (m_pCurDotaInfo->iGameStartCoolTime - m_iPlayerGameStartCoolTime) > m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_STARTGAME].second )
        g_pkUIScene->m_pkGameOverlayUI->SetDotaCount( (m_iPlayerGameStartCoolTime/GC_FPS_LIMIT) + 1 );
    }
    else  if( 0 == m_iPlayerGameStartCoolTime )
    {
        m_iPlayerGameStartCoolTime = -1;

        // 이동가능 알림 메시지
        KInGameMessageAttribute kMessage;
        kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PORTALOPEN].first;
        kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PORTALOPEN].second;
        pMessage->SetMessage( &kMessage );

        // 포탈 이펙트 출력
        ShowPortalEffect();
    }

    // 아레나 쿨타임 처리
    if( 0 < m_iArenaCoolTime )
    {
        --m_iArenaCoolTime;

        // 카운터 숫자랜더링 셋팅( 아래주석은 알림문 출력 후 숫자렌더링 되도록 제어한것 )
        //if( (m_pCurDotaInfo->iArenaCoolTime - m_iArenaCoolTime) > m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_ARENA].second )
        g_pkUIScene->m_pkGameOverlayUI->SetDotaCount( (m_iArenaCoolTime/GC_FPS_LIMIT) + 1 );
    }
    else if( 0 == m_iArenaCoolTime )
    {
        m_iArenaCoolTime = -1;

        // 이동가능 알림 메시지
        KGCInGameMessage *pMessage = g_pkGameOverlayUI->GetInGameMessage();
        KInGameMessageAttribute kMessage;
        kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PORTALOPEN].first;
        kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PORTALOPEN].second;
        pMessage->SetMessage( &kMessage );

        // 포탈 이펙트 출력
        ShowPortalEffect();
    }

    return S_OK;
}

void KGCDota::ShowPortalEffect()
{
    // 시작지점 포탈에 이펙트 붙혀주기
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex(iPlayerIdx) )
    {
        KGCTeleporter *teleporter = KGCTeleporter::GetDotaStageInTeleporter( g_MyD3D->MyPlayer[iPlayerIdx]->GetDotaStage() );
        if( teleporter )
        {
            D3DXVECTOR2 vPos = teleporter->GetPos();
            g_ParticleManager->CreateSequence( "Dota_Open_01", vPos.x+1.0f, vPos.y+0.45f, 1.0f );
            g_ParticleManager->CreateSequence( "Dota_Open_02", vPos.x+1.0f, vPos.y+0.45f, 1.0f );
            g_ParticleManager->CreateSequence( "Dota_Open_03", vPos.x+1.0f, vPos.y+0.45f, 1.0f );
        }
    }
}

// 도타 템플릿 로드
void KGCDota::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
    {
        MessageBoxA( NULL, "Enum.stg Load Failed!", "", MB_OK );
        return;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "DotaTemplate.stg" ) == false )
    {
        MessageBoxA( NULL, "DotaTemplate.stg Load Failed!", "", MB_OK );
        return;
    }

    g_kGlobalValue.m_vecDotaTemplate.clear();

    luaMgr.BeginTable( "DotaTemplate" );
    {
        for(int iLoop=1; ; ++iLoop)
        {
            char szTemp[256];
            sprintf(szTemp, "dota_quest_%d", iLoop);
            if( FAILED( luaMgr.BeginTable( szTemp ) ) ) break;
            {
                SDotaTemplate kDotaInfo;
                
                LUA_GET_VALUE_DEF("UseAIFromScript", kDotaInfo.bUseAIFromScript, false);
                LUA_GET_VALUE_DEF("Map", kDotaInfo.strMap, "");

                LUA_GET_VALUE_DEF("GameStartCoolTime", kDotaInfo.iGameStartCoolTime, 0);
                LUA_GET_VALUE_DEF("ArenaCoolTime", kDotaInfo.iArenaCoolTime, 0);
                LUA_GET_VALUE_DEF("GameEndCoolTime", kDotaInfo.iGameEndCoolTime, 0);
                LUA_GET_VALUE_DEF("GameStartLimitUser", kDotaInfo.iGameStartLimitUser, 0);
                LUA_GET_VALUE_DEF("NumZoneMax", kDotaInfo.iNumZoneMax, 0);

                LUA_GET_VALUE_DEF("CoreZone_Serdin", kDotaInfo.iCoreZone_Serdin, 0);
                LUA_GET_VALUE_DEF("CoreZone_Canaban", kDotaInfo.iCoreZone_Canaban, 0);

                LUA_GET_VALUE_DEF("D_PointToPlayer", kDotaInfo.iPlayer_D_Point, 0);
                LUA_GET_VALUE_DEF("D_PointToMonster", kDotaInfo.iMonster_D_Point, 0);
                LUA_GET_VALUE_DEF("D_PointToKillerMonster", kDotaInfo.iKillerMonster_D_Point, 0);
                LUA_GET_VALUE_DEF("CoreSerdin_MonSlotID", kDotaInfo.iCoreSerdin_MonSlotID, -1);
                LUA_GET_VALUE_DEF("CoreCanaban_MonSlotID", kDotaInfo.iCoreCanaban_MonSlotID, -1);

                LUA_GET_VALUE_DEF("SummonMonsterStartSlotIndex", kDotaInfo.iSummonMonsterStartSlotIndex, 0);
                
                LoadAbility(luaMgr, kDotaInfo);
                LoadDefault_D_Point(luaMgr, kDotaInfo);
                LoadCore_D_Point(luaMgr, kDotaInfo);
                LoadStaticMonster(luaMgr, kDotaInfo);
                LoadSummonGate(luaMgr, kDotaInfo);
                LoadMonsterPortal(luaMgr, kDotaInfo);
                LoadDPointShop(luaMgr, kDotaInfo);
                LoadDPointShopItem(luaMgr, kDotaInfo);
                LoadDPointItem(luaMgr, kDotaInfo);
                LoadDotaMsg(luaMgr, kDotaInfo);
                LoadDotaGameGuide(luaMgr, kDotaInfo);

                kDotaInfo.iDotaMapID = iLoop;
                g_kGlobalValue.m_vecDotaTemplate.push_back( kDotaInfo );
            }
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

#if !defined(__PATH__)
    // 방장이면 아이템 능력정보를 보내주자
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        KGCDota::SendDotaItemInfo();
    }
#endif

}

void KGCDota::LoadAbility( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "CharacterAbility" ) ) )
    {
        int iAbilityLoop=1;
        while( true )
        {
            SDotaAbility sDotaAbility;
            if( SUCCEEDED(luaMgr.BeginTable( iAbilityLoop )) )
            {
                LUA_GET_VALUE_DEF(1, sDotaAbility.fAbilityAtk, 0.0f);
                LUA_GET_VALUE_DEF(2, sDotaAbility.fAbilityDef, 0.0f);
                LUA_GET_VALUE_DEF(3, sDotaAbility.fAbilityHp, 0.0f);
            }
            else
                break;
            luaMgr.EndTable();

            ++iAbilityLoop;
            kDotaInfo.vecsDotaAbility.push_back( sDotaAbility );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDefault_D_Point( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "Default_D_Point" ) ) )
    {
        LUA_GET_VALUE_DEF("COOLTIME", kDotaInfo.sDefault_D_Point.iCoolTime, 0);
        LUA_GET_VALUE_DEF("D_POINT", kDotaInfo.sDefault_D_Point.iDPoint, 0);
    }
    luaMgr.EndTable();
}

void KGCDota::LoadCore_D_Point( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo ) 
{
    if( SUCCEEDED( luaMgr.BeginTable( "Core_D_Point" ) ) )
    {
        LUA_GET_VALUE_DEF("HP_RADIO", kDotaInfo.sCore_D_Point.fHpRadio, 0.0f);
        LUA_GET_VALUE_DEF("COOLTIME", kDotaInfo.sCore_D_Point.iCoolTime, 0);
        LUA_GET_VALUE_DEF("D_POINT", kDotaInfo.sCore_D_Point.iDPoint, 0);
    }
    luaMgr.EndTable();
}

void KGCDota::LoadStaticMonster( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo ) 
{
    if( SUCCEEDED( luaMgr.BeginTable( "StaticMonster" ) ) )
    {
        int iStaticMonsterInfoLoop=1;
        while( true )
        {
            SDotaStaticMonsterInfo sStaticMonsterInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iStaticMonsterInfoLoop )) )
            {
                LUA_GET_VALUE_DEF("MonsterID",   sStaticMonsterInfo.iMonsterID, -1);
                LUA_GET_VALUE_DEF("SlotID",      sStaticMonsterInfo.iSlotID, -1);
                LUA_GET_VALUE_DEF("IsRight",     sStaticMonsterInfo.bIsRight, false);
                LUA_GET_VALUE_DEF("Team",        sStaticMonsterInfo.iTeam, -1);
                LUA_GET_VALUE_DEF("HP",          sStaticMonsterInfo.iHP, 0);
                LUA_GET_VALUE_DEF("PositionX",   sStaticMonsterInfo.vPos.x, 0.0f);
                LUA_GET_VALUE_DEF("PositionY",   sStaticMonsterInfo.vPos.y, 0.0f);
            }
            else
                break;
            luaMgr.EndTable();

            ++iStaticMonsterInfoLoop;
            kDotaInfo.vecsStaticMonsterInfo.push_back( sStaticMonsterInfo );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadSummonGate( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo ) 
{
    if( SUCCEEDED( luaMgr.BeginTable( "MonsterSummonGateInfo" ) ) )
    {
        int iGateInfoLoop=1;
        while( true )
        {
            SDotaGateInfo sGateInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iGateInfoLoop )) )
            {
                LUA_GET_VALUE_DEF("GateID",     sGateInfo.iGateID, -1);
                LUA_GET_VALUE_DEF("HP",         sGateInfo.iHP, 0);
                LUA_GET_VALUE_DEF("CoolTime",   sGateInfo.iSummonCoolTime, 0);
                LUA_GET_VALUE_DEF("Limit",      sGateInfo.iSummonLimit, 0);
                LUA_GET_VALUE_DEF("Team",       sGateInfo.iTeam, -1);
                LUA_GET_VALUE_DEF("IsRight",    sGateInfo.bIsRight, false);

                sGateInfo.vecMonsterType.clear();
                if( SUCCEEDED( luaMgr.BeginTable( "MonsterType" ) ) )
                {
                    int iTemp;
                    int iMonsterTypeLoop=1;
                    while(true)
                    {
                        LUA_GET_VALUE_NOASSERT( iMonsterTypeLoop, iTemp, break; );
                        if( 0 <= iTemp && iTemp < MONSTER_MODEL_NUM )
                            sGateInfo.vecMonsterType.push_back( iTemp );
                        iMonsterTypeLoop++;
                    }
                }
                luaMgr.EndTable();
            }
            else
                break;
            luaMgr.EndTable();

            ++iGateInfoLoop;
            kDotaInfo.vecsGateInfo.push_back( sGateInfo );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadMonsterPortal( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo ) 
{
    if( SUCCEEDED( luaMgr.BeginTable( "MonsterPortalInfo" ) ) )
    {
        int iPortalInfoLoop=1;
        while( true )
        {
            SDotaMonsterPortalInfo sMonsterPortalInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iPortalInfoLoop )) )
            {
                LUA_GET_VALUE_DEF("PortalID",  sMonsterPortalInfo.iPortalID, -1);
                LUA_GET_VALUE_DEF("Width",     sMonsterPortalInfo.vSize.x, 0.0f);
                LUA_GET_VALUE_DEF("Height",    sMonsterPortalInfo.vSize.y, 0.0f);
                LUA_GET_VALUE_DEF("MoveX",     sMonsterPortalInfo.vMovePos.x, 0.0f);
                LUA_GET_VALUE_DEF("MoveY",     sMonsterPortalInfo.vMovePos.y, 0.0f);
                LUA_GET_VALUE_DEF("Team",      sMonsterPortalInfo.iTeam, false);
            }
            else
                break;
            luaMgr.EndTable();

            ++iPortalInfoLoop;
            kDotaInfo.vecsMonsterPortalInfo.push_back( sMonsterPortalInfo );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDPointShop( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "DPointShopInfo" ) ) )
    {
        int iDpointShopLoop=1;
        while( true )
        {
            SDota_D_PointShopInfo sDPointShopInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iDpointShopLoop )) )
            {
                LUA_GET_VALUE_DEF("ShopID",  sDPointShopInfo.iShopID, -1);
                LUA_GET_VALUE_DEF("Width",   sDPointShopInfo.vSize.x, 0.0f);
                LUA_GET_VALUE_DEF("Height",  sDPointShopInfo.vSize.y, 0.0f);
                LUA_GET_VALUE_DEF("Team",    sDPointShopInfo.iTeam, -1);
            }
            else
                break;
            luaMgr.EndTable();

            ++iDpointShopLoop;
            kDotaInfo.vecsDPointShopInfo.push_back( sDPointShopInfo );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDPointShopItem( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "DPointShopItemInfo" ) ) )
    {
        int iItemID, iItemName, iItemDesc;
        int iDpointShopItemLoop=1;
        kDotaInfo.sDPointShopItemList.mapItemDesc.clear();
        while( true )
        {
            std::map<int, pair<int, int>> sDPointShopInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iDpointShopItemLoop )) )
            {
                LUA_GET_VALUE_DEF(1,  iItemID, 0);
                LUA_GET_VALUE_DEF(2,  iItemName, 0);
                LUA_GET_VALUE_DEF(3,  iItemDesc, 0);
            }
            else
                break;
            luaMgr.EndTable();

            ++iDpointShopItemLoop;
            kDotaInfo.sDPointShopItemList.mapItemDesc.insert( std::make_pair(iItemID, std::make_pair(iItemName, iItemDesc)) );
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDPointItem( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "DPointItemInfo" ) ) )
    {
        LUA_GET_VALUE_DEF("ITEM_DOTA_ATK",  kDotaInfo.sDPointItemInfo.fAtk, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_HPUP",  kDotaInfo.sDPointItemInfo.fHp, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_FULLHPUP",  kDotaInfo.sDPointItemInfo.fFullHp, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_CRITICAL",  kDotaInfo.sDPointItemInfo.fCritical, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_NPC_ATTACKUP",  kDotaInfo.sDPointItemInfo.fNPCAtk, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_NPC_DEFANCEUP",  kDotaInfo.sDPointItemInfo.fNPCDef, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_NPC_HPUP",  kDotaInfo.sDPointItemInfo.fNPCHp, 0.0f);
        LUA_GET_VALUE_DEF("ITEM_DOTA_NPC_SPEEDUP",  kDotaInfo.sDPointItemInfo.fNPCSpeed, 0.0f);
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDotaMsg( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "PlayingDesc" ) ) )
    {
        char szTemp[SDotaPlayingDesc::MAX_DOTA_MSG][128] = 
        {
            "START_GAME",
            "START_PLAYE",
            "PORTAL_OPEN",
            "ARENA",
            "SHOP",
            "IN_COREZONE",
            "HP_CORE",
            "HP_CORE_RADIO",
            "BREAK_CORE",
            "BREAK_GUARDIANTOWER",
            "BREAK_GUARDTOWER",
            "PLAYER_DEATH",
            "EMPTY_TEAM",
        };
        std::pair<int,int> paDotaPlayingDesc;
        for(int iLoop=0; iLoop<SDotaPlayingDesc::MAX_DOTA_MSG; ++iLoop)
        {
            if( SUCCEEDED( luaMgr.BeginTable( szTemp[iLoop] ) ) )
            {
                LUA_GET_VALUE_DEF(1,  paDotaPlayingDesc.first, 0);
                LUA_GET_VALUE_DEF(2,  paDotaPlayingDesc.second, 0);
                kDotaInfo.sDotaMsgInfo.mapDotaMsg[iLoop] = paDotaPlayingDesc;
                luaMgr.EndTable();
            }
        }
    }
    luaMgr.EndTable();
}

void KGCDota::LoadDotaGameGuide( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo )
{
    if( SUCCEEDED( luaMgr.BeginTable( "GameGuide" ) ) )
    {
        char szTemp[SDotaPlayingDesc::MAX_DOTA_MSG][128] = 
        {
            "START_GAME",
            "MINIMAP",
            "FIRST_DEATH",
            "FIRST_KILL",
            "APOINT",
            "SHOP_CLOSE",
            "ATTACK_CORE",
            "BREAK_GUARDTOWER",
            "IN_ZONE",
            "BREAK_GUARDIANTOWER",
        };

        int iTemp;
        for(int iLoop=0; iLoop<SDotaPlayingDesc::MAX_DOTA_MSG; ++iLoop)
        {
            LUA_GET_VALUE_DEF(szTemp[iLoop],  iTemp, -1);
            kDotaInfo.sDotaGameGuide.mapDotaGameGuide[iLoop] = iTemp;
        }
    }
    luaMgr.EndTable();
}

void KGCDota::SendDotaItemInfo(int iIndex, float fValue)
{
#if !defined(__PATH__)
    SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pSDotaTemplate )
    {
        KGC_PID_BROAD_DOTA_ITEM_CHEAT kPacket;

        kPacket.fAtk       = pSDotaTemplate->sDPointItemInfo.fAtk;           
        kPacket.fHp        = pSDotaTemplate->sDPointItemInfo.fHp;            
        kPacket.fFullHp    = pSDotaTemplate->sDPointItemInfo.fFullHp;        
        kPacket.fCritical  = pSDotaTemplate->sDPointItemInfo.fCritical;      
        kPacket.fNPCAtk    = pSDotaTemplate->sDPointItemInfo.fNPCAtk;        
        kPacket.fNPCDef    = pSDotaTemplate->sDPointItemInfo.fNPCDef;        
        kPacket.fNPCHp     = pSDotaTemplate->sDPointItemInfo.fNPCHp;         
        kPacket.fNPCSpeed  = pSDotaTemplate->sDPointItemInfo.fNPCSpeed;      

        switch( iIndex )
        {
        case GC_GAME_ITEM_DOTA_ATK:
            kPacket.fAtk = fValue;
            break;
        case GC_GAME_ITEM_DOTA_HPUP:
            kPacket.fHp = fValue;
            break;
        case GC_GAME_ITEM_DOTA_FULLHPUP:
            kPacket.fFullHp = fValue;
            break;
        case GC_GAME_ITEM_DOTA_CRITICAL:
            kPacket.fCritical = fValue;
            break;
        case GC_GAME_ITEM_DOTA_NPC_ATTACKUP:
            kPacket.fNPCAtk = fValue;
            break;
        case GC_GAME_ITEM_DOTA_NPC_DEFANCEUP:
            kPacket.fNPCDef = fValue;
            break;
        case GC_GAME_ITEM_DOTA_NPC_HPUP:
            kPacket.fNPCHp = fValue;
            break;
        case GC_GAME_ITEM_DOTA_NPC_SPEEDUP:
            kPacket.fNPCSpeed = fValue;
            break;
        }

        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_DOTA_ITEM_CHEAT" );
    }
#endif
}