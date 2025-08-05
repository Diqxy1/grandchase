#include "StdAfx.h"
#include ".\opendgate.h"
#include "struct.h"

COpenDGate::COpenDGate(void)
{
    Init();
}

COpenDGate::~COpenDGate(void)
{
}

void COpenDGate::Init(void)
{
    m_enType            = TG_EACTION_OPEN_DGATE;
    m_iDirection        = 0;
}

void COpenDGate::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGOpenDGate", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Direction", m_iDirection, iTab+1);    
    WriteCloseBrace(hWrite, iTab, 0);
}

void COpenDGate::CopyEventActionToDest(CBaseEventAction **pDest)
{
    COpenDGate *pOpenDGate = new COpenDGate;
    *pOpenDGate = *(COpenDGate*)this;
    *pDest = (CBaseEventAction*)pOpenDGate;
}

HRESULT COpenDGate::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Direction", m_iDirection, return E_FAIL);

    return S_OK;
}

// 이미지 넘버를 넣으면 콤보박스 번호로 리턴해준다.
int COpenDGate::ConvertToComboBoxNumber(int iGateNum)
{
    switch(iGateNum)
    {
    case DIRECTION_LEFT:             // left
        return 0;
        break;
    case DIRECTION_UP:                // top
        return 1;
        break;
    case DIRECTION_RIGHT:             // right
        return 2;
        break;
    case DIRECTION_DOWN:             // bottom
        return 3;
        break;
    }

    return 0;
}

// 콤보박스의 0, 1, 2, 3을 기준으로 left right up down 값을 판단하여 리턴
int COpenDGate::ConvertToImageNumber(int iGateNum)
{
    switch(iGateNum)
    {
        case 0:             // left
            return DIRECTION_LEFT;
            break;
        case 1:             // top
            return DIRECTION_UP;
            break;
        case 2:             // right
            return DIRECTION_RIGHT;
            break;
        case 3:             // bottom
            return DIRECTION_DOWN;
            break;
    }

    return 0;
}