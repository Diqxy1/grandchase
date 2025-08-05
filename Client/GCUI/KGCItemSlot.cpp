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
	char strWndName[32] = {0,};	// LINK_CONTROL ���� ���� ���ڿ�����

	// ���� �ʱ�ȭ
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

	// ��Ÿ�� ǥ�����ִ� �гε�
	for( int i = 0 ; i < COOLTIME_PANNEL_NUM ; i++ )
	{
		m_pkCoolTime_Pannel[i] = NULL;

		// 0~3	�� �䷸�� ������
		// 4~5	�� �䷸�� ������
		// 6~7	�� �䷸�� ������
		sprintf( strWndName, "cooltime_pannel_%d", i );
		LINK_CONTROL( strWndName, m_pkCoolTime_Pannel[i] );
	}
	
	// ���� ǥ�����ִ� �гε�
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
	// �������� ������ ȭ�� ǥ�ø� �� �ʿ䰡 ��������?
	if( m_bDrawItem == false )
		return;

    m_pkFrame->ReCalcReLocatedRect();
    m_pItemVertex	= m_pkFrame->GetReLocatedWindowRect();

	g_pGCDeviceManager2->DrawInScreen(m_pkItem_Pannel,
		m_pItemVertex[0].m_vPos.x, m_pItemVertex[0].m_vPos.y, m_pItemVertex[3].m_vPos.x, m_pItemVertex[3].m_vPos.y,
		0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f, D3DCOLOR_ARGB(static_cast<BYTE>(m_iItemAlpha),255,255,255));

	// CoolTime ���� �Լ�
	m_iCoolTime_Now = g_MyD3D->GetMyShortCutSlot()->GetItemCoolTime(m_MyIndex);
	if( m_iCoolTime < m_iCoolTime_Now )
	{
		m_iCoolTime = m_iCoolTime_Now;
	}

	// Item ���� ǥ��
	OnChangeQuantity();

	// �������� ���Ǵ� ���� ĳġ
	int NewQuantity = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(m_MyIndex);
	if( NewQuantity < m_iQuantity )	// Item ������ �پ�����
	{
		m_iQuantity = NewQuantity;
		m_bItemSpin = true;
	}

	// �������� ��۹��
	if( m_bItemSpin == true )
	{
		OnItemSpin();
	}
	// ��Ÿ��
	if( m_iCoolTime > 0 )
	{
		OnCoolTime();
	}
	// ��Ÿ���� �����ٰ� �������� ��¦
	if( m_bOnFlash == true )
	{
		OnFlash();
	}
}

// Cool Time�� ����ǰ� �ִ� ���� 
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
		m_pkendCoolTime_Flash->ToggleRender(true);	// Cool Time�� ������

		ResetCoolTimePos();
	}
}
// CoolTime�� 0�� �Ǿ��� ��
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
 // Item�� ��۹��
void KGCItemSlot::OnItemSpin()
{
	if( m_iSpinCount%2 == 1 )	// ¦���� ���������� ��
	{
		m_pItemVertex[0].m_vPos.x -= (m_iSpinCount+5)/3+1;
		m_pItemVertex[3].m_vPos.x += (m_iSpinCount+5)/3+1;
	}
	else					// Ȧ���� �ȹٷ� �Ǵ� ��
	{
		m_pItemVertex[0].m_vPos.x += (m_iSpinCount+1)/2+1;
		m_pItemVertex[3].m_vPos.x -= (m_iSpinCount+1)/2+1;
	}

	// ũ�⸦ ���ؼ�
	float fWidth = m_pItemVertex[3].m_vPos.x - m_pItemVertex[0].m_vPos.x;
	if(fWidth < 0)
		fWidth*=(-1);

	// ���ٰ� ���� ũ��� ���ƿ����� ���� ���� ��ȯ
	if( fWidth >= m_pkBack_Pannel->GetWidth() )
	{
		m_iSpinCount--;
	}

	// �� �������� �� �̻� �� �ʿ䰡 ����.
	if( m_iSpinCount <= 0 )
	{
		m_bItemSpin = false;

		// ���� ũ��� ������
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
    // ������ 0�̸� ������ ��ü�� �Ⱥ��̰�
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

    // �� ���� �� 
    int iNumFirst  = m_iQuantity % 10;            // 1����
    int iNumSecond = ( m_iQuantity / 10 ) % 10;   // 10����
    int iNumThird  = m_iQuantity / 100;           // 100����

	//  1 ����
	for( int i = 0 ; i < DECIMAL_SIZE ; i++ )
	{
		if( iNumFirst == i )
			m_pkNumber1[i]->ToggleRender(true);
		else
			m_pkNumber1[i]->ToggleRender(false);
	}

    // 10 ���� : 100������ �������� 0��µǵ���
    for( int i = 0 ; i < DECIMAL_SIZE ; i++ )
    {
        
        if( iNumSecond == i && false == (0 == iNumSecond && 0 == iNumThird) )
            m_pkNumber10[i]->ToggleRender(true);
        else
            m_pkNumber10[i]->ToggleRender(false);
    }

    // 100���� 
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

	// ���� �гε� ��ġ
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
	// �ϰ� �� ���̰�
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


// ����, �ۼ��� �ڵ带 ���� ��� �������� ����ų �� �ֽ��ϴ�.

int square	= 0;							// ���� 0~3�� CoolTime Wnd�� �� ���� ����.(���簢��)
int	width	= 0;							// ƴƴ ���� �ɰ��� ����
int height	= 0;							// ƴƴ ���� �ɰ��� ����
D3DXVECTOR2 pos;							// �ش� Wnd�� ������ ��ǥ
const int PANNEL_FULL_SIZE		= 35+1;		// +1�� ���� ���Ǹ� ���� ����.
const int gap_x					= 2;		// UI ��ũ��Ʈ �󿡼� ���� �����ϴ� X ��ǥ ���� ����
const int gap_y					= 2;		// UI ��ũ��Ʈ �󿡼� ���� �����ϴ� Y ��ǥ ���� ����
////////////////////////////////////////////////
// ��Ʈ�� ��� CoolTime Pannel�� �����ϴ� �Լ�
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

	// �⺻���� Pannel���� ��ġ�Ѵ�.
	for( int i = 0 ; i < COOLTIME_PANNEL_NUM ; i++ )
	{
		m_pkCoolTime_Pannel[i]->SetWindowPos(m_VecCoolTime_Pannel_Pos[i]);
		m_pkCoolTime_Pannel[i]->ToggleRender(false);
	}
}
void KGCItemSlot::DecreaseCoolTime_0()	// �� �䷸�� ������
{
	square = static_cast<int>(static_cast<float>(m_iCoolTime_Now)/static_cast<float>(m_iCoolTime)*17);
	m_pkCoolTime_Pannel[0]->SetWidthHeight(square-1,square);

	pos.x = static_cast<float>(gap_x-2);
	pos.y = static_cast<float>(gap_y-1);
	m_pkCoolTime_Pannel[1]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_1()	// �� �䷸�� ������
{
	m_pkCoolTime_Pannel[1]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(gap_y);
	m_pkCoolTime_Pannel[1]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_2()	// �� �䷸�� ������
{
	m_pkCoolTime_Pannel[2]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y + 1);
	m_pkCoolTime_Pannel[2]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_3()	// �� �䷸�� ������
{
	m_pkCoolTime_Pannel[3]->SetWidthHeight(square,square);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y);
	m_pkCoolTime_Pannel[3]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_4()	// �� �䷸�� ������
{
	width	= PANNEL_FULL_SIZE - (square*2);
	height	= square;
	m_pkCoolTime_Pannel[4]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(square + gap_x);
	pos.y = static_cast<float>(gap_y);
	m_pkCoolTime_Pannel[4]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_5()	// �� �䷸�� ������
{
	m_pkCoolTime_Pannel[5]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(square + gap_x);
	pos.y = static_cast<float>(PANNEL_FULL_SIZE - square + gap_y);
	m_pkCoolTime_Pannel[5]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_6()	// �� �䷸�� ������
{
	width	= square;
	height	= PANNEL_FULL_SIZE - (square*2);
	m_pkCoolTime_Pannel[6]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(gap_x);
	pos.y = static_cast<float>(square + gap_y + 1);
	m_pkCoolTime_Pannel[6]->SetWindowPos(pos);
}
void KGCItemSlot::DecreaseCoolTime_7()	// �� �䷸�� ������
{
	m_pkCoolTime_Pannel[7]->SetWidthHeight(width,height);

	pos.x = static_cast<float>(PANNEL_FULL_SIZE - square + gap_x);
	pos.y = static_cast<float>(square + gap_y);
	m_pkCoolTime_Pannel[7]->SetWindowPos(pos);
}

void KGCItemSlot::SetIndex(int index)
{
    m_MyIndex = index;
    // Item�� ������ �о�´�. 
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

    // Pannel���� ��ǥ�� ���Ѵ�.
    ResetCoolTimePos();
    SetQuantityPos();
    SetQuantityOff();

	m_pkFrame->ReCalcReLocatedRect();
	m_pItemVertex	= m_pkFrame->GetReLocatedWindowRect();
    m_iItemAlpha	= 200;

    // ���� ���� �ʿ���� �гε��� �� ���̰� �Ѵ�.
    m_pkBack_Pannel->ToggleRender(false);
    m_pkendCoolTime_Flash->ToggleRender(false);
}