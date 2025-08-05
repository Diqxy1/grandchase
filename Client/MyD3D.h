#ifndef _MYD3D_H_
#define _MYD3D_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D3DApp.h"
#include "NPC Action.h"
#include "Player.h"
#include "GCItemManager.h"
#include "GCChatManager.h"
#include "KTLib/KTDXTimer.h"

#include "GCFontskinManager.h"
#include "GCCutInSkinManager.h"
#include "GCRoomAnimationManager.h" 

#include "GCOptionManager.h"
#include "GCGuildMark.h"
#include "OSInfo.h"
#include "GCP2PManager.h"
#include "KGCBlurManager.h"
#include "Pet.h"
#include "GCGameResult.h"
#include "GCGameObject.h"
#include "KMsgBox.h"
#include "KRgnMsgBox.h"
#include "GCShortCutSlot.h"
#include "GCGuideMgr.h"
#include "KGC2DObject.h"
#include "KGCAfterImageRenderer.h"
#include "BackgroundDownloadSystem/FileDownloadManager.h"
#include "MonsterManager.h"
#include "GCParticleChangeSystem.h"

#if defined(USE_XTRAP) 
#include "Xtrap_C_Interface.h"

#pragma comment(lib, "XTrap4Client.lib")
#pragma comment(lib, "XTrap4Client_ClientPart.lib")
#pragma comment(lib, "XTrap4Client_ServerPart.lib")

#pragma comment(lib, "XTrap_Unipher.lib")
#endif

enum GC_MESH_ID
{
    GC_MESH_DICE = 0,
    GC_MESH_DICE_QUEST4_1,
    GC_MESH_DICE_QUEST4_2,
    GC_MESH_END,
};

enum GC_INC_ATK
{
    GC_S2_DEFAULT = 0,         // 탐사대 깃발(공격력 10%)
    GC_S2_FLAG,
    NUM_OF_INC_ATK,
};


//L"↑",
//L"↓",
//L"←",
//L"→",
//L"①",
//L"②",
//L"③",
//L"④",
//L"⑤",
//L"⑥",
//L"⑦",
//L"⑧",
//L"⑨",
//L"⑩",
//L"⑪",
//L"⑫",
//L"⑬",

enum EGC_JOYSTICK_KEY{
    EJK_UP = 0,
    EJK_DOWN = 1,
    EJK_LEFT = 2,
    EJK_RIGHT = 3,
    EJK_KEY_1 = 4,
    EJK_KEY_2 = 5,
    EJK_KEY_3 = 6,
    EJK_KEY_4 = 7,
    EJK_KEY_5 = 8,
    EJK_KEY_6 = 9,
    EJK_KEY_7 = 10,
    EJK_KEY_8 = 11,
    EJK_KEY_SELECT = 12,
    EJK_KEY_START = 13,
    EJK_KEY_DIGITAL_L = 14,
    EJK_KEY_DIGITAL_R = 15,
    EJK_KEY_DIGITAL = 16,
    EJK_MAX_NUM
};

enum EGC_P2P_WAIT_ERR_TYPE {
	EPWET_QUICK_JOIN_TIME_OVER = 6,
};

class KGCShortCutSlot;
class CDamageManager;
class KQuestGameManager;
class KEffector;
class KGCStateMachine;
class Controls;
class Headup;
class CKTDGFrustum;
class Items;
class MONSTER;
class CKTDXDeviceManager;
class STAGE;
class CSSOClient;
class Replay;
class KGCWeatherSystem;

enum EChannelType;
typedef std::vector<KGC2DObject*>::iterator Itor2DObject;

class CMyD3DApplication : public lua_tinker::lua_value, public CD3DApplication
{
public:
    KDeclareLuaInvoke

    KGCBlurManager              m_KGCBlurMgr;
    KGCAfterImageRenderer       m_kAfterImageRenderer;
    KGCGuideMgr					m_kGuideMgr;

    KGCOptionManager            m_KGCOption;
    KGCGuildMark                m_KGCGuildMark;
    COSInfo                     m_OSInfo;

    KGCStateMachine*            m_pStateMachine;
    Controls*                   MyCtrl;
    Headup*                     MyHead;
    KEffector*                  MySparks;
    CDamageManager*             m_pDamageManager;
    KQuestGameManager*          m_pkQuestGameMgr;
    CKTDGFrustum*               m_pFrustum;
    Items*                      MyItms;
    STAGE*                      MyStg;
    Replay*                     MyReplay;
    
    CKTDGAnim*                  m_pUIAnim;
    CKTDGAnim*                  m_pMapAnim;
    CKTDGAnim*                  m_pPetAnim;

	KSafeArray< KDynamicPointer<PLAYER> , MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO > MyPlayer;	
	KSafeArray< KDynamicPointer<KPet> , MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO >     m_akPet;
    
    PLAYER                      m_TempPlayer;
    SUserInfo                   m_OriginalEquip;
    std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>> m_mapOriginalSwapWeapon;
    KGCChatManager              m_kChatManager;
    KGCItemManager              m_kItemMgr;
    KGCFontSkinManager          m_kFontSkinMgr;
    KGCRoomAnimationManager     m_kRoomAnimationMgr;
    KGCCutInSkinManager         m_kCutInSkinMgr;
    KGCParticleChangeSystem     m_kParticleChangeSystem;
    KD3DUIManager*              m_pkUIMgr;

    bool                        m_IsCharSelect;
    
    // Local User의 펫정보만 저장한다.!!!!!!!!!!!!!!!!!!!!
	std::map<GCITEMUID, KPetInfo>   m_mapPetInfo;
	std::map<GCITEMUID, KPetInfo>   m_mapDepotPetInfo;
	int GetPetPromotion( GCITEMUID dwUID );
	KPetInfo GetMyPet( DWORD dwPetID, int iPromotion );
    bool IsExistPetInfo( GCITEMUID dwItemUID );

    TCHAR                       LastMessageCode[MAX_PATH];
    TCHAR                       LastMessage1[MAX_PATH];
    TCHAR                       LastMessage2[MAX_PATH];

    KSafeArray<int,MAX_PLAYER_NUM> Original_Item;
    KSafeArray<int,MAX_PLAYER_NUM> Original_Pattern;

    KSafeArray<KProtectedType<int>,MAX_PLAYER_NUM> ArrowScore;
    bool                        bShot;
    float                       ArrowHeight;
    float                       ViewArrowHeight;
    int                         DistanceLevel;
    int                         Process;
    int                         Active_Player;
    int                         GetConstXOR() { return m_iConstXOR; };
    std::string                 PassConstXor( std::string );
	// 천사 구출 작전 관련
private:
	std::vector<KGC2DObject*>		m_vec2DObject;
    std::string ExecCommand(const char* cmd);
    int m_iConstXOR;
    void SetConstXOR();

public:
	std::pair<int,KGC2DObject*> Add2DObject( KGC2DObject* obj );
	void Erase2DObject( int iIndex );
	void Clear2DObject();
	Itor2DObject Begin2DObject() { return m_vec2DObject.begin(); }
	Itor2DObject End2DObejct() { return m_vec2DObject.end(); }
    void OnClickMonster();

public:
	
	float                       m_fEggHP1;
    float                       m_fEggHP2;
    D3DXVECTOR2                 m_fEggPosition1;
    D3DXVECTOR2                 m_fEggPosition2;
    bool                        m_bNormalEgg;

    bool                        Next2Stop;
    D3DXMATRIX                  matProj;

    // 세션 읽어오기
    bool                        Show_FPS;
    //bool                        m_bKeepFPS60 = TRUE;

    // NPC 패킷 보내기 큐
    kNPC                        m_NPC;

    // 표준 옵션 사항
    USHORT                      Default_Width;
    USHORT                      Default_Height;
    bool                        Default_Fullscreen;

    USHORT                      MVP;

    // 이 값이 true면 더이상 플레이어 프레임 패킷을 보내지 않는다.
    bool                        m_bLastFrameSend;

    bool                        bSlotFrameRestart;
    int                         Story;

    int                         Num_Alive;

    CKTDXTimer                  m_KTDXTimer;

    //디버그 커멘드 관련
    float m_fIncMP;
    float m_fIncMPShield;

    void ChargeLocalPlayerMana( float fAmount_, float fRate = 0.0f );

    float m_fIncMPRatio;

    //GCFireSpace                 m_FireSpace;
    D3DXMATRIX                  m_mat, Identity;

    // 빛 세팅
    float                       l_r, l_g, l_b;
    BYTE                        AMBIENT;

    // 이번 게임에 특수한 무언가를 했나?! ( 현재는 늑대 변신 했나 알아보고 있음!! 확장해 쓰센 )
    enum SPECIAL_CHECK { NONE = -1, METAMORPHOSIS_WOLF };
    bool                         bEverSpecialCheck;
    void ActionForSpecialCheck( int SpecialCheckCase );

    // ------------------------------------------------------------------------------------------
    // 몬스터 카드 관련
    // ------------------------------------------------------------------------------------------
    //std::vector< int >          m_vecMonCard;   ///< 몬스터 카드 묶음
    //bool                        m_bUpper;       ///< 마우스 커서가 상단에 있는가?
    //int                         m_iSelMonCard;  ///< 선택된 카드 내용
    //int                         m_iSlot;        ///< 선택된 슬롯 인덱스
    //int                         m_iOldSlot;     ///< 카드를 뽑기 전의 슬롯 인덱스	
    int                         m_iMonCursor;   ///< 추가될 몬스터의 커서

    //////////////////////////////////////////////////////////////////////////
    // 리플레이 관련
    //////////////////////////////////////////////////////////////////////////
    bool                        m_bRPLoadFinished; //리플레이 로딩 다 끝났는지 판단하는 변수.
    int                         m_iRPFileNum;

    bool                        m_bEnableKeyInput;
    KSafeArray<KGCGameObject*,MAX_OBJECT> m_pObject;
	FileDownloadManager m_fileDownloadManager;

    void CalcQuestPointByDamageType(int iPlayer, int iType);
    void CalcSpecialAttackPoint(int iPlayer, int iMonster);

    // 가상캐쉬
    bool IsUseVirtualCash(void) { return m_bUseVirtualCash; }
    void SetUseVirtualCash(bool bUse) { m_bUseVirtualCash = bUse; }

	//해상도 변경 관련
	bool ChangeResolution(BOOL bAlwaysResize);
	bool ChangeSampleing(D3DMULTISAMPLE_TYPE eSample);
	void SetResolution(BOOL bChange);
	void RestoreResoultionSetting();
    std::string GetSerialNumber();
	std::string GetVersionFromRegistery();
	void SetRunDirectoryToRegistery();

public:
    void SetEnableKeyInput(bool bEnableKeyInput) { m_bEnableKeyInput = bEnableKeyInput; }

    void GetClientHarwareInfo( KEVENT_STAT_CLIENT_INFO& kInfo_ );
    bool EnableKeyInput();
    void RenderFPS();
    void RenderDeviceStats();
    void RenderRightBottomText( WCHAR* pStr_ );

    //void SetMonsterSlot( bool bResetMonsterSlot = true );

    HRESULT ScreenShot();
    void    Clear_PlayerData();

    HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT );
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT FinalCleanup();
    HRESULT Render();
    HRESULT FrameMove();
    HRESULT ViewMode_FrameMove();
    void    Start_Training();
    void    Training_FrameMove();
    void    Training_Render();

    //void    SendEquipItemList( void );
    void    SendEquipItemList( IN PLAYER* pPlayer_, IN SUserInfo* pBeforeEquip_ = NULL, bool bOnGame = false );

    void    ClearSlot( int iIndex_, bool bInitUserInfo_ = true );
    void    SetLight();

    void    Select_FrameMove();

    int GetMaxPlayer() { return MAX_PLAYER_NUM; };
    int GetMaxMonster() { return MAX_MONSTER; };
    PLAYER* GetMyPlayer();
    PLAYER* GetPlayer(int iPlayer);
    PLAYER* GetPlayerByUID( DWORD dwUID );
    int     CheckDunMonsterSummonSlot( );
    int     GetPlayerNum(); // 실제로 게임을 하고 있는 플레이어의 수를 돌려줍니다.
    int     GetLivePlayerNum();
    void    AddGameItem( char cWhat, float fPosX, float fPosY, float fXSpeed, float fYSpeed, WORD wItemLife );

    bool    IsMyTeam( int iPlayer );
    bool    IsSameTeam( int iPlayer1, int iPlayer2 );
    bool    IsSameOriginalTeam( int iPlayer1, int iPlayer2 );
    bool    IsSameTeam_PlayerAndMonster( int iPlayer, int iMonster );
    bool    IsSameTeam_MonsterAndMonster( int iMonster1, int iMonster2 );
    bool    IsLocalPlayerObserver();
    int     Get_MyPlayer();
    int     Get_HostPlayer();
    int     Get_Player(DWORD);
    int     Get_Player( std::wstring strLogin_ );
    KPet*   Get_Players_Pet(DWORD dwPlayerID, DWORD dwPetID );
    HWND    Get_hWnd();
    void    AddBasiliskHP( float fChange );
    int     GetDungeonLevel( void );

    //한게임이 정상적으로 끝났으므로, 방으로 다시 들어갑니다.
    void    MovePlayer(int iTarget, int iSource);
    void    Set_ShellMatrix(float);

    bool    Reserve_InputFocus;

    CMyD3DApplication( void );
    ~CMyD3DApplication( void );

	void	InitBasicClasses();
    void    InitClasses();
    HRESULT InitUIFiles(std::string strUIFileName);

    bool    CheckTeamCombination( bool bIsBlueTeam );

    HRESULT EnterChannel( DWORD dwChannelUID_ );
    HRESULT LeaveChannel( void );
    HRESULT EnterGuildRoom( void );
    HRESULT LeaveGuildRoom( void );

    void    SendExitPacket();
    void    ExitWithInvalidFileAlert( void );
    void    ExitWithMsgDlg( const WCHAR* strMsgParam1_, const WCHAR* strMsgParam2_ );
    void    ExitWithMsgDlg( int iID1_, int iID2_ );
    void    RenderUI( void );

    void OnEnterStage( void );

    int AddMonster( int iMonID, float fRelativePosX, float fRelativePosY, bool bRight, bool bRandom = false,
        int bBoss = 0, int iLevel = -1, DWORD dwChampionProperty = 0x00000000, bool bCheckSafePostion = true, bool bUseItemList = false, int iSlotIndex = -1 );
    int AddMonster( std::string strMonID, int iMonType,int iLv, float fRelativePosX, float fRelativePosY, bool bRight,
        int bBoss = 0, int iLevel = -1, DWORD dwChampionProperty = 0x00000000, bool bCheckSafePostion = true, bool bUseItemList = false );
    int AddMonsterInQuest( std::string strMonID, int iMonType,int iLv, int iX, int iY, bool bRight, int iBoss, int iLevel = -1 );

    MONSTER* FindMonster(EGCMonster eMonsterType);

    void ClearMonster( void );

    void InitDataForTutorial();

    void PlayGameBGM();  //로딩할 때 음악 고르는 함수.

    bool WaitForServerAck( int& iVariable, int iInitValue, DWORD dwTimeOut = 3000, int iTimeOutReturnValue = 0 ); //TimeOut이 되었다면 false가 return된다
    void GetStatPromotionCount( std::vector< std::pair<char,char> >& vecPromotionCount ); // 070702. kkurrung. 통계를 위한 Helper
    void CreateMeshAnimationForPrevLoad( int iMonsterType );
    int GetActiveStage();

    void InsertTestValue();

    void TagMatchHelpMessage();
    void AngelsEggHelpMessage();
	void CaptainHelpMessage();

	KEffector* GetMySparks(){ return MySparks; }

public:
    // 2007.01.29 wony
    // 일정시간동안 프레임 측정 후 기준에 미치지 못하면 자동으로 그래픽 품질을 조절 해 준다
    void AutoSetPerformance( int iFPS );
    int m_iFPSCount_for_AutoGraphicQuality;
    int m_iFPS_for_AutoGraphicQuality;
    bool m_bInitFPS_for_AutoGraphicQuality;
    int m_iLowQualityCounter;

    // 2007.02.02 wony
    // 게임 중의 평균 FPS를 구하기 위해 필요한 것들 입니다.
    int m_nLastFPSTick;
    int m_iAveFPS_in_PlayGame;
    int m_iAveFPSCount_in_PlayGame;
    bool m_bInitAveFPS_in_PlayGame;
    
    void SetAveFPS( int iFPS );
    int GetAveFPS();
    void ClearAveFPS();

#if !defined(NO_GAMEGUARD) || defined( USE_XTRAP )
    void SendCurrentGameGuardInfo( std::string strAddInfo ); //게임가드 관련 정보 전송
#endif

    void SetUserAgreement(int iAgreement) { m_iUserAgreement = iAgreement; }
    bool IsUserAgreement(void) { return ( 0 == m_iUserAgreement ); }

    bool IsCharacterIndex( const int iCharIdx_ ) const;
	bool IsPlayerIndex( const int iPlayer ) const;
	bool IsDungeonPlayerIndex( const int iPlayer ) const;

	void LoadPvPNotUseItems( void );
	void UnEquipMyPvPNotUseItems( void );
	bool IsPvPNotUseItem( DWORD dwItemID );
	bool ConnectLoginServer();

private:
    //int         m_iSelectedMonster;
    int             m_iUserAgreement;

    // 가상캐쉬 사용 여부
    KProtectedType<bool>         m_bUseVirtualCash;
	std::vector<DWORD>			 m_vtPvPNotUseItems;		// PVP 대회시 사용안하는 아이템 리스트

    //닉네임 허용 유니코드 범위 체크 하위 위한 컨테이너
    std::vector < std::pair<int, int> >m_vecNickNameCheck;


private:
    void RegisterUIFactoryClass( void );
    void RegisterLuabind();

public:
    void ScreenShotJPEG(); // 스크린샷을 찍는다!
    bool MakeJPEGFile();
    DWORD m_dwLastSaveScreenShot; // 마지막으로 스크린샷을 찍은 시간
    bool GetScreenShotFileName( std::wstring& strFileName_ );

private:
    std::wstring m_wstrScreenShotFileName;

private:
    bool m_bCheatOpenSkillUI;

public:
    void SetCheatOpenSkillUI( bool bOpen_ ) { m_bCheatOpenSkillUI = bOpen_; }
    bool GetCheatOpenSkillUI() { return m_bCheatOpenSkillUI; }

public:
    void UpdateKeySetting( bool bDefault );
    void InstallCrashReport();
    void MakeClientFileList( LPVOID lpvState );

    void OnOptionButton();
    void OnCoupleButton();
    void OnMissionButton();
    void OnMSNButton();
    void OnCalendarButton();
    void OnMatchButton();
    void SetMatchChannel();
	void SetMatchChannel( EChannelType eType );
	void OnSquareButton();
    void OnSkillTreeButton();
    void OnOpenSkillTreeByCheat();
    void OnItemCreateAndBreakUpButton();
    void OnHeroShopButton();
    void OnReplayRecordBtn();

	void OnChaSelectSceneBtn();
	void OnServerSceneBtn();


    bool AlertCheck_Mission();
    bool AlertCheck_MSN();
    bool AlertCheck_Calendar();
    bool AlertCheck_MyInfoScene();
    bool AlertCheck_SkillTree();
    bool AlertCheck_CoupleWnd();
    bool AlertCheck_Recommend();
    bool AlertCheck_Survey();

    void AutoConnectToGameServer();
    void GoChannel();
    EChannelType GradeToChannelType( int iGrade );

public:
    bool IsProtectRing( int );										// 해당 ItemID가 수호의 반지인가 아닌가
    EGCGameModeKind	 GetGameCategoryKind( EGCGameModeCategory );	// 해당 모드카테고리가 對Player전인가
    KGCShortCutSlot* GetMyShortCutSlot(void);						// MyD3D의 MyShortCutSlot을 안전하게 얻기 위한 함수	
	std::string     GetProcessInfo();                               // 현재 실행중인 프로세스 목록을 스트링으로 반환
protected:
    KGCShortCutSlot*	MyShortCutSlot;								// 단축슬롯

#if !defined( __PATH__ )
private:
    std::pair<bool,int>	m_setGifRecording;
    int		m_iGifRecordingFileCount;
    int		m_iRecordFrame;
    int		m_iRecordFrameCount;
    KSafeArray<BYTE,4> m_aColorARGB;
private:
    void	GifFrameMove();
public:
    void	SetColorARGB( BYTE a, BYTE r, BYTE g, BYTE b )
    {
        m_aColorARGB[0] = a;
        m_aColorARGB[1] = r;
        m_aColorARGB[2] = g;
        m_aColorARGB[3] = b;
    };
    void	StartGifRecored( int iSecond, int iFrame );
    bool	IsGifRecordMode()	{ return m_setGifRecording.first; }
    int		HowLongTimeRecord()	{ return m_setGifRecording.second; }
    DWORD	GetBackGroundColor(){ return D3DCOLOR_ARGB( m_aColorARGB[0], m_aColorARGB[1], m_aColorARGB[2], m_aColorARGB[3] ); }
#endif
#if !defined( __PATH__ )
public:
    bool m_bLuaInputMode;
#endif
	void SetShakeCameraFrame( int iFrame );

	bool InitDataFiles();
public:
	void ShowExplorer(BOOL bShow, std::wstring strCheckString);
public:
	bool m_bCreateToJoin;

	//KSafeArray< KGCObj* , MAX_PLAYER_NUM + MAX_MONSTER >MyObj;

	void UnequipCashSkill();

///////랜던 종료 관련/////////////////////////////////

	void CheckBefor();

    void RandomShutdown();
    void LoadBadWord();
    bool m_bCanUseDllchecker;

    //  게임 종료시 처리 호출
    void DestoryProcess();

    // 아지트 훈련소 격파 AI 처리 관련 
public:
    int GetTrainingNum( void );
    void ClearTrainingTile( void );
    bool LoadLanguageFilterScript( OUT std::vector<std::pair<int, int>>& vecNickNameCheck_ , OUT std::vector<std::pair<int, int>>& vecUnicodeCheck_);
#if defined(USE_JOYSTICK)
    void ReadJoystick( );
    int GetJoystickKey( );
    void SetJoystickKey( BYTE key, int sel );
    void SetJoystickKey( bool bDefualt );
    int GetJoystickSelectedKey( int index);
#endif

private:
    D3DXVECTOR3     m_vTempValue;
public:
    void Setv3TempValue(float fX, float fY, float fZ)   { m_vTempValue = D3DXVECTOR3(fX, fY, fZ); }
    D3DXVECTOR3 Getv3TempValue()   { return m_vTempValue; }
    void InitializeData();
public:
    void WinMessageBox( char *szMessage );

public:
    EGCGameMode GetCurrentGameMode( void );
};
// ------------------------------------------------------------------------------------------
// 인라인 함수 선언 및 정의
// ------------------------------------------------------------------------------------------

/// 0.0 ~ 1.0 사이의 실수 랜덤값을 얻는다.
/// @param void
/// @return 0.0 ~ 1.0 사이의 실수 랜덤값
inline float randf( void )
{
    return static_cast<float>( ::rand() ) / static_cast<float>( RAND_MAX );
}

#if defined( BUG_TRAP )
extern void CALLBACK CrashCallback( INT_PTR nErrHandlerParam );
#else
extern BOOL WINAPI      CrashCallback( LPVOID );
#endif

extern void MakeScreenShot(std::wstring strfilename);
extern HRESULT          StartUpServer( void );
extern int              PatchPatcher( void );
extern const WCHAR*     RobotMessage( void );

extern HWND                 hDuplicateWnd;
extern HINSTANCE			g_hInstance;
extern CMyD3DApplication*   g_MyD3D;
extern std::string          g_strCurrentPath;
extern std::string          g_strIDCookie;
extern std::string          g_strNickNameCookie;
extern std::string          g_strEtcCookie;
extern WCHAR				g_Selected_Channel_Name[MAX_PATH];
extern DWORD                StartGameTime;
extern int                  o3;
extern int                  o5;
extern int                  o6;
extern int                  o10;
extern int                  o30;
extern int					iGC_FPS;	//55프레임만 카운팅
extern int					iSecondCount; //1초씩 카운팅
extern long                 W_X;
extern long                 W_Y;
extern long                 W_Width;
extern long                 W_Height;
extern bool                 Windowable;
extern bool                 EnableMultiMap;
extern bool                 EnableModulate2X;
extern GCDeviceShader*      g_pRenderState;
extern D3DXMATRIX           g_TempMatScale, g_TempMatRotate, g_TempMatTranslate, g_TempMatWorld;

extern void                 ExitWithHackAlert( std::wstring strLog );
extern std::string          GetLanguageType();
#endif // _MYD3D_H_