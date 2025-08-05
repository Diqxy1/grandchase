#ifndef _GCSQAURELOADINGSCENE_H_
#define _GCSQAURELOADINGSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
#include "GCScene.h"

class KGCSquareLoadingScene : public KGCScene
{
public:
	DECLARE_CLASSNAME( KGCSquareLoadingScene );
	/// Default constructor
	KGCSquareLoadingScene( void );
	/// Default destructor
	virtual ~KGCSquareLoadingScene( void );
	/// Copy constructor
	KGCSquareLoadingScene( const KGCSquareLoadingScene& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSquareLoadingScene& operator=( const KGCSquareLoadingScene& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    // [1/24/2008] breadceo. 0 ~ 1 사이
    void SetGage( float fPercent );
    bool IsEventEnd() const { return m_bEventEnd; }
    void Init();
	void RenderLoadingType();

protected:
    virtual void InitScene() {}
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState();
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnEnterScene();
    void EventFrameMove();

    KD3DWnd*    m_pkBarGage;
	KD3DWnd*    m_pkOpenDoor;
	KD3DWnd*    m_pkCuteElesis;
	KD3DWnd*    m_pkOpenDoor2;

	KD3DWnd*	m_pkLoading1;
	KD3DWnd*	m_pkLoading2;
	KD3DWnd*	m_pkLoadingGuild1;
	KD3DWnd*	m_pkLoadingGuild2;

	KD3DWnd*	m_pkBar;
	KD3DWnd*	m_pkBar2;
	KD3DWnd*	m_pkdoor;
	KD3DWnd*	m_pkdoor2;

    D3DXVECTOR2 m_vCompletePos;
    D3DXVECTOR2 m_vElesisPos;
    D3DXVECTOR2 m_vBarPos;
    D3DXVECTOR2 m_vElesisGoalPos;
    bool        m_bEventEnd;
    bool        m_bFirst;
    DWORD       m_dwDoorOpenTime;
};

DEFINE_WND_FACTORY( KGCSquareLoadingScene );
DECLARE_WND_FACTORY( KGCSquareLoadingScene );
DECLARE_WND_FACTORY_NAME( KGCSquareLoadingScene );

#endif // _GCSQAURELOADINGSCENE_H_