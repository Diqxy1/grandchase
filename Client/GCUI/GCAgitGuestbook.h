#pragma once

class KGCAgitGuestbook : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { NUM_SLOT = 4, NUM_LINK_PAGE = 10, LENGTH_MAX_MENT = 60 };
    enum PAGE_BTN { BTN_PREV = 0, BTN_NEXT = 1 };

public:
    DECLARE_CLASSNAME( KGCAgitGuestbook );
    /// Default constructor
    KGCAgitGuestbook( void );
    /// Default destructor
    virtual ~KGCAgitGuestbook( void );
    /// Copy constructor
    KGCAgitGuestbook( const KGCAgitGuestbook& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitGuestbook& operator=( const KGCAgitGuestbook& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnInput;
    KD3DEdit*   m_pkEditMent;
    KD3DStatic* m_pkStaticNumMent;
    KD3DStaticLink* m_pkStaticLink;
    KSafeArray< KGCGuestbookSlot*, NUM_SLOT >   m_apkSlot;

    int m_iCurPage;
    int m_iTotalPage;
    int m_iPageOffset;
    int m_iTotalMent;

    
    std::vector< std::pair< int, KGuestMsg > > m_vecGuestBook;
    std::map<int, KGuestMsg >                  m_mapGuestBook;  // 글 수가 많아질 때 검색 목적으로 가지고 있겠음 

public:
    void InitUI( void );
    void OnInput( void );
    void OnClose( void );
    void OnClickEditMent( void );
    void SetGuestbookInfo( std::map<int, KGuestMsg>& info_ );

    void AddNewMessage( int iMsgNum_, KGuestMsg& Msg_ );
    void DeleteMessage( std::set<int>& setDeleteID_ );

private:
    void InitPage( void );
    void UpdatePage( void );
    void SetLinkPage( int iStartPage );
    void SetPage( PAGE_BTN eBtn_ );
    void SetPage( int iPage_ );
    void SetSlot( int iPage_ );
    void SetEditNum( void );

    bool IsValidMent( std::wstring& strMent_ );
    bool IsValidLength( std::wstring& strMent_ );
};

DEFINE_WND_FACTORY( KGCAgitGuestbook );
DECLARE_WND_FACTORY( KGCAgitGuestbook );
DECLARE_WND_FACTORY_NAME( KGCAgitGuestbook );
