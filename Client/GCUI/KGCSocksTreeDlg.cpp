#include "stdafx.h"
#include "KGCSocksTreeDlg.h"

#include <boost/lexical_cast.hpp>

extern int Result_CheckEnoughSocksMaterial;

IMPLEMENT_CLASSNAME( KGCSocksTreeDlg );
IMPLEMENT_WND_FACTORY( KGCSocksTreeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSocksTreeDlg, "gc_socks_tree_dlg" );

EGCRenderLayer	egcRenderLayer = GC_LAYER_UI;

enum
{
    GOODSID_OF_SOCKS_RED		=	451100,
    GOODSID_OF_SOCKS_BLUE		=	451110,
    GOODSID_OF_SOCKS_YELLOW		=	451120,
};

KGCSocksTreeDlg::~KGCSocksTreeDlg( void ){}
KGCSocksTreeDlg::KGCSocksTreeDlg( void )
{
    //	sample code
    LINK_CONTROL("btnClose",					m_pkBtnClose);
    LINK_CONTROL("btn_hangup_socks_all",		m_pkBtnHangupSocksAll);
    LINK_CONTROL("btn_get_socks_all",			m_pkBtnGetSocksAll);
    LINK_CONTROL("static_info_main",			m_pkStaticInfoMain);

    //LINK_CONTROL("static_socks_name_red",			m_pkStaticNameRed);
    //LINK_CONTROL("static_socks_name_blue",			m_pkStaticNameBlue);
    //LINK_CONTROL("static_socks_name_yellow",			m_pkStaticNameYellow);

    m_mapSocksTreeDlgProperty[GOODSID_OF_SOCKS_RED].InitData( std::string("red") );
    m_mapSocksTreeDlgProperty[GOODSID_OF_SOCKS_BLUE].InitData( std::string("blue") );
    m_mapSocksTreeDlgProperty[GOODSID_OF_SOCKS_YELLOW].InitData( std::string("yellow") );

    //LINK_CONTROL("title",			m_pkTitle );

    this->LinkControlSocks();
}


/*[ On~Func() ]
========================================================================*/
//	처음 접속시 걸린 양말 정보를 준다.
void KGCSocksTreeDlg::OnEventSocksInfoNot( const KUserSocksInfo& kUserSocksInfo_ )
{
    this->SyncData( kUserSocksInfo_ );
    this->CheckGetSome();
}

//	양말 걸기 리퀘에 대한 응답
void KGCSocksTreeDlg::OnEventSocksHangUpAck( KSocksResult& kSocksResult_ )
{
    g_pItemMgr->m_kInventory.AddItemList( kSocksResult_.m_vecUseItem, true );
    this->SyncData( kSocksResult_.m_kSocksInfo );
}

//	양말 걷기 리퀘에 대한 응답
void KGCSocksTreeDlg::OnEventSocksCollectAck( KSocksResult& kSocksResult_ )
{
    for( size_t i = 0; i < kSocksResult_.m_vecRewardItem.size(); ++i )
    {
        const KItem&	rItem = kSocksResult_.m_vecRewardItem[i];
        GCItem*			kItem = g_MyD3D->m_kItemMgr.GetItemData( rItem.m_ItemID );
        std::wstring	strItemDesc( kItem ? kItem->strItemDesc : g_pkStrLoader->GetString( STR_ID_REPETITION_MESSAGE1 ) );

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX,
            std::wstring(L""),
            g_pkStrLoader->GetString( STR_ID_REPETITION_MESSAGE1 ),
            KGCUIScene::GC_MBOX_USE_NORMAL, rItem.m_ItemID/10, 1, true, true );
    }


    g_pItemMgr->m_kInventory.AddItemList( kSocksResult_.m_vecRewardItem, true );
    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateData();
    this->SyncData( kSocksResult_.m_kSocksInfo );
}


//	털실이 양말로 바뀐경우 호출되는 함수. 즉 양말카운트가 올랐다는 소리
void KGCSocksTreeDlg::OnEventSocksMaterialExchangeACK( KSocksExchangeData& kRecv_ )
{
    std::vector< KItem >::iterator vIt = kRecv_.m_vecUseItem.begin();
    for(;vIt != kRecv_.m_vecUseItem.end();vIt++)
    {
        if(vIt->m_nCount == 0)
        {
            //g_pkUIScene->m_pkItemReceiveBox->RemoveItem(vIt->m_ItemUID);
        }
    }

    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecUseItem, true );
    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecReward, true );

    g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    g_pkUIScene->m_pkMyInfoScene->UpdateData();

    for( size_t i=0; i<kRecv_.m_vecReward.size(); ++i ){
        //	걸려있지 않은 양말을 획득한 경우
        if(!(m_mapSocksTreeDlgProperty[ kRecv_.m_vecReward[i].m_ItemID*10 ].m_bIsHangup)){
            KGCHotEventBtn::SetHotStateOn();
        }

        //2010년에 노란 양말을 상점에서 팔아서 특수 파티클을 띄워 줄려고 만들었지만, 2011년에는 상점에서 팔지 않는 국가가 있다.
#if !defined (NATION_BRAZIL) && !defined (NATION_TAIWAN) && !defined (NATION_KOREA)
        if( GOODSID_OF_SOCKS_YELLOW/10 == kRecv_.m_vecReward[i].m_ItemID ){
            //	노란 양말일때만 띄워주는 파티클
            this->CreateParticle_YellowSocksDone();
        }
#endif

    }

    this->SyncData( m_socksInfoFromServer );
}

void KGCSocksTreeDlg::OnEventSocksHangupCompleteNot( const KSocksResult& kSocksResult_ )
{
    //	이 메세지가 서버로 부터 왔을때, m_nIsHangup이 1인 양말은 걷을수 있는 상태다.
    const KUserSocksInfo& rSockInfo = kSocksResult_.m_kSocksInfo;
    for( KUserSocksInfo::const_iterator itr = rSockInfo.begin(); itr != rSockInfo.end(); ++itr ){
        if( (1 == itr->second.m_nIsHangup) && (0 == itr->second.m_dwRemainSec) ){
            KGCHotEventBtn::SetHotStateOn();
            m_mapSocksTreeDlgProperty[itr->first].m_dwRemainSec		=	0;
            m_mapSocksTreeDlgProperty[itr->first].m_bIsCanCollect	=	true;
        }
        else{
            m_mapSocksTreeDlgProperty[itr->first].m_bIsCanCollect	=	false;
        }
    }

    this->SyncData( kSocksResult_.m_kSocksInfo );
}

void KGCSocksTreeDlg::OnEventSocksScriptInfoNot( const KUserSocksInfo& kUserSocksInfo_ )
{
    this->SyncData( kUserSocksInfo_ );
}



void KGCSocksTreeDlg::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ){
        this->OnUIClickEvent( event );		
        this->UpdateSocksUI();
    }
    else if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ){
        this->OnUIEnterEvent( event );
        this->UpdateSocksUI();
    }
}


void KGCSocksTreeDlg::OnUIClickEvent( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnClose ){
        SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
        this->ClearParticle();	//	파티클 모두 삭제
        KGCHotEventBtn::SetHotStateOff();	//	창을 닫으면 일단 반짝거리는것 취소
    }

    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
    for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        //	양말 걸기 버튼 눌렀을 경우
        if( event.m_pWnd == itr->second.m_pkBtnHangupSocks && m_bPushHangUpBtn == false ){
            m_bPushHangUpBtn = true;
            this->SendSocksHangupReqOne( itr->first );
        }

        //	양말 따기 버튼 눌렀을 경우
        if( event.m_pWnd == itr->second.m_pkToggleHangupSocks && m_bPushDetachBtn == false ){
            if( itr->second.m_dwRemainSec == 0 ){
                std::vector< GCITEMID >	vecSocks;
                vecSocks.push_back( itr->first );
                m_bPushDetachBtn = true;
                KP2P::GetInstance()->Send_SocksCollectReq(vecSocks);
                itr->second.DeleteParticle();
                itr->second.AddCollectParticle();
            }
        }
    }


    //	양말 모두 걸기 버튼 눌렀을 경우
    if( event.m_pWnd == m_pkBtnHangupSocksAll && m_bPushHangUpBtn == false ){
        m_bPushHangUpBtn = true;
        std::vector< GCITEMID >	vecSocks;
        std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
        for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
            vecSocks.push_back( itr->first );
        }
        this->SendSocksHangupReqVector( vecSocks );
    }

    //	양말 모두 따기 버튼 눌렀을 경우
    if( event.m_pWnd == m_pkBtnGetSocksAll && m_bPushDetachBtn == false ){
        std::vector< GCITEMID >	vecSocks;
        std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
        for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
            if( itr->second.m_pkToggleHangupSocks->IsRenderOn() && (itr->second.m_dwRemainSec == 0) ){
                itr->second.DeleteParticle();
                vecSocks.push_back( itr->first );
            }
        }
        m_bPushDetachBtn = true;
        KP2P::GetInstance()->Send_SocksCollectReq(vecSocks);
    }
}


void KGCSocksTreeDlg::OnUIEnterEvent( const KActionEvent& event )
{
    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
    for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        if( event.m_pWnd == itr->second.m_pkToggleHangupSocks ){
            this->CalcRemainSec( itr->second );			
        }
    }
}



void KGCSocksTreeDlg::OnCreate( void )
{
    m_vecParticle.clear();

    m_bPushHangUpBtn = false;
    m_bPushDetachBtn = false;

    //m_pkTitle->InitState( true, true, this );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBtnHangupSocksAll->InitState( true, true, this );
    m_pkBtnGetSocksAll->InitState( true, true, this );
    m_pkStaticInfoMain->InitState( true, true, this );
    m_pkStaticInfoMain->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_SOCKS_SYSTEM_INFO ));

     //m_pkStaticNameRed->InitState( true, true, this );
     //m_pkStaticNameRed->SetText(g_pkStrLoader->GetString(STR_ID_TAEGEUKGI_RED));
     //m_pkStaticNameBlue->InitState( true, true, this );
     //m_pkStaticNameBlue->SetText(g_pkStrLoader->GetString(STR_ID_TAEGEUKGI_BLUE));
     //m_pkStaticNameYellow->InitState( true, true, this );
     //m_pkStaticNameYellow->SetText(g_pkStrLoader->GetString(STR_ID_TAEGEUKGI_YELLOW));

    Result_CheckEnoughSocksMaterial = INT_MAX;
    if( this->CheckEnoughSocksMaterial() ){
        KP2P::GetInstance()->Send_SocksMaterialExchangeReq();
        g_MyD3D->WaitForServerAck( Result_CheckEnoughSocksMaterial, INT_MAX, 3000, -1 );
        if( 0 == Result_CheckEnoughSocksMaterial ){

        }
    }
    this->InitStateSocks();
    this->SyncData( m_socksInfoFromServer, true );
    this->CheckParticle();

    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ){
        egcRenderLayer = GC_LAYER_UPPER_SQUARE_UI_0;
    }
    else{
        egcRenderLayer = GC_LAYER_UI;
    }


}


void KGCSocksTreeDlg::PostChildDraw()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

        g_ParticleManager->Render( egcRenderLayer );
        //g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_1 );
    }
    g_pGCDeviceManager2->PopState();

    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
    for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        float		posX = itr->second.m_pkStaticSocksNumberPos->GetRelocatedWindowPos().x;
        float		posY = itr->second.m_pkStaticSocksNumberPos->GetRelocatedWindowPos().y;
        D3DXVECTOR2	DrawPos( posX, posY );

        g_pkUIMgr->RenderNumberScale( "blue_gradation_num",	DrawPos, itr->second.m_duration, 1.0f, 0xffffffff, true, true, false );
    }
}


void KGCSocksTreeDlg::LinkControlSocks()
{
    std::string					strBtnHangupSocks		("btn_hangup_socks_");
    std::string					strToggleHangupSocks	("toggle_hangup_socks_");
    std::string					strStaticSocksGetInfo	("static_info_socks_");
    std::string					strStaticSocksPercent	("static_percent_socks_");
    std::string					strStaticSocksNumberPos	("static_socks_number_pos_");
    std::string					strpkGuageSocks			("guage_socks_");

    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
    for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        std::string		strBtnHangupSocksColor = strBtnHangupSocks + itr->second.m_strColor;
        LINK_CONTROL(	strBtnHangupSocksColor.c_str(),	itr->second.m_pkBtnHangupSocks );

        std::string		strToggleHangupSocksColor = strToggleHangupSocks + itr->second.m_strColor;
        LINK_CONTROL(	strToggleHangupSocksColor.c_str(),	itr->second.m_pkToggleHangupSocks );

        std::string		m_pkStaticSocksGetInfoColor = strStaticSocksGetInfo + itr->second.m_strColor;
        LINK_CONTROL(	m_pkStaticSocksGetInfoColor.c_str(),	itr->second.m_pkStaticSocksGetInfo );

        std::string		m_pkStaticSocksPercentColor = strStaticSocksPercent + itr->second.m_strColor;
        LINK_CONTROL(	m_pkStaticSocksPercentColor.c_str(),	itr->second.m_pkStaticSocksPercent );

        std::string		m_pkStaticSocksNumberPosColor = strStaticSocksNumberPos + itr->second.m_strColor;
        LINK_CONTROL(	m_pkStaticSocksNumberPosColor.c_str(),	itr->second.m_pkStaticSocksNumberPos );

        std::string		strpkGuageSocksColor = strpkGuageSocks + itr->second.m_strColor;
        LINK_CONTROL(	strpkGuageSocksColor.c_str(),	itr->second.m_pkGuageSocks );

        itr->second.InitData();
    }
}


void KGCSocksTreeDlg::InitStateSocks()
{
    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin();
    for( ; itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        if(itr->second.m_pkBtnHangupSocks)
            itr->second.m_pkBtnHangupSocks->InitState( true, true, this );		//	걸기버튼
        if(itr->second.m_pkStaticSocksGetInfo)
            itr->second.m_pkStaticSocksGetInfo->InitState( true, true, this );	//	양말 정보
        if(itr->second.m_pkToggleHangupSocks)
            itr->second.m_pkToggleHangupSocks->InitState( false, true, this );	//	일단 걸린거 모두 끈다.
        if(itr->second.m_pkStaticSocksPercent)
            itr->second.m_pkStaticSocksPercent->InitState( true, true, this );	//	퍼센트도 일단 0으로 초기화
        if(itr->second.m_pkGuageSocks)
            itr->second.m_pkGuageSocks->InitState( true );
    }
}


//------------------------------------------------------------------------
//	서버가 준 정보로 양말 이벤트 정보를 덮어쓴다.
//------------------------------------------------------------------------
void KGCSocksTreeDlg::SyncData( const KUserSocksInfo& dataFromServer_, bool timeDataSkip )
{
    KUserSocksInfo::const_iterator	itrDataFromPacket = dataFromServer_.begin();
    for( ; itrDataFromPacket != dataFromServer_.end() ; ++itrDataFromPacket ){
        m_socksInfoFromServer[itrDataFromPacket->first] = itrDataFromPacket->second;
    }
    //m_socksInfoFromServer	=	dataFromServer_;


    KUserSocksInfo::const_iterator	itrDataFromServer = m_socksInfoFromServer.begin();
    for( ; itrDataFromServer != m_socksInfoFromServer.end() ; ++itrDataFromServer ){
        std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itrFind = m_mapSocksTreeDlgProperty.find( itrDataFromServer->first );
        if( itrFind != m_mapSocksTreeDlgProperty.end() ){

            KGCSocksTreeDlgProperty&	cashData	=	itrFind->second;			//	양말 UI가 들고있을 정보
            const KSocksInfo&			serverData	=	itrDataFromServer->second;	//	서버에서 준 정보

            cashData.m_dwPeriodSec		=	serverData.m_dwPeriodSec;

            if( !timeDataSkip ){
                cashData.m_dwRemainSec		=	serverData.m_dwRemainSec;
                cashData.m_remainTick		=	GetTickCount();
            }

            cashData.m_materialGoodsID	=	serverData.m_prMaterialID.first;
            cashData.m_materialRequire	=	serverData.m_prMaterialID.second;
            cashData.m_bIsHangup		=	( 0 != serverData.m_nIsHangup ) ? true : false;

            if( cashData.m_bIsHangup && (cashData.m_dwRemainSec == 0) ){	//	걸려있는데 남은 시간이 0이면 걷을수 있는 상태
                cashData.m_bIsCanCollect	=	true;
            }

            KItem*			pMaterial		=	g_pItemMgr->m_kInventory.FindItemByItemID( cashData.m_materialGoodsID/10 );
            if( pMaterial ){
                float fSocksPercent			=	static_cast<float>(pMaterial->m_nCount)/static_cast<float>(cashData.m_materialRequire);
                float fFullGuageWidth		=	220.0f;
                if( fSocksPercent > 1.0f ) fSocksPercent = 1.0f;
                cashData.m_dwGuageWidth		=	static_cast<DWORD>( fFullGuageWidth * fSocksPercent );
                cashData.m_dwSocksPercent	=	static_cast<DWORD>( fSocksPercent * 100.0f );
            }
            else{
                cashData.m_dwSocksPercent	=	0;
                cashData.m_dwGuageWidth		=	0;
            }

            const GCITEMID&	socksGoodsID	=	itrFind->first;
            KItem*			pItemSocks		=	g_pItemMgr->m_kInventory.FindItemByItemID( socksGoodsID/10 );
            if( pItemSocks ){
                cashData.m_duration			=	pItemSocks->m_nCount;
            }
            else{
                cashData.m_duration			=	0;
            }
        }
    }

    this->UpdateSocksUI();
}


void KGCSocksTreeDlg::UpdateSocksUI()
{
    int iToggleSocksCount = 0;
    int iSocksCount = 0;

    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itrSocksInfo = m_mapSocksTreeDlgProperty.begin();
    for( ; itrSocksInfo != m_mapSocksTreeDlgProperty.end(); ++itrSocksInfo ){
        KGCSocksTreeDlgProperty& socksUI	=	itrSocksInfo->second;

        if( socksUI.m_bIsHangup && (socksUI.m_dwRemainSec == 0) ){
            iToggleSocksCount++;
        }

        if(	socksUI.m_duration && !itrSocksInfo->second.m_pkToggleHangupSocks->IsRenderOn() ){
            iSocksCount	+= socksUI.m_duration;
            socksUI.m_pkBtnHangupSocks->Lock( false );
        }
        else{
            socksUI.m_pkBtnHangupSocks->Lock( true );
        }

        socksUI.m_pkToggleHangupSocks->ToggleRender( socksUI.m_bIsHangup );	//	걸려있는 양말 UI

        std::wstring strSocksPercent	=	boost::lexical_cast<std::wstring>( socksUI.m_dwSocksPercent );	//	양말 퍼센트 문자
        if(socksUI.m_pkStaticSocksPercent)
        {
            socksUI.m_pkStaticSocksPercent->SetText( strSocksPercent );
            socksUI.m_pkStaticSocksPercent->SetFontOutline( true );
        }
        if(socksUI.m_pkGuageSocks)
            socksUI.m_pkGuageSocks->SetWidth( socksUI.m_dwGuageWidth );
        if(socksUI.m_pkStaticSocksGetInfo){
            socksUI.m_pkStaticSocksGetInfo->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString(STR_ID_SOCKS_GET_INFO, "i", socksUI.m_dwPeriodSec/60 ) );
        }
        if(socksUI.m_pkStaticSocksPercent)
            socksUI.m_pkStaticSocksPercent->SetText( g_pkStrLoader->GetReplacedString(STR_ID_SOCKS_PERCENT, "i", socksUI.m_dwSocksPercent ) );
    }

    m_pkBtnGetSocksAll->Lock( iToggleSocksCount ? false : true );	//	모두 걷기 버튼의 락 상태 판단
    m_pkBtnHangupSocksAll->Lock( iSocksCount ? false : true );		//	모두 걸기 버튼의 락 상태 판단

}


void KGCSocksTreeDlg::SendSocksHangupReqOne( const GCITEMID socksGoodsID_ )
{ 
    std::vector< GCITEMID >	vecSendSocks;
    vecSendSocks.push_back( socksGoodsID_ );
    this->SendSocksHangupReqVector( vecSendSocks );
}


void KGCSocksTreeDlg::SendSocksHangupReqVector( const std::vector< GCITEMID >& vecSocksGoodsID_ )
{
    std::vector< GCITEMID >	vecSendSocks;
    for( size_t i=0; i<vecSocksGoodsID_.size(); ++i ){
        //	인벤토리에 있어야 하고, 트리에 걸려있지 않아야 한다
        if( ( g_pItemMgr->m_kInventory.FindItemByItemID( vecSocksGoodsID_[i]/10 )) &&
            ( !m_mapSocksTreeDlgProperty[vecSocksGoodsID_[i]].m_pkToggleHangupSocks->IsRenderOn() )
            ){
                vecSendSocks.push_back( vecSocksGoodsID_[i] );
        }
    }
    KP2P::GetInstance()->Send_SocksHangupReq( vecSendSocks );
}


void KGCSocksTreeDlg::FrameMoveInEnabledState()
{
    this->CheckParticle();

    static	DWORD	oldTime = GetTickCount();
    if( GetTickCount() - oldTime > 1000 ){
        this->UpdateSocksUI();
        this->CheckGetSome();
        oldTime = GetTickCount();
    }
}


void KGCSocksTreeDlg::CheckParticle()
{
    //	testcode
    for( std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin(); itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        if( itr->second.m_bIsCanCollect ){
            itr->second.AddCanCollectParticle();
            itr->second.m_bIsCanCollect	=	false;
        }
    }
}

void KGCSocksTreeDlg::CreateParticle_YellowSocksDone()
{
    float fx1 = 0.0f;
    float fy1 = 0.0f;

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Yangmal_100_01",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Yangmal_100_02",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Yangmal_100_03",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Yangmal_100_04",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Special_Reward_01",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Special_Reward_02",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Special_Reward_03",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Lire_Magic_Missile_Scope_01",       fx1, fy1, 0.5f ), egcRenderLayer);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Lire_Magic_Missile_Scope_02",       fx1, fy1, 0.5f ), egcRenderLayer);
}


void KGCSocksTreeDlg::ClearParticle()
{
    for( std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin(); itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        itr->second.DeleteParticle();
    }
}


void KGCSocksTreeDlg::AddParticle( const std::string& strParticle_, float fx_, float fy_ )
{
    m_vecParticle.push_back( g_ParticleManager->CreateSequence( strParticle_.c_str(), fx_, fy_, 0.5f ) );
    g_ParticleManager->SetLayer( m_vecParticle.back(), egcRenderLayer );
}


void KGCSocksTreeDlg::CalcRemainSec( KGCSocksTreeDlgProperty& socksProperty_ )
{
    //	아래 두값은 회수 상태가 될때까지 변하면 안된다
    const DWORD&	remainSecond	=	socksProperty_.m_dwRemainSec;	//	서버가 알려준 양말 남은 시간
    const DWORD&	remainTick		=	socksProperty_.m_remainTick;	//	그때 기록해둔 틱

    DWORD	pastTick	=	GetTickCount() - remainTick;	//	흐른 틱
    DWORD	totalTick	=	remainSecond * 1000;

    if( totalTick > pastTick ){
        DWORD	extraSecond = totalTick - pastTick;
        extraSecond /= 1000;
        DWORD	minute = extraSecond/60;
        DWORD	second = extraSecond%60;
        std::wstring strRemainSecond( g_pkStrLoader->GetReplacedString(STR_ID_SOCKS_REMAINSEC, "ii", minute, second ) );
        socksProperty_.m_pkToggleHangupSocks->SetToolTip( strRemainSecond );
    }
    else{
        std::wstring strRemainSecond( g_pkStrLoader->GetReplacedString(STR_ID_SOCKS_REMAINSEC, "ii", 0, 0 ) );
        socksProperty_.m_pkToggleHangupSocks->SetToolTip( strRemainSecond );
    }
}


void KGCSocksTreeDlg::CheckGetSome()
{
    bool bIsGetSome = false;


    for( std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itr = m_mapSocksTreeDlgProperty.begin(); itr != m_mapSocksTreeDlgProperty.end(); ++itr ){
        if( ( itr->second.m_bIsHangup ) &&
            ( itr->second.m_dwRemainSec == 0 )  ){
                bIsGetSome = true;
                break;
        }
    }

    if( this->CheckEnoughSocksMaterial() ){
        bIsGetSome = true;
    }



    if( bIsGetSome ){
        KGCHotEventBtn::SetHotStateOn();
    }
    else{
        KGCHotEventBtn::SetHotStateOff();
    }
}

bool KGCSocksTreeDlg::CheckEnoughSocksMaterial()
{
    bool bEnoughSocksMaterial = false;
    std::map< GCITEMID, KGCSocksTreeDlgProperty >::iterator itrSocksInfo = m_mapSocksTreeDlgProperty.begin();
    for( ; itrSocksInfo != m_mapSocksTreeDlgProperty.end(); ++itrSocksInfo ){

        KItem*			pMaterial		=	g_pItemMgr->m_kInventory.FindItemByItemID( itrSocksInfo->second.m_materialGoodsID/10 );
        if( pMaterial ){
            if( static_cast<int>(itrSocksInfo->second.m_materialRequire) <= pMaterial->m_nCount ){
                bEnoughSocksMaterial = true;
                break;
            }
        }
    }
    return bEnoughSocksMaterial;
}


void KGCSocksTreeDlg::FrameMoveInDisabledState()
{
    static	DWORD	oldTime = GetTickCount();
    if( GetTickCount() - oldTime > 1000 ){
        this->CheckGetSome();
        oldTime = GetTickCount();
    }
}


void KGCSocksTreeDlgProperty::AddCanCollectParticle()
{
    std::string paticleName;
    if( m_strColor == std::string("red") ){
        paticleName = std::string("Yangmal_Red");
        m_vecParticle.push_back( g_ParticleManager->CreateSequence( paticleName.c_str(), /*-0.43f*/-0.1963f, -0.0259f, 0.5f ) );
        g_ParticleManager->SetLayer( m_vecParticle.back(), egcRenderLayer );
    }
    else if( m_strColor == std::string("blue") ){
        paticleName = std::string("Yangmal_Blue");
        m_vecParticle.push_back( g_ParticleManager->CreateSequence( paticleName.c_str(), /*-0.2775f*/-0.4674f, 0.0853f, 0.5f ) );
        g_ParticleManager->SetLayer( m_vecParticle.back(), egcRenderLayer );
    }
    else if( m_strColor == std::string("yellow") ){
        paticleName = std::string("Yangmal_Yellow");
        m_vecParticle.push_back( g_ParticleManager->CreateSequence( paticleName.c_str(), /*-0.3924f*/-0.2061f, 0.18f, 0.5f ) );
        g_ParticleManager->SetLayer( m_vecParticle.back(), egcRenderLayer );
    }
}





void KGCSocksTreeDlgProperty::DeleteParticle()
{
    for( size_t i=0; i<m_vecParticle.size(); ++i ){
        g_ParticleManager->DeleteSequence( m_vecParticle[i] );
    }
    m_vecParticle.clear();
}



void KGCSocksTreeDlgProperty::AddCollectParticle()
{
    float ax = 0.0f;
    float ay = 0.0f;

    std::string paticleName;
    if( m_strColor == std::string("red") ){
        this->AddParticleOne( std::string("Get_GP_Effect_02"),		-0.1963f + ax, -0.0259f + ay );
        this->AddParticleOne( std::string("Kungji_Punch_01"),		-0.1963f + ax, -0.0259f + ay );
        this->AddParticleOne( std::string("Las01_Brind_Spark_01"),	-0.1963f + ax, -0.0259f + ay );
    }
    else if( m_strColor == std::string("blue") ){
        this->AddParticleOne( std::string("Exp_Add_Effect_01"),		-0.4674f + ax, 0.0853f + ay );
        this->AddParticleOne( std::string("Exp_Add_Effect_02"),		-0.4674f + ax, 0.0853f + ay );
        this->AddParticleOne( std::string("Pet_Mermering_Hit_01"),	-0.4674f + ax, 0.0853f + ay );
    }
    else if( m_strColor == std::string("yellow") ){
        this->AddParticleOne( std::string("Get_GP_Effect_01"),		-0.2061f + ax, 0.18f + ay );
        this->AddParticleOne( std::string("Get_GP_Effect_02"),		-0.2061f + ax, 0.18f + ay );
        this->AddParticleOne( std::string("Las01_Brind_Spark_01"),	-0.2061f + ax, 0.18f + ay );
    }
}


void KGCSocksTreeDlgProperty::AddParticleOne( const std::string& rName_, float x_, float y_ )
{
    m_vecParticle.push_back( g_ParticleManager->CreateSequence( rName_.c_str(), x_, y_, 0.5f ) );
    g_ParticleManager->SetLayer( m_vecParticle.back(), egcRenderLayer );
}

