#include "UserLetterBox.h"
#include "PostConfig.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

KUserLetterBox::KUserLetterBox(void)
{
}

KUserLetterBox::~KUserLetterBox(void)
{
}

void KUserLetterBox::Clear()
{
    m_mapLetters.clear();
    m_vecUserLetterList.clear();
    m_vecSystemLetterList.clear();
    m_setBlackList.clear();
}

void KUserLetterBox::Init( IN const std::vector< KPostItemInfo >& vecPostItems_, 
                          OUT int& nNewUserLetterCount_, 
                          OUT int& nNewSystemLetterCount_,
                          OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle_ )
{
    // PostNo가 작을수록 옛날 편지
    std::vector< KPostItemInfo >::const_iterator vit;
    for ( vit = vecPostItems_.begin() ; vit != vecPostItems_.end() ; ++vit ) {
        AddLetter( *vit );
    }

    // 읽지 않은 편지 제목
    // 시스템 메일은 AddLetter에서 문장을 완성하기 때문에.
    GetRenewalLetter( nNewUserLetterCount_, nNewSystemLetterCount_, vecNewLetterTitle_ );
}

// GetRenewalLetter() 안에서 날자 비교를 위해 사용되어짐.
bool ComRenewalLetter( IN const KPostItemTitleInfo& _First, IN const KPostItemTitleInfo& _Last )
{
    return ( _First.m_tmRegDate > _Last.m_tmRegDate );
}

void KUserLetterBox::GetRenewalLetter( OUT int& nNewUserLetterCount_, OUT int& nNewSystemLetterCount_, OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle_ )
{
    nNewUserLetterCount_ = 0;
    nNewSystemLetterCount_ = 0;
    vecNewLetterTitle_.clear();

    USHORT usNewLetterTitleCount = SiKPostConfig()->GetNewLetterTitleCount();
    std::vector< KPostItemTitleInfo > vecTemp;
    std::vector< KPostItemTitleInfo >::iterator vit;

    //#1.유저메일 리스트중 읽지않은 메일을 찾아낸다.
    for ( vit = m_vecUserLetterList.begin() ; vit != m_vecUserLetterList.end() ; ++vit ) {
        if ( false == vit->m_bRead ) {
            vecTemp.push_back( *vit );
            ++nNewUserLetterCount_;
        }
    }

    //#2.시스템메일 리스트중 읽지않은 메일을 찾아낸다.
    for ( vit = m_vecSystemLetterList.begin() ; vit != m_vecSystemLetterList.end() ; ++vit ) {
        if( false == vit->m_bRead ) {
            vecTemp.push_back( *vit );
            ++nNewSystemLetterCount_;
        }
    }

    //#3.읽지않은 메일을 최근순으로 정렬한다.
    std::sort(vecTemp.begin(), vecTemp.end(), &ComRenewalLetter );

    //#4.읽지않은 5개의 메일을 뽑아내어 넣어준다.
    for ( USHORT i = 0 ; i < vecTemp.size() && i < usNewLetterTitleCount; ++i ) {
        KPostSmallTitleInfo kData;
        kData.m_cScriptType = vecTemp[i].m_cScriptType;
        kData.m_strLetterTitle = vecTemp[i].m_strTitle;
        vecNewLetterTitle_.push_back( kData );
    }

    START_LOG( clog, L"GetRenewalLetter" )
        << BUILD_LOG( nNewUserLetterCount_ )
        << BUILD_LOG( nNewSystemLetterCount_ )
        << BUILD_LOG( m_vecUserLetterList.size() )
        << BUILD_LOG( m_vecSystemLetterList.size() )
        << BUILD_LOG( vecNewLetterTitle_.size() ) << END_LOG;
}

void KUserLetterBox::AddLetter( IN const KPostItemInfo& kPostItem_ )
{
    // map
    m_mapLetters.insert( std::make_pair( kPostItem_.m_PostUID, kPostItem_ ) );

    // list
    KPostItemTitleInfo kPostTitle;
    kPostTitle.SetData( kPostItem_ );

    if ( kPostTitle.m_cScriptType == KPostItemInfo::LT_USER ) {
        m_vecUserLetterList.push_back( kPostTitle );
    }
    else {
        m_vecSystemLetterList.push_back( kPostTitle );
    }
}

bool KUserLetterBox::IsExist( IN const POSTUID& PostUID_ )
{
    std::map<POSTUID,KPostItemInfo>::const_iterator cmit;
    cmit = m_mapLetters.find( PostUID_ );
    if ( cmit == m_mapLetters.end() ) {
        return false;
    }
    return true;
}

bool KUserLetterBox::IsExist( IN const std::vector<POSTUID>& vecPostNo_ )
{
    std::vector<POSTUID>::const_iterator cvit;
    for ( cvit = vecPostNo_.begin() ; cvit != vecPostNo_.end() ; ++cvit ) {
        if ( false == IsExist( *cvit ) ) {
            START_LOG( cerr, L"해당 우편이 없음.. PostUID : " << *cvit ) << END_LOG;
            return false;
        }
    }
    return true;
}

bool KUserLetterBox::IsSystemLetter( IN const POSTUID& PostUID_ )
{
    std::map<POSTUID,KPostItemInfo>::const_iterator cmit;
    cmit = m_mapLetters.find( PostUID_ );
    if ( cmit == m_mapLetters.end() ) {
        START_LOG( cerr, L"편지가 없네.. PostNo : " << PostUID_ ) << END_LOG;
        return false;
    }

    return cmit->second.IsSystemLetter();
}

bool KUserLetterBox::GetLetter( IN const POSTUID& PostUID_, OUT KPostItemInfo& kPostItem_ )
{
    std::map<POSTUID,KPostItemInfo>::const_iterator cmit;
    cmit = m_mapLetters.find( PostUID_ );
    if ( cmit == m_mapLetters.end() ) {
        return false;
    }

    kPostItem_ = cmit->second;
    return true;
}

bool KUserLetterBox::ReadLetter( IN const POSTUID& PostUID_, OUT KPostItemInfo& kPostItem_, OUT bool& bReadUpdated_ )
{
    std::map<POSTUID,KPostItemInfo>::iterator mit;
    mit = m_mapLetters.find( PostUID_ );
    if ( mit == m_mapLetters.end() ) {
        return false;
    }

    bReadUpdated_ = mit->second.m_bRead;
    mit->second.m_bRead = true;
    kPostItem_ = mit->second;

    // 시스템 우편과 일반 우편을 나눈다.
    if ( kPostItem_.IsSystemLetter() ) {
        std::vector< KPostItemTitleInfo >::iterator vit;
        for ( vit = m_vecSystemLetterList.begin() ; vit != m_vecSystemLetterList.end() ; ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                vit->m_bRead = true;
                break;
            }
        }
    }
    else
    {
        std::vector< KPostItemTitleInfo >::iterator vit;
        for ( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                vit->m_bRead = true;
                break;
            }
        }
    }

    return true;
}

int KUserLetterBox::GetTotalLetterCount( IN const char& cLetterListType_ )
{
    int nLetterCount = 0;
    switch ( cLetterListType_ ) {
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL:
            nLetterCount = m_vecSystemLetterList.size() + m_vecUserLetterList.size();
            break;
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_SYSTEM:
            nLetterCount = m_vecSystemLetterList.size();
            break;
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_USER:
            nLetterCount = m_vecUserLetterList.size();
            break;
        default:
            START_LOG( cerr, L"정의되지않은 우편 리스트 타입 : " << static_cast<int>(cLetterListType_) ) << END_LOG;
            break;
    }

    return nLetterCount;
}

bool KUserLetterBox::GetItemInfo( IN const POSTUID& PostUID_, std::vector<KItem>& vecNewItemInfo_ )
{
    vecNewItemInfo_.clear();

    std::map<POSTUID,KPostItemInfo>::const_iterator mit;
    mit = m_mapLetters.find( PostUID_ );
    if ( mit == m_mapLetters.end() ) {
        return false;
    }

    const KPostItemInfo& kPostItemInfo = mit->second;

    switch( kPostItemInfo.m_cScriptType )
    {
    case KPostItemInfo::LT_SYSTEM:
    case KPostItemInfo::LT_USER:
        break;

    default:
        {
            START_LOG( cerr, L"일반편지 또는 정의된 메시지가 아닌데 아이템정보를 얻으려 합니다." )
                << BUILD_LOG( PostUID_ )
                << BUILD_LOGc( kPostItemInfo.m_cScriptType )
                << END_LOG;
            return false;
        }
        break;
    }

    // 아이템 정보 담기
    vecNewItemInfo_ = kPostItemInfo.m_vecItem;
    return true;
}

bool KUserLetterBox::GetLetterList( IN const char& cLetterListType_, IN const int& nPageNo_, OUT std::vector<KPostItemTitleInfo>& vecLetterList_ )
{
    vecLetterList_.clear();

    USHORT usOnePageLetterCount = SiKPostConfig()->GetOnePageLetterCount();

    if ( nPageNo_ <= 0 ) {
        START_LOG( cerr, L"페이지 요청이 이상합니다." )
            << BUILD_LOGc( cLetterListType_ )
            << BUILD_LOG( nPageNo_ )
            << END_LOG;
        return false;
    }

    u_int iBeginIndex = ( nPageNo_ - 1 ) * usOnePageLetterCount;

    switch ( cLetterListType_ ) {
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL:
            {
                u_int iCnt = 0;
                std::map<POSTUID,KPostItemInfo>::reverse_iterator mit;
                for ( mit = m_mapLetters.rbegin() ; mit != m_mapLetters.rend() ; ++mit, ++iCnt ) {
                    if ( iBeginIndex > iCnt ) {
                        continue;
                    }

                    KPostItemTitleInfo kTitleInfo;
                    kTitleInfo.SetData( mit->second );
                    vecLetterList_.push_back( kTitleInfo );

                    if ( vecLetterList_.size() >= usOnePageLetterCount ) {
                        break;
                    }
                }
            }
            break;
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_SYSTEM:
            {
                u_int iCnt = 0;
                std::vector<KPostItemTitleInfo>::reverse_iterator vit;
                for ( vit = m_vecSystemLetterList.rbegin() ; vit != m_vecSystemLetterList.rend() ; ++vit, ++iCnt ) {
                    if ( iBeginIndex > iCnt ) {
                        continue;
                    }

                    vecLetterList_.push_back( *vit );

                    if ( vecLetterList_.size() >= usOnePageLetterCount ) {
                        break;
                    }
                }
            }
            break;
        case KEVENT_GET_POST_LETTER_LIST_REQ::LL_USER:
            {
                u_int iCnt = 0;
                std::vector<KPostItemTitleInfo>::reverse_iterator vit;
                for ( vit = m_vecUserLetterList.rbegin() ; vit != m_vecUserLetterList.rend() ; ++vit, ++iCnt ) {
                    if ( iBeginIndex > iCnt ) {
                        continue;
                    }

                    vecLetterList_.push_back( *vit );

                    if ( vecLetterList_.size() >= usOnePageLetterCount ) {
                        break;
                    }
                }
            }
            break;
        default:
            START_LOG( cerr, L"정의되지않은 우편 리스트 타입 : " << static_cast<int>(cLetterListType_) ) << END_LOG;
            break;
    }

    return true;
}

void KUserLetterBox::DeleteLetter( IN const POSTUID& PostUID_ )
{
    std::map<POSTUID,KPostItemInfo>::const_iterator mit;
    mit = m_mapLetters.find( PostUID_ );
    if ( mit == m_mapLetters.end() ) {
        return;
    }

    // 우편 리스트에서 삭제
    if ( mit->second.IsSystemLetter() ) {
        std::vector< KPostItemTitleInfo >::iterator vit;
        for ( vit = m_vecSystemLetterList.begin() ; vit != m_vecSystemLetterList.end() ; ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                break;
            }
        }

        if ( vit != m_vecSystemLetterList.end() ) {
            m_vecSystemLetterList.erase( vit );
        }
    }
    else {
        std::vector< KPostItemTitleInfo >::iterator vit;
        for ( vit = m_vecUserLetterList.begin() ; vit != m_vecUserLetterList.end() ; ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                break;
            }
        }

        if ( vit != m_vecUserLetterList.end() ) {
            m_vecUserLetterList.erase( vit );
        }
    }

    // 우편 map에서 삭제
    m_mapLetters.erase( PostUID_ );
}

void KUserLetterBox::DeleteLetter( IN const std::vector<POSTUID>& vecPostNo_ )
{
    std::vector<POSTUID>::const_iterator vit;
    for ( vit = vecPostNo_.begin() ; vit != vecPostNo_.end() ; ++vit ) {
        DeleteLetter( *vit );
    }
}

void KUserLetterBox::SetReceivedFlag( IN const POSTUID& PostUID_ )
{
    std::map<POSTUID,KPostItemInfo>::iterator mit;
    mit = m_mapLetters.find( PostUID_ );
    if ( mit == m_mapLetters.end() ) {
        return;
    }

    KPostItemInfo& kPostItemInfo = mit->second;
    kPostItemInfo.m_bRead = true;
    kPostItemInfo.m_bGetItemGP = true;

    // 첨부물 정보 초기화(?)
    kPostItemInfo.m_vecItem.clear();
    kPostItemInfo.m_nGamePoint = 0;

    if ( mit->second.IsSystemLetter() ) {
        std::vector<KPostItemTitleInfo>::iterator vit;
        for ( vit = m_vecSystemLetterList.begin() ; vit != m_vecSystemLetterList.end() ; ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                break;
            }
        }

        if ( vit != m_vecSystemLetterList.end() ) {
            vit->m_vecItemID.clear();
            vit->m_nGamePoint = 0;
        }
    }
    else
    {
        std::vector< KPostItemTitleInfo >::iterator vit;
        for ( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit ) {
            if ( vit->m_PostUID == PostUID_ ) {
                break;
            }
        }

        if ( vit != m_vecUserLetterList.end() ) {
            vit->m_vecItemID.clear();
            vit->m_nGamePoint = 0;
        }
    }
}

void KUserLetterBox::InitBlackList( IN const std::set<DWORD>& setBlackList_ )
{
    m_setBlackList = setBlackList_;
}

void KUserLetterBox::GetBlackList( OUT std::set< DWORD >& setBlackList_ )
{
    setBlackList_ = m_setBlackList;
}

bool KUserLetterBox::IsBlackList( IN const DWORD& dwUserUID_ )
{
    return ( m_setBlackList.find( dwUserUID_ ) != m_setBlackList.end() );
}

void KUserLetterBox::AddBlackList( IN const DWORD& dwUserUID_ )
{
    m_setBlackList.insert( dwUserUID_ );
}

void KUserLetterBox::DelBlackList( IN const DWORD& dwUserUID_ )
{
    m_setBlackList.erase( dwUserUID_ );
}