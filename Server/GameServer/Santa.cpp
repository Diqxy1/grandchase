#include "Santa.h"
#include "ResultManager.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSanta );
//ImplOstmOperatorW2A( KSanta );
ImplToStringW( KSanta )
{
    return stm_;
}
KSanta::KSanta(void)
{

}

KSanta::~KSanta(void)
{}

void KSanta::GetSanta()
{

}

void KSanta::UpdateSantaList( std::vector< sSanta >& vecSantaList_ )
{
    KLocker lock( m_csSantaList );
    m_vecSantaList = vecSantaList_;

    START_LOG( clog, L"산타 리스트 " )
        << BUILD_LOG( m_vecSantaList.size() ) << END_LOG;
}

int KSanta::CheckUserType( sSanta& sData_, CTime& firstLogin_, CTime& lastLogin_ )
{ //  090507  woosh. 휴면, 신규 , 기존 구분
    if( firstLogin_ >= sData_.m_tmStartDateTime && firstLogin_ <= sData_.m_tmEndDateTime )
    {
        //then this user is New user
        return NEW_USER;
    }
    else if( lastLogin_ < sData_.m_tmDormantDate )
    {
        //then user is dormant(sleeping) user
        return DORMANT_USER;
    }
    else if( lastLogin_ >= sData_.m_tmDormantDate )
    {
        // then user is active(normal) user
        return NORMAL_USER;
    }
    //UserType = enum type;
    return RET_ERROR;
}

bool KSanta::CharacterCheck( IN int nCharType_, std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{ // 산타 적용할 캐릭터 종류
    std::map<char, KCharacterInfo>::iterator mit;

    mit = mapCharacterInfo_.find( static_cast<char>(nCharType_) );
    
    if( nCharType_ == -1 )
    {
        START_LOG( clog, L"산타, 전체 캐릭터 대상")
            << BUILD_LOG( nCharType_ )
            << END_LOG;
        return true;
    }
    if( mit != mapCharacterInfo_.end() )
    {
        START_LOG( clog, L"산타 적용할 캐릭터 보유")
            << BUILD_LOG( nCharType_ )
            << END_LOG;
        return true;
    }
    else
    {
        START_LOG( clog, L"산타 적용할 캐릭터 없음")
            << BUILD_LOG( nCharType_ )
            << END_LOG;
        return false;
    }
}

bool KSanta::LevelCompare( IN int nCharType_  ,IN int nBeginLevel_, IN int nEndLevel_, std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{ // 산타 적용할 레벨
    std::map<char, KCharacterInfo>::iterator mit;
    int nLevel;
    // 캐릭터

    if( nCharType_ == -1 )
    {
        START_LOG( clog, L"전체 캐릭터 & 레벨에 산타 적용")
            << BUILD_LOG( nCharType_ )
            << END_LOG;
        return true;
    }

    mit = mapCharacterInfo_.find( static_cast<char>(nCharType_) );

    nLevel = SiKResultManager()->Exp_2_Level( mit->second.m_biExp );

    if( nBeginLevel_ <= nLevel && nEndLevel_ >= nLevel )
    {
        START_LOG( clog, L"캐릭터 & 레벨 산타 적용조건 만족")
            << BUILD_LOG( nLevel )
            << BUILD_LOG( nCharType_ )
            << BUILD_LOG( nBeginLevel_ )
            << BUILD_LOG( nEndLevel_ )
            << END_LOG;
        return true;
    }
    else
    {
        START_LOG( clog, L"캐릭터 & 레벨 산타 적용조건 틀림")
            << BUILD_LOG( nLevel )
            << BUILD_LOG( nCharType_ )
            << BUILD_LOG( nBeginLevel_ )
            << BUILD_LOG( nEndLevel_ )
            << END_LOG;
        return false;
    }
}

void KSanta::SortCategory( IN CTime& tmFirstLogin_, IN CTime& tmLastLogin_, std::map<char, KCharacterInfo>& mapCharacterInfo_ , OUT std::vector< sSanta >& dwSanta_ )
{
    std::vector< sSanta >::iterator vit;
    std::map<char, KCharacterInfo>::iterator mit;
    int nResult = -99;
    for( vit = m_vecSantaList.begin(); vit != m_vecSantaList.end(); ++vit )
    {
        if( vit->m_nUserType == ALL_USER )
        {
            START_LOG( clog, L"전체 유저 대상의 산타") << END_LOG;
            dwSanta_.push_back( *vit );
            continue;
        }

        nResult = CheckUserType( *vit, tmFirstLogin_, tmLastLogin_ );

        switch( nResult )
        {
        case NEW_USER:
            START_LOG( clog, L"산타, 신규유저입니다." )
                << END_LOG;
            break;
        case DORMANT_USER:
            START_LOG( clog, L"산타, 휴면유저입니다." )
                << END_LOG;
            break;
        case NORMAL_USER:
            START_LOG( clog, L"산타, 활성유저입니다." )
                << END_LOG;
            break;

        default:
            break;
        }

        if( nResult != vit->m_nUserType ) // for loop 에서 continue로 건너뛰는 방법은 바람직한가요
        {
            START_LOG( clog, L"유저의 타입과 산타 보상 유저타입 불일치")
                << BUILD_LOG( nResult )
                << BUILD_LOG( vit->m_nUserType )
                << END_LOG;
            continue;
        }

        if( CharacterCheck( vit->m_nCharType, mapCharacterInfo_ ) == false )
            continue;
        if( LevelCompare( vit->m_nCharType, vit->m_nBeginLv, vit->m_nEndLv , mapCharacterInfo_ ) == false )
            continue;

        //해당되는 산타가 있을때, 산타 UID를 넣어줌.
        dwSanta_.push_back( *vit );
    }

    START_LOG( clog, L"산타 카테고리 나누기")
        << BUILD_LOG( dwSanta_.size() )
        << BUILD_LOGtm( tmFirstLogin_ )
        << BUILD_LOGtm( tmLastLogin_ ) << END_LOG;
}