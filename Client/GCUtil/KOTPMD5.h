/*[ KOTPMD5.h ] : 2010 11 15 exxa222@kog.co.kr
    
========================================================================*/
#pragma once

#include <string>

class KOTPMD5
{
    enum KOTPMD5_CONST_ENUM
    {
        TIME_INTERVAL       =   1000,   //  1분 단위로 오티피 변경
        CHECK_PREV_LEVEL    =   2*60,      //  최대 2*TIME_INTERVAL 만큼 이전 값까지 비교한다.
    };

public:
    bool            CheckOTP( /*IN*/const std::string& strOTP_, /*IN*/const unsigned int uiPrevLevel_ = CHECK_PREV_LEVEL +1 );
    std::string&    GetOTP();

private:
    void            SetTick( /*IN*/const unsigned int uiTick_ );
    std::string&    CreateOTP( /*IN*/const std::wstring& strKey_, /*IN*/const unsigned int uiTick_ );

public:
    KOTPMD5();
    ~KOTPMD5();

private:    //  복사대입연산자, 복사생성자는 필요없다.
    KOTPMD5& operator=( KOTPMD5& src_ );    //  구현 안함
    KOTPMD5( KOTPMD5& src_ );               //  구현 안함

private:
    unsigned int    m_uiKeyTick;
    std::string     m_strOTP;
};
