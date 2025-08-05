#pragma once

class KGCPostLetterManager
{
    DeclareSingleton( KGCPostLetterManager );

public:
    KGCPostLetterManager( void );
    ~KGCPostLetterManager( void );

public:
    enum { PAGE_PER_LETTER = 5 };   // 한 페이지에 편지 몇개 표시하는가?

private:
    std::map< int, std::vector< KPostItemTitleInfo > > m_mapPostLetterPage; // 페이지별 우편 리스트 정보 
    std::map< POSTUID, KPostItemInfo >    m_mapPostLtterInfo;     // 각 편지별 상세 정보 

    int  m_iCurPage;            // 현재 페이지 
    int  m_iTotalPage;          // 총 페이지 
    int  m_iTotalLetterCount;   // 총 편지 수 
    int  m_iTotalNotReadLetterCount;   // 총 안 읽은 편지 수 
    char m_cLetterListType;     // 현재 보고 있는 편지 리스트 타입( 시스템 / 유저 / 전체 )
    bool m_bRecvNewLetter;      // 새 편지가 왔나?

public:
    void SetNewLetter( bool bSet_ ) { m_bRecvNewLetter = bSet_; }
    bool IsNewLetterRecv( void ) { return m_bRecvNewLetter; }

    int GetCurPageNum( void ) { return m_iCurPage; }
    int GetTotalPageNum( void ) { return m_iTotalPage; }

    void GetPostLetterPageInfoReq( int iPageNum_, const char cLetterListType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );
    void RefreshPostLetterPageInfo( IN int iPageNum_, IN const char cLetterType = KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );
    //void SetPageInfo( const int iPageNum_, const int iTotalCount_, const std::vector< KPostItemTitleInfo >& vecPageInfo_ );
    void SetPageInfo( const int iPageNum_, const int iTotalCount_, std::vector< KPostItemTitleInfo >& vecPageInfo_ );
    
    void SetTotalLetterCount( const int iCount_ ) { m_iTotalLetterCount = iCount_; }
    int  GetTotalLetterCount( void ) { return m_iTotalLetterCount; }

    void GetReadPostLetterInfoReq( POSTUID PostUID_ );
    void SetReadPostLetterInfo( KPostItemInfo& info_ );

    void SetTotalNotReadLetterCount( const int iCount_ ) { m_iTotalNotReadLetterCount = iCount_; }
    int  GetTotalNotReadLetterCount( void ) { return m_iTotalNotReadLetterCount; }

    bool GetPageLetterInfo( IN int iPage_, IN int iIndex_, OUT KPostItemTitleInfo& Info_ );
    bool SetPageLetterInfo( IN int iPage_, IN int iIndex_, IN KPostItemTitleInfo& Info_ );
    bool GetReadPostLetterInfo( IN const POSTUID postUID_, OUT KPostItemInfo& Info_ );

    void RecieveItemGP( IN const POSTUID postUID_ );
    void ClearItemInfo( IN const int iPage_, IN const POSTUID postUID_ );

    void ClearPostInfo( void );
    void ClearAllLetterInfo( void );
};

DefSingletonInline( KGCPostLetterManager );