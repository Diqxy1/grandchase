 /*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 11월 15일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 탑승물 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "KGCEmbarkObject.h"
#include "GCUI/KGCPrograssBar.h"
#include "Spark.h"
#include "GCPortal.h"

KGCEmbarkObject::KGCEmbarkObject( DWORD dwUID, KGCEmbarkTemplate* pTemplate )
{
    m_dwUID = dwUID;
    m_pOriginalTemplate = pTemplate;
    Init();
}

KGCEmbarkObject::~KGCEmbarkObject()
{
    Release();
}

void KGCEmbarkObject::Release()
{
    if( IsEmbarkation() )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerPlayerUID );
        if( pPlayer )
        {
            pPlayer->Super = 55;
            pPlayer->y_Speed = 0.03f;
            pPlayer->m_fFall_Speed = DEAULT_FALL_SPEED;
            pPlayer->IsContact = false;
            pPlayer->SetMeshToggleState( false );
            pPlayer->SetEmbarkTypeOfInPortal( INT_MAX );
            pPlayer->SetEmbarkUIDOfInPortal( INT_MAX );
        }
        SetDownEmbarkation( m_dwOwnerPlayerUID, MID_COMMON_DOWN_IN_SKY );
    }

    SAFE_REMOVE_DELETE( m_pObject );
    ReleaseParticle();
    ReleaseMotionData();
    SiKGCInGamePrograssBar()->ClearData( m_iHPBarID );
}

void KGCEmbarkObject::Init()
{
    m_iLatency = 0;
    m_iHPBarID = -1;
    m_dwOwnerPlayerUID = UINT_MAX;
    m_pObject = NULL;

    m_fSpeedX = 0.0f;
    m_fSpeedY = 0.0f;
    m_iWhatDamage = -1;
    m_bIsFly = false;
    m_bRender = false;
    m_bFrameLock = false;
    m_IsContact = false;
    m_bNotEmbarkation = false;
    m_dwNoContactFrame = 0;
    m_fHp = 0.0f;

    m_bPushCheck = !m_pOriginalTemplate->m_bNoPushCheck;

    m_iBlinkTime = -1;
    m_iShakeFrame = 0;
    m_iFlashCartoonFrame = 0;
    m_fShakeStrength = 0.0f;
    m_bDeleteReservation = false;

    m_iOriCartoon = CARTOON_DEFAULT;
    m_vecOriChildCartoon.clear();

    m_iLatencyAngleBone = -1;
    m_vLatencyAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vTraceParticlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vRealRenderPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    for(int iLoop=0; iLoop<(LATENCY + SHOWLATENCY); ++iLoop)
    {
        m_kFrameData[iLoop].Init();
    }

    ReleaseParticle();
    ReleaseMotionData();

#if !defined(__PATH__)
    m_bIsReload = true;
#endif
}

void KGCEmbarkObject::StartEmbark()
{
    if( NULL == m_pOriginalTemplate )
        return;

    SetToggleRender( true );
    SetHP( m_pOriginalTemplate->m_fMaxHp );
    CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_INIT );
}

void KGCEmbarkObject::EndEmbark( int iBlinkTime )
{
    if( -1 < m_iBlinkTime )
        return;

    if( IsEmbarkation() )
    {
        SetDownEmbarkation( m_dwOwnerPlayerUID, GetDownCharMotion(), GetDownCallback() );
    }

    m_iBlinkTime = iBlinkTime;
}

void KGCEmbarkObject::LoadResource()
{
    //LoadActionScript();
    LoadObjectMesh();
    LoadObjectMotion();
    LoadETC();
}

void KGCEmbarkObject::LoadActionScript()
{
    if( NULL == m_pOriginalTemplate )
        return;

    KLuaManager luaMgr;
    if( false == GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) )
        return;

    if( false == GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) )
        return;

    if( false == GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), m_pOriginalTemplate->m_strActionScript ) ||
        false == GCFUNC::LoadLuaScript( luaMgr, m_pOriginalTemplate->m_strActionScript ) )
    {
        return;
    }
}

void KGCEmbarkObject::LoadObjectMesh()
{
    if( NULL == m_pOriginalTemplate )
        return;

    SAFE_REMOVE_DELETE( m_pObject );
    m_pObject = g_RenderManager->CreateObject( -1 );
    LoadExtraMesh( m_pOriginalTemplate->m_vecMeshInfo );
    m_pObject->SetShellMatrixScale( m_pOriginalTemplate->m_fScale );
    m_pObject->SetMeshComputePaletteInterface( (IGCComputePaletteP3M*)this );

    std::vector< EXTRA_MESH >::const_iterator vecIter = m_pOriginalTemplate->m_vecMeshInfo.begin();
    if( vecIter != m_pOriginalTemplate->m_vecMeshInfo.end() )
        m_iOriCartoon = vecIter->dwCartoon;

    m_vecOriChildCartoon.clear();
    KGCObjectAndMotion* pChild = NULL;
    for( int iIndex = 0; (pChild = m_pObject->GetChildObject( iIndex )); iIndex++ )
    {
        m_vecOriChildCartoon.push_back( pChild->GetCartoon() );
    }
}

void KGCEmbarkObject::LoadObjectMotion()
{
    if( NULL == m_pOriginalTemplate )
        return;

    ReleaseMotionData();
    std::vector< std::pair<std::string, KGCEmbarkAction> >::const_iterator vecIter = m_pOriginalTemplate->m_vecActionList.begin();
    for(; vecIter != m_pOriginalTemplate->m_vecActionList.end(); ++vecIter)
    {
        if( 0 == static_cast<int>(strlen(vecIter->second.strMotionName.c_str())) )
            continue;

        GCDeviceMotionFRM* pFrm = g_pGCDeviceManager2->CreateMotionFRM( vecIter->second.strMotionName );
        if( pFrm == NULL || pFrm->GetDeviceState() != GCDS_LOADED || pFrm->IsDisable() )
            continue;

        m_vecMotionList.push_back( pFrm );
    }
}

void KGCEmbarkObject::LoadETC()
{
    if( NULL == m_pOriginalTemplate )
        return;

    if( 0.0f < m_pOriginalTemplate->m_fMaxHp )
    {
        DWORD dwID = m_dwUID + 1000;
        do
        {
            if( SiKGCInGamePrograssBar()->IsCreatePrograssBarFromID( dwID ) )
            {
                dwID++;
                continue;
            }
            else
            {
                KGCPrograssBar* pPrograssBar = SiKGCInGamePrograssBar()->CreatePrograssBar( dwID );
                if( pPrograssBar )
                {
                    m_iHPBarID = dwID;
                    pPrograssBar->SetDefaultTextureToWorld();
                    pPrograssBar->SetMaxValue( static_cast<int>(m_pOriginalTemplate->m_fMaxHp) );
                    pPrograssBar->SetMinValue( 0 );
                    pPrograssBar->SetNowValue( static_cast<int>(m_fHp = m_pOriginalTemplate->m_fMaxHp) );
                    pPrograssBar->SetRenderPos( -((m_pOriginalTemplate->m_fAttackWidth/400.0f)/2.0f), -0.1f );
                    pPrograssBar->SetWidth( (m_pOriginalTemplate->m_fAttackWidth/400.0f) );
                    pPrograssBar->SetHeight( 0.15f );
                    pPrograssBar->SetDrawLayer( GC_LAYER_CHAR );
                    pPrograssBar->SetTargetType( KGCPrograssBar::TARGET_EMBARK );
                    pPrograssBar->SetTargetUnitIdx( m_dwUID );
                    pPrograssBar->ToggleRender( m_bRender );
                }
            }
        } while( 0 );
    }

    ReleaseParticle();
}

void KGCEmbarkObject::ReleaseMotionData()
{
    std::vector< GCDeviceMotionFRM* >::iterator vecIter = m_vecMotionList.begin();
    for(; vecIter != m_vecMotionList.end(); ++vecIter)
    {
        SAFE_RELEASE( (*vecIter) );
    }
    m_vecMotionList.clear();
}

void KGCEmbarkObject::ReleaseParticle()
{
    std::map<std::string, std::vector<CParticleEventSeqPTR>>::iterator mapIter = m_mapParticleList.begin();
    for(; mapIter != m_mapParticleList.end(); ++mapIter)
    {
        g_ParticleManager->DeleteSequence( mapIter->second );
    }
    m_mapParticleList.clear();
}

void KGCEmbarkObject::TextRender()
{
    if( NULL == m_pOriginalTemplate )
        return;
    
    if( false == m_bRender )
        return;
    
    if( (g_pkGameOverlayUI->IsRenderOn() == false) &&
        (g_pkUIScene->m_pkAgitOverlayUI->IsRenderOn() == false) )
        return;

    KGCEmbarkFrame& kRenderData = GetRenderFrameData();
    D3DXMATRIX mat1, mat2, mat3;

    if( kRenderData.bIsRight )
    {
        D3DXMatrixScaling( &mat3, 0.5f, 0.5f, 1.0f );
    }
    else
    {
        D3DXMatrixScaling( &mat1, 0.5f, 1.5f, 1.0f );
        D3DXMatrixRotationY( &mat2, 3.1415f );
        D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
    }

    D3DXMatrixTranslation( &mat1, kRenderData.vPos.x - 1.0f, kRenderData.vPos.y - 0.48f, 0.5f );
    D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
    D3DXMatrixMultiply( &mat1, &mat2, &g_kCamera.m_matCamera );
    
    D3DXVECTOR3 vPos;
    float fTransX = -0.15f;
    float fTransY = -0.1f;
    ::D3DXMatrixTranslation( &mat2, fTransX, fTransY, 0.0f );
    ::D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
    ::D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat3 );

    vPos.x += 0.15f;
    vPos.y -= 0.04f;

    float fScreenX, fScreenY;
    fScreenX = int( ( vPos.x + 1.0f ) * (400.f * GC_SCREEN_DIV_WIDTH ) ) + 0.5f * GC_SCREEN_DIV_WIDTH;
    fScreenY = int( ( -vPos.y + 0.75f ) * ( 400.f * GC_SCREEN_DIV_HEIGHT) ) + 0.5f  * GC_SCREEN_DIV_HEIGHT;
    fScreenY -= 52.0f * GC_SCREEN_DIV_HEIGHT;
    
    fScreenX += (m_pOriginalTemplate->m_fNameOffsetX * GC_SCREEN_DIV_WIDTH);
    fScreenY += (m_pOriginalTemplate->m_fNameOffsetY * GC_SCREEN_DIV_HEIGHT);

    fScreenX *= g_pGCDeviceManager->GetWindowScaleX();
    fScreenY *= g_pGCDeviceManager->GetWindowScaleY();

    WCHAR strName[MAX_PATH];
    D3DCOLOR dxColor = 0xFF00FF00;
    PLAYER *pPlayer = GetEmbarkationPlayer();
    if( pPlayer )
    {
        dxColor = g_MyD3D->IsMyTeam(pPlayer->m_iPlayerIndex) ? SiGCTextColorManager()->StringToColor(pPlayer->GetPlayerColor()) : 0xFFFF0000;
        wsprintf( strName, L"%s(%s)", g_pkStrLoader->GetString( m_pOriginalTemplate->m_iNameID ).c_str(), pPlayer->GetPlayerName() );
    }
    else
    {
        wsprintf( strName, L"%s", g_pkStrLoader->GetString( m_pOriginalTemplate->m_iNameID ).c_str() );
    }

    g_pkFontMgrOutline->OutTextXY(static_cast<int>(fScreenX + 1), static_cast<int>(fScreenY + 1), strName, 0xFF000000, NULL, DT_CENTER | DT_TOP, 0xff000000, GCDeviceFont::INLINE);
    g_pkFontMgrOutline->OutTextXY(static_cast<int>(fScreenX), static_cast<int>(fScreenY), strName, dxColor, NULL, DT_CENTER | DT_TOP, 0xff000000, GCDeviceFont::INLINE);
    
#if !defined(__PATH__)
    if( g_kGlobalValue.m_bRenderMonsterStatus )
    {
        const KGCEmbarkAction* pAction = GetActionFromIndex( kRenderData.iStateIndex );
        if( NULL == pAction )
            return;

        PLAYER *pEmbarkationPlayer = GetEmbarkationPlayer();

        std::wstringstream strmStateHelper;    
        strmStateHelper << L"[ " << KncUtil::toWideString( pAction->strStateID.c_str() ).c_str() << L" ]" << std::endl;
        strmStateHelper << L"UID : " << static_cast<int>(GetEmbarkUID()) << std::endl;
        strmStateHelper << L"X/Y : " << static_cast<float>(GetPosX()) << ", " << static_cast<float>(GetPosY()) << std::endl;
        strmStateHelper << L"HP : " << static_cast<int>(GetHP()) << std::endl;

        fScreenY -= (100.0f * GC_SCREEN_DIV_HEIGHT);

        g_pkFontMgr->OutTextMultiline( (int)(fScreenX + 1), (int)(fScreenY + 1), strmStateHelper.str().c_str(), 0xFFFFFFFF, 1.2f, NULL, DT_CENTER|DT_TOP );
        g_pkFontMgr->OutTextMultiline( (int)fScreenX, (int)fScreenY, strmStateHelper.str().c_str(), 0xFF00FF00, 1.2f, NULL, DT_CENTER|DT_TOP );
    }

    RenderBoundingBox();
#endif
}

void KGCEmbarkObject::FrameMove()
{
    UpDateToCommonKey();
    CallLuaFunc_Action();
    UpDateToMotion();
    UpDateToMove();
    UpDateToRender();
    UpDateToLatency();
    UpDateToPush();
    IncreaseFrame();
    UpDateToETC();
}

void KGCEmbarkObject::CallLuaFunc_Action()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    KGCEmbarkFrame& kFrameData = GetFrameData();
    const KGCEmbarkAction* pAction = GetActionFromIndex( kFrameData.iStateIndex );
    if( NULL == pAction )
        return;

    CallLuaFunction( pAction->strFunctionName.c_str(), kFrameData.cFrame );

    if( IsMyOwnerOfEmbarkObject() )
        CallLuaFunction( pAction->strOwnerFunctionName.c_str(), kFrameData.cFrame );
}

void KGCEmbarkObject::CallLuaFunc_Callback( int iIndex )
{
    if( NULL == m_pOriginalTemplate )
        return;

    std::string strFunc = m_pOriginalTemplate->GetCallBackFuncName( iIndex );
    if( 0 < static_cast<int>(strlen(strFunc.c_str())) )
        CallLuaFunction( strFunc.c_str(), 0 );
}

void KGCEmbarkObject::CallLuaFunction( const char *szFunction, UCHAR cFrame )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();

    lua_checkstack( L, 10 );
    lua_pushstring( L, szFunction );
    lua_rawget( L, LUA_GLOBALSINDEX );
    if( lua_isfunction( L, -1 ) )
    {
        lua_pushinteger( L, m_dwUID );
        lua_pushinteger( L, cFrame );
        if( 0 != lua_pcall( L, 2, 0, 0 ) )
        {
#if !defined(__PATH__)
            printf( "%s\n", lua_tostring( L, -1 ) );
#endif
            lua_pop( L, 1 );
        }
    }
    else
    {
        lua_pop( L, 1 );
    }
}

void KGCEmbarkObject::UpDateToCommonKey()
{
    if( NULL == m_pOriginalTemplate )
        return;

    PLAYER *pPlayer = GetEmbarkationPlayer();
    if( NULL == pPlayer )
        return;

    if( false == pPlayer->IsLocalPlayer() )
        return;

    if( m_pOriginalTemplate->m_bCheckPortal )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Down )
        {
            KGCPortal *pPortal = NULL;
            if( NULL != (pPortal = KGCPortal::CheckPortal( pPlayer )) )
            {
                g_MyD3D->MyCtrl->k_Down = false;
                g_MyD3D->MyCtrl->k_Fresh_Down = false;

                KGC_PID_BROAD_EMBARK_IN_PORTAL kPacket;
                kPacket.dwEmbarkUID = m_dwUID;
                kPacket.fPosX = pPortal->GetPosX()-0.15f;
                SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_IN_PORTAL" );
            }
        }
    }
}

void KGCEmbarkObject::UpDateToMotion()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    if( NULL == m_pOriginalTemplate )
        return;

    if( false == IsMyOwnerOfEmbarkObject() )
        return;

    KGCEmbarkFrame& kFrameData = GetFrameData();
    if( 0 > kFrameData.iStateIndex || kFrameData.iStateIndex >= static_cast<int>(m_vecMotionList.size()) )
        return;

    float fMotionX = m_vecMotionList[ kFrameData.iStateIndex ]->GetFrameData( kFrameData.cFrame ).Plus_x * m_pOriginalTemplate->m_fScale;
    if( 0.0f != fMotionX )
    {
        if( kFrameData.bIsRight )
            kFrameData.vPos.x += fMotionX;
        else
            kFrameData.vPos.x -= fMotionX;
    }
}

void KGCEmbarkObject::UpDateToMove()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    if( NULL == m_pOriginalTemplate )
        return;

    if( false == IsMyOwnerOfEmbarkObject() )
        return;

    KGCEmbarkFrame& kFrameData = GetFrameData();
    if( 0.0f != m_fSpeedX )
    {
        float fBeforeX = kFrameData.vPos.x;
        float fAfterX = kFrameData.vPos.x + m_fSpeedX;

        if( fBeforeX < fAfterX )
        {
            fBeforeX -= FOOTHOLDSIZE;
            fAfterX += FOOTHOLDSIZE;
        }
        else
        {
            fBeforeX += FOOTHOLDSIZE;
            fAfterX -= FOOTHOLDSIZE;
        }

        if( SiKGCFantasticMap()->CheckPierceBlockXPos(fBeforeX, fAfterX, (kFrameData.vPos.y+m_fSpeedY)) )
        {
            m_fSpeedX = 0.0f;
        }
    }

    if( false == m_bIsFly )
    {
        m_fSpeedY -= DEAULT_FALL_SPEED;

        if( 0 == m_dwNoContactFrame )
        {
            KGCContectInfo kInfo( kFrameData.vPos.x, kFrameData.vPos.y, m_fSpeedX, m_fSpeedY, false, false, false );
            if( g_MyD3D->MyStg->Check_Contact( kInfo ) )
            {
                if( false == m_IsContact )
                {
                    m_IsContact = true;
                    EffectDust( 0.0f, 0.0f );
                    CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_LANDING );
                }

                if( kInfo.m_ySpeed <= 0.0f )
                {
                    m_fSpeedY = 0.0f;
                    kFrameData.vPos.y = kInfo.m_y;
                }
            }
            else
            {
                m_IsContact = false;
            }
        }
        else
        {
            --m_dwNoContactFrame;
        }
    }
    
    kFrameData.bContact = m_IsContact;
    kFrameData.vPos.x += m_fSpeedX;
    kFrameData.vPos.y += m_fSpeedY;
    
    // 스테이지 좌/우 끝 처리
    float fHalfWidth = ((m_pOriginalTemplate->m_fPushWidth/400.0f)/2.0f);
    if( kFrameData.vPos.x < fHalfWidth )
        kFrameData.vPos.x = fHalfWidth;
    if( kFrameData.vPos.x > (SiKGCFantasticMap()->GetPixelWidth() - fHalfWidth) )
        kFrameData.vPos.x = SiKGCFantasticMap()->GetPixelWidth() - fHalfWidth;
}

void KGCEmbarkObject::UpDateToPush()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    if( false == IsPushCheck() )
        return;

    PLAYER *pOwnerPlayer = GetEmbarkationPlayer();
    if( NULL == pOwnerPlayer )
        return;

    PLAYER *pMyPlayer = g_MyD3D->GetMyPlayer();
    if( NULL == pMyPlayer )
        return;

    if( g_MyD3D->IsSameTeam(pOwnerPlayer->m_iPlayerIndex, pMyPlayer->m_iPlayerIndex) )
        return;
    
    if ( pMyPlayer->m_bNoCrashCheck || !pMyPlayer->m_bCrashCheck || pMyPlayer->IsNoCrashCheckMotion(pMyPlayer->uiMotion) )
        return;

    if( pMyPlayer->uiMotion == pMyPlayer->GetOwnMotion(MID_COMMON_ARENA ) )
        return;

    if( pMyPlayer->GetHP() == 0.0f &&
        ( pMyPlayer->uiMotion == pMyPlayer->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP ) ||
        pMyPlayer->uiMotion == pMyPlayer->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP ) ||
        pMyPlayer->uiMotion == pMyPlayer->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER ) ||
        pMyPlayer->uiMotion == pMyPlayer->GetOwnMotion( MID_COMMON_DOWN_IN_SKY) ) )
        return;

    KGCEmbarkObject* pMyEmbark = pMyPlayer->GetMyEmbarkInstance();
    GCCollisionRect<float> kMyPlayerRect;
    if( pMyEmbark )
    {
        kMyPlayerRect = pMyEmbark->GetPushRect();
    }
    else
    {
        kMyPlayerRect.SetRect(pMyPlayer->vPos.x - 1.0f, pMyPlayer->vPos.y - 0.25f + pMyPlayer->y_Ex, pMyPlayer->vPos.x - 1.0f, pMyPlayer->vPos.y - 0.25f + pMyPlayer->y_Ex);
        kMyPlayerRect.m_Top -= 0.2f;    kMyPlayerRect.m_Bottom -= 0.2f;
    }
    
    GCCollisionRect<float> kColl;
    GCCollisionRect<float> kEmbark = GetPushRect();
    if( kEmbark.CheckCollision( kMyPlayerRect, &kColl ) )
    {
        float fBefore, fAfter, fY;
        float fPlayerHalfWidth = ((kMyPlayerRect.m_Right - kMyPlayerRect.m_Left)/2.0f);

        // 탑승물 대 탑승물 Push체크
        if( pMyEmbark )
        {
            fAfter = fBefore = pMyEmbark->GetPosX();
            fY = pMyEmbark->GetPosY();
            if( pMyEmbark->GetIsRight() && kEmbark.GetCenterX() > kMyPlayerRect.GetCenterX() )
                fAfter = (kEmbark.m_Left + 1.0f) - fPlayerHalfWidth;
            else if( false == pMyEmbark->GetIsRight() && kEmbark.GetCenterX() < kMyPlayerRect.GetCenterX() )
                fAfter = (kEmbark.m_Right + 1.0f) + fPlayerHalfWidth;
        }
        // 플레이어 대 탑승물 Push체크
        else
        {
            fAfter = fBefore = pMyPlayer->vPos.x;
            fY = pMyPlayer->vPos.y;
            if( kEmbark.GetCenterX() > kMyPlayerRect.GetCenterX() )
                fAfter = (kEmbark.m_Left + 1.0f) - fPlayerHalfWidth;
            else if( kEmbark.GetCenterX() < kMyPlayerRect.GetCenterX() )
                fAfter = (kEmbark.m_Right + 1.0f) + fPlayerHalfWidth;
        }

        // 사이벽 처리
        float fAfterWall = fAfter;
        if( fBefore < fAfterWall )
        {
            fBefore -= FOOTHOLDSIZE;
            fAfterWall += FOOTHOLDSIZE;
        }
        else
        {
            fBefore += FOOTHOLDSIZE;
            fAfterWall -= FOOTHOLDSIZE;
        }
        if( false == SiKGCFantasticMap()->CheckPierceBlockXPos(fBefore, fAfterWall, fY) )
            fAfterWall = fAfter;

        // 스테이지 끝 처리
        if( fAfterWall < fPlayerHalfWidth )
            fAfterWall = fPlayerHalfWidth;
        if( fAfterWall > (SiKGCFantasticMap()->GetPixelWidth() - fPlayerHalfWidth) )
            fAfterWall = SiKGCFantasticMap()->GetPixelWidth() - fPlayerHalfWidth;

        // 위치 적용
        if( pMyEmbark )
        {
            D3DXVECTOR3 vPos( fAfterWall, pMyEmbark->GetPosY(), pMyEmbark->GetPosZ() );
            pMyEmbark->SetPos( vPos.x, vPos.y, vPos.z );
        }
        else
            pMyPlayer->vPos.x = fAfterWall;
    }
}

void KGCEmbarkObject::UpDateToRender()
{
    if( NULL == m_pObject )
        return;

    if( NULL == m_pOriginalTemplate )
        return;

    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    D3DXMATRIX matScale, matRot, matTrans, matWorld;
    KGCEmbarkFrame& kRenderData = GetRenderFrameData();

    if( 0 > kRenderData.iStateIndex || kRenderData.iStateIndex >= static_cast<int>(m_vecMotionList.size()) )
        return;

    if( 0 > kRenderData.cFrame || kRenderData.cFrame >= m_vecMotionList[kRenderData.iStateIndex]->GetNum_Frame() )
        return;

    // 쉐이크 처리( 데미지와 충돌 효과로 사용됨 )
    float fShakeX = 0.0f, fShakeY = 0.0f;
    if( 0 < m_iShakeFrame )
    {
        fShakeX = (float)( rand() % 2 ? m_fShakeStrength : -m_fShakeStrength );
        fShakeY = (float)( rand() % 2 ? m_fShakeStrength : -m_fShakeStrength );
        --m_iShakeFrame;
    }

    // 월드위치 처리
    D3DXMatrixScaling( &matScale, m_pOriginalTemplate->m_fScale, m_pOriginalTemplate->m_fScale, m_pOriginalTemplate->m_fScale );
    if( kRenderData.bIsRight )
        D3DXMatrixRotationY( &matRot, 0.0f );
    else
        D3DXMatrixRotationY( &matRot, 3.1415f );

    D3DXMatrixTranslation( &matTrans,
        kRenderData.vPos.x - 1.0f + fShakeX,
        kRenderData.vPos.y + m_vecMotionList[kRenderData.iStateIndex]->GetFrameData( kRenderData.cFrame ).Pos_y * m_pOriginalTemplate->m_fScale - 0.5f + fShakeY,
        kRenderData.vPos.z );

    // 카툰 처리
    ChangeCartoon( kRenderData );

    matWorld = matScale * matRot * matTrans;

    m_pObject->SetWorldMat( &matWorld );
    m_pObject->SetFrame( &m_vecMotionList[ kRenderData.iStateIndex ]->GetFrameData( kRenderData.cFrame ) );
    m_pObject->AdjustParticlePos();

    m_vRealRenderPos = kRenderData.vPos;
}

void KGCEmbarkObject::ChangeCartoon( KGCEmbarkFrame& kRenderData )
{
    bool bRestoreCartoon = false;

    //  데미지 카툰
    if( 0 < m_iFlashCartoonFrame )
    {
        if( 0 == (--m_iFlashCartoonFrame) )
            bRestoreCartoon = true;
        else
        {
            if ( g_MyD3D->m_KGCOption.IsCartoon() )
            {
                if( kRenderData.bIsRight )
                    m_pObject->SetCartoon( CARTOON_DAMAGE_REVERSE );
                else
                    m_pObject->SetCartoon( CARTOON_DAMAGE );
                m_pObject->SetOutlineColor( EDGE_COLOR_SUPERARMOR );
            }
        }
    }

    // 카툰 복구
    if( bRestoreCartoon )
    {
        m_pObject->SetCartoon( static_cast<EGCCartoonTexture>(m_iOriCartoon) );

        KGCObjectAndMotion* pChild = NULL;
        for( int iIndex = 0; (pChild = m_pObject->GetChildObject( iIndex )); iIndex++ )
        {
            if( NULL == pChild )
                continue;

            if( iIndex >= static_cast<int>(m_vecOriChildCartoon.size()) )
                continue;

            pChild->SetCartoon( static_cast<EGCCartoonTexture>(m_vecOriChildCartoon[iIndex]) );
            pChild->SetOutlineColor( EDGE_COLOR_FRIEND );
        }
    }
}

void KGCEmbarkObject::UpDateToLatency()
{
    if( false == IsMyOwnerOfEmbarkObject() )
    {
        m_iLatency = m_iLatency + ((m_iLatency < LATENCY) ? 1 : 0);
        return;
    }

    for(int iLoop=0; iLoop<((LATENCY+SHOWLATENCY)-1); ++iLoop)
    {
        m_kFrameData[iLoop] = m_kFrameData[iLoop+1];
    }
    
    if( m_iLatency < (LATENCY-1) )
    {
        ++m_iLatency;
    }
    else
    {
        SendLatencyData();
    }
}

void KGCEmbarkObject::IncreaseFrame()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    if( m_bFrameLock )
        return;

    KGCEmbarkFrame& kFrameData = GetFrameData();
    if( 0 > kFrameData.iStateIndex || kFrameData.iStateIndex >= static_cast<int>(m_vecMotionList.size()) )
        return;

    bool bUpDateMotionFrame = false;
    if( false == (bUpDateMotionFrame = IsMyOwnerOfEmbarkObject()) )
        bUpDateMotionFrame = (m_iLatency >= LATENCY);

    if( bUpDateMotionFrame )
    {
        int iLastFrame = GetLastFrame();
        if( kFrameData.cFrame < iLastFrame )
            ++kFrameData.cFrame;
        else
            kFrameData.cFrame = iLastFrame;
    }
}

void KGCEmbarkObject::UpDateToETC()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    if( -1 != m_iHPBarID )
    {
        KGCPrograssBar* pPrograss = SiKGCInGamePrograssBar()->GetInstanceToID( m_iHPBarID );
        if( pPrograss )
            pPrograss->SetNowValue( static_cast<int>(m_fHp) );
    }

    if( -1 < m_iBlinkTime )
    {
        bool bRender = 0 < (--m_iBlinkTime % 5);

        if( m_pObject )
            m_pObject->SetRender( bRender );

        if( -1 == m_iBlinkTime )
        {
            SiKGCInGamePrograssBar()->ClearData( m_iHPBarID );
            m_iHPBarID = -1;
            m_bDeleteReservation = true;
        }
    }

    CheckParticleList();
    CheckEmbarkator();
}

void KGCEmbarkObject::LatencyAllEqual( int iLatency )
{
    if( 0 > iLatency || iLatency >= (LATENCY + SHOWLATENCY) )
        return;

    for(int iLoop=0; iLoop<(LATENCY + SHOWLATENCY); ++iLoop)
    {
        if( iLoop == iLatency )
            continue;

        m_kFrameData[iLoop] = m_kFrameData[iLatency];
    }
}

KGCEmbarkFrame& KGCEmbarkObject::GetFrameData()
{
    return m_kFrameData[ (IsMyOwnerOfEmbarkObject() ? (LATENCY + SHOWLATENCY - 1) : m_iLatency) ];
}

KGCEmbarkFrame& KGCEmbarkObject::GetRenderFrameData()
{
    return m_kFrameData[ (IsMyOwnerOfEmbarkObject() ? 0 : m_iLatency) ];
}

void KGCEmbarkObject::SetFrameData( KGC_PID_BROAD_EMBARK_FRAME_INFO *pPacket )
{
    if( NULL == pPacket )
        return;

    if( IsMyOwnerOfEmbarkObject() )
        return;

    for(int iLoop=0; iLoop<LATENCY; ++iLoop)
    {
        m_kFrameData[iLoop].bIsRight = pPacket->bIsRight[iLoop];
        m_kFrameData[iLoop].bContact = pPacket->bIsContact[iLoop];
        m_kFrameData[iLoop].iStateIndex = pPacket->iStateIndex[iLoop];
        m_kFrameData[iLoop].cFrame = pPacket->cFrame[iLoop];
        m_kFrameData[iLoop].vPos = pPacket->vPos[iLoop];
    }

    for(int iLoop=LATENCY; iLoop<LATENCY+SHOWLATENCY; ++iLoop)
    {
        m_kFrameData[iLoop].bIsRight = m_kFrameData[LATENCY-1].bIsRight;
        m_kFrameData[iLoop].bContact = m_kFrameData[LATENCY-1].bContact;
        m_kFrameData[iLoop].iStateIndex = m_kFrameData[LATENCY-1].iStateIndex;
        m_kFrameData[iLoop].cFrame = m_kFrameData[LATENCY-1].cFrame;
        m_kFrameData[iLoop].vPos = m_kFrameData[LATENCY-1].vPos;
    }

    m_fHp = pPacket->fHP;

    if( -1 != pPacket->iAngleBone )
    {
        m_mapUserBoneAngle[ pPacket->iAngleBone ] = D3DXVECTOR3(pPacket->vAngle.x, pPacket->vAngle.y, pPacket->vAngle.z);
    }

    m_iLatency = 0;
}

const KGCEmbarkAction* KGCEmbarkObject::GetActionFromIndex( int iIndex )
{
    if( NULL == m_pOriginalTemplate )
        return NULL;

    std::vector< std::pair<std::string, KGCEmbarkAction> >::const_iterator vecActionIter = m_pOriginalTemplate->m_vecActionList.begin();
    for(; vecActionIter != m_pOriginalTemplate->m_vecActionList.end(); ++vecActionIter)
    {
        if( iIndex == vecActionIter->second.m_iStateIndex )
            break;
    }

    if( vecActionIter == m_pOriginalTemplate->m_vecActionList.end() )
        return NULL;

    return &vecActionIter->second;
}

const KGCEmbarkAction* KGCEmbarkObject::GetActionFromState( const char *szState )
{
    if( NULL == m_pOriginalTemplate )
        return NULL;

    std::vector< std::pair<std::string, KGCEmbarkAction> >::const_iterator vecActionIter = m_pOriginalTemplate->m_vecActionList.begin();
    for(; vecActionIter != m_pOriginalTemplate->m_vecActionList.end(); ++vecActionIter)
    {
        if( vecActionIter->first == szState )
            break;
    }

    if( vecActionIter == m_pOriginalTemplate->m_vecActionList.end() )
        return NULL;

    return &vecActionIter->second;
}

DWORD KGCEmbarkObject::GetEmbarkType()
{
    if( NULL == m_pOriginalTemplate )
        return 0;
    return m_pOriginalTemplate->m_dwID;
}

DWORD KGCEmbarkObject::GetOwnerUID()
{
    if( UINT_MAX > m_dwOwnerPlayerUID )
        return m_dwOwnerPlayerUID;
    return g_kGlobalValue.m_kUserInfo.dwUID;
}

void KGCEmbarkObject::SetToggleRender( bool bRender )
{
    if( m_pObject )
        m_pObject->SetRender( bRender );

    KGCPrograssBar* pPrograss = SiKGCInGamePrograssBar()->GetInstanceToID( m_iHPBarID );
    if( pPrograss )
        pPrograss->ToggleRender( bRender );

    m_bRender = bRender;
}

bool KGCEmbarkObject::IsEmbarkation()
{
    if( UINT_MAX > m_dwOwnerPlayerUID )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerPlayerUID );
        if( NULL != pPlayer )
            return true;
    }

    return false;
}

bool KGCEmbarkObject::IsMyOwnerOfEmbarkObject()
{
    if( IsEmbarkation() )
    {
        PLAYER *pMyPlayer = g_MyD3D->GetMyPlayer();
        if( pMyPlayer && 
            pMyPlayer->m_kUserInfo.dwUID == m_dwOwnerPlayerUID )
            return true;
    }
    else
    {
        if( g_kGlobalValue.m_kUserInfo.bHost )
            return true;
    }

    return false;
}

PLAYER* KGCEmbarkObject::GetEmbarkationPlayer()
{
    if( UINT_MAX > m_dwOwnerPlayerUID )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerPlayerUID );
        if( NULL != pPlayer )
            return pPlayer;
    }

    return NULL;
}

void KGCEmbarkObject::SendLatencyData()
{
    if( false == IsMyOwnerOfEmbarkObject() )
        return;

    KGC_PID_BROAD_EMBARK_FRAME_INFO kPacket;
    kPacket.Init();
    kPacket.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kPacket.dwEmbarkUID = m_dwUID;
    kPacket.fHP = m_fHp;

    kPacket.iAngleBone = m_iLatencyAngleBone;
    kPacket.vAngle = m_vLatencyAngle;

    for(int iLoop=0; iLoop<LATENCY; ++iLoop)
    {
        kPacket.bIsRight[iLoop] = m_kFrameData[iLoop].bIsRight;
        kPacket.bIsContact[iLoop] = m_kFrameData[iLoop].bContact;
        kPacket.iStateIndex[iLoop] = m_kFrameData[iLoop].iStateIndex;
        kPacket.cFrame[iLoop] = m_kFrameData[iLoop].cFrame;
        kPacket.vPos[iLoop] = m_kFrameData[iLoop].vPos;
    }
    
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _UNRELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_FRAME_INFO" );
    
    m_iLatency = 0;
}

void KGCEmbarkObject::SendSyncCallback()
{
    if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
    {
        CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_SYNC );
    }
    else
    {
        KGC_PID_BROAD_EMBARK_SYNC_CALLBACK kPacket;
        kPacket.dwEmbarkUID = m_dwUID;
        kPacket.fValue = GetTempValue( "SYNC" );
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_SYNC_CALLBACK" );
    }
}

void KGCEmbarkObject::SendEmbarkation( DWORD dwPlayerUID, DWORD dwReSendCoolTime, int iBoneID )
{
    if( NULL == m_pOriginalTemplate )
        return;

    if( m_bNotEmbarkation )
        return;

    PLAYER *pMyPlayer = g_MyD3D->GetPlayerByUID( dwPlayerUID );
    if( NULL == pMyPlayer )
        return;

    if( false == pMyPlayer->IsLocalPlayer() )
        return;

    DWORD dwCoolTime = pMyPlayer->GetEmbarkSendCool();
    if( 0 < dwCoolTime )
        return;

    pMyPlayer->SetEmbarkSendCool( dwReSendCoolTime );
    if( UINT_MAX == pMyPlayer->GetEmbarkUID() )
    {
        if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
        {
            SetUpEmbarkation( dwPlayerUID, m_pOriginalTemplate->m_iUpCharMotion, KGCEmbarkTemplate::CALLBACK_UP_EMBARKATION );
            PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwPlayerUID );
            if( pPlayer )
                pPlayer->SetEmbarkAdaptBoneID( iBoneID );
        }
        else
        {
            KGC_PID_BROAD_EMBARKATION_REQ kPacket;
            kPacket.dwUID = dwPlayerUID;
            kPacket.dwEmbarkUID = m_dwUID;
            kPacket.bEmbarkation = true;
            kPacket.iBoneID = iBoneID;
            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_BROAD_EMBARKATION_REQ" );
        }
    }
    else
    {
        if( m_dwOwnerPlayerUID != dwPlayerUID )
            return;

        if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
        {
            SetDownEmbarkation( dwPlayerUID, m_pOriginalTemplate->m_iDownCharMotion, KGCEmbarkTemplate::CALLBACK_DOWN_EMBARKATION );
            PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwPlayerUID );
            if( pPlayer )
                pPlayer->SetEmbarkAdaptBoneID( -1 );
        }
        else
        {
            KGC_PID_BROAD_EMBARKATION_REQ kPacket;
            kPacket.dwUID = dwPlayerUID;
            kPacket.dwEmbarkUID = m_dwUID;
            kPacket.bEmbarkation = false;
            kPacket.iBoneID = iBoneID;
            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_BROAD_EMBARKATION_REQ" );
        }
    }
}

bool KGCEmbarkObject::SetUpEmbarkation( DWORD dwOwnerUID, int iCharMotionID, int iCallback )
{
    if( NULL == m_pOriginalTemplate )
        return false;

    if( m_bNotEmbarkation )
        return false;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwOwnerUID );
    if( NULL == pPlayer )
        return false;

    pPlayer->SetEmbarkUID( m_dwUID );
    m_dwOwnerPlayerUID = dwOwnerUID;
    LatencyAllEqual();

    if( 0 != iCharMotionID )
        pPlayer->SetPlayerState( iCharMotionID );

    if( 0 != iCallback )
        CallLuaFunc_Callback( iCallback );

    std::wstring strPlayerName = pPlayer->GetPlayerName();
    std::wstring strEmbarkName = g_pkStrLoader->GetString( m_pOriginalTemplate->m_iNameID );
    std::wstring strMsg = g_pkStrLoader->GetReplacedString( 11223, "ll", strPlayerName, strEmbarkName );
    g_MyD3D->m_kChatManager.AddSystemMsg( -1, strMsg );
    
    return true;
}

bool KGCEmbarkObject::SetDownEmbarkation( DWORD dwOwnerUID, int iCharMotionID, int iCallback )
{
    if( NULL == m_pOriginalTemplate )
        return false;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwOwnerUID );
    if( NULL == pPlayer )
        return false;

    // 플레이어객체로 이젠 탑승물객체를 얻을 수 없다.
    pPlayer->SetEmbarkUID( UINT_MAX );

    if( 0 != iCallback )
        CallLuaFunc_Callback( iCallback );

    if( 0 != iCharMotionID )
        pPlayer->SetPlayerState( iCharMotionID );

    // 탑승물객체로 이젠 플레이어객체를 얻을 수 없다.
    m_dwOwnerPlayerUID = UINT_MAX;
    LatencyAllEqual();

    return true;
}

int KGCEmbarkObject::GetLastFrame()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    if( 0 > kEmbarkFrame.iStateIndex || kEmbarkFrame.iStateIndex >= static_cast<int>(m_vecMotionList.size()) )
        return 0;

    return (m_vecMotionList[ kEmbarkFrame.iStateIndex ]->GetNum_Frame() - 1);
}

void KGCEmbarkObject::SetState( const char *szState, bool bSendLatency )
{
    if( NULL == m_pOriginalTemplate )
        return;

    if( false == IsMyOwnerOfEmbarkObject() )
        return;

    const KGCEmbarkAction* pAction = GetActionFromState( szState );
    if( NULL == pAction )
        return;

    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    kEmbarkFrame.iStateIndex = pAction->m_iStateIndex;
    kEmbarkFrame.cFrame = 0;

    if( bSendLatency )
    {
        LatencyAllEqual();
        SendLatencyData();
    }
}

const char* KGCEmbarkObject::GetState()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();

    const KGCEmbarkAction* pAction = GetActionFromIndex( kEmbarkFrame.iStateIndex );
    if( NULL == pAction )
        return "";

    return pAction->strStateID.c_str();
}

bool KGCEmbarkObject::IsState( char *szState )
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();

    const KGCEmbarkAction* pAction = GetActionFromIndex( kEmbarkFrame.iStateIndex );
    if( NULL == pAction )
        return false;

    if( pAction->strStateID == szState )
        return true;

    return false;
}

void KGCEmbarkObject::SetPos( float fX, float fY, float fZ )
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    kEmbarkFrame.vPos.x = fX;
    kEmbarkFrame.vPos.y = fY;
    kEmbarkFrame.vPos.z = fZ;
}

float KGCEmbarkObject::GetPosX()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return kEmbarkFrame.vPos.x;
}

float KGCEmbarkObject::GetPosY()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return kEmbarkFrame.vPos.y;
}

float KGCEmbarkObject::GetPosZ()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return kEmbarkFrame.vPos.z;
}

float KGCEmbarkObject::GetRenderPosX()
{
    KGCEmbarkFrame& kEmbarkFrame = GetRenderFrameData();
    return kEmbarkFrame.vPos.x;
}

float KGCEmbarkObject::GetRenderPosY()
{
    KGCEmbarkFrame& kEmbarkFrame = GetRenderFrameData();
    return kEmbarkFrame.vPos.y;
}

float KGCEmbarkObject::GetRenderPosZ()
{
    KGCEmbarkFrame& kEmbarkFrame = GetRenderFrameData();
    return kEmbarkFrame.vPos.z;
}

void KGCEmbarkObject::SetIsRight( bool bRight )
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    kEmbarkFrame.bIsRight = bRight;
}

bool KGCEmbarkObject::GetIsRight()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return kEmbarkFrame.bIsRight;
}

float KGCEmbarkObject::GetPushWidth()
{
    if( NULL == m_pOriginalTemplate )
        return 0.0f;

    return m_pOriginalTemplate->m_fPushWidth;
}

float KGCEmbarkObject::GetPushHeight()
{
    if( NULL == m_pOriginalTemplate )
        return 0.0f;

    return m_pOriginalTemplate->m_fPushHeight;
}

float KGCEmbarkObject::GetAttackWidth()
{
    if( NULL == m_pOriginalTemplate )
        return 0.0f;

    return m_pOriginalTemplate->m_fAttackWidth;
}

float KGCEmbarkObject::GetAttackHeight()
{
    if( NULL == m_pOriginalTemplate )
        return 0.0f;

    return m_pOriginalTemplate->m_fAttackHeight;
}

GCCollisionRect<float>& KGCEmbarkObject::GetAttackRect()
{
    if( NULL == m_pOriginalTemplate )
        return m_kBoundRect;

    float fHalfWidth = m_pOriginalTemplate->m_fAttackWidth/400.0f/2.0f;
    float fOffsetWidth = m_pOriginalTemplate->m_fOffsetAttackWidth;
    float fOffsetHeight = m_pOriginalTemplate->m_fOffsetAttackHeight;
    KGCEmbarkFrame& kFrameData = GetFrameData();
    D3DXVECTOR4 vMyPos( kFrameData.vPos.x - fHalfWidth - 1.0f, kFrameData.vPos.y - 0.5f, (fHalfWidth*2.0f), (m_pOriginalTemplate->m_fAttackHeight/400.0f) );

    m_kBoundRect.SetRect( (vMyPos.x + fOffsetWidth), ((vMyPos.y + vMyPos.w) + fOffsetHeight), ((vMyPos.x + vMyPos.z) + fOffsetWidth), (vMyPos.y + fOffsetHeight) );
    return m_kBoundRect;
}

GCCollisionRect<float>& KGCEmbarkObject::GetPushRect()
{
    if( NULL == m_pOriginalTemplate )
        return m_kBoundRect;

    float fHalfWidth = m_pOriginalTemplate->m_fPushWidth/400.0f/2.0f;
    float fOffsetWidth = m_pOriginalTemplate->m_fOffsetPushWidth;
    float fOffsetHeight = m_pOriginalTemplate->m_fOffsetPushHeight; 
    KGCEmbarkFrame& kFrameData = GetFrameData();
    D3DXVECTOR4 vMyPos( kFrameData.vPos.x - fHalfWidth - 1.0f, kFrameData.vPos.y - 0.5f, (fHalfWidth*2.0f), (m_pOriginalTemplate->m_fPushHeight/400.0f) );

    m_kBoundRect.SetRect( (vMyPos.x + fOffsetWidth), ((vMyPos.y + vMyPos.w) + fOffsetHeight), ((vMyPos.x + vMyPos.z) + fOffsetWidth), (vMyPos.y + fOffsetHeight) );
    return m_kBoundRect;
}

bool KGCEmbarkObject::IsPushCheck()
{
    return m_bPushCheck;
}


void KGCEmbarkObject::SetHP( float fHP, bool bCallBack )
{
    if( NULL == m_pOriginalTemplate )
        return;

    if( 0.0f >= m_pOriginalTemplate->m_fMaxHp )
        return;

    if( false == IsMyOwnerOfEmbarkObject() )
        return;

    m_fDiffHp = fHP - m_fHp;
    if( bCallBack )
    {
        if( (0.0f < m_fHp) && (0.0f != m_fDiffHp) )
        {
            CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_CHANGE_HP );
        }

        if( (0.0f < m_fHp) && ((m_fHp + m_fDiffHp) <= 0.0f) )
        {
            CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_DIE );
        }
    }

    m_fHp = m_fHp + m_fDiffHp;
    if( m_fHp > m_pOriginalTemplate->m_fMaxHp )
        m_fHp = m_pOriginalTemplate->m_fMaxHp;
    else if( m_fHp < 0.0f )
        m_fHp = 0.0f;
}

void KGCEmbarkObject::SetIsContact(bool bContact)
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    kEmbarkFrame.bContact = bContact;
    if( IsMyOwnerOfEmbarkObject() )
        m_IsContact = bContact;
}

bool KGCEmbarkObject::GetIsContact()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return kEmbarkFrame.bContact;
}

bool KGCEmbarkObject::IsDownJumpBlock()
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    return ( g_MyD3D->MyStg->IsEnableNoCheck(kEmbarkFrame.vPos.x, kEmbarkFrame.vPos.y) && kEmbarkFrame.vPos.y > -0.5f);
}

void KGCEmbarkObject::EffectDust( float fOffsetX, float fOffsetY )
{
    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    if( false == kEmbarkFrame.bIsRight )
    {
        fOffsetX = -fOffsetX;
    }

    D3DXVECTOR2 vDustPos( kEmbarkFrame.vPos.x - 1.0f + fOffsetX, kEmbarkFrame.vPos.y - 0.2f + fOffsetY );
    g_MyD3D->MySparks->Smogs( vDustPos.x, vDustPos.y, 0 );
}

int KGCEmbarkObject::GetUpCharMotion()
{
    if( NULL == m_pOriginalTemplate )
        return 0;
    return m_pOriginalTemplate->m_iUpCharMotion;
}

int KGCEmbarkObject::GetDownCharMotion()
{
    if( NULL == m_pOriginalTemplate )
        return 0;
    return m_pOriginalTemplate->m_iDownCharMotion;
}

int KGCEmbarkObject::GetUpCallback()
{
    if( NULL == m_pOriginalTemplate )
        return 0;
    return KGCEmbarkTemplate::CALLBACK_UP_EMBARKATION;
}

int KGCEmbarkObject::GetDownCallback()
{
    if( NULL == m_pOriginalTemplate )
        return 0;
    return KGCEmbarkTemplate::CALLBACK_DOWN_EMBARKATION;
}

int KGCEmbarkObject::GetDefaultCharUpBoneID()
{
    if( NULL == m_pOriginalTemplate )
        return -1;
    return m_pOriginalTemplate->m_iDefaultCharUpBoneID;
}

bool KGCEmbarkObject::IsCheckPortal()
{
    if( NULL == m_pOriginalTemplate )
        return false;
    return m_pOriginalTemplate->m_bCheckPortal;
}

bool KGCEmbarkObject::IsNotDrawPetName()
{
    if( NULL == m_pOriginalTemplate )
        return false;
    return m_pOriginalTemplate->m_bNotDrawPetName;
}

bool KGCEmbarkObject::CheckPosInRange( float fPosX, float fPosY, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY )
{
    float fOffsetX = static_cast<float>(iOffsetX / 400.0f);
    float fOffsetY = static_cast<float>(iOffsetY / 400.0f);

    // 게임안에서 사용하는 수치로 바꾼다..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    if( kEmbarkFrame.bIsRight )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += kEmbarkFrame.vPos.x;
    iX2 += kEmbarkFrame.vPos.x;
    iY2 += kEmbarkFrame.vPos.y;
    iY1 += kEmbarkFrame.vPos.y;

    if( fPosX > iX1 && fPosX < iX2 &&
        fPosY > iY2 && fPosY < iY1 )
    {  
        return true;
    }

    return false;
}

bool KGCEmbarkObject::CheckInRangeToPlayer( int iPlayerIndex, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY )
{
    if( false == g_MyD3D->IsPlayerIndex(iPlayerIndex) )
        return false;

    PLAYER *pPlayer = g_MyD3D->GetPlayer( iPlayerIndex );
    if( NULL == pPlayer )
        return false;

    KGCEmbarkFrame& kEmbarkFrame = GetFrameData();
    float fOffsetX = static_cast<float>(iOffsetX / 400.0f);
    float fOffsetY = static_cast<float>(iOffsetY / 400.0f);
    if( false == kEmbarkFrame.bIsRight )
    {
        fOffsetX = fOffsetX * -1.0f;
    }

    float iX1 = kEmbarkFrame.vPos.x + fOffsetX + static_cast<float>(iLeft / 400.0f);
    float iY1 = kEmbarkFrame.vPos.y + fOffsetY + static_cast<float>(iTop / 400.0f);
    float iX2 = kEmbarkFrame.vPos.x + fOffsetX + static_cast<float>(iRight / 400.0f);
    float iY2 = kEmbarkFrame.vPos.y + fOffsetY + static_cast<float>(iBottom / 400.0f);

    if( pPlayer->vPos.x > iX1 && pPlayer->vPos.x < iX2 &&
        pPlayer->vPos.y > iY2 && pPlayer->vPos.y < iY1 )
    {
        return true;
    }

    return false;
}

void KGCEmbarkObject::AddParticle( char *szIdentifier, char *szSequence, float fOffsetX, float fOffsetY )
{
    CParticleEventSeqPTR pSequence = g_ParticleManager->CreateSequence( szSequence, (GetRenderPosX() - 1.0f) + fOffsetX, (GetRenderPosY() - 0.5f) + fOffsetY, 0.5f );
    if( NULL == pSequence )
        return;

    InsertParticleSequence( szIdentifier, pSequence );
}

void KGCEmbarkObject::AddTraceParticle( char *szIdentifier, char *szSequence, float fOffsetX, float fOffsetY, float fTraceTime )
{
    CParticleEventSeqPTR pSequence = g_ParticleManager->CreateSequence( szSequence, (GetRenderPosX() - 1.0f), (GetRenderPosY() - 0.5f), 0.5f );
    if( NULL == pSequence )
        return;

    KGCEmbarkFrame& kEmbarkFrame = m_kFrameData[0];

    TRACE_INFO stTrace;
    stTrace.m_pvPos = &m_vTraceParticlePos;
    stTrace.m_fXOffset = fOffsetX;
    stTrace.m_fYOffset = fOffsetY;
    stTrace.m_fParticleTraceTime = fTraceTime;
    pSequence->SetTrace( &stTrace );

    InsertParticleSequence( szIdentifier, pSequence );
}

void KGCEmbarkObject::AddTraceParticleToBone( char *szIdentifier, char *szSequence, int iBoneID, float fOffsetX, float fOffsetY, float fTraceTime, bool bNoDirection )
{
    if ( NULL == m_pObject )
        return;

    D3DXVECTOR2 vPos(0.0f, 0.0f);
    if( bNoDirection ) 
        vPos.x = fOffsetX;
    vPos.y = fOffsetY;

    KGCEmbarkFrame& kEmbarkFrame = m_kFrameData[0];
    CParticleEventSeqPTR pSequence = g_ParticleManager->CreateSequence( szSequence, kEmbarkFrame.vPos.x + vPos.x, kEmbarkFrame.vPos.y + vPos.y, 0.5f );
    if( NULL == pSequence )
        return;

    if( false == bNoDirection )
    {
        DIRECTION_INFO stDirect;
        stDirect.m_bDirVelocity = true;
        stDirect.m_fDirPosX = -fOffsetX;
        stDirect.m_pbIsRight = &kEmbarkFrame.bIsRight;
        pSequence->SetDirectInfo( &stDirect );
        pSequence->SetShow( true );
    }

    TRACE_INFO stTrace;
    if( fTraceTime == 0.0f )
        stTrace.m_fParticleTraceTime = pSequence->GetKillTime();
    else
        stTrace.m_fParticleTraceTime = fTraceTime;
    stTrace.m_fXOffset = vPos.x;
    stTrace.m_fYOffset = vPos.y;

    if( -1 == iBoneID )
    {
        stTrace.m_pvPos = &m_vTraceParticlePos;
        pSequence->SetShow( &m_bRender );

    }
    else
    {
        ParticlePosDesc* pDesc = new ParticlePosDesc();			
        stTrace.m_pvPos =  &pDesc->m_vPos;			
        pDesc->pParticle = pSequence;
        pDesc->m_iBoneIndex = iBoneID;
        pDesc->m_iLeftSideLayer = pSequence->GetLayer();
        pDesc->m_iRightSideLayer = pSequence->GetLayer();
        
        pSequence->SetShow( &m_bRender );
        m_pObject->m_vecPosDesc.push_back( pDesc );

        if( 0.0f > stTrace.m_fParticleTraceTime )
        {
            D3DXMATRIX matPartcle;
            const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();
            m_pObject->GetAngleBone( iBoneID, &matPartcle );
            D3DXMatrixMultiply( &matPartcle, &matPartcle, matWorld );
            pSequence->SetPosition( D3DXVECTOR3(matPartcle._41,matPartcle._42,matPartcle._43)  );
        }
    }
    pSequence->SetTrace( &stTrace );

    InsertParticleSequence( szIdentifier, pSequence );
}

void KGCEmbarkObject::InsertParticleSequence( char *szIdentifier, CParticleEventSeqPTR pSequence )
{
    std::map<std::string, std::vector<CParticleEventSeqPTR>>::iterator mapIter = m_mapParticleList.find( szIdentifier );
    if( mapIter != m_mapParticleList.end() )
    {
        mapIter->second.push_back( pSequence );
    }
    else
    {
        std::vector<CParticleEventSeqPTR> vecSeqeunce;
        vecSeqeunce.push_back( pSequence );
        m_mapParticleList.insert( std::make_pair(szIdentifier, vecSeqeunce) );
    }
}

void KGCEmbarkObject::DestroyParticle( char *szIdentifier )
{
    std::map<std::string, std::vector<CParticleEventSeqPTR>>::iterator mapIter = m_mapParticleList.find( szIdentifier );
    if( mapIter != m_mapParticleList.end() )
    {
        g_ParticleManager->DeleteSequence( mapIter->second );
        m_mapParticleList.erase( mapIter );
    }
}

void KGCEmbarkObject::CheckParticleList()
{
    KGCEmbarkFrame& kEmbarkFrame = GetRenderFrameData();
    m_vTraceParticlePos = kEmbarkFrame.vPos;

    std::map<std::string, std::vector<CParticleEventSeqPTR>>::iterator mapIter = m_mapParticleList.begin();
    for(; mapIter != m_mapParticleList.end(); )
    {
        std::vector<CParticleEventSeqPTR>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( g_ParticleManager->IsLiveInstance( *vecIter ) )
                break;
        }
        if( vecIter == mapIter->second.end() )
        {
            mapIter = m_mapParticleList.erase( mapIter );
            continue;
        }

        ++mapIter;
    }
}

void KGCEmbarkObject::CheckEmbarkator()
{
    if( false == IsEmbarkation() )
        return;

    if( !g_pkQuestManager->IsStartedStage() && g_pkQuestManager->m_iCurStage == 0 )
        return;

    PLAYER *pPlayer = GetEmbarkationPlayer();
    if( NULL == pPlayer )
        return;

    if( pPlayer->m_kUserInfo.nUserState != GC_RUS_PLAYING )
    {
        SetDownEmbarkation( m_dwOwnerPlayerUID, GetDownCharMotion(), GetDownCallback() );
    }
}

void KGCEmbarkObject::PlaySound( char* strSound, int iLoop /*= 0*/ )
{
    g_KDSound.Play( strSound, iLoop, LATENCY_BUFFER_SIZE );
}

void KGCEmbarkObject::StopSound( char* strSound )
{
    g_KDSound.Stop( strSound );
}

bool KGCEmbarkObject::IsLiveParticle( char *szIdentifier )
{
    if( NULL == szIdentifier )
        return false;

    std::map<std::string, std::vector<CParticleEventSeqPTR>>::iterator mapIter = m_mapParticleList.find( szIdentifier );
    if( mapIter == m_mapParticleList.end() )
        return false;
        
    std::vector<CParticleEventSeqPTR>::iterator vecIter = mapIter->second.begin();
    for(; vecIter != mapIter->second.end(); ++vecIter )
    {
        if( g_ParticleManager->IsLiveInstance( *vecIter ) )
            break;
    }

    if( vecIter == mapIter->second.end() )
    {
        m_mapParticleList.erase( mapIter );
        return false;
    }

    return true;
}

void KGCEmbarkObject::GetBoneMatrix( unsigned int iIndex, D3DXMATRIX* pOutMatrix )
{
    if( NULL == m_pOriginalTemplate )
        return;

    if( NULL == pOutMatrix )
        return;

    const D3DXMATRIX *matBone;
    matBone = m_pObject->GetAngleBone( iIndex );

    if( NULL == matBone )
        return;

    if( m_pOriginalTemplate->m_bOnlyPosBone )
    {
        pOutMatrix->_41 = matBone->_41;
        pOutMatrix->_42 = matBone->_42;
        pOutMatrix->_43 = matBone->_43;
    }
    else
    {
        *pOutMatrix = *matBone;
    }
}

void KGCEmbarkObject::SetTempValue( char *strKey, float fValue )
{
    m_mapTempValue[strKey] = fValue;
}

float KGCEmbarkObject::GetTempValue( char *strKey )
{
    std::map<std::string, float>::iterator mapIter = m_mapTempValue.find( strKey );
    if( mapIter != m_mapTempValue.end() )
        return mapIter->second;
    return 0.0f;
}

D3DXVECTOR3 KGCEmbarkObject::GetUserBoneAngle( int iBoneIndex )
{
    std::map<int, D3DXVECTOR3>::iterator mapIter = m_mapUserBoneAngle.find( iBoneIndex );
    if( mapIter != m_mapUserBoneAngle.end() )
        return mapIter->second;
    return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void KGCEmbarkObject::SetUserBoneAngle( int iBoneIndex, float fAngleX, float fAngleY, float fAngleZ, bool bSetLatency )
{
    m_mapUserBoneAngle[iBoneIndex] = D3DXVECTOR3( fAngleX, fAngleY, fAngleZ );

    // AngleBone의 동기화는 Bone하나만 가능합니다..ㅠㅠ( 캐릭터의 경우 BodyBone만 하고있음. )
    // map을 패킷에 추가하게되면 패킷직렬화 시켜주면 되긴하지만
    // 레이턴시 데이터는 최대경량화를 해야되기 때문에 map을 담아주기 부담스러워
    // 하나만 가능하도록 해두었습니다.
    m_iLatencyAngleBone = -1;
    m_vLatencyAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    if( bSetLatency )
    {
        m_iLatencyAngleBone = iBoneIndex;
        m_vLatencyAngle = D3DXVECTOR3(fAngleX, fAngleY, fAngleZ);
    }
}

D3DXMATRIX* KGCEmbarkObject::ComputePositionBone( D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone )
{
    return IGCComputePaletteP3M::ComputePositionBone( matOut, Bone_Num, matBasic, vecPositionBone);
}

D3DXMATRIX* KGCEmbarkObject::ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic )
{
    if( pFrame == NULL || pFrame->A_Bone == NULL )
        return matOut;

    if ( pFrame->dwNumAngleBone <= (DWORD)Bone_Num  )
    {
        *matOut = matBasic;
        return matOut;
    }

    std::map<int, D3DXVECTOR3>::iterator mapIter = m_mapUserBoneAngle.find( Bone_Num );
    if( mapIter != m_mapUserBoneAngle.end() )
    {
        D3DXMATRIX matRotX, matRotY, matRotZ, matRotation;

        D3DXMatrixIdentity( &matRotX );
        D3DXMatrixIdentity( &matRotY );
        D3DXMatrixIdentity( &matRotZ );
        D3DXMatrixIdentity( &matRotation );

        if( 0.0f != mapIter->second.x )
        {
            D3DXMatrixRotationX( &matRotX, mapIter->second.x );
        }
        if( 0.0f != mapIter->second.y )
        {
            D3DXMatrixRotationY( &matRotY, mapIter->second.y );
        }
        if( 0.0f != mapIter->second.z )
        {
            D3DXMatrixRotationZ( &matRotZ, mapIter->second.z );
        }

        matRotation = matRotX * matRotY * matRotZ * matBasic;
        D3DXMatrixMultiply(matOut, &pFrame->A_Bone[Bone_Num], &matRotation);

        return matOut;
    }

    return IGCComputePaletteP3M::ComputeAngleBone(matOut, pFrame, Bone_Num, matBasic );
}

void KGCEmbarkObject::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCEmbarkObject>( L, "KGCEmbarkObject" );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetLastFrame",               &KGCEmbarkObject::GetLastFrame );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetPos",                     &KGCEmbarkObject::SetPos );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetPosX",                    &KGCEmbarkObject::GetPosX );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetPosY",                    &KGCEmbarkObject::GetPosY );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetPosZ",                    &KGCEmbarkObject::GetPosZ );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetRenderPosX",              &KGCEmbarkObject::GetRenderPosX );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetRenderPosY",              &KGCEmbarkObject::GetRenderPosY );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetRenderPosZ",              &KGCEmbarkObject::GetRenderPosZ );
    
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetIsRight",                 &KGCEmbarkObject::SetIsRight );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetIsRight",                 &KGCEmbarkObject::GetIsRight );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetSpeedX",                 &KGCEmbarkObject::SetSpeedX );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetSpeedY",                 &KGCEmbarkObject::SetSpeedY );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetSpeedX",                 &KGCEmbarkObject::GetSpeedX );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetSpeedY",                 &KGCEmbarkObject::GetSpeedY );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetFly",                    &KGCEmbarkObject::SetFly );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetFly",                    &KGCEmbarkObject::GetFly );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetHP",                     &KGCEmbarkObject::SetHP );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetHP",                     &KGCEmbarkObject::GetHP );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetDiffHP",                 &KGCEmbarkObject::SetDiffHP );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetDiffHP",                 &KGCEmbarkObject::GetDiffHP );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetWhatDamage",             &KGCEmbarkObject::SetWhatDamage );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetWhatDamage",             &KGCEmbarkObject::GetWhatDamage );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetNotEmbarkation",         &KGCEmbarkObject::SetNotEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetNotEmbarkation",         &KGCEmbarkObject::GetNotEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetIsContact",              &KGCEmbarkObject::SetIsContact );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetIsContact",              &KGCEmbarkObject::GetIsContact );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetNoContactFrame",         &KGCEmbarkObject::SetNoContactFrame );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetNoContactFrame",         &KGCEmbarkObject::GetNoContactFrame );
    lua_tinker::class_def<KGCEmbarkObject>( L, "IsDownJumpBlock",           &KGCEmbarkObject::IsDownJumpBlock );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetToggleRender",           &KGCEmbarkObject::GetToggleRender );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetToggleRender",           &KGCEmbarkObject::SetToggleRender );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetPushCheck",              &KGCEmbarkObject::GetPushCheck );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetPushCheck",              &KGCEmbarkObject::SetPushCheck );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetFrameLock",              &KGCEmbarkObject::SetFrameLock );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetFrameLock",              &KGCEmbarkObject::GetFrameLock );

    lua_tinker::class_def<KGCEmbarkObject>( L, "GetOwnerUID",               &KGCEmbarkObject::GetOwnerUID );
    lua_tinker::class_def<KGCEmbarkObject>( L, "IsEmbarkation",             &KGCEmbarkObject::IsEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "IsMyOwnerOfEmbarkObject",   &KGCEmbarkObject::IsMyOwnerOfEmbarkObject );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetEmbarkationPlayer",      &KGCEmbarkObject::GetEmbarkationPlayer );
    
    lua_tinker::class_def<KGCEmbarkObject>( L, "SendEmbarkation",            &KGCEmbarkObject::SendEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SendSyncCallback",           &KGCEmbarkObject::SendSyncCallback );
    
    lua_tinker::class_def<KGCEmbarkObject>( L, "CheckPosInRange",            &KGCEmbarkObject::CheckPosInRange );
    lua_tinker::class_def<KGCEmbarkObject>( L, "CheckInRangeToPlayer",       &KGCEmbarkObject::CheckInRangeToPlayer );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetUpEmbarkation",           &KGCEmbarkObject::SetUpEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetDownEmbarkation",         &KGCEmbarkObject::SetDownEmbarkation );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetState",                   &KGCEmbarkObject::SetState );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetState",                   &KGCEmbarkObject::GetState );
    lua_tinker::class_def<KGCEmbarkObject>( L, "IsState",                    &KGCEmbarkObject::IsState );
    lua_tinker::class_def<KGCEmbarkObject>( L, "EndEmbark",                  &KGCEmbarkObject::EndEmbark );

    lua_tinker::class_def<KGCEmbarkObject>( L, "GetUpCharMotion",            &KGCEmbarkObject::GetUpCharMotion );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetDownCharMotion",          &KGCEmbarkObject::GetDownCharMotion );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetDownCallback",            &KGCEmbarkObject::GetDownCallback );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetUpCallback",              &KGCEmbarkObject::GetUpCallback );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetEmbarkType",              &KGCEmbarkObject::GetEmbarkType );

    lua_tinker::class_def<KGCEmbarkObject>( L, "GetUserBoneAngle",           &KGCEmbarkObject::GetUserBoneAngle );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetUserBoneAngle",           &KGCEmbarkObject::SetUserBoneAngle );

    lua_tinker::class_def<KGCEmbarkObject>( L, "AddParticle",                &KGCEmbarkObject::AddParticle );
    lua_tinker::class_def<KGCEmbarkObject>( L, "AddTraceParticle",           &KGCEmbarkObject::AddTraceParticle );
    lua_tinker::class_def<KGCEmbarkObject>( L, "AddTraceParticleToBone",     &KGCEmbarkObject::AddTraceParticleToBone );
    lua_tinker::class_def<KGCEmbarkObject>( L, "DestroyParticle",            &KGCEmbarkObject::DestroyParticle );
    lua_tinker::class_def<KGCEmbarkObject>( L, "IsLiveParticle",             &KGCEmbarkObject::IsLiveParticle );

    lua_tinker::class_def<KGCEmbarkObject>( L, "SetTempValue",               &KGCEmbarkObject::SetTempValue );
    lua_tinker::class_def<KGCEmbarkObject>( L, "GetTempValue",               &KGCEmbarkObject::GetTempValue );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetShake",                   &KGCEmbarkObject::SetShake );
    lua_tinker::class_def<KGCEmbarkObject>( L, "SetDamageCartoon",           &KGCEmbarkObject::SetDamageCartoon );
    lua_tinker::class_def<KGCEmbarkObject>( L, "PlaySound",                  &KGCEmbarkObject::PlaySound );
    lua_tinker::class_def<KGCEmbarkObject>( L, "StopSound",                  &KGCEmbarkObject::StopSound );

#if !defined(__PATH__)
    lua_tinker::class_mem<KGCEmbarkObject>( L, "m_bIsReload",                &KGCEmbarkObject::m_bIsReload );
    lua_tinker::class_def<KGCEmbarkObject>( L, "RenderBox",                  &KGCEmbarkObject::RenderBox );
#endif
}

#if !defined(__PATH__)
void KGCEmbarkObject::RenderBoundingBox()
{    
    if( g_kGlobalValue.m_bRenderBoundBox )
    {
        if( NULL == m_pOriginalTemplate )
            return;

        GCCollisionRect<float> kBody;
        D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;

        float fHalfWidth = m_pOriginalTemplate->m_fAttackWidth/400.0f/2.0f;
        float fOffsetWidth = m_pOriginalTemplate->m_fOffsetAttackWidth;
        float fOffsetHeight = m_pOriginalTemplate->m_fOffsetAttackHeight;
        D3DXVECTOR4 vMyPos( m_vRealRenderPos.x - fHalfWidth - 1.0f, m_vRealRenderPos.y - 0.5f, (fHalfWidth*2.0f), (m_pOriginalTemplate->m_fAttackHeight/400.0f) );
        kBody.SetRect( (vMyPos.x + fOffsetWidth), ((vMyPos.y + vMyPos.w) + fOffsetHeight), ((vMyPos.x + vMyPos.z) + fOffsetWidth), (vMyPos.y + fOffsetHeight) );
      
        SETVECTOR3( lefttop, kBody.m_Left, kBody.m_Top , 0.4f );
        SETVECTOR3( righttop, kBody.m_Right, kBody.m_Top , 0.4f );
        SETVECTOR3( leftbottom, kBody.m_Left, kBody.m_Bottom, 0.4f );
        SETVECTOR3( rightbottom, kBody.m_Right, kBody.m_Bottom, 0.4f );

        g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
            lefttop, righttop, leftbottom, rightbottom,0,0,0,
            D3DCOLOR_ARGB(180, 0, 200, 100));

        fHalfWidth = m_pOriginalTemplate->m_fPushWidth/400.0f/2.0f;
        fOffsetWidth = m_pOriginalTemplate->m_fOffsetPushWidth;
        fOffsetHeight = m_pOriginalTemplate->m_fOffsetPushHeight;
        vMyPos = D3DXVECTOR4( m_vRealRenderPos.x - fHalfWidth - 1.0f, m_vRealRenderPos.y - 0.5f, (fHalfWidth*2.0f), (m_pOriginalTemplate->m_fPushHeight/400.0f) );
        kBody.SetRect( (vMyPos.x + fOffsetWidth), ((vMyPos.y + vMyPos.w) + fOffsetHeight), ((vMyPos.x + vMyPos.z) + fOffsetWidth), (vMyPos.y + fOffsetHeight) );
        SETVECTOR3( lefttop, kBody.m_Left, kBody.m_Top , 0.4f );
        SETVECTOR3( righttop, kBody.m_Right, kBody.m_Top , 0.4f );
        SETVECTOR3( leftbottom, kBody.m_Left, kBody.m_Bottom, 0.4f );
        SETVECTOR3( rightbottom, kBody.m_Right, kBody.m_Bottom, 0.4f );

        g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
            lefttop, righttop, leftbottom, rightbottom,0,0,0,
            D3DCOLOR_ARGB(180, 0, 0, 100));
    }
}

void KGCEmbarkObject::RenderBox( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    KGCEmbarkFrame& kEmbarkFrame = GetRenderFrameData();
    float fOffsetX = static_cast<float>(iOffsetX / 400.0f);
    float fOffsetY = static_cast<float>(iOffsetY / 400.0f);
    if( false == kEmbarkFrame.bIsRight )
    {
        fOffsetX = fOffsetX * -1.0f;
    }

    float iX1 = m_vRealRenderPos.x + fOffsetX + static_cast<float>(iLeft / 400.0f);
    float iY1 = m_vRealRenderPos.y + fOffsetY + static_cast<float>(iTop / 400.0f);
    float iX2 = m_vRealRenderPos.x + fOffsetX + static_cast<float>(iRight / 400.0f);
    float iY2 = m_vRealRenderPos.y + fOffsetY + static_cast<float>(iBottom / 400.0f);

    D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
    SETVECTOR3( lefttop, iX1- 1.0f, iY1- 0.25f , 0.4f );
    SETVECTOR3( righttop, iX2- 1.0f, iY1- 0.25f , 0.4f );
    SETVECTOR3( leftbottom, iX1- 1.0f, iY2- 0.25f , 0.4f );
    SETVECTOR3( rightbottom, iX2- 1.0f, iY2- 0.25f , 0.4f );

    g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
        lefttop, righttop, leftbottom, rightbottom,0,0,0,
        D3DCOLOR_ARGB(90, 0, 200, 0));
}
#endif