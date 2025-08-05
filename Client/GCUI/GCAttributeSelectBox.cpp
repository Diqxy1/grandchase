#include "stdafx.h"
#include "GCAttributeSelectBox.h"
#include "GCAttributeTable.h"

IMPLEMENT_CLASSNAME( KGCAttributeSelectBox );
IMPLEMENT_WND_FACTORY( KGCAttributeSelectBox );
IMPLEMENT_WND_FACTORY_NAME( KGCAttributeSelectBox, "gc_item_attribute_select" );

KGCAttributeSelectBox::KGCAttributeSelectBox( void )
: m_pkTitleStatic(NULL)
, m_pkDescStatic(NULL)
, m_pkRand(NULL)
, m_pkRandClose(NULL)
, m_pkSelectComplete(NULL)
, m_pkRandComplete(NULL)
, m_iSelectState(STATE_SELECT)
, m_pkItemInfo(NULL)
, m_iMaxSelectCnt(0)
, m_pkNomalStatic(NULL)
, m_pkSpecialStatic(NULL)
, m_pkItemStatic(NULL)
, m_pkItemNumStatic(NULL)
, m_pkClose(NULL)
, m_iPrice(0)
, m_bFirstSelect(true)
, m_bFirstSendPacket(true)
, m_pkInitItemNumStatic( NULL )
, m_pkBtnInitAttribute( NULL )
, m_InitItemID( 0 )
, m_ItemCashType( USE_CASH )
, m_pkBtnAttributeMigrationConfirm( NULL )
, m_pkMainBack( NULL )
, m_pkSubBack( NULL )
, m_pkUseItemlBack( NULL )
, m_pkUseItemTitleBack( NULL )
, m_nTableID( -1 )
{
	char str[MAX_PATH] = {0,};

	for(int i = 0; i < ATTRIBUTE_SELECT_MAX ;i++)
	{
		m_arrSelectCheckBox[i] = NULL;
		sprintf_s( str, "item_attribute_%d", i );
		LINK_CONTROL( str, m_arrSelectCheckBox[i] );
	}

	for(int i = 0; i < ATTRIBUTE_RANDOM_MAX ;i++)
	{
		m_arrRandomCheckBox[i] = NULL;
		sprintf_s( str, "item_rand_attribute_%d", i );
		LINK_CONTROL( str, m_arrRandomCheckBox[i] );
	}
	LINK_CONTROL( "staticattributeselecttype", m_pkTitleStatic );
	LINK_CONTROL( "staticDesc", m_pkDescStatic );
	LINK_CONTROL( "btn_rand",m_pkRand);
	LINK_CONTROL( "btn_rand_close",m_pkRandClose);
	LINK_CONTROL( "btn_complete",m_pkSelectComplete);
	LINK_CONTROL( "btn_rand_complete",m_pkRandComplete);
    LINK_CONTROL( "staticnomal", m_pkNomalStatic );
    LINK_CONTROL( "static_special", m_pkSpecialStatic );
    LINK_CONTROL( "static_item", m_pkItemStatic );
    LINK_CONTROL( "static_item_num", m_pkItemNumStatic );
    LINK_CONTROL( "static_Inititem_num", m_pkInitItemNumStatic );
    LINK_CONTROL( "btn_close", m_pkClose );
    LINK_CONTROL( "btn_init_attribute", m_pkBtnInitAttribute );
    LINK_CONTROL( "btn_attribute_confirm", m_pkBtnAttributeMigrationConfirm );

    LINK_CONTROL( "main_background", m_pkMainBack );
    LINK_CONTROL( "sub_background",  m_pkSubBack );
    LINK_CONTROL( "Item_background", m_pkUseItemlBack );
    LINK_CONTROL( "item_title_back", m_pkUseItemTitleBack );
    
	m_mapSelectInfo.clear();
    m_vecInitAttribute.clear();
}

KGCAttributeSelectBox::~KGCAttributeSelectBox( void )
{
}

void KGCAttributeSelectBox::PostChildDraw( void )
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

        g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );     
    }
    g_pGCDeviceManager2->PopState();

}

void KGCAttributeSelectBox::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkRand && m_bFirstSendPacket)
		{
			OnBtnRandom();
			return;
		}
		else if( event.m_pWnd == m_pkRandClose || event.m_pWnd == m_pkClose )
		{
			OnClose();
			return;
		}
		else if( event.m_pWnd == m_pkSelectComplete)
		{
			OnBtnSelect();
			return;

		}
		else if( event.m_pWnd == m_pkRandComplete && m_bFirstSendPacket)
		{
			OnBtnRandom();  
			return;
		}
        else if ( event.m_pWnd == m_pkBtnInitAttribute )
        {
            OnInitAttributeBtn();
            return;
        }
        else if ( event.m_pWnd == m_pkBtnAttributeMigrationConfirm )
        {
            OnClickAttributeMigration();
            return;
        }
		else
		{
			OnAttributeCheckBoxClick(event);
		}

	}
}

void KGCAttributeSelectBox::OnAttributeCheckBoxClick( const KActionEvent& event )
{
    switch( m_iSelectState ) 
    {
    case STATE_SELECT:
        {
            int cnt = 0;
            for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
            {
                if(m_arrSelectCheckBox[i]->GetCheck()){
                    cnt++;
                }
            }

            for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
            {
                if(m_arrSelectCheckBox[i] == event.m_pWnd ){
                    if(m_iMaxSelectCnt > cnt || m_arrSelectCheckBox[i]->GetCheck() ){
                        m_arrSelectCheckBox[i]->SetCheck(!m_arrSelectCheckBox[i]->GetCheck());
                        if(m_arrSelectCheckBox[i]->GetCheck()== true)
                            cnt++;
                        else
                            cnt--;
                    }
                    if(m_iMaxSelectCnt == cnt)
                        m_pkSelectComplete->Lock(false);
                    else
                        m_pkSelectComplete->Lock(true);
                }	
            }

            return;
        }
        break;
    case STATE_RANDOM:
        {
            return;
        }        
        break;
    case STATE_MIGRATION:
        {
            int iCnt = 0;

            for( int i=0 ; i<ATTRIBUTE_SELECT_MAX ; ++i ) { 
                if( m_arrSelectCheckBox[i]->IsCheckBackRenderOn() && m_arrSelectCheckBox[i]->GetCheck() ) { 
                    ++iCnt;
                }
            }

            for( int i=0 ; i<ATTRIBUTE_SELECT_MAX ; ++i ) { 
                if( m_arrSelectCheckBox[i] == event.m_pWnd ) {
                    if( m_arrSelectCheckBox[i]->IsCheckBackRenderOn() && ( m_iMaxSelectCnt > iCnt || m_arrSelectCheckBox[i]->GetCheck() ) ) { 
                        m_arrSelectCheckBox[i]->SetCheck( !m_arrSelectCheckBox[i]->GetCheck() );
                        if( m_arrSelectCheckBox[i]->GetCheck() ) { 
                            ++iCnt;
                        }
                        else { 
                            --iCnt;
                        }

                        if( m_iMaxSelectCnt == iCnt ) { 
                            m_pkBtnAttributeMigrationConfirm->Lock(false);
                        }
                        else { 
                            m_pkBtnAttributeMigrationConfirm->Lock(true);
                        }
                    }
                }
            }

            return;

        }
        break;
    case STATE_SINGLE_RANDOM:
        {
            bool bCheck = false;

            for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
            {
                if( m_arrSelectCheckBox[i] == event.m_pWnd && m_arrSelectCheckBox[i]->IsCheckBackRenderOn() )
                {
                    m_arrSelectCheckBox[i]->SetCheck(!m_arrSelectCheckBox[i]->GetCheck());
                    
                    if ( m_arrSelectCheckBox[i]->GetCheck() )
                        bCheck = true;

                }
                else
                {
                    m_arrSelectCheckBox[i]->SetCheck( false );
                }
            }

            for( int i = 0; i < ATTRIBUTE_RANDOM_MAX; i++ )
            {
                if( m_arrRandomCheckBox[i] == event.m_pWnd && m_arrRandomCheckBox[i]->IsCheckBackRenderOn() )
                {
                    m_arrRandomCheckBox[i]->SetCheck(!m_arrRandomCheckBox[i]->GetCheck());

                    if ( m_arrRandomCheckBox[i]->GetCheck() )
                        bCheck = true;
                }
                else
                {
                    m_arrRandomCheckBox[i]->SetCheck( false );
                }
            }

            m_pkBtnAttributeMigrationConfirm->Lock( !bCheck );


            return;
        }
        break;
    }    
}

void KGCAttributeSelectBox::OnCreate( void )
{
	for(int i = 0; i < ATTRIBUTE_SELECT_MAX ;i++)
	{
		m_arrSelectCheckBox[i]->InitState(m_arrSelectCheckBox[i]->IsRenderOn(),true,this);
	}
	for(int i = 0; i < ATTRIBUTE_RANDOM_MAX ;i++)
	{
		m_arrRandomCheckBox[i]->InitState(m_arrRandomCheckBox[i]->IsRenderOn(),true,this);
	}
	
	m_pkRand->InitState(m_pkRand->IsRenderOn(),true,this);
	m_pkRandClose->InitState(m_pkRandClose->IsRenderOn(),true,this);
    m_pkSelectComplete->InitState(m_pkSelectComplete->IsRenderOn(),true,this);
	m_pkRandComplete->InitState(m_pkRandComplete->IsRenderOn(),true,this);

    m_pkNomalStatic->InitState(true,false,this);
    m_pkSpecialStatic->InitState(true,false,this);
    m_pkItemStatic->InitState(true,false,this);
    m_pkItemNumStatic->InitState(true,false,this);
    if ( m_pkInitItemNumStatic  != NULL ) {
        m_pkInitItemNumStatic->InitState( true, false, this );
    }
    m_pkClose->InitState(true,true,this);

    m_pkClose->DisconnectAllEventProcedure();
    m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCAttributeSelectBox::OnClose );
    m_pkClose->SetHotKey(DIK_ESCAPE);

    m_pkNomalStatic->SetForceWordWrapText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_1 ), true);
    m_pkNomalStatic->SetToolTip(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_1 ) );
    m_pkNomalStatic->SetAlign(DT_CENTER);
    m_pkNomalStatic->SetFontOutline(true,D3DCOLOR_ARGB( 255, 36, 50, 64));
    m_pkSpecialStatic->SetForceWordWrapText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_2 ), true );
    m_pkSpecialStatic->SetToolTip( g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_2 ) );
    m_pkSpecialStatic->SetAlign(DT_CENTER);
    m_pkSpecialStatic->SetFontOutline(true,D3DCOLOR_ARGB( 255, 36, 50, 64));
    m_pkItemStatic->SetForceWordWrapText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_3 ), true );
    m_pkItemStatic->SetToolTip(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_3 ) );
    m_pkItemStatic->SetAlign(DT_CENTER);
    m_pkItemStatic->SetFontOutline(true,D3DCOLOR_ARGB( 255, 36, 50, 64));
    m_bFirstSelect = true;
    m_bFirstSendPacket = true;

    if ( m_pkBtnInitAttribute != NULL ) {
        m_pkBtnInitAttribute->InitState( false, true, this );
    }

    if( m_pkBtnAttributeMigrationConfirm ) { 
        m_pkBtnAttributeMigrationConfirm->InitState( false, true, this );
    }

    m_pkMainBack->InitState( true, false, NULL );
    m_pkSubBack->InitState( true, false, NULL );
    m_pkUseItemlBack->InitState( true, false, NULL );
    m_pkUseItemTitleBack->InitState( true, false, NULL );
}

void KGCAttributeSelectBox::InitAttributeList()
{
	for(int i = 0; i < ATTRIBUTE_SELECT_MAX ;i++)
	{
		m_arrSelectCheckBox[i]->ToggleRender(false);
		m_arrSelectCheckBox[i]->SetCheck(false);
	}

    for(int i = 0; i < ATTRIBUTE_RANDOM_MAX ;i++)
    {
        m_arrRandomCheckBox[i]->ToggleRender(false);
        m_arrRandomCheckBox[i]->SetCheck(false);
    }
    m_bFirstSendPacket = true;
}

void KGCAttributeSelectBox::SetAttributeInfoSelect( KEVENT_ATTRIBUTE_LIST_ACK arg_ )
{
    m_iSelectState = STATE_SELECT;

    InitAttributeList();
    SetAttributeMigrationUI();

    m_pkTitleStatic->SetText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_5 ));
    m_pkTitleStatic->SetAlign(DT_CENTER);

    m_iPrice = 0;
    if(SetItemInfo(m_dwItemUID) == false)
        return;

    m_pkSelectComplete->Lock(true);

	m_pkItemInfo = NULL;
	m_mapSelectInfo.clear();
	m_dwItemID = 0;
	m_iMaxSelectCnt = arg_.m_nSelectCount;
	SetAttributeList(arg_.m_vecAttributeList, arg_.m_setRandomAttributeIndex);
	SetAttributeBtn();

    if( m_ItemCashType == USE_CASH ) { 
        m_pkDescStatic->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_RANDOM_ATTRIBUTE_7, "i",m_iMaxSelectCnt));
    }
    else { 
        m_pkDescStatic->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_GP_INIT_ATTRBUTE_DESC, "i", m_iMaxSelectCnt));
    }

    if ( m_pkBtnInitAttribute != NULL ) {
        m_pkBtnInitAttribute->ToggleRender( true );
        m_pkBtnInitAttribute->Lock( true );
    }
}

// 속성 테이블 번호를 셋팅한다. ( 서버 패킷 처리용 )
void KGCAttributeSelectBox::SetAttributeTableID ( GCITEMUID dwItemUID, int iTableID_ )
{
    switch( m_iSelectState ) 
    {
    case STATE_MIGRATION:
        {
            SetAttributeMigrationInfo( iTableID_ );
        }
        break;
    case STATE_SINGLE_RANDOM:
        {
            SetAttributeInfoSingle( iTableID_ );
        }
        break;
    default:
        {
            SetAttributeInfoLevel( dwItemUID, iTableID_ );
        }
        break;
    }
}

void KGCAttributeSelectBox::SetAttributeInfoRandom( GCITEMUID dwItemUID, const bool bUseSingle, const bool bUseMigration)
{
    KItem* pKItem = g_pItemMgr->GetInventoryItem(dwItemUID);
    GCItem *pGCItem = g_pItemMgr->GetItemData( pKItem->m_ItemID );

    if ( bUseSingle )
    {
        m_iSelectState = STATE_SINGLE_RANDOM;
    }
    else if ( bUseMigration )
    {
        m_iSelectState = STATE_MIGRATION;
    }

    if( pGCItem->eItemKind == GIK_SELECT_LEVEL_ITEM )
    {
        SetItemUID(dwItemUID);
        Result_Random_Init_attribute = INT_MAX;

        KP2P::GetInstance()->Send_ItemAttributeTableIDReq( dwItemUID );

        g_MyD3D->WaitForServerAck( Result_Random_Init_attribute, 0, 300000);
        return;
    }
    else if ( bUseSingle )
    {
        SetItemUID( dwItemUID );
        SetAttributeInfoSingle( g_pItemMgr->GetAttributeTableID( pKItem->m_ItemID ) );
    }
    else if( bUseMigration ) 
    {    
        SetItemUID( dwItemUID );
        SetAttributeMigrationInfo( g_pItemMgr->GetAttributeTableID( pKItem->m_ItemID ) );
    }
    else
    {
        SetAttributeInfoLevel(dwItemUID, g_pItemMgr->GetAttributeTableID( pKItem->m_ItemID ) );
    }
}

void KGCAttributeSelectBox::SetAttributeInfoLevel( GCITEMUID dwItemUID, int iTableID_ )
{
    m_iSelectState = STATE_RANDOM;

    InitAttributeList();
    SetAttributeMigrationUI();

    m_pkTitleStatic->SetText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_6 ));
    m_pkTitleStatic->SetAlign(DT_CENTER);

    m_iPrice = 1;

    if(dwItemUID == -1) dwItemUID = m_dwItemUID;
    if(SetItemInfo(dwItemUID) == false)
        return;

    KGCAttributeTable kItemAttributeTable;

    m_nTableID = iTableID_;

    if(false == g_pItemMgr->GetAttributeTable(m_nTableID, m_pkItemInfo->m_cGradeID, kItemAttributeTable))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_15   ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        OnClose();
        return;
    }
    
    std::vector<KAttributeInfo> attributelist;
    ConvertList( kItemAttributeTable,  attributelist );
    SetAttributeList( attributelist,  kItemAttributeTable.GetRandomContaner() );
    SetAttributeBtn();
    m_pkDescStatic->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_RANDOM_ATTRIBUTE_8, "i", static_cast<int>(m_pkItemInfo->m_cGradeID) +1));

}

void KGCAttributeSelectBox::SetAttributeList(std::vector<KAttributeInfo> &attributelist ,const std::set<DWORD> &randomlist, const bool bTypeCheck /*= false*/  )
{
	std::vector<KAttributeInfo>::iterator vIter = attributelist.begin();
	for(int i = 0, j = 0;vIter != attributelist.end();vIter++ )
	{
        float absValue = 1.0f;
        if(vIter->m_fValue < 0.0f)
            absValue = -1.0f;

		std::wstring strAttributeType;
		strAttributeType = g_pItemMgr->GetAtrributeString( vIter->m_cType, vIter->m_fValue * absValue);
		
		DWORD staticcolor = GetAttributeStaticColor( vIter->m_cType, vIter->m_fValue, bTypeCheck );
		if(randomlist.find(vIter->m_cType) == randomlist.end() )
		{
			m_arrSelectCheckBox[i]->SetCheck(false);
			m_arrSelectCheckBox[i]->ToggleRender(true);

			m_arrSelectCheckBox[i]->SetWordWrap(strAttributeType);
			
            
            m_arrSelectCheckBox[i]->ToggleCheckBox( m_iSelectState == STATE_SELECT );
			
            // 색상이 다르면 속성이 있다는 뜻이다.
            if( staticcolor != 0x000000 )
            {
				m_arrSelectCheckBox[i]->SetStaticColor(staticcolor);

                if ( m_iSelectState == STATE_SINGLE_RANDOM )
                {
                    m_arrSelectCheckBox[i]->ToggleCheckBox( true );
                    m_mapSelectInfo.insert(std::pair<int,KAttributeInfo>(i, *vIter ));
                }
                else
                {
                    m_arrSelectCheckBox[i]->SetCheck(true);
                }

            }
			else
            {
				m_arrSelectCheckBox[i]->SetStaticColor(D3DCOLOR_ARGB( 255, 255, 186, 0));
            }
			
			
            if(m_iSelectState == STATE_SELECT)
				m_mapSelectInfo.insert(std::pair<int,KAttributeInfo>(i, *vIter ));
			
            i++;
    
		}
		else
		{
			m_arrRandomCheckBox[j]->SetCheck(false);
			m_arrRandomCheckBox[j]->ToggleRender(true);
			m_arrRandomCheckBox[j]->SetStatic(strAttributeType);
            
            m_arrRandomCheckBox[j]->ToggleCheckBox( m_iSelectState == STATE_SELECT );
            m_arrRandomCheckBox[j]->CheckBoxLock( true );

            // 색상이 다르면 속성이 있다는 뜻이다.
            if(staticcolor != 0x000000)
            {
				m_arrRandomCheckBox[j]->SetStaticColor(staticcolor);
                
                if ( m_iSelectState == STATE_SINGLE_RANDOM )
                {
                    m_arrRandomCheckBox[j]->ToggleCheckBox( true );
                    m_arrRandomCheckBox[j]->CheckBoxLock( false );
                    m_mapSelectInfo.insert(std::pair<int,KAttributeInfo>(j+ATTRIBUTE_SELECT_MAX, *vIter ));                                        
                }
                else
                {
                    m_arrRandomCheckBox[j]->SetCheck(true);
                }

            }
			else
            {
				m_arrRandomCheckBox[j]->SetStaticColor(D3DCOLOR_ARGB( 255, 0xfe, 0x4e, 0x4e));
            }

			j++;
		}
	}
    if(m_bFirstSelect == false){
        g_KDSound.Play( "ui_attribute_select_sound" );
    }
}

DWORD KGCAttributeSelectBox::GetAttributeStaticColor( char cType, float fValue, const bool bUseMigration_ /*= false*/ )
{
    if(m_pkItemInfo == NULL) return 0;

    std::vector< KAttributeInfo >::iterator vit = m_vecInitAttribute.begin();
    for( ; vit != m_vecInitAttribute.end() ; ++vit ) { 
        if( bUseMigration_ == false ) {
            if( vit->m_cType == cType  ) { 
                char szBuf1[126] = { 0, };
                char szBuf2[126] = { 0, };

                sprintf_s( szBuf1, 125, "%.2f", vit->m_fValue );
                sprintf_s( szBuf2, 125, "%.2f", fValue );

                float fVal1 = static_cast<float>( atof( szBuf1 ) );
                float fVal2 = static_cast<float>( atof( szBuf2 ) );

                if( fVal1 == fVal2 ) { 
                    m_vecInitAttribute.erase( vit );
                    return D3DCOLOR_ARGB( 255, 156, 255, 0 );
                }
            }
        }
        else {
            if( vit->m_cType == cType ) { 
                m_vecInitAttribute.erase( vit );
                return D3DCOLOR_ARGB( 255, 156, 255, 0 );
            }
        }
    }

    return 0;
}

void KGCAttributeSelectBox::SetAttributeBtn()
{
    if( m_ItemCashType == USE_CASH ) {
        m_pkRand->ToggleRender( m_iSelectState == STATE_SELECT );
        if( m_pkRand->IsRenderOn() ) { 
            m_pkRand->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
        }
    }
    else { 
        m_pkRand->ToggleRender( m_iSelectState == STATE_SELECT );
        if( m_pkRand->IsRenderOn() ) { 
            m_pkRand->SetWndMode( KD3DWnd::D3DWM_LOCK );
        }
    }

    if( m_iSelectState == STATE_SELECT ) { 
        m_pkSelectComplete->SetWndMode( KD3DWnd::D3DWM_LOCK );
    }
    else { 
        m_pkSelectComplete->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
    }

	m_pkSelectComplete->ToggleRender( m_iSelectState == STATE_SELECT );
	m_pkRandClose->ToggleRender( m_iSelectState != STATE_SELECT );
    m_pkRandComplete->ToggleRender( m_iSelectState != STATE_SELECT );
	return;	
}

void KGCAttributeSelectBox::ConvertList( KGCAttributeTable &kItemAttributeTable,std::vector<KAttributeInfo> &attributelist )
{
	const std::vector<KGCAttributeTable::SAttribute>	&vecAttributelist = kItemAttributeTable.GetContainer();

	std::vector<KGCAttributeTable::SAttribute>::const_iterator vIter = vecAttributelist.begin();
	for(;vIter != vecAttributelist.end(); vIter++)
	{
		KAttributeInfo buf;
		buf.m_cType = static_cast<char>(vIter->m_iType);
		buf.m_fValue = vIter->m_fValue;
		attributelist.push_back(buf);
	}
}

void KGCAttributeSelectBox::OnBtnRandom()
{
	if(m_dwItemID == 0 &&  m_iSelectState != STATE_SELECT )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_17  ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
		return;
	}

    if(g_pItemMgr->CheckEquipItem(m_dwItemUID))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP1 ), g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,true,true );
        return;
    }
    
    m_bFirstSelect = false;


    m_bFirstSendPacket = false;

    if( m_ItemCashType == USE_CASH ) { 
        KP2P::GetInstance()->Send_ItemAttributeRandomSelectReq(m_dwItemUID,m_dwItemID);
    }
    else { 
        KP2P::GetInstance()->Send_GPItemAttributeRandomReq( m_dwItemUID, m_dwItemID );
    }

	Result_Random_Init_attribute = INT_MAX;
	g_MyD3D->WaitForServerAck( Result_Random_Init_attribute, 0, 300000, 2 );

}

void KGCAttributeSelectBox::OnBtnSelect()
{
	std::vector<KAttributeInfo>     vecSelectedAttributeInfo;

	for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
	{
		if(m_arrSelectCheckBox[i]->GetCheck()){
			if(m_mapSelectInfo.end() !=m_mapSelectInfo.find(i))
				vecSelectedAttributeInfo.push_back(m_mapSelectInfo.find(i)->second);
		}
	}

    if(g_pItemMgr->CheckEquipItem(m_dwItemUID))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP1 ), g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,true,true );
        return;
    }


	if(m_iMaxSelectCnt == vecSelectedAttributeInfo.size() )
	{
        OnClose();
		KP2P::GetInstance()->Send_AttributeSelectReq(m_dwItemUID, vecSelectedAttributeInfo);
		int temp;
		g_MyD3D->WaitForServerAck( temp, -1, 5000, -1 );
		
		return;
	}
	else
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_SELECT_ATTRIBUTE_OVERLAP_SELECT),
			L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);

	}

	return;
}

void KGCAttributeSelectBox::OnClose()
{
    m_bFirstSelect = true;
    m_iSelectState = STATE_SELECT;
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));	
}

bool KGCAttributeSelectBox::SetItemInfo( GCITEMUID dwItemUID )
{
    m_pkItemInfo = g_pItemMgr->GetInventoryItem(dwItemUID);

    if(m_pkItemInfo == NULL)
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_15   ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        OnClose();
        return false;
    }

    m_dwItemUID = dwItemUID;

    m_vecInitAttribute.clear();
    m_vecInitAttribute = m_pkItemInfo->m_vecAttribute;

    GCItem* pItemData = g_pItemMgr->GetItemData( m_pkItemInfo->m_ItemID );
    if( pItemData == NULL ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_15   ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        OnClose();
        return false;
    }
    m_ItemCashType = ( pItemData->eMoneyType == EMT_CASH ? USE_CASH : USE_GP );

    if ( m_pkBtnInitAttribute != NULL ) {
        m_pkBtnInitAttribute->ToggleRender( true );

        if ( m_ItemCashType == USE_CASH || 
            m_iSelectState == STATE_SELECT ||
            m_iSelectState == STATE_SINGLE_RANDOM )
        {
            m_pkBtnInitAttribute->Lock( true );
        }
        else
        {
            m_pkBtnInitAttribute->Lock( false );
        }   
    } 

    if( m_ItemCashType == USE_CASH ) 
    { 
        if( m_iSelectState == STATE_SINGLE_RANDOM )
        {
            int iRandomItemNum = GetSingleRandomItemCount();
            m_pkItemNumStatic->SetForceWordWrapText(g_pkStrLoader->GetReplacedString(STR_ID_SINGLE_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) , true );
            m_pkItemNumStatic->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_SINGLE_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) );
        }
        else
        {
            int iRandomItemNum = GetRandomItemCount();
            
            m_pkItemNumStatic->SetForceWordWrapText(g_pkStrLoader->GetReplacedString(STR_ID_RANDOM_ATTRIBUTE_4, "ii",m_iPrice, iRandomItemNum ) , true );
            m_pkItemNumStatic->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_RANDOM_ATTRIBUTE_4, "ii",m_iPrice, iRandomItemNum ) );
        }
    }
    else { 

        if( m_iSelectState == STATE_SINGLE_RANDOM )
        {
            int iRandomItemNum = GetSingleRandomItemCount();
            m_pkItemNumStatic->SetForceWordWrapText(g_pkStrLoader->GetReplacedString(STR_ID_SINGLE_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) , true );
            m_pkItemNumStatic->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_SINGLE_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) );

        }
        else
        {
            int iRandomItemNum = GetGPRandomItemCount();
            int iInitItemNum = GetInitRandomItemCount();

            m_pkItemNumStatic->SetForceWordWrapText(g_pkStrLoader->GetReplacedString(STR_ID_GP_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) , true );
            m_pkItemNumStatic->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_GP_RANDOM_ATTRIBUTE_ITEM, "ii",m_iPrice, iRandomItemNum ) );
            
            if ( m_pkInitItemNumStatic != NULL ) {
                m_pkInitItemNumStatic->SetForceWordWrapText(g_pkStrLoader->GetReplacedString(STR_ID_GP_INIT_ATTRIBUTE_ITEM, "ii",m_iPrice, iInitItemNum ), true );
                m_pkInitItemNumStatic->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_GP_INIT_ATTRIBUTE_ITEM, "ii",m_iPrice, iInitItemNum ));
            }
        }
    }

    return true;
}

void KGCAttributeSelectBox::OnInitAttributeBtn( void )
{
    if(m_InitItemID == 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_GP_INIT_ATTRBUTE_ITEM_ERR  ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        return;
    }

    if(g_pItemMgr->CheckEquipItem(m_dwItemUID))
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP1 ), g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,true,true );
        return;
    }

    Result_Random_Init_attribute = INT_MAX;
    KP2P::GetInstance()->Send_InitGPItemAttributeReq( m_dwItemUID, m_InitItemID );
    g_MyD3D->WaitForServerAck( Result_Random_Init_attribute, 0, 300000, 2 );
}

void KGCAttributeSelectBox::SetAttributeInfoSingle( const int iTableID_ )
{
    InitAttributeList();
    SetAttributeMigrationUI();

    m_iPrice = 1;

    if( SetItemInfo(m_dwItemUID) == false ) { 
        return;
    }

    m_pkTitleStatic->SetText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_6 ));
    m_pkTitleStatic->SetAlign(DT_CENTER);

    m_pkItemInfo = g_pItemMgr->GetInventoryItem( m_dwItemUID );
    if( m_pkItemInfo == NULL ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_15   ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        OnClose();
        return;
    }

    if( false == SetAttributeListSingle( iTableID_ ) ) { 
        return;
    }

    m_pkDescStatic->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SINGLE_RANDOM_ATTRIBUTE_DESC ) );
}

void KGCAttributeSelectBox::SetAttributeMigrationInfo( const int iTableID_ /*= -1*/ )
{
    InitAttributeList();
    SetAttributeMigrationUI();

    if( SetItemInfo(m_dwItemUID) == false ) { 
        return;
    }

    m_pkTitleStatic->SetText(g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_6 ));
    m_pkTitleStatic->SetAlign(DT_CENTER);

    m_pkItemInfo = g_pItemMgr->GetInventoryItem( m_dwItemUID );
    if( m_pkItemInfo == NULL ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_RANDOM_ATTRIBUTE_15   ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,false,true );
        OnClose();
        return;
    }

    m_iPrice = 0;

    if( false == SetAttributeMigrationList( iTableID_ ) ) { 
        return;
    }
    m_pkDescStatic->SetTextAutoMultiline(g_pkStrLoader->GetReplacedString(STR_ID_GP_INIT_ATTRBUTE_DESC, "i", m_iMaxSelectCnt));
}

bool KGCAttributeSelectBox::SetAttributeListSingle( const int iTableID_ )
{
    m_mapSelectInfo.clear();

    // 현재 아이템의 전채 속성 가져옴 
    m_nTableID = iTableID_ == -1 ? g_pItemMgr->GetAttributeTableID( m_pkItemInfo->m_ItemID ) : iTableID_;
    KGCAttributeTable kItemAttributeTable;
    if( false == g_pItemMgr->GetAttributeTable( m_nTableID, m_pkItemInfo->m_cGradeID, kItemAttributeTable ) ) {
        return false ;
    }

    // 속성 UI 세팅 표시 
    std::vector< KAttributeInfo > vecAllAttribute;
    ConvertList( kItemAttributeTable, vecAllAttribute );
    SetAttributeList( vecAllAttribute, kItemAttributeTable.GetRandomContaner(), true );

    return true;    

}

bool KGCAttributeSelectBox::SetAttributeMigrationList( const int iTableID_ /*= -1*/ )
{
    m_SetEnableMigration.clear();
    m_vecMigrationItemAttribute.clear();
    m_mapSelectInfo.clear();

    // 아이템의 현재 속성 정보와 선택 가능한 속성 리스트 얻어 옴 
    if( false == g_kGlobalValue.GetMigrationChangeEnableAttributeList( m_SetEnableMigration ) ||
        false == g_kGlobalValue.GetMigrationItemCurrentAttribute( m_vecMigrationItemAttribute ) ) { 
            return false;
    }

    // 변경 가능한 속성 수 
    m_iMaxSelectCnt = 0;
    for( std::vector< KAttributeInfo >::const_iterator it = m_vecMigrationItemAttribute.begin() ; it != m_vecMigrationItemAttribute.end() ; ++it ) { 
        if( m_SetEnableMigration.end() != m_SetEnableMigration.find( it->m_cType ) ) { 
            ++m_iMaxSelectCnt;
        }
    }

    // 현재 아이템의 전채 속성 가져옴 
    m_nTableID = iTableID_ == -1 ? g_pItemMgr->GetAttributeTableID( m_pkItemInfo->m_ItemID ) : iTableID_;
    KGCAttributeTable kItemAttributeTable;
    if( false == g_pItemMgr->GetAttributeTable( m_nTableID, m_pkItemInfo->m_cGradeID, kItemAttributeTable ) ) {
        return false ;
    }

    // 속성 UI 세팅 표시 
    std::vector< KAttributeInfo > vecAllAttribute;
    ConvertList( kItemAttributeTable, vecAllAttribute );
    SetAttributeList( vecAllAttribute, kItemAttributeTable.GetRandomContaner(), true );

    if( m_iMaxSelectCnt == 0 ) { 
        return true;
    }

    // 현재 아이템이 가진 속성 중 변환 할수 있는것 없는것 세팅 
    std::set<DWORD> setRandomAttribute = kItemAttributeTable.GetRandomContaner();
    std::vector< KAttributeInfo >::const_iterator vitAllAttribute = vecAllAttribute.begin();
    for( int iSel=0, iRan=0  ; vitAllAttribute != vecAllAttribute.end() ; ++vitAllAttribute ) { 
        // 선택 속성 처리 - 공방생 등..
        if( setRandomAttribute.find( vitAllAttribute->m_cType ) == setRandomAttribute.end() ) { 
            if( m_SetEnableMigration.find( vitAllAttribute->m_cType ) != m_SetEnableMigration.end() ) { 
                m_arrSelectCheckBox[iSel]->SetCheck( false );
                m_arrSelectCheckBox[iSel]->ToggleRender( true );
                m_arrSelectCheckBox[iSel]->ToggleCheckBox( true );
                m_arrSelectCheckBox[iSel]->SetStaticColor( D3DCOLOR_ARGB( 255, 255, 186, 0 ) );
                m_mapSelectInfo.insert(std::pair<int,KAttributeInfo>(iSel, *vitAllAttribute ));
            }

            ++iSel;
        }
        // 랜덤 속성 처리 - 헬스피어 등...
        else { 
            if( m_SetEnableMigration.find( vitAllAttribute->m_cType ) != m_SetEnableMigration.end() ) { 
                m_arrRandomCheckBox[iRan]->SetCheck( false );
                m_arrRandomCheckBox[iRan]->ToggleRender( true );
                m_arrRandomCheckBox[iRan]->ToggleCheckBox( true );
                m_arrRandomCheckBox[iRan]->SetStaticColor( D3DCOLOR_ARGB( 255, 255, 186, 0 ) );
                m_mapSelectInfo.insert(std::pair<int,KAttributeInfo>(iRan, *vitAllAttribute ));
            }

            ++iRan;
        }
    }

    return true;
}

void KGCAttributeSelectBox::OnClickAttributeMigration( void )
{
    if ( m_iSelectState == STATE_MIGRATION )
    {
        // 선택 속성 리스트 만들기 
        std::vector< KAttributeInfo > vecAddAttributeInfo;
        for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
        {
            if( m_arrSelectCheckBox[i]->GetCheck() ) {
                if(m_mapSelectInfo.end() !=m_mapSelectInfo.find(i))
                    vecAddAttributeInfo.push_back(m_mapSelectInfo.find(i)->second);
            }
        }

        if( g_pItemMgr->CheckEquipItem(m_dwItemUID) ) {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP1 ), g_pkStrLoader->GetString( STR_ID_ALL_CHAR_UNEQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,true,true );
            return;
        }

        // 삭제 속성 리스트 만들기 
        // 원래 있던 속성 중 선택 되지 않은 것은 지워져야 할 속성 
        std::vector< KAttributeInfo > vecDelAttributeInfo = m_vecMigrationItemAttribute;
        std::vector< KAttributeInfo >::iterator vitDel = vecDelAttributeInfo.begin();
        for( ; vitDel != vecDelAttributeInfo.end() ;  ) { 

            bool bErase = false;

            // 원래 속성에서 선택 속성 제외 
            for( std::vector< KAttributeInfo >::const_iterator vitAdd = vecAddAttributeInfo.begin() ; vitAdd != vecAddAttributeInfo.end() ; ++vitAdd ) { 
                if( vitAdd->m_cType == vitDel->m_cType ) { 
                    bErase = true;
                    break;
                }
            }

            // 원래 속성에서 선택 가능하지 않은것 제외 
            if( m_SetEnableMigration.find( vitDel->m_cType ) == m_SetEnableMigration.end() ) {
                bErase = true;
            }

            if( bErase ) { 
                vitDel = vecDelAttributeInfo.erase( vitDel );
            }
            else { 
                ++vitDel;
            }
        }

        if( m_iMaxSelectCnt == vecAddAttributeInfo.size() ) { 
            OnClose();
            Result_SelectAttribute = INT_MAX;
            KP2P::GetInstance()->Send_AttributeMigrationSelectReq( m_dwItemUID, vecAddAttributeInfo, vecDelAttributeInfo );
            g_MyD3D->WaitForServerAck( Result_SelectAttribute, INT_MAX, 5000, TIME_OUT_VALUE );
            return;
        }
        else
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_SELECT_ATTRIBUTE_OVERLAP_SELECT),
                L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);

        }
    }
    else if ( m_iSelectState == STATE_SINGLE_RANDOM )
    {
        KAttributeInfo kAttributeInfo;
        bool bCheck = false;

        for( int i = 0; i < ATTRIBUTE_SELECT_MAX; i++ )
        {
            if( m_arrSelectCheckBox[i]->GetCheck() ) 
            {
                std::map<int,KAttributeInfo>::iterator iter = m_mapSelectInfo.find( i );
                if( m_mapSelectInfo.end() != iter )
                {
                    bCheck = true;
                    kAttributeInfo = iter->second;                   

                    break;                    
                }                
            }
        }

        for( int i = 0; i < ATTRIBUTE_RANDOM_MAX; i++ )
        {
            if( m_arrRandomCheckBox[i]->GetCheck() ) 
            {
                std::map<int,KAttributeInfo>::iterator iter = m_mapSelectInfo.find( i+ATTRIBUTE_SELECT_MAX );
                if( m_mapSelectInfo.end() != iter )
                {
                    bCheck = true;
                    kAttributeInfo = iter->second;

                    break;
                }
            }
        }

        if ( bCheck )
        {
            for ( int i = 0; i < (int)m_pkItemInfo->m_vecAttribute.size(); ++i ) 
            {
                if ( m_pkItemInfo->m_vecAttribute[i].m_cType == kAttributeInfo.m_cType )
                {
                    KP2P::GetInstance()->Send_SingleRandomAttributeRandomReq( 
                        m_pkItemInfo->m_vecAttribute[i].m_cSlotID, m_dwItemUID, m_dwItemID * 10 );

                    Result_Random_Init_attribute = INT_MAX;
                    g_MyD3D->WaitForServerAck( Result_Random_Init_attribute, 0, 300000, 2 );

                    return;
                } 
            }      
        }

        return;

    }

    return;
}

void KGCAttributeSelectBox::SetAttributeMigrationUI()
{
    switch( m_iSelectState )
    {
    case STATE_SELECT:
    case STATE_RANDOM:
        {
            // 아이템 소비 표시 UI 모두 켬
            m_pkUseItemlBack->ToggleRender( true );
            m_pkUseItemTitleBack->ToggleRender( true );
            m_pkItemStatic->ToggleRender( true );
            m_pkItemNumStatic->ToggleRender( true );
            m_pkInitItemNumStatic->ToggleRender( true );

            // 기존 속성 변환 버튼 모두 켬 
            m_pkRandClose->ToggleRender( true );
            m_pkRandComplete->ToggleRender( true );
            m_pkSelectComplete->ToggleRender( true );
            m_pkRand->ToggleRender( true );
            m_pkBtnInitAttribute->ToggleRender( true );
            m_pkBtnAttributeMigrationConfirm->ToggleRender( false );

            // UI 크기 조정
            m_pkMainBack->SetHeight( UI_MAIN_FRAME_HEIGHT );
            m_pkSubBack->SetHeight( UI_SUB_FRAME_HEIGHT );

            D3DXVECTOR2 dxvDescPos = m_pkDescStatic->GetFixedWindowLocalPos();
            dxvDescPos.y = UI_DESC_Y_POS;
            m_pkDescStatic->SetFixedWindowLocalPos( dxvDescPos );

            D3DXVECTOR2 dxvBtnPos = m_pkBtnAttributeMigrationConfirm->GetFixedWindowLocalPos();
            dxvBtnPos.y = UI_MIG_CONFIRM_Y_POS;
            m_pkBtnAttributeMigrationConfirm->SetFixedWindowLocalPos( dxvBtnPos );
        }
        break;
    case STATE_MIGRATION:
        {
            // 아이템 소비 표시 UI 모두 끔 
            m_pkUseItemlBack->ToggleRender( false );
            m_pkUseItemTitleBack->ToggleRender( false );
            m_pkItemStatic->ToggleRender( false );
            m_pkItemNumStatic->ToggleRender( false );
            m_pkInitItemNumStatic->ToggleRender( false );

            // 기존 속성 변환 버튼 모두 끔 
            m_pkRandClose->ToggleRender( false );
            m_pkRandComplete->ToggleRender( false );
            m_pkSelectComplete->ToggleRender( false );
            m_pkRand->ToggleRender( false );
            m_pkBtnInitAttribute->ToggleRender( false );
            m_pkBtnAttributeMigrationConfirm->ToggleRender( true );
            m_pkBtnAttributeMigrationConfirm->Lock( true );

            // UI 크기 조정
            m_pkMainBack->SetHeight( UI_MAIN_FRAME_HEIGHT - UI_HEIGHT_OFFSET );
            m_pkSubBack->SetHeight( UI_SUB_FRAME_HEIGHT - UI_HEIGHT_OFFSET );

            D3DXVECTOR2 dxvDescPos = m_pkDescStatic->GetFixedWindowLocalPos();
            dxvDescPos.y = UI_DESC_Y_POS - UI_HEIGHT_OFFSET;
            m_pkDescStatic->SetFixedWindowLocalPos( dxvDescPos );

            D3DXVECTOR2 dxvBtnPos = m_pkBtnAttributeMigrationConfirm->GetFixedWindowLocalPos();
            dxvBtnPos.y = UI_MIG_CONFIRM_Y_POS - UI_HEIGHT_OFFSET;
            m_pkBtnAttributeMigrationConfirm->SetFixedWindowLocalPos( dxvBtnPos );
        }
        break;
    case STATE_SINGLE_RANDOM:
        {
            // 아이템 소비 표시 UI 모두 켬 
            m_pkUseItemlBack->ToggleRender( true );
            m_pkUseItemTitleBack->ToggleRender( true );
            m_pkItemStatic->ToggleRender( true );
            m_pkItemNumStatic->ToggleRender( true );
            m_pkInitItemNumStatic->ToggleRender( false );

            // 기존 속성 변환 버튼 모두 끔 
            m_pkRandClose->ToggleRender( false );
            m_pkRandComplete->ToggleRender( false );
            m_pkSelectComplete->ToggleRender( false );
            m_pkRand->ToggleRender( false );
            m_pkBtnInitAttribute->ToggleRender( false );

            m_pkBtnAttributeMigrationConfirm->ToggleRender( true );
            m_pkBtnAttributeMigrationConfirm->Lock( true );

            // UI 크기 조정
            m_pkMainBack->SetHeight( UI_MAIN_FRAME_HEIGHT );
            m_pkSubBack->SetHeight( UI_SUB_FRAME_HEIGHT );

            D3DXVECTOR2 dxvDescPos = m_pkDescStatic->GetFixedWindowLocalPos();
            dxvDescPos.y = UI_DESC_Y_POS;
            m_pkDescStatic->SetFixedWindowLocalPos( dxvDescPos );

            D3DXVECTOR2 dxvBtnPos = m_pkBtnAttributeMigrationConfirm->GetFixedWindowLocalPos();
            dxvBtnPos.y = UI_MIG_CONFIRM_Y_POS;
            m_pkBtnAttributeMigrationConfirm->SetFixedWindowLocalPos( dxvBtnPos );
        }
        break;
    }
}

int KGCAttributeSelectBox::GetRandomItemCount()
{
    int iRandomItemNum = 0;

    if(false == g_pItemMgr->CheckRandomInitItemtoInventory(m_dwItemID, static_cast<int>(m_pkItemInfo->m_cGradeID))) {
        m_dwItemID = 0;
    }
    else
    {
        KItem * pItem = NULL;
        if(true == g_pItemMgr->FindInventoryForItemID(m_dwItemID, &pItem))
            iRandomItemNum = pItem->m_nCount;

    }

    return iRandomItemNum;
}

int KGCAttributeSelectBox::GetGPRandomItemCount()
{
    int iRandomItemNum = 0;

    // 랜덤 아이템 
    if( false == g_pItemMgr->CheckGPAttributeRandomItemFromInventory( m_dwItemID, static_cast<int>( m_pkItemInfo->m_cGradeID ) ) ) { 
        m_dwItemID = 0;
    }
    else { 
        KItem * pItem = NULL;
        if(true == g_pItemMgr->FindInventoryForItemID(m_dwItemID, &pItem)) {
            iRandomItemNum = pItem->m_nCount;
        }
    }

    return iRandomItemNum;
}

int KGCAttributeSelectBox::GetInitRandomItemCount()
{
    int iInitItemNum = 0;

    // 초기화 아이템 
    if( false == g_pItemMgr->CheckGPAttributeInitItemFromInventory( m_InitItemID, static_cast<int>( m_pkItemInfo->m_cGradeID ) ) ) { 
        m_InitItemID = 0;
    }
    else { 
        KItem * pItem = NULL;
        if(true == g_pItemMgr->FindInventoryForItemID(m_InitItemID, &pItem)) {
            iInitItemNum = pItem->m_nCount;
        }
    }

    return iInitItemNum;
}

int KGCAttributeSelectBox::GetSingleRandomItemCount()
{
    int iRandomItemNum = 0;     

    int iItemLevel = m_pkItemInfo->m_sEquipLevel;
    if( iItemLevel == 0 || iItemLevel == -1 )
    {
        GCItem* pItem = g_pItemMgr->GetItemData(m_pkItemInfo->m_ItemID);
        if( pItem )
        {
            iItemLevel = pItem->iNeedLevelForEquippingItem;
        }
    }

    // 초기화 아이템 
    if( false == g_pItemMgr->CheckAttributeSingleRandomItemFromInventory( m_dwItemID, iItemLevel ) ) { 
        m_dwItemID = 0;
    }
    else { 
        KItem * pItem = NULL;
        if(true == g_pItemMgr->FindInventoryForItemID(m_dwItemID, &pItem)) {
            iRandomItemNum = pItem->m_nCount;
        }
    }

    return iRandomItemNum;
}

bool KGCAttributeSelectBox::GetSelectSlot( int nSlotID, int& nSelect, bool& bRandom )
{
    KAttributeInfo kAttribute;
    bool bCheck = false;

    for ( int i = 0; i < (int)m_pkItemInfo->m_vecAttribute.size(); ++i ) 
    {
        if ( m_pkItemInfo->m_vecAttribute[i].m_cSlotID == nSlotID )
        {
            kAttribute = m_pkItemInfo->m_vecAttribute[i];

            bCheck = true;

            break;
        }
    }

    if ( !bCheck )
        return false;

    KGCAttributeTable kItemAttributeTable;
    if( false == g_pItemMgr->GetAttributeTable( m_nTableID, m_pkItemInfo->m_cGradeID, kItemAttributeTable ) ) {
        return false;
    }

    // 속성 UI 세팅 표시 
    std::vector< KAttributeInfo > vecAllAttribute;
    ConvertList( kItemAttributeTable, vecAllAttribute );

    const std::set<DWORD> &randomlist = kItemAttributeTable.GetRandomContaner();

    std::vector<KAttributeInfo>::iterator vIter = vecAllAttribute.begin();
    for(int i = 0, j = 0; vIter != vecAllAttribute.end();vIter++ )
    {
        if ( vIter->m_cType == kAttribute.m_cType && vIter->m_fValue == kAttribute.m_fValue )
        {
            if( randomlist.find(vIter->m_cType) == randomlist.end() )
            {
                nSelect = i;
            }
            else
            {
                bRandom = true;
                nSelect = j;
            }

            return true;
        }

        if(randomlist.find(vIter->m_cType) == randomlist.end() )
        {
            i++;
        }
        else
        {
            j++;
        }
    }

    return false;
}

void KGCAttributeSelectBox::SetResultSingle( int nSlotID )
{   
    int nSelect = -1;
    bool bRandom = false;

    D3DXVECTOR2 vtSelectPos;
    bool bOK = GetSelectSlot( nSlotID,nSelect, bRandom );

    if ( !bOK )
        return;

    if ( bRandom )
    {
        m_arrRandomCheckBox[nSelect]->SetCheck( true );

        // 파티클 출력
        vtSelectPos = m_arrRandomCheckBox[nSelect]->GetFixedWindowPos();
    }
    else
    {
        m_arrSelectCheckBox[nSelect]->SetCheck( true );

        // 파티클 출력
        vtSelectPos = m_arrSelectCheckBox[nSelect]->GetFixedWindowPos();
    }

    m_pkBtnAttributeMigrationConfirm->Lock( false );

    float fPosX = 90.0f;
    float fPosY = 8.0f;

//     KLuaManager luaMgr;
//     GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
//     LUA_GET_VALUE_DEF( "PosX", fPosX, 0.0f );
//     LUA_GET_VALUE_DEF( "PosY", fPosY, 0.0f );

    vtSelectPos.x += fPosX;
    vtSelectPos.y += fPosY;


    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Change_Sok_01", PARTICLEPOS_X(vtSelectPos.x), PARTICLEPOS_Y(vtSelectPos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Change_Sok_02", PARTICLEPOS_X(vtSelectPos.x), PARTICLEPOS_Y(vtSelectPos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Change_Sok_03", PARTICLEPOS_X(vtSelectPos.x), PARTICLEPOS_Y(vtSelectPos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Change_Sok_04", PARTICLEPOS_X(vtSelectPos.x), PARTICLEPOS_Y(vtSelectPos.y), 0.5f ), GC_LAYER_UPPER_SQUARE_UI_0);
}