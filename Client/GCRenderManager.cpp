#include "stdafx.h"

#include "GCDeviceLib\/GCDeviceShader.h"
#include ".\gcrendermanager.h"

//

//
#include "MyD3D.h"

char* KGCRenderManager::ms_strTechnique[] = { "CartoonBlackEdge", "CartoonNoEdge" , "NoCartoonNoEdge", "ResultModel", "CartoonNoEdgeAlpha", "CartoonEdgeAlpha", "CartoonNoEdgeAlphaBright", "CartoonEdgeAlphaBright" };

extern LPDIRECT3DDEVICE9        g_pd3dDevice;
KGCRenderManager * g_RenderManager;

ImplementSingleton(KGCRenderManager)

RenderFunc            KGCRenderManager::ms_pRenderFunc = NULL;

KGCRenderManager::KGCRenderManager(void)
{
	g_RenderManager = this;

	m_mapObjects.clear();
	m_dwIDCount = 0;
	m_iTechniqueID = NUM_TECHNIQUE;
	m_iObjectNum = 0;
	m_pDeviceShader = NULL;
}

KGCRenderManager::~KGCRenderManager(void)
{
	SAFE_RELEASE(m_pDeviceShader);
}

void KGCRenderManager::SetDeviceShader(GCDeviceShader* pDeviceShader_)
{
	m_pDeviceShader = pDeviceShader_;
}
void KGCRenderManager::SetRenderTech(int iTechniqueID_)
{
	m_iTechniqueID = iTechniqueID_;
}

int KGCRenderManager::GetRenderTech()
{
	return m_iTechniqueID;
}

// NUM_TECHNIQUE면 그래픽 품질의 테크닉을 따른다.
// ID는 0xFFFFFFFF까지 가는일이 없다!!!
GCObject* KGCRenderManager::CreateObject(int iTechniqueID_)
{
	GCObject* pObejct = new GCObject();
	pObejct->SetID(m_dwIDCount++);
	if (m_dwIDCount == 0xFFFFFFFF)
	{
		m_dwIDCount = 0;
	}

	if (iTechniqueID_ == -1)
		iTechniqueID_ = NUM_TECHNIQUE;

	m_mapObjects[iTechniqueID_].push_back(pObejct);
	m_iObjectNum++;
	return pObejct;
}

void KGCRenderManager::AddObject(GCObject* pObj_, int iTechniqueID_)
{
	if (!pObj_)
		return;

	pObj_->SetID(m_dwIDCount++);
	if (m_dwIDCount == 0xFFFFFFFF)
	{
		m_dwIDCount = 0;
	}

	if (iTechniqueID_ == -1)
		iTechniqueID_ = NUM_TECHNIQUE;

	m_mapObjects[iTechniqueID_].push_back(pObj_);
	m_iObjectNum++;

	for (unsigned int i = 0; i < pObj_->m_vecChild.size(); i++)
	{
		AddObject(pObj_->m_vecChild[i].first, iTechniqueID_);
	}
}

bool KGCRenderManager::RemoveObject(GCObject* pObj_)
{
	if (pObj_ == NULL)
		return false;

	std::map< int, std::vector<GCObject* > >::iterator mit;
	bool bRet = false;
	for (mit = m_mapObjects.begin(); mit != m_mapObjects.end(); mit++)
	{
		for (int i = (int)mit->second.size() - 1; i >= 0; i--)
		{
			if (pObj_ == mit->second[i])
			{
				mit->second.erase(mit->second.begin() + i);
				m_iObjectNum--;
				bRet = true;
			}
		}
	}

	for (unsigned int i = 0; i < pObj_->m_vecChild.size(); i++)
	{
		RemoveObject(pObj_->m_vecChild[i].first);
	}

	return bRet;
}

void KGCRenderManager::DeleteAll()
{
	std::map< int, std::vector<GCObject* > >::iterator mit;
	for (mit = m_mapObjects.begin(); mit != m_mapObjects.end(); mit++)
	{
		for (int i = (int)mit->second.size() - 1; i >= 0; i--)
		{
			SAFE_DELETE(mit->second[i]);
			mit->second.erase(mit->second.begin() + i);
		}
	}
	m_iObjectNum = 0;
}

void KGCRenderManager::RenderFixedPipe(bool bCamera_, GCObject* pObj_, int iTechniqueID_, int iLayer_)
{
	int iCurrentTechnique;

	//D3DXMATRIX matIdentity;
	//D3DXMatrixIdentity( &matIdentity );
	//g_pd3dDevice->SetTransform( D3DTS_WORLD , &matIdentity );

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_pGCDeviceManager2->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);		// Matrix Palette 사용
	g_pGCDeviceManager2->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_1WEIGHTS);		// Blend Weight는 2개
	g_pGCDeviceManager2->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (g_MyD3D->m_KGCOption.IsLinearFilter())
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}
	else
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	}

	if (pObj_)
	{
		if (iLayer_ != pObj_->m_iLayer)
		{
			g_pGCDeviceManager2->PopState();
			return;
		}

		iCurrentTechnique = (iTechniqueID_ == NUM_TECHNIQUE) ? m_iTechniqueID : iTechniqueID_;

		SetRenderStateFixedPipe(iCurrentTechnique);
		pObj_->SetMotionMatrix(bCamera_, NULL);
		pObj_->Render(iCurrentTechnique, bCamera_);
		if (pObj_->GetRender())
		{
			std::vector< ParasiteMesh >::iterator vit;
			for (vit = pObj_->m_vecChild.begin(); vit != pObj_->m_vecChild.end(); ++vit)
			{
				RenderFixedPipe(bCamera_, vit->first, iTechniqueID_, iLayer_);
			}
		}
	}
	else
	{
		std::map< int, std::vector<GCObject* > >::iterator mit;

		for (mit = m_mapObjects.begin(); mit != m_mapObjects.end(); mit++)
		{
			iCurrentTechnique = (mit->first == NUM_TECHNIQUE) ? m_iTechniqueID : mit->first;

			SetRenderStateFixedPipe(iCurrentTechnique);
			for (int j = 0; j < (int)mit->second.size(); j++)
			{
				if (iLayer_ != mit->second[j]->m_iLayer)
					continue;
				if (mit->second[j]->SetMotionMatrix(bCamera_, NULL))
				{
					mit->second[j]->Render(iCurrentTechnique, bCamera_);
					if (mit->second[j]->GetRender())
					{
						std::vector< ParasiteMesh >::iterator vit;
						for (vit = mit->second[j]->m_vecChild.begin(); vit != mit->second[j]->m_vecChild.end(); ++vit)
						{
							RenderFixedPipe(bCamera_, vit->first, iTechniqueID_, iLayer_);
						}
					}
				}
			}
		}
	}
	g_pGCDeviceManager2->PopState();
}

void KGCRenderManager::RenderVS(bool bCamera_, GCObject* pObj_, int iTechniqueID_, int iLayer_)
{
	D3DXMATRIX matProj, matView, matViewProj;
	int iCurrentTechnique;

	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	matViewProj = matView * matProj;

	//m_pDeviceShader->GetEffect()->SetMatrix( "g_mViewProj", &matViewProj );

	g_pGCDeviceManager2->PushState();
	{
		if (g_MyD3D->m_KGCOption.IsLinearFilter())
		{
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		}
		else
		{
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		}

		if (pObj_)
		{
			if (iLayer_ != pObj_->m_iLayer)
			{
				g_pGCDeviceManager2->PopState();
				return;
			}

			// 오브젝트가.. 나는 이 테크닉으로만 그리고 싶다고 한다면 그렇게 하라고 하쟈 ㅎ
			if (pObj_->GetTechnique() != -1)
				iTechniqueID_ = pObj_->GetTechnique();

			iCurrentTechnique = (iTechniqueID_ == NUM_TECHNIQUE) ? m_iTechniqueID : iTechniqueID_;
			m_pDeviceShader->GetEffect()->SetTechnique(ms_strTechnique[iCurrentTechnique]);

			if (pObj_->SetMotionMatrix(bCamera_, &matViewProj))
			{
				pObj_->Render(iCurrentTechnique, bCamera_);
				if (pObj_->GetRender())
				{
					std::vector< ParasiteMesh >::iterator vit;
					for (vit = pObj_->m_vecChild.begin(); vit != pObj_->m_vecChild.end(); ++vit)
					{
						RenderVS(bCamera_, vit->first, iTechniqueID_, iLayer_);
					}
				}
			}
		}
		else
		{
			std::map< int, std::vector<GCObject* > >::iterator mit;

			for (mit = m_mapObjects.begin(); mit != m_mapObjects.end(); mit++)
			{
				iCurrentTechnique = (mit->first == NUM_TECHNIQUE) ? m_iTechniqueID : mit->first;

				m_pDeviceShader->GetEffect()->SetTechnique(ms_strTechnique[iCurrentTechnique]);

				for (int j = 0; j < (int)mit->second.size(); j++)
				{
					if (iLayer_ != mit->second[j]->m_iLayer)
						continue;
					if (mit->second[j]->SetMotionMatrix(bCamera_, &matViewProj))
					{
						mit->second[j]->Render(iCurrentTechnique, bCamera_);

						if (mit->second[j]->GetRender())
						{
							std::vector< ParasiteMesh >::iterator vit;
							for (vit = mit->second[j]->m_vecChild.begin(); vit != mit->second[j]->m_vecChild.end(); ++vit)
							{
								RenderVS(bCamera_, vit->first, iTechniqueID_, iLayer_);
							}
						}
					}
				}
			}
		}
	}
	g_pGCDeviceManager2->PopState();
}

void KGCRenderManager::SetOutlineWidth(float fWidth_)
{
	m_pDeviceShader->GetEffect()->SetFloat("fOutLineWidth", fWidth_);
}

float KGCRenderManager::GetOutlineWidth(void)
{
	float fWidth_ = 0;
	m_pDeviceShader->GetEffect()->GetFloat("fOutLineWidth", &fWidth_);
	return fWidth_;
}

void KGCRenderManager::SetUsingVS(bool bUsingVS_)
{
	if (bUsingVS_)
	{
		ms_pRenderFunc = &KGCRenderManager::RenderVS;
	}
	else
	{
		ms_pRenderFunc = &KGCRenderManager::RenderFixedPipe;
	}
	GCObject::SetUsingVS(bUsingVS_);

	g_pGraphicsHelper->LoadCartoonTexture(bUsingVS_);
}
bool KGCRenderManager::GetUsingVS()
{
	if (ms_pRenderFunc == &KGCRenderManager::RenderVS)
	{
		return true;
	}
	else
		return false;
}

void KGCRenderManager::SetRenderStateFixedPipe(int iTech_)
{
	switch (iTech_)
	{
	case EGCShaderTechnique::CARTOON_BLACKEDGE:
	{
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		g_pGCDeviceManager2->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
		g_pGCDeviceManager2->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		g_pGCDeviceManager2->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

		g_pGCDeviceManager2->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		g_pGCDeviceManager2->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
		//SetShellMatrix( 0.26f );
		break;
	}
	case EGCShaderTechnique::CARTOON_NOEDGE:
	{
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		g_pGCDeviceManager2->SetRenderState(D3DRS_LIGHTING, TRUE);
		break;
	}
	case EGCShaderTechnique::NOCARTOON_NOEDGE:
	{
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		break;
	}
	case EGCShaderTechnique::RESULT_MODEL:
	{
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
		g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		break;
	}
	default:
		g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		break;
	}
}

void KGCRenderManager::Render(bool bCamera_ /*= true*/, GCObject* pObj_ /*= NULL*/, int iTech /*= NUM_TECHNIQUE*/, int iLayer_ /*= GC_LAYER_CHAR*/)
{
#ifdef _DEBUG
	//if (GetAsyncKeyState('V'))
	//{
	//    SetUsingVS( true );
	//}
	//if (GetAsyncKeyState('F'))
	//{
	//    SetUsingVS( false );
	//}
	if (GetAsyncKeyState('R'))
	{
		m_pDeviceShader->Reload();
	}
#endif
	g_pGCDeviceManager2->Flush();
	(this->*ms_pRenderFunc)(bCamera_, pObj_, iTech, iLayer_);
}

void KGCRenderManager::ForcedRenderVS(bool bCamera_ /*= true */, GCObject* pObj_ /*= NULL*/, int iTech /*= NUM_TECHNIQUE*/, int iLayer_ /*= GC_LAYER_CHAR */)
{
	RenderVS(bCamera_, pObj_, iTech, iLayer_);
}