#pragma once
//#include <string>
//#include <map>
//#include <vector>
//#include <set>
#include <WTypes.h>
#include "GCUI/GCGuide.h"
#include "GCUI/GCUIScene.h"

enum GUIDE_PROGRESS_STATE {
    GPS_BELOW = 0,      // 조건미달
    GPS_READY,          // 가이드 선행조건 완료
    GPS_CAN_START,      // 가이드 실행 가능함
    GPS_DONE,           // 가이드 진행 완료
};

enum GUIDE_START_CONDITION {
    GSC_TUTORIAL_GUIDE_START = 0,
    GSC_DUNGEON_TUTORIAL_GUDIE_START =1,
    GSC_DUNGEON_TUTORIAL_GUDIE_START_IN_ROOM = 2,
    GSC_GUIDE_SKILL_TRAINABLE = 3,
};

// 액션
struct SAction {
    int nTime;
    bool bHighlightClick;
    bool bMenuOpenThenPass;

    SAction()
        : nTime( -1 )
        , bHighlightClick( false )
        , bMenuOpenThenPass( false )
    {}

};

// 가이드 발동을 위한 선행조건
struct SRequirementinfo {
    int nGuideState;
    int nGameMode;
    int nRequirementLevelMin;
    int nRequirementLevelMax;
    int nCompleteGuide;
    std::vector<int> vecMissionID;
    int nSpecifiedCondition;
    int nConcurrentCompleteGuideType;
    std::vector<int> vecRegistMissionID;
    int nDungeonID;

    SRequirementinfo()
        : nGuideState( -1 )
        , nGameMode( -1 ) 
        , nRequirementLevelMin( -1 )
        , nRequirementLevelMax( -1 )
        , nCompleteGuide( -1 )
        , nSpecifiedCondition(-1)
        , nConcurrentCompleteGuideType(-1)
        , nDungeonID(-1)
    {
        vecMissionID.clear();
        vecRegistMissionID.clear();
    }

};

// 밝게 표시해야 하는 부분
struct SHighlightInfo {
    int nHighLightType;
    std::string UIScriptName;
    std::string UIName;
    float fHighLightPosX;
    float fHighLightPosY;
    DWORD dwHighlightWidth;
    DWORD dwHighlightHeight;
    bool bUseSizeFromUI;
    bool bUsePosFromUI;
    DWORD ActiveOffset;

    SHighlightInfo()
        : nHighLightType( 0 )
        , fHighLightPosX( 0.0f )
        , fHighLightPosY( 0.0f )
        , dwHighlightWidth( 0 )
        , dwHighlightHeight( 0 )
        , bUseSizeFromUI( false )
        , bUsePosFromUI( false )
        , ActiveOffset( 5 )
    {
        UIScriptName.clear();
        UIName.clear();
    }

};

// NPC
struct SNPCInfo {
    int nNPCType;
    float fNPCPosX;
    float fNPCPosY;
    int nNPCDesc;

    SNPCInfo()
        : nNPCType( -1 )
        , fNPCPosX( 0.0f )
        , fNPCPosY( 0.0f )
        , nNPCDesc( -1 )
    {}
};

// 화살표
struct SArrowInfo {
    int nArrowType;
    float fArrowPosX;
    float fArrowPosY;

    SArrowInfo()
        : nArrowType( -1 )
        , fArrowPosX( 0.0f )
        , fArrowPosY( 0.0f )
    {}
};

// 파티클 사용시 구조체
struct SGuidePaticleInfo {
    std::string strPaticleName;
    float x;
    float y;
    int iLayer;

    SGuidePaticleInfo()
        : x(0.0f)
        , y(0.0f)
        , iLayer(0)
    { strPaticleName.clear();}

    const SGuidePaticleInfo& operator=(const SGuidePaticleInfo rht)
    {
        strPaticleName = rht.strPaticleName;
        x = rht.x;
        y = rht.y;
        iLayer = rht.iLayer;
        return *this;
    }

};

// 스텝 : 가이드 단계별 UI를 어떻게 표시할 것인가를 저장한 데이터 구조체
struct SStep {
    SAction kAction;                                // 스텝을 넘어가기 위한 동작
    SHighlightInfo kHighlightInfo;                  // 하이라이트 영역
    SNPCInfo kNPCInfo;                              // NPC
    SArrowInfo kArrowInfo;                          // 화살표
    std::vector<SGuidePaticleInfo> vecPaticle;      // 파티클
    std::vector<DWORD> vecData;                     // 기타 코드 내 처리를 위한 데이터
};

class KGCGuideMgr
{
public:
    KGCGuideMgr(void);
    ~KGCGuideMgr(void);

#ifndef __PATH__ 
    void RegisterFileMoniter();
#endif

    bool LoadScript();
    void FrameMove();
    void UpdateGuideState();
    void UpdateExcuteState();
    void CheckEnableGuide();
    void StartGuide( IN int nGuideType );
    void EndGuide();
    void OnEventSystemGuideCompleteInfoNot( IN KEVENT_SYSTEM_GUIDE_COMPLETE_INFO_NOT& kRecv_ );
    void UpdateMissionInfo( IN DWORD dwMissionID );

    bool CheckSpecifiedCondition( int iSpecifiedCondition );
    void SetContinentEffect( bool bEffect );
    bool GetContinentEffect();
    void ForcedGuide( int iGuideType );
    bool IsCompleteMissioninfo( IN int iGuideType );
    bool IsGuideOngoing(){ return ( m_nCurrentGuideType != -1 ); }
    bool CanUseMsgBox();
    void SendGuideCompleteInfo( IN int nGuideType );
    int GetCurrentGuideType(){ return m_nCurrentGuideType; }
    bool IsCompleteGuide( int iGuideType );


protected:
    int m_nCurrentGuideType;

    // 아래 맵 데이터의 int 값은 GuideType
    std::map< int, std::vector<SStep> > m_mapGuideStepInfo;        // 가이드 정보 GCGuide.lua
    std::map< int, SRequirementinfo >   m_mapGuideRequirementInfo; // 가이드별 발동 요구조건 GCGuide.lua
    std::map< int, int >                m_mapGuidProgressState;    // 가이드 진행상태 GUIDE_PROGRESS_STATE
    std::map< int, KGuideCompleteInfo > m_mapGuideComplete;        // 서버에서 받은 가이드 완료여부
    std::map<int,int>                   m_mapPairCompleteGuideType; //가이드 끼리 엮어 있는 묶음
private:
    bool m_bEffect;


#ifndef __PATH__ 
    // 테스트용 변수
    int m_nTestType;
    bool Test();

#endif

};

