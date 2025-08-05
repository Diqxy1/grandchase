/// @file   KDlgPrivateInfo.h
/// @author xu362@kogstudios.com

#ifndef _KDLGPRIVATEINFO_H_
#define _KDLGPRIVATEINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
#include <tchar.h>
//#include "KAssert.h"
#include "resource.h"

/// - 바뀌어진 개인정보 동의안에 반드시 동의해야만 게임을 할 수 있음
class KDlgPrivateInfo
{
private: // Disable construction of this class!
    /// Default constructor - disabled.
    KDlgPrivateInfo( void )
    { ASSERT( false && _T("Do not use default constructor!") ); }
    /// Copy constructor - disabled.
    KDlgPrivateInfo( const KDlgPrivateInfo& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Default destructor.
    ~KDlgPrivateInfo( void ) {}
    /// Assignment operator - disabled.
    const KDlgPrivateInfo& operator=( const KDlgPrivateInfo& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:

    enum { USER_AGREEMENT = 0, USER_AGREEMENT_SHOP };
    static INT_PTR DoModal( HWND hParentWnd = NULL, int iType = USER_AGREEMENT );
};

#endif // _KDLGPRIVATEINFO_H_