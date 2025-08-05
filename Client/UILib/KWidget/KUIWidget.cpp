// Precompile Header
#include "stdafx.h"

#include "KUIManager.h"
#include "KUIWidget.h"
#include "KUIFrame.h"
#include "tinyxml.h"

//NiImplementRootRTTI(KUIWidget);

//---------------------------------------------------------------------------
KUIWidget::KUIWidget()
: m_pkParent(NULL), m_strName("Unknown"), m_kOffset(this),
m_bVisible(true), m_kScript(this), m_bInputEnable(true),
m_strExtName("Unknown")
{
}
//---------------------------------------------------------------------------
KUIWidget::~KUIWidget()
{
}
//---------------------------------------------------------------------------
void KUIWidget::RegisterLuabind()
{
    lua_tinker::class_<KUIWidget>("KUIWidget")
        .def("GetName", &KUIWidget::GetName)
        .def("SetName", &KUIWidget::SetName)
        .def("Show", &KUIWidget::Show)
        .def("Hide", &KUIWidget::Hide)
        .def("Update", &KUIWidget::Update)
        .def("GetParent", &KUIWidget::GetParent_)        
        ;
}
//---------------------------------------------------------------------------
void KUIWidget::Load(TiXmlNode* pkNode)
{
    LoadInheritance(pkNode);

    m_kSize.Load(pkNode);
    m_kOffset.Load(pkNode);

    LoadName(pkNode);

    RegisterLuaVariable();

    m_kScript.Load(pkNode);
}
//---------------------------------------------------------------------------
void KUIWidget::RegisterLuaVariable()
{
    //if (m_strName.find("Unknown") != std::string::npos)
    //    return;

    if (!g_pUIManager->GetWidgetByName(GetName()))
        return;

    std::string strCmd = GetName();
    strCmd += " = GetUIManager():GetWidgetByName(\"";
    strCmd += GetName();
    strCmd += "\")";

    /*
    Widget = GetUIManager():GetWidgetByName("name")
    */

    g_kLuabinder.DoString(strCmd.c_str());
}
//---------------------------------------------------------------------------
void KUIWidget::LoadInheritance(TiXmlNode* pkNode)
{
    TiXmlElement* pkWidgetElt = pkNode->ToElement();
    const char* pkParentName = pkWidgetElt->Attribute("inherits");
    if (pkParentName)
    {
        KUIWidget* pkParentWidget = 
            g_pUIManager->GetBackFrame()->GetWidgetByName(pkParentName);
        //KAssert(pkParentWidget);
        CopyMember(pkParentWidget);
    }
}
//---------------------------------------------------------------------------
void KUIWidget::Update()
{
    if (!m_bVisible)
        return;

    m_kScript.Update();
}
//---------------------------------------------------------------------------
void KUIWidget::LoadName(TiXmlNode* pkNode)
{
    TiXmlElement* pkWidgetElt = pkNode->ToElement();
    const char* pcName = pkWidgetElt->Attribute("name");
    if (pcName)
    {
        SetName(pcName);
    }
}
//---------------------------------------------------------------------------
const char* KUIWidget::GetName()
{
    return m_strName.c_str();
}
//---------------------------------------------------------------------------
void KUIWidget::SetName(const char* pcName)
{
    m_strName = pcName;
}
//---------------------------------------------------------------------------
void KUIWidget::Render()
{
}
//---------------------------------------------------------------------------
bool KUIWidget::UpdateInput()
{
    if (!m_bVisible)
        return false;

    m_kScript.Update();

    return false;
}
//---------------------------------------------------------------------------

KUIWidget* KUIWidget::CreateObject()
{
    return new KUIWidget;
}
//---------------------------------------------------------------------------
void KUIWidget::CopyMember(KUIWidget* pkWidget)
{
    // Name
    std::string strName = pkWidget->m_strName;

    while (true)
    {
        KUIWidget* pkWidget = g_pUIManager->GetWidgetByName(strName.c_str());
        if (pkWidget == NULL)
            break;

        static int s_iCnt = 0;
        s_iCnt++;

        //strName += "_C";
        std::ostringstream os;
        os << s_iCnt;
        strName += os.str();
    }
    m_strName = strName;
    RegisterLuaVariable();

    // Offset
    m_kSize = pkWidget->m_kSize;
    m_kOffset = pkWidget->m_kOffset;
    m_kOffset.SetOwner(this);

    // Script
    m_kScript.CopyMember(&pkWidget->m_kScript);
}
//---------------------------------------------------------------------------
KUIWidget* KUIWidget::Clone(KUIFrame* pkParent)
{
    KUIWidget* pkWidget = CreateObject();
    pkParent->AttachWidget(pkWidget);
    pkWidget->CopyMember(this);
    return pkWidget;
}
//---------------------------------------------------------------------------
bool KUIWidget::IsCursorInRect()
{
	/*
    D3DXVECTOR2 kCurPos = _KAppHolder()->GetCursorPos();
    return (GetWorldPos().x < kCurPos.x && kCurPos.x < GetWorldPos().x + m_kSize.GetDim().x &&
        GetWorldPos().y < kCurPos.y && kCurPos.y < GetWorldPos().y + m_kSize.GetDim().y);
	*/
	//TODO
	return false;
}
//---------------------------------------------------------------------------
D3DXVECTOR2 KUIWidget::GetWorldPos()
{
    if (m_pkParent)
    {
        return m_pkParent->GetWorldPos() + GetLocalPos();
    }

    return GetLocalPos();
}
//---------------------------------------------------------------------------
D3DXVECTOR2 KUIWidget::GetLocalPos()
{
    return m_kOffset.GetDim();
}
//---------------------------------------------------------------------------
void KUIWidget::SetLocalPos(D3DXVECTOR2 kPos)
{
    m_kOffset.SetDim(kPos);
}
//---------------------------------------------------------------------------
float KUIWidget::GetArea()
{
    return m_kSize.GetDim().x * m_kSize.GetDim().y;
}
//---------------------------------------------------------------------------
D3DXVECTOR2 KUIWidget::GetSize()
{
    if (m_pkParent)
    {
        if (m_kSize.IsZero())
        {
            return m_pkParent->GetSize();
        }
    }

    return m_kSize.GetDim();
}
//---------------------------------------------------------------------------
void KUIWidget::AttachParent(KUIWidget* pkParent)
{
    m_pkParent = pkParent;
}
//---------------------------------------------------------------------------
void KUIWidget::DetachParent()
{
    m_pkParent = NULL;
}
//---------------------------------------------------------------------------
KUIWidget* KUIWidget::GetParent()
{
    return m_pkParent;
}
//---------------------------------------------------------------------------
lua_tinker::lua_value* KUIWidget::GetParent_()
{
    return m_pkParent;
}
//---------------------------------------------------------------------------
bool KUIWidget::IsBackWidget()
{
    return g_pUIManager->IsBackWidget(this);
}
//---------------------------------------------------------------------------
void KUIWidget::Hide()
{
    m_bVisible = false;
}
//---------------------------------------------------------------------------
void KUIWidget::Show()
{
    m_bVisible = true;
}
//---------------------------------------------------------------------------
bool KUIWidget::IsVisible()
{
    bool bVisible = true;
    KApplyUpWidget(this, KIsVisibleWidget(bVisible));

    return bVisible;
}
//---------------------------------------------------------------------------
void KUIWidget::LockInput()
{
    m_bInputEnable = false;
}
//---------------------------------------------------------------------------
void KUIWidget::UnlockInput()
{
    m_bInputEnable = true;
}
//---------------------------------------------------------------------------
bool KUIWidget::IsInputEnable()
{
    return m_bInputEnable;
}
//---------------------------------------------------------------------------
void KUIWidget::SetExtName(const char* pcName)
{
    m_strExtName = pcName;
}
//---------------------------------------------------------------------------
const char* KUIWidget::GetExtName()
{
    return m_strExtName.c_str();
}
//---------------------------------------------------------------------------
