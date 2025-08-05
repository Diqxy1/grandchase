#ifndef _GCGUILDMEMBERBOX_H_
#define _GCGUILDMEMBERBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "UserPacket.h"
#include "Packet.h"
//#include "../GCEnum.h"

class KD3DStatic;

class KGCGuildMemberBox : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCGuildMemberBox );
    /// Default constructor
    KGCGuildMemberBox( void );
    /// Default destructor
    virtual ~KGCGuildMemberBox( void );
    /// Copy constructor
    KGCGuildMemberBox( const KGCGuildMemberBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMemberBox& operator=( const KGCGuildMemberBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_CHARACTER       = 3,
        MAX_CHAR_NAME_LEN   = 8,
        GUILD_MEMBER_INDEX  = 7,
    };


public:
    void SetGuildMemberInfo( const KDetailUserInfo& info );
	void SetGuildMemberInfo( const SUserInfo& info );

protected:
    void SetCharacterGrade( char cGrade );
    virtual void OnCreate( void );
    virtual void OnActivate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected:    
    UCHAR       m_ucCharType;

    KD3DStatic* m_pkID;
    KD3DStatic* m_pkClass;
    KD3DStatic* m_pkWinRate;
    KD3DStatic* m_pkContPoint;
    KD3DWnd*    m_pkInfo;

    KSafeArray<KD3DWnd*,GC_GRADE_NUM> m_apkGrade;
    int         m_iExtraCharNum;
};

DEFINE_WND_FACTORY( KGCGuildMemberBox );
DECLARE_WND_FACTORY( KGCGuildMemberBox );
DECLARE_WND_FACTORY_NAME( KGCGuildMemberBox );

extern KGCGuildMemberBox* g_pkGuildMemberBox;

#endif // _GCGUILDMEMBERBOX_H_
