#include "stdafx.h"
#include ".\QSFantasticMap.h"
//#include "../GCDeviceLib/KD3DDeviceState.h"
#include "KQSLayerManager.h"
#include "../../GCCollisionRect.h"

//extern GCDeviceShader* g_pRenderState;
//#include "../../GCDeviceManager2.h"
LPDIRECT3D9              g_pD3D;


#define ISHEAD(p)   ( (p->GetFootHoldType() & foothold_type::FHT_HEAD) > 0 )
#define ISFOOT(p)   ( (p->GetFootHoldType() & foothold_type::FHT_FOOT) > 0 )

#define MAP_ARRAY ((foothold_type**)m_ppFHWorld)

ImplementSingleton(KQSFantasticMap);

KQSFantasticMap::KQSFantasticMap(void) : KMap<KQSFootHold>()
{
    RegisterLuabind();

//#ifdef _DEBUG
//    m_pDebugFH = NULL;
//#endif
}

KQSFantasticMap::~KQSFantasticMap(void)
{
    UnLoad();
}

void KQSFantasticMap::RegisterLuabind()
{
    lua_tinker::class_add<KQSFantasticMap>( g_kLuabinder.GetLuaState(), "KQSFantasticMap" );
    lua_tinker::class_def<KQSFantasticMap>( g_kLuabinder.GetLuaState(), "Load", &KQSFantasticMap::Load);
    lua_tinker::class_def<KQSFantasticMap>( g_kLuabinder.GetLuaState(), "SetWorld", &KQSFantasticMap::SetWorld);
    lua_tinker::class_def<KQSFantasticMap>( g_kLuabinder.GetLuaState(), "AddDungeonFootHold", &KQSFantasticMap::AddDungeonFootHold);

    lua_tinker::decl( g_kLuabinder.GetLuaState(), "pMap", this );
}

void KQSFantasticMap::Load()
{
    UnLoad();

//#ifdef _DEBUG
//    m_pDebugFH = new KGC2DRenderer();
//#endif

    m_ppFHWorld = new foothold_type*[m_iFHWidth];
    for( int i = 0 ; i < m_iFHWidth; i++ )
    {
        m_ppFHWorld[i] = new foothold_type[m_iFHHeight];
    }
}

void KQSFantasticMap::UnLoad()
{
//#ifdef _DEBUG
//    SAFE_DELETE( m_pDebugFH );
//#endif

    if( m_ppFHWorld != NULL )
    {
        for( int i = 0 ; i < m_iFHWidth; i++ )
        {
            if( m_ppFHWorld[i] != NULL )
            {
                delete [] m_ppFHWorld[i];
                m_ppFHWorld[i] = NULL;
            }
        }
    }

    SAFE_DELETE_ARRAY( m_ppFHWorld );
}

void KQSFantasticMap::AddDungeonFootHold( KLuaManager& luaMgr )
{
    if( m_ppFHWorld == NULL )
        return;

    int PosX = 0;
    int PosY = 0;
    DWORD Prop = 0;
    std::string strValue;

    LUA_GET_VALUE_DEF( 1, PosX, 0 );
    LUA_GET_VALUE_DEF( 2, PosY, 0 );
    LUA_GET_VALUE_DEF( 3, Prop, foothold_type::FHP_JUST_FOOTHOLD );
    LUA_GET_VALUE_DEF( 4, strValue, "" );

    std::string strID;
    foothold_type* FHTemp = new foothold_type();

    FHTemp->SetFootHoldType( foothold_type::FHT_HEAD );
    FHTemp->SetFootHoldProp( Prop );
    FHTemp->SetPos( static_cast<float>(PosX*FOOTHOLDSIZE), static_cast<float>((PosY+1)*FOOTHOLDSIZE) );
    FHTemp->SetParent( NULL );
    FHTemp->SetValue( strValue );

    m_ppFHWorld[ FHTemp->GetIndexX() ][ FHTemp->GetIndexY() ] = *FHTemp;
    delete FHTemp;
}

void KQSFantasticMap::Render()
{
    SiKQSLayerManager()->NoAlphaRender();
//#if defined( _DEBUG ) || !defined( __PATH__ )
//    DebugRender();
//#endif

//#ifdef _DEBUG
//    if( m_pDebugFH != NULL )
//    {
//        SiKGCRenderer()->AddRenderObj( static_cast<IGCRenderObj*>(m_pDebugFH) );
//    }
//#endif
}

void KQSFantasticMap::FrameMove( float fElapsedTime )
{
    SiKQSLayerManager()->FrameMove( fElapsedTime );
}

void KQSFantasticMap::LoadBaseData( KLuaManager& luaMgr )
{
    LUA_BEGIN_TABLE("BASE_DATA", return )
    {
        int Width, Height;
        LUA_GET_VALUE("WORLD_WIDTH", Width, return );
        LUA_GET_VALUE("WORLD_HEIGHT", Height, return );

        SetWorld( Width, Height );

        int iLayerTot, iGamePlayLayer;
        LUA_GET_VALUE("LAYERTOTAL", iLayerTot, return );
        LUA_GET_VALUE("GAMEPLAYLAYER", iGamePlayLayer, return );

        SiKQSLayerManager()->SetMaxLayerNum( iLayerTot );
        SiKQSLayerManager()->SetGamePlayLayer( iGamePlayLayer );
        SiKQSLayerManager()->SetLayer();
    }
    LUA_END_TABLE( return );
}

void KQSFantasticMap::LoadFootHoldData( KLuaManager& luaMgr )
{
    KMap<KQSFootHold>::LoadFootHoldData( luaMgr );
}

void KQSFantasticMap::SetWorld( int iWidth, int iHeight )
{
    // Jaeho 2007.09.21
    // m_iFHWidth가 변경되면 UnLoad 함수 메모리 해지 할때 문제 생겨서 한번 해줬어요~
    UnLoad();

    m_iFHWidth = iWidth;
    m_iFHHeight = iHeight+1;

    m_fWidth = m_iFHWidth * FOOTHOLDSIZE;
    m_fHeight = m_iFHHeight * FOOTHOLDSIZE;

    Load();
}

void KQSFantasticMap::Draw_Billboard3( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2,
                                        D3DXVECTOR3& pos3, D3DXVECTOR3& pos4,
                                        BYTE Power, BYTE R, BYTE G, BYTE B )
{
    SLVERTEX v[4];

    g_pd3dDevice->SetFVF( FVF_LVERTEX );

    v[0].SetSLVER( pos1.x, pos1.y, pos1.z, 0, 1 );
    v[1].SetSLVER( pos2.x, pos2.y, pos2.z, 0, 0 );
    v[2].SetSLVER( pos3.x, pos3.y, pos3.z, 1, 1 );
    v[3].SetSLVER( pos4.x, pos4.y, pos4.z, 1, 0 );
    v[0].Diff = v[1].Diff = v[2].Diff = v[3].Diff = D3DCOLOR_ARGB( Power, R, G, B );

    g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(SLVERTEX) );
}


void KQSFantasticMap::DebugRender()
{
//    g_pd3dDevice->PushRenderState();
//    {

	  // 툴에서 랜더 스테이트를 변화 시킬일이나 있는지 모르겠다.
	  // Manager를 이상하게 사용해서 일단 아예 스테이트 푸쉬 팝 부분 날려버림
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        static GCDeviceTexture* pNullTexture = g_pGCDeviceManager->CreateTexture("임시로 사용하는 널 텍스쳐");

        //g_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matCamera );
        pNullTexture->SetDeviceTexture();

        for( int i = 0; i < m_iFHWidth; ++i )
        {
            for( int j = 0; j < m_iFHHeight; ++j )
            {
                if( m_ppFHWorld[i][j].GetFootHoldType() != foothold_type::FHT_INIT )
                {
                    float fX = m_ppFHWorld[i][j].GetX();
                    float fY = m_ppFHWorld[i][j].GetY();

                    GCCollisionRect<float> fRect;
                    fRect.m_Left    = (fX) * 400.0f;
                    fRect.m_Top     = (fY) * 400.0f;
                    fRect.m_Right   = (fX + FOOTHOLDSIZE) * 400.0f;
                    fRect.m_Bottom  = (fY - FOOTHOLDSIZE) * 400.0f;

                    D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                    SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top, 0.4f );
                    SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top, 0.4f );
                    SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom, 0.4f );
                    SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom, 0.4f );

                    Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 180, 180, 180 ); // 회색쯤?

                    if( m_ppFHWorld[i][j].GetFootHoldProp() & foothold_type::FHP_NO_JUMP_DOWN )
                        Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 0, 0, 255 ); // 파랑
                    if( m_ppFHWorld[i][j].GetFootHoldProp() & foothold_type::FHP_NO_COME_RIGHT )
                        Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 255, 0, 0 ); // 빨강
                    if( m_ppFHWorld[i][j].GetFootHoldProp() & foothold_type::FHP_NO_COME_LEFT )
                        Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 200, 0, 255, 0 ); // 초록
                }
            }
        }
    D3DXMATRIX matWorld;	
	::D3DXMatrixIdentity( &matWorld );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    //g_pd3dDevice->PopRenderState();
}

void KQSFantasticMap::AddDungeonFootHoldByValue( float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue )
{

}

void KQSFantasticMap::RemoveDungeonFootHold( float PosX, float PosY, void* pParent )
{

}