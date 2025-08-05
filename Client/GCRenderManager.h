//#ifndef __GC_RENDER_MANAGER_H__
//#define __GC_RENDER_MANAGER_H__

#pragma once
#include <KNCSingleton.h>

//====================================================================================
// 2007-06-25 오후 3:33:38 : 우동완
//
// 음 ㅡ 그냥 코드만 놔두면 어떻게 써야 할지 모를까봐 사족을 조금 답니다.
//
// 제가 작업을 다 한 뒤로는 화면에 나오는 모든 Mesh를 g_RenderManager를 통해 GCObject를 생성하기 바랍니다.
//
// 매니저의 Render를 부르면 등록된 모든 GCObject들이 렌더 됩니다.
//
// 그리고 혹 필요한 경우 Parameter로 GCObject를 넣어주면 해당 객체만 렌더 할수 있습니다.
//
// 화면에 표시될 수 있는 모든 메쉬의 속성들은 GCObject의 MeshState를 등록해서 사용하기 바랍니다.
//
// 현재 메쉬에 적용되는 여러 효과들이 코드 곳곳에 흗어져 있는데 조금씩 모으다 보면 나름 깔끔하게 정리가 될것 같습니다.

#define NUM_TECHNIQUE 8

#define OUT_LINE_WIDTH_IN_GAME          0.0025f
#define OUT_LINE_WIDTH_IN_SHOP          0.0035f
#define OUT_LINE_WIDTH_IN_WAIT_ROOM     0.0033f
#define OUT_LINE_WIDTH_IN_LOADING       0.0040f
#define OUT_LINE_WIDTH_IN_CHANNEL       0.0017f

class GCObject;
class GCDeviceShader;
class KGCRenderManager;
typedef void (KGCRenderManager::*RenderFunc)(bool bCamera_, GCObject* pObj_, int iTech, int iLayer_);
extern LPDIRECT3DDEVICE9       g_pd3dDevice;

class KGCRenderManager
{
	DeclareSingleton(KGCRenderManager);
	KGCRenderManager(void);
public:
	virtual ~KGCRenderManager(void);
	KGCRenderManager(const KGCRenderManager&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	const KGCRenderManager& operator=(const KGCRenderManager&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

	void AddObject(GCObject* pObj_, int iTechniqueID_ = NUM_TECHNIQUE);
	GCObject* CreateObject(int iTechniqueID_ = NUM_TECHNIQUE);
	bool RemoveObject(GCObject* pObj_);
	void DeleteAll();
	void Render(bool bCamera_ = true, GCObject* pObj_ = NULL, int iTech = NUM_TECHNIQUE, int iLayer_ = GC_LAYER_CHAR);
	void ForcedRenderVS(bool bCamera_ = true, GCObject* pObj_ = NULL, int iTech = NUM_TECHNIQUE, int iLayer_ = GC_LAYER_CHAR);
	void SetRenderTech(int iTechniqueID_);
	int GetRenderTech();
	void SetOutlineWidth(float fWidth_);
	float GetOutlineWidth(void);

	void SetDeviceShader(GCDeviceShader* pDeviceShader_);
	void SetUsingVS(bool bUsingVS_);
	bool GetUsingVS();
	static RenderFunc                               ms_pRenderFunc;
protected:
	void RenderVS(bool bCamera_, GCObject* pObj_, int iTech, int iLayer_);
	void RenderFixedPipe(bool bCamera_, GCObject* pObj_, int iTech, int iLayer_);
	void SetRenderStateFixedPipe(int iTech_);

	std::map< int, std::vector<GCObject* > >  m_mapObjects;
	GCDeviceShader*                          m_pDeviceShader;
	DWORD                                    m_dwIDCount;
	static char*                             ms_strTechnique[];
	int                                      m_iTechniqueID;
	int                                      m_iObjectNum;
};
DefSingletonInline(KGCRenderManager);

extern KGCRenderManager * g_RenderManager;
//#endif //__GC_RENDER_MANAGER_H_
