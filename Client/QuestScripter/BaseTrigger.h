#pragma once
//#include <vector>
//#include <string>
//#include "../GCLuaManager/KLuaManager.h"
#include "FunctionToScriptFile.h"
#include "EnumOfTrigger.h"

// 패킷 패커를 이용하게 되면 최대 256 바이트 패킷밖에 던질 수 가 없다.
// 그러므로 문자열의 길이는 최대 그 절반으로 한정하겠다.
#define MAX_TG_STR_LENGTH    (130)

class CBaseTGFunction
{
public:
    CBaseTGFunction() { m_enTGFType = TG_TYPE_EACTION; m_iTGID = 0; m_iStage = -1; }
    virtual ~CBaseTGFunction() {}

    TG_TYPE             m_enTGFType;
    bool                m_bRepeat;
    int                 m_iTGID;
    int                 m_iStage;
    HTREEITEM           m_hItem;
    std::wstring        m_strName;
    std::wstring        m_strDesc;

    virtual void Init(void) {}
    virtual void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID) {}
    virtual HRESULT LoadFromFile_Item(KLuaManager &luaMgr) { return S_OK; }
};

//************************************************************************
// 기본 클래스 생성
//************************************************************************
class CBaseCondition : public CBaseTGFunction
{
public:
    CBaseCondition() { m_enTGFType = TG_TYPE_CONDITION; m_enType = TG_CONDITION_DEFAULT; m_hItem = NULL; m_bRepeat = false; }
    virtual ~CBaseCondition() {}
    TG_CONDITION        m_enType;

    virtual void CopyConditionToDest(CBaseCondition **pDest) {}
};

class CBaseEventAction : public CBaseTGFunction
{
public:
    CBaseEventAction() { m_enTGFType = TG_TYPE_EACTION; m_enType = TG_EACTION_DEFAULT; m_hItem = NULL; m_bRepeat = false; }
    virtual ~CBaseEventAction() {}

    TG_EACTION          m_enType;

    virtual void CopyEventActionToDest(CBaseEventAction **pDest) {}
};

class CBaseTrigger
{
public:
    CBaseTrigger() { m_bActive = false; m_bUse = true; m_hItem = NULL; }
    virtual ~CBaseTrigger() { ClearTrigger(); }

    bool                        m_bActive;     // 초기 활성화 여부
    bool                        m_bUse;        // 트리거 사용여부
    HTREEITEM                   m_hItem;       // 트리 컨트롤을 위한 식별자
    std::wstring                m_strTGName;
    std::wstring                m_strTGComment;
    std::vector<CBaseEventAction*>   m_vtEvent;
    std::vector<CBaseCondition*>     m_vtCondition;
    std::vector<CBaseEventAction*>   m_vtAction;

    CBaseTrigger &operator=(const CBaseTrigger &rhs);

    virtual void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    virtual HRESULT LoadFromFile(KLuaManager& luaMgr);
    virtual bool IsActive(void) { return m_bActive; }
    virtual bool IsUse(void) { return m_bUse; }
    virtual void AddEvent(CBaseEventAction *pEvent);
    virtual void AddCondition(CBaseCondition *pCondition);
    virtual void AddAction(CBaseEventAction *pAction);
    virtual void ClearTrigger(void);
    virtual void ChangeTGOnOffName(std::wstring strChanged, std::wstring strTGName);

protected:
    virtual void SaveToFile_Event(HANDLE hWrite, int iTab, int &iTG_ID);
    virtual void SaveToFile_Condition(HANDLE hWrite, int iTab, int &iTG_ID);
    virtual void SaveToFile_Action(HANDLE hWrite, int iTab, int &iTG_ID);
};
