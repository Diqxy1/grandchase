#ifndef _GCUSERBAR_H_
#define _GCUSERBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KD3DStatic;

class KGCUserBar : public KD3DWnd, // extends
					public KActionListener  // implements
{
public:
    DECLARE_CLASSNAME( KGCUserBar );
    /// Default constructor
    KGCUserBar( void );
    /// Default destructor
    virtual ~KGCUserBar( void );
    /// Copy constructor
    KGCUserBar( const KGCUserBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUserBar& operator=( const KGCUserBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EUserStatus
    {
        US_WAIT,
        US_PLAY,
        US_LOBBY,
        US_SQUARE,
        NUM_USER_STATUS,
    };

    enum EUserGrade
    {
        UG_VAGABOND,
        UG_MERCENARY,
        UG_ROYAL,
        UG_HOLY,
        NUM_USER_GRADE,
    };

public:
    void SetGuildUserBar( bool bGuildUserBar );
    void SetGuildChannel( bool bGuildChannel );
    void SetMark( int iMark );
    void SetUserID( const std::wstring& strUserID );
    void SetUserNickName( const std::wstring& strUserNickName );
    void SetUserStatus( char cStatus, USHORT usRoomID );
    void SetUserStatus( EUserStatus eStatus );
    void SetUserGrade( char cGrade );

	void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected:
    bool        m_bGuildUserBar;
    int         m_iMark;
    bool        m_bGuildChannel;
    bool        m_bJoin;
    char        m_cStatus;
    USHORT      m_usRoomID;
    EUserStatus m_eUserStatus;
    EUserGrade  m_eUserGrade;
    std::wstring m_strLoginID;

    KD3DWnd*    m_pkWait;
    KD3DWnd*    m_pkPlay;
    KD3DWnd*    m_pkLobby;
    KD3DWnd*    m_pkJoin;
    KD3DWnd*    m_pkSquare;

    KSafeArray<KD3DWnd*,NUM_USER_GRADE> m_apkGrade;

    KD3DStatic* m_pkUserID;
};

DEFINE_WND_FACTORY( KGCUserBar );
DECLARE_WND_FACTORY( KGCUserBar );
DECLARE_WND_FACTORY_NAME( KGCUserBar );

#endif // _GCUSERBAR_H_