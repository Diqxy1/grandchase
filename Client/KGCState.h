// KGCState.h: interface for the KGCState class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCSTATE_H__8317CD99_4FBC_4EB2_92D6_BD23A4AB9BE4__INCLUDED_)
//#define AFX_KGCSTATE_H__8317CD99_4FBC_4EB2_92D6_BD23A4AB9BE4__INCLUDED_

#ifndef _KGCSTATE_H_
#define _KGCSTATE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include <vector>

#ifdef _ENUM
    #undef _ENUM
    #include "GCStateDef.h"
#else
    #include "GCStateDef.h"
#endif

/*
  필히 함수 선언 순서를 지켜줘야 된다
  KGCStateMachine클래스에서 virtual table을 참조하기 때문

  0번째 virtual 함수 : 소멸자
  1번째 virtual 함수 : FrameMove
  2번째 virtual 함수 : Render
*/

#define ON_SETUP_FSM( eTarget, eError )	m_vecVaildNextState.push_back( std::make_pair( eTarget, eError ) )

class KGCState
{
protected:
	enum AdminCommandType {
		PRINT_CURRENT_STATE_USER = 0,
	};

	bool m_bStateLock;
	GAME_STATE m_eStateForFail;
	std::vector< std::pair< GAME_STATE, GAME_STATE> > m_vecVaildNextState;
	//std::vector< std::pair< GAME_STATE, GAME_STATE>, bool > m_mapVaildNextState;

public:
	KGCState();
	virtual ~KGCState();

	// 아래의 순서대로 흘러간다.
	// 1. OnBeforeStateChange
	// 2. 실패시 OnDestroyBeforeStateChange
	// 3. 성공시 OnInit
	// 4. FrameMove + Render
	// 5. 다른 스테이트로 전환시 OnDestroy
	// Destroy들을 pure virtual로 만드는 이유는 추후에 State추가시에 작업자가 고려하도록 하기 위함이다.

	virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ ) = 0;			//스테이트가 바뀌기 이전에 초기화 + 스테이트 전환에 대한 검증.
	virtual bool OnDestroyBeforeStateChange() = 0;							

	virtual bool OnInit() = 0;												//스테이트가 바뀌고 난 후 초기화 해주는 부분입니다.	
	virtual bool FrameMove(float fElapsedTime) = 0;							//프레임마다 호출 됩니다.
	virtual bool Render() = 0;												//그려줍니다.
	virtual void OnDestroy() = 0;											//스테이트가 다음으로 넘어갈 때 호출됩니다. 다음 스테이트의 OnInitBeforeStateChange() 함수 성공시 호출합니다.

	virtual void OnSetupFSM() = 0;											//이동 가능한 스테이트들을 셋팅하고, m_eStateForFail 이 변수에 실패했을 때 이동할 State도 셋팅해 주세요.
	virtual bool OnCheckNextState( GAME_STATE eNextState_ );				//스테이트 변환시에 다음스테이트로 갈 수 있는지를 검사한다.
	GAME_STATE	 GetStateForFail( GAME_STATE  eTargetState_, bool bComplete_ = false );						//실패했을 때 가야할 State를 알려줍니다.
	
	virtual void Process_Admin_Command( AdminCommandType _ACT ) {}

    // 게임 끝나고 나오는 메세지 박스들
    virtual void ProcessNotifyMessage();    


	//==========================================================================================
	// 필요성 검토 필요.
	//현재 상태에서 다른 상태로 넘어가지 못하도록 잠글때 사용합니다. ( 일종의 Critical Section : FrameMove에서 차근차근 수행해야할 것들이 있는데 중간에 빠져 나가면 안될때...) 하지만 위험한 코드.
	virtual bool GetStateLock()	{ return m_bStateLock; }
	virtual void SetStateLock( bool bLock_ ) { m_bStateLock = bLock_; }

    bool BeginScene();
    void EndScene();
};

#endif // _KGCSTATE_H_
