#pragma once

//#include <string>

/************************************************************************/
/* 일회용으로 쓸 녀석이라서 하드하게 합니다.                            */
/************************************************************************/
class KGCNewTerm
{
public:
    KGCNewTerm(void);
    ~KGCNewTerm(void);

    void ProcessEventPacket(  int iType );
    void ProcessRecommandAck( int iOk, int iPoint );
    void ProcessRecommand( int iRecomCount, std::vector< std::pair< char, bool > > vecRewardList );

    ///************************************************************************/
    ///* 이벤트에 사용되는 메시지들을 보여줄 함수들입니다.                    */
    ///************************************************************************/
    //void ShowMsg_NewUserRecommand();	//가입을 축하합니다. 교복세트와 무기가 지급되었습니다. 추천할 넷마블 ID를 입력해 주세요.
    //void ShowMsg_SleepUserRecommand();	//축하합니다. 교복세트와 무기가 지급되었습니다. 추천할 넷마블 ID를 입력해 주세요.
    //void ShowMsg_Over_30Days();			// 무기 사용 남은 기간이 30일을 초과하여 더 이상 지급 받을 수 없습니다.
    //void ShowMsg_Recommanded( std::wstring strFromWho_ );	// **님으로 부터 추천 받으셨습니다.
    //void ShowMsg_RecommandFail();		// 그랜드 체이스에 존재하지 않는 유저 ID 입니다.
    //void ShowMsg_RecommandYous();		// 자신의 ID를 추천 할 수 없습니다.
    //void ShowMsg_SuccessRecommand();	// 추천을 성공했습니다.
    //void ShowMsg_GetItem();             // 가입을 축하합니다. *^0^*\n교복세트와 무기가 지급되었습니다.\n[장비] 버튼을 눌러 확인하세요.
};
