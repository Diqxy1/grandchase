//** @file   KD3DDeviceState.h
//	@desc   interface of KD3DDeviceState
//	@author robobeg@kogstudios.com
//    @since  2003-1-1
//
//	@date   seojt on 2005-8-26, 11:26:53
//            - example comment added
//*/

#pragma warning(disable: 4189 4100)
#pragma once
#include "../GCUTil/KJohnGen.h"

#define RENDER_STATE_STACK_SIZE         256
#define RENDER_STATE_STACK_SET_SIZE     16
#define TEXTURE_STAGE_STACK_SIZE        256
#define TEXTURE_STAGE_STACK_SET_SIZE    16
#define SAMPLER_STAGE_STACK_SIZE        256
#define SAMPLER_STAGE_STACK_SET_SIZE    16

#define D3DGENERALIZE(interf,pointer,ver) \
	typedef interf##ver interf; \
	typedef struct interf##ver  *LP##pointer, *P##pointer;

#ifdef _XBOX

#define TEXTURE_STAGE_NUM   4

D3DGENERALIZE( IDirect3D, DIRECT3D, 8 )
	D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 8 )
	D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 8 )
	D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 8 )
	D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 8 )
	D3DGENERALIZE( IDirect3DPushBuffer, DIRECT3DPUSHBUFFER, 8 )
	D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 8 )
	D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 8 )
	D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 8 )
	D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 8 )
	D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 8 )
	D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 8 )
	typedef D3DADAPTER_IDENTIFIER9  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS9                D3DCAPS;
typedef D3DLIGHT9               D3DLIGHT;
typedef D3DMATERIAL9            D3DMATERIAL;
typedef D3DVIEWPORT9            D3DVIEWPORT;

#else

#define TEXTURE_STAGE_NUM   8

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

#define D3DRS_MAX           (D3DRS_NORMALORDER+1)
#define D3DTSS_MAX          (D3DTSS_RESULTARG+1)

D3DGENERALIZE( IDirect3D, DIRECT3D, 8 )
	D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 8 )
	D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 8 )
	D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 8 )
	D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 8 )
	D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 8 )
	D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 8 )
	D3DGENERALIZE( IDirect3DSwapChain, DIRECT3DSWAPCHAIN, 8 )
	D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 8 )
	D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 8 )
	D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 8 )
	D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 8 )
	typedef D3DADAPTER_IDENTIFIER9  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS9                D3DCAPS;
typedef D3DCLIPSTATUS8          D3DCLIPSTATUS;
typedef D3DLIGHT9               D3DLIGHT;
typedef D3DMATERIAL9            D3DMATERIAL;
typedef D3DVIEWPORT9            D3DVIEWPORT;

#elif ( DIRECT3D_VERSION >= 0x0900 )

#define SAMPLER_STAGE_NUM   16

#define D3DRS_MAX           (D3DRS_BLENDOPALPHA+1)
#define D3DTSS_MAX          (D3DTSS_CONSTANT+1)
#define D3DSSS_MAX          (D3DSAMP_DMAPOFFSET+1)

D3DGENERALIZE( IDirect3D, DIRECT3D, 9 )
	D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 9 )
	D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 9 )
	D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 9 )
	D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 9 )
	D3DGENERALIZE( IDirect3DPixelShader, DIRECT3DPIXELSHADER, 9 )
	D3DGENERALIZE( IDirect3DQuery, DIRECT3DQUERY, 9 )
	D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 9 )
	D3DGENERALIZE( IDirect3DStateBlock, DIRECT3DSTATEBLOCK, 9 )
	D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 9 )
	D3DGENERALIZE( IDirect3DSwapChain, DIRECT3DSWAPCHAIN, 9 )
	D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 9 )
	D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 9 )
	D3DGENERALIZE( IDirect3DVertexDeclaration, DIRECT3DVERTEXDECLARATION, 9 )
	D3DGENERALIZE( IDirect3DVertexShader, DIRECT3DVERTEXSHADER, 9 )
	D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 9 )
	D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 9 )
	typedef D3DADAPTER_IDENTIFIER9  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS9                D3DCAPS;
typedef D3DCLIPSTATUS9          D3DCLIPSTATUS;
typedef D3DLIGHT9               D3DLIGHT;
typedef D3DMATERIAL9            D3DMATERIAL;
typedef D3DVIEWPORT9            D3DVIEWPORT;

#endif // #ifdef ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

#endif // #ifdef _XBOX

class   KD3DDeviceState
{
private:

	static UINT                         ms_uiAdapter;
	static D3DDEVTYPE                   ms_d3dDeviceType;

	static BOOL                         ms_bRestored;

	//static int                          ms_iBeginSceneCounter;

	static DWORD                        ms_adwRenderState[D3DRS_MAX];
	static D3DRENDERSTATETYPE           ms_aeRenderStateStack[RENDER_STATE_STACK_SET_SIZE][RENDER_STATE_STACK_SIZE];
	static DWORD                        ms_adwRenderValueStack[RENDER_STATE_STACK_SET_SIZE][RENDER_STATE_STACK_SIZE];
	static int                          ms_aiRenderStateStackPointer[RENDER_STATE_STACK_SET_SIZE];
	static int                          ms_iRenderStateSetStackPtr;

	static DWORD                       ms_adwTextureStageState[TEXTURE_STAGE_NUM][D3DTSS_MAX];
	static DWORD                       ms_adwTextureStageStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
	static D3DTEXTURESTAGESTATETYPE    ms_aeTextureStageTypeStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
	static DWORD                       ms_adwTextureStageValueStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
	static int                         ms_aiTextureStageStackPointer[TEXTURE_STAGE_STACK_SET_SIZE];
	static int                         ms_iTextureStageSetStackPtr;

#ifdef SAMPLER_STAGE_NUM

	static DWORD                       ms_adwSamplerState[SAMPLER_STAGE_NUM][D3DSSS_MAX];
	static DWORD                       ms_adwSamplerStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
	static D3DSAMPLERSTATETYPE         ms_aeSamplerTypeStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
	static DWORD                       ms_adwSamplerValueStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
	static int                         ms_aiSamplerStackPointer[SAMPLER_STAGE_STACK_SET_SIZE];
	static int                         ms_iSamplerStackSetPtr;

#endif // #ifdef SAMPLER_STAGE_NUM

	//static D3DVIEWPORT                ms_d3dViewport;

	static LPDIRECT3DSURFACE          ms_pd3dBackBuffer, ms_pd3dBackBufferZStencil;
	static D3DSURFACE_DESC            ms_d3dBackBufferDesc, ms_d3dBackBufferZStencilDesc;
	static D3DVIEWPORT                ms_d3dBackBufferViewport;
	static LPDIRECT3DSURFACE          ms_pd3dRenderTarget, ms_pd3dRenderTargetZStencil;
	static D3DVIEWPORT                ms_d3dRenderTargetViewport;

	static D3DDISPLAYMODE               ms_d3dDisplayMode;

	static D3DFORMAT                    ms_d3dColorKeyTextureFormat;
	static D3DFORMAT                    ms_d3dAlphaTextureFormat;

private:

	static HRESULT  _SetPossibleColorKeyTextureFormat();
	static HRESULT  _SetPossibleAlphaTextureFormat();

public:

	//static void             BeginScene()
	//{
	//    if ( ms_iBeginSceneCounter++ == 0 )
	//    {
	//        extern LPDIRECT3DDEVICE   g_pd3dDevice;
	//        HRESULT hr = g_pd3dDevice->BeginScene();
	//        ASSERT( SUCCEEDED( hr ) );
	//    }//if
	//}//BeginScene()

	//static void             EndScene( BOOL bPresent = TRUE )
	//{
	//    if ( --ms_iBeginSceneCounter == 0 )
	//    {
	//        extern LPDIRECT3DDEVICE   g_pd3dDevice;
	//        HRESULT hr = g_pd3dDevice->EndScene();
	//        ASSERT( SUCCEEDED( hr ) );
	//        if ( bPresent )
	//            g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	//    }//if
	//    ASSERT( ms_iBeginSceneCounter >= 0 );
	//}//EndScene()

	static void            InvalidateDeviceObjects();
	static void            RestoreDeviceObjects(
		UINT uiAdapter, D3DDEVTYPE d3dDeviceType);

	static __forceinline void   UpdateRenderState( D3DRENDERSTATETYPE State )
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );

		extern LPDIRECT3DDEVICE   g_pd3dDevice;
		HRESULT hr = g_pd3dDevice->SetRenderState( State, ms_adwRenderState[State] );
		ASSERT( SUCCEEDED( hr ) );
	}//UpdateRenderState()

	static __forceinline HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );

		if ( ms_adwRenderState[State] != Value )
		{
			extern LPDIRECT3DDEVICE   g_pd3dDevice;
			HRESULT hr = g_pd3dDevice->SetRenderState( State, Value );
			ASSERT( SUCCEEDED( hr ) );
			ms_adwRenderState[State] = Value;
			return hr;
		}//if

		return S_OK;
	}//SetRenderState()
	static __forceinline  void  PushRenderStateSet()
	{
		ASSERT( ms_iRenderStateSetStackPtr < RENDER_STATE_STACK_SET_SIZE );
		ms_iRenderStateSetStackPtr++;
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] == 0 );
	}
	static __forceinline  void  PushRenderState( D3DRENDERSTATETYPE State )
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] < RENDER_STATE_STACK_SIZE );
		ASSERT( ms_iRenderStateSetStackPtr < RENDER_STATE_STACK_SET_SIZE && ms_iRenderStateSetStackPtr >= 0 );

		ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] = State;
		ms_adwRenderValueStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] = ms_adwRenderState[State];
		ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]++;
	}//PushRenderState();

	static __forceinline  void    PushRenderState(D3DRENDERSTATETYPE State, DWORD Value)
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] < RENDER_STATE_STACK_SIZE );
		ASSERT( ms_iRenderStateSetStackPtr < RENDER_STATE_STACK_SET_SIZE && ms_iRenderStateSetStackPtr >= 0 );

		ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] = State;
		ms_adwRenderValueStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] = ms_adwRenderState[State];
		ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]++;

		SetRenderState( State, Value );
	}//PushRenderState()

	static __forceinline void     PopRenderState(D3DRENDERSTATETYPE State)
	{
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] > 0 );
		ASSERT( ms_iRenderStateSetStackPtr < RENDER_STATE_STACK_SET_SIZE && ms_iRenderStateSetStackPtr >= 0 );

		ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]--;

		ASSERT( ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] == State );
		SetRenderState( ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]],
			ms_adwRenderValueStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr]] );
	}//PopRenderState()
	static __forceinline void    PopRenderState()
	{
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] > 0 );
		ASSERT( ms_iRenderStateSetStackPtr > 0 );
		PopRenderState( ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] - 1] );
	}
	static __forceinline void    PopRenderStateSet()
	{
		ASSERT( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] > 0 );
		ASSERT( ms_iRenderStateSetStackPtr > 0 );
		while( ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] > 0 )
		{
			PopRenderState( ms_aeRenderStateStack[ms_iRenderStateSetStackPtr][ms_aiRenderStateStackPointer[ms_iRenderStateSetStackPtr] - 1] );
		}
		ms_iRenderStateSetStackPtr--;
	}
	static __forceinline void    GetRenderState(D3DRENDERSTATETYPE State, DWORD*  pValue)
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );
		*pValue = ms_adwRenderState[State];
	}//GetRenderState()
	static __forceinline DWORD    GetRenderState(D3DRENDERSTATETYPE State)
	{
		ASSERT( State >= D3DRENDERSTATETYPE(0) && State < D3DRS_MAX );
		return  ms_adwRenderState[State];
	}//GetRenderState()

	static __forceinline void     UpdateTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type)
	{
		ASSERT( Stage < TEXTURE_STAGE_NUM );
		ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
		extern LPDIRECT3DDEVICE   g_pd3dDevice;
		HRESULT hr = g_pd3dDevice->SetTextureStageState( Stage, Type, ms_adwTextureStageState[Stage][Type] );
		ASSERT( SUCCEEDED( hr ) );
	}//SetTextureStageState()

	static __forceinline void     SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		ASSERT( Stage < TEXTURE_STAGE_NUM );
		ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
		if ( ms_adwTextureStageState[Stage][Type] != Value )
		{
			extern LPDIRECT3DDEVICE   g_pd3dDevice;
			HRESULT hr = g_pd3dDevice->SetTextureStageState( Stage, Type, Value );
			ASSERT( SUCCEEDED( hr ) );
			ms_adwTextureStageState[Stage][Type] = Value;
		}//if
	}//SetTextureStageState()
	static __forceinline  void    PushTextureStageStateSet()
	{
		ASSERT( ms_iTextureStageSetStackPtr < TEXTURE_STAGE_STACK_SET_SIZE );
		ms_iTextureStageSetStackPtr++;
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] == 0 );
	}
	static __forceinline  void    PushTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type )
	{
		ASSERT( Stage < TEXTURE_STAGE_NUM );
		ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] < TEXTURE_STAGE_STACK_SIZE );
		ASSERT( ms_iTextureStageSetStackPtr < TEXTURE_STAGE_STACK_SET_SIZE && ms_iTextureStageSetStackPtr >= 0 );

		ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = Stage;
		ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = Type;
		ms_adwTextureStageValueStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = ms_adwTextureStageState[Stage][Type];
		ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]++;
	}//PushTextureStageState()

	static __forceinline  void    PushTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		ASSERT( Stage < TEXTURE_STAGE_NUM );
		ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] < TEXTURE_STAGE_STACK_SIZE );
		ASSERT( ms_iTextureStageSetStackPtr < TEXTURE_STAGE_STACK_SET_SIZE && ms_iTextureStageSetStackPtr >= 0 );

		ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = Stage;
		ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = Type;
		ms_adwTextureStageValueStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] = ms_adwTextureStageState[Stage][Type];
		ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]++;

		SetTextureStageState(Stage, Type, Value);
	}//PushTextureStageState()

	static __forceinline void     PopTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type)
	{
		ASSERT( Stage < TEXTURE_STAGE_NUM );
		ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] > 0 );
		ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]--;

		ASSERT( ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] == Stage );
		ASSERT( ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]] == Type );

		SetTextureStageState(ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]]
		, ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]]
		, ms_adwTextureStageValueStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr]]);
	}//PopRenderState()
	static __forceinline void   PopTextureStageState()
	{
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] > 0 );
		ASSERT( ms_iTextureStageSetStackPtr > 0 );
		PopTextureStageState( ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] - 1],
			ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] - 1]);
	}
	static __forceinline void   PopTextureStageStateSet()
	{
		ASSERT( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] > 0 );
		ASSERT( ms_iTextureStageSetStackPtr > 0 );
		while( ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] > 0 )
		{
			PopTextureStageState( ms_adwTextureStageStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] - 1],
				ms_aeTextureStageTypeStack[ms_iTextureStageSetStackPtr][ms_aiTextureStageStackPointer[ms_iTextureStageSetStackPtr] - 1]);
		}
		ms_iTextureStageSetStackPtr--;
	}
#ifdef SAMPLER_STAGE_NUM

	static __forceinline void     UpdateSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		ASSERT( Stage < SAMPLER_STAGE_NUM );
		ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
		extern LPDIRECT3DDEVICE   g_pd3dDevice;
		HRESULT hr = g_pd3dDevice->SetSamplerState( Stage, Type, ms_adwSamplerState[Stage][Type] );
		ASSERT( SUCCEEDED( hr ) );
		ms_adwSamplerState[Stage][Type] = Value;
	}//UpdateSamplerState()

	static __forceinline void     SetSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		ASSERT( Stage < SAMPLER_STAGE_NUM );
		ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
		if ( ms_adwSamplerState[Stage][Type] != Value )
		{
			extern LPDIRECT3DDEVICE   g_pd3dDevice;
			HRESULT hr = g_pd3dDevice->SetSamplerState( Stage, Type, Value );
			ASSERT( SUCCEEDED( hr ) );
			ms_adwSamplerState[Stage][Type] = Value;
		}//if
	}//SetSamplerState()
	static __forceinline  void    PushSamplerStateSet()
	{
		ASSERT( ms_iSamplerStackSetPtr < SAMPLER_STAGE_STACK_SET_SIZE );
		ms_iSamplerStackSetPtr++;
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] == 0 );
	}
	static __forceinline  void    PushSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type )
	{
		ASSERT( Stage < SAMPLER_STAGE_NUM );
		ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] <  SAMPLER_STAGE_STACK_SIZE );
		ASSERT( ms_iSamplerStackSetPtr < SAMPLER_STAGE_STACK_SET_SIZE && ms_iSamplerStackSetPtr >= 0 );

		ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = Stage;
		ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = Type;
		ms_adwSamplerValueStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = ms_adwSamplerState[Stage][Type];
		ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]++;
	}//PushSamplerState()

	static __forceinline  void    PushSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		ASSERT( Stage < SAMPLER_STAGE_NUM );
		ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] <  SAMPLER_STAGE_STACK_SIZE );
		ASSERT( ms_iSamplerStackSetPtr < SAMPLER_STAGE_STACK_SET_SIZE && ms_iSamplerStackSetPtr >= 0 );

		ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = Stage;
		ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = Type;
		ms_adwSamplerValueStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] = ms_adwSamplerState[Stage][Type];
		ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]++;

		SetSamplerState(Stage, Type, Value);
	}//PushSamplerState()

	static __forceinline void     PopSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type)
	{
		ASSERT( Stage < SAMPLER_STAGE_NUM );
		ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DTSS_MAX );
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] > 0 );
		ASSERT( ms_iSamplerStackSetPtr < SAMPLER_STAGE_STACK_SET_SIZE && ms_iSamplerStackSetPtr >= 0 );
		ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]--;

		ASSERT( ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] == Stage );
		ASSERT( ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]] == Type );

		SetSamplerState(ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]]
		, ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]]
		, ms_adwSamplerValueStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr]]);
	}//PopRenderState()
	static __forceinline void    PopSamplerState()
	{
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] > 0 );
		ASSERT( ms_iSamplerStackSetPtr > 0 );
		PopSamplerState( ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] - 1],
			ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] - 1]);
	}
	static __forceinline void    PopSamplerStateSet()
	{
		ASSERT( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] > 0 );
		ASSERT( ms_iSamplerStackSetPtr > 0 );
		while( ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] > 0 )
		{
			PopSamplerState( ms_adwSamplerStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] - 1],
				ms_aeSamplerTypeStack[ms_iSamplerStackSetPtr][ms_aiSamplerStackPointer[ms_iSamplerStackSetPtr] - 1]);
		}
		ms_iSamplerStackSetPtr--;
	}
#endif // #ifdef SAMPLER_STAGE_NUM

	static __forceinline void SetViewport(const D3DVIEWPORT *pViewport)
	{
		extern LPDIRECT3DDEVICE   g_pd3dDevice;
		HRESULT hr = g_pd3dDevice->SetViewport( pViewport );
		ASSERT( SUCCEEDED( hr ) );
		CopyMemory( &ms_d3dRenderTargetViewport, pViewport, sizeof(D3DVIEWPORT) );
	}//SetViewport()

	static __forceinline void     GetViewport(D3DVIEWPORT *pViewport)
	{
		CopyMemory( pViewport, &ms_d3dRenderTargetViewport, sizeof(D3DVIEWPORT) );
	}//GetViewport()

	static __forceinline const D3DVIEWPORT* GetViewport() { return  &ms_d3dRenderTargetViewport; }
	static __forceinline const D3DVIEWPORT* GetBackBufferViewport() { return &ms_d3dBackBufferViewport; }

	static __forceinline void SetRenderTarget(LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pNewZStencil)
	{
		extern LPDIRECT3DDEVICE   g_pd3dDevice;

		//ASSERT( ms_pd3dRenderTarget == ms_pd3dBackBuffer );
		//ASSERT( ms_pd3dRenderTargetZStencil == ms_pd3dBackBufferZStencil );

		D3DSURFACE_DESC    desc;
		HRESULT hr = pRenderTarget->GetDesc( &desc );
		ASSERT( SUCCEEDED( hr ) );

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( pRenderTarget, pNewZStencil );
		ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( 0, pRenderTarget );
		ASSERT( SUCCEEDED( hr ) );
		hr = g_pd3dDevice->SetDepthStencilSurface( pNewZStencil );
		ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

		D3DVIEWPORT viewport;
		viewport.X = viewport.Y = 0;
		viewport.Width = desc.Width;
		viewport.Height = desc.Height;
		viewport.MinZ = 0.f;
		viewport.MaxZ = 1.f;
		SetViewport( &viewport );

		ms_pd3dRenderTarget = pRenderTarget;
		ms_pd3dRenderTargetZStencil = pNewZStencil;
	}//SetRenderTarget()

	static __forceinline void SetRenderTarget(LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pNewZStencil
		, const D3DSURFACE_DESC* pSurfaceDesc )
	{
		extern LPDIRECT3DDEVICE   g_pd3dDevice;

		//ASSERT( ms_pd3dRenderTarget == ms_pd3dBackBuffer );
		//ASSERT( ms_pd3dRenderTargetZStencil == ms_pd3dBackBufferZStencil );

		//CopyMemory( &ms_d3dViewportSaved, &ms_d3dViewport, sizeof(D3DVIEWPORT) );

		HRESULT hr;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( pRenderTarget, pNewZStencil );
		ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( 0, pRenderTarget );
		ASSERT( SUCCEEDED( hr ) );
		hr = g_pd3dDevice->SetDepthStencilSurface( pNewZStencil );
		ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

		D3DVIEWPORT viewport;
		viewport.X = 0; viewport.Y = 0;
		viewport.Width = pSurfaceDesc->Width;
		viewport.Height = pSurfaceDesc->Height;
		viewport.MinZ = 0.f;
		viewport.MaxZ = 1.f;
		SetViewport( &viewport );

		ms_pd3dRenderTarget = pRenderTarget;
		ms_pd3dRenderTargetZStencil = pNewZStencil;
	}//SetRenderTarget()

	static __forceinline void RestoreRenderTarget()
	{
		extern LPDIRECT3DDEVICE   g_pd3dDevice;

		ms_pd3dRenderTarget = ms_pd3dBackBuffer;
		ms_pd3dRenderTargetZStencil = ms_pd3dBackBufferZStencil;

		HRESULT hr;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( ms_pd3dRenderTarget, ms_pd3dRenderTargetZStencil );
		ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

		hr = g_pd3dDevice->SetRenderTarget( 0, ms_pd3dRenderTarget );
		ASSERT( SUCCEEDED( hr ) );
		hr = g_pd3dDevice->SetDepthStencilSurface( ms_pd3dRenderTargetZStencil );
		ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

		SetViewport( &ms_d3dBackBufferViewport );
	}//RestoreRenderTarget()

	static __forceinline LPDIRECT3DSURFACE   GetRenderTarget()               { return ms_pd3dRenderTarget; }
	static __forceinline LPDIRECT3DSURFACE   GetZStencilSurface()            { return ms_pd3dRenderTargetZStencil; }
	static __forceinline LPDIRECT3DSURFACE   GetBackBuffer()                 { return ms_pd3dBackBuffer; }
	static __forceinline LPDIRECT3DSURFACE   GetBackBufferZStencil()         { return ms_pd3dBackBufferZStencil; }
	static __forceinline const D3DSURFACE_DESC&     GetBackBufferDesc()       { return ms_d3dBackBufferDesc; }
	static __forceinline const D3DSURFACE_DESC&     GetBackBufferZStencilDesc() { return ms_d3dBackBufferZStencilDesc; }

	static __forceinline const D3DDISPLAYMODE       GetDisplayMode()             { return ms_d3dDisplayMode; }

	static HRESULT CheckDeviceFormat( DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat );
	static HRESULT CheckDepthStencilMatch( D3DFORMAT BackBufferFormat, D3DFORMAT DepthFormat );

	static void ClearBackBuffer( D3DCOLOR d3dColor = 0xffffffff, float fZ = 1.0f, DWORD dwStencil = 0x00000000 );

	__forceinline static int  GetAlphaBits( D3DFORMAT d3dFormat )
	{
		switch( d3dFormat )
		{
		case D3DFMT_A1R5G5B5:
			return 1;
		case D3DFMT_A4R4G4B4:
			return 4;
		case D3DFMT_A8R3G3B2:
			return 8;
		case D3DFMT_A8R8G8B8:
			return 8;
		case D3DFMT_A2B10G10R10:
			return 2;
#if ( DIRECT3D_VERSION >= 0x0900 )
		case D3DFMT_A2R10G10B10:
			return 2;
		case D3DFMT_A16B16G16R16:
			return 1;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
		}//switch
		return 0;
	}//

	__forceinline static int  GetFormatDepth( D3DFORMAT d3dFormat )
	{
		switch( d3dFormat )
		{
		case D3DFMT_R3G3B2:
			return 8;
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_X4R4G4B4:
		case D3DFMT_A4R4G4B4:
		case D3DFMT_A8R3G3B2:
			return 16;
		case D3DFMT_R8G8B8:
			return 24;
		case D3DFMT_X8R8G8B8:
		case D3DFMT_A8R8G8B8:
		case D3DFMT_A2B10G10R10:
			return 32;
#if ( DIRECT3D_VERSION >= 0x0900 )
		case D3DFMT_A2R10G10B10:
			return 32;
		case D3DFMT_A16B16G16R16:
			return 64;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
		}//switch
		return 0;
	}//

	__forceinline static int    GetStencilDepth( D3DFORMAT d3dFormat )
	{
		switch( d3dFormat )
		{
		case D3DFMT_D15S1:
			return 1;
		case D3DFMT_D24X4S4:
			return 4;
		case D3DFMT_D24S8:
			return 8;
#if ( DIRECT3D_VERSION >= 0x0900 )
		case D3DFMT_D24FS8:
			return 8;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
		}//switch
		return 0;
	}//

	__forceinline static D3DFORMAT GetColorKeyTextureFormat()  { return ms_d3dColorKeyTextureFormat; }

	/// get alpha texture surface format.
	__forceinline static D3DFORMAT GetAlphaTextureFormat()     { return ms_d3dAlphaTextureFormat; }

	__forceinline static HRESULT   SetFVF( DWORD dwFVF )
	{
		extern LPDIRECT3DDEVICE   g_pd3dDevice;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

		return  g_pd3dDevice->SetVertexShader( dwFVF );

#elif ( DIRECT3D_VERSION >= 0x0900 )

		return  g_pd3dDevice->SetFVF( dwFVF );
#endif// #if.. #elif
		return E_FAIL;
	}// SetFVF()
};//class   KD3DDeviceState

//** @example    class KD3DDeviceState
//
//  1) OnResetDevice():
//
//    KD3DDeviceState::RestoreDeviceObjects( 0, D3DDEVTYPE_HAL );
//
//    // Setup render state
//    KD3DDeviceState::SetRenderState( D3DRS_LIGHTING, TRUE );
//    KD3DDeviceState::SetRenderState( D3DRS_DITHERENABLE, TRUE );
//    KD3DDeviceState::SetRenderState( D3DRS_ZENABLE, TRUE );
//    KD3DDeviceState::SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
//    KD3DDeviceState::SetRenderState( D3DRS_AMBIENT, 0x33333333 );
//    KD3DDeviceState::SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
//    KD3DDeviceState::SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//    KD3DDeviceState::SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//
//  2) OnFrameRender():
//
//    KD3DDeviceState::PushRenderState( D3DRS_LIGHTING, FALSE );
//    g_stdMesh.DrawMesh();
//    KD3DDeviceState::PopRenderState( D3DRS_LIGHTING );
//
//  2) OnLostDevice():
//
//    KD3DDeviceState::InvalidateDeviceObjects();
//
//*/
