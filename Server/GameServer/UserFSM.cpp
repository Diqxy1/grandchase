#include "UserFSM.h"
#include "FSM/FSMstate.h"

#undef _ENUM
#define _ENUM( id ) L"KUserFSM::" L ## #id,
const wchar_t*  KUserFSM::ms_szState[] = { 
#   include "UserFSMState_def.h" 
};

const wchar_t*  KUserFSM::ms_szInput[] = { 
#   include "UserFSMInput_def.h" 
};

KUserFSM::KUserFSM() : FSMclass( KUserFSM::STATE_INIT )
{
    // STATE_INIT
    FSMstate* pState = new FSMstate( STATE_INIT, 2 );
    pState->AddTransition( INPUT_CONNECT,               STATE_CONNECTED );
    pState->AddTransition( INPUT_EXIT_GAME_ACK,         STATE_EXIT );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_CONNECTED
    pState = new FSMstate( STATE_CONNECTED, 2 );
    pState->AddTransition( INPUT_VERIFICATION_REQ,      STATE_LOGINING );
    pState->AddTransition( INPUT_EXIT_GAME_ACK,         STATE_EXIT );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_LOGINING
    pState = new FSMstate( STATE_LOGINING, 2 );
    pState->AddTransition( INPUT_VERIFICATION_OK,       STATE_CHANNELLOBBY );
    pState->AddTransition( INPUT_VERIFICATION_FAIL,     STATE_CONNECTED );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_CHANNELLOBBY
    pState = new FSMstate( STATE_CHANNELLOBBY, 5 );
    pState->AddTransition( INPUT_TO_CHANNEL,            STATE_CHANNEL );
    pState->AddTransition( INPUT_EXIT_GAME_ACK,         STATE_EXIT );
    pState->AddTransition( INPUT_JOIN_ROOM,             STATE_ROOM );
    pState->AddTransition( INPUT_TO_SQUARE_JOIN,        STATE_SQUARE_JOIN );
    pState->AddTransition( INPUT_TO_AGIT_JOIN,          STATE_AGIT_JOIN );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_CHANNEL
    pState = new FSMstate( STATE_CHANNEL, 3 );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY );
    pState->AddTransition( INPUT_JOIN_ROOM,             STATE_ROOM );
    pState->AddTransition( INPUT_JOIN_ROOM_GUILD,       STATE_GUILD_ROOM ); // 길드룸 입장
    //pState->AddTransition( INPUT_TO_AGIT_JOIN,          STATE_AGIT_JOIN );
    //pState->AddTransition( INPUT_JOIN_ROOM,             STATE_JOIN );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    ////STATE_JOIN
    //pState = new FSMstate( STATE_JOIN, 2 );
    //pState->AddTransition( INPUT_TO_JOINROOM_COLPLETE,  STATE_ROOM );
    //pState->AddTransition( INPUT_TO_CHANNEL,            STATE_CHANNEL );
    //AddState( pState );

    // STATE_ROOM
    pState = new FSMstate( STATE_ROOM, 3 );
    pState->AddTransition( INPUT_TO_CHANNEL,            STATE_CHANNEL );
    pState->AddTransition( INPUT_START_GAME_OK,         STATE_PLAYING );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY );
    //pState->AddTransition( INPUT_TO_AGIT_JOIN,          STATE_AGIT_JOIN );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_PLAYING
    pState = new FSMstate( STATE_PLAYING, 2 );
    pState->AddTransition( INPUT_END_GAME_OK,           STATE_ROOM );
    pState->AddTransition( INPUT_TO_CHANNEL,            STATE_CHANNEL );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_SQUARE_JOIN
    pState = new FSMstate( STATE_SQUARE_JOIN, 2 );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY );
    pState->AddTransition( INPUT_ENTER_SQUARE,          STATE_SQUARE );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_SQUARE
    pState = new FSMstate( STATE_SQUARE, 3 ); // 상태 이름, 상태 수
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY );
    pState->AddTransition( INPUT_START_MINIGAME,        STATE_MINIGAME_PLAYING ); // 광장에서 미니게임 플레이로 바로 넘어가기로 했는데...
    pState->AddTransition( INPUT_JOIN_ROOM_GUILD,       STATE_GUILD_ROOM ); // 광장에서 길드룸 입장
    //pState->AddTransition( INPUT_TO_AGIT_JOIN,          STATE_AGIT_JOIN );
    if( false == AddState( pState ) ) {
        delete pState;
    }


    pState = new FSMstate( STATE_MINIGAME, 3 ); // 미니게임 로비상태
    pState->AddTransition( INPUT_TO_SQUARE_JOIN,        STATE_SQUARE_JOIN ); // 플레이중에 나가면 광장으로 감
    //pState->AddTransition( INPUT_ENTER_SQUARE,          STATE_SQUARE ); // 광장으로 돌아가기
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY ); // 상태가 꼬였을 때 
    pState->AddTransition( INPUT_START_MINIGAME,        STATE_MINIGAME_PLAYING ); // 미니게임 플레이
    if( false == AddState( pState ) ) {
        delete pState;
    }

    pState = new FSMstate( STATE_MINIGAME_PLAYING, 3 ); // 미니게임 플레이중
    pState->AddTransition( INPUT_TO_MINIGAME,           STATE_MINIGAME ); // 미니게임 로비
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,       STATE_CHANNELLOBBY ); // 상태가 잘못되었을 때 채널로비로 감
    pState->AddTransition( INPUT_TO_SQUARE_JOIN,        STATE_SQUARE_JOIN ); // 플레이중에 나가면 광장으로 감
    //pState->AddTransition( INPUT_ENTER_SQUARE,          STATE_SQUARE ); 
    if( false == AddState( pState ) ) {
        delete pState;
    }

    pState = new FSMstate( STATE_GUILD_ROOM, 3 );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY,  STATE_CHANNELLOBBY ); // 상태가 비정상인 경우 채널로비로 이동
    pState->AddTransition( INPUT_TO_CHANNEL,       STATE_CHANNEL );
    pState->AddTransition( INPUT_START_GAME_GUILD, STATE_GUILD_PLAYING );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    pState = new FSMstate( STATE_GUILD_PLAYING, 2 );
    pState->AddTransition( INPUT_END_GAME_GUILD, STATE_GUILD_ROOM );
    pState->AddTransition( INPUT_TO_CHANNEL,     STATE_CHANNEL );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    pState = new FSMstate( STATE_AGIT_JOIN, 4 );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY, STATE_CHANNELLOBBY ); // 상태이상시, 채널로비로
    pState->AddTransition( INPUT_TO_CHANNEL,      STATE_CHANNEL );
    pState->AddTransition( INPUT_TO_AGIT_JOIN,    STATE_AGIT_JOIN );
    pState->AddTransition( INPUT_ENTER_AGIT,      STATE_AGIT );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    pState = new FSMstate( STATE_AGIT, 4 );
    pState->AddTransition( INPUT_TO_CHANNELLOBBY, STATE_CHANNELLOBBY ); // 상태이상시, 채널로비로
    pState->AddTransition( INPUT_TO_CHANNEL,      STATE_CHANNEL );
    pState->AddTransition( INPUT_TO_AGIT_JOIN,    STATE_AGIT_JOIN );
    pState->AddTransition( INPUT_ENTER_AGIT,      STATE_AGIT );
    if( false == AddState( pState ) ) {
        delete pState;
    }

    // STATE_EXIT
    pState = new FSMstate( STATE_EXIT, 0 );
    if( false == AddState( pState ) ) {
        delete pState;
    }
}

KUserFSM::~KUserFSM()
{
}


const wchar_t* KUserFSM::GetStateIDString(int iStateID_) const
{
    return ms_szState[iStateID_];
}


const wchar_t* KUserFSM::GetInputIDString(int iInputID_) const
{
    return ms_szInput[iInputID_];
}
