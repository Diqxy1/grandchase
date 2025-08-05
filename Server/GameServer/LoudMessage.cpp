#include "LoudMessage.h"
#include "dbg/dbg.hpp"
#include <boost/algorithm/string.hpp> // for replace_all
#include <boost/lexical_cast.hpp> // lexical_cast
#include <string>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define MSG_COUPLE_KEY 5
#define MSG_STRONG_KEY 6
#define MSG_GACHA_KEY 7
#define MSG_MISSION_KEY 9
#define MSG_DONATION_KEY 10
#define MSG_STRENGTH_KEY 11
#define MSG_GWCEVENT_KEY 12
#define MSG_ECLIPSE_START_KEY 13
#define MSG_ECLIPSE_END_KEY 14
#define MSG_LEVEL_RESET_EVENT_REWARD    15
#define MSG_GACHA_LOTTERY_KEY    16
#define MSG_GACHA_PON_KEY   17

ImplementSingleton( KLoudMessage );

KLoudMessage::KLoudMessage(void)
{

}
KLoudMessage::~KLoudMessage(void)
{

}

// 아래에 파라미터 추가된 함수를 
bool KLoudMessage::GetLoudMsg( IN int iItemID_, IN char cOffset_, IN std::wstring& strNickName_, OUT std::wstring& strLoudMsg_, IN const int nLanguageType_ )
{ // User 에서 호출한다. 언어 타입 넘기면 됨.
    std::wstring strItemName;
    {
        KLocker lock( m_csMsgNames );

        std::vector< boost::tuple<DWORD,char,std::wstring,DWORD, int> >::const_iterator vit;
        for( vit = m_vecNameTable.begin() ; vit != m_vecNameTable.end() ; ++vit )
        {
            if( vit->get<0>() == iItemID_ && vit->get<1>() == cOffset_ && vit->get<4>() == nLanguageType_ )
                break;
        }

        if( vit == m_vecNameTable.end() )
            return false;

        std::map< std::pair<int, DWORD>, std::wstring >::iterator mitMsg;
        mitMsg = m_mapMessageTable.find( std::make_pair( vit->get<4>(), vit->get<3>() ) );
        if( mitMsg == m_mapMessageTable.end() )
            return false;

        strItemName = vit->get<2>();
        strLoudMsg_ = mitMsg->second;
    }

    boost::replace_all( strLoudMsg_, L"__Nick__", strNickName_ );
    boost::replace_all( strLoudMsg_, L"__Item__", strItemName );
    return true;
}

bool KLoudMessage::GetLoudMsgByComment( IN int nCommoneID_, IN std::wstring& strNickName_, OUT std::wstring& strLoudMsg_, IN int nLanguageCode_ )
{
    {
        KLocker lock( m_csMsgNames );

        std::map< std::pair<int, DWORD>, std::wstring >::iterator mitMsg;
        mitMsg = m_mapMessageTable.find( std::make_pair(nCommoneID_, nLanguageCode_) );
        if( mitMsg == m_mapMessageTable.end() )
            return false;

        strLoudMsg_ = mitMsg->second;
    }

    boost::replace_all( strLoudMsg_, L"__Nick__", strNickName_ );
    return true;
}

bool KLoudMessage::GetItemName( IN int iItemID_, IN char cOffset_, OUT std::wstring& strItemName_, IN int nLanguageCode_ )
{
    KLocker lock( m_csMsgNames );
    std::vector< boost::tuple<DWORD,char,std::wstring,DWORD,int> >::const_iterator vit;
    for( vit = m_vecNameTable.begin() ; vit != m_vecNameTable.end() ; ++vit )
    {
        if( vit->get<0>() == iItemID_ && vit->get<1>() == cOffset_ && vit->get<4>() == nLanguageCode_ )
        {
            strItemName_ = vit->get<2>();
            return true;
        }
    }
    return false;
}

void KLoudMessage::UpdateMsgNames( std::map< std::pair< int, DWORD >,std::wstring>& mapMessageTable_, std::vector< boost::tuple<DWORD,char,std::wstring,DWORD,int> >& vecNameTable_ )
{
    if( mapMessageTable_.empty() || vecNameTable_.empty() )
    {
        START_LOG( clog, L"mapMessageTable_.empty() OR vecNameTable_.empty()" ) << END_LOG;
    }

    {
        KLocker lock( m_csMsgNames );
        m_mapMessageTable.clear();
        m_vecNameTable.clear();
        m_mapMessageTable   = mapMessageTable_;
        m_vecNameTable  = vecNameTable_;
    }
    START_LOG( clog, L" Update Msg And Names Table" )
        << BUILD_LOG( m_mapMessageTable.size() )
        << BUILD_LOG( m_vecNameTable.size() ) << END_LOG;

}

bool KLoudMessage::GetLoudMsg( IN int iItemID_, IN char cOffset_, IN std::wstring& strNickName_, OUT std::wstring& strLoudMsg_, 
                              IN std::wstring& strDungeonName_, IN std::wstring& strGettingMethod_, IN const int nLanguageType_ )
{
    std::wstring strItemName;
    {
        KLocker lock( m_csMsgNames );

        std::vector< boost::tuple<DWORD,char,std::wstring,DWORD,int> >::const_iterator vit;
        for( vit = m_vecNameTable.begin() ; vit != m_vecNameTable.end() ; ++vit )
        {
            if( vit->get<0>() == iItemID_ && vit->get<1>() == cOffset_ && vit->get<4>() == nLanguageType_ )
                break;
        }

        if( vit == m_vecNameTable.end() )
            return false;

        std::map< std::pair< int, DWORD>, std::wstring >::iterator mitMsg;
        mitMsg = m_mapMessageTable.find( std::make_pair( vit->get<4>(), vit->get<3>() ) );
        if( mitMsg == m_mapMessageTable.end() )
            return false;

        strItemName = vit->get<2>();
        strLoudMsg_ = mitMsg->second;
    }

    boost::replace_all( strLoudMsg_, L"__Nick__", strNickName_ );
    boost::replace_all( strLoudMsg_, L"__Item__", strItemName );
    boost::replace_all( strLoudMsg_, L"__Where__", strDungeonName_ );
    boost::replace_all( strLoudMsg_, L"__How__", strGettingMethod_ );
    return true;
}

bool KLoudMessage::GetCoupleMsg( IN std::wstring& strNick1_, IN std::wstring& strNick2_, OUT std::wstring& strMgs_,  IN const int nLanguageType_ )
{
    if( !GetMsg( MSG_COUPLE_KEY, strMgs_, nLanguageType_ ) ) return false;

    boost::replace_all( strMgs_, L"__Nick1__", strNick1_ );
    boost::replace_all( strMgs_, L"__Nick2__", strNick2_ );
    return true;
}

bool KLoudMessage::GetMsg( DWORD dwKey_, std::wstring& strMsg_, IN const int nLanguageType_ /* = 0 */ )
{
    std::map< std::pair< int, DWORD >, std::wstring >::iterator mit;
    KLocker lock( m_csMsgNames );
    mit = m_mapMessageTable.find( std::make_pair( nLanguageType_, dwKey_ ) );
    if( mit == m_mapMessageTable.end() ) return false;
    strMsg_ = mit->second;
    return true;
}

bool KLoudMessage::GetItemNickMsg( IN DWORD dwCommentID_, IN GCITEMID nItemID_, IN std::wstring& strNick_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    std::wstring strItemName;
    {
        KLocker lock( m_csMsgNames );

        std::vector< boost::tuple<DWORD,char,std::wstring,DWORD,int> >::const_iterator vit;
        for( vit = m_vecNameTable.begin() ; vit != m_vecNameTable.end() ; ++vit )
        {
            if( vit->get<0>() == nItemID_ && vit->get<1>() == 0 )
                break;
        }

        if( vit == m_vecNameTable.end() )
            return false;

        if( dwCommentID_ != vit->get<3>() )
        {
            START_LOG( clog, L"스트링번호가 일치하지 않습니다. ItemID : " << nItemID_ )
                << BUILD_LOG( dwCommentID_ )
                << BUILD_LOG( vit->get<3>() ) << END_LOG;
            return false;
        }

        strItemName = vit->get<2>();
    }

    if( !GetMsg( dwCommentID_, strMgs_, nLanguageType_ ) ) return false;

    boost::replace_all( strMgs_, L"__Nick__", strNick_ );
    boost::replace_all( strMgs_, L"__Item__", strItemName );
    return true;
}

bool KLoudMessage::GetStrongMsg( IN GCITEMID nItemID_, IN std::wstring& strNick_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    return GetItemNickMsg( MSG_STRONG_KEY, nItemID_, strNick_, strMgs_, nLanguageType_ );
}

bool KLoudMessage::GetGachaMsg( IN GCITEMID nItemID_, IN std::wstring& strNick_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    return GetItemNickMsg( MSG_GACHA_KEY, nItemID_, strNick_, strMgs_, nLanguageType_ );
}

bool KLoudMessage::GetMissionMsg( IN DWORD dwTime_, OUT std::wstring& strMgs_ )
{
    if( !GetMsg( MSG_MISSION_KEY, strMgs_, 0 ) ) return false;

    std::wstring wstr = boost::lexical_cast<std::wstring>(dwTime_);

    boost::replace_all( strMgs_, L"__Time__", wstr );
    return true;
}

bool KLoudMessage::GetItemDonationMsg( IN DWORD dwGrade_, OUT std::wstring& strMgs_ )
{
    if( !GetMsg( MSG_DONATION_KEY, strMgs_,0 ) ) return false;

    std::wstring wstr = boost::lexical_cast<std::wstring>(dwGrade_);

    boost::replace_all( strMgs_, L"__Grade__", wstr );
    return true;
}

bool KLoudMessage::GetStrengthMsg( IN std::wstring strNick_, IN DWORD dwStrengthLevel_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    if( !GetMsg( MSG_STRENGTH_KEY, strMgs_, nLanguageType_ ) ) return false;

    std::wstring wstrLevel = boost::lexical_cast<std::wstring>(dwStrengthLevel_);

    boost::replace_all( strMgs_, L"__Nick__", strNick_ );
    boost::replace_all( strMgs_, L"__StrengthLevel__", wstrLevel );
    return true;
}

bool KLoudMessage::GetGWCEventMsg( IN std::wstring wstrCountryName_, IN DWORD dwGrade_, OUT std::wstring& strMgs_ )
{
    if( !GetMsg( MSG_GWCEVENT_KEY, strMgs_ ) ) return false;

    std::wstring wstrGrade = boost::lexical_cast<std::wstring>(dwGrade_);

    boost::replace_all( strMgs_, L"__Country__", wstrCountryName_ );
    boost::replace_all( strMgs_, L"__Grade__", wstrGrade );
    return true;
}

bool KLoudMessage::GetEclipseMsg( OUT std::wstring& wstrStartEventMgs_, OUT std::wstring& wstrEndEventMgs_, IN const int& nLanguageType_ /*= 0*/ )
{
    if ( !GetMsg( MSG_ECLIPSE_START_KEY, wstrStartEventMgs_, nLanguageType_ ) ) return false;
    if ( !GetMsg( MSG_ECLIPSE_END_KEY, wstrEndEventMgs_, nLanguageType_ ) ) return false;

    return true;
}

bool KLoudMessage::GetLevelResetMsg( IN std::wstring strNick_, IN DWORD dwLevel_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    if( !GetMsg( MSG_LEVEL_RESET_EVENT_REWARD, strMgs_, nLanguageType_ ) ) return false;

    std::wstring wstrLevel = boost::lexical_cast<std::wstring>( dwLevel_ );

    boost::replace_all( strMgs_, L"__Nick__", strNick_ );
    boost::replace_all( strMgs_, L"__Level__", wstrLevel );
    return true;
}

bool KLoudMessage::GetGachaLotteryMsg( IN std::wstring strNick_, IN std::wstring wstrGrade_, OUT std::wstring& strMgs_, IN const int nLanguageType_ )
{
    if( !GetMsg( MSG_GACHA_LOTTERY_KEY, strMgs_, nLanguageType_ ) ) return false;

    boost::replace_all( strMgs_, L"__Nick__", strNick_ );
    boost::replace_all( strMgs_, L"__Level__", wstrGrade_ );
    return true;
}

bool KLoudMessage::GetGachaPonMsg(IN std::wstring strNick_, OUT std::wstring& strMgs_, IN const int nLanguageType_)
{
    if (!GetMsg(MSG_GACHA_PON_KEY, strMgs_, nLanguageType_)) return false;

    boost::replace_all(strMgs_, L"__Nick__", strNick_);
    return true;
}