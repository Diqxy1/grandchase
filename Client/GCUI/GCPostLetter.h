#pragma once

class KGCRecvLetterSlot;
class KGCRecvLetterUI;

class KGCPostLetter : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPostLetter );
    /// Default constructor
    KGCPostLetter( void );
    /// Default destructor
    virtual ~KGCPostLetter( void );
    /// Copy constructor
    KGCPostLetter( const KGCPostLetter& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPostLetter& operator=( const KGCPostLetter& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum { NUM_RECV_SLOT = 5,
#if defined(NATION_USA)
		NUM_TOTAL_LETTER = 100
#else
		NUM_TOTAL_LETTER = 30 
#endif
	};
    enum { TAB_RECV = 0, TAB_SEND = 1, NUM_TAB = 2 };
    enum { LETTER_SYSTEM = 0, LETTER_NORMAL = 1, NUM_LETTER_TYPE = 2 };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

private:
    KD3DWnd*    m_pkWndReceive;
    KD3DWnd*    m_pkWndSend;
    KD3DWnd*    m_pkBtnClose;

    KD3DWnd*    m_pkBtnSelectAllDefault;
    KD3DWnd*    m_pkBtnSelectAllActive;

    KD3DWnd*    m_pkBtnDelete;
    KD3DWnd*    m_pkBtnDeleteAll;

    KD3DWnd*    m_pkBtnPrev;
    KD3DWnd*    m_pkBtnNext;

    KD3DStatic* m_pkStaticSystem;
    KD3DStatic* m_pkStaticNormal;
    KD3DStatic* m_pkStaticSelectAll;
    KD3DStatic* m_pkStaticCapacity;
    KD3DStatic* m_pkStaticPage;

    KGCRecvLetterUI* m_pkRecvUI;

    KSafeArray< KD3DWnd*, NUM_LETTER_TYPE > m_apkBtnLetterTypeDefault;
    KSafeArray< KD3DWnd*, NUM_LETTER_TYPE > m_apkBtnLetterTypeActive;
    
    KSafeArray< KD3DWnd*, NUM_TAB > m_apkTabDefault;
    KSafeArray< KD3DWnd*, NUM_TAB > m_apkTabActive;
    
    KSafeArray< KGCRecvLetterSlot*, NUM_RECV_SLOT > m_apkRecvSlot;

    int     m_iCurTab;
    char    m_cLetterType;

    int     m_iCurLetterCount;
    int     m_iTotalLetterCount;
    int     m_iCurPage;
    int     m_iTotalPage;

private:
    void OnClickClose( void );
    void OnClickTab( int iTab_ );
    void OnClickSystemLetter( void );
    void OnClickNormalLetter( void );
    void OnClickRecvSlot( int iIndex_ );
    void OnClickSelectAll( void );
    void OnClickPrev( void );
    void OnClickNext( void );
    void OnClickDelete( void );

    void SetLetterType( void );
    char GetLetterType( void ) { return m_cLetterType; }

    void GetLetterPageInfo( void );
    void SetCapacityAndPage( void );

    void SetRecvLetterSlot( void );
    void ShowRecvLetterUI( POSTUID postUID_ );

    bool IsNowLetterType( char cType_ );

public:
    void SetItemRecvInfo( IN const POSTUID postUID_ );
    void RefreshPageInfo( void );
    
    void DeleteLetter( void );
    void DeleteRecvUILetter( void );

    bool IsSelectAllLetter( void ) { return m_pkBtnSelectAllActive->IsRenderOn(); }
    void ToggleSelectAll( bool bToggle_ );

    void ClearLetterInfo( void );
};

DEFINE_WND_FACTORY( KGCPostLetter );
DECLARE_WND_FACTORY( KGCPostLetter );
DECLARE_WND_FACTORY_NAME( KGCPostLetter );
