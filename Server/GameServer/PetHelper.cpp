
#include "PetHelper.h"
#include "ResultManager.h"
#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include "IPAdvantage.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KPetHelper );
NiImplementRootRTTI( KPetHelper );

KPetHelper::KPetHelper(void) : m_iMaxFeed(0), m_nMaxDecreseMinnute(0), m_nMinDecreseMinute(INT_MAX)
{

}

KPetHelper::~KPetHelper(void)
{

}

ImplOstmOperatorW2A( KPetHelper );
ImplToStringW( KPetHelper )
{
    int nMaxFeed;
    DWORD dwPetFeedTableSize;
    DWORD dwPetExpInfoSize;
    DWORD dwDecreseTableSize;
    DWORD dwAssistTableSize;
    DWORD dwPanaltyTableSize;
    DWORD dwPetBonusInfo;
    { KLocker lock( m_csMaxFeed ); nMaxFeed = m_iMaxFeed; }
    { KLocker lock( m_csPetFeedTable ); dwPetFeedTableSize  = m_mapPetFeedTable.size(); }
    { KLocker lock( m_csPetExpInfo ); dwPetExpInfoSize      = m_vecPetExpInfo.size(); }
    { KLocker lock( m_csDecreseTable ); dwDecreseTableSize  = m_mapDecreseTable.size(); }
    { KLocker lock( m_csPetAssist ); dwAssistTableSize      = m_mapAssistTable.size(); }
    { KLocker lock( m_csPanaltyTable ); dwPanaltyTableSize  = m_vecPanaltyTable.size(); }
    { KLocker lock( m_csPetExpInfo ); dwPetBonusInfo        = m_vecPetBonusInfo.size(); }

    START_TOSTRINGW
        << L" Max Feed ( " << nMaxFeed << L" )" << std::endl
        << L" Feed Factor Size    : " << dwPetFeedTableSize << std::endl
        << L" ExpInfo Size        : " << dwPetExpInfoSize << std::endl
        << L" Decrese Table Size  : " << dwDecreseTableSize << std::endl
        << L" Assist Table Size   : " << dwAssistTableSize << std::endl
        << L" Panalty Table Size  : " << dwPanaltyTableSize << std::endl
        << L" PetBonus Table Size : " << dwPetBonusInfo << std::endl;

    return stm_;
}

void KPetHelper::AddPetFeed( IN const int iItemID_, IN const int iFeedFactor_ )
{
   JIF( iItemID_ > 0 && iFeedFactor_ > 0 );

    {
        KLocker lock( m_csPetFeedTable );    
        //std::map<int,KFeedFactor>::iterator mit;
        //mit = m_mapPetFeedTable.find( iItemID_ );
        //JIF( mit == m_mapPetFeedTable.end() ); //호원아 != 이게 아니래 땡큐
        // 2007/1/10. iridology. map.Insert 시에는 중복되면 insert 안되므로 괜찮을듯
        KFeedFactor kFeedFactor;
        kFeedFactor.m_iItemID       = iItemID_;
        kFeedFactor.m_iFeedFactor   = iFeedFactor_;
        m_mapPetFeedTable.insert( std::make_pair( iItemID_, kFeedFactor ) );
    }
}

bool KPetHelper::GetPetFeedFactor( IN int iItemID_, OUT int& iFeedFactor_ )
{
    iFeedFactor_ = 0;
    _JIF( iItemID_ > 0 , return false );

    std::map<int,KFeedFactor>::iterator mit;
    {
        KLocker lock( m_csPetFeedTable );
        mit = m_mapPetFeedTable.find( iItemID_ );
        _JIF( mit != m_mapPetFeedTable.end(), return false );
        iFeedFactor_ = mit->second.m_iFeedFactor;
    }
    return true;
}

void KPetHelper::ClearPetFeed()
{
    int nCurrentSize;
    {
        KLocker lock( m_csPetFeedTable );
        nCurrentSize = m_mapPetFeedTable.size();
        m_mapPetFeedTable.clear();
    }

    std::cout << "-- Feed Table Clear --" << std::endl
        << "Current Size : " << nCurrentSize << std::endl;
}

void KPetHelper::PrintPetFeed()
{
    std::stringstream stm;

    stm << "-- Print Feed Table --" << std::endl;

    {
        KLocker lock( m_csPetFeedTable );
        std::map<int, KFeedFactor>::iterator mit;
        for( mit = m_mapPetFeedTable.begin() ; mit != m_mapPetFeedTable.end() ; ++mit )
        {
            stm << "Item ID : " << mit->second.m_iItemID << " , Feed Factor : " << mit->second.m_iFeedFactor << std::endl;
        }

        stm << "    Feed Table size : " << m_mapPetFeedTable.size() << std::endl;
        stm << "--- Feed Table End ---" << std::endl;
    }

    std::cout << stm.str();
}

void KPetHelper::AddExpInfo( IN const int nLv_, IN const int nWin_, IN const int nLose_, IN const int nTotalExp_ )
{
    KPetExpInfo kPetExpInfo;
    kPetExpInfo.m_nLv       = nLv_;
    kPetExpInfo.m_nWin      = nWin_;
    kPetExpInfo.m_nLose     = nLose_;
    kPetExpInfo.m_nTotalExp = nTotalExp_;

    {
        KLocker lock( m_csPetExpInfo );
        m_vecPetExpInfo.push_back( kPetExpInfo );
    }
}

void KPetHelper::SortExpInfo()
{
    KLocker lock( m_csPetExpInfo );
    std::sort(m_vecPetExpInfo.begin(), m_vecPetExpInfo.end(),
        boost::bind(&KPetExpInfo::m_nLv, _1) < boost::bind(&KPetExpInfo::m_nLv, _2) );
}

void KPetHelper::ClearExpInfo()
{
    KLocker lock( m_csPetExpInfo );
    m_vecPetExpInfo.clear();
}

int KPetHelper::GetPetLevel( IN int nTotalExp_ ) // 레벨 얻기..
{
	//이미 정렬 되어 있는 상태이다.
	std::vector<KPetExpInfo>::iterator vit;
    KLocker lock( m_csPetExpInfo );
	vit = std::find_if( m_vecPetExpInfo.begin(), m_vecPetExpInfo.end(),
			boost::bind( &KPetExpInfo::m_nTotalExp, _1 ) > nTotalExp_ );

    if( vit != m_vecPetExpInfo.begin() )
        --vit;

    return vit->m_nLv; //이러면 끝인것이다.
}


int KPetHelper::GetRewardExpByLv( IN const int nLevel_, IN const bool bWin_, IN const float fRewardRatio_, IN const DWORD dwPetID_, IN const char cPetPromotion_, IN const bool bPCBang_ ) // 이번에 얻게 되는 경험치
{
    //이미 정렬 되어 있는 상태이다.
    std::vector<KPetExpInfo>::iterator vit;
    KLocker lock( m_csPetExpInfo );

    // Lv이 nLevel_인 녀석을 찾는다
    vit = std::find_if( m_vecPetExpInfo.begin(), m_vecPetExpInfo.end(), 
        boost::bind( &KPetExpInfo::m_nLv, _1 ) == nLevel_ );

    int nReward = 0;
    if( bWin_ )
        nReward = vit->m_nWin;
    else
        nReward = vit->m_nLose;

    float fTimeEventBonus = GetPetExpBonusRatio( dwPetID_, (int)cPetPromotion_ );
    float fPCBangBonus =0.f;
    if( bPCBang_ ) {
        fPCBangBonus = SiKIPAdvantage()->GetPetExpRatio( KIPAdvantage::PCBANG_PUBLISHER );
    }

    int nResult = static_cast<int>( nReward * ( fRewardRatio_ + fTimeEventBonus + fPCBangBonus ) );

    START_LOG( clog, L"Pet Reward Exp Info" )
        << BUILD_LOG( bWin_ )
        << BUILD_LOG( nReward )
        << BUILD_LOG( fRewardRatio_ )
        << BUILD_LOG( dwPetID_ )
        << BUILD_LOGc( cPetPromotion_ )
        << BUILD_LOG( nResult )
        << BUILD_LOG( fTimeEventBonus )
        << BUILD_LOG( bPCBang_ )
        << BUILD_LOG( fPCBangBonus ) << END_LOG;

    return nResult;
}

bool KPetHelper::IsMaxLevel( IN DWORD dwExp_ )
{
    // 이미 정렬된 상태이다.
    // 마지막 요소를 가지고 오면 된다.
    DWORD dwMaxExp = 0xffffffff;
    {
        KLocker lock( m_csPetExpInfo );
        if( m_vecPetExpInfo.empty() )
            return false;
        dwMaxExp = m_vecPetExpInfo.rbegin()->m_nTotalExp;
    }
    return dwMaxExp <= dwExp_;
}

bool KPetHelper::GetInitExpInfo( OUT KPetExpInfo& kPetExpInfo_ )
{
    // 이미 정렬된 상태이다.
    KLocker lock( m_csPetExpInfo );
    if( m_vecPetExpInfo.empty() )
        return false;
    
    kPetExpInfo_ = *m_vecPetExpInfo.begin();
    return true;
}

bool KPetHelper::GetMaxExpInfo( OUT KPetExpInfo& kPetExpInfo_ )
{
    // 이미 정렬된 상태이다.
    KLocker lock( m_csPetExpInfo );
    if( m_vecPetExpInfo.empty() )
        return false;

    kPetExpInfo_ = *m_vecPetExpInfo.rbegin();
    return true;
}

void KPetHelper::AddDecreseRate( IN const int nPlayMin_, IN const int nDecreseSatiation_ )
{
    JIF( nPlayMin_ > 0 && nDecreseSatiation_ < 0 );

    // Min/Max 설정
    if( nPlayMin_ < m_nMinDecreseMinute )
        m_nMinDecreseMinute = nPlayMin_;
    if( nPlayMin_ > m_nMaxDecreseMinnute )
        m_nMaxDecreseMinnute = nPlayMin_;

    KLocker lock( m_csDecreseTable );
    m_mapDecreseTable.insert( std::make_pair( nPlayMin_, nDecreseSatiation_ ) );
}

void KPetHelper::ClearDecreseTable()
{
    int nDecreseTableSize;
    {
        KLocker lock( m_csDecreseTable );
        nDecreseTableSize = (int)m_mapDecreseTable.size();
        m_mapDecreseTable.clear();
    }

    std::cout << "-- Decrese Table Clear --" << std::endl
        << "Current Size : " << nDecreseTableSize << std::endl;
}

//{{ 2007/1/11. iridology.
int KPetHelper::GetDecreseSatiation( IN int nPlayTime )
{
    KLocker lock( m_csDecreseTable );

    // 바운드 설정
    if( nPlayTime < m_nMinDecreseMinute )
        nPlayTime = m_nMinDecreseMinute;
    if( nPlayTime > m_nMaxDecreseMinnute )
        nPlayTime = m_nMaxDecreseMinnute;

    std::map< int, int >::iterator mit = m_mapDecreseTable.find( nPlayTime );
    // 없으면 최대 패널티
    if( mit == m_mapDecreseTable.end() )
    {
        mit--;
    }

    return mit->second;
}
//}}}

void KPetHelper::PrintDecreseSatiation()
{
    std::stringstream stm;

    stm << "-- Print Decrese Satiation Table -- " << std::endl;

    {
        KLocker lock( m_csDecreseTable );
        std::map< int, int >::iterator mit;
        for( mit = m_mapDecreseTable.begin() ; mit != m_mapDecreseTable.end() ; ++mit )
        {
            stm << " PlayTime : " << mit->first << " , Decrese Factor : " << mit->second << std::endl;
        }

        stm << "    Minimum Decrese Minute : " << m_nMinDecreseMinute << std::endl;
        stm << "    Maximum Decrese Minute : " << m_nMaxDecreseMinnute << std::endl;
        stm << "    Decrese Table size : " << m_mapDecreseTable.size() << std::endl;
        stm << "--- Decrese Satiation Table End ---" << std::endl;
    }

    std::cout << stm.str();
}

void KPetHelper::ClearAssistItemTable()
{
    int nCurrentSize;
    {
        KLocker lock( m_csPetAssist );
        nCurrentSize = m_mapAssistTable.size();
        m_mapAssistTable.clear();
    }

    std::cout << "-- Assist Table Clear --" << std::endl
        << "Current Size : " << nCurrentSize << std::endl;
}

void KPetHelper::PrintAssistItemTable()
{
    std::stringstream stm;

    stm << "-- Print Assist Item Table -- " << std::endl;

    {
        KLocker lock( m_csPetAssist );
        std::map< int, KAssistFactor >::iterator mit;
        for( mit = m_mapAssistTable.begin() ; mit != m_mapAssistTable.end() ; ++mit )
        {
            stm << " Item ID : " << mit->first << " , Exp Factor : " << mit->second.m_fExpAssist << std::endl;
        }

        stm << "    Assist Item size : " << m_mapAssistTable.size() << std::endl;
        stm << "--- Assist Item Table End ---" << std::endl;
    }

    std::cout << stm.str();
}

void KPetHelper::AddAssistItem( IN const int iItemID_, IN const float fExpAssistFacor_ )
{
    JIF( iItemID_ > 0 && fExpAssistFacor_ > 0.0f/* && fGpAssistFactor > 0.0f*/ );

    {
        KLocker lock( m_csPetAssist );
        KAssistFactor kAssistFactor;
        kAssistFactor.m_iItemID       = iItemID_;
        kAssistFactor.m_fExpAssist    = fExpAssistFacor_;
        m_mapAssistTable.insert( std::make_pair( iItemID_, kAssistFactor ) );
    }
}

bool KPetHelper::GetPetExpAssistFactor( IN int iItemID_, OUT float& fExpAssistFacor_ )
{
    if( iItemID_ <= 0 ) return false;

    std::map< int, KAssistFactor >::iterator mit;
    {
        KLocker lock( m_csPetAssist );
        mit = m_mapAssistTable.find( iItemID_ );

        if( mit == m_mapAssistTable.end() ) return false;

        if( mit->second.m_fExpAssist > fExpAssistFacor_ )
            fExpAssistFacor_ = mit->second.m_fExpAssist;
    }
    return true;
}

DWORD KPetHelper::GetPetMinExp()
{
    KLocker lock( m_csPetExpInfo );
    if( m_vecPetExpInfo.empty() )
    {
        START_LOG( cerr, L"아직 펫 경험치 정보가 로딩 되지 訪弩?.." ) << END_LOG;
        return 100;
    }
    return (DWORD)m_vecPetExpInfo.begin()->m_nTotalExp;

}

void KPetHelper::SetMaxFeed( IN const int iMaxFeed )
{
    JIF( iMaxFeed > 0 );

    KLocker lock( m_csMaxFeed );
    int iPrevFeed = m_iMaxFeed;
    m_iMaxFeed = iMaxFeed;

    std::cout << " MaxFeed Change from " << iPrevFeed << " to " << m_iMaxFeed << std::endl;
}

void KPetHelper::AddExpPanalty( IN const int iFeed, IN const float fPanalty )
{
    JIF( iFeed >= 0 && fPanalty <= 1.0f );

    KPanaltyFactor kPanelty;
    kPanelty.m_iLimitFeed = iFeed;
    kPanelty.m_fPanaltyFactor = fPanalty;

    {
        KLocker lock( m_csPanaltyTable );
        m_vecPanaltyTable.push_back( kPanelty );
    }
}

void KPetHelper::PrintExpPanaltyTable()
{
    std::stringstream stm;

    stm << "-- Print Panalty Table -- " << std::endl;

    {
        KLocker lock( m_csPanaltyTable );
        std::vector< KPanaltyFactor >::iterator vit;
        for( vit = m_vecPanaltyTable.begin() ; vit != m_vecPanaltyTable.end() ; ++vit )
        {
            stm << " Feed : " << (*vit).m_iLimitFeed << " , Panalty Factor : " << (*vit).m_fPanaltyFactor << std::endl;
        }

        stm << "    Panalty size : " << m_vecPanaltyTable.size() << std::endl;
        stm << "--- Panalty Table End ---" << std::endl;
    }

    std::cout << stm.str();
}

void KPetHelper::ClearExpPanaltyTable()
{
    int nCurrentSize;
    {
        KLocker lock( m_csPanaltyTable );
        nCurrentSize = m_vecPanaltyTable.size();
        m_vecPanaltyTable.clear();
    }

    std::cout << "-- Panalty Table Clear --" << std::endl
        << "Current Size : " << nCurrentSize << std::endl;
}

void KPetHelper::SortExpPanaltyTable()
{
    KLocker lock( m_csPanaltyTable );
    std::sort( m_vecPanaltyTable.begin(), m_vecPanaltyTable.end(),
        boost::bind(&KPanaltyFactor::m_iLimitFeed, _1) < boost::bind(&KPanaltyFactor::m_iLimitFeed, _2) );
}

float KPetHelper::GetExpPanalty( IN int iFeed ) const
{
    _JIF( iFeed <= m_iMaxFeed, return 1.0f );

    // 이미 정렬 되어 있는 상태
    std::vector< KPanaltyFactor >::const_iterator vit;
    KLocker lock( m_csPanaltyTable );

    // Panalty Table 에서 iFeed보다 큰 첫번째 녀석을 찾는다.
    vit = std::find_if( m_vecPanaltyTable.begin(), m_vecPanaltyTable.end(),
        boost::bind( &KPanaltyFactor::m_iLimitFeed, _1 ) >= iFeed );

    if( vit == m_vecPanaltyTable.end() )
        --vit;
    return vit->m_fPanaltyFactor;
}

int KPetHelper::GetMonsterCrusadersRewardExp( IN const int nPetLv_, IN const bool bWin_, IN int nKillNum_, IN const float fRewardRatio_, IN const DWORD dwPetID_, IN const char cPetPromotion_, IN const bool bPCBang_ )
{
    // 2007/1/11. iridology. 몬스터 원정대 경험치 보상
    // 몬스터 마리수 * 펫 레벨 보상 exp / 5
    if( !bWin_ ) return 0;
    _JIF( nPetLv_ >= 0, return 0 );

    //070220. ZzangDoL. 몬스터 죽인수가 5마리 이상이면 5마리로 고정..MAX 5마리
    if( nKillNum_ > 5) nKillNum_ = 5;

    int nRewardExp = GetRewardExpByLv( nPetLv_, bWin_, fRewardRatio_, dwPetID_, cPetPromotion_, bPCBang_ ) * nKillNum_;

    return static_cast< int >( std::ceil( nRewardExp * 0.2f ) );
}

int KPetHelper::GetMonsterHuntRewardExp( IN const int nPetLv_, IN const bool bWin_, IN const int nDifficulty_, IN float fRewardRatio_, IN const DWORD dwPetID_, IN const char cPetPromotion_, IN const bool bPCBang_ )
{
    // 2007/1/11. iridology. 몬스터 대전 경험치 보상 방식
    // 패배 -> 0
    // 승리 ->  펫 레벨 <= 난이도 : 그대로
    //          펫 레벨 > 난이도 : 펫 레벨 = 난이도 의 보상
    if( !bWin_ ) return 0;
    _JIF( nDifficulty_ >= 0 && nPetLv_ >= 0, return 0 );
    return GetRewardExpByLv( ( nPetLv_ < nDifficulty_ ? nPetLv_ : nDifficulty_ ), bWin_, fRewardRatio_, dwPetID_, cPetPromotion_, bPCBang_ );
}


void KPetHelper::ClearPromotionPanaltyTable()
{
    KLocker lock( m_csPromotionPanaltyTable );
    m_mapPromotionPanaltyTable.clear();
}

void KPetHelper::AddPromotionPanalty( const char cPromotion_, const float fPanaltyRate_ )
{
    KLocker lock( m_csPromotionPanaltyTable );
    m_mapPromotionPanaltyTable.insert( std::make_pair( cPromotion_, fPanaltyRate_ ) );
}

float KPetHelper::GetPromotionPanalty( const char cPromotion_ )
{
    float fRet = 0.f;
    std::map< char, float >::const_iterator mit;
    {
        KLocker lock( m_csPromotionPanaltyTable );
        mit = m_mapPromotionPanaltyTable.find( cPromotion_ );
        if( mit != m_mapPromotionPanaltyTable.end() )
            fRet = mit->second;
    }
    return fRet;
}

void KPetHelper::PrintPromotionPanaltyTable()
{
    std::cout << "-- PrintPromotionPanaltyTable --" << std::endl;
    std::map< char, float >::const_iterator mit;
    KLocker lock( m_csPromotionPanaltyTable );
    for( mit = m_mapPromotionPanaltyTable.begin() ; mit != m_mapPromotionPanaltyTable.end() ; ++mit )
        std::cout << " Promotion : " << static_cast<int>(mit->first) << ", Panalty : " << mit->second << std::endl;
}

void KPetHelper::AddTransformItem( IN int iItemID_ , IN int iMinLv_ , IN int iMaxLv_, IN int iEvolutionType_ )
{
    {
        KLocker lock( m_csTransformItem );
        KTransformLv kTransformLv;
        kTransformLv.m_iMinLevel = iMinLv_;
        kTransformLv.m_iMaxLevel = iMaxLv_;
        kTransformLv.m_iEvolutionType = iEvolutionType_;
        
        m_mapTransformItem.insert( std::make_pair( iItemID_, kTransformLv ) );
    }
}

int KPetHelper::GetEvolutionType( IN int iItemID_ )
{
    KLocker lock( m_csTransformItem );
    std::map< int, KTransformLv >::const_iterator mit;
    mit = m_mapTransformItem.find( iItemID_ );

    _JIF( mit != m_mapTransformItem.end(), return -1 );
    return mit->second.m_iEvolutionType;
}

bool KPetHelper::GetTransformLv( IN int iItemID_, OUT int& iTransformLv_ )
{
    KLocker lock( m_csTransformItem );
    std::map< int, KTransformLv >::const_iterator mit;
    mit = m_mapTransformItem.find( iItemID_ );
    _JIF( mit != m_mapTransformItem.end(), return false);
    iTransformLv_ = mit->second.m_iMinLevel;
    return true;
}

void KPetHelper::ClearTransformItem()
{
    KLocker lock( m_csTransformItem );
    m_mapTransformItem.clear();
}

// 부화 관련
void KPetHelper::AddHatchingInfo( const GCITEMID nItemID_, const int nMinLv_, const int nNextID_ )
{
    KHatchingInfo kHatchingInfo;
    KLocker lock( m_csTransformItem );

    kHatchingInfo.m_nItemID = nItemID_;
    kHatchingInfo.m_nMinLv  = nMinLv_;
    kHatchingInfo.m_nNextID = nNextID_;

    m_mapHatchingInfoTable.insert( std::make_pair( nItemID_, kHatchingInfo ) );
}

void KPetHelper::ClearHatchingInfo()
{
    KLocker lock( m_csTransformItem );
    m_mapHatchingInfoTable.clear();
}

int KPetHelper::GetNextID( GCITEMID nItemID_ )
{
    std::map<int,KHatchingInfo>::iterator mit;
    KLocker lock( m_csTransformItem );
    mit = m_mapHatchingInfoTable.find( nItemID_ );

    if( mit != m_mapHatchingInfoTable.end() )
        return mit->second.m_nNextID;

    return -1;
}

void KPetHelper::PrintHatchingInfo()
{
    std::map<int,KHatchingInfo>::iterator mit;
    std::cout << "--- Hatching Info -- " << std::endl;
    KLocker lock( m_csTransformItem );
    for( mit = m_mapHatchingInfoTable.begin() ; mit != m_mapHatchingInfoTable.end() ; ++mit )
    {
        std::cout << "Egg ID : " << mit->second.m_nItemID
            << ", Lv : " << mit->second.m_nMinLv
            << ", Hatching ID : " << mit->second.m_nNextID << std::endl;
    }
}

bool KPetHelper::IsEggPet( IN GCITEMID nItemID_ )
{
    std::map<int,KHatchingInfo>::iterator mit;
    KLocker lock( m_csHatchingInfoTable );
    mit = m_mapHatchingInfoTable.find( nItemID_ );

    if( mit != m_mapHatchingInfoTable.end() )
    {
        return true;
    }

    return false;
}

int KPetHelper::GetHatchingLv( IN GCITEMID nItemID_ )
{
    std::map<int,KHatchingInfo>::iterator mit;
    KLocker lock( m_csHatchingInfoTable );
    mit = m_mapHatchingInfoTable.find( nItemID_ );
    if( mit != m_mapHatchingInfoTable.end() )
    {
        return mit->second.m_nMinLv;
    }
    return 100;
}

void KPetHelper::ClearPetCostume()
{
    KLocker lock( m_csPetCostume );
    m_mapPetCostume.clear();
}

void KPetHelper::AddPetCostume( int nPetID_, int nPetPromotion_, int nPetCostumeID_ )
{
    std::pair<DWORD,char> prKey( (DWORD)nPetID_, (char)nPetPromotion_ );
    DWORD dwPetCostumeID    = (DWORD)nPetCostumeID_;
    KLocker lock( m_csPetCostume );
    m_mapPetCostume[prKey].insert( dwPetCostumeID );
}

bool KPetHelper::IsPetCostume( DWORD dwPetID_, char petPromotion_, DWORD dwPetCostumeID_ )
{
    std::pair<DWORD,char> prKey( dwPetID_, petPromotion_ );
    std::map<std::pair<DWORD,char>, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;
    KLocker lock( m_csPetCostume );

    mit = m_mapPetCostume.find( prKey );
    if( mit == m_mapPetCostume.end() )
    {
        START_LOG( cwarn, L"해당 키의 펫 코스튬 정보가 없음. Key:<" << dwPetID_ << L"," << petPromotion_ << L">, PetCostumeID:" << dwPetCostumeID_ ) << END_LOG;
        return false;
    }

    sit = mit->second.find( dwPetCostumeID_ );
    if( sit == mit->second.end() )
    {
        START_LOG( cwarn, L"해당 ItemID의 펫 코스튬이 없음. Key:<" << dwPetID_ << L"," << petPromotion_ << L">, PetCostumeID:" << dwPetCostumeID_ ) << END_LOG;
        return false;
    }

    return true;
}

void KPetHelper::GetCoustumeList( std::map<std::pair<DWORD,char>, std::set<DWORD> >& mapPetCostume_ )
{
    KLocker lock( m_csPetCostume );
    mapPetCostume_ = m_mapPetCostume;
    
    START_LOG( clog, L"PetCostumeList Size : " <<  m_mapPetCostume.size() )
        << END_LOG;
}

void KPetHelper::DumpCostume( std::map<std::pair<DWORD,char>, std::set<DWORD> >& mapPetCostume_ )
{
    //std::map<std::pair<DWORD,char>, std::set<DWORD> >::iterator mit;

    //std::set<DWORD>::iterator sit;
    //KLocker lock( m_csPetCostume );
    //std::cout << L"Pet Costume List " << dbg::endl;
    //for( mit = mapPetCostume_.begin() ; mit != mapPetCostume_.end() ; ++mit )
    //{
    //    for( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit )
    //    {
    //        std::cout << L"Pet ID : " << mit->first.first << L",Promotion : " << static_cast<char>(mit->first.second) << L",Costume ID : " << *sit << dbg::endl;
    //    }
    //}
}

void KPetHelper::SetPetExpBonusInfo( IN const std::vector< KPetBonusInfo >& vecPetBonus_ )
{
    START_LOG( clog, L"Set Pet ExpBonus Info. size : " << vecPetBonus_.size() ) << END_LOG;

    KLocker lock( m_csPetExpInfo );
    m_vecPetBonusInfo = vecPetBonus_;
}

float KPetHelper::GetPetExpBonusRatio( IN const DWORD dwPetID_, IN const int nPromotion_ )
{
    std::vector< KPetBonusInfo >::const_iterator cvit;
    float fExpBonus = 0.f;

    KLocker lock( m_csPetExpInfo );
    for( cvit = m_vecPetBonusInfo.begin() ; cvit != m_vecPetBonusInfo.end() ; ++cvit )
    {
        // 펫 전체 적용(-1)
        if( cvit->m_nPetID == -1 )
        {
            // 진화체크 안하거나(-1), 이벤트대상 진화상태인 경우.
            if( cvit->m_nPromotion == -1 || cvit->m_nPromotion == nPromotion_ )
            {
                fExpBonus += cvit->m_fExpBonusRatio;
                continue;
            }
        }

        // 특정 펫 지정
        if( cvit->m_nPetID == dwPetID_ )
        {
            // 진화체크 안하거나(-1), 이벤트대상 진화상태인 경우.
            if( cvit->m_nPromotion == -1 || cvit->m_nPromotion == nPromotion_ )
            {
                fExpBonus += cvit->m_fExpBonusRatio;
                continue;
            }
        }
    }

    return fExpBonus;
}

void KPetHelper::DumpPetExpBonusInfo()
{
    //std::vector< KPetBonusInfo >::const_iterator cvit;

    //KLocker lock( m_csPetExpInfo );
    //std::cout << L"==== Start Dump Pet ExpBonus Info ====" << dbg::endl;
    //for( cvit = m_vecPetBonusInfo.begin() ; cvit != m_vecPetBonusInfo.end() ; ++cvit )
    //{
    //    std::cout << L" PetID : " << cvit->m_nPetID << L",Promotion : " << cvit->m_nPromotion << L",ExpBonusRatio : " << cvit->m_fExpBonusRatio << dbg::endl;
    //}
    //std::cout << L"==== End Dump Pet ExpBonus Info ====" << dbg::endl;
}

void KPetHelper::ClearPetVestedItem()
{
    KLocker lock( m_csPetVestedItem );
    m_vecPetVestedItem.clear();
}

void KPetHelper::AddPetVestedItem( IN GCITEMID ItemID_ )
{
    KLocker lock( m_csPetVestedItem );
    m_vecPetVestedItem.push_back( ItemID_ );
}

void KPetHelper::GetPetVestedItem( OUT std::vector<GCITEMID>& vecPetVestedItem_ )
{
    vecPetVestedItem_.clear();

    KLocker lock( m_csPetVestedItem );
    vecPetVestedItem_ = m_vecPetVestedItem;
}

bool KPetHelper::CheckPetVestedItem( IN const GCITEMID& ItemID_ )
{
    std::vector<GCITEMID>::iterator vit;
    KLocker lock( m_csPetVestedItem );
    vit = std::find( m_vecPetVestedItem.begin(), m_vecPetVestedItem.end(), ItemID_ );
    if( vit == m_vecPetVestedItem.end() ) {
        return false;
    }
    return true;
}