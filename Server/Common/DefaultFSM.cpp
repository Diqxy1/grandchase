#include "DefaultFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KCenterFSM::" L ## #id,
const wchar_t*  KDefaultFSM::ms_szState[] = {
#   include "DefaultFSMState_def.h"
};

const wchar_t*  KDefaultFSM::ms_szInput[] = {
#   include "DefaultFSMInput_def.h"
};

KDefaultFSM::KDefaultFSM() : FSMclass(KDefaultFSM::STATE_INIT)
{
	// STATE_INIT
	FSMstate* pState = new FSMstate(STATE_INIT, 1);
	pState->AddTransition(INPUT_CONNECT, STATE_CONNECTED);
	if (false == AddState(pState)) {
		delete pState;
	}

	// STATE_CONNECTED
	pState = new FSMstate(STATE_CONNECTED, 2);
	pState->AddTransition(INPUT_VERIFICATION_OK, STATE_LOGINED);
	pState->AddTransition(INPUT_EXIT_GAME, STATE_EXIT);
	if (false == AddState(pState)) {
		delete pState;
	}

	// STATE_LOGINED
	pState = new FSMstate(STATE_LOGINED, 1);
	pState->AddTransition(INPUT_EXIT_GAME, STATE_EXIT);
	if (false == AddState(pState)) {
		delete pState;
	}

	// STATE_EXIT
	pState = new FSMstate(STATE_EXIT, 1);
	pState->AddTransition(INPUT_CONNECT, STATE_CONNECTED);
	if (false == AddState(pState)) {
		delete pState;
	}
}

KDefaultFSM::~KDefaultFSM()
{
}

const wchar_t* KDefaultFSM::GetStateIDString(int iStateID_) const
{
	return ms_szState[iStateID_];
}

const wchar_t* KDefaultFSM::GetInputIDString(int iInputID_) const
{
	return ms_szInput[iInputID_];
}