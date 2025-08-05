#include "stdafx.h"
#include "KGCEmoticonSlot.h"

#include "../MyD3D.h"

//
//
#include "KGCEmoticon.h"

IMPLEMENT_CLASSNAME( KGCEmoticonSlot );
IMPLEMENT_WND_FACTORY( KGCEmoticonSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCEmoticonSlot, "emoticon_slot" );

KGCEmoticonSlot::KGCEmoticonSlot()
{
	m_MyIndex = 0;

	m_pkFrame = NULL;
	m_pkEmoticon = NULL;
    m_pkCooltime_Pannel = NULL;

    LINK_CONTROL( "frame", m_pkFrame );
	//LINK_CONTROL( "emoticon_pannel", m_pkEmoticon_Pannel );
	
	// 수량 표시해주는 패널들
	char strWndName[8] = {0,};
	LINK_CONTROL( "num_0", m_pkNumber1[0] );
	for( int i = 1 ; i < 10 ; i++ )
	{
		m_pkNumber1[i] = NULL;
//		m_pkNumber10[i] = NULL;

		sprintf( strWndName, "num_%d", i );
		LINK_CONTROL( strWndName, m_pkNumber1[i] );
// 		sprintf( strWndName, "num_%d", (i*10) );
// 		LINK_CONTROL( strWndName, m_pkNumber10[i] );
	}

    m_iRealCoolTime = 5;

    m_bIsSpecialItem = false;
}

KGCEmoticonSlot::~KGCEmoticonSlot()
{
    SAFE_RELEASE(m_pkCooltime_Pannel);
}

void KGCEmoticonSlot::OnCreate()
{
    m_pkFrame->SetPredraw( true );

    //if (m_pkEmoticon_Pannel->GetDeviceTexture() )
    //{
    //    m_pkFrame->SetAllWindowColor( D3DCOLOR_ARGB( ((int)g_kGlobalValue.m_fDevValue[3]), 255, 255, 255)  );
    //}

    //m_pkFrame->ToggleRender(false);

}

void KGCEmoticonSlot::OnDestroy()
{
	
}

void KGCEmoticonSlot::PostDraw()
{
    if ( !m_pkEmoticon || !m_pkEmoticon->m_pTexture->GetDeviceTexture() )
    {
        return;
    }   

    m_pkFrame->ReCalcReLocatedRect();
    m_pItemVertex = m_pkFrame->GetReLocatedWindowRect();

    float fCoolTimeRatio = g_MyD3D->GetMyShortCutSlot()->GetEmotCoolTime( m_MyIndex ) / ( m_iRealCoolTime * 55.0f );

	g_pGCDeviceManager2->PushState();
	{		
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		D3DXVECTOR4 vUV;
		vUV.x = m_pkEmoticon->m_safTextureCoordList[0][0];
		vUV.y = m_pkEmoticon->m_safTextureCoordList[0][1];
		vUV.z = m_pkEmoticon->m_safTextureCoordList[0][2];
		vUV.w = m_pkEmoticon->m_safTextureCoordList[0][3];

        if( m_bIsSpecialItem ) {
		    g_pGCDeviceManager2->DrawInScreen(m_pkEmoticon->m_pTexture,
			    m_pItemVertex[0].m_vPos.x, m_pItemVertex[0].m_vPos.y, m_pItemVertex[3].m_vPos.x, m_pItemVertex[3].m_vPos.y,
			    0.0f, 0.0f, 1.0f, 1.0f, false,10.0f,0xffffffff);
        } else {
            g_pGCDeviceManager2->DrawInScreen(m_pkEmoticon->m_pTexture,
                m_pItemVertex[0].m_vPos.x, m_pItemVertex[0].m_vPos.y, m_pItemVertex[3].m_vPos.x, m_pItemVertex[3].m_vPos.y,
                vUV.x, vUV.y, vUV.z, vUV.w, false,10.0f,0xffffffff);
        }

		g_pGCDeviceManager2->DrawInScreen(m_pkCooltime_Pannel,
			m_pItemVertex[3].m_vPos.x - 2 , m_pItemVertex[3].m_vPos.y - 1, m_pItemVertex[0].m_vPos.x + 2,
			( m_pItemVertex[0].m_vPos.y - 1 )+ (m_pItemVertex[3].m_vPos.y - m_pItemVertex[0].m_vPos.y - 1 ) * ( 1.0f - fCoolTimeRatio ) + 1 ,
			0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f,D3DCOLOR_ARGB(200,0,0,0));	    
		
	
	}
	g_pGCDeviceManager2->PopState();
}

void KGCEmoticonSlot::InitData()
{
    ReCalcReLocatedRect();
    m_iQuantity = 30;

    SetQuantityPos();
    SetQuantityOff();

    m_pkEmoticon = SiKGCEmoticonManager()->GetEmoticonData(g_MyD3D->GetMyShortCutSlot()->GetEmotID( m_MyIndex ));
    m_bIsSpecialItem = g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( g_MyD3D->GetMyShortCutSlot()->GetEmotID( m_MyIndex ) );
    if( m_bIsSpecialItem ) {
        m_iRealCoolTime = g_MyD3D->GetMyShortCutSlot()->GetSpecialSkillItemCoolTime( g_MyD3D->GetMyShortCutSlot()->GetEmotID( m_MyIndex ) );
    }
    m_pkCooltime_Pannel = g_pGCDeviceManager2->GetNullTexture();
    m_pkFrame->ReCalcReLocatedRect();
    m_pItemVertex = m_pkFrame->GetReLocatedWindowRect();

}


void KGCEmoticonSlot::OnChangeQuantity()
{
	if( m_iQuantity <= 0 )
	{
		SetQuantityOff();
		return;
	}
	if( m_iQuantity > 99 )
	{
		SetQuantityOff();
		m_pkNumber1[9]->ToggleRender(true);
//	m_pkNumber10[9]->ToggleRender(true);
		return;
	}

	//  1 단위
	for( int i = 0 ; i < DECIMAL_SIZE ; i++ )
	{
		if( (m_iQuantity%10) == i )
			m_pkNumber1[i]->ToggleRender(true);
		else
			m_pkNumber1[i]->ToggleRender(false);
	}
// 	// 10 단위
// 	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
// 	{
// 		if( (m_iQuantity/10) == i )
// 			m_pkNumber10[i]->ToggleRender(true);
// 		else
// 			m_pkNumber10[i]->ToggleRender(false);
// 	}
}

void KGCEmoticonSlot::SetQuantityPos()
{
	D3DXVECTOR2 VecNumber1_Pos = D3DXVECTOR2(18,15);
	//D3DXVECTOR2 VecNumber10_Pos = D3DXVECTOR2(20,20);

	// 숫자 패널들 위치
	m_pkNumber1[0]->SetWindowPos(VecNumber1_Pos);
	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
	{
		m_pkNumber1[i]->SetWindowPos(VecNumber1_Pos);
		//m_pkNumber10[i]->SetWindowPos(VecNumber10_Pos);
	}
}
void KGCEmoticonSlot::SetQuantityOff()
{
	// 일괄 안 보이게
	m_pkNumber1[0]->ToggleRender(false);
	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
	{
		m_pkNumber1[i]->ToggleRender(false);
	//	m_pkNumber10[i]->ToggleRender(false);
	}
}

void KGCEmoticonSlot::ActionPerformed(const KActionEvent& event )
{
    if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        int i = 0;

        
    }
}