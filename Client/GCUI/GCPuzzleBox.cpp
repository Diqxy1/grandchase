#include "stdafx.h"
//#include "GCPuzzleBox.h"
#include "GCMyInfoScene.h"

IMPLEMENT_CLASSNAME( KGCPuzzleBox );
IMPLEMENT_WND_FACTORY( KGCPuzzleBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPuzzleBox, "gc_puzzle" );

KGCPuzzleBox::KGCPuzzleBox( void )
{
	LINK_CONTROL( "close_button", m_pkBtnClose );

	LINK_CONTROL( "puzzle_board", m_pkPuzzleBoard );
	LINK_CONTROL( "reward_board", m_pkRewardBoard );

	LINK_CONTROL( "btn_open", m_pkBtnOpen );
	LINK_CONTROL( "btn_buy_package", m_pkBtnBuyPackage );

	LINK_CONTROL( "top_prize_1", m_pkItemWnd[0] );
	LINK_CONTROL( "top_prize_2", m_pkItemWnd[1] );

	m_vecpTextureItem.resize( 9, NULL );
	m_iWaitTimeForShow = 0;
	m_bComplete = false;
}

KGCPuzzleBox::~KGCPuzzleBox( void )
{
}

void KGCPuzzleBox::OnCreate( void )
{
	m_pkBtnClose->SetHotKey( DIK_ESCAPE );

	m_pkBtnClose->InitState(true,true,this);
	m_pkPuzzleBoard->InitState( true );
	m_pkRewardBoard->InitState( true,true, this );

	
	m_pkBtnOpen->InitState( true,true, this );

#if defined( NATION_KOREA )
    m_pkBtnBuyPackage->InitState( true );
    m_pkBtnBuyPackage->SetWindowColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.6f ) );
#else
	m_pkBtnBuyPackage->InitState( true,true, this );
#endif

	{
		KD3DStatic* pStatic;
		pStatic = (KD3DStatic*)GetChildByName("static_1");
		pStatic->SetText( g_pkStrLoader->GetString( 5970 ) );
		pStatic->SetAlign( DT_CENTER );
		pStatic->SetFontOutline( true, 0xff236F80 );

		pStatic = (KD3DStatic*)GetChildByName("static_2");
		pStatic->SetText( g_pkStrLoader->GetString( 5971 ) );
		pStatic->SetAlign( DT_CENTER );
		pStatic->SetFontOutline( true, 0xff236F80 );

		pStatic = (KD3DStatic*)GetChildByName("static_4");
#if !defined( NATION_KOREA ) && !defined(NATION_BRAZIL) 
		pStatic->SetText( g_pkStrLoader->GetString( 5972 ) );
		pStatic->SetAlign( DT_CENTER );
		pStatic->SetFontOutline( true, 0xff236F80 );	
#endif
		pStatic = (KD3DStatic*)GetChildByName("static_3");	
		pStatic->SetTextAutoMultiline( g_pkStrLoader->GetString( 5973 ) );
	}

	KP2P::GetInstance()->Send_PresentboxInfoReq();
	WaitForServer();
	UpdateBoards();
}

void KGCPuzzleBox::ActionPerformed( const KActionEvent& event )
{
	if( m_iWaitTimeForShow )
		return;
    else
        UpdateBoards();
	
	if( KD3DWnd::D3DWE_BUTTON_CLICK != event.m_dwCode )
		return;

	if( m_pkBtnClose == event.m_pWnd )
		SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );

	if( m_pkRewardBoard == event.m_pWnd )
	{
		if( event.m_dwlParam )
		{
			g_KDSound.Play( "72" );
			KP2P::GetInstance()->Send_PresentboxTradeReq( m_pkPuzzleBoard->GetData(static_cast<GCITEMID>( event.m_dwlParam2 )) );
			WaitForServer();
			UpdateBoards();
		}
	}

	if( m_pkBtnOpen == event.m_pWnd )
	{
        D3DXVECTOR2 vPos = m_pkPuzzleBoard->GetCurrentWindowPos();
        vPos.x += static_cast<float>(m_pkPuzzleBoard->GetWidth()/2);
        vPos.y += static_cast<float>(m_pkPuzzleBoard->GetHeight()/2);

        float fPosX = PARTICLEPOS_X(vPos.x);
        float fPosY = PARTICLEPOS_Y(vPos.y);
        
        //g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Wall", fPosX , fPosY, 0.5f) , GC_LAYER_PUZZLEBOX);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_Resserection_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_Resserection_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Open_01",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Open_02",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Open_03",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Open_04",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("2th_Giftbox_Open_05",fPosX , fPosY, 0.5f) , GC_LAYER_CUSTOM);
		g_KDSound.Play( "78" );
		DestroyParticle();
		m_iWaitTimeForShow = 60;
	}

	if( m_pkBtnBuyPackage == event.m_pWnd )
	{
        DWORD dwBuyItemID = 30193;
        GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );
        if( pItem->eMoneyType == EMT_CASH )
        {
#if defined ( USE_VIP_POINT )
            g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
            g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else    
            g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif
        }
        else
        {
            std::wostringstream strmText1;
            std::wostringstream strmText2;
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
            strmText1 << pItem->strItemName;
            strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH )
            strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
            strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
            strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
                strmText1.str(), strmText2.str(),
                KGCUIScene::GC_MBOX_USE_BUY, dwBuyItemID, pItem->iItemTypeValue, true, true );
        }

	}

}

void KGCPuzzleBox::OnDestroy()
{
	g_KDSound.Play( "31" );
	ReleaseCapture();
	g_pkUIScene->m_pkMyInfoScene->UpdateData();

	DestroyParticle();
}

void KGCPuzzleBox::UpdateBoards()
{	
	for ( unsigned int i = 0; i < m_vecpTextureItem.size(); i++ )
	{
		if( m_vecpTextureItem[i] )
			m_vecpTextureItem[i]->Release();
	}
	m_vecpTextureItem.clear();
	
#if defined( NATION_BRAZIL ) || defined( NATION_THAILAND )   //  브라질과 태국은 파티클 안띄운다.
	
#else
	if( m_vecParticle.empty() )
    {
//         D3DXVECTOR2 vPos = m_pkPuzzleBoard->GetCurrentWindowPos();
//         vPos.x += static_cast<float>(m_pkPuzzleBoard->GetWidth()/2);
//         vPos.y += static_cast<float>(m_pkPuzzleBoard->GetHeight()/2);

        D3DXVECTOR2 vPos = GetFixedWindowLocalPos();
        vPos.x += m_pkPuzzleBoard->GetFixedWindowLocalPos().x;
        vPos.y += m_pkPuzzleBoard->GetFixedWindowLocalPos().y;
        vPos.x += static_cast<float>(m_pkPuzzleBoard->GetWidth()/2);
        vPos.y += static_cast<float>(m_pkPuzzleBoard->GetHeight()/2);

        m_vecParticle.push_back( g_ParticleManager->CreateSequence("2th_Giftbox_01",PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), 0.5f) );
        m_vecParticle.push_back( g_ParticleManager->CreateSequence("2th_Giftbox_02",PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), 0.5f) );
	}
#endif

	unsigned int i = 0;
	m_bComplete = true;
	std::map<DWORD, std::vector<KDropItemInfo>>::iterator mit = m_mapTradeList.begin();	
	while( mit != m_mapTradeList.end() )
	{
		KItem* item;
		bool bGot = g_MyD3D->m_kItemMgr.FindInventoryForItemID( mit->first / 10, &item );
		if( bGot )
		{
			WCHAR temp[10];
			int iDur = item->m_nCount;
			swprintf_s( temp, 10, L"%d", iDur );
			((KD3DStatic*)(m_pkPuzzleBoard->GetChild( i )->GetChildByName("static")))->SetText( temp );
			((KD3DStatic*)(m_pkPuzzleBoard->GetChild( i )->GetChildByName("static")))->SetShadow( true );
			((KD3DStatic*)(m_pkPuzzleBoard->GetChild( i )->GetChildByName("static")))->SetAlign( DT_CENTER );
			
		}
		else
		{
			m_bComplete = false;
		}

		m_pkPuzzleBoard->ToggleRender( bGot, i );
		m_pkRewardBoard->Lock( !bGot, i );

		m_pkPuzzleBoard->SetData( mit->first, i );
		m_pkRewardBoard->SetData( mit->second[0].m_ItemID, i );

		m_vecpTextureItem.push_back( g_pItemMgr->CreateItemTexture( m_pkRewardBoard->GetData( i ) / 10 ) );
		
		i++;
		mit++;
	}
	m_pkBtnOpen->Lock( !m_bComplete );

	while( i < 9 )
	{
		m_pkPuzzleBoard->ToggleRender( false, i );
		m_pkRewardBoard->Lock( true, i );
		i++;
	}

	for( unsigned int i = 0; i < m_vecRewardList.size() && i < 2; i++ )
	{
		m_pkItemWnd[i]->SetItemInfo( m_vecRewardList[i].m_ItemID / 10);
	}
	
}

void KGCPuzzleBox::PostChildDraw( void )
{
	std::map<DWORD, std::vector<KDropItemInfo>>::iterator mit = m_mapTradeList.begin();	
	int i = 0;
	while( mit != m_mapTradeList.end() )
	{
		float fOffset = 7.0f;
		if( m_pkRewardBoard->GetWndMode( i ) == D3DWM_HOVER )
			fOffset = 6.0f;

		D3DCOLOR col = 0xffffffff;
		if( m_pkRewardBoard->GetWndMode( i ) == D3DWM_LOCK )
			col = 0x88888888;
        float x,y;
        x = (m_pkRewardBoard->GetCurrentBlockWindowPos( i ).x + fOffset) *m_fWindowScaleX;
        y = (m_pkRewardBoard->GetCurrentBlockWindowPos( i ).y + fOffset) *m_fWindowScaleY;

		g_pItemMgr->Draw_ShopImage( D3DXVECTOR2( x,y ), 
								54.0f *  m_fWindowScaleX , 54.0f *  m_fWindowScaleY, m_vecpTextureItem[i], false,-1, col );
		
		mit++;
		i++;
	}


    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_ParticleManager->Render( GC_LAYER_PUZZLEBOX );
    }
    g_pGCDeviceManager2->PopState();

}

void KGCPuzzleBox::WaitForServer()
{
	m_iOK = -100;
	g_MyD3D->WaitForServerAck( m_iOK, -100, 15000, -99 );
    switch( m_iOK )
	{
    case 1:
        close();
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_ERR_10 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        break;
    case 99:
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
        break;
	}

}

void KGCPuzzleBox::FrameMoveInEnabledState( void )
{
    DrawInfoWindow();

	if( m_iWaitTimeForShow )
	{
		m_iWaitTimeForShow--;
		if( !m_iWaitTimeForShow )
		{
			KP2P::GetInstance()->Send_PresentboxActionReq();		
			WaitForServer();
			UpdateBoards();
		}
	}

	ToggleParticle( g_pkUIScene->GetCurMsgBox() == this && m_bComplete );	
}

void KGCPuzzleBox::ToggleParticle( bool bToggle )
{
	for (unsigned int i = 0 ; i < m_vecParticle.size(); i++ )
	{
        g_ParticleManager->SetLayer( m_vecParticle[i] , bToggle ? GC_LAYER_PUZZLEBOX : GC_LAYER_SKY );
	}
}

void KGCPuzzleBox::DestroyParticle()
{
	for (unsigned int i = 0 ; i < m_vecParticle.size(); i++ )
	{
		g_ParticleManager->DeleteSequence( m_vecParticle[i]	);
	}
	m_vecParticle.clear();
}

void KGCPuzzleBox::FrameMoveInDisabledState( void )
{
	ToggleParticle( false );	
}

void KGCPuzzleBox::DrawInfoWindow()
{
    D3DXVECTOR2 vMousePos = D3DXVECTOR2( static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y) );

    for ( int i = 0; i < 2; ++i ) {
        if( m_pkItemWnd[i]->CheckPosInWindowBound( vMousePos ) ) {
            GCITEMID dwItemID = m_vecRewardList[i].m_ItemID;
            GCItem* kItem = g_pItemMgr->GetItemData( dwItemID / 10 );

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

            D3DXVECTOR2 vecPos = m_pkItemWnd[i]->GetCurrentWindowPos();
            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            break;
        }
    }

    // 퍼즐판 바운드 검사
    if ( m_pkRewardBoard->CheckPosInWindowBound( vMousePos ) == false )
        return;

    // 피스 개당 사이즈 = 퍼즐판의 1/3
    D3DXVECTOR2 vPieceSize;
    vPieceSize.x = static_cast<float>(m_pkRewardBoard->GetWidth()) / 3;
    vPieceSize.y = static_cast<float>(m_pkRewardBoard->GetHeight()) / 3;

    D3DXVECTOR4 vPieceRect;
    KD3DWnd* pkPiece;
    D3DXVECTOR2 vCurrentPos;

    for( UINT i = 0; i < m_mapTradeList.size(); ++i ) {
        pkPiece = m_pkRewardBoard->GetChild(i);
        vCurrentPos = pkPiece->GetFixedWindowPos();

        vPieceRect.x = vCurrentPos.x * g_pGCDeviceManager->GetWindowScaleX();
        vPieceRect.y = vCurrentPos.y * g_pGCDeviceManager->GetWindowScaleY();
        vPieceRect.z = vPieceRect.x + vPieceSize.x * g_pGCDeviceManager->GetWindowScaleX();
        vPieceRect.w = vPieceRect.y + vPieceSize.y * g_pGCDeviceManager->GetWindowScaleY();

        if( CheckPosInPieceBound( vPieceRect, vMousePos ) ) {
            GCItem* kItem = g_pItemMgr->GetItemData( m_pkRewardBoard->GetData( i ) / 10 );

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vCurrentPos );
            break;
        }
    }

}


bool KGCPuzzleBox::CheckPosInPieceBound( D3DXVECTOR4& vPieceRect_, D3DXVECTOR2& vMousePos_ )
{
    if ( vPieceRect_.x <= vMousePos_.x 
        && vPieceRect_.y <= vMousePos_.y 
        && vPieceRect_.z > vMousePos_.x 
        && vPieceRect_.w > vMousePos_.y 
        )
        return true;
    else
        return false;
}

void KGCPuzzleBox::close()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    KGCHotEventBtn::SetHotStateOff();
}
