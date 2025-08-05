#include "StdAfx.h"
#include ".\areaassignfunctionmanager.h"
#include "MapSystem/KQSLayerPart.h"
#include "MapSystem/KQSLayerManager.h"

CAreaAssignFunctionManager::CAreaAssignFunctionManager(void)
{
    m_pListener = 0;
}

CAreaAssignFunctionManager::~CAreaAssignFunctionManager(void)
{
}

void CAreaAssignFunctionManager::FrameMoveMode(void)
{

}

void CAreaAssignFunctionManager::DrawMode(void)
{
    //DrawObjectMode();

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

        kRect.m_Width   = (float)m_rtArea.Width();
        kRect.m_Height  = (float)m_rtArea.Height();
        pLayerPart->SetLayerPos( kRect );

        pLayerPart->ResetPosition_X((float)m_rtArea.right, (float)m_rtArea.bottom);
        pLayerPart->ResetPosition_Y((float)m_rtArea.right, (float)m_rtArea.bottom);
        
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

void CAreaAssignFunctionManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_bDrag )
    {
        m_rtArea.right   = (int)pos.x;
        m_rtArea.top     = (int)pos.y;
    }
}

void CAreaAssignFunctionManager::MouseLBDownMode(D3DVECTOR pos)
{
    if( !m_bDrag )
    {
        m_rtArea.left   = (int)pos.x;
        m_rtArea.bottom = (int)pos.y;
        m_rtArea.left   = (int)pos.x;
        m_rtArea.bottom = (int)pos.y;
    }

    m_bDrag = true;
}

void CAreaAssignFunctionManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pListener )
    {
        m_pListener->SendMessage(WM_AREASEL_FUNCTION, (WPARAM)&m_rtArea, 0);
    }

    m_bDrag = false;
}