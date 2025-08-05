// Precompile Header
#include "stdafx.h"

#include "KUIEditBox.h"
#include "KUIFrame.h"
//#include "KTextInput.h"
#include "KUIEditBoxMgr.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIEditBox, KUIFontString);

//---------------------------------------------------------------------------
KUIEditBox::KUIEditBox()
{
}
//---------------------------------------------------------------------------
void KUIEditBox::RegisterLuabind()
{
    lua_tinker::class_<KUIEditBox>("KUIEditBox")
        .inh<KUIFontString>()
        .def("ClearBuf", &KUIEditBox::ClearBuf)
        .def("GetBuf", &KUIEditBox::GetBuf)        
        ;
}
//---------------------------------------------------------------------------
void KUIEditBox::Load(TiXmlNode* pkNode)
{
    KUIFontString::Load(pkNode);

    m_strBuf.clear();

    int iLetters = 0;
    TiXmlElement* pkEBElt = pkNode->ToElement();
    pkEBElt->Attribute("letters", &iLetters);
    if (iLetters)
        m_iLetters = iLetters;
}
//---------------------------------------------------------------------------
void KUIEditBox::Update()
{
    KUIFrame::Update();

    UpdateCursor();

	/*
    if (m_spString2D)
    {
        m_spString2D->SetPos(GetWorldPos());
        m_spString2D->OStream() << m_strBuf << m_strComp << m_cCursor;
    } 
	*/
}
//---------------------------------------------------------------------------
KUIWidget* KUIEditBox::CreateObject()
{
    return new KUIEditBox;
}
//---------------------------------------------------------------------------
void KUIEditBox::CopyMember(KUIWidget* pkWidget)
{
    KUIFontString::CopyMember(pkWidget);

    KUIEditBox* pkEditbox = (KUIEditBox*) pkWidget;

    m_iLetters = pkEditbox->m_iLetters;

    m_strBuf.clear();
}
//---------------------------------------------------------------------------
bool KUIEditBox::UpdateInput()
{
	/*
    NiInputKeyboard* pkKeyboard = _KAppHolder()->GetInputSystem()->GetKeyboard();
    if (pkKeyboard)
    {
        if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_RETURN))
        {
            m_kScript.Execute(KUIScript::eOnKeyEnter);
        }
    }
	*/

    return false;
}
//---------------------------------------------------------------------------
void KUIEditBox::SetBuf(const char* pcText)
{
    m_strBuf = pcText;
}
//---------------------------------------------------------------------------
void KUIEditBox::SetComp(const char* pcText)
{
    m_strComp = pcText;
}
//---------------------------------------------------------------------------
void KUIEditBox::ClearBuf()
{
    m_strBuf.clear();
}
//---------------------------------------------------------------------------
int KUIEditBox::GetBufSize()
{
    return (int)m_strBuf.size();
}
//---------------------------------------------------------------------------
const char* KUIEditBox::GetBuf()
{
    return m_strBuf.c_str();
}
//---------------------------------------------------------------------------
void KUIEditBox::UpdateCursor()
{
    if ( g_pUIEditBoxMgr->GetCurEditBox() != this)
    {
        m_cCursor = ' ';
        return;
    }

    // 임시 코드 - 스탑와치 추가시 변경
    m_cCursor = '|';
    static int s_iFrameCnt = 0;

    if (s_iFrameCnt++ > 100)
        s_iFrameCnt = 0;

    if (s_iFrameCnt < 50)
        m_cCursor = ' ';
}
//---------------------------------------------------------------------------