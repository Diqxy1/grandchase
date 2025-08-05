#pragma once
//#include "KLuaManager.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif

// 패킷 패커를 이용하게 되면 최대 256 바이트 패킷밖에 던질 수 가 없다.
// 그러므로 문자열의 길이는 최대 그 절반으로 한정하겠다.
#define MAX_TG_STR_LENTH    (130)

// 패킷패커가 들고 있을 수 있는 최대 버퍼를 지정해줍니다.
// 패킷패커가 1Mb 이상이 되면 렉의 원인이 됩니다.
#define MAX_TG_PACKER       (400)

//************************************************************************
// 스크립트 툴에 사용되는 Eeum 값들이다.
// 루아 스크립트 로드시 타입을 확인하기 위하여 필요.
//************************************************************************
enum TG_EACTION { TG_EACTION_DEFAULT, TG_EACTION_POSITIONPARTICLE, TG_EACTION_WIN, TG_EACTION_LOSE, TG_EACTION_MON_GENNER
                , TG_EACTION_UNITMOVEMENT, TG_EACTION_SOUND_ONOFF, TG_EACTION_LOADING, TG_EACTION_CLEAROFSTAGE, TG_EACTION_TRIGGERONOFF
                , TG_EACTION_REWARD, TG_EACTION_NEXTSTAGE, TG_EACTION_SUPERPLAYER, TG_EACTION_PLAYERPOSITION, TG_EACTION_OBJECTGENERATER
                , TG_EACTION_OPEN_DGATE, TG_EACTION_TITLETEXT, TG_EACTION_TRIGGERREADY, TG_EACTION_DIALOGUE, TG_EACTION_DELETEMONSTER
                , TG_EACTION_FADE_INOUT, TG_EACTION_HP, TG_EACTION_MP, TG_EACTION_PLAYERMOTION, TG_EACTION_UNIT_PARTICLE
                , TG_EACTION_PLAYERMOVINGTOAREA, TG_EACTION_CHANGE_OBJ_STATE, TG_EACTION_GATE_OBJECT_GEN, TG_EACTION_ADD_DAMAGE, TG_EACTION_WATER
                , TG_EACTION_WEATHER, TG_EACTION_EMBARK, TG_EACTION_WALL, TG_EACTION_SUBJECT, TG_EACTION_INFINITY_MON_GEN,
                };
enum TG_CONDITION { TG_CONDITION_DEFAULT, TG_CONDITION_KILLMASTER, TG_CONDITION_TIMER, TG_CONDITION_POSITION, TG_CONDITION_OBJ_DESTROY, TG_CONDITION_DIALOGUE_STATE, TG_CONDITION_HP_CHANGE, TG_CONDITION_WALL, TG_CONDITION_KILLALLMONSTER };


class CTriggerCondition
{
public:
    CTriggerCondition();
    virtual ~CTriggerCondition();

    // 컨디션 조건
    enum enTriggerCondition {
                                TG_DEFAULT = 0              // 기본 컨디션
                                , TG_KILL_MASTER            // 몬스터 죽이기
                                , TG_STAGE_CHANGER          // 스테이지 넘기기
                                , TG_MON_GEN                // 몬스터 생성
                                , TG_PLAYER_GEN             // 플레이어 생성
                                , TG_LOADING                // 로딩
                                , TG_CLEAR_STAGE            // 스테이지 클리어
                                , TG_DELAY                  // 시간 딜레이
                                , TG_WIN                    // 승리
                                , TG_LOSE                   // 패배
                                , TG_STAGE_DOOR             // 스테이지 문
                                , TG_NEXT_STAGE
                                , TG_REWARD
                                , TG_SOUND_ONOFF
                                , TG_SUPERPLAYER
                                , TG_TRIGGER_ONOFF
                                , TG_PLAYERPOSITION
                                , TG_POSITION
                                , TG_OBJECTGENERATER
                                , TG_OPEN_DGATE
                                , TG_TITLETEXT
                                , TG_TRIGGER_READY
                                , TG_TIMER
                                , TG_DIALOGUE
                                , TG_MP
                                , TG_HP
                                , TG_UNITMOVEMENT
                                , TG_FADEINOUT
                                , TG_PLAYERMOTION
                                , TG_MOVINGTOAREA
                                , TG_DELETEMONSTER
                                , TG_UNITPARTICLE
                                , TG_POSITIONPARTICLE
                                , TG_OBJ_DESTROY
                                , TG_OBJ_CHANGE_STATE
                                , TG_GATE_OBJECT_GENERATER
                                , TG_WATER
                                , TG_ADDDAMAGE
                                , TG_DIALOGUE_STATE
                                , TG_WEATHER
                                , TG_EMBARK
                                , TG_WALL
                                , TG_SUBJECT
                                , TG_INFINITY_MON_GEN
                            };

    enTriggerCondition      enType;                         // 컨디션의 타입
    bool                    m_bCheckByHost;                 // Host가 아닐경우 컨디션을 실행여부
    bool                    m_bRepeat;
    int                     m_iTG_ID;                       // 컨디션 아이디
    short                   m_sMaxTG_ID;                    // 총 컨디션 아이디
    bool                    m_bSendPacket;                  // 패킷을 전송 여부

    void SetComplete(bool bComplete);
    virtual void SetConditionID(int iTG_ID);
    virtual void StartTrigger(void);
    virtual bool CheckEqualCondition(CTriggerCondition *pCondition);
    virtual void EndTrigger(void);
    virtual void FrameMove(void);
    virtual bool IsComplete(void);
    virtual HRESULT LoadItemFromLua(KLuaManager &luaMgr) { return S_OK; }

protected:
    bool                    m_bComplete;                    // 컨디션 실행완료 여부

};

//************************************************************************
// 퀘스트 컨디션에 ID를 부여하기 위한 변수 및 함수...
// - MaxQuestID를 제외하곤 안쓴다. 자동생성 아이디를 고정 아이디로 변경.
//************************************************************************
extern int g_iTG_ID;            // 현재 컨디션의 ID
extern short g_sTG_MaxQuestID;    // 퀘스트 컨디션 ID의 갯수
void InitTG_ID(void);           // ID 초기화
void SetMaxQuestID(int iMaxQuestID);
