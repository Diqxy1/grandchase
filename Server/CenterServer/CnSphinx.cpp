#include "CenterSimLayer.h"
#include "CnSphinx.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h" // centerpacket
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnSphinx );
ImplOstmOperatorW2A( KCnSphinx );
NiImplementRootRTTI( KCnSphinx );

KCnSphinx::KCnSphinx(void)
:m_dwTickGap(60000)
,m_nVersion(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_bChanged(false)
{
    // Init.
    m_dwLastTick = ::GetTickCount();
    m_mapSphinxQuestionInfo.clear();
    m_mapCompressedSphinxQuestionInfo.clear();
}

KCnSphinx::~KCnSphinx(void)
{
}

ImplToStringW( KCnSphinx )
{
    KLocker lock( m_csSphinx );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_bChanged )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_mapSphinxQuestionInfo.size() )
        << TOSTRINGW( m_mapCompressedSphinxQuestionInfo.size() );
}

bool KCnSphinx::LoadScript()
{
    KLuaManager kLuaMng;
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwTickGap;
    int         nVersion;

    _JIF( kLuaMng.DoFile( "InitCnSphinx.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SphinxVersion", nVersion ) == S_OK, return false );

    START_LOG( cerr, L"센터 스핑크스 OX 게임 스크립트 읽기 완료." ) << END_LOG;
    KLocker lock( m_csSphinx );
    m_dwTickGap         = dwTickGap;
    m_tmBegin           = tmBegin;
    m_tmEnd             = tmEnd;
    m_nVersion          = nVersion;

    return true;
}

bool KCnSphinx::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

void KCnSphinx::Tick()
{
    if ( CheckTick() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_SPHINX_DATA_NOT, L"", 0 );
    }

    if ( CheckChange() ) {
        KECN_SPHINX_DATA_NOT kPacket;
        GetSphinxData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SPHINX_DATA_NOT, kPacket );
    }
}

bool KCnSphinx::CheckTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

bool KCnSphinx::CheckChange()
{
    KLocker lock( m_csSphinx );
    if ( m_bChanged ) {
        m_bChanged = false;
        return true;
    }

    return false;
}

void KCnSphinx::UpdateSphinxData( IN std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo_ )
{
    KLocker lock( m_csSphinx );
    m_mapSphinxQuestionInfo = mapSphinxQuestionInfo_;
    CompressSphinxQuestions( mapSphinxQuestionInfo_ );
    m_bChanged = true;
}

bool KCnSphinx::VerifySphinxData( IN const std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo_ )
{
    // 문제답이 잘못된 부분에 대한 체크.( 0 : X, 1 : O )
    bool bRet = true;
    std::map<int,KSphinxQuestionInfo>::const_iterator mit;
    for ( mit = mapSphinxQuestionInfo_.begin() ; mit != mapSphinxQuestionInfo_.end() ; ++mit ) {
        if( mit->second.m_nAnswer > 1 ) {
            bRet = false;
            START_LOG( cerr, L" Question ID : " << mit->first ) << END_LOG;
        }
    }
    return bRet;
}

void KCnSphinx::GetSphinxData( OUT KECN_SPHINX_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csSphinx );
    kPacket_.m_nVersion = m_nVersion;
    kPacket_.m_tmBegin = m_tmBegin;
    kPacket_.m_tmEnd = m_tmEnd;
    kPacket_.m_mapCompressedSphinxQuestionInfo = m_mapCompressedSphinxQuestionInfo;
}

void KCnSphinx::CompressSphinxQuestions( IN const std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo_ )
{
    // Lock은 호출 하는곳에 걸려 있다.
    m_mapCompressedSphinxQuestionInfo.clear();
    std::map<int,KSphinxQuestionInfo>::const_iterator mit;
    for ( mit = mapSphinxQuestionInfo_.begin() ; mit != mapSphinxQuestionInfo_.end() ; ++mit ) {
        KSerBuffer kBuff;
        KSerializer ks;
        ks.BeginWriting( &kBuff );
        if( !ks.Put( mit->second ) ) continue;
        ks.EndWriting();
        kBuff.Compress();
        if ( !m_mapCompressedSphinxQuestionInfo.insert( std::make_pair( mit->first, kBuff) ).second ) {
            START_LOG( cerr, L"문제 압축중 중복된 문제가 존재 ID : " << mit->first ) << END_LOG;
        }
    }
}