#include "stdafx.h"
#include ".\objectmanager.h"
#include "Mapsystem/KQSLayerManager.h"

CObjectManager::CObjectManager(void)
{
    m_pFont             = 0;
}

CObjectManager::~CObjectManager(void)
{
}

bool CObjectManager::IsInLayerPart(D3DVECTOR pos, float fLayerPosX, float fLayerPosY, CString strMon)
{
    std::string strMon2 = GCUTIL_STR::GCStrWideToChar( strMon.GetBuffer() );

    KQSLayerPart *pLayerPart = SiKQSLayerManager()->GetOriginalPart(strMon2);   
    if( pLayerPart )
    {
        KLayerRect<int> kRect;
        pLayerPart->GetTexRect(kRect);
        pLayerPart->ResetPosition_X(fLayerPosX, (float)fLayerPosY+kRect.m_Height);
        pLayerPart->ResetPosition_Y(fLayerPosX, (float)fLayerPosY+kRect.m_Height);
        if( pLayerPart->IsMouseIn(pos) )
        {
            return true;
        }
    }

    return false;
}

void CObjectManager::DrawPartDesc(LPDIRECT3DDEVICE9 pd3dDevice, float fLayerPosX, float fLayerPosY, CString strDesc, CString strObject)
{
    std::string strTemp = GCUTIL_STR::GCStrWideToChar( strObject.GetBuffer() );

    KQSLayerPart *pLayerPart = SiKQSLayerManager()->GetOriginalPart(strTemp);
    if( pLayerPart )
    {
        KLayerRect<int> kRect;
        pLayerPart->GetTexRect(kRect);
        POINT ptText = Convert_3DTo2D_Point(pd3dDevice, D3DXVECTOR3(fLayerPosX, fLayerPosY+kRect.m_Height, 0));
        D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
        if( m_pFont ) m_pFont->DrawText((float)ptText.x, (float)ptText.y, fontColor, strDesc);
    }
}

void CObjectManager::DrawPartObject(float fLayerPosX, float fLayerPosY, bool bInverse, CString strObject)
{
    std::string strTemp = GCUTIL_STR::GCStrWideToChar( strObject.GetBuffer() );

    KQSLayerPart *pLayerPart = SiKQSLayerManager()->GetOriginalPart(strTemp);
    if( pLayerPart )
    {
        KLayerRect<int> kRect;
        pLayerPart->GetTexRect(kRect);
        if( bInverse )  // 텍스쳐들이 기본적으로 왼쪽을 바라보므로 Right인경우 Inverse 되야 함.
        {
            if( kRect.m_Width < 0 )
                pLayerPart->Inverse(1);
        }
        else
        {
            if( kRect.m_Width > 0 )     // Width가 0보다 크다는 말은 inverse 된 텍스쳐가 아니라는 뜻.
                pLayerPart->Inverse(1);
        }
        pLayerPart->ResetPosition_X(fLayerPosX, (float)(fLayerPosY+kRect.m_Height));
        pLayerPart->ResetPosition_Y(fLayerPosX, (float)(fLayerPosY+kRect.m_Height));
        pLayerPart->FrameMove( QS_ELAPSED_TIME );
        pLayerPart->Render();
    }
}