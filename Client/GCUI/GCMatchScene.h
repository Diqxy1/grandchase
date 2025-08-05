#ifndef _GCMATCHSCENE_H_
#define _GCMATCHSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"

class KGCMatchWindowS6;

//Alto-Falante
class KGCBillBoardChatBoard;

class KGCMatchScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCMatchScene );
    /// Default constructor
    KGCMatchScene( void );
    /// Default destructor
    virtual ~KGCMatchScene( void );
    /// Copy constructor
    KGCMatchScene( const KGCMatchScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMatchScene& operator=( const KGCMatchScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );
    virtual void OnInitialize();

    void UpdateRoomList();

    void OnEnterScene( void );
    void OnLeaveScene( void );

    void UpdateGameData();
	void OnClickUserInfoBtn( );
	void OnClickStatusBtn( );
	void UpdateBlind();


private:
	KGCInfoPanel*		m_pkUserInfo;
	KGCStatusInfoPanel* m_pkStatusPanel;
	KGCCharWnd*			m_pkCharWnd;
	KGCMatchWindowS6*	m_pkMatchWndS6;

	KD3DWnd* m_pkUserInfoBtn;
	KD3DWnd* m_pkStatusBtn;

	KSafeArray<KD3DSizingBox*,2> m_saBlind;

    //Alto-falante
    KGCBillBoardChatBoard* m_pkBillboardChatBoard;
};

DEFINE_WND_FACTORY( KGCMatchScene );
DECLARE_WND_FACTORY( KGCMatchScene );
DECLARE_WND_FACTORY_NAME( KGCMatchScene );

#endif // _GCMATCHSCENE_H_
