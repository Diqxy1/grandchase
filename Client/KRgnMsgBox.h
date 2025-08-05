/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

★ 설계자 ☞ 이상호
★ 설계일 ☞ 2010년 1월 15일
★ E-Mail ☞ shmhlove@naver.com

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

#ifndef _KRGNMSGBOX_H_
#define _KRGNMSGBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KRgnMsgBox
{
public:
    KRgnMsgBox();
    ~KRgnMsgBox();
    inline static KRgnMsgBox* GetInstance()
    {   return &m_pKRgnMsgBox;   }

public:
    static BOOL Close(int iResult = IDOK);
    BOOL ImitClose(int iResult = IDOK);
    static INT_PTR Open(const WCHAR* szTxtLn1, const WCHAR* szTxtLn2,
                        UINT uType = MBOX_DEFAULT, HWND hParentWnd = NULL);
    INT_PTR ImitOpen(const WCHAR* szTxtLn1, const WCHAR* szTxtLn2,
        UINT uType = MBOX_DEFAULT, HWND hParentWnd = NULL);

private:
    static BOOL CALLBACK RgnDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL ImitRgnDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void OnInitDialog( HWND hDlg );
    void OnDestroyDialog( HWND hDlg );
    void OnLbuttonDown( HWND hDlg );
    void OnLbuttonUp( HWND hDlg );
    void OnMouseMove( HWND hDlg );
    void OnCommand( HWND hDlg, WPARAM wParam );
    void OnPaint( HWND hDlg );

public:
    enum
    {
        MBOX_DEFAULT,       // 버튼이 없는 기본 타입.
        MBOX_OK,            // "확인" 버튼만 있는 타입.
        MBOX_OKCANCEL,      // "확인", "취소" 버튼이 있는 타입.
        MBOX_YESNO,         // "예", "아니오" 버튼이 있는 타입.
    };
    enum
    {
        LINE_LENGTH = 36,   // 한 라인에 출력가능한 문자길이
    };

private:
    static KRgnMsgBox m_pKRgnMsgBox;

private:
    HDC             m_hDC;
    HDC             m_MemDC;
    HWND            m_hDlgWnd;
    HINSTANCE       m_hInstance;

private:
    HRGN            m_hDialogRGN;
    HBITMAP         m_BitBackground;

private:
    bool            m_bTrackMove;
    RECT            m_hWindowRect;
    POINT           m_cMouseDWPoint;
    POINT           m_cGapMouse;

private:
    UINT    m_iMsgBoxType;
    WCHAR   m_szLine1[ (LINE_LENGTH+1) ];
    WCHAR   m_szLine2[ (LINE_LENGTH+1) ];
};

#endif // _KRGNMSGBOX_H_