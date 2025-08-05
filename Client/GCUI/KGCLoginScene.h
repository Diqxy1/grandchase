
#ifndef _KGCLOGINSCENE_H_
#define _KGCLOGINSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCLoginDlg;
class KGCLoginErrorDlg;
class KGCLoginEventDlg;


class KGCLoginScene : public KGCScene
{
public:
	DECLARE_CLASSNAME( KGCLoginScene );
	/// Default constructor
	KGCLoginScene( void );
	/// Default destructor
	virtual ~KGCLoginScene( void );
	/// Copy constructor
	KGCLoginScene( const KGCLoginScene& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLoginScene& operator=( const KGCLoginScene& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );	
	void OnError( KD3DWnd* pWnd, const WCHAR* wcsMsg, bool bExitProgram_ = false );
	void ConnectLoginServer();

	// 071129. kkurrung. 가상 키보드를 통해서 받은 문자를 적용하기 위해서 추가 했음.. 
	void SetIDString( const std::wstring& strIDString );
	void SetPWString( const std::wstring& strPWString );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnEnterScene();
    virtual void InitScene() {};
	void OnEndError();
	void OnEndLogin();
	void OnEndEventLoginDlg();
	void OnGoEvent();
	void OnGoWebShop();
	void OnGoHomePage();
	void OnExit();

    bool     m_bExitProgram;
	KD3DWnd* m_pWndLastDlg;			/// 오류상자가 나타나기 이전의 다이얼로그를 저장해둡니다..-> 다시보여줘야지

	KD3DWnd* m_pWndBalck;			/// 오류메세지에서 사용할 검은 바탕
	KD3DWnd* m_pWndGoEvent;		/// 홈페이지 바로가기 버튼
	KD3DWnd* m_pWndGoWebShop;		/// 웹 샵 바로가기 버튼
	KD3DWnd* m_pWndGoHomePage;		/// Event 바로가기 버튼
	KD3DWnd* m_pWndExit;			/// 나가기 버튼

    KD3DWnd* m_pClickCheck;         //중국 화면 클릭 판단용

	//아래 두개는 클래스 바꿔서 연결해야함.
	KGCLoginDlg* m_pLoginDlg;
//#if defined(NATION_THAILAND)	//태국은 2중 로긴창
//	KGCLoginDlg* m_pLoginDlg2;
//#endif

	KGCLoginErrorDlg* m_pErrorDlg;
	KGCLoginEventDlg* m_pEventLoginDlg;
};

extern KGCLoginScene* g_pLoginScene;

DEFINE_WND_FACTORY( KGCLoginScene );
DECLARE_WND_FACTORY( KGCLoginScene );
DECLARE_WND_FACTORY_NAME( KGCLoginScene );

#endif // _KGCLOGINSCENE_H_
