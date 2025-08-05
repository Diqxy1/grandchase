#include "stdafx.h"
#include ".\kgclayermanager.h"
//

#include "GCCameraOrtho.h"
//


//

ImplementSingleton(KGCLayerManager);

KGCLayerManager::KGCLayerManager(void) : KLayerManager<KGCLayerPart, KGCLayer, KGCFantasticMap>( SiKGCFantasticMap() )
{
    RegisterLuabind();
}

KGCLayerManager::~KGCLayerManager(void)
{
}

void KGCLayerManager::RegisterLuabind()
{
    KLayerManager<KGCLayerPart, KGCLayer, KGCFantasticMap>::clear();

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCLayerManager>( L, "CLayerManager" );
        lua_tinker::class_def<KGCLayerManager>( L, "SetPartsCategoryList",  &KGCLayerManager::SetPartsCategoryList );
        lua_tinker::class_def<KGCLayerManager>( L, "AddLayerParts",  &KGCLayerManager::AddLayerParts );
        ;

    lua_tinker::class_add<KGCLayer>( L, "KGCLayer" );
        ;

    lua_tinker::decl( L, "LayerManager", this );

	if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "PartsCategoryList.lua") == false )
    {
        ASSERT( !"KGCLuabinder::getInstance().DoFile( ""Stage/PartsCategoryList.lua"" ) == E_FAIL" );
    }
}

void KGCLayerManager::UnLoad()
{
    std::map< std::string, KGCLayerPart* >::iterator itor;
    for( itor = m_mapPartPool.begin(); itor != m_mapPartPool.end(); ++itor )
    {
        KGCLayerPart* pPart = itor->second;
        SAFE_DELETE( pPart );
    }
    KLayerManager<KGCLayerPart, KGCLayer, KGCFantasticMap>::UnLoad();
    m_mapPartPool.clear();
    m_vecPartCategoryList.clear();
}

void KGCLayerManager::SetLayer()
{
    UnLoad();

    for( int i = 0; i < GC_ZORDER_NUM; ++i )
    {
        m_vecLayer.push_back( new KGCLayer( this ) );
    }
    for( int i = 0; i < (int)m_vecLayer.size(); i++ )
    {
        m_vecLayer[i]->SetLayerNum(i);
    }
}

void KGCLayerManager::FrameMove( float fElapsedTime )
{
    if( m_vecLayer.empty() )
        return;

    std::for_each( m_vecLayer.begin(), m_vecLayer.end(), 
        boost::bind( &KGCLayer::FrameMove, _1, fElapsedTime ) );
}

void KGCLayerManager::SetColor( DWORD dwColorNormal, DWORD dwColorSky )
{
    std::for_each( m_vecLayer.begin(), m_vecLayer.end(),
        boost::bind( &KGCLayer::SetColor, _1, dwColorNormal, dwColorSky ) );
}

bool KGCLayerManager::LoadScript( std::string& Script )
{
    m_pMap->RegisterLuabind();

    KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, Script );

    m_pMap->LoadBaseData( luaMgr );
    LoadParts( luaMgr );
    CreateLayer( luaMgr);
    SetLayerSize( luaMgr );
    m_pMap->LoadFootHoldData( luaMgr );

    for( int i = 0; i < (int)m_vecLayer.size(); ++i )
    {
        m_vecLayer[i]->SetZoffset();
    }

    return true;
}

void KGCLayerManager::AlphaRender()
{
    if( m_vecLayer.empty() )
        return;

    g_pRenderState->SetTechnique( "RenderState" );

    // Alpha
    if( g_MyD3D->m_KGCOption.IsLinearFilter() )
    {
        g_pRenderState->BeginPass( EGCRENDER_STATE_NEAR_MAP_LINEAR );
    }
    else
    {
        g_pRenderState->BeginPass( EGCRENDER_STATE_NEAR_MAP_POINT );
    }
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    D3DXMatrixTranslation( &matWorld, -1.0f, -0.5f, 0.0f );
    //D3DXMatrixMultiply( &matWorld, &matWorld, &g_kCamera.m_matCamera );
   // g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    for( int i = m_iPlayingLayer; i >= 0; --i )
    {
        m_vecLayer[i]->SetStaticCamera( false );
        m_vecLayer[i]->Render(&matWorld);
    }

	g_pGCDeviceManager2->Flush();
    g_pRenderState->EndPass();

   // D3DXMatrixIdentity( &matWorld );
   // g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

void KGCLayerManager::NoAlphaRender( int iLayerNum_ )
{
    g_pRenderState->SetTechnique( "RenderState" );
    // No Alpha
    if( g_MyD3D->m_KGCOption.IsLinearFilter() )
    {
        g_pRenderState->BeginPass( EGCRENDER_STATE_FAR_MAP_LINEAR );
    }
    else
    {
        g_pRenderState->BeginPass( EGCRENDER_STATE_FAR_MAP_POINT );
    }
    
    D3DXMATRIX matWorld, matTemp;
    D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matTemp );

    if ( GC_LAYER_SKY == iLayerNum_ )
    {
		D3DXMatrixTranslation( &matWorld, -1.0f, -0.75f, 0.0f );

        if ( m_vecLayer.size() >= LAST_LAYER + 1)
        {
            m_vecLayer[LAST_LAYER]->SetStaticCamera( true );
            m_vecLayer[LAST_LAYER]->Render(&matWorld );
        }
    }
	else if ( iLayerNum_ >= GC_LAYER_SCROLLED_LAYER1 && iLayerNum_ <= GC_LAYER_SCROLLED_LAYER3  )
	{
		D3DXMatrixTranslation( &matWorld, -1.0f, -0.5f, 0.0f );
		//D3DXMatrixMultiply( &matWorld, &matWorld, &g_kCamera.m_matCamera );
		if ( static_cast<int>( m_vecLayer.size() ) >= LAST_LAYER - iLayerNum_ + 1 )
		{
            m_vecLayer[LAST_LAYER - iLayerNum_]->SetStaticCamera( false );
			m_vecLayer[LAST_LAYER - iLayerNum_]->Render(&matWorld );
		}
	}
    else
    {
        D3DXMatrixTranslation( &matWorld, -1.0f, -0.5f, 0.0f );
        //D3DXMatrixMultiply( &matWorld, &matWorld, &g_kCamera.m_matCamera );
        //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

        for( int i = m_iPlayingLayer+6; i >= m_iPlayingLayer+1; --i )
        {
            if ( i >= static_cast<int> ( m_vecLayer.size()) || i < GC_LAYER_SKY )
                continue;
		
            m_vecLayer[i]->SetStaticCamera( false );
            m_vecLayer[i]->Render(&matWorld );
        }
    }

	g_pGCDeviceManager2->Flush();
    g_pRenderState->EndPass();

    //D3DXMatrixIdentity( &matWorld );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

//void KGCLayerManager::Render()
//{
//    for( int i = 0; i < (int)m_vecLayer.size(); ++i )
//    {
//        SiKGCRenderer()->AddRenderObj( &(*m_vecLayer[i]) );
//    }
//}

void KGCLayerManager::LoadParts( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("PARTS_LIST", return)
    {
        for( int i = 1; ; ++i )
        {
            std::string strScriptName;
            LUA_GET_VALUE_NOASSERT( i, strScriptName, break );
            if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strScriptName ) == false )
            {
                ASSERT( !"LoadParts DoFile Failed!!" );
            }
        }
    }
    LUA_END_TABLE(return);
}

void KGCLayerManager::SetPartsCategoryList()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    KLuaManager luaMgr( L );

    m_vecPartCategoryList.clear();

    for( int i = 1; ; ++i )
    {
        std::string strScriptName;
        LUA_GET_VALUE_NOASSERT( i, strScriptName, break );
        m_vecPartCategoryList.push_back( strScriptName );
    }
}

bool KGCLayerManager::LoadFromLua( KLayerPartData& kData )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    KLuaManager luaMgr( L );

    LUA_GET_VALUE_DEF( "CATEGORY", kData.PartCategory, "" );
    LUA_GET_VALUE_DEF( "PARTNAME", kData.PartName, "" );
    LUA_GET_VALUE_DEF( "TEX_NAME", kData.TexName, "" );
    LUA_GET_VALUE_DEF( "TEX_X", kData.kTexRect.m_X, 0 );
    LUA_GET_VALUE_DEF( "TEX_Y", kData.kTexRect.m_Y, 0 );
    LUA_GET_VALUE_DEF( "TEX_WIDTH", kData.kTexRect.m_Width, 0 );
    LUA_GET_VALUE_DEF( "TEX_HEIGHT", kData.kTexRect.m_Height, 0 );
    LUA_GET_VALUE_DEF( "PARTS_WIDTH", kData.kLayerRect.m_Width, static_cast<float>(kData.kTexRect.m_Width) );
    LUA_GET_VALUE_DEF( "PARTS_HEIGHT", kData.kLayerRect.m_Height, static_cast<float>(kData.kTexRect.m_Height) );


    // 전장 배경 처리용 코드 추가 
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        std::string strTex( kData.TexName );
        GCUTIL_STR::ToUpper( strTex );
        if( std::string::npos != strTex.find( "SKY" ) ||
            std::string::npos != strTex.find( "_BG") ) { 
            
            for( int iTempIdx=0 ; iTempIdx < 3 ; ++iTempIdx ) { 
                KLayerParticleData kParticle;
                char strParticleName[128];
                sprintf_s( strParticleName, 127, "Infinity_Dungeon_Back_%02d", iTempIdx+1 );
                
                kParticle.strParticleName = strParticleName;
                kParticle.fOffsetX = 0.0f;
                kParticle.fOffsetY = 0.0f;

                kData.vecParticle.push_back( kParticle );
            }
        }
    }

    LUA_BEGIN_TABLE( "PARTICLE", goto PARTICLE_LOAD_END)
    {
        for ( int i = 1 ; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                KLayerParticleData kParticle;
                LUA_GET_VALUE_DEF( "PARTICLE_NAME", kParticle.strParticleName, "" );
                LUA_GET_VALUE_DEF( "OFFSET_X", kParticle.fOffsetX, 0.0f );
                LUA_GET_VALUE_DEF( "OFFSET_Y", kParticle.fOffsetY, 0.0f );
                kData.vecParticle.push_back( kParticle );
            }
            LUA_END_TABLE( break )
        }        
    }
    LUA_END_TABLE( ; )

PARTICLE_LOAD_END:

    std::map< std::string, KGCLayerPart* >::iterator mit = m_mapPartPool.find( kData.PartName );
    if( mit != m_mapPartPool.end() )
    {
        AddLayerPartsErr( kData.PartName );
        return false;
    }

    return true;
}

void KGCLayerManager::SetLayerSize( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("LAYER_SIZE_DATA", return)
    {
        for( int i = 1; ; ++i )
        {
            LUA_BEGIN_TABLE(i, break)
            {
                int iLayerNum;
                float fSizeX, fSizeY;
                LUA_GET_VALUE(1, iLayerNum, break);
                LUA_GET_VALUE(2, fSizeX, break);
                LUA_GET_VALUE(3, fSizeY, break);

                m_vecLayer[iLayerNum]->SetLayerNum( iLayerNum );
                m_vecLayer[iLayerNum]->SetLayerXSize( fSizeX );
                m_vecLayer[iLayerNum]->SetLayerYSize( fSizeY );
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void KGCLayerManager::SetLayerMoveSpeed( int iLayer, char *szPartName, float fSpeedX, float fSpeedY )
{
    if( iLayer >= static_cast<int>(m_vecLayer.size()) )
        return;

    int iMaxSize = static_cast<int>(m_vecLayer[iLayer]->m_vecPart.size());
    for(int iLoop=0; iLoop<iMaxSize; ++iLoop)
    {
        std::string szPartName = m_vecLayer[iLayer]->m_vecPart[iLoop]->GetPartName();
        if( szPartName == szPartName )
        {
            m_vecLayer[iLayer]->m_vecPart[iLoop]->SetMoveProp( true, fSpeedX, fSpeedY );
            break;
        }
    }
}

float KGCLayerManager::GetLayerMoveSpeedX( int iLayer, char *szPartName )
{
    if( iLayer >= static_cast<int>(m_vecLayer.size()) )
        return 0.0f;

    int iMaxSize = static_cast<int>(m_vecLayer[iLayer]->m_vecPart.size());
    for(int iLoop=0; iLoop<iMaxSize; ++iLoop)
    {
        std::string szPartName = m_vecLayer[iLayer]->m_vecPart[iLoop]->GetPartName();
        if( szPartName == szPartName )
        {
            bool bMoveable;
            float fSpeedX, fSpeedY;
            m_vecLayer[iLayer]->m_vecPart[iLoop]->GetMoveProp( bMoveable, fSpeedX, fSpeedY );
            return fSpeedX;
        }
    }
    return 0.0f;
}

float KGCLayerManager::GetLayerMoveSpeedY( int iLayer, char *szPartName )
{
    if( iLayer >= static_cast<int>(m_vecLayer.size()) )
        return 0.0f;

    int iMaxSize = static_cast<int>(m_vecLayer[iLayer]->m_vecPart.size());
    for(int iLoop=0; iLoop<iMaxSize; ++iLoop)
    {
        std::string szPartName = m_vecLayer[iLayer]->m_vecPart[iLoop]->GetPartName();
        if( szPartName == szPartName )
        {
            bool bMoveable;
            float fSpeedX, fSpeedY;
            m_vecLayer[iLayer]->m_vecPart[iLoop]->GetMoveProp( bMoveable, fSpeedX, fSpeedY );
            return fSpeedY;
        }
    }
    return 0.0f;
}