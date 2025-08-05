#ifndef _ENUM
#define _ENUM(VAL) VAL,
#define ___ENUM___
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( STATE_INIT = 1 )         // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( STATE_CONNECTED = 2 )    // 050203. 고정적인 상태의 값을 통일한다.
_ENUM( STATE_EXIT = 3 )         // 050203.
_ENUM( STATE_REGISTERING )
_ENUM( STATE_REGISTERED )

_ENUM( STATE_SENTINEL )

#ifdef ___ENUM___
};
#undef ___ENUM___
#undef _ENUM
#endif