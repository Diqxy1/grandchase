#include "user.h" // winsock 땜시롱..
#include "CnConnector.h"
#include "MiniGameRankManager.h"
#include "GSSimLayer.h"
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"

namespace{

    class Sort_By_Score
    {
    public:
        bool operator () ( const KMiniGameRankInfo& a, const KMiniGameRankInfo& b )
        {
            if( a.m_dwScore > b.m_dwScore ) return true;
            return false;
        }

        bool operator () ( const DWORD a, const KMiniGameRankInfo& b )
        {
            if( a > b.m_dwScore ) return true;
            return false;
        }

        bool operator () ( const KMiniGameRankInfo& a, const DWORD b )
        {
            if( a.m_dwScore > b ) return true;
            return false;
        }
    };

    class Sort_By_UID
    {
    public:
        bool operator () ( const KMiniGameRankInfo& a, const KMiniGameRankInfo& b )
        {
            if( a.m_dwUserUID > b.m_dwUserUID ) return true;
            return false;
        }

        bool operator () ( const DWORD a, const KMiniGameRankInfo& b )
        {
            if( a > b.m_dwUserUID ) return true;
            return false;
        }

        bool operator () ( const KMiniGameRankInfo& a, const DWORD b )
        {
            if( a.m_dwUserUID > b ) return true;
            return false;
        }
    };

    class KRankNumbering
    {
    public:
        KRankNumbering():nNum(0){};
        void operator () ( KMiniGameRankInfo& kRank ) { kRank.m_nRank = ++nNum; };
    private:
        int nNum;
    };
} // 정렬방법과 탐색방법이 같아야 된다는 

//FILE_NAME_FOR_LOG

ImplementSingleton( KMiniGameRankManager );
NiImplementRootRTTI( KMiniGameRankManager );
ImplOstmOperatorW2A( KMiniGameRankManager );

ImplToStringW( KMiniGameRankManager )
{
    KLocker lock1(m_csTickInfo);
    KLocker lock2(m_csRank);

    START_TOSTRINGW
        << TOSTRINGW( m_dwUpdateGap )
        << TOSTRINGW( m_dwLastUpdateTick )
        << TOSTRINGW( m_mapTotalRank.size() )
        << TOSTRINGW( m_mapTotalUser.size() );

    return stm_;
}

KMiniGameRankManager::KMiniGameRankManager(void)
:m_dwUpdateGap( 1000 * 60 * 5 ) // test -> 2
,m_dwLastUpdateTick( ::GetTickCount() )
{
}

KMiniGameRankManager::~KMiniGameRankManager(void)
{
}

bool KMiniGameRankManager::GetUserRankRange( IN char cType_, IN int nBegin_, IN int nCount_, OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    KLocker lock( m_csRank );
    _JIF( nBegin_>= 0 && nCount_ > 0, return false );
    vecRank_.clear();
    std::vector<KMiniGameRankInfo>* pVecRank = NULL;
    _JIF( GetRankVector( m_mapTotalRank, cType_, &pVecRank ), return false );
    _JIF( pVecRank, return false );

    // 영역 유효성 검사.
    if( pVecRank->empty() ) return false;

    if( pVecRank->size() <= (DWORD)nBegin_ ) return false;

    if( pVecRank->size() < (DWORD)(nBegin_ + nCount_) )
        nCount_ = (int)(pVecRank->size() - (DWORD)nBegin_);
    
    vecRank_.insert( vecRank_.end(), pVecRank->begin() + nBegin_, pVecRank->begin() + (nBegin_+nCount_) );

    return true;
}

bool KMiniGameRankManager::GetUserRank( IN char cType_, IN DWORD dwUID_, OUT KMiniGameRankInfo& kInfo_ )
{
    KLocker lock( m_csRank );
    kInfo_.m_nRank = -1;
    std::vector<KMiniGameRankInfo>* pVecRank = NULL;
    _JIF( GetRankVector( m_mapTotalRank, cType_, &pVecRank ), return false );
    _JIF( pVecRank, return false );

    //////////////////////////////////////////////////////////

    if( GetUserRank( *pVecRank, kInfo_ ) == false )
    {
        return false;
    }

    //////////////////////////////////////////////////////////

    //std::vector<KMiniGameRankInfo>::iterator vit;
    //vit = std::lower_bound( pVecRank->begin(), pVecRank->end(), dwUID_, Sort_By_UID() );
    //if( vit == pVecRank->end() ||
    //    (vit->m_dwUserUID < dwUID_ || vit->m_dwUserUID > dwUID_) ) // 동등성 검사를 한다.
    //{
    //    std::clog << "GetUserRank 에서 동등성 검사 실패, vit->m_dwUserUID :" << vit->m_dwUserUID << ", uid(parameter):"<< dwUID_ <<ENDL;
    //    DumpUID( cType_ );
    //    return false;
    //}

    //kInfo_.m_nRank = vit->m_nRank;

    //START_LOG( clog, L"UID : " << dwUID_ <<"유저의 랭킹 찾기 : " << kInfo_.m_nRank << ", vit->m_dwUserUID" << vit->m_dwUserUID  ) << END_LOG;

    //kInfo_ = *vit;
   
    //DumpMiniRank();
    //DumpUID( cType_ );

    return true;
}

void KMiniGameRankManager::AddRank( IN char cType_, IN OUT KMiniGameRankInfo& kInfo_ )
{
    KLocker lock( m_csRank );
    std::vector<KMiniGameRankInfo>* pVecRank;
    JIF( GetRankVector( m_mapTotalRank, cType_, &pVecRank ) );
    JIF( pVecRank );
    if( AddGameRank( *pVecRank, kInfo_ ) ) // pVecRank 는 m_mapTotalRank 에서  cType_ 에 해당하는 미니게임 랭크 벡터
    {
        m_mapTotalUser = m_mapTotalRank; // 동기화

        std::vector<KMiniGameRankInfo>* pVecUIDRank;
        JIF( GetRankVector( m_mapTotalUser, cType_, &pVecUIDRank ) );
        JIF( pVecUIDRank );
        pVecUIDRank->push_back( kInfo_ );
        BuildRankByUID( *pVecUIDRank ); // pVecUIDRank 이건 또..?
    }
    
    //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    //    DumpMiniRank();
}

void KMiniGameRankManager::UpdateRank( IN std::map<char, std::vector<KMiniGameRankInfo> >& m_mapTotalRank_ )
{
    KLocker lock( m_csRank );
    m_mapTotalRank = m_mapTotalRank_;
    BuildRank();

    //if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    //    DumpMiniRank();
}

void KMiniGameRankManager::SetUpdateGap( IN DWORD dwUpdateGap_ )
{
    JIF( dwUpdateGap_ > 1000 );
    KLocker lock(m_csTickInfo);
    m_dwUpdateGap = dwUpdateGap_;
}

bool KMiniGameRankManager::CheckUpdateTime()
{
    const DWORD dwCurrentTick = ::GetTickCount();
    KLocker lock(m_csTickInfo);
    if( dwCurrentTick - m_dwLastUpdateTick > m_dwUpdateGap )
    {
        m_dwLastUpdateTick = dwCurrentTick;
        return true;
    }

    return false;
}

bool KMiniGameRankManager::SendLocalRankToCenter()
{
    KLocker lock( m_csRank );
    // Send...
    KMiniGameRankList kPacket;
    kPacket.m_mapRank = m_mapTotalRank;
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_MINIGAME_RANK_REQ, kPacket );

    //DumpMiniRank();

    return true;
}

void KMiniGameRankManager::Tick()
{
    if( CheckUpdateTime() )
    {
        LIF( SendLocalRankToCenter() );
    }
}

void KMiniGameRankManager::BuildRank()
{
    std::map<char, std::vector<KMiniGameRankInfo> >::iterator mit;
    for( mit = m_mapTotalRank.begin() ; mit != m_mapTotalRank.end() ; ++mit )
    {
        BuildRankByScore( mit->second ); // 이안에서 넘버링 까지 한다.
    }

    // 넘버링 된 새로 갱신된 랭크를 UID로 정렬된 랭크에 할당한다.
    m_mapTotalUser = m_mapTotalRank;

    for( mit = m_mapTotalUser.begin() ; mit != m_mapTotalUser.end() ; ++mit )
    {
        BuildRankByUID( mit->second );
    }
}

void KMiniGameRankManager::BuildRankByScore( IN OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    std::sort( vecRank_.begin(), vecRank_.end(), Sort_By_Score() );
    std::for_each( vecRank_.begin(), vecRank_.end(), KRankNumbering() ); // 득점으로 넘버링을 한다.
}

void KMiniGameRankManager::BuildRankByUID( IN OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    // In Param 으로 들어온 녀석은 고득점으로 소팅된 상태다.
    // 고 득점으로 소팅 된 녀석의 순서가 바뀌지 않도록 하기 위해서 stable_sort 사용 한다.
    std::stable_sort( vecRank_.begin(), vecRank_.end(), Sort_By_UID() );
}

void KMiniGameRankManager::Clipping( IN DWORD dwSize_, IN OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    if( vecRank_.empty() ) return;
    
    if( vecRank_.size() <= dwSize_ ) return;

    vecRank_.erase( vecRank_.begin() + dwSize_ , vecRank_.end() );
}

bool KMiniGameRankManager::AddGameRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank_, IN OUT KMiniGameRankInfo& kInfo_ )
{
    kInfo_.m_nRank = -1;

    if( kInfo_.m_dwScore <= 0 ) // 0점 이하는 랭킹에 안끼워줍니다
    {
        return false;
    }

    if( kInfo_.m_dwScore > 9999 ) // 해킹 유저의 과도한 점수를 제한
    {
        kInfo_.m_dwScore = 9999;
    }

    if( !vecRank_.empty() )
    {
        std::vector<KMiniGameRankInfo>::iterator vit = vecRank_.end() -1; // 순위권 중 마지막 
       
        //START_LOG( cwarn, L"랭크 마지막 순위의 유저 : " << vit->m_strNick << ", 점수 : " << vit->m_dwScore << " , Rank :  " << vit->m_nRank  ) << END_LOG;

        if( vit->m_dwScore >= kInfo_.m_dwScore && vecRank_.size() >= 100 ) // 순위권의 마지막 점수 이하면 무시, 그러나 랭킹이 100 개가 되지 않았다면 추가해야지
            return false;
        
        vit = std::find_if( vecRank_.begin(), vecRank_.end(), 
                boost::bind(&KMiniGameRankInfo::m_dwUserUID, _1 ) == kInfo_.m_dwUserUID ); // 같은 유저의 기록이 있다면, 동일 유저(자신) 최고 기록보다 더 높은 점수여야 갱신이 됨
                
        if( vit != vecRank_.end() )
        {
            if( vit->m_dwScore >= kInfo_.m_dwScore )
            {
                //std::clog << "랭킹 안에 동일 유저 있으나 최고 점수 갱신 못함, " << kInfo_.m_strNick << ", 점수 " << kInfo_.m_dwScore <<ENDL;
                return false;
            }

            *vit = kInfo_;
            //std::clog << "랭킹안에 있는 유저 정보갱신, " << vit->m_strNick << ", 점수 " << vit->m_dwScore <<ENDL;

        }
        else
        {
            START_LOG( clog, L"새로운 유저 데이터를 미니게임 랭킹에 추가, " << kInfo_.m_strNick  ) << END_LOG;
            vecRank_.push_back( kInfo_ );
        }
    }
    else //if( vecRank_.size() < 100 )
    {
        vecRank_.push_back( kInfo_ ); // 최초 1회만 불려질거 같음
    }

    BuildRankByScore( vecRank_ ); // 점수로 정렬해서, 등수 부여

    std::vector<KMiniGameRankInfo>::iterator vit;

    vit = std::find_if( vecRank_.begin(), vecRank_.end(), 
        boost::bind(&KMiniGameRankInfo::m_dwUserUID, _1 ) == kInfo_.m_dwUserUID );

    if( vit == vecRank_.end() || vit->m_nRank < -1 || vit->m_nRank > 100 )
    {
        return false;
    }

    kInfo_.m_nRank = vit->m_nRank; // 이번에 기록된 점수로 구한 랭킹..

    return true;
}

bool KMiniGameRankManager::GetRankVector( IN std::map<char, std::vector<KMiniGameRankInfo> >& mapRank_, IN char cType_, OUT std::vector<KMiniGameRankInfo>** ppvecRank_ )
{
    *ppvecRank_ = NULL;
    std::map<char, std::vector<KMiniGameRankInfo> >::iterator mit;
    mit = mapRank_.find( cType_ );
    if( mit == mapRank_.end() )
    {
        std::pair< std::map<char, std::vector<KMiniGameRankInfo> >::iterator, bool > prib;
        prib = mapRank_.insert( std::make_pair(cType_,std::vector<KMiniGameRankInfo>()) );
        _JIF( prib.second, return false );
        mit = prib.first;
    }

    *ppvecRank_ = &mit->second; // 주소를 넣는?
    return true;
}

void KMiniGameRankManager::DumpMiniRank()
{
    //KLocker lock( m_csRank );

    //std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit;
    //std::vector<KMiniGameRankInfo>::iterator vit;

    //for( mit = m_mapTotalRank.begin(); mit != m_mapTotalRank.end() ; ++mit )
    //{
    //    for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
    //    {
    //        std::cout << L"GameMode : " << static_cast<int>(vit->m_cGameMode) << ", Nick : " << vit->m_strNick
    //            << L", Score : " << vit->m_dwScore << ", Rank : " << vit->m_nRank << dbg::endl;
    //    }
    //}
}

std::wostream& operator<<( std::wostream& stm_, const KMiniGameRankInfo& data_ )
{
    return stm_ << data_.m_dwUserUID;
}

void KMiniGameRankManager::DumpUID( char cType_ )
{
    //KLocker lock( m_csRank );
    //std::map<char, std::vector<KMiniGameRankInfo> >::iterator mit;
    //mit = m_mapTotalUser.find( cType_ );
    //if( mit == m_mapTotalUser.end() )
    //    return;

    //std::wstringstream stm;
    //std::copy( mit->second.begin(), mit->second.end(), 
    //    std::ostream_iterator< std::vector<KMiniGameRankInfo>::value_type, wchar_t>( stm , L", " ) );

    //std::cout << stm.str() << dbg::endl;

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KMiniGameRankManager::GetUserRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank_, IN OUT KMiniGameRankInfo& kInfo_ )
{
    std::vector<KMiniGameRankInfo>::iterator vit;

    vit = std::find_if( vecRank_.begin(), vecRank_.end(), 
        boost::bind(&KMiniGameRankInfo::m_dwUserUID, _1 ) == kInfo_.m_dwUserUID );

    if( vit == vecRank_.end() || vit->m_nRank < -1 || vit->m_nRank > 100 )
    {
        return false;
    }

    kInfo_.m_nRank = vit->m_nRank; // top 100 안에  user uid 로 검색한 랭크

    return true;
}