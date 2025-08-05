#include "StdAfx.h"
#include "TGWeather.h"

#include "MyD3D.h"

CTGWeather::CTGWeather(void)
{
    enType              = TG_WEATHER;
    m_iWeatherID        = WEATHER_NONE;
    InitData();
}

CTGWeather::~CTGWeather(void)
{
}

void CTGWeather::InitData(void)
{
    m_fWidth            = 0.0f;
    m_fHeight           = 0.0f;
    m_fEntranceX        = 0.0f;
    m_fEntranceY        = 0.0f;

    m_fWindSpeedX       = 0.0f;
    m_fWindSpeedY       = 0.0f;
    m_fJump_WindRadio   = 1.0f;

    m_vecParticle.clear();

    m_bBeginTG          = false;
}

void CTGWeather::StartParticle()
{
    std::vector<WEATHER_PARTICLE>::iterator vecIter = m_vecParticle.begin();
    for(; vecIter != m_vecParticle.end(); ++vecIter )
    {
        g_ParticleManager->CreateSequence( vecIter->m_strSeq, vecIter->m_vPos.x, vecIter->m_vPos.y, 1.0f );
    }
}

void CTGWeather::FrameMove(void)
{
    if( IsComplete() == true ) return;
	
    if( false == m_bBeginTG )
    {
        StartParticle();
        m_bBeginTG = true;
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        CheckEffect();
        if( !m_bSendPacket )
        {
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        CheckEffect();
    }

    //g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );
}

void CTGWeather::CheckEffect()
{
    switch( m_iWeatherID )
    {
    case WEATHER_WIND:
        CheckWind();
        break;
    }
}

void CTGWeather::CheckWind()
{
    if( 0.0f != m_fWindSpeedX || 0.0f != m_fWindSpeedY )
    {
        GCCollisionRect<float> fWindRect;
        fWindRect.SetRect( m_fEntranceX, m_fEntranceY, m_fEntranceX+m_fWidth, m_fEntranceY+m_fHeight );
        fWindRect.AdjustRect();

        // 아래 MAX_PLAYER 루프를 안돌게 하려고 추가했음.
        int iMyPlayer = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iMyPlayer) )
        {
            if( 0 >= g_MyD3D->MyPlayer[iMyPlayer]->m_cLife )
                return;

            if( false == g_MyD3D->MyPlayer[iMyPlayer]->IsLive() )
                return;

            if( PS_DEAD == g_MyD3D->MyPlayer[iMyPlayer]->GetPlayerState() )
                return;

            GCCollisionRect<float>& kPlayerBody = g_MyD3D->MyPlayer[iMyPlayer]->GetCollisionRect();
            GCCollisionRect<float> kCollision;
            kPlayerBody.m_Top -= 0.15f;
            kPlayerBody.AdjustRect();   
            if( kPlayerBody.CheckCollision( fWindRect, &kCollision ) )
            {
                g_MyD3D->MyPlayer[iMyPlayer]->x_WindSpeed = m_fWindSpeedX;
                g_MyD3D->MyPlayer[iMyPlayer]->y_WindSpeed = m_fWindSpeedY;
                g_MyD3D->MyPlayer[iMyPlayer]->m_fJump_WindRadio = m_fJump_WindRadio;
            }
        }

        //for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        //{
        //    if( false == g_MyD3D->MyPlayer[iLoop]->IsLocalPlayer() )
        //        continue;

        //    if( 0 >= g_MyD3D->MyPlayer[iLoop]->m_cLife )
        //        continue;

        //    if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
        //        continue;

        //    if( PS_DEAD == g_MyD3D->MyPlayer[iLoop]->GetPlayerState() )
        //        continue;

        //    GCCollisionRect<float>& kPlayerBody = g_MyD3D->MyPlayer[iLoop]->GetCollisionRect();
        //    GCCollisionRect<float> kCollision;
        //    kPlayerBody.m_Top -= 0.15f;
        //    kPlayerBody.AdjustRect();   
        //    if( kPlayerBody.CheckCollision( fWindRect, &kCollision ) )
        //    {
        //        g_MyD3D->MyPlayer[iLoop]->x_WindSpeed = m_fWindSpeedX;
        //        g_MyD3D->MyPlayer[iLoop]->y_WindSpeed = m_fWindSpeedY;
        //        g_MyD3D->MyPlayer[iLoop]->m_fJump_WindRadio = m_fJump_WindRadio;
        //    }
        //}
    }

#if !defined( __PATH__ ) // 바람영역출력
    {
        if( g_kGlobalValue.m_bRenderBoundBox )
        {
            GCCollisionRect<float> fRect;
            fRect.SetRect( m_fEntranceX, m_fEntranceY, m_fEntranceX+m_fWidth, m_fEntranceY+m_fHeight );
            fRect.AdjustRect();

            D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
            SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top, 0.6f );
            SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top, 0.6f );
            SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom, 0.6f );
            SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom, 0.6f );
            g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
                lefttop, righttop, leftbottom, rightbottom,0,0,0,
                D3DCOLOR_ARGB(180, 200, 0, 0));
        }
    }
#endif
}

HRESULT CTGWeather::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("WeatherID", m_iWeatherID, return E_FAIL);

    InitData();
    switch( m_iWeatherID )
    {
    case WEATHER_WIND:
        LUA_GET_VALUE_DEF( "Width", m_fWidth, 0.0f );
        LUA_GET_VALUE_DEF( "Height", m_fHeight, 0.0f );
        LUA_GET_VALUE_DEF( "EntranceX", m_fEntranceX, 0.0f );
        LUA_GET_VALUE_DEF( "EntranceY", m_fEntranceY, 0.0f );
        LUA_GET_VALUE_DEF( "WindSpeedX", m_fWindSpeedX, 0.0f );
        LUA_GET_VALUE_DEF( "WindSpeedY", m_fWindSpeedY, 0.0f );
        LUA_GET_VALUE_DEF( "JumpWindRadio", m_fJump_WindRadio, 0.0f );
        
        break;
    }
	
    LUA_BEGIN_TABLE_RAII( "WeatherParticle" )
    {
        int iLoop=1;
        while( true )
        {
            WEATHER_PARTICLE kParticle;
            SDotaMonsterPortalInfo sMonsterPortalInfo;
            if( SUCCEEDED(luaMgr.BeginTable( iLoop )) )
            {
                LUA_GET_VALUE_DEF( 1, kParticle.m_strSeq, "" );
                LUA_GET_VALUE_DEF( 2, kParticle.m_vPos.x, 0.0f );
                LUA_GET_VALUE_DEF( 3, kParticle.m_vPos.y, 0.0f );
            }
            else
                break;
            luaMgr.EndTable();

            ++iLoop;
            m_vecParticle.push_back( kParticle );   
        }
    }

    return S_OK;
}
