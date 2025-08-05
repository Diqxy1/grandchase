#include "stdafx.h"
#include "KGCObj.h"
#include "GCObjectAndMotion.h"

KGCObj::KGCObj()
{
	y_Ex = 0.0f;
	vRenderPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vParticleToDamageMagicNumber = D3DXVECTOR2( 0.0f, 0.0f );
}


void KGCObj::RegisterLuabind()
{
	lua_tinker::class_add<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "KGCObj" );
	lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "ToggleExtraMesh", 					&KGCObj::ToggleExtraMesh  );
	lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "ToggleExtraMeshAll", 				&KGCObj::ToggleExtraMeshAll  );
	lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "ToggleBaseMesh", 					&KGCObj::ToggleBaseMesh  );
	lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "SetChildMeshMotion",				&KGCObj::SetChildMeshMotion );
    lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "ClearChildMeshOneTimeMotion",	&KGCObj::ClearChildMeshOneTimeMotion );
    lua_tinker::class_def<KGCObj>( KGCLuabinder::getInstance().GetLuaState(), "AddParticleToChildMeshBone",	&KGCObj::AddParticleToChildMeshBone );

}




void KGCObj::SetChildMeshMotion( char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame, bool bNotOneTime, int iLoopMode )
{
	KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
	if ( pChild )
	{
		std::string strMotionFile;
		strMotionFile = strMotion;
        if( false == strstr(strMotion, ".frm") )
		strMotionFile = strMotionFile + ".frm";
		pChild->AddMotion( strMotionFile, !bNotOneTime, bReserveNext );

		if( bForce )
			pChild->NextMotion();

        if( bResetFrame )
            pChild->ReSetMotion();

        pChild->SetLoopMode( iLoopMode );
	}
}

void KGCObj::SetChildMeshMotion_Front( char* strChlid, char* strMotion )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
    if ( pChild )
    {
        std::string strMotionFile;
        strMotionFile = strMotion;
        if( false == strstr(strMotion, ".frm") )
            strMotionFile = strMotionFile + ".frm";
        pChild->AddMotion_Front( strMotionFile, true );
    }
}

void KGCObj::SetChildMeshFrame( char* strChlid, char* strMotion, int iFrame )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
    if ( pChild )
    {
        pChild->SetRandomlyFrame( strMotion, iFrame );
    }
}

void KGCObj::SetChildMeshCurrentFrame( char* strChlid, int iFrame )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
    if ( pChild )
    {
        pChild->SetCurrentFrame( iFrame );
    }
}

int KGCObj::GetChildMeshCurrentFrame( char* strChlid )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
    if ( pChild )
    {
        return pChild->GetCurrentFrame();
    }
    return -1;
}

const char* KGCObj::GetChildMeshCurrentMotionName( char* strChild )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChild );
    if ( pChild )
    {
        return pChild->GetCurrentMotion();
    }
    return "";
}

bool KGCObj::IsToggleExtraMesh( const char* strID )
{
    if( m_pObject )
        return m_pObject->IsToggleExtraMesh( strID );
    return false;
}

void KGCObj::ToggleMeshAll( bool bToggle_ )
{
    if( m_pObject )
        m_pObject->ToggleMeshAll( bToggle_ );    
}

void KGCObj::ToggleExtraMesh( const char* strID, bool bToggle_ , int iShaderTech)
{
	if( m_pObject )
		m_pObject->ToggleExtraMesh( strID, bToggle_ , iShaderTech);    
}

void KGCObj::ToggleExtraMeshAll( bool bToggle_ )
{
	if( m_pObject )
		m_pObject->ToggleExtraMeshAll( bToggle_ );    
}

void KGCObj::ToggleBaseMesh( bool bToggle_ )
{
	if( m_pObject )
		m_pObject->ToggleBaseMesh( bToggle_ );    
}

void KGCObj::ToggleSecondBaseMesh( const char* strID, bool bToggle_ )
{
    if( m_pObject )
        m_pObject->ToggleSecondBaseMesh( strID, bToggle_ );    
}

void KGCObj::ToggleSecondBaseMeshAll( bool bToggle_ )
{
    if( m_pObject )
        m_pObject->ToggleSecondBaseMeshAll( bToggle_ );    
}

void KGCObj::ClearChildMeshOneTimeMotion( char* strChlid )
{
	KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
	if ( pChild )
	{
        for ( std::vector<ParticlePosDesc*>::iterator it = pChild->m_vecPosDesc.begin(); it != pChild->m_vecPosDesc.end(); ++it )
        {        
            g_ParticleManager->DeleteSequence( (*it)->pParticle );
        }
		pChild->m_vecPosDesc.clear();
		pChild->ClearOneTimeMotion();
	}
}


CParticleEventSeqPTR KGCObj::AddParticleToChildMeshBone( char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime /*= 0.5f*/, float offset_x /*= 0.0f*/, float offset_y /*= 0.0f*/, bool bNoDirection /*= false */ )
{
    KGCObjectAndMotion* pChild =  m_pObject->GetChildObject( strChlid );
    if ( pChild )
    {
        D3DXMATRIX matPartcle;
        const D3DXMATRIX *matWorld = pChild->GetWorldMat();
        pChild->GetAngleBone( iBoneIndex_, &matPartcle );
        D3DXMatrixMultiply( &matPartcle, &matPartcle, matWorld );

        CParticleEventSeqPTR pTemp;
        pTemp = g_ParticleManager->CreateSequence( strSeqeunce, matPartcle._41,matPartcle._42,matPartcle._43 );

        if( pTemp != NULL )
        {
            TRACE_INFO stTrace;
            ParticlePosDesc* pDesc = new ParticlePosDesc();
            if( pTemp->GetKillNum() == 0 && pTemp->GetKillTime() == 0 ) 
            {
                pDesc->pParticle = pTemp;
            }                
            stTrace.m_fParticleTraceTime = fTraceTime;
            stTrace.m_pvPos =  &pDesc->m_vPos;
            stTrace.m_fXOffset = offset_x;
            stTrace.m_fYOffset = offset_y;
            
            pDesc->m_iBoneIndex = iBoneIndex_;            
            pChild->m_vecPosDesc.push_back( pDesc );
            pTemp->SetTrace( &stTrace );           

            // 방향성 세팅
            DIRECTION_INFO stDirect;
            stDirect.m_bDirVelocity = true;
            if ( false == bNoDirection )
                stDirect.m_pbIsRight = &pChild->m_bIsRight;
            pTemp->SetDirectInfo( &stDirect );
        }

        return pTemp;
    }

    return NULL;
}

void KGCObj::LoadExtraMesh( std::vector< EXTRA_MESH > vecMesh )
{
    if ( !m_pObject )
        return;

	for( std::vector< EXTRA_MESH >::iterator i = vecMesh.begin(); i != vecMesh.end(); i++ )
	{
		if( (*i).iBoneId == -1 )
		{
			GCMeshObject* pMesh;
			pMesh = m_pObject->AddMesh( (*i).strMeshName ,(*i).strTextureName, -1,(EGCCartoonTexture) (*i).dwCartoon, (*i).strID, (*i).dwTechnique );
			
			if( pMesh && !(*i).strID.empty() )
			{
				pMesh->m_bExtraMeshObject = true;
				pMesh->m_bRender =false;
			}
		}
		else
		{
			KGCObjectAndMotion* test = new KGCObjectAndMotion();    
			test->m_strName = (*i).strID;
			test->AddMesh( (*i).strMeshName ,(*i).strTextureName, -1,(EGCCartoonTexture) (*i).dwCartoon, (*i).strID, (*i).dwTechnique );
			test->AddMotion( (*i).strMotionName );
			test->FrameMove();    
			test->SetRender( false );
			test->SetPosition( (*i).vOptionalPos );
			test->SetScale( D3DXVECTOR3( 0.35f, 0.35f, 0.35f ) );
			test->m_bIsExtraObject = true;

			D3DXVECTOR3 v(0.0f,0.0f,0.0f);
			m_pObject->AddChildObject( test, (*i).iBoneId, ( v != (*i).vOptionalPos ) == TRUE );
		}
	}
}


CParticleEventSeqPTR KGCObj::CreateParticle( MSG_PARTICE_CREATE_INFO& msgParticle )
{
    if ( NULL == m_pObject )
        return NULL;
    
	MSG_PARTICE_CREATE_INFO& ptMsg = msgParticle;

	float fY_Ex = 0.0f;
	if( msgParticle.bApplyMotionY )
		fY_Ex = y_Ex;

	CParticleEventSeqPTR pSequence = NULL;	
	if( *(ptMsg.strSeqname.end() - 1) == '/' )
	{
		ptMsg.strSeqname.erase(ptMsg.strSeqname.end()-1);
		std::string strParticleName = ptMsg.strSeqname;
		if ( !bIsRight )
		{			
			strParticleName += "_R";
			pSequence = g_ParticleManager->CreateSequence( strParticleName, vRenderPos.x + ptMsg.vPos.x, vRenderPos.y + fY_Ex + ptMsg.vPos.y, 0.5f );
		}
	}
	if ( !pSequence )
	{
		pSequence = g_ParticleManager->CreateSequence( ptMsg.strSeqname, vRenderPos.x + ptMsg.vPos.x, vRenderPos.y + fY_Ex + ptMsg.vPos.y, 0.5f );
	}
	

	if( pSequence )
	{
		if( ptMsg.bDirecitonal )
		{
			DIRECTION_INFO stDirect;
			stDirect.m_bDirVelocity = true;
			stDirect.m_fDirPosX = -ptMsg.fOffsetX;
			stDirect.m_pbIsRight = &bIsRight;

			pSequence->SetDirectInfo( &stDirect );
			pSequence->SetShow( true );
		}

		if( ptMsg.iParticleNum != 0 )
		{
			CMinMax<float> emitRate(1000.0f,1000.0f);
			pSequence->SetEmitRate( emitRate );
			pSequence->SetKillNum( ptMsg.iParticleNum );
		}

		{
			TRACE_INFO stTrace;

			if( ptMsg.fTraceTime == 0.0f )
				stTrace.m_fParticleTraceTime = pSequence->GetKillTime();
			else
				stTrace.m_fParticleTraceTime = ptMsg.fTraceTime;

			stTrace.m_fXOffset = ptMsg.vPos.x;
			stTrace.m_fYOffset = ptMsg.vPos.y;
			if( ptMsg.pTracePos )
			{			
				stTrace.m_pvPos = ptMsg.pTracePos;
				pSequence->SetShow( &m_bRender );

			}
			else if( ptMsg.iBoneID != -1 )
			{
				ParticlePosDesc* pDesc = new ParticlePosDesc();			
				if( pSequence->GetKillNum() == 0 && pSequence->GetKillTime() == 0 ) 
				{
					pDesc->pParticle = pSequence;
				}                
				stTrace.m_pvPos =  &pDesc->m_vPos;			
				pDesc->m_iBoneIndex = ptMsg.iBoneID;
				pDesc->m_iLeftSideLayer = ptMsg.iLeftSideLayer;
				pDesc->m_iRightSideLayer = ptMsg.iRightSideLayer;

				pSequence->SetShow( &m_bRender );
				m_pObject->m_vecPosDesc.push_back( pDesc );

				if( stTrace.m_fParticleTraceTime < 0.0f )
				{
					D3DXMATRIX matPartcle;
					const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();

					m_pObject->GetAngleBone( ptMsg.iBoneID, &matPartcle );
					D3DXMatrixMultiply( &matPartcle, &matPartcle, matWorld );

					pSequence->SetPosition( D3DXVECTOR3(matPartcle._41,matPartcle._42,matPartcle._43)  );
				}
			}
			pSequence->SetTrace( &stTrace );
		}
		
		if( ptMsg.bUserAngle || ptMsg.bBlackHole )
		{
			float fAngle = 0.0f;

			if( ptMsg.bUserAngle )
			{
				fAngle = Body_Angle;
				if( !bIsRight )
					fAngle = D3DX_PI - fAngle;
			}

			D3DXVECTOR3 vOffsetPos( ptMsg.fOffsetX, 0.f, 0.5f );
			D3DXVECTOR3 vDestPos;
			D3DXVECTOR3 vCahrPos((vRenderPos.x + ptMsg.vPos.x), (vRenderPos.y + fY_Ex + ptMsg.vPos.y), 0.5f );

			D3DXMATRIX matRot;
			D3DXMatrixIdentity( &matRot );
			D3DXMatrixRotationZ( &matRot, fAngle );    
			D3DXVec3TransformCoord( &vDestPos, &vOffsetPos, &matRot );
			vDestPos += vCahrPos;

			pSequence->SetAdditinalZRot( fAngle );
			pSequence->SetBeginPos( vDestPos );

			if( ptMsg.bBlackHole )
			{
				pSequence->SetInternalBlackHolePos( vDestPos );
			}
		}
	}

	return pSequence;
}

void KGCObj::ParticleQueueProcess( VecParticleMsg& particleMsg )
{
    PROFILE_SET( "KGCObj::ParticleQueueProcess" );

	for( VecParticleMsg::iterator it = particleMsg.begin();it != particleMsg.end(); it++ )
	{	
		CreateParticle( *it );
	}
	particleMsg.clear();    
}

CParticleEventSeqPTR KGCObj::CreateParticleProcess(MSG_PARTICE_CREATE_INFO *particle)
{
    return CreateParticle( *particle );
}

void KGCObj::OnFrameMove()
{
	if ( m_pObject )
	{
		m_pObject->m_bIsRight = bIsRight;
	}	
}

bool KGCObj::IsToggleSecondBaseMesh( const char* strID_ )
{
    if( m_pObject )
        return m_pObject->IsToggleSecondBaseMesh( strID_ );
    return false;
}
