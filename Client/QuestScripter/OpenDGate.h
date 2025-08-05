#pragma once
#include "BaseTrigger.h"

class COpenDGate : public CBaseEventAction
{
public:
    COpenDGate(void);
    ~COpenDGate(void);

    int m_iDirection;                           // 0 : left, 1 : top, 2 : right, 3 : bottom
    int ConvertToImageNumber(int iGateNum);     // 0, 1, 2, 3 의 방향을 방향 이미지 파일 번호로 변경
    int ConvertToComboBoxNumber(int iGateNum);  // 파일명 번호에서 0, 1, 2, 3의 콤보박스 번호로 변경

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
