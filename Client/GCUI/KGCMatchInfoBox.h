#ifndef _KGCMatchInfoBox_H_
#define _KGCMatchInfoBox_H_

#if _MSC_VER > 1000
#pragma once
#endif 

const int GUILD_BATTLE_INFO_PAIR = 2;
const int GUILD_INFO_GAP =-2;

class KGCSingleImage;
class KGCDeathMatchOption;
class KGCGuildBattleInfo;

class KGCMatchInfoBox : public KD3DWnd // extends
    , public KActionListener 
{
public:
    DECLARE_CLASSNAME( KGCMatchInfoBox );
    /// Default constructor
    KGCMatchInfoBox( void );
    /// Default destructor
    virtual ~KGCMatchInfoBox( void );
    /// Copy constructor
    KGCMatchInfoBox( const KGCMatchInfoBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMatchInfoBox& operator=( const KGCMatchInfoBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum ECHECKWNDTYPE {
        SUBMODE_CHECK = 0,
        TEAM_OR_SURVIVAL_CHECK = 1,
    };

public:
    void OnInitialize( void );
    void UpdateGameData();									//UI에 현재 상태를 적용합니다
    void ChangeGameModeCategory( bool bRightMove );
    void ChangeSubGameMode( bool bRightMove );
    void ChangeUIGameMode( void );
    void SendRoomInfo();
    void EnableModeBox( bool bEnable );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void CreateModeCategoryContainer();
    void UpdateModeImage( EGCGameModeCategory eGameMode );
    bool GetTextureFileNum(IN EGCGameModeCategory eGameMode, OUT int& iFileNum, OUT int& iIndex);
    void OnClickOptionBtn();
    void OnClickModeInfoBtn();
    void SetGuildBattleInfo();
    void ModeCheck();
    void DrawCheckImg( KD3DWnd *pkBaseWnd , ECHECKWNDTYPE eCheckType );

protected:
    KGCDeathMatchOption* m_pDeathMatchOption;
    KGCGuildBattleInfo* m_pGuildBattleInfo;
    KGCChangeBox*       m_pkGameModeBox;
    KGCChangeBox*       m_pkSubGameModeBox;
    KD3DWnd*            m_pkSubModeNoItem;
    KD3DWnd*            m_pkSubModeItem;
    KD3DWnd*            m_pkTeamMode;
    KD3DWnd*            m_pkSurvivalMode;
    KD3DWnd*            m_pkOptionBtn;
    KD3DWnd*            m_pkModeInfoBtn;
    KD3DWnd*            m_pkSubModeCheck;
    KD3DWnd*            m_pkTeamSurvCheck;

    EGCGameModeCategory m_eUIGameModeCategory;
    int                 m_nUISubGameMode;

    std::map<EGCGameModeCategory, std::pair<int, int> >  m_mapGameModeCategoryNameID;
    KSafeArray<KGCGuildBattleInfo*,GUILD_BATTLE_INFO_PAIR> m_pGuildInfo;
    KGCSingleImage* m_pkModeImage;

};

DEFINE_WND_FACTORY( KGCMatchInfoBox );
DECLARE_WND_FACTORY( KGCMatchInfoBox );
DECLARE_WND_FACTORY_NAME( KGCMatchInfoBox );

#endif // _GCTEAMBOX_H_