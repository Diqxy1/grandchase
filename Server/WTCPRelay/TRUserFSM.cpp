#include "TRUserFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KTRUserFSM::" L ## #id,
const wchar_t*  KTRUserFSM::ms_szState[] = { 
#   include "TRUserFSMState_def.h" 
};

const wchar_t*  KTRUserFSM::ms_szInput[] = { 
#   include "TRUserFSMInput_def.h" 
};

KTRUserFSM::KTRUserFSM() : FSMclass( KTRUserFSM::STATE_INIT )
{
    // STATE_INIT
    FSMstate* pState = new FSMstate( STATE_INIT, 1 );
    pState->AddTransition( INPUT_CONNECT,           STATE_CONNECTED );
    AddState( pState );

    // STATE_CONNECTED
    pState = new FSMstate( STATE_CONNECTED, 2 );
    pState->AddTransition( INPUT_START_REG_UID,     STATE_REGISTERING );
    pState->AddTransition( INPUT_EXIT,              STATE_EXIT );
    AddState( pState );

    // STATE_REGISTERING
    pState = new FSMstate( STATE_REGISTERING, 2 );
    pState->AddTransition( INPUT_END_REG_UID,       STATE_REGISTERED );
    pState->AddTransition( INPUT_REG_UID_FAILED,    STATE_REGISTERING );
    AddState( pState );

    // STATE_REGISTERED
    pState = new FSMstate( STATE_REGISTERED, 2 );
    pState->AddTransition( INPUT_DISCONNECT,        STATE_INIT );
    pState->AddTransition( INPUT_EXIT,              STATE_EXIT );
    AddState( pState );

    // STATE_EXIT
    pState = new FSMstate( STATE_EXIT, 0 );
    AddState( pState );

}

KTRUserFSM::~KTRUserFSM()
{
}


const wchar_t* KTRUserFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KTRUserFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}