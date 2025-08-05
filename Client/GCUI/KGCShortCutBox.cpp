#include "stdafx.h"
#include "KGCShortCutBox.h"

#include "MyD3D.h"
//
//

#include "KGCMacroCommunitySlot.h"
#include "KGCEmoticonSlot.h"
#include "KGCItemSlot.h"

IMPLEMENT_CLASSNAME( KGCShortCutBox );
IMPLEMENT_WND_FACTORY( KGCShortCutBox );
IMPLEMENT_WND_FACTORY_NAME( KGCShortCutBox, "gc_short_cut_wnd" );

#define _HIDE_POSX_OFFSET_ 80
#define _HIDE_POSX_STRIDE_ 5


KGCShortCutBox::KGCShortCutBox()
{
	char strWndName[MAX_PATH] = {0,};
	for( int i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
	{
		m_pkItem_Slot[i] = NULL;

		sprintf( strWndName, "item_%d", i );
		LINK_CONTROL( strWndName, m_pkItem_Slot[i] );
	}
	for( int i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
	{
		m_pkEmot_Slot[i] = NULL;

		sprintf( strWndName, "emot_%d", i );
		LINK_CONTROL( strWndName, m_pkEmot_Slot[i] );
	}

    for( int i = 0 ; i < MCOMMU_SLOT_SIZE ; i++ )
    {
        m_pkMcommu_Slot[i] = NULL;
        
        sprintf( strWndName, "mcommu_%d", i );
        LINK_CONTROL( strWndName, m_pkMcommu_Slot[i] );
    }

	m_pkWaveKey = NULL;
	LINK_CONTROL( "wave_key", m_pkWaveKey );

	m_eModeState = SCB_MODE_ALL;
    m_eShowState = SFS_SHOW;
}

KGCShortCutBox::~KGCShortCutBox()
{
}

void KGCShortCutBox::OnInitialize()
{
    ReCalcReLocatedRect();
    for ( int i = 0 ; i < EMOT_SLOT_SIZE ; i++ )
    {
        m_pkEmot_Slot[i]->SetIndex(i);
        m_pkEmot_Slot[i]->InitData();
        
    }
    for ( int i = 0 ; i < ITEM_SLOT_SIZE ; i++ )
    {
        m_pkItem_Slot[i]->SetIndex(i);
        m_pkItem_Slot[i]->InitData();
        
    }
    for ( int i = 0 ; i < MCOMMU_SLOT_SIZE ; i++ )
    {
        if( m_pkMcommu_Slot[i] ) { 
            m_pkMcommu_Slot[i]->SetIndex(i);
            m_pkMcommu_Slot[i]->InitData();
        }
    }

#if !defined(EMOTICON_SYSTEM)
	if (m_eModeState==SCB_MODE_ITEM)
		ToggleRender( true );
#endif
}

void KGCShortCutBox::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode != D3DWE_BUTTON_CLICK )
    {
        return;
    }

 /*   for ( int i = 0 ; i < 10 ; i++)
    { 
        if ( event.m_pWnd == m_pkEmot_Slot[i] )
        {
            m_pkWaveKey->ToggleRender( !m_pkWaveKey->IsRenderOn() );
        }
    }

    if ( event.m_pWnd == m_pkWaveKey )
    {

    }*/
}

void KGCShortCutBox::SetShortCutMode( SCB_MODE_STATE _state )
{
	if( _state >= SCB_MODE_ALL && _state < SCB_MODE_STATE_NUM )
		m_eModeState = _state;
	else
        m_eModeState = SCB_MODE_ALL;

    if ( _state == SCB_MODE_EMOT )
    {
        D3DXVECTOR2 vCurrentSlotPos = m_vOriginalPos;
        vCurrentSlotPos.y += 80.0f;
        SetWindowPosDirect(vCurrentSlotPos);
    }
    else
    {
        SetWindowPosDirect(m_vOriginalPos);
    }

    bool bDisplayItem = true;
    bool bDisplayEmot = true;
    bool bDisplayMcommu = false;


    // SHOW_STATE SETTING에 따라
    // ITEM SLOT
    if( m_eModeState == SCB_MODE_ALL )
    {
        ResetBoxPos();
        ResetEmotPos();
        ResetMcommuPos();
    }
    else if( m_eModeState == SCB_MODE_EMOT )
    {
        bDisplayItem = false;
        bDisplayMcommu = false;

        // Emoticon 슬롯을 위로
        D3DXVECTOR2 CurrPos;
        float fGap = m_vFixedEmotSlotPos[0].y - m_vFixedItemSlotPos[0].y;
        for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
        {
            CurrPos = m_vFixedEmotSlotPos[index];
            CurrPos.y -= fGap;
            m_pkEmot_Slot[index]->SetWindowPos(CurrPos);
        }
    }
    else if( m_eModeState == SCB_MODE_ITEM )
    {
        bDisplayEmot = false;
        bDisplayMcommu = false;
    }
    // SCB_HIDE
    else{
        bDisplayItem = false;
        bDisplayEmot = false;
        bDisplayMcommu = false;
    }

    // 매크로 커뮤니티 설정 박스별(키별)로 셋팅
    std::map<int, SMacroCommunity> mapMacroCommunity = SiKGCWorldMapManager()->GetMacroCommunity( SiKGCRoomManager()->GetGameMode() );
    if( false == mapMacroCommunity.empty() )
    {
        bDisplayMcommu = true;
        bDisplayEmot = false;

        std::map<int, SMacroCommunity>::iterator mapIter = mapMacroCommunity.begin();
        for(; mapIter != mapMacroCommunity.end(); ++mapIter)
        {
            int iIndex = mapIter->first - 1;
            if( 0 > iIndex || iIndex > MCOMMU_SLOT_SIZE )
                continue;
            
            if( m_pkMcommu_Slot[iIndex] ) { 
                m_pkMcommu_Slot[iIndex]->m_sMacroCommunityInfo = mapIter->second;
            }
        }
    }

    // 상태 세팅
    for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ ){
        
        m_pkItem_Slot[index]->ToggleRender(bDisplayItem);
    }
    for(int index = 0 ; index < EMOT_SLOT_SIZE ; index++ ){
        m_pkEmot_Slot[index]->SetIndex(index);
        m_pkEmot_Slot[index]->InitState( bDisplayEmot, true, this );
    }
    for(int index = 0 ; index < MCOMMU_SLOT_SIZE ; index++ ){
        if( m_pkMcommu_Slot[index] ) { 
            m_pkMcommu_Slot[index]->SetIndex(index);
            m_pkMcommu_Slot[index]->InitState( bDisplayMcommu, true, this );
        }
    }

}

//////////////////////////////////////////////////////////////////////////
//						protected member function						//
//////////////////////////////////////////////////////////////////////////
void KGCShortCutBox::OnCreate()
{
    m_vOriginalPos = GetFixedWindowLocalPos();

	for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
		m_vFixedItemSlotPos[index] = m_pkItem_Slot[index]->GetFixedWindowLocalPos();
	for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
		m_vFixedEmotSlotPos[index] = m_pkEmot_Slot[index]->GetFixedWindowLocalPos();
    for( int index = 0 ; index < MCOMMU_SLOT_SIZE ; index++ )
    {
        if( m_pkMcommu_Slot[index] ) { 
            m_vFixedMcommuSlotPos[index] = m_pkMcommu_Slot[index]->GetFixedWindowLocalPos();
        }
    }

    m_pkWaveKey->InitState( true ,true ,this); 
}

void KGCShortCutBox::OnDestroy()
{
}

void KGCShortCutBox::FrameMoveInEnabledState()
{
    if( m_eShowState == SFS_FADE_OUT )
    {
        D3DXVECTOR2 vPos = GetCurrentWindowLocalPos();
        if ( vPos.x >= m_vOriginalPos.x + _HIDE_POSX_OFFSET_ )
        {
            m_eShowState = SFS_HIDE;
            ToggleRender( false );
        }
        else
        {
            vPos.x += _HIDE_POSX_STRIDE_;
            SetWindowPosDirect( vPos );
        }

    }
    else if( m_eShowState == SFS_FADE_IN )
    {        
        D3DXVECTOR2 vPos = GetCurrentWindowLocalPos();
        if ( vPos.x <= m_vOriginalPos.x )
        {
            m_eShowState = SFS_SHOW;
        }
        else
        {
            vPos.x -= _HIDE_POSX_STRIDE_;
            SetWindowPosDirect( vPos );
        }
    }
}

void KGCShortCutBox::PostDraw()
{
}


void KGCShortCutBox::SetItemID( int _index )
{
	m_iID[_index] = g_MyD3D->GetMyShortCutSlot()->GetItemID(_index);
}
void KGCShortCutBox::SetItemQuantity( int _index )
{
	m_iQuantity[_index] = g_MyD3D->GetMyShortCutSlot()->GetItemQuantity(_index);
}
void KGCShortCutBox::SetItemCoolTime( int _index )
{
	m_iCooltime[_index] = g_MyD3D->GetMyShortCutSlot()->GetItemCoolTime(_index);
}
void KGCShortCutBox::SetEmotID( int _index )
{
	m_iID[_index+ITEM_SLOT_SIZE] = g_MyD3D->GetMyShortCutSlot()->GetEmotID(_index);
}
void KGCShortCutBox::SetEmotQuantity( int _index )
{
	m_iQuantity[_index+ITEM_SLOT_SIZE] = g_MyD3D->GetMyShortCutSlot()->GetEmotQuantity(_index);
}
void KGCShortCutBox::SetEmotCoolTime( int _index )
{
	m_iCooltime[_index+ITEM_SLOT_SIZE] = g_MyD3D->GetMyShortCutSlot()->GetEmotCoolTime(_index);
}

void KGCShortCutBox::SetItemPos( float _fPos )
{
	for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ ){
		m_pkItem_Slot[index]->SetItemPos(_fPos);
	}
	for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ ){
//		m_pkEmot_Slot[index]->SetItemPos(_fPos);
	}
}
void KGCShortCutBox::SetItemAlpha( int _iVar)
{
	for( int index = 0 ; index < ITEM_SLOT_SIZE ; index++ )
	{
		m_pkItem_Slot[index]->SetItemAlpha(_iVar);
	}
	for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
	{
		m_pkEmot_Slot[index]->SetItemAlpha(_iVar);
	}
}


void KGCShortCutBox::ToggleShowState()
{
#if !defined(EMOTICON_SYSTEM)
	if (m_eModeState!=SCB_MODE_ITEM)
		return;
#endif

    if ( m_eShowState == SFS_FADE_IN || m_eShowState == SFS_SHOW )
    {
        SetShowState( SFS_FADE_OUT );
    }    
    else
    {
        SetShowState( SFS_FADE_IN );        
    }


}

void KGCShortCutBox::ResetEmotPos()
{
	for( int index = 0 ; index < EMOT_SLOT_SIZE ; index++ )
		m_pkEmot_Slot[index]->SetWindowPos( m_vFixedEmotSlotPos[index] );
}

void KGCShortCutBox::ResetMcommuPos()
{
    for( int index = 0 ; index < MCOMMU_SLOT_SIZE ; index++ ) { 
        if( m_pkMcommu_Slot[index] ) { 
            m_pkMcommu_Slot[index]->SetWindowPos( m_vFixedMcommuSlotPos[index] );
        }
    }
}

void KGCShortCutBox::SetInputCommunityKey( int iKeyIndex, int iCoolTime )
{
    if( 0 > iKeyIndex || iKeyIndex > MCOMMU_SLOT_SIZE )
        return;

    if( m_pkMcommu_Slot[iKeyIndex] ) { 
        if( m_pkMcommu_Slot[iKeyIndex]->SendCommunityMsg() )
        {
            for( int index = 0; index < MCOMMU_SLOT_SIZE; index++ )
            {
                m_pkMcommu_Slot[index]->SetCoolTime( iCoolTime );
            }   
        }
    }
}

void KGCShortCutBox::MacroCommunityFrameMove()
{
    for( int index = 0; index < MCOMMU_SLOT_SIZE; index++ )
    {
        if( m_pkMcommu_Slot[index] ) { 
            m_pkMcommu_Slot[index]->CoolTimeFrameMove();
        }
    }
}