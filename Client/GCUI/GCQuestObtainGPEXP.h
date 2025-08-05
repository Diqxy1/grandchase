#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "TypeDef.h"
//#include "GCQuestRankScore.h"

class KGCQuestRankScore;

class KGCQuestObtainGPEXP : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCQuestObtainGPEXP );

    KGCQuestObtainGPEXP(void);
    virtual ~KGCQuestObtainGPEXP(void);
    
    /// Copy constructor
    KGCQuestObtainGPEXP( const KGCQuestObtainGPEXP& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestObtainGPEXP& operator=( const KGCQuestObtainGPEXP& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    KGCQuestRankScore   *m_pkEXPScore;
    KGCQuestRankScore   *m_pkGPScore;

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& ) {}

public:
    void SetGP(int iGp, bool bDirect = false);
    void SetEXP( GCEXPTYPE biExp, bool bDirect = false );

	int GetGp();
	GCEXPTYPE GetExp();

    bool isComplete(void);
    void SetExpGpColor( DWORD dwColor_ );
};

DEFINE_WND_FACTORY( KGCQuestObtainGPEXP );
DECLARE_WND_FACTORY( KGCQuestObtainGPEXP );
DECLARE_WND_FACTORY_NAME( KGCQuestObtainGPEXP );
