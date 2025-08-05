#include "stdafx.h"
#include "GCGameObject.h"


KGCGameObject::KGCGameObject()
{
	m_fPosX         = 0.0f;
	m_fPosY         = 0.0f;
    m_fWidth        = 0.0f;
    m_fHeight       = 0.0f;
	m_eObjType      = GC_OBJ_DEFAULT;
	m_bLive         = true;
	m_bInitEnable   = true;
	m_iIndex        = 0;
    m_bCrashCheck   = false;
}

GCCollisionRect<float> KGCGameObject::GetCollisionRect( void )
{
    GCCollisionRect<float> rtRect;
    rtRect.m_Left       = m_fPosX;
    rtRect.m_Right      = m_fPosX;
    rtRect.m_Top        = m_fPosY;
    rtRect.m_Bottom     = m_fPosY;

    return rtRect;
}