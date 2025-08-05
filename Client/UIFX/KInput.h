// KInput.h: interface for the KInput class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KInput_H__CAD549C1_F843_4931_B51C_BE3028BB9357__INCLUDED_)
//#define AFX_KInput_H__CAD549C1_F843_4931_B51C_BE3028BB9357__INCLUDED_

#ifndef _KINPUT_H_
#define _KINPUT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DIRECTINPUT_VERSION 0x0800
#define IS_PRESS( x ) ( ((x) & 0x80) > 0 )

#include <dinput.h>
////#include <vector>

struct SInputData
{
public:
	BYTE                   m_key[256];  ///< 키보드의 이전 입력을 기억하기 위한 공간. 
	DIMOUSESTATE           m_ms;        ///< 마우스의 현재 입력을 알아내기 위한 공간. 
	POINT                  m_ptMouse;     ///< 마우스 클라이언트 영역 좌표.
	SInputData()
	{
		Clear();
	}

	void Clear()
	{
		ClearKey();
		ClearMouse();
		m_ptMouse.x = m_ptMouse.y = 0;
	}

	void ClearKey()
	{
		ZeroMemory( &m_key, sizeof( m_key ) );
	}

	void ClearMouse()
	{
		ZeroMemory( &m_ms, sizeof( m_ms ) );
	}

	SInputData& operator=( const SInputData& arg )
	{
		memcpy( m_key, arg.m_key, 256 );
		memcpy( &m_ms, &arg.m_ms, sizeof( DIMOUSESTATE ) );
		m_ptMouse = arg.m_ptMouse;
		return (*this);
	}
};

class KInput
{
public:
    enum 
    {
        MBLEFT = 0,
        MBRIGHT = 1,
        MBMIDDLE = 2,
    };

    KInput();
    virtual ~KInput();

//////////////////////////////////////////////////////////////////////////
// member data    
private:

	SInputData		m_sCurInput;
	SInputData		m_sOldInput;
	SInputData		m_sUserInput;
	SInputData		m_sUserOldInput;
	SInputData		m_sVirtualDefault;

    LPDIRECTINPUT           m_pDI;         ///< DirectInput 구조체의 포인터. (IDIRECTINPUT)
    LPDIRECTINPUTDEVICE     m_pKeyboard;   ///< DirectInput 으로 키보드를 제어하기 위한 포인터. 
    LPDIRECTINPUTDEVICE     m_pMouse;      ///< DirectINput 으로 마우스를 제어하기 위한 포인터. 

//{{ Appended by parkch: 2004-06-10 오전 9:40:37
    POINT                   m_ptMouse;     ///< 마우스 클라이언트 영역 좌표.
	bool					m_bIdle;
//}} Appended by parkch: 2004-06-10 오전 9:40:37

	DWORD			m_dwStopTime;

	HWND    m_hWnd; ///< 입력을 받는 윈도우 핸들.
//////////////////////////////////////////////////////////////////////////
// member function
public :
    // 키를 누르거나, 떼거나, 계속 누르고 있는 상태일 때를 알아낸다. 
    inline bool IsPressed( int nKeyID ) { return  IS_PRESS(m_sCurInput.m_key[ nKeyID ]); }
    inline bool IsDown( int nKeyID )    { return ( !IS_PRESS(m_sOldInput.m_key[nKeyID] ) && IS_PRESS(m_sCurInput.m_key[nKeyID] ) ); }
    inline bool IsUp( int nKeyID )      { return ( IS_PRESS(m_sOldInput.m_key[nKeyID]) && !IS_PRESS(m_sCurInput.m_key[nKeyID]) ); }

    // 마우스버튼에 관한 상태를 알아낸다. 인자를 통해서 버튼의 종류 선택. 디폴트로 왼쪽 버튼.
    inline bool BtnPressed( int nWhich = 0 ) { return IS_PRESS(m_sCurInput.m_ms.rgbButtons[nWhich]) ; }
    inline bool BtnDown( int nWhich = 0 )    { return ( !IS_PRESS(m_sOldInput.m_ms.rgbButtons[nWhich]) && IS_PRESS(m_sCurInput.m_ms.rgbButtons[nWhich]) ); }
	inline bool BtnUp( int nWhich = 0 )      { return ( IS_PRESS(m_sOldInput.m_ms.rgbButtons[nWhich]) && !IS_PRESS(m_sCurInput.m_ms.rgbButtons[nWhich]) ); }
	
	// 키를 누르거나, 떼거나, 계속 누르고 있는 상태일 때를 알아낸다. 
	inline bool IsUserKeyPressed( int nKeyID ) { return IS_PRESS( m_sUserInput.m_key[ nKeyID ] ); }
	inline bool IsUserKeyDown( int nKeyID )    { return ( !IS_PRESS(m_sUserOldInput.m_key[nKeyID]) && IS_PRESS(m_sUserInput.m_key[nKeyID]) ); }
	inline bool IsUserKeyUp( int nKeyID )      { return ( IS_PRESS(m_sUserOldInput.m_key[nKeyID]) && !IS_PRESS(m_sUserInput.m_key[nKeyID]) ); }

	// 마우스버튼에 관한 상태를 알아낸다. 인자를 통해서 버튼의 종류 선택. 디폴트로 왼쪽 버튼.
	inline bool UserBtnPressed( int nWhich = 0 ) { return IS_PRESS(m_sUserInput.m_ms.rgbButtons[nWhich]); }
	inline bool UserBtnDown( int nWhich = 0 )    { return ( !IS_PRESS(m_sUserOldInput.m_ms.rgbButtons[nWhich]) && IS_PRESS(m_sUserInput.m_ms.rgbButtons[nWhich]) ); }
	inline bool UserBtnUp( int nWhich = 0 )      { return ( IS_PRESS(m_sUserOldInput.m_ms.rgbButtons[nWhich]) && !IS_PRESS(m_sUserInput.m_ms.rgbButtons[nWhich]) ); }

    // 마우스의 움직임이 있었는지를 알아낸다. 
    inline bool IsMouseMoving() { return ( (m_sCurInput.m_ms.lX != 0) || (m_sCurInput.m_ms.lY != 0) || (m_sCurInput.m_ms.lZ != 0) ); }
	inline bool ISWheelUp() { return (m_sCurInput.m_ms.lZ > 0 ); }
	inline bool ISWheelDown() { return (m_sCurInput.m_ms.lZ < 0 ); }
    
    // 마우스의 위치 변화량을 리턴한다. 
    inline LONG GetXMov()  { return  m_sCurInput.m_ms.lX; }
    inline LONG GetYMov()  { return  m_sCurInput.m_ms.lY; }
    inline LONG GetZMov()  { return  m_sCurInput.m_ms.lZ; }

//{{ Appended by parkch: 2004-06-10 오전 9:42:01
    // 마우스의 클라이언트 영역 좌표를 얻어온다.
    inline const POINT& GetMousePos( void ) const { return m_sCurInput.m_ptMouse; }
    inline const POINT& GetUserMousePos( void ) const { return m_sUserOldInput.m_ptMouse; }
//}} Appended by parkch: 2004-06-10 오전 9:42:01

    // 키의 입력을 무효화한다. 게임 짜다보니까 이런거 필요하겠더라구... (040325)
    inline void IgnoreDown( int nKeyID , int iDelay) { 
		m_sCurInput.m_key[ nKeyID ] = 1; 
		m_sOldInput.m_key[ nKeyID ] = 1; 
		if (iDelay!=0)
			m_mapIgnoreKey.insert(make_pair(nKeyID,iDelay));
	}
    // 마우스 클릭을 무효화 (040504)
    inline void IgnoreBtnUp( int nWhich = 0 ) 
    {
        m_sOldInput.m_ms.rgbButtons[nWhich] = 0;
        m_sCurInput.m_ms.rgbButtons[nWhich] = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    // DirectInput의 사용을 위한 기본적인 처리들. 
    HRESULT UpdateInput();
    HRESULT FreeDirectInput();
    HRESULT InitDevice( HWND hWnd , BOOL bCursor);

    //////////////////////////////////////////////////////////////////////////
    // 마우스 커서 제어를 위해 호출.
    HRESULT ShowCursor( BOOL bShow );

    std::vector<BYTE> GetKeyList( bool (KInput::*func)(int) );

	//////////////////////////////////////////////////////////////////////////
	// 2008-02-21 milennium9
	// 가상 입력을 위한 추가 함수
private:
	std::map<int,int> m_mapIgnoreKey;
	std::vector< SInputData > m_vecSenario;
	int				m_iSenarioCount;

	BYTE			m_keyDefault[256];
	DIMOUSESTATE	m_msDefault;
	POINT			m_ptDefault;
	bool			m_bVirtualMode;
public:
	bool IsVirtualMode() const { return m_bVirtualMode; }
	void SetVirtualMode(bool val) { m_bVirtualMode = val; }

	HRESULT	UpdateVirtualInput();
    void VirtualClick( POINT pt );
    void VirtualMouseMove( POINT _ptBefore, POINT _ptAfter, int _ispeed );
	void SetDefaultMousePos( POINT ptPos );
    void VirtualDrag( POINT startpt , POINT endpt, int delay = 55);
    void VirtualKey( std::vector<std::pair<int,int>>& vecKey);
};

extern KInput* g_pkInput;

#endif // _KINPUT_H_


/*
 *	

  1. InitDevice() 한번 불러준다.
  2. UpdateInput() 매프레임 호출한다. (즉, input이 갱신되어야 할때 호출한다)
  3. 기능에 맞는 함수를 호출해 사용한다. 직관적이므로 설명하지 않는다.

  4. 커서를 보이거나 숨길때 ShowCursor를 이용한다.
  5. 키보드 눌림 혹은 마우스 클릭 이벤트를 무시해야 할때, Ignore... 함수를 사용한다.
 */
