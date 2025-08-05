#pragma once
#include "uifx\d3dwnd.h"

#define MAX_ARTICLE     6

class KGCMissionProgressBox :
	public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCMissionProgressBox );
	/// Default constructor
	KGCMissionProgressBox( void );
	/// Default destructor
	virtual ~KGCMissionProgressBox( void );
	/// Copy constructor
	KGCMissionProgressBox( const KGCMissionProgressBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMissionProgressBox& operator=( const KGCMissionProgressBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    //미션진행상황
    enum {
        EM_NON,
        EM_COMPLETE, 
        EM_INCOMPLETE, 
        EM_FAIL, 
    };

	virtual void OnCreate( void );
	virtual void PostChildDraw( void );

	bool IsEmpty();
    DWORD GetBackWidth(){ return m_pkBack->GetWidth(); }

public:
    int MakeSubjectList();
    int MakeMissionList();

protected:
	KD3DSizingBox*	m_pkBack;
    KD3DSizingBox*	m_pkTitleMissionBack;
    KD3DSizingBox*	m_pkTitleSubjectBack;
    KD3DWnd*		m_pkTitleSubMission;
    KD3DWnd*		m_pkTitleSubject;
	KD3DStatic*		m_pkStaticSubMission;
	KD3DStatic*		m_pkStaticCount;
    KD3DStatic*		m_pkStaticSubject;
    KD3DStatic*		m_pkStaticEmpty;
};


DEFINE_WND_FACTORY( KGCMissionProgressBox );
DECLARE_WND_FACTORY( KGCMissionProgressBox );
DECLARE_WND_FACTORY_NAME( KGCMissionProgressBox );