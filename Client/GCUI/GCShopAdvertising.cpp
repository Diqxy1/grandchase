#include "stdafx.h"
#include "GCShopAdvertising.h"

KGCShopAdvertising::KGCShopAdvertising( void )
:m_nADShop(0),
m_nADAvatarShop(0)
{
}

KGCShopAdvertising::~KGCShopAdvertising( void )
{
    //
}

void KGCShopAdvertising::CreatePaticles()
{
    // 알리미 사용안함
    if( m_nADShop == 0 && m_nADAvatarShop == 0 )
        return;

    ClearPaticles();

    //테스트코드
//     KLuaManager luaMgr;
//     GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
//     float s_x = 0;
//     float s_y = 0;
//     float a_x = 0;
//     float a_y = 0;
// 
//     LUA_GET_VALUE_DEF( "s_x", s_x, 0 );
//     LUA_GET_VALUE_DEF( "s_y", s_y, 0 );
//     LUA_GET_VALUE_DEF( "a_x", a_x, 0 );
//     LUA_GET_VALUE_DEF( "a_y", a_y, 0 );


    // shop
    if( m_nADShop > 0 ){
        if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MATCH ||
#if defined USE_COORDI_SHOP
            g_MyD3D->m_pStateMachine->GetState() == GS_COORDI_SHOP ||
#endif
            g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO ) {

                AddParticle( "UI_Tab", -0.425f, 0.62f, 0.5f, GC_LAYER_UI_SCENE );

                switch ( m_nADShop ){
                case AD_SHOP_NEW :
                    AddParticle( "UI_Tab_New", -0.506f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                    break;
                case AD_SHOP_DC :
                    AddParticle( "UI_Tab_DC", -0.506f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                    break;
                case AD_SHOP_EVENT :
                    AddParticle( "UI_Tab_Event", -0.506f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                    break;
                case AD_SHOP_HOT :
                    AddParticle( "UI_Tab_Hot", -0.506f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                    break;
                default:
                    break;
                }
        }
    }

    // avatar
    if( m_nADAvatarShop > 0 ){
        if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MATCH ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO ) {

                AddParticle( "UI_Tab", -0.232f, 0.62f, 0.5f, GC_LAYER_UI_SCENE );

                switch ( m_nADAvatarShop ){
                    case AD_SHOP_NEW :
                        AddParticle( "UI_Tab_New", -0.3f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                        break;
                    case AD_SHOP_DC :
                        AddParticle( "UI_Tab_DC", -0.3f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                        break;
                    case AD_SHOP_EVENT :
                        AddParticle( "UI_Tab_Event", -0.3f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                        break;
                    case AD_SHOP_HOT :
                        AddParticle( "UI_Tab_Hot", -0.3f, 0.65f, 0.5f, GC_LAYER_CUSTOM2 );
                        break;
                    default:
                        break;
                }
        }
    }
}

void KGCShopAdvertising::AddParticle( std::string sequenceName, float x, float y, float z, EGCRenderLayer layer )
{
    CParticleEventSeqPTR pParticle = NULL;
    pParticle = g_ParticleManager->CreateSequence( sequenceName, x, y, z );
    if( pParticle )
    {
        g_ParticleManager->SetLayer( pParticle, layer );
        m_vecParticle.push_back( pParticle );
    }
}

void KGCShopAdvertising::ClearPaticles()
{
    for( int i=0; i < (int)m_vecParticle.size(); ++i ){
        g_ParticleManager->DeleteSequence( m_vecParticle[i] );
    }
    
    m_vecParticle.clear();
}

void KGCShopAdvertising::RenderParticle( bool bRender )
{
    for( int i = 0; i < (int)m_vecParticle.size(); ++i )
    {
        if( NULL != m_vecParticle[i] ) { 
            m_vecParticle[i]->SetShow( bRender ) ;
        }
    }
}

KGCShopAdvertising g_kShopAdvertising;