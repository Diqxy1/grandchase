#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Lua/KLuaManager.h"
#include "UserPacket.h"


// 시스템 가이드
// 작업자 :
//    서버 - 손기운
//    클라 - 이재준
//    기획 - 장상준
//    Comment : 클라이언트에서 재접속한 유저의 완료 미션에 대한 가이드 정보를 알 수 없기 때문에 해당 처리만 서버에서 함.
//              서버는 클라이언트의 약식 버전 스크립트를 사용. (조건 미션과 지급 아이템 정보)
//              추가적으로 캐릭터 별로 다르게 아이템 지급할 수 있게 수정 함.

class KSystemGuide
{
    DeclareSingleton( KSystemGuide );
    NiDeclareRootRTTI( KSystemGuide );
    DeclToStringW;
public:
    KSystemGuide(void);
    ~KSystemGuide(void);

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool GetProvideItem( IN int& nGuideType, IN int& nCharType, OUT VEC_REWARD& vecProvide );
    bool CheckGuideNeedMission( IN DWORD& dwMissionID, OUT int& nGuideType );

private:
    bool LoadGuideElements( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, std::map< int, VEC_REWARD > >& mapProvideItem,
        OUT std::map<int, std::vector< DWORD > >& mapGuideRequireMission );
    bool LoadRequireMissionList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector< DWORD >& vecRequireMission );
    bool LoadMissionList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT OUT std::vector< DWORD >& vecMissionList );
    bool LoadProvideItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, VEC_REWARD >& mapProvideItemInfo );
    bool LoadItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );

private:
    mutable KncCriticalSection m_csSystemGuide;

    // reward랑은 개념이 달라서 provide로 단어 선정 함
    std::map< int, std::map< int, VEC_REWARD > > m_mapProvideItem;
    std::map< int, std::vector< DWORD > >        m_mapRequirMission;
};

DefSingletonInline( KSystemGuide );
DeclOstmOperatorA( KSystemGuide );

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class KSystemGuideUserData
{
public:
    KSystemGuideUserData();
    ~KSystemGuideUserData();

    void SetGuideCompleteInfo( IN std::map< int, KGuideCompleteInfo >& mapCompleteInfo );
    void GetGuideCompleteInfo( OUT std::map< int, KGuideCompleteInfo >& mapCompleteInfo );
    void UpdateGuideComplete( IN int& nGuideType, IN const KGuideCompleteInfo& kCompleteInfo );
    void UpdateItemProvideComplete( IN int& nGuideType );
    void GetProvideDataByGuideType( IN int& nGuideType, OUT KGuideCompleteInfo& kCompleteInfo );
    bool CheckAlreadyCompleteGuide( IN int& nGuideType );
    bool CheckAlreadyItemProvide( IN int& nGuideType );

private:
    std::map< int, KGuideCompleteInfo > m_mapComplete;
};