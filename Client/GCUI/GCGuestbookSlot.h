#pragma once

class KGCGuestbookSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGuestbookSlot );
    /// Default constructor
    KGCGuestbookSlot( void );
    /// Default destructor
    virtual ~KGCGuestbookSlot( void );
    /// Copy constructor
    KGCGuestbookSlot( const KGCGuestbookSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuestbookSlot& operator=( const KGCGuestbookSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DStatic* m_pkStaticNumber;
    KD3DStatic* m_pkStaticNickname;
    KD3DStatic* m_pkstaticDate;
    KD3DStatic* m_pkStaticMent;
    KD3DWnd*    m_pkBtnDelete;

    int         m_iNum;         // 메시지 번호 
    DWORD       m_dwUserUID;    // 작성자 UID

public:
    void  SetSlot( int iNum_, DWORD dwUserUID_, std::wstring& strNick_, time_t tmDate_, std::wstring& strMent_ );
    void  Clear( void );
    
    int   GetMsgNum( void ) { return m_iNum; }
    DWORD GetUserUID( void ) { return m_dwUserUID; }    
    void  OnClickDelete( void );

private:
    std::wstring GetColorNickName( std::wstring& strNick_ );
    std::wstring GetFormatDate( time_t tmDate_ );
};

DEFINE_WND_FACTORY( KGCGuestbookSlot );
DECLARE_WND_FACTORY( KGCGuestbookSlot );
DECLARE_WND_FACTORY_NAME( KGCGuestbookSlot );
