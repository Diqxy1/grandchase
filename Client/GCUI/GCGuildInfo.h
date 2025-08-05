#ifndef _GCGUILDINFO_H_
#define _GCGUILDINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "UserPacket.h"

class KD3DStatic;
class KD3DListBox;

class KGCGuildInfo : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCGuildInfo );
    /// Default constructor
    KGCGuildInfo( void );
    /// Default destructor
    virtual ~KGCGuildInfo( void );
    /// Copy constructor
    KGCGuildInfo( const KGCGuildInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildInfo& operator=( const KGCGuildInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_MAX_COMMANDER = 5,
    };

public:
    void SetGuildMark( int iMark );
    void SetGuildName( const std::wstring& strName );
    void SetGuildPoint( int iPoint );
    void SetGuildMaster( const std::wstring& strMaster );
    void ClearGuildCommander( void );
    void AddGuildCommander( const std::wstring& strCommander );
    void SetGuildMemberInfo( int iCurNum, int iMaxNum );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected:
    int             m_iMark;
    KD3DStatic*     m_pkGuildName;
    KD3DStatic*     m_pkGuildPoint;
    KD3DStatic*     m_pkGuildMaster;
    KD3DListBox*    m_pkGuildCommander;
    KD3DStatic*     m_pkGuildMemberInfo;
};

DEFINE_WND_FACTORY( KGCGuildInfo );
DECLARE_WND_FACTORY( KGCGuildInfo );
DECLARE_WND_FACTORY_NAME( KGCGuildInfo );

#endif // _GCGUILDINFO_H_