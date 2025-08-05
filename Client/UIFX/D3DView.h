#pragma once
#include "Particle/KGCParticleManager.h"

//#include "D3DWnd.h"
class GCDeviceMeshP3M;
class KGCObjectAndMotion;

class KD3DView : public KD3DWnd
{
public:
	DECLARE_CLASSNAME(KD3DView);
	/// Default constructor
	KD3DView(void);
	/// Default destructor
	virtual ~KD3DView(void);
	/// Copy constructor
	KD3DView(const KD3DView&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KD3DView& operator=(const KD3DView&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

	//virtual void Initialize();
	//virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);
	virtual void OnDestroy(void);

	KGCObjectAndMotion* AddCharacter(std::string strName, KUserItemVector ItemList, int iCharType, int iPromotion, bool bWeapon);
	bool SetCharacterEquip(std::string strName, KUserItemVector ItemList, int iCharType, int iPromotion, bool bWeapon);
	bool AddMotionToCharacter(std::string strName, std::string strMotion, bool bOneTime);
	//bool RemoveMotionFromCharacter( std::string strName, std::string strMotion );
	bool ClearMotionFromCharacter(std::string strName);
	bool SetCharacterPosition(std::string strName, D3DXVECTOR3 vPos);
	//bool AddPet();
	bool AddObject(std::string strName, std::string strModel, std::string strTexture);
	bool AddMeshToObject(std::string strName, std::string strModel, std::string strTexture);
	bool AddMotionToObject(std::string strName, std::string strMotion);
	bool SetObjectPosition(std::string strName, D3DXVECTOR3 vPos);
	bool SetObjectScale(std::string strName, D3DXVECTOR3 vScale);
	bool SetObjectRotation(std::string strName, D3DXVECTOR3 vRotation);
	void LoadFaceTexture(GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition, bool bBigHead, bool bShadowHead);

	bool AddParticle(std::string strName, std::string strParticle, D3DXVECTOR3 vPos, EGCRenderLayer eLayer = GC_LAYER_CUSTOM);
	bool RemoveParticle(std::string strName);
	bool ClearParticle();

	bool RemoveObject(std::string strName);
	void ClearObject();

	bool SetBackground(std::string strModel, std::string strTexture);

	std::string GetRandomName();

private:
	void InitViewport();
	void InitMatrix();
private:
	D3DVIEWPORT9        m_viewport;
	D3DXMATRIX			m_matScale;	// 뷰표트 비율만큼 케릭터의 크기가 변하니까 비율에 맞춰서 변경해 줍니다.
	D3DXMATRIX			m_matRotation;
	D3DXMATRIX			m_matTransform;
	D3DXMATRIX			m_matProject;

	KGCObjectAndMotion* m_pkRoom;
	std::map< std::string, KGCObjectAndMotion* > m_mapObject;

	typedef std::map< std::string, CParticleEventSeqPTR > GCViewMapParticle;
	std::map< std::string, CParticleEventSeqPTR > m_mapParticle;

private:
	bool                m_bLoadDownHelmetNFL;
	GCMeshObject* m_pNonChangeFaceLift;
};

DEFINE_WND_FACTORY(KD3DView);
DECLARE_WND_FACTORY(KD3DView);
DECLARE_WND_FACTORY_NAME(KD3DView);
