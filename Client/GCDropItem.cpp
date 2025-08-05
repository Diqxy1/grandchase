#include "stdafx.h"
#include "KncP2PLib\KncP2P.h"
#include "Message.h"
#include ".\gcdropitem.h"
//

#include "Headup Display.h"
#include "GCCameraOrtho.h"
//
#include "MapSystem/GCFantasticMap.h"


#include "KDInput.h"
#include "KGCRoomManager.h"
#include "QuestGameManager.h"
#include "GCDropItemManager.h"
#include "EveryTriggerManager.h"
#include "KGCEmbarkManager.h"
#include "MapSystem/GCWallManager.h"
#include "KGCInfinityDungeonManager.h"

//////////////////////////////////////////////////////////////////////////
// 이 코드에 등장하는 수치들은 모두 체험적인 값들입니다.
// 값을 변경하시려거든 

#define ITEM_GRAVITY  0.0015f
#define ITEM_Y_VELOCITY 0.03f

std::map< GCITEM_KIND, std::string > KGCDropItem::m_mapItemKindColor;

KGCDropItem::KGCDropItem(void)
: m_iTriggerID ( -1)
, m_dwDropUID ( 0)
, m_dwUserUID ( 0)
, m_iOwnerIndex ( -1)
, m_cDropType ( -1)
, m_dwGoodsID ( 0)
, m_iStageCountType( -1 )
, m_fx ( 0.0f)
, m_fy ( 0.0f)
, m_fMaxLeftX( 1.0f )
, m_fMaxRightX( 1.0f )
, m_fXspeed ( 0.0f)
, m_fYspeed ( 0.0f)
, m_fXrotation ( 0.0f)
, m_fYrotation ( 0.0f)
, m_fZrotation ( 0.0f)
, m_fXscale ( 1.0f)
, m_fYscale ( 1.0f)
, m_fZScale ( 1.0f )
, m_fXtrans ( 0.0f)
, m_fYtrans ( 0.056f)
, m_iTime ( 0)
, m_pTexture ( NULL)
, m_pParticle ( NULL)
, m_iState ( IS_NONE)
, m_cAlpha ( 0xff)
, m_bAddMatrixCamera( true )
, m_pBoxMotion( NULL )
, m_pBoxObject( NULL )
, m_dwCurrentFrame( 0 )
, m_fGettingUpSpeed( 0.4f )
, m_dwPlusFrame( 0 )
, m_fGettingUpValocity( 0.0f )
, m_fGettingUpGravity( 0.0f )
, m_fReverseSpeed( 1.7f )
, m_fGravity( 0.0015f )
, m_vParticleTracePos( 0.0f, 0.0f, 0.0f)
{

}

KGCDropItem::~KGCDropItem(void)
{
    SAFE_DEVICE_RELEASE( m_pTexture );

    SAFE_RELEASE( m_pBoxMotion );
    if( m_pBoxObject )
    {
        SiKGCRenderManager()->RemoveObject( m_pBoxObject );
        SAFE_DELETE( m_pBoxObject );
    }
}

void KGCDropItem::FrameMove()
{
    switch( m_iState )
    {
    case IS_DROP:    Drop_FrameMove();    break;
    case IS_GETTING: Getting_FrameMove(); break;
    case IS_GOT:     Got_FrameMove();     break;
	case IS_GOBOX:	 GoBox_FrameMove();	  break;
    default: break;
    }
    
    m_vParticleTracePos = D3DXVECTOR3(  m_fx + m_fXtrans, m_fy +m_fYtrans, 0.3f );
}

void KGCDropItem::Render()
{
    D3DXMATRIX    mat, mat1, mat2, mat3;

    if( m_iState == IS_NONE || m_iState == IS_GOBOX_READY || m_iState == IS_END )
    {
        m_pBoxObject->SetRender( false );
        return;
    }
    
    m_pBoxObject->SetRender( true );

    m_dwCurrentFrame += m_dwPlusFrame + 1;
    if(m_dwCurrentFrame >= static_cast<DWORD>(m_pBoxMotion->GetNum_Frame()) )
        m_dwCurrentFrame = 0;

    SFrame* pFrame = &m_pBoxMotion->GetFrameData( m_dwCurrentFrame);
    m_pBoxObject->SetFrame( pFrame );



    if( m_pTexture == NULL )
    {
        m_pTexture = GetTexture();
    }

    if( m_pTexture->GetDeviceState() == GCDS_NOT_LOADED )
        m_pTexture->AddRefCount();

    if( m_pTexture == NULL )
        return;

//    g_pGCDeviceManager2->PushState();
	//위치 계산.
    {
		KSafeArray<SLVERTEX,4> v;
		v[0].SetSLVER( -0.5,     0.5f,     0,       0,      0,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[1].SetSLVER(  0.5f,    0.5f,     0,    1.0f,      0,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[2].SetSLVER( -0.5,    -0.5,      0,       0,    1.0f,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );
		v[3].SetSLVER(  0.5f,   -0.5,      0,    1.0f,    1.0f,	D3DCOLOR_ARGB( m_cAlpha, 0xff, 0xff, 0xff ) );		

        D3DXMatrixIdentity( &mat1 );
        D3DXMatrixRotationYawPitchRoll( &mat3, m_fYrotation, m_fXrotation, m_fZrotation );
        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
        D3DXMatrixScaling( &mat3, m_fXscale, m_fYscale, m_fZScale );
        D3DXMatrixMultiply( &mat1, &mat1, &mat3 );
        D3DXMatrixTranslation( &mat2, m_fx + m_fXtrans, m_fy + m_fYtrans + pFrame->Pos_y, 0.35f );
        D3DXMatrixMultiply( &mat1, &mat1, &mat2 );
        m_pBoxObject->SetWorldMat( &mat1 );

		//if( m_bAddMatrixCamera == true )
		//	D3DXMatrixMultiply( &mat1, &mat1, &g_kCamera.m_matCamera );        

		//g_pGCDeviceManager2->DrawInWorld(m_pTexture,&mat1,v.as_array());

    }
    //g_pGCDeviceManager2->PopState();


#if !defined( __PATH__ ) // 아이템 드랍 영역
    {
        if( g_kGlobalValue.m_bRenderBoundBox )
        {
            float fX = m_fx + m_fXtrans;
            float fY = m_fy + m_fYtrans;

            GCCollisionRect<float> kItemBox;
            kItemBox.SetRect( (fX - 0.1f), (fY + 0.2f), (fX + 0.1f), fY );

            D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
            SETVECTOR3( lefttop, kItemBox.m_Left, kItemBox.m_Top , 1.0f );
            SETVECTOR3( righttop, kItemBox.m_Right, kItemBox.m_Top , 1.0f );
            SETVECTOR3( leftbottom, kItemBox.m_Left, kItemBox.m_Bottom, 1.0f );
            SETVECTOR3( rightbottom, kItemBox.m_Right, kItemBox.m_Bottom, 1.0f );

            g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                lefttop, righttop, leftbottom, rightbottom,0,0,0,
                D3DCOLOR_ARGB(90, 120, 255, 165));
        }
    }
#endif

}

// 부딧히면 1, 안부딧혔으면 0, 에러가 있으면 -1
int KGCDropItem::CheckBoundWithStage()
{
    D3DXVECTOR2 dxvCrashPos;
    if( SiKGCFantasticMap()->CrashCheck( m_fx, m_fy, dxvCrashPos ) == false )
    {
        return 0;
    }
    else
    {
        m_fy = dxvCrashPos.y;
        return 1;
    }
}

// 부딧히면 Index, 안부딧혔으면  -1
int KGCDropItem::CheckBoundWithPlayer()
{
    float fX = m_fx + m_fXtrans;
    float fY = m_fy + m_fYtrans;
    GCCollisionRect<float> kItemBox, kColl;
    kItemBox.SetRect( (fX - 0.1f), (fY + 0.3f), (fX + 0.1f), (fY + 0.2f) );

    // 부딧히면 Index, 안부딧혔으면  -1
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.bLive == false )
            continue;

        GCCollisionRect<float> kRect = g_MyD3D->MyPlayer[iLoop]->GetCollisionRect();
        if( UINT_MAX  > g_MyD3D->MyPlayer[iLoop]->GetEmbarkUID() )
        {
            KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( g_MyD3D->MyPlayer[iLoop]->GetEmbarkUID() );
            if( pEmbark )
                kRect = pEmbark->GetPushRect();
        }

        if( kItemBox.CheckCollision( kRect, &kColl ) )
        {
            return iLoop;
        }
    }

    return -1;
}

//성공하면 1, 실패하면 0;
int KGCDropItem::SetParticleToItem( char* strParticleName_ )
{
    RemoveParticleFromItem();
    m_pParticle = g_ParticleManager->CreateSequence( strParticleName_ , m_fx + m_fXtrans, m_fy +m_fYtrans, 0.3f );
    m_vParticleTracePos = D3DXVECTOR3(  m_fx + m_fXtrans, m_fy +m_fYtrans, 0.3f );

    TRACE_INFO stTrace;
    if (m_pParticle != NULL){
        stTrace.m_pvPos = &m_vParticleTracePos;
        stTrace.m_fParticleTraceTime = 5.0f;
        m_pParticle->SetTrace( &stTrace );
    }
    

    return 1;
}

int KGCDropItem::RemoveParticleFromItem()
{
    if( m_pParticle == NULL )
        return 1;

    g_ParticleManager->DeleteSequence( m_pParticle );
    m_pParticle = NULL;
    return 1;
}   
//

int KGCDropItem::PlayerGetDropedItem( int iPlayerIndex_ )
{
	if( false ==  g_MyD3D->IsPlayerIndex(iPlayerIndex_ ) || 
        false == g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.bLive)
	{
		ASSERT( "먹는 유저가 올바르지 않습니다." );
		return 0;
	}

    if( g_kGlobalValue.m_kUserInfo.bHost == true )
    {
        KGC_PID_PLAYERGETITEM packet;
        packet.Type = GC_PID_PLAYERGETITEM;
        packet.m_dwDropUID = m_dwDropUID;
        packet.m_cDropTpye = m_cDropType;
        packet.m_dwPlayerUID = m_dwUserUID;
        m_iState = IS_END;
        RemoveParticleFromItem();

        SiKGCDropItemManager()->PushGotItem( packet.m_dwPlayerUID, packet.m_dwDropUID, packet.m_cDropTpye );
        SiKGCDropItemManager()->SetGPListForSend(packet);            

        return 1;
    }
    return 0;
    // 내가 방장이면 먹었다고 판단하고, 아니면 방장의 판정을 기다리자.
}

bool KGCDropItem::PlayerGetItem( DWORD dwUserUID_ )
{
    //이 플레이어가 아이템을 먹었으니 얘 머리위에 이펙트를 띄우자.
    m_iTime = 0;
    m_iState = IS_GETTING;
    RemoveParticleFromItem();
    KGCPC_MEMO( "Drop item get!!" );
    return true;
}

GCDeviceTexture* KGCDropItem::GetTexture()
{
    return g_pGCDeviceManager2->CreateTexture( "흰 텍스쳐를 로드합니다." );
}


void KGCDropItem::InitItemKindColor()
{
	typedef std::map< GCITEM_KIND, std::string >::value_type DIKC_Type;	//DropItemKindColor_Type
	m_mapItemKindColor.clear();
	m_mapItemKindColor.insert( DIKC_Type(GIK_NORMAL		            ,"item_cover_white.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_CERTIFICATE            ,"item_cover_white.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_MAGIC_PAPER		    ,"item_cover_blue.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_STRONG_ITEM            ,"item_cover_yellow.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_STRONG_ITEM_NOT_STRONG ,"item_cover_yellow.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_PRINT_UID              ,"item_cover_blue.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_MISSION_SCROLL			,"item_cover_orange.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_PET					,"item_cover_white.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_POINT_ITEM				,"item_cover_white.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_ENCHANT_ASIST_ITEM     ,"item_cover_violet.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_ENCHANT_ENABLE_ITEM    ,"item_cover_violet.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_ENCHANT_CORE           ,"item_cover_violet.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_RECIPE                 ,"item_cover_violet.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_STUFF                  ,"item_cover_green.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_BOSS_RARE              ,"item_cover_yellow.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_EQUIP                  ,"item_cover_blue.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_EQUIP_RARE             ,"item_cover_yellow.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_MISSION_STUFF          ,"item_cover_orange.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_SHORTCUT_SLOT			,"item_cover_white.dds"));
	m_mapItemKindColor.insert( DIKC_Type(GIK_TREASUREBOX_ITEM		,"item_cover_yellow.dds"));
}

void KGCDropItem::ClearItemKindColor()
{
	m_mapItemKindColor.clear();
}

std::string& KGCDropItem::GetItemKindColor( GCITEM_KIND eKind )
{
	return m_mapItemKindColor[eKind];
}

KGCDropItem_ITEM::KGCDropItem_ITEM(void)
{
    m_fXrotation = m_fYrotation = m_fZrotation = 0.0f;
    m_iTime = 0;
    m_fy += 0.15f;
    m_fYspeed = ITEM_Y_VELOCITY;
    m_iState = IS_DROP;
    float fx_rand = (float)m_kRandom.rand();
    fx_rand = (float)( fx_rand - 50 ) / 4000.0f;
    m_fYspeed += fx_rand / 20.0f;
    m_fXspeed = fx_rand;
}

void KGCDropItem_ITEM::Drop_FrameMove()
{
	m_bAddMatrixCamera = true;
    if( SiKGCFantasticMap()->GetWorldWidth() < m_fx + m_fXspeed ||
        0 > m_fx + m_fXspeed )
    {
        m_fXspeed *= -1.0f;
    }
    m_fx += m_fXspeed;    m_fy += m_fYspeed;

    if( m_fYspeed < 0.0f && CheckBoundWithStage() == 1 )
    {
        if( m_fYspeed > -0.0000001f )
        {
            m_fYspeed = m_fXspeed = 0.0f;
        }
        else
        {
            m_fYspeed /= -m_fReverseSpeed;
            m_fXspeed /= m_fReverseSpeed;
        }
    }
    else
    {
        m_fYspeed -= m_fGravity;
    }

    // 블럭이 있으면 블럭안으로 아이템 넣어주자.
    SiKGCWallManager()->CheckInWallBound( m_fx );

	static const float fMoveToSafeRange = 0.01f;
	//아이템의 영역을 체크해서 영역안으로 끌고 옵시다.
	int iMaxRangeResult = CheckBoundWithMaxXRange();
	if( (m_fXspeed == 0 ) && 
		(m_fYspeed == 0 ) &&
		(iMaxRangeResult != 0 ) )
	{
		m_fx -= (fMoveToSafeRange * iMaxRangeResult);
		D3DXVECTOR2 vPos = SiKGCFantasticMap()->GetPositionNear( m_fx , m_fy );
		m_fy = vPos.y;
	}

    CheckBoundWithSettingRect();

    ++m_iTime;
    int iOwner = CheckBoundWithPlayer();
    if( m_iTime >= 50 && iOwner != -1 )
    {
        PlayerGetDropedItem( iOwner );
    }
}

void KGCDropItem::CheckBoundWithSettingRect()
{
    float fMinPos = 999999.0f;
    D3DXVECTOR2 vItemCenter, vItemDropRectCenter;
    std::vector<D3DXVECTOR4> vecItemDropRect;
    GCCollisionRect<float> Collision, ItemRect, ItemDropRect, MinDropRect;
    SiKGCDropItemManager()->GetItemDropRect( SiKGCRoomManager()->GetGameMode(), g_kEveryTriggerManager.GetActiveStage(), vecItemDropRect);

    std::vector<D3DXVECTOR4>::iterator vecIter = vecItemDropRect.begin();
    for(; vecIter != vecItemDropRect.end(); ++vecIter)
    {
        float fX = m_fx + m_fXtrans - 0.1f;
        float fY = m_fy + m_fYtrans + 0.1f;
        ItemRect.SetRect(fX, fY + 0.056f, (fX + 0.2f), (fY - 0.1f) );
        ItemDropRect.SetRect(vecIter->x, vecIter->y, vecIter->z, vecIter->w);
        
        if( false == ItemDropRect.CheckCollision( ItemRect, &Collision ) )
        {
            float fScalar;
            ItemRect.GetCenter(vItemCenter.x, vItemCenter.y);
            ItemDropRect.GetCenter(vItemDropRectCenter.x, vItemDropRectCenter.y);

            // 벡터크기 처리
            D3DXVECTOR2 vLength = vItemCenter - vItemDropRectCenter;
            fScalar = D3DXVec2Length( &vLength );

            // 최소거리 저장
            if( fMinPos > fScalar )
            {
                fMinPos = fScalar;
                MinDropRect = ItemDropRect;
            }
        }
        else
        {
            fMinPos = 999999.0f;
            break;
        }
    }

    if( 999999.0f != fMinPos )
    {
        float fX = m_fx + m_fXtrans;
        float fY = m_fy + m_fYtrans + 0.056f;

        if( fX < MinDropRect.m_Left )
        {
            // 0.1f : Left보다 0.1만큼 안쪽으로
            m_fx = (MinDropRect.m_Left + 0.1f - m_fXtrans);
        }
        if( fX > MinDropRect.m_Right )
        {
            // 0.1f : Right보다 0.1만큼 안쪽으로
            m_fx = (MinDropRect.m_Right - 0.1f - m_fXtrans);
        }
        if( fY > MinDropRect.m_Top )
        {
            m_fy = (MinDropRect.m_Top - m_fYtrans - 0.056f);
            m_fYspeed = 0.0;
        }
        if( fY < MinDropRect.m_Bottom )
        {
            m_fy = (MinDropRect.m_Bottom - m_fYtrans - 0.056f);
            m_fYspeed = ITEM_Y_VELOCITY;
        }
        m_fXspeed = 0.0f;
    }
}

bool KGCDropItem::LineCollision(D3DXVECTOR2 &vPos1, D3DXVECTOR2 &vPos2, D3DXVECTOR2 &vPos3, D3DXVECTOR2 &vPos4, D3DXVECTOR2 &vCrossPos)
{
    float fUnder = (vPos2.x-vPos1.x) * (vPos4.y-vPos3.y) - (vPos2.y-vPos1.y) * (vPos4.x-vPos3.x);
    if( 0 == fUnder )
        return false;

    float fFrontCross = (vPos1.y-vPos3.y) * (vPos4.x-vPos3.x) - (vPos1.x-vPos3.x) * (vPos4.y-vPos3.y);
    float fBackCross = (vPos2.x-vPos1.x) * (vPos1.y-vPos3.y) - (vPos2.y-vPos1.y) * (vPos1.x-vPos3.x);
    if( 0 == fFrontCross && 0 == fBackCross )
        return false;

    fFrontCross = fFrontCross / fUnder;
    fBackCross = fBackCross / fUnder;
    if( ((0.0f > fFrontCross) || (fFrontCross > 1.0f)) || ((0.0f > fBackCross) || (fBackCross > 1.0f)) )
        return false;

    vCrossPos.x = vPos1.x + fFrontCross * (vPos2.x - vPos1.x);
    vCrossPos.y = vPos1.y + fFrontCross * (vPos2.y - vPos1.y);
    return true;
}

void KGCDropItem_ITEM::Getting_FrameMove()
{
	m_bAddMatrixCamera = true;

	m_iTime++;
	if( m_iTime < 25 )
	{
        m_fGettingUpValocity += m_fGettingUpGravity;
        if(m_fGettingUpValocity < 0.0f )
            m_fGettingUpValocity = 0.0f;

		m_fy += m_fGettingUpSpeed + m_fGettingUpValocity;
	}
	else
	{
        m_iTime = 0;
        m_iState = IS_END;
        RemoveParticleFromItem();
	}

}

void KGCDropItem_ITEM::Got_FrameMove()
{
	m_cAlpha = 0xFF;
	m_bAddMatrixCamera = true;

    float fPlayerPosX = g_MyD3D->MyPlayer[m_iOwnerIndex]->vPos.x;
    float fPlayerPosY = g_MyD3D->MyPlayer[m_iOwnerIndex]->vPos.y;

    if( m_iTime < 10 )
    {
        m_fx = fPlayerPosX;
        m_fy = fPlayerPosY + 0.3f + ( (float)m_iTime / 100.0f );
        
    }
    else
    {
        //m_fXscale = 0.3f;
        //m_fYscale = 0.3f;
    }

    if( m_iTime == 50 )
    {
        m_iTime = 0;
        m_iState = IS_END;
        RemoveParticleFromItem();
    }

    ++m_iTime;
}

bool KGCDropItem_ITEM::DropItem( D3DXVECTOR2 Pos )
{
	if( g_pkQuestManager->GetNumOfClearStage() != m_iStageCountType && m_iStageCountType != -1 )
		return false;

    if( KGameDropElement::RT_GP != m_cDropType && -1 == GetPlayerIndexFromUID( m_dwUserUID ) )
        return false;

    if( KGameDropElement::RT_GP != m_cDropType )
        g_KDSound.Play( "991026" );

    GCItem* pItem = g_pItemMgr->GetItemData( m_dwGoodsID );
    
    if( pItem == NULL ) { 
        return false;
    }
    
    if( GIK_MISSION_SCROLL == pItem->eItemKind || GIK_RECIPE == pItem->eItemKind )
    {
        SetParticleToItem( "Mission_Scroll_01" );
    }

    m_fx = Pos.x;
    m_fy = Pos.y;
    m_fXrotation = m_fYrotation = m_fZrotation = 0.0f;
    m_iTime = 0;
    m_fy += 0.15f;
    m_iState = IS_DROP;

	//X축 속도는 아이템 드랍 가능 영역체크해서 영역 밖이면 영역 안쪽으로 속도를 줍니다.
	int iMaxRangeResult = CheckBoundWithMaxXRange();
	if( ( iMaxRangeResult != 0 ) &&
		(  (iMaxRangeResult == 1 && m_fXspeed < 0.0f) ||
		(iMaxRangeResult == -1 && m_fXspeed > 0.0f )  ) )
	{
		m_fXspeed *= -1.0f;
	}

	return true;
}

GCDeviceTexture* KGCDropItem_ITEM::GetTexture()
{
    char TextureID[30];

	int dwGoodsID = g_pItemMgr->GetSameImageIndex(m_dwGoodsID);

    sprintf( TextureID, "Sbta%03d.dds", dwGoodsID );
    return g_pGCDeviceManager2->CreateTexture( TextureID );
}

void KGCDropItem_ITEM::Render()
{
	KGCDropItem::Render();
}

template< class T >
T GetPowValue( T StartValue, T EndValue, int iFrame, int iTotalFrame = 30, double dPow = 0.5 )
{

	T tStart = StartValue;
	T tEnd = EndValue;

	double first = (double)iFrame/(double)iTotalFrame;
	long double a = pow( first, dPow );
	T tDelta = (T)((double)(tEnd - tStart) * a);

	return tStart + tDelta;
}

#define GO_BOX_FRAME_NUM   30
void KGCDropItem_ITEM::GoBox_FrameMove()
{
	m_cAlpha = 0xFF;
	m_bAddMatrixCamera = false;

//#if defined( _DEBUG )
	static bool bFirst = true;
	static std::vector<D3DXVECTOR2> vecItemPos;
	static std::vector<float> vecScale;

	static float startx = 0.3074f;
	static float starty = 0.125f;
	static float endx = 0.1994f;
	static float endy = -0.0489f;
	static float startscale = 1.1652f;
	static float endscale = 0.5594f;
	static float fpow = 1.65f;

	if( bFirst )
	{
		vecItemPos.clear();
		vecScale.clear();
		for( int i = 0 ; i < GO_BOX_FRAME_NUM ; ++i )
		{
			D3DXVECTOR2 vPos;

			vPos.x = GetPowValue( startx, endx, i , GO_BOX_FRAME_NUM, fpow);
			vPos.y = GetPowValue( starty, endy, i , GO_BOX_FRAME_NUM, fpow);
			vecScale.push_back( GetPowValue( startscale, endscale, i , GO_BOX_FRAME_NUM, fpow) );
			vecItemPos.push_back( vPos );
		}
		bFirst = false;
	}
//#endif
	m_fx = vecItemPos[m_iTime].x;
	m_fy = vecItemPos[m_iTime].y;
	//m_fXscale = m_fYscale = vecScale[m_iTime];
	
	++m_iTime;
	if( m_iTime >= GO_BOX_FRAME_NUM )
	{
		m_iTime = 0;
		m_iState = IS_END;
	}

	if( GetAsyncKeyState( VK_HOME ) )
	{
		m_fx += 0.005f;
	}
	if( GetAsyncKeyState( VK_END ) )
	{
		m_fx -= 0.005f;
	}
	if( GetAsyncKeyState( VK_INSERT ) )
	{
		m_fy += 0.005f;
	}
	if( GetAsyncKeyState( VK_DELETE ) )
	{
		m_fy -= 0.005f;
	}
}

bool KGCDropItem_ITEM::PlayerGetItem(  DWORD dwUserUID_ )
{
	if( false == KGCDropItem::PlayerGetItem( dwUserUID_ ) )
        return false;

    m_iOwnerIndex = GetPlayerIndexFromUID( dwUserUID_ );
    if( -1 == m_iOwnerIndex)
    {
        ASSERT( !"Item Owner Index Error !!" );
        return false;
    }



    // 방장이 드랍된 아이템과 방에 있는 유저 UID를 서버로 날린다 -> 서버가 누가 먹는가 결정해 준다 
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {
        std::pair<DWORD,DWORD>               prDropList;
        std::pair<DWORD,std::vector<DWORD> > prRouletteList;

        prRouletteList.first = m_dwDropUID;
        for(int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if( g_MyD3D->MyPlayer[i]->IsLive() && g_MyD3D->MyPlayer[i]->m_kUserInfo.iInvenCapacity > g_MyD3D->MyPlayer[i]->m_kUserInfo.iUserItemCount )
                prRouletteList.second.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        }   

        EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();

        if ( SiKGCWorldMapManager()->IsInfinityDungeon( emGameMode ) )
        {
            SiKP2P()->GetInstance()->Send_InfinityDungeonRewardItemReq( m_dwDropUID, dwUserUID_ );
        }
        else
        {
            SiKP2P()->GetInstance()->Send_DungeonRewardItemReq( prDropList, prRouletteList );
        }
    }


    PLAYER* pMyPlayer = g_MyD3D->GetMyPlayer();
    if( pMyPlayer && pMyPlayer->m_kUserInfo.iInvenCapacity <= pMyPlayer->m_kUserInfo.iUserItemCount )
    {
        std::wstring strInvenFullMsg = g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR4);
        if( 1 < g_MyD3D->GetPlayerNum() )
            strInvenFullMsg += L"\n" +  g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR5);

        g_pkChatManager->AddChatMsg( strInvenFullMsg );
    }

	return true;
}

void KGCDropItem_ITEM::SetPacketData( const KGameDropElement& src_ )
{
    KGCDropItem::SetPacketData(src_);
    m_dwGoodsID /= 10;

    m_fGettingUpSpeed = 0.00015f;
    m_fGettingUpValocity = 0.01f;
    m_fGettingUpGravity = -0.0008f;

    m_fXtrans = -1.0f;
    m_fYtrans = -0.38f;

	if (m_pBoxObject!=NULL)
	{
		SAFE_DELETE(m_pBoxObject);
	}
    m_pBoxObject = new GCObject;
    SiKGCRenderManager()->AddObject( m_pBoxObject );

    SAFE_RELEASE( m_pTexture );
    m_pTexture = GetTexture();
    GCItem* pItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwGoodsID );
	if ( NULL == pItem )
	{
		return;
	}
    m_strName = pItem->strItemName;
    m_dwFontColor = g_MyD3D->m_kItemMgr.GetItemStringColor( pItem->eItemKind );

    if( GIK_MONSTER_CARD == pItem->eItemKind)
    {
        std::stringstream text;
        text << "Monster_Card_N_" << std::to_string(pItem->cItemGrade) << ".dds";
        m_pBoxObject->AddMesh( "Monster_Card.p3m", text.str());
        m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Monster_Card.frm");
        m_fXscale = m_fYscale = m_fZScale = 1.2f;
    }
    else if( GIK_MISSION_SCROLL == pItem->eItemKind || GIK_RECIPE == pItem->eItemKind )
    {
        m_pBoxObject->AddMesh( "Mission_Scroll.p3m", "Mission_Scroll.dds");
        m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Mission_Scroll.frm");
    }
    else
    {
        m_fXscale = m_fYscale = m_fZScale = 1.2f;
        if( ESP_WEAPON & pItem->dwSlotPosition)
        {
            std::stringstream text;
            text << "Item_Weapon_N_" << std::to_string(pItem->cItemGrade) << ".dds";
            m_pBoxObject->AddMesh( "drop_item_box.p3m", text.str());
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "drop_item_box.frm");
        }
        else if( ESP_SET_ITEM & pItem->dwSlotPosition ||
                 ESP_HELMET & pItem->dwSlotPosition || 
                 ESP_JACKET & pItem->dwSlotPosition || 
                 ESP_PANTS & pItem->dwSlotPosition || 
                 ESP_GLOVES & pItem->dwSlotPosition || 
                 ESP_SHOES & pItem->dwSlotPosition || 
                 ESP_MANTLE & pItem->dwSlotPosition )
        {
            std::stringstream text;
            text << "Item_Defence_N_" << std::to_string(pItem->cItemGrade) << ".dds";
            m_pBoxObject->AddMesh( "drop_item_box.p3m", text.str());
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "drop_item_box.frm");
        }
        else if( ESP_A_UPPER_HELMET & pItem->dwSlotPosition ||
                 ESP_A_DOWN_HELMET & pItem->dwSlotPosition ||
                 ESP_A_NECKLET & pItem->dwSlotPosition ||
                 ESP_A_JACKET & pItem->dwSlotPosition ||
                 ESP_A_PANTS & pItem->dwSlotPosition ||
                 ESP_A_ARM & pItem->dwSlotPosition ||
                 ESP_A_FINGER & pItem->dwSlotPosition ||
                 ESP_A_SHOES & pItem->dwSlotPosition ||
                 ESP_A_EARRING & pItem->dwSlotPosition )
        {
            std::stringstream text;
            text << "Item_Accessory_N_" << std::to_string(pItem->cItemGrade) << ".dds";
            m_pBoxObject->AddMesh( "drop_item_box.p3m", text.str());
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "drop_item_box.frm");
        }
        else
        {
            m_pBoxObject->AddMesh( "drop_item_box.p3m", "Item_ETC_N.dds");
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "drop_item_box.frm");
        }
    }
    m_kRandom.SetRange( -200, 200 );
    int iYrand = m_kRandom.rand();
    float fy_rand = static_cast< float >( iYrand / 240000.0f );
    m_fYspeed = ITEM_Y_VELOCITY + ASMfabs(fy_rand);	// 작은 폭의 랜덤성을 줄려고 20으로 나눔. 큰 의미 없음.

    m_kRandom.SetSeed( static_cast< DWORD>( iYrand) );
    m_kRandom.SetRange( -50, 50 );
    int iXrand = m_kRandom.rand();
    float fx_rand = static_cast< float >( iXrand / 4000.0f );
    m_fXspeed = fx_rand;

    m_kRandom.SetRange( 0, 10 );
    m_dwCurrentFrame = static_cast<DWORD>( m_kRandom.rand() * 5 );

}

GCDeviceTexture* KGCDropItem_GP::GetTexture()
{
    return g_pGCDeviceManager2->CreateTexture( "Item06.dds" );
}

bool KGCDropItem_GP::DropItem( D3DXVECTOR2 Pos )
{
	if( KGCDropItem_ITEM::DropItem( Pos ) )
	{
        g_KDSound.Play( "991020");
		return true;
	}
	return true;
}

void KGCDropItem_GP::Getting_FrameMove()
{
    KGCDropItem_ITEM::Getting_FrameMove();
}

void KGCDropItem_GP::SetPacketData( const KGameDropElement& src_ )
{
    KGCDropItem::SetPacketData(src_);

	if (m_pBoxObject!=NULL)
	{
		SAFE_DELETE(m_pBoxObject);
	}


    m_pBoxObject = new GCObject;

    m_kRandom.SetRange( 0, 10 );
    int iRand = m_kRandom.rand();

    float fYSpeedDevide = 240000.0f;
    float fXSpeedDevide = 4000.0f;

    SGPStyle* pGpStyle = SiKGCDropItemManager()->GetGpStyle( m_dwGoodsID );
    if( pGpStyle )
    {
        m_pBoxObject->AddMesh( pGpStyle->strMesh.c_str(), pGpStyle->strTex.c_str() );
        m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( pGpStyle->strMotion.c_str() );

        m_fXscale = pGpStyle->vScale.x;
        m_fYscale = pGpStyle->vScale.y;
        m_fZScale = pGpStyle->vScale.z;

        m_fXtrans = pGpStyle->vTrans.x;
        m_fYtrans = pGpStyle->vTrans.y;

        fXSpeedDevide = pGpStyle->vSpeed.x;
        fYSpeedDevide = pGpStyle->vSpeed.y;

        m_dwCurrentFrame = static_cast< DWORD>( iRand * pGpStyle->iStartFrame );
    }
    else
    {
        if( KGameDropElement::GT_SILVER == m_dwGoodsID )
        {
            m_pBoxObject->AddMesh( "Gp.p3m", "Gp_silver.dds");
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Gp_s.frm");
            m_fXscale = m_fYscale = m_fZScale = .5f;
            m_fYtrans = -0.52f;
            m_dwCurrentFrame = static_cast< DWORD>( iRand * 2 );
        }
        else if( KGameDropElement::GT_GOLD == m_dwGoodsID )
        {
            m_pBoxObject->AddMesh( "Gp.p3m", "Gp_Gold.dds");
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Gp.frm");
            m_fXscale = m_fYscale = m_fZScale = 0.75f;
            m_fYtrans = - 0.5f;
            m_dwCurrentFrame = static_cast< DWORD>( iRand * 5 );
        }
        else if( KGameDropElement::GT_JP_SILVER == m_dwGoodsID )
        {
            m_pBoxObject->AddMesh( "Gp.p3m", "Gp_silver.dds");
            m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Gp_s.frm");
            m_fXscale = m_fYscale = m_fZScale = .5f;
            m_fYtrans = -0.52f;
            m_dwCurrentFrame = static_cast< DWORD>( iRand * 2 );

            fYSpeedDevide = 18000.0f;
            fXSpeedDevide = 3000.0f;
        }
    }

    m_fGettingUpSpeed = 0.003f;
    m_fXtrans = -1.0f;

    m_kRandom.SetRange( -200, 200 );
    int iYrand = m_kRandom.rand();
    float fy_rand = static_cast< float >( iYrand / fYSpeedDevide );
    m_fYspeed = ITEM_Y_VELOCITY + ASMfabs(fy_rand);	// 작은 폭의 랜덤성을 줄려고 20으로 나눔. 큰 의미 없음.

    m_kRandom.SetSeed( static_cast< DWORD>( iYrand ) );
    m_kRandom.SetRange( -50, 50 );
    int iXrand = m_kRandom.rand();
    float fx_rand = static_cast< float >( iXrand / fXSpeedDevide );
    m_fXspeed = fx_rand;

    SiKGCRenderManager()->AddObject( m_pBoxObject );
}

bool KGCDropItem_GP::PlayerGetItem( DWORD dwUserUID_ )
{
    if( false == KGCDropItem::PlayerGetItem( dwUserUID_ ) )
        return false;

    // 누군가 gp를 ㅊ묵ㅊ묵 하면 저장해놓는다!!
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {
        if ( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            g_pkQuestManager->m_setGpUID.insert( m_dwDropUID );

            if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
            {
                if ( g_pkQuestManager->m_setGpUID.size() > KGCInfinityDungeonManager::MAX_GP_REWARD_NUM )
                {
                    SiKP2P()->GetInstance()->Send_InfinityDungeonRewardGPNot( g_pkQuestManager->m_setGpUID );
                    g_pkQuestManager->m_setGpUID.clear();
                }
            }
        }
    }

    g_ParticleManager->CreateSequence( "Get_GP_Effect_01", m_fx + m_fXtrans, m_fy + m_fYtrans + 0.12f, 0.3f);
    g_ParticleManager->CreateSequence( "Get_GP_Effect_02", m_fx + m_fXtrans, m_fy + m_fYtrans + 0.12f, 0.3f);
    
    if( KGameDropElement::GT_SILVER == m_dwGoodsID || KGameDropElement::GT_JP_SILVER == m_dwGoodsID)
    {
        m_dwPlusFrame = 2;
    }
    else if( KGameDropElement::GT_GOLD == m_dwGoodsID )
    {
        m_dwPlusFrame = 4;
    }
    
    g_KDSound.Play( "991023" );
    return true;
}

bool KGCDropItem_EXP::DropItem( D3DXVECTOR2 Pos )
{
    PlayerGetDropedItem( g_kGlobalValue.m_kUserInfo.dwUID );
	return true;
}

void KGCDropItem_EXP::Getting_FrameMove()
{
	m_bAddMatrixCamera = true;
    m_iState = IS_END;
    RemoveParticleFromItem();
}

void KGCDropItem_EXP::SetPacketData( const KGameDropElement& src_ )
{
    KGCDropItem::SetPacketData(src_);
}

void KGCDropItem::SetPacketData( const KGameDropElement& src_ )
{
    m_dwDropUID     =   src_.m_dwUID;
    m_dwUserUID     =   src_.m_dwUserUID;
    m_iTriggerID    =   src_.m_nTriggerID;
    m_iMonID        =   src_.m_nMonID;
    m_cDropType     =   src_.m_cRewardType;
    m_dwGoodsID     =   src_.m_dwItemID;
	m_iStageCountType = src_.m_nStageCountType;
    m_iState        =   IS_NONE;
}

int KGCDropItem::GetPlayerIndexFromUID( DWORD dwUID )
{
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->IsLive() && g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUID )
			return i;
	}
	return -1;
}

bool KGCDropItem::IsRareItem()
{
	GCItem* pItem = g_pItemMgr->GetItemData( m_dwGoodsID );
	if( pItem->cItemGrade )
		return true;
	return false;
}

// 아이템이 움직일 수 있는 범위를 체크해 줍니다.
// 1은 오른쪽, -1은 왼쪽으로 영역을 초과 한 것이고, 0은 적절한 영역
int KGCDropItem::CheckBoundWithMaxXRange()
{
	// 두 값이 같으면 값을 셋팅 안한것으로 간주합니다.
	if( m_fMaxLeftX == m_fMaxRightX )
		return 0;

	if( m_fx < m_fMaxLeftX )
		return -1;

	if( m_fx > m_fMaxRightX )
		return 1;

	return 0;
}

void KGCDropItem_MISSION::Drop_FrameMove()
{
	PlayerGetDropedItem( GetPlayerIndexFromUID(m_dwUserUID) );
}

void KGCDropItem_MISSION::Getting_FrameMove()
{
	m_iState = IS_GOT;
    int iPlayerIndex = GetPlayerIndexFromUID(m_dwUserUID);
	if( g_MyD3D->IsPlayerIndex(iPlayerIndex) && g_MyD3D->MyPlayer[ iPlayerIndex ]->IsLocalPlayer() == true )
		g_kGlobalValue.m_kUserMission.VirtualGetMissionItem( m_dwGoodsID );
}

void KGCDropItem_MISSION::Render()
{
	if( m_iOwnerIndex < 0 ||
		m_iOwnerIndex >= MAX_PLAYER_NUM ||
		g_MyD3D->MyPlayer[m_iOwnerIndex]->IsLocalPlayer() == false ) //로컬 플레이어만 보임
		return;

	KGCDropItem_ITEM::Render();
}

bool KGCDropItem_MISSION::DropItem( D3DXVECTOR2 Pos )
{
	return KGCDropItem_ITEM::DropItem( Pos );
}

bool KGCDropItem_MISSION::PlayerGetItem(  DWORD dwUserUID_ )
{
    if( false == KGCDropItem::PlayerGetItem( dwUserUID_ ) )
        return false;

    m_iOwnerIndex = GetPlayerIndexFromUID( dwUserUID_ );
    if( -1 == m_iOwnerIndex)
    {
        ASSERT( !"Item Owner Index Error !!" );
        return false;
    }

    // 누가 아이템 ㅊ묵ㅊ묵했는지 서버에게 알리자! ( 방장만 )
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {
        std::pair<DWORD,std::vector<DWORD> > prRouletteList;

        EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();

        if ( SiKGCWorldMapManager()->IsInfinityDungeon( emGameMode ) )
        {
            SiKP2P()->GetInstance()->Send_InfinityDungeonRewardItemReq( m_dwDropUID, dwUserUID_ );
        }
        else
        {
            SiKP2P()->GetInstance()->Send_DungeonRewardItemReq( std::make_pair( m_dwDropUID ,dwUserUID_ ), prRouletteList );
        }
    }

    return true;
}

void KGCDropItem_MISSION::SetPacketData( const KGameDropElement& src_ )
{
    KGCDropItem::SetPacketData(src_);
    m_dwGoodsID /= 10;
    m_fXtrans = -1.0f;
    m_fYtrans = -0.38f;

	if (m_pBoxObject!=NULL)
	{
		SAFE_DELETE(m_pBoxObject);
	}
    m_pBoxObject = new GCObject;
    m_pBoxObject->AddMesh( "Mission_Scroll.p3m", "Mission_Scroll.dds");
    m_pBoxMotion = g_pGCDeviceManager2->CreateMotionFRM( "Mission_Scroll.frm");
    SiKGCRenderManager()->AddObject( m_pBoxObject );

    SAFE_RELEASE( m_pTexture );
    m_pTexture = GetTexture();
    GCItem* pItem = g_MyD3D->m_kItemMgr.GetItemData( m_dwGoodsID );
    m_strName = pItem->strItemName;
    m_dwFontColor = g_MyD3D->m_kItemMgr.GetItemStringColor( pItem->eItemKind );
	m_pBoxObject->SetRender(false);
}
