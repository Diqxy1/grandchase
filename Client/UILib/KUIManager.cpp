// Precompile Header
#include "stdafx.h"

#include "KUIManager.h"
//#include "KLuabinder.h"
#include "KUIManager.h"
#include "KUIFontString.h"
#include "KUIEditBoxMgr.h"
//#include "KFontMgr.h"
//#include "KTextInput.h"
#include "tinyxml.h"

KUIManager* g_pUIManager = NULL;
KLuabinder g_kLuabinder;
//---------------------------------------------------------------------------
KUIManager::KUIManager()
{
	g_pUIManager = this;
}
//---------------------------------------------------------------------------
KUIManager::~KUIManager()
{
	g_pUIManager = NULL;
}
//---------------------------------------------------------------------------
void KUIManager::RegisterLuabind()
{
    lua_tinker::class_<KUIManager>("KUIManager")
        .def("GetWidgetByName", &KUIManager::GetWidgetByName_)
        ;

    lua_tinker::def("UIManager", &KUIManager::GetInstance);
}
//---------------------------------------------------------------------------
void KUIManager::Load(const char* strFilename)
{
    TiXmlDocument kDoc(strFilename);
    bool bSuccess = kDoc.LoadFile();
    //KAssert(bSuccess);

    TiXmlNode* pkUINode = kDoc.FirstChild("UI");
    if (!pkUINode)
        return;

    //m_strXMLFilename = strFilename;

    TiXmlNode* pkScript = pkUINode->FirstChild("Script");
    if (pkScript)
    {
        TiXmlElement* pkElement = pkScript->ToElement();
        std::string strLuaFilename = pkElement->Attribute("file");
        //m_strScriptFilename = strLuaFilename;
        g_kLuabinder.DoFile(strLuaFilename.c_str());
    }

    LoadBackWidgets(pkUINode);

    TiXmlNode* pkFrameNode = pkUINode->FirstChild("Frame");
    if (pkFrameNode)
    {
        const char* pcVirtual = 
            pkFrameNode->ToElement()->Attribute("virtual");
        if (pcVirtual && !strcmp(pcVirtual, "true"))
        {
            KUIWidget::Creates<KUIFrame>(m_spBackFrame, pkUINode, "Frame");
        }
        else
        {
            KUIFrame* pkLoadFrame = new KUIFrame;
            m_spCurFrame->AttachWidget(pkLoadFrame);

            pkLoadFrame->Load(pkFrameNode);
        }
    }
}
//---------------------------------------------------------------------------
void KUIManager::LoadBackWidgets(TiXmlNode* pkNode)
{
    m_spBackFrame->CreateWidgets(pkNode);
}
//---------------------------------------------------------------------------
void KUIManager::AddXMLFile(const char* strFilename)
{
    m_vecXMLFilename.push_back(strFilename);
}
//---------------------------------------------------------------------------
void KUIManager::ClearXMLFile()
{
    m_vecXMLFilename.clear();
}
//---------------------------------------------------------------------------
void KUIManager::Load()
{
    //_KFontMgr()->RemoveAllFonts();
    InitFrame();

    //_KTextInput()->RemoveAllWidgets();

    unsigned int iF;
    for (iF = 0; iF < m_vecXMLFilename.size(); ++iF)
    {
        Load(m_vecXMLFilename[iF].c_str());
    }

    g_pUIEditBoxMgr->UpdateEditBoxList();
}
//---------------------------------------------------------------------------
//void KUIManager::Save(const char* strFilename)
//{
//    TiXmlDocument kDoc(strFilename);
//    kDoc.Parse("<?xml version='1.0' standalone='no'?>");
//
//    TiXmlElement kUIRootElt("UI");
//    TiXmlElement kScriptElt("Script");
//    kScriptElt.SetAttribute("file", m_strScriptFilename.c_str());
//
//    m_spCurFrame->Save(&kUIRootElt);
//
//    kDoc.InsertEndChild(kScriptElt);
//    kDoc.InsertEndChild(kUIRootElt);
//
//    kDoc.SaveFile();
//}
////---------------------------------------------------------------------------
void KUIManager::Render()
{
    if (m_spCurFrame)
        m_spCurFrame->Render();
}
//---------------------------------------------------------------------------
void KUIManager::Update()
{
    if (m_spCurFrame)
        m_spCurFrame->Update();
}
//---------------------------------------------------------------------------
void KUIManager::UpdateInput()
{
    if (m_spCurFrame)
        m_spCurFrame->UpdateInput();
}
//---------------------------------------------------------------------------
void KUIManager::InitFrame()
{
    m_spCurFrame = new KUIFrame;
    m_spCurFrame->SetName("RootFrame");

    m_spBackFrame = new KUIFrame;
    m_spBackFrame->SetName("BackFrame");
}
//---------------------------------------------------------------------------
void KUIManager::SetCurFrame(KUIFrame* pkUIFrame)
{
    m_spCurFrame = pkUIFrame;
}
//---------------------------------------------------------------------------
KUIFrame* KUIManager::GetCurFrame()
{
    return m_spCurFrame;
}
//---------------------------------------------------------------------------
KUIFrame* KUIManager::GetBackFrame()
{
    return m_spBackFrame;
}
//---------------------------------------------------------------------------
bool KUIManager::IsBackWidget(KUIWidget* pkWidget)
{
    bool bIsIncluded = false;
    KApplyDownWidget(m_spBackFrame, 
        KUIFrame::KIsIncludedFrame(pkWidget, bIsIncluded));

    return bIsIncluded;
}
//---------------------------------------------------------------------------
KUIWidget* KUIManager::GetWidgetByName(const char* pcName)
{
    KUIWidget* pkWidget = NULL;
    pkWidget = m_spBackFrame->GetWidgetByName(pcName);

    if (!pkWidget)
        pkWidget = m_spCurFrame->GetWidgetByName(pcName);

    return pkWidget;
}
//---------------------------------------------------------------------------
lua_tinker::lua_value* KUIManager::GetWidgetByName_(const char* pcName)
{
    return GetWidgetByName(pcName);
}
//---------------------------------------------------------------------------
