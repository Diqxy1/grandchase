//#include <boost/bind.hpp>

template< class T0 >
class KFindFileName
{
public:
	KFindFileName(std::string strComp) : m_strComp(strComp) {}

	bool operator()(const CRenderPartPair<T0> comp) const
	{
		return m_strComp.compare(comp.first) == 0;
	}

private:
	std::string m_strComp;
};

// T0 : LayerPart, T1 Manager
template <class T0, class T1>
KLayer<T0, T1>::KLayer(T1* pManager) : m_fLayerSizeX(0), m_fLayerSizeY(0), m_bShow(true),
m_iLayerNum(0), m_pVBPartsRender(NULL), m_pIBPartsRender(NULL),
m_pLayerManager(NULL)
{
	init();
	m_pLayerManager = pManager;
}

template <class T0, class T1>
KLayer<T0, T1>::~KLayer(void)
{
	// Jaeho 2007.10.10
	// 던전개편 시 맵스크립트가 여러번 로딩되는데
	// 버택스, 인덱스 Parts가 Release 되지 않아 메모리가 계속 증가해서 아래코드 추가.
	if (!m_vecPart.empty())
	{
		g_pGCDeviceManager->ReleaseDevice(m_pVBPartsRender);
		g_pGCDeviceManager->ReleaseDevice(m_pIBPartsRender);
	}

	clear();
	m_pLayerManager = NULL;
}

template <class T0, class T1>
void KLayer<T0, T1>::clear()
{
	for (int i = 0; i < (int)m_vecPart.size(); ++i)
		SAFE_DELETE(m_vecPart[i]);

	m_vecPart.clear();
	m_vecRenderPart.clear();
	m_vecDPInfo.clear();
}

template <class T0, class T1>
void KLayer<T0, T1>::init()
{
	m_vecDPInfo.clear();
	m_vecRenderPart.clear();
	m_vecPart.clear();
}

template <class T0, class T1>
void KLayer<T0, T1>::AddPartsErrFunc(std::string partName_)
{
	ASSERT(partName_.c_str() && !"Invalid Part Add");
}

template <class T0, class T1>
void KLayer<T0, T1>::SetBufferType1()
{
	if (m_vecPart.empty())
		return;

	m_vecDPInfo.clear();

	VBIBIndexData kIndexData;

	SLVERTEX* pVertex = NULL;
	m_pVBPartsRender->VideoLock(0, (MAX_PART_VERTEX - 1) * sizeof(SLVERTEX),
		(BYTE**)&pVertex, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);
	WORD* pIndex = NULL;
	m_pIBPartsRender->VideoLock(0, (MAX_PART_INDEX - 1) * sizeof(WORD),
		(BYTE**)&pIndex, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);

	std::vector< CRenderPartPair<T0> >::iterator vitRenderPart;
	std::vector< T0* >::reverse_iterator vitPart;
	for (vitRenderPart = m_vecRenderPart.begin(); vitRenderPart != m_vecRenderPart.end(); ++vitRenderPart)
	{
		std::vector< T0* >& vecRenderPart = (*vitRenderPart).second;
		if (vecRenderPart.empty())
			continue;

		vitPart = vecRenderPart.rbegin();
		GCDeviceTexture* pTexture = (*vitPart)->GetTexture();
		ASSERT(pTexture != NULL);

		for (; vitPart != vecRenderPart.rend(); ++vitPart)
		{
			T0* pPart = (*vitPart);
			if (pPart->GetShow() == true && pPart->CheckCulling() == true)
			{
				const SLVERTEX* pScrolledVertex = pPart->GetRenderVertex();
				ASSERT(pScrolledVertex != NULL);
				if (pScrolledVertex == NULL)
					continue;

				SetVBIBIndexData(pScrolledVertex, kIndexData, pVertex, pIndex);
				if (pPart->GetMoveable() == true)
				{
					if (pPart->GetRepeat_X() == true)
					{
						if (pScrolledVertex[1].Pos.x > m_fLayerSizeX)
						{
							T0 kPart;
							kPart.CopyMember(pPart);
							KLayerRect<float> kRect;
							kPart.GetLayerRect(kRect);
							kRect.m_X = pScrolledVertex[0].Pos.x - m_fLayerSizeX;
							kPart.SetLayer(pPart->GetLayer());
							kPart.SetLayerPos(kRect);
							kPart.SetZoffset(pPart->GetZoffset());
							kPart.Flush();
							SetVBIBIndexData(kPart.GetRenderVertex(), kIndexData, pVertex, pIndex);
						}
						else if (pScrolledVertex[0].Pos.x < 0.0f)
						{
							T0 kPart;
							kPart.CopyMember(pPart);
							KLayerRect<float> kRect;
							kPart.GetLayerRect(kRect);
							kRect.m_X = m_fLayerSizeX + pScrolledVertex[0].Pos.x;
							kPart.SetLayer(pPart->GetLayer());
							kPart.SetLayerPos(kRect);
							kPart.SetZoffset(pPart->GetZoffset());
							kPart.Flush();
							SetVBIBIndexData(kPart.GetRenderVertex(), kIndexData, pVertex, pIndex);
						}
					}

					if (pPart->GetRepeat_Y() == true)
					{
						if (pScrolledVertex[0].Pos.y > m_fLayerSizeY)
						{
							T0 kPart;
							kPart.CopyMember(pPart);
							KLayerRect<float> kRect;
							kPart.GetLayerRect(kRect);
							kRect.m_Y = -kRect.m_Height + (pScrolledVertex[0].Pos.y - m_fLayerSizeY);
							kPart.SetLayer(pPart->GetLayer());
							kPart.SetLayerPos(kRect);
							kPart.SetZoffset(pPart->GetZoffset());
							kPart.Flush();
							SetVBIBIndexData(kPart.GetRenderVertex(), kIndexData, pVertex, pIndex);
						}
						else if (pScrolledVertex[2].Pos.y < 0.0f)
						{
							T0 kPart;
							kPart.CopyMember(pPart);
							KLayerRect<float> kRect;
							kPart.GetLayerRect(kRect);
							kRect.m_Y = m_fLayerSizeY + pScrolledVertex[2].Pos.y;
							kPart.SetLayer(pPart->GetLayer());
							kPart.SetLayerPos(kRect);
							kPart.SetZoffset(pPart->GetZoffset());
							kPart.Flush();
							SetVBIBIndexData(kPart.GetRenderVertex(), kIndexData, pVertex, pIndex);
						}
					}
				}
				ASSERT(kIndexData.dwSumRenderSize <= MAX_RENDERING_PARTS_PER_LAYER);
			}
		}

		if (kIndexData.dwRenderSize != 0)
		{
			DPInfo dpInfo;
			dpInfo.m_pTexture = pTexture;
			dpInfo.m_dwRenderSize = kIndexData.dwRenderSize;
			dpInfo.m_dwVBIndex = kIndexData.dwVBIndex;
			dpInfo.m_dwIBIndex = kIndexData.dwIBIndex;

			m_vecDPInfo.push_back(dpInfo);

			kIndexData.dwVBIndex = kIndexData.dwVertexCount;
			kIndexData.dwIBIndex = kIndexData.dwIndexCount;
			kIndexData.dwRenderSize = 0;
		}
	}

	m_pVBPartsRender->VideoUnlock();
	m_pIBPartsRender->VideoUnlock();
}

template <class T0, class T1>
void KLayer<T0, T1>::SetVBIBIndexData(IN const SLVERTEX* pScrolledVertex, OUT VBIBIndexData& kIndexData, OUT SLVERTEX* pVertex, OUT WORD* pIndex)
{
	pVertex[kIndexData.dwVertexCount] = pScrolledVertex[0];
	pVertex[kIndexData.dwVertexCount + 1] = pScrolledVertex[1];
	pVertex[kIndexData.dwVertexCount + 2] = pScrolledVertex[2];
	pVertex[kIndexData.dwVertexCount + 3] = pScrolledVertex[3];
	kIndexData.wVertexIndex = static_cast<WORD>(kIndexData.dwVertexCount);
	pIndex[kIndexData.dwIndexCount] = kIndexData.wVertexIndex;
	pIndex[kIndexData.dwIndexCount + 1] = kIndexData.wVertexIndex + 1;
	pIndex[kIndexData.dwIndexCount + 2] = kIndexData.wVertexIndex + 2;
	pIndex[kIndexData.dwIndexCount + 3] = kIndexData.wVertexIndex + 2;
	pIndex[kIndexData.dwIndexCount + 4] = kIndexData.wVertexIndex + 1;
	pIndex[kIndexData.dwIndexCount + 5] = kIndexData.wVertexIndex + 3;
	kIndexData.dwVertexCount += 4;
	kIndexData.dwIndexCount += 6;
	kIndexData.dwRenderSize++;
	kIndexData.dwSumRenderSize++;
}

template <class T0, class T1>
HRESULT KLayer<T0, T1>::Render()
{
	HRESULT hr;

	if (m_vecPart.empty() || m_bShow == false)
		return E_FAIL;

	g_pd3dDevice->SetFVF(SLVERTEX::FVF);
	g_pd3dDevice->SetStreamSource(0, m_pVBPartsRender->GetBuffer(), 0, sizeof(SLVERTEX));
	g_pd3dDevice->SetIndices(m_pIBPartsRender->GetBuffer());
	for (int i = 0; i < (int)m_vecDPInfo.size(); ++i)
	{
		DPInfo& dpInfo = m_vecDPInfo[i];
		ASSERT(dpInfo.m_pTexture != NULL);
		dpInfo.m_pTexture->SetDeviceTexture();

		// DP
		if (FAILED(hr = g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, dpInfo.m_dwVBIndex, dpInfo.m_dwRenderSize * 4, dpInfo.m_dwIBIndex, dpInfo.m_dwRenderSize * 2)))
		{
			return hr;
		}
	}

	return S_OK;
}

template <class T0, class T1>
void KLayer<T0, T1>::SetZoffset()
{
	if (m_vecPart.empty())
		return;

	float fZterm = (float)(5.0f / static_cast<float>(m_vecPart.size()));

	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		float fZoffset = fZterm * ((int)m_vecPart.size() - i);
		m_vecPart[i]->SetZoffset(fZoffset);
	}
}