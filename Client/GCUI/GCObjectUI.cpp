#include "StdAfx.h"
#include "GCObjectUI.h"

IMPLEMENT_CLASSNAME( KGCObjectUI );
IMPLEMENT_WND_FACTORY( KGCObjectUI );
IMPLEMENT_WND_FACTORY_NAME( KGCObjectUI, "gc_kgcobjectui" );

KGCObjectUI::KGCObjectUI(void)
{
}

KGCObjectUI::~KGCObjectUI(void)
{
    Clear2DObject();
}

void KGCObjectUI::PostDraw()
{
    D3DXVECTOR2 vPos = GetFixedWindowPos();
    BOOST_FOREACH( KGC2DObject* pObj, m_vecObjects ) {
        pObj->Render();
    }
}

void KGCObjectUI::BindToLua()
{
    lua_tinker::class_add<KGCObjectUI>( KGCLuabinder::getInstance().GetLuaState(), "KGCObjectUI" );
    lua_tinker::class_def<KGCObjectUI>( KGCLuabinder::getInstance().GetLuaState(), "Create2DObject",    &KGCObjectUI::Create2DObject );
    lua_tinker::class_def<KGCObjectUI>( KGCLuabinder::getInstance().GetLuaState(), "Remove2DObject",    &KGCObjectUI::Remove2DObject );
    lua_tinker::class_def<KGCObjectUI>( KGCLuabinder::getInstance().GetLuaState(), "Clear2DObject",     &KGCObjectUI::Clear2DObject );
    lua_tinker::class_def<KGCObjectUI>( KGCLuabinder::getInstance().GetLuaState(), "Get2DObject",       &KGCObjectUI::Get2DObject );
    lua_tinker::class_inh<KGCObjectUI,KD3DWnd>( KGCLuabinder::getInstance().GetLuaState() );
}

KGC2DObject* KGCObjectUI::Create2DObject()
{
    KGC2DObject* pObj = new KGC2DObject();
    m_vecObjects.push_back( pObj );
    return pObj;
}

bool KGCObjectUI::Remove2DObject( int iIndex )
{
    if( iIndex < 0 || iIndex >= (int)m_vecObjects.size() )
        return false;

    SAFE_DELETE( m_vecObjects[iIndex] );
    m_vecObjects.erase( m_vecObjects.begin() + iIndex );
    return true;
}

KGC2DObject* KGCObjectUI::Get2DObject( int iIndex )
{
    if( iIndex < 0 || iIndex >= (int)m_vecObjects.size() )
        return NULL;

    return m_vecObjects[iIndex];
}

void KGCObjectUI::Clear2DObject()
{
    BOOST_FOREACH( KGC2DObject* pObj, m_vecObjects ) {
        SAFE_DELETE( pObj );
    }
    m_vecObjects.clear();
}