
#pragma once

//#include <vector>
//#include <algorithm>
#include "Particle/KTDGParticleSystem.h"
#include "GCGraphicsHelper.h"
#include "IGCBone.h"
#include "GCDeviceMotionFRM.h"

class IGCComputePaletteP3M;
class GCDeviceMotionFRM;
class GCDeviceTexture;
class GCDeviceMeshP3M;

#define EDGE_COLOR_ENEMY D3DXVECTOR4(1,0,0,1)
#define EDGE_COLOR_FRIEND D3DXVECTOR4(0,0,0,1)
#define EDGE_COLOR_SUPERARMOR D3DXVECTOR4(1,0,0,1)
namespace EGCShaderTechnique
{
    enum
    {
        CARTOON_BLACKEDGE = 0,
        CARTOON_NOEDGE,
        NOCARTOON_NOEDGE,
        RESULT_MODEL,
		CARTOON_NOEDGE_ALPHA,
		CARTOON_EDGE_ALPHA,
        CARTOON_NOEDGE_ALPHA_BRIGHT,
        CARTOON_EDGE_ALPHA_BRIGHT,

        TECHNIQUE_NUM,
    };
};

namespace EGCRenderMethod
{
    enum
    {
        VERTEX_SHADER = 0,
        FIXED_PIPE,
        TECHNIQUE_NUM,
    };
};

struct ParticlePosDesc 
{
    int m_iBoneIndex;
    D3DXVECTOR3 m_vPos;
    CParticleEventSeqPTR pParticle;
    int m_iLeftSideLayer;
    int m_iRightSideLayer;    

    ParticlePosDesc()
    {
        Init();
    }

    ParticlePosDesc( ParticlePosDesc& kRight_)
    {
        pParticle = NULL;
        m_iBoneIndex = kRight_.m_iBoneIndex;
        m_vPos = kRight_.m_vPos;
        m_iLeftSideLayer = kRight_.m_iLeftSideLayer;
        m_iRightSideLayer = kRight_.m_iRightSideLayer;        
    }

    const ParticlePosDesc& operator = ( const ParticlePosDesc& kRight_ )
    {
        pParticle = NULL;
        m_iBoneIndex = kRight_.m_iBoneIndex;
        m_vPos = kRight_.m_vPos;
        m_iLeftSideLayer = kRight_.m_iLeftSideLayer;
        m_iRightSideLayer = kRight_.m_iRightSideLayer;        
    }

    void Init()
    {
        pParticle = NULL;
        m_iBoneIndex = -1;
        m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
        m_iLeftSideLayer = 0;
        m_iRightSideLayer = 0;
    }
};

struct GCMeshObject
{
    GCBaseMesh*    m_pMesh;
    GCBaseTexture*    m_pTexture;
    EGCCartoonTexture   m_eCartoon;
    bool                m_bRender;
    std::string         m_strName;
	int					m_iTechnique;
    int                 m_iOriginalTechnique;
	bool				m_bExtraMeshObject;
    std::string         m_strTextureName;
    bool                m_bSecondBaseMesh;

    GCMeshObject()
    {
        m_pMesh = NULL;
        m_pTexture = NULL;
        m_eCartoon = CARTOON_NORMAL;
        m_bRender = true;
		m_iTechnique = -1;
        m_iOriginalTechnique = -1;
		m_bExtraMeshObject = false;
        m_bSecondBaseMesh = false;
    }
    GCMeshObject( const GCMeshObject& obj )
    {
        m_pMesh = obj.m_pMesh;
        m_pTexture = obj.m_pTexture;
        m_eCartoon = obj.m_eCartoon;
        m_bRender = obj.m_bRender;
        m_strName = obj.m_strName;
        m_strTextureName = obj.m_strTextureName;
		m_iTechnique = obj.m_iTechnique;
        m_iOriginalTechnique = obj.m_iOriginalTechnique;
		m_bExtraMeshObject = obj.m_bExtraMeshObject;
        m_bSecondBaseMesh = obj.m_bSecondBaseMesh;
    }

};

//====================================================================================
// 2007-06-25 오후 3:41:13 : 우동완
// 
// Flag가 조금 복잡해 보이지만 상위에서 무슨일을 하는지 상관없이 짜려고 하다가 보니까 이렇게 되었습니다.
//
// Invislble 아이템이나 닭쌔 빽스텝시에 적인지 아군인지 판별해서 State를 설정해 주어야 합니다.
//
// 원래는 깜빡이는 효과는 모두 BLINK로 통일하려 했으나 투명 쓴 상태로 빽스텝을 쓰고 죽는다든가하는 복잡한 경우가 생길것 같아서,
//
// 각각 독립적으로 처리해 줍니다.
//

#define MS_IMMORTAL             0x00000001
#define MS_DEAD                 0x00000002
#define MS_INVISIBLE            0x00000004
#define MS_BACK_STEP            0x00000008
#define MS_REBIRTH              0x00000010
#define MS_INVISIBLE_ENEMY      0x00000020
#define MS_BACK_STEP_ENEMY      0x00000040
#define MS_EMPTY_SAVING         0x00000080

typedef void (GCObject::*GCMeshRenderFunc)( GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_ ) const;
typedef void (GCObject::*GCPrimitiveRenderFunc)() const;
typedef bool (GCObject::*GCSetMotionMatrixFunc)( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_, bool bForce_ );
class KGCObjectAndMotion;


struct GCMeshTraceInfo
{
	int iBoneID;
	bool bOnlyTraceParentPos;
		
	GCMeshTraceInfo()
	{
		iBoneID = -1;
		bOnlyTraceParentPos = false;
	}
	GCMeshTraceInfo( const GCMeshTraceInfo& obj )
	{
		iBoneID = obj.iBoneID;
		bOnlyTraceParentPos = obj.bOnlyTraceParentPos;
	}

};
typedef std::pair<KGCObjectAndMotion*, GCMeshTraceInfo> ParasiteMesh; 
class GCObject : public IGCBone
{
    friend class KGCRenderManager;
public:

	GCObject(void);
	virtual ~GCObject(void);
    virtual void SetPosition( D3DXVECTOR3 vPos ){}
    virtual void SetScale( D3DXVECTOR3 vPos );

    // Parent Mesh를 세팅한다. Child Mesh는 Parent Mesh에서 Setting해 놓은 Matrix Palette를 이용해 Rendering한다.
    GCMeshObject* AddMesh( GCDeviceMeshP3M* pMesh_, GCDeviceTexture* pTexture_, EGCCartoonTexture eCartoon_ = CARTOON_NORMAL , const std::string strID = "", int iTech_ = -1 );
    virtual GCMeshObject* AddMesh( const std::string& strMesh, const std::string& strTex, char cIgBoneIdx = -1, EGCCartoonTexture eCartoon_ = CARTOON_NORMAL , const std::string strID = "", int iTech_ = -1, int iItemID_ = -1 );
    void RemoveMesh( GCBaseMesh* pMesh );
	void RemoveMesh( const std::string& strName );
    GCMeshObject* GetMeshObject( int iIndex_ = 0 );
    
	GCMeshObject* Build(EGCCartoonTexture eCartoon_ = CARTOON_NORMAL );
	void ClearBuild();
	bool AddStaticMesh(const std::string& strMesh, const std::string& strTex, char cIgBoneIdx = -1);

    void RenderOneTexture( GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_ ) const;
    void RenderOneTextureAlpha( GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_ ) const;
    void RenderTwoTexture( GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_ ) const;
	void RenderTwoTextureAlpha( GCDeviceTexture* pMeshTexture_, GCDeviceTexture* pCartoonTexture_, int iTech_ ) const;
    void SetID( DWORD dwID_ ){ m_dwID = dwID_; }
    DWORD GetID(){ return m_dwID; }
    int GetMeshObjectNum() { return (int)m_vecMeshObject.size(); }
    virtual void Render( int iTechnique_ = 0, bool bCull_ = true );
    void SetFrame( SFrame* pFrame_ ) 
    { 
        m_kFrame = *pFrame_;
    }
    SFrame* GetFrame()
    {
        return &m_kFrame;
    }    
    void SetWorldMat( const D3DXMATRIX* MatWorld_ ) { m_matWorld = *MatWorld_; }
    const D3DXMATRIX* GetWorldMat();
    static void SetRenderFunc();
    bool SetMotionMatrix( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_ = true, bool bForce_ = false );
    
    void SetRender( bool bRender_ );
    bool GetRender() const { return m_bRender; }

    inline DWORD GetMeshState() const { return m_dwMeshState; }
    
    void MeshStateOn( DWORD dwMeshState_ ) ;
    void MeshStateOff( DWORD dwMeshState_ ) ;
    void ResetMeshState();
    void SetMeshState( DWORD dwMeshState_ );
    bool IsInvisible()
    {        
        return ( 0 != ( m_dwMeshState & ( MS_BACK_STEP_ENEMY | MS_INVISIBLE_ENEMY ) ) );
    }
    void SetMeshComputePaletteInterface( IGCComputePaletteP3M* IMeshComputePalette );
	void SetVolume( float fVolume_ ) { m_fVolume = fVolume_; }
    void SetCartoon( EGCCartoonTexture eCartoon_ );
    EGCCartoonTexture GetCartoon() const { return m_eCartoon; }
    int AddTexture( GCDeviceTexture* pTexture ) { m_vecTexture.push_back(pTexture);	return (m_vecTexture.size()-1);}
    void SetTexture( int iTexture_ ) { m_iSelectTexture = iTexture_; }
    int IsLoadTexture( const std::string& szTexName );
    void SetOutlineColor( D3DXVECTOR4 vColor );
    static void SetUsingVS( bool bUsingVS_ );
    void SetShellMatrixScale( float fMatrixScale_ );
    float GetShellMatrixScale() { return m_fShellMatrixScale; }
    void SetRecord( bool bRecord );
    const D3DXMATRIX* GetAngleBone( unsigned int iIndex, D3DXMATRIX* pOutMatrix = NULL );
    void GetAngleBonePos( unsigned int iIndex, D3DXVECTOR3* pOutVector );
    
	bool IsUseBuildNickNameTex( void );
	void SetText(int i,std::wstring nickname,D3DCOLOR col=0xFF00FF00,D3DCOLOR outline_col=0xFF000000,std::string icon="");
	void DrawText(int i,float x,float y);
    bool IsCullObject(bool bCamera_);

    bool IsToggleExtraMesh( std::string strID );
    void ToggleMeshAll( bool bToggle );
    void ToggleExtraMesh( std::string strID, bool bToggle , int iShaderTech = -1);
    void ToggleExtraMeshAll( bool bToggle );
    void ToggleBaseMesh( bool bToggle );
    void ToggleBaseMeshAll( bool bToggle );
    void ToggleSecondBaseMesh( std::string strID, bool bToggle );
    void ToggleSecondBaseMeshAll( bool bToggle );
    bool IsToggleSecondBaseMesh( const std::string strID_ ) const;
    bool GetMeshRender( std::string strID ) const;
    void SetTechnique( int iTech_ ) { m_iTechnique = iTech_; }
	int GetTechnique( void ) { return m_iTechnique; }
	void SetLayer( int iLayer_ ) { m_iLayer = iLayer_; }
	int GetLayer( void ) { return m_iLayer; }

    std::string                     m_strName;

protected:

    void MatrixCompute( D3DXMATRIX* pMatBase, int iBone , bool IsPos );
    void RenderPrimitiveVS() const;
    void RenderPrimitiveFixedPipe() const;
    bool SetMotionMatrixFP( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_, bool bForce_ );
    bool SetMotionMatrixVS( bool bCamera_, D3DXMATRIX* matViewProj, bool bTransform_, bool bForce_ );
    static GCMeshRenderFunc         ms_pRenderFunc[EGCShaderTechnique::TECHNIQUE_NUM];
    static GCPrimitiveRenderFunc    ms_pPrimitiveRenderFunc;
    static GCSetMotionMatrixFunc    ms_pSetMotionMatrixFunc;
    std::vector< GCMeshObject* >    m_vecMeshObject;
    DWORD                           m_dwMeshState;
    DWORD                           m_dwID;
    SFrame                          m_kFrame;
    D3DXMATRIX*                     m_matMotion;
    bool                            m_bRender;
    IGCComputePaletteP3M*           m_pIMeshComputePalette;
    float                           m_fVolume;
    static bool                     ms_bUsingVertexShader;
    static unsigned int             ms_uiMeshIndex;             // 현재 그리고 있는 Mesh의 Index, 이것 없이도 짤수는 있으나 있는게 조금 빠를것 같음,
    //====================================================================================
    // 2007-06-09 오후 7:04:53 : 우동완
    // m_eCartoon 이 CARTOON_DEFAULT 가 아닐때는 meshObject에 설정된 카툰은 무시하고 요걸로 그린다.
    EGCCartoonTexture               m_eCartoon;
    // 몇몇 몬스터나 특수상황에서 셋팅해야 하는 텍스쳐.
    std::vector<GCDeviceTexture*>	m_vecTexture;
    int                             m_iSelectTexture;
    D3DXMATRIX                      m_matWorld;
    D3DXVECTOR4                     m_vOutlineColor;    
    D3DXMATRIX                      m_matShellMatrix;
    float                           m_fShellMatrixScale;
	bool							m_bBuilded;
	GCMeshObject					*m_pBuildMeshObject;
	int								m_iTechnique;
	int								m_iLayer;

	static char*                    ms_strTechnique[];

	static int						ms_iRenderCount;


    //특정 본에 붙힐 자식 메쉬
    std::vector< ParasiteMesh >        m_vecChild;

public:
    GCMeshObject* GetBuildMeshObject() { return m_pBuildMeshObject; };
    void AddChildObject( KGCObjectAndMotion* pObj_, int iBoneIndex_, bool bOnlyTraceParentPos_ );
    KGCObjectAndMotion* GetChildObject( std::string strName );
	KGCObjectAndMotion* GetChildObject( unsigned int iIndex );
    void ChildFrameMove();

	bool m_bIsRight;

    int m_iCenterBone;
    void SetCenterBone( int iBone_ ) { m_iCenterBone = iBone_; }

    std::vector<ParticlePosDesc*>         m_vecPosDesc;
    void AdjustParticlePos( void );
	bool m_bIsExtraObject;

    void QualityMeshTechChange() const;
};
