#ifndef _GCWORLDMAPSCENE_H_
#define _GCWORLDMAPSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"

class KGCMainFrame;

//Alto-Falante
class KGCBillBoardChatBoard;

class KGCWorldMapScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCWorldMapScene );
    /// Default constructor
    KGCWorldMapScene( void );
    /// Default destructor
    virtual ~KGCWorldMapScene( void );
    /// Copy constructor
    KGCWorldMapScene( const KGCWorldMapScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapScene& operator=( const KGCWorldMapScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );
    virtual void OnInitialize();
    virtual void OnEnterScene();

    //Alto-falante
    KGCBillBoardChatBoard* m_pkBillboardChatBoard;

private:
    //KGCWorldMapContainer*   m_pkWorldMapContainer;
};

DEFINE_WND_FACTORY( KGCWorldMapScene );
DECLARE_WND_FACTORY( KGCWorldMapScene );
DECLARE_WND_FACTORY_NAME( KGCWorldMapScene );

#endif // _GCWORLDMAPSCENE_H_