#include "stdafx.h"
#include "BaseTrigger.h"
#include "TriggerOnOff.h"

void CBaseTrigger::AddEvent(CBaseEventAction *pEvent)
{
    m_vtEvent.push_back(pEvent);
}

void CBaseTrigger::AddCondition(CBaseCondition *pCondition)
{
    m_vtCondition.push_back(pCondition);
}

void CBaseTrigger::AddAction(CBaseEventAction *pAction)
{
    m_vtAction.push_back(pAction);
}

void CBaseTrigger::ClearTrigger(void)
{
    for(int i=0;i<(int)m_vtEvent.size();++i)
        SAFE_DELETE(m_vtEvent[i]);
    m_vtEvent.clear();

    for(int i=0;i<(int)m_vtCondition.size();++i)
        SAFE_DELETE(m_vtCondition[i]);
    m_vtCondition.clear();

    for(int i=0;i<(int)m_vtAction.size();++i)
        SAFE_DELETE(m_vtAction[i]);
    m_vtAction.clear();
}

CBaseTrigger &CBaseTrigger::operator=(const CBaseTrigger &rhs)
{
    this->m_bActive = rhs.m_bActive;
    this->m_bUse = rhs.m_bUse;        // 트리거 사용여부
    this->m_hItem = rhs.m_hItem;        // 트리 컨트롤을 위한 식별자
    this->m_strTGName = rhs.m_strTGName;
    this->m_strTGComment = rhs.m_strTGComment;

    ClearTrigger();
    for(int i=0;i<(int)rhs.m_vtEvent.size();++i)
    {
        CBaseEventAction *pEvent = NULL;
        rhs.m_vtEvent[i]->CopyEventActionToDest(&pEvent);
        if( pEvent ) this->m_vtEvent.push_back(pEvent);
    }
    for(int i=0;i<(int)rhs.m_vtCondition.size();++i)
    {
        CBaseCondition *pCondition = NULL;
        rhs.m_vtCondition[i]->CopyConditionToDest(&pCondition);
        if( pCondition ) m_vtCondition.push_back(pCondition);
    }
    for(int i=0;i<(int)rhs.m_vtAction.size();++i)
    {
        CBaseEventAction *pAction = NULL;
        rhs.m_vtAction[i]->CopyEventActionToDest(&pAction);
        if( pAction ) this->m_vtAction.push_back(pAction);
    }

    return *this;
}

HRESULT CBaseTrigger::LoadFromFile(KLuaManager& luaMgr)
{
    std::string strTemp;

    LUA_GET_VALUE("TriggerName", strTemp, return E_FAIL);    
    m_strTGName = GCUTIL_STR::GCStrCharToWide( strTemp.c_str() );

    LUA_GET_VALUE("TriggerComment", strTemp, return E_FAIL);
    m_strTGComment = GCUTIL_STR::GCStrCharToWide( strTemp.c_str() );

    LUA_GET_VALUE("Active", m_bActive, return E_FAIL);
    LUA_GET_VALUE("Use", m_bUse, return E_FAIL);
    return S_OK;
}

void CBaseTrigger::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Trigger%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "TriggerName", m_strTGName, iTab+1);
    WriteToFileScript(hWrite, "TriggerComment", m_strTGComment, iTab+1);
    WriteToFileScript(hWrite, "Active", m_bActive, iTab+1);
    WriteToFileScript(hWrite, "Use", m_bUse, iTab+1);
    SaveToFile_Event(hWrite, iTab+1, iTG_ID);
    SaveToFile_Condition(hWrite, iTab+1, iTG_ID);
    SaveToFile_Action(hWrite, iTab+1, iTG_ID);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CBaseTrigger::SaveToFile_Event(HANDLE hWrite, int iTab, int &iTG_ID)
{
    WriteOpenBrace(hWrite, "Event", iTab);
    for(int i=0;i<(int)m_vtEvent.size();++i)
    {
        m_vtEvent[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
    }
    WriteCloseBrace(hWrite, iTab, 0);
}

void CBaseTrigger::SaveToFile_Condition(HANDLE hWrite, int iTab, int &iTG_ID)
{
    WriteOpenBrace(hWrite, "Condition", iTab);
    for(int i=0;i<(int)m_vtCondition.size();++i)
    {
        m_vtCondition[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
    }
    WriteCloseBrace(hWrite, iTab, 0);
}

void CBaseTrigger::SaveToFile_Action(HANDLE hWrite, int iTab, int &iTG_ID)
{
    WriteOpenBrace(hWrite, "Action", iTab);
    for(int i=0;i<(int)m_vtAction.size();++i)
    {
        m_vtAction[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
    }
    WriteCloseBrace(hWrite, iTab, 0);
}

void CBaseTrigger::ChangeTGOnOffName(std::wstring strChanged, std::wstring strTGName)
{
    for(int i=0;i<(int)m_vtEvent.size();++i)
    {
        if( TG_EACTION_TRIGGERONOFF == m_vtEvent[i]->m_enType )
        {
            CString strTriggerName = ((CTriggerOnOff*)m_vtEvent[i])->m_szTriggerName;
            if( !wcsicmp(((CTriggerOnOff*)m_vtEvent[i])->m_szTriggerName, strTGName.c_str()) )
            {
                CTriggerOnOff *pTGOnOff = ((CTriggerOnOff*)m_vtEvent[i]);
                CString strDesc = pTGOnOff->m_strDesc.c_str();
                strDesc.Replace(strTGName.c_str(), strChanged.c_str());
                pTGOnOff->m_strDesc = strDesc.GetBuffer();
                
                StringCchCopyN( pTGOnOff->m_szTriggerName, sizeof( pTGOnOff->m_szTriggerName ),
                                strChanged.c_str(), strChanged.length() );
                
            }
        }
    }

    for(int i=0;i<(int)m_vtAction.size();++i)
    {
        if( TG_EACTION_TRIGGERONOFF == m_vtAction[i]->m_enType )
        {
            CString strTriggerName = ((CTriggerOnOff*)m_vtAction[i])->m_szTriggerName;
            if( !wcsicmp(((CTriggerOnOff*)m_vtAction[i])->m_szTriggerName, strTGName.c_str()) )
            {
                CTriggerOnOff *pTGOnOff = ((CTriggerOnOff*)m_vtAction[i]);
                CString strDesc = pTGOnOff->m_strDesc.c_str();
                strDesc.Replace(strTGName.c_str(), strChanged.c_str());
                pTGOnOff->m_strDesc = strDesc.GetBuffer();

                StringCchCopyN( ((CTriggerOnOff*)m_vtAction[i])->m_szTriggerName, sizeof(((CTriggerOnOff*)m_vtAction[i])->m_szTriggerName),
                    strChanged.c_str(), strChanged.length() );
            }
        }
    }
}