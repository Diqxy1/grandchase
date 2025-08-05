#include "stdafx.h"
#include "GCGuideMgr.h"
#include "GCSKT.h"

#include "GCUI/GCAddtionGuide.h"
#include "MyD3D.h"

#ifndef __PATH__ 
#include "GCUtil/GCFileMoniter.h"

void GuideFileMoniter()
{
    g_MyD3D->m_kGuideMgr.LoadScript();
}
#endif



KGCGuideMgr::KGCGuideMgr(void)
: m_nCurrentGuideType( -1 )
,m_bEffect(false)
{
    m_mapGuideStepInfo.clear();
    m_mapGuideRequirementInfo.clear();
    m_mapGuidProgressState.clear();
    m_mapGuideComplete.clear();

#ifndef __PATH__ 
    m_nTestType = -1;
#endif
}

KGCGuideMgr::~KGCGuideMgr(void)
{
}

#ifndef __PATH__ 
void KGCGuideMgr::RegisterFileMoniter()
{
    SiKGCFileMoniter()->RegisterFile( "./Stage/GCGuide.lua" , GuideFileMoniter );
}
#endif

bool KGCGuideMgr::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "enum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr, "GCGuide.lua" ) == false )
        return false;

    // 데이터 초기화
    m_mapGuideStepInfo.clear();
    m_mapGuideRequirementInfo.clear();
    m_mapPairCompleteGuideType.clear();

    // 스크립트 읽기~
    LUA_BEGIN_TABLE( "GuideElements", return false )
    {
        for( int i = 1 ; ; ++i)
        {
            LUA_BEGIN_TABLE( i , break )
            {
                // 가이드 종류(타입)
                int nGuideType = -1;
                LUA_GET_VALUE( "Guide_Type", nGuideType, return false );

                // 요구 조건
                SRequirementinfo kRequirementInfo;

                LUA_BEGIN_TABLE( "Requirement", return false );
                {
                    LUA_GET_VALUE_DEF( "GuideState", kRequirementInfo.nGuideState, -1 );
                    LUA_GET_VALUE_DEF( "GameMode", kRequirementInfo.nGameMode, -1 );
                    LUA_GET_VALUE_DEF( "RequirementLevel_Min", kRequirementInfo.nRequirementLevelMin, -1 );
                    LUA_GET_VALUE_DEF( "RequirementLevel_Max", kRequirementInfo.nRequirementLevelMax, -1 );
                    LUA_GET_VALUE_DEF( "CompleteGuide", kRequirementInfo.nCompleteGuide, -1 );
                    LUA_GET_VALUE_DEF( "DungeonID", kRequirementInfo.nDungeonID, -1 );

                    // 미션
                    if( SUCCEEDED(luaMgr.BeginTable( "CompleteMission") ) )
                    {
                        int iTemp;
                        for( int i = 1; ; ++i )
                        {
                            LUA_GET_VALUE_NOASSERT( i, iTemp, break );
                            kRequirementInfo.vecMissionID.push_back( iTemp );
                        }
                        LUA_END_TABLE( ; )
                    }

                    //등록된 미션
                    if( SUCCEEDED(luaMgr.BeginTable( "RegistMission") ) )
                    {
                        int iTemp;
                        for( int i = 1; ; ++i )
                        {
                            LUA_GET_VALUE_NOASSERT( i, iTemp, break );
                            kRequirementInfo.vecRegistMissionID.push_back( iTemp );
                        }
                        LUA_END_TABLE( ; )
                    }

                    //특정조건
                    LUA_GET_VALUE_DEF( "SpecifiedCondition", kRequirementInfo.nSpecifiedCondition, -1 );
                    LUA_GET_VALUE_DEF( "ConcurrentCompleteGuideType", kRequirementInfo.nConcurrentCompleteGuideType, -1 );
                    m_mapPairCompleteGuideType.insert( std::make_pair( nGuideType , kRequirementInfo.nConcurrentCompleteGuideType) );

                    LUA_END_TABLE( ;)
                }

                // 가이드 발동 조건 정보 저장
                m_mapGuideRequirementInfo[nGuideType] = kRequirementInfo;

                // Step별 가이드 정보
                std::vector<SStep> vecStep;

                LUA_BEGIN_TABLE( "Guide", return false );
                {
                    /////////////////////////////////////////////////////////////////////
                    for( int j = 1 ; ; ++j)
                    {
                        LUA_BEGIN_TABLE(j, break )
                        {
                            SStep kStep;

                            // 이 스텝을 통과하기 위한 행동
                            LUA_BEGIN_TABLE( "Action", break )
                            {
                                LUA_GET_VALUE_DEF( "Time", kStep.kAction.nTime, -1 );
                                LUA_GET_VALUE_DEF( "HighlightClick", kStep.kAction.bHighlightClick, false );
                                LUA_GET_VALUE_DEF( "MenuOpenThenPass", kStep.kAction.bMenuOpenThenPass, false );
                                LUA_END_TABLE(;)
                            }

                            // 하이라이트 영역
                            LUA_BEGIN_TABLE( "Highlight_Rect", ; )
                            {
                                LUA_GET_VALUE_DEF( "HighLightType", kStep.kHighlightInfo.nHighLightType, 1 );
                                LUA_GET_VALUE_DEF( "UI_Script_Name", kStep.kHighlightInfo.UIScriptName, "" );
                                LUA_GET_VALUE_DEF( "UI_Name", kStep.kHighlightInfo.UIName, "" );
                                LUA_GET_VALUE_DEF( "UseWndSizeFromUI", kStep.kHighlightInfo.bUseSizeFromUI, false );
                                LUA_GET_VALUE_DEF( "UseWndPosFromUI", kStep.kHighlightInfo.bUsePosFromUI, false );
                                LUA_GET_VALUE_DEF( "Highlight_Pos_x", kStep.kHighlightInfo.fHighLightPosX, 0.0f );
                                LUA_GET_VALUE_DEF( "Highlight_Pos_y", kStep.kHighlightInfo.fHighLightPosY, 0.0f );
                                LUA_GET_VALUE_DEF( "Highlight_Width", kStep.kHighlightInfo.dwHighlightWidth, 0 );
                                LUA_GET_VALUE_DEF( "Highlight_Height", kStep.kHighlightInfo.dwHighlightHeight, 0 );
                                LUA_GET_VALUE_DEF( "ActiveOffset", kStep.kHighlightInfo.ActiveOffset, 5 );
                                LUA_END_TABLE(;)
                            }

                            // NPC
                            LUA_BEGIN_TABLE( "NPC", ; )
                            {
                                LUA_GET_VALUE_DEF( "NPC_Type", kStep.kNPCInfo.nNPCType, -1 );
                                LUA_GET_VALUE_DEF( "NPC_Pos_x", kStep.kNPCInfo.fNPCPosX, 0.0f );
                                LUA_GET_VALUE_DEF( "NPC_Pos_y", kStep.kNPCInfo.fNPCPosY, 0.0f );
                                LUA_GET_VALUE_DEF( "NPC_Desc", kStep.kNPCInfo.nNPCDesc, -1 );
                                LUA_END_TABLE(;)
                            }

                            // Arrow
                            LUA_BEGIN_TABLE( "Arrow", ; )
                            {
                                LUA_GET_VALUE_DEF( "Arrow_Type", kStep.kArrowInfo.nArrowType, -1 );
                                LUA_GET_VALUE_DEF( "Arrow_pos_x", kStep.kArrowInfo.fArrowPosX, 0.0f );
                                LUA_GET_VALUE_DEF( "Arrow_pos_y", kStep.kArrowInfo.fArrowPosY, 0.0f );
                                LUA_END_TABLE(;)
                            }

                            // // 코드 내 처리를 위한 데이터값
                            if( SUCCEEDED( luaMgr.BeginTable( "arg" ) ) ){
                                for( int i = 1; ; ++i ) {
                                    DWORD dwData;

                                    if( FAILED( luaMgr.GetValue(i, dwData) ) )
                                        break;

                                    kStep.vecData.push_back( dwData );
                                }
                                luaMgr.EndTable();
                            }

                            // Particle
                            LUA_BEGIN_TABLE( "Paticle", ; );
                            {
                                for( int k = 1 ; ; ++k)
                                {
                                    LUA_BEGIN_TABLE(k, break )
                                    {
                                        SGuidePaticleInfo particleTemp;
                                        LUA_GET_VALUE_DEF( 1, particleTemp.strPaticleName, "" );
                                        LUA_GET_VALUE_DEF( 2, particleTemp.x, 0.0f );
                                        LUA_GET_VALUE_DEF( 3, particleTemp.y, 0.0f );
                                        LUA_GET_VALUE_DEF( 4, particleTemp.iLayer, 0 );
                                        kStep.vecPaticle.push_back(particleTemp);
                                        LUA_END_TABLE( ; )
                                    }
                                }
                                LUA_END_TABLE( ; )
                            }

                            /////////////////////////// 가이드 정보 읽기 끝. 스텝 백터에 추가
                            vecStep.push_back( kStep );
                        }
                    }//for
                    LUA_END_TABLE( ;)
                }
                // 데이터 저장
                m_mapGuideStepInfo.insert( std::make_pair( nGuideType, vecStep ) );
                LUA_END_TABLE( ;)
            }

        }//for
        LUA_END_TABLE( return false );
    }

#if !defined( __PATH__ )
    LUA_GET_VALUE( "Test_Guide_Type", m_nTestType, -1 );
#endif

    return true;
}


void KGCGuideMgr::FrameMove()
{
    // 현재 진행중인 가이드가 있다면
    if( m_nCurrentGuideType != -1 ) {
        g_pkUIScene->m_pkGCGuide->ToggleRender( true );
        g_pkUIScene->m_pkGCGuide->SetActive();
        g_pkUIScene->m_pkGCGuide->FrameMove();
    }
}

void KGCGuideMgr::UpdateGuideState()
{
    std::map< int, SRequirementinfo >::iterator iter = m_mapGuideRequirementInfo.begin();

    for( ; iter != m_mapGuideRequirementInfo.end(); ++iter ) {
        // 가이드 중 조건미달로 남아있는 가이드만 체크한다.
        std::map< int, int >::iterator iterState = m_mapGuidProgressState.find( (*iter).first );
        if( iterState == m_mapGuidProgressState.end() || (*iterState).second != GPS_BELOW )
            continue;

        SRequirementinfo& kReqInfo = (*iter).second;

        // 미션 완료 검사 ( 서버에서 받은 플래그 검사 )
        if( !kReqInfo.vecMissionID.empty() ) {
            std::map< int, KGuideCompleteInfo >::iterator iterGC = m_mapGuideComplete.find( (*iter).first );

            if( iterGC == m_mapGuideComplete.end() ){
                continue;
            } else {
                if( iterGC->second.m_nCompleteFlag != KGuideCompleteInfo::FLAG_GUIDE_YET ){
                    continue;
                }
            }
        }

        // 등록된 미션 검사
        if( !kReqInfo.vecRegistMissionID.empty() ) {
            bool bRegistMission = false;
            std::vector<int>::iterator vit = kReqInfo.vecRegistMissionID.begin();
            for ( ; vit != kReqInfo.vecRegistMissionID.end(); ++vit )
            {
                //현재 캐릭터 미션이 아니면 패스
                SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
                if( SiKGCMissionManager()->IsAvailableRegister( *vit, sInfo ) != KGCMissionManager::GC_MM_OK )
                    continue;

                //미션이 등록되어있는지 확인
                if( g_kGlobalValue.m_kUserMission.IsUserMission( *vit ) == true ||
                    g_kGlobalValue.m_kUserMission.IsInvalidMission( *vit ) != 0 )
                {
                    bRegistMission = true;
                    break;
                }
            }

            if ( !bRegistMission )
                continue;
        }

        // 선행가이드가 있나 확인
        if( kReqInfo.nCompleteGuide != -1 ){
            std::map< int, int >::iterator iterGPS = m_mapGuidProgressState.find( kReqInfo.nCompleteGuide );

            if( iterGPS == m_mapGuidProgressState.end() || iterGPS != m_mapGuidProgressState.end() && (*iterGPS).second != GPS_DONE ){
                continue;
            }
        }

        //특정조건 검사
        if ( kReqInfo.nSpecifiedCondition != -1 
            && CheckSpecifiedCondition( kReqInfo.nSpecifiedCondition ) ) {
            (*iterState).second = GPS_READY;
        }
        else if ( kReqInfo.nSpecifiedCondition == -1 )
        {
            //특정조건 사용하지 않는 가이드 들이 가이드 진행 할 수 있게 구문 체크
            (*iterState).second = GPS_READY;
        }
    }
}

void KGCGuideMgr::UpdateExcuteState()
{
    std::map< int, SRequirementinfo >::iterator iter = m_mapGuideRequirementInfo.begin();

    for( ; iter != m_mapGuideRequirementInfo.end(); ++iter ) {
        std::map< int, int >::iterator iterState = m_mapGuidProgressState.find( (*iter).first );

        // 선행조건이 충족된 가이드만 검사
        if( (*iterState).second == GPS_READY || (*iterState).second == GPS_CAN_START ){
            SRequirementinfo& kReqInfo = (*iter).second;

            // 스테이트 검사
            if( kReqInfo.nGuideState != -1 // -1 : 스테이트 검사하지 않음
                && g_MyD3D->m_pStateMachine->GetState() != kReqInfo.nGuideState ){
                    (*iterState).second = GPS_READY;
                    continue;
            }

            // 게임모드카테고리 검사
            if( kReqInfo.nGameMode != -1 // GC_GMC_INVALID : 게임모드카테고리 검사하지 않음
                && SiKGCRoomManager()->GetGameModeCategory() != kReqInfo.nGameMode ){
                    (*iterState).second = GPS_READY;
                    continue;
            }

            // 게임모드 검사
            if( kReqInfo.nDungeonID != -1 // GC_GM_INVALID : 게임모드 검사하지 않음
                && SiKGCRoomManager()->GetGameMode() != kReqInfo.nDungeonID ){
                    (*iterState).second = GPS_READY;
                    continue;
            }

            // 현재 케릭터 레벨 검사
            GCEXPTYPE biExp = g_kGlobalValue.m_kUserInfo.GetCurrentChar().biExp; // 이거써도 되나.. g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().biExp;
            int nLevel = static_cast<int>(Exp_2_Level( biExp ));

            if( kReqInfo.nRequirementLevelMin != -1 || kReqInfo.nRequirementLevelMax != -1 ) {// -1 : 레벨 검사하지 않음
                if( nLevel < kReqInfo.nRequirementLevelMin || nLevel > kReqInfo.nRequirementLevelMax ){
                    (*iterState).second = GPS_READY;
                    continue;
                }
            }

            // 현재 스테이트, 게임모드에서 실행 가능함
            (*iterState).second = GPS_CAN_START;
        }

    }
}

void KGCGuideMgr::CheckEnableGuide()
{
    // 체크하지 않는 스테이트
    if( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_GAIN ||
        g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT ||
        g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM ||
        g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ||
        g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ||
        g_MyD3D->m_pStateMachine->GetState() == GS_GAME ||
        g_MyD3D->m_pStateMachine->GetState() == GS_INTRO ||
        g_MyD3D->m_pStateMachine->GetState() == GS_LOGIN ||
        g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING ||
        g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL ||
        g_MyD3D->m_pStateMachine->GetState() == GS_REPLAY ||
        g_MyD3D->m_pStateMachine->GetState() == GS_LOGO )
        return;

    //가이드 진행중인지 체크
    if ( g_MyD3D->m_kGuideMgr.IsGuideOngoing() )
        return;

    // 가이드 선행조건 검사
    UpdateGuideState();

    // 가이드 발동해도 되는지 검사
    UpdateExcuteState();

    // 가이드 진행 상태를 체크해서 요구조건이 만족된 가이드가 있는지 찾는다.
    std::map< int, int >::iterator iter = m_mapGuidProgressState.begin();

    for( ; iter != m_mapGuidProgressState.end(); ++iter ){
        if( (*iter).second == GPS_CAN_START ) { // 실행 가능한 가이드가 있다면 시작
            StartGuide( (*iter).first );
            return;
        }
    }
}

void KGCGuideMgr::StartGuide( IN int nGuideType )
{
    if( m_nCurrentGuideType != -1 )
        return;

    m_nCurrentGuideType = nGuideType;
    g_pkUIScene->m_pkGCGuide->ToggleRender( true );
     
    // 현재 가이드 정보를 넘겨주자
    std::map< int, std::vector<SStep> >::iterator iter = m_mapGuideStepInfo.find( m_nCurrentGuideType );
    g_pkUIScene->m_pkGCGuide->SetCurrentGuide( m_nCurrentGuideType, (*iter).second );
}

void KGCGuideMgr::EndGuide()
{
    // 가이드 진행상황 갱신
    m_mapGuidProgressState[m_nCurrentGuideType] = GPS_DONE;

    std::map<int, int>::iterator mit = m_mapPairCompleteGuideType.find(m_nCurrentGuideType);
    if ( mit != m_mapPairCompleteGuideType.end() &&  (*mit).second != -1 ) {
        m_mapGuidProgressState[ (*mit).first ] = GPS_DONE;
        m_mapGuidProgressState[ (*mit).second ] = GPS_DONE;

        SendGuideCompleteInfo( (*mit).first );
        SendGuideCompleteInfo( (*mit).second );
    }
    else {
        SendGuideCompleteInfo( m_nCurrentGuideType );
    }

    // 현재 진행 중인 가이드는 초기화
    m_nCurrentGuideType = -1;
    g_pkUIScene->m_pkGCGuide->ToggleRender( false );

#ifndef __PATH__ 
    m_nTestType = -1;
#endif
}

void KGCGuideMgr::OnEventSystemGuideCompleteInfoNot( IN KEVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT& kRecv_ )
{
    // 가이드 완료 정보
    m_mapGuideComplete = kRecv_.m_mapComplete;

    // 
    std::map< int, SRequirementinfo >::iterator iterGR = m_mapGuideRequirementInfo.begin();
    for( ; iterGR != m_mapGuideRequirementInfo.end(); ++iterGR ){

        // 가이드 진행상태도 갱신 (완료여부만)
        std::map< int, KGuideCompleteInfo >::iterator iterGC = m_mapGuideComplete.find( (*iterGR).first );

        if( iterGC == m_mapGuideComplete.end() ){
            m_mapGuidProgressState[(*iterGR).first] = GPS_BELOW;
        } else {
            KGuideCompleteInfo& kCompleteInfo = (*iterGC).second;

            if( kCompleteInfo.m_nCompleteFlag == KGuideCompleteInfo::FLAG_GUIDE_COMPLETE ){
                m_mapGuidProgressState[(*iterGR).first] = GPS_DONE;
            } else {
                m_mapGuidProgressState[(*iterGR).first] = GPS_BELOW;
            }
        }
    }

}

void KGCGuideMgr::UpdateMissionInfo( IN DWORD dwMissionID )
{
    // 가이드 선행미션 중에 있는지 찾아보자
    std::map< int, SRequirementinfo >::iterator iterGRI = m_mapGuideRequirementInfo.begin();
    for( ; iterGRI != m_mapGuideRequirementInfo.end(); ++iterGRI ){
        std::vector<int>::iterator iterMI = (*iterGRI).second.vecMissionID.begin();

        for(; iterMI != (*iterGRI).second.vecMissionID.end(); ++iterMI ) {
            // 있다면 서버에서 받은 GuideComplete를 갱신해서 선행미션 완료 상태로 만들어줌
            if( (*iterMI) == dwMissionID ) {
                // 서버에서 받은 데이터에서 완료가 아닌것만 미션을 검사해서 갱신 (서버와 동기화 횟수를 줄이기 위해서 클라에서 처리)
                std::map< int, KGuideCompleteInfo >::iterator iterGC = m_mapGuideComplete.find( m_nCurrentGuideType );

                if( iterGC != m_mapGuideComplete.end() ){
                    // 찾았다면 해당 가이드가 완료된 가이드인지 확인
                    if( (*iterGC).second.m_nCompleteFlag != KGuideCompleteInfo::FLAG_GUIDE_COMPLETE ) {
                        (*iterGC).second.m_nCompleteFlag = KGuideCompleteInfo::FLAG_GUIDE_YET;
                    }
                } else {
                    // 없는 경우에는 하나 만들어서 넣어주자
                    KGuideCompleteInfo kInfo;
                    m_mapGuideComplete.insert( std::make_pair( (*iterGRI).first, kInfo ) );
                }
            }
        }
    }
}

#if !defined( __PATH__ )
bool KGCGuideMgr::Test()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_LOGIN 
        || g_MyD3D->m_pStateMachine->GetState() == GS_SERVER
        || g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_GAIN
        || g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT )
        return false;

    if( m_nTestType == -1 )
        return false;
    else{
        StartGuide( m_nTestType );
        return true;
    }

    return false;
}
#endif

bool KGCGuideMgr::CheckSpecifiedCondition( int iSpecifiedCondition )
{
    if ( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT && iSpecifiedCondition == GSC_TUTORIAL_GUIDE_START )
        return true;

    //if ( true ==  g_pkUIScene->m_pkWorldMapContainer->GetSilverLandOpenStateChanged() && iSpecifiedCondition == GSC_DUNGEON_TUTORIAL_GUDIE_START)
    //    return true;

    if( iSpecifiedCondition == GSC_GUIDE_SKILL_TRAINABLE 
        && SiGCSKT()->GetNewSkillNum() > 0 
        && g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() != STATE_PROGRESS_IN_TUTORIAL )
        return true;

    return false;
}

void KGCGuideMgr::SetContinentEffect( bool bEffect )
{
    m_bEffect = bEffect;
}

bool KGCGuideMgr::GetContinentEffect()
{
    return m_bEffect;
}

void KGCGuideMgr::ForcedGuide( int iGuideType )
{
    if( m_mapGuidProgressState.empty() )
        return;

    std::map< int, SRequirementinfo >::iterator iter = m_mapGuideRequirementInfo.find( iGuideType );

    if ( iter == m_mapGuideRequirementInfo.end() )
        return;

    std::map< int, int >::iterator iterState = m_mapGuidProgressState.find( (*iter).first );
    
    if( iterState == m_mapGuidProgressState.end() || (*iterState).second == GPS_DONE )
        return;

    StartGuide(iGuideType);
}

bool KGCGuideMgr::IsCompleteMissioninfo( IN int iGuideType )
{
    // 가이드 선행미션 중에 있는지 찾아보자
    std::map< int, SRequirementinfo >::iterator iterGRI = m_mapGuideRequirementInfo.find(iGuideType);
    std::vector<int> vecMission;
    if ( iterGRI != m_mapGuideRequirementInfo.end() )
         vecMission = (*iterGRI).second.vecMissionID;
    else 
        return false;

    std::vector<int>::iterator vit = vecMission.begin();
    
    for ( ; vit != vecMission.end() ; vit ++)
    {
        if( g_kGlobalValue.m_kUserMission.IsCompleteMission(*vit)  ) {
            std::map< int, KGuideCompleteInfo >::iterator iterGC = m_mapGuideComplete.find( iGuideType );
            if( iterGC != m_mapGuideComplete.end() ) {
                return true;
            }
            else 
                return false;
        }
        else {
            continue;
        }
    }
    return false;
}

bool KGCGuideMgr::CanUseMsgBox()
{
    // 처음(0스텝)부터 미션이나 이이템 획득창을 봐야 하는 경우 예외처리
    if( m_nCurrentGuideType == -1 ||
        m_nCurrentGuideType == 9 ) // GT_TUTORIAL_GUIDE_1

        return true;

    // 가이드를 시작하는 0스텝에서 다른 UI 간섭을 막기위한 조치입니다. 이 후에 뜨는 UI는 괜춘
    if( g_pkUIScene->m_pkGCGuide->GetCurrStep() == 0 )
        return false;

    return true;
}

void KGCGuideMgr::SendGuideCompleteInfo( IN int nGuideType )
{
    Result_ReceiveGuideCompleteReq = INT_MAX;
    KP2P::GetInstance()->Send_SystemGuideCompleteReq( nGuideType );
    g_MyD3D->WaitForServerAck( Result_ReceiveGuideCompleteReq, INT_MAX, 3000, TIME_OUT_VALUE );
}

bool KGCGuideMgr::IsCompleteGuide( int iGuideType )
{
    std::map<int, int>::iterator mit = m_mapPairCompleteGuideType.find(iGuideType);
    if ( mit != m_mapPairCompleteGuideType.end() ) {
        if ( m_mapGuidProgressState[ (*mit).first ] == GPS_DONE )
            return true;
    }
    return false;
}