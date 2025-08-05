#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <string>
#include <windows.h>    // DWORD
#include "KncException.h"

struct KJoinRoomAckInfo;
class KFailRate
{
    DeclareSingleton( KFailRate );
    DeclToStringW;

protected:
    KFailRate(void);
public:
    virtual ~KFailRate(void);

#   undef _ENUM
#   define _ENUM( id, comment ) id,
    enum ENUM_COUNT {
#include "FailRate_def.h"
    };

    enum ENUM_CLIENTERR {
#include "ClientErr_def.h"
    };

    int IncreaseCount( int nIndex );    ///< @return : 해당 인덱스의 변경되고 난 후의 수치
    int IncreaseCount( int nIndex, int nNum );
    int IncreaseCErrCount( IN int nIndex );
    int IncreaseCErrCount( IN int nIndex_, int nNum_ );
    void Tick();    // 수시로 호출되면서 DB에 데이터를 업데이트한다.
    //bool AnalysisJoinRoomAck( KEVENT_JOIN_ROOM_ACK& kAck ); // 처리루틴을 한곳에 모으자.
    static const wchar_t* IDToString( int nReason );
    static const wchar_t* CErrIDToString( int nReason );

    static const wchar_t* IDString( int nReason );
    static const wchar_t* CErrIDString( int nReason );

    // for lua
    void SetWriteToDB( bool bWriteToDB );
    void ToLog();   // 접속률 수집 데이터를 로그파일에 기록한다. (일본의 경우 콘솔 출력이 깨짐)
    void ClientErrLog(); // 클라이언트 에러 리포팅
    void SetWriteToDBTick( DWORD dwTick ) { m_dwFailRateWriteTick = dwTick; }
    void EndDataToDB() { WriteToDB(); }
    void SetUpdateTime( IN int nMinute );

protected:  // util function
    inline void WriteToDB();
    void UpdateFlushTime( IN const int& nMinute );
    bool CheckUpdate();

protected:
    bool                        m_bWriteToDB;   // 24시간마다 DB에 데이터 기록 여부
    mutable KncCriticalSection  m_csFailCount;
    int                         m_anFailCount[COUNT_TOTAL];
    static const wchar_t*       ms_szCountReason[];
    static const wchar_t*       ms_szCountIDs[];

    mutable KncCriticalSection  m_csClientErrCount;
    int                         m_anClientErrCount[ CE_TOTAL ];
    static const wchar_t*       ms_szClientErrReason[];
    static const wchar_t*       ms_szClientErrIDs[];

    DWORD                       m_dwLastTick;
    DWORD                       m_dwFailRateWriteTick;	    // 080825. woosh. FailRate 에서 DB 기록 주기

    mutable KncCriticalSection  m_csUpdateTime;
    CTime                       m_tmUpdateTime;  // DB 업데이트 시간.
    int                         m_nUpdateMinute; // DB Update 주기.
};

DefSingletonInline( KFailRate );
DeclOstmOperatorA( KFailRate );
