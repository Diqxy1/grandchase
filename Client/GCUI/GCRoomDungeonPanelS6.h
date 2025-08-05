#pragma once

class KGCDungeonBossInfoS6;
class KGCDungeonSubInfoS6;
class KGCRoomDungeonMissionInfoS6;


class KGCRoomDungeonPanelS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomDungeonPanelS6 );
    /// Default constructor
    KGCRoomDungeonPanelS6( void );
    /// Default destructor
    virtual ~KGCRoomDungeonPanelS6( void );
    /// Copy constructor
    KGCRoomDungeonPanelS6( const KGCRoomDungeonPanelS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomDungeonPanelS6& operator=( const KGCRoomDungeonPanelS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnChangeDungeon();
    void OnClickAddMission();


public:
    void UpdateDunSelBtn();
    void UpdateGameData();
    void UpdateMissionData();
    void SetNormalMission( DWORD dwMissionID );

public:

    KGCDungeonBossInfoS6*           m_pkDungeonBossInfo;
    KGCDungeonSubInfoS6*            m_pkDungeonSubInfo;
    KGCRoomDungeonMissionInfoS6*    m_pkDungeonMissionInfo;
    KD3DWnd*                        m_pkDungeonSelButton;
    KD3DWnd*                        m_pkAddMissionButton;
    KD3DWnd*                        m_pkMissionAlarm;

private:
    DWORD m_dwNormalMissionID;
};

DEFINE_WND_FACTORY( KGCRoomDungeonPanelS6 );
DECLARE_WND_FACTORY( KGCRoomDungeonPanelS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomDungeonPanelS6 );
