/*===============================================================================
GCDeviceManager를 상속받아 
GrandChase에서 자주사용하는 편의함수를 제공합니다.
===============================================================================*/
#pragma once

#include "GCDeviceManager.h"
#include "./GCDeviceLib/GCDeviceMotionFRM.h"
#include "./GCDeviceLib/GCDeviceTexture.h"
#include "./GCDeviceLib/GCDeviceMeshP3M.h"
#include "KGC2DObject.h"

#define SAFE_DEVICE_RELEASE(p)      { if(p && g_pGCDeviceManager2) { (p)->Release(); (p)=NULL; } }
class GCObject;
struct GCMeshObject;

class GCDeviceManagerWrapper : public GCDeviceManager
{
public:
	GCDeviceManagerWrapper(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCDeviceManagerWrapper(void);

	GCDeviceTexture* CreateShopItemTexture( int iItemID );
	GCDeviceTexture* CreateItemTexture( int iItemID, int iCharType = -1, int iCharPromotion = -1 );
	GCDeviceTexture* CreateShopSetItemTexture( int iSetID );
    GCDeviceTexture* CreatePetItemTexture( int iPetGoodsID, int iEvolutionLevel = 0 );
	GCDeviceTexture* CreateItemTextureCustom(int iItemID, int iCharType = -1, int iCharPromotion = -1 );
	GCMeshObject* CreateAbtaModel( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, bool build=false, char cIgBoneIdx = -1 );
	GCMeshObject* CreateRoomAnimationModel(std::string RoomAnimationModel, GCObject* pObj_, bool build, char cIgBoneIdx = -1);
	void CreateTmpModel( int iItemID_, int iCharType_, int iPromotion, char cIgBoneIdx = -1 );
	GCMeshObject* CreateLas4MergeAbtaModel( int iItemID_, int iCharType_, GCObject* pObj_ ,bool build = false);	
    std::vector<GCObject*> CreateLiveModelList( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos = D3DXVECTOR3(0.0f,0.0f,0.0f), bool bOnlyTraceParentPos = false, char *szMotionFileName = NULL, int iBoneIdx = -1, bool bUseBoneIDFromArg = false );

private:
    GCObject* CreateLiveModel( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos = D3DXVECTOR3(0.0f,0.0f,0.0f), bool bOnlyTraceParentPos = false, char *szMotionFileName = NULL, int iBoneIdx = -1, bool bUseBoneIDFromArg = false);

public:
	const char* GetAbtaName( int iItemID_, const char* strNose = "Abta" );
	const char* GetResourceFileName( const char* strNose, int iCharType, int iPromotion, const char* strTail );
    const char* GetLupusResourceFileName( const char* strNose, int iCharType, int iPromotion, const char* strTail );
	const char* GetLiveAbtaName( int iItemID_, int iCharType, int iPromotion, const char* strTail, int* piBoneID = NULL );
	GCObject* CreateLiveModel( std::string modelName, std::string textureName, std::string motionName, int BoneIndex, GCObject* pObj_, D3DXVECTOR3 vOptionalPos = D3DXVECTOR3(0.0f,0.0f,0.0f), bool bOneTimeMotion = false, bool bOnlyTraceParentPos = false );

	void Draw_ShopImage( D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex,  bool bSetItem, int iSetItemNumber = -1, D3DCOLOR col=0xffffffff);
	void DrawUI(GCBaseTexture * tex, KD3DUIVertex* vert,D3DXVECTOR4 * uv1, D3DXVECTOR4 * uv2);
	void Draw_Text(float f_x, float f_y, const WCHAR* Text, D3DCOLOR Color = 0xffffffff, int iSize = -1, bool bShadow = false ,bool bAutoScale = true);
	void Draw_Text( D3DXVECTOR2 vPos, const WCHAR* Text, D3DCOLOR Color = 0xffffffff, int iSize = -1, bool bShadow = false ,bool bAutoScale = true);

    bool CheckFileExist( const char* strFileName );

public:
	void AddRelation( std::string a,std::string b )
	{		
		m_setRelation.insert(a+"->"+b);
	}

	std::string GetRelation()
	{
		std::set<std::string>::iterator i = m_setRelation.begin();
		std::string str="digraph Resource\n{\n";
		for(;i!=m_setRelation.end();i++)
		{
			str+="\t";
			str+=*i;	
			str+=";\n";
		}
		str+="}\n";
		return str;
	}

#if defined( _DEBUG )
	void Add2DObject( KGC2DObject* pObj );
	void Remove2DObject( KGC2DObject* pObj );

	KGC2DObject* Get2DObject( int iIndex );
	int	Get2DObjectSize()	{ return m_vec2DObject.size(); };
	char* GetResourceFileName( char* strHead, int iCharType, int iPromotion, char* strExt );
	
#endif
protected:

	std::set<std::string> m_setRelation;
	std::vector<KGC2DObject*> m_vec2DObject;
};


class IResourceNode
{
public:
	virtual std::string GetName()=0;
};

extern GCDeviceManagerWrapper* g_pGCDeviceManager2;
