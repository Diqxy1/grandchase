#include "TH_PayLetterEvent.h"
#include <dbg/dbg.hpp>
#include <KncUtil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

std::map< int, std::wstring > KTHPayLetterEvent::ms_mapEventIDString;

void KTH_BILL_HEADER::SwapByteOrder()
{
    KncUtil::SwapByteOrder( ReqLen );
    KncUtil::SwapByteOrder( ReqType );
    KncUtil::SwapByteOrder( ReqKey );
}

void _BILL_PACK_GETBALANCE::SwapByteOrder()
{
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( RetCode );

}

void _BILL_PACK_INSBONUSCASH::SwapByteOrder()
{
    KncUtil::SwapByteOrder( ClientIP );
    KncUtil::SwapByteOrder( ClientPort );
    KncUtil::SwapByteOrder( CashAmt );
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( RetCode );

}

void _BILL_PACK_PURCHASEITEM::SwapByteOrder()
{
    KncUtil::SwapByteOrder( UserNo );
    KncUtil::SwapByteOrder( ClientIP );
    KncUtil::SwapByteOrder( ClientPort );
    KncUtil::SwapByteOrder( ReqChargeCashAmt );
    KncUtil::SwapByteOrder( ItemCategory );
    KncUtil::SwapByteOrder( ItemID );
    KncUtil::SwapByteOrder( ItemCnt );
    KncUtil::SwapByteOrder( ItemUnitPrice );
    KncUtil::SwapByteOrder( ItemType );
    KncUtil::SwapByteOrder( GameServerNo );
    KncUtil::SwapByteOrder( WorldNo );
    KncUtil::SwapByteOrder( GChargeNo );
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( ChargedCashAmt );
    KncUtil::SwapByteOrder( ChargeNo );
    KncUtil::SwapByteOrder( RetCode );
}

void _BILL_PACK_GIFTITEM::SwapByteOrder()
{
    KncUtil::SwapByteOrder( UserNo );
    KncUtil::SwapByteOrder( RUserNo );
    KncUtil::SwapByteOrder( ClientIP );
    KncUtil::SwapByteOrder( ClientPort );
    KncUtil::SwapByteOrder( ReqChargeCashAmt );
    KncUtil::SwapByteOrder( ItemCategory );
    KncUtil::SwapByteOrder( ItemID );
    KncUtil::SwapByteOrder( ItemCnt );
    KncUtil::SwapByteOrder( ItemUnitPrice );
    KncUtil::SwapByteOrder( ItemType );
    KncUtil::SwapByteOrder( GameServerNo );
    KncUtil::SwapByteOrder( WorldNo );
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( ChargedCashAmt );
    KncUtil::SwapByteOrder( ChargeNo );
    KncUtil::SwapByteOrder( RetCode );
}

void _BILL_PACK_CNLPURCHASE::SwapByteOrder()
{
    KncUtil::SwapByteOrder( ChargeNo );
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( CanceledCashAmt );
    KncUtil::SwapByteOrder( RetCode );
}

void _BILL_PACK_REGCOUPON::SwapByteOrder()
{
    KncUtil::SwapByteOrder( UserNo );
    KncUtil::SwapByteOrder( ClientIP );
    KncUtil::SwapByteOrder( ClientPort );
    KncUtil::SwapByteOrder( GameServerNo );
    KncUtil::SwapByteOrder( WorldNo );
    KncUtil::SwapByteOrder( RealCash );
    KncUtil::SwapByteOrder( BonusCash );
    KncUtil::SwapByteOrder( GameCash );
    KncUtil::SwapByteOrder( ItemID );
    KncUtil::SwapByteOrder( ItemCnt );
    KncUtil::SwapByteOrder( ChargeNo );
    KncUtil::SwapByteOrder( RetCode );
}

void _BILL_PACK_CNLCOUPON::SwapByteOrder()
{
    KncUtil::SwapByteOrder( RetCode );
}

void _BILL_PACK_HEALTH_CHECK::SwapByteOrder()
{
    KncUtil::SwapByteOrder( RetCode );
}

KTHPayLetterEvent::KTHPayLetterEvent(void)
{
    Reset();
}

KTHPayLetterEvent::~KTHPayLetterEvent(void)
{
}

void KTHPayLetterEvent::Init()
{
#undef _ENUM
#define _ENUM( name, id ) KTHPayLetterEvent::ms_mapEventIDString[id] = L#name;
#include "TH_PayLetterEvent_def.h"
}

void KTHPayLetterEvent::Reset()
{
    ::memset( m_abyteNBuffer, 0, sizeof(m_abyteNBuffer) );
}

std::wstring KTHPayLetterEvent::GetEventIDString( const unsigned short usID_ ) // usually for log
{
    std::map< int, std::wstring >::const_iterator mit;
    std::wstring strID;
    mit = KTHPayLetterEvent::ms_mapEventIDString.find( usID_ );
    if( mit != KTHPayLetterEvent::ms_mapEventIDString.end() )
        strID = mit->second;
    return strID;
}

void KTHPayLetterEvent::WriteFromGTX( BYTE* abytePLBuffer_, unsigned short usSize_ )
{
    ::memcpy( m_abyteNBuffer, abytePLBuffer_, usSize_ );
}

void KTHPayLetterEvent::ReadHeader( OUT KTH_BILL_HEADER& kHeader_ ) const
{
    ::memcpy( &kHeader_, m_abyteNBuffer, sizeof(kHeader_) );
    kHeader_.SwapByteOrder();
}

void KTHPayLetterEvent::WriteHeader( IN const KTH_BILL_HEADER& kHeader_ )
{
    KTH_BILL_HEADER tmpHeader = kHeader_;
    tmpHeader.SwapByteOrder();
    ::memcpy( m_abyteNBuffer, &tmpHeader, sizeof(tmpHeader) );
}

#include <iomanip>
void KTHPayLetterEvent::DumpBuffer() const
{
    // 패킷 헤더를 가져 온다.
    KTH_BILL_HEADER kHder;
    ReadHeader( kHder );
    if( kHder.ReqLen <= sizeof(kHder) )
    {
        START_LOG( cwarn, L" 패킷이 비어 있음.." ) << END_LOG;
        return;
    }

    if( kHder.ReqLen >= MAX_TH_BILLING_PACKET_SIZE )
    {
        START_LOG( cwarn, L" 패킷길이가 잘못 되었음..ID: " << GetEventIDString( kHder.ReqType )
            << ", size : " << kHder.ReqLen ) << END_LOG;
        return;
    }

    std::wstringstream stm;
    stm.setf( std::ios::hex, std::ios::basefield );
    for( int i = 0 ; i < kHder.ReqLen ; ++i )
    {
        if( i > 0 && i % 16 == 0 )
        {
            stm << std::endl;
        }
        stm << std::setw(2) << std::setfill(L'0') << (int)m_abyteNBuffer[i] << " ";
    }
    START_LOG( clog, L"Packet Dump.." )
        << BUILD_LOG( GetEventIDString( kHder.ReqType ) )
        << BUILD_LOG( kHder.ReqLen )
        << L"____Dump____"
        << BUILD_LOG( stm.str().c_str() )
        << END_LOG;
}