#include "StdAfx.h"
#include "GCParticleUI.h"
#include "MyD3D.h"

IMPLEMENT_CLASSNAME( KGCParticleUI );
IMPLEMENT_WND_FACTORY( KGCParticleUI );
IMPLEMENT_WND_FACTORY_NAME( KGCParticleUI, "gc_kgcparticleui" );

int KGCParticleUI::ms_iLayer = GC_LAYER_CUSTOM_UI;

int KGCParticleUI::GetOwnLayer()
{
    return ++ms_iLayer;
}

KGCParticleUI::KGCParticleUI(void)
{
    m_iLayer = GetOwnLayer();
}

KGCParticleUI::~KGCParticleUI(void)
{
}

void KGCParticleUI::AddParticle( const char* strName, float fOffsetX, float fOffsetY )
{
    D3DXVECTOR2 vPos = GetFixedWindowPos();

    CParticleEventSeqPTR pParticle = GCFUNC::CreateSequenceToUI( this, strName, fOffsetX, fOffsetY, m_iLayer );
    m_listParticles.push_back( pParticle );
}

void KGCParticleUI::PostChildDraw()
{
    g_ParticleManager->Render(m_iLayer);
}

void KGCParticleUI::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    lua_tinker::class_add<KGCParticleUI>( L, "KGCParticleUI" );
    lua_tinker::class_def<KGCParticleUI>( L, "AddParticle",               &KGCParticleUI::AddParticle );
    lua_tinker::class_inh<KGCParticleUI,KD3DWnd>( L );
}

void KGCParticleUI::OnDestroy()
{
    BOOST_FOREACH( CParticleEventSeqPTR pParticle, m_listParticles )
    {
        if( g_ParticleManager->IsLiveInstance( pParticle ) == true )
        {
            g_ParticleManager->DeleteSequence( pParticle );
        }
    }
    m_listParticles.clear();
}

void KGCParticleUI::FrameMoveInEnabledState()
{
    if( iGC_FPS == 0 ) {
        std::list<CParticleEventSeqPTR>::iterator lit;
        for( lit = m_listParticles.begin() ; lit != m_listParticles.end() ; )
        {
            if( g_ParticleManager->IsLiveInstance( *lit ) == false )
            {
                lit = m_listParticles.erase( lit );
            }
            else{
                ++lit;
            }
        }
    }
}