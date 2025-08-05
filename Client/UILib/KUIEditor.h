#pragma once

#include "KUIFrame.h"

//singletone
class KUIEditor
{

public:
    KUIEditor();

    void UpdateInput();

protected:
    KUIWidget* m_pkSelWidget;
    //NiPoint2 m_kPrevPos;
};

extern KUIEditor* g_pUIEditor;

#include "KUIEditor.inl"