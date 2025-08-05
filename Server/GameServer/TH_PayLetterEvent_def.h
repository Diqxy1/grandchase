#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,

enum
{
#endif

_ENUM( EVENT_TH_GET_BALANCE ,           10 )        // 캐시 잔액 조회
_ENUM( EVENT_TH_INSERT_BONUSCASH ,      11 )        // 보너스캐시 지급
_ENUM( EVENT_TH_PURCHASE_ITEM ,         20 )        // 아이템 구매(직접구매)
_ENUM( EVENT_TH_GIFT_ITEM ,             21 )        // 아이템 구매(선물하기)
_ENUM( EVENT_TH_CANCEL_PURCHASE ,       22 )        // 아이템 구매 취소
_ENUM( EVENT_TH_REG_COUPON ,            30 )        // 쿠폰 등록
_ENUM( EVENT_TH_CANCEL_COUPON ,         31 )        // 쿠폰 등록 취소
_ENUM( EVENT_TH_KEEP_ALIVE ,            50 )        // GTX 서버 Health Check

_ENUM( TH_BILL_OK ,                     0 )         // 성공
_ENUM( TH_ERR_SP_EXEC ,                 450 )       // Stored Procedure 실행 오류입니다.
_ENUM( TH_ERR_DB_CONNECT ,              452 )       // 데이터베이스에 연결할 수 없습니다.
_ENUM( TH_ERR_PARAM ,                   1001 )      // 파라메터 오류입니다.
_ENUM( TH_ERR_PACKET_OP ,               1003 )      // 정의되지 않은 전문 명령어 요청입니다.
_ENUM( TH_ERR_TIME_OUT ,                1004 )      // Socket Timeout 이 발생하였습니다.
_ENUM( TH_ERR_GTX_SUSPEND ,             1006 )      // GTX서버가 일시 중지 되었습니다.
_ENUM( TH_ERR_IP ,                      1007 )      // 허용되지 않은 IP 요청입니다.
_ENUM( TH_ERR_GTX_DOWN ,                1010 )      // GTX서버가 중지중입니다. 클라이언트의 요청을 수락할 수 없습니다.
_ENUM( TH_ERR_EXCEED_REQ ,              1011 )      // 요청된 클라이언트 수가 GTX서버의 처리 용량을 초과하였습니다.
_ENUM( TH_ERR_GTX_EXCEPTION ,           1012 )      // GTX 서버 내부 Exception 오류입니다.
_ENUM( TH_ERR_PACKET_RULE ,             1013 )      // 요청된 전문이 정의 규약에 벗어납니다.
_ENUM( TH_ERR_DB_ECEPTION ,             2000 )      // SP 내부 Database Exception 오류입니다.

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif 













