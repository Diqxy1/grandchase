#ifdef __NEVER_DEFINED_MACRO__
#ifdef _ENUM
#undef _ENUM
#endif
#define _ENUM( VAL ) VAL,
enum ECONNECT_STATE
{
#endif
    _ENUM( E_CONNECT_WAIT )
    _ENUM( E_CONNECT_P2P )
    _ENUM( E_CONNECT_RELAYSERVER )
    _ENUM( E_CONNECT_FINISH )
#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif