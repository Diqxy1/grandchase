#ifndef __KMSGBOX_H__
#define __KMSGBOX_H__

#include "stdafx.h"
#include "resource.h"

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
    enum
    {
        // Message box type
        MBOX_NOTHING,
        MBOX_DEFAULT,
        MBOX_OK,
        MBOX_OKCANCEL,
    };
public:
    static INT_PTR Open(const char* szTxtLn1,
                        const char* szTxtLn2,
                        HWND hParentWnd = NULL,
                        UINT uType = MBOX_DEFAULT);
    static BOOL Close(int iResult = 0);
private:
    enum
    {
        // Message text line
        LINE_LENGTH = 34,
        LINE_INDENT = 16,
        FIRST_LINE  = 66,
        SECOND_LINE = 86,

        // Message box size
        MBOX_WIDTH  = 303,
        MBOX_HEIGHT = 145,
    };
private:
    static BOOL CALLBACK MsgBoxProc(HWND hDlg, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam);
private:
    static HWND ms_hDlg;
    static UINT ms_uType;
    static char ms_szTxtLn1[LINE_LENGTH];
    static char ms_szTxtLn2[LINE_LENGTH];
};

#endif // __KMSGBOX_H__