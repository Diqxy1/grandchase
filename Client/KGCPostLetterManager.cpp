#include "stdafx.h"
#include "Procedure.h"
#include "Message.h"
#include "KSingleton.h"

#include "KGCPostLetterManager.h"
#include "gcui/GCPostLetter.h"

ImplementSingleton( KGCPostLetterManager );

KGCPostLetterManager::KGCPostLetterManager( void )
{
    m_mapPostLetterPage.clear();
    m_mapPostLtterInfo.clear();

    m_iCurPage = -1;
    m_iTotalPage = -1;
    m_iTotalLetterCount = -1;
    m_iTotalNotReadLetterCount = 0;
    m_cLetterListType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;
    m_bRecvNewLetter = false;
}

KGCPostLetterManager::~KGCPostLetterManager( void )
{
    m_mapPostLetterPage.clear();
    m_mapPostLtterInfo.clear();

    m_iCurPage = -1;
    m_iTotalPage = -1;
    m_cLetterListType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;
    m_bRecvNewLetter = false;
}

void KGCPostLetterManager::GetPostLetterPageInfoReq( int iPageNum_, const char cLetterListType /*= KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL*/ )
{
    if ( iPageNum_ > m_iTotalPage && m_iTotalPage > 0  ) 
    {
        iPageNum_ = m_iTotalPage;
    }

    if( IsNewLetterRecv() ) { 
        RefreshPostLetterPageInfo( iPageNum_, cLetterListType );
        return;
    }

    std::map< int, std::vector< KPostItemTitleInfo > >::iterator mit = m_mapPostLetterPage.find( iPageNum_ );
    if( m_mapPostLetterPage.end() == mit ) { 
        Result_PostLetter = INT_MAX;
        KP2P::GetInstance()->Send_GetPostLetterListReq( iPageNum_, cLetterListType );
        g_MyD3D->WaitForServerAck( Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE );
    }
    
    m_iCurPage = iPageNum_;
    m_cLetterListType = cLetterListType;
}

void KGCPostLetterManager::RefreshPostLetterPageInfo( IN int iPageNum_, IN const char cLetterType /*= KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL */ )
{
    if (iPageNum_ > m_iTotalPage && m_iTotalPage > 0)
    {
        iPageNum_ = m_iTotalPage;
    }

    m_mapPostLetterPage.clear();

    Result_PostLetter = INT_MAX;
    KP2P::GetInstance()->Send_GetPostLetterListReq(iPageNum_, cLetterType);
    g_MyD3D->WaitForServerAck(Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE);

    m_iCurPage = iPageNum_;
    m_cLetterListType = cLetterType;
}

//void KGCPostLetterManager::SetPageInfo( const int iPageNum_, const int iTotalCount_, const std::vector< KPostItemTitleInfo >& vecPageInfo_ )
void KGCPostLetterManager::SetPageInfo( const int iPageNum_, const int iTotalCount_, std::vector< KPostItemTitleInfo >& vecPageInfo_ )
{
    m_iCurPage = iPageNum_;
    m_iTotalPage = static_cast<int>(iTotalCount_ / PAGE_PER_LETTER);

    if (0 != iTotalCount_ % PAGE_PER_LETTER) {
        ++m_iTotalPage;
    }

    std::map< int, std::vector< KPostItemTitleInfo > >::iterator mit = m_mapPostLetterPage.find(iPageNum_);
    if (m_mapPostLetterPage.end() != mit) {
        m_mapPostLetterPage.erase(mit);
    }

    for (std::vector< KPostItemTitleInfo >::iterator vit = vecPageInfo_.begin(); vit != vecPageInfo_.end(); ++vit) {
        for (std::vector<GCITEMID>::iterator itemIter = vit->m_vecItemID.begin(); itemIter != vit->m_vecItemID.end(); ) {
            if (*itemIter == 0) {
                itemIter = vit->m_vecItemID.erase(itemIter);
            }
            else {
                ++itemIter;
            }
        }
    }

    m_mapPostLetterPage.insert(std::make_pair(iPageNum_, vecPageInfo_));
}

void KGCPostLetterManager::GetReadPostLetterInfoReq( POSTUID PostUID_ )
{
    Result_PostLetter = INT_MAX;
    KP2P::GetInstance()->Send_ReadLetterReq(PostUID_);
    g_MyD3D->WaitForServerAck(Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE);
}

void KGCPostLetterManager::SetReadPostLetterInfo( KPostItemInfo& info_ )
{
    for (std::vector<KItem>::iterator vit = info_.m_vecItem.begin(); vit != info_.m_vecItem.end(); ) {
        if (vit->m_ItemID == 0) {
            vit = info_.m_vecItem.erase(vit);
        }
        else {
            ++vit;
        }
    }
    m_mapPostLtterInfo.insert(std::make_pair(info_.m_PostUID, info_));
}

bool KGCPostLetterManager::GetPageLetterInfo( IN int iPage_, IN int iIndex_, OUT KPostItemTitleInfo& Info_ )
{
    std::map< int, std::vector< KPostItemTitleInfo > >::iterator mit = m_mapPostLetterPage.find(iPage_);
    if (m_mapPostLetterPage.end() == mit) {
        return false;
    }

    if (iIndex_ < 0 || iIndex_ >= static_cast<int>(mit->second.size())) {
        return false;
    }

    Info_ = mit->second[iIndex_];
    return true;
}

bool KGCPostLetterManager::SetPageLetterInfo( IN int iPage_, IN int iIndex_, IN KPostItemTitleInfo& Info_ )
{
    std::map< int, std::vector< KPostItemTitleInfo > >::iterator mit = m_mapPostLetterPage.find(iPage_);
    if (m_mapPostLetterPage.end() == mit) {
        return false;
    }

    if (iIndex_ < 0 || iIndex_ >= static_cast<int>(mit->second.size())) {
        return false;
    }

    mit->second[iIndex_] = Info_;
    return true;
}

bool KGCPostLetterManager::GetReadPostLetterInfo( IN const POSTUID postUID_, OUT KPostItemInfo& Info_ )
{
    std::map< POSTUID, KPostItemInfo >::iterator mit = m_mapPostLtterInfo.find(postUID_);

    if (mit != m_mapPostLtterInfo.end()) {
        Info_ = mit->second;
        return true;
    }

    GetReadPostLetterInfoReq(postUID_);
    mit = m_mapPostLtterInfo.find(postUID_);
    if (mit == m_mapPostLtterInfo.end()) {
        return false;
    }

    Info_ = mit->second;
    return true;
}

void KGCPostLetterManager::RecieveItemGP( IN const POSTUID postUID_ )
{
    // 아이템 받기 요청 
    KP2P::GetInstance()->Send_GetItemFromLetterReq( postUID_ );
}

void KGCPostLetterManager::ClearItemInfo( IN const int iPage_, IN const POSTUID postUID_ )
{
    // 페이지 정보에서 삭제 
    std::map< int, std::vector< KPostItemTitleInfo > >::iterator pageItr = m_mapPostLetterPage.find( iPage_ );
    if( pageItr != m_mapPostLetterPage.end() ) { 
        for( std::vector< KPostItemTitleInfo >::iterator vit = pageItr->second.begin() ; vit != pageItr->second.end() ; ++vit ) { 
            if( vit->m_PostUID == postUID_ ) { 
                vit->m_vecItemID.clear();
                vit->m_nGamePoint = 0;
                break;
            }
        }
    }

    // 편지 정보에서 삭제 
    std::map< POSTUID, KPostItemInfo >::iterator infoItr = m_mapPostLtterInfo.find( postUID_ );
    if( infoItr != m_mapPostLtterInfo.end() ) { 
        infoItr->second.m_nGamePoint = 0;
        infoItr->second.m_vecItem.clear();
        infoItr->second.m_bGetItemGP = true;
    }
}

void KGCPostLetterManager::ClearPostInfo( void )
{
    m_mapPostLetterPage.clear();
    m_mapPostLtterInfo.clear();
}

void KGCPostLetterManager::ClearAllLetterInfo( void )
{
    m_mapPostLetterPage.clear();
    m_mapPostLtterInfo.clear();

    m_iCurPage = -1;
    m_iTotalPage = -1;
    m_iTotalLetterCount = -1;
    m_cLetterListType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL;
    m_bRecvNewLetter = false;
}