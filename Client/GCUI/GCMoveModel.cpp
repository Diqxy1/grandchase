#include "stdafx.h"
#include "GCMoveModel.h"
#include "./GCUtil/GCFileMoniter.h"

static bool m_bReloadModelData = false;
void ReLoadModelData()
{
    KGCGlobal::LoadMoveModelData();
    m_bReloadModelData = true;
}

IMPLEMENT_CLASSNAME( KGCMoveModel );
IMPLEMENT_WND_FACTORY( KGCMoveModel );
IMPLEMENT_WND_FACTORY_NAME( KGCMoveModel, "gc_move_model" );

KGCMoveModel::KGCMoveModel( void )
: m_pModelObject(NULL),
m_pModelMotion(NULL),
m_3A0(0),
m_cFrame(0),
m_iNowModelID(-1),
m_3B8(0),
m_3BC(0)
{
}

KGCMoveModel::~KGCMoveModel( void )
{
}

void KGCMoveModel::ActionPerformed( const KActionEvent& event )
{

}

void KGCMoveModel::OnCreate( void )
{
	D3DXMATRIX matLook, matScale, matTran;
	::D3DXMatrixOrthoLH( &matLook, float( GetWidth() ) / float( GetHeight() ), 1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 1.0f );
	::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.1f );
	m_matProject = matLook * matScale * matTran;

	D3DXMATRIX matLook2, matScale2, matTran2;
	::D3DXMatrixOrthoLH( &matLook2, 1.0f, 1.0f, 1.0f, 100.0f );
	::D3DXMatrixScaling( &matScale2, 1.0f, 1.0f, 1.0f );
	::D3DXMatrixTranslation( &matTran2, 0.0f, 0.0f, 0.1f );
	m_matOrtho = matLook2 * matTran2 * matScale2;

    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( "MoveModelInfo.lua" ) , ReLoadModelData );

    InitNPCData();
}

void KGCMoveModel::OnDestroy( void )
{
    if( m_pModelObject )
    {
        g_RenderManager->RemoveObject( m_pModelObject );
        SAFE_DELETE( m_pModelObject );
    }
}

void KGCMoveModel::SetModelID( int iModel)
{
	if(iModel != -1 && iModel != 0)
		m_iNowModelID = iModel;
	else
		m_iNowModelID = 4;
}

void KGCMoveModel::SetModelImage()
{
    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    D3DXVECTOR2 vLocalPos( GetFixedWindowLocalPos());
    m_viewport.X = static_cast<DWORD>(( vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(GetWidth()) * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Height = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(GetHeight()) * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;


    //SetModelID에서 제대로 NPC 번호를 받아와야 셋팅 가능.
    if( m_iNowModelID == -1 || m_iNowModelID == 0)
	{
		m_iNowModelID = 4; 
	}

    if ( g_kGlobalValue.GetMoveModelData( m_iNowModelID, m_MoveModelData ) )
    {
        InitNPCData();
        m_iNowModelID = m_MoveModelData.m_NPCID;
        if( m_pModelObject )
        {
            g_RenderManager->RemoveObject( m_pModelObject );
            SAFE_DELETE( m_pModelObject );
        }

        m_pModelObject = new GCObject();

		if( m_MoveModelData.m_strMeshName != "NULL" && m_MoveModelData.m_strTexName != "NULL" )
			m_pModelObject->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( m_MoveModelData.m_strMeshName ), g_pGCDeviceManager2->CreateTexture( m_MoveModelData.m_strTexName ) );

        int index = 0;
        std::vector< MoveModelMotion >::iterator vit;
        for ( vit = m_MoveModelData.m_vecMotionList.begin(); vit != m_MoveModelData.m_vecMotionList.end(); ++vit )
        {
			if ( vit->m_strMotionName != "NULL" )
			{
				std::pair< GCDeviceMotionFRM*, MoveModelMotion > pairMotionData( std::make_pair( g_pGCDeviceManager2->CreateMotionFRM( vit->m_strMotionName ), *vit ) );
				m_mapMotionList.insert( std::make_pair( index, pairMotionData ) );
				index++;
			}
        }

		std::vector< MoveModelParticle >::iterator vit2;
		for ( vit2 = m_MoveModelData.m_vecParticle.begin(); vit2 != m_MoveModelData.m_vecParticle.end(); ++vit2 )
		{
			CParticleEventSeqPTR pParticle = NULL;
			pParticle = g_ParticleManager->CreateSequence( vit2->m_particleSeqName, vit2->m_fXOffset, vit2->m_fYOffset, vit2->m_fTraceTime );

			g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI_2 );

			if(vit2->m_iMouseEventType == EVENT_MOUSE_CLICK)
			{
				g_ParticleManager->DeleteSequence(pParticle);
				m_vecHiddenEffects.push_back(pParticle);
			}
			else
			{
				std::pair< CParticleEventSeqPTR, MoveModelParticle > pairParticleData( std::make_pair(pParticle, *vit2) );
				m_mapParticleList.insert( std::make_pair( vit2->m_particleSeqName, pairParticleData ) );
				m_vecEffects.push_back(pParticle);
			}
		}

        if ( IsMotion( m_iNowModelMotionIndex ) )
        {
            SetMotion(m_iNowModelMotionIndex);
        }
    }
}

void KGCMoveModel::PostChildDraw( void )
{
    if ( m_bReloadModelData )
    {
        int tempNPCID = m_iNowModelID;
        m_iNowModelID = -1;
        SetModelID( tempNPCID );
        m_bReloadModelData = false;
    }

    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();

    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

    if ( m_iNowModelID != -1 )
    {
        float fScale = m_MoveModelData.m_fScale;
        float fPosX = m_MoveModelData.m_fPosX;
        float fPosY = m_MoveModelData.m_fPosY;

		if (m_pModelMotion != nullptr)
		{
			if (m_cFrame > (int)m_pModelMotion->GetNum_Frame() - 1)
			{
				LoadMotion();
				m_cFrame = 0;
			}

			fPosY += m_pModelMotion->GetFrameData(m_cFrame).Pos_y;
			m_pModelObject->SetFrame(&m_pModelMotion->GetFrameData((int)m_cFrame));
		}

		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
		D3DXMatrixRotationY(&matRot, 0.0f);
		D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 1.0);

        matWorld = matScale * matRot * matTrans;
        if( m_pModelObject != nullptr )
        {
            m_pModelObject->SetWorldMat( &matWorld );
            g_RenderManager->Render( false, m_pModelObject, EGCShaderTechnique::CARTOON_NOEDGE_ALPHA);
        }
        m_cFrame++;
    }

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCMoveModel::FrameMoveInEnabledState( void )
{
	RenderEffect(true);
}

void KGCMoveModel::InitNPCData()
{
    m_cFrame = 0;
    m_iNowModelID = -1;
    m_iNowModelMotionIndex = 0;
    m_iMotionCount = 0;
    m_mapMotionList.clear();
	m_mapParticleList.clear();
}

void KGCMoveModel::SetMotion( int index )
{
    std::map< int, std::pair< GCDeviceMotionFRM*, MoveModelMotion > >::iterator mit;
    mit = m_mapMotionList.find(index);
    if ( mit != m_mapMotionList.end() )
    {
        m_pModelMotion = mit->second.first;
    }
}

void KGCMoveModel::ShowEffect(bool show)
{
	this->m_3B8 = (int)show;
	this->m_3BC = (int)show;

	std::vector<CParticleEventSeqPTR>::iterator it = m_vecHiddenEffects.begin();
	int index = 0;
	for(; it != m_vecHiddenEffects.end(); ++it)
	{
		std::map<std::string, std::pair<CParticleEventSeqPTR, MoveModelParticle> >::iterator it2 = m_mapParticleList.find(it->m_strParticleName);
		if(it2 != m_mapParticleList.end())
		{
			continue;
		}

		std::vector<MoveModelParticle>::iterator it3 = m_MoveModelData.m_vecParticle.begin();

		for( ; it3 != m_MoveModelData.m_vecParticle.end(); ++it3 )
		{
			if(it3->m_particleSeqName.compare(it->m_strParticleName) == 0)
			{
				CreateEffect(it3->m_particleSeqName, it3->m_fXOffset, it3->m_fYOffset, it3->m_fTraceTime);
				break;
			}
		}
		index++;
	}
}

bool KGCMoveModel::IsMotion( int index )
{
    std::map< int, std::pair< GCDeviceMotionFRM*, MoveModelMotion > >::iterator mit;
    mit = m_mapMotionList.find(index);
    if ( mit != m_mapMotionList.end() )
    {
        return true;
    }

    return false;
}

void KGCMoveModel::CreateEffect(std::string seq, float x, float y, float z)
{
	CParticleEventSeqPTR pParticle = NULL;
	pParticle = g_ParticleManager->CreateSequence( seq, x, y, z );
	if( pParticle )
	{
		g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI_2 );
		m_vecEffects.push_back(pParticle);
	}
}

void KGCMoveModel::RenderEffect( bool bRender )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecEffects.begin();

	for (; iterPtr != m_vecEffects.end() ; ++iterPtr )
	{
		if ((*iterPtr))
			(*iterPtr)->SetShow(bRender);
	}
}

void KGCMoveModel::ClearEffect(CParticleEventSeqPTR particle)
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecEffects.begin();

	for (; iterPtr != m_vecEffects.end() ; ++iterPtr )
	{
		g_ParticleManager->DeleteSequence( *iterPtr );
		m_vecEffects.erase(iterPtr);
	}
}

void KGCMoveModel::ClearAllEffects()
{
	std::map<std::string, std::pair<CParticleEventSeqPTR, MoveModelParticle>>::iterator iterPtr = m_mapParticleList.begin();
	for(; iterPtr != m_mapParticleList.end(); ++iterPtr)
	{
		g_ParticleManager->DeleteSequence( iterPtr->second.first );
	}

	std::vector<CParticleEventSeqPTR>::iterator it = m_vecEffects.begin();
	for(; it != m_vecEffects.end(); ++it)
	{
		g_ParticleManager->DeleteSequence( *it );
	}

	for( it = m_vecHiddenEffects.begin(); it != m_vecHiddenEffects.end(); ++it )
	{
		g_ParticleManager->DeleteSequence( *it );
	}

	if(m_vecHiddenEffects.begin() != m_vecHiddenEffects.end())
	{
		m_vecHiddenEffects.clear();
	}

	m_vecEffects.clear();
	m_mapParticleList.clear();
}


void KGCMoveModel::LoadMotion()
{
	std::map< int, std::pair< GCDeviceMotionFRM*, MoveModelMotion > >::iterator it;
	for(it = m_mapMotionList.begin(); it != m_mapMotionList.end(); ++it )
	{
		switch(m_3B8)
		{
		case 0:
			{
				if(it->second.second.m_iMouseEventType == 0 && IsMotion(it->first))
				{
					if(m_3A0 != 2)
					{
						SetMotion(it->first);
					}
				}
				else if(it->second.second.m_iMouseEventType == 2 && IsMotion(it->first))
				{
					if(m_3A0 == 1)
					{
						SetMotion(it->first);
						m_3A0 = 2;
					}
				}
			}
			break;
		case 1:
			{
				if(it->second.second.m_iMouseEventType == 1 && IsMotion(it->first))
				{
					m_iMotionCount++;
					SetMotion(it->first);

					m_cFrame = 0;

					if(m_iMotionCount == it->second.second.m_iReplayCount)
					{
						m_iMotionCount = 0;
					}

					m_3A0 = 1;
				}
			}
			break;
		}
	}

	m_3B8 = 0;
}