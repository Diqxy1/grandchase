#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,

enum
{
#endif

_ENUM( EVENT_PAYLETTER_BALANCE,     10 )                // 사용자 잔액 요청
_ENUM( EVENT_PAYLETTER_BUY,         20 )                // 구매 요청
_ENUM( EVENT_PAYLETTER_GIFT,        30 )                // 선물 구매 요청
_ENUM( EVENT_PAYLETTER_BUY_CANCEL,  40 )                // 선물 구매 요청
_ENUM( EVENT_PAYLETTER_ALIVE,       301 )               // keep alive

_ENUM( EVENT_PAYLETTER_ERROR_NOT_01, 100 )        // Short of Cash
_ENUM( EVENT_PAYLETTER_ERROR_NOT_02, 200 )        // Non-Existing User
_ENUM( EVENT_PAYLETTER_ERROR_NOT_03, 300 )        // DB Internal Error
_ENUM( EVENT_PAYLETTER_ERROR_NOT_04, 400 )        // Billing Server Internal Error
_ENUM( EVENT_PAYLETTER_ERROR_NOT_05, 500 )        // Already cancelled Purchase

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif 