#include "stdafx.h"
#include ".\kgc3dobjectmgr.h"
#include "KGC3DObject.h"
//
//
//
#include "MyD3d.h"

ImplementSingleton( KGC3DObjectMgr );

#define RELEASE_VECTOR(p)   for( int i = 0; i < (int)p.size(); ++i )\
                            {\
                                SAFE_RELEASE( p[i] );\
                            }\
                            p.clear()

KGC3DObjectMgr::KGC3DObjectMgr(void)
{
    m_vec3DObject.clear();
}

KGC3DObjectMgr::~KGC3DObjectMgr(void)
{
    ReleaseGame();
}

void KGC3DObjectMgr::ReleaseGame()
{
    std::for_each( m_vec3DObject.begin(), m_vec3DObject.end(),
        boost::bind( &KGC3DObject::Release, _1 ) );

    for( int i = 0; i < (int)m_vec3DObject.size(); ++i )
    {
        SAFE_DELETE( m_vec3DObject[i] );
    }
    m_vec3DObject.clear();

    std::for_each( m_vec3DObjectPrevLoad.begin(), m_vec3DObjectPrevLoad.end(),
        boost::bind( &KGC3DObject::Release, _1 ) );

    for( int i = 0; i < (int)m_vec3DObjectPrevLoad.size(); ++i )
    {
        SAFE_DELETE( m_vec3DObjectPrevLoad[i] );
    }
    m_vec3DObjectPrevLoad.clear();

    RELEASE_VECTOR( m_vecPreLoadMesh );
    RELEASE_VECTOR( m_vecPreLoadMotion );
    RELEASE_VECTOR( m_vecPreLoadTex );
}

class KFunctorCheckDeviceID
{
public:
    KFunctorCheckDeviceID( const std::string& strFind ) : m_stdFind(strFind) {}
    ~KFunctorCheckDeviceID() {}

    bool operator()( GCDeviceMeshP3M*& _Arg ) const
    {
        return (m_stdFind == _Arg->GetDeviceID());
    }
    bool operator()( GCDeviceMotionFRM*& _Arg ) const
    {
        return (m_stdFind == _Arg->GetDeviceID());
    }
    bool operator()( GCDeviceTexture*& _Arg ) const
    {
        return (m_stdFind == _Arg->GetDeviceID());
    }

    std::string m_stdFind;
};

void KGC3DObjectMgr::PreLoadMesh( const std::string& strMesh )
{
    GCDeviceMeshP3M* pMesh = g_pGCDeviceManager2->CreateMeshP3M( strMesh );

    if( std::find_if( m_vecPreLoadMesh.begin(), m_vecPreLoadMesh.end(),
            KFunctorCheckDeviceID( pMesh->GetDeviceID() ) ) == m_vecPreLoadMesh.end() )
    {
        m_vecPreLoadMesh.push_back( pMesh );
    }
}

void KGC3DObjectMgr::PreLoadMotion( const std::string& strMotion )
{
    GCDeviceMotionFRM* pMotion = g_pGCDeviceManager2->CreateMotionFRM( strMotion );

    if( std::find_if( m_vecPreLoadMotion.begin(), m_vecPreLoadMotion.end(),
        KFunctorCheckDeviceID( pMotion->GetDeviceID() ) ) == m_vecPreLoadMotion.end() )
    {
        m_vecPreLoadMotion.push_back( pMotion );
    }
}

void KGC3DObjectMgr::PreLoadTex( const std::string& strTex )
{
    GCDeviceTexture* pTex = g_pGCDeviceManager2->CreateTexture( strTex );

    if( std::find_if( m_vecPreLoadTex.begin(), m_vecPreLoadTex.end(),
        KFunctorCheckDeviceID( pTex ->GetDeviceID() ) ) == m_vecPreLoadTex.end() )
    {
        m_vecPreLoadTex.push_back( pTex );
    }
}

void KGC3DObjectMgr::FrameMove()
{
    PROFILE_SET("KGC3DObjectMgr::FrameMove");

    if( g_kGlobalValue.m_bFrameLock && o3 != 0 ) 
        return;

    std::for_each( m_vec3DObject.begin(), m_vec3DObject.end(),
        boost::bind( &KGC3DObject::FrameMove, _1 ) );
}

void KGC3DObjectMgr::Add3DObject( KGC3DObject* pObj )
{
    m_vec3DObject.push_back( pObj );
}

void KGC3DObjectMgr::Remove3DObject( KGC3DObject* pObj )
{
    std::vector< KGC3DObject* >::iterator vit;
    vit = std::find( m_vec3DObject.begin(), m_vec3DObject.end(), pObj );
    if( vit != m_vec3DObject.end() )
    {
        (*vit)->Release();
        SAFE_DELETE( *vit );
        m_vec3DObject.erase( vit );
    }
}

void KGC3DObjectMgr::Add3DObjectForPrevLoad( KGC3DObject* pObj )
{
    m_vec3DObjectPrevLoad.push_back( pObj );
}

void KGC3DObjectMgr::ReleaseStage()
{
    std::for_each( m_vec3DObject.begin(), m_vec3DObject.end(),
        boost::bind( &KGC3DObject::Release, _1 ) );

    for( int i = 0; i < (int)m_vec3DObject.size(); ++i )
    {
        SAFE_DELETE( m_vec3DObject[i] );
    }
    m_vec3DObject.clear();
}
