#pragma once

class KGCCountryGroup;

class KGCRoomTitleS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomTitleS6 );
    /// Default constructor
    KGCRoomTitleS6( void );
    /// Default destructor
    virtual ~KGCRoomTitleS6( void );
    /// Copy constructor
    KGCRoomTitleS6( const KGCRoomTitleS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomTitleS6& operator=( const KGCRoomTitleS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnOptionClick();

public:
    void SetRoomName( int m_nRoomNum, const std::wstring& strName);
    void SetRoomPassword( const std::wstring& strPass );
    void SetPrimium();
    void SetCountryGroup();
    void LockOptionBtn( bool bLock );

public:
    KD3DStatic*     m_pkRoomNameStatic;
    KD3DStatic*     m_pkRoomNumStatic;
    KD3DStatic*     m_pkRoomPassWordStatic;

    KD3DWnd*        m_pkRoomLockMark;
    KD3DWnd*        m_pkGCBlogMark;
    KD3DWnd*        m_pkGCClubMark;
    KD3DWnd*        m_pkGoldenTicketMark;
    KD3DWnd*        m_pkPremiumGCClub;
    KD3DWnd*        m_pkOptionButton;

    KGCCountryGroup*    m_pkCountryGroup;

};

DEFINE_WND_FACTORY( KGCRoomTitleS6 );
DECLARE_WND_FACTORY( KGCRoomTitleS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomTitleS6 );
