#ifndef _KMSGBOX_H_
#define _KMSGBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
#include <tchar.h>
//#include "KAssert.h"
#include "resource.h"

/// 메세지를 사용자에게 보여주기 위한 메세지박스 클래스.
/// 그랜드체이스의 이미지를 사용하여 꾸며놓았다.
/// Open 메쏘드로 세가지 타입의 출력이 가능하며,
/// 그 증 기본 타입은 모델리스 다이얼로그 박스로서
/// 반드시 Close메쏘드로 닫아주어야 한다.
/// 나머지 타입들은 모달로 출력되므로 Close메쏘드를 사용하지 않아도 된다.
/// 패치 프로그램 뿐만 아니라, 게임 클라이언트에서도 사용하는 클래스이다.
class KMsgBox
{
private: // Disable construction of this class!
    /// Default constructor - disabled.
    KMsgBox(void)
    { ASSERT(false && _T("Do not use default constructor!")); }
    /// Copy constructor - disabled.
    KMsgBox(const KMsgBox&)
    { ASSERT(false && _T("Do not use copy constructor!")); }
    /// Default destructor.
    ~KMsgBox(void) {}
    /// Assignment operator - disabled.
    const KMsgBox& operator=(const KMsgBox&)
    { ASSERT(false && _T("Do not use assignment operator!")); return *this; }
public:
    enum // Message box type
    {
        MBOX_NOTHING,   ///< Dummy value.
        MBOX_DEFAULT,   ///< 버튼이 없는 기본 타입.
        MBOX_OK,        ///< "확인" 버튼만 있는 타입.
        MBOX_OKCANCEL,  ///< "확인", "취소" 버튼이 있는 타입.
    };
public:
    /// Open message box.
    static INT_PTR Open(const WCHAR* szTxtLn1,
                        const WCHAR* szTxtLn2,
                        UINT uType = MBOX_DEFAULT,
                        HWND hParentWnd = NULL);
    /// Close message box.
    static BOOL Close(int iResult = IDOK);
private:
    enum
    {
        // Message text line
        LINE_LENGTH = 36,
        LINE_INDENT = 16,
        FIRST_LINE  = 66,
        SECOND_LINE = 86,

        // Message box size
        MBOX_WIDTH  = 303,
        MBOX_HEIGHT = 145,
    };
private:
    /// Application-defined dialog box callback function.
    static BOOL CALLBACK MsgBoxProc(HWND hDlg, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam);
private:
    static HWND ms_hDlg;                      ///< Handle to message box.
    static UINT ms_uType;                     ///< Type of message box.
    static WCHAR ms_szTxtLn1[LINE_LENGTH + 1]; ///< First line message.
    static WCHAR ms_szTxtLn2[LINE_LENGTH + 1]; ///< Second line message.
};

#endif // _KMSGBOX_H_