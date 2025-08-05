#include "KGCWeatherSystem.h"

KGCWeather::KGCWeather(void)
{
    m_pWeatherLargeTex = NULL;
    m_pWeatherMiddleTex = NULL;
    m_pWeatherSmallTex = NULL;


    ZeroMemory( m_FrontWeatherVertex, sizeof(SLVERTEX) * 4 );
    ZeroMemory( m_MiddleWeatherVertex, sizeof(SLVERTEX) * 4 );
    ZeroMemory( m_BackWeatherVertex, sizeof(SLVERTEX) * 4 );

    m_fPointX = -1.0f;
    m_fPointZ = 0.5f;

    m_fVertexHeight = 0.0f;
    m_fVertexWidth = 1.0f;

    m_fCurrentCameraX = 0;
    m_fPrevCameraX = 0;

    m_bRender = false;
    m_bAutoRenderOff = false;

    //Init();

}

KGCWeather::~KGCWeather(void)
{
    DeleteDevice();
}

void KGCWeather::Init( KWeatherInfo kInfo_ )
{

    //bool bLoad;
    //bLoad = LoadWeatherInfo();

    m_strLargeTex = kInfo_.m_strLargeTex;
    m_strMiddleTex = kInfo_.m_strMiddleTex;
    m_strSmallTex = kInfo_.m_strSmallTex;
    m_fSpeedLargeTexY = kInfo_.m_fSpeedLargeTexY;
    m_fSpeedMiddleTexY = kInfo_.m_fSpeedMiddleTexY;
    m_fSpeedSmallTexY = kInfo_.m_fSpeedSmallTexY;
    m_fSpeedLargeTexX = kInfo_.m_fSpeedLargeTexX;
    m_fSpeedMiddleTexX = kInfo_.m_fSpeedMiddleTexX;
    m_fSpeedSmallTexX = kInfo_.m_fSpeedSmallTexX;
    m_fbalanceX = kInfo_.m_fbalanceX;       


    m_pWeatherLargeTex = g_pGCDeviceManager2->CreateTexture( m_strLargeTex );
    m_pWeatherMiddleTex = g_pGCDeviceManager2->CreateTexture( m_strMiddleTex );
    m_pWeatherSmallTex = g_pGCDeviceManager2->CreateTexture( m_strSmallTex );

    m_FrontWeatherVertex[0].SetSLVER( m_fPointX, m_fVertexHeight + 1.0f,  m_fPointZ, 0.0f,	0.0f );
    m_FrontWeatherVertex[1].SetSLVER( m_fVertexWidth * 1.5f, m_fVertexHeight + 1.0f,  m_fPointZ, m_fVertexWidth + 1, 0.0f );
    m_FrontWeatherVertex[2].SetSLVER( m_fPointX, m_fVertexHeight - 1.0f, m_fPointZ,	 0.0f,	1.0f );
    m_FrontWeatherVertex[3].SetSLVER( m_fVertexWidth * 1.5f, m_fVertexHeight - 1.0f, m_fPointZ, m_fVertexWidth + 1, 1.0f );


    m_MiddleWeatherVertex[0].SetSLVER( m_fPointX, m_fVertexHeight + 1.0f,  m_fPointZ, 0.0f,	0.0f );
    m_MiddleWeatherVertex[1].SetSLVER( m_fVertexWidth * 2.5f, m_fVertexHeight + 1.0f,  m_fPointZ, m_fVertexWidth + 1, 0.0f );
    m_MiddleWeatherVertex[2].SetSLVER( m_fPointX, m_fVertexHeight - 1.0f, m_fPointZ,	 0.0f,	1.0f );
    m_MiddleWeatherVertex[3].SetSLVER( m_fVertexWidth * 2.5f, m_fVertexHeight - 1.0f, m_fPointZ, m_fVertexWidth + 1, 1.0f );


    m_BackWeatherVertex[0].SetSLVER( m_fPointX, m_fVertexHeight + 1.0f,  m_fPointZ, 0.0f,	0.0f );
    m_BackWeatherVertex[1].SetSLVER( m_fVertexWidth * 2.5f, m_fVertexHeight + 1.0f,  m_fPointZ, m_fVertexWidth + 1, 0.0f );
    m_BackWeatherVertex[2].SetSLVER( m_fPointX, m_fVertexHeight - 1.0f, m_fPointZ,	 0.0f,	1.0f );
    m_BackWeatherVertex[3].SetSLVER( m_fVertexWidth * 2.5f, m_fVertexHeight - 1.0f, m_fPointZ, m_fVertexWidth + 1, 1.0f );

    g_pd3dDevice->GetTransform(D3DTS_WORLD, &m_matWorld);



}

bool KGCWeather::Render( void )
{
    if( m_bRender && !m_bAutoRenderOff ) {

        FrameMove();

        g_pGCDeviceManager2->DrawInWorld(m_pWeatherLargeTex, &m_matWorld, m_FrontWeatherVertex );
        g_pGCDeviceManager2->DrawInWorld(m_pWeatherMiddleTex, &m_matWorld, m_MiddleWeatherVertex );
        g_pGCDeviceManager2->DrawInWorld(m_pWeatherSmallTex, &m_matWorld, m_BackWeatherVertex );

        if( m_kRenderWeatherStopWatch.IsStop() ) {
            m_bRender = false;
            m_kRenderWeatherStopWatch.SetStopCheck( true );
        }

        return true;
    } else {
        return false;
    }

}

void KGCWeather::FrameMove( void )
{
    float m_fCenterX = ( ( g_kCamera.m_rtCamera.left + g_kCamera.m_rtCamera.right ) / 2.0f) * 1000.f;
    int m_iCenterX = static_cast<int>(m_fCenterX);

    m_fCurrentCameraX = static_cast<float>(m_iCenterX);
    m_fCurrentCameraX /= 1000.0f;


    float m_fMoveX = m_fCurrentCameraX - m_fPrevCameraX;

    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        for ( int i = 0; i < 4; i++ )
        {
            m_FrontWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedLargeTexY;
            m_MiddleWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedMiddleTexY;
            m_BackWeatherVertex[i].tv -= GC_ELAPSED_TIME / m_fSpeedSmallTexY;

            if( m_fCurrentCameraX != m_fPrevCameraX ) {
                m_FrontWeatherVertex[i].tu	+= m_fMoveX * m_fSpeedLargeTexX;
                m_MiddleWeatherVertex[i].tu	+= m_fMoveX * m_fSpeedMiddleTexX;
                m_BackWeatherVertex[i].tu +=  m_fMoveX * m_fSpeedSmallTexX;
            } else {    
                m_FrontWeatherVertex[i].tu	+= 0.0f;
                m_MiddleWeatherVertex[i].tu	+= 0.0f;
                m_BackWeatherVertex[i].tu +=  0.0f;
            }
        }
    } else {
        for ( int i = 0; i < 4; i++ )
        {
            m_FrontWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedLargeTexY;
            m_MiddleWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedMiddleTexY;
            m_BackWeatherVertex[i].tv   -= GC_ELAPSED_TIME / m_fSpeedSmallTexY;

            if( m_fCurrentCameraX != m_fPrevCameraX ) {
                m_FrontWeatherVertex[i].tu	+= m_fMoveX * ( m_fSpeedLargeTexX - m_fbalanceX );
                m_MiddleWeatherVertex[i].tu	+= m_fMoveX * ( m_fSpeedMiddleTexX - m_fbalanceX );
                m_BackWeatherVertex[i].tu   += m_fMoveX * ( m_fSpeedSmallTexX  - m_fbalanceX );
            } else {    
                m_FrontWeatherVertex[i].tu	+= 0.0f;
                m_MiddleWeatherVertex[i].tu	+= 0.0f;
                m_BackWeatherVertex[i].tu   += 0.0f;
            }         
        }
    }

    m_fPrevCameraX = m_fCurrentCameraX;    

}

void KGCWeather::DeleteDevice( void )
{
    SAFE_RELEASE( m_pWeatherLargeTex );
    SAFE_RELEASE( m_pWeatherMiddleTex );
    SAFE_RELEASE( m_pWeatherSmallTex );
}

void KGCWeather::SetRender( bool bRender_, DWORD dwTime_ )
{
    m_bRender = bRender_;

    if( bRender_ ) {
        m_kRenderWeatherStopWatch.SetStopWatch( dwTime_ * 1000 );
        m_kRenderWeatherStopWatch.SetStopCheck( false );
    } else {
        m_kRenderWeatherStopWatch.SetStopWatch( 0 );
        m_kRenderWeatherStopWatch.SetStopCheck( true );
    }    
}


/////////////////////////////////////////////////////////////////////////////////////////////////

ImplementSingleton( KGCWeatherSystem )

KGCWeatherSystem::KGCWeatherSystem(void)
{
    LoadWeatherInfo();
}

bool KGCWeatherSystem::LoadWeatherInfo(void )
{
    m_mapWeatherObj.clear();
    KLuaManager     luaMgr;

    if(false == GCFUNC::LoadLuaScript(luaMgr, "WeatherSystemInfo.stg") )
        return false;
    
    LUA_BEGIN_TABLE("WeatherInfo", return false )
    {
        for ( int i = 1; ; ++i)
        {
            KWeatherInfo kWeatherInfo;
            LUA_BEGIN_TABLE(i, break)
            {
                LUA_GET_VALUE_DEF( "fxspeedLargeTexX", kWeatherInfo.m_fSpeedLargeTexX, 0.0f);
                LUA_GET_VALUE_DEF( "fxspeedMiddleTexX", kWeatherInfo.m_fSpeedMiddleTexX, 0.0f);
                LUA_GET_VALUE_DEF( "fxspeedSmallTexX", kWeatherInfo.m_fSpeedSmallTexX, 0.0f);
                LUA_GET_VALUE_DEF( "fspeedLargeTexY", kWeatherInfo.m_fSpeedLargeTexY, 0.0f);
                LUA_GET_VALUE_DEF( "fspeedMiddleTexY", kWeatherInfo.m_fSpeedMiddleTexY, 0.0f);
                LUA_GET_VALUE_DEF( "fsSpeedSmallTexY", kWeatherInfo.m_fSpeedSmallTexY, 0.0f);
                LUA_GET_VALUE_DEF( "LargeTex", kWeatherInfo.m_strLargeTex, "");
                LUA_GET_VALUE_DEF( "MiddleTex", kWeatherInfo.m_strMiddleTex, "");
                LUA_GET_VALUE_DEF( "SmallTex", kWeatherInfo.m_strSmallTex, "");
                LUA_GET_VALUE_DEF( "BalanceX", kWeatherInfo.m_fbalanceX, 0.0f);
            }
            LUA_END_TABLE( return false )

            m_mapWeatherObj[i].Init( kWeatherInfo );
        }
    }
    LUA_END_TABLE(return false )

    return true;
}


KGCWeatherSystem::~KGCWeatherSystem(void)
{
    DeleteDevice();
}

void KGCWeatherSystem::FrameMove( void )
{
    std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.find( m_iWeatherIndex );
    if( mit != m_mapWeatherObj.end() ) {
        mit->second.FrameMove();
    }
    //float m_fCenterX = ( ( g_kCamera.m_rtCamera.left + g_kCamera.m_rtCamera.right ) / 2.0f) * 1000.f;
    //int m_iCenterX = static_cast<int>(m_fCenterX);

    //m_fCurrentCameraX = static_cast<float>(m_iCenterX);
    //m_fCurrentCameraX /= 1000.0f;


    //float m_fMoveX = m_fCurrentCameraX - m_fPrevCameraX;

    //if( SiKGCRoomManager()->IsMonsterGameMode() )
    //{
    //    for ( int i = 0; i < 4; i++ )
    //    {
    //        m_FrontWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedLargeTexY;
    //        m_MiddleWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedMiddleTexY;
    //        m_BackWeatherVertex[i].tv -= GC_ELAPSED_TIME / m_fSpeedSmallTexY;

    //        if( m_fCurrentCameraX != m_fPrevCameraX ) {
    //            m_FrontWeatherVertex[i].tu	+= m_fMoveX * m_fSpeedLargeTexX;
    //            m_MiddleWeatherVertex[i].tu	+= m_fMoveX * m_fSpeedMiddleTexX;
    //            m_BackWeatherVertex[i].tu +=  m_fMoveX * m_fSpeedSmallTexX;
    //        } else {    
    //            m_FrontWeatherVertex[i].tu	+= 0.0f;
    //            m_MiddleWeatherVertex[i].tu	+= 0.0f;
    //            m_BackWeatherVertex[i].tu +=  0.0f;
    //        }
    //    }
    //} else {
    //    for ( int i = 0; i < 4; i++ )
    //    {
    //        m_FrontWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedLargeTexY;
    //        m_MiddleWeatherVertex[i].tv	-= GC_ELAPSED_TIME / m_fSpeedMiddleTexY;
    //        m_BackWeatherVertex[i].tv   -= GC_ELAPSED_TIME / m_fSpeedSmallTexY;

    //        if( m_fCurrentCameraX != m_fPrevCameraX ) {
    //            m_FrontWeatherVertex[i].tu	+= m_fMoveX * ( m_fSpeedLargeTexX - m_fbalanceX );
    //            m_MiddleWeatherVertex[i].tu	+= m_fMoveX * ( m_fSpeedMiddleTexX - m_fbalanceX );
    //            m_BackWeatherVertex[i].tu   += m_fMoveX * ( m_fSpeedSmallTexX  - m_fbalanceX );
    //        } else {    
    //            m_FrontWeatherVertex[i].tu	+= 0.0f;
    //            m_MiddleWeatherVertex[i].tu	+= 0.0f;
    //            m_BackWeatherVertex[i].tu   += 0.0f;
    //        }         
    //    }
    //}

    //m_fPrevCameraX = m_fCurrentCameraX;    

}

void KGCWeatherSystem::DeleteDevice( void )
{
    std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.begin();
    for( mit; mit != m_mapWeatherObj.end(); ++mit ) {
        mit->second.DeleteDevice();
    }
}

void KGCWeatherSystem::SetRender( int iWeatherIndex_, bool bRender_, DWORD dwTime_ )
{
    m_iWeatherIndex = iWeatherIndex_;
    if( iWeatherIndex_ == -1 ) {
        std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.begin();
        for( mit; mit != m_mapWeatherObj.end(); ++mit ) {
            mit->second.SetRender( bRender_, dwTime_ );
        }
    } else {
        std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.find( iWeatherIndex_ );
        if( mit != m_mapWeatherObj.end() ) {
            mit->second.SetRender( bRender_, dwTime_ );
        }
    }    
    
    //m_bRender = bRender_;

    //if( bRender_ ) {
    //    m_kRenderWeatherStopWatch.SetStopWatch( dwTime_ * 1000 );
    //    m_kRenderWeatherStopWatch.SetStopCheck( false );
    //} else {
    //    m_kRenderWeatherStopWatch.SetStopWatch( 0 );
    //    m_kRenderWeatherStopWatch.SetStopCheck( true );
    //}    
}

bool KGCWeatherSystem::Render( void )
{
    std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.find( m_iWeatherIndex );
    if( mit != m_mapWeatherObj.end() ) {
        return mit->second.Render();
    }

    return false;
}

void KGCWeatherSystem::SetAutoRenderOff( bool bRender_ )
{
    std::map< int, KGCWeather >::iterator mit = m_mapWeatherObj.find( m_iWeatherIndex );
    if( mit != m_mapWeatherObj.end() ) {
        return mit->second.SetAutoRenderOff( bRender_ );
    }
}
