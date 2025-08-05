//#include "../stdafx.h"
#include "stdafx.h"
#include ".\KQSLayerManager.h"
//#include "../../GCLuaManager/KLuabinder.h"
//#include "../../KGCLuabinder.h"
//#include "../../GCDeviceLib/GCDeviceManager.h"
//#include <dbg/dbg.hpp>
//#include "GCCameraOrtho.h"
//#include <boost/bind.hpp>
//#include "MyD3D.h"
//#include "Headup Display.h"
//#include "../DlgMapToObject.h"

ImplementSingleton(KQSLayerManager);

KQSLayerManager::KQSLayerManager(void) : KLayerManager<KQSLayerPart, KQSLayer, KQSFantasticMap>( SiKQSFantasticMap() )
{
    //RegisterLuabind();
    g_pRenderState = NULL;
}

KQSLayerManager::~KQSLayerManager(void)
{
}

void KQSLayerManager::RegisterLuabind()
{
    KLayerManager<KQSLayerPart, KQSLayer, KQSFantasticMap>::clear();

    lua_tinker::class_add<KQSLayerManager>( g_kLuabinder.GetLuaState(), "CLayerManager" );
    lua_tinker::class_def<KQSLayerManager>( g_kLuabinder.GetLuaState(), "SetPartsCategoryList", &KQSLayerManager::SetPartsCategoryList);
    lua_tinker::class_def<KQSLayerManager>( g_kLuabinder.GetLuaState(), "AddLayerParts", &KQSLayerManager::AddLayerParts);

    lua_tinker::decl( g_kLuabinder.GetLuaState(), "KQSLayer", this );
    lua_tinker::decl( g_kLuabinder.GetLuaState(), "LayerManager", this );

    std::string strScriptDir = m_strScriptDir;
    strScriptDir += "\\PartsCategoryList.lua";
    if( g_kLuabinder.DoFile( strScriptDir.c_str() ) == E_FAIL )
    {
        //START_LOG( clog, g_kLuabinder.GetLastError() );
        std::string strLastError = g_kLuabinder.GetLastError();
        MessageBox(NULL, _T("g_kLuabinder.DoFile - Stage/PartsCategoryList.lua  == E_FAIL"), _T("알림"), MB_OK);
    }
}

void KQSLayerManager::UnLoad()
{
    std::map< std::string, KQSLayerPart* >::iterator itor;
    for( itor = m_mapPartPool.begin(); itor != m_mapPartPool.end(); ++itor )
    {
        KQSLayerPart* pPart = itor->second;
        SAFE_DELETE( pPart );
    }
    KLayerManager<KQSLayerPart, KQSLayer, KQSFantasticMap>::UnLoad();
    m_mapPartPool.clear();
    m_vecPartCategoryList.clear();
}

void KQSLayerManager::SetLayer()
{
    UnLoad();

    for( int i = 0; i < GC_ZORDER_NUM; ++i )
    {
        m_vecLayer.push_back( new KQSLayer( this ) );
    }
    for( int i = 0; i < (int)m_vecLayer.size(); i++ )
    {
        m_vecLayer[i]->SetLayerNum(i);
    }
}

void KQSLayerManager::FrameMove( float fElapsedTime )
{
    if( m_vecLayer.empty() )
        return;

    std::for_each( m_vecLayer.begin(), m_vecLayer.end(), 
        boost::bind( &KQSLayer::FrameMove, _1, fElapsedTime ) );
}

void KQSLayerManager::SetColor( DWORD dwColorNormal, DWORD dwColorSky )
{
    std::for_each( m_vecLayer.begin(), m_vecLayer.end(),
        boost::bind( &KQSLayer::SetColor, _1, dwColorNormal, dwColorSky ) );
}

bool KQSLayerManager::LoadScript( std::string Script )
{
    m_pMap->RegisterLuabind();

    KLuaManager luaMgr;
    if( luaMgr.DoFile( Script.c_str() ) == E_FAIL )
    {
        ASSERT( !"Load Failed!" && Script.c_str() );
        return false;
    }

    m_pMap->LoadBaseData( luaMgr );
    LoadParts( luaMgr );
    CreateLayer( luaMgr);
    SetLayerSize( luaMgr );
    m_pMap->LoadFootHoldData( luaMgr );

    for( int i = 0; i < (int)m_vecLayer.size(); ++i )
    {
        m_vecLayer[i]->SetZoffset();
    }

    if( !g_pRenderState ) SetRenderState();

    return true;
}

void KQSLayerManager::AlphaRender()
{
    if( m_vecLayer.empty() ) return;

    g_pRenderState->SetTechnique( "RenderState" );

    // Alpha
    g_pRenderState->BeginPass( EGCRENDER_STATE_NEAR_MAP_LINEAR );
    //D3DXMATRIX matWorld;
    //D3DXMatrixIdentity( &matWorld );
    //D3DXMatrixTranslation( &matWorld, -1.0f, -0.55f, 0.0f );
    //D3DXMatrixMultiply( &matWorld, &matWorld, &m_matCamera );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    for( int i = m_iPlayingLayer; i >= 0; --i )
    {
        m_vecLayer[i]->Render();
    }
    g_pRenderState->EndPass();

    //D3DXMatrixIdentity( &matWorld );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

void KQSLayerManager::NoAlphaRender()
{
    g_pRenderState->SetTechnique( "RenderState" );

    // No Alpha
    g_pRenderState->BeginPass( EGCRENDER_STATE_FAR_MAP_LINEAR );
    //D3DXMATRIX matWorld;
    //D3DXMatrixIdentity( &matWorld );
    //D3DXMatrixTranslation( &matWorld, -1.0f, -0.5f, 0.0f );
    //D3DXMatrixMultiply( &matWorld, &matWorld, &m_matCamera );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    for( int i = m_iPlayingLayer+1; i < (int)m_vecLayer.size(); ++i )
    {
        //if( g_MyD3D->MyHead->White_Count ==1 && g_MyD3D->MyHead->White_Count ==2 )
        //    continue;

        if( i == LAST_LAYER )
        {
            //if( !g_MyD3D->m_KGCOption.Is1Back() )
            //    continue;

            //D3DXMATRIX matWorld;
            //D3DXMatrixIdentity( &matWorld );
            //D3DXMatrixTranslation( &matWorld, -1.0f, -0.75f, 0.0f );
            //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
        }
        m_vecLayer[i]->Render();
    }
    g_pRenderState->EndPass();

    //D3DXMatrixIdentity( &matWorld );
    //g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

//void KQSLayerManager::Render()
//{
//    for( int i = 0; i < (int)m_vecLayer.size(); ++i )
//    {
//        SiKGCRenderer()->AddRenderObj( &(*m_vecLayer[i]) );
//    }
//}

void KQSLayerManager::LoadParts( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("PARTS_LIST", return)
    {
        for( int i = 1; ; ++i )
        {
            std::string strFullScriptName = m_strScriptDir;
            std::string strScriptName;
            LUA_GET_VALUE_NOASSERT( i, strScriptName, break );
            strFullScriptName += "\\";
            strFullScriptName += strScriptName;
            if( g_kLuabinder.DoFile( strFullScriptName .c_str() ) == E_FAIL )
                ASSERT( !"LoadParts DoFile Failed!!" );
        }
    }
    LUA_END_TABLE(return);
}

void KQSLayerManager::SetPartsCategoryList()
{
    KLuaManager luaMgr( g_kLuabinder.GetLuaState() );

    m_vecPartCategoryList.clear();

    for( int i = 1; ; ++i )
    {
        std::string strScriptName;
        LUA_GET_VALUE_NOASSERT( i, strScriptName, break );
        m_vecPartCategoryList.push_back( strScriptName );
    }
}

bool KQSLayerManager::LoadFromLua( KLayerPartData& kData )
{
    KLuaManager luaMgr( g_kLuabinder.GetLuaState() );

    LUA_GET_VALUE_DEF( "CATEGORY", kData.PartCategory, "" );
    LUA_GET_VALUE_DEF( "PARTNAME", kData.PartName, "" );
    LUA_GET_VALUE_DEF( "TEX_NAME", kData.TexName, "" );
    LUA_GET_VALUE_DEF( "TEX_X", kData.kTexRect.m_X, 0 );
    LUA_GET_VALUE_DEF( "TEX_Y", kData.kTexRect.m_Y, 0 );
    LUA_GET_VALUE_DEF( "TEX_WIDTH", kData.kTexRect.m_Width, 0 );
    LUA_GET_VALUE_DEF( "TEX_HEIGHT", kData.kTexRect.m_Height, 0 );
    LUA_GET_VALUE_DEF( "PARTS_WIDTH", kData.kLayerRect.m_Width, static_cast<float>(kData.kTexRect.m_Width) );
    LUA_GET_VALUE_DEF( "PARTS_HEIGHT", kData.kLayerRect.m_Height, static_cast<float>(kData.kTexRect.m_Height) );
    
    // 툴에서는 픽셀좌표를 사용하므로 400을 곱해 픽셀좌표로 변환.
    kData.kLayerRect.m_Width        *= 400.0f;
    kData.kLayerRect.m_Height       *= 400.0f;

    std::map< std::string, KQSLayerPart* >::iterator mit = m_mapPartPool.find( kData.PartName );
    if( mit != m_mapPartPool.end() )
    {
        AddLayerPartsErr( kData.PartName );
        return false;
    }

    return true;
}

void KQSLayerManager::SetLayerSize( KLuaManager& luaMgr )
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
                m_vecLayer[iLayerNum]->SetLayerXSize( fSizeX * 400.0f );
                m_vecLayer[iLayerNum]->SetLayerYSize( fSizeY * 400.0f );
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void KQSLayerManager::SetCameraMatrix(D3DXMATRIX matCamera)
{
    m_matCamera = matCamera;
}

void KQSLayerManager::SetScriptDirectory(std::string strScriptDir)
{
    m_strScriptDir = strScriptDir;
    RegisterLuabind();
}

void KQSLayerManager::SetRenderState(void)
{
    char szPath[MAX_PATH];
    SetCurrentDirectoryA( szPath );
    std::string strShader = szPath;
    strShader += "/Data/Shader/RenderState.fx";
    g_pRenderState = g_pGCDeviceManager->CreateShader( strShader.c_str() );
    g_pRenderState->SetTechnique( "RenderState" );
}

void KQSLayerManager::SetCameraRectToQSLayer(float fCMLeft, float fCMRight, float fCMTop, float fCMBottom)
{
    for(int i=0;i<(int)m_vecLayer.size();++i)
    {
        m_vecLayer[i]->SetShowRect(fCMLeft, fCMRight, fCMTop, fCMBottom);
    }
}

void KQSLayerManager::SetCameraPosToQSLayer(float fLookX, float LookY)
{
    for(int i=0;i<(int)m_vecLayer.size();++i)
    {
        m_vecLayer[i]->SetCameraPos(fLookX, LookY);
    }
}

void KQSLayerManager::AddLayerPart( KQSLayerPart* pPart ) 
{ 
    if( pPart != NULL )
    {
        m_mapPartPool[pPart->GetPartName()] = pPart;
    }
}
