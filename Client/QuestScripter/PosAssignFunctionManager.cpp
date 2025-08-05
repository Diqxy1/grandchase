#include "StdAfx.h"
#include ".\posassignfunctionmanager.h"
#include "MapSystem/KQSLayerPart.h"
#include "MapSystem/KQSLayerManager.h"

CPosAssignFunctionManager::CPosAssignFunctionManager(void)
{
    m_pListener = NULL;
    m_iHeight = 0;
    m_bDrag = false;
}

CPosAssignFunctionManager::~CPosAssignFunctionManager(void)
{
}

void CPosAssignFunctionManager::DrawMode(void)
{
    KQSLayerPart *pLayerPart = SiKQSLayerManager()->GetOriginalPart("BLANK");   
    if( pLayerPart )
    {
        KLayerRect<int> rtiLayerRect;
        pLayerPart->GetTexRect(rtiLayerRect);
        int iWidth = rtiLayerRect.m_Width;
        int iHeight = rtiLayerRect.m_Height;

        KLayerRect<float> kRect;
        KLayerRect<int> kiRect;
        pLayerPart->GetTexRect(kiRect);
        kRect.m_X       = (float)kiRect.m_X;
        kRect.m_Y       = (float)kiRect.m_Y;
        kRect.m_Width   = (float)kiRect.m_Width;
        kRect.m_Height  = (float)kiRect.m_Height;

        kRect.m_Width   = (float)800;
        kRect.m_Height  = (float)600;
        pLayerPart->SetLayerPos( kRect );

        pLayerPart->ResetPosition_X((float)800, (float)m_iHeight);
        pLayerPart->ResetPosition_Y((float)800, (float)m_iHeight);

        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
        g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_SRCALPHA );
        pLayerPart->FrameMove( QS_ELAPSED_TIME );
        pLayerPart->Render();
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

        kRect.m_Width   = (float)iWidth;
        kRect.m_Height  = (float)iHeight;
        pLayerPart->SetLayerPos( kRect );
    }
}

void CPosAssignFunctionManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_bDrag )
    {
        m_iHeight     = (int)pos.y;
    }
}

void CPosAssignFunctionManager::MouseLBDownMode(D3DVECTOR pos)
{
    m_bDrag = true;
}

void CPosAssignFunctionManager::MouseLBUpMode(D3DVECTOR pos)
{
    const bool bShift =  GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000;
    if( m_pListener && !bShift )
    {
        m_pListener->SendMessage(WM_POSASEL_FUNCTION, (WPARAM)pos.x, (LPARAM)pos.y);
    }

    m_bDrag = false;
}
