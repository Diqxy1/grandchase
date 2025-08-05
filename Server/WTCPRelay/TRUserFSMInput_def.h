#ifndef _ENUM
#define _ENUM(VAL) VAL,
#define ___ENUM___
enum
{
#endif

_ENUM( INPUT_CONNECT = 0 )

_ENUM( INPUT_START_REG_UID )
_ENUM( INPUT_REG_UID_FAILED )
_ENUM( INPUT_END_REG_UID )
_ENUM( INPUT_DISCONNECT )
_ENUM( INPUT_EXIT )             // 5

#ifdef ___ENUM___
};
#undef ___ENUM___
#undef _ENUM
#endif