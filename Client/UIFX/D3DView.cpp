#include "StdAfx.h"
#include "MyD3D.h"

#include "GCObjectAndMotion.h"
#include "../GCRenderManager.h"
#include "KGCRoomManager.h"

#include "D3DView.h"

IMPLEMENT_CLASSNAME(KD3DView);
IMPLEMENT_WND_FACTORY(KD3DView);
IMPLEMENT_WND_FACTORY_NAME(KD3DView, "gc_viewbox");

KD3DView::KD3DView(void)
	: m_pkRoom(NULL)
	, m_bLoadDownHelmetNFL(false)
	, m_pNonChangeFaceLift(NULL)
{
}

KD3DView::~KD3DView(void)
{
	SAFE_DELETE(m_pkRoom);
}

void KD3DView::OnCreate(void)
{
	InitViewport();
	InitMatrix();
}

void KD3DView::InitViewport()
{
	D3DXVECTOR2 vPos(GetFixedWindowPos());
	m_viewport.X = static_cast<DWORD>(vPos.x * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>(vPos.y * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>(GetWidth() * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Height = static_cast<DWORD>(GetHeight() * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 10.0f;
}

void KD3DView::InitMatrix()
{
#if defined( _DEBUG1234 )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

	float MinZ = 0.0f, MaxZ = 1.0f;
	LUA_GET_VALUE_DEF("MinZ", MinZ, 0.0f);
	LUA_GET_VALUE_DEF("MaxZ", MaxZ, 1.0f);

	m_viewport.MinZ = MinZ;
	m_viewport.MaxZ = MaxZ;

	float s1, s2, s3;
	LUA_GET_VALUE_DEF("s1", s1, 1.0f);
	LUA_GET_VALUE_DEF("s2", s2, 1.0f);
	LUA_GET_VALUE_DEF("s3", s3, 0.01f);

	float t1, t2, t3;
	LUA_GET_VALUE_DEF("t1", t1, 0.0f);
	LUA_GET_VALUE_DEF("t2", t2, 0.0f);
	LUA_GET_VALUE_DEF("t3", t3, 0.1f);

	float f1, f2, f3, f4;
	LUA_GET_VALUE_DEF("f1", f1, D3DX_PI / 4.0f);
	LUA_GET_VALUE_DEF("f2", f2, 1.0f);
	LUA_GET_VALUE_DEF("f3", f3, 1.0f);
	LUA_GET_VALUE_DEF("f4", f4, 100.0f);

	D3DXMATRIX matLook, matScale, matTran, matFOV;
	::D3DXMatrixScaling(&matScale, s1, s2, s3);
	::D3DXMatrixTranslation(&matTran, t1, t2, t3);
	::D3DXMatrixPerspectiveFovLH(&matFOV, f1, f2, f3, f4);
	m_matProject = matFOV * matScale * matTran;
#else
	D3DXMATRIX matLook, matScale, matTran, matFOV;
	::D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.01f);
	::D3DXMatrixTranslation(&matTran, 0.0f, 0.0f, 0.1f);
	::D3DXMatrixPerspectiveFovLH(&matFOV, D3DX_PI / 3.46f, (float)m_viewport.Width / (float)m_viewport.Height, 2.0f, 50.0f);
	m_matProject = matFOV * matScale * matTran;
#endif

	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixScaling(&m_matScale, ((float)(GC_SCREEN_WIDTH * g_pGCDeviceManager2->GetWindowScaleX()) / m_viewport.Width),
		((float)(GC_SCREEN_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY()) / m_viewport.Height), 1.0f);

	D3DXMatrixIdentity(&m_matRotation);

	D3DXMatrixIdentity(&m_matTransform);
	D3DXMatrixTranslation(&m_matTransform, 0.0f, -0.36f, -0.08f);
}

void KD3DView::PostChildDraw(void)
{
	g_pGCDeviceManager2->Flush();
	g_pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L);

	//g_ParticleManager->Z_Enable( true );

	//뷰포트 변경
	D3DVIEWPORT9 orgViewport;
	g_pd3dDevice->GetViewport(&orgViewport);
	g_pd3dDevice->SetViewport(&m_viewport);

	g_pd3dDevice->SetFVF(SKINVERTEX::FVF);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	//g_pGCDeviceManager2->SetRenderState( D3DRS_LIGHTING, TRUE );

	// 배경을 그리자.
	g_RenderManager->Render(false, m_pkRoom, EGCShaderTechnique::NOCARTOON_NOEDGE);
	g_ParticleManager->Render(GC_LAYER_CUSTOM);

	// 그려야할 오브젝트들을 그리자.
	std::map< std::string, KGCObjectAndMotion* >::iterator mitObj;
	for (mitObj = m_mapObject.begin(); mitObj != m_mapObject.end(); ++mitObj)
	{
		D3DXMATRIX ObjWorldMat = *mitObj->second->GetWorldMat();
		ObjWorldMat *= m_matScale * m_matRotation * m_matTransform;
		mitObj->second->SetWorldMat(&ObjWorldMat);
		g_RenderManager->Render(false, (GCObject*)mitObj->second);
	}
	g_ParticleManager->Render(GC_LAYER_CUSTOM2);

	g_ParticleManager->Z_Enable(false);
	//g_pGCDeviceManager2->SetRenderState( D3DRS_LIGHTING, FALSE );

	// 뷰포트 복구
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_MyD3D->matProj);
	g_pd3dDevice->SetViewport(&orgViewport);
	g_pd3dDevice->SetTexture(1, NULL);
}

KGCObjectAndMotion* KD3DView::AddCharacter(std::string strName, KUserItemVector ItemList, int iCharType, int iPromotion, bool bWeapon)
{
	KGCObjectAndMotion* pObject = NULL;

	if (ItemList.empty())
		return pObject;

	PLAYER_TEMPLATE kPlayerTemplate;
	GetPlayerTemplate(iCharType + GC_CHAR_NUM * iPromotion, kPlayerTemplate);

	bool bBigHead = false;
	bool bShadowHead = false;
	m_bLoadDownHelmetNFL = false;
	m_pNonChangeFaceLift = NULL;
	pObject = new KGCObjectAndMotion();
	KUserItemVector::iterator vit;
	for (vit = ItemList.begin(); vit != ItemList.end(); ++vit)
	{
		SUserItem& item = (*vit);
		GCItem* pCItem = g_pItemMgr->GetItemData(item.iItemID);
		CONTINUE_NIL(pCItem);

		if (pCItem->dwSlotPosition & ESP_WEAPON && !bWeapon)
			continue;

		// 빅헤드를 끼고있으면 빅헤드용 앞면 처리를 해줘야한다.
		DWORD dwBigHeadPos = (ESP_A_DOWN_HELMET + ESP_A_UPPER_HELMET);
		if (dwBigHeadPos == (pCItem->dwSlotPosition & dwBigHeadPos))
		{
			bBigHead = true;
		}

		ITEM_MESH_OPTION kOption;
		std::vector< ITEM_MESH_OPTION >::iterator vecMeshOptionIter = kPlayerTemplate.vecItemMeshOption.begin();
		for (; vecMeshOptionIter != kPlayerTemplate.vecItemMeshOption.end(); ++vecMeshOptionIter)
		{
			// 장착 포지션 확인
			if (pCItem->dwSlotPosition != vecMeshOptionIter->dwConSlotPosition)
				continue;

			// 게임 플레이 중일때만 적용하는 옵션
			if (1 == vecMeshOptionIter->iConPlaying)
				continue;

			// 광장에서만 적용하는 옵션
			if (1 == vecMeshOptionIter->iConSquare)
				continue;

			// 옵션적용
			kOption = (*vecMeshOptionIter);
			break;
		}

		if (kOption.bIsNotLoad)
			continue;

		if (pCItem->dwGoodsID == ITEMID_ASIN_SHAADOW_SET) {
			bShadowHead = true;
		}

		GCMeshObject* pMeshObj = g_pItemMgr->CreateAbtaModel(item.iItemID, iCharType, iPromotion, (GCObject*)pObject, false, kOption.iBoneID);
		if (pMeshObj)
		{
			if (kOption.iTechnique != -1)
			{
				pMeshObj->m_iTechnique = kOption.iTechnique;
			}
		}

		LoadFaceTexture(pObject, iCharType, iPromotion, pCItem->dwGoodsID, pCItem->dwSlotPosition, bBigHead, bShadowHead);
	}

	std::vector< ADD_MESH >::iterator vecAddMeshIter = kPlayerTemplate.vecAddMesh.begin();
	for (; vecAddMeshIter != kPlayerTemplate.vecAddMesh.end(); ++vecAddMeshIter)
	{
		// 모든 조건을 통과해야만 추가 메시를 로드할 수 있닷!!
		bool bConBigHead = true, bConEquipItem = true, bConNonEquipItem = true, bConGameModeCategory = true, bConPlaying = true, bConEquipSetItem = true, bConNonEquipSetItem = true;

		// 빅헤드 조건확인( 0이면 빅헤드가 아닐때 로드, 1이면 빅헤드 일때 로드 )
		if (-1 < vecAddMeshIter->iConBigHead)
		{
			if (bBigHead)
				bConBigHead = !(0 == vecAddMeshIter->iConBigHead);
			else
				bConBigHead = !(1 == vecAddMeshIter->iConBigHead);
		}

		// 장착한 장비 조건 확인
		if (-1 < vecAddMeshIter->iConEquipItem)
		{
			KUserItemVector::iterator vit;
			for (vit = ItemList.begin(); vit != ItemList.end(); ++vit)
			{
				if (vit->iItemID == vecAddMeshIter->iConEquipItem)
					break;
			}
			bConEquipItem = (vit != ItemList.end());
		}

		// 장착한 안한 장비 조건 확인
		if (-1 < vecAddMeshIter->iConNonEquipItem)
		{
			KUserItemVector::iterator vit;
			for (vit = ItemList.begin(); vit != ItemList.end(); ++vit)
			{
				if (vit->iItemID == vecAddMeshIter->iConNonEquipItem)
					break;
			}
			bConNonEquipItem = (vit == ItemList.end());
		}

		// 장착한 세트 장비 조건 확인
		if (-1 < vecAddMeshIter->iConEquipSetItem)
		{
			bConEquipSetItem = g_MyD3D->m_kItemMgr.IsChangeMeshCoordi(vecAddMeshIter->iConEquipSetItem);
		}

		// 장착 안한 세트 장비 조건 확인
		if (-1 < vecAddMeshIter->iConNonEquipSetItem)
		{
			bConNonEquipSetItem = (g_MyD3D->m_kItemMgr.IsChangeMeshCoordi(vecAddMeshIter->iConNonEquipSetItem) == true) ? false : true;
		}

		// 게임모드카테고리 조건 확인
		if (-1 < vecAddMeshIter->iConGameModeCategory)
		{
			bConGameModeCategory = (vecAddMeshIter->iConGameModeCategory == static_cast<int>(SiKGCRoomManager()->GetGameModeCategory()));
		}

		// 플레이 중인지 조건 확인
		if (-1 < vecAddMeshIter->iConPlaying)
		{
			if (0 == vecAddMeshIter->iConPlaying)
				bConPlaying = false;
		}

		// 종착지
		if (bConBigHead && bConEquipItem && bConNonEquipItem && bConGameModeCategory && bConPlaying && bConEquipSetItem && bConNonEquipSetItem)
		{
			if ("" == vecAddMeshIter->piLiveInfo.first)
			{
				GCMeshObject* pAddMesh = pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(vecAddMeshIter->strMeshName), g_pGCDeviceManager2->CreateTexture(vecAddMeshIter->strTextureName));
				if (pAddMesh != NULL)
				{
					pAddMesh->m_bExtraMeshObject = vecAddMeshIter->bIsExtraMesh;
					pAddMesh->m_iTechnique = vecAddMeshIter->iTechnique;
					pAddMesh->m_bRender = vecAddMeshIter->bIsRender;
					pAddMesh->m_bSecondBaseMesh = vecAddMeshIter->bIsSecondBaseMesh;
					if (false == vecAddMeshIter->strChildName.empty())
						pAddMesh->m_strName = vecAddMeshIter->strChildName;
				}
			}
			else
			{
				GCObject* pAddMesh = g_pGCDeviceManager2->CreateLiveModel(vecAddMeshIter->strMeshName.c_str(), vecAddMeshIter->strTextureName.c_str(), vecAddMeshIter->piLiveInfo.first.c_str(), vecAddMeshIter->piLiveInfo.second, pObject);
				if (pAddMesh != NULL)
				{
					pAddMesh->m_bIsExtraObject = vecAddMeshIter->bIsExtraMesh;
					pAddMesh->SetTechnique(vecAddMeshIter->iTechnique);
					pAddMesh->SetRender(vecAddMeshIter->bIsRender);
					if (false == vecAddMeshIter->strChildName.empty())
						pAddMesh->m_strName = vecAddMeshIter->strChildName;
				}
			}
		}
	}

	// 얼굴표정 리소스 로드
	if (NULL == m_pNonChangeFaceLift)
	{
		int iFaceLiftType = -1;
		if (bBigHead) {
			iFaceLiftType = FACE_LIFT_BIG_HEAD;
		}
		else {
			iFaceLiftType = FACE_LIFT_NORMAL;
		}
		if (bShadowHead) {
			iFaceLiftType = FACE_LIFT_SHADOW;
		}

		std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find(iFaceLiftType);
		if (mit != kPlayerTemplate.mapFaceLiftMesh.end()) {
			pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(mit->second.strMeshName), g_pGCDeviceManager2->CreateTexture(mit->second.strTextureName));
		}
	}

	pObject->SetScale(D3DXVECTOR3(CHAR_SCALE_RATIO, CHAR_SCALE_RATIO, CHAR_SCALE_RATIO));
	pObject->SetShellMatrixScale(CHAR_SCALE_RATIO);

#if defined( _DEBUG1234 )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

	KSafeArray<float, GC_CHAR_NUM> charOffset;
	LUA_GET_VALUE_DEF("Char0", charOffset[0], 0.0f);
	LUA_GET_VALUE_DEF("Char1", charOffset[1], 0.0f);
	LUA_GET_VALUE_DEF("Char2", charOffset[2], 0.0f);
	LUA_GET_VALUE_DEF("Char3", charOffset[3], 0.0f);
	LUA_GET_VALUE_DEF("Char4", charOffset[4], 0.0f);
	LUA_GET_VALUE_DEF("Char5", charOffset[5], 0.0f);
	LUA_GET_VALUE_DEF("Char6", charOffset[6], 0.0f);
#else
	float charOffset[GC_CHAR_NUM] = { 0.0f, -0.01f, -0.02f, 0.01f, 0.0f, 0.0f, 0.0f };
#endif
	pObject->SetOffset(D3DXVECTOR3(0.0f, charOffset[iCharType], 0.0f));

	m_mapObject.insert(std::map< std::string, KGCObjectAndMotion* >::value_type(strName, pObject));

	return pObject;
}

// 얼굴 표정변환 시스템에 어울리지 않는 특수복장과 악세사리 아이템때문에
// 어울리는 표정 하나만 로드하기 위한 코드입니다.
void KD3DView::LoadFaceTexture(GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition, bool bBigHead, bool bShadowHead)
{
	// 일반 머리와 빅헤드, 헤드하위장식이 아니면 나가
	DWORD dwBigHead = ESP_A_UPPER_HELMET + ESP_A_DOWN_HELMET;
	if ((ESP_HELMET != (dwSlotPosition & ESP_HELMET)) &&
		(ESP_A_DOWN_HELMET != (dwSlotPosition & ESP_A_DOWN_HELMET)) &&
		(dwBigHead != (dwSlotPosition & dwBigHead)))
		return;

	// 하위장식이 로드되었는데 헬멧로드하려고 하면 나가
	if (m_bLoadDownHelmetNFL && (dwSlotPosition == (dwSlotPosition & ESP_HELMET)))
		return;

	// 리소스 이름을 만들어보자
	std::string strTextureName;
	const char* strAbtaName = g_pGCDeviceManager2->GetAbtaName(g_pItemMgr->GetSameImageIndex(iItemID));
	strTextureName = g_pGCDeviceManager2->GetLupusResourceFileName(strAbtaName, iCharType, iPromotion, "_NFL.dds");
	if ("" == strTextureName)
		strTextureName = g_pGCDeviceManager2->GetResourceFileName(strAbtaName, iCharType, iPromotion, "_NFL.dds");

	// 텍스쳐 파일이름기록이 안됫으면 없는 리소스니깐 나가
	if (strTextureName.empty() || "" == strTextureName)
		return;

	int iFaceLiftType;
	if (bBigHead) {
		iFaceLiftType = FACE_LIFT_BIG_HEAD;
	}
	else {
		iFaceLiftType = FACE_LIFT_NORMAL;
	}

	if (bShadowHead) {
		iFaceLiftType = FACE_LIFT_SHADOW;
	}

	PLAYER_TEMPLATE kPlayerTemplate;
	GetPlayerTemplate(iCharType + iPromotion * GC_CHAR_NUM, kPlayerTemplate);
	std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find(iFaceLiftType);
	if (mit != kPlayerTemplate.mapFaceLiftMesh.end())
	{
		bool bDownHelmet = false;
		if ((dwSlotPosition == (dwSlotPosition & ESP_A_DOWN_HELMET)))
			bDownHelmet = true;

		// 하위장식 로드하려고 하는데 다른 헬멧로드된 상태면 그 헬멧 지워
		if (bDownHelmet && m_pNonChangeFaceLift)
		{
			pObject->RemoveMesh(m_pNonChangeFaceLift->m_pMesh);
			m_pNonChangeFaceLift = NULL;
		}

		GCMeshObject* pMeshObj = pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(mit->second.strMeshName), g_pGCDeviceManager2->CreateTexture(strTextureName.c_str()));
		if (pMeshObj != NULL)
		{
			m_pNonChangeFaceLift = pMeshObj;

			// 하위장식 로드했으면 로드했다고 표시
			if (bDownHelmet)
				m_bLoadDownHelmetNFL = true;
		}
	}
}

bool KD3DView::SetBackground(std::string strModel, std::string strTexture)
{
	SAFE_DELETE(m_pkRoom);
	m_pkRoom = new KGCObjectAndMotion();
	m_pkRoom->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(strModel), g_pGCDeviceManager2->CreateTexture(strTexture));

	//milennium9 test code
#if defined( _DEBUG1234 )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

	float x = 0.0f, y = 0.0f, z = 0.0f;
	LUA_GET_VALUE_DEF("bg_x", x, 0.0f);
	LUA_GET_VALUE_DEF("bg_y", y, 0.0f);
	LUA_GET_VALUE_DEF("bg_z", z, 0.0f);

	float ss1, ss2, ss3;
	LUA_GET_VALUE_DEF("ss1", ss1, 0.1f);
	LUA_GET_VALUE_DEF("ss2", ss2, 0.1f);
	LUA_GET_VALUE_DEF("ss3", ss3, 0.1f);

	float r1, r2, r3;
	LUA_GET_VALUE_DEF("r1", r1, 0.0f);
	LUA_GET_VALUE_DEF("r2", r2, 0.0f);
	LUA_GET_VALUE_DEF("r3", r3, 0.0f);

	m_pkRoom->SetScale(D3DXVECTOR3(ss1, ss2, ss3));
	m_pkRoom->SetPosition(D3DXVECTOR3(x, y, z));
#else
	// 마법의 숫자는 체험적인 값입니다. 나중에 알아서 잘 리팩토링 해 주세요.
	m_pkRoom->SetPosition(D3DXVECTOR3(0.0f, -0.07f, 0.437f));
#endif
	return true;
}

bool KD3DView::AddMotionToCharacter(std::string strName, std::string strMotion, bool bOneTime)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->AddMotion(strMotion, bOneTime, true);
	return true;
}

void KD3DView::FrameMoveInEnabledState(void)
{
	// 그려야할 오브젝트들의 프레임을 갱신한다.
	std::map< std::string, KGCObjectAndMotion* >::iterator mitObj;
	for (mitObj = m_mapObject.begin(); mitObj != m_mapObject.end(); ++mitObj)
	{
		mitObj->second->FrameMove();
	}
	if (m_pkRoom != NULL)
		m_pkRoom->FrameMove();

	//milennium9 test code
#ifdef _DEBUG1234
	if (!o30 && m_pkRoom != NULL)
	{
		InitMatrix();

		KLuaManager luaMgr;
		GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

		float x = 0.0f, y = 0.0f, z = 0.0f;
		LUA_GET_VALUE_DEF("bg_x", x, 0.0f);
		LUA_GET_VALUE_DEF("bg_y", y, 0.0f);
		LUA_GET_VALUE_DEF("bg_z", z, 0.0f);

		float ss1, ss2, ss3;
		LUA_GET_VALUE_DEF("ss1", ss1, 0.1f);
		LUA_GET_VALUE_DEF("ss2", ss2, 0.1f);
		LUA_GET_VALUE_DEF("ss3", ss3, 0.1f);

		m_pkRoom->SetScale(D3DXVECTOR3(ss1, ss2, ss3));
		m_pkRoom->SetPosition(D3DXVECTOR3(x, y, z));
	}
#endif
}

bool KD3DView::SetCharacterPosition(std::string strName, D3DXVECTOR3 vPos)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->SetPosition(vPos);
	return true;
}

bool KD3DView::AddMotionToObject(std::string strName, std::string strMotion)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->AddMotion(strMotion);
	return true;
}

void KD3DView::OnDestroy(void)
{
	ClearObject();
	ClearParticle();
}

void KD3DView::ClearObject()
{
	// 오브젝트들을 제거 합니다.
	std::map< std::string, KGCObjectAndMotion* >::iterator mitObj;
	for (mitObj = m_mapObject.begin(); mitObj != m_mapObject.end(); ++mitObj)
	{
		SAFE_DELETE(mitObj->second);
	}
	m_mapObject.clear();
}

bool KD3DView::RemoveObject(std::string strName)
{
	// 오브젝트들을 제거 합니다.
	std::map< std::string, KGCObjectAndMotion* >::iterator mitObj;

	mitObj = m_mapObject.find(strName);

	if (mitObj == m_mapObject.end())
		return false;

	SAFE_DELETE(m_mapObject[strName]);
	m_mapObject.erase(mitObj);
	return true;
}

bool KD3DView::SetObjectPosition(std::string strName, D3DXVECTOR3 vPos)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->SetPosition(vPos);
	return true;
}

bool KD3DView::SetObjectScale(std::string strName, D3DXVECTOR3 vScale)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->SetScale(vScale);
	return true;
}

bool KD3DView::SetObjectRotation(std::string strName, D3DXVECTOR3 vRotation)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->SetRotation(vRotation);
	return true;
}

bool KD3DView::AddObject(std::string strName, std::string strModel, std::string strTexture)
{
	if (m_mapObject.find(strName) != m_mapObject.end())
		return AddMeshToObject(strName, strModel, strTexture);

	KGCObjectAndMotion* pObject = new KGCObjectAndMotion();

	pObject->AddMesh(strModel, strTexture);
	m_mapObject.insert(std::map< std::string, KGCObjectAndMotion* >::value_type(strName, pObject));

	pObject->SetScale(D3DXVECTOR3(CHAR_SCALE_RATIO, CHAR_SCALE_RATIO, CHAR_SCALE_RATIO));
	pObject->SetShellMatrixScale(CHAR_SCALE_RATIO);

	return true;
}

bool KD3DView::AddMeshToObject(std::string strName, std::string strModel, std::string strTexture)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	m_mapObject[strName]->AddMesh(strModel, strTexture);
	return true;
}

bool KD3DView::SetCharacterEquip(std::string strName, KUserItemVector ItemList, int iCharType, int iPromotion, bool bWeapon)
{
	if (ItemList.empty())
		return false;

	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	KGCObjectAndMotion* pObject = m_mapObject[strName];
	pObject->RemoveMesh(NULL);
	KUserItemVector::iterator vit;
	for (vit = ItemList.begin(); vit != ItemList.end(); ++vit)
	{
		SUserItem& item = (*vit);

		if (g_pItemMgr->GetItemData(item.iItemID)->dwSlotPosition & ESP_WEAPON && !bWeapon)
			continue;

		char cIgBoneIdx = -1;
		// 진 장갑 한쪽 끄는 처리 해야 된다면 이쪽에서 예외처리 해주자!!!!
		if (g_pItemMgr->GetItemData(item.iItemID)->dwCharType & CID_RONAN4 &&
			g_pItemMgr->GetItemData(item.iItemID)->dwSlotPosition == ESP_GLOVES)
		{
			cIgBoneIdx = 10;
		}

		// 진 장갑 한쪽 끄는 처리 해야 된다면 이쪽에서 예외처리 해주자!!!!
		if (g_pItemMgr->GetItemData(item.iItemID)->dwCharType & CID_JIN1 &&
			g_pItemMgr->GetItemData(item.iItemID)->dwSlotPosition == ESP_GLOVES)
		{
			cIgBoneIdx = 13;
		}
		else if ((g_pItemMgr->GetItemData(item.iItemID)->dwCharType & CID_LEY1 ||
			g_pItemMgr->GetItemData(item.iItemID)->dwCharType & CID_LEY2) &&
			g_pItemMgr->GetItemData(item.iItemID)->dwSlotPosition == ESP_GLOVES)
		{
			cIgBoneIdx = 10;
		}

		g_pItemMgr->CreateAbtaModel(item.iItemID, iCharType, iPromotion, (GCObject*)pObject, false, cIgBoneIdx);
	}
	pObject->SetScale(D3DXVECTOR3(CHAR_SCALE_RATIO, CHAR_SCALE_RATIO, CHAR_SCALE_RATIO));

	return true;
}

/*
bool KD3DView::RemoveMotionFromCharacter( std::string strName, std::string strMotion )
{
	if( m_mapObject.find( strName ) == m_mapObject.end() )
		return false;

	KGCObjectAndMotion* pObject = m_mapObject[strName];

	pObject->
}
//*/

bool KD3DView::ClearMotionFromCharacter(std::string strName)
{
	if (m_mapObject.find(strName) == m_mapObject.end())
		return false;

	KGCObjectAndMotion* pObject = m_mapObject[strName];
	pObject->ClearMotion();
	return true;
}

bool KD3DView::AddParticle(std::string strName, std::string strParticle, D3DXVECTOR3 vPos, EGCRenderLayer eLayer /*= GC_LAYER_CUSTOM*/)
{
	if (m_mapParticle.find(strName) != m_mapParticle.end())
		return false;

	m_mapParticle[strName] = g_ParticleManager->CreateSequence(strParticle, vPos.x, vPos.y, vPos.z);
	g_ParticleManager->SetLayer(m_mapParticle[strName], eLayer);
	return true;
}

bool KD3DView::RemoveParticle(std::string strName)
{
	if (m_mapParticle.find(strName) == m_mapParticle.end())
		return true;

	if (m_mapParticle[strName] != NULL)
		g_ParticleManager->DeleteSequence(m_mapParticle[strName]);

	m_mapParticle.erase(strName);
	return true;
}

bool KD3DView::ClearParticle()
{
	GCViewMapParticle::iterator mit;
	for (mit = m_mapParticle.begin(); mit != m_mapParticle.end(); ++mit)
	{
		if (mit->second != NULL)
			g_ParticleManager->DeleteSequence(mit->second);
	}
	m_mapParticle.clear();
	return true;
}

std::string KD3DView::GetRandomName()
{
	std::stringstream stm;
	stm << "__Object" << m_mapObject.size() + m_mapParticle.size();
	return stm.str();
}