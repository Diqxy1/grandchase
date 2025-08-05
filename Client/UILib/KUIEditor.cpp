// Precompile Header
#include "stdafx.h"
#include "KUIEditor.h"
#include "KUIManager.h"


KUIEditor* g_pUIEditor = NULL;

//---------------------------------------------------------------------------
KUIEditor::KUIEditor()
: m_pkSelWidget(NULL)
{
}
//---------------------------------------------------------------------------
void KUIEditor::UpdateInput()
{
	/*
    NiInputMouse* pkMouse = _KAppHolder()->GetInputSystem()->GetMouse();
    if (pkMouse)
    {
        if (pkMouse->ButtonWasPressed(NiInputMouse::NIM_LEFT))
        {
            KUIFrame* pkUIFrame = g_pUIManager->GetCurFrame();
            KUIWidget* pkInWidget = NULL;
            KApplyDownWidget(pkUIFrame, KIsCursorInRect(pkInWidget));

            if (pkInWidget)
            {
                m_pkSelWidget = pkInWidget;
                m_kPrevPos = _KAppHolder()->GetCursorPos();
            }
        }

        if (pkMouse->ButtonIsDown(NiInputMouse::NIM_LEFT))
        {
            if (m_pkSelWidget)
            {
                NiPoint2 kCurPos = _KAppHolder()->GetCursorPos();
                NiPoint2 kDelta = kCurPos - m_kPrevPos;
                NiPoint2 kResPos = m_pkSelWidget->GetPos() + kDelta;

                if (kResPos.x < 0)
                    kResPos.x = 0;
                if (kResPos.y < 0)
                    kResPos.y = 0;
                m_pkSelWidget->Move(kResPos);
                m_kPrevPos = kCurPos;
            }
        }
    }
	*/
}
//---------------------------------------------------------------------------
