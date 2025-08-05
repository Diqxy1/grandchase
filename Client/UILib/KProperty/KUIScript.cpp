// Precompile Header
#include "stdafx.h"

#include "KUIScript.h"
//#include "KLuabinder.h"
#include "KUIWidget.h"
#include "tinyxml.h"

/*
    {
        TiXmlElement* pkEventElt = pkNode->FirstChildElement("OnLoad");
        if (pkEventElt)
        {
            std::string strBuf = pkEventElt->GetText();
            m_vecEvents[eOnLoad] = strBuf;
        }
    }
*/

#define KLoadEvent(mEvent) \
{ \
    TiXmlElement* pkEventElt = pkNode->FirstChildElement(#mEvent); \
    if (pkEventElt) \
    { \
        std::string strBuf = pkEventElt->GetText(); \
        m_vecEvents[e##mEvent] = strBuf; \
    } \
}

//---------------------------------------------------------------------------
KUIScript::KUIScript(KUIWidget* pkWidget)
: m_pkOwner(pkWidget)
{
    m_vecEvents.reserve(eEventTypeMax);
    m_vecEvents.resize(eEventTypeMax);
}
//---------------------------------------------------------------------------
void KUIScript::Load(TiXmlNode* pkNode)
{
    TiXmlNode* pkScriptNode = pkNode->FirstChild("Script");

    if (pkScriptNode)
        LoadEvent(pkScriptNode);

    Execute(eOnLoad);
}
//---------------------------------------------------------------------------
void KUIScript::LoadEvent(TiXmlNode* pkNode)
{
    KLoadEvent(OnLoad);
    KLoadEvent(OnUpdate);
    KLoadEvent(OnBtnDown);    
    KLoadEvent(OnKeyEnter);
}
//---------------------------------------------------------------------------
void KUIScript::Update()
{
    Execute(eOnUpdate);
}
//---------------------------------------------------------------------------
void KUIScript::UpdateInput()
{
}
//---------------------------------------------------------------------------
void KUIScript::CopyMember(KUIScript* pkScript)
{
    std::copy(pkScript->m_vecEvents.begin(), pkScript->m_vecEvents.end(),
        m_vecEvents.begin());
}
//---------------------------------------------------------------------------
void KUIScript::Execute(int iEventType)
{
    if (m_vecEvents[iEventType].empty())
        return;

    std::string strCmd = "this = ";
    strCmd += m_pkOwner->GetName();
    g_kLuabinder.DoString(strCmd.c_str());

    strCmd = m_vecEvents[iEventType];
    g_kLuabinder.DoString(strCmd.c_str());
}
//---------------------------------------------------------------------------
