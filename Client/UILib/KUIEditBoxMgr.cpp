// Precompile Header
#include "stdafx.h"
#include "KUIEditBoxMgr.h"
//#include "KTextInput.h"
#include "KUIManager.h"

KUIEditBoxMgr* g_pUIEditBoxMgr = NULL;

//---------------------------------------------------------------------------
KUIEditBoxMgr::KUIEditBoxMgr()
: m_iCurFocus(0), m_pkCurEditBox(NULL)
{
	g_pUIEditBoxMgr = this;
}
//---------------------------------------------------------------------------
void KUIEditBoxMgr::RegisterLuabind()
{
    lua_tinker::class_<KUIEditBoxMgr>("KUIEditBoxMgr")
        .def("UpdateEditBoxList", &KUIEditBoxMgr::UpdateEditBoxList)
        .def("ClearBuf", &KUIEditBoxMgr::ClearBuf)
        ;

    lua_tinker::def("GetUIEditBoxMgr", &KUIEditBoxMgr::GetInstance);
}
//---------------------------------------------------------------------------
void KUIEditBoxMgr::ClearBuf()
{
    //_KTextInput()->ClearBuf();
    
    Update();
}
//---------------------------------------------------------------------------
void KUIEditBoxMgr::UpdateEditBoxList()
{
    KUIFrame* pkCurFrame = g_pUIManager->GetCurFrame();
    KApplyDownWidget(pkCurFrame, KComposeList(m_vecEditBox));

    m_iCurFocus = -1;
    NextFocus();
}
//---------------------------------------------------------------------------
KUIEditBox* KUIEditBoxMgr::NextFocus()
{
    if (m_vecEditBox.empty())
        return NULL;

    m_iCurFocus++;
    if (m_vecEditBox.size() <= m_iCurFocus)
        m_iCurFocus = 0;

    m_pkCurEditBox = m_vecEditBox[m_iCurFocus];

	/*
    _KTextInput()->ClearBuf();
    _KTextInput()->SetInput(m_pkCurEditBox->GetBuf(), 
        m_pkCurEditBox->GetBufSize());
	*/

    return m_vecEditBox[m_iCurFocus];
}
//---------------------------------------------------------------------------
KUIEditBox* KUIEditBoxMgr::PrevFocus()
{
    if (m_vecEditBox.empty())
        return NULL;

    m_iCurFocus--;
    if ( (int)m_vecEditBox.size() > m_iCurFocus)
        m_iCurFocus = (int)m_vecEditBox.size() - 1;

    m_pkCurEditBox = m_vecEditBox[m_iCurFocus];

	/*
    _KTextInput()->ClearBuf();
    _KTextInput()->SetInput(m_pkCurEditBox->GetBuf(), 
        m_pkCurEditBox->GetBufSize());    
	*/

    return m_vecEditBox[m_iCurFocus];
}
//---------------------------------------------------------------------------
void KUIEditBoxMgr::UpdateInput()
{
	/*
    NiInputKeyboard* pkKeyboard = _KAppHolder()->GetInputSystem()->GetKeyboard();
    if (pkKeyboard)
    {
        if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_TAB))
        {
            m_pkCurEditBox = NextFocus();
        }
    }
	*/
}
//---------------------------------------------------------------------------
KUIEditBox* KUIEditBoxMgr::GetCurEditBox()
{
    return m_pkCurEditBox;
}
//---------------------------------------------------------------------------
void KUIEditBoxMgr::Update()
{
    if (!m_pkCurEditBox)
        return;

    //m_pkCurEditBox->SetBuf(_KTextInput()->GetInput().c_str());
    //m_pkCurEditBox->SetComp(_KTextInput()->GetComp().c_str());
}
//---------------------------------------------------------------------------
