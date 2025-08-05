#include "GambleBuyManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp >
#include "Lottery.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGambleBuyManager );
ImplOstmOperatorW2A( KGambleBuyManager );
NiImplementRootRTTI( KGambleBuyManager );

ImplToStringW( KGambleBuyManager )
{
    KLocker lock1( m_csGradeRatio );
    KLocker lock2( m_csCostRate );

    return START_TOSTRINGW
        << TOSTRINGW( m_fCostRate )
        << TOSTRINGW( m_pGradeRatio.size() )
        << TOSTRINGW( m_pGradeRatio[KItem::GRADE_NORMAL] )
        << TOSTRINGW( m_pGradeRatio[KItem::GRADE_RARE] )
        << TOSTRINGW( m_pGradeRatio[KItem::GRADE_EPIC] )
        << TOSTRINGW( m_pGradeRatio[KItem::GRADE_LEGEND] )
        << TOSTRINGW( m_pGradeRatio[KItem::GRADE_MYSTIC] );
}

KGambleBuyManager::KGambleBuyManager(void)
:m_fCostRate(1.f)
,m_fMinGambleRatio(0.f)
,m_fMaxGambleRatio(0.f)
,m_fLevelModNum(1.f)
{
    for ( size_t i = 0 ; i < m_pGradeRatio.max_size() ; ++i ) {
        m_pGradeRatio[i] = 0.f;
    }
}

KGambleBuyManager::~KGambleBuyManager(void)
{
}

bool KGambleBuyManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGambleBuyManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    float fCostRate = 1.f;
    float fMinGambleRatio = 0;
    float fMaxGambleRatio = 0;
    float fLevelModNum = 1;
    boost::array<float,KItem::GRADE_NUM> pGradeRate;
    pGradeRate[KItem::GRADE_NORMAL] = 0;

    const std::string strScriptName = "InitGambleBuy.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "GambleBuy" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CostRate", fCostRate ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MinGambleRatio", fMinGambleRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxGambleRatio", fMaxGambleRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LevelModNum", fLevelModNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "RareRatio", pGradeRate[KItem::GRADE_RARE] ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "EpicRatio", pGradeRate[KItem::GRADE_EPIC] ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LegendRatio", pGradeRate[KItem::GRADE_LEGEND] ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MysticRatio", pGradeRate[KItem::GRADE_MYSTIC] ) == S_OK, return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load GambleBuy script.. CostRate : " << fCostRate << L", size : " << pGradeRate.size() ) << END_LOG;

    SetCostRate( fCostRate );

    KLocker lock( m_csGradeRatio );
    m_fMinGambleRatio = fMinGambleRatio;
    m_fMaxGambleRatio = fMaxGambleRatio;
    m_fLevelModNum = std::max<float>( 1.f, fLevelModNum );
    m_pGradeRatio.swap( pGradeRate );
    return true;
}

int KGambleBuyManager::GetGradeByLevel( IN const int nLevel_ )
{
    KLocker lock( m_csGradeRatio );

    KLottery kLottery;
    int nGrade = KItem::GRADE_NORMAL;
    int nItemLevel = std::max<int>( 1,nLevel_ );

    float fNormalRate = 100 - std::max<float>( m_fMinGambleRatio, m_fMaxGambleRatio - (int)(nItemLevel / m_fLevelModNum) );
    kLottery.AddCase(KItem::GRADE_NORMAL, fNormalRate );
    nGrade = kLottery.Decision();

    if ( nGrade == KItem::GRADE_NORMAL ) {
        return nGrade;
    }
    kLottery.RemoveCase( KItem::GRADE_NORMAL );

    for ( UINT i = KItem::GRADE_MYSTIC ; i > KItem::GRADE_NORMAL ; --i ) {
        kLottery.AddCase( i, m_pGradeRatio[i] * 100 );
    }

    nGrade = kLottery.Decision();

    if ( nGrade == KLottery::CASE_BLANK ) {
        nGrade = KItem::GRADE_NORMAL;
    }

    return nGrade;
}

void KGambleBuyManager::TestGamble( IN const int nLevel_, IN const int nTry_ )
{
    std::map<int,int> mapResult;

    for ( int i = 0 ; i < nTry_ ; ++i ) {
        int nGrade = GetGradeByLevel( nLevel_ );
        ++mapResult[nGrade];
    }

    //std::cout << L"--- Test Gamble Result ---" << dbg::endl
    //          << L"Item Level : " << nLevel_ << L", Try : " << nTry_ << L", Result size : " << mapResult.size() << dbg::endl;

    //std::map<int,int>::const_iterator cmit;
    //for ( cmit = mapResult.begin() ; cmit != mapResult.end() ; ++cmit ) {
    //    std::cout << std::tab << cmit->first << L" : " << cmit->second << dbg::endl;
    //}
    //std::cout << L"--- Test End ---" << dbg::endl;
}