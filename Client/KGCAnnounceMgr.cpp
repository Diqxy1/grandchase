#include "stdafx.h"
//
#include ".\KGCAnnounceMgr.h"



#include "MyD3D.h"
//
#include "GCStateMachine.h"
//
//
#include "KGCEmoticon.h"


ImplementSingleton( KGCAnnounceMgr );

// 55 * 15
#define MAX_ANNOUNCE_TIME (825)
#define ANNOUNCE_TIME_SEC (15)
#define ANNOUNCE_SERVER_TIME_SEC (15)
#define SCREENTOCLIENTWIDTH(p) ( ( ((p)*0.0025f)/ GC_SCREEN_DIV_WIDTH) - 1.0f )
#define SCREENTOCLIENTHEIGHT(p) ( 0.75f - ( ((p)*0.003333f) / GC_SCREEN_DIV_WIDTH) * 0.75f )

SimpleAnnounce::SimpleAnnounce()
: m_fMoveTerm(0.0f)
, m_dwAnnounceTime(0)
, m_vTextTerm(0.0f, 0.0f)
, m_vTextPos(0.0f, 0.0f)
, m_vTextRect(0.0f, 0.0f)
{
    InitAnnounceData( m_kAnnounceData );
    m_kNeonSignData.Init();
}

SimpleAnnounce::~SimpleAnnounce()
{

}

void SimpleAnnounce::Init()
{
    m_fMoveTerm = 0.0f;
    m_dwAnnounceTime = 0;
    InitAnnounceData( m_kAnnounceData );
    m_kNeonSignData.Init();

    
}

void SimpleAnnounce::InitAnnounceData( KSignBoardData& kData )
{
    kData.m_ItemID = 0;
    kData.m_dwSenderUID = 0;
    kData.m_eType = KSignBoardData::SBT_MAX;
    kData.m_strMsg = L"";
    kData.m_strSenderNick = L"";
}

// 메세지 포맷을 바꿀수도 있는데 그렇게는 필요하면 하자
void SimpleAnnounce::GetMessage( std::wstring& strMsg ) const
{
    strMsg = L"";
    std::wstringstream stm;

    switch( m_kAnnounceData.m_eType )
    {
    case KSignBoardData::SBT_SERVER_ADMIN:
    case KSignBoardData::SBT_SQUARE_ADMIN:
	case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        stm << m_kAnnounceData.m_strMsg;
        strMsg = stm.str();
        break;
    default:
		if( !m_kAnnounceData.m_strSenderNick.empty() )
		{
			 stm << m_kAnnounceData.m_strSenderNick << L" : ";
		}
       
		stm << m_kAnnounceData.m_strMsg;
        strMsg = stm.str();
        break;
    }
}

#define RENDER_FRAME(p,t) vPos2 = vPos1 + kFrameData.TexSize[p];\
                        vUV = kFrameData.TexUVPos[p];\
                        if( vPos2.x - vPos1.x > 1.0f && vPos2.y - vPos1.y > 1.0f )\
                        {\
						g_pGCDeviceManager2->DrawInScreen(t, vPos1.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos1.y * g_pGCDeviceManager2->GetWindowScaleY(),\
						vPos2.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos2.y * g_pGCDeviceManager2->GetWindowScaleY(),\
						vUV.x, vUV.y, vUV.z, vUV.w );\
                        }

void SimpleAnnounce::RenderFrame(GCDeviceTexture * tex)
{
    switch( m_kAnnounceData.m_eType )
    {
    case KSignBoardData::SBT_SERVER_ADMIN:
    case KSignBoardData::SBT_SQUARE_ADMIN:
	case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        return RenderFrameAdmin(tex);
        break;
    default:
        return RenderFrameType2(tex);
        break;
    }
}

//void SimpleAnnounce::RenderFrameType0()
//{
//    const SNeonSignFrame& kFrameData = m_kNeonSignData.GetCurFrameData();
//
//    D3DXVECTOR2 vPos1, vPos2, vCenterPos;
//    D3DXVECTOR4 vUV = kFrameData.TexUVPos[KNeonSignItem::NSP_LEFTTOP];
//    vCenterPos = m_vTextPos - m_vTextTerm;
//
//    // lefttop
//    vPos1 = vCenterPos - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP];
//    RENDER_FRAME( KNeonSignItem::NSP_LEFTTOP );
//    // left
//    vPos1 = vCenterPos;
//    vPos1.x -= kFrameData.TexSize[KNeonSignItem::NSP_LEFT].x;
//    RENDER_FRAME( KNeonSignItem::NSP_LEFT );
//    // leftbottom
//    vPos1 = vPos2;
//    vPos1.x -= kFrameData.TexSize[KNeonSignItem::NSP_LEFTBOTTOM].x;
//    RENDER_FRAME( KNeonSignItem::NSP_LEFTBOTTOM );
//
//    // top
//    vPos1.y = vCenterPos.y - kFrameData.TexSize[KNeonSignItem::NSP_TOP].y;
//    for( float fPatternX = vCenterPos.x; fPatternX <= vCenterPos.x + m_vTextRect.x; fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_TOP].x )
//    {
//        vPos1.x = fPatternX;
//        RENDER_FRAME( KNeonSignItem::NSP_TOP );
//    }
//    float fRightPos = fPatternX;
//
//    // righttop
//    vPos1.x = fRightPos;
//    vPos1.y = vCenterPos.y - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHTTOP );
//    // right
//    vPos1.x = fRightPos;
//    vPos1.y = vCenterPos.y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHT );
//
//    //rightbottom
//    vPos1.x = fRightPos;
//    vPos1.y = vPos2.y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHTBOTTOM );
//
//    // Center
//    vPos1 = vCenterPos;
//    vPos2.x = fRightPos;
//    vPos2.y = vCenterPos.y + m_vTextRect.y + m_vTextTerm.y * 2.0f;
//    vUV = kFrameData.TexUVPos[ KNeonSignItem::NSP_CENTER];
//    g_MyD3D->Draw_TL( vPos1.x, vPos1.y, vPos2.x, vPos2.y, vUV.x, vUV.y, vUV.z, vUV.w );
//
//    // bottom
//    vPos1.y = vPos2.y;
//    for( float fPatternX = vCenterPos.x; fPatternX <= vCenterPos.x + m_vTextRect.x; fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_BOTTOM].x )
//    {
//        vPos1.x = fPatternX;
//        RENDER_FRAME( KNeonSignItem::NSP_BOTTOM );
//    }
//}
//
//void SimpleAnnounce::RenderFrameType1()
//{
//    const SNeonSignFrame& kFrameData = m_kNeonSignData.GetCurFrameData();
//
//    D3DXVECTOR2 vPos1, vPos2, vCenterPos;
//    D3DXVECTOR4 vUV = kFrameData.TexUVPos[KNeonSignItem::NSP_LEFTTOP];
//
//    // lefttop
//    vPos1 = D3DXVECTOR2( 0.0f, m_vTextPos.y - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].y - m_vTextTerm.y );
//    RENDER_FRAME( KNeonSignItem::NSP_LEFTTOP );
//    // left
//    vPos1 = vPos2;
//    vPos1.x = 0.0f;
//    RENDER_FRAME( KNeonSignItem::NSP_LEFT );
//    // leftbottom
//    vPos1 = vPos2;
//    vPos1.x = 0.0f;
//    RENDER_FRAME( KNeonSignItem::NSP_LEFTBOTTOM );
//
//    // top
//    vPos1.x = kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].x;
//    vPos1.y = m_vTextPos.y - kFrameData.TexSize[KNeonSignItem::NSP_TOP].y - m_vTextTerm.y;
//    float fStart = vPos1.x;
//    for( float fPatternX = fStart; fPatternX < 800.0f - kFrameData.TexSize[KNeonSignItem::NSP_TOP].x; fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_TOP].x )
//    {
//        vPos1.x = fPatternX;
//        RENDER_FRAME( KNeonSignItem::NSP_TOP );
//    }
//    float fRightPos = fPatternX;
//
//    // righttop
//    vPos1.x = fRightPos;
//    vPos1.y = m_vTextPos.y - kFrameData.TexSize[KNeonSignItem::NSP_RIGHTTOP].y - m_vTextTerm.y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHTTOP );
//    // right
//    vPos1.x = fRightPos;
//    vPos1.y = m_vTextPos.y - m_vTextTerm.y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHT );
//
//    //rightbottom
//    vPos1.x = fRightPos;
//    vPos1.y = vPos2.y;
//    RENDER_FRAME( KNeonSignItem::NSP_RIGHTBOTTOM );
//
//    // Center
//    vPos1 = D3DXVECTOR2( kFrameData.TexSize[KNeonSignItem::NSP_LEFT].x, m_vTextPos.y - m_vTextTerm.y );
//    vPos2.x = fRightPos;
//    vPos2.y = vPos1.y + m_vTextRect.y + m_vTextTerm.y * 2.0f;
//    vUV = kFrameData.TexUVPos[ KNeonSignItem::NSP_CENTER];
//    g_MyD3D->Draw_TL( vPos1.x, vPos1.y, vPos2.x, vPos2.y, vUV.x, vUV.y, vUV.z, vUV.w );
//
//    // bottom
//    vPos1.x = kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].x;
//    vPos1.y = vPos2.y;
//    fStart = vPos1.x;
//    for( float fPatternX = fStart; fPatternX < 800.0f - kFrameData.TexSize[KNeonSignItem::NSP_BOTTOM].x; fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_BOTTOM].x )
//    {
//        vPos1.x = fPatternX;
//        RENDER_FRAME( KNeonSignItem::NSP_BOTTOM );
//    }
//}

void SimpleAnnounce::RenderFrameType2(GCDeviceTexture * tex)
{
    const SNeonSignFrame& kFrameData = m_kNeonSignData.GetCurFrameData();

    D3DXVECTOR2 vPos1, vPos2, vCenterPos;
    D3DXVECTOR4 vUV = kFrameData.TexUVPos[KNeonSignItem::NSP_LEFTTOP];
    vCenterPos = m_vTextPos - m_vTextTerm;

    // Frame
    vPos1 = D3DXVECTOR2( 0.0f, m_vTextPos.y - m_vTextTerm.y - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].y );
    vPos2.x = GC_SCREEN_FLOAT_WIDTH;
    vPos2.y = vCenterPos.y + m_vTextRect.y + m_vTextTerm.y * 2.0f + kFrameData.TexSize[KNeonSignItem::NSP_LEFTBOTTOM].y;
    vUV = kFrameData.TexUVPos[ KNeonSignItem::NSP_CENTER];
	g_pGCDeviceManager2->DrawInScreen(tex, vPos1.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos1.y * g_pGCDeviceManager2->GetWindowScaleY(), 
		vPos2.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos2.y * g_pGCDeviceManager2->GetWindowScaleY(),
		vUV.x, vUV.y, vUV.z, vUV.w );

    // lefttop
    vPos1 = vCenterPos - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP];
    RENDER_FRAME( KNeonSignItem::NSP_LEFTTOP , tex);
    // left
    vPos1 = vCenterPos;
    vPos1.x -= kFrameData.TexSize[KNeonSignItem::NSP_LEFT].x;
    RENDER_FRAME( KNeonSignItem::NSP_LEFT , tex);
    // leftbottom
    vPos1 = vPos2;
    vPos1.x -= kFrameData.TexSize[KNeonSignItem::NSP_LEFTBOTTOM].x;
    RENDER_FRAME( KNeonSignItem::NSP_LEFTBOTTOM , tex);

    // top
    float fPatternX = vCenterPos.x;
    vPos1.y = vCenterPos.y - kFrameData.TexSize[KNeonSignItem::NSP_TOP].y;
    for( int i = 0; i < m_nTopLoopCnt; ++i )
    {
        vPos1.x = fPatternX;
        RENDER_FRAME( KNeonSignItem::NSP_TOP , tex);
        fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_TOP].x;
    }
    float fRightPos = fPatternX;

    // righttop
    vPos1.x = fRightPos;
    vPos1.y = vCenterPos.y - kFrameData.TexSize[KNeonSignItem::NSP_LEFTTOP].y;
    RENDER_FRAME( KNeonSignItem::NSP_RIGHTTOP , tex);
    // right
    vPos1.x = fRightPos;
    vPos1.y = vCenterPos.y;
    RENDER_FRAME( KNeonSignItem::NSP_RIGHT , tex);

    //rightbottom
    vPos1.x = fRightPos;
    vPos1.y = vPos2.y;
    RENDER_FRAME( KNeonSignItem::NSP_RIGHTBOTTOM , tex);

    // Center
    vPos1 = vCenterPos;
    vPos2.x = fRightPos;
    vPos2.y = vCenterPos.y + m_vTextRect.y + m_vTextTerm.y * 2.0f;
    vUV = kFrameData.TexUVPos[ KNeonSignItem::NSP_CENTER];
	g_pGCDeviceManager2->DrawInScreen(tex, vPos1.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos1.y * g_pGCDeviceManager2->GetWindowScaleY(),
		vPos2.x * g_pGCDeviceManager2->GetWindowScaleX(), vPos2.y * g_pGCDeviceManager2->GetWindowScaleY(),
		vUV.x, vUV.y, vUV.z, vUV.w );

    // bottom
    vPos1.y = vCenterPos.y + m_vTextRect.y + m_vTextTerm.y * 2.0f;
    fPatternX = vCenterPos.x;
    for( int i = 0; i < m_nBottomLoopCnt; ++i )
    {
        vPos1.x = fPatternX;
        RENDER_FRAME( KNeonSignItem::NSP_BOTTOM , tex);
        fPatternX += kFrameData.TexSize[KNeonSignItem::NSP_BOTTOM].x;
    }
}

void SimpleAnnounce::ResetTime()
{
    m_dwAnnounceTime = 0;
    m_kNeonSignData.ResetTime();
}

void SimpleAnnounce::SetAnnounceData( const KSignBoardData& kData )
{
    m_kAnnounceData = kData;
    switch( m_kAnnounceData.m_eType )
    {
    case KSignBoardData::SBT_SERVER_ADMIN:
    case KSignBoardData::SBT_SQUARE_ADMIN:
	case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        m_iFontSize = SiKGCMultipleLanguages()->GetLargeFontSize();
        break;
    default:
        m_iFontSize = SiKGCMultipleLanguages()->GetMiddleFontSize();
        break;
    }

    // 전광판은 미지원..
    KD3DEmoticonString::EmotStrPair prPush;
    prPush.first = false;
    prPush.second.first = NULL;
    prPush.second.second = m_kAnnounceData.m_strMsg;
    m_vecText.push_back( prPush );
    // 지원하려면 아래코드로
    //SiKD3DEmoticonString()->clear();
    //SiKD3DEmoticonString()->DivideStringPiece( m_kAnnounceData.m_strMsg );
    //SiKD3DEmoticonString()->GetDivideString( m_vecText );
    //SiKD3DEmoticonString()->clear();
}

void SimpleAnnounce::RenderFrameAdmin(GCDeviceTexture * tex)
{
    g_pGCDeviceManager2->DrawInScreen(tex, 0.0f, (m_vTextPos.y - 7.0f) * g_pGCDeviceManager2->GetWindowScaleY(), GC_SCREEN_FLOAT_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(),( m_vTextPos.y + m_vTextRect.y + 10.0f ) * g_pGCDeviceManager2->GetWindowScaleY(), 0.0f, 0.0f, 1.0f, 1.0f, false,
        10.0f, D3DCOLOR_ARGB(200, 0, 0, 0 ));
}

bool SimpleAnnounce::IsAdminAnnounce() const
{
    switch( m_kAnnounceData.m_eType )
    {
    case KSignBoardData::SBT_SERVER_ADMIN:
    case KSignBoardData::SBT_SQUARE_ADMIN:
	case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}

bool SimpleAnnounce::IsHasEmoticon() const
{
    std::vector< KD3DEmoticonString::EmotStrPair >::const_iterator vit;
    for( vit = m_vecText.begin(); vit != m_vecText.end(); ++vit )
    {
        const KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
        if( prEmot.first == true )
        {
            return true;
        }
    }
    return false;
}

float SimpleAnnounce::GetTextWidth()
{
    // 여기에도 닉네임 고려안되어있음.
    //float fRet = 0.0f;

    //std::vector< KD3DEmoticonString::EmotStrPair >::const_iterator vit;
    //for( vit = m_vecText.begin(); vit != m_vecText.end(); ++vit )
    //{
    //    const KD3DEmoticonString::EmotStrPair& prEmot = (*vit);
    //    if( prEmot.first == true )
    //    {
    //        fRet += (float)EMOTICON_UI_SIZE;
    //    }
    //    else
    //    {
    //        fRet += static_cast<float>( GCFUNC::GetFontManager( GetFontSize() , true )->GetWidth( prEmot.second.second.c_str() ));
    //    }
    //}

    //return fRet;

    std::wstring strMsg;
    GetMessage( strMsg );
	KD3DStatic kStr;
	kStr.SetText(strMsg);

	POINT ptSize = kStr.GetStringWidthAndHeightInPixel();

    return static_cast<float>( GCFUNC::GetFontManager( GetFontSize() , true )->GetWidth( strMsg.c_str() ) );
}

KGCAnnounceMgr::KGCAnnounceMgr(void)
: m_bAnnounce(false)
, m_nWaitTime(0)
, m_pTexture(NULL)
, m_pPreviewTex(NULL)
, m_bPreview(false)
{
    m_vecAnnounce.clear();
    m_kCurrentAnnounce.Init();
    m_pairAnnounceList.first = 0;
    m_pairAnnounceList.second = 0;
    m_kPreview.Init();
	m_pRainbowEvent = NULL;
	m_fX1Pos = 1.0f;
	m_bIsRight = true;

    for (int i=0 ; i<2 ; i++ )
        m_pFullMoonPtr[i] = NULL;
}

KGCAnnounceMgr::~KGCAnnounceMgr(void)
{
    ClearFullMoonPtr();
}

void KGCAnnounceMgr::FrameMove()
{
    PROFILE_SET("KGCAnnounceMgr::FrameMove");

    if( m_vecAnnounce.empty() && false == m_bAnnounce )
    {
        return;
    }

    if( false == m_bAnnounce )
    {
        GetAnnounceInfo( m_kCurrentAnnounce );
    }

    ProcessAnnounce( m_kCurrentAnnounce );

    if( true == TimeOver( m_kCurrentAnnounce ) )
    {
        m_kCurrentAnnounce.Init();
        m_bAnnounce = false;
    }
}

void KGCAnnounceMgr::Render()
{
	// 무지개가 떴다
	if( g_kGlobalValue.m_bIsRainbowEvent &&
		!( g_MyD3D->m_pStateMachine->GetState() == GS_INIT_LOADING ||
			g_MyD3D->m_pStateMachine->GetState() == GS_LOGO ||
			g_MyD3D->m_pStateMachine->GetState() == GS_LOGIN ||
#if defined(FULLMOON_EVENT)
			g_MyD3D->m_pStateMachine->GetState() == GS_GAME ||
#endif
			g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ||
			g_MyD3D->m_pStateMachine->GetState() == GS_GAME_END ||
			g_MyD3D->m_pStateMachine->GetState() == GS_REPLAY ||
			g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ||
			g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT ||
            g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM ||
			g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING ) )

	{
#if defined(FULLMOON_EVENT)
        CreateFullMoonPtr();
#else
        if ( m_pRainbowEvent == NULL )
			m_pRainbowEvent = g_pGCDeviceManager2->CreateTexture( "rainbow_.dds" );
#endif
		if( g_kGlobalValue.m_bJackPot )
		{
#if defined(FULLMOON_EVENT)
            ToggleFullMoon( true );
            RenderFullMoon();
#else
            if (m_pRainbowEvent != NULL )
            {
			    g_pGCDeviceManager2->DrawInScreen(m_pRainbowEvent, m_fX1Pos, -42.0f, m_fX1Pos + 128.0f, -42.0f + 128.0f + 10.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			    SetRainbowPos();
            }
#endif
		}
#if defined(FULLMOON_EVENT)
        else
        {
            ToggleFullMoon( false );
        }
#endif
//         ToggleFullMoon( true );
//         RenderFullMoon();
	}
    
	// 무지개를 띄웠다

    if( (m_vecAnnounce.empty() && false == m_bAnnounce) ||
        false == IsAnnounceRenderState() )
    {
        return;
    }

    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    const KNeonSignItem& kNeonSignData = m_kCurrentAnnounce.GetNeonsignData();
    if( false == kNeonSignData.empty() || m_kCurrentAnnounce.IsAdminAnnounce() )
    {
        m_kCurrentAnnounce.RenderFrame(m_pTexture);
    }

    RenderMessage( m_kCurrentAnnounce );
}

void KGCAnnounceMgr::ReserveAnnounce( const KSignBoardData& kData )
{
    SimpleAnnounce kReserve;
    kReserve.Init();
    kReserve.SetAnnounceData( kData );

    switch( kData.m_eType )
    {
    case KSignBoardData::SBT_SERVER_ADMIN:
    case KSignBoardData::SBT_SQUARE_ADMIN:
	case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        PreprocessForAdmin( kReserve, 548.0f * GC_SCREEN_DIV_WIDTH );
        break;
    default:
        Preprocess( kReserve, 570.0f * GC_SCREEN_DIV_WIDTH );
        break;
    }
    m_vecAnnounce.push_back( kReserve );
}

void KGCAnnounceMgr::GetAnnounceInfo( SimpleAnnounce& kInfo )
{
    kInfo.Init();
    if( m_vecAnnounce.empty() )
    {
        ASSERT( m_vecAnnounce.empty() == false );
        return;
    }

    kInfo = (*m_vecAnnounce.begin());
    m_vecAnnounce.erase( m_vecAnnounce.begin() );

    m_bAnnounce = true;
    const KNeonSignItem& kNeonSignData = kInfo.GetNeonsignData();
    if( false == kNeonSignData.empty() )
    {
        GCDeviceTexture* pTempTex = m_pTexture;
        m_pTexture = g_pGCDeviceManager2->CreateTexture( kNeonSignData.GetTextureName() );
        SAFE_RELEASE( pTempTex );
    }
    else if( kInfo.IsAdminAnnounce() )
    {
        GCDeviceTexture* pTempTex = m_pTexture;
        m_pTexture = g_pGCDeviceManager2->GetNullTexture();
        SAFE_RELEASE( pTempTex );
    }
}

void KGCAnnounceMgr::ProcessAnnounce( SimpleAnnounce& kAnnounce )
{
    D3DXVECTOR2 vPos = kAnnounce.GetTextPos();
    vPos.x -= kAnnounce.GetMoveTerm();
    kAnnounce.SetTextPos( vPos );
    kAnnounce.IncreaseTime();
}

void KGCAnnounceMgr::ReserveforDebug( const KSignBoardData& kData )
{
    ReserveAnnounce( kData );
}

void KGCAnnounceMgr::RefreshAnnounceList()
{
    KP2P::GetInstance()->Send_GetSignBoardRegNum();
}

void KGCAnnounceMgr::SetAnnounceList( const std::pair< DWORD, DWORD >& kList )
{
    m_pairAnnounceList = kList;
}

void KGCAnnounceMgr::SetWaitTime( int nWait )
{
    // nWait == 대기열 이고 서버에서 18초 간격으로 준다
    m_nWaitTime = nWait * ANNOUNCE_SERVER_TIME_SEC;
}

std::wstring KGCAnnounceMgr::GetWaitTime()
{
    CTime kAnnounce = CTime::GetCurrentTime();
    kAnnounce += CTimeSpan(0,0,0,m_nWaitTime); // m_nWaitTime 초 뒤의 시간
    return g_pkStrLoader->GetReplacedString( STR_ID_NEONSIGN_ANNOUNCE_RESULT, "iiii",
        kAnnounce.GetMonth(), kAnnounce.GetDay(), kAnnounce.GetHour(), kAnnounce.GetMinute() );
}

void KGCAnnounceMgr::RenderMessage( const SimpleAnnounce& kAnnounce )
{
    std::wstring strMessage = L"";
    kAnnounce.GetMessage( strMessage );

    GCDeviceFont* pFont = GCFUNC::GetFontManager( kAnnounce.GetFontSize(), true );

    const D3DXVECTOR2& vPos = kAnnounce.GetTextPos();
    int left = (int)( ( SCREENTOCLIENTWIDTH(vPos.x) + 1.0f ) * GC_SCREEN_DIV_WIDTH_HALF );
    int top = (int)( GC_SCREEN_FLOAT_HEIGHT - (int)( ( SCREENTOCLIENTHEIGHT(vPos.y) + 0.75f ) / 1.5f *GC_SCREEN_FLOAT_HEIGHT ) );

	pFont->OutTextXY( 
		(int)(left * g_pGCDeviceManager2->GetWindowScaleX()), 
		(int)(top * g_pGCDeviceManager2->GetWindowScaleY()), 
		strMessage.c_str(), kAnnounce.GetColor() );

    // 여기에는 닉네임 출력이 고려되어있지 않다.. 전광판에 이모티콘 넣으려면 고려하세요.
    //const std::vector< KD3DEmoticonString::EmotStrPair >& vecText = kAnnounce.GetText();
    //std::vector< KD3DEmoticonString::EmotStrPair >::const_iterator cvit;
    //bool bHasEmoticon = kAnnounce.IsHasEmoticon();
    //const D3DXVECTOR2& vPos = kAnnounce.GetTextPos();
    //D3DXVECTOR2 vPosTemp = vPos;
    //for( cvit = vecText.begin(); cvit != vecText.end(); ++cvit )
    //{
    //    const KD3DEmoticonString::EmotStrPair& prEmot = (*cvit);
    //    // 이모티콘인 경우
    //    if( prEmot.first == true )
    //    {
    //        if( prEmot.second.first != NULL )
    //        {
    //            DrawEmoticon_( prEmot.second.first, vPosTemp );
    //        }
    //    }
    //    else
    //    {
    //        DrawContent_( prEmot.second.second, vPosTemp, bHasEmoticon, kAnnounce.GetColor(), kAnnounce.GetFontSize() );
    //    }
    //}
}

bool KGCAnnounceMgr::IsAnnounceRenderState()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING || 
        g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING ||
        (SiKGCWorldMapManager()->IsHeroDungeonMode() && g_MyD3D->m_pStateMachine->GetState() == GS_GAME))
    {
        return false;
    }

    return true;
}

void KGCAnnounceMgr::Preprocess( OUT SimpleAnnounce& kReserve, float fAnnouncePosY )
{
    const KSignBoardData& kData = kReserve.GetAnnounceData();

    KNeonSignItem kNeonSignData;
    SiKNeonSign()->GetNeonSignData( kData.m_ItemID, kNeonSignData );
    kReserve.SetNeonsignData( kNeonSignData );

    // [3/3/2008] breadceo. 이쪽 정리해야함. 스크립트 값에 유동적이지 못하다
    D3DXVECTOR2 kLeftTop, kRightTop, kRight, kTop, kLeftBottom, kBottom;
    kNeonSignData.GetRect( KNeonSignItem::NSP_LEFTTOP, kLeftTop );
    kNeonSignData.GetRect( KNeonSignItem::NSP_RIGHTTOP, kRightTop );
    kNeonSignData.GetRect( KNeonSignItem::NSP_RIGHT, kRight );
    kNeonSignData.GetRect( KNeonSignItem::NSP_TOP, kTop );
    kNeonSignData.GetRect( KNeonSignItem::NSP_LEFTBOTTOM, kLeftBottom );
    kNeonSignData.GetRect( KNeonSignItem::NSP_BOTTOM, kBottom );

    float fFontSize = static_cast<float>(kReserve.GetFontSize());

    std::wstring strMsg;
    kReserve.GetMessage( strMsg );
    float fTextWidth = kReserve.GetTextWidth()/g_pGCDeviceManager2->GetWindowScaleX();

    int nTopCount = 0;
	float fX;
    for( fX = 0; fX <= fTextWidth; fX += kTop.x )
    {
        nTopCount++;
    }
    int nBottomCount = 0;
    for( fX = 0; fX <= fTextWidth; fX += kBottom.x )
    {
        nBottomCount++;
    }
    float fNeonSignWidth = fX + kLeftTop.x + kRightTop.x;
    float fNeonSignHeight = kLeftBottom.y + kLeftTop.y + fFontSize;

    kReserve.SetTopLoopCnt( nTopCount );
    kReserve.SetBottomLoopCnt( nBottomCount );
    kReserve.SetTextTerm( D3DXVECTOR2( ( fX - fTextWidth ) * 0.5f, ( kRight.y - fFontSize ) * 0.5f ) );
    kReserve.SetTextPos( D3DXVECTOR2( GC_SCREEN_FLOAT_WIDTH + kLeftTop.x + kReserve.GetTextTerm().x, fAnnouncePosY - fNeonSignHeight + kReserve.GetTextTerm().y ) );
    kReserve.SetTextRect( D3DXVECTOR2( fTextWidth, fFontSize ) );
    kReserve.SetNeonsignRect( D3DXVECTOR2( fNeonSignWidth, fNeonSignHeight ) );

    // 1 Frame 당 움직어야할 거리
    float fMoveTerm = ( GC_SCREEN_FLOAT_WIDTH + fNeonSignWidth ) / MAX_ANNOUNCE_TIME;
    kReserve.SetMoveTerm( fMoveTerm );
}

void KGCAnnounceMgr::PreviewFrameMove()
{
	if( false == m_bPreview )
		return;

	ProcessAnnounce( m_kPreview );
	if( true == TimeOver( m_kPreview ) )
	{
		m_kPreview.ResetTime();
		const KSignBoardData& kData = m_kPreview.GetAnnounceData();
		D3DXVECTOR2 kLeftTop;
		const KNeonSignItem& kNeonSignData = m_kPreview.GetNeonsignData();
		kNeonSignData.GetRect( KNeonSignItem::NSP_LEFTTOP, kLeftTop );
		m_kPreview.SetTextPos( D3DXVECTOR2( GC_SCREEN_FLOAT_WIDTH + kLeftTop.x + m_kPreview.GetTextTerm().x, m_kPreview.GetTextPos().y ) );
	}
}

void KGCAnnounceMgr::PreviewRender()
{
    if( false == m_bPreview )
        return;

    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    const KNeonSignItem& kNeonSignData = m_kPreview.GetNeonsignData();
    if( false == kNeonSignData.empty() )
    {        
        m_kPreview.RenderFrame(m_pPreviewTex);
    }

    RenderMessage( m_kPreview );
}

void KGCAnnounceMgr::SetPreviewData( const SimpleAnnounce& kPreview )
{
    m_kPreview = kPreview;

    const KNeonSignItem& kNeonSignData = m_kPreview.GetNeonsignData();
    if( false == kNeonSignData.empty() )
    {
        GCDeviceTexture* pTempTex = m_pPreviewTex;
        m_pPreviewTex = g_pGCDeviceManager2->CreateTexture( kNeonSignData.GetTextureName() );
        SAFE_RELEASE( pTempTex );
    }
}

bool KGCAnnounceMgr::TimeOver( const SimpleAnnounce& kAnnounce )
{
    if( kAnnounce.GetAnnounceTime() > MAX_ANNOUNCE_TIME )
    {
        return true;
    }
    return false;
}

void KGCAnnounceMgr::PreprocessForAdmin( OUT SimpleAnnounce& kReserve, float fAnnouncePosY )
{
    kReserve.SetColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );

    float fFontSize = static_cast<float>(kReserve.GetFontSize());
    std::wstring strMsg;
    kReserve.GetMessage( strMsg );
    float fTextWidth = static_cast<float>( GCFUNC::GetFontManager(kReserve.GetFontSize() , true )->GetWidth( strMsg.c_str() ));

    kReserve.SetTextTerm( D3DXVECTOR2( 0.0f, 0.0f ) );
    kReserve.SetTextPos( D3DXVECTOR2( GC_SCREEN_FLOAT_WIDTH , fAnnouncePosY ) );
    kReserve.SetTextRect( D3DXVECTOR2( fTextWidth, fFontSize ) );
    kReserve.SetNeonsignRect( D3DXVECTOR2( 0.0f, 0.0f ) );

    // 1 Frame 당 움직어야할 거리
    float fMoveTerm = ( GC_SCREEN_FLOAT_WIDTH + fTextWidth ) / MAX_ANNOUNCE_TIME;
    kReserve.SetMoveTerm( fMoveTerm );
}

void KGCAnnounceMgr::DrawContent_( const std::wstring& strContent, D3DXVECTOR2& vPos, bool bHasEmoticon, DWORD dwColor, int iFontSize )
{
    GCDeviceFont* pFont = GCFUNC::GetFontManager( iFontSize, true );

    int left = (int)( ( SCREENTOCLIENTWIDTH(vPos.x) + 1.0f ) * GC_SCREEN_DIV_WIDTH_HALF );
    int top = (int)( GC_SCREEN_FLOAT_HEIGHT - (int)( ( SCREENTOCLIENTHEIGHT(vPos.y) + 0.75f ) / 1.5f * GC_SCREEN_FLOAT_HEIGHT ) );
    pFont->OutTextXY( int( vPos.x ), int( vPos.y ), strContent.c_str(), dwColor );
    vPos.x += (float)pFont->GetWidth( strContent.c_str() );
}

void KGCAnnounceMgr::DrawEmoticon_( KGCEmoticon* pEmoticon, D3DXVECTOR2& vPos )
{
    const float fHalfSize = (float)EMOTICON_UI_SIZE * 0.5f;
    pEmoticon->RenderForChatbox( vPos.x, vPos.y - fHalfSize, vPos.x + EMOTICON_UI_SIZE, vPos.y + fHalfSize );
    vPos.x += (float)EMOTICON_UI_SIZE;
}

void KGCAnnounceMgr::SetRainbowPos()
{
	if( m_fX1Pos >= GC_SCREEN_DIV_WIDTH_HALF )
		m_bIsRight = false;
	else if( m_fX1Pos <= 1.0f )
		m_bIsRight = true;

	m_fX1Pos += ( (m_bIsRight) ? 0.1f: -0.1f);
}

void KGCAnnounceMgr::CreateFullMoonPtr()
{
    //위치를 UI에서 가져오게 하기에는 연관인은 UI 가 없네요..
    
    if ( m_pFullMoonPtr[0] == NULL )
        m_pFullMoonPtr[0] = g_ParticleManager->CreateSequence( "UI_Fulllmoon_01", PARTICLEPOS_X(700.0f), PARTICLEPOS_Y(45.0f), 0.5f );
    if ( m_pFullMoonPtr[0] )
    {
        m_pFullMoonPtr[0]->SetShow( false );
        g_ParticleManager->SetLayer( m_pFullMoonPtr[0], GC_LAYER_FULLMOON );
    }

    if ( m_pFullMoonPtr[1] == NULL )
        m_pFullMoonPtr[1] = g_ParticleManager->CreateSequence( "UI_Fulllmoon_02", PARTICLEPOS_X(700.0f), PARTICLEPOS_Y(45.0f), 0.5f );
    if ( m_pFullMoonPtr[1] )
    {
        m_pFullMoonPtr[1]->SetShow( false );
        g_ParticleManager->SetLayer( m_pFullMoonPtr[1], GC_LAYER_FULLMOON );
    }

}

void KGCAnnounceMgr::ToggleFullMoon( bool bRender_ )
{
    for ( int i=0 ; i<2 ; i++ )
    {
        if ( m_pFullMoonPtr[i] )
            m_pFullMoonPtr[i]->SetShow( bRender_ );
    }
}

void KGCAnnounceMgr::ClearFullMoonPtr()
{
    for ( int i=0 ; i<2 ; i++ )
    {
        if ( m_pFullMoonPtr[i] )
            g_ParticleManager->DeleteSequence(m_pFullMoonPtr[i]);
    }
}

void KGCAnnounceMgr::RenderFullMoon()
{
    g_ParticleManager->Render(GC_LAYER_FULLMOON);
}
