#include "stdafx.h"
#include "KQSLayer.h"
#include "KQSLayerManager.h"
#include "QSFantasticMap.h"
#include "../DlgMapToObject.h"

KQSLayer::KQSLayer( KQSLayerManager* pManager )
: KLayer<KQSLayerPart, KQSLayerManager>( pManager )
{
    m_fLookX      = 0.0f;
    m_fLookY      = 0.0f;
    m_fCMLeft     = 0.0f;
    m_fCMRight    = 0.0f;
    m_fCMTop      = 0.0f;
    m_fCMBottom   = 0.0f;
}

KQSLayer::~KQSLayer(void)
{
}

void KQSLayer::SetColor( DWORD dwColorNormal, DWORD dwColorSky )
{
    DWORD dwColor = 0xffffffff;
    if( m_iLayerNum == LAST_LAYER )
        dwColor = dwColorSky;
    else
        dwColor = dwColorNormal;

    std::for_each( m_vecPart.begin(), m_vecPart.end(),
        boost::bind( &KQSLayerPart::SetColor, _1, dwColor ) );
}

bool KQSLayer::IsAlpha()
{
    return m_iLayerNum <= m_pLayerManager->GetPlayerLayer() ? true : false;
}

void KQSLayer::FrameMove( float fElapsedTime )
{
    //PROFILE2( "KQSLayer::FrameMove" );

    if( m_vecPart.empty() )
        return;

    Process();
    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    {
        KQSLayerPart *pLayerPart = m_vecPart[i];
        m_vecPart[i]->FrameMove( fElapsedTime );        
    }
    KLayer<KQSLayerPart, KQSLayerManager>::SetBufferType1();
}

void KQSLayer::Process()
{
    //if( m_vecPart.empty() )
    //    return;

    //if( m_iLayerNum == LAST_LAYER )
    //{
    //    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    //    {
    //        KQSLayerPart* pPart = static_cast< KQSLayerPart* >( m_vecPart[i] );
    //        pPart->ProcessRepeat( m_fLayerSizeX, m_fLayerSizeY );
    //    }
    //    return;
    //}

    //D3DXVECTOR3& CameraPos = D3DXVECTOR3( m_fLookX/400.0f + 1.0f, m_fLookY/400.0f + 0.75f, 0.0f );

    //// 카메라가 볼수 있는 영역 알아내자. 줌이 적용되어 있다.
    //float fWidth = m_fCMRight - m_fCMLeft;//2.0f;//800.0f;
    //float fHeight = m_fCMTop - m_fCMBottom;//1.5f;//600.0f;
    //fWidth /= 400.0f;
    //fWidth /= 400.0f;

    //// 맵 전체의 크기를 알아낸다
    //float PlayingWidth = m_pLayerManager->GetMapPtr()->GetPixelWidth();
    //float PlayingHeight = m_pLayerManager->GetMapPtr()->GetPixelHeight();

    //float ScrollX = 0.0f;
    //float ScrollY = 0.0f;

    //// cw = 카메라가 카메라 영역에서 얼마나 움직였는가
    //float cwx = CameraPos.x - fWidth * 0.5f;
    //float cwy = CameraPos.y - fHeight * 0.5f;

    //// 스크롤될 값 = 기준 레이어의 크기 - 화면크기 / 전체크기 - 화면크기 * cw
    //if( PlayingWidth > fWidth )
    //    ScrollX = ( (m_fLayerSizeX - fWidth) / (PlayingWidth - fWidth) ) * cwx;
    //else
    //    ScrollX = (m_fLayerSizeX - fWidth);

    //if( PlayingHeight > fHeight )
    //    ScrollY = ( (m_fLayerSizeY - fHeight) / (PlayingHeight - fHeight) ) * cwy;
    //else
    //    ScrollY = (m_fLayerSizeY - fHeight);

    //for( int i = 0; i < (int)m_vecPart.size(); ++i )
    //{
    //    KQSLayerPart* pPart = static_cast< KQSLayerPart* >( m_vecPart[i] );

    //    // 스케일값 설정 & Scroll 될 값 설정
    //    if( m_fLayerSizeX <= PlayingWidth && m_fLayerSizeY <= PlayingHeight )
    //    {
    //        // 원래 스크롤 되어야 하는 값에 카메라영역을 적용시켜줘야함 ( 레이어를 통채로 이동하는게 아니고 파트들이 이동하기 때문 )
    //        pPart->SetScrollPos( -ScrollX + cwx, -ScrollY + cwy );
    //    }
    //    pPart->ProcessRepeat( m_fLayerSizeX, m_fLayerSizeY );

    //    if( m_iLayerNum < SiKQSLayerManager()->GetPlayerLayer() )
    //    {
    //        // PlayerBack 설정
    //        //SetPlayerBack( pPart, g_kCamera.m_iTargetPlayer );
    //    }
    //}

#define WOG_WIDTH 800.0f
#define WOG_HEIGHT 600.0f

    if( m_vecPart.empty() )
        return;

    float PlayingWidth = static_cast<float>( m_pLayerManager->GetMapPtr()->GetPixelWidth() )*400.0f;
    float PlayingHeight = static_cast<float>( m_pLayerManager->GetMapPtr()->GetPixelHeight() )*400.0f;

    float ScrollX = 0.0f;
    float ScrollY = 0.0f;
    ScrollX = ( ((float)m_fLayerSizeX - WOG_WIDTH) / (PlayingWidth - WOG_WIDTH) ) * ( m_fLookX - WOG_WIDTH * 0.5f );
    ScrollY = ( ((float)m_fLayerSizeY - WOG_HEIGHT) / (PlayingHeight - WOG_HEIGHT) ) * ( m_fLookY - WOG_HEIGHT * 0.5f );

    if( m_fLayerSizeX == 800 )
        ScrollX = 0;
    if( m_fLayerSizeY == 600 )
        ScrollY = 0;

    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    {
        m_vecPart[i]->SetScrollPos( -ScrollX + m_fLookX - WOG_WIDTH * 0.5f , 
            -ScrollY + m_fLookY - WOG_HEIGHT * 0.5f);
        m_vecPart[i]->ProcessRepeat( m_fLayerSizeX, m_fLayerSizeY );
    }
}

HRESULT KQSLayer::Render()
{
    return KLayer<KQSLayerPart, KQSLayerManager>::Render();
}

void KQSLayer::AddParts( KLuaManager& luaMgr )
{
    std::string     PartName;
    LUA_GET_VALUE_DEF( "PARTNAME", PartName, "" );
    KQSLayerPart* TempPart = m_pLayerManager->CreateClonePart( PartName );

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
    kLayerRect.m_X      *= 400;
    kLayerRect.m_Y      *= 400;

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
    MoveSpeed_X      *= 400.0f;
    MoveSpeed_Y      *= 400.0f;
    TempPart->SetMoveProp( Moveable, MoveSpeed_X, MoveSpeed_Y );

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

    m_vecPart.push_back( TempPart );

    std::vector< CRenderPartPair<KQSLayerPart> >::iterator vit = std::find_if( m_vecRenderPart.begin(), m_vecRenderPart.end(), 
        KFindFileName<KQSLayerPart>( TempPart->GetFileName() ) );

    if( vit == m_vecRenderPart.end() )
    {
        CRenderPartPair<KQSLayerPart> partPair;
        partPair.first = TempPart->GetFileName();
        partPair.push_back( TempPart );
        m_vecRenderPart.push_back( partPair );
    }
    else
    {
        CRenderPartPair<KQSLayerPart>& renderPartPair = (*vit);
        renderPartPair.push_back( TempPart );
    }
}

void KQSLayer::SetCameraPos(float fLookX, float fLookY)
{
    m_fLookX = fLookX;
    m_fLookY = fLookY;
}

void KQSLayer::SetShowRect(float fCMLeft, float fCMRight, float fCMTop, float fCMBottom)
{
    m_fCMLeft = fCMLeft;
    m_fCMRight = fCMRight;
    m_fCMTop = fCMTop;
    m_fCMBottom = fCMBottom;

    for(int i=0;i<(int)m_vecPart.size();++i)
    {
        FRect_Quest rtCamera;
        rtCamera.SetRect( fCMLeft, fCMTop, fCMRight, fCMBottom );
        m_vecPart[i]->SetCameraRect(rtCamera);
    }
}

void KQSLayer::CreateBuffer()
{
    if( m_vecPart.empty() )
        return;

    char szTemp[256] = { 0, };

    sprintf( szTemp, "KQSLayer_m_pVBPartsRender%d", m_iLayerNum );
    m_pVBPartsRender = g_pGCDeviceManager->CreateVertexBuffer( szTemp, MAX_PART_VERTEX * sizeof(SLVERTEX), 
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, SLVERTEX::FVF, D3DPOOL_DEFAULT, NULL );

    sprintf( szTemp, "KQSLayer_m_pIBPartsRender%d", m_iLayerNum );
    m_pIBPartsRender = g_pGCDeviceManager->CreateIndexBuffer( szTemp, MAX_PART_INDEX * sizeof(WORD),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, NULL );
}
