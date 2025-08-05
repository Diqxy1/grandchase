#pragma once

#include "KUIEditBox.h"

class KUIEditBoxMgr;
extern KUIEditBoxMgr* g_pUIEditBoxMgr;

//singletone
class KUIEditBoxMgr
{

public:
    KUIEditBoxMgr();

    static void RegisterLuabind();
	static KUIEditBoxMgr* GetInstance() { return g_pUIEditBoxMgr; }

    void Update();
    void UpdateInput();
    void UpdateEditBoxList();
    void ClearBuf();

    KUIEditBox* NextFocus();
    KUIEditBox* PrevFocus();
    KUIEditBox* GetCurEditBox();

protected:
    int m_iCurFocus;
    KUIEditBox* m_pkCurEditBox;
   
    std::vector<KUIEditBox*> m_vecEditBox;

public:
    class KComposeList;
};

#include "KUIEditBoxMgr.inl"