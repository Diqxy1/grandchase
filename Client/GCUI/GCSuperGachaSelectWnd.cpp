#include "stdafx.h"
#include "GCSuperGachaSelectWnd.h"
#include "GCGachaSelectSlot.h"

IMPLEMENT_CLASSNAME( KGCSuperGachaSelectWnd );
IMPLEMENT_WND_FACTORY( KGCSuperGachaSelectWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCSuperGachaSelectWnd, "gc_gacha_select_wnd" );

KGCSuperGachaSelectWnd::KGCSuperGachaSelectWnd( void )
: m_pkScroll( NULL )
, m_iSelectSlotIndex( 0 )
, m_iSelectVersion( 0 )
, m_SelectPetID( 0 )
, m_iSelectType( 0 )
, m_bFirstTime(false)
{
    LINK_CONTROL( "scroll", m_pkScroll );

    for( int i=0 ; i<NUM_GACHA_SLOT ; ++i ) { 
        m_apkSlot[i] = NULL;

        char strTemp[128];
        sprintf_s( strTemp, 127, "gacha_select_slot%d", i );
        LINK_CONTROL( strTemp, m_apkSlot[i] );
    }

	m_btnCoord = NULL;
	LINK_CONTROL("btn_diamond", m_btnCoord);

	m_btnEquip = NULL;
	LINK_CONTROL("btn_common", m_btnEquip);

    m_vecGachaList.clear();
	m_vecGachaDiamondList.clear();
	m_vecGachaCommonList.clear();
}

KGCSuperGachaSelectWnd::~KGCSuperGachaSelectWnd( void )
{
}

void KGCSuperGachaSelectWnd::OnCreate( void )
{
    m_iSelectSlotIndex = 0;
    m_iSelectVersion = 0;
    m_SelectPetID = 0;
    m_vecGachaList.clear();

    for( int i=0 ; i<NUM_GACHA_SLOT ; ++i ) { 
        m_apkSlot[i]->InitState( true, true, this );
        m_apkSlot[i]->ClearSlot();
    }

    m_pkScroll->InitState( true, true, this );
    m_pkScroll->DisconnectAllEventProcedure();
    m_pkScroll->InitScroll( 0, 1, 0, 1, 2, 2, 1, true );

	m_btnCoord->InitState( true, true, this );
	m_btnCoord->DisconnectAllEventProcedure();
	m_btnCoord->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSuperGachaSelectWnd::OnClickCoord );
	m_btnCoord->Activate(true);

	m_btnEquip->InitState( true, true, this );
	m_btnEquip->Lock(true);
	m_btnEquip->DisconnectAllEventProcedure();
	m_btnEquip->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSuperGachaSelectWnd::OnClickEquip );
}

void KGCSuperGachaSelectWnd::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP(m_pkScroll, OnScroll);
}

void KGCSuperGachaSelectWnd::SetGachaListInfo( const IN std::vector< KGachaNewList >& vecInfo_, int GachaType )
{
    if( !m_vecGachaList.empty() ) { 
        return;
    }

    m_vecGachaList.clear();

	std::vector< KGachaNewList >::const_iterator it = vecInfo_.begin();
    for( ; it != vecInfo_.end() ; ++it ) 
	{
		if(m_bFirstTime == false)
		{
			if(it->m_nTabID == GACHA_TYPE_DIAMOND) {
				m_vecGachaDiamondList.push_back(*it);
			} 
			else {
				m_vecGachaCommonList.push_back(*it);
			}
		}

		if( GachaType == it->m_nTabID )
		{
			if( g_pItemMgr->GetItemData( it->m_iItemID / 10 ) == NULL ) { 
				continue;
			}

			const SPetTemplate petTemplate = g_kGlobalValue.GetPetTemplateFromItemID( it->m_iItemID/10, 0 );
			if( petTemplate.iPetID == -1 ) { 
				continue;
			}
			std::pair< GCITEMID, int > prPetInfo( it->m_iItemID / 10, petTemplate.iNameID );

			KGachaInfo info;
			info.PetInfo = prPetInfo;
			info.GachaType = it->m_nTabID;
			info.GachaID = it->m_nID;

			m_vecGachaList.push_back( info );
		}
    }

	if(m_bFirstTime == false) {
		m_bFirstTime = true;
	}

	if(m_vecGachaDiamondList.empty()) {
		SetGachaListInfo(m_vecGachaDiamondList, GACHA_TYPE_DIAMOND);
		m_btnCoord->ToggleRender(false);
		m_btnEquip->ToggleRender(false);
	}

	if(m_vecGachaCommonList.empty()) {
		SetGachaListInfo(m_vecGachaCommonList, GACHA_TYPE_EQUIP);
		m_btnCoord->ToggleRender(false);
		m_btnEquip->ToggleRender(false);
	}
}

void KGCSuperGachaSelectWnd::InitSelectWnd( void )
{
	for(int i = 0; i < NUM_GACHA_SLOT; i++)
	{
		m_apkSlot[i]->ClearSlot();
	}

    // ����â ���� 
    int iGachaCount = static_cast<int>( m_vecGachaList.size() );
    int iScrollMax = 0;
    int iCount = iGachaCount - NUM_GACHA_SLOT;
    if( iCount > 0 ) { 
        iScrollMax = iCount;
    }

    m_pkScroll->InitScroll( 0, 1, 0, iScrollMax + 1, 2, 2, 1, true );
    OnScroll();

    // �� ó������ ���� ���·�
    std::vector< KGachaInfo >::const_iterator it = m_vecGachaList.begin();
    if( it == m_vecGachaList.end() ) {
        return;
    }
    SetSelectSlot( it->PetInfo.first );
}

void KGCSuperGachaSelectWnd::OnScroll( void )
{
    if( m_vecGachaList.empty() ) { 
        return;
    }

    int iRealindex = m_pkScroll->GetScrollPos();
    int iMaxListView = static_cast<int>( m_vecGachaList.size() ) - iRealindex;

    if( iMaxListView >= NUM_GACHA_SLOT) {
        iMaxListView = NUM_GACHA_SLOT;
    }

    for(int i = 0; i < iMaxListView; ++i ) {
        int iIndex = i + iRealindex;
        if( 0 <= iIndex && iIndex < static_cast<int>( m_vecGachaList.size() ) ) {	
            if( static_cast<int>( m_vecGachaList.size() ) > iIndex) {
                m_apkSlot[i]->SetGachaSelectSlotInfo( m_vecGachaList[iIndex].PetInfo.first, 
                                                      m_vecGachaList[iIndex].PetInfo.second,
                                                      m_vecGachaList[iIndex].GachaID,
													  m_vecGachaList[iIndex].GachaType);
            }
            else {
                m_apkSlot[i]->ClearSlot();
            }
        }
    }
}

void KGCSuperGachaSelectWnd::SetSelectSlot( const GCITEMID petID_ )
{
    bool bSelect = false;
    std::vector< KGachaInfo >::const_iterator it = m_vecGachaList.begin();
    for( ; it != m_vecGachaList.end() ; ++it ) { 
        if( it->PetInfo.first == petID_ ) { 
            m_iSelectVersion = it->GachaID;
            m_SelectPetID = petID_;
			m_iSelectType = it->GachaType;
            bSelect = true;
            break;
        }
    }

    if( bSelect ) { 
        for( int i=0 ; i<NUM_GACHA_SLOT ; ++i ) { 
            if( m_SelectPetID == m_apkSlot[i]->GetPetID() ) { 
                m_iSelectSlotIndex = i;
                m_apkSlot[i]->SetClick( true );
				g_kGlobalValue.m_iCurrentGachaType = m_iSelectType;
                static_cast<KGCSuperGachaDlg*>( this->GetParent() )->SetSelectVersion( m_apkSlot[i]->GetGachaVer() );
            }
            else { 
                m_apkSlot[i]->SetClick( false );
            }
        }
    }
    else { 
        for( int i=0 ; i<NUM_GACHA_SLOT ; ++i ) { 
            m_apkSlot[i]->SetClick( false );
        }
    }
}

void KGCSuperGachaSelectWnd::OnClickCoord()
{
	g_kGlobalValue.m_iCurrentGachaType = 2;
	OnChangeGachaType(GACHA_TYPE_DIAMOND);
	InitSelectWnd();
	m_btnEquip->Lock(false);
	m_btnEquip->Activate(true);
	m_btnCoord->Lock(true);
}

void KGCSuperGachaSelectWnd::OnClickEquip()
{
	g_kGlobalValue.m_iCurrentGachaType = 0;
	OnChangeGachaType(GACHA_TYPE_EQUIP);
	InitSelectWnd();
	m_btnEquip->Lock(true);
	m_btnCoord->Lock(false);
	m_btnCoord->Activate(true);
	
}

void KGCSuperGachaSelectWnd::OnChangeGachaType(int tab)
{
	m_vecGachaList.clear();
	std::vector< KGachaNewList >::const_iterator it, end;

	if(tab == GACHA_TYPE_DIAMOND) {
		it = m_vecGachaDiamondList.begin();
		end = m_vecGachaDiamondList.end();
	} else {
		it = m_vecGachaCommonList.begin();
		end = m_vecGachaCommonList.end();
	}

	for( ; it != end ; ++it ) 
	{
		if( g_pItemMgr->GetItemData( it->m_iItemID / 10 ) == NULL ) { 
			continue;
		}

		const SPetTemplate petTemplate = g_kGlobalValue.GetPetTemplateFromItemID( it->m_iItemID/10, 0 );
		if( petTemplate.iPetID == -1 ) { 
			continue;
		}
		std::pair< GCITEMID, int > prPetInfo( it->m_iItemID / 10, petTemplate.iNameID );

		KGachaInfo info;
		info.PetInfo = prPetInfo;
		info.GachaType = it->m_nTabID;
		info.GachaID = it->m_nID;

		m_vecGachaList.push_back( info );
	}
}