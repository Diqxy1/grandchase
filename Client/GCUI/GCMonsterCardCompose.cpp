#include "stdafx.h"
#include "GCMonsterCardCompose.h"
#include "GCMonsterCardRoulette.h"
#include "GCMonsterCardInven.h"
#include "GCItemManager.h"
#include "GCCheckBox.h"
#include "GCItemImgUIWnd.h"


IMPLEMENT_CLASSNAME( KGCMonsterCardCompose );
IMPLEMENT_WND_FACTORY( KGCMonsterCardCompose );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCardCompose, "gc_monstercardcompose" );


bool MonsterCardGradeSort ( const PAIR_MONSTER_CARD& p1, const PAIR_MONSTER_CARD& p2 )
{
	GCItem* pItem1 = g_pItemMgr->GetItemData( p1.first );
	GCItem* pItem2 = g_pItemMgr->GetItemData( p2.first );

	if ( !pItem1 || !pItem2 ) 
		return false;

	if ( pItem1->cItemGrade > pItem2->cItemGrade )
		return true;
	else if ( pItem1->cItemGrade == pItem2->cItemGrade )
	{
		if ( pItem1->iNeedLevelForEquippingItem > pItem2->iNeedLevelForEquippingItem ) 
			return true;
		else 
			return false;
	}
	else
		return false;
}

KGCMonsterCardCompose::KGCMonsterCardCompose( void )
: m_dwGradeFlag ( FLAG_GRADE_ALL )
, m_nIndex( 0 )
, m_nPrevIndex( 0 )
, m_bMoveCard( false )
, m_dwState( SEL_CARD_STATE )
{
	m_pkBtnOK = NULL;
	m_pkBtnCancel = NULL;
	m_pkMonsterCardRoulette = NULL;
	m_pkBtnPrevPage = NULL;
	m_pkBtnNextPage = NULL;
	m_pkMainDescStatic = NULL;
	m_pkInvenCardDescStatic = NULL;
	m_pkPageNum = NULL;
	m_pkCardImageOn = NULL;
	m_pkCardImageOnBack = NULL;
    m_pkRectInven = NULL;
    m_pkRectMaterial = NULL;

	m_vecMonsterCardList.clear();
	m_vecMaterialList.clear();

    m_bSendScriptReq = false;
    iCurrentSelectedMonsterCard = -1;    


	LINK_CONTROL( "btn_ok", m_pkBtnOK );
	LINK_CONTROL( "btn_cancel", m_pkBtnCancel );
	LINK_CONTROL( "monster_card_roulette", m_pkMonsterCardRoulette );
	LINK_CONTROL( "btn_prev_page", m_pkBtnPrevPage );
	LINK_CONTROL( "btn_next_page", m_pkBtnNextPage );

	LINK_CONTROL( "monster_card_mix_desc", m_pkMainDescStatic );
	LINK_CONTROL( "inven_card_desc", m_pkInvenCardDescStatic );
	LINK_CONTROL( "page_num", m_pkPageNum );
	LINK_CONTROL( "item_img_on", m_pkCardImageOn );
	LINK_CONTROL( "back_item_img_on", m_pkCardImageOnBack );
    
    LINK_CONTROL( "rect_inven", m_pkRectInven );
    LINK_CONTROL( "rect_material", m_pkRectMaterial );
    
    LINK_CONTROL( "item_wnd_dummy", m_pkMonsterCardContentsDummy );
    LINK_CONTROL( "item_img_on_dummy", m_pkMonsterCardContentsBigDummy);
    
	char str[MAX_PATH] = {0,};

	for ( int i = 0; i < EM_GRADE_SIZE; ++i ) 
	{
		m_pkGradeCheckBtn[i] = NULL;
		m_pkGradeStatic[i] = NULL;

		sprintf( str, "item_check_btn%d", i );  
		LINK_CONTROL( str, m_pkGradeCheckBtn[i] );

		sprintf( str, "static_item_grade%d", i );  
		LINK_CONTROL( str, m_pkGradeStatic[i] );
	}	

	for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i ) 
	{
		m_pkInvenSlot[i] = NULL;

		sprintf( str, "inven_item_wnd%02d", i );
		LINK_CONTROL( str, m_pkInvenSlot[i] );
	}

	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		m_pkMaterialSlot[i] = NULL;

		sprintf( str, "material_item_wnd%02d", i );
		LINK_CONTROL( str, m_pkMaterialSlot[i] );
	}
}

KGCMonsterCardCompose::~KGCMonsterCardCompose( void )
{
}

void KGCMonsterCardCompose::OnCreate( void )
{
	m_pkMainDescStatic->SetFontColor( 0xFF000000 );
#if defined(NATION_IDN) || defined( NATION_USA ) || defined( NATION_LATINAMERICA )
	m_pkMainDescStatic->SetFontSpace( 0.5f );
#else
	m_pkMainDescStatic->SetFontSpace( 1.0f );
#endif
	m_pkMainDescStatic->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
	m_pkMainDescStatic->SetAlign(DT_LEFT);
	m_pkMainDescStatic->SetText( g_pkStrLoader->GetString( STR_ID_MONSTER_CARD_MIX_MAIN_DESC ));
	m_pkMainDescStatic->SetMultiLine( true );

	m_pkInvenCardDescStatic->SetFontColor(0xffE9EEF3);
	m_pkInvenCardDescStatic->SetFontOutline(true,0xff14212f);
	m_pkInvenCardDescStatic->SetFontSpace( 1.0f );
	m_pkInvenCardDescStatic->SetAlign(DT_RIGHT);
	m_pkInvenCardDescStatic->SetText( g_pkStrLoader->GetString( STR_ID_MONSTER_CARD_MIX_SUB_DESC ));

	m_pkPageNum->SetFontColor(0xffE9EEF3);
	m_pkPageNum->SetFontOutline(true,0xff14212f);
	m_pkPageNum->SetFontSpace( 1.0f );
	m_pkPageNum->SetAlign(DT_CENTER);
	m_pkPageNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_MIX_PAGE_NUM, "ii", 0, 0 ));


	for ( int i = 0; i < EM_GRADE_SIZE; ++i ) 
	{
		m_pkGradeCheckBtn[i]->InitState( true, true, this );
		m_pkGradeStatic[i]->InitState( true, true, this );

		m_pkGradeCheckBtn[i]->DisconnectAllEventProcedure();

		int nStringID = 0;

		switch (i) {
			default:
				nStringID = STR_ID_ITEM_GRADE_NORMAL;
				break;
			case 0:
				nStringID = STR_ID_ITEM_GRADE_NORMAL;
				break;
			case 1:
				nStringID = STR_ID_ITEM_GRADE_MAGIC;
				break;
			case 2:
				nStringID = STR_ID_ITEM_GRADE_EPIC;
				break;
			case 3:
				nStringID = STR_ID_ITEM_GRADE_LEGEND;
				break;
		}

		m_pkGradeStatic[i]->SetText( g_pkStrLoader->GetString( nStringID ) );        
	}

	for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i ) 
	{
		m_pkInvenSlot[i]->InitState( true, true, this );
        m_pkInvenSlot[i]->SetMonsterCardSignPos(m_pkMonsterCardContentsDummy->InitMonsterCardSignPos());
     //   m_pkInvenSlot[i]->InitValue();
	}

	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		m_pkMaterialSlot[i]->InitState( true, true, this );
        m_pkMaterialSlot[i]->SetMonsterCardSignPos(m_pkMonsterCardContentsDummy->InitMonsterCardSignPos());
   //     m_pkMaterialSlot[i]->InitValue();
	}

	m_pkGradeCheckBtn[EM_GRADE_NORMAL]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMonsterCardCompose::OnCheckBtn1 );
	m_pkGradeCheckBtn[EM_GRADE_REAR]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMonsterCardCompose::OnCheckBtn2 );
	m_pkGradeCheckBtn[EM_GRADE_EPIC]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMonsterCardCompose::OnCheckBtn3 );
	m_pkGradeCheckBtn[EM_GRADE_REGEOND]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMonsterCardCompose::OnCheckBtn4 );


	m_pkMonsterCardRoulette->InitState( false, true, this );

	m_pkBtnOK->InitState( true, true, this );
	m_pkBtnCancel->InitState( true, true, this );
	m_pkBtnCancel->SetHotKey( DIK_ESCAPE );

    m_pkRectInven->InitState( true, true, this);
    m_pkRectMaterial->InitState( true, true, this);

	m_pkBtnPrevPage->InitState(true, true, this);
	m_pkBtnNextPage->InitState(true, true, this);

	m_pkCardImageOn->InitState(false, false);
    m_pkCardImageOn->SetMonsterCardSignPos(m_pkMonsterCardContentsBigDummy->InitMonsterCardSignPos());
    
	m_pkCardImageOnBack->InitState(false);
 
	m_dwGradeFlag = FLAG_GRADE_ALL;
	m_nIndex = 0;

	InitMeterialPos();

	UpdateItemList();
	UpdateInventory();
}

void KGCMonsterCardCompose::OnCreateComplete( void )
{
	m_dwState = SEL_CARD_STATE;
	m_dwGradeFlag = FLAG_GRADE_ALL;
	m_nIndex = 0;

	m_vecMaterialList.clear();
	m_vecMonsterCardList.clear();

	ResetMeterialPos();

	UpdateItemList();
	UpdateInventory();
	UpdateMaterial();
	UpdateCheckBtn();
}

void KGCMonsterCardCompose::FrameMoveInEnabledState( void )
{
	static GCITEMID _BackitemID = 0;

	switch( m_dwState ) 
	{
	case SEL_CARD_STATE:
		{
			const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

			bool bCheck = false;
			D3DXVECTOR2 vecPos;
			GCITEMID _itemID;
            
            m_pkCardImageOn->AllOff();
			for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i ) 
			{
				if( m_pkInvenSlot[i]->CheckPosInWindowBound( vMousePos ) )
				{
					// 아이템 정보 표시창 위치 보정.
					if ( (int)m_vecMonsterCardList.size() <= m_nIndex + i ) 
						break;

					_itemID = m_vecMonsterCardList[m_nIndex+i].first;
					vecPos = m_pkInvenSlot[i]->GetFixedWindowLocalPos();
					bCheck = true;
                    iCurrentSelectedMonsterCard = i;
				}
			}

			for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
			{
				if( m_pkMaterialSlot[i]->CheckPosInWindowBound( vMousePos ) )
				{
					// 아이템 정보 표시창 위치 보정.
					if ( (int)m_vecMaterialList.size() <= i ) 
						break;

					_itemID = m_vecMaterialList[i];

					// 아이템 정보 표시창 위치 보정.
					vecPos = m_pkMaterialSlot[i]->GetFixedWindowLocalPos();
					bCheck = true;
                    iCurrentSelectedMonsterCard = i;
				}
			}

			if( bCheck )
			{
				D3DXVECTOR2 vecPosBack = vecPos;
				
				vecPos.x -= 81.0f* GC_SCREEN_DIV_WIDTH;
				vecPos.y -= 64.0f * GC_SCREEN_DIV_HEIGHT;

				vecPosBack.x -= 44.0f * GC_SCREEN_DIV_WIDTH;
				vecPosBack.y -= 62.0f * GC_SCREEN_DIV_HEIGHT;

				m_pkCardImageOn->SetMonsterCardItemID( _itemID );
                m_pkCardImageOn->SetMonsterCardContents();
                m_pkCardImageOn->SetMonsterCardDecimalSignOffSet(12.f);
                m_pkCardImageOn->SetMonsterCardSignOffSet(24.f);
                m_pkCardImageOn->SetMonsterCardLVNumScale(0.6f);
                m_pkCardImageOn->SetMonsterCardNumScale(0.7f);
                m_pkCardImageOn->SetMonsterCardPrimeNumScale(0.7f);
                m_pkCardImageOn->SetMonsterCardAllNumShadow(false);

				m_pkCardImageOn->ToggleRender( true );
				m_pkCardImageOnBack->ToggleRender( true );

				//	위치 조정
				m_pkCardImageOn->SetWindowPos( vecPos );
				m_pkCardImageOnBack->SetWindowPos ( vecPosBack );
         
                
				if ( _BackitemID != _itemID ) 
				{
					g_KDSound.Play( "MonsterCardMix01" );
					_BackitemID = _itemID;
				}
			}
			else
			{
				_BackitemID = 0;  
				m_pkCardImageOn->AllOff();
                m_pkCardImageOn->ToggleRender( false );
                m_pkCardImageOnBack->ToggleRender( false );
			} 
		}
		break;
	case MOVE_CARD_STATE:
		{
			_BackitemID = 0;  
			D3DXVECTOR2 vecPos = m_pkMaterialSlot[EM_MATERIAL_SLOT_SIZE/2]->GetFixedWindowLocalPos();

			float fMove = 20.0f;

			bool bCheck = false;

			for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
			{
				D3DXVECTOR2 vecPos2 =  m_pkMaterialSlot[i]->GetFixedWindowLocalPos();

				if ( vecPos2.x != vecPos.x ) 
				{
					bCheck = true;

					float fDiff = abs( vecPos2.x - vecPos.x );

					if ( fDiff > fMove ) 
					{
						if ( vecPos2.x > vecPos.x ) 
						{
							vecPos2.x -= fMove;
						}
						else if ( vecPos2.x < vecPos.x )
						{
							vecPos2.x += fMove;
						}
					}
					else
					{
						vecPos2.x = vecPos.x;
					}


					m_pkMaterialSlot[i]->SetWindowPos( vecPos2 );
				}
			}

			if ( !bCheck ) 
			{
				OpenRoulette();
			}
		}
		break;
	case MIX_CARD_STATE:
		{
		}
		break;
	}
}

void KGCMonsterCardCompose::InitMeterialPos()
{
	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		m_vtFixedWindowPos[i] = m_pkMaterialSlot[i]->GetFixedWindowLocalPos();
	}
}

void KGCMonsterCardCompose::ResetMeterialPos()
{
	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		m_pkMaterialSlot[i]->SetWindowPos( m_vtFixedWindowPos[i] );
		m_pkMaterialSlot[i]->AllOff();
	}
}

void KGCMonsterCardCompose::SetResult( GCITEMID _itemID )
{
	m_pkMonsterCardRoulette->SetResult( _itemID );
	UpdateData();
}

void KGCMonsterCardCompose::SetFailResult()
{
	g_KDSound.Stop( "MonsterCardMix04" );

	UpdateData();
	ButtonLock( false );
	m_pkMonsterCardRoulette->ToggleRender( false );
	m_pkBtnOK->Lock( true );
}

void KGCMonsterCardCompose::OnClickOkCard()
{
	g_KDSound.Stop( "MonsterCardMix04" );

	ButtonLock( false );
	m_pkMonsterCardRoulette->ToggleRender( false );
	m_pkBtnOK->Lock( true );
}


void KGCMonsterCardCompose::UpdateData()
{
	m_dwState = SEL_CARD_STATE;
    m_nPrevIndex = m_nIndex;
	m_nIndex = 0;

	m_vecMaterialList.clear();
	m_vecMonsterCardList.clear();

	UpdateItemList();
    SetPrevIndex();     // 마지막 사용 페이지로 세팅 
	UpdateInventory();
	UpdateMaterial();
	UpdateCheckBtn();
}

void KGCMonsterCardCompose::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
	GCWND_MSG_MAP( m_pkBtnCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	_GCWND_MSG_MAP( m_pkBtnPrevPage,		KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevMove );
	_GCWND_MSG_MAP( m_pkBtnNextPage,		KD3DWnd::D3DWE_BUTTON_CLICK, OnNextMove );

    POINT ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );

	for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i ) 
	{
		if( event.m_pWnd == m_pkInvenSlot[i] && event.m_dwCode == D3DWE_BUTTON_CLICK )
		{
			OnClickInvenSlot(i);
		}
	}

    if( event.m_pWnd == m_pkCardImageOn && event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        if( m_pkRectInven->CheckPosInWindowBoundWithChild( vMousePos ) )
            OnClickInvenSlot(iCurrentSelectedMonsterCard);
        else if( m_pkRectMaterial->CheckPosInWindowBoundWithChild( vMousePos ) )
            OnClickMaterialSlot(iCurrentSelectedMonsterCard);
    }

	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		if( event.m_pWnd == m_pkMaterialSlot[i] && event.m_dwCode == D3DWE_BUTTON_CLICK )
		{
			OnClickMaterialSlot(i);
		}
	}
}

void KGCMonsterCardCompose::ReqMonsterCardMix()
{
	if ( m_vecMaterialList.empty() ) 
		return;


	std::map<GCITEMUID,int> mapList;

	for ( int i = 0; i < (int)m_vecMaterialList.size(); ++ i ) 
	{
		KItem* kItem = g_pItemMgr->GetInventoryItemFromID( m_vecMaterialList[i] );
		if ( !kItem ) 
			return;

		if ( mapList.find( kItem->m_ItemUID ) != mapList.end() )
		{
			mapList[kItem->m_ItemUID] += 1;
		}
		else 
		{
			mapList.insert( std::make_pair( kItem->m_ItemUID, 1 ) );
		}
	}


	KP2P::GetInstance()->Send_MonsterCardMixReq( mapList );
}

void KGCMonsterCardCompose::ButtonLock( bool _bLock )
{
	m_pkBtnOK->Lock(_bLock);
	m_pkBtnCancel->Lock(_bLock);

	m_pkBtnPrevPage->Lock(_bLock);
	m_pkBtnNextPage->Lock(_bLock);

	for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i ) 
	{
		m_pkInvenSlot[i]->Lock( _bLock );
	}

	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i ) 
	{
		m_pkMaterialSlot[i]->Lock( _bLock );
	}
}

void KGCMonsterCardCompose::OpenRoulette()
{
	if ( m_vecMaterialList.empty() ) 
		return;

	std::vector<GCITEMID> vecRewardList;

	g_pItemMgr->GetMonsterCardRewardList( m_vecMaterialList, vecRewardList );

	if ( vecRewardList.empty() )
		return;

	m_pkMonsterCardRoulette->Init();
	m_pkMonsterCardRoulette->SetRewardList( vecRewardList );

	g_KDSound.Play( "MonsterCardMix04", 1 );
	m_pkMonsterCardRoulette->ToggleRender( true );

	m_dwState = MIX_CARD_STATE;

	ResetMeterialPos();
}

void KGCMonsterCardCompose::OnOK()
{
	if ( m_vecMaterialList.empty() ) 
		return;

	m_dwState = MOVE_CARD_STATE;
	ButtonLock( true );
	g_KDSound.Play( "MonsterCardMix03" );
}

void KGCMonsterCardCompose::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}


void KGCMonsterCardCompose::OnCheckBtn1()
{
	ClickCheckBtn( EM_GRADE_NORMAL );
}

void KGCMonsterCardCompose::OnCheckBtn2()
{
	ClickCheckBtn( EM_GRADE_REAR );
}

void KGCMonsterCardCompose::OnCheckBtn3()
{
	ClickCheckBtn( EM_GRADE_EPIC );
}

void KGCMonsterCardCompose::OnCheckBtn4()
{
	ClickCheckBtn( EM_GRADE_REGEOND );
}

void KGCMonsterCardCompose::OnPrevMove()
{
	if( m_nIndex < EM_INVEN_SLOT_SIZE )
		m_nIndex = ( static_cast<int>(m_vecMonsterCardList.size()) - 1 ) / EM_INVEN_SLOT_SIZE * EM_INVEN_SLOT_SIZE;
	else
		m_nIndex-=EM_INVEN_SLOT_SIZE;

	UpdateInventory();
}

void KGCMonsterCardCompose::OnNextMove()
{
	if( m_nIndex + EM_INVEN_SLOT_SIZE >= static_cast<int>(m_vecMonsterCardList.size()) ||
		static_cast<int>(m_vecMonsterCardList.size()) < EM_INVEN_SLOT_SIZE )
		m_nIndex = 0;
	else
		m_nIndex+=EM_INVEN_SLOT_SIZE;

	UpdateInventory();
}

void KGCMonsterCardCompose::ClickCheckBtn( int iCheck )
{
	if ( iCheck < EM_GRADE_NORMAL || iCheck > EM_GRADE_REGEOND )
		return;

	bool bCheck = m_pkGradeCheckBtn[iCheck]->GetCheck();

	m_pkGradeCheckBtn[iCheck]->SetCheck( !bCheck );


	DWORD dwFlag = GetGradeFlag( iCheck );

	if ( bCheck ) 
	{
		m_dwGradeFlag &= ~dwFlag;
	}
	else
	{
		m_dwGradeFlag |= dwFlag;
	}


	// UpdateFlag
	UpdateItemList();
	UpdateInventory();
}

DWORD KGCMonsterCardCompose::GetGradeFlag( int emGrade )
{
	DWORD dwFlag = 0;
	switch( emGrade )
	{
	case EM_GRADE_NORMAL:       dwFlag = FLAG_GRADE_NORMAL;     break;
	case EM_GRADE_REAR:         dwFlag = FLAG_GRADE_REAR;       break;
	case EM_GRADE_EPIC:         dwFlag = FLAG_GRADE_EPIC;       break;
	case EM_GRADE_REGEOND:      dwFlag = FLAG_GRADE_REGEOND;    break;
	default:					dwFlag = 0;
	}

	return dwFlag;
}

bool KGCMonsterCardCompose::IsGradeCheck( char cItemGrade )
{
	DWORD dwFlag = GetGradeFlag( cItemGrade );

	if ( m_dwGradeFlag & dwFlag ) 
		return true;

	return false;
}

void KGCMonsterCardCompose::UpdateItemList()
{
	std::vector< GCItem* > vecMonsterCardList = g_pItemMgr->GetOwnMonsterCardList();

	m_vecMonsterCardList.clear();

	for ( int i = 0; i < (int)vecMonsterCardList.size(); ++i ) 
	{
		if ( !vecMonsterCardList[i] ) 
			continue;

		// 등급 체크
		if ( !IsGradeCheck( vecMonsterCardList[i]->cItemGrade ) ) 
			continue;

		int nDuration = g_pItemMgr->GetInventoryItemDuration( vecMonsterCardList[i]->dwGoodsID );

        if( -1 != nDuration )
		    m_vecMonsterCardList.push_back( std::make_pair( vecMonsterCardList[i]->dwGoodsID, nDuration ));
	}

	std::sort( m_vecMonsterCardList.begin(), m_vecMonsterCardList.end(), MonsterCardGradeSort );
}

void KGCMonsterCardCompose::UpdateInventory()
{
	if ( m_nIndex >= (int)m_vecMonsterCardList.size() )
	{
		m_nIndex = 0;
	}

	for ( int i = 0; i < EM_INVEN_SLOT_SIZE; ++i )
	{
		m_pkInvenSlot[i]->AllOff();
		//m_pkInvenSlot[i]->ShowDuration( false );
	}

	int nSlotIndex = 0;
	for ( int i = m_nIndex; i < m_nIndex + EM_INVEN_SLOT_SIZE; ++i ) 
	{
		if ( i >= (int)m_vecMonsterCardList.size() ) 
			break;
		
        m_pkInvenSlot[nSlotIndex]->SetMonsterCardItemID( m_vecMonsterCardList[i].first );
        m_pkInvenSlot[nSlotIndex]->SetDuration( GetReMainCardNum( i ) );
        m_pkInvenSlot[nSlotIndex]->SetMonsterCardDecimalSignOffSet(5.f);
        m_pkInvenSlot[nSlotIndex]->SetMonsterCardSignOffSet(13.f);
        m_pkInvenSlot[nSlotIndex]->SetMonsterCardContents();
        m_pkInvenSlot[nSlotIndex]->SetMonsterCardAllNumShadow( false );
        

		nSlotIndex++;
	}

	int nSize = m_vecMonsterCardList.size();
	int nCurPage = ( m_nIndex / EM_INVEN_SLOT_SIZE ) + 1;
	int nMaxPage = ((nSize -1) / EM_INVEN_SLOT_SIZE ) + 1;

	m_pkPageNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_MIX_PAGE_NUM, "ii", nCurPage, nMaxPage ));
}

void KGCMonsterCardCompose::UpdateMaterial()
{
	for ( int i = 0; i < EM_MATERIAL_SLOT_SIZE; ++i )
	{
		m_pkMaterialSlot[i]->AllOff();
	}

	for ( int i = 0; i < (int)m_vecMaterialList.size(); ++i )
	{
		if ( i >= EM_MATERIAL_SLOT_SIZE ) 
			break;

        m_pkMaterialSlot[i]->SetMonsterCardItemID( m_vecMaterialList[i]);
        //m_pkMaterialSlot[i]->SetDuration( GetReMainCardNum( i ) );
        m_pkMaterialSlot[i]->ShowDuration(false);
        m_pkMaterialSlot[i]->SetMonsterCardDecimalSignOffSet(5.f);
        m_pkMaterialSlot[i]->SetMonsterCardSignOffSet(13.f);
        m_pkMaterialSlot[i]->SetMonsterCardContents();
        m_pkMaterialSlot[i]->SetMonsterCardAllNumShadow( false );

	}

	if ( EM_MIN_CARD_MIX > (int)m_vecMaterialList.size() )
	{
		m_pkBtnOK->Lock( true );
	}
	else
	{
		m_pkBtnOK->Lock( false );
	}
}

void KGCMonsterCardCompose::UpdateCheckBtn()
{
	for ( int i = 0; i < EM_GRADE_SIZE; ++i ) 
	{
		DWORD dwFlag = GetGradeFlag( i );

		bool bCheck = false;

		if ( m_dwGradeFlag & dwFlag )
			bCheck = true;
		
		m_pkGradeCheckBtn[i]->SetCheck( bCheck );
	}
}

int KGCMonsterCardCompose::GetReMainCardNum( int nIndex )
{
	if ( nIndex >= (int)m_vecMonsterCardList.size() )
		return 0;

	int nCount = m_vecMonsterCardList[nIndex].second;

	int nMaterial = std::count( m_vecMaterialList.begin(), m_vecMaterialList.end(), m_vecMonsterCardList[nIndex].first );

	return nCount - nMaterial;
}

void KGCMonsterCardCompose::OnClickInvenSlot( int nSlot )
{
	int nIndex = m_nIndex + nSlot;	
	
	if ( nIndex >= (int)m_vecMonsterCardList.size() )
		return;

	if ( m_vecMaterialList.size() >= EM_MATERIAL_SLOT_SIZE ) 
		return;

	//	남은 카드 갯수 체크
	if ( GetReMainCardNum( nIndex ) <= 0 )
		return;

	m_vecMaterialList.push_back( m_vecMonsterCardList[nIndex].first );


	g_KDSound.Play( "MonsterCardMix02" );

	UpdateMaterial();
	UpdateInventory();
}

void KGCMonsterCardCompose::OnClickMaterialSlot( int nSlot )
{
	if ( nSlot >= (int)m_vecMaterialList.size() )
		return;

	m_vecMaterialList.erase( m_vecMaterialList.begin() + nSlot );

	g_KDSound.Play( "MonsterCardMix02" );
	
	UpdateMaterial();
	UpdateInventory();
}

void KGCMonsterCardCompose::SetPrevIndex( void )
{
    if( m_nPrevIndex < EM_INVEN_SLOT_SIZE )
        m_nPrevIndex = ( static_cast<int>(m_vecMonsterCardList.size()) - 1 ) / EM_INVEN_SLOT_SIZE * EM_INVEN_SLOT_SIZE;
    else
        m_nPrevIndex-=EM_INVEN_SLOT_SIZE;


    if( m_nPrevIndex + EM_INVEN_SLOT_SIZE >= static_cast<int>(m_vecMonsterCardList.size()) ||
        static_cast<int>(m_vecMonsterCardList.size()) < EM_INVEN_SLOT_SIZE )
        m_nPrevIndex = 0;
    else
        m_nPrevIndex+=EM_INVEN_SLOT_SIZE;

    m_nIndex = m_nPrevIndex;
}
