#include "stdafx.h"
//#include ".\kd3ddevicestate.h"
//
///** @file   KD3DDeviceState.cpp
//@desc   implementation of KD3DDeviceState
//*/
//
//#ifdef PROJECT_PCH
//#define _TOSTR1(x)    #x
//#define _TOSTR(x)     _TOSTR1(x)
//#include _TOSTR(PROJECT_PCH)
//#endif // PROJECT_PCH
//#include "KJohnGen.h"
//#include "KD3DDeviceState.h"
//
//extern LPDIRECT3DDEVICE9        g_pd3dDevice;
//extern LPDIRECT3D9              g_pD3D;
//
//
//BOOL                        KD3DDeviceState::ms_bRestored = FALSE;
//
//
//UINT                        KD3DDeviceState::ms_uiAdapter = D3DADAPTER_DEFAULT;
//D3DDEVTYPE                  KD3DDeviceState::ms_d3dDeviceType = D3DDEVTYPE_HAL;
//
////int                         KD3DDeviceState::ms_iBeginSceneCounter = 0;
//
//DWORD                       KD3DDeviceState::ms_adwRenderState[D3DRS_MAX];
//D3DRENDERSTATETYPE          KD3DDeviceState::ms_aeRenderStateStack[RENDER_STATE_STACK_SET_SIZE][RENDER_STATE_STACK_SIZE];
//DWORD                       KD3DDeviceState::ms_adwRenderValueStack[RENDER_STATE_STACK_SET_SIZE][RENDER_STATE_STACK_SIZE];
//int                         KD3DDeviceState::ms_aiRenderStateStackPointer[RENDER_STATE_STACK_SET_SIZE];
//int                         KD3DDeviceState::ms_iRenderStateSetStackPtr;
//
//DWORD                       KD3DDeviceState::ms_adwTextureStageState[TEXTURE_STAGE_NUM][D3DTSS_MAX];
//DWORD                       KD3DDeviceState::ms_adwTextureStageStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
//D3DTEXTURESTAGESTATETYPE    KD3DDeviceState::ms_aeTextureStageTypeStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
//DWORD                       KD3DDeviceState::ms_adwTextureStageValueStack[TEXTURE_STAGE_STACK_SET_SIZE][TEXTURE_STAGE_STACK_SIZE];
//int                         KD3DDeviceState::ms_aiTextureStageStackPointer[TEXTURE_STAGE_STACK_SET_SIZE];
//int                         KD3DDeviceState::ms_iTextureStageSetStackPtr;
//
//#ifdef SAMPLER_STAGE_NUM
//
//DWORD                       KD3DDeviceState::ms_adwSamplerState[SAMPLER_STAGE_NUM][D3DSSS_MAX];
//DWORD                       KD3DDeviceState::ms_adwSamplerStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
//D3DSAMPLERSTATETYPE         KD3DDeviceState::ms_aeSamplerTypeStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
//DWORD                       KD3DDeviceState::ms_adwSamplerValueStack[SAMPLER_STAGE_STACK_SET_SIZE][SAMPLER_STAGE_STACK_SIZE];
//int                         KD3DDeviceState::ms_aiSamplerStackPointer[SAMPLER_STAGE_STACK_SET_SIZE];
//int                         KD3DDeviceState::ms_iSamplerStackSetPtr;
//
//#endif // #ifdef SAMPLER_STAGE_NUM
//
//
////D3DVIEWPORT                KD3DDeviceState::ms_d3dViewport;
//
//LPDIRECT3DSURFACE          KD3DDeviceState::ms_pd3dBackBuffer = NULL;
//LPDIRECT3DSURFACE          KD3DDeviceState::ms_pd3dBackBufferZStencil = NULL;
//D3DSURFACE_DESC             KD3DDeviceState::ms_d3dBackBufferDesc;
//D3DSURFACE_DESC             KD3DDeviceState::ms_d3dBackBufferZStencilDesc;
//D3DVIEWPORT                KD3DDeviceState::ms_d3dBackBufferViewport;
//LPDIRECT3DSURFACE          KD3DDeviceState::ms_pd3dRenderTarget = NULL;
//LPDIRECT3DSURFACE          KD3DDeviceState::ms_pd3dRenderTargetZStencil = NULL;
//D3DVIEWPORT                KD3DDeviceState::ms_d3dRenderTargetViewport;
//
//D3DDISPLAYMODE              KD3DDeviceState::ms_d3dDisplayMode;
//
//D3DFORMAT                   KD3DDeviceState::ms_d3dColorKeyTextureFormat = D3DFMT_UNKNOWN;
//D3DFORMAT                   KD3DDeviceState::ms_d3dAlphaTextureFormat = D3DFMT_UNKNOWN;
//
//
//void    KD3DDeviceState::InvalidateDeviceObjects()
//{
//    if ( !ms_bRestored )
//        return;
//
//    memset( ms_aiRenderStateStackPointer, 0, sizeof(int)*RENDER_STATE_STACK_SET_SIZE );
//    memset( ms_aiTextureStageStackPointer, 0, sizeof(int)*TEXTURE_STAGE_STACK_SET_SIZE );
//    ms_iRenderStateSetStackPtr = 0;
//    ms_iTextureStageSetStackPtr = 0;
//#ifdef SAMPLER_STAGE_NUM
//    memset( ms_aiSamplerStackPointer, 0, sizeof(int)*SAMPLER_STAGE_STACK_SET_SIZE );
//    ms_iSamplerStackSetPtr = 0;
//#endif // #ifdef SAMPLER_STAGE_NUM
//
//    SAFE_RELEASE( ms_pd3dBackBuffer );
//    SAFE_RELEASE( ms_pd3dBackBufferZStencil );
//
//    ms_pd3dRenderTarget = NULL;
//    ms_pd3dRenderTargetZStencil = NULL;
//
//    ms_bRestored = FALSE;
//
//    //SAFE_DELETE( ms_pd3dVertexShaderConstant );
//    //ms_iNumVertexShaderConstant = 0;
//}//InvalidateDeviceObjects()
//
//void    KD3DDeviceState::RestoreDeviceObjects(
//    UINT uiAdapter, D3DDEVTYPE d3dDeviceType)
//{
//    if ( ms_bRestored )
//        return;
//
//    ASSERT( g_pd3dDevice != NULL );
//
//    ms_uiAdapter = uiAdapter;
//    ms_d3dDeviceType = d3dDeviceType;
//
//    //ASSERT( ms_iBeginSceneCounter == 0 );
//    ASSERT( ms_aiRenderStateStackPointer[0] == 0 );
//    ASSERT( ms_aiTextureStageStackPointer[0] == 0 );
//    ASSERT( ms_iRenderStateSetStackPtr == 0 );
//    ASSERT( ms_iTextureStageSetStackPtr == 0 );
//#ifdef SAMPLER_STAGE_NUM
//    ASSERT( ms_aiSamplerStackPointer[0] == 0 );
//    ASSERT( ms_iSamplerStackSetPtr == 0 );
//#endif // #ifdef SAMPLER_STAGE_NUM
//    ASSERT( ms_pd3dBackBuffer == NULL );
//    ASSERT( ms_pd3dBackBufferZStencil == NULL );
//    ASSERT( ms_pd3dRenderTarget == NULL );
//    ASSERT( ms_pd3dRenderTargetZStencil == NULL );
//    //ASSERT( ms_pd3dVertexShaderConstant == NULL );
//    //ASSERT( ms_iNumVertexShaderConstant == 0 );
//
//    int i, j;
//    HRESULT hr;
//
//
//
//    //for ( i = 0; i < D3DRS_MAX; i++ )
//    //{
//    //    hr = g_pd3dDevice->GetRenderState( D3DRENDERSTATETYPE( i ), &ms_adwRenderState[i] );
//    //    ASSERT( SUCCEEDED( hr ) );
//    //}//for
//
//    //for ( i = 0; i < TEXTURE_STAGE_NUM; i++ )
//    //{
//    //    for ( j = 0; j < D3DTSS_MAX; j ++ )
//    //    {
//    //        hr = g_pd3dDevice->GetTextureStageState( i, D3DTEXTURESTAGESTATETYPE( j )
//    //            , &ms_adwTextureStageState[i][j] );
//    //        ASSERT( SUCCEEDED( hr ) );
//    //    }
//    //}//for
//
//    static const float   fZero = 0.0f;
//    static const float   fOne = 1.0f;
//    static const float   f64 = 64.0f;
//    static const DWORD   dwfZero = *((const DWORD*) (&fZero));
//    static const DWORD   dwfOne = *((const DWORD*) (&fOne));
//    static const DWORD   dwf64 = *((const DWORD*) (&f64));
//
//    struct StateValue
//    {
//        D3DRENDERSTATETYPE  eType;
//        DWORD               dwValue;
//    };
//
//    typedef DWORD   TextureStateValue[TEXTURE_STAGE_NUM + 1];
//
//#ifdef _XBOX
//
//    static StateValue  aeDRS[] = 
//    {
//        { D3DRS_ZFUNC,    D3DCMP_LESSEQUAL },
//        { D3DRS_ALPHAFUNC,    D3DCMP_ALWAYS },
//        { D3DRS_ALPHABLENDENABLE,    FALSE },
//        { D3DRS_ALPHATESTENABLE,    FALSE },
//        { D3DRS_ALPHAREF,    0 },
//        { D3DRS_SRCBLEND,    D3DBLEND_ONE },
//        { D3DRS_DESTBLEND,    D3DBLEND_ZERO },
//        { D3DRS_ZWRITEENABLE,    TRUE },
//        { D3DRS_DITHERENABLE,    FALSE },
//        { D3DRS_SHADEMODE,    D3DSHADE_GOURAUD },
//        { D3DRS_COLORWRITEENABLE,    D3DCOLORWRITEENABLE_ALL },
//        { D3DRS_STENCILZFAIL,    D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILPASS,    D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILFUNC,    D3DCMP_ALWAYS },
//        { D3DRS_STENCILREF,    0 },
//        { D3DRS_STENCILMASK,    0xffffffff },
//        { D3DRS_STENCILWRITEMASK,    0xffffffff },
//        { D3DRS_BLENDOP,    D3DBLENDOP_ADD },
//        { D3DRS_BLENDCOLOR,    0 },
//        { D3DRS_SWATHWIDTH,    D3DSWATH_128 },
//        { D3DRS_POLYGONOFFSETZSLOPESCALE,    dwfZero },
//        { D3DRS_POLYGONOFFSETZOFFSET,    dwfZero },
//        { D3DRS_POINTOFFSETENABLE,    FALSE },
//        { D3DRS_WIREFRAMEOFFSETENABLE,    FALSE },
//        { D3DRS_SOLIDOFFSETENABLE,    FALSE },
//        { D3DRS_DEPTHCLIPCONTROL,    D3DDCC_CULLPRIMITIVE },
//        { D3DRS_STIPPLEENABLE,    FALSE },
//        { D3DRS_FOGENABLE,    FALSE },
//        { D3DRS_FOGTABLEMODE,    D3DFOG_NONE },
//        { D3DRS_FOGSTART,    dwfZero },
//        { D3DRS_FOGEND,    dwfOne },
//        { D3DRS_FOGDENSITY,    dwfOne },
//        { D3DRS_RANGEFOGENABLE,    FALSE },
//        { D3DRS_WRAP0,    0 },
//        { D3DRS_WRAP1,    0 },
//        { D3DRS_WRAP2,    0 },
//        { D3DRS_WRAP3,    0 },
//        { D3DRS_LIGHTING,    TRUE },
//        { D3DRS_SPECULARENABLE,    FALSE },
//        { D3DRS_LOCALVIEWER,    TRUE },
//        { D3DRS_COLORVERTEX,    TRUE },
//        { D3DRS_BACKSPECULARMATERIALSOURCE,    D3DMCS_COLOR2 },
//        { D3DRS_BACKDIFFUSEMATERIALSOURCE,    D3DMCS_COLOR1 },
//        { D3DRS_BACKAMBIENTMATERIALSOURCE,    D3DMCS_MATERIAL },
//        { D3DRS_BACKEMISSIVEMATERIALSOURCE,    D3DMCS_MATERIAL },
//        { D3DRS_SPECULARMATERIALSOURCE,    D3DMCS_COLOR2 },
//        { D3DRS_DIFFUSEMATERIALSOURCE,    D3DMCS_COLOR1 },
//        { D3DRS_AMBIENTMATERIALSOURCE,    D3DMCS_MATERIAL },
//        { D3DRS_EMISSIVEMATERIALSOURCE,    D3DMCS_MATERIAL },
//        { D3DRS_BACKAMBIENT,    0 },
//        { D3DRS_AMBIENT,    0 },
//        { D3DRS_POINTSIZE,    dwfOne },
//        { D3DRS_POINTSIZE_MIN,    dwfZero },
//        { D3DRS_POINTSPRITEENABLE,    FALSE },
//        { D3DRS_POINTSCALEENABLE,    FALSE },
//        { D3DRS_POINTSCALE_A,    dwfOne },
//        { D3DRS_POINTSCALE_B,    dwfZero },
//        { D3DRS_POINTSCALE_C,    dwfZero },
//        { D3DRS_POINTSIZE_MAX,    dwf64 },
//        { D3DRS_PATCHEDGESTYLE,    D3DPATCHEDGE_DISCRETE },
//        { D3DRS_PATCHSEGMENTS,    dwfOne },
//        { D3DRS_SWAPFILTER,    D3DTEXF_LINEAR },
//        { D3DRS_PRESENTATIONINTERVAL,    D3DPRESENT_INTERVAL_DEFAULT | D3DPRESENT_INTERVAL_ONE },
//        { D3DRS_PSTEXTUREMODES,    0 },
//        { D3DRS_VERTEXBLEND,    D3DVBF_DISABLE },
//        { D3DRS_FOGCOLOR,    0 },
//        { D3DRS_FILLMODE,    D3DFILL_SOLID },
//        { D3DRS_BACKFILLMODE,    D3DFILL_SOLID },
//        { D3DRS_TWOSIDEDLIGHTING,    FALSE },
//        { D3DRS_NORMALIZENORMALS,    FALSE },
//        { D3DRS_ZENABLE,    D3DZB_TRUE },
//        { D3DRS_STENCILENABLE,    FALSE },
//        { D3DRS_STENCILFAIL,    D3DSTENCILOP_KEEP },
//        { D3DRS_FRONTFACE,    D3DFRONT_CW },
//        { D3DRS_CULLMODE,    D3DCULL_CCW },
//        { D3DRS_TEXTUREFACTOR,    0xffffffff },
//        { D3DRS_ZBIAS,    0 },
//        { D3DRS_LOGICOP,    D3DLOGICOP_NONE },
//        { D3DRS_EDGEANTIALIAS,    FALSE },
//        { D3DRS_MULTISAMPLEANTIALIAS,    TRUE },
//        { D3DRS_MULTISAMPLEMASK,    0xffffffff },
//        { D3DRS_MULTISAMPLEMODE,    D3DMULTISAMPLEMODE_1X },
//        { D3DRS_MULTISAMPLERENDERTARGETMODE,    D3DMULTISAMPLEMODE_1X },
//        { D3DRS_SHADOWFUNC,    D3DCMP_NEVER },
//        { D3DRS_LINEWIDTH,    dwfOne },
//        { D3DRS_SAMPLEALPHA,    0 },
//        { D3DRS_DXT1NOISEENABLE,    TRUE },
//        { D3DRS_YUVENABLE,    FALSE },
//        { D3DRS_OCCLUSIONCULLENABLE,    TRUE },
//        { D3DRS_STENCILCULLENABLE,    TRUE },
//        { D3DRS_ROPZCMPALWAYSREAD,    FALSE },
//        { D3DRS_ROPZREAD,    FALSE },
//        { D3DRS_DONOTCULLUNCOMPRESSED,    FALSE },
//    };
//
//    static TextureStateValue  aeDTSS[] =
//    {
//        { D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DTSS_ADDRESSW, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DSAMP_MAGFILTER,    D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT },
//        { D3DSAMP_MINFILTER,    D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT },
//        { D3DTSS_MIPFILTER,    D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE },
//        { D3DTSS_MIPMAPLODBIAS,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_MAXMIPLEVEL,    0, 0, 0, 0 },
//        { D3DTSS_MAXANISOTROPY,    1, 1, 1, 1 },
//        { D3DTSS_COLORKEYOP,    D3DTCOLORKEYOP_DISABLE,    D3DTCOLORKEYOP_DISABLE, D3DTCOLORKEYOP_DISABLE, D3DTCOLORKEYOP_DISABLE },
//        { D3DTSS_COLORSIGN,    0, 0, 0, 0 },
//        { D3DTSS_ALPHAKILL,    D3DTALPHAKILL_DISABLE, D3DTALPHAKILL_DISABLE, D3DTALPHAKILL_DISABLE, D3DTALPHAKILL_DISABLE },
//        { D3DTSS_COLOROP,        D3DTOP_MODULATE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE },
//        { D3DTSS_COLORARG0,    D3DTA_CURRENT,    D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_COLORARG1,    D3DTA_TEXTURE,    D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE },
//        { D3DTSS_COLORARG2,    D3DTA_CURRENT,    D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_ALPHAOP,    D3DTOP_SELECTARG1,    D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE },
//        { D3DTSS_ALPHAARG0,    D3DTA_CURRENT,    D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_ALPHAARG1,    D3DTA_TEXTURE,    D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE },
//        { D3DTSS_ALPHAARG2,    D3DTA_CURRENT,    D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_RESULTARG,    D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_TEXTURETRANSFORMFLAGS,    D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE },
//        { D3DTSS_BUMPENVMAT00,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVMAT01,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVMAT10,    dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_BUMPENVMAT11,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVLSCALE,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVLOFFSET,    dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_TEXCOORDINDEX,    D3DTSS_TCI_PASSTHRU | 0, D3DTSS_TCI_PASSTHRU | 1, D3DTSS_TCI_PASSTHRU | 2, D3DTSS_TCI_PASSTHRU | 3 },
//        { D3DTSS_BORDERCOLOR,    0, 0, 0, 0 },
//        { D3DTSS_COLORKEYCOLOR,    0, 0, 0, 0 },
//    };
//
//#else
//
//#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )
//
//    static StateValue  aeDRS[] = 
//    {
//        { D3DRS_ZENABLE, D3DZB_TRUE },
//        { D3DRS_FILLMODE, D3DFILL_SOLID },
//        { D3DRS_SHADEMODE, D3DSHADE_GOURAUD },
//        { D3DRS_LINEPATTERN, 0L },
//        { D3DRS_ZWRITEENABLE, TRUE },
//        { D3DRS_ALPHATESTENABLE, FALSE },
//        { D3DRS_LASTPIXEL, TRUE },
//        { D3DRS_SRCBLEND, D3DBLEND_ONE },
//        { D3DRS_DESTBLEND, D3DBLEND_ZERO },
//        { D3DRS_CULLMODE, D3DCULL_CCW },
//        { D3DRS_ZFUNC, D3DCMP_LESSEQUAL },
//        { D3DRS_ALPHAREF, 0L },
//        { D3DRS_ALPHAFUNC, D3DCMP_ALWAYS },
//        { D3DRS_DITHERENABLE, FALSE },
//        { D3DRS_ALPHABLENDENABLE, FALSE },
//        { D3DRS_FOGENABLE, FALSE },
//        { D3DRS_SPECULARENABLE, FALSE },
//        { D3DRS_FOGCOLOR, 0L },
//        { D3DRS_FOGTABLEMODE, D3DFOG_NONE },
//        { D3DRS_FOGSTART, dwfZero },
//        { D3DRS_FOGEND, dwfOne },
//        { D3DRS_FOGDENSITY, dwfOne },
//        { D3DRS_EDGEANTIALIAS, FALSE },
//        { D3DRS_ZBIAS, 0L },
//        { D3DRS_RANGEFOGENABLE, FALSE },
//        { D3DRS_STENCILENABLE, FALSE },
//        { D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILPASS, D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILFUNC, D3DCMP_ALWAYS },
//        { D3DRS_STENCILREF, 0L },
//        { D3DRS_STENCILMASK, 0xFFFFFFFFL },
//        { D3DRS_STENCILWRITEMASK, 0xFFFFFFFFL },
//        { D3DRS_TEXTUREFACTOR, 0xFFFFFFFFL },
//        { D3DRS_WRAP0, 0L },
//        { D3DRS_WRAP1, 0L },
//        { D3DRS_WRAP2, 0L },
//        { D3DRS_WRAP3, 0L },
//        { D3DRS_WRAP4, 0L },
//        { D3DRS_WRAP5, 0L },
//        { D3DRS_WRAP6, 0L },
//        { D3DRS_WRAP7, 0L },
//        { D3DRS_CLIPPING, TRUE },
//        { D3DRS_LIGHTING, TRUE },
//        { D3DRS_AMBIENT, 0L },
//        { D3DRS_FOGVERTEXMODE, D3DFOG_NONE },
//        { D3DRS_COLORVERTEX, TRUE },
//        { D3DRS_LOCALVIEWER, TRUE },
//        { D3DRS_NORMALIZENORMALS, FALSE },
//        { D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 },
//        { D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2 },
//        { D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR2 },
//        { D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL },
//        { D3DRS_VERTEXBLEND, D3DVBF_DISABLE },
//        { D3DRS_CLIPPLANEENABLE, 0L },
//        { D3DRS_SOFTWAREVERTEXPROCESSING, FALSE },
//        { D3DRS_POINTSIZE, dwfOne },
//        { D3DRS_POINTSIZE_MIN, dwfZero },
//        { D3DRS_POINTSPRITEENABLE, FALSE },
//        { D3DRS_POINTSCALEENABLE, FALSE },
//        { D3DRS_POINTSCALE_A, dwfOne },
//        { D3DRS_POINTSCALE_B, dwfZero },
//        { D3DRS_POINTSCALE_C, dwfZero },
//        { D3DRS_MULTISAMPLEANTIALIAS, TRUE },
//        { D3DRS_MULTISAMPLEMASK, 0xFFFFFFFFL },
//        { D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE },
//        { D3DRS_PATCHSEGMENTS, dwfOne },
//        { D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE },
//        { D3DRS_POINTSIZE_MAX, dwf64 },
//        { D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE },
//        { D3DRS_COLORWRITEENABLE, 0x0000000F },
//        { D3DRS_TWEENFACTOR, dwfZero },
//        { D3DRS_BLENDOP, D3DBLENDOP_ADD },
//        { D3DRS_POSITIONORDER, D3DORDER_CUBIC },
//        { D3DRS_NORMALORDER, D3DORDER_LINEAR },
//    };
//
//    static TextureStateValue  aeDTSS[] = {
//        { D3DTSS_COLOROP, D3DTOP_MODULATE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE }, 
//        { D3DTSS_COLORARG1, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE }, 
//        { D3DTSS_COLORARG2, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT }, 
//        { D3DTSS_ALPHAOP, D3DTOP_SELECTARG1, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE }, 
//        { D3DTSS_ALPHAARG1, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE }, 
//        { D3DTSS_ALPHAARG2, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT }, 
//        { D3DTSS_BUMPENVMAT00, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_BUMPENVMAT01, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_BUMPENVMAT10, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_BUMPENVMAT11, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_TEXCOORDINDEX, 0, 1, 2, 3, 4, 5, 6, 7 }, 
//        { D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP }, 
//        { D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP }, 
//        { D3DTSS_BORDERCOLOR, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 }, 
//        { D3DSAMP_MAGFILTER, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT }, 
//        { D3DSAMP_MINFILTER, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT }, 
//        { D3DTSS_MIPFILTER, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE }, 
//        { D3DTSS_MIPMAPLODBIAS, 0L,0L, 0L, 0L, 0L, 0L, 0L, 0L }, 
//        { D3DTSS_MAXMIPLEVEL, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L }, 
//        { D3DTSS_MAXANISOTROPY, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L }, 
//        { D3DTSS_BUMPENVLSCALE, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_BUMPENVLOFFSET, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero }, 
//        { D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE }, 
//        { D3DTSS_ADDRESSW, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP }, 
//        { D3DTSS_COLORARG0, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT }, 
//        { D3DTSS_ALPHAARG0, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT }, 
//        { D3DTSS_RESULTARG, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT }, 
//    };
//
//#elif ( DIRECT3D_VERSION >= 0x0900 )
//
//    static StateValue  aeDRS[] = 
//    {
//        { D3DRS_ZENABLE , D3DZB_TRUE },
//        { D3DRS_FILLMODE , D3DFILL_SOLID },
//        { D3DRS_SHADEMODE , D3DSHADE_GOURAUD },
//        { D3DRS_ZWRITEENABLE , TRUE },
//        { D3DRS_ALPHATESTENABLE , FALSE },
//        { D3DRS_LASTPIXEL , TRUE },
//        { D3DRS_SRCBLEND , D3DBLEND_ONE },
//        { D3DRS_DESTBLEND , D3DBLEND_ZERO },
//        { D3DRS_CULLMODE , D3DCULL_CCW },
//        { D3DRS_ZFUNC , D3DCMP_LESSEQUAL },
//        { D3DRS_ALPHAREF , 0 },
//        { D3DRS_ALPHAFUNC , D3DCMP_ALWAYS },
//        { D3DRS_DITHERENABLE , FALSE },
//        { D3DRS_ALPHABLENDENABLE , FALSE },
//        { D3DRS_FOGENABLE , FALSE },
//        { D3DRS_SPECULARENABLE , FALSE },
//        { D3DRS_FOGCOLOR , 0 },
//        { D3DRS_FOGTABLEMODE , D3DFOG_NONE },
//        { D3DRS_FOGSTART , dwfZero },
//        { D3DRS_FOGEND , dwfOne },
//        { D3DRS_FOGDENSITY , dwfOne },
//        { D3DRS_RANGEFOGENABLE , FALSE },
//        { D3DRS_STENCILENABLE , FALSE },
//        { D3DRS_STENCILFAIL , D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILZFAIL , D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILPASS , D3DSTENCILOP_KEEP },
//        { D3DRS_STENCILFUNC , D3DCMP_ALWAYS },
//        { D3DRS_STENCILREF , 0 },
//        { D3DRS_STENCILMASK , 0xffffffff },
//        { D3DRS_STENCILWRITEMASK , 0xffffffff },
//        { D3DRS_TEXTUREFACTOR , 0xffffffff },
//        { D3DRS_WRAP0 , 0 },
//        { D3DRS_WRAP1 , 0 },
//        { D3DRS_WRAP2 , 0 },
//        { D3DRS_WRAP3 , 0 },
//        { D3DRS_WRAP4 , 0 },
//        { D3DRS_WRAP5 , 0 },
//        { D3DRS_WRAP6 , 0 },
//        { D3DRS_WRAP7 , 0 },
//        { D3DRS_CLIPPING , TRUE },
//        { D3DRS_LIGHTING , TRUE },
//        { D3DRS_AMBIENT , 0 },
//        { D3DRS_FOGVERTEXMODE , D3DFOG_NONE },
//        { D3DRS_COLORVERTEX , TRUE },
//        { D3DRS_LOCALVIEWER , TRUE },
//        { D3DRS_NORMALIZENORMALS , FALSE },
//        { D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_COLOR1 },
//        { D3DRS_SPECULARMATERIALSOURCE , D3DMCS_COLOR2 },
//        { D3DRS_AMBIENTMATERIALSOURCE , D3DMCS_MATERIAL },
//        { D3DRS_EMISSIVEMATERIALSOURCE , D3DMCS_MATERIAL },
//        { D3DRS_VERTEXBLEND , D3DVBF_DISABLE },
//        { D3DRS_CLIPPLANEENABLE , 0 },
//        { D3DRS_POINTSIZE , dwfOne },
//        { D3DRS_POINTSIZE_MIN , dwfOne },
//        { D3DRS_POINTSPRITEENABLE , FALSE },
//        { D3DRS_POINTSCALEENABLE , FALSE },
//        { D3DRS_POINTSCALE_A , dwfOne },
//        { D3DRS_POINTSCALE_B , dwfZero },
//        { D3DRS_POINTSCALE_C , dwfZero },
//        { D3DRS_MULTISAMPLEANTIALIAS , TRUE },
//        { D3DRS_MULTISAMPLEMASK , 0xffffffff },
//        { D3DRS_PATCHEDGESTYLE , D3DPATCHEDGE_DISCRETE },
//        { D3DRS_DEBUGMONITORTOKEN , D3DDMT_ENABLE },
//        { D3DRS_POINTSIZE_MAX , dwf64 },
//        { D3DRS_INDEXEDVERTEXBLENDENABLE , FALSE },
//        { D3DRS_COLORWRITEENABLE , 0x0000000F },
//        { D3DRS_TWEENFACTOR , dwfZero },
//        { D3DRS_BLENDOP  , D3DBLENDOP_ADD },
//        { D3DRS_POSITIONDEGREE , D3DDEGREE_CUBIC },
//        { D3DRS_NORMALDEGREE , D3DDEGREE_LINEAR },
//        { D3DRS_SCISSORTESTENABLE , FALSE },
//        { D3DRS_SLOPESCALEDEPTHBIAS , 0 },
//        { D3DRS_ANTIALIASEDLINEENABLE , FALSE },
//        { D3DRS_MINTESSELLATIONLEVEL , dwfOne },
//        { D3DRS_MAXTESSELLATIONLEVEL , dwfOne },
//        { D3DRS_ADAPTIVETESS_X , dwfZero },
//        { D3DRS_ADAPTIVETESS_Y , dwfZero },
//        { D3DRS_ADAPTIVETESS_Z , dwfOne },
//        { D3DRS_ADAPTIVETESS_W , dwfZero },
//        { D3DRS_ENABLEADAPTIVETESSELLATION , FALSE },
//        { D3DRS_TWOSIDEDSTENCILMODE , FALSE },
//        { D3DRS_CCW_STENCILFAIL , D3DSTENCILOP_KEEP },
//        { D3DRS_CCW_STENCILZFAIL , D3DSTENCILOP_KEEP },
//        { D3DRS_CCW_STENCILPASS , D3DSTENCILOP_KEEP },
//        { D3DRS_CCW_STENCILFUNC , D3DCMP_ALWAYS },
//        { D3DRS_COLORWRITEENABLE1 , 0x0000000f },
//        { D3DRS_COLORWRITEENABLE2 , 0x0000000f },
//        { D3DRS_COLORWRITEENABLE3 , 0x0000000f },
//        { D3DRS_BLENDFACTOR , 0xffffffff },
//        { D3DRS_SRGBWRITEENABLE , 0 },
//        { D3DRS_DEPTHBIAS , 0 },
//        { D3DRS_WRAP8 , 0 },
//        { D3DRS_WRAP9 , 0 },
//        { D3DRS_WRAP10 , 0 },
//        { D3DRS_WRAP11 , 0 },
//        { D3DRS_WRAP12 , 0 },
//        { D3DRS_WRAP13 , 0 },
//        { D3DRS_WRAP14 , 0 },
//        { D3DRS_WRAP15 , 0 },
//        { D3DRS_SEPARATEALPHABLENDENABLE , FALSE },
//        { D3DRS_SRCBLENDALPHA , D3DBLEND_ONE },
//        { D3DRS_DESTBLENDALPHA , D3DBLEND_ZERO },
//        { D3DRS_BLENDOPALPHA , TRUE },
//    };
//
//    static TextureStateValue  aeDTSS[] =
//    {
//        { D3DTSS_COLOROP , D3DTOP_MODULATE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE, D3DTOP_DISABLE },
//        { D3DTSS_COLORARG1 , D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE },
//        { D3DTSS_COLORARG2 , D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_ALPHAOP , D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1, D3DTOP_SELECTARG1 },
//        { D3DTSS_ALPHAARG1 , D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE },
//        { D3DTSS_ALPHAARG2 , D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_BUMPENVMAT00 , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVMAT01 , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVMAT10 , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVMAT11 , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_TEXCOORDINDEX , 0, 1, 2, 3, 4, 5, 6, 7 },
//        { D3DTSS_BUMPENVLSCALE , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_BUMPENVLOFFSET , dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero, dwfZero },
//        { D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE, D3DTTFF_DISABLE },
//        { D3DTSS_COLORARG0 , D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_ALPHAARG0 , D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_RESULTARG , D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT },
//        { D3DTSS_CONSTANT , 0, 0, 0, 0, 0, 0, 0, 0 },
//    };
//
//    typedef DWORD   SamplerStateValue[SAMPLER_STAGE_NUM + 1];
//
//    static SamplerStateValue aeDSSS[] = 
//    {
//        { D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DSAMP_ADDRESSW , D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP },
//        { D3DSAMP_BORDERCOLOR , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//        { D3DSAMP_MAGFILTER , D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT },
//        { D3DSAMP_MINFILTER , D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_POINT },
//        { D3DSAMP_MIPFILTER , D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE, D3DTEXF_NONE },
//        { D3DSAMP_MIPMAPLODBIAS , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//        { D3DSAMP_MAXMIPLEVEL , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//        { D3DSAMP_MAXANISOTROPY , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
//        { D3DSAMP_SRGBTEXTURE , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//        { D3DSAMP_ELEMENTINDEX , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//        { D3DSAMP_DMAPOFFSET , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//    };
//
//
//#endif // #if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )
//
//#endif // _XBOX
//
//    for ( i = 0; i < ARRAY_SIZE( aeDRS ); i++ )
//    {
//        //hr = g_pd3dDevice->GetRenderState( aeDRS[ i ], &ms_adwRenderState[ aeDRS[ i ] ] );
//        //ASSERT( SUCCEEDED( hr ) );
//        hr = g_pd3dDevice->SetRenderState( aeDRS[ i ].eType, aeDRS[ i ].dwValue );
//        ASSERT( SUCCEEDED( hr ) );
//        ms_adwRenderState[ aeDRS[ i ].eType ] = aeDRS[ i ].dwValue;
//    }//for
//
//    for ( i = 0; i < ARRAY_SIZE( aeDTSS ); i++ )
//    {
//        DWORD dwState = aeDTSS[ i ][ 0 ];
//
//        for( j = 0; j < TEXTURE_STAGE_NUM; j++ )
//        {
//            ms_adwTextureStageState[ j ][ dwState ] =  aeDTSS[ i ][ j + 1 ];
//            hr = g_pd3dDevice->SetTextureStageState( j, D3DTEXTURESTAGESTATETYPE( dwState )
//                , ms_adwTextureStageState[ j ][ dwState ] );
//            ASSERT( SUCCEEDED( hr ) );
//        }//for
//    }//for
//
//#ifdef SAMPLER_STAGE_NUM
//
//    for( i = 0; i < ARRAY_SIZE( aeDSSS ); i++ )
//    {
//        DWORD   dwState = aeDSSS[ i ][ 0 ];
//
//        for( j = 0; j < SAMPLER_STAGE_NUM; j++ )
//        {
//            ms_adwSamplerState[ j ][ dwState ] = aeDSSS[ i ][ j + 1 ];
//            hr = g_pd3dDevice->SetSamplerState( j, D3DSAMPLERSTATETYPE( dwState )
//                , ms_adwSamplerState[ j ][ dwState ] );
//            ASSERT( SUCCEEDED( hr ) );
//        }//for
//    }//for
//
//#endif // #ifdef SAMPLER_STAGE_NUM
//
//    //hr = g_pd3dDevice->GetTransform( D3DTS_VIEW, &ms_d3dTransformView );
//    //ASSERT( SUCCEEDED( hr ) );
//    //hr = g_pd3dDevice->GetTransform( D3DTS_PROJECTION, &ms_d3dTransformProjection );
//    //ASSERT( SUCCEEDED( hr ) );
//
//
//#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )
//
//    hr = g_pd3dDevice->GetDisplayMode( &ms_d3dDisplayMode );
//    ASSERT( SUCCEEDED( hr ) );
//    hr = g_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &ms_pd3dBackBuffer );
//    ASSERT( SUCCEEDED( hr ) );
//
//#elif ( DIRECT3D_VERSION >= 0x0900 ) 
//
//    hr = g_pd3dDevice->GetDisplayMode( 0, &ms_d3dDisplayMode );
//    ASSERT( SUCCEEDED( hr ) );
//    hr = g_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &ms_pd3dBackBuffer );
//    ASSERT( SUCCEEDED( hr ) );
//
//#endif // #if .. #elif
//
//    hr = g_pd3dDevice->GetDepthStencilSurface( &ms_pd3dBackBufferZStencil );
//    ASSERT( SUCCEEDED( hr ) );
//    hr = ms_pd3dBackBuffer->GetDesc( &ms_d3dBackBufferDesc );
//    ASSERT( SUCCEEDED( hr ) );
//    hr = ms_pd3dBackBufferZStencil->GetDesc( &ms_d3dBackBufferZStencilDesc );
//    ASSERT( SUCCEEDED( hr ) );
//
//    ms_pd3dRenderTarget = ms_pd3dBackBuffer;
//    ms_pd3dRenderTargetZStencil = ms_pd3dBackBufferZStencil;
//
//    ms_d3dBackBufferViewport.Width = ms_d3dBackBufferDesc.Width;
//    ms_d3dBackBufferViewport.Height = ms_d3dBackBufferDesc.Height;
//    ms_d3dBackBufferViewport.X = 0;
//    ms_d3dBackBufferViewport.Y = 0;
//    ms_d3dBackBufferViewport.MinZ = 0.f;
//    ms_d3dBackBufferViewport.MaxZ = 1.f;
//
//    hr = g_pd3dDevice->SetViewport( &ms_d3dBackBufferViewport );
//    ASSERT( SUCCEEDED( hr ) );
//    CopyMemory( &ms_d3dRenderTargetViewport, &ms_d3dBackBufferViewport, sizeof(D3DVIEWPORT) );
//
//    _SetPossibleColorKeyTextureFormat();
//    _SetPossibleAlphaTextureFormat();
//
//    ms_bRestored = TRUE;
//}//D3DDeviceState::Initialize()
//
//
//HRESULT KD3DDeviceState::CheckDeviceFormat( DWORD Usage, D3DRESOURCETYPE RType
//                                           , D3DFORMAT CheckFormat )
//{
//    ASSERT( g_pD3D != NULL );
//    return g_pD3D->CheckDeviceFormat( ms_uiAdapter, ms_d3dDeviceType
//        , ms_d3dBackBufferDesc.Format, Usage, RType, CheckFormat );
//}//KD3DDeviceState::CheckDeviceFormat()
//
//
//HRESULT KD3DDeviceState::CheckDepthStencilMatch( D3DFORMAT BackBufferFormat, D3DFORMAT DepthFormat )
//{
//    ASSERT( g_pD3D != NULL );
//    return g_pD3D->CheckDepthStencilMatch( ms_uiAdapter, ms_d3dDeviceType
//        , ms_d3dBackBufferDesc.Format, BackBufferFormat, DepthFormat );
//}//KD3DDeviceState::CheckDepthStencilMatch()
//
//
//void    KD3DDeviceState::ClearBackBuffer( D3DCOLOR d3dColor, float fZ, DWORD dwStencil )
//{
//    // Clear the backbuffer
//    if ( GetStencilDepth( ms_d3dBackBufferZStencilDesc.Format ) > 0  )
//    {
//        g_pd3dDevice->Clear( 0, NULL, 
//            D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
//            d3dColor & 0xffffff, fZ, dwStencil );
//    } 
//    else
//    {
//        g_pd3dDevice->Clear( 0, NULL, 
//            D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
//            d3dColor & 0xffffff, fZ, dwStencil );
//    }//if.. else..
//}//KD3DDeviceState::ClearBackBuffer()
//
//
//HRESULT KD3DDeviceState::_SetPossibleColorKeyTextureFormat()
//{
//    HRESULT hr;
//
//    ms_d3dColorKeyTextureFormat = D3DFMT_UNKNOWN;
//
//    // check 32bit 8888 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A8R8G8B8);
//    if (SUCCEEDED(hr))
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A8R8G8B8;
//        return S_OK;
//    }//if
//
//    // check 16bit 1555 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A1R5G5B5);
//    if (SUCCEEDED(hr))
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A1R5G5B5;
//        return S_OK;
//    }//if
//
//    // check 16bit 4444 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A4R4G4B4);
//    if (SUCCEEDED(hr))
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A4R4G4B4;
//        return S_OK;
//    }//if
//
//#if ( DIRECT3D_VERSION >= 0x0900 )
//
//    // check 32bit 2101010 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A2R10G10B10 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A2R10G10B10;
//        return  S_OK;
//    }//if
//
//#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
//
//    // check 32bit 2101010 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A2B10G10R10 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A2B10G10R10;
//        return  S_OK;
//    }//if
//
//    // check 16bit 8332 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A8R3G3B2 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dColorKeyTextureFormat = D3DFMT_A8R3G3B2;
//        return  S_OK;
//    }//if
//
//    // Ooops! There is no alpha texture format.
//    return E_FAIL;
//}//KD3DDeviceState::_SetPossibleColorKeyTextureFormat()
//
//
//HRESULT KD3DDeviceState::_SetPossibleAlphaTextureFormat()
//{
//    HRESULT hr;
//
//    ms_d3dAlphaTextureFormat = D3DFMT_UNKNOWN;
//
//    // check 32bit 8888 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A8R8G8B8);
//    if (SUCCEEDED(hr))
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A8R8G8B8;
//        return S_OK;
//    }//if
//
//    // check 16bit 4444 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A4R4G4B4);
//    if ( SUCCEEDED(hr) )
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A4R4G4B4;
//        return S_OK;
//    }//if
//
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A8R3G3B2 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A8R3G3B2;
//        return  S_OK;
//    }//if
//
//#if ( DIRECT3D_VERSION >= 0x0900 )
//
//    // check 32bit 2101010 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A2R10G10B10 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A2R10G10B10;
//        return  S_OK;
//    }//if
//
//#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
//
//
//    // check 32bit 2101010 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A2B10G10R10 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A2B10G10R10;
//        return  S_OK;
//    }//if
//
//    // check 16bit 1555 format
//    hr = KD3DDeviceState::CheckDeviceFormat( 0,
//        D3DRTYPE_TEXTURE,
//        D3DFMT_A1R5G5B5 );
//    if ( SUCCEEDED( hr ) )
//    {
//        ms_d3dAlphaTextureFormat = D3DFMT_A1R5G5B5;
//        return S_OK;
//    }//if
//
//    // Ooops! There is no alpha texture format.
//    return E_FAIL;
//}//KD3DDeviceState::_SetPossibleAlphaTextureFormat()
