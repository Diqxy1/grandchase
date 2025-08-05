#include "stdafx.h"
#include "KGCItemSlot.h"

#include "../MyD3D.h"

//
//

IMPLEMENT_CLASSNAME( KGCItemSlot );
IMPLEMENT_WND_FACTORY( KGCItemSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCItemSlot, "item_slot" );

KGCItemSlot::KGCItemSlot()
{
	char strWndName[32] = {0,};	// LINK_CONTROL 에서 쓰일 문자열공간

	// 변수 초기화
	m_bDrawItem		= false;
	m_bItemSpin		= false;
	m_bOnFlash		= false;
	m_iItemID		= -1;
	m_iQuantity		= 0;
	m_iCoolTime		= 0;
	m_iCoolTime_Now = 0;
	m_MyIndex		= -1;
	m_iFlash		= 6;
	m_iSpinCount	= 6;
	m_iItemAlpha	= 200;

	m_pkendCoolTime_Flash = NULL;

	m_pkBack_Pannel = NULL;
	m_pkFrame = NULL;
	m_pkCharacter = NULL;

	m_pkItem_Pannel = NULL;

	// initialization
	LINK_CONTROL( "frame", m_pkFrame );
	LINK_CONTROL( "character", m_pkCharacter );
	LINK_CONTROL( "back_pannel", m_pkBack_Pannel );
	//LINK_CONTROL( "item_pannel", m_pkItem_Pannel );
	LINK_CONTROL( "end_cooltime_flash", m_pkendCoolTime_Flash );

	// 쿨타임 표시해주는 패널들
	for( int i = 0 ; i < COOLTIME_PANNEL_NUM ; i++ )
	{
		m_pkCoolTime_Pannel[i] = NULL;

		// 0~3	□ 요렇게 생겼음
		// 4~5	│ 요렇게 생겼음
		// 6~7	─ 요렇게 생겼음
		sprintf( strWndName, "cooltime_pannel_%d", i );
		LINK_CONTROL( strWndName, m_pkCoolTime_Pannel[i] );
	}
	
	// 수량 표시해주는 패널들
	m_pkNumber1[0] = NULL;
	LINK_CONTROL( "num_0", m_pkNumber1[0] );
	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
	{
		m_pkNumber1[i] = NULL;
		m_pkNumber10[i] = NULL;

		sprintf( strWndName, "num_%d", i );
		LINK_CONTROL( strWndName, m_pkNumber1[i] );

        m_pkNumber100[i] = NULL;
        sprintf( strWndName, "num_%03d", ( i * DECIMAL_SIZE * DECIMAL_SIZE ) );
        LINK_CONTROL( strWndName, m_pkNumber100[i] );

    }

    for( int i=0 ; i<DECIMAL_SIZE ; i++ ) { 
        sprintf( strWndName, "num_%02d", (i*DECIMAL_SIZE) );
        LINK_CONTROL( strWndName, m_pkNumber10[i] );
	}
}

KGCItemSlot::~KGCItemSlot()
{
    SAFE_RELEASE(m_pkItem_Pannel);
}

void KGCItemSlot::OnCreate()
{

}

void KGCItemSlot::OnDestroy()
{
	SAFE_RELEASE(m_pkItem_Pannel);
}

void KGCItemSlot::PreDraw()
{
	//g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
}

//void KGCItemSlot::PostChildDrawDraw()
void KGCItemSlot::PostDraw()
{
	// 아이템이 없으면 화면 표시를 할 필요가 없겠지요?
	if( m_bDrawItem == false )
		return;

    m_pkFrame->ReCalcReLocatedRect();
    m_pItemVertex	= m_pkFrame->GetReLocatedWindowRect();

	g_pGCDeviceManager2->DrawInScreen(m_pkItem_Pannel,
		m_pItemVertex[0].m_vPos.x, m_pItemVertex[0].m_vPos.y, m_pItemVertex[3].m_vPos.x, m_pItemVertex[3].m_vPos.y,
		0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, D3DCOLOR_ARGB(static_cast<BYTE>(m_iItemAlpha),255,255,255));

	// CoolTime 정보 입수
	m_iCoolTime_Now = g_MyD3D->GetMyShortCutSlot()->GetItemCoolTime(m_MyIndex);
	if( m_iCoolTime < m_iCoolTime_Now )
	{
		m_iCoolTime = m_iCoolTime_Now;
	}

	// Item 숫자 표시
	OnChangeQuantity();

	// 아이템이 사용되는 순간 캐치
	int NewQuantity = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(m_MyIndex);
	if( NewQuantity < m_iQuantity )	// Item 수량이 줄었으면
	{
		m_iQuantity = NewQuantity;
		m_bItemSpin = true;
	}

	// 아이템이 뱅글뱅글
	if( m_bItemSpin == true )
	{
		OnItemSpin();
	}
	// 쿨타임
	if( m_iCoolTime > 0 )
	{
		OnCoolTime();
	}
	// 쿨타임이 끝났다고 아이템이 반짝
	if( m_bOnFlash == true )
	{
		OnFlash();
	}
}

// Cool Time이 적용되고 있는 동안 
void KGCItemSlot::OnCoolTime()
{	
	if( m_iQuantity <= 0 )
		return;

	for( int i = 0 ; i < COOLTIME_PANNEL_NUM ; i++ )
	{
		m_pkCoolTime_Pannel[i]->ToggleRender(true);
	}

	DecreaseCoolTime_0();
	DecreaseCoolTime_1();
	DecreaseCoolTime_2();
	DecreaseCoolTime_3();
	DecreaseCoolTime_4();
	DecreaseCoolTime_5();
	DecreaseCoolTime_6();
	DecreaseCoolTime_7();

	m_iCoolTime_Now--;

	if( m_iCoolTime_Now <= 0 )
	{
		m_iCoolTime = 0;
		m_bOnFlash = true;
		m_pkendCoolTime_Flash->ToggleRender(true);	// Cool Time이 끝나면

		ResetCoolTimePos();
	}
}
// CoolTime이 0이 되었을 때
void KGCItemSlot::OnFlash()
{
	m_iFlash--;
	if( m_iFlash == 0 )
	{
		m_bOnFlash = false;
		m_pkendCoolTime_Flash->ToggleRender(false);
		m_iFlash = 6;
	}
}
 // Item이 뱅글뱅글
void KGCItemSlot::OnItemSpin()
{
	if( m_iSpinCount%2 == 1 )	// 짝수면 뒤집어지는 중
	{
		m_pItemVertex[0].m_vPos.x -= (m_iSpinCount+5)/3+1;
		m_pItemVertex[3].m_vPos.x += (m_iSpinCount+5)/3+1;
	}
	else					// 홀수면 똑바로 되는 중
	{
		m_pItemVertex[0].m_vPos.x += (m_iSpinCount+1)/2+1;
		m_pItemVertex[3].m_vPos.x -= (m_iSpinCount+1)/2+1;
	}

	// 크기를 구해서
	float fWidth = m_pItemVertex[3].m_vPos.x - m_pItemVertex[0].m_vPos.x;
	if(fWidth < 0)
		fWidth*=(-1);

	// 돌다가 원래 크기로 돌아왔으면 도는 방향 전환
	if( fWidth >= m_pkBack_Pannel->GetWidth() )
	{
		m_iSpinCount--;
	}

	// 다 돌았으면 더 이상 돌 필요가 없다.
	if( m_iSpinCount <= 0 )
	{
		m_bItemSpin = false;

		// 원래 크기로 돌려줌
		m_pItemVertex = m_pkFrame->GetReLocatedWindowRect();
		m_iSpinCount = 6;

		if( m_iQuantity <= 0 )
		{
			m_bDrawItem = false;
		}
	}
}

void KGCItemSlot::OnChangeQuantity()
{
    // 수량이 0이면 아이템 자체가 안보이게
    if( m_iQuantity <= 0 )
    {
        SetQuantityOff();
        m_bDrawItem = false;
        return;
    }

    if( m_iQuantity > 999 )
    {
        SetQuantityOff();
        m_pkNumber1[9]->ToggleRender(true);
        m_pkNumber10[9]->ToggleRender(true);
        m_pkNumber100[9]->ToggleRender(true);
        return;
    }

    // 각 단위 수 
    int iNumFirst  = m_iQuantity % 10;            // 1단위
    int iNumSecond = ( m_iQuantity / 10 ) % 10;   // 10단위
    int iNumThird  = m_iQuantity / 100;           // 100단위

	//  1 단위
	for( int i = 0 ; i < DECIMAL_SIZE ; i++ )
	{
		if( iNumFirst == i )
			m_pkNumber1[i]->ToggleRender(true);
		else
			m_pkNumber1[i]->ToggleRender(false);
	}

    // 10 단위 : 100단위가 있을때만 0출력되도록
    for( int i = 0 ; i < DECIMAL_SIZE ; i++ )
    {
        
        if( iNumSecond == i && false == (0 == iNumSecond && 0 == iNumThird) )
            m_pkNumber10[i]->ToggleRender(true);
        else
            m_pkNumber10[i]->ToggleRender(false);
    }

    // 100단위 
    for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
    {
        if( iNumThird == i )
            m_pkNumber100[i]->ToggleRender(true);
        else
            m_pkNumber100[i]->ToggleRender(false);
    }



}

void KGCItemSlot::SetQuantityPos()
{
	D3DXVECTOR2 VecNumber1_Pos = D3DXVECTOR2(32,24);
	D3DXVECTOR2 VecNumber10_Pos = D3DXVECTOR2(24,24);
    D3DXVECTOR2 VecNumber100_Pos = D3DXVECTOR2(16,24);

	// 숫자 패널들 위치
	m_pkNumber1[0]->SetWindowPos(VecNumber1_Pos);
	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
	{
		m_pkNumber1[i]->SetWindowPos(VecNumber1_Pos);
        m_pkNumber100[i]->SetWindowPos(VecNumber100_Pos);
    }

    for( int i = 0 ; i < DECIMAL_SIZE ; ++i ) { 
        m_pkNumber10[i]->SetWindowPos(VecNumber10_Pos);
	}
}
void KGCItemSlot::SetQuantityOff()
{
	// 일괄 안 보이게
	m_pkNumber1[0]->ToggleRender(false);
	for( int i = 1 ; i < DECIMAL_SIZE ; i++ )
	{
		m_pkNumber1[i]->ToggleRender(false);
        m_pkNumber100[i]->ToggleRender( false );
	}

    for( int i = 0 ; i < DECIMAL_SIZE ; ++i ) { 
        m_pkNumber10[i]->ToggleRender( false );
    }
}


// 이하, 작성된 코드를 보는 경우 뇌출혈을 일으킬 수 있습니다.

int square	= 0;							// 현재 0~3번 CoolTime Wnd의 한 변의 길이.(정사각형)
int	width	= 0;							// 틈틈 사이 쪼가리 가로
int height	= 0;							// 틈틈 사이 쪼가리 세로
D3DXVECTOR2 pos;							// 해당 Wnd의 시작점 좌표
const int PANNEL_FULL_SIZE		= 35+1;		// +1은 계산상 편의를 위해 존재.
const int gap_x					= 2;		// UI 스크립트 상에서 부터 존재하는 X 좌표 상의 격차
const int gap_y					= 2;		// UI 스크립트 상에서 부터 존재하는 Y 좌표 상의 격차
////////////////////////////////////////////////
// 세트로 묶어서 CoolTime Pannel을 조종하는 함수
void KGCItemSlot::ResetCoolTimePos()
{
	m_VecCoolTime_Pannel_Pos[0] = D3DXVECTOR2(3,3);
	m_VecCoolTime_Pannel_Pos[1] = D3DXVECTOR2(21,3);
	m_VecCoolTime_Pannel_Pos[2] = D3DXVECTOR2(3,21);
	m_VecCoolTime_Pannel_Pos[3] = D3DXVECTOR2(21,21);
	m_VecCoolTime_Pannel_Pos[4] = D3DXVECTOR2(20,3);
	m_VecCoolTime_Pannel_Pos[5] = D3DXVECTOR2(20,21);
	m_VecCoolTime_Pannel_Pos[6] = D3DXVECTOR2(3,20);
	m_VecCoolTime_Pannel_Pos[7] = D3DXVECTOR2(21,20);

	m_pkCoolTime_Pannel[0]->SetWidthHeight(17,17);
	m_pkCoolTime_Pannel[1]->SetWidthHeight(17,17);
	m_pkCoolTime_Pannel[2]->SetWidthHeight(17,17);
	m_pkCoolTime_Pannel[3]->SetWidthHeight(17,17);
	m_pkCoolTime_Pannel[4]->SetWidthHeight(1,17);
	m_pkCoolTime_Pannel[5]->SetWidthHeight(1,17);
	m_pkCoolTime_Pannel[6]->SetWidthHeight(17,1);
	m_pkCoolTime_Pannel[7]->SetWidthHeight(17,1);

	// 기본적인 Pannel들을 배치한다.
	for( int i = 0 ; i < COOLTIME_PANNEL_NUM ; i++ )
	{
		m_pkCoolTime_Pannel[i]->SetWindowPos(m_VecCoolTime_Pannel_Pos[i]);
		m_pkCoolTime_Pannel[i]->ToggleRender(false);
	}
}
void KGCItemSlot::DecreaseCoolTime_0()	// □ 요렇게 생겼음
{
	square = static_cast<int>(static_cast<float>(m_iCoolTime_Now)/static_cast<float>(m_iCoolTime)*17);
	m_pkCoolTime_Pannel[0]->SetWidthHeight(square-1,square);

	pos.x = static_cast<float>(gap_x-2);
	pos.y = static_cast<float>(gap_y-1);
	m_pkCoolTime_Pannel[1]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_1()	// □ 요렇게 생겼음
{
	m_pkCoolTime_Pannel[1]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(gap_y);
	m_pkCoolTime_Pannel[1]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_2()	// □ 요렇게 생겼음
{
	m_pkCoolTime_Pannel[2]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y + 1);
	m_pkCoolTime_Pannel[2]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_3()	// □ 요렇게 생겼음
{
	m_pkCoolTime_Pannel[3]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y);
	m_pkCoolTime_Pannel[3]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_4()	// │ 요렇게 생겼음
{
	width	= PANNEL_FULL_SIZE - (square*2);
	height	= square;
	m_pkCoolTime_Pannel[4]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(square + gap_x);
	pos.y = static_cast<float>(gap_y);
	m_pkCoolTime_Pannel[4]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_5()	// □ 요렇게 생겼음
{
	m_pkCoolTime_Pannel[5]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(square + gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y);
	m_pkCoolTime_Pannel[5]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_6()	// ─ 요렇게 생겼음
{
	width	= square;
	height	= PANNEL_FULL_SIZE - (square*2);
	m_pkCoolTime_Pannel[6]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(gap_x);
	pos.y = static_cast<float>(square + gap_y + 1);
	m_pkCoolTime_Pannel[6]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_7()	// ─ 요렇게 생겼음
{
	m_pkCoolTime_Pannel[7]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(square + gap_y);
	m_pkCoolTime_Pannel[7]->SetWindowPos(pos);
}

void KGCItemSlot::SetIndex(int index)
{
    m_MyIndex = index;
    // Item의 정보를 읽어온다. 
    m_iItemID = g_MyD3D->GetMyShortCutSlot()->GetItemID(m_MyIndex);
    m_iQuantity = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(m_MyIndex);
    m_iCoolTime = g_MyD3D->GetMyShortCutSlot()->GetItemCoolTime(m_MyIndex);

	m_bDrawItem = false;
	m_bEnabled  = true;
	if(SiKGCWorldMapManager()->IsNotUseShortCutItem(SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel()))
	{
		m_bEnabled = g_MyD3D->GetMyShortCutSlot()->IsGameModeAllowed(SiKGCRoomManager()->GetGameMode(), m_MyIndex);
	}
    
	if( m_iQuantity > 0 && m_bEnabled)
	{
		m_bDrawItem = true;
		SAFE_RELEASE( m_pkItem_Pannel );
		m_pkItem_Pannel = g_pItemMgr->CreateItemTexture(m_iItemID, 0);
	}

    // Pannel들의 좌표를 정한다.
    ResetCoolTimePos();
    SetQuantityPos();
    SetQuantityOff();

	m_pkFrame->ReCalcReLocatedRect();
	m_pItemVertex	= m_pkFrame->GetReLocatedWindowRect();
    m_iItemAlpha	= 200;

    // 당장 보일 필요없는 패널들을 안 보이게 한다.
    m_pkBack_Pannel->ToggleRender(false);
    m_pkendCoolTime_Flash->ToggleRender(false);
}