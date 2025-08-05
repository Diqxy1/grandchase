#pragma once

class KD3DWnd;
class KGCCharacterUI;
class KGCDungeonMissionList : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCDungeonMissionList );
    /// Default constructor
    KGCDungeonMissionList( void );
    /// Default destructor
    virtual ~KGCDungeonMissionList( void );
    /// Copy constructor
    KGCDungeonMissionList( const KGCDungeonMissionList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonMissionList& operator=( const KGCDungeonMissionList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        DML_LIST_SIZE = 5,
    };
private:
    void RefreshBackgroundSize();
public:
    virtual void OnCreate( void );

    bool AddMissionInfo( DWORD dwMissionID );
    void ClearMissionInfo();

private:
    KD3DStatic* m_pkTitle;
    KD3DSizingBox* m_pkBackGround;
    KSafeArray<KD3DStatic*,DML_LIST_SIZE> m_apkMissionName;
    KSafeArray<KGCCharacterUI*,DML_LIST_SIZE> m_apkCharacter;
    KSafeArray<KD3DSizingBox*,DML_LIST_SIZE> m_apkMissionBG;

    int m_iListNum;
};

DEFINE_WND_FACTORY( KGCDungeonMissionList );
DECLARE_WND_FACTORY( KGCDungeonMissionList );
DECLARE_WND_FACTORY_NAME( KGCDungeonMissionList );
