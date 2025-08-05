#include "stdafx.h"
#include "GCBossHPBar.h"
#include "./gcui/GCGameOverlayUI.h"
#include "monster.h"
#include "KGCHPGauge.h"

IMPLEMENT_CLASSNAME( KGCBossHPBar );
IMPLEMENT_WND_FACTORY( KGCBossHPBar );
IMPLEMENT_WND_FACTORY_NAME( KGCBossHPBar, "gc_boss_hp_bar" );

KGCBossHPBar::KGCBossHPBar( void )
{
    m_fNowHP        = 0.0f;
    m_fMaxHP        = 0.0f;
    m_bEnable       = false;
    m_fTimeParam    = 0.0f;
    m_fOffset_Start_Y   = -40.0f;
    m_fOffset_End_Y     = 0.0f;
    m_nBossType = EM_BOSS_SIZE;
    m_nMonType = MON_INVALID;

	m_pkHPGauge = NULL;
	m_pkBossImageTexture = NULL;

	LINK_CONTROL( "hp_gauge",        m_pkHPGauge);

    InitBossMon();
}

KGCBossHPBar::~KGCBossHPBar( void )
{
    SAFE_RELEASE( m_pkBossImageTexture );
}

void KGCBossHPBar::OnCreate( void )
{

}

void KGCBossHPBar::InitBossMon()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return;

	if( GCFUNC::LoadLuaScript( luaMgr , "BossHPImageInfo.lua" ) == false )
	{
		return;        
	}

	LUA_BEGIN_TABLE("BossHPImgae",return);
	{
		for(int i = 1; ; i++)
		{
			BossImageHPInfo sBossImageHPInfo;
			LUA_BEGIN_TABLE(i,break)
			{
				LUA_GET_VALUE_DEF( "BOSS_IMAGE", sBossImageHPInfo.nBossImageIndex, 0 );
				LUA_GET_VALUE_DEF( "TEX_NAME", sBossImageHPInfo.strTexName, "" );
				LUA_GET_VALUE_DEF( "TEX_TU1", sBossImageHPInfo.nTexTU1, 0 );
				LUA_GET_VALUE_DEF( "TEX_TV1", sBossImageHPInfo.nTexTV1, 0 );
				LUA_GET_VALUE_DEF( "TEX_TU2", sBossImageHPInfo.nTexTU2, 1 );
				LUA_GET_VALUE_DEF( "TEX_TV2", sBossImageHPInfo.nTexTV2, 1 );

				m_pkBoss[sBossImageHPInfo.nBossImageIndex] = sBossImageHPInfo;
			}
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return)

	LUA_BEGIN_TABLE("BossMonInfo",return);
	{
		for(int i = 1; ; i++)
		{
			LUA_BEGIN_TABLE(i,break)
			{
				int MonType = -1;
				int BossType = -1;

				LUA_GET_VALUE_DEF( 1, MonType, -1 );
				LUA_GET_VALUE_DEF( 2, BossType, 1 );

				m_mapBossHPInfo.insert(std::make_pair(MonType, BossType));
			}
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return)
}

void KGCBossHPBar::FrameMoveInEnabledState( void )
{
    m_fTimeParam += GC_ELAPSED_TIME;
    if ( m_fTimeParam > 1.0f ) m_fTimeParam = 1.0f;
    D3DXVECTOR2 vCurOffset;
    if ( m_bEnable )
    {
        ::D3DXVec2Lerp( &vCurOffset, &D3DXVECTOR2( 0.0f, m_fOffset_End_Y ),
                                     &D3DXVECTOR2( 0.0f, m_fOffset_Start_Y ), m_fTimeParam );
    }
    else
    {
        ::D3DXVec2Lerp( &vCurOffset, &D3DXVECTOR2( 0.0f, m_fOffset_Start_Y ),
                                     &D3DXVECTOR2( 0.0f, m_fOffset_End_Y ), m_fTimeParam );
    }
    vCurOffset.x = static_cast<float>( static_cast<int>( vCurOffset.x ) );
    vCurOffset.y = static_cast<float>( static_cast<int>( vCurOffset.y ) );
    SetCurrentWindowLocalPos( GetFixedWindowLocalPos() + vCurOffset );
	
}

void KGCBossHPBar::PostChildDraw()
{
	if(m_pkBossImageTexture == NULL) return;

	D3DXVECTOR2 vPos( GetRelocatedWindowPos().x + 4.0f, GetRelocatedWindowPos().y + 4.0f );

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->DrawInScreen(m_pkBossImageTexture,
		vPos.x, 
		vPos.y,
		vPos.x + nXSize * m_fWindowScaleX, 
		vPos.y + nYSize * m_fWindowScaleY,
		m_vImageTexStartAxis.x,
		m_vImageTexStartAxis.y, 
		m_vImageTexEndtAxis.x,
		m_vImageTexEndtAxis.y,
		false,0.1f,D3DCOLOR_ARGB(255,255,255,255), false);
	g_pGCDeviceManager2->PopState();
}

int KGCBossHPBar::GetBossIndex( int nBossType )
{
    std::map<int,int>::iterator pos = m_mapBossHPInfo.find( nBossType );

    if ( pos != m_mapBossHPInfo.end() )
        return pos->second;

    return -1;
}

void KGCBossHPBar::HideBossHPBar()
{
    m_bEnable = false;

    if( m_nBossType < 0 || m_nBossType >= EM_BOSS_SIZE )
        return;

    m_nBossType = EM_BOSS_SIZE;
    m_nMonType = MON_INVALID;
}

void KGCBossHPBar::EnableHPBar( bool bEnable, int iMonsterType )
{
    //  모든 UI다 끈다.
    int nBossType = GetBossIndex( iMonsterType );
    
    if ( nBossType == -1 )
    {
	    assert(!"너는 뭐하는 보스인데 기어나왔냐");      
        return;
    }
    else
    {
        //  현재 설정된 보스랑 다르면 동작하지 않는다.
        if ( !bEnable && m_nMonType != iMonsterType ) 
            return;

        m_fTimeParam = 0.0f;

        m_bEnable = bEnable;

        // 기존꺼 끄고 새로 켜준다.
        if ( bEnable ) 
        {
			m_nBossType = nBossType;
			m_nMonType = iMonsterType;

            if ( m_nBossType >= EM_BOSS_GORGOS && m_nBossType < EM_BOSS_SIZE )
            {
				m_pkHPGauge->SetMonID( iMonsterType );

				BossImageHPInfo sBossImageHPInfo = m_pkBoss[nBossType];

				if( sBossImageHPInfo.strTexName.empty() == false )
				{
					m_pkBossImageTexture = g_pGCDeviceManager2->CreateTexture(sBossImageHPInfo.strTexName);

					D3DXVECTOR2 vecTextureFullSize( 512.0f, 512.0f );

					m_vImageTexStartAxis.x = static_cast<float>(sBossImageHPInfo.nTexTU1)/vecTextureFullSize.x;
					m_vImageTexStartAxis.y = static_cast<float>(sBossImageHPInfo.nTexTV1)/vecTextureFullSize.y;

					m_vImageTexEndtAxis.x = static_cast<float>(sBossImageHPInfo.nTexTU2)/vecTextureFullSize.x;
					m_vImageTexEndtAxis.y = static_cast<float>(sBossImageHPInfo.nTexTV2)/vecTextureFullSize.y;

					nXSize = sBossImageHPInfo.nTexTU2 - sBossImageHPInfo.nTexTU1;
					nYSize = sBossImageHPInfo.nTexTV2 - sBossImageHPInfo.nTexTV1;
				}
            }
        }
        else
        {
            m_nBossType = EM_BOSS_SIZE;
            m_nMonType = MON_INVALID;
        }
    }
}

void KGCBossHPBar::RenderOffHPBar()
{
    // 보스타입이 설정안되어 있으면 그냥 끈다.
    if( m_nBossType < 0 || m_nBossType >= EM_BOSS_SIZE )
    {
        ToggleRender( false );
        return;
    }
}

void KGCBossHPBar::SetNowHP( float fNowHP )
{
	if(m_pkHPGauge)
		m_pkHPGauge->SetNowHP( fNowHP );
}

void KGCBossHPBar::SetMaxHP( float fMaxHP )
{
	if(m_pkHPGauge)
		m_pkHPGauge->SetMaxHP( fMaxHP );
}
