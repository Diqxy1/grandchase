#include "stdafx.h"
#include "./KGCPrograssBar.h"
#include "../KGCEmbarkManager.h"

const float	KGCPrograssBar::m_fCheckDistance = 200.0f;

KGCPrograssBar::KGCPrograssBar() 
: m_pBackTex( NULL )
, m_pBarTex( NULL )
, m_pAddBackTex( NULL )
, m_pAddFrontTex( NULL )
, m_dx4Pos(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
, m_dx4PosToBarOffset(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
, m_dx4AddFrontTexUV(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
, m_dx4AddFrontTexPos(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
, m_iMinValue(0)
, m_iMaxValue(0)
, m_iNowValue(0)
, m_iTargetType( TARGET_UI )
, m_iTargetUnitIndex( 0 )
, m_iLayer( GC_LAYER_UI )
, m_iCoolTime( 0 )
, m_iNowTime( 0 )
, m_iAccValue( 0 )
, m_bIsRender( true )
, m_bIsAddFrontTexRender( true )
, m_bIsAddBackTexRender( true )
, m_iBarAlpha( 255 )
, m_iBackAlpha( 255 )
, m_iAddTexFrontAlpha( 255 )
, m_iAddTexBackAlpha( 255 )
, m_fAddTexFrontScale( 0.0f )
, m_fAddTexBackScale( 0.0f )
, m_vNowPosAni( 0.0f, 0.0f )
, m_iString( -1 )
, m_bVertical( false )
, m_bVerticalEx( false )
{
    m_dwAddTexFrontColor = D3DCOLOR_ARGB(0, 255, 255, 255);
    m_dwAddTexBackColor = D3DCOLOR_ARGB(0, 255, 255, 255);

    for(int iLoop=0; iLoop<PG_NUM; ++iLoop)
    {
        m_dx4BGUV[iLoop] = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
        m_dx4BarUV[iLoop] = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
    }

	m_iNowState = NORMAL;
	m_vHidTargetPos = D3DXVECTOR2(0.0f,0.0f);
	m_vOpenTargetPos = D3DXVECTOR2(0.0f,0.0f);
	m_fStateAnitime = 0.0f;
    m_mapParticle.clear();
}

KGCPrograssBar::~KGCPrograssBar()
{
    ClearAllResource();
}

void KGCPrograssBar::ClearAllResource()
{
    SAFE_RELEASE( m_pBackTex );
    SAFE_RELEASE( m_pBarTex );
    SAFE_RELEASE( m_pAddBackTex );
    SAFE_RELEASE( m_pAddFrontTex );
    ClearParticle();
}

void KGCPrograssBar::ClearParticle()
{
    std::map<int, CParticleEventSeqPTR>::iterator mapIter = m_mapParticle.begin();
    while( mapIter != m_mapParticle.end() )
    {
        g_ParticleManager->DeleteSequence( mapIter->second );
        mapIter->second = NULL;
        mapIter = m_mapParticle.erase( mapIter );
    }
}

void KGCPrograssBar::SetDefaultTextureToScreen()
{
    SetBackTexture("EnergyBarBack.dds");                // 텍스쳐 셋팅
    SetBGFrontUV( 0.0f, 0.0f, 54.0f, 64.0f );           // Back Front UV
    SetBGCenterUV( 54.0f, 0.0f, 59.0f, 64.0f );         // Back Center UV
    SetBGBackUV( 59.0f, 0.0f, 64.0f, 64.0f );           // Back Back UV
    SetBarTexture("Energybar.dds");                     // 텍스쳐 셋팅
    SetBarFrontUV( 0.0f, 0.0f, 54.0f, 64.0f );          // Bar Front UV
    SetBarCenterUV( 54.0f, 0.0f, 59.0f, 64.0f );        // Bar Center UV
    SetBarBackUV( 59.0f, 0.0f, 64.0f, 64.0f );          // Bar Back UV
}

void KGCPrograssBar::SetDefaultTextureToWorld()
{
    SetBackTexture("EnergyBarBack.dds");                // 텍스쳐 셋팅
    SetBGFrontUV( 0.0f, 0.0f, 64.0f, 64.0f );           // Back Front UV
    SetBarTexture("Energybar.dds");                     // 텍스쳐 셋팅
    SetBarFrontUV( 0.0f, 0.0f, 64.0f, 64.0f );          // Bar Front UV
}

void KGCPrograssBar::SetBackTexture(char *szTexName)
{
    if( 0 >= static_cast<int>(strlen( szTexName )) )
        return;

    SAFE_RELEASE( m_pBackTex );
    m_pBackTex = g_pGCDeviceManager2->CreateTexture( szTexName );
}

void KGCPrograssBar::SetBarTexture(char *szTexName)
{
    if( 0 >= static_cast<int>(strlen( szTexName )) )
        return;

    SAFE_RELEASE( m_pBarTex );
    m_pBarTex = g_pGCDeviceManager2->CreateTexture( szTexName );
}

void KGCPrograssBar::SetAddFrontTexture(char *szTexName)
{
    if( 0 >= static_cast<int>(strlen( szTexName )) )
        return;

    SAFE_RELEASE( m_pAddFrontTex );
    m_pAddFrontTex = g_pGCDeviceManager2->CreateTexture( szTexName );
}

void KGCPrograssBar::SetAddBackTexture(char *szTexName)
{
    if( 0 >= static_cast<int>(strlen( szTexName )) )
        return;

    SAFE_RELEASE( m_pAddBackTex );
    m_pAddBackTex = g_pGCDeviceManager2->CreateTexture( szTexName );
}

void KGCPrograssBar::SetRenderNumber( bool bRender, char *szType, DWORD dwNumber, float fOffsetX, float fOffsetY, DWORD dwColor )
{
    m_bIsRenderNumber = bRender;
    if( bRender )
    {
        m_kNumberInfo.strType = szType;
        m_kNumberInfo.dwNumber = dwNumber;
        m_kNumberInfo.fOffsetX = fOffsetX;
        m_kNumberInfo.fOffsetY = fOffsetY;
        m_kNumberInfo.dwColor = dwColor;
    }
}

void KGCPrograssBar::AddParticle( int iID, char *strSeqeunce, float fOffsetX, float fOffsetY )
{
    if( 0 == static_cast<int>(strlen( strSeqeunce )) )
        return;

    std::map<int, CParticleEventSeqPTR>::iterator mapIter = m_mapParticle.find( iID );
    if( mapIter != m_mapParticle.end() )
    {
        g_ParticleManager->DeleteSequence( mapIter->second );
        mapIter->second = NULL;
        m_mapParticle.erase( mapIter );
    }

    CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( strSeqeunce );

    if( pParticle )
    {
        if( (m_iLayer == GC_LAYER_UI) )
        {
            TRACE_INFO stTrace;
            stTrace.m_pvPos = &g_MyD3D->MyHead->m_vCameraLookPos;
            stTrace.m_fXOffset = fOffsetX;
            stTrace.m_fYOffset = fOffsetY;
            stTrace.m_fParticleTraceTime = 5.0f;
            pParticle->SetTrace( &stTrace );
        }
        else
        {
            TRACE_INFO stTrace;
            stTrace.m_pvPos = &m_dx3ParticleTracePos;
            stTrace.m_fXOffset = fOffsetX;
            stTrace.m_fYOffset = fOffsetY;
            stTrace.m_fParticleTraceTime = 0.5f;
            pParticle->SetTrace( &stTrace );
        }
        
        pParticle->SetShow( &m_bIsRender );
        m_mapParticle.insert( std::make_pair( iID, pParticle) );
    }
}

void KGCPrograssBar::DeleteParticle( int iID )
{
    std::map<int, CParticleEventSeqPTR>::iterator mapIter = m_mapParticle.find( iID );
    if( mapIter != m_mapParticle.end() )
    {
        g_ParticleManager->DeleteSequence( mapIter->second );
        mapIter->second = NULL;
        m_mapParticle.erase( mapIter );
    }
}

void KGCPrograssBar::SetBarAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni )
{
    if( 0 >= m_kBarAlphaAni.iKeepFrame )
    {
        m_kBarAlphaAni.fMinLimit = fMinLimit;
        m_kBarAlphaAni.fMaxLimit = fMaxLimit;
        m_kBarAlphaAni.fAniSpeed = fAniSpeed;
        m_kBarAlphaAni.bReturnAni = bReturnAni;
    }
    
    m_kBarAlphaAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetBackAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni )
{
    if( 0 >= m_kBackAlphaAni.iKeepFrame )
    {
        m_kBackAlphaAni.fMinLimit = fMinLimit;
        m_kBackAlphaAni.fMaxLimit = fMaxLimit;
        m_kBackAlphaAni.fAniSpeed = fAniSpeed;
        m_kBackAlphaAni.bReturnAni = bReturnAni;
    }
    m_kBackAlphaAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetAddTexFrontAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni )
{
    if( 0 >= m_kAddTexFrontAlphaAni.iKeepFrame )
    {
        m_kAddTexFrontAlphaAni.fMinLimit = fMinLimit;
        m_kAddTexFrontAlphaAni.fMaxLimit = fMaxLimit;
        m_kAddTexFrontAlphaAni.fAniSpeed = fAniSpeed;
        m_kAddTexFrontAlphaAni.bReturnAni = bReturnAni;
    }

    m_kAddTexFrontAlphaAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetAddTexBackAlphaAni( float fMinLimit, float fMaxLimit, float fAniSpeed, int iKeepFrame, bool bReturnAni )
{
    if( 0 >= m_kAddTexBackAlphaAni.iKeepFrame )
    {
        m_kAddTexBackAlphaAni.fMinLimit = fMinLimit;
        m_kAddTexBackAlphaAni.fMaxLimit = fMaxLimit;
        m_kAddTexBackAlphaAni.fAniSpeed = fAniSpeed;
        m_kAddTexBackAlphaAni.bReturnAni = bReturnAni;
    }
    m_kAddTexBackAlphaAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetAddTexFrontScaleAni( int iKeepFrame, float fMinLimit, float fMaxLimit, float fAniSpeed, int iSoundID, bool bReturnAni )
{
    if( 0 >= m_kAddTexFrontScaleAni.iKeepFrame )
    {
        m_kAddTexFrontScaleAni.fMinLimit = fMinLimit;
        m_kAddTexFrontScaleAni.fMaxLimit = fMaxLimit;
        m_kAddTexFrontScaleAni.fAniSpeed = fAniSpeed;
        m_kAddTexFrontScaleAni.iSoundID = iSoundID;
        m_kAddTexFrontScaleAni.bReturnAni = bReturnAni;
    }

    m_kAddTexFrontScaleAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetAddTexBackScaleAni( int iKeepFrame, float fMinLimit, float fMaxLimit, float fAniSpeed, int iSoundID, bool bReturnAni )
{
    if( 0 >= m_kAddTexBackScaleAni.iKeepFrame )
    {
        m_kAddTexBackScaleAni.fMinLimit = fMinLimit;
        m_kAddTexBackScaleAni.fMaxLimit = fMaxLimit;
        m_kAddTexBackScaleAni.fAniSpeed = fAniSpeed;
        m_kAddTexBackScaleAni.iSoundID = iSoundID;
        m_kAddTexBackScaleAni.bReturnAni = bReturnAni;
    }
    m_kAddTexBackScaleAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::SetAddTexFrontColorAni( PG_COLOR_ANIINFO &kColorAniInfo )
{
    if( 0 >= kColorAniInfo.iKeepFrame )
        return;

    if( 0 == m_kAddTexFrontColorAni.iKeepFrame )
    {
        m_kAddTexFrontColorAni.vMinLimit = kColorAniInfo.vMinLimit;
        m_kAddTexFrontColorAni.vMaxLimit = kColorAniInfo.vMaxLimit;
        m_kAddTexFrontColorAni.vAniSpeed = kColorAniInfo.vAniSpeed;
        m_kAddTexFrontColorAni.bReturnAni = kColorAniInfo.bReturnAni;
    }
    m_kAddTexFrontColorAni.iKeepFrame = kColorAniInfo.iKeepFrame;
}

void KGCPrograssBar::SetAddTexBackColorAni( PG_COLOR_ANIINFO &kColorAniInfo )
{
    if( 0 >= kColorAniInfo.iKeepFrame )
        return;

    if( 0 == m_kAddTexBackColorAni.iKeepFrame )
    {
        m_kAddTexBackColorAni.vMinLimit = kColorAniInfo.vMinLimit;
        m_kAddTexBackColorAni.vMaxLimit = kColorAniInfo.vMaxLimit;
        m_kAddTexBackColorAni.vAniSpeed = kColorAniInfo.vAniSpeed;
        m_kAddTexBackColorAni.bReturnAni = kColorAniInfo.bReturnAni;
    }
    m_kAddTexBackColorAni.iKeepFrame = kColorAniInfo.iKeepFrame;
}

void KGCPrograssBar::SetPosAni( float fMinX, float fMaxX, float fAniSpeedX, float fMinY, float fMaxY, float fAniSpeedY, int iKeepFrame )
{
    if( 0 >= m_kPosAni.iKeepFrame )
    {
        m_kPosAni.vMinLimit = D3DXVECTOR2( fMinX, fMinY );
        m_kPosAni.vMaxLimit = D3DXVECTOR2( fMaxX, fMaxY );
        m_kPosAni.vAniSpeed = D3DXVECTOR2( fAniSpeedX, fAniSpeedY );
    }
    m_kPosAni.iKeepFrame = iKeepFrame;
}

void KGCPrograssBar::DrawPrograssBar( int iLayer )
{
    if( false == m_bIsRender )
        return;

    bool bRenderScreen = false;
    bool bRenderWorld = false;
    D3DXVECTOR4 vRenderPos = m_dx4Pos;
    switch( m_iTargetType )
    {
    case TARGET_UI:
        bRenderScreen = (iLayer == GC_LAYER_UI);
        break;
    case TARGET_WORLD:
        bRenderWorld = (m_iLayer == iLayer);
        break;
    case TARGET_PLAYER:
        if( g_MyD3D->IsPlayerIndex( m_iTargetUnitIndex ) )
        {
            if( bRenderWorld = (m_iLayer == iLayer) )
            {
                vRenderPos.x += g_MyD3D->MyPlayer[ m_iTargetUnitIndex ]->vPos.x - 1.0f;
                vRenderPos.y += g_MyD3D->MyPlayer[ m_iTargetUnitIndex ]->vPos.y - 0.5f;
            }
        }
        break;
    case TARGET_MONSTER:
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster( m_iTargetUnitIndex );
            if( pMonster )
            {
                if( bRenderWorld = (m_iLayer == iLayer) )
                {
                    vRenderPos.x += pMonster->GetRenderX() - 1.0f - (vRenderPos.z/2.0f);
                    vRenderPos.y += pMonster->GetRenderY() - 0.5f;
                }
            }
        }
        break;
    case TARGET_EMBARK:
        {
            KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( m_iTargetUnitIndex );
            if( pEmbark )
            {
                if( bRenderWorld = (m_iLayer == iLayer) )
                {
                    // 앵커를 Bar중심으로
                    vRenderPos.x += pEmbark->GetRenderPosX() - 1.0f + (vRenderPos.z/2.0f);
                    vRenderPos.y += pEmbark->GetRenderPosY() - 0.5f;
                }
            }
        }
        break;
    }

    if( bRenderScreen )
    {
        DrawScreenAddBackTextrue( vRenderPos );
        DrawScreenBackGround( vRenderPos );
        DrawScreenBar( vRenderPos );
        DrawScreenAddFrontTextrue( vRenderPos );
    }

    if( bRenderWorld )
    {
        DrawWorldAddBackTextrue( vRenderPos );
        DrawWorldBackGround( vRenderPos );
        DrawWorldBar( vRenderPos );
        DrawWorldAddFrontTextrue( vRenderPos );

        m_dx3ParticleTracePos.x = vRenderPos.x;
        m_dx3ParticleTracePos.y = vRenderPos.y;
        m_dx3ParticleTracePos.z = 0.5f;
    }
}

void KGCPrograssBar::PosAnimation()
{
    if( 0 < m_kPosAni.iKeepFrame )
    {
        m_vNowPosAni.x += m_kPosAni.vAniSpeed.x;
        m_vNowPosAni.y += m_kPosAni.vAniSpeed.y;
        
        if( m_vNowPosAni.x < m_kPosAni.vMinLimit.x )
        {
            m_vNowPosAni.x = m_kPosAni.vMinLimit.x;
            m_kPosAni.vAniSpeed.x *= -1.0f;
        }
        if( m_vNowPosAni.x > m_kPosAni.vMaxLimit.x )
        {
            m_vNowPosAni.x = m_kPosAni.vMaxLimit.x;
            m_kPosAni.vAniSpeed.x *= -1.0f;
        }
        if( m_vNowPosAni.y < m_kPosAni.vMinLimit.y )
        {
            m_vNowPosAni.y = m_kPosAni.vMinLimit.y;
            m_kPosAni.vAniSpeed.y *= -1.0f;
        }
        if( m_vNowPosAni.y > m_kPosAni.vMaxLimit.y )
        {
            m_vNowPosAni.y = m_kPosAni.vMaxLimit.y;
            m_kPosAni.vAniSpeed.y *= -1.0f;
        }
       m_kPosAni.iKeepFrame--;
    }
    else
    {
        m_vNowPosAni.x = 0.0f;
        m_vNowPosAni.y = 0.0f;
    }
}

void KGCPrograssBar::DrawScreenBackGround( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pBackTex )
        return;

    D3DXVECTOR4 vPos = vRenderPos;

    vPos.x *= GC_SCREEN_DIV_WIDTH;
    vPos.y *= GC_SCREEN_DIV_HEIGHT;
    vPos.z *= GC_SCREEN_DIV_WIDTH;
    vPos.w *= GC_SCREEN_DIV_HEIGHT;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    float fCenterStartPos = ((m_dx4BGUV[PG_FRONT].z - m_dx4BGUV[PG_FRONT].x) * GC_SCREEN_DIV_WIDTH) + vPos.x;
    float fCenterEndPos = vPos.z - ((m_dx4BGUV[PG_BACK].z - m_dx4BGUV[PG_BACK].x) * GC_SCREEN_DIV_WIDTH);
    float fTextureWidth = static_cast<float>(m_pBackTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pBackTex->GetHeight());

    // Front출력
    g_pGCDeviceManager2->DrawInScreenResize(m_pBackTex,
        vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, fCenterStartPos+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y,
        ( (m_dx4BGUV[PG_FRONT].x)/fTextureWidth ),
        ( (m_dx4BGUV[PG_FRONT].y)/fTextureHeight ),
        ( (m_dx4BGUV[PG_FRONT].z)/fTextureWidth ),
        ( (m_dx4BGUV[PG_FRONT].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBackAlpha, 255, 255, 255) );

    // Center출력
    g_pGCDeviceManager2->DrawInScreenResize(m_pBackTex,
        fCenterStartPos+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, fCenterEndPos+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y,
        ( (m_dx4BGUV[PG_CENTER].x)/fTextureWidth ),
        ( (m_dx4BGUV[PG_CENTER].y)/fTextureHeight ),
        ( (m_dx4BGUV[PG_CENTER].z)/fTextureWidth ),
        ( (m_dx4BGUV[PG_CENTER].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBackAlpha, 255, 255, 255) );

    // Back출력
    g_pGCDeviceManager2->DrawInScreenResize(m_pBackTex,
        fCenterEndPos+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y,
        ( (m_dx4BGUV[PG_BACK].x)/fTextureWidth ),
        ( (m_dx4BGUV[PG_BACK].y)/fTextureHeight ),
        ( (m_dx4BGUV[PG_BACK].z)/fTextureWidth ),
        ( (m_dx4BGUV[PG_BACK].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBackAlpha, 255, 255, 255) );

    // NowValue 숫자 렌더링
    if( m_bIsRenderNumber )
    {
        D3DXVECTOR2 vNumPos;
        vNumPos.x = (vPos.z + (m_kNumberInfo.fOffsetX*GC_SCREEN_DIV_WIDTH)) * g_pGCDeviceManager2->GetWindowScaleX();
        vNumPos.y = (vPos.y + (m_kNumberInfo.fOffsetY*GC_SCREEN_DIV_HEIGHT)) * g_pGCDeviceManager2->GetWindowScaleY();
        g_pkUIMgr->RenderNumber( m_kNumberInfo.strType, vNumPos, m_kNumberInfo.dwNumber, D3DXCOLOR(m_kNumberInfo.dwColor), true, false );
    }

    // Text 렌더링
    if( -1 != m_iString )
    {
        D3DXVECTOR2 vTexPos;
        vTexPos.x = (vPos.x + (m_vStringOffset.x*GC_SCREEN_DIV_WIDTH)) * g_pGCDeviceManager2->GetWindowScaleX();
        vTexPos.y = (vPos.y + (m_vStringOffset.y*GC_SCREEN_DIV_HEIGHT)) * g_pGCDeviceManager2->GetWindowScaleY();
        g_pkFontMgrOutline->OutTextXY(static_cast<int>(vTexPos.x + m_vNowPosAni.x), static_cast<int>(vTexPos.y + m_vNowPosAni.y), 
            g_pkStrLoader->GetString( m_iString ).c_str(), 0xffffffff, NULL, DT_LEFT | DT_TOP, 0xff000000 );
    }
}

void KGCPrograssBar::DrawScreenBar( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pBarTex )
        return;

    if( m_iNowValue > m_iMaxValue )
        m_iNowValue = m_iMinValue;

    int iValueSize = (m_iNowValue - m_iMinValue);

    if( iValueSize <= 0)
        return;

    D3DXVECTOR4 vPos = vRenderPos;

    vPos.x *= GC_SCREEN_DIV_WIDTH;
    vPos.y *= GC_SCREEN_DIV_HEIGHT;
    vPos.z *= GC_SCREEN_DIV_WIDTH;
    vPos.w *= GC_SCREEN_DIV_HEIGHT;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    float fTextureWidth = static_cast<float>(m_pBarTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pBarTex->GetHeight());

    float fXGapPixel = GAP_PG_PIXEL * GC_SCREEN_DIV_WIDTH;
    float fYGapPixel = GAP_PG_PIXEL * GC_SCREEN_DIV_HEIGHT;

    // Front출력
    D3DXVECTOR4 vTempPos;

    if ( m_bVertical )
    {
        vTempPos.x = vPos.x + fXGapPixel + 1.0f;
        vTempPos.y = vPos.w - ((m_dx4BarUV[PG_FRONT].w - m_dx4BarUV[PG_FRONT].y) * GC_SCREEN_DIV_HEIGHT);
        vTempPos.z = vPos.z - fXGapPixel;
        vTempPos.w = vPos.w - fYGapPixel;
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            vTempPos.x+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y, vTempPos.z+m_vNowPosAni.x, vTempPos.w+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_FRONT].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_FRONT].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );

        // Center출력 : MinMax간 비율을 구하고, 좌표를 0점으로 보내어 비율을 적용시킨 후 Start점으로 이동시킴
        float fPercent = float(iValueSize) / float(m_iMaxValue-m_iMinValue);
        float fCenterPos = vTempPos.y - (((vPos.w - fYGapPixel) - vPos.y) * fPercent);

        float fHeight = ( m_dx4BarUV[PG_CENTER].w - m_dx4BarUV[PG_CENTER].y ) * fPercent;
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            vTempPos.x+m_vNowPosAni.x, fCenterPos+m_vNowPosAni.y, vTempPos.z+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_CENTER].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_CENTER].w - fHeight)/fTextureHeight ),
            ( (m_dx4BarUV[PG_CENTER].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_CENTER].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );

        // Back출력
        vTempPos.y = fCenterPos - ((m_dx4BarUV[PG_BACK].w - m_dx4BarUV[PG_BACK].y) * GC_SCREEN_DIV_HEIGHT);
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            vTempPos.x+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y, vTempPos.z+m_vNowPosAni.x, fCenterPos+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_BACK].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_BACK].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_BACK].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_BACK].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );
    }
    else
    {
        vTempPos.x = vPos.x + fXGapPixel + 1.0f;
        vTempPos.y = vPos.y + fYGapPixel;
        vTempPos.z = ((m_dx4BarUV[PG_FRONT].z - m_dx4BarUV[PG_FRONT].x) * GC_SCREEN_DIV_WIDTH) + vTempPos.x;
        vTempPos.w = vPos.w - fYGapPixel;
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            vTempPos.x+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y, vTempPos.z+m_vNowPosAni.x, vTempPos.w+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_FRONT].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_FRONT].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );

        // Center출력 : MinMax간 비율을 구하고, 좌표를 0점으로 보내어 비율을 적용시킨 후 Start점으로 이동시킴
        float fPercent = float(iValueSize) / float(m_iMaxValue-m_iMinValue);
        float fCenterPos = (((vPos.z - fXGapPixel) - vTempPos.x) * fPercent) + vTempPos.x;
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            vTempPos.z+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y, fCenterPos+m_vNowPosAni.x, vTempPos.w+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_CENTER].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_CENTER].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_CENTER].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_CENTER].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );

        // Back출력
        vTempPos.z = ((m_dx4BarUV[PG_BACK].z - m_dx4BarUV[PG_BACK].x) * GC_SCREEN_DIV_WIDTH) + fCenterPos;
        g_pGCDeviceManager2->DrawInScreenResize(m_pBarTex,
            fCenterPos+m_vNowPosAni.x, vTempPos.y+m_vNowPosAni.y, vTempPos.z+m_vNowPosAni.x, vTempPos.w+m_vNowPosAni.y,
            ( (m_dx4BarUV[PG_BACK].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_BACK].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_BACK].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_BACK].w)/fTextureHeight ), false, 10.0f, D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );
    }    
}

void KGCPrograssBar::DrawScreenAddBackTextrue( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pAddBackTex )
        return;

    if( false == m_bIsAddBackTexRender )
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.x += m_dx4AddBackTexPos.x;
    vPos.y += m_dx4AddBackTexPos.y;
    vPos.z = m_dx4AddBackTexPos.z;
    vPos.w = m_dx4AddBackTexPos.w;

    vPos.x *= GC_SCREEN_DIV_WIDTH;
    vPos.y *= GC_SCREEN_DIV_HEIGHT;
    vPos.z *= GC_SCREEN_DIV_WIDTH;
    vPos.w *= GC_SCREEN_DIV_HEIGHT;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    vPos.x -= m_fAddTexBackScale;
    vPos.y -= m_fAddTexBackScale;
    vPos.z += m_fAddTexBackScale;
    vPos.w += m_fAddTexBackScale;

    float fTextureWidth = static_cast<float>(m_pAddBackTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pAddBackTex->GetHeight());

    DWORD dwColor = (m_dwAddTexBackColor | D3DCOLOR_ARGB(m_iAddTexBackAlpha, 0, 0, 0));
    g_pGCDeviceManager2->DrawInScreenResize(m_pAddBackTex,
        vPos.x + m_vNowPosAni.x, vPos.y + m_vNowPosAni.y, vPos.z + m_vNowPosAni.x, vPos.w + m_vNowPosAni.y,
        ( (m_dx4AddBackTexUV.x)/fTextureWidth ),
        ( (m_dx4AddBackTexUV.y)/fTextureHeight ),
        ( (m_dx4AddBackTexUV.z)/fTextureWidth ),
        ( (m_dx4AddBackTexUV.w)/fTextureHeight ), false, 10.0f, dwColor );
}

void KGCPrograssBar::DrawScreenAddFrontTextrue( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pAddFrontTex )
        return;

    if( false == m_bIsAddFrontTexRender )
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.x += m_dx4AddFrontTexPos.x;
    vPos.y += m_dx4AddFrontTexPos.y;
    vPos.z = m_dx4AddFrontTexPos.z;
    vPos.w = m_dx4AddFrontTexPos.w;

    vPos.x *= GC_SCREEN_DIV_WIDTH;
    vPos.y *= GC_SCREEN_DIV_HEIGHT;
    vPos.z *= GC_SCREEN_DIV_WIDTH;
    vPos.w *= GC_SCREEN_DIV_HEIGHT;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    vPos.x -= m_fAddTexFrontScale;
    vPos.y -= m_fAddTexFrontScale;
    vPos.z += m_fAddTexFrontScale;
    vPos.w += m_fAddTexFrontScale;

    float fTextureWidth = static_cast<float>(m_pAddFrontTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pAddFrontTex->GetHeight());

    DWORD dwColor = (m_dwAddTexFrontColor | D3DCOLOR_ARGB(m_iAddTexFrontAlpha, 0, 0, 0));
    g_pGCDeviceManager2->DrawInScreenResize(m_pAddFrontTex,
        vPos.x + m_vNowPosAni.x, vPos.y + m_vNowPosAni.y, vPos.z + m_vNowPosAni.x, vPos.w + m_vNowPosAni.y,
        ( (m_dx4AddFrontTexUV.x)/fTextureWidth ),
        ( (m_dx4AddFrontTexUV.y)/fTextureHeight ),
        ( (m_dx4AddFrontTexUV.z)/fTextureWidth ),
        ( (m_dx4AddFrontTexUV.w)/fTextureHeight ), false, 10.0f, dwColor );
}

void KGCPrograssBar::DrawWorldBackGround( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pBackTex )
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    float fTextureWidth = static_cast<float>(m_pBackTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pBackTex->GetHeight());

    D3DXVECTOR3 vPos1( vPos.x+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos2( vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos3( vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos4( vPos.z+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    g_pGCDeviceManager2->DrawInWorldResize( m_pBackTex, &g_kCamera.m_matCamera, 
        vPos1, vPos2, vPos3, vPos4,
        ( (m_dx4BGUV[PG_FRONT].x)/fTextureWidth ),
        ( (m_dx4BGUV[PG_FRONT].y)/fTextureHeight ),
        ( (m_dx4BGUV[PG_FRONT].z)/fTextureWidth ),
        ( (m_dx4BGUV[PG_FRONT].w)/fTextureHeight ), D3DCOLOR_ARGB(m_iBackAlpha, 255, 255, 255) );
}

void KGCPrograssBar::DrawWorldBar( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pBarTex )
        return;

    if( m_iNowValue > m_iMaxValue )
        m_iNowValue = m_iMinValue;

    int iValueSize = (m_iNowValue - m_iMinValue);

    if( iValueSize <= 0)
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.x += m_dx4PosToBarOffset.x; 
    vPos.y += m_dx4PosToBarOffset.y; 
    vPos.z += m_dx4PosToBarOffset.z; 
    vPos.w += m_dx4PosToBarOffset.w; 


    if ( m_bVertical )
    {
        float fPercent = float(iValueSize) / float(m_iMaxValue-m_iMinValue);
        vPos.z = vPos.x + vPos.z;
        vPos.w = vPos.y + ( vPos.w * fPercent );

        float fTextureWidth = static_cast<float>(m_pBarTex->GetWidth());
        float fTextureHeight = static_cast<float>(m_pBarTex->GetHeight());

        D3DXVECTOR3 vPos1( vPos.x+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos2( vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos3( vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos4( vPos.z+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );

        if ( m_bVerticalEx )
        {
            float fHeight = ( m_dx4BarUV[PG_FRONT].w - m_dx4BarUV[PG_FRONT].y ) * fPercent;
            float fYPos = ( ( m_dx4BarUV[PG_FRONT].w - fHeight )/ fTextureHeight );

            g_pGCDeviceManager2->DrawInWorldResize( m_pBarTex, &g_kCamera.m_matCamera, 
                vPos1, vPos2, vPos3, vPos4,
                ( (m_dx4BarUV[PG_FRONT].x)/fTextureWidth ),
                fYPos,
                ( (m_dx4BarUV[PG_FRONT].z)/fTextureWidth ),
                ( (m_dx4BarUV[PG_FRONT].w)/fTextureHeight ), D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );
        }
        else
        {
            g_pGCDeviceManager2->DrawInWorldResize( m_pBarTex, &g_kCamera.m_matCamera, 
                vPos1, vPos2, vPos3, vPos4,
                ( (m_dx4BarUV[PG_FRONT].x)/fTextureWidth ),
                ( (m_dx4BarUV[PG_FRONT].y)/fTextureHeight ),
                ( (m_dx4BarUV[PG_FRONT].z)/fTextureWidth ),
                ( (m_dx4BarUV[PG_FRONT].w)/fTextureHeight ), D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );
        }

    }
    else
    {
        float fPercent = float(iValueSize) / float(m_iMaxValue-m_iMinValue);
        vPos.z = vPos.x + ( vPos.z * fPercent);
        vPos.w = vPos.y + vPos.w;

        float fTextureWidth = static_cast<float>(m_pBarTex->GetWidth());
        float fTextureHeight = static_cast<float>(m_pBarTex->GetHeight());

        D3DXVECTOR3 vPos1( vPos.x+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos2( vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos3( vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
        D3DXVECTOR3 vPos4( vPos.z+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );

        g_pGCDeviceManager2->DrawInWorldResize( m_pBarTex, &g_kCamera.m_matCamera, 
            vPos1, vPos2, vPos3, vPos4,
            ( (m_dx4BarUV[PG_FRONT].x)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].y)/fTextureHeight ),
            ( (m_dx4BarUV[PG_FRONT].z)/fTextureWidth ),
            ( (m_dx4BarUV[PG_FRONT].w)/fTextureHeight ), D3DCOLOR_ARGB(m_iBarAlpha, 255, 255, 255) );


    }
}

void KGCPrograssBar::DrawWorldAddBackTextrue( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pAddBackTex )
        return;

    if( false == m_bIsAddBackTexRender )
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.x += m_dx4AddBackTexPos.x;
    vPos.y += m_dx4AddBackTexPos.y;
    vPos.z = m_dx4AddBackTexPos.z;
    vPos.w = m_dx4AddBackTexPos.w;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    float fTextureWidth = static_cast<float>(m_pAddBackTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pAddBackTex->GetHeight());

    D3DXVECTOR3 vPos1( vPos.x+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos2( vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos3( vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos4( vPos.z+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    g_pGCDeviceManager2->DrawInWorldResize( m_pAddBackTex, &g_kCamera.m_matCamera, 
        vPos1, vPos2, vPos3, vPos4,
        ( (m_dx4AddBackTexUV.x)/fTextureWidth ),
        ( (m_dx4AddBackTexUV.y)/fTextureHeight ),
        ( (m_dx4AddBackTexUV.z)/fTextureWidth ),
        ( (m_dx4AddBackTexUV.w)/fTextureHeight ), D3DCOLOR_ARGB(m_iAddTexBackAlpha, 255, 255, 255) );
}

void KGCPrograssBar::DrawWorldAddFrontTextrue( D3DXVECTOR4 vRenderPos )
{
    if( NULL == m_pAddFrontTex )
        return;

    if( false == m_bIsAddFrontTexRender )
        return;

    D3DXVECTOR4 vPos = vRenderPos;
    vPos.x += m_dx4AddFrontTexPos.x;
    vPos.y += m_dx4AddFrontTexPos.y;
    vPos.z = m_dx4AddFrontTexPos.z;
    vPos.w = m_dx4AddFrontTexPos.w;

    vPos.z = vPos.x + vPos.z;
    vPos.w = vPos.y + vPos.w;

    float fTextureWidth = static_cast<float>(m_pAddFrontTex->GetWidth());
    float fTextureHeight = static_cast<float>(m_pAddFrontTex->GetHeight());

    D3DXVECTOR3 vPos1( vPos.x+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos2( vPos.z+m_vNowPosAni.x, vPos.w+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos3( vPos.x+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    D3DXVECTOR3 vPos4( vPos.z+m_vNowPosAni.x, vPos.y+m_vNowPosAni.y, KGCLayerPart::ms_fZOrder[m_iLayer] );
    g_pGCDeviceManager2->DrawInWorldResize( m_pAddFrontTex, &g_kCamera.m_matCamera, 
        vPos1, vPos2, vPos3, vPos4,
        ( (m_dx4AddFrontTexUV.x)/fTextureWidth ),
        ( (m_dx4AddFrontTexUV.y)/fTextureHeight ),
        ( (m_dx4AddFrontTexUV.z)/fTextureWidth ),
        ( (m_dx4AddFrontTexUV.w)/fTextureHeight ), D3DCOLOR_ARGB(m_iAddTexFrontAlpha, 255, 255, 255) );
}

void KGCPrograssBar::FrameMove()
{
    if( 0 < m_iCoolTime )
    {
        if( (++m_iNowTime) > m_iCoolTime )
        {
            m_iNowTime = 0;
            SetNowValue( GetNowValue() + m_iAccValue );
            if( GetNowValue() > GetMaxValue() )
                SetNowValue( GetMaxValue() );
        }
    }


	//CheckAndChangeState();

	switch(m_iNowState)
	{
	case NORMAL:
		break;
	case OPENING:
		{
			D3DXVECTOR2 vPos = D3DXVECTOR2(m_dx4Pos.x , m_dx4Pos.y);
			D3DXVec2Lerp( &vPos , &vPos , &m_vOpenTargetPos , m_fStateAnitime);
			m_dx4Pos.x = vPos.x;
			m_dx4Pos.y = vPos.y;
		}
		if (m_fStateAnitime >= 1.0f)
		{
			m_iNowState = NORMAL;
			m_fStateAnitime = 0.0f;
		}
		else
			m_fStateAnitime += 0.05f;
		break;
	case HIDDING:
		{
			D3DXVECTOR2 vPos = D3DXVECTOR2(m_dx4Pos.x , m_dx4Pos.y);
			D3DXVec2Lerp( &vPos , &vPos , &m_vHidTargetPos , m_fStateAnitime);
			m_dx4Pos.x = vPos.x;
			m_dx4Pos.y = vPos.y;
		}
		if (m_fStateAnitime >= 1.0f)
		{
			m_iNowState = HIDDEN;
			m_fStateAnitime = 0.0f;
		}
		else
			m_fStateAnitime += 0.05f;
		break;
	case HIDDEN:
		break;
	}

	
    
    m_iBarAlpha = static_cast<int>(Animation( static_cast<float>(m_iBarAlpha), 255.0f, m_kBarAlphaAni ));
    m_iBackAlpha = static_cast<int>(Animation( static_cast<float>(m_iBackAlpha), 255.0f, m_kBackAlphaAni ));
    m_iAddTexFrontAlpha = static_cast<int>(Animation( static_cast<float>(m_iAddTexFrontAlpha), 255.0f, m_kAddTexFrontAlphaAni ));
    m_iAddTexBackAlpha = static_cast<int>(Animation( static_cast<float>(m_iAddTexBackAlpha), 255.0f, m_kAddTexBackAlphaAni ));

    m_fAddTexFrontScale = Animation( m_fAddTexFrontScale, 0.0f, m_kAddTexFrontScaleAni );
    m_fAddTexBackScale = Animation( m_fAddTexBackScale, 0.0f, m_kAddTexBackScaleAni );

    m_dwAddTexFrontColor = ColorAnimation( m_dwAddTexFrontColor, m_kAddTexFrontColorAni );
    m_dwAddTexBackColor = ColorAnimation( m_dwAddTexBackColor, m_kAddTexBackColorAni );

    PosAnimation();
}

float KGCPrograssBar::Animation( float fNowValue, float fDefaultValue, PG_ANIINFO &kAniInfo )
{
    float fReturnValue = fDefaultValue;
    if( 0 < kAniInfo.iKeepFrame || -1 == kAniInfo.iKeepFrame )
    {
        fNowValue += kAniInfo.fAniSpeed;
        if( fNowValue < kAniInfo.fMinLimit )
        {
            if( kAniInfo.bReturnAni )
            {
                fNowValue = kAniInfo.fMinLimit;
                kAniInfo.fAniSpeed *= -1;
            }
            else
                fNowValue = kAniInfo.fMaxLimit;

            if( -1 != kAniInfo.iSoundID )
                g_KDSound.Play( kAniInfo.iSoundID, 0, LATENCY_BUFFER_SIZE );
        }
        if( fNowValue > kAniInfo.fMaxLimit )
        {
            if( kAniInfo.bReturnAni )
            {
                fNowValue = kAniInfo.fMaxLimit;
                kAniInfo.fAniSpeed *= -1;
            }
            else
                fNowValue = kAniInfo.fMinLimit;

            if( -1 != kAniInfo.iSoundID )
                g_KDSound.Play( kAniInfo.iSoundID, 0, LATENCY_BUFFER_SIZE );
        }

        fReturnValue = fNowValue;

        if( 0 < kAniInfo.iKeepFrame )
            kAniInfo.iKeepFrame--;
    }

    return fReturnValue;
}

DWORD KGCPrograssBar::ColorAnimation( DWORD dwNowValue, PG_COLOR_ANIINFO &kColorInfo )
{
    DWORD dwReturnValue = D3DCOLOR_ARGB( 0, 255, 255, 255);
    if( 0 < kColorInfo.iKeepFrame )
    {
        PG_ANIINFO kTempAniInfo;
        kTempAniInfo.iKeepFrame = 10;
        kTempAniInfo.bReturnAni = kColorInfo.bReturnAni;
        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.x;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.x;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.x;
        int iR = static_cast<int>(Animation( static_cast<float>(GetBValue(dwNowValue)), 0.0f, kTempAniInfo ));// R보간

        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.y;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.y;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.y;
        int iG = static_cast<int>(Animation( static_cast<float>(GetGValue(dwNowValue)), 0.0f, kTempAniInfo ));// G보간

        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.z;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.z;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.z;
        int iB = static_cast<int>(Animation( static_cast<float>(GetRValue(dwNowValue)), 0.0f, kTempAniInfo ));// B보간

        dwReturnValue = D3DCOLOR_ARGB( 0, iR, iG, iB );
        kColorInfo.iKeepFrame--;
    }

    return dwReturnValue;
}

void KGCPrograssBar::RegisterLuabind()
{
    // 바인딩
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<PG_COLOR_ANIINFO>( L, "KPrograssBarColorAniInfo" );
    lua_tinker::class_con<PG_COLOR_ANIINFO>( L, lua_tinker::constructor<>() );

    lua_tinker::class_def<PG_COLOR_ANIINFO>( L,  "SetMinLimit",           &PG_COLOR_ANIINFO::SetMinLimit  );
    lua_tinker::class_def<PG_COLOR_ANIINFO>( L,  "SetMaxLimit",           &PG_COLOR_ANIINFO::SetMaxLimit  );
    lua_tinker::class_def<PG_COLOR_ANIINFO>( L,  "SetAniSpeed",           &PG_COLOR_ANIINFO::SetAniSpeed  );
    lua_tinker::class_def<PG_COLOR_ANIINFO>( L,  "SetAniInfo",            &PG_COLOR_ANIINFO::SetAniInfo  );

    lua_tinker::class_add<KGCPrograssBar>( L, "KGCPrograssBar" );
	lua_tinker::class_def<KGCPrograssBar>( L, "SetOpenTargetPos",       &KGCPrograssBar::SetOpenTargetPos  );
	lua_tinker::class_def<KGCPrograssBar>( L, "SetHidTargetPos",        &KGCPrograssBar::SetHidTargetPos  );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetNowMyState",        &KGCPrograssBar::SetNowMyState  );
	lua_tinker::class_def<KGCPrograssBar>( L, "SetBackTexture",         &KGCPrograssBar::SetBackTexture );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBarTexture",          &KGCPrograssBar::SetBarTexture );
    
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBGFrontUV",           &KGCPrograssBar::SetBGFrontUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBGCenterUV",          &KGCPrograssBar::SetBGCenterUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBGBackUV",            &KGCPrograssBar::SetBGBackUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBarFrontUV",          &KGCPrograssBar::SetBarFrontUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBarCenterUV",         &KGCPrograssBar::SetBarCenterUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBarBackUV",           &KGCPrograssBar::SetBarBackUV );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetRenderPos",           &KGCPrograssBar::SetRenderPos );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetRenderPosX",          &KGCPrograssBar::GetRenderPosX );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetRenderPosY",          &KGCPrograssBar::GetRenderPosY );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetPosToBarOffset",      &KGCPrograssBar::SetPosToBarOffset );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetWidth",               &KGCPrograssBar::SetWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetHeight",              &KGCPrograssBar::SetHeight );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetWidth",               &KGCPrograssBar::GetWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetHeight",              &KGCPrograssBar::GetHeight );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetMaxValue",            &KGCPrograssBar::SetMaxValue );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetMinValue",            &KGCPrograssBar::SetMinValue );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetNowValue",            &KGCPrograssBar::SetNowValue );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetMaxValue",            &KGCPrograssBar::GetMaxValue );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetMinValue",            &KGCPrograssBar::GetMinValue );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetNowValue",            &KGCPrograssBar::GetNowValue );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetTargetType",          &KGCPrograssBar::SetTargetType );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetTargetType",          &KGCPrograssBar::GetTargetType );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetTargetUnitIdx",       &KGCPrograssBar::SetTargetUnitIdx );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetTargetUnitIdx",       &KGCPrograssBar::GetTargetUnitIdx );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetDrawLayer",           &KGCPrograssBar::SetDrawLayer );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetDrawLayer",           &KGCPrograssBar::GetDrawLayer );

    lua_tinker::class_def<KGCPrograssBar>( L, "StartCoolTime",          &KGCPrograssBar::StartCoolTime );
    lua_tinker::class_def<KGCPrograssBar>( L, "StopCoolTime",           &KGCPrograssBar::StopCoolTime );
    lua_tinker::class_def<KGCPrograssBar>( L, "ToggleRender",           &KGCPrograssBar::ToggleRender );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetRender",              &KGCPrograssBar::GetRender );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTexture",          &KGCPrograssBar::SetAddFrontTexture );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTextrueUV",        &KGCPrograssBar::SetAddFrontTextrueUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTextrueRenderOffset",&KGCPrograssBar::SetAddFrontTextrueRenderOffset );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddFrontTextrueRenderPosX",&KGCPrograssBar::GetAddFrontTextrueRenderPosX );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddFrontTextrueRenderPosY",&KGCPrograssBar::GetAddFrontTextrueRenderPosY );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTextrueWidth",     &KGCPrograssBar::SetAddFrontTextrueWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTextrueHeight",    &KGCPrograssBar::SetAddFrontTextrueHeight );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddFrontTextrueWidth",     &KGCPrograssBar::GetAddFrontTextrueWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddFrontTextrueHeight",    &KGCPrograssBar::GetAddFrontTextrueHeight );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTexture",          &KGCPrograssBar::SetAddBackTexture );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTextrueUV",        &KGCPrograssBar::SetAddBackTextrueUV );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTextrueRenderOffset",&KGCPrograssBar::SetAddBackTextrueRenderOffset );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddBackTextrueRenderPosX",&KGCPrograssBar::GetAddBackTextrueRenderPosX );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddBackTextrueRenderPosY",&KGCPrograssBar::GetAddBackTextrueRenderPosY );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTextrueWidth",     &KGCPrograssBar::SetAddBackTextrueWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTextrueHeight",    &KGCPrograssBar::SetAddBackTextrueHeight );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddBackTextrueWidth",     &KGCPrograssBar::GetAddBackTextrueWidth );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddBackTextrueHeight",    &KGCPrograssBar::GetAddBackTextrueHeight );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddFrontTextureToggleRender",    &KGCPrograssBar::SetAddFrontTextureToggleRender );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddFrontTextureToggleRender",    &KGCPrograssBar::GetAddFrontTextureToggleRender );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddBackTextureToggleRender",    &KGCPrograssBar::SetAddBackTextureToggleRender );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetAddBackTextureToggleRender",    &KGCPrograssBar::GetAddBackTextureToggleRender );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetVertical",    &KGCPrograssBar::SetVertical );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetVertical",    &KGCPrograssBar::GetVertical );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetVerticalEx",    &KGCPrograssBar::SetVerticalEx );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetVerticalEx",    &KGCPrograssBar::GetVerticalEx );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetRenderNumber",        &KGCPrograssBar::SetRenderNumber );

    lua_tinker::class_def<KGCPrograssBar>( L, "AddParticle",            &KGCPrograssBar::AddParticle );
    lua_tinker::class_def<KGCPrograssBar>( L, "DeleteParticle",         &KGCPrograssBar::DeleteParticle );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetBarAlphaAni",         &KGCPrograssBar::SetBarAlphaAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetBackAlphaAni",        &KGCPrograssBar::SetBackAlphaAni );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexFrontAlphaAni",       &KGCPrograssBar::SetAddTexFrontAlphaAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexBackAlphaAni",        &KGCPrograssBar::SetAddTexBackAlphaAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexFrontScaleAni",       &KGCPrograssBar::SetAddTexFrontScaleAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexBackScaleAni",        &KGCPrograssBar::SetAddTexBackScaleAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexFrontColorAni",       &KGCPrograssBar::SetAddTexFrontColorAni );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetAddTexBackColorAni",        &KGCPrograssBar::SetAddTexBackColorAni );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetPosAni",              &KGCPrograssBar::SetPosAni );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetText",                &KGCPrograssBar::SetText );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetText",                &KGCPrograssBar::GetText );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetTextOffset",          &KGCPrograssBar::SetTextOffset );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetTextOffsetX",         &KGCPrograssBar::GetTextOffsetX );
    lua_tinker::class_def<KGCPrograssBar>( L, "GetTextOffsetY",         &KGCPrograssBar::GetTextOffsetY );

    lua_tinker::class_def<KGCPrograssBar>( L, "SetDefaultTextureToScreen",        &KGCPrograssBar::SetDefaultTextureToScreen );
    lua_tinker::class_def<KGCPrograssBar>( L, "SetDefaultTextureToWorld",         &KGCPrograssBar::SetDefaultTextureToWorld );
}

void KGCPrograssBar::CheckAndChangeState()
{

	D3DXMATRIX mat1,mat2,matBasic;
	D3DXVECTOR3 vPlayerPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	

	g_MyD3D->GetMyPlayer()->Get_BasicMatrix( &matBasic ,true, true ,true);
	D3DXMatrixTranslation( &mat2, -0.15f, -0.1f, 0);
	D3DXMatrixMultiply( &mat1, &matBasic, &mat2 );

	::D3DXVec3TransformCoord( &vPlayerPos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat1 );

	float fScreenX = int( ( vPlayerPos.x + 1.0f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
	float fScreenY = int( ( -vPlayerPos.y + 0.75f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
	fScreenX*=g_pGCDeviceManager->GetWindowScaleX();
	fScreenY*=g_pGCDeviceManager->GetWindowScaleY();

	D3DXVECTOR2 vScreenPos = D3DXVECTOR2(fScreenX,fScreenY);

#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )

	float fStandardDis = m_fCheckDistance;
	if (m_iNowState == HIDDEN)
		fStandardDis *= 1.8f;
		

	if ( DISTANCE(m_dx4Pos.x , m_dx4Pos.y , fScreenX , fScreenY) < m_fCheckDistance )
	{
		if (m_iNowState == NORMAL)
			m_iNowState = HIDDING;
	}
	else
	{
		if (m_iNowState == HIDDEN)
			m_iNowState = OPENING;
	}
}

void KGCPrograssBar::SetNowMyState( int iState_ )
{
	//Opening 하고 hidding 상태값만 받는다..
	if (iState_ == OPENING)
	{
		if (m_iNowState == HIDDEN)
			m_iNowState = iState_;
	}
	else if (iState_ == HIDDING)
	{
		if (m_iNowState == NORMAL)
			m_iNowState = iState_;
	}

}


ImplementSingleton( KGCInGamePrograssBar )

KGCInGamePrograssBar::KGCInGamePrograssBar()
{

}

KGCInGamePrograssBar::~KGCInGamePrograssBar()
{

}

void KGCInGamePrograssBar::ClearALLData()
{
    std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.begin();
    for(; mapIter != m_mapPG.end(); ++mapIter)
    {
        SAFE_DELETE( mapIter->second );
    }
    m_mapPG.clear();
}

void KGCInGamePrograssBar::ClearData( int iID )
{
    std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.find( iID );
    if( mapIter != m_mapPG.end() )
    {
        SAFE_DELETE( mapIter->second );
        m_mapPG.erase( mapIter );
    }
}

void KGCInGamePrograssBar::Render( int iLayer )
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.begin();
        for(; mapIter != m_mapPG.end(); ++mapIter)
        {
            if( NULL == mapIter->second )
                continue;

            mapIter->second->DrawPrograssBar( iLayer );
        }
    }
    g_pGCDeviceManager2->PopState();
}

void KGCInGamePrograssBar::FrameMove()
{
    std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.begin();
    for(; mapIter != m_mapPG.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->FrameMove();
    }
}

KGCPrograssBar* KGCInGamePrograssBar::CreatePrograssBar( int iID )
{
    ClearData( iID );
    KGCPrograssBar* pPrograssBar = new KGCPrograssBar();
    m_mapPG[iID] = pPrograssBar;
    return pPrograssBar;
}

KGCPrograssBar* KGCInGamePrograssBar::GetInstanceToID( int iID )
{
    std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.find( iID );
    if( mapIter != m_mapPG.end() )
    {
        return mapIter->second;
    }
    return NULL;
}

bool KGCInGamePrograssBar::IsCreatePrograssBarFromID( int iID )
{
    std::map<DWORD, KGCPrograssBar*>::iterator mapIter = m_mapPG.find( iID );
    if( mapIter != m_mapPG.end() )
    {
        return true;
    }
    return false;
}

void KGCInGamePrograssBar::RegisterLuabind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCInGamePrograssBar>( L, "KGCInGamePrograssBar" );
    lua_tinker::class_def<KGCInGamePrograssBar>( L, "CreatePrograssBar",            &KGCInGamePrograssBar::CreatePrograssBar );
    lua_tinker::class_def<KGCInGamePrograssBar>( L, "GetInstanceToID",              &KGCInGamePrograssBar::GetInstanceToID );
    lua_tinker::class_def<KGCInGamePrograssBar>( L, "ClearData",                    &KGCInGamePrograssBar::ClearData );
    lua_tinker::class_def<KGCInGamePrograssBar>( L, "IsCreatePrograssBarFromID",    &KGCInGamePrograssBar::IsCreatePrograssBarFromID );
    
    lua_tinker::decl( L, "KGCInGamePrograssBar", this );
}