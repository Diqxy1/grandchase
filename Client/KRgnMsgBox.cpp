/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

★ 설계자 ☞ 이상호
★ 설계일 ☞ 2010년 1월 15일
★ E-Mail ☞ shmhlove@naver.com

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

#include "stdafx.h"
#include "KRgnMsgBox.h"

KRgnMsgBox KRgnMsgBox::m_pKRgnMsgBox;
KRgnMsgBox::KRgnMsgBox()
: m_hDC( NULL )
, m_MemDC( NULL )
, m_hDlgWnd( NULL )
, m_hInstance( NULL )
, m_BitBackground( NULL )
, m_bTrackMove( false )
, m_iMsgBoxType( MBOX_DEFAULT )
{}

KRgnMsgBox::~KRgnMsgBox()
{}

INT_PTR KRgnMsgBox::Open(   const WCHAR* szTxtLn1,
                            const WCHAR* szTxtLn2,
                            UINT uType,     /* = MBOX_DEFAULT */
                            HWND hParentWnd /* = NULL */          )
{
    return KRgnMsgBox::GetInstance()->ImitOpen( szTxtLn1, szTxtLn2, uType, hParentWnd );
}

INT_PTR KRgnMsgBox::ImitOpen(const WCHAR* szTxtLn1, const WCHAR* szTxtLn2, UINT uType, HWND hParentWnd )
{
    if (NULL == hParentWnd) {
        hParentWnd = ::GetDesktopWindow();
    }

    ::ZeroMemory(m_szLine1, sizeof(m_szLine1));
    ::ZeroMemory(m_szLine2, sizeof(m_szLine2));
    StringCchCopyW(m_szLine1, LINE_LENGTH, szTxtLn1);
    StringCchCopyW(m_szLine2, LINE_LENGTH, szTxtLn2);

    m_iMsgBoxType = uType;

    if( MBOX_DEFAULT == m_iMsgBoxType )
        return (INT_PTR)CreateDialogA( GetModuleHandle(NULL), MAKEINTRESOURCEA(IDD_REGION_MSGBOX_DEFAULT), hParentWnd, RgnDialogProc );
    else
        return (INT_PTR)DialogBoxA( GetModuleHandle(NULL), MAKEINTRESOURCEA(IDD_REGION_MSGBOX_DEFAULT), hParentWnd, RgnDialogProc );
}

BOOL KRgnMsgBox::Close( int iResult /* = IDOK */ )
{
    return KRgnMsgBox::GetInstance()->ImitClose( iResult );
}

BOOL KRgnMsgBox::ImitClose(int iResult)
{
    if( NULL != m_hDlgWnd )
    {
        if( MBOX_DEFAULT == m_iMsgBoxType )
        {
            ::SendMessage( m_hDlgWnd, WM_CLOSE, 0, 0 );
            return TRUE;
        }
        else
        {
            return ::EndDialog( m_hDlgWnd, iResult );
        }
    }
    return FALSE;
}

BOOL CALLBACK KRgnMsgBox::RgnDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return KRgnMsgBox::GetInstance()->ImitRgnDialogProc( hDlg, uMsg, wParam, lParam );
}

BOOL KRgnMsgBox::ImitRgnDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch( uMsg )
    {
    case WM_INITDIALOG:     OnInitDialog( hDlg );      return TRUE;
    case WM_LBUTTONDOWN:    OnLbuttonDown( hDlg );     return TRUE;
    case WM_LBUTTONUP:      OnLbuttonUp( hDlg );       return TRUE;
    case WM_MOUSEMOVE:      OnMouseMove( hDlg );       return TRUE;
    case WM_COMMAND:        OnCommand( hDlg, wParam ); return TRUE;
    case WM_PAINT:          OnPaint( hDlg );           return FALSE;
    case WM_CLOSE:          DestroyWindow( hDlg );     return TRUE;
    case WM_DESTROY:        OnDestroyDialog( hDlg );   return TRUE;
    }
    return FALSE;
}

void KRgnMsgBox::OnDestroyDialog( HWND hDlg )
{
    if( m_BitBackground )	DeleteObject( m_BitBackground );
    if( m_MemDC )			DeleteDC( m_MemDC );
    if( m_hDC )             ReleaseDC( m_hDlgWnd, m_hDC );

    m_hDlgWnd  = NULL;
    m_iMsgBoxType = MBOX_DEFAULT;
}

void KRgnMsgBox::OnInitDialog( HWND hDlg )
{
    // 리젠 로드
    HRSRC hResInfo = FindResource( m_hInstance, MAKEINTRESOURCE(IDR_REGION1), L"REGION" );
    HGLOBAL hGlobal = LoadResource( NULL, hResInfo );
    if( hGlobal )
    {
        BYTE *rgndata = (BYTE FAR*)LockResource( hGlobal );
        if( rgndata )
        {
            XFORM xform;
            xform.eM11 = 1.0f;                   xform.eM22 = 1.0f;
            xform.eM12 = 0.0f;                   xform.eM21 = 0.0f;
            xform.eDx  = 0.0f;                   xform.eDy  = 0.0f;

            HRGN hDialogRGN = ExtCreateRegion( &xform, sizeof(RGNDATAHEADER) + (sizeof(RECT) * ((RGNDATA*)rgndata)->rdh.nCount),(RGNDATA*)rgndata );
            SetWindowRgn( hDlg, hDialogRGN, TRUE );
            UnlockResource( hGlobal );
        }
        FreeResource( hGlobal );
    }

    // 윈도우 텍스트
    SetWindowText( hDlg, g_pkStrLoader->GetString( STR_ID_GRANDCHASE_TITLE ).c_str() );

    // DC생성
    m_hDlgWnd = hDlg;
    m_hDC = GetDC( hDlg );
    m_MemDC = CreateCompatibleDC( m_hDC );

    // Background로드
    m_BitBackground = ::LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_REGION_DEFAULT) );

    // 윈도우 스타일처리
    switch( m_iMsgBoxType )
    {
    case MBOX_DEFAULT:
        {
            ShowWindow( GetDlgItem( hDlg, IDOK), SW_HIDE );
            ShowWindow( GetDlgItem( hDlg, IDCANCEL), SW_HIDE );
        }
        break;
    case MBOX_OK:
        {
            MoveWindow( GetDlgItem( hDlg, IDOK), 132, 19, 80, 25, TRUE );
            SetWindowText( GetDlgItem( hDlg, IDOK), L"확인" );
            ShowWindow( GetDlgItem( hDlg, IDOK), SW_SHOW );
            ShowWindow( GetDlgItem( hDlg, IDCANCEL), SW_HIDE );
        }
        break;
    case MBOX_OKCANCEL:
        {
            MoveWindow( GetDlgItem( hDlg, IDOK), 87, 195, 80, 25, TRUE );
            SetWindowText( GetDlgItem( hDlg, IDOK), L"확인" );
            ShowWindow( GetDlgItem( hDlg, IDOK), SW_SHOW );
            MoveWindow( GetDlgItem( hDlg, IDCANCEL), 177, 195, 80, 25, TRUE );
            SetWindowText( GetDlgItem( hDlg, IDCANCEL), L"취소" );
            ShowWindow( GetDlgItem( hDlg, IDCANCEL), SW_SHOW );
        }
        break;
    case MBOX_YESNO:
        {
            MoveWindow( GetDlgItem( hDlg, IDOK), 87, 195, 80, 25, TRUE );
            SetWindowText( GetDlgItem( hDlg, IDOK), L"예" );
            ShowWindow( GetDlgItem( hDlg, IDOK), SW_SHOW );
            MoveWindow( GetDlgItem( hDlg, IDCANCEL), 177, 195, 80, 25, TRUE );
            SetWindowText( GetDlgItem( hDlg, IDCANCEL), L"아니오" );
            ShowWindow( GetDlgItem( hDlg, IDCANCEL), SW_SHOW );
        }
        break;
    }

    // 윈도우 출력
    ::SetWindowPos( hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
}

void KRgnMsgBox::OnLbuttonDown( HWND hDlg )
{
    m_bTrackMove = true;
    SetCapture( hDlg );
    GetCursorPos( &m_cMouseDWPoint );
    GetWindowRect( hDlg, &m_hWindowRect );
    m_cGapMouse.x = m_cMouseDWPoint.x - m_hWindowRect.left;
    m_cGapMouse.y = m_cMouseDWPoint.y - m_hWindowRect.top;
}

void KRgnMsgBox::OnLbuttonUp( HWND hDlg )
{
    if( m_bTrackMove )
    {
        m_bTrackMove = false;
        ReleaseCapture();
    }
}

void KRgnMsgBox::OnMouseMove( HWND hDlg )
{
    if( m_bTrackMove )
    {
        POINT point;
        GetCursorPos( &point );
        SetWindowPos( hDlg, HWND_TOPMOST, point.x-m_cGapMouse.x, point.y-m_cGapMouse.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }
}

void KRgnMsgBox::OnCommand( HWND hDlg, WPARAM wParam )
{
    switch( LOWORD(wParam) )
    {
    case IDOK:     ::EndDialog( hDlg, IDOK );     break;
    case IDCANCEL: ::EndDialog( hDlg, IDCANCEL ); break;
    }
}

void KRgnMsgBox::OnPaint( HWND hDlg )
{
    HBITMAP oldBit = (HBITMAP)SelectObject(m_MemDC, m_BitBackground);
    BITMAP BitmapInfo;
    GetObject(m_BitBackground, sizeof(BITMAP), &BitmapInfo);

    ::SetBkMode(m_MemDC, TRANSPARENT);
    ::SetTextAlign(m_MemDC, TA_CENTER);
    ::SetTextColor(m_MemDC, 0x00000000);
    int iRevise = 100;
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            ::TextOut(m_MemDC, x + (BitmapInfo.bmWidth / 2) - 2, 
                               y + (BitmapInfo.bmHeight / 2) - 2 + iRevise,
                               m_szLine1, (int)::wcslen(m_szLine1));
            ::TextOut(m_MemDC, x + (BitmapInfo.bmWidth / 2) - 2,
                               y + (BitmapInfo.bmHeight / 2) + 20 - 2 + iRevise,
                               m_szLine2, (int)::wcslen(m_szLine2));
        }
    }
    ::SetTextColor(m_MemDC, 0x00ffffff);
    ::TextOut(m_MemDC, (BitmapInfo.bmWidth / 2), (BitmapInfo.bmHeight / 2) + iRevise,
                        m_szLine1, (int)::wcslen(m_szLine1));
    ::TextOut(m_MemDC, (BitmapInfo.bmWidth / 2), (BitmapInfo.bmHeight / 2) + 20 + iRevise,
                        m_szLine2, (int)::wcslen(m_szLine2));

    TransparentBlt(m_hDC, 0, 0, BitmapInfo.bmWidth, BitmapInfo.bmHeight, m_MemDC, 0, 0, BitmapInfo.bmWidth, BitmapInfo.bmHeight, RGB(255,0,255) );
    SelectObject(m_MemDC, oldBit);
}