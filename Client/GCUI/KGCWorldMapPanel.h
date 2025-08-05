#ifndef _KGCWORLDMAPPANEL_H_
#define _KGCWORLDMAPPANEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "KGCWorldMapManager.h"
#include "KGCWorldMapTitle.h"

class KGCNameTag;
class KGCWorldMapIcon;
class KGCWorldMapPath;
class KGCWorldMapManager;
class KGCWorldMapTitle;
class KGCWorldMapUpperTitle;

class KGCWorldMapPanel : public KD3DWnd,
                         public KActionListener
{
public:
    friend KGCWorldMapManager;

public:
    DECLARE_CLASSNAME( KGCWorldMapPanel );
    /// Default constructor
    KGCWorldMapPanel( void );
    /// Default destructor
    virtual ~KGCWorldMapPanel( void );
    /// Copy constructor
    KGCWorldMapPanel( const KGCWorldMapPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapPanel& operator=( const KGCWorldMapPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );
	virtual void PreDraw( void );
	virtual	void PostChildDraw( void );

public:
    struct sCharIconTexInfo
    {
        int             iTexNum;
        D3DXVECTOR4     dxVecTexCoord;

        void clear()
        {
            iTexNum = -1;
            dxVecTexCoord = D3DXVECTOR4(0,0,0,0);
        }
    };

private:
    KD3DWnd*        m_pkContinentShopBtn;

    KSafeArray<KGCWorldMapIcon*, KGCWorldMapManager::MAX_WORLDMAP_ICON > m_apkWorldMapIcon;
    KSafeArray<KGCWorldMapPath*, KGCWorldMapManager::MAX_WORLDMAP_PATH > m_apkWorldMapPath;

	KGCWorldMapIcon* m_pkFocusingIcon;
	//KGCNameTag*		m_pkNameTag;
    KGCWorldMapTitle* m_pkGpShop;
    KGCWorldMapTitle* m_pkManufacture;
    KGCWorldMapTitle* m_pkReinforce;
    
#ifdef DIMENSIONAL_BREAK
    KD3DWnd* m_pkIconErnas;
    KD3DWnd* m_pkIconElyos;
#endif
    
    KGCWorldMapUpperTitle* m_pkUpperTitle[KGCWorldMapManager::NUM_UPPERWORLDMAP];
    std::map< EGCCharType, sCharIconTexInfo >  m_mapCharIcon;

public:
	KGCWorldMapIcon*	GetWorldMapIcon(int iIndex_);

	void SetNameTag( const int& iDun_ );
    void SetDungeonName();
    void ShowAvailableContinentIcon();
    void ShowPreContinentCharIcon();
    void ShowContinentQuestIcon();
    void InitCharIconTexInfo();
};

DEFINE_WND_FACTORY( KGCWorldMapPanel );
DECLARE_WND_FACTORY( KGCWorldMapPanel );
DECLARE_WND_FACTORY_NAME( KGCWorldMapPanel );

#endif // _KGCWORLDMAPPANEL_H_
