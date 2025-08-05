#include "CenterSimLayer.h"
#include "CnQuizBingo.h"
#include <dbg/dbg.hpp >
#include "CenterDBThread.h" // centerpacket
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnQuizBingo );
ImplOstmOperatorW2A( KCnQuizBingo );
NiImplementRootRTTI( KCnQuizBingo );

KCnQuizBingo::KCnQuizBingo(void)
:m_bChanged(false)
,m_dwTickGap(60000)
,m_nEventID(0)
,m_tmBegin(1)
,m_tmEnd(1)
{
    m_dwLastTick = ::GetTickCount();
}

KCnQuizBingo::~KCnQuizBingo(void)
{
}

ImplToStringW( KCnQuizBingo )
{
    KLocker lock( m_csBingo );
    return START_TOSTRINGW
        << TOSTRINGW( m_bChanged )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_nEventID )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_mapQuestionList.size() )
        << TOSTRINGW( m_mapCompressedQuestionList.size() );
}

void KCnQuizBingo::Tick()
{
    if ( CheckTick() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_QUIZ_BINGO_DATA_NOT, L"", 0 );
    }

    if ( CheckChange() ) {
        KECN_QUIZ_BINGO_DATA_NOT kPacket;
        GetQuizBingoData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_QUIZ_BINGO_DATA_NOT, kPacket );
    }
}

bool KCnQuizBingo::CheckChange()
{
    KLocker lock( m_csBingo );
    if ( m_bChanged ) {
        m_bChanged = false;
        return true;
    }

    return false;
}

bool KCnQuizBingo::CheckTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

bool KCnQuizBingo::VerifyQuestions( IN const std::map<int,KBingoQuestionInfo>& mapQuestions_ )
{
    // 이터레이션 하면서 지문이 비어 있는 녀석이 있으면 오류를 일으키겠음..
    bool bRet = true;
    std::map<int,KBingoQuestionInfo>::const_iterator mit;
    for ( mit = mapQuestions_.begin() ; mit != mapQuestions_.end() ; ++mit ) {
        if( mit->second.m_mapChoices.empty() ) {
            bRet = false;
            START_LOG( cerr, L" Question ID : " << mit->first ) << END_LOG;
        }
    }
    return bRet;
}

void KCnQuizBingo::UpdateBingoConfig( KCnQuizBingo::SBingoConfigData& bingoConfig_ )
{
    KLocker lock(m_csBingo);
    m_nEventID = bingoConfig_.m_nEventID;
    m_tmBegin = bingoConfig_.m_tmBegin;
    m_tmEnd = bingoConfig_.m_tmEnd;
    m_mapQuestionList = bingoConfig_.m_mapQuestions;
    CompressQuestions( bingoConfig_.m_mapQuestions );
    m_bChanged = true;
}

void KCnQuizBingo::GetQuizBingoData( OUT KECN_QUIZ_BINGO_DATA_NOT& kPacket_ )
{

    KLocker lock(m_csBingo);
    kPacket_.m_nEventID = m_nEventID;
    kPacket_.m_tmBegin = m_tmBegin;
    kPacket_.m_tmEnd = m_tmEnd;
    kPacket_.m_mapCompressedQuestionList = m_mapCompressedQuestionList;
}

void KCnQuizBingo::CompressQuestions( IN const std::map<int,KBingoQuestionInfo>& mapQuestions_ )
{
    // Lock은 호출 하는곳에 걸려 있다.
    m_mapCompressedQuestionList.clear();
    std::map<int,KBingoQuestionInfo>::const_iterator mit;
    for ( mit = mapQuestions_.begin() ; mit != mapQuestions_.end() ; ++mit ) {
        KSerBuffer kBuff;
        KSerializer ks;
        ks.BeginWriting( &kBuff );
        if( !ks.Put( mit->second ) ) continue;
        ks.EndWriting();
        kBuff.Compress();
        if ( !m_mapCompressedQuestionList.insert( std::make_pair( mit->first, kBuff) ).second ) {
            START_LOG( cerr, L"문제 압축중 중복된 문제가 존재 ID : " << mit->first ) << END_LOG;
        }
    }
}