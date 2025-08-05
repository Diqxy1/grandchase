#include "stdafx.h"
#include "KGCLayer.h"
#include "KGCLayerManager.h"
//

#include "GCCameraOrtho.h"
#include "../EveryTriggerManager.h"
//


//

KGCLayer::KGCLayer( KGCLayerManager* pManager )
: KLayer<KGCLayerPart, KGCLayerManager>( pManager )
, m_bStaticCamera( false )
{
}

KGCLayer::~KGCLayer(void)
{
}

void KGCLayer::SetColor( DWORD dwColorNormal, DWORD dwColorSky )
{
    DWORD dwColor = 0xffffffff;
    if( m_iLayerNum == LAST_LAYER )
        dwColor = dwColorSky;
    else
        dwColor = dwColorNormal;

    std::for_each( m_vecPart.begin(), m_vecPart.end(),
        boost::bind( &KGCLayerPart::SetColor, _1, dwColor ) );
}

//void KGCLayer::SetColor( D3DXCOLOR& Color )
//{
//    for( int i = 0; i < (int)m_vecPart.size(); ++i )
//    {
//        m_vecPart[i]->SetColor( Color );
//    }
//}

bool KGCLayer::IsAlpha()
{
    return m_iLayerNum <= m_pLayerManager->GetPlayerLayer() ? true : false;
}

void KGCLayer::FrameMove( float fElapsedTime )
{
    //PROFILE2( "KGCLayer::FrameMove" );

    if( m_vecPart.empty() )
        return;

    Process();
    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    {
        m_vecPart[i]->FrameMove( fElapsedTime );
    }
    //KLayer<KGCLayerPart, KGCLayerManager>::SetBufferType1();
}

//KGCCamera* KGCLayer::GetCurCamera()
//{
//    return &g_kCamera[CameraPlay];
//}

void KGCLayer::Process()
{
    if( m_vecPart.empty() )
        return;

    // RemainDelay가 남아있으면 레이어 로딩이 되지 않은 상태이다.
    // m_vecPart는 게임시작 할때 초기화하기때문에 이전 던전의 파츠정보가 그대로 남아있고
    // 로딩이 된것처럼 거짓말을한다.
    if( 0 < g_kEveryTriggerManager.GetRemainNextDelay() )
        return;

    if( m_iLayerNum == LAST_LAYER )
    {
        for( int i = 0; i < (int)m_vecPart.size(); ++i )
        {
            KGCLayerPart* pPart = static_cast< KGCLayerPart* >( m_vecPart[i] );
            pPart->ProcessRepeat( m_fLayerSizeX, m_fLayerSizeY );
        }
        return;
    }

    //D3DXVECTOR3 CameraPos( 0.0f, 0.0f, 0.0f );
    //D3DXVec3TransformCoord( &CameraPos, &CameraPos, &g_kCamera.m_matCamera );
    D3DXVECTOR3& CameraPos = D3DXVECTOR3( g_kCamera.m_fLookX + 1.0f, g_kCamera.m_fLookY + 0.75f, 0.0f );

    // 카메라가 볼수 있는 영역 알아내자. 줌이 적용되어 있다.
    float fWidth = g_kCamera.m_rtCameraBasic.right - g_kCamera.m_rtCameraBasic.left;//2.0f;//800.0f;
    float fHeight = g_kCamera.m_rtCameraBasic.top - g_kCamera.m_rtCameraBasic.bottom;//1.5f;//600.0f;

    // 맵 전체의 크기를 알아낸다
    float PlayingWidth = m_pLayerManager->GetMapPtr()->GetPixelWidth();
    float PlayingHeight = m_pLayerManager->GetMapPtr()->GetPixelHeight();

    float ScrollX = 0.0f;
    float ScrollY = 0.0f;

    // cw = 카메라가 카메라 영역에서 얼마나 움직였는가
    float cwx = CameraPos.x - fWidth * 0.5f;
    float cwy = CameraPos.y - fHeight * 0.5f;

    // 스크롤될 값 = 기준 레이어의 크기 - 화면크기 / 전체크기 - 화면크기 * cw
    if( PlayingWidth > fWidth )
        ScrollX = ( (m_fLayerSizeX - fWidth) / (PlayingWidth - fWidth) ) * cwx;
    else
        ScrollX = (m_fLayerSizeX - fWidth);

    if( PlayingHeight > fHeight )
        ScrollY = ( (m_fLayerSizeY - fHeight) / (PlayingHeight - fHeight) ) * cwy;
    else
        ScrollY = (m_fLayerSizeY - fHeight);

    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    {
        KGCLayerPart* pPart = static_cast< KGCLayerPart* >( m_vecPart[i] );

        // 스케일값 설정 & Scroll 될 값 설정
        if( m_fLayerSizeX <= PlayingWidth && m_fLayerSizeY <= PlayingHeight )
        {
			int bScrollX = static_cast<int>( pPart->GetScroll_X() ? 1.0f : 0.0f );
			int bScrollY = static_cast<int>( pPart->GetScroll_Y() ? 1.0f : 0.0f );
			float fScrollX, fScrollY;

            // 원래 스크롤 되어야 하는 값에 카메라영역을 적용시켜줘야함 ( 레이어를 통채로 이동하는게 아니고 파트들이 이동하기 때문 )
			// LAST_LAYER는 배경 그 앞은 원경들이다~ 
			// 배경과 가까울 수록 스크롤이 적게 된다.
			if ( pPart->GetLayer() == LAST_LAYER - 1 )
			{
				fScrollX = -ScrollX * 0.1f * bScrollX + cwx;
				fScrollY = -ScrollY * 0.3f * bScrollY + cwy;
			}
			else if ( pPart->GetLayer() == LAST_LAYER - 2 )
			{
				fScrollX = -ScrollX * 0.3f * bScrollX + cwx;
				fScrollY = -ScrollY * 0.45f * bScrollY + cwy;
			}
			else if ( pPart->GetLayer() == LAST_LAYER - 3 )
			{
				fScrollX = -ScrollX * 0.65f * bScrollX + cwx;
				fScrollY = -ScrollY * 0.8f * bScrollY + cwy;
			}
			else
			{
				fScrollX = -ScrollX + cwx;
				fScrollY = -ScrollY + cwy;
			}

			pPart->SetScrollPos( fScrollX, fScrollY );

			if ( fScrollX != 0.0f || fScrollY != 0.0f )
			{
				std::vector< CParticleEventSeqPTR >::iterator it;
				for ( it = pPart->m_vecParticleSeq.begin(); it != pPart->m_vecParticleSeq.end(); it++ )
				{
					KLayerRect<float> kRect;
					pPart->GetLayerRect( kRect );
					float fParticlePos_X = kRect.m_X + pPart->GetParticleData(i).fOffsetX + fScrollX;
					float fParticlePos_Y = kRect.m_Y + pPart->GetParticleData(i).fOffsetY + fScrollY;
					(*it)->SetPosition( fParticlePos_X, fParticlePos_Y, 1.0f );
				}
			}
        }
        pPart->ProcessRepeat( m_fLayerSizeX, m_fLayerSizeY );

        //if( m_iLayerNum < SiKGCLayerManager()->GetPlayerLayer() )
        //{
        //    // PlayerBack 설정
        //    SetPlayerBack( pPart, g_kCamera.m_iTargetPlayer );
        //}
    }
}

//void KGCLayer::SetPlayerBack( KGCLayerPart* pPart, int iPlayerIndex )
//{
//    // 나도 내가 왜 이렇게 했는지 모르겠다 -_-;
//    KLayerRect<float> kRect;
//    pPart->GetLayerRect( kRect );
//
//    float fScrollX;
//    float fScrollY;
//    pPart->GetScrollPos( fScrollX, fScrollY );
//
//    PLAYER* pPlayer = &g_MyD3D->MyPlayer[iPlayerIndex];
//    if( pPlayer != NULL )
//    {
//        // 영역안에 플레이어가 있는가
//        if( pPlayer->vPos.x - 1.0f + fScrollX > kRect.m_X && pPlayer->vPos.x - 1.0f < kRect.m_X + kRect.m_Width + fScrollX )
//        {
//            if( pPlayer->vPos.y - 0.75f + fScrollY > kRect.m_Y && pPlayer->vPos.y - 0.75f < kRect.m_Y + kRect.m_Height + fScrollY )
//            {
//                pPart->SetPlayerBack( true );
//            }
//            else
//                pPart->SetPlayerBack( false );
//        }
//        else
//            pPart->SetPlayerBack( false );
//    }
//}

HRESULT KGCLayer::Render(D3DXMATRIX * mat)
{   
	std::vector< CRenderPartPair<KGCLayerPart> >::iterator vitRenderPart;
	std::vector< KGCLayerPart* >::reverse_iterator vitPart;
	for( vitRenderPart = m_vecRenderPart.begin(); vitRenderPart != m_vecRenderPart.end(); ++vitRenderPart )
	{
		std::vector< KGCLayerPart* >& vecRenderPart = (*vitRenderPart).second;
		if( vecRenderPart.empty() )
			continue;

		vitPart = vecRenderPart.rbegin();
		GCDeviceTexture* pTexture = (*vitPart)->GetTexture();
		ASSERT( pTexture != NULL );

		for( ; vitPart != vecRenderPart.rend(); ++vitPart )
		{
			KGCLayerPart* pPart = (*vitPart);
            D3DXMATRIX matWorld = *mat;

			if( pPart->GetShow() == true && pPart->CheckCulling() == true )
			{
				const KSafeArray<SLVERTEX,4>& pScrolledVertex = pPart->GetRenderVertex();

                if( false == m_bStaticCamera )
                {
                    D3DXMATRIX matCamera = g_kCamera.m_matCamera;
                    matCamera._41 += (g_kCamera.m_matCamera._41 * pPart->GetCMRadio_X());
                    matCamera._42 += (g_kCamera.m_matCamera._42 * pPart->GetCMRadio_Y());
                    D3DXMatrixMultiply( &matWorld, &matWorld, &matCamera );
                }

				pPart->Render(&matWorld);

				if( pPart->GetMoveable() == true )
				{
					if( pPart->GetRepeat_X() == true )
					{
						if( pScrolledVertex[1].Pos.x > m_fLayerSizeX )
						{
							KGCLayerPart kPart;
							kPart.CopyMember( pPart );
							KLayerRect<float> kRect;
							kPart.GetLayerRect( kRect );
							kRect.m_X = pScrolledVertex[0].Pos.x - m_fLayerSizeX;
							kPart.SetLayer( pPart->GetLayer() );
							kPart.SetLayerPos( kRect );
							kPart.SetZoffset( pPart->GetZoffset() );
							kPart.Flush();	
							kPart.Render(&matWorld);
						}
						else if( pScrolledVertex[0].Pos.x < 0.0f )
						{
							KGCLayerPart kPart;
							kPart.CopyMember( pPart );
							KLayerRect<float> kRect;
							kPart.GetLayerRect( kRect );
							kRect.m_X = m_fLayerSizeX + pScrolledVertex[0].Pos.x;
							kPart.SetLayer( pPart->GetLayer() );
							kPart.SetLayerPos( kRect );
							kPart.SetZoffset( pPart->GetZoffset() );
							kPart.Flush();	
							kPart.Render(&matWorld);
						}
					}

					if( pPart->GetRepeat_Y() == true )
					{
						if( pScrolledVertex[0].Pos.y > m_fLayerSizeY )
						{
							KGCLayerPart kPart;
							kPart.CopyMember( pPart );
							KLayerRect<float> kRect;
							kPart.GetLayerRect( kRect );
							kRect.m_Y = -kRect.m_Height + (pScrolledVertex[0].Pos.y - m_fLayerSizeY);
							kPart.SetLayer( pPart->GetLayer() );
							kPart.SetLayerPos( kRect );
							kPart.SetZoffset( pPart->GetZoffset() );
							kPart.Flush();
							kPart.Render(&matWorld);
						}
						else if( pScrolledVertex[2].Pos.y < 0.0f )
						{
							KGCLayerPart kPart;
							kPart.CopyMember( pPart );
							KLayerRect<float> kRect;
							kPart.GetLayerRect( kRect );
							kRect.m_Y = m_fLayerSizeY + pScrolledVertex[2].Pos.y;
							kPart.SetLayer( pPart->GetLayer() );
							kPart.SetLayerPos( kRect );
							kPart.SetZoffset( pPart->GetZoffset() );
							kPart.Flush();
							kPart.Render(&matWorld);
						}
					}
				}

			}
		}
	}
	return S_OK;
	//return KLayer<KGCLayerPart, KGCLayerManager>::Render();	
}

void KGCLayer::AddParts( KLuaManager& luaMgr )
{
    std::string     PartName;
    LUA_GET_VALUE_DEF( "PARTNAME", PartName, "" );
    KGCLayerPart* TempPart = m_pLayerManager->CreateClonePart( PartName );

    if( TempPart == NULL )
    {
        AddPartsErrFunc( PartName );
        return;
    }

    KLayerRect<int> kTexRect;
    TempPart->GetTexRect( kTexRect );
    LUA_GET_VALUE_DEF( "TEX_X", kTexRect.m_X, kTexRect.m_X );
    LUA_GET_VALUE_DEF( "TEX_Y", kTexRect.m_Y, kTexRect.m_Y );
    LUA_GET_VALUE_DEF( "TEX_WIDTH", kTexRect.m_Width, kTexRect.m_Width );
    LUA_GET_VALUE_DEF( "TEX_HEIGHT", kTexRect.m_Height, kTexRect.m_Height );
    TempPart->SetTextureRedirect( kTexRect );

    KLayerRect<float> kLayerRect;
    TempPart->GetLayerRect( kLayerRect );
    LUA_GET_VALUE_DEF( "LAYER_X", kLayerRect.m_X, kLayerRect.m_X );
    LUA_GET_VALUE_DEF( "LAYER_Y", kLayerRect.m_Y, kLayerRect.m_Y );

    //// [8/25/2007 breadceo] 좌표계
    //kLayerRect.m_X -= 1.0f;
    //if( m_iLayerNum == LAST_LAYER )
    //    kLayerRect.m_Y -= 0.75f;
    //else
    //    kLayerRect.m_Y -= 0.5f;

    TempPart->SetLayerPos( kLayerRect );
    TempPart->SetLayer( m_iLayerNum );
    TempPart->SetVertex();

    bool Moveable;
    float  MoveSpeed_X;
    float  MoveSpeed_Y;
    LUA_GET_VALUE_DEF( "MOVEABLE", Moveable, false );
    LUA_GET_VALUE_DEF( "MOVESPEED_X", MoveSpeed_X, 0 );
    LUA_GET_VALUE_DEF( "MOVESPEED_Y", MoveSpeed_Y, 0 );
    TempPart->SetMoveProp( Moveable, MoveSpeed_X, MoveSpeed_Y );

	bool bScroll_X;
	bool bScroll_Y;
	LUA_GET_VALUE_DEF( "SCROLL_X", bScroll_X, false );
	LUA_GET_VALUE_DEF( "SCROLL_Y", bScroll_Y, false );
	TempPart->SetScroll_X(bScroll_X);
	TempPart->SetScroll_Y(bScroll_Y);

    float fCMRadio_X;
    float fCMRadio_Y;
    LUA_GET_VALUE_DEF( "CMRADIO_X", fCMRadio_X, 0.0f );
    LUA_GET_VALUE_DEF( "CMRADIO_Y", fCMRadio_Y, 0.0f );
    TempPart->SetCMRadio_X(fCMRadio_X);
    TempPart->SetCMRadio_Y(fCMRadio_Y);

    int   Z_Order;
    LUA_GET_VALUE_DEF( "Z_ORDER", Z_Order, 0 );
    TempPart->SetZOrder( Z_Order );

    bool PositionReset;
    LUA_GET_VALUE_DEF( "POSITION_RESET_X", PositionReset, false );
    TempPart->SetRepeat_X( PositionReset );
    LUA_GET_VALUE_DEF( "POSITION_RESET_Y", PositionReset, false );
    TempPart->SetRepeat_Y( PositionReset );

    //std::string     EffectName;
    //int             Effect_X;
    //int             Effect_Y;
    //LUA_GET_VALUE_DEF( "PART_EFFECT", EffectName, "" );
    //LUA_GET_VALUE_DEF( "EFFECT_X", Effect_X, 0 );
    //LUA_GET_VALUE_DEF( "EFFECT_Y", Effect_Y, 0 );
    //TempPart->CreateChildEffect( EffectName, Effect_X, Effect_Y );

    std::string     UniqueName;
    LUA_GET_VALUE_DEF( "OBJECT_NAME", UniqueName, TempPart->GetPartName() );
    TempPart->SetObjectName( UniqueName );

    DWORD           dwUserValue;
    LUA_GET_VALUE_DEF( "USER_VALUE", dwUserValue, TempPart->GetUserValue() );
    TempPart->SetUserValue( dwUserValue );

    std::string userString;
    LUA_GET_VALUE_DEF( "USER_STRING", userString, TempPart->GetUserString() );
    TempPart->SetUserString( userString );

    bool bShow;
    LUA_GET_VALUE_DEF( "SHOW", bShow, true );
    TempPart->SetShow( bShow );

    for ( unsigned int i = 0 ; i < TempPart->GetParticleCount(); ++i )
    {
        KLayerRect<float> kRect;
        TempPart->GetLayerRect( kRect );
        CParticleEventSeqPTR pParticle;
        pParticle = g_ParticleManager->CreateSequence( TempPart->GetParticleData(i).strParticleName, 
            kRect.m_X + TempPart->GetParticleData(i).fOffsetX , 
            kRect.m_Y + TempPart->GetParticleData(i).fOffsetY , 
            1.0f );
        if ( GetLayer() == LAST_LAYER )
        {
            g_ParticleManager->SetLayer( pParticle,GC_LAYER_SKY );
        }
		else if ( GetLayer() == LAST_LAYER - 1 )
		{
			g_ParticleManager->SetLayer( pParticle,GC_LAYER_SCROLLED_LAYER1 );
		}
		else if ( GetLayer() == LAST_LAYER - 2 )
		{
			g_ParticleManager->SetLayer( pParticle,GC_LAYER_SCROLLED_LAYER2 );
		}
		else if ( GetLayer() == LAST_LAYER - 3 )
		{
			g_ParticleManager->SetLayer( pParticle,GC_LAYER_SCROLLED_LAYER3 );
		}
        else if ( GetLayer() == LAST_LAYER - 4 )
        {
            g_ParticleManager->SetLayer( pParticle,GC_LAYER_BETWEEN_SKY_BLOCK );
        }
        else
        {
            g_ParticleManager->SetLayer( pParticle,GC_LAYER_BETWEEN_BLOCK_CHAR );
        }
        TempPart->m_vecParticleSeq.push_back( pParticle );
	}

    
    m_vecPart.push_back( TempPart );

    std::vector< CRenderPartPair<KGCLayerPart> >::iterator vit = std::find_if( m_vecRenderPart.begin(), m_vecRenderPart.end(), 
        KFindFileName<KGCLayerPart>( TempPart->GetFileName() ) );

    if( vit == m_vecRenderPart.end() )
    {
        CRenderPartPair<KGCLayerPart> partPair;
        partPair.first = TempPart->GetFileName();
        partPair.push_back( TempPart );
        m_vecRenderPart.push_back( partPair );
    }
    else
    {
        CRenderPartPair<KGCLayerPart>& renderPartPair = (*vit);
        renderPartPair.push_back( TempPart );
    }

    
   

}

void KGCLayer::CreateBuffer()
{
	/*
    if( m_vecPart.empty() )
        return;

    char szTemp[256] = { 0, };

    sprintf( szTemp, "KGCLayer_m_pVBPartsRender%d", m_iLayerNum );
    m_pVBPartsRender = g_pGCDeviceManager->CreateVertexBuffer( szTemp, MAX_PART_VERTEX * sizeof(SLVERTEX), 
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, SLVERTEX::FVF, D3DPOOL_DEFAULT, NULL );

    sprintf( szTemp, "KGCLayer_m_pIBPartsRender%d", m_iLayerNum );
    m_pIBPartsRender = g_pGCDeviceManager->CreateIndexBuffer( szTemp, MAX_PART_INDEX * sizeof(WORD),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, NULL );*/
}
