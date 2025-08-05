#include "stdafx.h"
#include ".\GCSquareSayBalloon.h"
//
//
//
//

#include "../dxutil.h"

IMPLEMENT_CLASSNAME(KGCSayBalloonSquare);
IMPLEMENT_WND_FACTORY(KGCSayBalloonSquare);
IMPLEMENT_WND_FACTORY_NAME(KGCSayBalloonSquare, "gc_say_balloon_square");

#ifdef SAY_BOX_ALPHA
#undef SAY_BOX_ALPHA
#endif
#define SAY_BOX_ALPHA  255

#ifdef GAP_INCURSIUON
#undef GAP_INCURSIUON
#endif
#define GAP_INCURSIUON   2

#ifdef GAP_BORDER
#undef GAP_BORDER
#endif
#define GAP_BORDER      10


KGCSayBalloonSquare::KGCSayBalloonSquare(void)
{
    m_vecpkMessageBalloon.reserve(MAX_BALLOON_COUNT);
    m_vecpkSayMsg.reserve(MAX_BALLOON_COUNT);
    m_vecfElapsedTime.reserve(MAX_BALLOON_COUNT);
    m_vecdwUserUID.reserve(MAX_BALLOON_COUNT);
    m_vecdwNPCUID.reserve(MAX_BALLOON_COUNT);
    m_veceChatType.reserve(MAX_BALLOON_COUNT);
    m_vecdwColor.reserve(MAX_BALLOON_COUNT);
}

KGCSayBalloonSquare::~KGCSayBalloonSquare(void)
{


}

void KGCSayBalloonSquare::DeleteAllBalloon()
{
    //오브젝트 다 지웁니다.

    for (int i = m_iUseBalloonCount - 1; i >= 0; i--)
    {
        DisableBalloon(i);
    }

    // 켜져 있는 것들이 있을수 있으니 다 끈다.
    for (int i = 0; i < m_iBalloonCount; i++)
    {
        m_vecpkMessageBalloon[i]->InitState(false, false, NULL);
        m_vecpkSayMsg[i]->InitState(false, false, NULL);
    }

    RemoveAllChild();
    m_iBalloonCount = 0;
    m_iUseBalloonCount = 0;
    m_vecpkMessageBalloon.clear();
    m_vecpkSayMsg.clear();
    m_vecfElapsedTime.clear();
    m_vecdwUserUID.clear();
    m_vecdwNPCUID.clear();
}

void KGCSayBalloonSquare::OnCreate()
{
    m_iBalloonCount = 0;
    m_iUseBalloonCount = 0;
    m_iOtherBalloonIndex = -1;
    m_mapOffset.clear();
    m_mapDist.clear();
}


void KGCSayBalloonSquare::AddBallonControl(int iIndex, int iballoontype /*= 0*/)
{
    //   KGCLuaManager LuaMgr;
    //   if( GCFUNC::LoadLuaScript( LuaMgr , "BalloonBoxInfo.lua" ) == false )
    //   {
    //       ASSERT("BalloonBoxInfo.lua Load Failed!");
    //       exit( 0 );
    //   }

    //   LuaMgr.BeginTable( "BalloonBoxInfo" );
    //   m_vecpkMessageBalloon.push_back(new KD3DBalloonBox);
    //   char strUIName[20];
    //   sprintf(strUIName,"say_box%d",iIndex);
    //   g_pkUIMgr->LoadUIWindowTable(&LuaMgr, m_vecpkMessageBalloon[iIndex], this,strUIName);
       //LuaMgr.EndTable();

    char strUIName[20];
    sprintf(strUIName, "say_box%d", iIndex);
    KD3DBalloonBox* pkBalloonBox;

    switch (iballoontype) //말풍선 종류에 따라 나눔, CreateTemplateUI의 세번째인자에 말풍선 스크립트를 넣습니다.
    {
    case 0://기본 말풍선
        pkBalloonBox = (KD3DBalloonBox*)g_pkUIMgr->CreateTemplateUI(this, "gc_balloon_box", "ui_balloonboxinfo.stg", strUIName, true, true);
        break;
    case 1:// 
        pkBalloonBox = (KD3DBalloonBox*)g_pkUIMgr->CreateTemplateUI(this, "gc_balloon_box", "ui_balloonboxinfo2.stg", strUIName, true, true);
        if (m_iOtherBalloonIndex == -1)
            m_iOtherBalloonIndex = iIndex;
        break;
    }

    if (pkBalloonBox == NULL)
    {
        ASSERT("Something Error in CreateTemplateUI");
        return;
    }

    m_vecpkMessageBalloon.push_back(pkBalloonBox);

    m_vecpkMessageBalloon[iIndex]->SetAllWindowColor(D3DCOLOR_ARGB(SAY_BOX_ALPHA, 0xff, 0xff, 0xff));
    m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(true);
    m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(false);
    m_vecpkMessageBalloon[iIndex]->SetWndState(D3DWS_ENABLED);

    m_vecpkSayMsg.push_back(new KD3DStatic);
    sprintf(strUIName, "say_what%d", iIndex);
    m_vecpkSayMsg[iIndex]->SetWindowName(strUIName);
    AddChildWithRegistToLua(m_vecpkSayMsg[iIndex]);
    m_vecpkSayMsg[iIndex]->SetWndState(D3DWS_ENABLED);

    m_vecpkSayMsg[iIndex]->SetAlign(DT_CENTER);
    m_vecpkSayMsg[iIndex]->SetSelfInputCheck(true);
    m_vecpkSayMsg[iIndex]->AddActionListener(NULL);
    m_vecpkSayMsg[iIndex]->SetText(L"");
    m_vecpkSayMsg[iIndex]->SetMultiLine(true);
    m_vecpkSayMsg[iIndex]->SetLineSpace(1.2f);
    m_vecpkSayMsg[iIndex]->SetFontColor(D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
    m_vecpkSayMsg[iIndex]->ToggleRender(true);
    m_vecpkMessageBalloon[iIndex]->InitState(true, true, this);
    m_iBalloonCount++;
    m_vecfElapsedTime.push_back(NULL);
    m_vecdwUserUID.push_back(NULL);
    m_vecdwNPCUID.push_back(NULL);
    m_veceChatType.push_back(KChatData::MSG_COMMON);
    m_vecdwColor.push_back(0xffffffff);
}

void KGCSayBalloonSquare::ActionPerformed(const KActionEvent& event)
{

}

void KGCSayBalloonSquare::MoveBalloonInfo(int iSIndex, int iTIndex)
{
    if (iSIndex<0 || iTIndex <0 || iSIndex > m_iBalloonCount || iTIndex > m_iBalloonCount || iSIndex == iTIndex)
    {
        ASSERT("Balloon Object Control Error");
    }

    m_vecpkMessageBalloon[iTIndex]->SetArrowType(m_vecpkMessageBalloon[iSIndex]->GetArrowType());
    SetWidthHeightFromMsgSize(m_vecpkSayMsg[iSIndex]->GetOriginalText(), iTIndex); // 메시지 복사하고 리사이징
    m_veceChatType[iTIndex] = m_veceChatType[iSIndex];
    m_vecdwColor[iTIndex] = m_vecdwColor[iSIndex];
    SetMsgColor(iTIndex, m_veceChatType[iTIndex]);  //색깔 복사
    m_vecdwUserUID[iTIndex] = m_vecdwUserUID[iSIndex];                    // 소속 풍선의 유저님 UID 복사 함
    m_vecdwNPCUID[iTIndex] = m_vecdwNPCUID[iSIndex];
    m_vecfElapsedTime[iTIndex] = m_vecfElapsedTime[iSIndex];              // 소속 풍선님 생성 시간도 복사 함

}

#define BALLOON_DISPLAY_TIME          0.1f
#define BALLOON_DISPLAY_TIME_BASE    10.0f

void KGCSayBalloonSquare::TimeCheck()
{
    //    return;

    double dwNowTime = DXUtil_Timer(TIMER_GETAPPTIME);

    if (m_iUseBalloonCount == 0) //풍선하나두 없음 시간이고 나발이고 그냥 나가라
        return;

    for (int i = 0; i < m_iUseBalloonCount; i++)
    {
        if (m_vecfElapsedTime[i] < dwNowTime)
        {
            DisableBalloon(i);
        }
    }
    return;

}


void KGCSayBalloonSquare::DisableBalloon(int iIndex)
{
    m_iUseBalloonCount--;
    for (int i = iIndex; i < m_iUseBalloonCount; i++)
    {
        MoveBalloonInfo(i + 1, i);
    }

    m_vecpkMessageBalloon[m_iUseBalloonCount]->DeleteArrow();
    m_vecpkMessageBalloon[m_iUseBalloonCount]->InitState(false, false, NULL);
    m_vecpkSayMsg[m_iUseBalloonCount]->InitState(false, false, NULL);
}

void KGCSayBalloonSquare::DeleteUserMsg(DWORD dwCharUID)
{
    for (int i = 0; i < m_iUseBalloonCount; i++)
    {
        if (m_vecdwUserUID[i] == dwCharUID)
        { //풍선 떠 있는넘이 또 말씀하신 경우
            DisableBalloon(i);//기존 풍선 삭제
            break;
        }
    }
}

void KGCSayBalloonSquare::DeleteNPCMsg(DWORD dwNPCUID)
{
    for (int i = 0; i < m_iUseBalloonCount; i++)
    {
        if (m_vecdwNPCUID[i] == dwNPCUID)
        { //풍선 떠 있는넘이 또 말씀하신 경우
            DisableBalloon(i);//기존 풍선 삭제
            break;
        }
    }
}

void KGCSayBalloonSquare::NewUserMsg(std::wstring strMsg, DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor)
{
    if (strMsg.empty() == true) return; //공백 문자 빼고나니까 문자 없는 경우..

    //KChatData::MSG_COMMON
    DeleteUserMsg(dwCharUID); //기존 풍선이 있는 경우 찾아서 지움

    GCSquarePeople* pPerson = SiGCSquare()->GetSquarePeople(dwCharUID);
    KCoordiSetTemplet kCoordiSetTemplet;
    bool bIsEquipEnchantSetItem = false;
    if (pPerson)
    {
        bIsEquipEnchantSetItem = g_pItemMgr->GetEquipCoordiSetItem(kCoordiSetTemplet, pPerson->GetPeopleInfo());
    }

    if (m_iBalloonCount < m_iUseBalloonCount)
    {
        ASSERT("사용중인 풍선 오브젝트가 생성중인 수보다 많은 어처구니 없는 상황");
    }
    else if (m_iBalloonCount == m_iUseBalloonCount)//만들어진 수랑 사용하고 있는 수가 같은 경우, 메시지 오브젝트를 추가하고 거기에 메시지를 넣어야 한다.
    {
        if (bIsEquipEnchantSetItem && kCoordiSetTemplet.bAddEffect)
            AddBallonControl(m_iBalloonCount, 1);
        else
            AddBallonControl(m_iBalloonCount, 0);

        SetMsg(strMsg, m_iUseBalloonCount, dwCharUID, eMsgType, dwChatColor);
    }
    else //남아 있는 오브젝트가 있는 경우.
    {
        if (bIsEquipEnchantSetItem && kCoordiSetTemplet.bAddEffect)
        {
            SetMsg(strMsg, m_iOtherBalloonIndex, dwCharUID, eMsgType, dwChatColor);
        }
        else
        {
            int iIndex;
            if (m_iOtherBalloonIndex == m_iUseBalloonCount)
            {
                if (m_iUseBalloonCount - 1 >= 0)
                    iIndex = m_iUseBalloonCount - 1;
                else
                    iIndex = 0;
            }
            else
                iIndex = m_iUseBalloonCount;

            SetMsg(strMsg, iIndex, dwCharUID, eMsgType, dwChatColor);
        }
    }

    // P.S 최적 위치 생성 루틴 추가 필요
}

void KGCSayBalloonSquare::NewNPCMsg(std::wstring strMsg, DWORD dwUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor)
{
    if (strMsg.empty() == true) return; //공백 문자 빼고나니까 문자 없는 경우..

    //KChatData::MSG_COMMON
    DeleteNPCMsg(dwUID); //기존 풍선이 있는 경우 찾아서 지움

    if (m_iBalloonCount < m_iUseBalloonCount)
    {
        ASSERT("사용중인 풍선 오브젝트가 생성중인 수보다 많은 어처구니 없는 상황");
    }
    else if (m_iBalloonCount == m_iUseBalloonCount)//만들어진 수랑 사용하고 있는 수가 같은 경우, 메시지 오브젝트를 추가하고 거기에 메시지를 넣어야 한다.
    {
        AddBallonControl(m_iBalloonCount);
        SetMsg(strMsg, m_iUseBalloonCount, dwUID, eMsgType, dwChatColor);
    }
    else //남아 있는 오브젝트가 있는 경우.
    {
        int iIndex;
        if (m_iOtherBalloonIndex == m_iUseBalloonCount)
        {
            if (m_iUseBalloonCount - 1 >= 0)
                iIndex = m_iUseBalloonCount - 1;
            else
                iIndex = 0;
        }
        else
            iIndex = m_iUseBalloonCount;

        SetMsg(strMsg, iIndex, dwUID, eMsgType, dwChatColor);
    }

    // P.S 최적 위치 생성 루틴 추가 필요
}

void KGCSayBalloonSquare::SetMsg(std::wstring strMsg, int iIndex, DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor)
{
    if (false == CheckIndex(iIndex)) {
        return;
    }

    SetWidthHeightFromMsgSize(strMsg, iIndex);
    m_iUseBalloonCount++;
    m_vecfElapsedTime[iIndex] = DXUtil_Timer(TIMER_GETAPPTIME) + BALLOON_DISPLAY_TIME_BASE + BALLOON_DISPLAY_TIME * strMsg.length();
    m_vecpkMessageBalloon[iIndex]->ToggleRender(true);
    m_vecpkSayMsg[iIndex]->ToggleRender(true);
    m_vecdwUserUID[iIndex] = dwCharUID;
    m_vecdwNPCUID[iIndex] = NULL;
    m_veceChatType[iIndex] = eMsgType;
    m_vecdwColor[iIndex] = dwChatColor;
    SetMsgColor(iIndex, eMsgType);
}

void KGCSayBalloonSquare::SetNPCMsg(std::wstring strMsg, int iIndex, DWORD dwUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor)
{
    SetWidthHeightFromMsgSize(strMsg, iIndex);
    m_iUseBalloonCount++;
    m_vecfElapsedTime[iIndex] = DXUtil_Timer(TIMER_GETAPPTIME) + BALLOON_DISPLAY_TIME_BASE + BALLOON_DISPLAY_TIME * strMsg.length();
    m_vecpkMessageBalloon[iIndex]->ToggleRender(true);
    m_vecpkSayMsg[iIndex]->ToggleRender(true);
    m_vecdwUserUID[iIndex] = NULL;
    m_vecdwNPCUID[iIndex] = dwUID;
    m_veceChatType[iIndex] = eMsgType;
    m_vecdwColor[iIndex] = dwChatColor;
    SetMsgColor(iIndex, eMsgType);
}


void KGCSayBalloonSquare::SetMsgColor(int iIndex, KChatData::ENUM_CHAT_TYPE eMsgType)
{
    switch (eMsgType)
    {
    case KChatData::MSG_ADMIN_COMMON:
    case KChatData::MSG_COLOR_CHAT:
    case KChatData::MSG_COMMON:
        if (m_vecdwColor[iIndex] == 0xffffffff)
        {
            m_vecpkMessageBalloon[iIndex]->SetAllWindowColor(D3DCOLOR_ARGB(SAY_BOX_ALPHA, 0xff, 0xff, 0xff));
            m_vecpkSayMsg[iIndex]->SetFontColor(D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
            m_vecpkSayMsg[iIndex]->SetFontOutline(false);
        }
        else
        {
            m_vecpkMessageBalloon[iIndex]->SetAllWindowColor(m_vecdwColor[iIndex] & 0x96ffffff);
            m_vecpkSayMsg[iIndex]->SetFontOutline(true, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
            m_vecpkSayMsg[iIndex]->SetFontColor(m_vecdwColor[iIndex]);
        }
        m_vecpkSayMsg[iIndex]->SetShadow(false);
        break;
    }
}

//#define FOLD_GAP_PER 1.0f
//#define FOLD_GAP_LIMIT 0

void KGCSayBalloonSquare::ChooseArrow(int iIndex, D3DXVECTOR2 vPos, EArrowPos eArrow, int BalloonDistance) //화살표 방향을 계산한다.
{
    D3DXVECTOR2 vPosLeft, vPosRight, vPosTemp1, vPosTemp2;

    int iSizeRight = 0, iSizeLeft = 0;

    vPosLeft = vPosRight = vPos;

    //오른쪽 일때
    vPosRight.x += BalloonDistance;
    vPosRight.y -= m_vecpkMessageBalloon[iIndex]->GetCenterHeight();

    //왼쪽일때
    vPosLeft.x -= BalloonDistance;
    vPosLeft.x -= m_vecpkMessageBalloon[iIndex]->GetCenterWidth();
    vPosLeft.y -= m_vecpkMessageBalloon[iIndex]->GetCenterHeight();

    if (m_vecpkMessageBalloon[iIndex]->GetFirstDraw() == true || eArrow != GCSB_ARROW_POS_AUTO)
    {

        m_vecpkMessageBalloon[iIndex]->SetFirstDraw(false);
        vPosTemp2 = m_vecpkMessageBalloon[iIndex]->GetCenterFixedWindowLocalPos();
        vPosTemp2.x += m_vecpkMessageBalloon[iIndex]->GetCenterWidth();
        vPosTemp2.y += m_vecpkMessageBalloon[iIndex]->GetCenterHeight();

        for (int i = 0; i < m_iUseBalloonCount; i++)
        {
            if (i == iIndex)//자기 자신은 A다.
                continue;

            vPosTemp1 = m_vecpkMessageBalloon[i]->GetCenterFixedWindowLocalPos();
            vPosTemp1.x += m_vecpkMessageBalloon[i]->GetCenterWidth();
            vPosTemp1.y += m_vecpkMessageBalloon[i]->GetCenterHeight();

            iSizeRight += GetfoldSize(
                m_vecpkMessageBalloon[i]->GetCurrentWindowLocalPos()
                , m_vecpkMessageBalloon[i]->GetCurrentWindowLocalPos() + vPosTemp1
                , vPosRight
                , vPosRight + vPosTemp2);

            iSizeLeft += GetfoldSize(
                m_vecpkMessageBalloon[i]->GetCurrentWindowLocalPos()
                , m_vecpkMessageBalloon[i]->GetCurrentWindowLocalPos() + vPosTemp1
                , vPosLeft
                , vPosLeft + vPosTemp2);
        }

        if (eArrow == GCSB_ARROW_POS_AUTO)
            eArrow = iSizeLeft < iSizeRight ? GCSB_ARROW_POS_RIGHT : GCSB_ARROW_POS_LEFT;

        if (eArrow == GCSB_ARROW_POS_LEFT)
        {
            vPos = vPosLeft;
            m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(false);
            m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(true);
        }
        else
        {
            vPos = vPosRight;
            m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(true);
            m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(false);
        }
    }
    else //그냥 기존 화살 그대로 유지하는 경우
    {
        switch (m_vecpkMessageBalloon[iIndex]->GetArrowType())
        {
        case KD3DBalloonBox::EAT_NONE:
        case KD3DBalloonBox::EAT_LEFT:
            vPos = vPosRight;
            m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(true);
            m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(false);
            break;
        case KD3DBalloonBox::EAT_RIGHT:
            vPos = vPosLeft;
            m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(false);
            m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(true);
            break;
        }
    }
    m_vecpkMessageBalloon[iIndex]->SetWindowPosDirect(vPos);
    m_vecpkSayMsg[iIndex]->SetWindowPosDirect(vPos + m_vecpkMessageBalloon[iIndex]->GetCenterFixedWindowLocalPos());

    return;
}


int KGCSayBalloonSquare::GetfoldSize(D3DXVECTOR2 vVec11, D3DXVECTOR2 vVec12, //겹치키는 부분 면적 구하는 함수
    D3DXVECTOR2 vVec21, D3DXVECTOR2 vVec22)
{
    //11,12가 한셋트 사각형 정보...
    //21,22가 한셋트 사각형 정보임..
    FLOAT farrX[4], farrY[4], fTemp;
    bool barrX[4], barrY[4], bTemp;
    int iSize;

    farrX[0] = vVec11.x; barrX[0] = true;
    farrX[1] = vVec12.x; barrX[1] = true;
    farrX[2] = vVec21.x; barrX[2] = false;
    farrX[3] = vVec22.x; barrX[3] = false;

    farrY[0] = vVec11.y; barrY[0] = true;
    farrY[1] = vVec12.y; barrY[1] = true;
    farrY[2] = vVec21.y; barrY[2] = false;
    farrY[3] = vVec22.y; barrY[3] = false;

    //sort ascending 
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4 - i - 1; j++)
        {
            if (farrX[j] > farrX[j + 1])
            {
                fTemp = farrX[j];
                farrX[j] = farrX[j + 1];
                farrX[j + 1] = fTemp;

                bTemp = barrX[j];
                barrX[j] = barrX[j + 1];
                barrX[j + 1] = bTemp;
            }

            if (farrY[j] > farrY[j + 1])
            {
                fTemp = farrY[j];
                farrY[j] = farrY[j + 1];
                farrY[j + 1] = fTemp;

                bTemp = barrY[j];
                barrY[j] = barrY[j + 1];
                barrY[j + 1] = bTemp;
            }
        }
    }

    //안겹치는 경우...
    if ((barrX[0] == barrX[1]) || (barrY[0] == barrY[1]))
    {
        iSize = 0;
    }
    else
    {
        iSize = (int)((farrX[2] - farrX[1]) * (farrY[2] - farrY[1]));
    }

    return iSize;
}

void KGCSayBalloonSquare::SetBalloonPos(D3DXVECTOR2 vPos, DWORD dwUID, EArrowPos eArrow, int BalloonDistance)
{
    for (int i = 0; i < m_iUseBalloonCount; i++)
    {
        if (m_vecdwUserUID[i] == dwUID)
        {   // UID를 찾아서 위치정보를 설정한다.
            vPos.x *= GC_SCREEN_DIV_WIDTH;
            vPos.y *= GC_SCREEN_DIV_HEIGHT;

            ChooseArrow(i, vPos, eArrow, BalloonDistance);
            break;
        }
    }
}


void KGCSayBalloonSquare::SetWidthHeightFromMsgSize(std::wstring strMsg, int iIndex)
{
    DWORD dwWidth, dwHeight;
    D3DXVECTOR2 vPos;

    int iLinePerChar = LINE_PER_CHAR;
    iLinePerChar += (int)strMsg.length() / LINE_PER_CHAR_INC;

    //RemoveSpaceChar(strMsg); //SetTextAutoMultiLine으로 붙어진 줄바꿈 문자 삭제
    if (strMsg.empty() == false)
    {
        ASSERT("Empty Message??? Why?");
    }

    if (SiKGCSocialMotionManager()->IsSocialMotionCommand(strMsg, true)) {
        return;
    }

    if (m_vecpkMessageBalloon.size() <= static_cast<size_t>(iIndex)) {
        return;
    }

    if (m_vecpkSayMsg.size() <= static_cast<size_t>(iIndex)) {
        return;
    }

    dwWidth = (DWORD)(((int)strMsg.length() > iLinePerChar ? iLinePerChar : strMsg.length()) * m_vecpkSayMsg[iIndex]->GetFontSize());
    m_vecpkMessageBalloon[iIndex]->SetWidth(dwWidth);
    m_vecpkSayMsg[iIndex]->SetWidth(m_vecpkMessageBalloon[iIndex]->GetCenterWidth()); //너비를 먼저 넣고
    m_vecpkSayMsg[iIndex]->SetTextAutoMultiline(strMsg, false); //텍스트를 집어 넣고
    dwHeight = (DWORD)(m_vecpkSayMsg[iIndex]->GetLineNum() * (int)((float)m_vecpkSayMsg[iIndex]->GetFontSize() * m_vecpkSayMsg[iIndex]->GetLineSpace())); //자동으로 정해진 라인수로 높이를 정한다.
    m_vecpkMessageBalloon[iIndex]->SetHeight(dwHeight);
    m_vecpkSayMsg[iIndex]->SetHeight(m_vecpkMessageBalloon[iIndex]->GetCenterHeight());

    if (m_vecpkMessageBalloon[iIndex]->GetArrowType() == KD3DBalloonBox::EAT_NONE)
    {
        m_vecpkMessageBalloon[iIndex]->ChangeLeftArrow(true);
        m_vecpkMessageBalloon[iIndex]->ChangeRightArrow(false);
        m_vecpkMessageBalloon[iIndex]->SetFirstDraw(true);
    }
}

bool KGCSayBalloonSquare::CheckIndex(const int iIndex_)
{
    if (iIndex_ < 0) {
        return false;
    }

    if (static_cast<int>(m_vecfElapsedTime.size()) <= iIndex_ ||
        static_cast<int>(m_vecpkMessageBalloon.size()) <= iIndex_ ||
        static_cast<int>(m_vecpkSayMsg.size()) <= iIndex_ ||
        static_cast<int>(m_vecdwUserUID.size()) <= iIndex_ ||
        static_cast<int>(m_vecdwNPCUID.size()) <= iIndex_ ||
        static_cast<int>(m_veceChatType.size()) <= iIndex_ ||
        static_cast<int>(m_vecdwColor.size()) <= iIndex_) {
        return false;
    }

    return true;
}

void KGCSayBalloonSquare::SetOffset(DWORD dwUID, float fOffsetX, float fOffsetY)
{
    m_mapOffset[dwUID] = D3DXVECTOR2(fOffsetX, fOffsetY);

}
D3DXVECTOR2 KGCSayBalloonSquare::GetOffset(DWORD dwUID)
{
    std::map<DWORD, D3DXVECTOR2>::iterator mapIter = m_mapOffset.find(dwUID);
    if (mapIter != m_mapOffset.end())
        return mapIter->second;
    return D3DXVECTOR2(0.0f, 0.0f);
}

void KGCSayBalloonSquare::SetArrowDist(DWORD dwUID, int iArrowDist)
{
    m_mapDist[dwUID] = iArrowDist;

}
int KGCSayBalloonSquare::GetArrowDist(DWORD dwUID)
{
    std::map<DWORD, int>::iterator mapIter = m_mapDist.find(dwUID);
    if (mapIter != m_mapDist.end())
        return mapIter->second;
    return 0;
}