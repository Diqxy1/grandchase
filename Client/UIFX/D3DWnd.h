#ifndef _D3DWND_H_
#define _D3DWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)


#include "Vertex.h"

#define GCWND_MSG_MAP(pWnd,MsgID,Func)    if( event.m_pWnd == pWnd && event.m_dwCode == MsgID ) \
                                        { \
                                            Func(); \
                                            return; \
                                        }

#define _GCWND_MSG_MAP(pWnd,MsgID,Func)    if( event.m_pWnd == pWnd && event.m_dwCode == MsgID ) \
                                        { \
                                            Func(); \
                                        }

#define _GCWND_MSG_MAP_PARAM(pWnd,MsgID,Func, Param)    if( event.m_pWnd == pWnd && event.m_dwCode == MsgID ) \
                                        { \
                                        Func( Param); \
                                        }

#define GCWND_MAP( pWnd, Func )         if ( event.m_pWnd == pWnd ) \
                                        { \
                                            Func(); \
                                            return; \
                                        }

#define GCWND_CODE_MAP( pWnd, Func )        if ( event.m_pWnd == pWnd ) \
                                        {\
                                            Func( event.m_dwCode );\
                                            return;\
                                        }

#define GCWND_WND_MAP( pWnd,MsgID,Func )    if( event.m_pWnd == pWnd && event.m_dwCode == MsgID ) \
                                        {\
                                            Func( static_cast<KD3DWnd*>(pWnd) );\
                                            return;\
                                        }

#define GCWND_WND_CODE_MAP( pWnd,Func )    if( event.m_pWnd == pWnd ) \
                                        {\
                                            Func( static_cast<KD3DWnd*>(pWnd), event.m_dwCode );\
                                            return;\
                                        }
class KD3DWnd;
class KD3DUIScene;
class GCUITextureCache;
class KD3DUIManager;

struct KD3DWndEventProcedure;
class KD3DWnd : public KD3DUIObject // extends
{
public:
    DECLARE_CLASSNAME( KD3DWnd );
    /// Default constructor
    KD3DWnd( void );
    /// Default destructor
    virtual ~KD3DWnd( void );
    /// Copy constructor
    KD3DWnd( const KD3DWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DWnd& operator=( const KD3DWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        DEFAULT_FONT_SIZE = 12,
    };

	enum ED3DWndUIAlignType
	{
		WNDUIAT_EXPANTION,  //그냥 키움 기본
		WNDUIAT_POSZEROBASE, // 텍스쳐를 키우진 않고 그냥 기준으로 이동 시킨다.
		WNDUIAT_POSRIGHTBASE,  // 오른쪽을 기준으로 이동시킨다.
		WNDUIAT_POSBOTTOMBASE, // 아래쪽을 기준으로 이동시킨다.
		WNDUIAT_POSRIGHTTOPBASE,  // 오른쪽 위에 붙친다..
	};

    enum ED3DWndToolTipAlignType
    {
        ED3DWND_TOOLTIP_ALIGN_NORMAL   = 0,
        ED3DWND_TOOLTIP_ALIGN_LEFT     = 1,
        ED3DWND_TOOLTIP_ALIGN_RIGHT    = 2,
		ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT = 3,
		ED3DWND_TOOLTIP_POS_LEFT_TEXT_ALIGN_LEFT =4,
    };

    enum ED3DWndState
    {
        D3DWS_DISABLED,
        D3DWS_CREATE,
        D3DWS_ENABLED,
        D3DWS_DESTROY,
    };

    enum ED3DWndMode
    {
        D3DWM_DEFAULT,
        D3DWM_HOVER,
        D3DWM_SELECT,
        D3DWM_ACTIVE,
        D3DWM_LOCK,
        NUM_WINDOW_MODE,
    };

    enum ED3DWndZOrder
    {
        D3DWZ_NOTOPMOST,
        D3DWZ_TOP,
        D3DWZ_TOPMOST,
    };

    enum ED3DWndEventCode
    {
        D3DWE_BUTTON_DOWN           = 100000,
        D3DWE_BUTTON_PRESS,
        D3DWE_BUTTON_UP,
        D3DWE_CURSOR_ENTER,
        D3DWE_CURSOR_LEAVE,
        D3DWE_BUTTON_CLICK,
        D3DWE_EDIT_RETURN,
        D3DWE_EDIT_TAB,
        D3DWE_DIALOG_END,
        D3DWE_EDIT_ESCAPE,
        D3DWE_COMBO_SELCHANGE,
        D3DWE_SCROLLBAR_WHEELUP,
        D3DWE_SCROLLBAR_WHEELDOWN,
        D3DWE_SCROLLBAR_DRAG,
        D3DWE_SCROLLBAR_UPBTN,
        D3DWE_SCROLLBAR_DOWNBTN,
        D3DWE_RBUTTON_CLICK,
        D3DWE_RBUTTON_DOWN,
        D3DWE_RBUTTON_UP,
        D3DWE_RBUTTON_PRESS,
        D3DWE_DOUBLE_CLICK,
        D3DWE_SCENE_CHANGE,
        D3DWE_CREATE,
        D3DWE_CREATE_COMPLETE,
        D3DWE_DESTROY,
        D3DWE_DESTROY_COMPLETE,
        D3DWE_KEY_DOWN,
        D3DWE_MESSAGEBOX_OPEN,
        D3DWE_MESSAGEBOX_CLOSE,
		D3DWE_TOGGLE_CHANGE,
		D3DWE_EDIT_NUMBER,


        D3DWE_ALL_EVENT         = 199999,
        K3DWE_USER              = 200000,        //이것 이상으로는 정의해서 사용하면됨.. #define GCWM_LOGIN_SUCCESS (K3DWE_USER + 1)
    };

    enum ED3DWndMessage
    {
        EWNDMESSAGE_CREATED,
        EWNDMESSAGE_CLOSE,
        EWNDMESSAGE_DESTROYED,
    };

    enum ED3DWndValue {
        EWND_ACTIVE             = 0x00000001,
        EWND_TOPMOST            = 0x00000002,
        EWND_RENDER             = 0x00000004,
        EWND_NOTDRAW            = 0x00000008,
        EWND_AUTOCREATE         = 0x00000010,
        EWND_SELFUPDATE         = 0x00000020,
        EWND_BTNEVENTCATCH      = 0x00000040,
        EWND_PREDRAW            = 0x00000080,
        EWND_HOVERPREV          = 0x00000100,
        EWND_UPDATELOCK         = 0x00000200,
        EWND_TOOLTIPRENDER      = 0x00000400,
        EWND_MODIFYALPHA        = 0x00000800,
        EWND_FONTOUTLINE        = 0x00001000,
        EWND_CREATED            = 0x00002000,
        EWND_CLIPCHILD          = 0x00004000,
        EWND_CHILD_LOCK         = 0x00008000,
        EWND_IGNORE_OFFEN_EVENT = 0x00010000,
        EWND_MOUSE_IN_WINDOW    = 0x00020000,
		EWND_RECALC_DRAW_RECT   = 0x00040000,
		EWND_RECALC_ROTATE_RECT = 0x00080000,

    };

    typedef int         uid_type;
private:
    static int ms_iAccUID;
    static inline int GetFreeUID();
    
    
public:
    inline void SetUIScene( KD3DUIScene* pUIScene )
    {
        m_pUIScene = pUIScene;
    }
    inline void SetMotionSpline( KSpline* pMotionSpline )
    {
        m_pMotionSpline = pMotionSpline;
    }
    inline void SetAlphaSpline( KSpline* pAlphaSpline )
    {
        m_pAlphaSpline = pAlphaSpline;
    }
    inline void AddActionListener( KActionListener* pActionListener )
    {
        m_pActionListener = pActionListener;
    }
    inline void SetAllWindowPos( const D3DXVECTOR2& vWindowPos )
    {
        m_vCreatePos = m_vDestroyPos = vWindowPos;
        SetCurrentWindowLocalPos( vWindowPos );
        SetFixedWindowLocalPos( vWindowPos );
    }
	inline void SetAlignGap( const D3DXVECTOR2& vGapPos )
	{
		m_vGapPos = D3DXVECTOR2(vGapPos.x*g_pGCDeviceManager->GetWindowScaleX() - vGapPos.x,
			                    vGapPos.y*g_pGCDeviceManager->GetWindowScaleY() - vGapPos.y);
		for ( UINT i = 0; i < m_vecChild.size(); ++i )
		{
			m_vecChild[i]->SetAlignGap(vGapPos);
		}
	}
    inline void SetAlignDirectGap( const D3DXVECTOR2& vGapPos )
    {
        m_vGapPos = vGapPos;
        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
            m_vecChild[i]->SetAlignDirectGap(vGapPos);
        }
    }
    inline void SetWindowPos( const D3DXVECTOR2& vWindowPos )
    {
        SetCurrentWindowLocalPos( vWindowPos );
        SetFixedWindowLocalPos( vWindowPos );
    }
	inline void SetWindowPosX( const float posX_ )
	{
        D3DXVECTOR2 vPos;
        vPos = GetCurrentWindowLocalPos();
        vPos.x = posX_;
        SetCurrentWindowLocalPos( vPos );

        vPos = GetFixedWindowLocalPos();
        vPos.x = posX_;
        SetFixedWindowLocalPos( vPos );
	}
	inline void SetWindowPosXDirect( const float posX_ )
	{
		D3DXVECTOR2 vPos;
		vPos = GetFixedWindowLocalPos();
		SetWindowPosDirectFloat(posX_, vPos.y );

	}
	inline void SetWindowPosY( const float posY_ )
    {
        D3DXVECTOR2 vPos;
        vPos = GetCurrentWindowLocalPos();
        vPos.y = posY_;
        SetCurrentWindowLocalPos( vPos );

        vPos = GetFixedWindowLocalPos();
        vPos.y = posY_;
        SetFixedWindowLocalPos( vPos );
	}
	inline void SetWindowPosYDirect( const float posY_ )
	{
		D3DXVECTOR2 vPos;
		vPos = GetFixedWindowLocalPos();
		SetWindowPosDirectFloat(vPos.x, posY_ );
	}

	void SetCurrentWindowLocalPos( const D3DXVECTOR2& vCurrentPos );
    inline void SetFixedWindowLocalPos( const D3DXVECTOR2& vWindowPos )
    {
        m_vWindowPos = vWindowPos;
    }
    inline const D3DXVECTOR2& GetCurrentWindowLocalPos( void ) const
    {
        return m_vCurrentPos;
    }
    inline const D3DXVECTOR2& GetFixedWindowLocalPos( void ) const
    {
        return m_vWindowPos;
    }

	inline const D3DXVECTOR2 GetCurrentWindowPos( void ) const
	{
		if( m_pParent == NULL )
			return m_vCurrentPos;
		else
			return m_vCurrentPos + m_pParent->GetCurrentWindowPos();
	}
	inline const D3DXVECTOR2 GetRelocatedWindowPos( void ) const
	{
		return D3DXVECTOR2(m_RelocatedDrawingrect[0].m_vPos.x,m_RelocatedDrawingrect[0].m_vPos.y);
	}
	inline const D3DXVECTOR2 GetFixedWindowPos( void ) const
	{
		if( m_pParent == NULL )
			return m_vWindowPos;
		else
			return m_vWindowPos + m_pParent->GetFixedWindowPos();
	}
	inline void SetWindowName( const std::string& strWndName )
	{
		m_strWndName = strWndName;
	}
	inline const std::string& GetWindowName( void ) const
	{
		return m_strWndName;
	}

    inline void SetWindowNamePtr( const char* strWndName )
    {
        SetWindowName( strWndName );
    }
    inline const char* GetWindowNamePtr( void ) const
    {
        return m_strWndName.c_str();
    }
    /*inline void SetFriendWindowName( const std::string& strFriend )
    {
        m_strFriend = strFriend;
    }*/
    /*inline const std::string& GetFriendWindowName( void ) const
    {
        return m_strFriend;
    }*/
	inline void ClearToolTip()
	{
		m_strToolTip.clear();
	}
    inline void SetToolTip( const std::wstring& strToolTip, float fOffsetX = 0.0f, float fOffsetY = 0.0f, ED3DWndUIAlignType eWndAlignType = WNDUIAT_EXPANTION, ED3DWndToolTipAlignType eToolTipAlignType = ED3DWND_TOOLTIP_ALIGN_NORMAL)
    {
        m_strToolTip = strToolTip;
        m_vToolTipOffset.x = fOffsetX;
        m_vToolTipOffset.y = fOffsetY;
        m_eToolTipWndAlignType = eWndAlignType;
        m_eToolTipAlignType = eToolTipAlignType; // 툴팁박스의 실재 위치를 어디로 할지 호버된 ui의 중앙, 왼쪽, 오른쪽 기준일지를 정한다.
    }
    inline const std::wstring& GetToolTip( void ) const
    {
        return m_strToolTip;
    }
    inline D3DXVECTOR2 GetToolTipOffset( void ) { return m_vToolTipOffset; }
    inline void SetWidthHeight( DWORD dwWidth, DWORD dwHeight )
    {
        m_dwWidth = dwWidth;
        m_dwHeight = dwHeight;
    }
    inline void SetWidth( DWORD dwWidth )
    {
        m_dwWidth   = dwWidth;
        OnSetWidth( dwWidth );
        SetRecalcRect( true );
    }
    inline void SetHeight( DWORD dwHeight )
    {
        m_dwHeight  = dwHeight;
        OnSetHeight( dwHeight );
        SetRecalcRect( true );
    }
    inline virtual void OnSetWidth( DWORD dwWidth ) {};
    inline virtual void OnSetHeight( DWORD dwHeight ) {};

    inline virtual DWORD GetWidth( void ) const
    {
        return m_dwWidth;
    }
    inline virtual DWORD GetHeight( void ) const
    {
        return m_dwHeight;
    }
    inline void SetFontSize( LONG lFontSize )
    {
        m_lFontSize  = lFontSize;
        m_lFontWidth = m_lFontSize / 2;
    }
    inline LONG GetFontSize( void ) const
    {
        return m_lFontSize;
    }
    inline void SetFontColor( D3DCOLOR dwColor )
    {
        m_dwFontColor = dwColor;
    }
    inline D3DCOLOR GetFontColor( void ) const
    {
        return m_dwFontColor;
    }
    inline void SetWindowColor( D3DCOLOR dwWndColor )
    {
        m_dwWndColor = dwWndColor;
    }
    inline D3DCOLOR GetWindowColor( void ) const
    {
        return m_dwWndColor;
    }
    inline KD3DWnd* GetParent( void ) const
    {
        return m_pParent;
    }
    inline KD3DWnd* GetChild( UINT index ) const
    {
        return ( ( index < 0 || index >= m_vecChild.size() ) ? NULL : m_vecChild[index] );
    }
    inline KD3DWnd* GetChildByName(std::string strName)
    {            
        boost::unordered_map<std::string, KD3DWnd*>::iterator itor = m_mapChild.find(strName);
        if( itor == m_mapChild.end() )
            return NULL;
        return itor->second;
    }
    inline UINT GetNumChild( void ) const
    {
        return (UINT)m_vecChild.size();
    }

	virtual inline void ResetUIforResolution()
	{
		//Scale 값 재 할당.
		if (m_eAlignType == WNDUIAT_EXPANTION)
		{
			m_fWindowScaleX=g_pGCDeviceManager->GetWindowScaleX();
			m_fWindowScaleY=g_pGCDeviceManager->GetWindowScaleY();
		}
		else
		{
			m_fWindowScaleX=1.0f;
			m_fWindowScaleY=1.0f;
		}

		for ( UINT i = 0; i < m_vecChild.size(); ++i )
		{
			m_vecChild[i]->ResetUIforResolution();
        }
	}

	inline void SetWindowScale( D3DXVECTOR2 dxv2 )
	{
		m_fWindowScaleX = dxv2.x;
		m_fWindowScaleY = dxv2.y;

		//하부객체도 스케일 동일하게 셋팅함.
		for ( UINT i = 0; i < m_vecChild.size(); ++i )
		{
			m_vecChild[i]->SetWindowScale( dxv2 );   
		}
	}

	inline void SetAlignType( ED3DWndUIAlignType eAlignType )
	{
		m_eAlignType = eAlignType;
		if (m_eAlignType == WNDUIAT_EXPANTION)
		{
			m_fWindowScaleX=g_pGCDeviceManager->GetWindowScaleX();
			m_fWindowScaleY=g_pGCDeviceManager->GetWindowScaleY();
		}
		else
		{
			m_fWindowScaleX=1.0f;
			m_fWindowScaleY=1.0f;
		}		
		for ( UINT i = 0; i < m_vecChild.size(); ++i )
		{
			//기본적으로 확장한다. But 개개별로 설정된 녀석들은 따로 변경시키지 않는다.
			/*if (m_vecChild[i]->GetAlignType()==WNDUIAT_EXPANTION)
			{*/
				m_vecChild[i]->SetAlignType( m_eAlignType );            
			//}
		}
	}

    inline void SetWndState( ED3DWndState eState, bool bSetChildren_ = true )
    {
        PushTable();
        __SetWndState( eState );

        if( bSetChildren_ )
            __SetChildWndState( eState );

        PopTable();
    }

    inline bool IsWndState( ED3DWndState eState )   { return (m_eState == eState); }

    inline ED3DWndState GetWndState( void ) const
    {
        return m_eState;
    }
    inline int GetWndTexture( ED3DWndMode eWndMode = D3DWM_DEFAULT ) const
    {
        return m_aiWndTex[eWndMode];
    }
    inline const D3DXVECTOR4& GetWndTexCoords( ED3DWndMode eWndMode = D3DWM_DEFAULT ) const
    {
        return m_avTexCoords[eWndMode];
    }
    inline bool IsRenderOn( void ) const
    {
        return (m_dwBooleanState & EWND_RENDER) != 0;
    }

    inline bool IsNotDraw( void ) const
    {
        return (m_dwBooleanState & EWND_NOTDRAW) != 0;
    }

    inline void SetNotDraw(bool bNotDraw)
    {

        if( bNotDraw )
            m_dwBooleanState |= EWND_NOTDRAW;
        else
            m_dwBooleanState &= ~EWND_NOTDRAW;
    }

    virtual inline void ToggleRender( bool bRender )
    {
//         if( bRender != IsRenderOn() ) {
//             SendEvent( KActionEvent( this, D3DWE_TOGGLE_CHANGE, bRender?1:0 ) );
//         }

        if( bRender )
            m_dwBooleanState |= EWND_RENDER;
        else
            m_dwBooleanState &= ~EWND_RENDER;

        if(IsRenderOn())
		{
            return;
		}

        if(m_pkMouseFocusedWnd == this)
        {
            m_pkMouseFocusedWnd = NULL;
            return;
        }

        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
			if( m_vecChild[i] == NULL )
			{
				ASSERT( !"Null Wnd child" );
				continue;
			}

            m_vecChild[i]->ParentRendeOff();
        }
    }
    
    virtual void ParentRendeOff()
    {
        if(m_pkMouseFocusedWnd == this)
        {
            m_pkMouseFocusedWnd = NULL;
            return;
        }
        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
			if( m_vecChild[i] == NULL )
			{
				ASSERT( !"Null Wnd child" );
				continue;
			}

            m_vecChild[i]->ParentRendeOff();           
        }
    }

    inline bool IsActive( void ) const
    {
        return (m_dwBooleanState & EWND_ACTIVE) != 0;
    }
    inline bool IsTopMost( void ) const
    {
        return (m_dwBooleanState & EWND_TOPMOST) != 0;
    }
    inline void SetAutoCreate( bool bAutoCreate )
    {
        if( bAutoCreate )
            m_dwBooleanState |= EWND_AUTOCREATE;
        else
            m_dwBooleanState &= ~EWND_AUTOCREATE;
    }
    inline bool IsAutoCreate( void ) const
    {
        return (m_dwBooleanState & EWND_AUTOCREATE) != 0;
    }
    inline void Lock( bool bLock )
    {
		m_eMode = (bLock?D3DWM_LOCK:D3DWM_DEFAULT);
        SetWndMode( m_eMode );
    }
    inline void LockAll( bool bLock )
    {
        Lock( bLock );
        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
            m_vecChild[i]->Lock( bLock );
        }
    }
    inline bool IsLocked( void ) const
    {
        return m_eMode == D3DWM_LOCK;
    }
    inline void SetSelfInputCheck( bool bSelfUpdate )
    {
        if( bSelfUpdate )
            m_dwBooleanState |= EWND_SELFUPDATE;
        else
            m_dwBooleanState &= ~EWND_SELFUPDATE;
    }
    inline bool IsSelfInputCheck( void ) const
    {
        return (m_dwBooleanState & EWND_SELFUPDATE) != 0;
    }
    inline void SetButtonEventCatch( bool bBtnEventCatch )
    {
        if( bBtnEventCatch )
            m_dwBooleanState |= EWND_BTNEVENTCATCH;
        else
            m_dwBooleanState &= ~EWND_BTNEVENTCATCH;
    }
    inline bool IsButtonEventCatch( void ) const
    {
        return (m_dwBooleanState & EWND_BTNEVENTCATCH) != 0;
    }
    inline void SetPredraw( bool bPredraw )
    {
        if( bPredraw )
            m_dwBooleanState |= EWND_PREDRAW;
        else
            m_dwBooleanState &= ~EWND_PREDRAW;
    }
    inline bool IsPredraw( void ) const
    {
        return (m_dwBooleanState & EWND_PREDRAW) != 0;
    }
    inline void LinkControl( std::string strWndName, void** ppvLink )
    {
        m_mapControlLink[strWndName] = ppvLink;
    }
    inline void SetHotKey( int iHotKey );

    inline void SetHotJoystickKey( int iHotJoystickKey )
    {
        if ( iHotJoystickKey < 0 || iHotJoystickKey >= 17 )
            return;
        
        m_setHotJostickKey.insert(iHotJoystickKey);
    }

    inline void SetHotKeyStyle( const std::string& strHotKey )
    {
        m_strHotKey = strHotKey;
    }
    inline int GetHotKey( int iIndex_ = 0 )
    {
        if ( iIndex_ >= (int)m_vecHotKey.size() )
            return 0;
        return    m_vecHotKey[iIndex_];
    }

    inline bool IsCreate() const 
    { 
        return (m_dwBooleanState & EWND_CREATED) != 0;
    }
    inline bool IsClipChild() const {
        return (m_dwBooleanState & EWND_CLIPCHILD) != 0;
    }
    inline bool IsChildLock() const {
        return (m_dwBooleanState & EWND_CHILD_LOCK ) != 0;
    }
    inline bool IsIgnoreOffenEvent() const {
        return (m_dwBooleanState & EWND_IGNORE_OFFEN_EVENT ) != 0;
    }
    inline void SetIgnoreOffenEvent( bool bClip_ ) {
        if( bClip_ ) {
            m_dwBooleanState |= EWND_IGNORE_OFFEN_EVENT;
        }
        else {
            m_dwBooleanState &= ~EWND_IGNORE_OFFEN_EVENT;
        }
    }
    inline bool IsMouseInWindow() const {
        return (m_dwBooleanState & EWND_MOUSE_IN_WINDOW ) != 0;
    }
    inline void SetMouseInWindow( bool bClip_ ) {
        if( bClip_ ) {
            m_dwBooleanState |= EWND_MOUSE_IN_WINDOW;
        }
        else {
            m_dwBooleanState &= ~EWND_MOUSE_IN_WINDOW;
        }
    }
    inline bool IsRecalcRect() const {
        return (m_dwBooleanState & EWND_RECALC_DRAW_RECT ) != 0;
    }
	inline bool IsRecalcRotateRect() const {
		return (m_dwBooleanState & EWND_RECALC_ROTATE_RECT ) != 0;
	}
    inline void SetRecalcRect( bool bClip_ ) {
        if( bClip_ ) {
            m_dwBooleanState |= EWND_RECALC_DRAW_RECT;
        }
        else {
            m_dwBooleanState &= ~EWND_RECALC_DRAW_RECT;
        }
    }
	inline void SetRecalcRotateRect( bool bClip_ ) {
		if( bClip_ ) {
			m_dwBooleanState |= EWND_RECALC_ROTATE_RECT;
		}
		else {
			m_dwBooleanState &= ~EWND_RECALC_ROTATE_RECT;
		}
	}
    inline void SetClipChild( bool bClip_ ) {
        if( bClip_ ) {
            m_dwBooleanState |= EWND_CLIPCHILD;
        }
        else {
            m_dwBooleanState &= ~EWND_CLIPCHILD;
        }
    }
    inline void SetChildLock( bool bClip_ ) {
        if( bClip_ ) {
            m_dwBooleanState |= EWND_CHILD_LOCK;
        }
        else {
            m_dwBooleanState &= ~EWND_CHILD_LOCK;
        }
    }

	inline void SetWndUpdateLock( bool bLock )
	{

        if( bLock )
            m_dwBooleanState |= EWND_UPDATELOCK;
        else
            m_dwBooleanState &= ~EWND_UPDATELOCK;

		if( !m_vecChild.empty() )
		{
			for ( int i = (int)m_vecChild.size() - 1 ; i >= 0 ; i-- )
			{
				m_vecChild[i]->SetWndUpdateLock( bLock );
			}
		}
	}

	inline bool IsWndUpdateLock()
    {
        return (m_dwBooleanState & EWND_UPDATELOCK) != 0;
    }

    inline bool IsDrawFontOutline()
    {
        return (m_dwBooleanState & EWND_FONTOUTLINE) != 0;
    }

    const KSafeArray<KD3DUIVertex,4>& GetWindowRect() { return m_rect; }
    const KSafeArray<KD3DUIVertex,4>& GetReLocatedWindowRect() { return m_RelocatedDrawingrect; }

	#define LINK_CONTROL( name, link )  LinkControl( (name), reinterpret_cast<void**>( &(link) ) )
	#define LINK_CONTROL_STM( name, link )  {\
												std::stringstream stm;\
												stm<<name;\
												LinkControl( (stm.str() ), reinterpret_cast<void**>( &(link) ) );\
											}\

    void SetWindowZOrder( ED3DWndZOrder eZOrder );

    void SetChildOrder( IN KD3DWnd* pScrWnd_, IN KD3DWnd* pTargetWnd_, bool bNext );
    void SetChildOrderTop( IN KD3DWnd* pScrWnd_ );
    void SetChildOrderBottom( IN KD3DWnd* pScrWnd_ );

    void SetOrder_Next( IN KD3DWnd* pWnd_ );
    void SetOrder_Before( IN KD3DWnd* pWnd_ );
    void SetOrder_Top();
    void SetOrder_Bottom();

    void SetOrder_NextbyName( IN const char* strName_ );
    void SetOrder_BeforebyName( IN const char* strName_ );

    void Activate( bool bActive );
    void AddChild( KD3DWnd* pChild );
    void AddChildWithRegistToLua( KD3DWnd* pChild );
    void RemoveAllChild( bool bWithoutLua = false );
    void DeleteChild( int iIndex );
    bool DeleteChildByName( const char* strName );
    bool DeleteChildByPtr( KD3DWnd* pWnd );
    bool IsAllEnabled( void ) const;
    bool IsAllDisabled( void ) const;

    void InitState( bool bRender, bool bSelfUpdate = false, KActionListener* pActionListener = NULL );

    void SpeakToActionListener( const KActionEvent& event, bool bSendLua_ = true );
    void SetWndTexCoords( const D3DXVECTOR4& vTexCoords = D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    void SetWndTexCoords( ED3DWndMode eMode, const D3DXVECTOR4& vTexCoords );
    void SetWndTex( ED3DWndMode eWnd_ , int iTex_ );

    void Register( const KD3DWndClass& rkWndClass );
    HRESULT LoadWnd( KLuaManager* pkLuaMgr, KD3DWnd* pkParent, std::string strWndName );
    void Create( bool bAuto = false );
    void Destroy( void );

    HRESULT FrameMove( void );
	virtual void ReCalcReLocatedRect();
    void ReCalcReLocatedDrawRect();

    virtual HRESULT Render( void );

    inline ED3DWndMode GetWndMode( void ) const { return m_eMode; }
	inline ED3DWndUIAlignType GetAlignType( void ) const { return m_eAlignType; }
    void SetWndMode( ED3DWndMode eMode, bool bExceptChild_ = true, bool bSendEvent_ = true );
    void ResetWndMode( void );
    virtual ED3DWndMode UpdateWndMode( void );

    void SetCapture(KD3DWnd* pkWnd);
    void ReleaseCapture();
    void SetWindowPosDirect( const D3DXVECTOR2& vWindowPos );
    void SetWindowPosDirectFloat( const float fx_, const float fy_ );
    virtual void SetHeightDirect( DWORD dwHeight );
    virtual void SetWidthDirect( DWORD dwWidth );

    virtual void SetHeightDirectInverse( DWORD dwHeight, DWORD dwOriHeight_ );
    virtual void SetWidthDirectInverse( DWORD dwWidth, DWORD dwOriWidth_  );

    float GetMoveGapTime(){ return m_fMoveGapTime; }

	void Update( void );
    inline uid_type GetUID() {
        return m_UID;
    }

    inline void SetHoverPrev( bool bHoverPrev ) {
        if( bHoverPrev )
            m_dwBooleanState |= EWND_HOVERPREV;
        else
            m_dwBooleanState &= ~EWND_HOVERPREV;
    }

    inline bool IsHoverPrev() const {
        return (m_dwBooleanState & EWND_HOVERPREV) != 0;
    }

    // utility functions
    bool CheckPosInWindowBound( const D3DXVECTOR2& vPos ) const;
    bool CheckPosInWindowBoundWithChild( const D3DXVECTOR2& vPos, bool bChildVisible = false ) const;
    void UpdateWindowBountWithMosuePos();
    bool CheckMousePosInWindowBound() const { return IsMouseInWindow(); }
    void TransformWindow( const D3DXVECTOR2& vA, const D3DXVECTOR2& vB, float t, bool bCreate );
    void TransformWindowInverse( const D3DXVECTOR2& vA, const D3DXVECTOR2& vB, float t, bool bCreate, DWORD dwOriWidth_ = -1, DWORD dwOriHeight_ = -1 );

    void CopyWindowPosInfo( const KD3DWnd* pSrc_ );

	D3DXVECTOR2 GetScaledVector(D3DXVECTOR2 vPos) const;


    int GetMemorySize();
    int GetChildSize();
    void GetLogData( FILE* pLogFile );

	//자식 UI 중 제일 위로 올리는 함수
	void MoveWndOnTop(KD3DWnd* ChildWnd_ );

public:
    virtual void FrameMoveInDisabledState( void )   {}
    virtual void FrameMoveInCreateState( void )     {}
    virtual void FrameMoveInEnabledState( void )    {}
    virtual void FrameMoveInDestroyState( void )    {}

    virtual void PreDraw( void )        {}
    virtual void PostDraw( void )       {}
    virtual void PostChildDraw( void )  {}
    void Initialize( void );
    virtual void OnInitialize( void )   {};
    virtual void OnPostChildInitialize( void ) {}

    virtual void OnLoad( void )             {}
protected:
    virtual void OnActivate( void )         {}
    virtual void OnCreate( void )           {}
    virtual void OnDestroy( void )          {}
    virtual void OnActivateComplete( void ) {}
    virtual void OnCreateComplete( void )   {}
    virtual void OnDestroyComplete( void )  {}
    virtual void OnUpdate( void )			{}

    void RefreshRelocatedPos();
    void RefreshRelocatedPosWithRotate( bool bRefreshChild_ );
    void RefreshRelocatedPosWithoutRotate( bool bRefreshChild_ );
    void __SetWndState( ED3DWndState eState, bool bPushTable = false );
    void __SetChildWndState(  ED3DWndState eState, bool bPushTable = false );
protected:
    uid_type            m_UID;

    std::string         m_strWndName;
    //std::string         m_strFriend;
    std::wstring        m_strToolTip;
    D3DXVECTOR2         m_vToolTipOffset;
    ED3DWndUIAlignType  m_eToolTipWndAlignType;
    ED3DWndToolTipAlignType  m_eToolTipAlignType;
    std::string         m_strHotKey;

    KD3DUIScene*            m_pUIScene;
    KD3DWnd*                m_pParent;
    typedef std::vector<KD3DWnd*>  ChildContainer;
    std::vector<KD3DWnd*>   m_vecChild;
    boost::unordered_map<std::string, KD3DWnd*>        m_mapChild;

    KSpline*            m_pMotionSpline;
    KSpline*            m_pAlphaSpline;
    KActionListener*    m_pActionListener;

    ED3DWndState        m_eState;
    ED3DWndMode         m_eModePrev;
    ED3DWndMode         m_eMode;
	ED3DWndUIAlignType  m_eAlignType;
	float				m_fWindowScaleX;
	float				m_fWindowScaleY;
	D3DXVECTOR2			m_vGapPos;

	KSafeArray<int,NUM_WINDOW_MODE> m_aiWndTex;
    KSafeArray<D3DXVECTOR4,NUM_WINDOW_MODE> m_avTexCoords;

    D3DXVECTOR2         m_vCreatePos;
    D3DXVECTOR2         m_vCurrentPos;
    D3DXVECTOR2         m_vScaledCurrentPos;
    D3DXVECTOR2         m_vWindowPos;
    D3DXVECTOR2         m_vDestroyPos;

    D3DXVECTOR2         m_vTrans;

    float               m_fTime;
    float               m_fMoveBegTime;
    float               m_fMoveGapTime;

    KSafeArray<KD3DUIVertex,4> m_rect;
	KSafeArray<KD3DUIVertex,4> m_RelocatedDrawingrect;

    LONG                m_lFontSize;
    LONG                m_lFontWidth;
    D3DCOLOR            m_dwFontColor;
    D3DCOLOR            m_dwWndColor;

    // 2007.11.16 wony855
    // 폰트에 아웃라인 설정!
    D3DCOLOR            m_dwFontOutlineColor;

    std::map<std::string, void**>   m_mapControlLink;

    DWORD               m_dwBooleanState;

	bool				m_bToolTipRender;
    
    std::vector< int >  m_vecHotKey;
    std::set< int >  m_setHotJostickKey;

    static KD3DWnd*     m_pkMouseFocusedWnd;
    static KD3DWnd*     m_pkMouseHoverWnd;

	GCUITextureCache*	m_pCache;
    std::string         m_strFileName;
public:
    void                ForceSetWindowColor( D3DCOLOR dwWndColor );
    void                SetAllWindowColor( D3DCOLOR dwWndColor );
    void                SetAllWindowAlpha( int iAlpha_ );
    void                SetFontOutline( bool bFontOutline = false, D3DCOLOR OutlineColor = 0xFF000000 );
    void				RenderToolTip( bool bRender_ ) { 
        if( bRender_ )
            m_dwBooleanState |= EWND_TOOLTIPRENDER;
        else
            m_dwBooleanState &= ~EWND_TOOLTIPRENDER;
    }
    bool                IsRenderToolTip() const {
        return (m_dwBooleanState & EWND_TOOLTIPRENDER) != 0;
    }

	DWORD				m_dwOptionalData;

private:
    void                CheckToolTip();
    std::string         GetUniqueWndName( KD3DWnd* pkParents, const std::string& strWndName );
protected:
    void CallFunction( const char* strFunctionName, bool bPushTable = false );
public:
    void __BindToLua()
	{ 
		BindToLua(); 
	};

    virtual void BindToLua();
    bool RegistToLua();
    bool PushTable( bool bPushParent = true );
    void PopTable( bool bPopParents_ = true );
    virtual void PushThis(lua_State* L);
    void AddChildFromLua( KD3DWnd* pChild );

    inline std::string GetPath() {
        std::stringstream stm;
        if( m_pParent != NULL ) {
            stm<<m_pParent->GetPath() << "\\";
        }
        stm << GetWindowName();

        return stm.str();
    };

    inline void SetFileName( const std::string& strFileName_ ) { m_strFileName = strFileName_; };
    inline std::string GetFileName() const {
        if( m_pParent != NULL ) {
            return m_pParent->GetFileName();
        }
        return m_strFileName;
    }

    template< typename UI, typename R, typename P >
    int ConnectEventProcedure( DWORD dwEventCode_, UI* pWnd, R (UI::*pFunc_)(P) )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,P>>    spCommand( new KD3DWndEventProcChild<UI,R,P>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        g_pkUIMgr->ConnectEventProcedure( GetUID(), dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    template< typename UI, typename R >
    int ConnectEventProcedure( DWORD dwEventCode_, UI* pWnd, R (UI::*pFunc_)() )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,void>>    spCommand( new KD3DWndEventProcChild<UI,R,void>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        g_pkUIMgr->ConnectEventProcedure( GetUID(), dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    template< typename UI, typename R, typename P >
    int ConnectEventProcedure( DWORD dwEventCode_, UI* pWnd, boost::function<R (UI*, P)> pFunc_ )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,P>>    spCommand( new KD3DWndEventProcChild<UI,R,P>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        g_pkUIMgr->ConnectEventProcedure( GetUID(), dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    template< typename UI, typename R >
    int ConnectEventProcedure( DWORD dwEventCode_, UI* pWnd, boost::function<R (UI*)> pFunc_ )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,void>>    spCommand( new KD3DWndEventProcChild<UI,R,void>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        g_pkUIMgr->ConnectEventProcedure( GetUID(), dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    void DisconnectAllEventProcedure();
    void DisconnectEventProcedure( DWORD dwCode_ );
    void DisconnectGlobalEventProcedure( DWORD dwCode_ );

	D3DXVECTOR2 AdjustIntoScreen();
    float GetWindowScaleX() { return m_fWindowScaleX; }
    float GetWindowScaleY() { return m_fWindowScaleY; }
    void SendEvent( const KActionEvent& kEvent_, bool bSendLua_ = true );

private:
    DWORD               m_dwWidth;
    DWORD               m_dwHeight;
    DWORD m_dwBeforeClickTime;
    int                 m_iOldJoystickKey;

    bool  m_bModifyAlpha;
    void SetModifyAlpha( bool bEnable ) {
        if( bEnable )
            m_dwBooleanState |= EWND_MODIFYALPHA;
        else
            m_dwBooleanState &= ~EWND_MODIFYALPHA;
    }
    bool IsModifyAlpha() const {
        return (m_dwBooleanState & EWND_MODIFYALPHA) != 0;
    }
	
};

class KD3DMoveWnd : public KD3DWnd // extends
{
public:
	DECLARE_CLASSNAME( KD3DMoveWnd );

	virtual void ReCalcReLocatedRect();
};

class KD3DWndActiveChild : public KD3DWnd // extends
{
public:
	DECLARE_CLASSNAME( KD3DWndActiveChild );
	virtual KD3DWnd::ED3DWndMode UpdateWndMode( void );
};

struct KD3DWndEventProcedure {
private:
    static int ms_GlobalUID;
    int m_UID;
public:
    KD3DWndEventProcedure() {
        m_UID = ++ms_GlobalUID;
    }
    KD3DWnd::uid_type m_ID;
    int GetUID() const { return m_UID; }
    virtual bool CallProcedure( const KActionEvent& kEvent_ ) = 0;
};

template< typename UI, typename R, typename P >
struct KD3DWndEventProcChild : public KD3DWndEventProcedure {
    boost::function<R (UI*,P)> m_pFunc;
    bool CallProcedure( const KActionEvent& kEvent_ ) {
        if( g_pkUIMgr == NULL )
            return false;

        UI* pWnd = static_cast<UI*>(g_pkUIMgr->GetWndFromID( m_ID ));
        if( pWnd == NULL )
            return false;

        return m_pFunc(pWnd,kEvent_);
    }
};

template< typename UI, typename P >
struct KD3DWndEventProcChild<UI,void,P> : public KD3DWndEventProcedure {
    boost::function<void (UI*,P)> m_pFunc;
    bool CallProcedure( const KActionEvent& kEvent_ ) {
        if( g_pkUIMgr == NULL )
            return false;

        UI* pWnd = static_cast<UI*>(g_pkUIMgr->GetWndFromID( m_ID ));
        if( pWnd == NULL )
            return false;

        m_pFunc(pWnd,kEvent_);
        return true;
    }
};

template< typename UI, typename R >
struct KD3DWndEventProcChild<UI,R,void> : public KD3DWndEventProcedure {
    boost::function<R (UI*)> m_pFunc;
    bool CallProcedure( const KActionEvent& kEvent_ ) {
        if( g_pkUIMgr == NULL )
            return false;

        UI* pWnd = static_cast<UI*>(g_pkUIMgr->GetWndFromID( m_ID ));
        if( pWnd == NULL )
            return false;

        return m_pFunc(pWnd);
    }
};

template< typename UI >
struct KD3DWndEventProcChild<UI,void,void> : public KD3DWndEventProcedure {
    boost::function<void (UI*)> m_pFunc;
    bool CallProcedure( const KActionEvent& kEvent_ ) {
        if( g_pkUIMgr == NULL )
            return false;

        UI* pWnd = static_cast<UI*>(g_pkUIMgr->GetWndFromID( m_ID ));
        if( pWnd == NULL )
            return false;

        m_pFunc(pWnd);
        return true;
    }
};

class KD3DWndFactory
{
protected:
    std::string m_strClassName;
public:
    KD3DWndFactory():m_strClassName("KD3DWnd") {}
    virtual KD3DWnd* Create( void ) const
    {
        static bool bFirstCreate = true;
        KD3DWnd* pWnd = new KD3DWnd();
        if( bFirstCreate ) {
            bFirstCreate = false;
            pWnd->__BindToLua();
        }
        return pWnd; 
    }
	virtual bool CanCreate()
	{
		return true;
	}
	virtual bool IsStatic()
	{
		return false;
	}
    const char* GetClassName() const { return m_strClassName.c_str(); }
};

#define DEFINE_WND_FACTORY(w) \
class w##Factory : public KD3DWndFactory \
{ \
public: \
    w##Factory() { m_strClassName = #w; }\
    virtual KD3DWnd* Create( void ) const \
    { \
        static bool bFirstCreate = true;\
        KD3DWnd* pWnd = new w##();\
        if( bFirstCreate ) {\
            bFirstCreate = false;\
            pWnd->__BindToLua();\
        }\
        return pWnd; \
    } \
}

#define DECLARE_WND_FACTORY(w)          extern w##Factory g_##w##Factory
#define IMPLEMENT_WND_FACTORY(w)        w##Factory g_##w##Factory
#define DECLARE_WND_FACTORY_NAME(w)     extern char g_sz##w##FactoryName[]
#define IMPLEMENT_WND_FACTORY_NAME(w,s) char g_sz##w##FactoryName[] = (s)
#define GET_WND_FACTORY_NAME(w)         (g_sz##w##FactoryName)
#define GET_WND_FACTORY_PTR(w)          (&g_##w##Factory)


#define DEFINE_WND_FACTORY_STATIC(w) \
class w##Factory : public KD3DWndFactory \
{ \
public: \
	static KD3DWnd * m_pD3DWnd; \
	virtual KD3DWnd* Create( void ) const \
	{\
		if(!m_pD3DWnd)	\
			m_pD3DWnd = new w##();	\
		return m_pD3DWnd;	\
	}\
	virtual bool CanCreate()\
	{\
		return (!m_pD3DWnd);\
	}\
	virtual bool IsStatic()\
	{\
		return true;\
	}\
}

// For Static Factory
#define IMPLEMENT_WND_FACTORY_STATIC(w)\
	w##Factory g_##w##Factory;	\
	KD3DWnd* w##Factory::m_pD3DWnd = NULL

#define DOUBLE_CLICK_GAP 450

extern float g_fUITime;

DEFINE_WND_FACTORY( KD3DMoveWnd );
DEFINE_WND_FACTORY( KD3DWndActiveChild );

#endif // _D3DWND_H_
