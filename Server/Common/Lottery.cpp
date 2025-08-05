#include "Lottery.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

KLottery::KLottery() 
: m_fTotalProb( 0.0f )
, m_iState(0)
{
}

KLottery::~KLottery() 
{
}

KLottery& KLottery::operator=( const KLottery& t )
{
    m_mapCase       = t.m_mapCase;
    m_fTotalProb    = t.m_fTotalProb;
    m_iState        = t.m_iState;
    return *this;
}

void KLottery::Clear()
{
	m_fTotalProb = 0.0f;
	m_iState = 0;
	m_mapCase.clear();
}

bool KLottery::AddCase(  int nCaseID, float fProbability )
{
    if( m_fTotalProb + fProbability > 100.1f )
    {
        START_LOG( cerr, L"Mais de 100%" )
            << BUILD_LOG( nCaseID )
            << BUILD_LOG( fProbability )
            << BUILD_LOG( m_fTotalProb )
            << BUILD_LOG( m_fTotalProb + fProbability )
            << BUILD_LOG( m_iState )
            << BUILD_LOG( m_mapCase.size() ) << END_LOG;

        return false;
    }

    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit != m_mapCase.end() )
    {
        mit->second += fProbability;
        m_fTotalProb += fProbability;
    }

    else
    {
        m_mapCase.insert( std::make_pair( nCaseID, fProbability ) );
        m_fTotalProb += fProbability;
    }

    return true;
}

bool KLottery::RemoveCase( int nCaseID )
{
    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit == m_mapCase.end() )
        return false;

    m_fTotalProb -= mit->second;

    m_mapCase.erase( mit );
    return true;
}

bool KLottery::MultipleCase( int nCaseID, float fRatio )
{
    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit == m_mapCase.end() )
        return false;

    m_fTotalProb -= mit->second;

    if( GetRemaindProb() > (mit->second * fRatio ) )
        mit->second *= fRatio;
    else
        mit->second = GetRemaindProb();

    m_fTotalProb += mit->second;

    return true;
}

#include <boost/random.hpp>

int KLottery::Decision( float fScale_ /*= 0.f*/ ) const
{
    static boost::minstd_rand generator(42u);
    static boost::uniform_real<> uni_dist(0,100);
    static boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator, uni_dist);

    if( m_mapCase.empty() )
	{
		START_LOG( cerr, "Lottery Error" )
			<< BUILD_LOG( m_mapCase.empty() )
			<< END_LOG;

        return CASE_BLANK;
	}

    float fRoulette = (float)uni();
    float fTotalProb = GetTotalProb();

    if ( m_iState & ENABLE_NO_FAILURE || fScale_* fTotalProb >= 100.f - fTotalProb ) {
        fRoulette = fRoulette * fTotalProb / 100.f;
    }
    else if ( fScale_ > 0.f ) {
        fRoulette = fRoulette / (1.f + fScale_);
    }

    // 혹시 모를 클리핑
    fRoulette = std::min<float>( 100.f, fRoulette );
    fRoulette = std::max<float>( 0.f, fRoulette );

    float fAccumulate = 0.0f;
    std::map<int,float>::const_iterator mit;

    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        float fRatio = mit->second;

        fAccumulate += fRatio;
        if( fRoulette <= fAccumulate )
        {
            return mit->first;
        }
    }

    return CASE_BLANK;
}

float KLottery::GetRatio( int nIndex_ )
{
    float fRet = 0;
    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nIndex_ );
    if( mit != m_mapCase.end() )
        fRet = mit->second;
    return fRet;
}

float KLottery::GetTotalProb() const
{
    return m_fTotalProb;
}

#include <boost/format.hpp>

ImplToStringW( KLottery )
{
    float fAccumulate = 0;
    std::map<int,float>::const_iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        stm_ << boost::wformat( L"%2.2f ~ %2.2f (%2.2f%%) : %d \n" )
            % fAccumulate
            % (float)(fAccumulate + mit->second)
            % (float)(mit->second)
            % mit->first;

        fAccumulate += mit->second;
    }

    stm_ << boost::wformat( L"%2.2f ~ 100 (%2.2f%%) : -1 \n" )
        % fAccumulate
        % (100.F - fAccumulate);

    return stm_ << std::endl;
}

bool KLottery::GetMaxCaseID( IN int nExceptCaseID_, OUT int& nMaxCaseID_ )
{
    std::map<int,float>::iterator mit;
    nMaxCaseID_ = -1;
    float fMaxRatio = 0.f;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); ++mit ) {
        if( mit->first == nExceptCaseID_ ) {
            continue;
        }
        if( mit->second > fMaxRatio ) {
            fMaxRatio = mit->second;
            nMaxCaseID_ = mit->first;
        }
    }
    return true;
}

bool KLottery::DecreaseCase( IN int nCaseID_, IN float fRatio_ )
{
    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID_ );

    if( mit == m_mapCase.end() )
        return false;

    mit->second -= fRatio_;
    mit->second = std::max<float>( 0.f, mit->second );
    m_fTotalProb -= fRatio_;

    return true;
}

bool KLottery::IncreaseCase( IN const int& nCaseID_, IN OUT float& fRatio_ )
{
    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID_ );
    if ( mit == m_mapCase.end() )
        return false;

    fRatio_ = std::min<float>( fRatio_, GetRemaindProb() );

    mit->second += fRatio_;
    m_fTotalProb += fRatio_;

    return true;
}