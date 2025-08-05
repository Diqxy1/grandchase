#ifndef _KGCROOMSCENES6_H_
#define _KGCROOMSCENES6_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"

class KGCRoomTitleS6;
class KGCRoomUserPanelS6;
class KGCRoomDungeonPanelS6;
class KGCRoomButtonPanelS6;
class KGCRoomMatchPanelS6;
class KGCHeroTicketSystem;

class KGCRoomSceneS6 : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCRoomSceneS6 );
    /// Default constructor
    KGCRoomSceneS6( void );
    /// Default destructor
    virtual ~KGCRoomSceneS6( void );
    /// Copy constructor
    KGCRoomSceneS6( const KGCRoomSceneS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomSceneS6& operator=( const KGCRoomSceneS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw();
    

public:
    virtual void InitScene() {}
    void HeroTicketPanelManager( bool iActive );
    void OnInitialize( void );
    void OnEnterScene( void );
    void OnLeaveScene( void );

public:
    void UpdateGameData( bool bUpdateExtraModel = false ); //UI에 현재 상태를 적용합니다
    void UpdateDungeonLevel();
    void UpdateMissionData();

public:
    void OnEnterMyInfo();
    void OnSkillTree();
    void OnExitRoom();

public:
    void SetRoomTitle( int nRoomNum, const std::wstring& strRoomName, const std::wstring& strRoomPassword );

    bool GetEnableStart( void ) { return m_bEnableStart; }
    void SetEnableStart( bool bEnableStart ) { m_bEnableStart = bEnableStart; }
    void ShowWorldMap( bool bShow );

    void ChangeCurrentLevel( int iLevel, bool bRight = false );

    void SetRoomMemberPingInfo(std::vector<std::pair<DWORD,DWORD>>& vecRoomMemberPingInfo_ );
   
    void SetLevelUpPlayer( const int i_ );
    void SetMissionOpenTime() { m_dwMissionOpenTime = ::timeGetTime(); }
    void InitLevelupEffect();

    bool IsMaxPlayerNumSix();

public:
    bool    m_bEnableStart;
    DWORD   m_dwMissionOpenTime;

public:
    //KD3DWnd*        m_pkRoomExitButton;

    KD3DWnd*        m_pkMyInfoButton;
    KD3DWnd*        m_pkSkilltreeButton;

    KGCRoomTitleS6*         m_pkRoomTitle;
    KGCRoomUserPanelS6*     m_pkRoomUserPanel;
    KGCRoomDungeonPanelS6*  m_pkRoomDungeonPanel;
    KGCRoomMatchPanelS6*    m_pkRoomMatchPanel;
    KGCRoomButtonPanelS6*   m_pkRoomButtonPanel;
    KGCHeroTicketSystem*    m_pkTicketPanel;

    bool m_bShowMissionUI;
};

DEFINE_WND_FACTORY( KGCRoomSceneS6 );
DECLARE_WND_FACTORY( KGCRoomSceneS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomSceneS6 );


#endif //_KGCROOMSCENE_S6_H_