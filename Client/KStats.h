#ifndef _KSTATS_H_
#define _KSTATS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <Windows.h>
//#include <string>
//#include <vector>
//#include <map>
#include "ProtectedType.h"
#ifndef __PATH__

extern KProtectedType<int>     g_aiArrowScore[MAX_PLAYER_NUM][3];

class KStatsCS : public CRITICAL_SECTION
{
public:
    KStatsCS()    { InitializeCriticalSection( this ); }
    ~KStatsCS()   { DeleteCriticalSection( this ); }
    CRITICAL_SECTION* GetCS(){ return (CRITICAL_SECTION*)this; }
};

// helper class for lock
class KStatsLocker
{
public:
    KStatsLocker( CRITICAL_SECTION* pCS ) : m_pCS( pCS )
    {
        Lock();
    }

    ~KStatsLocker() 
    { 
        if( m_pCS )
            Unlock(); 
    }
    void Lock()
    {
        EnterCriticalSection( m_pCS );
    }
    void Unlock()
    {
        LeaveCriticalSection( m_pCS );
        m_pCS = NULL;
    }
protected:
    CRITICAL_SECTION*   m_pCS;
};


//==============================================================================================
// 2005-11-09 Asirion 
// DirectX를 이용한 게임의 Data출력 및 변환 클래스.
//==============================================================================================

#define             KSTATS_LINE_HEIGHT          16
#define             KSTATS_VALUE_LENGTH         128
#define             KSTATS_VIEW_SEGMENT_NUM     20
enum STATS_SEG_TYPE
{
    STATS_TYPE_INT = 0,
    STATS_TYPE_UINT,
    STATS_TYPE_FLOAT,
    STATS_TYPE_BOOL,
    STATS_TYPE_DWORD,
    STATS_TYPE_PT_CHAR,
    STATS_TYPE_PT_INT,
    STATS_TYPE_PT_UINT,
    STATS_TYPE_PT_FLOAT,
    STATS_TYPE_PT_UCHAR,
    STATS_TYPE_STATIC_TEXT,
    STATS_TYPE_END,
};
class KStatsSegment
{
public:
    //변수
    //함수
    KStatsSegment( UINT uiType_, std::wstring strName_, void* pPtr_ );
    virtual ~KStatsSegment();
    WCHAR* ToString();
    // 새로운 데이터 형이 추가 될때 마다 아래 함수에 데이터 형을 추가해 주어야 합니다.
    void SetValue( WCHAR* pValue );
    void RestoreValue();
    std::wstring GetName(){ return m_strName; }
private:
    //변수
    UINT            m_uiType;
    std::wstring    m_strName;
    WCHAR           m_strBuffer[KSTATS_VALUE_LENGTH];
    WCHAR           m_strInitValue[KSTATS_VALUE_LENGTH];
    void*           m_pPtr;
    //함수
    UINT GetType() { return m_uiType; }
};

class KStatsPage
{    
public:
    //변수
    //함수
    KStatsPage( std::wstring strName_, D3DCOLOR dwPageNameColor_, D3DCOLOR dwTextColor_, D3DCOLOR dwSelectedTextColor_ );
    virtual ~KStatsPage();
    void CleanUp();
    void InsertValue( UINT uiType_, std::wstring strName_, void* pPtr_, bool bExceptSameSegment_ = false );
    void SetValue( WCHAR* pStrValue_ );
    void RestorePage();
    void RestoreCurSeg();
    void SetStartPos( POINT kStartPos_ );
    void Next();
    void Previous();
    KStatsSegment* GetCurSegment(){ return m_pkCurSegment; }
    void Render( UINT uiCurIndex_, UINT uiSize_ );
    void DrawText( WCHAR* str_, D3DCOLOR dwColor_);
    bool operator == ( KStatsPage* pPage_ );
    std::wstring GetName();
    KStatsSegment* GetSegment( int iIndex_ );
    int GetSize(){ return (int)m_vecSegment.size(); }
    void Print();
private:
    //변수
    std::wstring    m_strName;
    WCHAR           m_strPageName[KSTATS_VALUE_LENGTH + 32];
    WCHAR           m_strCurSegInfo[KSTATS_VALUE_LENGTH + 32];
    std::vector< KStatsSegment*  > m_vecSegment;
    POINT           m_kStartPos;
    POINT           m_kCurPos;
    KStatsSegment*  m_pkCurSegment;
    UINT            m_uiCurIndex;
    UINT            m_uiIndexOnScreen;
    D3DCOLOR        m_dwPageNameColor;
    D3DCOLOR        m_dwTextColor;
    D3DCOLOR        m_dwSelectedTextColor;
    KStatsCS        m_kPageCS;
    //함수
};

#define             KSTATS_LOG_NUM              5

class KStatsManager
{
public:
    struct KElement
    {
        std::wstring m_strName;
        KStatsPage* m_pPage;
        KElement( std::wstring strName_, KStatsPage* pPage_)
        {
            m_strName = strName_;
            m_pPage = pPage_;
        }
    };
public:
    //변수
    //함수
    KStatsManager(void);
    virtual ~KStatsManager(void);
    void Init( LONG lStartX_, LONG lStartY_, DWORD dwColor_ = 0xFFFFFFFF, bool bEnable_ = false );
    void CleanUp();
    bool Process( WCHAR* pMsg_ );
    void Render();
    void EventProc( UINT uiEvent_ );
    void SetEnable( bool bEnable_ );
    bool GetEnable(){ return m_bEnable; }
    void SetStartPos( POINT kPos_ );
    void InsertPage( std::wstring strName_ );

    void InsertLog( WCHAR* str_ );
    void InsertStaticText( WCHAR* pStr_ );
    void InsertDebugValue( WCHAR* pStr_ );
    void DeletePage( std::wstring strName_ );
    void ClearDebugValue();
    void SetValue( WCHAR* pStrValue_ );
    //==========================================================================================
    // 새로운 데이터 형을 추가 하고 싶으면 아래에 함수를 추가 하시면 됩니다.
    void InsertValue( std::wstring strName_, int* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, UINT* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, FLOAT* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, KProtectedType<char>* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, KProtectedType<int>* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, KProtectedType<float>* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, KProtectedType<UINT>* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, KProtectedType<UCHAR>* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, bool* pPtr_, WCHAR* pPageName_ = NULL );
    void InsertValue( std::wstring strName_, DWORD* pPtr_, WCHAR* pPageName_ = NULL );

    //==========================================================================================
    void InsertHandler();
    void InsertHelperHandler();

    //==========================================================================================
    //GC Code에 필요한 핸들링 함수들...
	void MakeCrash();
	void OnReSizeWindow();
    void OnNextStage();
    void OnFullHPMP();
    void OnSetValue();
    void OnReloadScript();
    void OnPrintPage();
    void OnDropGameItem();
    void OnReload_AI();
    void OnDeviceList();
    void OnAIEnable();
    void OnRenderMonster();
    void OnEagleSite();
    void OnPlayerAction();
    void OnActionScript();
    void OnReloadMonster();
    void OnReloadPlayerTemplate();
    void OnReloadPet();
    void OnReloadFireInfo();
    void OnOutputChat();
    void OnDummyReplay();
    void OnTutorialMotion();
    void OnCheckSbta();
    void OnSetPlayerLevel();
    void OnAnnounce();
    void OnAnnounceList();
	void OnMakeGif();
	void OnLuaMode();
    void OnLogLevel();
    void OnBuff();
    void OnAddDamage();
    void OnSkillTrain();
    void OnSkillUnTrain();
    void OnDumpSkillSetting();
    void OnDumpSkill();
    void SetSkillSetting();
    void ApplySetting();
    void OnEquipSkill();
    void OnUnEquipSkill();
    void OnUnlockGroup();
    void OnAllKill();
    void OnAllKillItem();
    void OnDropItem();
    void OnLogOff();
	void OnReloadSpecial75Texture();
	void OnSTLCrashTest();
	void OnAccessViolation();
	void OnCppException();
    void OnGodMode();
    void OnPerfect();
    void OnClear();
    void OnPrintDungeonItemDramaInfo();
    void OnAddMonsterInQuest();
    void OnToggleHUD();
    void OnMonsterAction();
    void OnTQ();
	void OnToggleAttackSlash();
	void OnExtraCharNum();
    void OnReloadHeroItemInfo();
    void OnItemBoxAlphaToggle();
    void OnReplayOpen();
    void OnCriticalToggle();
    void OnCriticalOffToggle();
    void OnChangeFont();
    void OnWndInfo();
    void OnOpenConsole();
    void OnMotion();
    void OnPetMotion();
    void OnSetDamageRaito();
    void OnGoExit();
    void OnCheckParentDamage();
    void OnWriteFrame();
    void OnProcessID();
    void OnSwitchScreenShot(); // 스샷 타입 JPG/BMP 전환
    void OnWriteQALog();
    void OnComboEffectShow();
    void OnSpecialFaceShow();
    void OnSendTimeForP2PPacket();
    void OnShowFootHold();
    void OnRecoverHP();
    void OnRecoverMP();
    void OnPlusHP();
    void OnPlusMP();
    void OnPlusPetMP();
    void OnFrameLock();
    void OnHelp();
    void OnProfileToggle();
    void OnSummonEmbark();
    void OnInvenDump();
    void OnOpenSkillTreeUI();
    void OnAddWall();	
    void OnAddSubject();
    void OnTestDrama();
    void OnShowTestImg();
    void OnDialog();
    void OnTcpTest();
    void OnTGPrint();
    
    void OnHost();


    //==========================================================================================
    //Cheat의 설명을 출력해줄 Helper함수들
    void MakeCrash_Helper();
    void OnReSizeWindow_Helper();
    void OnNextStage_Helper();
    void OnFullHPMP_Helper();
    void OnSetValue_Helper();
    void OnReloadScript_Helper();
    void OnPrintPage_Helper();
    void OnDropGameItem_Helper();
    void OnDeviceList_Helper();
    void OnAIEnable_Helper();
    void OnRenderMonster_Helper();
    void OnEagleSite_Helper();
    void OnPlayerAction_Helper();
    void OnActionScript_Helper();
    void OnReloadMonster_Helper();
    void OnReloadPet_Helper();
    void OnReloadFireInfo_Helper();
    void OnDummyReplay_Helper();
    void OnCheckSbta_Helper();
    void OnSetPlayerLevel_Helper();
    void OnMakeGif_Helper();
    void OnLuaMode_Helper();
    void OnLogLevel_Helper();
    void OnBuff_Helper();
    void OnAddDamage_Helper();
    void OnSkillTrain_Helper();
    void OnSkillUnTrain_Helper();
    void OnDumpSkillSetting_Helper();
    void OnDumpSkill_Helper();
    void ApplySetting_Helper();
    void OnEquipSkill_Helper();
    void OnUnEquipSkill_Helper();
    void OnUnlockGroup_Helper();
    void OnAllKill_Helper();
    void OnAllKillItem_Helper();
    void OnLogOff_Helper();
    void OnReloadSpecial75Texture_Helper();
    void OnSTLCrashTest_Helper();
    void OnAccessViolation_Helper();
    void OnGodMode_Helper();
    void OnPerfect_Helper();
    void OnClear_Helper();
    void OnPrintDungeonItemDramaInfo_Helper();
    void OnAddMonsterInQuest_Helper();
    void OnToggleHUD_Helper();
    void OnMonsterAction_Helper();
    void OnTQ_Helper();
    void OnToggleAttackSlash_Helper();
    void OnExtraCharNum_Helper();
    void OnReloadHeroItemInfo_Helper();
    void OnItemBoxAlphaToggle_Helper();
    void OnReplayOpen_Helper();
    void OnCriticalToggle_Helper();
    void OnCriticalOffToggle_Helper();
    void OnChangeFont_Helper();
    void OnWndInfo_Helper();
    void OnOpenConsole_Helper();
    void OnMotion_Helper();
    void OnPetMotion_Helper();
    void OnSetDamageRaito_Helper();
    void OnGoExit_Helper();
    void OnCheckParentDamage_Helper();
    void OnWriteFrame_Helper();
    void OnProcessID_Helper();
    void OnSwitchScreenShot_Helper();
    void OnWriteQALog_Helper();
    void OnComboEffectShow_Helper();
    void OnSpecialFaceShow_Helper();
    void OnShowFootHold_Helper();
    void OnSendTimeForP2PPacket_Helper();
    void OnRecoverHP_Helper();
    void OnRecoverMP_Helper();
    void OnPlusHP_Helper();
    void OnPlusMP_Helper();
    void OnPlusPetMP_Helper();
    void OnFrameLock_Helper();
    void OnSummonEmbark_Helper();
    void OnHelp_Helper();
    void OnInvenDump_Helper();
    void OnOpenSkillTreeUI_Helper();
    void OnShowTestImg_Helper();
    void OnAddWall_Helper();	
    void OnAddSubject_Helper();
    void OnDialog_Helper();
    void OnTGPrint_Helper();
    void OnToggleTestStatic();

private:
    //변수
    POINT           m_kStartPos;
    D3DCOLOR        m_dwTextColor;
    D3DCOLOR        m_dwSelectedTextColor;
    D3DCOLOR        m_dwPageNameColor;
    KStatsPage      *m_pkCurPage;
    std::vector< KElement > m_vecPage;  //맵으로 하면 귀찮다--;;;;
    bool            m_bEnable;
    UINT            m_uiCurIndex;
    typedef void (KStatsManager::*HANDLER)();
    std::map<std::wstring, HANDLER> m_mapHandler;
    std::map<std::wstring, HANDLER> m_mapHelperHandler;
    std::vector< std::wstring >     m_vecLog;

public:
    bool m_bModeHUD;
    bool m_bEaglesite;

public:
    //함수
    KStatsPage* GetPage( WCHAR* pStrName_ );
    void Next();
    void Previous();
    bool Find( std::wstring strName_ );
    void InsertValue( UINT uiType_, std::wstring strName_, void* pPtr_, KStatsPage* pPage_ );
    bool GetToogleHUD();
    bool GetEagleSiteCheat();
private:
    static int m_aiAbility[][3];
};
extern          KStatsManager          g_kStatsManager;

#endif // __PATH__

#endif // _KSTATS_H_
