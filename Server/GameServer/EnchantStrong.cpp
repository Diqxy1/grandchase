#include "EnchantStrong.h"
#include "ResultManager.h"
#include <KncUtilLua.h> // luabind::object, boost::tuple
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KEnchantStrong::KEnchantStrong(void)
:m_fEnchantBonusRate(0.f),
m_dwCoreItemID(0),
m_dwLimitEnchantLevel(30),
m_dwStrongAssistWeak(0)
{
}

KEnchantStrong::~KEnchantStrong(void)
{
}

ImplementSingleton( KEnchantStrong );
ImplOstmOperatorW2A( KEnchantStrong );
ImplToStringW( KEnchantStrong )
{
    return stm_;
}

void KEnchantStrong::ClearEnchantAssist()
{
    KLocker lock( m_csEnchant );
    m_mapEnchantAssist.clear();
}

void KEnchantStrong::ClearEquipItemID()
{
    KLocker lock( m_csEnchant );
    m_mapEnchantEnableItem.clear();
}

void KEnchantStrong::ClearEnchantLimitLv()
{
    KLocker lock( m_csEnchant );
    m_mapEnchantLimitLv.clear();
}

void KEnchantStrong::ClearEnchantInfo()
{
    KLocker lock( m_csEnchant );
    m_mapEnchantInfo.clear();

}

void KEnchantStrong::ClearMaterialCount()
{
    KLocker lock( m_csEnchant );
    m_mapNeedMaterialCount.clear();
}

void KEnchantStrong::ClearAssistInfoTable()
{
    KLocker lock( m_csEnchant );
    m_mapAssistInfoTable.clear();
}

void KEnchantStrong::SetEnchantFailResult( const UINT uiLv_, const UINT uiResult_ )
{
    KLocker lock( m_csEnchant );    
    if ( m_mapEnchantLimitLv.find( uiLv_ ) == m_mapEnchantLimitLv.end() )
    {
        m_mapEnchantLimitLv[uiLv_] = uiResult_;
    }
}
//====================================================================================
// 2007-10-15 오후 7:28:03 : Asirion 
// 이 레벨에서 실패 했을경우 어떠한 결과를 나타내는가?
// EA_SUCCESS의 경우 실패했을때도 결과는 EA_SUCCESS이다.
KEnchantStrong::ENCHANT_ACTION KEnchantStrong::GetEnchantFailResult( const UINT uiEnchantLv_ )
{
    KLocker lock( m_csEnchant );
    static std::map< UINT, UINT >::iterator s_mit;
    s_mit = m_mapEnchantLimitLv.find( uiEnchantLv_ );
    if ( s_mit == m_mapEnchantLimitLv.end() )
        return KEnchantStrong::EA_BREAK; // 리스트에 없으면 깨짐 처리.
    return (KEnchantStrong::ENCHANT_ACTION)s_mit->second;
}

void KEnchantStrong::SetEnchantAction(  const int nEnchantKind_, const int nStrongLV_, const int nStrongMaterialIdx_,
                                      const float fSuccess_, const float fWeakness_, const float fBreak_ )
{
    float fEnchantBonusRate = GetEnchantBonusRate();
    float fCurrentSuccess_ = 0.f;

    fCurrentSuccess_ = fSuccess_ + ( fSuccess_ * fEnchantBonusRate / 100 ); // 성공확률의 상대률 증가.
    fCurrentSuccess_ = std::min<float>( 100 - fWeakness_ - fBreak_, fCurrentSuccess_ ); // 총확률 100프로 넘지않도록.

    KLocker lock( m_csEnchant );
    if( fSuccess_ >= 0.f )
        m_mapEnchantInfo[nEnchantKind_<<24 | nStrongMaterialIdx_<<16 | nStrongLV_].AddCase( EA_SUCCESS, fCurrentSuccess_ );
    if( fWeakness_ >= 0.f )
        m_mapEnchantInfo[nEnchantKind_<<24 | nStrongMaterialIdx_<<16 | nStrongLV_].AddCase( EA_WEAKNESS, fWeakness_ );
    if( fBreak_ >= 0.f )
        m_mapEnchantInfo[nEnchantKind_<<24 | nStrongMaterialIdx_<<16 | nStrongLV_].AddCase( EA_BREAK, fBreak_ );
}

void KEnchantStrong::SetEnchantMaterialCount( const luabind::object& table_ )
{
    JIF( table_.type() == LUA_TTABLE );

    KLocker lock( m_csEnchant );
    m_mapNeedMaterialCount.clear();

    // Table2map 하면 map[1] 부터 있다. 0 base 아님.
    JIF( KncUtil::Table2map( table_, m_mapNeedMaterialCount ) );
}

bool KEnchantStrong::GetEnchantUnit( const int nEnchantKind_, const int nStrongLV_, const int nStrongMaterialIdx_, KLottery& kEnchantUnit_ )
{
    KLocker lock( m_csEnchant );
    std::map<int, KLottery>::const_iterator mit;
    mit = m_mapEnchantInfo.find( nEnchantKind_<<24 | nStrongMaterialIdx_<<16 | nStrongLV_ );
    if( mit == m_mapEnchantInfo.end() )
        return false;
    kEnchantUnit_ = mit->second;
    return true;
}

void KEnchantStrong::MaterialCountList( std::map<DWORD,int>& mapCount_ )
{ 
    KLocker lock(m_csEnchant);
    mapCount_ = m_mapNeedMaterialCount;
}

bool KEnchantStrong::GetNeedMaterialCount( IN const int nStrongLV_, OUT DWORD& dwNeedCount_ )
{
    dwNeedCount_ = 0xfffffff0; // 매우 많은 숫자를 지정.

    KLocker lock(m_csEnchant);
    std::map<DWORD,int>::const_iterator mit;
    mit = m_mapNeedMaterialCount.find( nStrongLV_ );
    if( mit == m_mapNeedMaterialCount.end() )
        return false;

    dwNeedCount_ = (DWORD)mit->second;
    return true;
}

void KEnchantStrong::AddStronAssist( const GCITEMID dwAssistItemID_, const int nStrongLV_, const luabind::object& table_ )
{
    JIF( dwAssistItemID_ > 0 );
    JIF( nStrongLV_ >= 1 && nStrongLV_ <= 9 );
    JIF( table_.type() == LUA_TTABLE );

    std::pair<DWORD,int> pairKey = std::make_pair( dwAssistItemID_, nStrongLV_);
    std::vector< std::pair<int,float> > vecRatio;

    KLocker lock( m_csEnchant ); // Lock

    // 동일한 키가 존재하는가?
    std::map< std::pair<DWORD,int>, std::vector< std::pair<int,float> > >::iterator mit;
    mit = m_mapAssistInfoTable.find( pairKey );
    JIF( m_mapAssistInfoTable.end() == mit );

    // 데이터 추출
    luabind::object::iterator itItem;
    itItem = table_.begin();
    while( itItem != table_.end() )
    {
        std::pair<int,float> pairItem;
        JIF( KncUtil::Table2pair( *itItem, pairItem ) );
        JIF( pairItem.first >= EA_SUCCESS && pairItem.first < EA_MAX );
        vecRatio.push_back( pairItem );
        ++itItem;
    }

    // 추가.
    m_mapAssistInfoTable.insert( std::make_pair( pairKey, vecRatio ) );
}

void KEnchantStrong::AddStrongAssistByCharacter( const int nCharacter_, const int nStrongLV_, const luabind::object& table_ )
{
    JIF( nCharacter_ >= 0 );
    JIF( nStrongLV_ >= 1 && nStrongLV_ <= 9 );
    JIF( table_.type() == LUA_TTABLE );

    std::pair<int,int> pairKey = std::make_pair( nCharacter_, nStrongLV_);
    std::vector< std::pair<int,float> > vecRatio;

    KLocker lock( m_csEnchant ); // Lock

    // 동일한 키가 존재하는가?
    std::map< std::pair<int,int>, std::vector< std::pair<int,float> > >::iterator mit;
    mit = m_mapEnchantBonusRateByChar.find( pairKey );

    // 데이터 추출
    luabind::object::iterator itItem;
    itItem = table_.begin();
    while( itItem != table_.end() )
    {
        std::pair<int,float> pairItem;
        JIF( KncUtil::Table2pair( *itItem, pairItem ) );
        JIF( pairItem.first >= EA_SUCCESS && pairItem.first < EA_MAX );
        vecRatio.push_back( pairItem );
        ++itItem;
    }
    if( m_mapEnchantBonusRateByChar.end() != mit )
    {
        m_mapEnchantBonusRateByChar[pairKey] = vecRatio;
    }
    else
    {   // 추가.
        m_mapEnchantBonusRateByChar.insert( std::make_pair( pairKey, vecRatio ) );
    }
}

bool KEnchantStrong::GetStrongAssistByCharacter( const int nCharacter_, const int nStrongLV_,
                                                std::vector< std::pair<int,float> >& vecRatio_ )
{
    if( nCharacter_ < 0 )
        return false;

    if( nStrongLV_ < 1 || nStrongLV_ > 9 )
        return false;

    vecRatio_.clear();

    std::pair<int,int> pairKey = std::make_pair( nCharacter_, nStrongLV_);

    KLocker lock(m_csEnchant);
    std::map< std::pair<int,int>, std::vector< std::pair<int,float> > >::iterator mit;
    mit = m_mapEnchantBonusRateByChar.find( pairKey );

    if( mit == m_mapEnchantBonusRateByChar.end() )
        return false;

    vecRatio_ = mit->second;
    return true;
}


bool KEnchantStrong::GetStrongAssist( const GCITEMID dwAssistItemID_, const int nStrongLV_,
                                     std::vector< std::pair<int,float> >& vecRatio_ )
{
    if( dwAssistItemID_ <= 0 )
        return false;

    if( nStrongLV_ < 1 || nStrongLV_ > 9 )
        return false;

    vecRatio_.clear();

    std::pair<DWORD,int> pairKey = std::make_pair( dwAssistItemID_, nStrongLV_);

    KLocker lock(m_csEnchant);
    std::map< std::pair<DWORD,int>, std::vector< std::pair<int,float> > >::iterator mit;
    mit = m_mapAssistInfoTable.find( pairKey );

    if( mit == m_mapAssistInfoTable.end() )
        return false;

    vecRatio_ = mit->second;
    return true;
}


void KEnchantStrong::SetEnchantRatio( const UINT uiGrade_, const UINT uiStartLv_, const UINT uiEndLv_, 
                                     const float fWeaknessRatio_, const float fDropRatio_, const float fBreakRatio_, const UINT uiInc_ )
{
    KLocker lock(m_csEnchant);
    m_akEnchant[uiGrade_].m_uiStartLv = uiStartLv_;
    m_akEnchant[uiGrade_].m_uiEndLv   = uiEndLv_;
    //====================================================================================
    // 2007-10-15 오후 7:26:41 : Asirion 
    // KEnchantStrong::EA_SUCCESS  : 안전인챈 단계의 실패율
    // KEnchantStrong::EA_WEAKNESS : 하락 단계의 실패율
    // KEnchantStrong::EA_DROP     : 초기화 단계의 실패율
    // KEnchantStrong::EA_BREAK    : 깨짐 단계의 실패율
    m_akEnchant[uiGrade_].m_uiInc                                = uiInc_;
    m_akEnchant[uiGrade_].m_afRatio[KEnchantStrong::EA_SUCCESS]  = 0.0f;
    m_akEnchant[uiGrade_].m_afRatio[KEnchantStrong::EA_WEAKNESS] = fWeaknessRatio_;
    m_akEnchant[uiGrade_].m_afRatio[KEnchantStrong::EA_DROP]     = fDropRatio_;
    m_akEnchant[uiGrade_].m_afRatio[KEnchantStrong::EA_BREAK]    = fBreakRatio_;
}

//KEnchantStrong::ENCHANT_GRADE KEnchantStrong::GetGrade( const UINT uiLv_ )
//{
//    KLocker lock(m_csEnchant);
//    for ( int i = 0; i < (int)KEnchantStrong::EG_MAX; i++ )
//    {
//        if ( m_akEnchant[i].CheckGrade( uiLv_) == true )
//            return (KEnchantStrong::ENCHANT_GRADE)i;
//    }
//    return KEnchantStrong::EG_MAX;
//}

KEnchantStrong::ENCHANT_GRADE KEnchantStrong::GetEnchantGrade( const GCITEMID dwItemID_ )
{
    std::map<DWORD, UINT>::const_iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapEnchantEnableItem.find( dwItemID_ );
    if( mit == m_mapEnchantEnableItem.end() )
        return KEnchantStrong::EG_MAX; // 밖에서 체크 한다..

    return (KEnchantStrong::ENCHANT_GRADE)mit->second;
}

float KEnchantStrong::GetEnchantAssistRatio( const GCITEMID dwAssistItemID_ )
{
    std::map< DWORD, float >::const_iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapEnchantAssist.find( dwAssistItemID_ );
    if( mit == m_mapEnchantAssist.end() ) return 0.f;

    return mit->second;
}

bool KEnchantStrong::ChkGrade_Action( int nGrade_, int nAction_ )
{
    if( nGrade_ < EG_FIRST || nGrade_ >= EG_MAX ) return false;
    //제자리는 없기 때문에..
    if( nAction_ == EA_NONE || nAction_ < EA_SUCCESS || nAction_ >= EA_MAX ) return false;

    return true;
}

KEnchantStrong::ENCHANT_ACTION KEnchantStrong::Enchant( IN const GCITEMID dwItemID_, IN const UINT uiEnchantLv_, IN const GCITEMID dwAssistItemID_, IN const float fScale_, IN const bool bPCBang_, OUT float& fFailRatio_, OUT float& fAddRatio_, OUT int& nLevel_ )
{
    KEnchantStrong::ENCHANT_GRADE eGrade = GetEnchantGrade(dwItemID_);
    KEnchantStrong::ENCHANT_ACTION eAction = GetEnchantFailResult( uiEnchantLv_ );
    // 액션과, 등급이 올바른 범위 인가?
    if( !ChkGrade_Action( eGrade, eAction ) ) // 범위 안에 없다면.. 드랍 시킬까나??
    {
        START_LOG( cerr, L" 룬 강화시.. 등급, 액션에 문제 있음.. " )
            << BUILD_LOG( dwItemID_ )
            << BUILD_LOG( uiEnchantLv_ )
            << BUILD_LOG( dwAssistItemID_ )
            << BUILD_LOG( eGrade )
            << BUILD_LOG( eAction )
            << BUILD_LOG( fScale_ ) << END_LOG;
        return KEnchantStrong::EA_DROP;
    }

    // 특정 강화 업그레이드 아이템에 대한 레벨 체크.
    if( SiKEnchantStrong()->CheckSpecificLevelItem( dwAssistItemID_ ) ) {
        if( SiKEnchantStrong()->GetSpecificLevelEnchant( dwAssistItemID_, nLevel_ ) ) {
            return KEnchantStrong::EA_SPECIFIC;
        }
    }

    float fAssistRate = GetEnchantAssistRatio( dwAssistItemID_ ); // 어시스트의 확률은 실패 확률을 감소 시킨다.    

    // 실패 확률은 0보다 크거나 같아야 한다.
    float fFailRate = std::max<float>( (m_akEnchant[eGrade].m_afRatio[eAction] - fAssistRate ), 0.f );

    float fAddRatio = (100.f - fFailRate) * fScale_;
    fAddRatio = std::min<float>( 100.f, fAddRatio );
    fAddRatio = std::max<float>( 0.f, fAddRatio );

    KLottery kLottery;
    kLottery.AddCase( (int)eAction, (fFailRate - fAddRatio)  ); // 실패 액션과 확률 등록
    kLottery.AddCase( (int)KEnchantStrong::EA_SUCCESS, 100.0f - (fFailRate - fAddRatio) ); // 성공 액션과 확률 등록.

    // 로그를 남기기 위해서 실제 확률값을 받아오자.
    fFailRatio_ = fFailRate; 
    fAddRatio_ = fAddRatio;

    return (KEnchantStrong::ENCHANT_ACTION)kLottery.Decision();
}

void KEnchantStrong::SetCoreItemID( const GCITEMID dwItemID_ )
{
    KLocker lock(m_csEnchant);
    m_dwCoreItemID = dwItemID_;
}

void KEnchantStrong::AddEnchantEnableItemID( const GCITEMID dwItemID_, KEnchantStrong::ENCHANT_GRADE eGrade_ )
{
    KLocker lock(m_csEnchant);
    assert( m_mapEnchantEnableItem.find( dwItemID_ ) == m_mapEnchantEnableItem.end() );
    m_mapEnchantEnableItem.insert( std::make_pair( dwItemID_, (UINT)eGrade_ ) );
}

void KEnchantStrong::AddEnchantAssistItem( const GCITEMID dwItemID_, const float fIncreaseRate_ )
{
    KLocker lock(m_csEnchant);  
    assert( m_mapEnchantAssist.find( dwItemID_ ) == m_mapEnchantAssist.end() );
    m_mapEnchantAssist.insert( std::make_pair( dwItemID_, fIncreaseRate_ ) );
}

bool KEnchantStrong::IsEnchantEnableItem( const GCITEMID dwItemID_ )
{
    KLocker lock(m_csEnchant);  
    return m_mapEnchantEnableItem.find( dwItemID_ ) != m_mapEnchantEnableItem.end();
}

bool KEnchantStrong::IsEnchantAssistItem( const GCITEMID dwItemID_ )
{
    KLocker lock(m_csEnchant);  
    return m_mapEnchantAssist.find( dwItemID_ ) != m_mapEnchantAssist.end();
} 

UINT KEnchantStrong::GetNeedCoreCount( const GCITEMID dwItemID_, const UINT uiEnchantLv_, const float fScale_ )
{
    KLocker lock(m_csEnchant);
    assert( m_mapEnchantEnableItem.find( dwItemID_ ) != m_mapEnchantEnableItem.end() );
    UINT uiGrade = m_mapEnchantEnableItem[dwItemID_];
    UINT uiNeedCount = m_akEnchant[uiGrade].GetNeedCoreCount( uiEnchantLv_ );

    if( uiNeedCount > 1 )
        uiNeedCount = (UINT)((uiNeedCount * fScale_) + 0.5f);

    return uiNeedCount;
}

void KEnchantStrong::SetEnchantBonusRate( IN float fBonusRate_ )
{
    // 0~100.f 확률 보정
    if( fBonusRate_ < 0.f )
        fBonusRate_ = 0.f;
    if( fBonusRate_ > 100.f )
        fBonusRate_ = 100.f;

    KLocker lock( m_csEnchant );
    m_fEnchantBonusRate = fBonusRate_;
    START_LOG( clog, L"강화 보너스 확률 설정 : " << fBonusRate_ ) << END_LOG;
}

float KEnchantStrong::GetEnchantBonusRate()
{
    KLocker lock( m_csEnchant );
    return m_fEnchantBonusRate;
}


void KEnchantStrong::EnchantTest( GCITEMID nItemID_, int nBeginLevel_, int nGoalLevel_, GCITEMID nAssistItemID_, int nLoopCount_ )
{
    GCITEMID dwItemID = nItemID_;
    //char cBeginLv = (char)nBeginLevel_;
    char cCurrentLv = (char)nBeginLevel_;
    GCITEMID dwAssistItemID = 0xFFFFFFFF;
    if( nAssistItemID_ > 0 )
        dwAssistItemID = nAssistItemID_;

    std::wstring strAction;
    int nLoopCount = 0;
    char cMaxLv = (char)nBeginLevel_;
    float fFailRatio = 0.f;
    float fAddRatio = 0.f;
    int nLevel = 0;
    while( true )
    {
        int nRes = Enchant( dwItemID, (UINT)cCurrentLv, dwAssistItemID, 0.f, false, fFailRatio, fAddRatio, nLevel );
        switch(nRes)
        {
        case EA_SUCCESS:
            ++cCurrentLv;
            strAction = L"EA_SUCCESS";
            break;
        case EA_WEAKNESS:
            --cCurrentLv;
            strAction = L"EA_WEAKNESS";
            break;
        case EA_BREAK:
            cCurrentLv = 0;
            strAction = L"EA_BREAK";
            break;
        case EA_DROP:
            cCurrentLv = 0;
            strAction = L"EA_DROP";
            break;
        case EA_SPECIFIC:
            cCurrentLv = nLevel;
            strAction = L"EA_SPECIFIC";
            break;
        default:
            strAction = L"UNKNOWN";
        }

        if( cMaxLv < cCurrentLv )
            cMaxLv = cCurrentLv;

        //std::cout2 << strAction << L" : " << (int)cCurrentLv << L" : " << (int)cMaxLv << L" : " << ++nLoopCount << dbg::endl;
        if( nLoopCount_ > 0 )
            --nLoopCount_;

        if( cCurrentLv == (char)nGoalLevel_ || nLoopCount_ == 0 )
            break;
    }

    //std::cout2 <<  L"Result : " << (int)cCurrentLv << L" : "<< (int)cMaxLv << L" : " << nGoalLevel_ << L" : " << nLoopCount << dbg::endl;
}

int KEnchantStrong::GetMaxMaterialCount()
{
    int nMaxCount = 0;

    KLocker lock(m_csEnchant);

    std::map<DWORD,int>::const_iterator mit;
    for ( mit = m_mapNeedMaterialCount.begin() ; mit != m_mapNeedMaterialCount.end() ; ++mit ) {
        if ( nMaxCount < mit->second ) {
            nMaxCount = mit->second;
        }
    }

    return nMaxCount;
}

void KEnchantStrong::SetStrongMaterial( IN const int nIdx_, IN const GCITEMID dwItemID_ )
{
    // 강화에 필요한 아이템(Gem)ID 설정
    KLocker lock(m_csEnchant);
    m_mapStrongMaterial[nIdx_] = dwItemID_;
}

bool KEnchantStrong::GetStrongMaterialIndex( IN const GCITEMID dwItemID_, OUT int& nIdx_ )
{
    std::map<int,DWORD>::iterator mit;
    nIdx_ = -99;

    KLocker lock(m_csEnchant);

    for ( mit = m_mapStrongMaterial.begin() ; mit != m_mapStrongMaterial.end() ; ++mit ) {
        if ( mit->second == dwItemID_ )
            break;
    }

    if ( mit == m_mapStrongMaterial.end() )
        return false;

    nIdx_ = mit->first;
    return true;
}

void KEnchantStrong::ClearStrongMaterial()
{
    KLocker lock(m_csEnchant);
    m_mapStrongMaterial.clear();
}

void KEnchantStrong::SetStrongAssistWeak( IN const GCITEMID dwItemID_ )
{
    // 강화 하락방지 아이템 설정
    KLocker lock(m_csEnchant);
    m_dwStrongAssistWeak = dwItemID_;
}

bool KEnchantStrong::IsStrongAssistWeak( IN const GCITEMID dwItemID_ )
{
    KLocker lock(m_csEnchant);
    return ( m_dwStrongAssistWeak == dwItemID_ );
}

void KEnchantStrong::ClearStrongAssistWeak()
{
    KLocker lock(m_csEnchant);
    m_dwStrongAssistWeak = 0;
}

void KEnchantStrong::SetSpecificLevelEnchant( IN const GCITEMID ItemID_, IN const int nLevel_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapSpecificLevelEnchant.find( ItemID_ );
    if( mit != m_mapSpecificLevelEnchant.end() ) {
        START_LOG( cerr, L"특정 강화 레벨 변경 아이템 중복 있음 : " << ItemID_ ) << END_LOG;
        return;
    }
    m_mapSpecificLevelEnchant[ItemID_] = nLevel_;
}

bool KEnchantStrong::CheckSpecificLevelItem( IN const GCITEMID ItemID_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapSpecificLevelEnchant.find( ItemID_ );
    if( mit == m_mapSpecificLevelEnchant.end() ) {
        return false;
    }
    return true;
}

bool KEnchantStrong::CheckEnchantLevel( IN const GCITEMID ItemID_, IN const int nLevel_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapSpecificLevelEnchant.find( ItemID_ );
    if( mit == m_mapSpecificLevelEnchant.end() ) {
        return false;
    }
    if( nLevel_ >= mit->second ) {
        return false;
    }
    return true;
}

bool KEnchantStrong::GetSpecificLevelEnchant( IN const GCITEMID ItemID_, OUT int& nLevel_ )
{
    std::map<DWORD, int>::iterator mit;
    KLocker lock(m_csEnchant);
    mit = m_mapSpecificLevelEnchant.find( ItemID_ );
    if( mit == m_mapSpecificLevelEnchant.end() ) {
        return false;
    }
    nLevel_ = mit->second;
    return true;
}