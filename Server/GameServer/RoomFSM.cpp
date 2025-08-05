#include "RoomFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KRoomFSM::" L ## #id,
const wchar_t*  KRoomFSM::ms_szState[] = { 
#   include "RoomFSMState_def.h" 
};

const wchar_t*  KRoomFSM::ms_szInput[] = { 
#   include "RoomFSMInput_def.h" 
};

KRoomFSM::KRoomFSM() : FSMclass( STATE_CLOSE )
{
    // STATE_CLOSE
    FSMstate* pState = new FSMstate( STATE_CLOSE, 1 );
    pState->AddTransition( INPUT_OPEN,                  STATE_WAITING );
    if( false == AddState( pState ) ) { 
        delete pState;
    }

    // STATE_WAITING
    pState = new FSMstate( STATE_WAITING, 3 );
    pState->AddTransition( INPUT_CLOSE,                 STATE_CLOSE );
    pState->AddTransition( INPUT_FULL,                  STATE_FULL );
    pState->AddTransition( INPUT_START_GAME,            STATE_PLAYING );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_FULL
    pState = new FSMstate( STATE_FULL, 3 );
    pState->AddTransition( INPUT_WAIT,                  STATE_WAITING );
    pState->AddTransition( INPUT_START_GAME,            STATE_PLAYING );
    pState->AddTransition( INPUT_CLOSE,                 STATE_CLOSE );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_PLAYING
    pState = new FSMstate( STATE_PLAYING, 2 );
    pState->AddTransition( INPUT_END_GAME,              STATE_RESULT );
    //pState->AddTransition( INPUT_FULL,                  STATE_FULL );
    pState->AddTransition( INPUT_CLOSE,                 STATE_CLOSE );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_RESULT
    pState = new FSMstate( STATE_RESULT, 3 );
    pState->AddTransition( INPUT_WAIT,                  STATE_WAITING );
    pState->AddTransition( INPUT_FULL,                  STATE_FULL );
    pState->AddTransition( INPUT_CLOSE,                 STATE_CLOSE );
    if( false == AddState( pState ) ) {
        delete pState;
    }
}

KRoomFSM::~KRoomFSM()
{
}


const wchar_t* KRoomFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KRoomFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}