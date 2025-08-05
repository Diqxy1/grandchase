/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2013년 05월 21일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 벽 클래스( 스테이지 내에서 블럭을 구분짓기 위한 벽 )

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "stdafx.h"
#include "GCWall.h"
#include "Monster.h"

/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★벽 템플릿 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
void KGCWallTemplate::Init()
{
    m_dwID = 0;
    m_iDirection = 0;
    m_iInBoundSize = 0;
    m_vecCreateParticleList.clear();
    m_vecBrokenParticleList.clear();
}


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★벽 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
KGCWall::KGCWall( const KGCWallTemplate *pTemplate, float fPosX )
{
    if( NULL == pTemplate )
        return;

    m_pWallTemplate = pTemplate;
    m_dwUID = 0;
    m_fPosX = fPosX;
    m_bDestroyWall = false;
    m_bCheckInBound = true;

    m_vecParticleList.clear();
    std::vector<KWallParticle>::const_iterator vecParticleIter = pTemplate->m_vecCreateParticleList.begin();
    for(; vecParticleIter != pTemplate->m_vecCreateParticleList.end(); ++vecParticleIter)
    {
        AddParticle( vecParticleIter->strSquence.c_str(), vecParticleIter->vOffset.x, vecParticleIter->vOffset.y );
    }
}

KGCWall::~KGCWall()
{
    ReleaseResource();

    if( m_pWallTemplate )
    {
        std::vector<KWallParticle>::const_iterator vecParticleIter = m_pWallTemplate->m_vecBrokenParticleList.begin();
        for(; vecParticleIter != m_pWallTemplate->m_vecBrokenParticleList.end(); ++vecParticleIter)
        {
            AddParticle( vecParticleIter->strSquence.c_str(), vecParticleIter->vOffset.x, vecParticleIter->vOffset.y );
        }
    }
}

void KGCWall::ReleaseResource()
{
    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecParticleList.begin();
    for(; vecIter != m_vecParticleList.end(); ++vecIter)
    {
        g_ParticleManager->DeleteSequence( (*vecIter) );

    }
    m_vecParticleList.clear();
}

void KGCWall::AddParticle( const char *szSequence, float fOffsetX, float fOffsetY )
{
    CParticleEventSeqPTR pSequence = g_ParticleManager->CreateSequence( szSequence, ((m_fPosX - 1.0f) + fOffsetX), (fOffsetY - 0.5f), 0.5f );
    if( NULL == pSequence )
        return;

    m_vecParticleList.push_back( pSequence );
}

void KGCWall::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
        lua_tinker::class_add<KGCWall>( L, "KGCWall" );
}

void KGCWall::FrameMove()
{
    if( NULL == m_pWallTemplate )
    {
        m_bDestroyWall = true;
        return;
    }

    CheckUnitPushAndInBound();
}

void KGCWall::TextRender()
{
#if !defined(__PATH__)
    RenderBoundingBox();
#endif
}

void KGCWall::CheckUnitPushAndInBound()
{
    int iNumEnemyLiveMonster = 0;
    MAP_INT_PMONSTER::iterator mapMonsterIter = g_kMonsterManager.m_mapMonster.begin();
    for(; mapMonsterIter != g_kMonsterManager.m_mapMonster.end(); ++mapMonsterIter)
    {
        MONSTER *pMonster = mapMonsterIter->second;
        if( NULL == pMonster )
            continue;

        if( false == pMonster->IsLive() )
            continue;

        if( pMonster->GetNotCheckStateWall() )
            continue;

        if( -1 == pMonster->m_iTeam )
            ++iNumEnemyLiveMonster;

        switch( m_pWallTemplate->m_iDirection )
        {
        case WALL_LEFT:
            if( m_fPosX < pMonster->GetX() )
            {
                pMonster->SetX( m_fPosX - PUSH_GAP );
                pMonster->CallLuaFucn_MoveFail();
            }
            break;
        case WALL_RIGHT:
            if( m_fPosX > pMonster->GetX() )
            {
                pMonster->SetX( m_fPosX + PUSH_GAP );
                pMonster->CallLuaFucn_MoveFail();
            }
            break;
        }
    }

    bool bDestroyWall = true;
    float fHalfInBoundWidth = (m_pWallTemplate->m_iInBoundSize/400.0f) / 2.0f;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        D3DXVECTOR3 vPos;
        PLAYER *pPlayer = g_MyD3D->MyPlayer[iLoop];
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( pPlayer->GetEmbarkUID() );

        if( pEmbark )
            vPos = D3DXVECTOR3(pEmbark->GetPosX(), pEmbark->GetPosY(), pEmbark->GetPosZ());
        else
            vPos = pPlayer->GetPosition();

        float fMovePosX = vPos.x;
        switch( m_pWallTemplate->m_iDirection )
        {
        case WALL_LEFT:
            if( m_fPosX < vPos.x )
                fMovePosX = m_fPosX - PUSH_GAP;
            break;
        case WALL_RIGHT:
            if( m_fPosX > vPos.x )
                fMovePosX = m_fPosX + PUSH_GAP;
            break;
        }

        // 부활할 때 위치는 벽 위치 기준으로 보정하자. 
        if( pPlayer->GetCurrentMotion() == MID_COMMON_ARENA && !m_pWallTemplate->m_vecCreateParticleList.empty() ) { 
            vPos.y = m_pWallTemplate->m_vecCreateParticleList.begin()->vOffset.y + 0.3f;
            switch( m_pWallTemplate->m_iDirection )
            {
            case WALL_LEFT:
                if( m_fPosX < vPos.x )
                    fMovePosX = m_fPosX - (PUSH_GAP*2.0f)
                    ;
                break;
            case WALL_RIGHT:
                if( m_fPosX > vPos.x )
                    fMovePosX = m_fPosX + (PUSH_GAP*2.0f);
                break;
            }
        }


        if( fMovePosX != vPos.x )
        {
            if( pEmbark )
                pEmbark->SetPos( fMovePosX, vPos.y, vPos.z );
            else
                pPlayer->SetDirectMove( fMovePosX, vPos.y, true, true );
        }

        if( 0 == iNumEnemyLiveMonster && m_bCheckInBound )
        {
            if( (m_fPosX - fHalfInBoundWidth) > fMovePosX || fMovePosX > (m_fPosX + fHalfInBoundWidth) )
            {
                bDestroyWall = false;
            }
        }
    }

    if( 0 == iNumEnemyLiveMonster && m_bCheckInBound )
    {
        m_bDestroyWall = bDestroyWall;
    }
}

void KGCWall::CheckInWallBound( float &fPosX )
{
    if( NULL == m_pWallTemplate )
        return;

    switch( m_pWallTemplate->m_iDirection )
    {
    case WALL_LEFT:
        if( m_fPosX < fPosX )
            fPosX = m_fPosX - PUSH_GAP;
        break;
    case WALL_RIGHT:
        if( m_fPosX > fPosX )
            fPosX = m_fPosX + PUSH_GAP;
        break;
    }
}

void KGCWall::RenderBoundingBox()
{
    if( NULL == m_pWallTemplate )
        return;

    if( g_kGlobalValue.m_bRenderBoundBox )
    {
        GCCollisionRect<float> kBody;
        D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;

        float fHalfWidth = 0.0375f;
        D3DXVECTOR4 vMyPos( m_fPosX - fHalfWidth - 1.0f, -0.5f, fHalfWidth * 2.0f, 100.0f );
        kBody.SetRect( vMyPos.x, (vMyPos.y + vMyPos.w), (vMyPos.x + vMyPos.z), vMyPos.y );
        SETVECTOR3( lefttop, kBody.m_Left, kBody.m_Top , 0.4f );
        SETVECTOR3( righttop, kBody.m_Right, kBody.m_Top , 0.4f );
        SETVECTOR3( leftbottom, kBody.m_Left, kBody.m_Bottom, 0.4f );
        SETVECTOR3( rightbottom, kBody.m_Right, kBody.m_Bottom, 0.4f );
        g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
            lefttop, righttop, leftbottom, rightbottom,0,0,0,
            D3DCOLOR_ARGB(180, 0, 200, 100));

        fHalfWidth = (m_pWallTemplate->m_iInBoundSize/400.0f) / 2.0f;
        vMyPos = D3DXVECTOR4( m_fPosX - fHalfWidth - 1.0f, -0.5f, fHalfWidth * 2.0f, 100.0f );
        kBody.SetRect( vMyPos.x, (vMyPos.y + vMyPos.w), (vMyPos.x + vMyPos.z), vMyPos.y );
        SETVECTOR3( lefttop, kBody.m_Left, kBody.m_Top , 0.4f );
        SETVECTOR3( righttop, kBody.m_Right, kBody.m_Top , 0.4f );
        SETVECTOR3( leftbottom, kBody.m_Left, kBody.m_Bottom, 0.4f );
        SETVECTOR3( rightbottom, kBody.m_Right, kBody.m_Bottom, 0.4f );
        g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
            lefttop, righttop, leftbottom, rightbottom,0,0,0,
            D3DCOLOR_ARGB(180, 0, 0, 100));
    }
}