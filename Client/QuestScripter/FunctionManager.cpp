#include "StdAfx.h"
#include ".\functionmanager.h"
#include "MapSystem/KQSLayerPart.h"
#include "MapSystem/KQSLayerManager.h"

CFunctionManager::CFunctionManager(void)
{
}

CFunctionManager::~CFunctionManager(void)
{
}

bool CFunctionManager::IsInLayerPart(D3DVECTOR pos, float fLayerPosX, float fLayerPosY, CString strMon)
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
