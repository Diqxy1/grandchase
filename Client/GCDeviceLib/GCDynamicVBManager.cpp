#include    "GCDynamicVBManager.h"


#define MAX_NUM_EXCEPTIONS_IN_ONE_FRAME  2

static inline unsigned _ConvertToNumVertices(D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumPrimitives)
{
	if (uNumPrimitives == 0)
		return 0;
	switch (d3dPrimitiveType)
	{
	case D3DPT_POINTLIST:
		return uNumPrimitives;
	case D3DPT_LINELIST:
		return uNumPrimitives * 2;
	case D3DPT_LINESTRIP:
		return uNumPrimitives + 1;
	case D3DPT_TRIANGLELIST:
		return uNumPrimitives * 3;
	case D3DPT_TRIANGLESTRIP:
	case D3DPT_TRIANGLEFAN:
		return uNumPrimitives + 2;
		//case D3DPT_QUAD:
		//    return  uNumPrimitives * 4;
	} //switch
	return 0;
} //_ConvertToNumVertices()


static inline unsigned _ConvertToNumPrimitives(D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumVertices)
{
	switch (d3dPrimitiveType)
	{
	case D3DPT_POINTLIST:
		return uNumVertices;
	case D3DPT_LINELIST:
		return uNumVertices / 2;
	case D3DPT_LINESTRIP:
		return (uNumVertices > 1) ? uNumVertices - 1 : 0;
	case D3DPT_TRIANGLELIST:
		return uNumVertices / 3;
	case D3DPT_TRIANGLESTRIP:
	case D3DPT_TRIANGLEFAN:
		return (uNumVertices > 2) ? uNumVertices - 2 : 0;
		//case D3DPT_QUAD:
		//    return uNumVertices  / 4;
	} //switch

	return 0;
} //_ConverToNumPrimitives()


KGCDynamicVBManager::KGCDynamicVBManager()
{
	CustomInitialize(true);
} //KGCDynamicVBManager::KGCDynamicVBManager()

/*virtual*/
KGCDynamicVBManager::~KGCDynamicVBManager()
{
	CustomFinalize(true);
} //KGCDynamicVBManager::~KGCDynamicVBManager()


void KGCDynamicVBManager::CustomInitialize(bool bInConstructor_)
{
	m_pQuadIndices = nullptr;

	//g_pd3dDevice->CreateIndexBuffer( 4 * m_uNumVertices * m_dwVertexStride, D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pQuadIndices, NULL );
	g_pd3dDevice->CreateIndexBuffer(sizeof(WORD) * max(WORLD_VB_SIZE, SCREEN_VB_SIZE) * 6, D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pQuadIndices, nullptr);
	WORD* index;
	m_pQuadIndices->Lock(0, 0, (void**)&index, 0);
	{
		for (int i = 0; i < max(WORLD_VB_SIZE, SCREEN_VB_SIZE); i++)
		{
			index[0] = 4 * i + 0;
			index[1] = 4 * i + 1;
			index[2] = 4 * i + 2;
			index[3] = 4 * i + 1;
			index[4] = 4 * i + 3;
			index[5] = 4 * i + 2;
			index += 6;
		}
	}
	m_pQuadIndices->Unlock();

	m_eDynamicVBMode = DYNAMIC_VB_MODE_MANAGED;
	m_uNumMaxPrimitiveCount = 0;
	m_pDrawPrimitiveFn = nullptr;
	m_uNumExceptionsOccurred = 0;

	BOOST_FOREACH(DynamicVBInfo & dvbInfo, m_aDynamicVBInfo)
	{
		dvbInfo.Reset();
	} //BOOST_FOREACH()

	//#ifdef  QUERY_DYNAMIC_VB_OPT_TO_SERVER

	//m_eDynamicVBMode = DYNAMIC_VB_MODE_MANAGED;

	//#else   QUERY_DYNAMIC_VB_OPT_TO_SERVER
	//
	//m_eDynamicVBMode = DYNAMIC_VB_MODE_INDEXED_DYNAMICVB;
	m_eDynamicVBMode = DYNAMIC_VB_MODE_DYNAMICVB;

#ifdef KTDGDEVICEFONT_SIZE_CACHE
	DWORD dwMajor = 0;
	DWORD dwMinor = 0;
	GetOSVersion(dwMajor, dwMinor);
	if (dwMajor == 5 && (dwMinor == 0 || dwMinor == 1))
	{
		m_eDynamicVBMode = DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH;
		//m_eDynamicVBMode = DYNAMIC_VB_MODE_INDEXED_DYNAMICVB_WITH_TRY_CATCH;
	}
#endif
	//
	//#endif  QUERY_DYNAMIC_VB_OPT_TO_SERVER

	RegisterVBType(DVB_TYPE_XYZ_DIFFUSE, D3DFVF_XYZ_DIFFUSE, 1024); // D3DFVF_SLASH_TRACE, D3DFVF_LINE_MAP_VERTEX
	RegisterVBType(DVB_TYPE_XYZ_DIFFUSE_TEX1, D3DFVF_XYZ_DIFFUSE_TEX1, 1024);
	// D3DFVF_SLASH_TRACE_TEXTURED, D3DFVF_UNIT_SHADOW_VERTEX, SLVERTEX::FVF, D3DFVF_PARTICLE
	RegisterVBType(DVB_TYPE_XYZ_TEX1, D3DFVF_XYZ_TEX1, 1024); // D3DFVF_RENDERTARGET_VERTEX
	RegisterVBType(DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DFVF_XYZRHW_DIFFUSE_TEX1, 1024);
	// D3DFVF_LENSFLARE, D3DFVF_VERTEX_UI, D3DFVF_TEXTURE_RHW_VERTEX, S2DUIVertex::FVF, D3DFVF_RENDERTARGET_RHW_VERTEX, D3DFVF_PARTICLE_RHW
	// D3DFVF_DRAWFACE_RHW_VERTEX

	//#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	//    if ( DXUTGetDeviceObjectsReset() == true )
	//    {
	//        if ( g_pKTDXApp != NULL && g_pKTDXApp->GetDevice() != NULL )
	//            g_pKTDXApp->GetDevice()->EvictManagedResources();
	//        OnResetDevice();
	//    }//if
	//#endif

	//m_hCheckLostDevice = g_pd3dDevice->TestCooperativeLevel();

	//OnResetDevice();
} //KGCDynamicVBManager::CustomInitialize()


/*virtual*/
void KGCDynamicVBManager::CustomFinalize(bool bInDestructor_)
{
	BOOST_FOREACH(DynamicVBInfo & dvbInfo, m_aDynamicVBInfo)
	{
		dvbInfo.Reset();
	} //BOOST_FOREACH()

	SAFE_RELEASE(m_pQuadIndices);
} //KGCDynamicVBManager::CustomFinalize()


bool KGCDynamicVBManager::SetDynamicVBMode(EDYNAMIC_VB_MODE eDynamicVBMode)
{
	if (eDynamicVBMode < EDYNAMIC_VB_MODE(0)
		|| eDynamicVBMode >= DYNAMIC_VB_MODE_NUMS)
		return false;

	if (m_eDynamicVBMode == eDynamicVBMode)
		return true;

	OnLostDevice();

	m_eDynamicVBMode = eDynamicVBMode;

	if (g_pd3dDevice != nullptr)
		g_pd3dDevice->EvictManagedResources();

	OnResetDevice();

	return true;
} //KGCDynamicVBManager::SetDynamicVBMode()


/*virtual*/
HRESULT KGCDynamicVBManager::OnFrameMove(float fElapsedTime)
{
	return S_OK;
} //KGCDynamicVBManager::OnFrameMove()


/*virtual*/
void KGCDynamicVBManager::OnResetDevice()
{
	m_uNumMaxPrimitiveCount = 0;

	if (g_pd3dDevice != nullptr)
	{
		D3DCAPS9 caps;
		ZeroMemory(&caps, sizeof(caps));
		HRESULT hr = g_pd3dDevice->GetDeviceCaps(&caps);
		if (SUCCEEDED(hr))
		{
			m_uNumMaxPrimitiveCount = caps.MaxPrimitiveCount;
		} //if
	} //if

	BOOST_FOREACH(DynamicVBInfo & dvbInfo, m_aDynamicVBInfo)
	{
		dvbInfo.OnResetDevice(m_eDynamicVBMode);
	} //BOOST_FOREACH()

	switch (m_eDynamicVBMode)
	{
	case DYNAMIC_VB_MODE_MANAGED:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveManaged;
		break;
	case DYNAMIC_VB_MODE_PRIMITIVEUP:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveUP;
		break;
	case DYNAMIC_VB_MODE_DYNAMICVB:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveDynamicVB;
		break;
	case DYNAMIC_VB_MODE_INDEXED_DYNAMICVB:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveDynamicVB_Quad;
		break;
#ifdef KTDGDEVICEFONT_SIZE_CACHE
	case DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveDynamicVBWithTryCatch;
		break;
	case DYNAMIC_VB_MODE_INDEXED_DYNAMICVB_WITH_TRY_CATCH:
		m_pDrawPrimitiveFn = &KGCDynamicVBManager::_DrawPrimitiveDynamicVBWithTryCatch_Quad;
		break;
#endif
	} //switch

	//return  S_OK;
} //KGCDynamicVBManager::OnResetDevice()


/*virtual*/
void KGCDynamicVBManager::OnLostDevice()
{
	m_pDrawPrimitiveFn = nullptr;


	BOOST_FOREACH(DynamicVBInfo & dvbInfo, m_aDynamicVBInfo)
	{
		dvbInfo.OnLostDevice(m_eDynamicVBMode);
	} //BOOST_FOREACH()

	//return  S_OK;
} //KGCDynamicVBManager::OnLostDevice()

/*static*/
void KGCDynamicVBManager::GetOSVersion(DWORD& dwOutMajor, DWORD& dwOutMinor)
{
	dwOutMajor = 0;
	dwOutMinor = 0;
	OSVERSIONINFOW info;
	info.dwOSVersionInfoSize = sizeof(info);
	BOOL bRet = GetVersionExW(&info);
	if (bRet)
	{
		dwOutMajor = info.dwMajorVersion;
		dwOutMinor = info.dwMinorVersion;
	} //if
} //KGCDynamicVBManager::GetOSVersion()


bool KGCDynamicVBManager::RegisterVBType(EDYNAMIC_VB_TYPE eType, DWORD dwFVF, unsigned uNumVertices, bool bGenVB)
{
	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END)
		return false;
	if (dwFVF == 0 || uNumVertices == 0)
		return false;

	DWORD dwVertexStride = D3DXGetFVFVertexSize(dwFVF);

	ASSERT(dwVertexStride != 0);
	if (dwVertexStride == 0)
		return false;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == true)
		return false;

	dvbInfo.m_uNumVertices = uNumVertices;
	dvbInfo.m_dwFVF = dwFVF;
	dvbInfo.m_dwVertexStride = dwVertexStride;
	dvbInfo.m_uCurVertexIndex = 0;

	if (bGenVB == true)
	{
		dvbInfo.OnResetDevice(m_eDynamicVBMode);
	} //if

	return true;
} //KGCDynamicVBManager::RegisterVBType()


void KGCDynamicVBManager::_DrawPrimitiveManaged(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumPrimitives, const void* pData, unsigned uNumDraws) throw()
{
	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumPrimitives == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false || dvbInfo.m_pVB == nullptr)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumPrimitives > m_uNumMaxPrimitiveCount)
		uNumPrimitives = m_uNumMaxPrimitiveCount;

	unsigned uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
	if (uNumVertices == 0)
		return;
	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumVertices > dvbInfo.m_uNumVertices)
	{
		uNumVertices = dvbInfo.m_uNumVertices;
		uNumPrimitives = _ConvertToNumPrimitives(D3DPT_TRIANGLEFAN, uNumVertices);
	} //if

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);
	g_pd3dDevice->SetStreamSource(0, dvbInfo.m_pVB, 0, dvbInfo.m_dwVertexStride);

	void* pLockedVertex = nullptr;
	unsigned uCurVertexIndex = dvbInfo.m_uCurVertexIndex;

	if (uCurVertexIndex + uNumVertices > dvbInfo.m_uNumVertices)
	{
		uCurVertexIndex = 0;
		unsigned uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
		if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
			uNumPrimsPossible = m_uNumMaxPrimitiveCount;
		if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
			uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
		if (uNumPrimsPossible == 0)
			return;

		while (uNumPrimsPossible < uNumPrimitives)
		{
			uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimsPossible);
			ASSERT(uNumVertices > 0 && uNumVertices <= dvbInfo.m_uNumVertices);

			HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
				, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex, 0);
			if (FAILED(hr))
				return;
			ASSERT(pLockedVertex != NULL);
			if (pLockedVertex != nullptr)
				CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
			dvbInfo.m_pVB->Unlock();

			if (uNumDraws == 1)
				g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
			else
			{
				for (unsigned u = 0; u != uNumDraws; u++)
				{
					g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
				} //for
			} //for

			uCurVertexIndex = 0;
			dvbInfo.m_uCurVertexIndex = uCurVertexIndex;
			uNumPrimitives -= uNumPrimsPossible;
			switch (d3dPrimitiveType)
			{
			case D3DPT_POINTLIST:
			case D3DPT_LINESTRIP:
			case D3DPT_TRIANGLESTRIP:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumPrimsPossible;
				break;
			case D3DPT_LINELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 2);
				break;
			case D3DPT_TRIANGLELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 3);
				break;
			default:
				return;
			} //switch

			unsigned uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
			if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
				uNumPrimsPossible = m_uNumMaxPrimitiveCount;
			if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
				uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
			if (uNumPrimsPossible == 0)
				return;
		}

		uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
		ASSERT(uNumVertices <= dvbInfo.m_uNumVertices);
		if (uNumVertices == 0 || uNumVertices > dvbInfo.m_uNumVertices)
			return;
	} //if

	//out:

	HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
		, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex, 0);
	if (FAILED(hr))
		return;
	ASSERT(pLockedVertex != NULL);
	if (pLockedVertex != nullptr)
		CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
	dvbInfo.m_pVB->Unlock();

	if (uNumDraws == 1)
		g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
		} //for
	} //for

	dvbInfo.m_uCurVertexIndex = uCurVertexIndex + uNumVertices;
} //KGCDynamicVBManager::_DrawPrimitiveManaged()


void KGCDynamicVBManager::_DrawPrimitiveUP(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumPrimitives, const void* pData, unsigned uNumDraws) throw()
{
	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumPrimitives == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumPrimitives > m_uNumMaxPrimitiveCount)
		uNumPrimitives = m_uNumMaxPrimitiveCount;

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);

	unsigned uNumPrimsPossible = m_uNumMaxPrimitiveCount;
	if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
		uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
	if (uNumPrimsPossible == 0)
		return;

	while (uNumPrimsPossible < uNumPrimitives)
	{
		if (uNumDraws == 1)
			g_pd3dDevice->DrawPrimitiveUP(d3dPrimitiveType, uNumPrimsPossible, pData, dvbInfo.m_dwVertexStride);
		else
		{
			for (unsigned u = 0; u != uNumDraws; u++)
			{
				g_pd3dDevice->DrawPrimitiveUP(d3dPrimitiveType, uNumPrimsPossible, pData, dvbInfo.m_dwVertexStride);
			} //for
		} //for

		uNumPrimitives -= uNumPrimsPossible;
		switch (d3dPrimitiveType)
		{
		case D3DPT_POINTLIST:
		case D3DPT_LINESTRIP:
		case D3DPT_TRIANGLESTRIP:
			pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumPrimsPossible;
			break;
		case D3DPT_LINELIST:
			pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 2);
			break;
		case D3DPT_TRIANGLELIST:
			pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 3);
			break;
		default:
			return;
		} //switch
		uNumPrimsPossible = m_uNumMaxPrimitiveCount;
		if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
			uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
		if (uNumPrimsPossible == 0)
			return;
	} //if

	if (uNumDraws == 1)
		g_pd3dDevice->DrawPrimitiveUP(d3dPrimitiveType, uNumPrimitives, pData, dvbInfo.m_dwVertexStride);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawPrimitiveUP(d3dPrimitiveType, uNumPrimitives, pData, dvbInfo.m_dwVertexStride);
		} //for
	} //for
}

void KGCDynamicVBManager::_DrawPrimitiveDynamicVB(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumPrimitives, const void* pData,
	unsigned uNumDraws) throw()
{
#pragma region 예외 처리
	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumPrimitives == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false || dvbInfo.m_pVB == nullptr)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumPrimitives > m_uNumMaxPrimitiveCount)
		uNumPrimitives = m_uNumMaxPrimitiveCount;

	unsigned uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
	if (uNumVertices == 0)
		return;
	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumVertices > dvbInfo.m_uNumVertices)
	{
		uNumVertices = dvbInfo.m_uNumVertices;
		uNumPrimitives = _ConvertToNumPrimitives(D3DPT_TRIANGLEFAN, uNumVertices);
	} //if
#pragma endregion

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);
	g_pd3dDevice->SetStreamSource(0, dvbInfo.m_pVB, 0, dvbInfo.m_dwVertexStride);

	void* pLockedVertex = nullptr;
	unsigned uCurVertexIndex = dvbInfo.m_uCurVertexIndex;

	if (uCurVertexIndex + uNumVertices > dvbInfo.m_uNumVertices)
	{
		unsigned uNumPrimsPossible = 0;

		if (uCurVertexIndex >= dvbInfo.m_uNumVertices
			|| d3dPrimitiveType == D3DPT_TRIANGLEFAN)
		{
			uCurVertexIndex = 0;
		}
#pragma region 할당된 버텍스 여유 확인
		uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices - uCurVertexIndex);
		if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
			uNumPrimsPossible = m_uNumMaxPrimitiveCount;
		if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
			uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
		if (uNumPrimsPossible == 0)
		{
			uCurVertexIndex = 0;
			uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
			if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
				uNumPrimsPossible = m_uNumMaxPrimitiveCount;
			if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
				uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
			if (uNumPrimsPossible == 0)
				return;
		}
#pragma endregion

		// 버텍스가 그려야할 버텍스의 크기보다 작다면, 한계량보다 많다면
		while (uNumPrimsPossible < uNumPrimitives)
		{
			uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimsPossible);
			ASSERT(uNumVertices > 0 && uNumVertices <= dvbInfo.m_uNumVertices);

			HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
				, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
				(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
			if (FAILED(hr))
				return;
			ASSERT(pLockedVertex != NULL);
			if (pLockedVertex != nullptr)
				CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
			dvbInfo.m_pVB->Unlock();
			if (uNumDraws == 1)
				g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
			else
			{
				for (unsigned u = 0; u != uNumDraws; u++)
				{
					g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
				} //for
			} //for

			uCurVertexIndex = uCurVertexIndex + uNumVertices;
			dvbInfo.m_uCurVertexIndex = uCurVertexIndex;
			uNumPrimitives -= uNumPrimsPossible;
			switch (d3dPrimitiveType)
			{
			case D3DPT_POINTLIST:
			case D3DPT_LINESTRIP:
			case D3DPT_TRIANGLESTRIP:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumPrimsPossible;
				break;
			case D3DPT_LINELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 2);
				break;
			case D3DPT_TRIANGLELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 3);
				break;
			default:
				return;
			} //switch

			uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices - uCurVertexIndex);
			if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
				uNumPrimsPossible = m_uNumMaxPrimitiveCount;
			if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
				uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
			if (uNumPrimsPossible == 0)
			{
				uCurVertexIndex = 0;
				uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
				if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
					uNumPrimsPossible = m_uNumMaxPrimitiveCount;
				if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
					uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
				if (uNumPrimsPossible == 0)
					return;
			}
		}

		uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
		ASSERT(uNumVertices <= dvbInfo.m_uNumVertices);
		if (uNumVertices == 0 || uNumVertices > dvbInfo.m_uNumVertices)
			return;
	} //if

	//out:

	HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
		, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
		(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
		return;
	ASSERT(pLockedVertex != NULL);
	if (pLockedVertex != nullptr)
		CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
	dvbInfo.m_pVB->Unlock();


	if (uNumDraws == 1)
		g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
		} //for
	} //for
	dvbInfo.m_uCurVertexIndex = uCurVertexIndex + uNumVertices;
} //KGCDynamicVBManager::_DrawPrimitiveDynamicVB()

#ifdef KTDGDEVICEFONT_SIZE_CACHE
void KGCDynamicVBManager::_DrawPrimitiveDynamicVBWithTryCatch(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumPrimitives, const void* pData,
	unsigned uNumDraws) throw()
{
	if (D3DERR_DEVICELOST == m_hCheckLostDevice || D3DERR_DEVICENOTRESET == m_hCheckLostDevice)
	{
		return;
	}

	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumPrimitives == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false || dvbInfo.m_pVB == nullptr)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumPrimitives > m_uNumMaxPrimitiveCount)
		uNumPrimitives = m_uNumMaxPrimitiveCount;

	unsigned uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
	if (uNumVertices == 0)
		return;
	if (d3dPrimitiveType == D3DPT_TRIANGLEFAN && uNumVertices > dvbInfo.m_uNumVertices)
	{
		uNumVertices = dvbInfo.m_uNumVertices;
		uNumPrimitives = _ConvertToNumPrimitives(D3DPT_TRIANGLEFAN, uNumVertices);
	} //if

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);
	g_pd3dDevice->SetStreamSource(0, dvbInfo.m_pVB, 0, dvbInfo.m_dwVertexStride);

	void* pLockedVertex = nullptr;
	unsigned uCurVertexIndex = dvbInfo.m_uCurVertexIndex;

	if (uCurVertexIndex + uNumVertices > dvbInfo.m_uNumVertices)
	{
		unsigned uNumPrimsPossible = 0;

		if (uCurVertexIndex >= dvbInfo.m_uNumVertices
			|| d3dPrimitiveType == D3DPT_TRIANGLEFAN)
		{
			uCurVertexIndex = 0;
		}
		uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices - uCurVertexIndex);
		if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
			uNumPrimsPossible = m_uNumMaxPrimitiveCount;
		if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
			uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
		if (uNumPrimsPossible == 0)
		{
			uCurVertexIndex = 0;
			uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
			if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
				uNumPrimsPossible = m_uNumMaxPrimitiveCount;
			if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
				uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
			if (uNumPrimsPossible == 0)
				return;
		}

		while (uNumPrimsPossible < uNumPrimitives)
		{
			uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimsPossible);
			ASSERT(uNumVertices > 0 && uNumVertices <= dvbInfo.m_uNumVertices);

			HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
				, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
				(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
			if (FAILED(hr))
				return;
			ASSERT(pLockedVertex != NULL);
			bool bExceptionOccurred = false;
			if (pLockedVertex != nullptr)
			{
				bExceptionOccurred = KExceptionSafeUtil::ESCopyMemory(pLockedVertex, pData,
					uNumVertices * dvbInfo.m_dwVertexStride);
			}
			dvbInfo.m_pVB->Unlock();
			if (bExceptionOccurred == true)
			{
				m_uNumExceptionsOccurred++;
				if (m_uNumExceptionsOccurred >= MAX_NUM_EXCEPTIONS_IN_ONE_FRAME)
				{
					m_hCheckLostDevice = g_pd3dDevice->TestCooperativeLevel();
				} //if
				return;
			} //if

			if (uNumDraws == 1)
				g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
			else
			{
				for (unsigned u = 0; u != uNumDraws; u++)
				{
					g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimsPossible);
				} //for
			} //for

			uCurVertexIndex = uCurVertexIndex + uNumVertices;
			dvbInfo.m_uCurVertexIndex = uCurVertexIndex;
			uNumPrimitives -= uNumPrimsPossible;
			switch (d3dPrimitiveType)
			{
			case D3DPT_POINTLIST:
			case D3DPT_LINESTRIP:
			case D3DPT_TRIANGLESTRIP:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumPrimsPossible;
				break;
			case D3DPT_LINELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 2);
				break;
			case D3DPT_TRIANGLELIST:
				pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * (uNumPrimsPossible * 3);
				break;
			default:
				return;
			} //switch

			uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices - uCurVertexIndex);
			if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
				uNumPrimsPossible = m_uNumMaxPrimitiveCount;
			if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
				uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
			if (uNumPrimsPossible == 0)
			{
				uCurVertexIndex = 0;
				uNumPrimsPossible = _ConvertToNumPrimitives(d3dPrimitiveType, dvbInfo.m_uNumVertices);
				if (uNumPrimsPossible > m_uNumMaxPrimitiveCount)
					uNumPrimsPossible = m_uNumMaxPrimitiveCount;
				if (d3dPrimitiveType == D3DPT_TRIANGLESTRIP && uNumPrimsPossible < uNumPrimitives)
					uNumPrimsPossible = (uNumPrimsPossible / 2) * 2;
				if (uNumPrimsPossible == 0)
					return;
			}
		}

		uNumVertices = _ConvertToNumVertices(d3dPrimitiveType, uNumPrimitives);
		ASSERT(uNumVertices <= dvbInfo.m_uNumVertices);
		if (uNumVertices == 0 || uNumVertices > dvbInfo.m_uNumVertices)
			return;
	} //if

	//out:

	HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
		, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
		(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
		return;
	ASSERT(pLockedVertex != NULL);
	bool bExceptionOccurred = false;
	if (pLockedVertex != nullptr)
	{
		bExceptionOccurred = KExceptionSafeUtil::ESCopyMemory(pLockedVertex, pData,
			uNumVertices * dvbInfo.m_dwVertexStride);
	} //if
	dvbInfo.m_pVB->Unlock();
	if (bExceptionOccurred == true)
	{
		m_uNumExceptionsOccurred++;
		if (m_uNumExceptionsOccurred >= MAX_NUM_EXCEPTIONS_IN_ONE_FRAME)
		{
			m_hCheckLostDevice = g_pd3dDevice->TestCooperativeLevel();
		} //if
		return;
	} //if

	if (uNumDraws == 1)
		g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawPrimitive(d3dPrimitiveType, uCurVertexIndex, uNumPrimitives);
		} //for
	} //for
	dvbInfo.m_uCurVertexIndex = uCurVertexIndex + uNumVertices;
} //KGCDynamicVBManager::_DrawPrimitiveDynamicVBWithTryCatch()
#endif

void KGCDynamicVBManager::_DrawPrimitiveDynamicVB_Quad(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumQuads, const void* pData,
	unsigned uNumDraws) throw()
{
	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumQuads == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false || dvbInfo.m_pVB == nullptr)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	unsigned uNumVertices = uNumQuads * 4;
	if (uNumVertices == 0)
		return;

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);
	g_pd3dDevice->SetStreamSource(0, dvbInfo.m_pVB, 0, dvbInfo.m_dwVertexStride);
	g_pd3dDevice->SetIndices(m_pQuadIndices);


	void* pLockedVertex = nullptr;
	unsigned uCurVertexIndex = dvbInfo.m_uCurVertexIndex;
	unsigned uMaxQuadCount = (m_uNumMaxPrimitiveCount / 2) * 2;

	if (uCurVertexIndex + uNumVertices > dvbInfo.m_uNumVertices)
	{
		unsigned uNumQuadsPossible = 0;

		uNumQuadsPossible = (dvbInfo.m_uNumVertices - uCurVertexIndex) / 4;
		if (uNumQuadsPossible > uMaxQuadCount)
			uNumQuadsPossible = uMaxQuadCount;
		if (uNumQuadsPossible == 0)
		{
			uCurVertexIndex = 0;
			uNumQuadsPossible = dvbInfo.m_uNumVertices / 4;
			if (uNumQuadsPossible > uMaxQuadCount)
				uNumQuadsPossible = uMaxQuadCount;
			if (uNumQuadsPossible == 0)
				return;
		}

		while (uNumQuadsPossible < uNumQuads)
		{
			uNumVertices = uNumQuadsPossible * 4;
			ASSERT(uNumVertices > 0 && uNumVertices <= dvbInfo.m_uNumVertices);

			HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
				, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
				(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
			if (FAILED(hr))
				return;
			ASSERT(pLockedVertex != NULL);
			if (pLockedVertex != nullptr)
				CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
			dvbInfo.m_pVB->Unlock();
			if (uNumDraws == 1)
				g_pd3dDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuadsPossible * 2);
			else
			{
				for (unsigned u = 0; u != uNumDraws; u++)
				{
					g_pd3dDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuadsPossible * 2);
				} //for
			} //for

			uCurVertexIndex = uCurVertexIndex + uNumVertices;
			dvbInfo.m_uCurVertexIndex = uCurVertexIndex;
			uNumQuads -= uNumQuadsPossible;
			pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumVertices;

			uNumQuadsPossible = (dvbInfo.m_uNumVertices - uCurVertexIndex) / 4;
			if (uNumQuadsPossible > uMaxQuadCount)
				uNumQuadsPossible = uMaxQuadCount;
			if (uNumQuadsPossible == 0)
			{
				uCurVertexIndex = 0;
				uNumQuadsPossible = dvbInfo.m_uNumVertices / 4;
				if (uNumQuadsPossible > uMaxQuadCount)
					uNumQuadsPossible = uMaxQuadCount;
				if (uNumQuadsPossible == 0)
					return;
			}
		}

		uNumVertices = uNumQuads * 4;
		ASSERT(uNumVertices <= dvbInfo.m_uNumVertices);
		if (uNumVertices == 0 || uNumVertices > dvbInfo.m_uNumVertices)
			return;
	} //if

	//out: -> 사용하지 않아 Warning 발생

	HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
		, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
		(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
		return;
	ASSERT(pLockedVertex != NULL);
	if (pLockedVertex != nullptr)
		CopyMemory(pLockedVertex, pData, uNumVertices * dvbInfo.m_dwVertexStride);
	dvbInfo.m_pVB->Unlock();

	if (uNumDraws == 1)
		g_pd3dDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuads * 2);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuads * 2);
		} //for
	} //for

	dvbInfo.m_uCurVertexIndex = uCurVertexIndex + uNumVertices;
} //CKTDGDynamicVBManager::_DrawPrimitiveDynamicVB()

#ifdef KTDGDEVICEFONT_SIZE_CACHE
void KGCDynamicVBManager::_DrawPrimitiveDynamicVBWithTryCatch_Quad(EDYNAMIC_VB_TYPE eType,
	D3DPRIMITIVETYPE d3dPrimitiveType,
	unsigned uNumQuads, const void* pData,
	unsigned uNumDraws) throw()
{
	if (D3DERR_DEVICELOST == m_hCheckLostDevice || D3DERR_DEVICENOTRESET == m_hCheckLostDevice)
	{
		return;
	}

	if (eType < EDYNAMIC_VB_TYPE(0)
		|| eType >= DVB_END || uNumQuads == 0 || pData == nullptr || uNumDraws == 0)
		return;

	DynamicVBInfo& dvbInfo = m_aDynamicVBInfo[eType];
	if (dvbInfo.IsRegistered() == false || dvbInfo.m_pVB == nullptr)
		return;

	if (g_pd3dDevice == nullptr || m_uNumMaxPrimitiveCount == 0)
		return;

	unsigned uNumVertices = uNumQuads * 4;
	if (uNumVertices == 0)
		return;

	g_pd3dDevice->SetFVF(dvbInfo.m_dwFVF);
	g_pd3dDevice->SetStreamSource(0, dvbInfo.m_pVB, 0, dvbInfo.m_dwVertexStride);
	g_pd3dDevice->SetIndices(m_pQuadIndices);


	void* pLockedVertex = nullptr;
	unsigned uCurVertexIndex = dvbInfo.m_uCurVertexIndex;
	unsigned uMaxQuadCount = (m_uNumMaxPrimitiveCount / 2) * 2;

	if (uCurVertexIndex + uNumVertices > dvbInfo.m_uNumVertices)
	{
		unsigned uNumQuadsPossible = 0;

		uNumQuadsPossible = (dvbInfo.m_uNumVertices - uCurVertexIndex) / 4;
		if (uNumQuadsPossible > uMaxQuadCount)
			uNumQuadsPossible = uMaxQuadCount;
		if (uNumQuadsPossible == 0)
		{
			uCurVertexIndex = 0;
			uNumQuadsPossible = dvbInfo.m_uNumVertices / 4;
			if (uNumQuadsPossible > uMaxQuadCount)
				uNumQuadsPossible = uMaxQuadCount;
			if (uNumQuadsPossible == 0)
				return;
		}

		while (uNumQuadsPossible < uNumQuads)
		{
			uNumVertices = uNumQuadsPossible * 4;
			ASSERT(uNumVertices > 0 && uNumVertices <= dvbInfo.m_uNumVertices);

			HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
				, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
				(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
			if (FAILED(hr))
				return;
			ASSERT(pLockedVertex != NULL);
			bool bExceptionOccurred = false;
			if (pLockedVertex != nullptr)
			{
				bExceptionOccurred = KExceptionSafeUtil::ESCopyMemory(pLockedVertex, pData,
					uNumVertices * dvbInfo.m_dwVertexStride);
			}
			dvbInfo.m_pVB->Unlock();
			if (bExceptionOccurred == true)
			{
				m_uNumExceptionsOccurred++;
				if (m_uNumExceptionsOccurred >= MAX_NUM_EXCEPTIONS_IN_ONE_FRAME)
				{
					m_hCheckLostDevice = g_pd3dDevice->TestCooperativeLevel();
				} //if
				return;
			} //if

			if (uNumDraws == 1)
				g_pd3dDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuadsPossible * 2);
			else
			{
				for (unsigned u = 0; u != uNumDraws; u++)
				{
					g_pd3dDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuadsPossible * 2);
				} //for
			} //for

			uCurVertexIndex = uCurVertexIndex + uNumVertices;
			dvbInfo.m_uCurVertexIndex = uCurVertexIndex;
			uNumQuads -= uNumQuadsPossible;
			pData = static_cast<const BYTE*>(pData) + dvbInfo.m_dwVertexStride * uNumVertices;

			uNumQuadsPossible = (dvbInfo.m_uNumVertices - uCurVertexIndex) / 4;
			if (uNumQuadsPossible > uMaxQuadCount)
				uNumQuadsPossible = uMaxQuadCount;
			if (uNumQuadsPossible == 0)
			{
				uCurVertexIndex = 0;
				uNumQuadsPossible = dvbInfo.m_uNumVertices / 4;
				if (uNumQuadsPossible > uMaxQuadCount)
					uNumQuadsPossible = uMaxQuadCount;
				if (uNumQuadsPossible == 0)
					return;
			}
		}

		uNumVertices = uNumQuads * 4;
		ASSERT(uNumVertices <= dvbInfo.m_uNumVertices);
		if (uNumVertices == 0 || uNumVertices > dvbInfo.m_uNumVertices)
			return;
	} //if

	//out: -> 사용하지 않아 Warning 발생

	HRESULT hr = dvbInfo.m_pVB->Lock(uCurVertexIndex * dvbInfo.m_dwVertexStride
		, uNumVertices * dvbInfo.m_dwVertexStride, &pLockedVertex,
		(uCurVertexIndex == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
		return;
	ASSERT(pLockedVertex != NULL);
	bool bExceptionOccurred = false;
	if (pLockedVertex != nullptr)
	{
		bExceptionOccurred = KExceptionSafeUtil::ESCopyMemory(pLockedVertex, pData,
			uNumVertices * dvbInfo.m_dwVertexStride);
	} //if
	dvbInfo.m_pVB->Unlock();

	if (bExceptionOccurred == true)
	{
		m_uNumExceptionsOccurred++;
		if (m_uNumExceptionsOccurred >= MAX_NUM_EXCEPTIONS_IN_ONE_FRAME)
		{
			m_hCheckLostDevice = g_pd3dDevice->TestCooperativeLevel();
			if (D3DERR_DEVICENOTRESET == m_hCheckLostDevice)
			{
			}
		} //if
		return;
	} //if

	if (uNumDraws == 1)
		g_pd3dDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuads * 2);
	else
	{
		for (unsigned u = 0; u != uNumDraws; u++)
		{
			g_pd3dDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST, uCurVertexIndex, 0, uNumVertices, 0, uNumQuads * 2);
		} //for
	} //for

	dvbInfo.m_uCurVertexIndex = uCurVertexIndex + uNumVertices;
}
#endif


KGCDynamicVBManager::DynamicVBInfo::~DynamicVBInfo()
{
	SAFE_RELEASE(m_pVB);
} //KGCDynamicVBManager::DynamicVBInfo::~DynamicVBInfo()


void KGCDynamicVBManager::DynamicVBInfo::OnResetDevice(EDYNAMIC_VB_MODE eMode)
{
	SAFE_RELEASE(m_pVB);
	m_uCurVertexIndex = 0;

	if (IsRegistered() == false)
		return;

	if (g_pd3dDevice == nullptr)
		return;

	DWORD dwD3DUsage = 0;
	D3DPOOL eD3DPool = D3DPOOL_DEFAULT;

	switch (eMode)
	{
	case DYNAMIC_VB_MODE_MANAGED:
		dwD3DUsage = D3DUSAGE_WRITEONLY;
		eD3DPool = D3DPOOL_MANAGED;
		break;
	case DYNAMIC_VB_MODE_PRIMITIVEUP:
		return;
	case DYNAMIC_VB_MODE_DYNAMICVB:
	case DYNAMIC_VB_MODE_INDEXED_DYNAMICVB:
#ifdef KTDGDEVICEFONT_SIZE_CACHE
	case DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH:
	case DYNAMIC_VB_MODE_INDEXED_DYNAMICVB_WITH_TRY_CATCH:
#endif
		dwD3DUsage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
		eD3DPool = D3DPOOL_DEFAULT;
		break;
	default:
		return;
	} //switch

	HRESULT hr = g_pd3dDevice->CreateVertexBuffer(4 * m_uNumVertices * m_dwVertexStride,
		dwD3DUsage, m_dwFVF,
		eD3DPool, &m_pVB, nullptr);

	ASSERT(SUCCEEDED(hr));
} //KGCDynamicVBManager::DynamicVBInfo::OnResetDevice()


void KGCDynamicVBManager::DynamicVBInfo::OnLostDevice(EDYNAMIC_VB_MODE)
{
	SAFE_RELEASE(m_pVB);
	m_uCurVertexIndex = 0;
} //KGCDynamicVBManager::DynamicVBInfo::OnLostDevice()
