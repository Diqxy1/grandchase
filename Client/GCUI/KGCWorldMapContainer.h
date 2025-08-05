#ifndef _KGCWORLDMAPCONTAINER_H_
#define _KGCWORLDMAPCONTAINER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "KGCWorldMapManager.h"
#include "KTDGParticleSystem.h"

class KD3DStatic;
class KGCChatScrollBackground;
class KGCWorldMapPanel;
class KGCWorldMapManager;
class KGCWorldMapScene;
class KGCDungeonMissionList;
class KGCSongkranWaterEffect;
class KGCVitalityWorldmap;

class KGCWorldMapContainer : public KD3DWnd,
                             public KActionListener
{
public:
    friend KGCWorldMapManager;
    friend KGCWorldMapScene;

public:
    DECLARE_CLASSNAME( KGCWorldMapContainer );
    /// Default constructor
    KGCWorldMapContainer( void );
    /// Default destructor
    virtual ~KGCWorldMapContainer( void );
    /// Copy constructor
    KGCWorldMapContainer( const KGCWorldMapContainer& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapContainer& operator=( const KGCWorldMapContainer& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void FrameMoveInEnabledState( void );
    virtual void OnPostChildInitialize();

    enum Mapas {
        Ernas,
        Elyos
    };

    void GoTo(Mapas ID);
    void UpdateWorldMap(bool bReset=false);
    void CreateMapParticle();
    void CreateMapEffect();
    void ClearMapEffect();
	void ADD_WORLDMAP_PARTICLE(std::string seq,float x=0.0f,float y=0.0f);
    void RenderMapEffect( bool bRender );
    void SetEffectRenderDelay( int iDelay = 3 ){ m_iEffectRenderDelay = iDelay; }
	void UpperMapBtnRenderProc();	// 아직 다른 대륙이 밝혀지지 못한 상태라면... 가리자..

//    void ToggleChatList( bool bShow );
//    bool GetShowList() const { return m_bShowList; }
//    void SetShowList(bool val) { m_bShowList = val; }
//    void ShowChatBox( bool bShow );

//    void ChatColorBoxInit( void );
	const KSafeArray<KGCWorldMapPanel*, NUM_WORLDMAP >& GetWolrdMapPanel(){return m_apkWorldMapPanel;}
	KGCWorldMapPanel* GetWolrdMapPanel(int iIndex_);

    void ShowDungeonMissionList( EGCGameMode eGameMode, D3DXVECTOR2 vPos );
    void HideDungeonMissionList();

    void RefreshOpenedContinent();

    KGCSongkranWaterEffect* GetSonkranWaterEffect() { return m_pkWaterEffect; }

    void EventNewContinent( bool bRender );

    void SetVitalityInfo();
	void RenderEventDungeonIcon( bool bRender );


    enum
    { HERO_SHOP_ENABLE_LV = 30, };// 영웅상점 사용 가능 레벨 

private:
    KGCDungeonMissionList*  m_pkDungeonMissionList;
    KSafeArray<KGCWorldMapPanel*, NUM_WORLDMAP > m_apkWorldMapPanel;

    KD3DWnd*            m_pkUpperMapButton;
    KD3DWnd*            m_pkNewMapButton;
    //KD3DWnd*            m_pkHellModeButton;
    //KD3DWnd*            m_pkHellModeArrow;
	//KD3DWnd*            m_pkChangeViewBtn;

	void ReLocateChatBox();

    D3DXVECTOR2         m_vBeforePos;
	DWORD		        m_vBeforeChatBoxPos;
	DWORD		        m_vBeforeBtnPos;
	DWORD               m_dwBeforeScrollHeight;
	DWORD               m_dwBeforeListHeight;
	DWORD               m_dwBeforScrollBackground;
	DWORD               m_dwBeforChatBoxBackground;

    bool                m_bDragWindowCheck;
	
    int                 m_iDirection;

    // 맵에 띄워주는 파티클..
    std::vector< CParticleEventSeqPTR > m_vecMapEffect;
    int m_iEffectRenderDelay;

    KD3DWnd*            m_pkBackGroundDeco;
	KGCSongkranWaterEffect*				m_pkWaterEffect;
    KD3DWnd* m_pkNewContinentNotice;
    KD3DSizingBox* m_pkBackgoundBlack;

    KGCVitalityWorldmap*    m_pkVitalityExpInfo;
	KD3DWnd* m_pkHeroDungeon;
    KD3DWnd* m_pkHeroName;;

    KD3DWnd* m_pkEventDungeon;
    KD3DWnd* m_pkEventName;
    
    KD3DWnd* m_pkHeroShop;
    KD3DWnd* m_pkHeroShopName;

    KD3DWnd* m_pkCollection;
    KD3DWnd* m_pkCollectionName;
};

DEFINE_WND_FACTORY( KGCWorldMapContainer );
DECLARE_WND_FACTORY( KGCWorldMapContainer );
DECLARE_WND_FACTORY_NAME( KGCWorldMapContainer );

#endif // _KGCWORLDMAPCONTAINER_H_
