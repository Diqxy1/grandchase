#include "stdafx.h"
#include ".\gcsquare3dobj.h"

#include "GCCameraOrtho.h"
//
#include "KGC3DObject.h"


//

#include "MapSystem/KGCLayerPart.h"
KGCSquare3DObj::KGCSquare3DObj(void)
: m_pObject( NULL )
, m_iCurMotion( 0 )
, m_iFrameCount( 0 )
, m_bNextMotion( false )
, m_fSpeechTimter(0.0f)
, m_egcCharType(GC_CHAR_INVALID)
, m_bUseNickNameString ( false )
, m_dwNickNameStringID( 0 )
, m_fSpeechOffsetX(0.0f)
, m_fSpeechOffsetY(0.0f)
, m_iSpeechDistance(50)
, m_eCartoonTexture(CARTOON_NORMAL)
, m_fNicknameOffsetX(0.0f)
, m_fNicknameOffsetY(0.0f)
{
    m_sMeshInfo.Init();
    m_vecStringID.clear();
    m_vecSpeechStr.clear();
	m_vecTraceParticleInfo.clear();
}

KGCSquare3DObj::~KGCSquare3DObj(void)
{
    if( m_pObject != NULL )
    {
        EndAfterImage();
        m_pObject->Release();
        SAFE_DELETE( m_pObject );
    }

	std::vector< TRACE_PARTICLE_INFO* >::iterator itor;
	for(itor = m_vecTraceParticleInfo.begin(); itor != m_vecTraceParticleInfo.end(); ++itor )
	{
		SAFE_DELETE( (*itor) );
	}

	m_vecTraceParticleInfo.clear();

}

void KGCSquare3DObj::Render()
{
    DrawNickName();

    if( true == g_kGlobalValue.m_bRenderSquarePeopleData )
    {
        DrawDumpData();
        DrawBodyRect();
    }
}

void KGCSquare3DObj::FrameMove()
{
	D3DXVECTOR2 temp = m_pObject->GetMotionPos();
	D3DXVECTOR3 temp1(temp.x, temp.y,m_BasicData.vPos.z + 1.0f);

	if( GetSubType() == SOST_NORMAL_NPC )
	{
		temp1.x += m_pObject->GetLocalPos().x;
	}
	m_pObject->SetLocalPos( temp1 );	

    ++m_iFrameCount;

    std::vector< std::pair< int, int > >::iterator vit;
    for( vit = m_sMeshInfo.m_vecAnimInfo.begin(); vit != m_sMeshInfo.m_vecAnimInfo.end(); ++vit )
    {
        std::pair< int, int >& pairAnim = (*vit);
        if ( m_iCurMotion == pairAnim.second && m_iFrameCount >= ( pairAnim.first * m_pObject->GetMotionPtr()->GetNum_Frame() - 1 ) )
        {
            m_bNextMotion = true;
        }
        else if ( true == m_bNextMotion )
        {
            m_iCurMotion = pairAnim.second;
            m_pObject->SetMotionFromIndex( m_iCurMotion );
            m_iFrameCount = 0;
            m_bNextMotion = false;
        }
    }

    m_pObject->FrameMove();
    CreateMeshChildParticle();
	FrameMoveTraceParticle();
}

bool KGCSquare3DObj::Load()
{
    CreateObject(m_sMeshInfo.m_iTechnique);

    m_pObject->SetCartoon( m_eCartoonTexture );

	for( int i = 0; i < (int)m_sMeshInfo.m_vecStrMesh.size(); ++i )
	{
		m_pObject->AddMesh( m_sMeshInfo.m_vecStrMesh[i], m_sMeshInfo.m_vecStrTex[i] );
	}
    for( int i = 0; i < (int)m_sMeshInfo.m_vecStrMotion.size(); ++i )
    {
        m_pObject->AddMotion( m_sMeshInfo.m_vecStrMotion[i] );
    }

    m_pObject->SetMotionFromIndex( 0 );
    m_iCurMotion = 0;
    m_pObject->SetScale( m_sMeshInfo.m_fScale );
    m_pObject->SetPos( D3DXVECTOR2(m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.48f) );

    if( -1 < m_sMeshInfo.m_iAfterImage &&
        !g_MyD3D->m_kAfterImageRenderer.IsRegisterObject( m_pObject->GetGCObject()) )
        g_MyD3D->m_kAfterImageRenderer.RegisterObject( m_pObject->GetGCObject(), m_sMeshInfo.m_iAfterImage );

    int iSpeechSize = m_vecStringID.size();
    for ( int i = 0; i < iSpeechSize; ++i )
    {
        m_vecSpeechStr.push_back( g_pkStrLoader->GetString( m_vecStringID[i] ) );
    }

	if(m_bUseNickNameString)
	{
		m_strNick = g_pkStrLoader->GetString( m_dwNickNameStringID );
	}else
	{
		m_strNick = g_pkStrLoader->GetString( STR_ID_MINIGAME_NPC_NICK );
	}

    return true;
}

bool KGCSquare3DObj::TimeShareLoading()
{
    return true;
}

void KGCSquare3DObj::CreateObject( int iTech_ )
{
    EndAfterImage();

    SAFE_DELETE( m_pObject );
    m_pObject = new KGC3DObject();
	m_pObject->Init(iTech_);
    m_pObject->SetRender( true );
    m_pObject->SetIsLive( true );
    m_pObject->SetScale( CHAR_SCALE_RATIO );
    m_pObject->IsRight( m_BasicData.bIsRight );
    m_pObject->SetModelZAxis( KGCLayerPart::ms_fZOrder[7] );
}

bool KGCSquare3DObj::Picking( const D3DXVECTOR2& vMousePos , const int iMouseBtn )
{
    if ( iMouseBtn == KInput::MBLEFT )
    {
        const float fWidth = m_vRect.x * 0.5f;
        if( vMousePos.x >= m_BasicData.vPos.x - fWidth && vMousePos.x <= m_BasicData.vPos.x + fWidth )
        {
            if( vMousePos.y >= m_BasicData.vPos.y && vMousePos.y <= m_BasicData.vPos.y + m_vRect.y )
            {
                return true;
            }
        }
    }

    return false;
}

void KGCSquare3DObj::SetPosition( const D3DXVECTOR3& vPos )
{
    m_BasicData.vPos.x = vPos.x;
    m_BasicData.vPos.y = vPos.y;
	m_BasicData.vPos.z = vPos.z;
}

void KGCSquare3DObj::Get_BasicMatrix( D3DXMATRIX* pMat )
{
    D3DXMATRIX mat2, mat3;
    if( m_BasicData.bIsRight )
    {
        D3DXMatrixScaling ( pMat, m_sMeshInfo.m_fScale, m_sMeshInfo.m_fScale, m_sMeshInfo.m_fScale );
    }
    else
    {
        D3DXMatrixScaling ( &mat2, m_sMeshInfo.m_fScale, m_sMeshInfo.m_fScale, m_sMeshInfo.m_fScale );
        D3DXMatrixRotationY( &mat3, 3.1415f);
        D3DXMatrixMultiply( pMat, &mat2, &mat3 );
    }
    D3DXMatrixTranslation( &mat2, m_BasicData.vPos.x - 1.0f, m_BasicData.vPos.y - 0.48f, 0.5f);
    D3DXMatrixMultiply( &mat3, pMat, &mat2 );
    D3DXMatrixMultiply( pMat, &mat3, &g_kCamera.m_matCamera );
}

void KGCSquare3DObj::DrawDumpData()
{
    D3DXVECTOR3 vPos;
    D3DXMATRIX matBasic;
    Get_BasicMatrix( &matBasic );
    D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

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
        stm << L"motion : " << m_BasicData.usMotion << L" frame : " << (int)m_BasicData.cFrame;
        Color = 0xFFFF0000;
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_CENTER | DT_TOP );
        fScreenY += 16.0f;
    }
    g_pGCDeviceManager2->PopState();
}

void KGCSquare3DObj::DrawBodyRect()
{
	g_pGCDeviceManager2->PushState();    
	{
		g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        //static GCDeviceTexture* pNullTexture = g_pGCDeviceManager2->CreateTexture("임시로 사용하는 널 텍스쳐");
        //g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_kCamera.m_matCamera );
        //g_pGCDeviceManager2->SetDeviceTextureWithLoad(pNullTexture);

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
    }
    g_pGCDeviceManager2->PopState();
}

void KGCSquare3DObj::SetEdgeColor( D3DXVECTOR4 vColor )
{
    m_pObject->GetGCObject()->SetOutlineColor( vColor );
}

void KGCSquare3DObj::ReloadNPCSpeech( void )
{
    m_vecSpeechStr.clear();
    int iSpeechSize = m_vecStringID.size();
    for ( int i = 0; i < iSpeechSize; ++i )
    {
        m_vecSpeechStr.push_back( g_pkStrLoader->GetString( m_vecStringID[i] ) );
    }
}

void KGCSquare3DObj::DrawNickName( void )
{
	if( !m_bUseNickNameString )
		return;

    D3DXMATRIX mat1,mat2,matBasic;
    Get_BasicMatrix( &matBasic );

    D3DXVECTOR3 vNickNamePos;
    D3DXMatrixTranslation( &mat2, -.15f, -.1f, 0);
    D3DXMatrixMultiply( &mat1, &matBasic, &mat2 );

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        ::D3DXVec3TransformCoord( &vNickNamePos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat1 );
        // offset
		vNickNamePos.x += m_fNicknameOffsetX;
		vNickNamePos.y += m_fNicknameOffsetY;

        // world coordinates -> screen coordinates
        float fScreenX = int( ( vNickNamePos.x + 1.0f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
        float fScreenY = int( ( -vNickNamePos.y + 0.75f ) * ( GC_SCREEN_FLOAT_WIDTH / 2.0f ) ) + 0.5f;
		fScreenX*=g_pGCDeviceManager->GetWindowScaleX();
		fScreenY*=g_pGCDeviceManager->GetWindowScaleY();

        // draw shadow
        g_pkFontMgrOutline->OutTextXY( (int)fScreenX, (int)fScreenY, m_strNick.c_str(), 0xFF00FF00, NULL, DT_CENTER | DT_TOP );
    }
    g_pGCDeviceManager2->PopState();
}

D3DXVECTOR3 KGCSquare3DObj::GetObjectLocalPos(void)
{ 
	return m_pObject->GetLocalPos(); 
}

void KGCSquare3DObj::FrameMoveTraceParticle()
{
	std::vector< TRACE_PARTICLE_INFO* >::iterator itor;

	for(itor = m_vecTraceParticleInfo.begin(); itor != m_vecTraceParticleInfo.end(); ++itor)
	{
		if( (*itor) && (*itor)->m_pParticleSequence)
		{		
			D3DXMATRIX matBone;
			m_pObject->GetGCObject()->GetAngleBone( (*itor)->m_iBoneIndex, &matBone );
			D3DXMatrixMultiply(&matBone, &matBone, m_pObject->GetGCObject()->GetWorldMat());
			(*itor)->m_vecTracePos.x = matBone._41 + (*itor)->m_fXOffset;
			(*itor)->m_vecTracePos.y = matBone._42 + (*itor)->m_fYOffset;

			TRACE_INFO stTrace;
			stTrace.m_pvPos = &(*itor)->m_vecTracePos;
			stTrace.m_fParticleTraceTime = 5.0f;
			(*itor)->m_pParticleSequence->SetTrace( &stTrace );
		}
	}
}

void KGCSquare3DObj::CreateMeshChildParticle( void )
{
    D3DXMATRIX matBone;
    std::vector< MESH_CHILD_PARTICLE >::iterator particleIter;
    for( particleIter = m_sMeshInfo.m_vecMeshChildParticle.begin() ; particleIter != m_sMeshInfo.m_vecMeshChildParticle.end() ; ++particleIter ) { 
        if( particleIter->m_iMontionIndex == m_iCurMotion && particleIter->m_iCreateFrame == m_iFrameCount ) { // 현재 모션, 프레임 확인 
            if( particleIter->m_iBoneIndex != -1 ) {
                m_pObject->GetGCObject()->SetMotionMatrix( true, NULL, false, true );
                m_pObject->GetGCObject()->GetAngleBone( particleIter->m_iBoneIndex, &matBone );
                matBone *= *( m_pObject->GetGCObject()->GetWorldMat() );
                if( bIsRight ) { 
                    particleIter->m_vBonePos.x = matBone._41 - particleIter->m_fOffSetX;
                    particleIter->m_vBonePos.y = matBone._42 + particleIter->m_fOffSetY;
                    particleIter->m_vBonePos.z = matBone._43;
                }
                else { 
                    particleIter->m_vBonePos.x = matBone._41 + particleIter->m_fOffSetX;
                    particleIter->m_vBonePos.y = matBone._42 + particleIter->m_fOffSetY;
                    particleIter->m_vBonePos.z = matBone._43;
                }
            }
            if( bIsRight ) { 
                CreateSequence( particleIter->m_strParticle.c_str(), particleIter->m_vBonePos.x + particleIter->m_fOffSetX, particleIter->m_vBonePos.y + particleIter->m_fOffSetY, 0.5f );
            }
            else { 
                CreateSequence( particleIter->m_strParticle.c_str(), particleIter->m_vBonePos.x - particleIter->m_fOffSetX, particleIter->m_vBonePos.y + particleIter->m_fOffSetY, 0.5f );
            }
        }
    }
}

CParticleEventSeqPTR KGCSquare3DObj::CreateSequence( const std::string& name, float x, float y, float z )
{
    CParticleEventSeqPTR seq = g_ParticleManager->CreateSequence(name,x,y,z);
    return seq;
}

void KGCSquare3DObj::EndAfterImage()
{
    if( m_pObject )
    {
        if( g_MyD3D->m_kAfterImageRenderer.IsRegisterObject(m_pObject->GetGCObject()) )
            g_MyD3D->m_kAfterImageRenderer.UnRegisterObject( m_pObject->GetGCObject() );
    }
}