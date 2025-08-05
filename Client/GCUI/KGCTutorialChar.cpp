#include "stdafx.h"
#include "KGCCharacterName.h"
#include "KGCTutorialChar.h"

IMPLEMENT_CLASSNAME(KGCTutorialChar);
IMPLEMENT_WND_FACTORY(KGCTutorialChar);
IMPLEMENT_WND_FACTORY_NAME(KGCTutorialChar, "gc_tutorial_char");

KGCTutorialChar::KGCTutorialChar()
    : m_pkCharNamePlayer(NULL)
    , m_pkCharNameCom(NULL)
{
    // 멤버 초기화
    m_iMySelCharIndex = 0;
    m_iExtSelCharIndex = 0;
    m_bPlayerTap = true;

    memset(m_iBeforeMySelIdx, 0, sizeof(int) * 2);
    memset(m_iBeforeExtSelIdx, 0, sizeof(int) * 2);
    memset(m_iJobState, 0, sizeof(int) * GC_CHAR_NUM * 2);

    LINK_CONTROL("My_Tap1", m_pkWndTab[MYTAP1]);
    LINK_CONTROL("My_Tap2", m_pkWndTab[MYTAP2]);
    LINK_CONTROL("Ext_Tap1", m_pkWndTab[EXTTAP1]);
    LINK_CONTROL("Ext_Tap2", m_pkWndTab[EXTTAP2]);
    LINK_CONTROL("My_Selector", m_pkWndSelector[MY_SELECTOR]);
    LINK_CONTROL("Ext_Selector", m_pkWndSelector[EXT_SELECTOR]);
    LINK_CONTROL("job_namebox_left", m_pkWndBox[LEFT_0]);
    LINK_CONTROL("job_namebox_right", m_pkWndBox[RIGHT_0]);
    LINK_CONTROL("BigFace0", m_pkWndBigFace[LEFT_0]);
    LINK_CONTROL("BigFace1", m_pkWndBigFace[RIGHT_0]);

    char szTemp[32];
    for (int iLoop = 0; iLoop < GC_CHAR_NUM; ++iLoop)
    {
        sprintf(szTemp, "CharFace%d", iLoop);
        LINK_CONTROL(szTemp, m_pkWndCharFace[iLoop]);
    }

    m_pkJob = NULL;
    LINK_CONTROL("JobBtn", m_pkJob);

    LINK_CONTROL("character_name_img_player", m_pkCharNamePlayer);
    LINK_CONTROL("character_name_img_com", m_pkCharNameCom);
}

KGCTutorialChar::~KGCTutorialChar() {}

void KGCTutorialChar::OnCreateComplete(void)
{
    m_pkCharNamePlayer->InitState(true);
    m_pkCharNameCom->InitState(true);

    m_pkWndTab[MYTAP2]->InitState(true, false, this);
    m_pkWndTab[EXTTAP1]->InitState(true, false, this);
    m_pkWndTab[EXTTAP2]->InitState(true, false, this);
    m_pkWndBox[LEFT_0]->InitState(true, false, this);
    m_pkWndBox[RIGHT_0]->InitState(true, false, this);
    m_pkWndBigFace[LEFT_0]->InitState(true, true, this);
    m_pkWndBigFace[RIGHT_0]->InitState(true, true, this);

    if (!g_kGlobalValue.IsCharacterOpenFromServer()) {
        Result_CharOpen = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_CharOpen, INT_MAX, 5000, 0);
    }

    // 캐릭터 선택버튼 셋팅
    float fPosX, fPosY;

    for (int i = 0; i < MAX_ICON_LINE; i++) {

        fPosX = 31.f * GC_SCREEN_DIV_WIDTH;
        fPosY = 57.f * GC_SCREEN_DIV_WIDTH + (63.0f * GC_SCREEN_DIV_WIDTH * i);
        for (int iLoop = 0 + (i * MAX_CHAR_LINE); iLoop < GC_CHAR_NUM; ++iLoop)
        {     
            if (g_kGlobalValue.IsOpenedChar(iLoop))
            {
                m_pkWndCharFace[iLoop]->InitState(true, true, this);
                m_pkWndCharFace[iLoop]->Lock(false);
            }
            else
            {
                m_pkWndCharFace[iLoop]->InitState(true, false, this);
                m_pkWndCharFace[iLoop]->Lock(true);
            }
            m_pkWndCharFace[iLoop]->SetWindowPosX(fPosX);
            m_pkWndCharFace[iLoop]->SetWindowPosY(fPosY);

            fPosX += (60.0f * GC_SCREEN_DIV_WIDTH);
        }
    }
    m_pkJob->InitState(true, true, this);

    // 필살기 스크립트 및 텍스쳐 로드
    char strFileName[MAX_PATH];
    int iChar, iJob, iEnd = GC_CHAR_NUM * NUM_JOB_LEVEL;
    for (int iLoop = 0; iLoop < iEnd; ++iLoop)
    {
        iChar = iLoop % GC_CHAR_NUM;
        iJob = iLoop / GC_CHAR_NUM;

        //진(0,2,3), 지크하트(3)는 버닝모드로 이미지로 처리
        if ((GC_CHAR_JIN == iChar) && (0 == iJob))
        {
            sprintf(strFileName, "playertemplate7_0.lua");
        }
        else if ((GC_CHAR_JIN == iChar) && (2 == iJob))
        {
            sprintf(strFileName, "playertemplate7_2_burning_0.lua");
        }
        else if ((GC_CHAR_JIN == iChar) && (3 == iJob))
        {
            sprintf(strFileName, "playertemplate7_3_burning_0.lua");
        }
        else if ((GC_CHAR_SIEG == iChar) && (3 == iJob))
        {
            sprintf(strFileName, "playertemplate8_3_burning_0.lua");
        }
        else
        {
            sprintf(strFileName, "PlayerTemplate%d_%d.lua", iChar, iJob);
        }

        if (!LoadSpecialFaceScript(strFileName, &m_SpecialFace[iChar][iJob]))
        {
            ASSERT("PlayerTemplate야~ 어딧니?");
        }
        m_SpecialFace[iChar][iJob].pTex = g_pGCDeviceManager2->CreateTexture(m_SpecialFace[iChar][iJob].strTexName);
    }

    // 스킬트리 스크립트 로드( SkillTreeUI.lua )
    CharOpenSkill();

    // 현 상태로 캐릭터 관련 UI셋팅
    m_bPlayerTap = true;
    SetPlayerTap(m_bPlayerTap);
    SetPlayerTap(m_bPlayerTap, true);
    m_pkJob->SetComJobBtnLock(true, g_kGlobalValue.GetOpenedPromotionNum(m_iExtSelCharIndex));

    // 애니메이션 상태 셋팅
    SetShowAnimation(true, m_bPlayerTap, &m_SpecialFace[m_iMySelCharIndex][m_iJobState[m_iMySelCharIndex][MY_JOB]]);
    SetShowAnimation(true, !m_bPlayerTap, &m_SpecialFace[m_iExtSelCharIndex][m_iJobState[m_iExtSelCharIndex][EXT_JOB]]);
}

void KGCTutorialChar::OnDestroyComplete(void)
{
    int iChar, iJob, iEnd = GC_CHAR_NUM * NUM_JOB_LEVEL;
    for (int iLoop = 0; iLoop < iEnd; ++iLoop)
    {
        iChar = iLoop % GC_CHAR_NUM;
        iJob = iLoop / GC_CHAR_NUM;
        SAFE_RELEASE(m_SpecialFace[iChar][iJob].pTex);
    }
}

// 이벤트 처리
void KGCTutorialChar::ActionPerformed(const KActionEvent& event)
{
    // 롤오버소리
    if (event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER)
    {
        g_KDSound.Play("30");
    }

    // 클릭소리
    if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN ||
        event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_DOWN)
    {
        g_KDSound.Play("31");
    }

    // 탭 처리
    if ((event.m_pWnd == m_pkWndBigFace[LEFT_0]))
    {
        switch (event.m_dwCode)
        {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
            SetRolloverAnimation(true, true, &m_SpecialFace[m_iMySelCharIndex][m_iJobState[m_iMySelCharIndex][MY_JOB]]);
            return;
        case KD3DWnd::D3DWE_CURSOR_LEAVE:
            SetRolloverAnimation(false, true, &m_SpecialFace[m_iMySelCharIndex][m_iJobState[m_iMySelCharIndex][MY_JOB]]);
            return;
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            SetPlayerTap(true);
            return;
        }
    }
    else if ((event.m_pWnd == m_pkWndBigFace[RIGHT_0]))
    {
        switch (event.m_dwCode)
        {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
            SetRolloverAnimation(true, false, &m_SpecialFace[m_iExtSelCharIndex][m_iJobState[m_iExtSelCharIndex][EXT_JOB]]);
            return;
        case KD3DWnd::D3DWE_CURSOR_LEAVE:
            SetRolloverAnimation(false, false, &m_SpecialFace[m_iExtSelCharIndex][m_iJobState[m_iExtSelCharIndex][EXT_JOB]]);
            return;
        case KD3DWnd::D3DWE_BUTTON_CLICK:
            SetPlayerTap(false);
            return;
        }
    }

    // 캐릭터 선택처리
    if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        for (int iLoop = 0; iLoop < GC_CHAR_NUM; ++iLoop)
        {
            if (event.m_pWnd == m_pkWndCharFace[iLoop])
            {
                if (iLoop != (m_bPlayerTap ? m_iMySelCharIndex : m_iExtSelCharIndex))
                {
                    SelectChar(iLoop);
                    g_pkUIScene->m_pkNewTutorialOption->RenderSubSkillBtn(false);
                }
                return;
            }
        }
    }

    // 전직 선택 버튼 이벤트
    if (event.m_pWnd == m_pkJob) {
        SetJop(event.m_dwCode);
        g_pkUIScene->m_pkNewTutorialOption->RenderSubSkillBtn(false);
        return;
    }
}

// 탭 선택처리
void KGCTutorialChar::SetPlayerTap(bool bIndex, bool bCreate)
{
    m_bPlayerTap = bIndex;

    if (m_bPlayerTap) {
        m_pkJob->SetJobBtnLock(false, g_kGlobalValue.GetOpenedPromotionNum(m_iMySelCharIndex));
        m_pkJob->SetComJobBtnLock(true, g_kGlobalValue.GetOpenedPromotionNum(m_iExtSelCharIndex));
    }
    else {
        m_pkJob->SetComJobBtnLock(false, g_kGlobalValue.GetOpenedPromotionNum(m_iExtSelCharIndex));
        m_pkJob->SetJobBtnLock(true, g_kGlobalValue.GetOpenedPromotionNum(m_iMySelCharIndex));
    }

    m_pkWndTab[MYTAP1]->ToggleRender(m_bPlayerTap);
    m_pkWndTab[MYTAP2]->ToggleRender(!m_bPlayerTap);
    m_pkWndTab[EXTTAP1]->ToggleRender(!m_bPlayerTap);
    m_pkWndTab[EXTTAP2]->ToggleRender(m_bPlayerTap);

    if (bCreate) {
        SelectChar(m_iExtSelCharIndex, true);
    }
    else {
        SelectChar(m_bPlayerTap ? m_iMySelCharIndex : m_iExtSelCharIndex, false);
    }
}

// 캐릭터 선택처리
void KGCTutorialChar::SelectChar(int iIndex, bool bCreate)
{
    if ((0 > iIndex || iIndex >= GC_CHAR_NUM))
    {
        return;
    }

    // 인덱스 갱신
    if (m_bPlayerTap && !bCreate)
    {
        m_iBeforeMySelIdx[CHAR] = m_iMySelCharIndex;
        m_iMySelCharIndex = iIndex;
    }
    else
    {
        m_iBeforeExtSelIdx[CHAR] = m_iExtSelCharIndex;
        m_iExtSelCharIndex = iIndex;
    }

    // 전직처리(기획 => 탭별 AND 캐릭터별 모든 전직상태 유지필요 )    
    if (bCreate) {
        SetJop(m_iJobState[iIndex][EXT_JOB], true);
    }
    else {
        // 캐릭터 선택표식위치 처리
        SetCharSelector(iIndex);
        SetJop(m_iJobState[iIndex][(m_bPlayerTap ? MY_JOB : EXT_JOB)]);
    }
}

// 캐릭터 선택 표식처리
void KGCTutorialChar::SetCharSelector(int iCharIdx)
{
    if (0 > iCharIdx || iCharIdx >= GC_CHAR_NUM)
    {
        return;
    }

    m_pkWndSelector[MY_SELECTOR]->ToggleRender(m_bPlayerTap);
    m_pkWndSelector[EXT_SELECTOR]->ToggleRender(!m_bPlayerTap);

    m_pkWndSelector[m_bPlayerTap ? MY_SELECTOR : EXT_SELECTOR]->SetWindowPosX(
        m_pkWndCharFace[iCharIdx]->GetCurrentWindowLocalPos().x - (4.0f * GC_SCREEN_DIV_WIDTH));
    m_pkWndSelector[m_bPlayerTap ? MY_SELECTOR : EXT_SELECTOR]->SetWindowPosY(m_pkWndCharFace[iCharIdx]->GetCurrentWindowLocalPos().y - (13.0f * GC_SCREEN_DIV_HEIGHT));
}

// 전직선택처리
void KGCTutorialChar::SetJop(int iIndex, bool bCreate)
{
    int iOffset;

    // 인덱스 갱신 및 애니메이션 처리
    if (m_bPlayerTap && bCreate == false) {
        // 인덱스 갱신
        iOffset = m_iMySelCharIndex;
        if (m_iBeforeMySelIdx[CHAR] != m_iMySelCharIndex)
        {
            m_iBeforeMySelIdx[JOB] = m_iJobState[m_iBeforeMySelIdx[CHAR]][MY_JOB];
        }
        else
        {
            m_iBeforeMySelIdx[JOB] = m_iJobState[iOffset][MY_JOB];
        }
        m_iJobState[iOffset][MY_JOB] = iIndex;
        m_pkCharNamePlayer->SetCharacterInfo(m_iMySelCharIndex, iIndex);
        // 애니메이션 처리(변경이 있을때만)
        if ((m_iBeforeMySelIdx[CHAR] != m_iMySelCharIndex) || (m_iBeforeMySelIdx[JOB] != iIndex))
        {
            SetShowAnimation(true, true, &m_SpecialFace[m_iMySelCharIndex][iIndex]);
            SetShowAnimation(false, true, &m_SpecialFace[m_iBeforeMySelIdx[CHAR]][m_iBeforeMySelIdx[JOB]]);
        }
    }
    else
    {
        // 인덱스 갱신
        iOffset = m_iExtSelCharIndex;
        if (m_iBeforeExtSelIdx[CHAR] != m_iExtSelCharIndex)
        {
            m_iBeforeExtSelIdx[JOB] = m_iJobState[m_iBeforeExtSelIdx[CHAR]][EXT_JOB];
        }
        else
        {
            m_iBeforeExtSelIdx[JOB] = m_iJobState[iOffset][EXT_JOB];
        }
        m_iJobState[iOffset][EXT_JOB] = iIndex;
        m_pkCharNameCom->SetCharacterInfo(m_iExtSelCharIndex, iIndex);
        // 애니메이션 처리(변경이 있을때만)
        if ((m_iBeforeExtSelIdx[CHAR] != m_iExtSelCharIndex) || (m_iBeforeExtSelIdx[JOB] != iIndex))
        {
            SetShowAnimation(true, false, &m_SpecialFace[m_iExtSelCharIndex][iIndex]);
            SetShowAnimation(false, false, &m_SpecialFace[m_iBeforeExtSelIdx[CHAR]][m_iBeforeExtSelIdx[JOB]]);
        }
    }
#ifdef DECANEE
    bool bPlayerUseSpecialClass = (m_iMySelCharIndex == GC_CHAR_ASIN || m_iMySelCharIndex == GC_CHAR_LIME || m_iMySelCharIndex == GC_CHAR_DECANEE);
    bool bComUseSpecialClass = (m_iExtSelCharIndex == GC_CHAR_ASIN || m_iExtSelCharIndex == GC_CHAR_LIME || m_iExtSelCharIndex == GC_CHAR_DECANEE);
#else
    bool bPlayerUseSpecialClass = (m_iMySelCharIndex == GC_CHAR_ASIN || m_iMySelCharIndex == GC_CHAR_LIME);
    bool bComUseSpecialClass = (m_iExtSelCharIndex == GC_CHAR_ASIN || m_iExtSelCharIndex == GC_CHAR_LIME);
#endif

    // 선택된 전직 아이콘 상태 변경
    if (false == bCreate) {
        m_pkJob->SetAllEnable(false, m_bPlayerTap, m_bPlayerTap ? bPlayerUseSpecialClass : bComUseSpecialClass);
    }
    else {
        m_pkJob->SetAllEnable(false, false, bComUseSpecialClass);
    }

    m_pkJob->SetEnableButton(true, bPlayerUseSpecialClass);
    m_pkJob->SetEnableComButton(true, bComUseSpecialClass);

    if (bCreate == false) {
        m_pkJob->SelectJop(g_kGlobalValue.GetOpenedPromotionNum(iOffset), iIndex, m_bPlayerTap, m_bPlayerTap ? bPlayerUseSpecialClass : bComUseSpecialClass);
    }
    else {
        m_pkJob->SelectJop(g_kGlobalValue.GetOpenedPromotionNum(iOffset), iIndex, false, bComUseSpecialClass);
    }

    // 스킬트리버튼 처리(활성화 / 비활성화) => SkillTreeUI.lua에서 얻어온 정보로 결정
    // 연습용 캐릭터는 스킬트리 버튼을 비활성화 합니다.
    std::set<int>::iterator setIter;
    setIter = m_setOpenCharSkill.find(iOffset + iIndex * GC_CHAR_NUM);
    if (!m_bPlayerTap || setIter == m_setOpenCharSkill.end() || g_kGlobalValue.IsSkillTreeOpenChar(iOffset) == false) {
        SpeakToActionListener(KActionEvent(this, 0));
    }
    else {
        SpeakToActionListener(KActionEvent(this, 1));
    }
}

// SpecialFace출력
void KGCTutorialChar::PreDraw(void)
{
    D3DXVECTOR4 vecTemp;

    // Draw Left Face
    {
        // 애니메이션 보간률 갱신
        SetInterpolationRadio(m_Ani[LEFT_0].fRadio);
        SetInterpolationRadio(m_Ani[LEFT_1].fRadio);

        if ((m_iBeforeMySelIdx[CHAR] != m_iMySelCharIndex) ||
            (m_iBeforeMySelIdx[JOB] != m_iJobState[m_iMySelCharIndex][MY_JOB]))
        {
            vecTemp = LinearInterpolation(m_Ani[LEFT_1].vecStartPos, m_Ani[LEFT_1].vecEndPos, m_Ani[LEFT_1].fRadio);
            RenderFace(vecTemp, m_iBeforeMySelIdx[CHAR], m_iBeforeMySelIdx[JOB], false);
        }
        if (1.0f <= m_Ani[LEFT_1].fRadio)
        {
            m_iBeforeMySelIdx[CHAR] = m_iMySelCharIndex;
            m_iBeforeMySelIdx[JOB] = m_iJobState[m_iMySelCharIndex][MY_JOB];
        }

        vecTemp = LinearInterpolation(m_Ani[LEFT_0].vecStartPos, m_Ani[LEFT_0].vecEndPos, m_Ani[LEFT_0].fRadio);
        RenderFace(vecTemp, m_iMySelCharIndex, m_iJobState[m_iMySelCharIndex][MY_JOB], false);
    }

    // Draw Right Face
    {
        // 애니메이션 보간률 갱신
        SetInterpolationRadio(m_Ani[RIGHT_0].fRadio);
        SetInterpolationRadio(m_Ani[RIGHT_1].fRadio);

        if ((m_iBeforeExtSelIdx[CHAR] != m_iExtSelCharIndex) ||
            (m_iBeforeExtSelIdx[JOB] != m_iJobState[m_iExtSelCharIndex][EXT_JOB]))
        {
            vecTemp = LinearInterpolation(m_Ani[RIGHT_1].vecStartPos, m_Ani[RIGHT_1].vecEndPos, m_Ani[RIGHT_1].fRadio);
            RenderFace(vecTemp, m_iBeforeExtSelIdx[CHAR], m_iBeforeExtSelIdx[JOB], true);
        }
        if (1.0f <= m_Ani[RIGHT_1].fRadio)
        {
            m_iBeforeExtSelIdx[CHAR] = m_iExtSelCharIndex;
            m_iBeforeExtSelIdx[JOB] = m_iJobState[m_iExtSelCharIndex][EXT_JOB];
        }

        vecTemp = LinearInterpolation(m_Ani[RIGHT_0].vecStartPos, m_Ani[RIGHT_0].vecEndPos, m_Ani[RIGHT_0].fRadio);
        RenderFace(vecTemp, m_iExtSelCharIndex, m_iJobState[m_iExtSelCharIndex][EXT_JOB], true);
    }
}

// 보간률 갱신
void KGCTutorialChar::SetInterpolationRadio(float& fRadio)
{
    if (fRadio < 1.0f) { fRadio += ANI_SPEED; }
    else { fRadio = 1.0f; }
}

// 애니메이션을 위한 선형보간
template<typename T>
T KGCTutorialChar::LinearInterpolation(T& t1, T& t2, float& fRatio)
{
    return T((t1 * (1.0f - fRatio)) + (t2 * fRatio));
}

// 필살기 이미지 렌더링
void KGCTutorialChar::RenderFace(D3DXVECTOR4 vecTempPos, int iChar, int iJob, bool bReverse)
{
    D3DXVECTOR4 vecTempUV = D3DXVECTOR4(m_SpecialFace[iChar][iJob].vecUVLeft.x, m_SpecialFace[iChar][iJob].vecUVLeft.y,
        m_SpecialFace[iChar][iJob].vecUVRight.x, m_SpecialFace[iChar][iJob].vecUVRight.y);
    // Right를 위한 이미지 반전
    if (bReverse)
    {
        float fTemp = vecTempPos.x;
        vecTempPos.x = GC_SCREEN_FLOAT_WIDTH - vecTempPos.z;
        vecTempPos.z = GC_SCREEN_FLOAT_WIDTH - fTemp;

        fTemp = vecTempUV.x;
        vecTempUV.x = vecTempUV.z;
        vecTempUV.z = fTemp;
    }

    g_pGCDeviceManager2->DrawInScreenResize(m_SpecialFace[iChar][iJob].pTex,
        vecTempPos.x, vecTempPos.y,
        vecTempPos.z, vecTempPos.w,
        PIXELTOTEXEL(vecTempUV.x),
        PIXELTOTEXEL(vecTempUV.y),
        PIXELTOTEXEL(vecTempUV.z),
        PIXELTOTEXEL(vecTempUV.w));
}

void KGCTutorialChar::SetShowAnimation(bool bShow, bool bDirection, SpecialFace* pOri)
{
    int iindex;

    // 출현하는 애니메이션인지 사라지는 애니메이션인지 결정
    if (bShow)
    {
        // 왼쪽 애니메이션인지 오른쪽 애니메이션인지 선택
        if (bDirection) iindex = LEFT_0;
        else            iindex = RIGHT_0;

        // 시작점과 끝점 셋팅
        m_Ani[iindex].vecStartPos.x = pOri->vecWndPos.x - pOri->vecWndPos.z;
        m_Ani[iindex].vecStartPos.y = pOri->vecWndPos.y;
        m_Ani[iindex].vecStartPos.z = 0.0f;
        m_Ani[iindex].vecStartPos.w = pOri->vecWndPos.w;
        m_Ani[iindex].vecEndPos = pOri->vecWndPos;
    }
    else
    {
        // 왼쪽 애니메이션인지 오른쪽 애니메이션인지 선택
        if (bDirection) iindex = LEFT_1;
        else            iindex = RIGHT_1;

        // 시작점과 끝점 셋팅
        m_Ani[iindex].vecStartPos = pOri->vecWndPos;
        m_Ani[iindex].vecEndPos.x = pOri->vecWndPos.x - pOri->vecWndPos.z;
        m_Ani[iindex].vecEndPos.y = pOri->vecWndPos.y;
        m_Ani[iindex].vecEndPos.z = 0.0f;
        m_Ani[iindex].vecEndPos.w = pOri->vecWndPos.w;
    }

    // 보간률 초기화
    m_Ani[iindex].fRadio = 0.0f;
}

void KGCTutorialChar::SetRolloverAnimation(bool bShow, bool bdirection, SpecialFace* pOri)
{
    int iindex;

    // 왼쪽 애니메이션인지 오른쪽 애니메이션인지 선택
    if (bdirection) iindex = LEFT_0;
    else            iindex = RIGHT_0;

    if (bShow)
    {
        // 시작점과 끝점 셋팅
        m_Ani[iindex].vecStartPos = pOri->vecWndPos;
        m_Ani[iindex].vecEndPos.x = pOri->vecWndPos.x - GAP_ROLLOVER;
        m_Ani[iindex].vecEndPos.y = pOri->vecWndPos.y - (GAP_ROLLOVER * 1.5f);
        m_Ani[iindex].vecEndPos.z = pOri->vecWndPos.z + GAP_ROLLOVER;
        m_Ani[iindex].vecEndPos.w = pOri->vecWndPos.w;
    }
    else
    {
        // 시작점과 끝점 셋팅
        m_Ani[iindex].vecStartPos.x = pOri->vecWndPos.x - GAP_ROLLOVER;
        m_Ani[iindex].vecStartPos.y = pOri->vecWndPos.y - (GAP_ROLLOVER * 1.5f);
        m_Ani[iindex].vecStartPos.z = pOri->vecWndPos.z + GAP_ROLLOVER;
        m_Ani[iindex].vecStartPos.w = pOri->vecWndPos.w;
        m_Ani[iindex].vecEndPos = pOri->vecWndPos;
    }

    // 보간률 초기화
    m_Ani[iindex].fRadio = 0.0f;
}

// playertemplate을 바인딩해서 필살기 이미지 정보얻기
bool KGCTutorialChar::LoadSpecialFaceScript(std::string strFileName, SpecialFace* pSpecialFace)
{
    KLuaManager luaMgr;
    if (!GCFUNC::LoadLuaScript(luaMgr, strFileName, false))
    {
        return false;
    }

    LUA_BEGIN_TABLE("Template", return false)
    {
        LUA_BEGIN_TABLE("SpecialFaceTex", goto NoSpecialFace)
        {
            LUA_GET_VALUE("Texture", pSpecialFace->strTexName, return false);

            LUA_BEGIN_TABLE("TutorialWinPos", return false)
            {
                float fX, fY, fScale;
                LUA_GET_VALUE(1, fX, return false);
                LUA_GET_VALUE(2, fY, return false);
                LUA_GET_VALUE(3, fScale, return false);

                pSpecialFace->vecWndPos.x = fX;
                pSpecialFace->vecWndPos.y = fY;
                pSpecialFace->vecWndPos.z = (fX + (TEX_WIDTH * fScale)) * GC_SCREEN_DIV_WIDTH;
                pSpecialFace->vecWndPos.w = (fY + (TEX_HEIGHT * fScale)) * GC_SCREEN_DIV_HEIGHT;
            }
            LUA_END_TABLE(return false);
            LUA_BEGIN_TABLE("TutorialTexUV", return false)
            {
                LUA_GET_VALUE(1, pSpecialFace->vecUVLeft.x, return false);
                LUA_GET_VALUE(2, pSpecialFace->vecUVLeft.y, return false);
                LUA_GET_VALUE(3, pSpecialFace->vecUVRight.x, return false);
                LUA_GET_VALUE(4, pSpecialFace->vecUVRight.y, return false);
            }
            LUA_END_TABLE(return false);
        }
        LUA_END_TABLE(goto NoSpecialFace);
    }
    goto EndSpecialFace;

NoSpecialFace:
    LUA_END_TABLE(return false);
    return false;

EndSpecialFace:
    LUA_END_TABLE(return false);
    return true;
}

// SkillTreeUI.lua를 바인딩해서 스킬정보 얻기
void KGCTutorialChar::CharOpenSkill()
{
    KLuaManager luaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        return;

    m_setOpenCharSkill.clear();
#if defined(NATION_KOREA)
    std::string strSkillTreeUI = "SkillTreeUI";
#else
    std::string strSkillTreeUI = "SkillTreeUI_kr";
#endif
    strSkillTreeUI += ".lua";

    GCFUNC::LoadLuaScript(luaMgr, strSkillTreeUI.c_str());

    LUA_BEGIN_TABLE("SkillTreeUI", ASSERT(!"테이블이 이상해!!!"))
    {
        for (int iLoopX = 0; iLoopX < GC_CHAR_NUM; ++iLoopX)
        {
            for (int iLoopY = 0; iLoopY < NUM_JOB_LEVEL; ++iLoopY)
            {
                if (SUCCEEDED(luaMgr.BeginTable(iLoopX + iLoopY * GC_CHAR_NUM)))
                {
                    m_setOpenCharSkill.insert(iLoopX + iLoopY * GC_CHAR_NUM);
                    luaMgr.EndTable();
                }
            }
        }
    }
    LUA_END_TABLE(ASSERT(!"테이블 사용에 문제가 있음"));
}
