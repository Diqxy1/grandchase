/*[ KOTPMD5.cpp ] : 2010 11 15 exxa222@kog.co.kr
========================================================================*/
#include "StdAfx.h"
#include "KOTPMD5.h"

#include "KncUtil.h"

#include "../GCUtil/MD5/MD5Checksum.h"
#include "../GCUtil/MD5/MD5ChecksumDefines.h"

#include <set>

//  패처와 클라이언트가 가지는 공통의 비밀 키
#define __RUNDIRECT_OTP_MD5_PASSWORD__  L"b4e82ab052560f57571e42ce20f8929e"




std::string& KOTPMD5::CreateOTP( /*IN*/const std::wstring& strKey_, /*IN*/const unsigned int uiTick_ )
{
    char tickBuffer[32] =   {0,};
    _itoa_s( uiTick_, tickBuffer, sizeof(tickBuffer)/sizeof(char), 10 );
    std::string strTick( tickBuffer );

    static std::string  strReturn;
    strReturn.clear();
    strReturn.append("Main.exe ");

    size_t          passwordSize    =   strKey_.size();
    BYTE*           pbyteData       =   new BYTE[ passwordSize + 1 ];

    ::memcpy( pbyteData, KncUtil::toNarrowString( strKey_.c_str(), passwordSize +1 ).c_str(), passwordSize +1 );
    std::wstring    strMD5String    =   CMD5Checksum::GetMD5( pbyteData, passwordSize );
    std::string     strResult       =   KncUtil::toNarrowString( strMD5String );

    if( pbyteData ){
        delete [] pbyteData;
    }

    strReturn.append(strResult);
    strReturn.append(strTick);

    return strReturn;
}


void KOTPMD5::SetTick( /*IN*/const unsigned int uiTick_ )
{
    m_uiKeyTick =   uiTick_;
}


bool KOTPMD5::CheckOTP( /*IN*/const std::string& strOTP_, /*IN*/const unsigned int uiPrevLevel_ )
{
    std::set< std::string > setStrOTP;

    std::string strInfo;

    for( unsigned int i=0; i<uiPrevLevel_; ++i ){
        std::string strOTPtemp( this->CreateOTP( std::wstring(__RUNDIRECT_OTP_MD5_PASSWORD__), m_uiKeyTick -i ) );
        setStrOTP.insert( strOTPtemp );
        strInfo.append( strOTPtemp );
        strInfo.append( std::string("\n") );
    }

#if !defined( __PATH__ )
    //::MessageBoxA( NULL, strInfo.c_str(), strOTP_.c_str(), MB_OK );
#endif

    return ( setStrOTP.end() != setStrOTP.find( strOTP_ ) );
}


KOTPMD5::KOTPMD5()
: m_uiKeyTick(0)
{
    this->SetTick( ::GetTickCount() / TIME_INTERVAL );
    m_strOTP = this->CreateOTP( std::wstring(__RUNDIRECT_OTP_MD5_PASSWORD__), m_uiKeyTick );
}

KOTPMD5::~KOTPMD5()
{
}

std::string& KOTPMD5::GetOTP()
{
    return m_strOTP;
}


