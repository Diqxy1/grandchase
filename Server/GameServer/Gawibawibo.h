#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CommonPacket.h"
#include "Kairo.h"

class KLuaManager;
struct KUserGawibawiboInfo;

class KGawibawibo
{
    DeclareSingleton( KGawibawibo );
    NiDeclareRootRTTI( KGawibawibo );

    DeclToStringW;

    enum ENUM_GAWIBAWIBO_TYPE {
        TRY = 0,
        RETRY = 1
    };
    
    typedef std::vector<KDropItemInfo> VEC_REWARD;
public:
    KGawibawibo(void);
    ~KGawibawibo(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    DWORD GetPointTimeGap() { return m_dwPointTimeGap; }
    DWORD GetVersion() { return m_dwVersion; }
    bool IsRun();
    int DoGawibawibo( IN DWORD dwBossID_, IN DWORD dwStage_ );

    DWORD GetLimitPointPerDay() { return m_dwTryPointLimitPerDay; }
    void TestPlay( IN DWORD dwBossID_, IN DWORD dwStage_ );
    DWORD MarkRetryPosition();
    void GetRewardList( OUT std::map< std::pair< DWORD, DWORD >, VEC_REWARD >& mapWinReward );
    DWORD GetRetryPointPerPlays() { return m_dwRetryPointPerPlays; }
    void GetBossReward( IN DWORD dwStage_, IN DWORD dwBossID_, OUT VEC_REWARD& vecReward_ );
    bool CheckFinalBoss( IN DWORD dwStage_, IN DWORD dwBossID_ );
    bool UsePoint( IN int nPointType_, OUT KUserGawibawiboInfo& kUserInfo_ );
    void GetNumberOfBoss( IN DWORD dwStage_, OUT USHORT& usNumberOfBoss_ );
    bool CheckTryPoint( IN const KUserGawibawiboInfo& kUserInfo_ );
    bool TradeItem( IN const GCITEMID ItemID_, IN const int nItemNum_, OUT KUserGawibawiboInfo& kUserInfo_ );
    void GetBossKairoReward( IN DWORD dwStage_, IN DWORD dwBossID_, OUT VEC_REWARD& vecReward_ );
    void GetLoseReward( OUT VEC_REWARD& vecReward_ );
    DWORD GetDefaultRewardStage(){ return m_dwDefaultRewardStage; }
    void SetInitGawibawiboPosition( IN OUT DWORD& dwCurrentPosition_ );
    void SetRetryPosition( IN OUT DWORD& dwCurrentPosition_ );
    void DeleteRetryPosition( IN OUT DWORD& dwCurrentPosition_ );
    void AddGawibawiboTurnCount( IN OUT DWORD& dwTurnCount_ );
    void MoveGawibawiboCurrentPosition( IN OUT DWORD& dwCurrentPosition_ );
    void AddGawibawiboPlayCount( IN OUT DWORD& dwPlayCount_ );
    void AddGawibawiboRetryPoint( IN OUT KUserGawibawiboInfo& kGawibawiboInfo_ );
    bool Get_Gawibawibo_UserData_FromDB( IN const std::string& strScript_ );

protected:
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< std::pair< DWORD, DWORD >,VEC_REWARD>& mapReward, std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > >& mapBossWinLoseRatio, OUT std::map< DWORD, DWORD >& mapStageFinalBoss, OUT std::map< std::pair< DWORD, DWORD >, Kairo >& mapKairoReward_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng, OUT VEC_REWARD& vecReward, OUT Kairo& kairo );
    bool LoadBoss( IN KLuaManager& kLuaMng_, OUT std::map< std::pair< DWORD, DWORD >,VEC_REWARD>& mapReward_, DWORD Stage_, OUT std::map< std::pair< DWORD, DWORD>, std::pair< DWORD, DWORD > >& mapBossWinLoseRatio, OUT std::map< std::pair< DWORD, DWORD >, Kairo >& mapKairoReward_ );
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair< GCITEMID, DWORD >& prItem_ );
    bool LoadLoseReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecReward );

    bool UseTryPoint( OUT DWORD& dwTryPoint_ );
    bool UseRetryPoint( OUT DWORD& dwRetryPoint_ );

protected:
    mutable KncCriticalSection          m_csGawibawibo;

private:
    time_t                              m_tmBegin;          // 이벤트 시작 날짜.
    time_t                              m_tmEnd;            // 이벤트 종료 날짜.

    std::map< std::pair< DWORD, DWORD >, VEC_REWARD > m_mapWinReward; // 승리 보상 : < 스테이지, 보스 번호 >, 보상리스트
    std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > > m_mapBossWinLoseRatio; // 승,패 확률 ( 무승부는 나머지 값)
    DWORD m_dwPointTimeGap; // 포인트 주는 시간 간격
    DWORD m_dwVersion;
    int m_nRetryState; // 128 , 클라이언트로 전달해주는 값. 서버에서도 사용 , 재도전 가능한 상태인지 판단 기준.
    
    DWORD m_dwUseTryPoint;   // 한 번에 소모되는  도전 포인트 수, 기본 1
    DWORD m_dwUseRetryPoint; // 소모되는 재도전 포인트 
    DWORD m_dwRetryPointPerPlays; // x 번 도전 하면 재도전 포인트 지급.
    DWORD m_dwTryPointLimitPerDay; // 하루에 받을 수 있는 최대 도전 포인트
    DWORD m_dwRetryPositionValue;
    std::pair< GCITEMID, DWORD > m_prTryPointItem;   // 도전 포인트 아이템, 교환 포인트
    std::pair< GCITEMID, DWORD > m_prRetryPointItem;

    std::map<DWORD, DWORD> m_mapStageFinalBoss;

    std::map< std::pair< DWORD, DWORD >, Kairo > m_mapKairoReward;
    VEC_REWARD m_vecLoseReward;
    DWORD m_dwDefaultRewardStage;

};

DefSingletonInline( KGawibawibo );
DeclOstmOperatorA( KGawibawibo );