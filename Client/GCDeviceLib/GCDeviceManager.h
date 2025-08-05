#pragma once

#include "GCDevice.h"
#include "../GCUTil/KJohnGen.h"

#define	WORLD_VB_SIZE					1024
#define	SCREEN_VB_SIZE					1024

class GCDeviceFontBase;
class GCDeviceFontTH;
class GCDeviceTexture;
class GCDeviceMeshP3M;
class GCDeviceIndexBuffer;
class GCDeviceVertexBuffer;
class GCDeviceMotionFRM;
class GCDeviceShader;
class GCTmpTexture;
class GCTmpMeshP3M;
class GCBaseTexture;
class GCBaseMesh;
class GCImageIconTexture;

class KomfileManager;

class KGCDynamicVBManager;

class GCDeviceManager
{
public:
	GCDeviceManager();
	GCDeviceManager(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCDeviceManager(void);

	bool ReleaseDevice( GCDevice* pDevice );
	bool ReleaseDevice( std::string DeviceID );
	GCDevice*		 FindDevice( std::string DeviceID );

	GCDeviceTexture* GetNullTexture(){return CreateTexture("NULL");}
	GCDeviceFontTH*	 CreateFontTHA( std::string DeviceID, int iFontSize, int iOutLine = 0, bool bRHW = true, int iWeight = FW_NORMAL , float fFontScale = 1.0f);
	GCDeviceFontBase* CreateFontA( std::string DeviceID, int iFontSize, int iOutLine = 0, bool bRHW = true, int iWeight = FW_NORMAL , float fFontScale = 1.0f);
	GCTmpTexture*	 CreateTmpTexture( std::string DeviceID);
	GCDeviceTexture* CreateTexture( std::string DeviceID, bool bLowQuality = false , bool bRealPath = false);
	GCImageIconTexture* CreateImageIconTexture( std::string DeviceID, bool bLowQuality = false , bool bRealPath = false);

	GCDeviceTexture* CreateTextureWithNoLoad( std::string DeviceID, bool bLowQuality = false );
	GCDeviceTexture* CreateTextureThread( std::string DeviceID, bool bLowQuality = false, bool bLoadThread = false );

	GCDeviceMeshP3M* CreateMeshP3M( std::string DeviceID, char cIgBoneIdx = -1 );
	GCTmpMeshP3M*	CreateTmpMeshP3M( std::string DeviceID, char cIgBoneIdx = -1 );

	GCDeviceMotionFRM* CreateMotionFRM( std::string DeviceID );
	GCDeviceVertexBuffer* CreateVertexBuffer( std::string DeviceID, UINT uiLength_,DWORD dwUsage_,DWORD dwFVF_,D3DPOOL Pool_, void* pData_ );
	GCDeviceIndexBuffer* CreateIndexBuffer( std::string DeviceID, UINT uiLength_,DWORD dwUsage_,D3DFORMAT Format_,D3DPOOL Pool_, void* pData_ );
	GCDeviceShader*	 CreateShader( std::string strDeviceID );
	bool			MakeDeviceList( std::string strFileName );		// 현재 Manager에 등록된 모든 Device의 상태를 보여줍니다
	void			WriteDeviceList( FILE* fp );

	void			SetReferenceResource( std::string strSource, std::string strTarget );

	void OnResetDevice();
	void OnLostDevice();
	void OnCreateDevice();
	void OnDestroyDevice();

	boost::shared_ptr<KomfileManager> GetMassFileManager() { return m_pMassFileManager; }
public:
	GCDeviceTexture*                m_pEdgeTexture;
protected:
	stdext::hash_map<std::string,GCDevice*> m_mapDevice;

	boost::shared_ptr<KomfileManager> m_pMassFileManager;
	LPDIRECT3DDEVICE9				m_d3ddevice;

	// 2D렌더링 함수들
public:

	// Screen Draw
	void DrawInScreen(const GCBaseTexture* tex,const KD3DUIVertex* vert,int size=1);
	void DrawInScreen(const GCBaseTexture* tex,float X1, float Y1, float X2, float Y2, float tu1, float tv1,
		float tu2, float tv2, bool Rotate=false, float Z=10.0f, D3DCOLOR col= 0xffffffff , bool bFixed = true);

	//도저히 안되서 만듬-_-;
	void DrawInScreenResize(const GCBaseTexture* tex,float X1, float Y1, float X2, float Y2, float tu1, float tv1,
		float tu2, float tv2, bool Rotate=false, float Z=10.0f, D3DCOLOR col= 0xffffffff);

	void DrawInWorld(const GCBaseTexture* tex,const SLVERTEX* vert,int size=1);
	void DrawInWorldWithStrip(const GCBaseTexture* tex,const D3DXMATRIX *mat,const SLVERTEX* vert,int size=1);
	void DrawInWorld(const GCBaseTexture* tex,const D3DXMATRIX *mat,const SLVERTEX* vert,int size=1);

	void DrawInWorld(const GCBaseTexture* tex,const D3DXMATRIX *mat,
		float Size_X, float Size_Y, float tu1, float tv1, float tu2, float tv2,
		float fOffsetX=0, float fOffsetY=0,float fOffsetZ=0.0f,D3DCOLOR col =0xffffffff);

	void DrawInWorld(const GCBaseTexture* tex,const D3DXMATRIX *mat,
		D3DXVECTOR3& pos1, D3DXVECTOR3& pos2,
		D3DXVECTOR3& pos3, D3DXVECTOR3& pos4,
		float fOffsetX=0, float fOffsetY=0,float fOffsetZ=0.0f,D3DCOLOR col =0xffffffff);
	void DrawInWorldResize( const GCBaseTexture* tex,const D3DXMATRIX *mat,
		D3DXVECTOR3& pos1, D3DXVECTOR3& pos2,
		D3DXVECTOR3& pos3, D3DXVECTOR3& pos4,
		float fU1, float fV1, float fU2, float fV2, D3DCOLOR col =0xffffffff);

	void FlushScreen();
	void FlushWorld();
	void Flush(){FlushScreen();FlushWorld();}

protected:

	GCBaseTexture								*m_pCurTex;

	LPDIRECT3DVERTEXBUFFER9						m_pWorldVB;
	LPDIRECT3DVERTEXBUFFER9						m_pScreenVB;
	LPDIRECT3DINDEXBUFFER9						m_pIB;

	KSafeArray<SLVERTEX,WORLD_VB_SIZE*4>        m_pWorldVertex;
	KSafeArray<KD3DUIVertex,SCREEN_VB_SIZE*4>   m_pScreenVertex;
	int											m_iWorldVBSize;
	int											m_iScreenVBSize;

	D3DXMATRIX									m_mat_id;

	KGCDynamicVBManager*                        m_pDVBManager;

public:
	KGCDynamicVBManager*
		GetDVBManager() { ASSERT( m_pDVBManager != NULL ); return m_pDVBManager; }

	// 스테이트 관련

protected:

	class KGCStateBlock
	{
	public:

		enum _statetype
		{
			STATE_RENDER,
			STATE_SAMPLER,
			STATE_TEXTURESTAGE,
		};

		class KGCState
		{
		public:
			KGCState(){};
			KGCState(DWORD statetype, DWORD stage,DWORD state, DWORD value)
			{
				m_type = statetype;
				m_stage = stage;
				m_state = state;
				m_value = value;
			};

			void Pop(LPDIRECT3DDEVICE9 device)
			{
				switch(m_type)
				{
				case STATE_RENDER:
					device->SetRenderState((D3DRENDERSTATETYPE)m_state,m_value);
					break;
				case STATE_SAMPLER:
					device->SetSamplerState(m_stage,(D3DSAMPLERSTATETYPE)m_state,m_value);
					break;
				case STATE_TEXTURESTAGE:
					device->SetTextureStageState(m_stage,(D3DTEXTURESTAGESTATETYPE)m_state,m_value);
					break;
				}
			}
			~KGCState(){}
		protected:

			DWORD m_type;
			DWORD m_stage;
			DWORD m_state;
			DWORD m_value;
		};

		void Push(DWORD statetype, DWORD stage,DWORD state, DWORD value)
		{
			m_stackState.push(KGCState(statetype,stage,state,value));
			m_count++;
		}
		bool IsChanged(){return m_count!=0;}
		void Pop(LPDIRECT3DDEVICE9 device)
		{
			while(m_count--)
			{
				m_stackState.top().Pop(device);
				m_stackState.pop();
			}
		}

		~KGCStateBlock(){}

		KGCStateBlock(){m_count=0;}
	protected:
		DWORD m_count;
		static std::stack<KGCState, std::deque<KGCState, boost::pool_allocator<KGCState> > > m_stackState;
	};

public:
	void PushState()
	{
		assert(m_stackState.size()<255);
		m_stackState.push(KGCStateBlock());
	}
	void PopState()
	{
		assert(!m_stackState.empty());

		if(m_stackState.top().IsChanged())
			Flush();

		m_stackState.top().Pop(m_d3ddevice);
		m_stackState.pop();
	}

	void SetScissorRect(RECT* pRect);
	void SetRenderState(D3DRENDERSTATETYPE state, DWORD value);
	HRESULT GetRenderState( D3DRENDERSTATETYPE State, DWORD *pValue ) {
		return m_d3ddevice->GetRenderState( State, pValue );
	}
	void SetSamplerState(int stage,D3DSAMPLERSTATETYPE state, DWORD value );
	HRESULT GetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD *pValue ) {
		return m_d3ddevice->GetSamplerState( Sampler, Type, pValue );
	}
	void SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value);

protected:

	KGCStateBlock* GetCurrentStateBlock()
	{
		if(m_stackState.empty())
			return NULL;
		return &m_stackState.top();
	}

	std::stack<KGCStateBlock, std::deque<KGCStateBlock, boost::pool_allocator<KGCStateBlock> > > m_stackState;

private:
	bool        m_bWindowMode;
public:
	float		m_windowScaleX;
	float		m_windowScaleY;

	bool IsWindowMode() { return m_bWindowMode; }
	void SetWindowMode( bool bWindowMode_ ) { m_bWindowMode = bWindowMode_; }

	float GetWindowScaleX();
	float GetWindowScaleY();

	void SetWindowScale( float x_, float y_ );
	void SetWindowScaleX( float x_ );
	void SetWindowScaleY( float y_ );
	void OutputLoadedDeviceList();

	// 크래시 로그 남길려고..
private:
	enum DeviceManagerErr{
		ERR_CREATE_WORLD_VB         = 0,
		ERR_CREATE_SCREEN_VB        = 1,
		ERR_CREATE_IB               = 2,
		ERR_FWORLD_VB_LOCK          = 3,
		ERR_FWORLD_VB_UNLOCK        = 4,
		ERR_FWORLD_DEVICE_TRANS     = 5,
		ERR_FWORLD_DEVICE_FVF       = 6,
		ERR_FWROLD_DEVICE_STREAM    = 7,
		ERR_FWROLD_DEVICE_INDEX     = 8,
		ERR_FWROLD_DEVICE_DRAW      = 9,
		ERR_FSCREEN_VB_LOCK         = 10,
		ERR_FSCREEN_VB_UNLOCK       = 11,
		ERR_FSCREEN_DEVICE_FVF      = 12,
		ERR_FSCREEN_DEVICE_STREAM   = 13,
		ERR_FSCREEN_DEVICE_INDEX    = 14,
		ERR_FSCREEN_DEVICE_DRAW     = 15,
	};

	std::map< DeviceManagerErr, HRESULT > m_mapError;
	std::string GetErrorString( DeviceManagerErr errType_ );

public:
	void GetCapsInfo( D3DCAPS9* caps );
	bool IsSoftwareVertexProcess( void );
	void SetDeviceErrorLog( IN DeviceManagerErr errType_, IN HRESULT hResult_ );
	std::string GetDeviceErrorLog( void );
};

extern GCDeviceManager*		g_pGCDeviceManager;
