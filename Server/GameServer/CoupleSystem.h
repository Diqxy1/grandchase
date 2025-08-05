#pragma once
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <map>
#include <vector>
#include <atltime.h>
#include "PrimitiveTypedef.h"

struct KCoupleEquip; // UserPacket
struct KSimpleItem; // UserPacket
struct KCoupleInfo; // UserPacket
class KLuaManager;

// 해당 값의 추가는 32bit time_t의 구간때문에 추가되었다.
// 32bit time_t는 1970년 1월 1일 ~ 2038년 1월 19일 23시 59분까지 가능하다.
// 해당 값을 CTime으로 변환할 때 문제가 발생하기 때문에 추가해서 체크한다.
#define TIME_T_MINIMUM 54000

struct SCoupleBenefit
{
    GCITEMID   m_dwItemID;
    float   m_fExpRatio;
    float   m_fGPRatio;
    float   m_fSPXRatio;
};

struct SRequestInfo
{
    DWORD       m_dwRequester;
    DWORD       m_dwRequestee;
    GCITEMID       m_dwItemID;
    GCITEMUID   m_dwItemUID;
    bool        m_bInCoupleInven;
    int         m_nState;

    SRequestInfo():m_dwRequester(0)
        ,m_dwRequestee(0)
        ,m_dwItemID(0)
        ,m_dwItemUID(0)
        ,m_bInCoupleInven(false)
        ,m_nState(0) {}
};

struct SMemorialDayInfo
{
    int     m_nMemorialDateGap;
    int     m_nMemorialMonthGap;
    int     m_nMemorialYearGap;
    float   m_ExpRatio;
    float   m_GpRatio;
    float   m_SpxRatio;
    void Clear()
    {
        m_nMemorialDateGap  = 0;
        m_nMemorialMonthGap = 0;
        m_nMemorialYearGap  = 0;
        m_ExpRatio          = 0.f;
        m_GpRatio           = 0.f;
        m_SpxRatio          = 0.f;
    }
};

struct SCoupleSignBoard
{
    DWORD   m_dwRingID;
    GCITEMID m_dwSignBoardID;
    DWORD   m_dwFontColor;
    int     m_nType; // Admin, Normal ...
    bool    m_bEnableBlock;
};

class KCoupleSystem
{
    DeclareSingleton( KCoupleSystem );
    NiDeclareRootRTTI( KCoupleSystem );
    DeclToStringW;
public:
    enum COUPLE_STATE
    {
        CS_NOTLOADED = 0,   // 커플 정보를 가져 오지 않은상태.
        CS_NONE,            // 일반
        CS_COUPLE,          // 커플중
        CS_REQUEST,         // 커플 요청중
        CS_REQUEST_FROM,    // 커플 요청 받는중
        CS_DENY,            // 커플 거부

        CS_MAX,
    };

    enum MAKE_COUPLE_STATE
    {
        MCS_REQ,
        MCS_ASK,
        MCS_ANS,
        MCS_ANS_FROM,
        MCS_DB_WORK,
        MCS_MAX,
    };

public:
    KCoupleSystem(void);
    ~KCoupleSystem(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsCoupleRequestable( DWORD dwState );
    bool IsRingItem( GCITEMID dwItemID );
    bool GetCoupleItem( IN GCITEMID dwItemID, OUT KCoupleEquip& kItem );
    bool GetBenefitInfo( IN GCITEMID dwItemID, OUT SCoupleBenefit& kInfo );
    bool IsCoupleItem( GCITEMID itemID );

    bool AddCoupleRequestInfo( DWORD dwRequester, DWORD dwRequestee, IN KSimpleItem& kItem );
    bool GetCoupleRequestInfo( DWORD dwUID, OUT SRequestInfo& sReqInfo );
    void DeleteCoupleRequestInfo( DWORD dwRequester, DWORD dwRequestee );
    bool IsInCoupleReqInfo( DWORD dwUID );
    bool UpdateCoupleReqInfoState( DWORD dwUID, int nState );
    int GetCoupleReqInfoState( DWORD dwUID );

    bool AddRingRequestInfo( SRequestInfo& reqData );
    bool GetRingRequestInfo( DWORD dwUID, OUT SRequestInfo& sReqInfo );
    void DeleteRingRequestInfo( DWORD dwRequester, DWORD dwRequestee );
    bool IsInRingReqInfo( DWORD dwUID );
    bool UpdateRingReqInfo( DWORD dwUID, int nState );
    int GetRingReqInfo( DWORD dwUID );


    void SwapCoupleInfo( IN OUT KCoupleInfo& kInfo );

    bool ChkEquipEqual( std::vector<KCoupleEquip>& vecEquip1, std::vector<KCoupleEquip>& vecEquip2 );
    void GetEquipList( std::vector<KCoupleEquip>& vecFullEquip, std::vector<DWORD>& vecSimpleEquip );

    bool IsMemorialDay( time_t tmRegDate );

    float GetMemorialExp();
    float GetMemorialGp();
    float GetMemorialSpx();

    bool GetCoupleSignBoard( DWORD dwRingID, SCoupleSignBoard& signBoard );

    time_t GetCoupleRegDate( IN const KCoupleInfo& kCoupleInfo_ );

protected:
    bool LoadCoupleItems( KLuaManager& kLuaMng );
    bool LoadCoupleBenefit( KLuaManager& kLuaMng );
    bool LoadMemorialDay( KLuaManager& kLuaMng );
    bool LoadCoupleSignBoard( KLuaManager& kLuaMng );


    bool AddRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData, SRequestInfo& reqData );
    bool GetRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData, DWORD dwUID, OUT SRequestInfo& sReqInfo );
    void DeleteRequestInfo( std::map<DWORD,SRequestInfo >& mpaReqData, DWORD dwRequester, DWORD dwRequestee );
    bool IsInReqInfo( std::map<DWORD,SRequestInfo >& mpaReqData, DWORD dwUID );
    bool UpdateReqInfoState( std::map<DWORD,SRequestInfo >& mpaReqData, DWORD dwUID, int nState );
    int GetReqInfoState( std::map<DWORD,SRequestInfo >& mpaReqData, DWORD dwUID );

    void GetRegDate( CTime& tmOut, time_t tmRegDate );
    void GetCurrentDate( CTime& tmCurrentDate );
    bool CheckValidValue( IN const time_t& tmDate );

protected:
    KncCriticalSection                  m_csCouple;
    std::map<DWORD,KCoupleEquip>        m_mapCoupleItems;
    std::map<DWORD,SCoupleBenefit >     m_mapBenefitItems;

    KncCriticalSection                  m_csRequest;
    std::map<DWORD,SRequestInfo >       m_mapCoupleRequestInfo;
    std::map<DWORD,SRequestInfo >       m_mapRingRequestInfo;

    mutable KncCriticalSection      m_csMemorial;
    SMemorialDayInfo                m_kMemorialInfo;

    KncCriticalSection                  m_csCoupleSignBoard;
    std::map<DWORD, SCoupleSignBoard>   m_mapCoupleSignBoard;
};

DefSingletonInline( KCoupleSystem );
DeclOstmOperatorA( KCoupleSystem );