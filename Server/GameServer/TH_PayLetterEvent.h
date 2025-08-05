#pragma once

#include <map>
#include <windows.h>
#include "PrimitiveTypedef.h"

#define MAX_TH_BILLING_PACKET_SIZE     (1024)

#pragma pack( push, 1 )

struct KTH_BILL_HEADER
{
    WORD    ReqLen;
    WORD    ReqType;
    DWORD   ReqKey;
    KTH_BILL_HEADER()
        :ReqLen(0),ReqType(0),ReqKey(0){}
    void SwapByteOrder();
};

struct _BILL_PACK_GETBALANCE  // 1)  캐시 잔액 조회
{
    char    UserID[50+1];
    DWORD   RealCash;
    DWORD   BonusCash;
    DWORD   GameCash;
    WORD    RetCode;
    _BILL_PACK_GETBALANCE()
        :RealCash(0),BonusCash(0),GameCash(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_INSBONUSCASH // 2)  보너스 캐시 지급
{
    char    UserID[50+1];
    DWORD   ClientIP;
    WORD    ClientPort;
    DWORD   CashAmt;
    DWORD   RealCash;
    DWORD   BonusCash;
    DWORD   GameCash;
    WORD    RetCode;
    _BILL_PACK_INSBONUSCASH()
        :ClientIP(0),ClientPort(0),CashAmt(0)
        ,RealCash(0),BonusCash(0),GameCash(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_PURCHASEITEM // 3)  아이템 구매(직접구매)
{
    DWORD       UserNo;
    DWORD       ClientIP;
    WORD        ClientPort;
    DWORD       ReqChargeCashAmt;
    WORD        ItemCategory;
    GCITEMID    ItemID;
    WORD        ItemCnt;
    DWORD       ItemUnitPrice;
    WORD        ItemType;
    WORD        GameServerNo;
    DWORD       WorldNo;
    char        UserID[50+1];
    char        CharacterID[30+1];
    char        ItemName[50+1];
    char        StatProperty1[50+1];
    char        StatProperty2[50+1];
    char        StatProperty3[50+1];
    char        Location[1+1];
    __int64     GChargeNo;
    DWORD       RealCash;
    DWORD       BonusCash;
    DWORD       GameCash;
    DWORD       ChargedCashAmt;
    __int64     ChargeNo;
    WORD        RetCode;
    _BILL_PACK_PURCHASEITEM()
        :UserNo(0),ClientIP(0),ClientPort(0),ReqChargeCashAmt(0),
        ItemCategory(0),ItemID(0),ItemCnt(0),ItemUnitPrice(0),
        ItemType(0),GameServerNo(0),WorldNo(0),GChargeNo(0),
        RealCash(0),BonusCash(0),GameCash(0),ChargedCashAmt(0),
        ChargeNo(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
        ::memset( CharacterID, 0, sizeof(CharacterID) );
        ::memset( ItemName, 0, sizeof(ItemName) );
        ::memset( StatProperty1, 0, sizeof(StatProperty1) );
        ::memset( StatProperty2, 0, sizeof(StatProperty2) );
        ::memset( StatProperty3, 0, sizeof(StatProperty3) );
        ::memset( Location, 0, sizeof(Location) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_GIFTITEM  //4)  아이템 구매(선물하기)
{
    DWORD       UserNo;
    DWORD       RUserNo;
    DWORD       ClientIP;
    WORD        ClientPort;
    DWORD       ReqChargeCashAmt;
    WORD        ItemCategory;
    GCITEMID       ItemID;
    WORD        ItemCnt;
    DWORD       ItemUnitPrice;
    WORD        ItemType;
    WORD        GameServerNo;
    DWORD       WorldNo;
    char        UserID[50+1];
    char        CharacterID[30+1];
    char        RUserID[50+1];
    char        RCharacterID[30+1];
    char        ItemName[50+1];
    char        StatProperty1[50+1];
    char        StatProperty2[50+1];
    char        StatProperty3[50+1];
    char        Location[1+1];
    DWORD       RealCash;
    DWORD       BonusCash;
    DWORD       GameCash;
    DWORD       ChargedCashAmt;
    __int64     ChargeNo;
    WORD        RetCode;
    _BILL_PACK_GIFTITEM()
        :UserNo(0),RUserNo(0),ClientIP(0),
        ClientPort(0),ReqChargeCashAmt(0),ItemCategory(0),
        ItemID(0),ItemCnt(0),ItemUnitPrice(0),ItemType(0),
        GameServerNo(0),WorldNo(0),RealCash(0),BonusCash(0),
        GameCash(0),ChargedCashAmt(0),ChargeNo(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
        ::memset( CharacterID, 0, sizeof(CharacterID) );
        ::memset( RUserID, 0, sizeof(RUserID) );
        ::memset( RCharacterID, 0, sizeof(RCharacterID) );
        ::memset( ItemName, 0, sizeof(ItemName) );
        ::memset( StatProperty1, 0, sizeof(StatProperty1) );
        ::memset( StatProperty2, 0, sizeof(StatProperty2) );
        ::memset( StatProperty3, 0, sizeof(StatProperty3) );
        ::memset( Location, 0, sizeof(Location) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_CNLPURCHASE // 5)  아이템구매 취소(직접구매, 선물하기 공통)
{
    char        UserID[50+1];
    __int64     ChargeNo;
    DWORD       RealCash;
    DWORD       BonusCash;
    DWORD       GameCash;
    DWORD       CanceledCashAmt;
    WORD        RetCode;
    _BILL_PACK_CNLPURCHASE()
        :ChargeNo(0),RealCash(0),BonusCash(0),
        GameCash(0),CanceledCashAmt(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_REGCOUPON // 6)  쿠폰등록
{
    DWORD       UserNo;
    DWORD       ClientIP;
    WORD        ClientPort;
    WORD        GameServerNo;
    DWORD       WorldNo;
    char        UserID[50+1];
    char        CharacterID[30+1];
    char        CouponNo[19+1];
    char        StatProperty1[50+1];
    char        StatProperty2[50+1];
    char        StatProperty3[50+1];
    char        Location[1+1];
    DWORD       RealCash;
    DWORD       BonusCash;
    DWORD       GameCash;
    GCITEMID       ItemID;
    WORD        ItemCnt;
    __int64     ChargeNo;
    WORD        RetCode;
    _BILL_PACK_REGCOUPON()
        :UserNo(0),ClientIP(0),ClientPort(0),
        GameServerNo(0),WorldNo(0),RealCash(0),
        BonusCash(0),GameCash(0),ItemID(0),
        ItemCnt(0),ChargeNo(0),RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
        ::memset( CharacterID, 0, sizeof(CharacterID) );
        ::memset( CouponNo, 0, sizeof(CouponNo) );
        ::memset( StatProperty1, 0, sizeof(StatProperty1) );
        ::memset( StatProperty2, 0, sizeof(StatProperty2) );
        ::memset( StatProperty3, 0, sizeof(StatProperty3) );
        ::memset( Location, 0, sizeof(Location) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_CNLCOUPON // 7)  쿠폰등록 취소
{
    char        UserID[50+1];
    char        CouponNo[19+1];
    WORD        RetCode;
    _BILL_PACK_CNLCOUPON():RetCode(0)
    {
        ::memset( UserID, 0, sizeof(UserID) );
        ::memset( CouponNo, 0, sizeof(CouponNo) );
    }
    void SwapByteOrder();
};

struct _BILL_PACK_HEALTH_CHECK // 8)  GTX 서버 Health Check
{
    WORD        RetCode;
    _BILL_PACK_HEALTH_CHECK():RetCode(0){}
    void SwapByteOrder();
};

#pragma pack( pop )

class KTHPayLetterEvent
{
public:
#   undef   _ENUM
#   define  _ENUM( name, id ) name = id,
    enum ENUM_EVENT_ID {
#       include "TH_PayLetterEvent_def.h"
    };

    static std::map< int, std::wstring > ms_mapEventIDString;

public:
    KTHPayLetterEvent(void);
    virtual ~KTHPayLetterEvent(void);

    static void Init();
    void Reset();
    static std::wstring GetEventIDString( const unsigned short usID );

    void WriteFromGTX( BYTE* abytePLBuffer, unsigned short usSize );
    void ReadHeader( OUT KTH_BILL_HEADER& kHeader ) const;
    template<typename T> void ReadPacket( OUT T& packet ) const;
    void WriteHeader( IN const KTH_BILL_HEADER& kHeader );
    template<typename T> void WritePacket( IN const T& packet );
    void DumpBuffer() const;
    const BYTE* GetBuffer() const { return m_abyteNBuffer; };

protected:
    BYTE    m_abyteNBuffer[MAX_TH_BILLING_PACKET_SIZE];// 이 버퍼는 Big Endian 형식만 들어 간다.
};

template<typename T>
void KTHPayLetterEvent::ReadPacket( OUT T& packet_ ) const
{
    ::memcpy( &packet_, m_abyteNBuffer + sizeof(KTH_BILL_HEADER), sizeof( T ) );
    packet_.SwapByteOrder();
}

template<typename T>
void KTHPayLetterEvent::WritePacket( IN const T& packet_ )
{
    T tmpPacket = packet_;
    tmpPacket.SwapByteOrder();
    ::memcpy( m_abyteNBuffer + sizeof(KTH_BILL_HEADER), &tmpPacket, sizeof( T ) );
}
