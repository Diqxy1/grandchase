#ifndef _ENUM
#define _ENUM(VAL) VAL,
#define ___ENUM___
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( STATE_CLOSE = 1 )    // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( STATE_WAITING )
_ENUM( STATE_FULL )
_ENUM( STATE_PLAYING )      // 4
_ENUM( STATE_RESULT )       // 5

_ENUM( STATE_SENTINEL )     // 6

#ifdef ___ENUM___
};
#undef ___ENUM___
#undef _ENUM
#endif
