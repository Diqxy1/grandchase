#include "stdafx.h"
//
//

#include "KDInput.h"
#include "GCCameraOrtho.h"
#include "GCUI/GCHeadupDisplay.h"


#include "Monster.h"
#include "Stage.h"

#include "Headup Display.h"
#include "Replay.h"

#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"
#include "./Square/GCSquare.h"
#include "./Square/GCSquarePeople.h"
#include "GCUI/KGCSquareOverlayUI.h"
#include "buff/KGCBuffManager.h"
#include "KGCWorldMapManager.h"
#include "KStats.h"

#if defined( ENABLE_SQUARE_LOGGING )

#endif

KGCCameraOrtho g_kCamera;

KGCCameraOrtho::KGCCameraOrtho()
: m_dwSquareUID(ULONG_MAX)
, m_bObserve(false)
{
    m_fZoom = 1.0f;
    m_fTargetZoom = 1.0f;
    m_iZoomCount = 0;
    m_iTargetPlayer = 0;
    m_bZoomViewMode = false;
    m_bFollow = true;
    m_iHostViewZoomMode = 0;

    m_bMonsterCamera = false;
    m_iTargetMonster = 0;
    m_fWideMode = 0.0f;
    m_fPrevWideMode = 0.0f;

    m_fLookX = 0.0f;
    m_fLookY = 0.0f;
    m_iEarthQuake = 0;
    m_iEarthQuakeY = 0;
    m_iShakeCameraFrame = 0;
    m_vCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    m_fTargetOffsetX = 0.0f;
    m_fTargetOffsetY = 0.0f;

    m_fStaticX = 0.0f;
    m_fStaticY = 0.0f;
    m_bQuestStartedOfResetCamera = true;

    D3DXMatrixIdentity(&m_matCamera);
}

KGCCameraOrtho::~KGCCameraOrtho()
{
}

void KGCCameraOrtho::RegisterLuabind()
{
    lua_tinker::class_add<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "KGCCameraOrtho" );
		lua_tinker::class_mem<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(),  "WideMode",          &KGCCameraOrtho::m_fWideMode  );
		lua_tinker::class_mem<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(),  "m_fLookX",          &KGCCameraOrtho::m_fLookX  );
		lua_tinker::class_mem<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(),  "m_fLookY",          &KGCCameraOrtho::m_fLookY  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetZoom", 			&KGCCameraOrtho::SetZoom );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetEarthQuake", 		&KGCCameraOrtho::SetEarthQuake );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetTargetPlayer", 	&KGCCameraOrtho::SetTargetPlayer );
		lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetTargetMonster", 	&KGCCameraOrtho::SetTargetMonster  );
		lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "InitCamera", 		&KGCCameraOrtho::InitCamera  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "Follow", 			&KGCCameraOrtho::Follow  );
		lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "GetFollow", 			&KGCCameraOrtho::GetFollow  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "CameraVib", 			&KGCCameraOrtho::CameraVib  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "CheckDashZoom", 		&KGCCameraOrtho::CheckDashZoom  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "CheckJumpZoom", 		&KGCCameraOrtho::CheckJumpZoom  );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SavePrevState", 		&KGCCameraOrtho::SavePrevState );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "BackToPrevState",	&KGCCameraOrtho::BackToPrevState );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetTargetOffsetX",	&KGCCameraOrtho::SetTargetOffsetX );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetTargetOffsetY",	&KGCCameraOrtho::SetTargetOffsetY );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetStaticX",	        &KGCCameraOrtho::SetStaticX );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetStaticY",	        &KGCCameraOrtho::SetStaticY );
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "SetQuestStartedOfResetCamera",	        &KGCCameraOrtho::SetQuestStartedOfResetCamera );        
        lua_tinker::class_def<KGCCameraOrtho>( KGCLuabinder::getInstance().GetLuaState(), "IsMonsterTargetCamera",	        &KGCCameraOrtho::IsMonsterTargetCamera );        
        

    lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "g_kCamera", this );
}

void KGCCameraOrtho::UpdateCameraMatrix()
{
	/*
    D3DXMATRIX mat1, mat2;
    D3DXMatrixScaling ( &mat1, m_fZoom, m_fZoom, 1.0f );
    // NOTE : X-마스의 바벨에서는 자동으로 카메라가 스크롤된다.
    if ( g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS
         && SiKGCRoomManager()->IsMonsterGameMode() == false
        )
    {
        D3DXMatrixTranslation( &mat2, -m_fLookX * m_fZoom, -g_MyD3D->MyStg->m_fScrollY * m_fZoom - 0.3125f, 0.0f );
    }
    else
    {
        D3DXMatrixTranslation( &mat2, -m_fLookX * m_fZoom, -m_fLookY * m_fZoom - 0.325f, 0.0f );
    }
    D3DXMatrixMultiply( &m_matCamera, &mat1, &mat2 );*/


	// 카메라 매트릭스에서 주석 처리한 부분은 결코 변하지 않는다 
	// 따라서 매번 초기화 해 줄 필요가 없다 
	m_matCamera._11=m_fZoom;	//m_matCamera._12=0;			m_matCamera._13=0;			m_matCamera._14=0; 
	/*m_matCamera._21=0;*/			m_matCamera._22=m_fZoom;	//m_matCamera._23=0;			m_matCamera._24=0;
	/*m_matCamera._31=0;			m_matCamera._32=0;*/			m_matCamera._33=1.0f;		//m_matCamera._34=0;
			
	m_matCamera._41=-m_fLookX * m_fZoom;	
	//m_matCamera._43=0.0f;	
	//m_matCamera._44=1.0f;	

	// NOTE : X-마스의 바벨에서는 자동으로 카메라가 스크롤된다.
	if ( g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS
		&& SiKGCRoomManager()->IsMonsterGameMode() == false
		)		
		m_matCamera._42=-g_MyD3D->MyStg->m_fScrollY * m_fZoom - 0.3125f;
	else	
		m_matCamera._42=-m_fLookY * m_fZoom - 0.325f;
}

void KGCCameraOrtho::SetZoom(float fZoom , char cZoomPlayer)
{
    if( ( cZoomPlayer != -1 && g_pMagicEffect->IsMagicEffect( cZoomPlayer, EGC_EFFECT_EAGLE_SITE ) ) || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME  )
        return;

    if( m_fWideMode != 0.0f )
    {
        if( fZoom < (float)m_fWideMode / 10.0f )//기본 Zoom보다 작아지게는 안된다
        {
            m_iZoomCount = 0;
            return;
        }
    }

    m_iZoomCount = 10;

    if ( fZoom < 1.0f )
        m_iZoomCount = 30;

    m_fTargetZoom = fZoom;
}

void KGCCameraOrtho::SetCameraPosition(float fX, float fY, int iPosMode)
{
    float Plus_x = 0.0f;
    float Plus_y = 0.0f;

    if( m_iEarthQuake )
    {
        m_iEarthQuake--;

        Plus_x = cosf((float)m_iEarthQuake/3)*(float)(m_iEarthQuake+10) * 0.02f;
        Plus_y = cosf((float)m_iEarthQuake/3)*(float)(m_iEarthQuake+10) * 0.02f;
    }

    if( m_iEarthQuakeY )
    {
        m_iEarthQuakeY--;
        Plus_y = cosf((float)m_iEarthQuake/3)*(float)(m_iEarthQuake+10) * 0.02f;
    }

    if( m_iShakeCameraFrame )
    {
        m_iShakeCameraFrame--;

        Plus_x += (float)( ( rand() % 2 ) ? 1 : -1 ) * 0.2f;
        Plus_y += (float)( ( rand() % 2 ) ? 1 : -1 ) * 0.3f;
    }

    if( m_fCameraVib > 0.0f )
    {
        static int iDir = 1;
        if( m_iDirection == 0 ) // 방향이 0이면 좌우 진동
            //          -1.0 ~ 1.0의 범위
            Plus_x = m_fCameraVib * iDir;
        else // 상하 진동
            Plus_y = m_fCameraVib * iDir;

        if( m_fCameraVib - m_fDecrease >= 0.0f )
            m_fCameraVib = m_fCameraVib - m_fDecrease;
        else
            m_fCameraVib = 0.0f;

        iDir = ( iDir == 1 ) ? -1 : 1;
    }

    // Base Camera Rect
    m_rtCamera.SetRect( fX - 1.0f, fY + 0.75f, fX + 1.0f, fY - 0.75f );
    // Zoom
    m_rtCamera.Zoom( m_fZoom );

    //  기본줌일때 정보가 필요 해서 이걸 쓴다.
    float fZoomValue = g_pkGameOverlayUI->GetZoomValue();
    if( 0.0f >= fZoomValue )
        fZoomValue = 10.0f;
    
    m_rtCameraBasic.SetRect( fX - 1.0f, fY + 0.75f, fX + 1.0f, fY - 0.75f );
    m_rtCameraBasic.Zoom( fZoomValue/10.0f );

    // 모션의 y값이 변할 때 카메라가 따라가도록 반영
    float fydelta = 0.0f;
    if( g_MyD3D->MyPlayer[m_iTargetPlayer]->m_kUserInfo.bLive )
        fydelta = g_MyD3D->MyPlayer[m_iTargetPlayer]->y_Ex;

    // 중심
    float fCenterX = ( m_rtCamera.left + m_rtCamera.right ) / 2.0f;
    float fCenterY = ( m_rtCamera.top + m_rtCamera.bottom ) / 2.0f + fydelta;

#if defined( USE_AGIT_SYSTEM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT ) {
        fCenterY = ( m_rtCamera.top + m_rtCamera.bottom ) / 2.0f;
    }
#endif

    // 도타맵은 한 스테이지에 존으로 구분되어 있기때문에 벽을 카메라가 넘어가서는 안된다.
    // 카메라가 벽을 넘어서는지 검사하자!!( 오른쪽벽만 처리  )
    float fStageWidth = SiKGCFantasticMap()->GetPixelWidth();
    if( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
    {
        float fAfterX = fCenterX + (m_fLookTermX + Plus_x);
        // 스테이지의 원래 폭보다 좁을 수 있다. 스테이지의 크기를 존이 끝나는 지점으로 보정.
        SiKGCFantasticMap()->CheckPierceBlockXPos(fCenterX, fStageWidth, fCenterY);
        if( SiKGCFantasticMap()->CheckPierceBlockXPos(fCenterX, fAfterX, fCenterY) )
        {
            // 이동포탈 등으로 갑자기 오른쪽벽을 통과했을 땐 아래와 같이 보정해주자!
            float fStaticX = fCenterX - m_fLookTermX + Plus_x;
            if( m_fLookX > fStaticX )
                m_fLookX = fStaticX;
        }

        // Y축도 보정해주자( 현재 Y와 이동해야될 곳의 Y차이가 많이 날때)
        float fStaticY = fCenterY - m_fLookTermY + Plus_y;
        if( (fStaticY-3.0f) > m_fLookY || m_fLookY > (fStaticY+3.0f) )
            m_fLookY = fStaticY;
    }

    /************************************************************************/
    /* 정상적인 작동(우리가 생각하는 카메라 워크)을 위해서 줌을 적용하는 방식이 X, Y 서로 다르다.
    tempLookX, tempLookY에 m_fZoom을 바로 적용하게 되면 옛날 처럼 위로 올라갔을때 울렁거리는 현상이 생긴다
    따라서, UpdateCameraMatrix() 함수에서 Camera Matrix에 m_fZoom을 적용시킨다.
    /************************************************************************/

    if( !m_bFollow && g_MyD3D->MyPlayer[m_iTargetPlayer]->IsLocalPlayer() )
        goto BoundCheck;

    // 케릭터 몸의 중심을 보기 위해서 ( m_fLookTermX, m_fLookTermY ) 만큼 뺀다.
    float tempLookX;
    float tempLookY;

    if( POS_MODE_DIRECT <= iPosMode )
    {
        tempLookX = fCenterX + Plus_x;
        tempLookY = fCenterY + Plus_y;
    }
    else
    {
        tempLookX = fCenterX - m_fLookTermX + Plus_x;
        tempLookY = fCenterY - m_fLookTermY + Plus_y;
    }

    if( (m_iZoomCount && g_MyD3D->MyStg->Color_Stop_Count) || (POS_MODE_DIRECT <= iPosMode) )
    {
        // 줌일때는 바로 보게
        m_fLookX = tempLookX;
        m_fLookY = tempLookY;
    }
    else
    {
        // m_fFollowRate 만큼 따라가도록
        m_fLookX += ( tempLookX - m_fLookX ) * m_fFollowRateX;

        // 너무 높게 보고 있는 경우
        if( ASMfabs( tempLookY - m_fLookY ) > (0.75f * 0.5f) )
            m_fLookY += ( tempLookY - m_fLookY ) * 2.0f * m_fFollowRateY;
        else m_fLookY += ( tempLookY - m_fLookY ) * m_fFollowRateY;
    }

BoundCheck:
    // 아래의 바운드 체크 코드는 선지자의 코드임 ( 의미를 도통 알 순 없지만 작동은 잘됨 ^____^ )

    // 왼쪽
    if( m_fLookX * m_fZoom < m_fBoundTermX - m_fZoom )
        m_fLookX = ( m_fBoundTermX - m_fZoom ) / m_fZoom;

    // 오른쪽
    const float Right_Limit = 1.5f;

    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        if( (m_fLookX * m_fZoom) > 
            (m_fZoom * ( fStageWidth - 1.0f ) - m_fBoundTermX) )
        {
            m_fLookX = ( m_fZoom * ( fStageWidth - 1.0f ) - m_fBoundTermX ) / m_fZoom;
            if( m_fLookX < 0.0f )
                m_fLookX = 0.0f;
    }
    }
    else
    {
        if( (m_fLookX * m_fZoom) > 
            (m_fZoom * (2.0f + ( (float)g_MyD3D->MyStg->Num_Width - 2.0f ) * Right_Limit ) - m_fBoundTermX) )
        {
            m_fLookX = ( m_fZoom * (2.0f + ( (float)g_MyD3D->MyStg->Num_Width - 2.0f ) * Right_Limit ) - m_fBoundTermX ) / m_fZoom;
    }
    }

    // 아래쪽 바운드 체크
    if( m_fLookY * m_fZoom < -(m_fZoom * 0.5f - m_fBoundTermY) )
        m_fLookY = -(m_fZoom * 0.5f - m_fBoundTermY) / m_fZoom;

    float fHalfWidth = ( m_rtCamera.right - m_rtCamera.left ) * 0.5f;
    float fHalfHeight = ( m_rtCamera.top - m_rtCamera.bottom ) * 0.5f;

    float fHalfWidthBasic = ( m_rtCameraBasic.right - m_rtCameraBasic.left ) * 0.5f;
    float fHalfHeightBasic = ( m_rtCameraBasic.top - m_rtCameraBasic.bottom ) * 0.5f;

    m_rtCamera.SetRect( m_fLookX - fHalfWidth, m_fLookY + fHalfHeight, m_fLookX + fHalfWidth, m_fLookY - fHalfHeight );
    m_rtCameraBasic.SetRect( m_fLookX - fHalfWidthBasic, m_fLookY + fHalfHeightBasic, m_fLookX + fHalfWidthBasic, m_fLookY - fHalfHeightBasic );

    m_vCenterPos.x = ( m_rtCamera.left + m_rtCamera.right ) / 2.0f;
    m_vCenterPos.y = ( m_rtCamera.top + m_rtCamera.bottom ) / 2.0f;
    m_vCenterPos.z = 0.5f;
}

void KGCCameraOrtho::FrameMove()
{
    PROFILE_SET("KGCCameraOrtho::FrameMove");

    if( m_fWideMode == 0.0f )
    {
        if( m_iZoomCount > 0)
        {
            if( m_fTargetZoom > 1.0f)
                m_fZoom += ( m_fTargetZoom - m_fZoom ) * 0.05f;
            else
                m_fZoom += ( m_fTargetZoom - m_fZoom ) * 0.03f;
            m_iZoomCount--;
        }
        else if( !g_MyD3D->MyStg->Color_Stop_Count )
        {
            if( m_fZoom > 1.01f )
                m_fZoom -= 0.01f;
            else if( m_fZoom < 0.99f )
                m_fZoom += 0.008f;
        }

        if ( m_bZoomViewMode )
        {
            m_fZoom += ( 1.5f - m_fZoom ) * 0.05f;
        }
    }
    else
    {
        float fDefaultZoom = m_fWideMode / 10.0f;

        if( m_iZoomCount > 0)
        {
            if( m_fTargetZoom > fDefaultZoom )
                m_fZoom += ( m_fTargetZoom - m_fZoom ) * 0.08f;
            else
                m_fZoom += ( m_fTargetZoom - m_fZoom ) * 0.06f;
            m_iZoomCount--;
        }
        else if( !g_MyD3D->MyStg->Color_Stop_Count )
        {
            if( m_fZoom > fDefaultZoom )
            {
                m_fZoom -= 0.01f;

                if(m_fZoom < fDefaultZoom)
                {
                    m_fZoom = fDefaultZoom;
                }
            }
            else if( m_fZoom < fDefaultZoom )
            {
                m_fZoom += 0.01f;

                if(m_fZoom > fDefaultZoom)
                {
                    m_fZoom = fDefaultZoom;
                }
            }
        }

        if ( m_bZoomViewMode )
        {
            m_fZoom += ( 1.5f - m_fZoom ) * 0.2f;
        }
        /*
        else
        m_fZoom += ( fDefaultZoom - m_fZoom ) * 0.2f;*/
    }

    //여기서 카메라를 처리합니다.
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( pPlayer != NULL )
    {
        if( g_MyD3D->MyReplay->m_iCurrState != RP_VIEW && ( pPlayer->m_cLife < 1 || g_MyD3D->IsLocalPlayerObserver() ))
            FrameMove_DeathPlayer();
    }

    //화면을 따라 갑니다
    FrameMove_OnGame();
}

void KGCCameraOrtho::ViewModeFrameMove()
{
    if( g_MyD3D->MyHead->IsCheatMode || KGCSquareOverlayUI::ECF_MAINWND!=g_pkSquareOverlayUI->GetCursorFocus())
        return;

    // 카메라 조정
    //if(diks[DIK_Z] && !Old_diks[DIK_Z])
    //{
    //    SetNextTargetPlayer();
    //}

    // 카메라 확대 축소
    if(diks[DIK_X] && !Old_diks[DIK_X])
    {
        m_iHostViewZoomMode++;
        if(m_iHostViewZoomMode>2)
            m_iHostViewZoomMode=0;

        if( m_iHostViewZoomMode == 0)
            ZoomMessage( false );
        else
            ZoomMessage( true );
    }

    // 특정키로 카메라 줌 조절
    if ( diks[DIK_I] && !Old_diks[DIK_I] )
    {
        m_iHostViewZoomMode = 0;
    }
    if ( diks[DIK_O] && !Old_diks[DIK_O] )
    {
        m_iHostViewZoomMode = 1;
    }
    if ( diks[DIK_P] && !Old_diks[DIK_P] )
    {
        m_iHostViewZoomMode = 2;
    }

    if( g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS
        && SiKGCRoomManager()->IsMonsterGameMode() == false )
        m_iHostViewZoomMode = 1;

    for(int key = DIK_1; key <= DIK_6; key++)
    {
        if(diks[key])
        {
            SetTargetPlayer(g_pkHUD->GetPlayerIndex(key - DIK_1));
        }
    }

    switch(m_iHostViewZoomMode)
    {
    case 0:// 전체 화면
        m_fZoom += ( 0.70f - m_fZoom ) * 0.2f;
        break;
    case 1:// 보통 화면
        m_fZoom += ( 1.0f - m_fZoom ) * 0.2f;
        break;
    case 2:// 확대
        m_fZoom +=(1.5f - m_fZoom ) * 0.2f;
        break;
    }
}

void KGCCameraOrtho::InitCamera()
{
    m_bZoomViewMode = false;
    m_bMonsterCamera = false;

    m_fZoom = m_fTargetZoom = 1.0f;
    g_MyD3D->MyStg->Color_Stop_Count = 0;

    // 판타스틱 맵을 사용하는가??
    // Stage.cpp 에.. LoadScript() 에서.. 카메라 초기화도 시켜주는데..
    // 판타스틱맵을 사용하면 그쪽 루틴으로 아예 들어가질 않는다.. 그래서 따로 처리를 해 주도록 함..
    // 아래에 초기화 시켜주는 값들은 MapInfo.lua 에 GlobalCamera 와 동일하다
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        // 몇몇 변수 초기화 시켜준다
        m_iHostViewZoomMode = false;

        float fZoomValue = g_pkGameOverlayUI->GetZoomValue();
        if( 0.0f < fZoomValue )
            g_kCamera.m_fWideMode = fZoomValue;
        else
            m_fWideMode = 10.0f;

        m_fFollowRateX = 0.03f;
        m_fFollowRateY = 0.03f;
        m_fBoundTermX = 1.0f;
        m_fBoundTermY = 0.45f;
        m_fLookTermX = 1.0f;
        m_fLookTermY = 0.6f;
        m_bQuestStartedOfResetCamera = true;
        SetTargetPlayer( g_MyD3D->Get_MyPlayer() );
    }

    m_fTargetOffsetX = 0.0f;
    m_fTargetOffsetY = 0.0f;
    m_fStaticX = 0.0f;
    m_fStaticY = 0.0f;
}

void KGCCameraOrtho::SetNextTargetPlayer(int iWhosNext)
{
    // Jaeho.Ready
    //if( KQuestGameManager::QPS_STORY_DIALOGUE == g_MyD3D->m_pkQuestGameMgr->GetQuestProgressState() )
    //    return;

    if( iWhosNext != -1)
    {
        m_iTargetPlayer = iWhosNext;
    }

    for(int i = m_iTargetPlayer + 1 ; i < MAX_PLAYER_NUM ; i++)
    {
        if(g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife)
        {
            if ( g_pkChatManager != NULL )
            {
                g_pkChatManager->AddSystemMsg( i, g_pkStrLoader->GetString( STR_ID_GAMEMSG_OBSERVE ) );
            }
            m_iTargetPlayer = i;
            return;
        }
    }

    for( int i = 0 ; i < m_iTargetPlayer; i++)
    {
        if(g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife)
        {
            if ( g_pkChatManager != NULL )
            {
                g_pkChatManager->AddSystemMsg( i, g_pkStrLoader->GetString( STR_ID_GAMEMSG_OBSERVE ) );
            }
            m_iTargetPlayer = i;
            return;
        }
    }
}

void KGCCameraOrtho::SetTargetPlayer(int iTarget)
{
    if( iTarget < 0 || iTarget >= MAX_PLAYER_NUM )
        return;
    if(g_MyD3D->MyPlayer[iTarget]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[iTarget]->m_cLife)
    {
        m_iTargetPlayer = iTarget;
    }
    else
    {
        SetNextTargetPlayer(iTarget);
    }
}

void KGCCameraOrtho::SetTargetMonster(int iTarget)
{
    if( iTarget == -1)
    {
        m_bMonsterCamera = false;
        return;
    }
    else
    {
		if( g_kMonsterManager.IsMonsterIndex(iTarget) == false )
            return;
        /*
        if( !g_kMonsterManager.m_mapMonster[iTarget]->m_bIsLoadMonster )
        {
        m_bMonsterCamera = false;
        return;
        }
        */
        m_bMonsterCamera = true;
        m_iTargetMonster = iTarget;
        return;
    }
}

void KGCCameraOrtho::InitCameraPosition()
{
    m_fLookX = g_kLocalPlayerLatency->kRenderData[(LATENCY+SHOWLATENCY)-1].vPos.x - 1.0f;
    m_fLookY = g_kLocalPlayerLatency->kRenderData[(LATENCY+SHOWLATENCY)-1].vPos.y - 0.65f;
    UpdateCameraMatrix();

    SetCameraPosition( m_fLookX, m_fLookY );
}

void KGCCameraOrtho::SetDirectCameraPosition(float iPosX, float iPosY, int iPosMode )
{
    m_fLookX = iPosX - 1.0f;
    m_fLookY = iPosY - 0.65f;
    UpdateCameraMatrix();

    SetCameraPosition( m_fLookX, m_fLookY, iPosMode );
}


void KGCCameraOrtho::ZoomMessage( bool bZoomMode )
{
    if ( g_pkChatManager != NULL )
    {
        {
            if( bZoomMode )
                g_pkChatManager->AddSystemMsg( m_iTargetPlayer, g_pkStrLoader->GetString( STR_ID_GAMEMSG_ZOOM_IN ) );
            else
                g_pkChatManager->AddSystemMsg( m_iTargetPlayer, g_pkStrLoader->GetString( STR_ID_GAMEMSG_ZOOM_OUT ) );
        }
    }
};

void KGCCameraOrtho::FrameMove_DeathPlayer()
{
    if( g_MyD3D->MyHead->IsCheatMode || KGCSquareOverlayUI::ECF_MAINWND!=g_pkSquareOverlayUI->GetCursorFocus())
        return;

    if( IsDetermineWin == true )
        return;

    if( (diks[DIK_Z] && !Old_diks[DIK_Z]) || g_MyD3D->MyPlayer[m_iTargetPlayer]->m_cLife < 1 )
        SetNextTargetPlayer();

    if( (diks[DIK_X] && !Old_diks[DIK_X]) )
    {
        m_bZoomViewMode = !m_bZoomViewMode;
        ZoomMessage( m_bZoomViewMode );
    }
}

void KGCCameraOrtho::FrameMove_OnGame()
{
    float fCameraPosX = m_fStaticX;
    float fCameraPosY = m_fStaticY;

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME )
    {
        if( 0.0f == fCameraPosX )
            fCameraPosX = 0.5f;
        if( 0.0f == fCameraPosY )
            fCameraPosY = 0.5f;
    }
    else
    {
        if( m_bMonsterCamera )
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iTargetMonster);
            if( NULL == pMonster )
            {
                InitCamera();
                return;
            }
            else
            {
                if( 0.0f == fCameraPosX )
                    fCameraPosX = pMonster->m_afX[(LATENCY+SHOWLATENCY)-1] + m_fTargetOffsetX;
                if( 0.0f == fCameraPosY )
                    fCameraPosY = pMonster->m_afY[(LATENCY+SHOWLATENCY)-1] + m_fTargetOffsetY;
            }
        }
        else
        {
            if( g_MyD3D->MyPlayer[m_iTargetPlayer]->IsLocalPlayer() ) 
            {
                if( 0.0f == fCameraPosX )
                    fCameraPosX = g_kLocalPlayerLatency->kRenderData[(LATENCY+SHOWLATENCY)-1].vPos.x + m_fTargetOffsetX;
                if( 0.0f == fCameraPosY )
                    fCameraPosY = g_kLocalPlayerLatency->kRenderData[(LATENCY+SHOWLATENCY)-1].vPos.y + m_fTargetOffsetY;
            }
            else 
            {
                if( 0.0f == fCameraPosX )
                    fCameraPosX = g_MyD3D->MyPlayer[m_iTargetPlayer]->vPos.x + m_fTargetOffsetX;
                if( 0.0f == fCameraPosY )
                    fCameraPosY = g_MyD3D->MyPlayer[m_iTargetPlayer]->vPos.y + m_fTargetOffsetY;
            }
        }
    }

    SetCameraPosition( fCameraPosX, fCameraPosY );
}

void KGCCameraOrtho::SetShakeCameraFrame( int iShakeCameraFrame )
{
    m_iShakeCameraFrame = iShakeCameraFrame;
}

// iDirection 0이면 좌우 진동, 1이면 상하진동
// fCameraVib 카메라가 흔들리는 정도
// fDecrease 흔들리는 정도가 줄어드는 정도
void KGCCameraOrtho::CameraVib( int iDirction, float fCameraVib, float fDecrease )
{
    m_fCameraVib = fCameraVib;
    m_fDecrease = fDecrease;
    m_iDirection = iDirction;
}

void KGCCameraOrtho::FrameMoveInSquare()
{
    if( m_bObserve )
    {
        SquareObserveFrameMove();
    }
    SquareFrameMove();
}

void KGCCameraOrtho::SquareObserveFrameMove()
{
    if( g_MyD3D->MyHead->IsCheatMode || KGCSquareOverlayUI::ECF_MAINWND!=g_pkSquareOverlayUI->GetCursorFocus())
        return;

    if( diks[DIK_Z] && !Old_diks[DIK_Z] )
        SetNextSquarePeople();
}

void KGCCameraOrtho::SquareFrameMove()
{
    GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( m_dwSquareUID );
    if( pPeople == NULL )
    {
        pPeople = SiGCSquare()->GetSquarePeople( g_kGlobalValue.m_kUserInfo.dwUID );
        // [1/8/2008] breadceo. 이거는 말이 안되
        ASSERT( pPeople != NULL );
    }
    D3DXVECTOR3 vPos = pPeople->GetPos();
    SetSquareCamera( vPos.x, vPos.y );
}

void KGCCameraOrtho::SetNextSquarePeople( bool bLocal/* = false*/ )
{
    if( bLocal == true )
    {
        m_dwSquareUID = g_kGlobalValue.m_kUserInfo.dwUID;
    }
    else
    {
        m_dwSquareUID = SiGCSquare()->GetNextSquarePeopleUID( m_dwSquareUID );
    }

#if defined( ENABLE_SQUARE_LOGGING )
    START_LOG( clog, m_dwSquareUID << L" 관찰합니다" );
#endif
}

void KGCCameraOrtho::SetSquareCamera( float fX, float fY )
{
    // Base Camera Rect
    m_rtCamera.SetRect( fX - 1.0f, fY + 0.75f, fX + 1.0f, fY - 0.75f );
    m_rtCamera.Zoom( m_fZoom );

    // 중심
    float fCenterX = ( m_rtCamera.left + m_rtCamera.right ) / 2.0f;
    float fCenterY = ( m_rtCamera.top + m_rtCamera.bottom ) / 2.0f;

    /************************************************************************/
    /* 정상적인 작동(우리가 생각하는 카메라 워크)을 위해서 줌을 적용하는 방식이 X, Y 서로 다르다.
    tempLookX, tempLookY에 m_fZoom을 바로 적용하게 되면 옛날 처럼 위로 올라갔을때 울렁거리는 현상이 생긴다
    따라서, UpdateCameraMatrix() 함수에서 Camera Matrix에 m_fZoom을 적용시킨다.
    /************************************************************************/

    float ydelta = 0.375f * ( 1.0f - ( 1.0f / m_fZoom ) );

    // 케릭터 몸의 중심을 보기 위해서 ( m_fLookTermX, m_fLookTermY ) 만큼 뺀다.
    float tempLookX = fCenterX - m_fLookTermX;
    float tempLookY = fCenterY - ( m_fLookTermY - ydelta );

    // m_fFollowRate 만큼 따라가도록
    m_fLookX += ( tempLookX - m_fLookX ) * m_fFollowRateX;

    // 너무 높게 보고 있는 경우
    if( ASMfabs( tempLookY - m_fLookY ) > (0.75f * 0.5f) )
        m_fLookY += ( tempLookY - m_fLookY ) * 2.0f * m_fFollowRateY;
    else m_fLookY += ( tempLookY - m_fLookY ) * m_fFollowRateY;

    // 왼쪽
    if( m_fLookX * m_fZoom < m_fBoundTermX - m_fZoom )
        m_fLookX = ( m_fBoundTermX - m_fZoom ) / m_fZoom;

    // 오른쪽
    const float Right_Limit = 1.5f;
    if( m_fLookX * m_fZoom > m_fZoom * (2.0f + SiKGCFantasticMap()->GetPixelWidth() - 3.0f ) - m_fBoundTermX)
        m_fLookX = ( m_fZoom * (2.0f + SiKGCFantasticMap()->GetPixelWidth() - 3.0f ) - m_fBoundTermX ) / m_fZoom;

    // 아래쪽 바운드 체크
    if( m_fLookY * m_fZoom < -(m_fZoom * 0.5f - m_fBoundTermY) )
        m_fLookY = -(m_fZoom * 0.5f - m_fBoundTermY) / m_fZoom;

    float fHalfWidth = ( m_rtCamera.right - m_rtCamera.left ) * 0.5f;
    float fHalfHeight = ( m_rtCamera.top - m_rtCamera.bottom ) * 0.5f;
    m_rtCamera.SetRect( m_fLookX - fHalfWidth, m_fLookY + fHalfHeight, m_fLookX + fHalfWidth, m_fLookY - fHalfHeight );
}

void KGCCameraOrtho::InitSquareCamera()
{
    SetNextSquarePeople( true );

    GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( m_dwSquareUID );

    if( pPeople ){
        m_fZoom = 1.0f;
        D3DXVECTOR3 vPos = pPeople->GetPos();
        m_fLookX = vPos.x;
        m_fLookY = vPos.y;
        UpdateCameraMatrixForSquare();
        SetSquareCamera( vPos.x, vPos.y );
    }
    else{
        pPeople = SiGCSquare()->GetSquarePeople( g_kGlobalValue.m_kUserInfo.dwUID );
        // [1/8/2008] breadceo. 이거는 말이 안되
        ASSERT( pPeople != NULL );
    }
}

void KGCCameraOrtho::UpdateCameraMatrixForSquare()
{
	/*
    D3DXMatrixIdentity( &m_matCamera );
    D3DXMatrixTranslation( &m_matCamera, -m_fLookX, -m_fLookY - 0.325f, 0.0f );*/

	// 카메라 매트릭스에서 주석 처리한 부분은 결코 변하지 않는다 
	// 따라서 매번 초기화 해 줄 필요가 없다 
    m_matCamera._11=m_fZoom;	//m_matCamera._12=0;			m_matCamera._13=0;			m_matCamera._14=0; 
    /*m_matCamera._21=0;*/			m_matCamera._22=m_fZoom;	//m_matCamera._23=0;			m_matCamera._24=0;
    /*m_matCamera._31=0;			m_matCamera._32=0;*/			m_matCamera._33=1.0f;		//m_matCamera._34=0;

    m_matCamera._41=-m_fLookX * m_fZoom;	
    //m_matCamera._43=0.0f;	
    //m_matCamera._44=1.0f;	
    m_matCamera._42=-m_fLookY * m_fZoom - 0.325f;
}

bool KGCCameraOrtho::CheckDashZoom()
{
#if !defined (__PATH__)
    if (g_kStatsManager.GetEagleSiteCheat() == true )
        return false;
#endif
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
    if( NULL == pCameraZoom || false == pCameraZoom->m_bNotDashZoom )
        return true;
    return false;
}

bool KGCCameraOrtho::CheckJumpZoom()
{
#if !defined (__PATH__)
    if (g_kStatsManager.GetEagleSiteCheat() == true )
        return false;
#endif
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
    if( NULL == pCameraZoom || false == pCameraZoom->m_bNotJumpZoom )
        return true;
    return false;
}

bool KGCCameraOrtho::CheckSpecialZoom()
{
#if !defined (__PATH__)
    if (g_kStatsManager.GetEagleSiteCheat() == true )
        return false;
#endif
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
    if( NULL == pCameraZoom || false == pCameraZoom->m_bNotSpecialZoom )
        return true;
    return false;
}