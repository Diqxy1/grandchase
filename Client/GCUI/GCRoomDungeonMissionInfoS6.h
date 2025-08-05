#pragma once

struct sMissionState 
{
    std::wstring m_strMissionTitle;
    std::wstring m_strMissionCount;
    int m_iMissionType;
};

class KGCRoomDungeonMissionInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomDungeonMissionInfoS6 );
    /// Default constructor
    KGCRoomDungeonMissionInfoS6( void );
    /// Default destructor
    virtual ~KGCRoomDungeonMissionInfoS6( void );
    /// Copy constructor
    KGCRoomDungeonMissionInfoS6( const KGCRoomDungeonMissionInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomDungeonMissionInfoS6& operator=( const KGCRoomDungeonMissionInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_MISSION_PAGE_NUM = 5,
    };

    enum
    {
        EM_MISSION_TYPE_NUM = 5,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void ClearListBox();
    void UpdateData();
    void UpdateMissionData();
    void UpdateMissionText();

public:
    void OnScroll();

public:
    KD3DScrollbar*  m_pkMissionScroll;
    KSafeArray<KD3DStatic*, EM_MISSION_PAGE_NUM>    m_apkMissionText;
    KSafeArray<KD3DStatic*, EM_MISSION_PAGE_NUM>    m_apkMissionTextCount;
    KSafeArray<KD3DWnd*, EM_MISSION_PAGE_NUM>        m_apkMissionState;


    KD3DStatic* m_pkMissionCount;
    KD3DWnd*    m_pkMissionEmpty;
    KD3DWnd*    m_pkMissionType[EM_MISSION_PAGE_NUM][EM_MISSION_TYPE_NUM];

public:
    std::vector<std::pair<bool,sMissionState>> m_vecMissionListState;

    int             m_nMissionIndex;
    DWORD           m_dwMissionStaticWidth; //미션스트링이 표시될 수있는 전체크기 (미션타이틀 + 미션카운드)

};

DEFINE_WND_FACTORY( KGCRoomDungeonMissionInfoS6 );
DECLARE_WND_FACTORY( KGCRoomDungeonMissionInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomDungeonMissionInfoS6 );
