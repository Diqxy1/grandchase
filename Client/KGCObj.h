#pragma once
#include "PlayerComp.h"


#ifndef __EQUAL_OPERATOR__
#define __EQUAL_OPERATOR__(p) this->p = rval.p
#endif

struct MSG_PARTICE_CREATE_INFO
{
	std::string strSeqname;
	int iParticleNum;
	D3DXVECTOR2 vPos;
	float fOffsetX;
	bool bUserAngle;
	bool bBlackHole;
	float fTraceTime;
	D3DXVECTOR3* pTracePos;
	bool bDirecitonal;
	bool bApplyMotionY;
	int iBoneID;
	int iLeftSideLayer;
	int iRightSideLayer;

	MSG_PARTICE_CREATE_INFO()
		: strSeqname( "" ), vPos( D3DXVECTOR2( 0.0f, 0.0f ) ),fOffsetX(0.0f), iParticleNum( 0 ), bUserAngle( false ), bBlackHole( false ), fTraceTime( 0.0f ), pTracePos( NULL ), bDirecitonal( true ), bApplyMotionY( true )
	{
		iBoneID = -1;
		iLeftSideLayer = 0;
		iRightSideLayer = 0;

	}

	MSG_PARTICE_CREATE_INFO(const std::string& strSeqname_, D3DXVECTOR2 vPos_, int iParticleNum_ = 0,float fOffsetX_ = 0.0f, bool bDirecitonal_ = true, float fTraceTime_ = 0.0f, D3DXVECTOR3* pTracePos_ = NULL, bool bUserAngle_ = false, bool bBlackHole_ = false, bool bApplyMotionY_ = true )
		: strSeqname( strSeqname_ ), vPos( vPos_ ), iParticleNum( iParticleNum_ ),fOffsetX(fOffsetX_), bUserAngle( bUserAngle_ ), bBlackHole( bBlackHole_ ), fTraceTime( fTraceTime_ ), pTracePos( pTracePos_ ), bDirecitonal( bDirecitonal_ ), bApplyMotionY( bApplyMotionY_ )
	{
		iBoneID = -1;
		iLeftSideLayer = 0;
		iRightSideLayer = 0;

	}
	MSG_PARTICE_CREATE_INFO(const std::string& strSeqname_, float xPos = 0.0f, float yPos = 0.0f, int iParticleNum_ = 0,float fOffsetX_ = 0.0f, bool bDirecitonal_ = true, float fTraceTime_ = 0.0f, D3DXVECTOR3* pTracePos_ = NULL, bool bUserAngle_ = false, bool bBlackHole_ = false, bool bApplyMotionY_ = true )
		: strSeqname( strSeqname_ ), vPos( D3DXVECTOR2( xPos, yPos ) ), iParticleNum( iParticleNum_ ),fOffsetX(fOffsetX_), bUserAngle( bUserAngle_ ), bBlackHole( bBlackHole_ ), fTraceTime( fTraceTime_ ), pTracePos( pTracePos_ ), bDirecitonal( bDirecitonal_ ), bApplyMotionY( bApplyMotionY_ )
	{
		iBoneID = -1;
		iLeftSideLayer = 0;
		iRightSideLayer = 0;

	}

	MSG_PARTICE_CREATE_INFO& operator =( const MSG_PARTICE_CREATE_INFO& rval )
	{
		__EQUAL_OPERATOR__( strSeqname );
		__EQUAL_OPERATOR__( iParticleNum );
		__EQUAL_OPERATOR__( vPos );
		__EQUAL_OPERATOR__( fOffsetX );
		__EQUAL_OPERATOR__( bUserAngle );
		__EQUAL_OPERATOR__( bBlackHole );
		__EQUAL_OPERATOR__( fTraceTime );
		__EQUAL_OPERATOR__( pTracePos );
		__EQUAL_OPERATOR__( bDirecitonal );
		__EQUAL_OPERATOR__( bApplyMotionY );        
		__EQUAL_OPERATOR__( iBoneID );
		__EQUAL_OPERATOR__( iLeftSideLayer );
		__EQUAL_OPERATOR__( iRightSideLayer );
		return *this;
	}

	MSG_PARTICE_CREATE_INFO( const MSG_PARTICE_CREATE_INFO& rval )
	{
		__EQUAL_OPERATOR__( strSeqname );
		__EQUAL_OPERATOR__( iParticleNum );
		__EQUAL_OPERATOR__( vPos );
		__EQUAL_OPERATOR__( fOffsetX );
		__EQUAL_OPERATOR__( bUserAngle );
		__EQUAL_OPERATOR__( bBlackHole );
		__EQUAL_OPERATOR__( fTraceTime );
		__EQUAL_OPERATOR__( pTracePos );
		__EQUAL_OPERATOR__( bDirecitonal );
		__EQUAL_OPERATOR__( bApplyMotionY );        
		__EQUAL_OPERATOR__( iBoneID );
		__EQUAL_OPERATOR__( iLeftSideLayer );
		__EQUAL_OPERATOR__( iRightSideLayer );
	}
};

typedef std::vector< MSG_PARTICE_CREATE_INFO > VecParticleMsg;

struct EXTRA_MESH
{
	std::string strID;
	std::string strTextureName;
	std::string strMeshName;
	std::string strMotionName;
	DWORD       dwCartoon;
	DWORD       dwTechnique;
	int 		iBoneId;
	D3DXVECTOR3 vOptionalPos;
	EXTRA_MESH()
        : iBoneId(0)
    {};

	EXTRA_MESH( const EXTRA_MESH& src )
	{
		strID = src.strID;
		strTextureName = src.strTextureName;
		strMeshName = src.strMeshName;
		strMotionName = src.strMotionName;
		dwCartoon = src.dwCartoon;
		dwTechnique = src.dwTechnique;
		iBoneId = src.iBoneId;
		vOptionalPos = src.vOptionalPos;
	}

	const EXTRA_MESH& operator=( const EXTRA_MESH& src )
	{
		strID = src.strID;
		strTextureName = src.strTextureName;
		strMeshName = src.strMeshName;
		strMotionName = src.strMotionName;
		dwCartoon = src.dwCartoon;
		dwTechnique = src.dwTechnique;
		iBoneId = src.iBoneId;
		vOptionalPos = src.vOptionalPos;
		return *this;
	}
};

struct FACE_LIFT_MESH
{
    std::string strTextureName;
    std::string strMeshName;

    FACE_LIFT_MESH() {};
 
    FACE_LIFT_MESH( const FACE_LIFT_MESH& src )
    {
        strTextureName = src.strTextureName;
        strMeshName = src.strMeshName;
    }

    const FACE_LIFT_MESH& operator=( const FACE_LIFT_MESH& src )
    {
        strTextureName = src.strTextureName;
        strMeshName = src.strMeshName;
        return *this;
    }
};

struct ADD_MESH
{
    // 조건
    int     iConBigHead;
    int     iConEquipItem;
    int     iConNonEquipItem;
    int     iConGameModeCategory;
    int     iConPlaying;

    // 옵션
    bool    bIsRender;
    bool    bIsExtraMesh;
    bool    bIsSecondBaseMesh;
    bool    bIsCoordi;
    int     iTechnique;
    int     iConEquipSetItem;
    int     iConNonEquipSetItem;
    std::string strChildName;
    std::string strTextureName;
    std::string strMeshName;
    std::string strID;
    std::pair<std::string, int>  piLiveInfo;

    ADD_MESH() {};

    ADD_MESH( const ADD_MESH& src )
    {
        iConBigHead = src.iConBigHead;
        iConEquipItem = src.iConEquipItem;
        iConNonEquipItem = src.iConNonEquipItem;
        iConGameModeCategory = src.iConGameModeCategory;
        iConPlaying = src.iConPlaying;

        bIsRender = src.bIsRender;
        bIsExtraMesh = src.bIsExtraMesh;
        bIsCoordi = src.bIsCoordi;
        iTechnique = src.iTechnique;
        strChildName = src.strChildName;
        strTextureName = src.strTextureName;
        strMeshName = src.strMeshName;
        strID = src.strID;
        piLiveInfo = src.piLiveInfo;
        bIsSecondBaseMesh = src.bIsSecondBaseMesh;
        iConEquipSetItem = src.iConEquipSetItem ;
        iConNonEquipSetItem = src.iConNonEquipSetItem;
    }

    const ADD_MESH& operator=( const ADD_MESH& src )
    {
        iConBigHead = src.iConBigHead;
        iConEquipItem = src.iConEquipItem;
        iConNonEquipItem = src.iConNonEquipItem;
        iConGameModeCategory = src.iConGameModeCategory;
        iConPlaying = src.iConPlaying;

        bIsRender = src.bIsRender;
        bIsExtraMesh = src.bIsExtraMesh;
        bIsCoordi = src.bIsCoordi;
        iTechnique = src.iTechnique;
        strChildName = src.strChildName;
        strTextureName = src.strTextureName;
        strMeshName = src.strMeshName;
        strID = src.strID;
        piLiveInfo = src.piLiveInfo;
        bIsSecondBaseMesh = src.bIsSecondBaseMesh;
        iConEquipSetItem = src.iConEquipSetItem;
        iConNonEquipSetItem = src.iConNonEquipSetItem;
        return *this;
    }
};

struct ADD_ITEM_LIVEMODEL
{
    int  iBoneID;
    int  iTechnique;
    bool bRender;
    bool bOnlyTraceParentPos;
    bool bUseBoneIDFromArg;
    std::string strMotion;
    std::string strChildName;
    D3DXVECTOR3 vPos;

    ADD_ITEM_LIVEMODEL()
    {
        iBoneID = -1;
        iTechnique = -1;
        bRender = false;
        bOnlyTraceParentPos = false;
        bUseBoneIDFromArg = false;
        strMotion.clear();
        strChildName.clear();
        vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    ADD_ITEM_LIVEMODEL( const ADD_ITEM_LIVEMODEL& src )
    {
        iBoneID = src.iBoneID;
        iTechnique = src.iTechnique;
        bRender = src.bRender;
        bOnlyTraceParentPos = src.bOnlyTraceParentPos;
        bUseBoneIDFromArg = src.bUseBoneIDFromArg;
        strMotion = src.strMotion;
        strChildName = src.strChildName;
        vPos = src.vPos;
    }

    const ADD_ITEM_LIVEMODEL& operator=( const ADD_ITEM_LIVEMODEL& src )
    {
        iBoneID = src.iBoneID;
        iTechnique = src.iTechnique;
        bRender = src.bRender;
        bOnlyTraceParentPos = src.bOnlyTraceParentPos;
        bUseBoneIDFromArg = src.bUseBoneIDFromArg;
        strMotion = src.strMotion;
        strChildName = src.strChildName;
        vPos = src.vPos;
        return *this;
    }
    
    void Init()
    {
        iBoneID = -1;
        iTechnique = -1;
        bRender = true;
        bOnlyTraceParentPos = false;
        bUseBoneIDFromArg = false;
        strMotion.clear();
        strChildName.clear();
        vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }
};

struct ITEM_MESH_OPTION
{
    // 장착위치
    DWORD		    dwConSlotPosition;

    // 조건
    int             iConPlaying;
    int             iConSquare;

    // 옵션
    int     iBoneID;
    int     iTechnique;
    bool    bIsNotLoad;
    bool    bIsRender;
    bool    bIsOnlyTraceParentPos;
    bool    bIsAutoChildNameAsLiveLoopCnt;
    D3DXVECTOR3 vPos;
    D3DXVECTOR3 vScale;
    std::string strChildName;
    std::string strLiveMotionName;
    std::vector<ADD_ITEM_LIVEMODEL> vecAddItemLiveModel;

    ITEM_MESH_OPTION() 
    {
        Init();
    };

    ITEM_MESH_OPTION( const ITEM_MESH_OPTION& src )
    {
        dwConSlotPosition = src.dwConSlotPosition;
        iConPlaying = src.iConPlaying;
        iConSquare = src.iConSquare;
        iBoneID = src.iBoneID;
        iTechnique = src.iTechnique;
        bIsNotLoad = src.bIsNotLoad;
        bIsRender = src.bIsRender;
        bIsOnlyTraceParentPos = src.bIsOnlyTraceParentPos;
        bIsAutoChildNameAsLiveLoopCnt = src.bIsAutoChildNameAsLiveLoopCnt;
        vPos = src.vPos;
        vScale = src.vScale;
        strChildName = src.strChildName;
        strLiveMotionName = src.strLiveMotionName;
        vecAddItemLiveModel = src.vecAddItemLiveModel;
    }

    const ITEM_MESH_OPTION& operator=( const ITEM_MESH_OPTION& src )
    {
        dwConSlotPosition = src.dwConSlotPosition;
        iConPlaying = src.iConPlaying;
        iConSquare = src.iConSquare;
        iBoneID = src.iBoneID;
        iTechnique = src.iTechnique;
        bIsNotLoad = src.bIsNotLoad;
        bIsRender = src.bIsRender;
        bIsOnlyTraceParentPos = src.bIsOnlyTraceParentPos;
        bIsAutoChildNameAsLiveLoopCnt = src.bIsAutoChildNameAsLiveLoopCnt;
        vPos = src.vPos;
        vScale = src.vScale;
        strChildName = src.strChildName;
        strLiveMotionName = src.strLiveMotionName;
        vecAddItemLiveModel = src.vecAddItemLiveModel;
        return *this;
    }

    void Init()
    {
        dwConSlotPosition = 0;
        iConPlaying = -1;
        iConSquare = -1;
        iBoneID = -1;
        iTechnique = -1;
        bIsNotLoad = false;
        bIsRender = true;
        bIsOnlyTraceParentPos = false;
        bIsAutoChildNameAsLiveLoopCnt = false;
        vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        vScale = D3DXVECTOR3(DEFAULT_PLAYER_SCALE_SIZE, DEFAULT_PLAYER_SCALE_SIZE, DEFAULT_PLAYER_SCALE_SIZE);
        strChildName.clear();
        strLiveMotionName.clear();
        vecAddItemLiveModel.clear();
    }
};

class KGCObj
	: public IGCComputePaletteP3M    
{
public:
	KGCObj();
	~KGCObj() {}

	static void RegisterLuabind();

	virtual bool IsDie() { return false; }

    virtual bool IsToggleExtraMesh( const char* strID );
    virtual void ToggleMeshAll( bool bToggle_ );
	virtual void ToggleExtraMesh( const char* strID, bool bToggle_ , int iShaderTech = -1);
	virtual void ToggleExtraMeshAll( bool bToggle_ );
	virtual void ToggleBaseMesh( bool bToggle_ );
    virtual void ToggleSecondBaseMesh( const char* strID, bool bToggle_ );
    virtual void ToggleSecondBaseMeshAll( bool bToggle_ );
    virtual bool IsToggleSecondBaseMesh( const char* strID_ );
	virtual void SetChildMeshMotion( char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame = false, bool bNotOneTime = false, int iLoopMode = 0 );
    virtual void SetChildMeshMotion_Front( char* strChlid, char* strMotion );
    virtual void SetChildMeshFrame( char* strChlid, char* strMotion, int iFrame );
    virtual void SetChildMeshCurrentFrame( char* strChlid, int iFrame );
    virtual int  GetChildMeshCurrentFrame( char* strChlid );
    virtual const char* GetChildMeshCurrentMotionName( char* strChild );
	virtual void ClearChildMeshOneTimeMotion( char* strChlid );
    virtual CParticleEventSeqPTR AddParticleToChildMeshBone( char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime = 0.5f, float offset_x = 0.0f, float offset_y = 0.0f, bool bNoDirection = false );

	virtual void LoadExtraMesh( std::vector< EXTRA_MESH > vecMesh );
	virtual void OnFrameMove();

	GCObject*						m_pObject;
	bool							m_bRender;
	bool                            bIsRight;
    float							y_Ex;
	D3DXVECTOR3                     vRenderPos;
	float							Body_Angle;

    std::map<int, D3DXVECTOR3>      m_mapUserBoneAngle;

	VecParticleMsg m_ParticleMsg;
	std::queue<VecParticleMsg> m_QueueParticleList;
	//파티클
	CParticleEventSeqPTR CreateParticle( MSG_PARTICE_CREATE_INFO& msgParticle );
	virtual void ParticleQueueProcess(VecParticleMsg&);
    virtual CParticleEventSeqPTR CreateParticleProcess(MSG_PARTICE_CREATE_INFO*);

	bool GetRender() const { return m_bRender; }
	virtual void SetRender( bool bRender ) { m_bRender = bRender; }

	D3DXVECTOR2						vParticleToDamageMagicNumber;

	virtual PositionDesc*			GetPosDesc(){ return NULL; }

};


