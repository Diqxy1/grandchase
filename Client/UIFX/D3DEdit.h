#ifndef _D3DEDIT_H_
#define _D3DEDIT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"
//#include "KGCIme.h"
#include "GCDeviceFontTH.h"
//#include "FontManager.h"
////#include <vector>

#define MAX_PATH_CHINA		(130)

class KD3DEdit : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KD3DEdit );
    /// Default constructor
    KD3DEdit( void );
    /// Default destructor
    virtual ~KD3DEdit( void );
    /// Copy constructor
    KD3DEdit( const KD3DEdit& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DEdit& operator=( const KD3DEdit& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void ParentRendeOff()
	{
		KD3DWnd::ParentRendeOff();
		if(m_bHasFocus)
		{
            extern HWND g_hUIMainWnd;
            ::SetFocus( g_hUIMainWnd );
		}
	}   
	void SetPasswd( bool bPasswd = true, WCHAR wcPassChar = L'●' );
    const WCHAR* GetText( bool bCheckLimit_ = false );
    inline void SetFocus( void )                { ::SetFocus( m_hWndEdit ); }
    void SetFocus( bool bIsFocus);
    inline bool HasFocus( void ) const          { return m_bHasFocus; }
    inline HWND GetHandle( void ) const         { return m_hWndEdit; }
    inline void SetText( const WCHAR* szText, bool bSelectAll = true )
    {
        // set selection
        if ( NULL == szText )
        {
            ::SetWindowText( m_hWndEdit, L"" );

            ::ZeroMemory( m_strEdit, sizeof( m_strEdit ) );
            SetSelection( 0, 0 );
            m_iOffset_X = 0; //범위 초과 했을때 이거 안들어가서 원래 자리로 복귀 안됨
            m_iOffset_Y = 0;
        }
        else
        {
            ::SetWindowText( m_hWndEdit, szText );
			#ifdef NATION_CHINA
			if ( ::wcslen( szText ) < MAX_PATH_CHINA )
			{
				::wcscpy( m_strEdit, szText );
			}
			#else 
			if ( ::wcslen( szText ) < MAX_PATH )
			{
				::wcscpy( m_strEdit, szText );
			}
			#endif
            
            int len = (int)::wcslen( szText );
            SetSelection( ( bSelectAll ? 0 : len ), len );
        }
    }
    inline void SetSelection( int iStart, int iEnd )
    {
        ::SendMessage( m_hWndEdit, EM_SETSEL, (WPARAM)iStart, (LPARAM)iEnd );
        ::SendMessage( m_hWndEdit, EM_GETSEL, (WPARAM)&m_iStart, (LPARAM)&m_iEnd );
    }
    inline int GetTextSize() { return m_iTexCount; }
	void SetLimitText( int iLimit, int iLimitPixelSize = -1, int iFontSize = 12 );
    void SetLimitTextByByte( int iLimitByte_ = 0 ) { m_iLimitByte = iLimitByte_; }
    bool CheckLimiteTextByByte();

    inline void EnableSelfFocusCheck( bool bEnable )
    {
        m_bSelfFocusCheck = bEnable;
    }

	inline void SetMultiLine( bool bMultyLine ) { m_bMultyLineMode = bMultyLine; }
	inline bool IsMultiLine( void ) { return m_bMultyLineMode; }

        inline void SetNewLine( bool bNewLine ) { m_bAllowNewLine = bNewLine; }
    inline bool IsSetNewLine( void ) { return m_bAllowNewLine; }

protected:
    enum
    {
        TEMP_STR_BUF_SIZE = 1024,
    };

protected:
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void OnActivate( void );

protected:
    inline int GetTextDisplayLimit( void ) const
    {
        return int( float( GetWidth() ) / float( m_lFontWidth ) );
    }
    void CreateEditCtrl( void );
    void DestroyEditCtrl( void );
    bool IsAlphaScanCode( BYTE scanCode );
    void DrawCursor( const D3DXVECTOR2& vCursorPos, int iOffset, bool bSelectDraw = false );
    LRESULT D3DEditProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool CheckLimitSize( void );
    static LRESULT CALLBACK EditCtrlProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void DeleteColorCode();

	std::vector<POINT> GetLineSelectionList(int iStart, int iEnd);
    int GetLineByIndex( const int iIndex, const int iStartIndex = 0 );
    int GetLeftSideWidthByIndex( const int iIndex, const int iStartIndex = 0 );

protected:
    WNDPROC         m_fpOrgEditProc;    // original edit control procedure function pointer
    HWND            m_hWndEdit;

    bool            m_bHasFocus;
	bool			m_bIsPasswd;
	WCHAR			m_wcPassChar;

    WCHAR           m_strPassWord[MAX_PATH];
    WCHAR           m_strEdit[MAX_PATH];    ///< string buffer

    int             m_iStart;               ///< selection start index
    int             m_iEnd;                 ///< selection end index

    KD3DUIVertex    m_cursor[4];
    int             m_iOffset_X;

    bool            m_bSelfFocusCheck;
#if defined( _THAI_LANG )	//태국은 Tahoma체를 사용합니다.
	GCDeviceFontTH*   m_pkFontMgrLimtPixelSize;			///< 길이 잴때 사용할 폰트임
#else
	GCDeviceFont*   m_pkFontMgrLimtPixelSize;			///< 길이 잴때 사용할 폰트임
#endif

	int				m_iLitmitPixelSize;

private:
	bool			m_bMultyLineMode;
	bool			m_bShift;
    bool            m_bAllowNewLine;
    int             m_iLimitSize;
	int				m_iLimitLine;
    int				m_iLimitByte;
    int             m_iOffset_Y;
    bool            m_bUseStart;
    int             m_iLimitLine_Y;
    int             m_iTexCount;
	int				m_iType;

    enum SELECT_INDEX
    {
        USE_START = 0,
        USE_END,
        NOT_DETERMINE,
    };



    SELECT_INDEX    m_eSelectIndex;
    int& GetCurCursorIndex();

public:
	KGCIme			m_Ime;

protected:
    virtual GCDeviceFont* GetFontMgr();

public:
	enum EDIT_TYPE
	{
		EDIT_ALL_TYPE = 0,
		EDIT_NUMBER,
	};

    virtual void BindToLua();
    virtual void PushThis(lua_State* L);

	void SetEditType(int iType){m_iType = iType;}
	bool IsEditType(WPARAM wParam);
    void PushNewLineAtIndex( int iIndex );
    void SwapCursor( void );
	void SetLimitLine( int iLimitLine_ ) { m_iLimitLine = iLimitLine_; }

    int GetCursorIndex();

private:// 입력된 채팅내용 자동완성 시스템
    enum 
    {   MAX_AUTO_CREATE = 10, };
    int                       m_iSelectAutoCreatte;
    std::list< std::wstring > m_listAutoCreate;

    void AddAutoCreateMsg();
    void FindAutoCreateMsg( bool bIsUp );
};

DEFINE_WND_FACTORY( KD3DEdit );

#endif // _D3DEDIT_H_
