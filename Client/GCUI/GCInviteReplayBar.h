#ifndef _GCINVITEREPLAYBAR_H_
#define _GCINVITEREPLAYBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCInviteReplayBar : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCInviteReplayBar );
    /// Default constructor
    KGCInviteReplayBar( void );
    /// Default destructor
    virtual ~KGCInviteReplayBar( void );
    /// Copy constructor
    KGCInviteReplayBar( const KGCInviteReplayBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInviteReplayBar& operator=( const KGCInviteReplayBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EUserGrade
    {
        UG_VAGABOND,
        UG_MERCENARY,
        UG_ROYAL,
        UG_HOLY,
        NUM_USER_GRADE,
    };

public:
    void SetOnOff( bool bOnOff );
    void SetGuildChannel( bool bGuildChannel );
    void SetIndex( int iIndex );
    void SetMode( bool bInviteMode );
    void SetMark( int iMark );
    void SetUserNickName( const std::wstring& strNickName );
    void SetUserGrade( char cGrade );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected:
    bool        m_bGuildChannel;
    int         m_iIndex;
    int         m_iMark;
    bool        m_bInviteMode;
    char        m_cGrade;
    EUserGrade  m_eUserGrade;
    std::wstring m_strNickName;

    KSafeArray<KD3DWnd*,NUM_USER_GRADE> m_apkGrade;
    KD3DStatic* m_pkUserID;
    KD3DWnd*    m_pkInviteReplayBtn;
};

DEFINE_WND_FACTORY( KGCInviteReplayBar );
DECLARE_WND_FACTORY( KGCInviteReplayBar );
DECLARE_WND_FACTORY_NAME( KGCInviteReplayBar );

#endif // _GCINVITEREPLAYBAR_H_