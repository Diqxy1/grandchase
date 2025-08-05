#include "stdafx.h"
#include ".\gcobject.h"
#include "GCDeviceLib\/GCDeviceShader.h"
#include "GCDeviceLib\/GCDeviceBuildMesh.h"
#include "MyD3D.h"
#include "GCCameraOrtho.h"
#include "KTDGFrustum.h"
#include "GCObjectAndMotion.h"

extern LPDIRECT3DDEVICE9	g_pd3dDevice;

GCMeshRenderFunc GCObject::ms_pRenderFunc[EGCShaderTechnique::TECHNIQUE_NUM] = { NULL, NULL, NULL, NULL };
GCPrimitiveRenderFunc GCObject::ms_pPrimitiveRenderFunc = NULL;
GCSetMotionMatrixFunc GCObject::ms_pSetMotionMatrixFunc = NULL;
unsigned int GCObject::ms_uiMeshIndex = 0;
bool GCObject::ms_bUsingVertexShader = false;

char* GCObject::ms_strTechnique[] = { "CartoonBlackEdge", "CartoonNoEdge" , "NoCartoonNoEdge", "ResultModel", "CartoonNoEdgeAlpha", "CartoonEdgeAlpha", "CartoonNoEdgeAlphaBright", "CartoonEdgeAlphaBright" };
GCObject::GCObject(void)
{
    m_vecMeshObject.clear();
    m_dwMeshState = 0;
    
    m_dwID          = 0;
    m_matMotion     = NULL;
    m_bRender       = true;
    m_fVolume       = 0.3f;
    SetMeshComputePaletteInterface( &g_DefaultIGCComputePaletteP3M );
    m_eCartoon      = CARTOON_DEFAULT;
    m_iSelectTexture = -1;
    m_vOutlineColor = D3DXVECTOR4(0,0,0,1);
    SetShellMatrixScale( 0.3f );

	m_pBuildMeshObject = new GCMeshObject();	
	m_bBuilded=false;
	m_iTechnique = -1;
	m_iLayer = GC_LAYER_CHAR;
    m_iCenterBone = 0;
    m_vecPosDesc.clear();
	m_bIsExtraObject = false;
}

GCObject::~GCObject(void)
{
    SAFE_DELETE_ARRAY(m_matMotion);
    ClearPositionBone();
    ClearAngleBon();

	ClearBuild();
	SAFE_DELETE(m_pBuildMeshObject);

    RemoveMesh(NULL);
    for ( unsigned int i = 0; i < m_vecTexture.size(); i++ )
    {
        SAFE_DEVICE_RELEASE( m_vecTexture[i] );
    }

	for( int i = 0 ; i < (int)m_vecChild.size(); i++ )
	{	
		m_vecChild[i].first->ClearMotion();
		SAFE_REMOVE_DELETE( m_vecChild[i].first );
	}

    for ( unsigned int i = 0 ; i < m_vecPosDesc.size(); ++i )
    {
        if ( m_vecPosDesc[i]->pParticle )
        {
            g_ParticleManager->DeleteSequence( m_vecPosDesc[i]->pParticle );
        }
        SAFE_DELETE( m_vecPosDesc[i] );
    }
    m_vecPosDesc.clear();
}

void GCObject::SetRenderFunc()
{
    ms_pRenderFunc[EGCShaderTechnique::CARTOON_BLACKEDGE]   = &GCObject::RenderTwoTexture;
    ms_pRenderFunc[EGCShaderTechnique::CARTOON_NOEDGE]      = &GCObject::RenderOneTexture;
    ms_pRenderFunc[EGCShaderTechnique::NOCARTOON_NOEDGE]    = &GCObject::RenderOneTextureAlpha;
    ms_pRenderFunc[EGCShaderTechnique::RESULT_MODEL]        = &GCObject::RenderOneTextureAlpha;
	ms_pRenderFunc[EGCShaderTechnique::CARTOON_NOEDGE_ALPHA]= &GCObject::RenderTwoTextureAlpha;
	ms_pRenderFunc[EGCShaderTechnique::CARTOON_EDGE_ALPHA]	= &GCObject::RenderTwoTextureAlpha;
    ms_pRenderFunc[EGCShaderTechnique::CARTOON_NOEDGE_ALPHA_BRIGHT]= &GCObject::RenderTwoTextureAlpha;
    ms_pRenderFunc[EGCShaderTechnique::CARTOON_EDGE_ALPHA_BRIGHT]	= &GCObject::RenderTwoTextureAlpha;
    if (ms_bUsingVertexShader)
    {
        ms_pRenderFunc[EGCShaderTechnique::CARTOON_NOEDGE]  = &GCObject::RenderTwoTexture;
    }
}

void GCObject::SetUsingVS( bool bUsingVS_ )
{
    ms_bUsingVertexShader = bUsingVS_;
    if ( ms_bUsingVertexShader )
    {
        ms_pPrimitiveRenderFunc = &GCObject::RenderPrimitiveVS;
        ms_pSetMotionMatrixFunc = &GCObject::SetMotionMatrixVS;
    }
    else
    {
        ms_pPrimitiveRenderFunc = &GCObject::RenderPrimitiveFixedPipe;
        ms_pSetMotionMatrixFunc = &GCObject::SetMotionMatrixFP;
    }
    SetRenderFunc();
}

GCMeshObject* GCObject::AddMesh(GCDeviceMeshP3M* pMesh_, GCDeviceTexture* pTexture_, EGCCartoonTexture eCartoon_, const std::string strID, int iTech_/* = -1*/)
{
    // 메쉬가 없는 아이템들도 있다.
    if (pMesh_ == NULL && pMesh_->GetDeviceState() != GCDS_LOADED)
        return NULL;
    //if ( m_vecMeshObject.size() != 0 )
    //{
    //    ASSERT( m_vecMeshObject[0]->m_pMesh->GetNumPositionBone() == pMesh_->GetNumPositionBone() && 
    //            m_vecMeshObject[0]->m_pMesh->GetNumAngleBone() == pMesh_->GetNumAngleBone() );
    //}
//    else 

    if (m_dwNumAngleBone < pMesh_->GetNumAngleBone())
    {
        SAFE_DELETE_ARRAY(m_matMotion);
        ClearPositionBone();
        ClearAngleBon();

        m_dwNumAngleBone = pMesh_->GetNumAngleBone();
        m_dwNumPositionBone = pMesh_->GetNumPositionBone();

        SetPositionBone(new KPositionBone[m_dwNumPositionBone], m_dwNumPositionBone);
        SetAngleBon(new KAngleBoneOnMemory[m_dwNumAngleBone], m_dwNumAngleBone);

        CopyPositionBone(pMesh_->GetPositionBone());
        CopyAngleBon(pMesh_->GetAngleBone());

        m_matMotion = new D3DXMATRIX[m_dwNumAngleBone];
        for (unsigned int i = 0; i < m_dwNumAngleBone; i++)
        {
            D3DXMatrixTranslation(&m_matMotion[i], 0.0f, 0.0f, 0.0f);
        }
        //pMesh_->SetMeshComputePaletteInterface( m_pIMeshComputePalette );
    }

    GCMeshObject* pkMeshObject = new GCMeshObject();
    pkMeshObject->m_pMesh = pMesh_;
    pkMeshObject->m_pTexture = pTexture_;
    pkMeshObject->m_eCartoon = eCartoon_;
    pkMeshObject->m_strName = strID;
    pkMeshObject->m_strTextureName = pTexture_->GetDeviceID();
    //정해진 Tech가 없을때만 설정
    if (iTech_ == -1)
    {
        if (pTexture_->GetFileFormat() == D3DFMT_DXT1)
        {
            iTech_ = EGCShaderTechnique::CARTOON_BLACKEDGE;
        }
        else if (pTexture_->GetFileFormat() == D3DFMT_DXT3 || pTexture_->GetFileFormat() == D3DFMT_A8R8G8B8)
        {
            iTech_ = EGCShaderTechnique::CARTOON_EDGE_ALPHA;
        }
        else if (pTexture_->GetFileFormat() == D3DFMT_DXT5)
        {
            iTech_ = EGCShaderTechnique::CARTOON_NOEDGE_ALPHA;
        }
    }
    //m_iOriginalTechnique에 원래 테크닉 저장 그래픽레벨 최상일때 이걸로 교체한다.
    pkMeshObject->m_iOriginalTechnique = iTech_;
    pkMeshObject->m_iTechnique = iTech_;

    if (pkMeshObject->m_iTechnique == EGCShaderTechnique::CARTOON_NOEDGE_ALPHA ||
        pkMeshObject->m_iTechnique == EGCShaderTechnique::CARTOON_EDGE_ALPHA)
        m_vecMeshObject.insert(m_vecMeshObject.begin(), pkMeshObject);
    else
        m_vecMeshObject.push_back(pkMeshObject);

    return pkMeshObject;
}

GCMeshObject* GCObject::AddMesh( const std::string& strMesh, const std::string& strTex, char cIgBoneIdx , EGCCartoonTexture eCartoon_  , const std::string strID, int iTech_, int iItemID_ )
{
	if( g_pGCDeviceManager->GetMassFileManager()->IsIn( strMesh ) == false )
	{
		//브라질에서는 패치를 제대로 받지 않고 게임을 실행하는 유저가 많다. 모델파일 없을 경우 그려질리가 없다.
		//그래서 안내를 위한 메시지를 띄워주기 위해 이와같은 구문을 추가한다. 단점은 원래 모델이 없는 장착아이템들(악세사리류)도 있기
		//때문에 장비아이템을 기본으로 해서 메시지를 띄워주기로 한다. 장비아이템이 없으면 캐릭터자체가 이상하게 그려지가 때문에 최소한
		//이점은 보완할 수 있다.
        if( iItemID_ != -1 )
            return NULL;

		if( g_pItemMgr->IsEquipmentItem(iItemID_) ) 
			g_kGlobalValue.m_bIncompletePatch = true; 

		return NULL;
	}

	return AddMesh(g_pGCDeviceManager2->CreateMeshP3M(strMesh, cIgBoneIdx), g_pGCDeviceManager2->CreateTexture( strTex), eCartoon_ , strID, iTech_ );
}

void GCObject::RemoveMesh( GCBaseMesh* pMesh )
{
	if(!pMesh)
    {
		ClearBuild();
        for( int i = 0; i < (int)m_vecChild.size() ; i++ )
        {
            m_vecChild[i].first->ClearMotion();
            SAFE_REMOVE_DELETE( m_vecChild[i].first );
        }
        m_vecChild.clear();
    }

    GCMeshObject* pDeleted;

    // 널이면 다 지운다. 내맘이다.
    for ( int i = (int)m_vecMeshObject.size() - 1 ; i >= 0 ; i-- )
    {
        if ( NULL != m_vecMeshObject[i] && (m_vecMeshObject[i]->m_pMesh == pMesh || pMesh == NULL ) )
        {
            pDeleted = m_vecMeshObject[i];
            SAFE_RELEASE(m_vecMeshObject[i]->m_pTexture);
            SAFE_RELEASE(m_vecMeshObject[i]->m_pMesh);
            SAFE_DELETE(m_vecMeshObject[i]);
            

			m_vecMeshObject.erase( m_vecMeshObject.begin() + i );
		}
    }
}

void GCObject::RemoveMesh( const std::string& strName )
{
    std::vector<ParasiteMesh>::iterator vitParasiteMesh = m_vecChild.begin();
	for( ; vitParasiteMesh != m_vecChild.end() ; )
	{
		if( (*vitParasiteMesh).first->m_strName == strName )
		{
			(*vitParasiteMesh).first->ClearMotion();
			SAFE_REMOVE_DELETE( (*vitParasiteMesh).first );
			vitParasiteMesh = m_vecChild.erase( vitParasiteMesh );
		}
        else {
            ++vitParasiteMesh;
        }

	}

    std::vector< GCMeshObject* >::iterator vitMesh = m_vecMeshObject.begin();
    for( ; vitMesh != m_vecMeshObject.end() ; ) {

        if( (*vitMesh)->m_strName == strName )
        {
            SAFE_RELEASE((*vitMesh)->m_pTexture);
            SAFE_RELEASE((*vitMesh)->m_pMesh);
            SAFE_DELETE((*vitMesh));

            vitMesh = m_vecMeshObject.erase( vitMesh );
        }
        else {
            ++vitMesh;
        }
    }
}
void GCObject::Render( int iTechnique_, bool bCull_ )
{
    if ( !m_bRender )
        return;

    if (  ( ( m_dwMeshState & MS_INVISIBLE_ENEMY ) == MS_INVISIBLE_ENEMY ) 
        || ( ( m_dwMeshState & MS_BACK_STEP_ENEMY ) == MS_BACK_STEP_ENEMY ) )
    {
        return;
    }

    if ( ( ( m_dwMeshState & MS_DEAD ) == MS_DEAD ) 
        || ( ( m_dwMeshState & MS_INVISIBLE ) == MS_INVISIBLE ) 
        || ( ( m_dwMeshState & MS_BACK_STEP ) == MS_BACK_STEP ) )
    {    
        if ( ( o5 < 2 ) )
        {
            return;
        }
    }
    
    GCDeviceTexture* pMeshTexure = NULL;
    GCDeviceTexture* pCartoonTexure = NULL;

    if ( ( m_dwMeshState & MS_IMMORTAL ) == MS_IMMORTAL 
        || ( m_dwMeshState & MS_REBIRTH ) == MS_REBIRTH  )
    {
        if ( o5 > 2 )
        {
            pMeshTexure = g_pGCDeviceManager2->GetNullTexture();
        }        
    }    
    else if ( m_iSelectTexture >= 0 && (UINT)m_iSelectTexture < m_vecTexture.size() )
    {
        pMeshTexure = m_vecTexture[m_iSelectTexture];
    }
    
    if ( ( m_dwMeshState & MS_EMPTY_SAVING ) == MS_EMPTY_SAVING )
    {
        if ( o10 < 10 )
        {
            pCartoonTexure = g_pGraphicsHelper->GetCartoonTexture( CARTOON_SAVING_EMPTY );
        }
    } 
    else 
    {
        if ( m_eCartoon != CARTOON_DEFAULT )
        {
            pCartoonTexure = g_pGraphicsHelper->GetCartoonTexture( m_eCartoon );
        }
    }    

	(this->*ms_pRenderFunc[iTechnique_])( pMeshTexure, pCartoonTexure, iTechnique_ );

}

void GCObject::RenderTwoTexture(GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_) const
{
    unsigned int uiSize = (UINT)m_vecMeshObject.size();

    if (pMeshTexture_)
        pMeshTexture_->SetDeviceTexture(0);
    if (pCartoonTexture_)
        pCartoonTexture_->SetDeviceTexture(1);

    if (!ms_bUsingVertexShader)
    {
        g_pd3dDevice->SetTransform(D3DTS_TEXTURE1, &m_matShellMatrix);
    }

    for (ms_uiMeshIndex = uiSize - 1; ms_uiMeshIndex != UINT_MAX; --ms_uiMeshIndex)
    {
        if (!m_vecMeshObject[ms_uiMeshIndex]->m_bRender)
            continue;

        // 왜 0 개의 정점 또는 0 개의면을 가진 널 (null) 오브젝트를 렌더 할 것인가? == N.A DEV DA KOG
        if (m_vecMeshObject[ms_uiMeshIndex] == NULL || m_vecMeshObject[ms_uiMeshIndex]->m_pMesh == NULL || m_vecMeshObject[ms_uiMeshIndex]->m_pMesh->GetNumVertex() == 0 || m_vecMeshObject[ms_uiMeshIndex]->m_pMesh->GetNumFace() == 0) {
            continue;
        }

        if (!pMeshTexture_)
            m_vecMeshObject[ms_uiMeshIndex]->m_pTexture->SetDeviceTexture(0);

        if (!pCartoonTexture_)
            g_pGraphicsHelper->GetCartoonTexture(m_vecMeshObject[ms_uiMeshIndex]->m_eCartoon)->SetDeviceTexture(1);

        // 아놔.. 이러긴 싫었는데 -_-;; 에이미가 이거 해달라해서.. 이렇게 짰음.. ㅠ
        // 메쉬에 설정된 테크닉이 있다면 그걸로 그리고,,,
        // 없다면 원래 테크닉으로 그리겠음!!
        // 그 오브젝트 전체의 테크닉을 설정하면 이런일은 없을텐데 ㅠㅠ
        // 이전 테크닉과 같으면 바꾸기 싫은데...static 넣을까?;;
        if (ms_bUsingVertexShader)
        {
            if (m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique != -1 && m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique < 6)
            {
                QualityMeshTechChange();
                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique]);
            }
            else {
                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[iTech_]);
            }
        }
        (this->*ms_pPrimitiveRenderFunc)();
    }
}

void GCObject::RenderTwoTextureAlpha(GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_) const
{
    unsigned int uiSize = (UINT)m_vecMeshObject.size();

    if (pMeshTexture_)
        pMeshTexture_->SetDeviceTexture(0);
    if (pCartoonTexture_)
        pCartoonTexture_->SetDeviceTexture(1);

    if (!ms_bUsingVertexShader)
    {
        g_pd3dDevice->SetTransform(D3DTS_TEXTURE1, &m_matShellMatrix);
    }

    for (ms_uiMeshIndex = 0; ms_uiMeshIndex < uiSize; ++ms_uiMeshIndex)
    {
        if (!m_vecMeshObject[ms_uiMeshIndex]->m_bRender)
            continue;

        if (!pMeshTexture_)
            m_vecMeshObject[ms_uiMeshIndex]->m_pTexture->SetDeviceTexture(0);

        if (!pCartoonTexture_)
            g_pGraphicsHelper->GetCartoonTexture(m_vecMeshObject[ms_uiMeshIndex]->m_eCartoon)->SetDeviceTexture(1);

        QualityMeshTechChange();

        g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique]);

        (this->*ms_pPrimitiveRenderFunc)();
    }
}

void GCObject::RenderOneTexture(GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_) const
{
    unsigned int uiSize = (UINT)m_vecMeshObject.size();

    if (pMeshTexture_)
        pMeshTexture_->SetDeviceTexture(0);

    for (ms_uiMeshIndex = 0; ms_uiMeshIndex < uiSize; ms_uiMeshIndex++)
    {
        if (!m_vecMeshObject[ms_uiMeshIndex]->m_bRender)
            continue;

        if (pMeshTexture_)
            pMeshTexture_->SetDeviceTexture(0);
        else
            m_vecMeshObject[ms_uiMeshIndex]->m_pTexture->SetDeviceTexture(0);

        if (ms_bUsingVertexShader)
        {
            if (m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique != -1)
            {
                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique]);
            }
            else
                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[iTech_]);
        }

        (this->*ms_pPrimitiveRenderFunc)();
    }
}

void GCObject::RenderOneTextureAlpha(GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_) const
{
    unsigned int uiSize = (UINT)m_vecMeshObject.size();

    if (pMeshTexture_)
        pMeshTexture_->SetDeviceTexture(0);

    if (!ms_bUsingVertexShader)
    {
        g_pd3dDevice->SetTransform(D3DTS_TEXTURE1, &m_matShellMatrix);
    }

    for (ms_uiMeshIndex = uiSize - 1; ms_uiMeshIndex != UINT_MAX; --ms_uiMeshIndex)
    {
        if (!m_vecMeshObject[ms_uiMeshIndex]->m_bRender)
            continue;

        if (!pMeshTexture_)
            m_vecMeshObject[ms_uiMeshIndex]->m_pTexture->SetDeviceTexture(0);

        if (ms_bUsingVertexShader)
        {
            if (m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique != -1)
            {
                QualityMeshTechChange();

                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique]);
            }
            else
                g_pRenderState->GetEffect()->SetTechnique(ms_strTechnique[iTech_]);
        }

        (this->*ms_pPrimitiveRenderFunc)();
    }
}

// if return value is false, then never drawed by manager.
bool GCObject::SetMotionMatrixVS( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_, bool bForce_ )
{
    //====================================================================================
    // 2007-06-15 오전 10:00:14 : 우동완
    // 메쉬가 없는 Object도 있다. Monster Dead Model..
    if ( m_vecMeshObject.empty() || !m_bRender )
    {
        return false;
    }


    if ( IsCullObject(bCamera_) && !bForce_ && bCamera_ )
    {
        return false;
    }


    D3DXMATRIX matWorld;
    if ( !bCamera_ )
    {
        matWorld = *GetWorldMat();
    }
    else
    {
        matWorld = *GetWorldMat() * g_kCamera.m_matCamera;
    }    

    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );    

    if ( m_kFrame.IsInit() )
    {
        if ( bTransform_ )
        {
            D3DXMATRIX matWorldViewProj = matWorld * (*matViewProj);
            g_pRenderState->GetEffect()->SetMatrix( "g_mWorld", &matWorld );                        
            g_pRenderState->GetEffect()->SetMatrix( "g_mWorldViewProj", &matWorldViewProj );            
            g_pRenderState->GetEffect()->SetMatrix( "g_BoneMatrix[0]", &matIdentity );
            return true;
        }
        return false;
        
    }
    else
    {        
        MatrixCompute( &matIdentity, 0 , true );        
        
        if ( !bTransform_ )
        {
            return false;
        }

        D3DXMATRIX matWorldViewProj = matWorld * (*matViewProj);
        g_pRenderState->GetEffect()->SetMatrix( "g_mWorld", &matWorld );
        g_pRenderState->GetEffect()->SetMatrix( "g_mWorldViewProj", &matWorldViewProj );
        g_pRenderState->GetEffect()->SetMatrixArray( "g_BoneMatrix", m_matMotion, m_dwNumAngleBone );        
        g_pRenderState->GetEffect()->SetVector( "g_vEdgeColor", &m_vOutlineColor );

        return true;
    }

}

bool GCObject::SetMotionMatrixFP( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_, bool bForce_ )
{
    //====================================================================================
    // 2007-06-15 오전 10:00:14 : 우동완
    // 메쉬가 없는 Object도 있다. Monster Dead Model..
    if ( m_vecMeshObject.empty() || !m_bRender )
    {
        return false;
    }

    if ( IsCullObject(bCamera_) && !bForce_ && bCamera_ )
    {
        return false;
    }

    D3DXMATRIX matWorld;

    if ( !bCamera_ )
    {
        matWorld = *GetWorldMat();
    }
    else
    {
        matWorld = *GetWorldMat() * g_kCamera.m_matCamera;
    }    

    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );

    if ( m_kFrame.IsInit() )
    {   
        if ( bTransform_ )
        {
            g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX(0), &matWorld );
        }        
        return false;
    }
    else
    {
        MatrixCompute( &matIdentity, 0 , true );
    
        if ( bTransform_ )
        {
            for ( unsigned int i = 0; i < m_dwNumAngleBone ; ++i )
            {
                g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX(i), &D3DXMATRIX( m_matMotion[ i ] * matWorld ) );
            }
        }
    }
    return true;
}

void GCObject::MatrixCompute( D3DXMATRIX* pMatBase, int iBone , bool IsPos )
{
    ASSERT( iBone < MAX_BONE );

    if(*pMatBase == NULL) return;
    if ( IsPos )
    {
        //compute
        D3DXMATRIX matTrans;
        m_pIMeshComputePalette->ComputePositionBone(&matTrans, iBone, *pMatBase, m_vecPositionBone);

        //child
        if((int)m_vecPositionBone.size() <= iBone || m_vecPositionBone.empty())
        {
            return;
        }

        for ( int i = 0 ; i < 10 ; i++ )
        {
            if ( m_vecPositionBone[iBone]->acChildIndex[i] == INVALID_BONE_INDEX )
            {
                break;
            }
            MatrixCompute(&matTrans,m_vecPositionBone[iBone]->acChildIndex[i],false);
        }
    }
    else
    {
        //Compute
        if( m_matMotion && (int(m_dwNumAngleBone) > iBone) )
            m_pIMeshComputePalette->ComputeAngleBone(&m_matMotion[iBone], &m_kFrame, iBone, *pMatBase );
        if((int)m_vecAngleBone.size() <= iBone || m_vecAngleBone.empty())
        {
            return;
        }
        if((int)m_vecAngleBone.size() <= iBone || m_vecAngleBone.empty())
        {
            return;
        }


        //child
        for ( int i = 0 ; i < 10 ; i++ )
        {
            if ( m_vecAngleBone[iBone]->acChildIndex[i] == INVALID_BONE_INDEX )
            {
                break;
            }
            MatrixCompute(&m_matMotion[iBone],m_vecAngleBone[iBone]->acChildIndex[i],true);
        }
    }
}

void GCObject::RenderPrimitiveVS() const
{
    UINT uiPass;
    g_pRenderState->GetEffect()->Begin(&uiPass,0);
    for ( UINT j = 0; j < uiPass; j++ )
    {
        g_pRenderState->GetEffect()->BeginPass( j );
        m_vecMeshObject[ms_uiMeshIndex]->m_pMesh->Render();
        g_pRenderState->GetEffect()->EndPass();
    }
    g_pRenderState->GetEffect()->End();

}

void GCObject::RenderPrimitiveFixedPipe() const
{    
    m_vecMeshObject[ms_uiMeshIndex]->m_pMesh->Render();
}

void GCObject::SetShellMatrixScale( float fMatrixScale_ )
{
    m_fShellMatrixScale = fMatrixScale_;
    m_matShellMatrix._11 = 0.5f*fMatrixScale_* g_kCamera.m_fZoom; m_matShellMatrix._12 = 0.0f; m_matShellMatrix._13 = 0.0f; m_matShellMatrix._14 = 0.0f; 
    m_matShellMatrix._21 = 0.0f; m_matShellMatrix._22 =-0.5f*fMatrixScale_ * g_kCamera.m_fZoom; m_matShellMatrix._23 = 0.0f; m_matShellMatrix._24 = 0.0f; 
    m_matShellMatrix._31 = 0.0f; m_matShellMatrix._32 = 0.0f; m_matShellMatrix._33 = 1.0f; m_matShellMatrix._34 = 0.0f; 
    m_matShellMatrix._41 = 0.5f; m_matShellMatrix._42 = 0.5f; m_matShellMatrix._43 = 0.0f; m_matShellMatrix._44 = 1.0f; 
    //g_pd3dDevice->SetTransform( D3DTS_TEXTURE1, &m_matShellMatrix );
}

bool GCObject::IsCullObject(bool bCamera_)
{
    D3DXMATRIX matWorld;
    if( bCamera_ )
    {
        matWorld = *GetWorldMat() * g_kCamera.m_matCamera;
    }
    else
    {
        matWorld = *GetWorldMat();
    }
    D3DXVECTOR3 vPoint;
    
    vPoint.x = matWorld._41;
    vPoint.y = matWorld._42;
    vPoint.z = 0.5f;

    return !g_MyD3D->m_pFrustum->CheckSphere( vPoint, m_fVolume );
}

const D3DXMATRIX* GCObject::GetAngleBone( unsigned int iIndex, D3DXMATRIX* pOutMatrix )
{
    //ASSERT(iIndex >= 0 && iIndex < m_dwNumAngleBone );
	if(iIndex < 0 || iIndex >= m_dwNumAngleBone)
    {
#if !defined( __PATH__ ) 
//      에러 메세지 박스 많이 떠서 삭제함.
//      MessageBoxA( NULL, "Error GCObject::GetAngleBone", "Error", MB_OK );
#endif
		return NULL;
    }

    if( pOutMatrix  && m_matMotion ) {
        *pOutMatrix = m_matMotion[iIndex];
   }

    if( m_matMotion ) {
        return &m_matMotion[iIndex];
    }
	
#if !defined( __PATH__ ) 
        MessageBoxA( NULL, "Error GCObject::GetAngleBone2", "Error", MB_OK );
#endif	
	
    return NULL;
}

void GCObject::GetAngleBonePos( unsigned int iIndex, D3DXVECTOR3* pOutVector )
{
    D3DXMATRIX mat;	
    GetAngleBone(iIndex,&mat);

    pOutVector->x = mat._41;
    pOutVector->y = mat._42;
    pOutVector->z = mat._43;    
}
GCMeshObject* GCObject::GetMeshObject( int iIndex_ )
{
    ASSERT( iIndex_ < (int)m_vecMeshObject.size() && 0 <= iIndex_ && "Mesh ObIndex Error" );
    return m_vecMeshObject[iIndex_];
}


bool GCObject::AddStaticMesh( const std::string& strMesh, const std::string& strTex, char cIgBoneIdx /*= -1*/ )
{
	if(m_bBuilded)
		ClearBuild();

    if ( !m_pBuildMeshObject )
        return false;

	if(!m_pBuildMeshObject->m_pTexture) 
		m_pBuildMeshObject->m_pTexture =new GCBuildTexture(g_pd3dDevice);// 이렇게 직접 접근하는건 정말 싫지만 어쩔수 없지비 

	if(!m_pBuildMeshObject->m_pMesh)
		m_pBuildMeshObject->m_pMesh =new GCBuildMesh(g_pd3dDevice);

    if( g_pGCDeviceManager->GetMassFileManager()->IsIn(strTex) == false )
    {
        return false;
    }

	STextureSection section;
	if ( false == ((GCBuildTexture*)m_pBuildMeshObject->m_pTexture)->AddTexture(strTex,&section) )
	{
		return false;
	}

    m_pBuildMeshObject->m_strTextureName = strTex;
    ((GCBuildMesh*)m_pBuildMeshObject->m_pMesh)->AddMesh(strMesh,&section.uv, cIgBoneIdx);
    
	return true;
}

GCMeshObject* GCObject::Build( EGCCartoonTexture eCartoon_ /*= CARTOON_NORMAL */ )
{
	if(m_bBuilded)
		return NULL;

	if(m_pBuildMeshObject->m_pTexture && m_pBuildMeshObject->m_pMesh)
	{
		((GCBuildMesh*)m_pBuildMeshObject->m_pMesh)->Build();

		if ( m_dwNumAngleBone < m_pBuildMeshObject->m_pMesh->GetNumAngleBone() )
		{
			SAFE_DELETE_ARRAY(m_matMotion);
            ClearPositionBone();
            ClearAngleBon();

			m_dwNumAngleBone    = m_pBuildMeshObject->m_pMesh->GetNumAngleBone();
			m_dwNumPositionBone = m_pBuildMeshObject->m_pMesh->GetNumPositionBone();

            SetPositionBone(new KPositionBone[m_dwNumPositionBone],m_dwNumPositionBone );
            SetAngleBon(new KAngleBoneOnMemory[m_dwNumAngleBone],m_dwNumAngleBone);

            CopyPositionBone(m_pBuildMeshObject->m_pMesh->GetPositionBone());
            CopyAngleBon(m_pBuildMeshObject->m_pMesh->GetAngleBone());


			m_matMotion         = new D3DXMATRIX[m_dwNumAngleBone];
			for(unsigned int i = 0; i < m_dwNumAngleBone; i++)
			{
				    D3DXMatrixTranslation( &m_matMotion[i], 0.0f, 0.0f, 0.0f );        
			}
		}				
		
		if(std::find(m_vecMeshObject.begin(),m_vecMeshObject.end(),m_pBuildMeshObject)==m_vecMeshObject.end())
			m_vecMeshObject.push_back( m_pBuildMeshObject );
	
		m_bBuilded = true;

		return m_pBuildMeshObject;
	}	
	return NULL;
}

void GCObject::ClearBuild()
{	
	if(m_pBuildMeshObject)
	{
		SAFE_RELEASE(m_pBuildMeshObject->m_pMesh);
		SAFE_RELEASE(m_pBuildMeshObject->m_pTexture);
		std::vector<GCMeshObject*>::iterator i = std::find(m_vecMeshObject.begin(),m_vecMeshObject.end(),m_pBuildMeshObject);
		if(i!=m_vecMeshObject.end())
			m_vecMeshObject.erase(i);
	}	
	m_bBuilded=false;
}

void GCObject::SetText( int i,std::wstring nickname,D3DCOLOR col,D3DCOLOR outline_col,std::string icon)
{	
    if ( m_pBuildMeshObject == NULL )
        return;

    //여기서 기존에 크래시가 났다. 그래서 m_pBuildMeshObject가 NULL인지 체크하는 구문 추가
	if(i<0 || i> 7 || !m_pBuildMeshObject->m_pTexture)
		return;

	RECT rect;
	if(icon=="")
	{
		rect.left=0;
		rect.right=256;
		rect.top=i*32;
		rect.bottom=rect.top+32;
		((GCBuildTexture*)m_pBuildMeshObject->m_pTexture)->TextDraw(g_pkFontMgrOutline,rect.left,rect.top,nickname,col, &rect, DT_VCENTER|DT_CENTER,outline_col);
	}
	else
	{
        rect.left=0;
        rect.right=256;
        rect.top=i*32;
        rect.bottom=rect.top+32;

        STextureSection section;
        section.Set(8,rect.top + 8, 16,16);

        ((GCBuildTexture*)m_pBuildMeshObject->m_pTexture)->TextDraw(g_pkFontMgrOutline,rect.left+36,rect.top,nickname,col, &rect, DT_VCENTER|DT_LEFT,outline_col);
        ((GCBuildTexture*)m_pBuildMeshObject->m_pTexture)->AddTexture(icon,&section,true);
	}	
}

bool GCObject::IsUseBuildNickNameTex( void )
{
	if( m_pBuildMeshObject->m_pTexture )
		return ((GCBuildTexture*)m_pBuildMeshObject->m_pTexture)->IsUseBuildNickNameTex();

	return false;
}

void GCObject::DrawText( int i,float x,float y )
{
	if(i<0 || i> 7 || !m_bBuilded)
		return;
	
	float tv1 = (32.0f  * i + 0.5f) / 512.0f;
	float tv2 = (32.0f  * (i+1) - 0.5f) / 512.0f;
	
	//g_pGCDeviceManager2->DrawInScreen(m_pBuildMeshObject->m_pTexture,
	//	800-512,600-512,800,600,0,0,1,1);

	
	g_pGCDeviceManager2->DrawInScreen(
		m_pBuildMeshObject->m_pTexture,x,y ,x+256,y+32,
		0 + (0.5f / 512.0f),tv1,
		0.5f - (0.5f / 512.0f),tv2);
}

void GCObject::SetMeshComputePaletteInterface( IGCComputePaletteP3M* IMeshComputePalette )
{
	if( IMeshComputePalette == NULL )
		m_pIMeshComputePalette = &g_DefaultIGCComputePaletteP3M;
	else
		m_pIMeshComputePalette = IMeshComputePalette;
}

bool GCObject::IsToggleExtraMesh( std::string strID )
{
    {
        std::vector< GCMeshObject* >::iterator vit;

        for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
        {
            if( (*vit)->m_strName == strID )
                return (*vit)->m_bRender;
        }
    }

    {
        std::vector< ParasiteMesh >::iterator vit;
        for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
        {
            if( (*vit).first->m_strName == strID )
                return (*vit).first->GetRender();
        }
    }

    return false;
}

void GCObject::ToggleMeshAll( bool bToggle )
{
    {
        std::vector< GCMeshObject* >::iterator vit;

        for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
        {
            if( !(*vit)->m_bSecondBaseMesh ) {
                (*vit)->m_bRender = bToggle;
            }            
        }
    }

    {
        std::vector< ParasiteMesh >::iterator vit;
        for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
        {
            (*vit).first->SetRender(bToggle);
        }
    }
}

void GCObject::ToggleExtraMesh( std::string strID, bool bToggle , int iShaderTech)
{
	{
		std::vector< GCMeshObject* >::iterator vit;

		for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
		{
            if( (*vit)->m_strName == strID ) {
				(*vit)->m_bRender = bToggle;
                (*vit)->m_iTechnique = iShaderTech;
                
                if ( iShaderTech != -1)
                    (*vit)->m_iOriginalTechnique = iShaderTech;
            }
		}
	}

	{
		std::vector< ParasiteMesh >::iterator vit;
		for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
		{
			if( (*vit).first->m_strName == strID )
				(*vit).first->SetRender(bToggle);
		}
	}
}

void GCObject::ToggleExtraMeshAll( bool bToggle )
{
    {
        std::vector< GCMeshObject* >::iterator vit;

        for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
        {
            if( (*vit)->m_bExtraMeshObject && !(*vit)->m_bSecondBaseMesh )
                (*vit)->m_bRender = bToggle;
        }
    }
    {
        std::vector< ParasiteMesh >::iterator vit;
        for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
        {
            if( (*vit).first->m_bIsExtraObject )
                (*vit).first->SetRender(bToggle);
        }
    }
}

void GCObject::ToggleBaseMeshAll( bool bToggle )
{
    {
        std::vector< GCMeshObject* >::iterator vit;

        for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
        {
            if( !(*vit)->m_bExtraMeshObject && !(*vit)->m_bSecondBaseMesh )
                (*vit)->m_bRender = bToggle;
        }
    }
    {
        std::vector< ParasiteMesh >::iterator vit;
        for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
        {
            if( !(*vit).first->m_bIsExtraObject )
                (*vit).first->SetRender(bToggle);
        }
    }
}

void GCObject::ToggleBaseMesh( bool bToggle )
{
    {
        std::vector< GCMeshObject* >::iterator vit;

        for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
        {
            if( !(*vit)->m_bExtraMeshObject && !(*vit)->m_bSecondBaseMesh )
                (*vit)->m_bRender = bToggle;
        }
    }
    {
        std::vector< ParasiteMesh >::iterator vit;
        for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
        {
            if( !(*vit).first->m_bIsExtraObject )
                (*vit).first->SetRender(bToggle);
        }
    }
}

void GCObject::ToggleSecondBaseMesh( std::string strID, bool bToggle )
{
    std::vector< GCMeshObject* >::iterator vit;
    for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
    {
        if( !(*vit)->m_bExtraMeshObject && (*vit)->m_bSecondBaseMesh && (*vit)->m_strName == strID ) { 
            (*vit)->m_bRender = bToggle;
        }
    }
}

void GCObject::ToggleSecondBaseMeshAll( bool bToggle )
{
    std::vector< GCMeshObject* >::iterator vit;
    for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
    {
        if( !(*vit)->m_bExtraMeshObject && (*vit)->m_bSecondBaseMesh )
            (*vit)->m_bRender = bToggle;
    }
}

void GCObject::AddChildObject( KGCObjectAndMotion* pObj_, int iBoneIndex_, bool bOnlyTraceParentPos_ )
{
	GCMeshTraceInfo tr;
	tr.iBoneID = iBoneIndex_;
	tr.bOnlyTraceParentPos = bOnlyTraceParentPos_;
    m_vecChild.push_back( std::make_pair(pObj_,tr) );
    //pObj_->SetPosition( D3DXVECTOR3( 0.0f,0.0f,0.0f ));
}

KGCObjectAndMotion* GCObject::GetChildObject( std::string strName )
{
    for( int i = 0 ; i < (int)m_vecChild.size(); i++ )
    {
        if( m_vecChild[i].first->m_strName == strName )
            return m_vecChild[i].first;
        
    }
    return NULL;
    
}

KGCObjectAndMotion* GCObject::GetChildObject( unsigned int iIndex )
{
	if( iIndex < m_vecChild.size() )
	{
		return m_vecChild[iIndex].first;
	}
	return NULL;
}


//************************************
// Method:    SetMotionMatrix
// FullName:  GCObject::SetMotionMatrix
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: bool bCamera_
// Parameter: D3DXMATRIX * matViewProj
// Parameter: bool bTransform_
// Parameter: bool bForce_
//************************************
bool GCObject::SetMotionMatrix( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_ /*= true*/, bool bForce_ /*= false */ )
{    

  
    bool bRet = (this->*ms_pSetMotionMatrixFunc)( bCamera_, matViewProj, bTransform_, bForce_ );    

    for( int i = 0; i < (int)m_vecChild.size(); i++ )
    {
        D3DXMATRIX matWorld;
        D3DXMATRIX* matMyWorld = const_cast<D3DXMATRIX*>(GetAngleBone(m_vecChild[i].second.iBoneID));

        if( matMyWorld ) {
            matWorld = ( (*matMyWorld) * (*GetWorldMat()) );
        }
        else
        {
            continue;
        }

        if( m_vecChild[i].second.bOnlyTraceParentPos )
        {
            D3DXMATRIX matRot,matScale,matTrans;
            D3DXVECTOR3 vPos = m_vecChild[i].first->GetOriginalPosition();				
            D3DXVECTOR3 vScale = m_vecChild[i].first->GetScale();
            D3DXMatrixScaling( &matScale,vScale.x, vScale.y, vScale.z );
            if( !m_bIsRight )
            {					
                D3DXMatrixRotationY( &matRot, 3.14159f );
                vPos.x = -vPos.x;
                vPos.z = -vPos.z;
            }
            else
            {
                D3DXMatrixIdentity( &matRot );
            }

            D3DXVec3TransformCoord( &vPos, &vPos, &matWorld );
            D3DXMatrixTranslation( &matTrans, vPos.x,vPos.y,vPos.z );

            matWorld = matScale * matRot * matTrans;

        }
        m_vecChild[i].first->SetWorldMat( &matWorld );
    }

    return bRet;
}

void GCObject::SetRender( bool bRender_ )
{
    m_bRender = bRender_; 
}

int GCObject::IsLoadTexture( const std::string &szTexName )
{
    int iLoop = 0;
    std::vector<GCDeviceTexture*>::iterator vecIter = m_vecTexture.begin();
    for( ; vecIter != m_vecTexture.end(); ++vecIter )
    {
        if( strstr((*vecIter)->GetDeviceID().c_str(), szTexName.c_str()) )
            return iLoop;
        ++iLoop;
    }
    return 0;
}

void GCObject::SetOutlineColor( D3DXVECTOR4 vColor )
{
    m_vOutlineColor = vColor; 
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->SetOutlineColor( vColor );            
    }    
}

void GCObject::MeshStateOn( DWORD dwMeshState_ )
{ 
    m_dwMeshState = m_dwMeshState | dwMeshState_; 
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->MeshStateOn( dwMeshState_ );            
    }
}


void GCObject::MeshStateOff( DWORD dwMeshState_ )
{
    m_dwMeshState = m_dwMeshState & ~dwMeshState_; 
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->MeshStateOff( dwMeshState_ );            
    }
}


void GCObject::ResetMeshState()
{
    m_dwMeshState = 0;
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->ResetMeshState();            
    }
}


void GCObject::SetMeshState( DWORD dwMeshState_ )
{
    m_dwMeshState = dwMeshState_;
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->SetMeshState( dwMeshState_ );            
    }
}

bool GCObject::GetMeshRender( std::string strID ) const
{
	{
		std::vector< ParasiteMesh >::const_iterator vit;
		for( vit = m_vecChild.begin(); vit != m_vecChild.end(); vit++ )
		{
			if( (*vit).first->m_strName == strID  )
				return (*vit).first->m_bRender;
		}
	}
	{
		std::vector< GCMeshObject* >::const_iterator vit;
		for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
		{
			if( (*vit)->m_strName == strID )
				return (*vit)->m_bRender;
		}   
	}
	
    return false;
}

void GCObject::ChildFrameMove()
{
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->FrameMove();            
    }
}

void GCObject::SetScale( D3DXVECTOR3 vPos )
{
    for( int i = 0; i < (int)m_vecChild.size() ; i++ )
    {
        m_vecChild[i].first->SetScale( vPos );            
    }
}

const D3DXMATRIX* GCObject::GetWorldMat()
{
    if ( m_iCenterBone != 0 )
    {
        D3DXMATRIX matBonePos;
        D3DXVECTOR3 vecBonePos;
        GetAngleBonePos( m_iCenterBone, &vecBonePos );
        D3DXMatrixTranslation( &matBonePos, -vecBonePos.x, vecBonePos.y, 0.0f );
        D3DXMatrixMultiply( &m_matWorld, &matBonePos, &m_matWorld );
    }

    return &m_matWorld;
}


void GCObject::AdjustParticlePos( void )
{
    if ( 0 != m_vecPosDesc.size() )
    {
        this->SetMotionMatrix( true, NULL, false, true );
    }
    for ( std::vector<ParticlePosDesc*>::iterator it = m_vecPosDesc.begin(); it != m_vecPosDesc.end(); it++ )
    {        
        if ( GetNumAngleBone() <= (DWORD)(*it)->m_iBoneIndex )
        {
            continue;
        }
        D3DXMATRIX matPartcle;
        GetAngleBone( (*it)->m_iBoneIndex, &matPartcle );
        D3DXMatrixMultiply( &matPartcle, &matPartcle, GetWorldMat() );
        if ( (*it)->pParticle != NULL )
        {
            if( (*it)->m_iRightSideLayer && m_bIsRight &&
                (*it)->pParticle->GetLayer() != (*it)->m_iRightSideLayer )
            {
                g_ParticleManager->SetLayer( (*it)->pParticle, (*it)->m_iRightSideLayer );
            }
            else if( (*it)->m_iLeftSideLayer && !m_bIsRight &&
                (*it)->pParticle->GetLayer() != (*it)->m_iLeftSideLayer )
            {
                g_ParticleManager->SetLayer( (*it)->pParticle, (*it)->m_iLeftSideLayer );
            }
        }
        (*it)->m_vPos.x = matPartcle._41;
        (*it)->m_vPos.y = matPartcle._42;
        (*it)->m_vPos.z = matPartcle._43;
    }

	for ( std::vector<ParasiteMesh>::iterator it = m_vecChild.begin(); it != m_vecChild.end(); it++ )	
	{
		(*it).first->AdjustParticlePos();
	}
}

void GCObject::SetCartoon( EGCCartoonTexture eCartoon_ )
{
    m_eCartoon = eCartoon_;
    std::vector< ParasiteMesh >::const_iterator vit = m_vecChild.begin();
    for( ; vit != m_vecChild.end() ; ++vit ) { 
        vit->first->SetCartoon( eCartoon_ );   
    }
}

void GCObject::QualityMeshTechChange() const
{
    //그래픽레벨 최상일때 기본설정된걸로 교체
    if ( g_MyD3D->m_KGCOption.GetQualityCount() == 6 )
    {
        m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique = m_vecMeshObject[ms_uiMeshIndex]->m_iOriginalTechnique;
    }
    else //그래픽레벨이 최상이 아닐때는 외각선을 그리지 않는다.
    {
        if( m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique == EGCShaderTechnique::CARTOON_BLACKEDGE)
        {
            m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique = EGCShaderTechnique::CARTOON_NOEDGE;
        }
        else if( m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique == EGCShaderTechnique::CARTOON_EDGE_ALPHA)
        {
            m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique = EGCShaderTechnique::CARTOON_NOEDGE_ALPHA;
        }
        else if( m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique == EGCShaderTechnique::CARTOON_EDGE_ALPHA_BRIGHT)
        {
            m_vecMeshObject[ms_uiMeshIndex]->m_iTechnique = EGCShaderTechnique::CARTOON_NOEDGE_ALPHA_BRIGHT;
        }
    }
}

bool GCObject::IsToggleSecondBaseMesh( const std::string strID_ ) const
{
    std::vector< GCMeshObject* >::const_iterator vit;
    for( vit = m_vecMeshObject.begin(); vit != m_vecMeshObject.end(); vit++ )
    {
        if( !(*vit)->m_bExtraMeshObject && (*vit)->m_bSecondBaseMesh && (*vit)->m_strName == strID_ ) { 
            return (*vit)->m_bRender;
        }
    }

    return false;
}
