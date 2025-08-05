#ifndef _ENUM
#define _ENUM(VAL) VAL,
#define ___ENUM___
enum
{
#endif

_ENUM( INPUT_OPEN = 0 )

_ENUM( INPUT_CLOSE )
_ENUM( INPUT_FULL )
_ENUM( INPUT_WAIT )
_ENUM( INPUT_START_GAME )
_ENUM( INPUT_END_GAME )         // 5

#ifdef ___ENUM___
};
#undef ___ENUM___
#undef _ENUM
#endif
