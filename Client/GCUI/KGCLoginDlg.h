#ifndef _KGCLOGINDLG_H_
#define _KGCLOGINDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DEdit;
class KD3DCheckBox;
class KGCSmartPanel;
class KGCVirtualKeyManager;

class KGCLoginDlg : public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCLoginDlg );
	/// Default constructor
	KGCLoginDlg( void );
	/// Default destructor
	virtual ~KGCLoginDlg( void );
	/// Copy constructor
	KGCLoginDlg( const KGCLoginDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLoginDlg& operator=( const KGCLoginDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );	
    void ConnectLoginServer();
	void SkipLogin();
	// 071129. kkurrung. 가상 키보드를 통해서 ID/PW를 입력 받기 위해서.. 
	void SetIDString( const std::wstring& strIDString );
	void SetPWString( const std::wstring& strPWString );
    void OnAgreementOK();
    void SetSelectedID();

    bool IsRenderSmartPanel();
    bool IsCusorInVK() { return m_bCursorInVK; }
    void CapsLockOn( bool capslock_ );

#if defined(NATION_THAILAND)
	inline void SetFunBoxPostfix(int iFunBoxPostfix)
	{
		m_iFunBoxPostfix = iFunBoxPostfix;
	}
#endif

protected:
	virtual void OnCreate( void );
    virtual void OnInitialize();
	void OnLogin();
    void OnAgreement();
	void OnJoin();
	void OnFindID();
	void OnFindPass();
	bool CheckValidate( std::string& strData );
    void KeydownEditNick();
    void FrameMoveInEnabledState();
    void OnVirtualKeyDown( KEVETPARM param_ );

#if defined(NATION_THAILAND)
	int m_iFunBoxPostfix; // 0 : FunBox, 1 : PlayPark, -1 (미선택)

	KD3DWnd* m_pWndMainBtnFunbox;		// 큰 버튼 Funbox용
	KD3DWnd* m_pWndMainBtnPlayPark;	    // 큰 버튼 PlayPark용

	KD3DWnd* m_pWndTabBtnFunbox;		// 큰 버튼 Funbox용
	KD3DWnd* m_pWndTabBtnPlayPark;	    // 큰 버튼 PlayPark용
	KD3DWnd* m_pWndBackGround;			// Background Image;
	KD3DWnd* m_pWndBackGround_pp;			// Background Image for PP;
	KD3DWnd* m_pWndBackGround_fb;			// Background Image for FB;


	void UpdateLoginType();
#endif
	KD3DEdit* m_pEditID;
	KD3DEdit* m_pEditPasswd;
	KD3DWnd* m_pButtonFindPass;		/// 홈페이지 바로가기 버튼
	KD3DWnd* m_pButtonFindID;		/// 웹 샵 바로가기 버튼
	KD3DWnd* m_pButtonLogin;		/// Event 바로가기 버튼
	KD3DWnd* m_pButtonJoin;			/// 나가기 버튼

	KD3DWnd* m_pBtnVKPassWD;        /// 가상 키보드 비번용으로 실행.
	KD3DWnd* m_pBtnVKPassID;        /// 가상 키보드 아이디 용으로..
    KD3DWnd* m_pkSaveID;
    KD3DWnd* m_pkSaveIDMsg;

	KD3DCheckBox* m_pCheckBox;

    KGCSmartPanel* m_pkSmartPanel;   ///중국용 스마트 패널
    KGCVirtualKeyManager*   m_pkVirtualKeyManager;
    bool    m_bCursorInVK;
    KD3DWnd* m_pButtonVK;

    KD3DSizingBox*  m_pkBackCapsLockOn;
    KD3DStatic*     m_pkCapsLockOnMsg;
};

DEFINE_WND_FACTORY( KGCLoginDlg );
DECLARE_WND_FACTORY( KGCLoginDlg );
DECLARE_WND_FACTORY_NAME( KGCLoginDlg );

#endif // _KGCLOGINDLG_H_

