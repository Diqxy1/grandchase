/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2013년 06월 30일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 커맨드 가이드

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "StdAfx.h"
#include "KGCCommandGuid.h"


IMPLEMENT_CLASSNAME( KGCCommandGuid );
IMPLEMENT_WND_FACTORY( KGCCommandGuid );
IMPLEMENT_WND_FACTORY_NAME( KGCCommandGuid, "gc_command_guid" );

KGCCommandGuid::KGCCommandGuid()
: m_bShow( false )
{
    m_pkBack = NULL;
    m_pKeyTexture = NULL;
    m_vecCommandList.clear();
    m_mapInputChecker.clear();
    m_mapInputParticleChecker.clear();

    LINK_CONTROL( "back", m_pkBack );
}

KGCCommandGuid::~KGCCommandGuid()
{
    SAFE_RELEASE( m_pKeyTexture );
}

void KGCCommandGuid::SetHardKeyUV()
{
    m_vUV_KeyIn[KEY_TEX_UP]      = D3DXVECTOR4( 72.0f,   2.0f, 137.0f,  67.0f);
    m_vUV_KeyOut[KEY_TEX_UP]     = D3DXVECTOR4( 72.0f,  71.0f, 137.0f, 136.0f);
    m_vUV_KeyIn[KEY_TEX_DOWN]    = D3DXVECTOR4(214.0f,   2.0f, 279.0f,  67.0f);
    m_vUV_KeyOut[KEY_TEX_DOWN]   = D3DXVECTOR4(214.0f,  71.0f, 279.0f, 136.0f);
    m_vUV_KeyIn[KEY_TEX_LEFT]    = D3DXVECTOR4(143.0f,   2.0f, 208.0f,  67.0f);
    m_vUV_KeyOut[KEY_TEX_LEFT]   = D3DXVECTOR4(143.0f,  71.0f, 208.0f, 136.0f);
    m_vUV_KeyIn[KEY_TEX_RIGHT]   = D3DXVECTOR4(  2.0f,   2.0f,  67.0f,  67.0f);
    m_vUV_KeyOut[KEY_TEX_RIGHT]  = D3DXVECTOR4(  2.0f,  71.0f,  67.0f, 136.0f);
    m_vUV_KeyIn[KEY_TEX_Z]       = D3DXVECTOR4( 72.0f, 141.0f, 137.0f, 206.0f);
    m_vUV_KeyOut[KEY_TEX_Z]      = D3DXVECTOR4( 72.0f, 210.0f, 137.0f, 275.0f);
    m_vUV_KeyIn[KEY_TEX_X]       = D3DXVECTOR4(  2.0f, 141.0f,  67.0f, 206.0f);
    m_vUV_KeyOut[KEY_TEX_X]      = D3DXVECTOR4(  2.0f, 210.0f,  67.0f, 275.0f);
}

int KGCCommandGuid::GetConvertToTexKeyID( int iKeyID )
{
    switch( iKeyID )
    {
    case DIK_UP:            return KEY_TEX_UP;
    case DIK_DOWN:          return KEY_TEX_DOWN;
    case DIK_LEFT:          return KEY_TEX_LEFT;
    case DIK_RIGHT:         return KEY_TEX_RIGHT;
    case DIK_Z:             return KEY_TEX_Z;
    case DIK_X:             return KEY_TEX_X;
    }
    return MAX_KEY_TEX;
}

void KGCCommandGuid::OnCreateComplete( void )
{
    m_pkBack->InitState( false );

    // 요레하면 텍스쳐바끼면 껏다켜야됨.ㅋ( 한번만 로드 )
    if( NULL == m_pKeyTexture )
    {
        SetHardKeyUV();
        m_pKeyTexture = g_pGCDeviceManager2->CreateTexture( "CommandGuidKey.dds" );
    }

    m_vecCommandList.clear();
    m_mapInputChecker.clear();
}

void KGCCommandGuid::ActionPerformed( const KActionEvent& event ){}
void KGCCommandGuid::FrameMoveInEnabledState( void ){}
void KGCCommandGuid::PostChildDraw( void )
{
    if( false == m_bShow )
        return;

    if( 0 >= static_cast<int>(m_vecCommandList.size()) )
        return;

    // fGapSize : 키 박스 사이간격
    float fGapSize = 10.0f;
    // iNumItem : 키 갯수
    int iNumKey = static_cast<int>(m_vecCommandList.size());
    // fAllGapWidth : 전체 아이템 박스 사이간격
    float fAllGapWidth = (iNumKey-1) * fGapSize;
    // fScreenHalfWidth : 게임화면 절반크기
    float fScreenHalfWidth = GC_SCREEN_WIDTH/2.0f;
    // iKeyBoxWidth : 키 박스 하나 넓이
    int iKeyBoxWidth = 65;
    // iKeyBoxWidth : 키 박스 하나 높이
    int iKeyBoxHeight = 65;
    // iAllKeyBoxWidth : 전체 아이템 박스 크기
    int iAllKeyBoxWidth = iNumKey * iKeyBoxWidth;

    // 텍스쳐 크기
    float fWidth = static_cast<float>(m_pKeyTexture->GetWidth());
    float fHeight = static_cast<float>(m_pKeyTexture->GetHeight());

    // 백그라운드 크기와 위치 잡기
    float fSideGap = fGapSize * 2.0f;
    D3DXVECTOR2 vPos = m_pkBack->GetFixedWindowLocalPos();
    vPos.x = fScreenHalfWidth - ((iAllKeyBoxWidth + fAllGapWidth)/2) - fSideGap;
    m_pkBack->OnSetWidth( static_cast<int>(iAllKeyBoxWidth + fAllGapWidth + (fSideGap*2.0f) + 1) );
    m_pkBack->SetWindowPosDirect( vPos );
    m_pkBack->ToggleRender( true );
    vPos.y += 12.0f;

    // 이제 함 그려보까?
    std::vector<DWORD>::iterator vecCommandListIter = m_vecCommandList.begin();
    for(int iLoop=0; vecCommandListIter != m_vecCommandList.end(); ++iLoop, ++vecCommandListIter)
    {
        int iTexKeyID = GetConvertToTexKeyID( (*vecCommandListIter) );
        if( MAX_KEY_TEX == iTexKeyID )
            continue;

        // 텍스쳐 UV찾기( In으로 그릴지, Out으로 그릴지 )
        D3DXVECTOR4 pTexUV;
        if( m_mapInputChecker[(iLoop+1)] )
            pTexUV = D3DXVECTOR4(m_vUV_KeyIn[iTexKeyID].x/fWidth, m_vUV_KeyIn[iTexKeyID].y/fHeight, m_vUV_KeyIn[iTexKeyID].z/fWidth, m_vUV_KeyIn[iTexKeyID].w/fHeight);
        else
            pTexUV = D3DXVECTOR4(m_vUV_KeyOut[iTexKeyID].x/fWidth, m_vUV_KeyOut[iTexKeyID].y/fHeight, m_vUV_KeyOut[iTexKeyID].z/fWidth, m_vUV_KeyOut[iTexKeyID].w/fHeight);

        // 출력위치 찾기
        float fAllKeyHalfWidth = (iKeyBoxWidth/2.0f) * iNumKey;
        float fStartPosX = fScreenHalfWidth - (fAllKeyHalfWidth + (fAllGapWidth/2));
        vPos.x = fStartPosX + (iLoop*iKeyBoxWidth) + (iLoop*fGapSize);

        g_pGCDeviceManager2->DrawInScreenResize( m_pKeyTexture,
            vPos.x, vPos.y, (vPos.x + iKeyBoxWidth), (vPos.y + iKeyBoxHeight),
            pTexUV.x, pTexUV.y, pTexUV.z, pTexUV.w,
            false, 10.0f, D3DCOLOR_ARGB(255, 255, 255, 255));

        // ON이 됫으면 파티클 함 뿌려주자!
        if( m_mapInputChecker[(iLoop+1)] )
        {
            if( 0 == m_mapInputParticleChecker[(iLoop+1)] )
            {
                CParticleEventSeqPTR pParticle = NULL;
                pParticle = g_ParticleManager->CreateSequence( "Rin_Arrow_Hit_01", 
                    (vPos.x+(iKeyBoxWidth/2.0f)) / (400.0f*GC_SCREEN_DIV_WIDTH) - 1.0f, 0.75f - ((vPos.y+(iKeyBoxHeight/2.0f)) / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f, 0.5f );
                g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
                pParticle = g_ParticleManager->CreateSequence( "Rin_Arrow_Hit_02", 
                    (vPos.x+(iKeyBoxWidth/2.0f)) / (400.0f*GC_SCREEN_DIV_WIDTH) - 1.0f, 0.75f - ((vPos.y+(iKeyBoxHeight/2.0f)) / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f, 0.5f );
                g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
                pParticle = g_ParticleManager->CreateSequence( "Rin_Arrow_Hit_03", 
                    (vPos.x+(iKeyBoxWidth/2.0f)) / (400.0f*GC_SCREEN_DIV_WIDTH) - 1.0f, 0.75f - ((vPos.y+(iKeyBoxHeight/2.0f)) / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f, 0.5f );
                g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
                m_mapInputParticleChecker[(iLoop+1)] = 1;
            }
        }
    }
}

void KGCCommandGuid::ClearCommand( bool bAllClear )
{
    m_vecCommandList.clear();
    m_mapInputChecker.clear();

    if( bAllClear )
    {        
        m_mapInputParticleChecker.clear();
    }
}

void KGCCommandGuid::AddCommand(int iKeyID)
{
    m_vecCommandList.push_back( iKeyID );
    m_mapInputChecker[static_cast<int>(m_vecCommandList.size())] = false;

    std::map<DWORD, int>::iterator mapIter = m_mapInputParticleChecker.find( static_cast<int>(m_vecCommandList.size()) );
    if( mapIter == m_mapInputParticleChecker.end() )
        m_mapInputParticleChecker[static_cast<int>(m_vecCommandList.size())] = -1;
}

void KGCCommandGuid::SetCheckCommand( bool bCheck )
{
    m_bShow = bCheck;
    this->ToggleRender( m_bShow );
}

bool KGCCommandGuid::IsKeyON( int iKeyUID )
{
    if( 0 > iKeyUID || iKeyUID > static_cast<int>(m_vecCommandList.size()) )
        return false;

    return m_mapInputChecker[iKeyUID];
}

void KGCCommandGuid::SetON( int iKeyUID )
{
    if( 0 > iKeyUID || iKeyUID > static_cast<int>(m_vecCommandList.size()) )
        return;

    m_mapInputChecker[iKeyUID] = true;
    if( -1 == m_mapInputParticleChecker[iKeyUID] )
        m_mapInputParticleChecker[iKeyUID] = 0;
}

void KGCCommandGuid::SetOFF( int iKeyUID )
{
    if( 0 > iKeyUID || iKeyUID > static_cast<int>(m_vecCommandList.size()) )
        return;

    m_mapInputChecker[iKeyUID] = false;
}

void KGCCommandGuid::SetAllON()
{
    std::map<DWORD, bool>::iterator mapIter = m_mapInputChecker.begin();
    for(; mapIter != m_mapInputChecker.end(); ++mapIter)
    {
        mapIter->second = true;
        m_mapInputParticleChecker[mapIter->first] = 0;
    }
}

void KGCCommandGuid::SetAllOFF()
{
    std::map<DWORD, bool>::iterator mapIter = m_mapInputChecker.begin();
    for(; mapIter != m_mapInputChecker.end(); ++mapIter)
    {
        mapIter->second = false;
        m_mapInputParticleChecker[mapIter->first] = -1;
    }
}

void KGCCommandGuid::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<KGCCommandGuid>( L, "KGCCommandGuid" );
    lua_tinker::class_def<KGCCommandGuid>( L,  "SetCheckCommand", 	    &KGCCommandGuid::SetCheckCommand  );

    lua_tinker::class_def<KGCCommandGuid>( L,  "ClearCommand", 	        &KGCCommandGuid::ClearCommand  );
    lua_tinker::class_def<KGCCommandGuid>( L,  "AddCommand", 	        &KGCCommandGuid::AddCommand  );

    lua_tinker::class_def<KGCCommandGuid>( L,  "IsKeyON", 	            &KGCCommandGuid::IsKeyON  );
    lua_tinker::class_def<KGCCommandGuid>( L,  "SetON", 	            &KGCCommandGuid::SetON  );
    lua_tinker::class_def<KGCCommandGuid>( L,  "SetOFF", 	            &KGCCommandGuid::SetOFF  );
    lua_tinker::class_def<KGCCommandGuid>( L,  "SetAllON", 	            &KGCCommandGuid::SetAllON  );
    lua_tinker::class_def<KGCCommandGuid>( L,  "SetAllOFF", 	        &KGCCommandGuid::SetAllOFF  );
}