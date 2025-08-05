//
//#ifndef _KGCSERVERLISTSCENE_H_
//#define _KGCSERVERLISTSCENE_H_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//////#include "../uifx/D3DStatic.h"
////#include "../uifx/D3DWnd.h"
////#include "../uifx/ActionListener.h"
//
//class KGCServerListDlg;
//
//class KGCServerListScene : public KD3DWnd, public KActionListener   // implements
//{
//public:
//	DECLARE_CLASSNAME( KGCServerListScene ); 
//	/// Default constructor
//	KGCServerListScene( void );
//	/// Default destructor
//	virtual ~KGCServerListScene( void );
//	/// Copy constructor
//	KGCServerListScene( const KGCServerListScene& )
//	{ ASSERT( false && "Do not use copy constructor!" ); }
//	/// Assignment operator
//	const KGCServerListScene& operator=( const KGCServerListScene& )
//	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
//
//	void ActionPerformed( const KActionEvent& event );	
//	void Initialize();
//protected:
//	virtual void OnCreate( void );	
//	void OnMinimize();
//	void OnMaximize();
//	void OnExit();
//
//	KD3DWnd* m_pWndMinimize;		/// 웹 샵 바로가기 버튼
//	KD3DWnd* m_pWndMaximize;		/// Event 바로가기 버튼
//	KD3DWnd* m_pWndExit;			/// 나가기 버튼
//
//	//아래 두개는 클래스 바꿔서 연결해야함.
//	KGCServerListDlg* m_pServerListDlg;
//};
//
//extern KGCServerListScene* g_pServerListScene;
//
//DEFINE_WND_FACTORY( KGCServerListScene );
//DECLARE_WND_FACTORY( KGCServerListScene );
//DECLARE_WND_FACTORY_NAME( KGCServerListScene );
//
//#endif // _KGCSERVERLISTSCENE_H_
