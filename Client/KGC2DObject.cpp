#include "StdAfx.h"
#include ".\kgc2dobject.h"


#include "GCCameraOrtho.h"

#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"
#include "Stage.h"

GCDeviceVertexBuffer*	KGC2DObject::m_pVertex = NULL;

KGC2DObject::KGC2DObject(void)
: m_iCurrTexIndex( 0 )
, m_cAlpha( 0xff )
, m_cRed( 0xff )
, m_cGreen( 0xff )
, m_cBlue( 0xff )
, m_fXrotation ( 0.0f)
, m_fYrotation ( 0.0f)
, m_fZrotation ( 0.0f)
, m_fXscale ( 1.0f)
, m_fYscale ( 1.0f)
, m_fXtrans ( 0.0f)
, m_fYtrans ( 0.0f)
, m_fXrotationSpd ( 0.0f)
, m_fYrotationSpd ( 0.0f)
, m_fZrotationSpd ( 0.0f)
, m_fXspeed ( 0.0f)
, m_fYspeed ( 0.0f)
, m_fXgravity ( 0.0f)
, m_fYgravity ( 0.0f)
, m_fXaxisOffset( 0.0f )
, m_fYaxisOffset( 0.0f )
, m_fZaxisOffset( 0.0f )
, m_fFrictionalForceX( 1.0f )
, m_fFrictionalForceY( 1.0f )
, m_iDamageType( -1 )
, m_bHit( false )
, m_bCrashed( false )
, m_bHitCheck( true )
, m_bPushCheck( false )
, m_bTeleport( false )
, m_bPlayerPushed( false )
, m_bMoveWithCamera( false )
, m_bBounceCheckWithMap( false )
, m_bDynamicFrameMove( true )
, m_fObjOffsetX( 0.0f )
, m_bAgitShopObject( false )
, m_iMoneyType( AGIT_OBJECT_MONEY_DANDELION )
, m_iPrice( 0 )
, m_iNowCount( 0 )
, m_iMaxCount( 0 )
, m_dxvPricePos( 0.0f, 0.0f )
, m_pkTexPriceback( NULL )
, m_pkTexPriceIcon( NULL )
, m_dxvIconUV( 0.0f, 0.0f, 0.0f, 0.0f )
, m_dxvBackUV( 0.0f, 0.0f, 0.0f, 0.0f )
, m_pParticle( NULL )
, m_dxvParticlePos( 0.0f, 0.0f )
, m_fTexPosX(0.0f)
, m_fTexPosY(0.0f)
, m_fTexSizeX(1.0f)
, m_fTexSizeY(1.0f)
{
    m_vPos.x = 0.0f;
    m_vPos.y = 0.0f;

    //#if defined(_DEBUG)
    //	g_pGCDeviceManager2->Add2DObject( this );
    //#endif
}

KGC2DObject::~KGC2DObject(void)
{
    //#if defined(_DEBUG)
    //	g_pGCDeviceManager2->Remove2DObject( this );
    //#endif
    RemoveAllTexture();

    SAFE_RELEASE( m_pkTexPriceback );
    SAFE_RELEASE( m_pkTexPriceIcon );
}

void KGC2DObject::FrameMove_Origin()
{
    FrameMove();
    FrameMove_Dynamics();
}

void KGC2DObject::FrameMove()
{
}

void KGC2DObject::FrameMove_Dynamics()
{
    if( IsPhysics() == false )
        return;

    m_bCrashed = false;

    // Position에 Speed 적용
    D3DXVECTOR2 dxvBeforePos = m_vPos;
    if( m_fXspeed != 0.0f )
        m_vPos.x += m_fXspeed;

    if( m_fYspeed != 0.0f )
        m_vPos.y += m_fYspeed;

    // 발판과의 충돌 설정
    if( true == m_bBounceCheckWithMap )
        SetCrashPos();

    do 
    {
        // 충돌이 있었다면 마찰력 적용
        if( m_bCrashed == true )
        {
            // 우리게임 특성상 위로 올라가면서 튕길 일은 없다.
            if( m_fYspeed < 0.0f )
                m_fYspeed = ( m_fYspeed * ( 1.0f - m_fFrictionalForceY ) ) * (-1.0f);

            if( ASMfabs( m_fYspeed ) < ASMfabs( m_fYgravity ) )
                m_fYspeed = 0.0f;

            if( m_fYspeed == 0.0f )
            {
                m_fXspeed = ( m_fXspeed * ( 1.0f - m_fFrictionalForceX ) );
            }
        }
        else
        {
            // Speed에 Gravity 적용
            if( m_fXgravity != 0.0f )
                m_fXspeed += m_fXgravity;

            if( m_fYgravity != 0.0f )
                m_fYspeed += m_fYgravity;
        }

    } while( false );
}

void KGC2DObject::Render()
{
    D3DXMATRIX    mat, mat1, mat2, mat3;

    g_pGCDeviceManager2->Flush();
    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

    if( m_vecTex.empty() )
        return;

    if( m_iCurrTexIndex < 0 || m_iCurrTexIndex >= (int)m_vecTex.size() )
    {
        ASSERT( !"Texture Index is wrong!!!" );
        return;
    }

    if( m_vecTex[m_iCurrTexIndex] == NULL )
    {
        ASSERT( !"Texture is NULL!!" );
        return;
    }
    if( m_vecTex[m_iCurrTexIndex] == NULL )
    {
        ASSERT( !"Wrong Texture in vector!!!" );
        return;
    }
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

        SetItemVertex( m_cAlpha );

        g_pd3dDevice->SetFVF( FVF_LVERTEX );
        g_pd3dDevice->SetStreamSource( 0, m_pVertex->GetBuffer(), 0, sizeof(SLVERTEX) );

        D3DXMatrixIdentity( &mat1 );
        D3DXMatrixTranslation( &mat2, m_fXaxisOffset, m_fYaxisOffset, m_fZaxisOffset );    
        D3DXMatrixMultiply( &mat1, &mat1, &mat2 );
        D3DXMatrixScaling( &mat3, m_fXscale, m_fYscale, 1.0f );
        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
D3DXMatrixRotationYawPitchRoll( &mat3, m_fYrotation, m_fXrotation, m_fZrotation );
        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
        D3DXMatrixTranslation( &mat2, m_vPos.x - 1.0f + m_fXtrans, m_vPos.y - 0.507f + m_fYtrans , 0.35f );    
        D3DXMatrixMultiply( &mat1, &mat1, &mat2 );
        if( m_bMoveWithCamera == false )
            D3DXMatrixMultiply( &mat1, &mat1, &g_kCamera.m_matCamera );

        g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
        m_vecTex[m_iCurrTexIndex]->SetDeviceTexture();
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

        D3DXMatrixIdentity( &mat );
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
    }
    g_pGCDeviceManager2->PopState();
}

void KGC2DObject::SetItemVertex( BYTE Alpha )
{
    SLVERTEX *v = NULL;
    if( m_pVertex == NULL )
    {
        m_pVertex = g_pGCDeviceManager2->CreateVertexBuffer( "KGC2DObj_m_pVertex", 4*sizeof(SLVERTEX), D3DUSAGE_WRITEONLY,FVF_LVERTEX,D3DPOOL_DEFAULT, (void*)v );
    }

    v = (SLVERTEX*)m_pVertex->Lock( (BYTE*)v );
    v[0].SetSLVER( 0,		1.0f,	0,		m_fTexPosX,	m_fTexPosY,		D3DCOLOR_ARGB( Alpha, m_cRed, m_cGreen, m_cBlue ) );
    v[1].SetSLVER( 1.0f,    1.0f,	0,		m_fTexPosX+m_fTexSizeX,	m_fTexPosY,		D3DCOLOR_ARGB( Alpha, m_cRed, m_cGreen, m_cBlue ) );
    v[2].SetSLVER( 0,		0,		0,		m_fTexPosX,	m_fTexPosY+m_fTexSizeY,	D3DCOLOR_ARGB( Alpha, m_cRed, m_cGreen, m_cBlue ) );
    v[3].SetSLVER( 1.0f,	0,		0,		m_fTexPosX+m_fTexSizeX,	m_fTexPosY+m_fTexSizeY,	D3DCOLOR_ARGB( Alpha, m_cRed, m_cGreen, m_cBlue ) );
    m_pVertex->Unlock();
}

void KGC2DObject::SetAlpha( int iAlpha )
{
    m_cAlpha = iAlpha;
}

int KGC2DObject::AddTexture( const char* strID )
{
    if( strID == NULL)
        return -1;

    GCDeviceTexture* pTexture = g_pGCDeviceManager2->CreateTexture( strID );
    return AddTexture( pTexture );
}

int KGC2DObject::_AddTexture( const char* strID )
{
    return AddTexture( strID );
}

int KGC2DObject::AddTexture( GCDeviceTexture* pTexture )
{
    if( pTexture != NULL )
    {
        m_vecTex.push_back( pTexture );
        return (int)m_vecTex.size() - 1;
    }

    return -1;
}

void KGC2DObject::SetCurrTexIndex( int iIndex )
{
    if( m_vecTex.empty() )
        return;

    if( iIndex < 0 || iIndex >= (int) m_vecTex.size() )
        return;

    m_iCurrTexIndex = iIndex;
}

D3DXVECTOR2* KGC2DObject::GetPosition()
{
    return &m_vPos;
}

void KGC2DObject::SetFrictionalForceX( float fForce )
{
    if( fForce > 1.0f )
        m_fFrictionalForceX = 1.0f;

    else if( fForce < 0.0f )
        m_fFrictionalForceX = 0.0f;

    else
        m_fFrictionalForceX = fForce;
}

void KGC2DObject::SetFrictionalForceY( float fForce )
{
    if( fForce > 1.0f )
        m_fFrictionalForceY = 1.0f;

    else if( fForce < 0.0f )
        m_fFrictionalForceY = 0.0f;

    else
        m_fFrictionalForceY = fForce;
}

void KGC2DObject::BindLua()
{
    lua_tinker::class_add<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "KGC2DObject" );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "iCurrTexIndex", 		&KGC2DObject::m_iCurrTexIndex );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "cAlpha", 				&KGC2DObject::m_cAlpha );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Red", 					&KGC2DObject::m_cRed );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Green", 				&KGC2DObject::m_cGreen );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Blue", 					&KGC2DObject::m_cBlue);
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Yrotation", 			&KGC2DObject::m_fYrotation );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xrotation", 			&KGC2DObject::m_fXrotation );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Zrotation", 			&KGC2DObject::m_fZrotation );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xaxis", 				&KGC2DObject::m_fXaxisOffset );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Yaxis", 				&KGC2DObject::m_fYaxisOffset );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Zaxis", 				&KGC2DObject::m_fZaxisOffset );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xscale", 				&KGC2DObject::m_fXscale );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Yscale", 				&KGC2DObject::m_fYscale );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xtrans", 				&KGC2DObject::m_fXtrans );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Ytrans", 				&KGC2DObject::m_fYtrans );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xspeed", 				&KGC2DObject::m_fXspeed );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Yspeed", 				&KGC2DObject::m_fYspeed );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Xgravity", 				&KGC2DObject::m_fXgravity );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Ygravity", 				&KGC2DObject::m_fYgravity );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "FrictionalForceX", 		&KGC2DObject::m_fFrictionalForceX );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "FrictionalForceY", 		&KGC2DObject::m_fFrictionalForceY );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "BounceCheckWithMap", 	&KGC2DObject::m_bBounceCheckWithMap  );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Hit", 					&KGC2DObject::m_bHit );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "HitCheck", 				&KGC2DObject::m_bHitCheck );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "PushCheck", 			&KGC2DObject::m_bPushCheck );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Crashed", 				&KGC2DObject::m_bCrashed );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "PlayerPushed", 			&KGC2DObject::m_bPlayerPushed );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "Teleport", 				&KGC2DObject::m_bTeleport );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "DynamicFrameMove", 		&KGC2DObject::m_bDynamicFrameMove );
    lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "DamageType", 			&KGC2DObject::m_iDamageType );

	lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "TexPosX", 	&KGC2DObject::m_fTexPosX );
	lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "TexPosY", 	&KGC2DObject::m_fTexPosY );
	lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "TexSizeX", 	&KGC2DObject::m_fTexSizeX );
	lua_tinker::class_mem<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "TexSizeY", 	&KGC2DObject::m_fTexSizeY );



    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "SetRotation", 			&KGC2DObject::SetRotation );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "SetPosition", 			&KGC2DObject::SetPosition );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "SetPositionPixel", 		&KGC2DObject::SetPositionPixel );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "SetSize", 				&KGC2DObject::SetSize );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "CameraMatrixOn", 		&KGC2DObject::CameraMatrixOn  );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "CameraMatrixOff", 		&KGC2DObject::CameraMatrixOff  );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "SetAlpha", 				&KGC2DObject::SetAlpha  );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "GetPosition", 			&KGC2DObject::GetPosition  );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "RemoveTexture", 		&KGC2DObject::RemoveTexture );
    lua_tinker::class_def<KGC2DObject>( KGCLuabinder::getInstance().GetLuaState(), "AddTexture", 			&KGC2DObject::_AddTexture );;
}

int KGC2DObject::RemoveTexture( int iIndex )
{
    if( m_vecTex.empty() )
        return 0;

    if( iIndex < 0 )
        iIndex = 0;

    if( iIndex > (int)m_vecTex.size() - 1 )
        iIndex = m_vecTex.size() - 1;

    SAFE_RELEASE( m_vecTex[iIndex] );
    m_vecTex.erase( m_vecTex.begin() + iIndex );
    return 1;
}

bool KGC2DObject::Damaged( GCCollisionRect<float>& weapon, float fAttackPoint, int iDamageType, int iAttackerIndex, bool bMonsterDamage )
{
    if( m_bHitCheck == false )
        return false;

    //m_bHit = false;
    m_iDamageType = iDamageType;

    GCCollisionRect<float> rtCollision = GetCollisionRect();
    if( m_kCollisionRect.CheckCollision( weapon, &rtCollision ) == true )
    {
        m_bHit = true;
        return true;
    }
    return false;
}

void KGC2DObject::PushCheck( float& fx, float fy, bool bTeleport )
{
    if( m_bPushCheck == false )
        return;

    if( m_bTeleport && bTeleport )
        return;

    m_bPlayerPushed = false;

    if( (m_vPos.y + m_fYscale * 0.9 > fy)
        && (m_vPos.y < fy )
        )
    {
        float fObjectLeft = m_vPos.x;
        float fObjectRight = m_vPos.x + m_fXscale;
        if( (fObjectRight > fx)
            && (fObjectLeft < fx) )
        {
            if( (fObjectLeft + fObjectRight)/2.0f > fx )
                fx = fObjectLeft;
            else
                fx = fObjectRight;

            m_bPlayerPushed = true;
        }
    }
}

GCCollisionRect<float> KGC2DObject::GetCollisionRect()
{
    m_kCollisionRect.SetRect( m_vPos.x - 1.0f, m_vPos.y - 0.25f, m_vPos.x + m_fXscale - 1.0f, m_vPos.y + m_fYscale * 0.95f - 0.25f );
    return m_kCollisionRect;
}

int KGC2DObject::RemoveAllTexture()
{
    int i = 0;
    for( i = 0 ; i < (int)m_vecTex.size() ; ++i )
    {
        SAFE_RELEASE( m_vecTex[i] );
    }
    m_vecTex.clear();

    return 1;
}

void KGC2DObject::SetRGB( BYTE R, BYTE G, BYTE B )
{
    m_cRed = R;
    m_cGreen = G;
    m_cBlue = B;
}

void KGC2DObject::SetPositionPixel( float fx, float fy )
{
    SetPosition( fx / (400.f*GC_SCREEN_DIV_WIDTH), fy / (400.f*GC_SCREEN_DIV_HEIGHT) );
}

void KGC2DObject::SetPosition( float fx, float fy )
{
   m_vPos.x = fx;
   m_vPos.y = fy;
}

void KGC2DObject::SetCrashPos( void )
{
    D3DXVECTOR2 vCrashPosLeft;

    if( SiKGCRoomManager()->IsMonsterGameMode() == true )
        SiKGCFantasticMap()->CrashCheck( m_vPos.x, m_vPos.y, vCrashPosLeft );
    else
    {
        D3DXVECTOR3 vCrashPosVector3L = g_MyD3D->MyStg->Check_Contact( m_vPos.x, m_vPos.y, 0 );

        vCrashPosLeft.x = vCrashPosVector3L.x;
        vCrashPosLeft.y = vCrashPosVector3L.y;
    }

    if( vCrashPosLeft.y != -1.0f && m_fYspeed < 0.0f )
    {
        m_bCrashed = true;
        m_vPos.y = vCrashPosLeft.y;
    }
}

GCDeviceTexture* KGC2DObject::GetCurrTex( void )
{
    if( m_vecTex.empty() )
        return NULL;

    return m_vecTex[m_iCurrTexIndex];
}

void KGC2DObject::GetRGB( BYTE& R, BYTE& G, BYTE& B )
{
    R = m_cRed;
    G = m_cGreen;
    B = m_cBlue;
}