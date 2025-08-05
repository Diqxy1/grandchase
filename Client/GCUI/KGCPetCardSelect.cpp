#include "stdafx.h"
#include ".\KGCPetCardSelect.h"

//

//
//
//

#include "MyD3D.h"

IMPLEMENT_CLASSNAME( KGCPetCardSelect );
IMPLEMENT_WND_FACTORY( KGCPetCardSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCPetCardSelect, "gc_pet_card_select" );

KGCPetCardSelect::KGCPetCardSelect(void)
{
    m_pkLeft = NULL;
    m_pkRight = NULL;
    m_pkPetName = NULL;

    LINK_CONTROL( "pet_change_left", m_pkLeft );
    LINK_CONTROL( "pet_change_right", m_pkRight );
    LINK_CONTROL( "pet_name_static", m_pkPetName );

    m_pTexturePet = NULL;
    m_pTextureLeftPet = NULL;
    m_pTextureRightPet = NULL;
    
    m_iPetGoodsID = -1;
    m_iPetEvolutionLevel = 0;
    m_iLeftPetGoodsID = -1;
    m_iLeftPetEvolutionLevel = 0;
    m_iRightPetGoodsID = -1;
    m_iRightPetEvolutionLevel = 0;
}

KGCPetCardSelect::~KGCPetCardSelect(void)
{
    Release();
}

void KGCPetCardSelect::OnCreate( void )
{
    m_pkLeft->InitState( true, true, this );
    m_pkRight->InitState( true, true, this );
    m_pkPetName->InitState( true );

    m_pkPetName->SetAlign( DT_CENTER );

    m_pkLeft->SetHotKey( DIK_LEFT );
    m_pkRight->SetHotKey( DIK_RIGHT );
}

void KGCPetCardSelect::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkLeft )
        {
            SpeakToActionListener( KActionEvent( this, CHANGE_PET_CARD_LEFT ) );
            g_KDSound.Play( "31" );
        }
        else if( event.m_pWnd == m_pkRight )
        {
            SpeakToActionListener( KActionEvent( this, CHANGE_PET_CARD_RIGHT ) );
            g_KDSound.Play( "31" );
        }
    }
}

void KGCPetCardSelect::PostChildDraw( void )
{	
    // 현재 선택된 펫카드
    if ( m_pTexturePet != NULL && IsRenderOn() && m_iPetGoodsID != -1 )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        /*m_pTexturePet->SetDeviceTexture();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            float fX = 74.0f;
            float fY = -1.0f;
            float fWidth = 114.0f;
            float fHeight = 115.0f;

            g_MyD3D->Draw_TL( vPos.x + fX, vPos.y + fY,
                vPos.x + fX + fWidth, vPos.y + fY + fHeight,
                0.0f, 0.0f, 1.0f, 1.0f );
        }
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/
		float fX = 74.0f * m_fWindowScaleX;
		float fY = -1.0f * m_fWindowScaleY;
		float fWidth = 114.0f * m_fWindowScaleX;
		float fHeight = 115.0f * m_fWindowScaleY;
		g_pGCDeviceManager2->DrawInScreen(m_pTexturePet,
			vPos.x + fX, vPos.y + fY,
			vPos.x + fX + fWidth, vPos.y + fY + fHeight,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
    }

    // 왼쪽 펫카드
    if ( m_pTextureLeftPet != NULL && IsRenderOn() && m_iLeftPetGoodsID != -1 )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        /*m_pTextureLeftPet->SetDeviceTexture();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            float fX = 1.0f;
            float fY = 16.0f;
            float fWidth = 90.0f;
            float fHeight = 90.0f;

            g_MyD3D->Draw_TL( vPos.x + fX, vPos.y + fY,
                vPos.x + fX + fWidth, vPos.y + fY + fHeight,
                0.0f, 0.0f, 1.0f, 1.0f );
        }
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/

		float fX = 1.0f * m_fWindowScaleX;
		float fY = 16.0f * m_fWindowScaleY;
		float fWidth = 90.0f * m_fWindowScaleX;
		float fHeight = 90.0f * m_fWindowScaleY;
		g_pGCDeviceManager2->DrawInScreen(m_pTextureLeftPet,
			vPos.x + fX, vPos.y + fY,
			vPos.x + fX + fWidth, vPos.y + fY + fHeight,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);

    }

    // 오른쪽 펫카드
    if ( m_pTextureRightPet != NULL && IsRenderOn() && m_iRightPetGoodsID != -1 )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );


      /*  m_pTextureRightPet->SetDeviceTexture();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            float fX = 170.0f;
            float fY = 16.0f;
            float fWidth = 90.0f;
            float fHeight = 90.0f;

            g_MyD3D->Draw_TL( vPos.x + fX, vPos.y + fY,
                vPos.x + fX + fWidth, vPos.y + fY + fHeight,
                0.0f, 0.0f, 1.0f, 1.0f );
        }
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/

		float fX = 170.0f * m_fWindowScaleX;
		float fY = 16.0f * m_fWindowScaleY;
		float fWidth = 90.0f * m_fWindowScaleX;
		float fHeight = 90.0f * m_fWindowScaleY;
		g_pGCDeviceManager2->DrawInScreen(m_pTextureRightPet,
			vPos.x + fX, vPos.y + fY,
			vPos.x + fX + fWidth, vPos.y + fY + fHeight,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
    }
}

void KGCPetCardSelect::SetCard( GCITEMUID dwPetUID, GCITEMUID dwLeftPetUID, GCITEMUID dwRightPetUID )
{
    SAFE_RELEASE( m_pTexturePet );
    SAFE_RELEASE( m_pTextureLeftPet );
    SAFE_RELEASE( m_pTextureRightPet );

    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( dwPetUID );
    std::map<GCITEMUID, KPetInfo>::iterator mitleft = g_MyD3D->m_mapPetInfo.find( dwLeftPetUID );
    std::map<GCITEMUID, KPetInfo>::iterator mitright = g_MyD3D->m_mapPetInfo.find( dwRightPetUID );

    // 현재 펫의 정보 설정
    KPetInfo kPetInfo = mit->second;
    m_dwPetUID = kPetInfo.m_dwUID;
    m_iPetGoodsID = (int)kPetInfo.m_dwID;
    m_iPetEvolutionLevel = kPetInfo.m_cPromotion;
    m_pTexturePet = g_pItemMgr->CreatePetItemTexture( m_iPetGoodsID, m_iPetEvolutionLevel );

    if( dwLeftPetUID != dwRightPetUID && dwLeftPetUID != dwPetUID && g_MyD3D->m_mapPetInfo.end() != mitleft )
    {
        // 왼쪽 펫카드
        KPetInfo kLeftPetInfo = mitleft->second;
        m_iLeftPetGoodsID = (int)kLeftPetInfo.m_dwID;
        m_iLeftPetEvolutionLevel = kLeftPetInfo.m_cPromotion;
        m_pTextureLeftPet = g_pItemMgr->CreatePetItemTexture( m_iLeftPetGoodsID, m_iLeftPetEvolutionLevel );
    }

    if( dwRightPetUID != dwPetUID && g_MyD3D->m_mapPetInfo.end() != mitright )
    {
        // 오른쪽 펫카드
        KPetInfo kRightPetInfo = mitright->second;
        m_iRightPetGoodsID = (int)kRightPetInfo.m_dwID;
        m_iRightPetEvolutionLevel = kRightPetInfo.m_cPromotion;
        m_pTextureRightPet = g_pItemMgr->CreatePetItemTexture( m_iRightPetGoodsID, m_iRightPetEvolutionLevel );
    }

    // 펫의 레벨과 이름을 표시해 준다
    std::wostringstream strmPetName;
    strmPetName << g_pkStrLoader->GetString( STR_ID_LEVEL ) << " " << kPetInfo.m_iLevel << ". " << kPetInfo.m_strName;
    m_pkPetName->SetText( strmPetName.str() );
}

void KGCPetCardSelect::Release()
{
    m_iPetGoodsID = -1;
    m_iPetEvolutionLevel = 0;
    m_iLeftPetGoodsID = -1;
    m_iLeftPetEvolutionLevel = 0;
    m_iRightPetGoodsID = -1;
    m_iRightPetEvolutionLevel = 0;
    
    SAFE_RELEASE( m_pTexturePet );
    SAFE_RELEASE( m_pTextureLeftPet );
    SAFE_RELEASE( m_pTextureRightPet );
}