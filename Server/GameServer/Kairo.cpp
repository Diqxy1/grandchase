#include "Kairo.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

Kairo::Kairo()
:m_dwType(0)
{
}

Kairo::~Kairo()
{
}

ImplToStringW( Kairo )
{
    std::map<int,KDropItemInfo>::const_iterator mit;
    for( mit = m_mapPostItem.begin(); mit != m_mapPostItem.end(); mit++ )
    {
        stm_ << boost::wformat( L"  %d : %d, %d, %d \n" ) 
            % mit->first 
            % mit->second.m_ItemID
            % mit->second.m_nDuration
            % mit->second.m_nPeriod;
    }

    return m_kLottery.ToString( stm_ );
}

bool Kairo::SetPostItem( GCITEMID nItemID_, int nDuration_, int nPeriod_, float fProb_ )
{
    int iCaseID = m_mapPostItem.size();
    if ( false == m_kLottery.AddCase( iCaseID, fProb_ ) ) {
        START_LOG( cerr, L"ItemID : " << nItemID_ << L", " << nDuration_ << L", " << nPeriod_ << L", " << fProb_ ) << END_LOG;
        return false;
    }

    KDropItemInfo kUnit;
    kUnit.m_ItemID      = (GCITEMID)nItemID_;
    kUnit.m_nDuration   = nDuration_;
    kUnit.m_nPeriod     = nPeriod_;

    m_mapPostItem.insert( std::make_pair( iCaseID, kUnit ) );

    return m_mapPostItem.size() == m_kLottery.GetCaseNum();
}

bool Kairo::SetEnchantDropItem( IN GCITEMID nItemID_, IN int nDuration_, IN int nPeriod_, IN char cGradeID_, IN char cEnchantLevel_, IN float fProb_ )
{
    int iCaseID = m_mapPostItem.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    KDropItemInfo kUnit;
    kUnit.m_ItemID          = (GCITEMID)nItemID_;
    kUnit.m_nDuration       = nDuration_;
    kUnit.m_nPeriod         = nPeriod_;
    kUnit.m_cGradeID        = cGradeID_;
    kUnit.m_cEnchantLevel   = cEnchantLevel_;

    m_mapPostItem.insert( std::make_pair( iCaseID, kUnit ) );

    return m_mapPostItem.size() == m_kLottery.GetCaseNum();
}

bool Kairo::Do( OUT KDropItemInfo& kUnit_ ) const
{
    int nDecision = m_kLottery.Decision();

    if( nDecision == KLottery::CASE_BLANK )
    {
        kUnit_.m_ItemID = 0;
        return true;
    }

    std::map<int,KDropItemInfo>::const_iterator mit;
    mit = m_mapPostItem.find( nDecision );

    if( mit == m_mapPostItem.end() )
    {
        START_LOG( cerr, L"Kairo1" )
            << BUILD_LOG( nDecision )
            << BUILD_LOG( m_mapPostItem.size() )
            << BUILD_LOG( m_kLottery.GetCaseNum() ) << END_LOG;
        return false;
    }

    kUnit_ = mit->second;
    return true;
}

bool Kairo::DoAndRemove( OUT KDropItemInfo& kUnit_, OUT int& nDecision, OUT int& nMaxCase )
{
	nDecision = m_kLottery.Decision();

	if( nDecision == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"Kairo failed" )
			<< BUILD_LOG( nDecision )
			<< BUILD_LOG( m_mapPostItem.size() )
			<< BUILD_LOG( m_kLottery.GetCaseNum() )
			<< END_LOG;
		kUnit_.m_ItemID = 0;
		return true;
	}

	std::map<int,KDropItemInfo>::const_iterator mit;
	mit = m_mapPostItem.find( nDecision );

	if( mit == m_mapPostItem.end() )
	{
		START_LOG( cerr, L"Kairo2" )
			<< BUILD_LOG( nDecision )
			<< BUILD_LOG( m_mapPostItem.size() )
			<< BUILD_LOG( m_kLottery.GetCaseNum() ) << END_LOG;
		return false;
	}

	START_LOG( clog, L"Kairo information" )
		<< BUILD_LOG( nDecision )
		<< BUILD_LOG( m_mapPostItem.size() )
		<< BUILD_LOG( m_kLottery.GetCaseNum() ) 
		<< BUILD_LOG( mit->second.m_ItemID )
		<< END_LOG;

	nMaxCase = m_kLottery.GetCaseNum();
	kUnit_ = mit->second;
	return RemoveCase(nDecision);
}

void Kairo::Clear()
{
	m_mapPostItem.clear();
	m_mapPostType.clear();
	m_mapPostPrData.clear();
	m_mapPostVecItem.clear();
	m_kLottery.Clear();
}

void Kairo::GetItemList( OUT std::set<GCITEMID>& setRewardItems_ )
{
    setRewardItems_.clear();
    std::map<int,KDropItemInfo>::iterator mit;
    for( mit = m_mapPostItem.begin() ; mit != m_mapPostItem.end() ; ++mit )
    {
        setRewardItems_.insert( mit->second.m_ItemID );
    }
}

void Kairo::GetItemList( OUT std::vector<GCITEMID>& vecRewardItems_ )
{
    vecRewardItems_.clear();
    vecRewardItems_.reserve( m_mapPostItem.size() );
    std::map<int,KDropItemInfo>::iterator mit;
    for( mit = m_mapPostItem.begin() ; mit != m_mapPostItem.end() ; ++mit )
    {
        vecRewardItems_.push_back( mit->second.m_ItemID );
    }
}

void Kairo::GetItemList( OUT std::vector<KDropItemInfo>& vecRewardItems_ )
{
    vecRewardItems_.clear();
    vecRewardItems_.reserve( m_mapPostItem.size() );
    std::map<int,KDropItemInfo>::iterator mit;
    for( mit = m_mapPostItem.begin() ; mit != m_mapPostItem.end() ; ++mit )
    {
        vecRewardItems_.push_back( mit->second );
    }
}

bool Kairo::SetPostType( DWORD dwType_, float fProb_ )
{
    int iCaseID = m_mapPostType.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    m_mapPostType.insert( std::make_pair( iCaseID, dwType_ ) );

    return m_mapPostType.size() == m_kLottery.GetCaseNum();
}

bool Kairo::Do( OUT DWORD& dwType_ ) const
{
    int nDecision = m_kLottery.Decision();

    if( nDecision == KLottery::CASE_BLANK ) {
        dwType_ = 0;
        return true;
    }

    std::map<int,DWORD>::const_iterator mit;
    mit = m_mapPostType.find( nDecision );

    if( mit == m_mapPostType.end() ) {
        START_LOG( cerr, L"Kairo3" )
            << BUILD_LOG( nDecision )
            << BUILD_LOG( m_mapPostType.size() )
            << BUILD_LOG( m_kLottery.GetCaseNum() ) << END_LOG;
        return false;
    }

    dwType_ = mit->second;
    return true;
}

bool Kairo::GetMaxCase( IN int nExceptCaseID_, OUT int& nMaxCaseID_ )
{
    if( m_kLottery.GetMaxCaseID( nExceptCaseID_, nMaxCaseID_ ) ) {
        return true;
    }
    return false;
}

bool Kairo::DecreaseCaseRatio( IN int nCaseID_, IN float fRatio_ )
{
    if( m_kLottery.DecreaseCase( nCaseID_, fRatio_ ) ) {
        return true;
    }
    return false;
}

bool Kairo::SetPostType( IN std::pair<int,float> prData_, IN float fProb_ )
{
    int iCaseID = m_mapPostPrData.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    m_mapPostPrData.insert( std::make_pair( iCaseID, prData_ ) );

    return m_mapPostPrData.size() == m_kLottery.GetCaseNum();
}

int Kairo::Do( OUT std::pair<int,float>& prData_ ) const
{
    int nDecision = m_kLottery.Decision();

    if( nDecision == KLottery::CASE_BLANK ) {
        return nDecision;
    }

    std::map<int,std::pair<int,float>>::const_iterator mit;
    mit = m_mapPostPrData.find( nDecision );

    if( mit == m_mapPostPrData.end() ) {
        START_LOG( cerr, L"Kairo4" )
            << BUILD_LOG( nDecision )
            << BUILD_LOG( m_mapPostPrData.size() )
            << BUILD_LOG( m_kLottery.GetCaseNum() ) << END_LOG;
        return false;
    }

    prData_ = mit->second;
    return nDecision;
}

bool Kairo::RemoveCase( IN int nIndexID_ )
{
	START_LOG( clog, L"Removing case" )
		<< BUILD_LOG( nIndexID_ )
		<< END_LOG;
    if( !m_kLottery.RemoveCase( nIndexID_ ) ) {
		START_LOG( clog, L"Failed to Remove case" )
			<< BUILD_LOG( nIndexID_ )
			<< END_LOG;
        return false;
    }
    return true;
}

bool Kairo::IncreaseCaseRatio( IN int nCaseID_, IN float fRatio_ )
{
    if( m_kLottery.IncreaseCase( nCaseID_, fRatio_ ) ) {
        return true;
    }
    return false;
}

bool Kairo::SetStrengthEquipDropItem( IN GCITEMID nItemID_, IN char cGradeID_, IN char cEnchantLevel_, IN char cEquipType_, IN char cStrengthGradeID_, IN char cStrengthEnchantLevel_, IN float fProb_ )
{
    int iCaseID = m_mapPostVecItem.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    KDropItemInfo kUnitEquipItem;
    KDropItemInfo kUnitStrengthItem;
    std::vector<KDropItemInfo> vecItem;
    vecItem.clear();

    kUnitEquipItem.m_ItemID          = (GCITEMID)nItemID_;
    kUnitEquipItem.m_nDuration       = -1;
    kUnitEquipItem.m_nPeriod         = -1;
    kUnitEquipItem.m_cGradeID        = cGradeID_;
    kUnitEquipItem.m_cEnchantLevel   = cEnchantLevel_;
    kUnitEquipItem.m_cEquipType      = cEquipType_;
    vecItem.push_back( kUnitEquipItem );

    kUnitStrengthItem.m_ItemID          = Kairo::IG_STRENGTH;
    kUnitStrengthItem.m_nDuration       = -1;
    kUnitStrengthItem.m_nPeriod         = -1;
    kUnitStrengthItem.m_cGradeID        = cStrengthGradeID_;
    kUnitStrengthItem.m_cEnchantLevel   = cStrengthEnchantLevel_;
    kUnitStrengthItem.m_cEquipType      = 0;
    vecItem.push_back( kUnitStrengthItem );

    m_mapPostVecItem.insert( std::make_pair( iCaseID, vecItem ) );
    SetKairoType( Kairo::RT_STRENGTH );

    return m_mapPostVecItem.size() == m_kLottery.GetCaseNum();
}

bool Kairo::SetStrengthPeriodEquipDropItem( IN GCITEMID nItemID_, IN char cGradeID_, IN char cEnchantLevel_, IN int nPeriod_, IN char cEquipType_, IN char cStrengthGradeID_, IN char cStrengthEnchantLevel_, IN float fProb_ )
{
    int iCaseID = m_mapPostVecItem.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    KDropItemInfo kUnitEquipItem;
    KDropItemInfo kUnitStrengthItem;
    std::vector<KDropItemInfo> vecItem;
    vecItem.clear();

    kUnitEquipItem.m_ItemID          = (GCITEMID)nItemID_;
    kUnitEquipItem.m_nDuration       = -1;
    kUnitEquipItem.m_nPeriod         = nPeriod_;
    kUnitEquipItem.m_cGradeID        = cGradeID_;
    kUnitEquipItem.m_cEnchantLevel   = cEnchantLevel_;
    kUnitEquipItem.m_cEquipType      = cEquipType_;
    vecItem.push_back( kUnitEquipItem );

    kUnitStrengthItem.m_ItemID          = Kairo::IG_STRENGTH;
    kUnitStrengthItem.m_nDuration       = -1;
    kUnitStrengthItem.m_nPeriod         = -1;
    kUnitStrengthItem.m_cGradeID        = cStrengthGradeID_;
    kUnitStrengthItem.m_cEnchantLevel   = cStrengthEnchantLevel_;
    kUnitStrengthItem.m_cEquipType      = 0;
    vecItem.push_back( kUnitStrengthItem );

    m_mapPostVecItem.insert( std::make_pair( iCaseID, vecItem ) );
    SetKairoType( Kairo::RT_STRENGTH );

    return m_mapPostVecItem.size() == m_kLottery.GetCaseNum();
}

bool Kairo::Do( OUT std::vector<KDropItemInfo>& vecItem_ ) const
{
    vecItem_.clear();

    int nDecision = m_kLottery.Decision();

    if( nDecision == KLottery::CASE_BLANK ) {
        return true;
    }

    std::map<int, std::vector<KDropItemInfo>>::const_iterator mit;
    mit = m_mapPostVecItem.find( nDecision );

    if( mit == m_mapPostVecItem.end() ) {
        START_LOG( cerr, L"Kairo5" )
            << BUILD_LOG( nDecision )
            << BUILD_LOG( m_mapPostVecItem.size() )
            << BUILD_LOG( m_kLottery.GetCaseNum() ) << END_LOG;
        return false;
    }

    vecItem_ = mit->second;
    return true;
}

void Kairo::SetKairoType( IN DWORD dwType_ )
{
    m_dwType = dwType_;
}

bool Kairo::SetEquipLevelDropItem( IN GCITEMID nItemID_, IN int nDuration_, IN int nPeriod_, IN char cGradeID_, IN char cEquipLevel_, IN float fProb_ )
{
    int iCaseID = m_mapPostItem.size();
    _JIF( m_kLottery.AddCase( iCaseID, fProb_ ), return false );

    KDropItemInfo kUnit;
    kUnit.m_ItemID          = (GCITEMID)nItemID_;
    kUnit.m_nDuration       = nDuration_;
    kUnit.m_nPeriod         = nPeriod_;
    kUnit.m_cGradeID        = cGradeID_;
    kUnit.m_cEquipLevel     = cEquipLevel_;

    m_mapPostItem.insert( std::make_pair( iCaseID, kUnit ) );

    return m_mapPostItem.size() == m_kLottery.GetCaseNum();
}

void Kairo::EnableNoFailure()
{
	m_kLottery.EnableState(KLottery::ENABLE_NO_FAILURE);
}