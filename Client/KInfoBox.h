/// @file   KInfoBox.h
/// @author pch413@kogstudios.com
/// @since  2004-03-03 오전 11:11:29

#ifndef _KINFOBOX_H_
#define _KINFOBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
#include <tchar.h>
//#include "KAssert.h"
#include "resource.h"

/// @class KInfoBox 옵션 초기화에 관련된 정보 다이얼로그 박스를 띄워준다.
class KInfoBox
{
private: // Disable construction of this class!
    /// Default constructor - disabled.
    KInfoBox( void )
    { ASSERT( false && _T("Do not use default constructor!") ); }
    /// Copy constructor - disabled.
    KInfoBox( const KInfoBox& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Default destructor.
    ~KInfoBox( void ) {}
    /// Assignment operator - disabled.
    const KInfoBox& operator=( const KInfoBox& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:
    /// Open Infomation box.
    static INT_PTR DoModal( HWND hParentWnd = NULL );
    /// Open Infomation box in force.
    static INT_PTR ForceModal( HWND hParentWnd = NULL );
};

#endif // _KINFOBOX_H_