#include "stdafx.h"
//

#include ".\kgcnewterm.h"
//

//

//

KGCNewTerm::KGCNewTerm( void )
{
}

KGCNewTerm::~KGCNewTerm( void )
{
}

/************************************************************************/
/* 이벤트 패킷 분석해서 처리하는 함수                                   */
/************************************************************************/
void KGCNewTerm::ProcessEventPacket( int iType )
{
    //  0 : 추천 할 수 있음
    // -1 : 이벤트 기간이 지났습니다
    // -2 : 이미추천

    switch( iType )
    {
        case 0: // 추천 해야 한다
            g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT2 ),
                KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND );
            break;

        case -1: // 휴면 유져 추천 이벤트 기간이 아님..
            break;

        case -2: // 이미 추천했다

            break;
    }
}

void KGCNewTerm::ProcessRecommandAck( int iOk, int iPoint )
{
     // 0;  //  성공
     // 1;  //  추천하는 유저의 포인트 정보가 없음
     // 2;  //  추천받은 유저가 존재하지 않음
     // 3;  //  추천받은 유저와 추천한 유저가 동일함
     // 4;  //  추천받은 유저의 포인트 정보가 없음

    int iGetPoint = iPoint - g_kGlobalValue.m_iGCPoint;

    g_kGlobalValue.m_iGCPoint = iPoint;

    switch( iOk )
    {
        case 0 : //성공
            g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
                g_pkStrLoader->GetReplacedString( STR_ID_NEWTERM_EVENT8, "i", iGetPoint ),
                KGCUIScene::GC_MBOX_USE_EVENT_MSG, 0, 0, true, false );
            break;
        case 3: // 추천받은 유저와 추천한 유저가 동일함
            g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT7 ),
                KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND, 0, 0, true, false );
            break;
        case 1: // 추천하는 유저의 포인트 정보가 없음
        case 2: // 추천받은 유저가 존재하지 않음
        case 4: // 추천받은 유저의 포인트 정보가 없음
        default:
            g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT6 ),
                KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND, 0, 0, true, false );
            break;
    }
}

void KGCNewTerm::ProcessRecommand( int iRecomCount, std::vector< std::pair< char, bool > > vecRewardList )
{
}

///************************************************************************/
///* 이벤트에 사용되는 메시지들을 보여줄 함수들입니다.                    */
///************************************************************************/
//void KGCNewTerm::ShowMsg_NewUserRecommand()	//가입을 축하합니다. 교복세트와 무기가 지급되었습니다. 추천할 넷마블 ID를 입력해 주세요.
//{
//	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT2 ),
//                            KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND );
//    // 확인 눌렀을 경우
//    // KP2P::GetInstance()->Send_RecommendKorUser( strLogin )
//    // 호출
//    // Procedure.cpp
//}
//void KGCNewTerm::ShowMsg_SleepUserRecommand()	//축하합니다. 교복세트와 무기가 지급되었습니다. 추천할 넷마블 ID를 입력해 주세요.
//{
//	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT3 ),
//                            KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND );
//    // 확인 눌렀을 경우
//    // KP2P::GetInstance()->Send_RecommendKorUser( strLogin )
//    // 호출
//}
//
//void KGCNewTerm::ShowMsg_Over_30Days()			// 무기 사용 남은 기간이 30일을 초과하여 더 이상 지급 받을 수 없습니다.
//{
//    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, 
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT4 ),
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT5 ) );
//}
//
//// 상구
//void KGCNewTerm::ShowMsg_Recommanded( std::wstring strFromWho_ )	// **님으로 부터 추천 받으셨습니다.
//{
//	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                            g_pkStrLoader->GetReplacedString( STR_ID_NEWTERM_EVENT9, "S", strFromWho_.c_str() ),
//                            KGCUIScene::GC_MBOX_USE_EVENT_MSG );
//}
//
//void KGCNewTerm::ShowMsg_RecommandFail()		// 그랜드 체이스에 존재하지 않는 유저 ID 입니다.
//{
//	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT6 ),
//                            KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND );
//
//    // 제대로 될때까지 요청해야 하지 않을까?
//}
//
//void KGCNewTerm::ShowMsg_RecommandYous()		// 자신의 ID를 추천 할 수 없습니다.
//{
//	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                            g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT7 ),
//                            KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND );
//
//    // 제대로 될때까지 요청해야 하지 않을까?
//}
//
//void KGCNewTerm::ShowMsg_SuccessRecommand()     // 추천을 성공하셨습니다.
//{
//    {
//    	g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//                                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//                                g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT8 ),
//                                KGCUIScene::GC_MBOX_USE_EVENT_MSG );
//    }
//}
//
//void KGCNewTerm::ShowMsg_GetItem()	// 가입을 축하합니다. *^0^*\n교복세트와 무기가 지급되었습니다.\n[장비] 버튼을 눌러 확인하세요.
//{
//    g_pkUIScene->MessageBox( KGCUIScene::GC_EVENTBOX,
//        g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT1 ),
//        g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT2 ),
//        KGCUIScene::GC_MBOX_USE_EVENT_MSG );
//}