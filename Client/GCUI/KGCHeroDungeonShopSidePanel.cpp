#include "stdafx.h"
#include "KGCHeroDungeonShopSidePanel.h"
#include "KGCHeroDungeonShopS6.h"
#include "KGCFaceLift.h"
#include "GCRenderManager.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonShopSidePanel);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonShopSidePanel);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonShopSidePanel, "gc_herodungeonshopsidepanel");

#define DELTAZOOM	0.01f
#define MAXZOOM	    0.25f
#define MINZOOM     0.18f

KGCHeroDungeonShopSidePanel::KGCHeroDungeonShopSidePanel(void) :
    m_pkImgRenderPos(NULL),
    m_pkViewBasic(NULL),
    m_pMeshFootStage(NULL),
    m_pAnimationMotion(NULL),
    m_iMotionCount(0),
    m_cFrame(0),
    m_fCharX(0.0f),
    m_fAngleMove(0.0f),
    m_fZoom(0.25),
    m_bAngleChanged(false),
    m_iExtraCharNum(0),
    m_pWaitMotion(NULL),
    m_pActionMotion(NULL)
{
    LINK_CONTROL("render_position", m_pkImgRenderPos);
    LINK_CONTROL("btn_view_basic", m_pkViewBasic);
}

KGCHeroDungeonShopSidePanel::~KGCHeroDungeonShopSidePanel(void)
{
    m_Player.ReleasePlayerResource();
}

void KGCHeroDungeonShopSidePanel::OnDestroy(void)
{
    SAFE_RELEASE(m_pAnimationMotion);
    SAFE_RELEASE(m_pWaitMotion);
    SAFE_RELEASE(m_pActionMotion);
}

void KGCHeroDungeonShopSidePanel::OnCreate(void)
{
    m_pkImgRenderPos->InitState(false);
    m_pkViewBasic->InitState(true, true, this);

    D3DXVECTOR2 vPos(GetFixedWindowPos());
    D3DXVECTOR2 vAddLocalPos(m_pkImgRenderPos->GetFixedWindowLocalPos());
    m_viewport.X = static_cast<DWORD>((vPos.x + vAddLocalPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>((vPos.y + vAddLocalPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>(m_pkImgRenderPos->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height = static_cast<DWORD>(m_pkImgRenderPos->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH(&matLook, float(GetWidth()) / float(GetHeight()), 1.0f, 1.0f, 100.0f);
    ::D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.01f);
    ::D3DXMatrixTranslation(&matTran, 0.0f, 0.172f, 0.5f);
    m_matProject = matLook * matScale * matTran;

    ReloadAnimationMotion();
}

void KGCHeroDungeonShopSidePanel::PostChildDraw(void)
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot, matTrans, matScale, matWorld;
    g_pd3dDevice->GetViewport(&orgViewport);
    g_pd3dDevice->SetViewport(&m_viewport);

    g_pd3dDevice->SetFVF(SKINVERTEX::FVF);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);
    g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0L);

    DWORD dwColor = 0xff142231;
    g_MyD3D->m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, dwColor, 1.0f, 0L);

    float fScale = 4.3f;
    float fFootStageY = -0.15f;
    float fFootStageZ = 0.38f;
    float fFootStageMul = 0.44f;
    float fAngle = 0.9f;

    D3DXMatrixScaling(&matScale, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale);
    D3DXMatrixRotationY(&matRot, m_fStageAngleMove);
    D3DXMatrixTranslation(&matTrans, 0.0f, fFootStageY + (MAXZOOM - m_fZoom) * fFootStageMul, fFootStageZ);

    // 방 출력 부분 
    matWorld = matScale * matRot * matTrans;
    if (m_pMeshFootStage != NULL) {
        m_pMeshFootStage->SetWorldMat(&matWorld);
        g_RenderManager->Render(false, m_pMeshFootStage, EGCShaderTechnique::NOCARTOON_NOEDGE);
    }

    if (m_pAnimationMotion != NULL)
    {
        std::string strMotionName = "";
        if (m_cFrame > (int)m_pAnimationMotion->GetNum_Frame() - 1)
        {
            m_iMotionCount++;
            if (m_iMotionCount == 5)
            {
                if (m_pActionMotion)
                {
                    // 모션이 바뀌었음
                    m_pAnimationMotion = m_pActionMotion;
                    m_iMotionCount = 0;
                    m_fCharX = 0.0f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName(m_pActionMotion->GetDeviceID());
                }
            }
            else
            {
                if (m_pWaitMotion)
                {
                    // 모션이 바뀌었음
                    m_pAnimationMotion = m_pWaitMotion;
                    m_fCharX = 0.0f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName(m_pWaitMotion->GetDeviceID());
                }
            }

            m_cFrame = 0;
        }

        float fPosX = m_fCharX * CHAR_SCALE_RATIO;
        float fPosY = m_pAnimationMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.06f;
        float fPosZ = 0.303f;

        m_fCharX += m_pAnimationMotion->GetFrameData(m_cFrame).Plus_x;

        fPosX = fPosX * cos(m_fAngleMove);

        D3DXMatrixScaling(&matScale, m_fZoom, m_fZoom, m_fZoom);
        D3DXMatrixRotationY(&matRot, m_fAngleMove);
        D3DXMatrixTranslation(&matTrans, fPosX, fPosY, fPosZ);

        matWorld = matScale * matRot * matTrans;

        m_Player.m_pObject->SetWorldMat(&matWorld);
        m_Player.m_pObject->SetFrame(&m_pAnimationMotion->GetFrameData((int)m_cFrame));

        m_Player.SetRenderFaceLift(m_Player.GetDefaultFaceLiftType());
        SiKGCFaceLift()->SetFaceLiftByMotionName(&m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame);

        g_RenderManager->Render(false, m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
        m_cFrame++;
    }

    // 뷰포트 복구
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_MyD3D->matProj);
    g_pd3dDevice->SetViewport(&orgViewport);
    g_pd3dDevice->SetTexture(1, NULL);

}

void KGCHeroDungeonShopSidePanel::ActionPerformed(const KActionEvent& event)
{
    GCWND_MSG_MAP(m_pkViewBasic, KD3DWnd::D3DWE_BUTTON_CLICK, [&]()
        {
            g_HeroDungeonShopS6.ClearEquippedItems();
            UnequipItems();
            UpdateMeshObject();
            ReloadAnimationMotion();
            g_HeroDungeonShopS6.UpdateShopPanel();
        });
}

void KGCHeroDungeonShopSidePanel::ChangeCharWndZoom(bool bZoom)
{
    if (bZoom)
        m_fZoom += DELTAZOOM;
    else
        m_fZoom -= DELTAZOOM;

    if (m_fZoom > MAXZOOM)
        m_fZoom = MAXZOOM;

    if (m_fZoom < MINZOOM)
        m_fZoom = MINZOOM;
}

void KGCHeroDungeonShopSidePanel::FrameMoveInEnabledState(void)
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
    bool bCheckBound = m_pkImgRenderPos->CheckPosInWindowBoundWithChild(vMousePos, false);

    if (!m_bAngleChanged)
    {
        m_fAngleMove += 0.005f;
        m_fStageAngleMove += 0.0035f;
        if (m_fAngleMove >= 2 * D3DX_PI) m_fAngleMove = 0.0f;
        if (m_fStageAngleMove >= 2 * D3DX_PI) m_fStageAngleMove = 0.0f;
    }

    if (g_pkInput->ISWheelUp() && bCheckBound)
        ChangeCharWndZoom(true);
    else if (g_pkInput->ISWheelDown() && bCheckBound)
        ChangeCharWndZoom(false);

    static bool Internal = false;

    if (g_pkInput->BtnDown() == true && bCheckBound)
    {
        m_bAngleChanged = true;
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
        m_fStartStageAngle = m_fStageAngleMove;
        Internal = true;
    }
    else if (g_pkInput->BtnDown() == true && !bCheckBound)
    {
        Internal = false;
    }

    if (g_pkInput->BtnDown(KInput::MBRIGHT) == TRUE && bCheckBound)
    {
        m_bAngleChanged = false;
    }

    if (g_pkInput->BtnPressed() && m_bAngleChanged == true && Internal == true)
        ProcessCharWndAngle();

    if (m_Player.m_pObject)
    {
        m_Player.SetRender(true);
        m_Player.EgoItemFrameMove(UCHAR(m_cFrame));
        m_Player.m_pObject->ChildFrameMove();
    }
}

void KGCHeroDungeonShopSidePanel::InitCharacterInfo()
{
    SCharInfo charInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
    m_Player.m_kUserInfo.Init();
    m_Player.m_kUserInfo.AddChar(charInfo);
    m_Player.m_kUserInfo.SetCurrentChar(charInfo.iCharType);
}

void KGCHeroDungeonShopSidePanel::SetPreviewWindow()
{
    D3DXVECTOR2 vPos(GetFixedWindowPos());

    int nWidthX = m_pkImgRenderPos->GetWidth();
    int nHeightX = m_pkImgRenderPos->GetHeight();
    D3DXVECTOR2 vtPos = m_pkImgRenderPos->GetCurrentWindowLocalPos();

    m_viewport.X = static_cast<DWORD>((static_cast<int>(vtPos.x) + vPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>((static_cast<int>(vtPos.y) + vPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>(nWidthX * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height = static_cast<DWORD>(nHeightX * g_pGCDeviceManager2->GetWindowScaleY());

    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    UnequipItems();
    UpdateMeshObject();
}

void KGCHeroDungeonShopSidePanel::EquipItem(GCItem* item)
{
    g_pItemMgr->EquipItem(&(m_Player.GetCurrentChar()), item);
    UpdateMeshObject();
}

void KGCHeroDungeonShopSidePanel::UnequipItem(GCItem* item)
{
    g_pItemMgr->UnequipItemForItemID(&(m_Player.GetCurrentChar()), item->dwGoodsID);
    UpdateMeshObject();
}

void KGCHeroDungeonShopSidePanel::UnequipItems()
{
    SCharInfo CharInfo = m_Player.GetCurrentChar();
    for (std::vector<SUserItem>::iterator it = CharInfo.vecLookInfo.begin(); it != CharInfo.vecLookInfo.end(); ++it)
        g_pItemMgr->UnequipItemForItemUID(&(m_Player.GetCurrentChar()), it->dwItemUID);
    for (std::vector<SUserItem>::iterator it = CharInfo.vecItemInfo.begin(); it != CharInfo.vecItemInfo.end(); ++it)
        g_pItemMgr->UnequipItemForItemUID(&(m_Player.GetCurrentChar()), it->dwItemUID);
}

void KGCHeroDungeonShopSidePanel::UpdateMeshObject()
{
    m_Player.SetMyPlayer(-1, true);
    m_Player.ReleasePlayerResource();
    m_Player.UpdatePlayerResource(true);

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = m_Player.Extra_Char_Num;

    if (m_iExtraCharNum < 0 || m_iExtraCharNum >= CID_MAX)
        return;

    if (m_Player.m_pObject)
        m_Player.m_pObject->SetShellMatrixScale(0.25f);

    if (m_iExtraCharNum != iBeforeExtraCharNum)
        ReloadAnimationMotion();

    if (!m_pAnimationMotion)
        ReloadAnimationMotion();

    if (!m_pMeshFootStage)
    {
        m_pMeshFootStage = new GCObject();
        m_pMeshFootStage->AddMesh(g_pGCDeviceManager2->CreateMeshP3M("footstage.p3m"), g_pGCDeviceManager2->CreateTexture("footstage.dds"));
    }
}

void KGCHeroDungeonShopSidePanel::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
    m_fStageAngleMove = m_fStartStageAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCHeroDungeonShopSidePanel::ReloadAnimationMotion()
{
    SCharInfo nCharNum = g_MyD3D->m_TempPlayer.GetCurrentChar();
    int nJobLevel = g_MyD3D->m_TempPlayer.GetExtraCharNum(m_iExtraCharNum);

    char strTemp[MAX_PATH];
    std::string strShopMotion;
    sprintf(strTemp, "shop_%d_%d", nCharNum, nJobLevel);

    strShopMotion = strTemp;

    m_pAnimationMotion = g_pGCDeviceManager2->CreateMotionFRM(strShopMotion + "_0.frm");

    GCFUNC::CreateCharViewMotion(m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion);
}