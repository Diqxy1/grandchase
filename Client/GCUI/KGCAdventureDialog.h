#pragma once

#include "GCAdventureManager.h"


class KGCAdventureReward;

class KGCAdventureDialog : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAdventureDialog );
    /// Default constructor
    KGCAdventureDialog( void );
    /// Default destructor
    virtual ~KGCAdventureDialog( void );
    /// Copy constructor
    KGCAdventureDialog( const KGCAdventureDialog& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAdventureDialog& operator=( const KGCAdventureDialog& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );

public:
    void OnClickClose();
    void InitContinentBtnAndName( void );
    void SelectContinent( int iIndex );
    void OnSelectContinentBtnList( const KActionEvent& event );
    void OnClickWholeContinent();
    void LockRewardBtn();
private:
    KD3DWnd     *m_pkBtnClose;
    KD3DWnd*    m_pkWholeContinentBtn;
    KD3DWnd*    m_pkWholeContinentName;
    KD3DStatic* m_pkStaticMent;
    std::vector< KDropItemInfo > m_vecContinentReward;

    KGCAdventureReward* m_apkAdventureReward;
    KSafeArray<KD3DWnd*,GCAdventureManager::CONTINENT_MAX>   m_saContinentBtn;
    KSafeArray<KD3DWnd*,GCAdventureManager::CONTINENT_MAX>   m_saContinentActBtn;
    KSafeArray<KD3DWnd*,GCAdventureManager::CONTINENT_MAX>   m_saContinentName;

};

DEFINE_WND_FACTORY( KGCAdventureDialog );
DECLARE_WND_FACTORY( KGCAdventureDialog );
DECLARE_WND_FACTORY_NAME( KGCAdventureDialog );
