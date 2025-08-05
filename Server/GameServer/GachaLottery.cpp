#include "GachaLottery.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "SignBoard.h"
#include "GameServer.h"
#include "NetError.h"
#include "Log4.h"

ImplementSingleton( KGachaLottery );
ImplOstmOperatorW2A( KGachaLottery );
NiImplementRootRTTI( KGachaLottery );

KGachaLottery::KGachaLottery(void)
:m_GachaLotteryItemID(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwVersion(0)
{
    m_mapGachaLotteryInfo.clear();
    m_mapGachaSuccessList.clear();
    m_mapGachaFailedList.clear();
    m_mapGradeDescInfo.clear();
}

KGachaLottery::~KGachaLottery(void)
{
}

ImplToStringW( KGachaLottery )
{
    KLocker lock( m_csGachaLottery );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_mapGachaLotteryInfo.size() )
        << TOSTRINGW( m_mapGachaSuccessList.size() )
        << TOSTRINGW( m_mapGachaFailedList.size() );
}

bool KGachaLottery::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGachaLottery::_LoadScript( OUT std::string& strScript_ )
{
    GCITEMID    GachaLotteryItemID;
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwVersion;
    bool        bNoticeEnable;
    std::map<char,KGachaLotteryInfo>        mapGachaLotteryInfo;
    std::map<char, std::vector<std::pair<GCITEMID,int>>>   mapGachaSuccessList;
    std::map<char, std::vector<std::pair<GCITEMID,int>>>   mapGachaFailedList;
    std::map<DWORD, std::wstring>           mapGradeDescInfo;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitGachaLottery.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "GachaLotteryVersion", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GachaLotteryItemID", GachaLotteryItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GachaNoticeEnable", bNoticeEnable ) == S_OK, return false );
    _JIF( LoadGradeDesc( kLuaMng, std::string("GradeStringDesc"), mapGradeDescInfo ), return false );
    _JIF( LoadGachaLotteryInfo( kLuaMng, std::string("InitGachaLottery"), mapGachaLotteryInfo, mapGachaSuccessList, mapGachaFailedList ), return false );

    START_LOG( cerr, L"가챠 복권 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csGachaLottery );
        m_dwVersion = dwVersion;
        m_GachaLotteryItemID = GachaLotteryItemID;
        m_bNoticeEnable = bNoticeEnable;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_mapGradeDescInfo.swap( mapGradeDescInfo );
        m_mapGachaLotteryInfo.swap( mapGachaLotteryInfo );
        m_mapGachaSuccessList.swap( mapGachaSuccessList );
        m_mapGachaFailedList.swap( mapGachaFailedList );
        return true;
    }
}

bool KGachaLottery::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KGachaLottery::LoadGachaLotteryInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<char,KGachaLotteryInfo>& mapGachaLotteryInfo_, OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaSuccessList_, OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaFailedList_ )
{
    mapGachaLotteryInfo_.clear();
    mapGachaSuccessList_.clear();
    mapGachaFailedList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nCharType;
        KGachaLotteryInfo kGachaLotteryInfo;
        std::vector<std::pair<GCITEMID,int>> vecSuccessItemInfo;
        std::vector<std::pair<GCITEMID,int>> vecFailedItemInfo;
        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( LoadGradeInfo( kLuaMng_, std::string("GradeInfo"), kGachaLotteryInfo.m_mapGradeInfo ), return false );
        _JIF( LoadRewardTypeRatio( kLuaMng_, std::string("RewardType"), kGachaLotteryInfo.m_kairoRewardType ), return false );
        _JIF( LoadRewardListRatio( kLuaMng_, std::string("SuccessRewardList"), kGachaLotteryInfo.m_kairoSuccessReward, vecSuccessItemInfo ), return false );
        _JIF( LoadRewardListRatio( kLuaMng_, std::string("FailRewardList"), kGachaLotteryInfo.m_kairoFailReward, vecFailedItemInfo ), return false );

        kGachaLotteryInfo.m_cCharType = static_cast<char>( nCharType );
        if ( !mapGachaLotteryInfo_.insert( std::make_pair( kGachaLotteryInfo.m_cCharType, kGachaLotteryInfo ) ).second ) {
            START_LOG( cerr, L"가챠 복권 정보에 중복된 CharType 있음. CharType : " << kGachaLotteryInfo.m_cCharType ) << END_LOG;
        }

        if ( !mapGachaSuccessList_.insert( std::make_pair( kGachaLotteryInfo.m_cCharType, vecSuccessItemInfo ) ).second ) {
            START_LOG( cerr, L"클라이언트 전달 성공 아이템 정보에 중복된 CharType 있음. CharType : " << kGachaLotteryInfo.m_cCharType ) << END_LOG;
        }

        if ( !mapGachaFailedList_.insert( std::make_pair( kGachaLotteryInfo.m_cCharType, vecFailedItemInfo ) ).second ) {
            START_LOG( cerr, L"클라이언트 전달 실패 아이템 정보에 중복된 CharType 있음. CharType : " << kGachaLotteryInfo.m_cCharType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGachaLottery::LoadGradeDesc( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD, std::wstring>& mapGradeDescInfo_ )
{
    mapGradeDescInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwGrade;
        std::string strMsgID;
        std::wstring wstrMsg;

        _JIF( kLuaMng_.GetValue( "Grade", dwGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "msgID", strMsgID ) == S_OK, return false );
        wstrMsg = SiKGameServer()->m_stringTable.GetValue( KncUtil::toWideString( strMsgID ).c_str() );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( !mapGradeDescInfo_.insert( std::make_pair( dwGrade, wstrMsg ) ).second ) {
            START_LOG( cerr, L"등급별 출력 메세지 정보에 중복 등급 있음.: " << dwGrade ) << END_LOG;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaLottery::LoadGradeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, DWORD>& mapGradeInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID;
        DWORD dwGrade;

        _JIF( kLuaMng_.GetValue( "Grade", dwGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ItemID", ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( !mapGradeInfo_.insert( std::make_pair( ItemID, dwGrade ) ).second ) {
            START_LOG( cerr, L"등급별 아이템 정보에 중복된 ItemID가 있음. ItemID : " << ItemID ) << END_LOG;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaLottery::LoadRewardTypeRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kKairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwType = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kKairo_.SetPostType( dwType, fProb ), return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaLottery::LoadRewardListRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kKairo_, OUT std::vector<std::pair<GCITEMID,int>>& vecItemInfo_ )
{
    vecItemInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        int nGrade = -1;
        int nLevel = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kKairo_.SetEquipLevelDropItem( itemID, nDuration, nPeriod, (char)nGrade, (char)nLevel, fProb ), return false );
        vecItemInfo_.push_back( std::make_pair( itemID, nLevel ) );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaLottery::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csGachaLottery );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KGachaLottery::DoGambleRewardType( IN const char& cCharType_, OUT DWORD& dwRewardType_ )
{
    bool bSuccess;
    {
        std::map<char, KGachaLotteryInfo>::iterator mit;

        KLocker lock( m_csGachaLottery );
        mit = m_mapGachaLotteryInfo.find( cCharType_ );
        if( mit == m_mapGachaLotteryInfo.end() ) {
            return false;
        }
        bSuccess = mit->second.m_kairoRewardType.Do( dwRewardType_ );
    }

    if( !bSuccess ) {
        return false;
    }

    return true;
}

bool KGachaLottery::DoGambleItemInfo( IN const char& cCharType_, IN const DWORD& dwType_, OUT KDropItemInfo& kItem_ )
{
    bool bSuccess;
    {
        std::map<char, KGachaLotteryInfo>::iterator mit;

        KLocker lock( m_csGachaLottery );
        mit = m_mapGachaLotteryInfo.find( cCharType_ );
        if( mit == m_mapGachaLotteryInfo.end() ) {
            return false;
        }
        if( dwType_ == KGachaLottery::SUCCESS ) {
            bSuccess = mit->second.m_kairoSuccessReward.Do( kItem_ );
        } else {
            bSuccess = mit->second.m_kairoFailReward.Do( kItem_ );
        }
    }

    if( !bSuccess ) {
        return false;
    }

    if( kItem_.m_ItemID == 0 ) {
        return false;
    }

    return true;
}

void KGachaLottery::GetGachaLotteryItemInfo( OUT std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaSuccessList_, std::map<char, std::vector<std::pair<GCITEMID,int>>>& mapGachaFailedList_ )
{
    KLocker lock( m_csGachaLottery );

    mapGachaSuccessList_ = m_mapGachaSuccessList;
    mapGachaFailedList_ = m_mapGachaFailedList;
}

bool KGachaLottery::CheckItemGrade( IN const char& cCharType_, IN GCITEMID& DisplayItemID_, OUT std::wstring& wstrMsg_ )
{
    std::map<char, KGachaLotteryInfo>::iterator mit;
    std::map<GCITEMID, DWORD>::iterator mitGrade;

    KLocker lock( m_csGachaLottery );
    mit = m_mapGachaLotteryInfo.find( cCharType_ );
    if( mit == m_mapGachaLotteryInfo.end() ) {
        return false;
    }

    mitGrade = mit->second.m_mapGradeInfo.find( DisplayItemID_ );
    if( mitGrade == mit->second.m_mapGradeInfo.end() ) {
        return false;
    }

    if( !GetGradeStringDesc( mitGrade->second, wstrMsg_ ) ) {
        return false;
    }

    return true;
}

void KGachaLottery::SendGachaLotteryMsg( IN std::wstring& wstrData_ )
{
    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    kData.m_strMsg = wstrData_;

    SiKSignBoard()->QueueingAdminData( kData );
}

bool KGachaLottery::GetGradeStringDesc( IN const DWORD& dwGrade_, OUT std::wstring& wstrMsg_ )
{
    std::map<DWORD, std::wstring>::iterator mit;
    mit = m_mapGradeDescInfo.find( dwGrade_ );
    if( mit == m_mapGradeDescInfo.end() ) {
        return false;
    }
    wstrMsg_ = mit->second;

    return true;
}