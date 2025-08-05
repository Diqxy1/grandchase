#include "stdafx.h"

#include ".\gcsquarepeople.h"
#include "Controls.h"
#include "Spark.h"
#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"
#include "GCSquarePeopleActivitySensor.h"
#include "KGC3DObject.h"
#include "GCCameraOrtho.h"
#include "../MapSystem/KGCLayerPart.h"
#include "GCUI/KGCSquareOverlayUI.h"
#include "PetAI.h"
#include "GCCoupleSystem.h"
#include "../KGCValentineDayManager.h"
#include "GCO2Manager.h"
#include "GCIconImageManager.h"
#include "../KGCSocialMotionManager.h"

#define NOTIFY_MYPOS_TO_SERVER_TIME (55*3)

#define FIND_AND_SET( q, p )         {                                              \
    mit = GCSquarePeopleStateFunctions::p.find( Extra_Char_Num );  \
    ASSERT( mit != GCSquarePeopleStateFunctions::p.end() );           \
    if( mit != GCSquarePeopleStateFunctions::p.end() )                      \
    {                                                                                              \
    q = mit->second;                                                                       \
    }                                                                                               \
}                                                                                                  \

//------------------------------------------------------------------------
//	20091025 광장 모션 세팅 매크로 : 이동석
#define FIND_AND_SET_SQUAREMOTION( q, p, r )       {				\
itr	=	GCSquarePeopleStateFunctions::r.find( iSetItem );	\
if( GCSquarePeopleStateFunctions::r.end() != itr ){			\
	    if( itr->second == -1 ){								\
	        FIND_AND_SET( q, p );								\
	    }else{													\
        	q = itr->second;									\
	    }														\
	}                                                       \
    else{ FIND_AND_SET( q, p );               \
    }                                                       \
}								                              \

//========================================================================


#ifdef _DEBUG
//#define LOAD_CHECK
#endif

GCSquarePeople::GCSquarePeople( DWORD dwUID )
: m_eState( GCSquarePeople::INVALID_PEOPLE )
, m_eOldState( GCSquarePeople::INVALID_PEOPLE )
, m_bIsContact( true )
, m_bOldIsContact( true )
, m_fXSpeed( 0.0f )
, m_fYSpeed( 0.0f )
, m_cNoCheckContact( 0 )
, m_bFrameLock( false )
, m_dwUID( dwUID )
, m_pObject( NULL )
, m_nNotifyPositionToServer( 0 )
, m_pEmoticon( NULL )
, m_bBufferEmpty(false)
, m_pTexture(NULL)
, m_pGCClubTexture( NULL )
, m_pGuildMarkTexture( NULL )
, m_Elemental( 0 )
, m_StrongLevel( 0 )
, m_bIsEquipCoordiSetItem( false )
, m_bLoadDownHelmetNFL( false )
, m_bEquipBigHead( false )
, m_bShadowHead( false )
, m_pNonChangeFaceLift( NULL )
, m_bEquipFaceOffHead(false)
{
    m_eType = SOT_PEOPLE;
    m_pSensor = new GCSquarePeopleActivitySensor();
    ASSERT( m_pSensor != NULL );

    SetBodyRect( D3DXVECTOR2( 0.175f, 0.3f ) );

	if( FAILED(D3DXCreateTexture( g_pd3dDevice, 128,64,1, 0, D3DFMT_DXT1,D3DPOOL_MANAGED, &m_pTexture ) ))
	{
		m_pTexture = NULL;
		assert(m_pTexture != NULL);
	}
}

GCSquarePeople::GCSquarePeople( void )
{
    ASSERT( !"Do Not Use This Constructor" );
}

GCSquarePeople::~GCSquarePeople(void)
{
	SAFE_RELEASE(m_pTexture);
    SAFE_DELETE( m_pSensor );
    if( m_pObject != NULL )
    {
        m_pObject->Release();
        SAFE_DELETE( m_pObject );
    }

    for( int i=0 ; i<(int)m_vecItemEff.size() ; ++i )
        g_ParticleManager->DeleteSequence( m_vecItemEff[i].pSeq );
    for( int i=0 ; i<(int)m_vecDamageEff.size() ; ++i )
        g_ParticleManager->DeleteSequence( m_vecDamageEff[i].pSeq );
    for( int i=0 ; i<(int)m_vecSkillEff.size() ; ++i )
        g_ParticleManager->DeleteSequence( m_vecSkillEff[i].pSeq );

    m_vecItemEff.clear();
    m_vecDamageEff.clear();
    m_vecSkillEff.clear();

    for( int i=0 ; i<(int)m_ItemAnim.size() ; ++i )
        g_AnimManager.DeleteInst( m_ItemAnim[i].animInst );

    for( int i=0 ; i<(int)m_SkillAnim.size() ; ++i )
        g_AnimManager.DeleteSkillInst( m_SkillAnim[i] );

    m_ItemAnim.clear();
    m_DamageAnim.clear();
    m_SkillAnim.clear();

    g_pkPetAI->ClearAIObject( GetUID() );

    m_mapOverlappedPair.clear();
    m_vecPetAiID.clear();

	SiKGCCoupleSystem()->_DeleteNearEffect( GetUID() );
}

void GCSquarePeople::FrameMove()
{
    if( false == GetInit() )
        return;

    m_bFrameLock = false;
    m_bBufferEmpty = false;

    ReadyToRender();

    if( true == IsLocalPlayer() )
    {
        FrameMoveLocalPlayer();
    }
    else
    {
        FrameMoveRemotePlayer();
    }

    EgoItemFrameMove();
    //DoFrameInfoProc();

    m_OldBasicData = m_BasicData;
    m_eOldState = m_eState;

    SquareBoundCheck();

    if( m_bFrameLock == false && m_bBufferEmpty == false && m_BasicData.cFrame >= (int)m_pObject->GetMotionPtr()->GetNum_Frame() )
    {
        m_BasicData.cFrame=0;
    }

    float fMotionY = GetMotionPtr()->GetFrameData(m_BasicData.cFrame).Pos_y * CHAR_SCALE_RATIO;
    ASSERT( NULL != m_pObject );
    m_pObject->SetPos( D3DXVECTOR2( m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.48f + fMotionY ) );
    m_pObject->IsRight( m_BasicData.bIsRight );
    m_pObject->SetFrame( m_BasicData.cFrame );
    m_pObject->FrameMove();

    if ( m_pEmoticon )
    {
        m_pEmoticon->SetPos( &m_pObject->GetPos() );
        //m_pEmoticon->FrameMove();

        if( m_pEmoticon->IsEnd() )
            m_pEmoticon = NULL;

    }

    UpdateBonePosition();

	SiKGCCoupleSystem()->NearCoupleEffect( this );
	SiKGCValentineDayManager()->NearChocolateEffect( this );
}

void GCSquarePeople::Render()
{
    if( true == g_kGlobalValue.m_bRenderSquarePeopleData )
    {
        DrawDumpData();
        DrawBodyRect();
    }

    if( false == GetInit() || false == GetRender() )
        return;

    DrawSquarePeople();
}

void GCSquarePeople::EgoItemInitialize()
{
    if( NULL == m_pObject )
        return;

    m_pObjectEgo.clear();
    BOOST_FOREACH( DWORD& sItem, m_kLoadedState.m_vecModelID ) {
        std::list<KGCObjectEgo*> listEgo = SiKGCObjectEgoMgr()->GetObjectEgo( (sItem/10) );
        if( listEgo.empty() == true )
            continue;

        BOOST_FOREACH( KGCObjectEgo* pEgo, listEgo ) {
            pEgo->AddMesh( (GCObject*)m_pObject->GetGCObject(), (sItem/10), 0, &m_pObjectEgo );
        }
    }
}

void GCSquarePeople::EgoItemFrameMove()
{
    if( m_pObjectEgo.empty() ) {
        return;
    }

    m_pObject->GetGCObject()->ChildFrameMove();

    // 광장에서 아이템모션을 변경하거나 파티클을 붙힐려면 아래 내용이 필요합니다.
    //int iIndex = 0;
    //const KGCObjectEgoState* state = NULL;
    //for( KGCObjectAndMotion* pChild = NULL; pChild = m_pObject->GetGCObject()->GetChildObject( iIndex ); iIndex++ )
    //{
    //    std::vector< KGCEgoItemInfo >::iterator vit = m_pObjectEgo.begin();
    //    for(; vit != m_pObjectEgo.end(); ++vit)
    //    {
    //        if( vit->m_dwUniqueID == pChild->m_dwEgoUniqueID )
    //            break;
    //    }
    //    if ( vit == m_pObjectEgo.end() )
    //        continue;
    //    
    //    KGCObjectEgo *pEgo = vit->m_pEgoObj;
    //    if( pEgo == NULL ) {
    //        ASSERT( !"List 안에서 Null이 됨 AddMesh를 디버깅해보셈~");
    //        continue;
    //    }

    //    DWORD test = pChild->m_dwEgoUniqueID;
    //    std::string strCurState = SiKGCObjectEgoMgr()->GetStateInfo( pChild->m_dwEgoUniqueID );
    //    if( strCurState.empty() )
    //        continue;

    //    state = NULL;
    //    int iConditionIdx = -1;
    //    if( IsLocalPlayer() )
    //    {
    //        state = pEgo->GetEgoState(strCurState, g_MyD3D->GetMyPlayer(), iConditionIdx);
    //    }
    //    else if( vit->m_bCheckCondition )
    //    {
    //        vit->m_bCheckCondition = false;
    //        state = pEgo->GetEgoStateNotCheckCondition( vit->m_iConditionID );
    //    }

    //    if( state == NULL ) {
    //        if( pChild->IsEmpty() ) {
    //            state = pEgo->GetEgoStateFromID( strCurState );
    //            if( state == NULL )
    //                continue;

    //            state = pEgo->GetEgoStateFromID( state->m_strAfterState );
    //            if( state == NULL )
    //                continue;
    //        }
    //        else
    //            continue;
    //    }

    //    if( IsLocalPlayer() )
    //    {
    //        KGC_PID_BROAD_EGOITEM_STATE kPacket;
    //        kPacket.dwUID = m_dwUID;
    //        kPacket.cConditionIdx = static_cast<char>( iConditionIdx );
    //        kPacket.dwModelUnique = pChild->m_dwEgoUniqueID;

    //        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
    //        KGCPC_MEMO( "KGC_PID_BROAD_EGOITEM_STATE" );
    //    }

    //    SiKGCObjectEgoMgr()->SetStateInfo( pChild->m_dwEgoUniqueID, state->m_strID );
    //    pChild->ClearMotion();
    //    if( state->m_iRepeat <= 0 ) {
    //        pChild->AddMotion( state->m_strMotion, false );
    //    }
    //    else {
    //        for( int i = 0 ; i < state->m_iRepeat ; ++i ) {
    //            pChild->AddMotion( state->m_strMotion, true );
    //        }
    //    }

    //     파티클생성
    //    std::vector< KGCObjectEgoParticle >::const_iterator vecIter = state->m_kParticleInfo.begin();
    //    for(; vecIter != state->m_kParticleInfo.end(); ++vecIter)
    //    {
    //        KGCObj::AddParticleToChildMeshBone( const_cast<char*>(pChild->m_strName.c_str()), const_cast<char*>(vecIter->m_strName.c_str()), 
    //            vecIter->m_iBoneIdx, vecIter->m_fTraceTime, vecIter->vOffsetPos.x, vecIter->vOffsetPos.y, 
    //            vecIter->m_bNoDirection );
    //    }
    //}

    //m_pObject->GetGCObject()->AdjustParticlePos();
}

void GCSquarePeople::SetEgoItemOrderPlayer(char cConditionIdx, DWORD dwUnique)
{
    //if( !IsLocalPlayer() )
    //{
    //    std::vector< KGCEgoItemInfo >::iterator vecIter = m_pObjectEgo.begin();
    //    for(; vecIter != m_pObjectEgo.end(); ++vecIter)
    //    {
    //        if( vecIter->m_dwUniqueID == dwUnique )
    //        {
    //            vecIter->m_bCheckCondition = true;
    //            vecIter->m_iConditionID = cConditionIdx;
    //            return;
    //        }
    //    }
    //}
}

void GCSquarePeople::SetPeopleState( PEOPLE_STATE eState )
{
    if( eState <= GCSquarePeople::INVALID_PEOPLE || eState >= GCSquarePeople::NUM_PEOPLE_STATE )
    {
        return;
    }

    if( eState != m_eState )
    {
        SetFrame( 0 );
        m_pObject->SetFrame( 0 );
    }
	
	if( IsLocalPlayer() )
	{
        std::map<MOTIONID, GCSQUARESTATEFUNCTION>::iterator mit;
        mit = GCSquarePeopleStateFunctions::ms_mapStateFunction.find( (MOTIONID)GetMotionID(eState) );
		
		if( mit != GCSquarePeopleStateFunctions::ms_mapStateFunction.end() )
		{
			m_pCurStateFunc = mit->second;
			m_eState = eState;
			m_BasicData.usMotion = mit->first;
			m_pObject->SetMotionFromMotionID( m_BasicData.usMotion );
			return;
		}

		ASSERT( mit != GCSquarePeopleStateFunctions::ms_mapStateFunction.end() );
	}
	else
	{
		std::map<MOTIONID, GCSQUARESTATEFUNCTIONREMOTE>::iterator mit;
        mit = GCSquarePeopleStateFunctions::ms_mapStateFunctionRemote.find( (MOTIONID)GetMotionID(eState) );

		if( mit != GCSquarePeopleStateFunctions::ms_mapStateFunctionRemote.end() )
		{
			m_pCurStateFuncRemote = mit->second;
			m_eState = eState;
			m_BasicData.usMotion = mit->first;
			m_pObject->SetMotionFromMotionID( m_BasicData.usMotion );
			return;
		}

		ASSERT( mit != GCSquarePeopleStateFunctions::ms_mapStateFunctionRemote.end() );
	}
	
}

UINT GCSquarePeople::GetMotionID( PEOPLE_STATE eState )
{
    switch( eState )
    {
    case GCSquarePeople::WAIT_PEOPLE:
        return m_kSpTemplate.WAIT;
    case GCSquarePeople::WALK_PEOPLE:
        return m_kSpTemplate.WALK;
    case GCSquarePeople::JUMP_READY_PEOPLE:
        return m_kSpTemplate.JUMP_READY;
    case GCSquarePeople::JUMP_PEOPLE:
    case GCSquarePeople::DASH_JUMP_PEOPLE:
        return m_kSpTemplate.JUMP;
    case GCSquarePeople::DASH_PEOPLE:
        return m_kSpTemplate.DASH;
    case GCSquarePeople::SOCIAL_PEOPLE:
        return m_kCoordiSetTemplet.iMotionID;
    default:
        ASSERT( !"Cannot Found State" );
        break;
    }

    ASSERT( !"Cannot Found State" );
    return MID_TOTAL;
}

void GCSquarePeople::MoveXInSky()
{
    if( m_bIsContact == true )
        return;

    if( !(g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left) )
    {
        return;
    }

    if( g_MyD3D->MyCtrl->k_Right )
    {
        m_fXSpeed = 0.012f;
    }

    if( g_MyD3D->MyCtrl->k_Left )
    {
        m_fXSpeed = -0.012f;
    }
}

void GCSquarePeople::CheckContact()
{
    if( GetPeopleState() == JUMP_PEOPLE && GetYSpeed() > 0.0f )
        return;

    m_bOldIsContact = m_bIsContact;
    m_bIsContact = false;

    KGCContectInfo kInfo( m_BasicData.vPos.x, m_BasicData.vPos.y, m_fXSpeed, m_fYSpeed, m_cNoCheckContact, false );
    if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
        {
            if( kInfo.m_ySpeed <= 0.0f )
            {
                m_bIsContact = true;
                m_BasicData.vPos.y = kInfo.m_y;
                kInfo.m_ySpeed=0;
        }
    }
    m_BasicData.vPos.x = kInfo.m_x;
    m_fXSpeed = kInfo.m_xSpeed;
    m_fYSpeed = kInfo.m_ySpeed;

    if( !m_bIsContact && m_bOldIsContact )
    {
        m_cNoCheckContact = 10;
    }
    else if( m_bIsContact && !m_bOldIsContact )
    {
        g_KDSound.Play( (DWORD)0, 0 );
        SetPeopleState( GCSquarePeople::WAIT_PEOPLE );
        m_BasicData.cFrame = 0;
        m_fXSpeed = 0.0f;
        EffectDust();
    }

    if( m_cNoCheckContact > 0 )
    {
        m_cNoCheckContact--;
    }
}

void GCSquarePeople::EffectDust()
{
	if( SiKGCFantasticMap()->IsWaterStage() && SiKGCFantasticMap()->GetO2Manager() && SiKGCFantasticMap()->GetO2Manager()->IsInSquareWater( m_BasicData.vPos.y ))
	{
		g_MyD3D->MySparks->WaterSmogs( m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.2f, 0 );
	}
    else if( m_bIsEquipCoordiSetItem )
    {
        g_ParticleManager->CreateSequence( m_kCoordiSetTemplet.strSequence, m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.53f, 0.5f );
    }
	else
	{
		g_MyD3D->MySparks->Smogs( m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.2f, 0 );
	}
}

// [12/26/2007] breadceo. 기존 GC Player의 FrameMove에서 가져옴
void GCSquarePeople::FrameMoveLocalPlayer()
{
    if( m_pCurStateFunc != NULL )
    {
        GCSQUARESTATEFUNCTION pPrevFunc;
        do 
        {
            pPrevFunc = m_pCurStateFunc;
            (*m_pCurStateFunc)( this, true );
        } while( m_pCurStateFunc != pPrevFunc );
    }

    ASSERT( m_pCurStateFunc != NULL );

    MoveXInSky();
    (*m_pCurStateFunc)( this, false );
    CheckContact();
    MovingProcess();
    DoNextFrame();

    SQUARE_PEOPLE_ACTIVITY_SAMPLE kOneFrame;
    MakeOneFrameSample( kOneFrame );
    m_pSensor->AddData( kOneFrame );

    m_pSensor->FrameMove();
    m_pObject->SetRender( GetRender() );

    if( --m_nNotifyPositionToServer <= 0 )
    {
        NotifyPositionToServer();
    }
}

void GCSquarePeople::MovingProcess()
{
    if( m_bBufferEmpty == true )
    {
        return;
    }

    // [12/26/2007] breadceo. y축 이동
    m_BasicData.vPos.y += m_fYSpeed;
    m_fYSpeed -= 0.0015f;
    if( m_fYSpeed < -0.03f )
    {
        m_fYSpeed = -0.03f;
    }
    // [12/26/2007] breadceo. y축 이동 끝

    // [12/26/2007] breadceo. x축 이동
    float fBeforeX = m_BasicData.vPos.x;
    if( m_bFrameLock == false )
    {
    if(m_BasicData.bIsRight)
    {
        m_BasicData.vPos.x += m_pObject->GetMotionPtr()->GetFrameData(m_BasicData.cFrame).Plus_x*CHAR_SCALE_RATIO;
    }
    else
    {
        m_BasicData.vPos.x -= m_pObject->GetMotionPtr()->GetFrameData(m_BasicData.cFrame).Plus_x*CHAR_SCALE_RATIO;
    }
    }

    m_BasicData.vPos.x += m_fXSpeed;

    float f_Accelation = 0.0005f;
    if(m_fXSpeed > f_Accelation - 0.0001f)
    {
        m_fXSpeed -= f_Accelation;
    }
    else if(m_fXSpeed < -(f_Accelation - 0.0001f ) )
    {
        m_fXSpeed += f_Accelation;
    }
    if(m_fXSpeed < f_Accelation && m_fXSpeed > -f_Accelation)
    {
        m_fXSpeed=0.0f;
    }

    KGCFantasticMap* pMap = SiKGCFantasticMap();
    if( pMap->CheckPierceBlockXPos( fBeforeX, m_BasicData.vPos.x, m_BasicData.vPos.y ) == true )
    {
        m_fXSpeed = 0.0f;
    }
    // [12/26/2007] breadceo. x축 이동 끝
}

void GCSquarePeople::DrawSquarePeople()
{
    D3DXVECTOR3 vPos;
    D3DXMATRIX matBasic;
    Get_BasicMatrix( &matBasic, true, false );
    D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

    // offset
    vPos.x -= 0.50f;
    vPos.y += 0.80f;

    // world coordinates -> screen coordinates
    int fScreenX = int( ( vPos.x + 1.0f ) * 400.0f );
    int fScreenY = int( ( -vPos.y + 0.75f ) * 400.0f );

    WorldCoordTOScreenCoord( vPos.x,vPos.y,fScreenX,fScreenY );
	g_pkSquareOverlayUI->ReLocateBalloon( D3DXVECTOR2( (float)fScreenX, (float)fScreenY ), GetUID());

}

void GCSquarePeople::DrawDumpData()
{
    D3DXVECTOR3 vPos;
    D3DXMATRIX matBasic;
    Get_BasicMatrix( &matBasic, true, false );
    D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

    if( IsLocalPlayer() )
    {
        vPos.y += 0.25f;
    }

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        // offset
        vPos.x += 0.15f;
        vPos.y += 0.55f;

        // world coordinates -> screen coordinates
        float fScreenX = int( ( vPos.x + 1.0f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
        float fScreenY = int( ( -vPos.y + 0.75f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;

        std::wstringstream stm;
        stm.precision(6);
        stm << L"[ " << GetPeopleStateStr() << L" ]";

        D3DCOLOR Color = 0xFFFFFF00;
		
		
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_CENTER | DT_TOP );
        fScreenY += 16.0f;

        stm.str(L"");
        stm << L"x : " << m_BasicData.vPos.x << L" y : " << m_BasicData.vPos.y << L" bIsRight : " << (int)m_BasicData.bIsRight;
        stm << L"fsx : " << fScreenX << L" fsy : " << fScreenY;
        Color = 0xFFFF0000;
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_CENTER | DT_TOP );
        fScreenY += 16.0f;

        stm.str(L"");
        stm << L"x_speed : " << m_fXSpeed << L" y_speed : " << m_fYSpeed;
        Color = 0xFFFF0000;
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_CENTER | DT_TOP );
        fScreenY += 16.0f;

        stm.str(L"");
        stm << L"motion : " << m_BasicData.usMotion << L" frame : " << (int)m_BasicData.cFrame;
        Color = 0xFFFF0000;
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_CENTER | DT_TOP );
        fScreenY += 16.0f;
    }
    g_pGCDeviceManager2->PopState();
}

std::wstring GCSquarePeople::GetPeopleStateStr()
{
    switch( m_eState )
    {
#define A(p) case p: return L#p
        A( GCSquarePeople::INVALID_PEOPLE );
        A( GCSquarePeople::WAIT_PEOPLE );
        A( GCSquarePeople::WALK_PEOPLE );
        A( GCSquarePeople::JUMP_READY_PEOPLE );
        A( GCSquarePeople::JUMP_PEOPLE );
        A( GCSquarePeople::DASH_PEOPLE );
        A( GCSquarePeople::DASH_JUMP_PEOPLE );
        A( GCSquarePeople::SOCIAL_PEOPLE );
        A( GCSquarePeople::NUM_PEOPLE_STATE );
    };

    return L"NUM_PEOPLE_STATE";
}

std::wstring GCSquarePeople::GetPeopleStateStr( PEOPLE_STATE eState )
{
    switch( eState )
    {
#define A(p) case p: return L#p
        A( GCSquarePeople::INVALID_PEOPLE );
        A( GCSquarePeople::WAIT_PEOPLE );
        A( GCSquarePeople::WALK_PEOPLE );
        A( GCSquarePeople::JUMP_READY_PEOPLE );
        A( GCSquarePeople::JUMP_PEOPLE );
        A( GCSquarePeople::DASH_PEOPLE );
        A( GCSquarePeople::DASH_JUMP_PEOPLE );
        A( GCSquarePeople::SOCIAL_PEOPLE );
        A( GCSquarePeople::NUM_PEOPLE_STATE );
    };

    return L"NUM_PEOPLE_STATE";
}

void GCSquarePeople::SquareBoundCheck()
{
    if( m_BasicData.vPos.x > SiKGCFantasticMap()->GetPixelWidth() - 0.1f )
    {
        m_BasicData.vPos.x = SiKGCFantasticMap()->GetPixelWidth() - 0.1f;
        m_fXSpeed = 0.0f;
    }
    else if( m_BasicData.vPos.x < 0.05f)
    {
        m_BasicData.vPos.x = 0.05f;
        m_fXSpeed = 0.0f;
    }
    m_OldBasicData.vPos.x = m_BasicData.vPos.x;
}

void GCSquarePeople::FrameMoveRemotePlayer()
{
    SQUARE_PEOPLE_ACTIVITY_SAMPLE kSample;
    kSample.Init();
    kSample.bIsRight = m_BasicData.bIsRight;
    kSample.eState = m_eState;
    kSample.fX = m_BasicData.vPos.x;
    kSample.fY = m_BasicData.vPos.y;
    bool bRemainBuffer = m_pSensor->GetCurrentActivity( kSample );
    SetPeopleState( kSample.eState );

    MoveXInSkyRemote();
    ASSERT( m_pCurStateFuncRemote != NULL );
    (*m_pCurStateFuncRemote)( this );

	KGCPC_BEGIN("CheckContact");
    CheckContact();
	KGCPC_END();

    // 점프레디일 때에는 버퍼가 다되면 프레임락
    if( GCSquarePeople::JUMP_READY_PEOPLE == m_eState || 
        GCSquarePeople::DASH_PEOPLE == m_eState )
    {
        m_bBufferEmpty = bRemainBuffer;
    }
    MovingProcess();
    DoNextFrame();

    // [1/24/2008] breadceo. 다음 프레임 처리
    if( true == m_pSensor->DoNextFrame( kSample ) || 
        ( m_eState == GCSquarePeople::DASH_JUMP_PEOPLE && kSample.eState != GCSquarePeople::DASH_JUMP_PEOPLE ) ||
        ( m_eState == GCSquarePeople::JUMP_PEOPLE && kSample.eState != GCSquarePeople::JUMP_PEOPLE ) ||
        ( m_eState == GCSquarePeople::WALK_PEOPLE && kSample.eState == GCSquarePeople::WAIT_PEOPLE ) || 
        ( m_eState == GCSquarePeople::SOCIAL_PEOPLE && kSample.eState == GCSquarePeople::WAIT_PEOPLE ) )
    {
        m_BasicData.vPos.x = kSample.fX;
        m_BasicData.vPos.y = kSample.fY;
    }
    m_BasicData.bIsRight = kSample.bIsRight;
    // [1/24/2008] breadceo. 다음 프레임 처리 끝
    SetJumpDir();
}

void GCSquarePeople::MoveXInSkyRemote()
{
    if( m_bIsContact == true || !IsJumpState() )
        return;

    m_fXSpeed = m_pSensor->GetJumpXSpeed();
}

bool GCSquarePeople::IsLocalPlayer()
{
    return g_kGlobalValue.m_kUserInfo.dwUID == m_dwUID;
}

void GCSquarePeople::CheckEquipItemState()
{
    m_bShadowHead = false;
    m_bEquipBigHead = false;
    m_bLoadDownHelmetNFL = false;
    m_pNonChangeFaceLift = NULL;

    DWORD dwBigHeadPos = (ESP_A_DOWN_HELMET+ESP_A_UPPER_HELMET);
    std::vector< DWORD >::iterator vecIter = m_kLoadedState.m_vecModelID.begin();
    for(; vecIter != m_kLoadedState.m_vecModelID.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData( (*vecIter/10) );
        if( pItem && (dwBigHeadPos == (pItem->dwSlotPosition & dwBigHeadPos)) )
        {
            m_bEquipBigHead = true;
        }
        if( (*vecIter/10) == ITEMID_ASIN_SHAADOW_SET ) 
        {
            m_bShadowHead = true;
        }

        if( m_bEquipBigHead && m_bShadowHead )
            break;
    }
}

// [1/7/2008] breadceo. People 보다 작은단위로 로딩하려면 조금 더 신경써야함
bool GCSquarePeople::Load()
{
    if( true == GetInit() )
        return true;

    CreateObject();
    MakeModelLoadList();
    MakeMotionLoadList();
    MakeEffectLoadList();
    CheckEquipItemState();

    std::vector< UINT >::iterator vitMotion;
    for( vitMotion = m_kLoadedState.m_vecMotionID.begin(); vitMotion != m_kLoadedState.m_vecMotionID.end(); ++vitMotion )
    {
        m_pObject->AddMotion( (*vitMotion) );
    }

    std::vector< DWORD >::iterator vitModel;
    // 개편된 투구 쓰고 있니?
    for( vitModel = m_kLoadedState.m_vecModelID.begin(); vitModel != m_kLoadedState.m_vecModelID.end(); ++vitModel )
    {
        if( SiGCFaceOffManager()->IsFaceOffItem( (*vitModel) / 10 ) ){
            m_bEquipFaceOffHead = true;
            break;
        }
    }

    for( vitModel = m_kLoadedState.m_vecModelID.begin(); vitModel != m_kLoadedState.m_vecModelID.end(); ++vitModel )
    {
        // 표정아이템(출석포인트로 사는)은 개편된 투구에서는 그리지 말자
        if( m_bEquipFaceOffHead == true && SiGCFaceOffManager()->IsEmotionItem( (*vitModel) / 10 ) ){
            continue;
        } else {
            LoadItemModel( (*vitModel) );
        }
    }

    LoadETCModel();
    ItemEffectParam kEffectParam( this, m_kLoadedState.m_vecEffectID );
    SetItemEffect( kEffectParam );
    SetItemEffect2( kEffectParam );

    for( int i=0; i< (int)m_vecItemEff.size(); ++i )
    {
        ITEM_EFFECT& stEffect = m_vecItemEff[i];
		CreateItemEffectParam kParam( m_bRender, m_BasicData.bIsRight, stEffect, m_posDesc );
		CreateItemEffect( kParam );
    }

    OnLoadingComplete();
    return true;
}

GCDeviceMotionFRM* GCSquarePeople::GetMotionPtr() const
{
	// 모델이 없어도 일단 홀드모션을 기본으로 해서 포인터를 반환하겠다. NULL반환하면 
	// NULL포인터 참조하다 죽어나는 곳이 꽤나 생길것 같다는 생각들에...
	if( m_pObject == NULL )
		return g_pGCDeviceManager2->CreateMotionFRM( "1455.frm" ); 

    return m_pObject->GetMotionPtr();
}

void GCSquarePeople::Get_BasicMatrix( D3DXMATRIX* pMat, bool bCamera, bool bForNickName )
{
    D3DXMATRIX mat2, mat3;
    float fCharScaleRatio = CHAR_SCALE_RATIO;
    if( m_PlayerTemplate.fPlayingScale != 0.f ) {
        fCharScaleRatio = CHAR_SCALE_RATIO * m_PlayerTemplate.fPlayingScale;
    }
    if( m_BasicData.bIsRight )
    {
        D3DXMatrixScaling ( pMat, fCharScaleRatio, fCharScaleRatio, fCharScaleRatio );
    }
    else
    {
        D3DXMatrixScaling ( &mat2, fCharScaleRatio, fCharScaleRatio, fCharScaleRatio);
        D3DXMatrixRotationY( &mat3, 3.1415f);
        D3DXMatrixMultiply( pMat, &mat2, &mat3 );
    }

    float f_y_Ex = 0.0f;
    if( !bForNickName )
    {
       f_y_Ex = ( m_pObject->GetGCObject()->GetFrame()->Pos_y ) * CHAR_SCALE_RATIO;
    }

    if( bCamera )
    {
        D3DXMatrixTranslation( &mat2, m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.48f + f_y_Ex, KGCLayerPart::ms_fZOrder[6] );
    }
    else
    {
        D3DXMatrixTranslation( &mat2, m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.23f + f_y_Ex, KGCLayerPart::ms_fZOrder[6] );
    }
    D3DXMatrixMultiply( &mat3, pMat, &mat2 );

    if( bCamera )
    {
    D3DXMatrixMultiply( pMat, &mat3, &g_kCamera.m_matCamera );
    }
    else
    {
        *pMat = mat3;
    }
}

void GCSquarePeople::DoLastFrame( bool bFrameOver )
{
    if( bFrameOver )
    {
        if( m_bIsContact )
        {
            SetPeopleState( GCSquarePeople::WAIT_PEOPLE );
            m_fXSpeed = 0.0f;
        }
        else
        {
            SetPeopleState( GCSquarePeople::JUMP_PEOPLE );
        }
    }
}

void GCSquarePeople::DoNextFrame()
{
    if( m_bFrameLock == false && m_bBufferEmpty == false )
    {
        ++m_BasicData.cFrame;
    }

    if( !m_bIsContact && ( m_eState == GCSquarePeople::WAIT_PEOPLE || m_eState == GCSquarePeople::WALK_PEOPLE || m_eState == GCSquarePeople::SOCIAL_PEOPLE ) )
    {
        SetPeopleState( GCSquarePeople::JUMP_PEOPLE );
        SetFrame( 0 );
    }
}

void GCSquarePeople::Jump()
{
    SetPeopleState( GCSquarePeople::JUMP_PEOPLE );
    SetFrame( 0 );
    SetContact( false );
    SetYSpeed( 0.035f );
}

void GCSquarePeople::DashJump()
{
    SetPeopleState( GCSquarePeople::DASH_JUMP_PEOPLE );
    SetFrame( 0 );
    SetContact( false );
    SetYSpeed( 0.03f );

    if( m_BasicData.bIsRight )
    {
        SetXSpeed( 0.025f );
    }
    else
    {
        SetXSpeed( -0.025f );
    }
}

void GCSquarePeople::SetJumpDir()
{
    if( IsJumpState() )
    {
        if( m_fXSpeed > 0.0f )
        {
            SetIsRight( true );
        }
        else if( m_fXSpeed < 0.0f )
        {
            SetIsRight( false );
        }
    }
}

void GCSquarePeople::ShadowRender()
{
    if( false == GetInit() || false == GetRender() )
        return;

    float Shadow_y = g_MyD3D->MyStg->Get_LowHeight( m_BasicData.vPos.x, m_BasicData.vPos.y );
    float fDistance = m_BasicData.vPos.y - Shadow_y + ( GetMotionPtr()->GetFrameData( (UINT)m_BasicData.cFrame ).Pos_y ) * CHAR_SCALE_RATIO;
    if( fDistance < -0.18f )
        fDistance = -0.18f;

    D3DXMATRIX mat1, mat2, mat3;
    D3DXMatrixScaling( &mat1, CHAR_SCALE_RATIO, CHAR_SCALE_RATIO, 0.0001f );
    D3DXMatrixTranslation( &mat2, m_BasicData.vPos.x - 1.0f, Shadow_y - 0.5f, KGCLayerPart::ms_fZOrder[6] );
    D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
    D3DXMatrixMultiply( &mat1, &mat3, &g_kCamera.m_matCamera );

    D3DXVECTOR4 vResult;
    D3DXVec3Transform( &vResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1 );

    float fShadowSize = 0.150f;
    g_pGraphicsHelper->SimpleShadowRender( vResult.x, vResult.y, fDistance, fShadowSize, KGCLayerPart::ms_fZOrder[7] );
}

void GCSquarePeople::EquipBaseItem( std::vector<DWORD>& vecItem )
{
    DWORD dwFlag = 0;

    for( int i = 0; i < (int)vecItem.size(); i++ )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( vecItem[i] / 10 );
        CONTINUE_NIL( pItem );
        dwFlag |= pItem->dwSlotPosition;
    }

    if ( !( dwFlag & ESP_HELMET ) )// 머리가 없다면 기본 머리를 넣는다.
    {
		vecItem.push_back( g_pItemMgr->GetBaseItemID( (int)m_kPeopleInfo.m_cCharType, ESP_HELMET ) * 10 );        
    }
    if ( !( dwFlag & ESP_JACKET ) )// 상의가 없다면 기본 상의를 넣는다.
    {
        vecItem.push_back( g_pItemMgr->GetBaseItemID( (int)m_kPeopleInfo.m_cCharType, ESP_JACKET ) * 10 );
    }
    if ( !( dwFlag & ESP_PANTS ) )// 하의가 없다면 기본 하의를 넣는다.
    {
        vecItem.push_back( g_pItemMgr->GetBaseItemID( (int)m_kPeopleInfo.m_cCharType, ESP_PANTS ) * 10 );
    }

    m_bIsEquipCoordiSetItem = g_pItemMgr->GetEquipCoordiSetItem(m_kCoordiSetTemplet, m_kPeopleInfo);
	
	//------------------------------------------------------------------------
	//	20091027 [광장 세트 아이템 모션 작업] : 세트 아이템인 경우에는 기본 무기를 넣지 않고 함수 리턴
	int iSetItem = g_pItemMgr->GetSetItemNumberByEquipVector( m_kPeopleInfo.m_vecEquips );
	if( iSetItem != -1 )
	{
		if( g_pItemMgr->IsSetItemMotion(iSetItem) )
			return;
	}
	//========================================================================

    if ( !( dwFlag & ESP_WEAPON ) )// 무기가 없다면 기본 무기를 넣는다.
    {
        vecItem.push_back( g_pItemMgr->GetBaseItemID( (int)m_kPeopleInfo.m_cCharType, ESP_WEAPON ) * 10 );
    }
}

void GCSquarePeople::NotifyPositionToServer()
{
    m_nNotifyPositionToServer = NOTIFY_MYPOS_TO_SERVER_TIME;
    SiKP2P()->Send_UpdateSquareMyPos( SiGCSquare()->GetSquareID(), m_BasicData.vPos.x, m_BasicData.vPos.y );
}

void GCSquarePeople::SetPeopleInfo( const KSquareUserInfo& sInfo_ )
{
    m_kPeopleInfo = sInfo_;

    if( m_kPeopleInfo.m_kPremiumInfo.CheckType( KPremiumInfo::TYPE_GCBLOG ) )
        m_pGCClubTexture = g_pItemMgr->CreateItemTexture( (DWORD)KGCItemManager::ITEM_GC_BLOG, 0 );
    else if( m_kPeopleInfo.m_kPremiumInfo.CheckType( KPremiumInfo::TYPE_GCCLUB ) )
        m_pGCClubTexture = g_pItemMgr->CreateItemTexture( (DWORD)KGCItemManager::ITEM_GC_CLUB, 0 );
    else if (m_kPeopleInfo.m_kPremiumInfo.CheckType(KPremiumInfo::TYPE_GCPREMIUM))
        m_pGCClubTexture = g_pItemMgr->CreateItemTexture((DWORD)KGCItemManager::ITEM_GC_PREMIUM, 0);
	else
		m_pGCClubTexture = NULL;

	if( m_kPeopleInfo.m_dwGuildUID != 0 )
	{
		if(m_kPeopleInfo.m_strMarkName == L"")
			m_kPeopleInfo.m_strMarkName = L"defaultmark.dds";
		SiKGCIconImageManager()->LoadIconImageFromHTTP(m_kPeopleInfo.m_strMarkName);	
	}

    SetTemplate();
    SetPos( D3DXVECTOR3( sInfo_.m_kSquarePos.m_fXpos, sInfo_.m_kSquarePos.m_fYpos, 0.0f ) );    
}

void GCSquarePeople::SetTemplate()
{
    std::vector<GCITEMID>::const_iterator vit = m_kPeopleInfo.m_vecEquips.begin();
    int iNowPromotion = 0;
    for( ; vit != m_kPeopleInfo.m_vecEquips.end() ; ++vit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( (*vit) / 10 );
        if( pItem == NULL ) { 
            continue;
        }

        if( pItem->dwSlotPosition & ESP_WEAPON ) { 
            iNowPromotion = pItem->iNeedChangeJobLevelForEquippingItem;
            break;
        }
    }

    if( g_kGlobalValue.IsOneWeaponCharacter( m_kPeopleInfo.m_cCharType ) ) { 
        //iNowPromotion = 0;
        iNowPromotion = m_kPeopleInfo.m_cCharPromotion;
        if( iNowPromotion < 0 ) {
            iNowPromotion = 0;
        }
    }

    //Extra_Char_Num = m_kPeopleInfo.m_cCharType + GC_CHAR_NUM * m_kPeopleInfo.m_cCharPromotion;
    Extra_Char_Num = m_kPeopleInfo.m_cCharType + GC_CHAR_NUM * iNowPromotion;
    m_posDesc.charID = (CHAR_IDENTY)Extra_Char_Num;

	//------------------------------------------------------------------------
	//20091023 [광장 세트 아이템 모션 작업] : 이동석
	int iSetItem = g_pItemMgr->GetSetItemNumberByEquipVector( m_kPeopleInfo.m_vecEquips );

	// 세트 아이템을 착용하고 있다면
	if( -1 != iSetItem  )
	{
		std::map< char, UINT >::iterator mit;	//일반모션 검색용 반복자
		std::map< int, UINT >::iterator itr;	//광장모션 검색용 반복자

		// 세트 모션이 있다면 세트 모션, 아니면 일반 모션
		FIND_AND_SET_SQUAREMOTION( m_kSpTemplate.WAIT, ms_mapWaitCharMotion, ms_mapSquareWaitCharMotion );
		FIND_AND_SET_SQUAREMOTION( m_kSpTemplate.WALK, ms_mapWalkCharMotion, ms_mapSquareWalkCharMotion );
		FIND_AND_SET_SQUAREMOTION( m_kSpTemplate.JUMP_READY, ms_mapJumpReadyCharMotion, ms_mapSquareJumpReadyCharMotion );
		FIND_AND_SET_SQUAREMOTION( m_kSpTemplate.JUMP, ms_mapJumpCharMotion, ms_mapSquareJumpCharMotion );
		FIND_AND_SET_SQUAREMOTION( m_kSpTemplate.DASH, ms_mapDashCharMotion, ms_mapSquareDashCharMotion );
	}
	else
	{
		std::map<char, UINT>::iterator mit;
		FIND_AND_SET( m_kSpTemplate.WAIT, ms_mapWaitCharMotion );
		FIND_AND_SET( m_kSpTemplate.WALK, ms_mapWalkCharMotion );
		FIND_AND_SET( m_kSpTemplate.JUMP_READY, ms_mapJumpReadyCharMotion );
		FIND_AND_SET( m_kSpTemplate.JUMP, ms_mapJumpCharMotion );
		FIND_AND_SET( m_kSpTemplate.DASH, ms_mapDashCharMotion );
	}
	//========================================================================
	std::vector< EGCSkillTree > setSkill;
    SetPlayerTemplate(setSkill, Extra_Char_Num, m_PlayerTemplate );
}


void GCSquarePeople::DrawPetName( int x,int y )
{	
	if( m_pObject )
	{
		if( m_pObject->GetGCObject() )
		{
			if( m_pObject->GetGCObject()->IsUseBuildNickNameTex() )
				m_pObject->GetGCObject()->DrawText( 3, (float)(x-128), (float)y );
			else
				g_pkFontMgrOutline->OutTextXY( x, y, m_prPetNameWithColor.first.c_str(), m_prPetNameWithColor.second, NULL, DT_CENTER | DT_TOP );
		}
	}
}

void GCSquarePeople::ResetPetName()
{	
	if( m_pObject )
	{
		if( m_pObject->GetGCObject() )
			m_pObject->GetGCObject()->SetText(3,m_prPetNameWithColor.first);
	}
}

void GCSquarePeople::SetPetName( std::wstring str )
{	
	if( m_pObject )
	{
		if( m_pObject->GetGCObject() )
			m_pObject->GetGCObject()->SetText(3,str);
	}

	m_prPetNameWithColor.first = str;
	m_prPetNameWithColor.second = 0xFF00FF00;
}

void GCSquarePeople::DrawNickName()
{
	KGCPC_BEGIN("DrawNickName");
    D3DXMATRIX mat1,mat2,matBasic;
    Get_BasicMatrix( &matBasic, true, true );

    D3DXVECTOR3 vNickNamePos;
    D3DXMatrixTranslation( &mat2, -.15f, -.1f, 0);
    D3DXMatrixMultiply( &mat1, &matBasic, &mat2 );

    ::D3DXVec3TransformCoord( &vNickNamePos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat1 );
    // offset
    vNickNamePos.x += 0.15f;
    vNickNamePos.y += 0.055f;

    // world coordinates -> screen coordinates
	float fScreenX = ( vNickNamePos.x + 1.0f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ;
	float fScreenY = ( -vNickNamePos.y + 0.75f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ;

	//해상도 키우면서 작은 모션 변화에도 이름이나 말풍선에 흔들림이 눈에 띄는거 수정하기 위해 보정
	//값 저장하고 있다가 보정치 이상 변화할때만 이동하도록 수정 할 필요 있음.
	/*int iTemp = (int)(g_pGCDeviceManager->GetWindowScaleX() * 25);
	fScreenX = (float)(((int)(fScreenX / iTemp))*iTemp);
	fScreenY = (float)(((int)(fScreenY / iTemp))*iTemp);*/

	fScreenX*=g_pGCDeviceManager->GetWindowScaleX();
	fScreenY*=g_pGCDeviceManager->GetWindowScaleY();
	if( m_pObject->GetGCObject()->IsUseBuildNickNameTex() )
	{
		m_pObject->GetGCObject()->DrawText(0,fScreenX-128, fScreenY);
	}
	else
	{
		std::wstringstream strmPlayerName;
        std::wstringstream strmPlayerColor;
		strmPlayerName << g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str() << L" " << m_kPeopleInfo.m_dwLevel << L". ";
		strmPlayerName << m_kPeopleInfo.m_strNick;
        strmPlayerColor << m_kPeopleInfo.m_strNickColor;
		g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, strmPlayerName.str().c_str(), SiGCTextColorManager()->StringToColor(strmPlayerColor.str()), NULL, DT_CENTER | DT_TOP );
	}

	KGCPC_END();
}

bool GCSquarePeople::TimeShareLoading()
{
    if( true == GetInit() )
        return true;

    if( m_pObject == NULL )
    {
        m_kLoadedState.RefreshTime();
        CreateObject();
        MakeModelLoadList();
        MakeMotionLoadList();
        MakeEffectLoadList();
        CheckEquipItemState();

		std::vector< DWORD >::iterator vitModel;
		// 개편된 투구 쓰고 있니?
		for( vitModel = m_kLoadedState.m_vecModelID.begin(); vitModel != m_kLoadedState.m_vecModelID.end(); ++vitModel )
		{
			if( SiGCFaceOffManager()->IsFaceOffItem( (*vitModel) / 10 ) ){
				m_bEquipFaceOffHead = true;
				break;
			}
		}

		for( vitModel = m_kLoadedState.m_vecModelID.begin(); vitModel != m_kLoadedState.m_vecModelID.end(); ++vitModel )
		{
			// 표정아이템(출석포인트로 사는)은 개편된 투구에서는 그리지 말자
			if( m_bEquipFaceOffHead == true && SiGCFaceOffManager()->IsEmotionItem( (*vitModel) / 10 ) ){
				continue;
			} else {
				LoadItemModel( (*vitModel) );
			}
		}
		LoadETCModel();

        if( m_kLoadedState.IsTimeOver() )
        {
            return false;
        }
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bMotionLoad )
    {
        while( false == m_kLoadedState.IsTimeOver() )
        {
            if( m_kLoadedState.m_vecMotionID.empty() )
            {
#ifdef LOAD_CHECK
                START_LOG( clog, m_kPeopleInfo.m_dwUID << " 모션 로딩 끝" );
#endif
                m_kLoadedState.m_bMotionLoad = true;
                break;
            }
            else
            {
                UINT uiCurLoading = (*m_kLoadedState.m_vecMotionID.begin());
                m_pObject->AddMotion( uiCurLoading );
                m_kLoadedState.m_vecMotionID.erase( m_kLoadedState.m_vecMotionID.begin() );
            }
        }
        if( false == m_kLoadedState.m_bMotionLoad )
        {
            return false;
        }
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bModelLoad )
    {
        while( false == m_kLoadedState.IsTimeOver() )
        {
            if( m_kLoadedState.m_vecModelID.empty() )
            {
#ifdef LOAD_CHECK
                START_LOG( clog, m_kPeopleInfo.m_dwUID << " 모델 로딩 끝" );
#endif
                m_kLoadedState.m_bModelLoad = true;
                break;
            }
            else
            {
                LoadItemModel( (*m_kLoadedState.m_vecModelID.begin()) );
                m_kLoadedState.m_vecModelID.erase( m_kLoadedState.m_vecModelID.begin() );
            }
        }

        if( false == m_kLoadedState.m_bModelLoad )
        {
            return false;
        }
        LoadETCModel();
    }

	

    m_kLoadedState.RefreshTime();

    if( false == m_kLoadedState.m_bEffectLoad )
    {
        while( false == m_kLoadedState.IsTimeOver() )
        {
            if( m_kLoadedState.m_vecEffectID.empty() )
            {
#ifdef LOAD_CHECK
                START_LOG( clog, m_kPeopleInfo.m_dwUID << " Effect1 로딩 끝" );
#endif
                m_kLoadedState.m_bEffectLoad = true;
                break;
            }
            else
            {
                LoadOneEffect( *m_kLoadedState.m_vecEffectID.begin() );
                m_kLoadedState.m_vecEffectID.erase( m_kLoadedState.m_vecEffectID.begin() );
            }
        }
        if( false == m_kLoadedState.m_bEffectLoad )
        {
            return false;
        }
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bEffectLoad2 )
    {
        // SetItem 인듯
        std::vector<int> vecItemInfo;
        for( int i = 0; i < (int)m_kPeopleInfo.m_vecEquips.size(); ++i )
        {
            vecItemInfo.push_back( m_kPeopleInfo.m_vecEquips[i] / 10 );
        }

        ItemEffectParam kEffectParam( this, vecItemInfo );
        SetItemEffect2( kEffectParam );
        m_kLoadedState.m_bEffectLoad2 = true;
#ifdef LOAD_CHECK
        START_LOG( clog, m_kPeopleInfo.m_dwUID << " Effect2 로딩 끝" );
#endif
        return false;
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bCreateEffect )
    {
        while( false == m_kLoadedState.IsTimeOver() )
        {
            if( m_vecItemEff.empty() )
            {
                m_kLoadedState.m_bCreateEffect = true;
                break;
            }

            ITEM_EFFECT& stEffect = m_vecItemEff[m_kLoadedState.m_iLoadedItemEffIdx++];
            CreateItemEffectParam kParam( m_bRender, m_BasicData.bIsRight, stEffect,m_posDesc );
            CreateItemEffect( kParam );

            if( m_kLoadedState.m_iLoadedItemEffIdx == (int)m_vecItemEff.size() )
            {
                m_kLoadedState.m_bCreateEffect = true;
                break;
            }
        }
        if( false == m_kLoadedState.m_bCreateEffect )
        {
            return false;
        }
    }

    if( m_kLoadedState.IsComplete() )
    {
        OnLoadingComplete();
#ifdef LOAD_CHECK
        START_LOG( clog, m_kPeopleInfo.m_dwUID << " 로딩 끝" );
#endif
        m_kLoadedState.Init();
        return true;
    }
    ASSERT( !"여기 오는건 이상한데?" );
    return false;
}

void GCSquarePeople::CreateObject()
{
    SAFE_DELETE( m_pObject );
    m_pObject = new KGC3DObject();
    m_pObject->Init();
    m_pObject->SetRender( false );
    m_pObject->SetIsLive( false );

    float fCharScaleRatio = CHAR_SCALE_RATIO;
    if( m_PlayerTemplate.fPlayingScale != 0.f ) {
        fCharScaleRatio = CHAR_SCALE_RATIO * m_PlayerTemplate.fPlayingScale;
    }
    m_pObject->SetScale( fCharScaleRatio );

    m_pObject->IsRight( m_BasicData.bIsRight );
	m_pObject->SetModelZAxis( KGCLayerPart::ms_fZOrder[6]);
}

void GCSquarePeople::LoadETCModel()
{
    if ( true == g_MyD3D->m_KGCOption.GetSquareQuality()  && IsRemovePartsItem(HEAD) ) 
        return;

    std::vector< ADD_MESH >::iterator vecAddMeshIter = m_PlayerTemplate.vecAddMesh.begin();
    for(; vecAddMeshIter != m_PlayerTemplate.vecAddMesh.end(); ++vecAddMeshIter)
    {
        // 모든 조건을 통과해야만 추가 메시를 로드할 수 있닷!!
        bool bConBigHead = true, bConEquipItem = true, bConNonEquipItem = true, bConGameModeCategory = true, bConPlaying = true, bConEquipSetItem = true, bConNonEquipSetItem = true;

        // 빅헤드 조건확인( 0이면 빅헤드가 아닐때 로드, 1이면 빅헤드 일때 로드 )
        if( -1 < vecAddMeshIter->iConBigHead )
        {
            if( m_bEquipBigHead )
                bConBigHead = !( 0 == vecAddMeshIter->iConBigHead );
            else
                bConBigHead = !( 1 == vecAddMeshIter->iConBigHead );
        }

        // 장착한 장비 조건 확인
        if( -1 < vecAddMeshIter->iConEquipItem )
        {
            std::vector<GCITEMID>::iterator vecEquipIter = m_kPeopleInfo.m_vecEquips.begin();
            for(; vecEquipIter != m_kPeopleInfo.m_vecEquips.end(); ++vecEquipIter) 
            {
                if( (*vecEquipIter / 10) == vecAddMeshIter->iConEquipItem ) {
                    break;
                }
            }
            bConEquipItem = (vecEquipIter != m_kPeopleInfo.m_vecEquips.end());
        }
        
        // 장착한 안한 장비 조건 확인
        if( -1 < vecAddMeshIter->iConNonEquipItem )
        {
            std::vector<GCITEMID>::iterator vecEquipIter = m_kPeopleInfo.m_vecEquips.begin();
            for(; vecEquipIter != m_kPeopleInfo.m_vecEquips.end(); ++vecEquipIter) 
            {
                if( (*vecEquipIter / 10) == vecAddMeshIter->iConNonEquipItem ) {
                    break;
                }
            }
            bConNonEquipItem = (vecEquipIter == m_kPeopleInfo.m_vecEquips.end());
        }

        //장착한 세트 장비 조건 확인
        if( -1 < vecAddMeshIter->iConEquipSetItem )
        {
            bConEquipSetItem = g_MyD3D->m_kItemMgr.IsChangeMeshCoordi(vecAddMeshIter->iConEquipSetItem);
        }
        //장착 안한 세트 장비 조건 확인
        if( -1 < vecAddMeshIter->iConNonEquipSetItem )
        {
            bConNonEquipSetItem = (g_MyD3D->m_kItemMgr.IsChangeMeshCoordi(vecAddMeshIter->iConNonEquipSetItem) == true ) ? false : true;
        }


        // 게임모드 카테고리 조건 확인
        if( -1 < vecAddMeshIter->iConGameModeCategory )
        {
            continue;
        }

        // 플레이 중인지 조건 확인
        if( -1 < vecAddMeshIter->iConPlaying )
        {
            if( 0 == vecAddMeshIter->iConPlaying )
                bConPlaying = false;
        }

        // 종착지
        if( bConBigHead && bConEquipItem && bConNonEquipItem && bConGameModeCategory && bConPlaying && bConEquipSetItem && bConNonEquipSetItem )
        {
            if( "" == vecAddMeshIter->piLiveInfo.first )
            {
                GCMeshObject* pAddMesh = m_pObject->GetGCObject()->AddMesh( g_pGCDeviceManager2->CreateMeshP3M(vecAddMeshIter->strMeshName), g_pGCDeviceManager2->CreateTexture(vecAddMeshIter->strTextureName) );
                if( pAddMesh != NULL ) 
                {
                    pAddMesh->m_bExtraMeshObject = vecAddMeshIter->bIsExtraMesh;
                    pAddMesh->m_iTechnique = vecAddMeshIter->iTechnique;
                    pAddMesh->m_bRender = vecAddMeshIter->bIsRender;
                    pAddMesh->m_bSecondBaseMesh = vecAddMeshIter->bIsSecondBaseMesh;
                    if( false == vecAddMeshIter->strChildName.empty() )
                        pAddMesh->m_strName = vecAddMeshIter->strChildName;
                }
            }
            else
            {
                GCObject* pAddMesh = g_pGCDeviceManager2->CreateLiveModel( vecAddMeshIter->strMeshName.c_str(), vecAddMeshIter->strTextureName.c_str(), vecAddMeshIter->piLiveInfo.first.c_str(), vecAddMeshIter->piLiveInfo.second, m_pObject->GetGCObject() );
                if( pAddMesh != NULL ) 
                {
                    pAddMesh->m_bIsExtraObject = vecAddMeshIter->bIsExtraMesh;
                    pAddMesh->SetTechnique( vecAddMeshIter->iTechnique );
                    pAddMesh->SetRender( vecAddMeshIter->bIsRender );
                    if( false == vecAddMeshIter->strChildName.empty() )
                        pAddMesh->m_strName = vecAddMeshIter->strChildName;
                }
            }
        }
    }

    // 표정 없음
    if( m_bEquipFaceOffHead == false ){
        if( m_kPeopleInfo.m_cCharType != GC_CHAR_LUPUS && 
            m_kPeopleInfo.m_cCharType != GC_CHAR_RIN && 
            m_kPeopleInfo.m_cCharType != GC_CHAR_ASIN &&
            m_kPeopleInfo.m_cCharType != GC_CHAR_LIME )
            return;
    }

    if( NULL == m_pNonChangeFaceLift )
    {
        int iFaceLiftType = -1;
        if( m_bEquipBigHead ) {
            iFaceLiftType = FACE_LIFT_BIG_HEAD;
        } else {
            iFaceLiftType = FACE_LIFT_NORMAL;
        }
        if( m_bShadowHead ) {
            iFaceLiftType = FACE_LIFT_SHADOW;
        }

        std::map< int, FACE_LIFT_MESH >::iterator mit = m_PlayerTemplate.mapFaceLiftMesh.find( iFaceLiftType );
        if( mit != m_PlayerTemplate.mapFaceLiftMesh.end() ) {
            m_pObject->GetGCObject()->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( mit->second.strMeshName ), g_pGCDeviceManager2->CreateTexture( mit->second.strTextureName ) );    
        }
    }
}

// 얼굴 표정변환 시스템에 어울리지 않는 특수복장과 악세사리 아이템때문에
// 어울리는 표정 하나만 로드하기 위한 코드입니다.
void GCSquarePeople::LoadFaceTexture( GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition )
{
    // 일반 머리와 빅헤드, 헤드하위장식이 아니면 나가
    DWORD dwBigHead = ESP_A_UPPER_HELMET + ESP_A_DOWN_HELMET;
    if( (ESP_HELMET != (dwSlotPosition & ESP_HELMET)) && 
        (ESP_A_DOWN_HELMET != (dwSlotPosition & ESP_A_DOWN_HELMET)) && 
        (dwBigHead != (dwSlotPosition & dwBigHead)) )
        return;

    // 하위장식이 로드되었는데 헬멧로드하려고 하면 나가
    if( m_bLoadDownHelmetNFL && (dwSlotPosition == (dwSlotPosition & ESP_HELMET)) )
        return;

    // 리소스 이름을 만들어보자
    std::string strTextureName;
    const char* strAbtaName = g_pGCDeviceManager2->GetAbtaName( g_pItemMgr->GetSameImageIndex( iItemID ) );
    strTextureName = g_pGCDeviceManager2->GetLupusResourceFileName( strAbtaName, iCharType, iPromotion, "_NFL.dds" );
    if( "" == strTextureName )
        strTextureName = g_pGCDeviceManager2->GetResourceFileName( strAbtaName, iCharType, iPromotion, "_NFL.dds" );

    // 텍스쳐 파일이름기록이 안됫으면 없는 리소스니깐 나가
    if( strTextureName.empty() || "" == strTextureName )
        return;

    int iFaceLiftType;
    if( m_bEquipBigHead ) {
        iFaceLiftType = FACE_LIFT_BIG_HEAD;
    } else {
        iFaceLiftType = FACE_LIFT_NORMAL;
    }

    if( m_bShadowHead ) {
        iFaceLiftType = FACE_LIFT_SHADOW;
    }

    PLAYER_TEMPLATE kPlayerTemplate = GetPlayerTemplate();
    std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find( iFaceLiftType );
    if( mit != kPlayerTemplate.mapFaceLiftMesh.end() )
    {
        bool bDownHelmet = false;
        if( (dwSlotPosition == (dwSlotPosition & ESP_A_DOWN_HELMET)) )
            bDownHelmet = true;

        // 하위장식 로드하려고 하는데 다른 헬멧로드된 상태면 그 헬멧 지워
        if( bDownHelmet && m_pNonChangeFaceLift )
        {
            pObject->RemoveMesh( m_pNonChangeFaceLift->m_pMesh );
            m_pNonChangeFaceLift = NULL;
        }

        GCMeshObject* pMeshObj = pObject->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( mit->second.strMeshName ), g_pGCDeviceManager2->CreateTexture( strTextureName.c_str() ) );
        if( pMeshObj != NULL ) 
        {
            m_pNonChangeFaceLift = pMeshObj;

            // 하위장식 로드했으면 로드했다고 표시
            if( bDownHelmet )
                m_bLoadDownHelmetNFL = true;
        }
    }
}

void GCSquarePeople::MakeModelLoadList()
{
    m_kLoadedState.m_vecModelID.clear();
    if( true == g_MyD3D->m_KGCOption.GetSquareQuality() )
    {
        //m_kLoadedState.m_vecModelID = m_kPeopleInfo.m_vecEquips;
        for( std::vector<GCITEMID>::const_iterator it = m_kPeopleInfo.m_vecEquips.begin() ; it != m_kPeopleInfo.m_vecEquips.end() ; ++it ) { 
            if( !g_pItemMgr->IsTransparentCoordiItem( *it / 10 ) ) {
                m_kLoadedState.m_vecModelID.push_back( *it );
            }
        }

        EquipBaseItem( m_kLoadedState.m_vecModelID );
        EgoItemInitialize();
    }
    else
    {
        return MakeLowModelLoadList();
    }
}

void GCSquarePeople::MakeLowModelLoadList()
{

    std::vector<GCITEMID>::const_iterator vit = m_kPeopleInfo.m_vecEquips.begin();
    int iNowPromotion = 0;
    for( ; vit != m_kPeopleInfo.m_vecEquips.end() ; ++vit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( (*vit) / 10 );
        if( pItem == NULL ) { 
            continue;
        }

        if( pItem->dwSlotPosition & ESP_WEAPON ) { 
            iNowPromotion = pItem->iNeedChangeJobLevelForEquippingItem;
            break;
        }
    }

    if ( g_MyD3D->m_TempPlayer.IsOneWeaponChar() )
        iNowPromotion = g_MyD3D->m_TempPlayer.GetCurrentChar().iPromotionLevel;

    char Extra_Char_Num = m_kPeopleInfo.m_cCharType + GC_CHAR_NUM * iNowPromotion;
    //char Extra_Char_Num = m_kPeopleInfo.m_cCharType + GC_CHAR_NUM * m_kPeopleInfo.m_cCharPromotion;
    const std::vector<KSquareBaseEquip>& vecSquareBaseEquip = SiGCSquare()->GetSquareBaseEquipContainer();
    std::vector<KSquareBaseEquip>::const_iterator it;
    for (it = vecSquareBaseEquip.begin(); it != vecSquareBaseEquip.end(); ++it)
    {
        if ( Extra_Char_Num == it->iCharNum )
        {
            for (int i=0; i < static_cast<int>(it->vecParts.size()); i++)
            {
                m_kLoadedState.m_vecModelID.push_back( it->vecParts[i] );
            }
        }
        else if ( Extra_Char_Num > CID_MAX || Extra_Char_Num <= CID_COMMON )
        {
            std::vector<KSquareBaseEquip>::const_iterator itTemp = vecSquareBaseEquip.begin();
            for (int i=0; i < static_cast<int>(itTemp->vecParts.size()); i++)
            {
                m_kLoadedState.m_vecModelID.push_back( itTemp->vecParts[i] );
            }
        }
    }
}
void GCSquarePeople::MakeMotionLoadList()
{
    m_kLoadedState.m_vecMotionID.clear();
    for( int i = (int)GCSquarePeople::WAIT_PEOPLE; i != (int)GCSquarePeople::NUM_PEOPLE_STATE; ++i )
    {
        m_kLoadedState.m_vecMotionID.push_back( GetMotionID( (GCSquarePeople::PEOPLE_STATE)i ) );
    }

    // 소셜 모션들을 담는다.
    std::set<int> setSocialMotion;
    if( SiKGCSocialMotionManager()->GetSocialMotion(setSocialMotion) ) {
        std::set<int>::iterator sit = setSocialMotion.begin();
        for( sit; sit != setSocialMotion.end(); ++sit ) {
            m_kLoadedState.m_vecMotionID.push_back(*sit);
        }
    }    
}

void GCSquarePeople::SetRender( bool bRender )
{
    m_pObject->SetRender( bRender );
    IGCSquareObj::SetRender( true );
    ToggleEffect( bRender );
}

void GCSquarePeople::LoadItemModel( DWORD dwItemID )
{
    GCItem* pItem = g_pItemMgr->GetItemData( dwItemID / 10 );
    if( NULL == pItem )
        return;

    // 조건별 옵션을 적용할 것인가 결정
    ITEM_MESH_OPTION kOption;
    std::vector< ITEM_MESH_OPTION >::iterator vecMeshOptionIter = m_PlayerTemplate.vecItemMeshOption.begin();
    for(; vecMeshOptionIter != m_PlayerTemplate.vecItemMeshOption.end(); ++vecMeshOptionIter )
    {
        // 장착 포지션 확인
        if( pItem->dwSlotPosition != vecMeshOptionIter->dwConSlotPosition )
            continue;

        // 게임 플레이 중이 아닐때만 적용하는 옵션
        if( 0 == vecMeshOptionIter->iConPlaying )
            continue;

        // 광장이면 적용하지 않는 옵션
        if( 0 == vecMeshOptionIter->iConSquare )
            continue;

        // 옵션적용
        kOption = (*vecMeshOptionIter);
        break;
    } 

    if( kOption.bIsNotLoad )
        return;

    if( kOption.strChildName.empty() )
        kOption.strChildName = GetDefaultMeshName( pItem->dwSlotPosition );

    // 라이브 모델 로드
    std::vector<GCObject*> vecLiveModel = g_pItemMgr->CreateLiveModel( pItem->dwGoodsID, m_kPeopleInfo.m_cCharType, m_kPeopleInfo.m_cCharPromotion, m_pObject->GetGCObject(), kOption.vPos, kOption.bIsOnlyTraceParentPos, const_cast<char*>(kOption.strLiveMotionName.c_str()), kOption.iBoneID );

    // 라이브 모델이 없으면 일반모델 로드
    if( vecLiveModel.empty() )
    {
        GCMeshObject* pMeshObject = g_pItemMgr->CreateAbtaModel( pItem->dwGoodsID, m_kPeopleInfo.m_cCharType, m_kPeopleInfo.m_cCharPromotion, m_pObject->GetGCObject(), true, static_cast<char>(kOption.iBoneID) );
        if( pMeshObject )
        {
            pMeshObject->m_bRender = kOption.bIsRender;
            pMeshObject->m_strName = kOption.strChildName;
            if ( kOption.iTechnique != -1 )
            {
                pMeshObject->m_iTechnique = kOption.iTechnique;
            }
        }

        LoadFaceTexture( m_pObject->GetGCObject(), m_kPeopleInfo.m_cCharType, m_kPeopleInfo.m_cCharPromotion, pItem->dwGoodsID, pItem->dwSlotPosition );

    }
    else
    {
        // 라이브모델 옵션적용
        std::vector<GCObject*>::iterator vecLiveIter = vecLiveModel.begin();
        for(int iLoopCnt=0; vecLiveIter != vecLiveModel.end(); ++vecLiveIter, ++iLoopCnt)
        {
            char szChildMeshName[1024];
            strcpy( szChildMeshName, kOption.strChildName.c_str() );
            if( kOption.bIsAutoChildNameAsLiveLoopCnt )
                sprintf(szChildMeshName, "%s%d", kOption.strChildName.c_str(), iLoopCnt);

            if( 0 < static_cast<int>(strlen(szChildMeshName)) )
                (*vecLiveIter)->m_strName = szChildMeshName;

            (*vecLiveIter)->SetPosition( kOption.vPos );
            (*vecLiveIter)->SetScale( kOption.vScale );
            (*vecLiveIter)->SetRender( kOption.bIsRender );
            if ( kOption.iTechnique != -1 )
            {
                (*vecLiveIter)->SetTechnique( kOption.iTechnique );
            }

            std::vector<ADD_ITEM_LIVEMODEL>::iterator vecAddLiveIter = kOption.vecAddItemLiveModel.begin();
            for(; vecAddLiveIter != kOption.vecAddItemLiveModel.end(); ++vecAddLiveIter)
            {
                std::vector<GCObject*> vecAddLiveModel = g_pItemMgr->CreateLiveModel( pItem->dwGoodsID, m_kPeopleInfo.m_cCharType, m_kPeopleInfo.m_cCharPromotion, m_pObject->GetGCObject(), vecAddLiveIter->vPos, vecAddLiveIter->bOnlyTraceParentPos, const_cast<char*>(vecAddLiveIter->strMotion.c_str()), vecAddLiveIter->iBoneID, vecAddLiveIter->bUseBoneIDFromArg );
                std::vector<GCObject*>::iterator vecAddLiveModelIter = vecAddLiveModel.begin();
                for(; vecAddLiveModelIter != vecAddLiveModel.end(); ++vecAddLiveModelIter)
                {
                    (*vecAddLiveModelIter)->m_strName = vecAddLiveIter->strChildName;
                    (*vecAddLiveModelIter)->SetTechnique( vecAddLiveIter->iTechnique );
                    (*vecAddLiveModelIter)->SetRender( vecAddLiveIter->bRender );
                }
            }
        }
    }
}

std::string GCSquarePeople::GetDefaultMeshName( DWORD dwSlotPosition )
{
    if( dwSlotPosition & ESP_WEAPON )
        return "WEAPON";
    return "";
}

bool GCSquarePeople::Picking( const D3DXVECTOR2& vMousePos, const int iMouseBtn )
{
    // 나를 추가할 필요는 없겠지.
    if( IsLocalPlayer() )
        return false;

    const float fWidth = m_vRect.x * 0.5f;
    if( vMousePos.x >= m_BasicData.vPos.x - fWidth && vMousePos.x <= m_BasicData.vPos.x + fWidth )
    {
        if( vMousePos.y >= m_BasicData.vPos.y && vMousePos.y <= m_BasicData.vPos.y + m_vRect.y )
        {
            return true;
        }
    }

    return false;
}

void GCSquarePeople::DrawBodyRect()
{
    /*g_pGCDeviceManager2->SetRenderStateSet();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        static GCDeviceTexture* pNullTexture = g_pGCDeviceManager2->CreateTexture("임시로 사용하는 널 텍스쳐");
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_kCamera.m_matCamera );
        g_pGCDeviceManager2->SetDeviceTextureWithLoad(pNullTexture);*/
        
        const float fWidth = m_vRect.x * 0.5f;

        float fLeft, fRight, fTop, fBottom;
        fLeft = m_BasicData.vPos.x - fWidth - 1.0f;
        fRight = m_BasicData.vPos.x + fWidth - 1.0f;
        fTop = m_BasicData.vPos.y + m_vRect.y - 0.5f;
        fBottom = m_BasicData.vPos.y - 0.5f;

        D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
        SETVECTOR3( lefttop, fLeft, fTop, 0.4f );
        SETVECTOR3( righttop, fRight, fTop, 0.4f );
        SETVECTOR3( leftbottom, fLeft, fBottom, 0.4f );
        SETVECTOR3( rightbottom, fRight, fBottom, 0.4f );

		g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
			lefttop, righttop, leftbottom, rightbottom, 0,0,0,
			D3DCOLOR_ARGB(180, rand() % 255, rand() % 255, rand() % 255));
        /*g_MyD3D->Draw_Billboard3( lefttop, righttop, leftbottom, rightbottom, 180, rand() % 255, rand() % 255, rand() % 255 );

        /*D3DXMATRIX matWorld;
        D3DXMatrixIdentity( &matWorld );
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    }
    KD3DDeviceState::PopRenderStateSet();*/
}
void GCSquarePeople::SetEmoticon( KGCEmoticonInstance* pEmoticonInstance_ )
{
	m_pEmoticon = pEmoticonInstance_;
	if ( m_pEmoticon )
	{
		m_pEmoticon->SetIsRight( GetIsRight() );
		m_pEmoticon->SetPos( &(D3DXVECTOR2)GetPos() );
	}
}
void GCSquarePeople::DownJump()
{
    SetYSpeed( -0.01f );
    SetNoCheckContact( 40 );
}

void GCSquarePeople::MakeOneFrameSample( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kOneFrame )
{
    kOneFrame.eState = m_eState;
    kOneFrame.bIsRight = m_BasicData.bIsRight;
    kOneFrame.fX = m_BasicData.vPos.x;
    kOneFrame.fY = m_BasicData.vPos.y;
    kOneFrame.nFrame = 0;
    kOneFrame.fXSpeed = m_fXSpeed;
    kOneFrame.fYSpeed = m_fYSpeed;
}

void GCSquarePeople::ReadyToRender()
{
    //if( false == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //    return;

    m_posDesc.right = m_BasicData.bIsRight;
    D3DXMATRIX matbasic, matinv;
    Get_BasicMatrix( &matbasic, true, false );
    D3DXMatrixInverse( &matinv, NULL, &g_kCamera.m_matCamera );
    D3DXMatrixMultiply( &matbasic, &matbasic, &matinv );

    m_posDesc.body.x = matbasic._41;
    m_posDesc.body.y = matbasic._42;
    m_posDesc.body.z = matbasic._43;

    UpdatePositionDesc( m_posDesc, false, FORM_NORMAL );

    GCObject* pObj = m_pObject->GetGCObject();
    pObj->GetAngleBone( 8, &g_TempMatTranslate );
    D3DXMatrixMultiply( &g_TempMatTranslate, &g_TempMatTranslate, pObj->GetWorldMat() );    
    D3DXMatrixTranslation( &g_TempMatScale, 0.154994f, 0.785072f, -0.213087f );
    D3DXMatrixMultiply( &g_TempMatTranslate, &g_TempMatScale, &g_TempMatTranslate );
    SETVECTOR3( m_posDesc.HeadBombSpark, g_TempMatTranslate._41, g_TempMatTranslate._42, g_TempMatTranslate._43 );

	//zstaiji - ParticleEffect.stg 스크립트에 머리중심 오프셋 이펙트 설정하기 위해서 추가시작
	D3DXMATRIX MatTranslateHead;
	D3DXMATRIX MatScaleHead;

	pObj->GetAngleBone( 8, &MatTranslateHead );
	D3DXMatrixMultiply( &MatTranslateHead, &MatTranslateHead, pObj->GetWorldMat() );    
	D3DXMatrixTranslation( &MatScaleHead, 0.0f, 0.15f, 0.0f);

	D3DXMatrixMultiply( &MatTranslateHead, &MatScaleHead, &MatTranslateHead );
	SETVECTOR3( m_posDesc.HeadPos, MatTranslateHead._41, MatTranslateHead._42, MatTranslateHead._43 );
	//zstaiji - ParticleEffect.stg 스크립트에 머리중심 오프셋 이펙트 설정하기 위해서 추가 끝

    for( std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
    {
        g_AnimManager.FrameUpdateItemAnim( &(*j), &m_posDesc, 1 );     // > 0 이면 됨
    }
    UpdateOverlappedPair( m_mapOverlappedPair, m_ItemAnim, m_vecItemEff );
}

void GCSquarePeople::UpdateBonePosition()
{
    //if( false == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //    return;

    // 2007/2/15. iridology. cFrame Bound Check
    if( m_BasicData.cFrame < 0 )
        m_BasicData.cFrame = 0;
    else if( m_BasicData.cFrame > m_pObject->GetMotionPtr()->GetNum_Frame() - 1 )
        m_BasicData.cFrame = m_pObject->GetMotionPtr()->GetNum_Frame() - 1;

    ProcessPositionDescParam kParam( this );
    kParam.bLeftAtktoRight = false;
    if( Extra_Char_Num == CID_RYAN2 )
    {
        kParam.bLeftAtktoRight = true;
    }
    Get_BasicMatrix( &kParam.matWorld, false, false );
    ProcessPositionDesc( kParam );
}

void GCSquarePeople::MakeEffectLoadList()
{
    m_kLoadedState.m_vecEffectID.clear();
    //if( true == g_MyD3D->m_KGCOption.GetSquareItemEffect() )
    //{
    for( int i = 0; i < (int)m_kPeopleInfo.m_vecEquips.size(); ++i )
    {
        m_kLoadedState.m_vecEffectID.push_back( m_kPeopleInfo.m_vecEquips[i] / 10 );
    }

    // shmhlove, 퀵 슬롯에 수호의 반지가 있다면 이펙트를 띄워줄 수 있도록 이쪽에서 ID를 등록시켜주자!! 
    // 기존 SetItemEffect()함수내에서 이상한 조건으로 수반이펙을 띄우고 있었는데
    // 그 조건으로 인해 한명이 접속하면 총 7번의 중복 파티클을 띄웁니다.
    // 기존 코드는 주석처리로 남겨둡니다.
    std::vector<GCITEMID>::iterator vecIter = m_kPeopleInfo.m_vecQuickSlot.begin();
    for(; vecIter!=m_kPeopleInfo.m_vecQuickSlot.end(); ++vecIter)
    {
        int iDevID = *vecIter / 10;
        if( g_MyD3D->IsProtectRing( iDevID ) == true )
        {
            m_kLoadedState.m_vecEffectID.push_back( iDevID );
        }
    }
}

GCObject* GCSquarePeople::GetGCObject()
{
    return m_pObject->GetGCObject();
}

void GCSquarePeople::LoadOneEffect( int iItemID )
{
    std::vector<int> vecTemp;
    vecTemp.push_back( iItemID );
    ItemEffectParam kEffectParam( this, vecTemp );
    SetItemEffect( kEffectParam );
}

void GCSquarePeople::SetPlayerName()
{
	std::wstringstream strmPlayerName;
	strmPlayerName << g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str() << L" " << m_kPeopleInfo.m_dwLevel << L". ";
	strmPlayerName << m_kPeopleInfo.m_strNick;

	m_pObject->GetGCObject()->SetText(0,strmPlayerName.str());

}

void GCSquarePeople::OnLoadingComplete()
{
	SetPlayerName();
	m_pObject->GetGCObject()->Build();
	m_pObject->GetGCObject()->SetCartoon(CARTOON_NORMAL);

    SetRender( m_bRender );
    m_pObject->SetIsLive( true );
    m_pObject->SetFrame( 0 );
    m_pObject->FrameMove();
    SetPeopleState( GCSquarePeople::WAIT_PEOPLE );

	if( m_kPeopleInfo.m_dwGuildUID != 0 )
    {
		m_pGuildMarkTexture = SiKGCIconImageManager()->GetTexture(m_kPeopleInfo.m_strMarkName); 

        if( m_pGuildMarkTexture == NULL )
        {            
            SiKGCIconImageManager()->LoadIconImageFromHTTP(m_kPeopleInfo.m_strMarkName);	
            m_pGuildMarkTexture = SiKGCIconImageManager()->GetTexture(m_kPeopleInfo.m_strMarkName); 
        }

    }
}

void GCSquarePeople::ToggleEffect( bool bRender )
{
    g_pkPetAI->Show( m_vecPetAiID, bRender );
    for( std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
    {
        (*j).animInst->SetShow( NoForceEffect( (*j).positionType )&& bRender );
    }

	for ( std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i !=
        m_vecItemEff.end(); i++ )
	{
		for( int j = 0; j < (int)(*i).pSeq.size(); j++ )
		{
			(*i).pSeq[j]->SetShow( NoForceEffect( (*i).emitPositionType[ j / (*i).sequenceName.size() ] ) && bRender );
		}
	}
}

CParticleEventSeqPTR GCSquarePeople::AddTraceParticle( char* strSequence, float fOffsetX, float fOffsetY )
{
	CParticleEventSeqPTR pTemp;
	pTemp = g_ParticleManager->CreateSequence( strSequence, m_BasicData.vPos.x - 1.0f,
		m_BasicData.vPos.y - 0.48f ,
		0.5f );
	if( pTemp != NULL )
	{
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = -fOffsetX;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &m_BasicData.bIsRight;
		pTemp->SetDirectInfo( &stDirect );
		pTemp->SetShow( true );

		TRACE_INFO stTrace;
		stTrace.m_pvPos = ComputePosDesc( m_posDesc, EPT_BODY_CENTER );
		stTrace.m_fParticleTraceTime = pTemp->GetKillTime();
		pTemp->SetTrace( &stTrace );
	}
	return pTemp;
}

void GCSquarePeople::DrawGCClubMark()
{
    if( m_pGCClubTexture != NULL )
    {
        D3DXMATRIX mat1,mat2,matBasic;
        Get_BasicMatrix( &matBasic, true, true );

        D3DXVECTOR3 vPos;
        D3DXMatrixTranslation( &mat2, -.15f, -.1f, 0);
        D3DXMatrixMultiply( &mat1, &matBasic, &mat2 );

        ::D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat1 );

        // offset
        vPos.x += 0.15f;
        vPos.y += 0.13f;

        // NickName Lenght
        std::wstringstream strmPlayerName;
        strmPlayerName << g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str() << L" " << m_kPeopleInfo.m_dwLevel << L". ";
        strmPlayerName << m_kPeopleInfo.m_strNick;
        float fNickWidth = (float)g_pkFontMgr->GetWidth( strmPlayerName.str().c_str() );

        // world coordinates -> screen coordinates
        float fScaleX = g_pGCDeviceManager->GetWindowScaleX();
        float fScaleY = g_pGCDeviceManager->GetWindowScaleY();
        float fScreenX = (int( ( vPos.x + 1.0f ) * 410.0f * GC_SCREEN_DIV_WIDTH ) + 0.5f);
        float fScreenY = (int( ( -vPos.y + 0.75f ) * 415.0f * GC_SCREEN_DIV_WIDTH ) + 0.5f);
        fScreenX *= fScaleX;
        fScreenY *= fScaleY;

        //fScreenX -= (fNickWidth / 0.75f ) * GC_SCREEN_DIV_WIDTH;
        fScreenX -= (fNickWidth / 0.95f ) * GC_SCREEN_DIV_WIDTH;

        g_pGCDeviceManager2->DrawInScreen(m_pGCClubTexture, fScreenX, fScreenY, fScreenX + (50.0f * fScaleX), fScreenY + (50.0f * fScaleY),  0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
    }
}

void GCSquarePeople::DrawGuildMark()
{
	if( m_kPeopleInfo.m_dwGuildUID == 0 )
		return;

	//#if defined( _DEBUG )
	//	KLuaManager luaMgr;
	//	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
	//
	//	float y = 0.0f;
	//	float y2 = 0.0f;
	//	LUA_GET_VALUE_DEF( "y", y, 0.0f );
	//	LUA_GET_VALUE_DEF( "y2", y2, 0.0f );
	//#endif

	D3DXMATRIX mat1,mat2,matBasic;
	Get_BasicMatrix( &matBasic, true, true );

	D3DXVECTOR3 vNickNamePos;
	D3DXMatrixTranslation( &mat2, -.15f, -.1f, 0);
	D3DXMatrixMultiply( &mat1, &matBasic, &mat2 );


	::D3DXVec3TransformCoord( &vNickNamePos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat1 );
	// offset
	vNickNamePos.x += 0.15f;
	vNickNamePos.y += 0.055f;

	// world coordinates -> screen coordinates
	float fScreenX = int( ( vNickNamePos.x + 1.0f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
	float fScreenY = int( ( -vNickNamePos.y + 0.75f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;

	fScreenX*=g_pGCDeviceManager->GetWindowScaleX();
	fScreenY*=g_pGCDeviceManager->GetWindowScaleY();
	g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY+(int)(SiKGCMultipleLanguages()->GetDefualtFontSize()*2.5f), m_kPeopleInfo.m_strGuildName.c_str(), 
		0xFF00FF00, NULL, DT_CENTER | DT_TOP );

	float fGuildNameWidth = (float)g_pkFontMgr->GetWidth( m_kPeopleInfo.m_strGuildName.c_str() );

	if (m_pGuildMarkTexture)
	{
		g_pGCDeviceManager2->DrawInScreen(m_pGuildMarkTexture,
			fScreenX - (fGuildNameWidth+(SiKGCMultipleLanguages()->GetDefualtFontSize()*2)), fScreenY+(SiKGCMultipleLanguages()->GetDefualtFontSize()*2), 
			fScreenX + 25.0f - (fGuildNameWidth+(SiKGCMultipleLanguages()->GetDefualtFontSize()*2)), fScreenY+(SiKGCMultipleLanguages()->GetDefualtFontSize()*2)+ 25.0f,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);

	}
}

bool GCSquarePeople::NoForceEffect(  EMIT_POSITION_TYPE positionType )
{
    if( Extra_Char_Num == CID_RONAN3 || Extra_Char_Num == CID_LAS4 )
        return true;

    bool bWeaponEffect = true;

    if( Extra_Char_Num == CID_SIEG3 )
    {
        bWeaponEffect = false;
    }

    switch ( positionType )
    {
    case EPT_SWORD_TIP:
    case EPT_SWORD_BODY:
    case EPT_SWORD_CENTER:
    case EPT_LEFT_SWORD_TIP:
    case EPT_LEFT_SWORD_BODY:
    case EPT_LEFT_SWORD_CENTER:
        return bWeaponEffect;
    default:
        return true;
    }
}

bool GCSquarePeople::NoForceEffect( std::vector<EMIT_POSITION_TYPE>& positionType )
{
	for ( int i = 0; i < (int)positionType.size(); i++ )
	{
		if( !NoForceEffect( positionType[i] ) )
			return false;
		
	}
	return true;
}

bool GCSquarePeople::IsRemovePartsItem( REMOVEPARTS removeParts)
{
    std::vector<GCITEMID>::iterator vecEquipIter = m_kPeopleInfo.m_vecEquips.begin();
    for ( ; vecEquipIter != m_kPeopleInfo.m_vecEquips.end() ; vecEquipIter++ )
    {
        if (g_pItemMgr->IsRemovePartsItem( (*vecEquipIter) / 10 , removeParts ))
        {
            return true;
        }
    }
    return false;
}