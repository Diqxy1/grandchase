#include "Lottery.h"
#include <dbg/dbg.hpp>

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

bool KLottery::AddCase(  int nCaseID, float fProbability )
{
    if( m_fTotalProb + fProbability > 100.1f )
    {
        START_LOG( cerr, L"100퍼센트를 넘는 확률 설정을 시도." )
            << BUILD_LOG( nCaseID )
            << BUILD_LOG( fProbability )
            << BUILD_LOG( m_fTotalProb )
            << BUILD_LOG( m_fTotalProb + fProbability )
            << BUILD_LOG( m_iState )
            << BUILD_LOG( m_mapCase.size() );

        return false;
    }

    std::map<int,float>::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit != m_mapCase.end() )    // 이전에 동일한 case의 확률이 존재하고 있던 경우
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
        return CASE_BLANK;

    // fScale_이 0.f ~ 1.f 사이의 값이 되도록 클리핑해준다.
    fScale_ = std::max<float>( 0.f, fScale_ );
    fScale_ = std::min<float>( 1.f, fScale_ );

    float fRoulette = (float)uni();

    fRoulette -= (fRoulette * fScale_); // scale 값만큼 랜덤 결과를 떨군다.. (상대적인 확률 상승)
    //cout<<fRoulette<<endl;

    float fAccumulate = 0.0f;
    std::map<int,float>::const_iterator mit;

    float fTotalProb = GetTotalProb();

    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        float fRatio = mit->second;

        if( m_iState & ENABLE_NO_FAILURE )
        {
            fRatio = mit->second / fTotalProb * 100.f;
        }

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
    if( !(m_iState & ENABLE_NO_FAILURE) )
    {
        return m_fTotalProb;
    }

    float fTotalProb = 0.f;
    std::map<int,float>::const_iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        fTotalProb += mit->second;
    }

    return fTotalProb;
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