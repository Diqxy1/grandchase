#include "stdafx.h"
#include "GCAgitObjectAction.h"


KGCAgitObjectAction::KGCAgitObjectAction()
{
    m_fPosX         = 0.0f;
    m_fPosY         = 0.0f;
    m_fWidth        = 0.0f;
    m_fHeight       = 0.0f;
    m_eObjType      = EM_AGIT_OBJECT_SIZE;
    m_bLive         = true;
    m_bInitEnable   = true;
    m_dwItemUID       = UINT_MAX;
}

GCCollisionRect<float> KGCAgitObjectAction::GetCollisionRect( void )
{
    GCCollisionRect<float> rtRect;
    rtRect.m_Left       = m_fPosX;
    rtRect.m_Right      = m_fPosX;
    rtRect.m_Top        = m_fPosY;
    rtRect.m_Bottom     = m_fPosY;

    return rtRect;
}
