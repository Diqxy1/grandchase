#pragma once

#define DUNGEON_NUM = 12

class KGCPartyMatch : public KD3DDialog,        // extends
                 public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCPartyMatch );
    /// Default constructor
    KGCPartyMatch( void );
    /// Default destructor
    virtual ~KGCPartyMatch( void );
    /// Copy constructor
    KGCPartyMatch( const KGCPartyMatch& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyMatch& operator=( const KGCPartyMatch& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_EDIT_LEN = 16,
    };

    enum MAP {
        FOREST_OF_ELF_REVERS = 1, 
        VALLEY_OF_OATH_REVERS,
        FOREST_OF_ELF,
        SWAMP_OF_OBLIVION , 
        FLYING_SHIP , 
        VALLEY_OF_OATH , 
        FOGOTTEN_CITY , 
        BABEL_OF_X_MAS , 
        TEMPLE_OF_FIRE , 
        SHOOTING_FIELD , 
        HELL_BRIDGE , 
        TEMPLE_OF_ORC , 
        GUILD , 
        CARRIBEACH , 
        FOREST_OF_TRIAL , 
        GRAVEYARD_OF_DECEASED , 
    };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void OnCancel( void );
    void OnClickMapSelect ( void );
    void UpdateMapList( void );
    void ChangeItemMode( bool bRightMove_ );
    void ChangeMatchMode( bool bRightMove_ );
    void OnConfirm( void );
    void SetValidGameCategory( void );
public:
    KD3DStatic* m_pkRoomNameStr;
    KD3DStatic* m_pkPasswordStr;    
    KD3DStatic* m_pkTeamStr;
    KD3DStatic* m_pkSurvivalStr;
    KD3DStatic* m_pkMapStr;    
    KD3DWnd* m_pkTeamModeChkBox;
    KD3DWnd* m_pkSurvModeChkBox;
    
    KD3DWnd* m_pkTeamModeCheck;
    KD3DWnd* m_pkSurvModeCheck;
    
    KD3DStatic* m_pkMapSelectStr;
    KD3DStatic* m_pkMapSelect;
    KD3DStatic* m_pkModeSelectStr;
    KD3DWnd* m_pkConfirm;
    KD3DWnd* m_pkCancel;
    KD3DWnd* m_pkClose;
    KD3DEdit*  m_pkRoomName;
    KD3DEdit*  m_pkPassword;    
    KGCMultiMenu* m_pkMapMenu;
    KGCChangeBox* m_pkItemModeBox;
    KGCChangeBox* m_pkMatchModeBox;

public:

    int m_iSelectedModeIndex;
    int m_iSelectedSubMode;
    int m_iGameModeCategory;

    EGCGameModeCategory m_eUIGameModeCategory;
    int                 m_nUISubGameMode;
    EGCGameMode         m_eUIGameMode;
    EGCGameStage        m_eUIGameStage;

    std::map<EGCGameModeCategory, std::wstring>  m_mapGameModeCategoryNameID;
    std::map<int, std::wstring>       m_mapSubGameModeNameID;
    std::map<int , EGCGameModeCategory>  m_mapValidGameCategory;
    std::map<EGCGameStage, int>         m_mapMapNameID;
};

DEFINE_WND_FACTORY( KGCPartyMatch );
DECLARE_WND_FACTORY( KGCPartyMatch );
DECLARE_WND_FACTORY_NAME( KGCPartyMatch );
