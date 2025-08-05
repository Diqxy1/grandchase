#pragma once

#include "KNCSingleton.h"

//  에러 타입 구분은 두가지이다.
//  카운트가 중요한 부분은 ErrorCollect 함수를 이용해서 카운팅한다.
//  ErrorCollectData 해당 함수는 패킷량때문에 최초 한번의 데이터만 보낸다.

class GCClientErrorManager
{
    DeclareSingleton( GCClientErrorManager );

    enum 
    {
        FPS_20 = 20,
        FPS_30 = 30,
        FPS_40 = 40,
        FPS_50 = 50,
        FPS_60 = 60,
    };

public:
    GCClientErrorManager(void);
    ~GCClientErrorManager(void);

    bool FrameMove(float fElapsedTime);

private:
    float fSendTime;

    std::map< std::pair<int,int>, int > m_mapErrorCount;   //  에러Type, 에러ID, Count
    std::set< std::pair<int,int> >     m_setOnceCount;    //  데이터를 실어 보내는것은 한번만 보낸다.

public: 
    void ErrorCollect( const int iErrorType, const int iErrorID );
    void ErrorCollect( const int iErrorType, const int iErrorID, const int nCount );
    void ErrorCollectData( const int iErrorType, const int iErrorID, const std::wstring strErrComment = L"", DWORD dwReserve1 = 0, DWORD dwReserve2 = 0, DWORD dwReserve3 = 0 );
    
    //  모아 놓은 데이터를 일괄 보낸다.
    void SendCollectError();

    void Send_PingTimeReport( bool bRelay, IN DWORD dwAvgPing_ );
    void Send_AveFPSReport( IN int iAveFPS );
    
};

DefSingletonInline( GCClientErrorManager );