#ifndef _GCTUTORIALMAP_H_
#define _GCTUTORIALMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../gcui/GCChangeBox.h"


class KGCModeTutorial;

class KGCTutorialMap : public KD3DWnd, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCTutorialMap );
    /// Default constructor
    KGCTutorialMap( void );
    /// Default destructor
    virtual ~KGCTutorialMap( void );
    /// Copy constructor
    KGCTutorialMap( const KGCTutorialMap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTutorialMap& operator=( const KGCTutorialMap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );

public:
    KGCModeTutorial* GetTutorialMode();

	EGCGameStage GetSeqMapStage( int nIndex );
    int GetSeqMapStringID( int nIndex );
    int GetTutorialMapSize();
    
    EGCGameStage GetMapStage();
    int GetMapStringID();

private:
    void SelectMap(int iIndex);

private:
    int m_iCurMapIndex;


    std::map<EGCGameStage, KD3DWnd*> m_mapWndMap;
    std::map<EGCGameStage, int> m_mapMapString;

    KGCChangeBox*   m_pkMapChangeBox;
};

DEFINE_WND_FACTORY( KGCTutorialMap );
DECLARE_WND_FACTORY( KGCTutorialMap );
DECLARE_WND_FACTORY_NAME( KGCTutorialMap );

#endif  // _GCTUTORIALMAP_H_