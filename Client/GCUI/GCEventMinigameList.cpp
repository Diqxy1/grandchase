#include "stdafx.h"
#include "GCEventMinigameList.h"
#include "GCItemCombinationManager.h"

IMPLEMENT_CLASSNAME( KGCEventMinigameList );
IMPLEMENT_WND_FACTORY( KGCEventMinigameList );
IMPLEMENT_WND_FACTORY_NAME( KGCEventMinigameList, "gc_event_minigame_list_dlg" );

KGCEventMinigameList::KGCEventMinigameList( void )
: m_pkEndBtn(NULL)
, m_pkEventBox(NULL)
, m_pkBtnClose( NULL )
, m_pkOutBackBox(NULL)
, m_pkInBackBox(NULL)
{
    LINK_CONTROL("end_btn",m_pkEndBtn);
    LINK_CONTROL("event_box",m_pkEventBox);
    LINK_CONTROL("btnClose",m_pkBtnClose );
    LINK_CONTROL("background",m_pkOutBackBox);
    LINK_CONTROL("background_1",m_pkInBackBox);
}

KGCEventMinigameList::~KGCEventMinigameList( void )
{
    ClearEventList();
}

void KGCEventMinigameList::ActionPerformed( const KActionEvent& event )
{
    if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if(OnClickEventBox(event))
            OnEndBtn();
    }
}

void KGCEventMinigameList::OnCreate( void )
{
    m_pkEndBtn->InitState(true,true,this);
	m_pkEndBtn->SetHotKey(DIK_ESCAPE);
    m_pkEndBtn->DisconnectAllEventProcedure();
    m_pkEndBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCEventMinigameList::OnEndBtn );

	if ( m_pkBtnClose )
	{
	    m_pkBtnClose->InitState( true, true, this );
	    m_pkBtnClose->DisconnectAllEventProcedure();
	    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCEventMinigameList::OnEndBtn );
	    m_pkBtnClose->SetHotKey( VK_ESCAPE );
	}

    m_pkEventBox->InitState(false,false,NULL);

    SetEventGameList();
}

void KGCEventMinigameList::OnDestroy( void )
{

}

void KGCEventMinigameList::OnEndBtn( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCEventMinigameList::SetEventGameList( void )
{
    ClearEventList();

    if(g_kGlobalValue.m_vecEventOpenInfo.empty())
        return;

    std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(this, m_pkEventBox->GetFileName().c_str(), "",m_pkEventBox->GetPath().c_str(),true,g_kGlobalValue.m_vecEventOpenInfo.size() );

    if(vecTemp.empty()) return;

    for(int index = 0; index < static_cast<int>(g_kGlobalValue.m_vecEventOpenInfo.size()); index++ )
    {
        SetEventGame(g_kGlobalValue.m_vecEventOpenInfo[index]);
        SetEventPosition(g_kGlobalValue.m_vecEventOpenInfo[index],vecTemp[index], index);
    }
    if(m_vecEventBoxList.size() % 4 != 0)
    {
        for(int i = m_vecEventBoxList.size() % 4;i < 4; i++)
        {
            KEventOpenInfo temp;
            temp.m_dwEventID = 0;
            temp.m_dwMBoxID = 0;
            temp.m_wstrFileName = L"tex_gc_mbox_defualt.dds";
            SetEventPosition(temp,NULL,m_vecEventBoxList.size());
        }
    }

    int sizingboxHeight = (m_vecEventBoxList.size()-1)/4 * 82;

	sizingboxHeight = sizingboxHeight;

    m_pkOutBackBox->SetHeight(171 + sizingboxHeight );
    m_pkInBackBox->SetHeight(94 + sizingboxHeight );
    m_pkEndBtn->SetWindowPosY(135.0f + static_cast<float>(sizingboxHeight));
}

void KGCEventMinigameList::SetEventGame( KEventOpenInfo& pInfo )
{
    //알람리스트에 등록
    for(std::vector<DWORD>::iterator vIter = pInfo.m_vecItemList.begin();vIter != pInfo.m_vecItemList.end();vIter++)
    {
        g_kGlobalValue.m_setEventAlarmItemList.insert((*vIter));
    }
}


void KGCEventMinigameList::SetEventPosition( KEventOpenInfo& pInfo ,KD3DWnd *pBtnWnd, int iPos)
{
    float _ImagePosX = 0.0f;
    float _ImagePosY = 0.0f;
    float _ImageRebertY = -1.0f;


    float _BtnPosX = 0.0f;
    float _BtnPosY = 0.0f;

    _ImagePosX = GetFixedWindowPos().x + 20.0f;
    _ImagePosY = GetFixedWindowPos().y + (-80.0f *_ImageRebertY);
    _BtnPosX = 20.0f;
    _BtnPosY = 42.0f;



    float fImagePosX = static_cast<float>((iPos % 4)) * 85.0f;
    float fImagePosY = static_cast<float>((iPos / 4)) * (_ImageRebertY * 82.0f);

    //이벤트 버튼 구성.
    KGC2DObject *pImage = new KGC2DObject();
    char filename[1000];
    memset(filename,0,1000);

    g_kGlobalValue.ConvertWideStringToAnsi(filename,pInfo.m_wstrFileName.c_str());
    pImage->AddTexture(g_pGCDeviceManager2->CreateTexture(filename));
    pImage->SetAxisOffset( 0.0f, 0.0f,0.0f );
    pImage->SetSize( 83 , 80  );
    pImage->CameraMatrixOn();
    pImage->SetPositionPixel( fImagePosX + _ImagePosX, fImagePosY + _ImagePosY );

    SEventMiniGameList temp;
    temp.dwEventID = pInfo.m_dwMBoxID;
    temp.pkEventBox = pBtnWnd;
    temp.pkImage = pImage;
    m_vecEventBoxList.push_back(temp);


// 	KLuaManager luaMgr;
// 	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
// 	float x,y = 0.0f;
// 	LUA_GET_VALUE_DEF( "x", x, 0.05f );   
// 	LUA_GET_VALUE_DEF( "y", y, 0.05f ); 

    if(pBtnWnd)
    {
        pBtnWnd->InitState(true,true,this);
        pBtnWnd->SetToolTip( g_pkStrLoader->GetString( pInfo.m_dwEventID ) );
        pBtnWnd->SetWindowPos(D3DXVECTOR2(fImagePosX + _BtnPosX, _BtnPosY - fImagePosY));
    }
}




bool KGCEventMinigameList::OnClickEventBox( const KActionEvent& event )
{
    for(std::vector<SEventMiniGameList>::iterator vIt = m_vecEventBoxList.begin();vIt != m_vecEventBoxList.end();vIt++)
    {
        if(vIt->pkEventBox == event.m_pWnd)
        {
            if(vIt->dwEventID != 0)
            {
                if (vIt->dwEventID == KGCUIScene::GC_MBOX_GAWIBAWIBO_DLG)
                {
                    this->Destroy();
                    Result_GawibawiboGameInfo = INT_MAX;
                    if(g_pkUIScene->m_pkGawibawiboGame == NULL)
                        break;
                    KP2P::GetInstance()->Send_GetUserGwibawiboDataReq(g_pkUIScene->m_pkGawibawiboGame->GetVerInfo());
                    g_MyD3D->WaitForServerAck( Result_GawibawiboGameInfo, INT_MAX, 5000, TIME_OUT_VALUE );
                    return true;
                }
                else if(vIt->dwEventID == KGCUIScene::GC_MBOX_EVENT_SPHINX_DLG)
                {
                    Result_SphinxReq = INT_MAX;
                    KP2P::GetInstance()->Send_UserSphinxDataReq();
                    g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
                    return true;
                }
                else if(vIt->dwEventID == KGCUIScene::GC_MBOX_GWC_EVENT_DLG)
                {
                    Result_GwcRakinglistReq = INT_MAX;
                    KP2P::GetInstance()->Send_GWCRakingListReq();
                    g_MyD3D->WaitForServerAck( Result_GwcRakinglistReq, INT_MAX, 5000, TIME_OUT_VALUE );
                    return true;
                }
                else if ( vIt->dwEventID == KGCUIScene::GC_MBOX_HALLOWEEN_DLG )
                {
                    Result_HalloweenBoardGame = INT_MAX;

                    KP2P::GetInstance()->Send_BoardGameInfoReq();
                    g_MyD3D->WaitForServerAck( Result_HalloweenBoardGame, INT_MAX, 5000, TIME_OUT_VALUE );

                    if( Result_HalloweenBoardGame == 0 || Result_HalloweenBoardGame == 2 || Result_HalloweenBoardGame == 3 )
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_HALLOWEEN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL);
                    return true;
                }
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_SOCKS_TREE_DLG )
                {
                    Result_SockTreeReq = INT_MAX;
                    KP2P::GetInstance()->Send_SockTreeReq();
                    g_MyD3D->WaitForServerAck( Result_SockTreeReq, INT_MAX, 4000, TIME_OUT_VALUE );
                    g_pkUIScene->MessageBox( static_cast<KGCUIScene::EGCMsgBoxType>(vIt->dwEventID) );
                    return true;
                }
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_ECLIPSE_OLD_BOOKS_DLG) 
                { 
                    Result_EclipseEvent = INT_MAX; 
                    KP2P::GetInstance()->Send_EclipseCollectInfoReq(); 
                    g_MyD3D->WaitForServerAck( Result_SockTreeReq, INT_MAX, 1000, TIME_OUT_VALUE ); 
                    return true; 
                }
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_RICKOON_TORNADO_DLG )
                {
                    if ( g_pkUIScene->m_pkRickoonTornado->GetEnableEvent() ) {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_RICKOON_TORNADO_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
                    }
                    else {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RICKOON_TORNADO_ERROR1 ),
                                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, false );
                    }
                    return true;
                }
                else if ( vIt->dwEventID == MINIGAME_WEB_EVENT ) {  // 웹 이벤트용일 경우 
                    // 유저 체널따라 다르게 열어주자 
                    switch( g_kGlobalValue.m_dwChannelType )
                    {
                    case CHANNEL_NETMARBLE:
                        WinExec("explorer  http://chase.netmarble.net/Event/2012/20120503_ICandoit/_Html/", FALSE);
                        break;
                    case CHANNEL_TOONILAND:
                        WinExec("explorer http://chase.tooniland.com/Event/2012/20120503_ICandoit/_Html", FALSE);
                        break;
                    case CHANNEL_NATEON:
                        WinExec("explorer http://chase.nate.netmarble.net/Event/2012/20120503_ICandoit/_Html/", FALSE);
                        break;
                    case CHANNEL_PLAYNET:
                        WinExec("explorer http://chase.playnetwork.co.kr/Event/2012/20120503_ICandoit/_Html", FALSE);
                        break;
                    }
                }
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_SONGKRAN_DLG )
				{
					if ( SiGCSongkranEventManager()->IsGetInfo() )
					{
						KP2P::GetInstance()->Send_SongkranEventUserInfoReq();
					}
					else
					{
						KP2P::GetInstance()->Send_SongkranEventScriptInfoReq();
						KP2P::GetInstance()->Send_SongkranEventUserInfoReq();
					}

                    return true;
                }	
                else if( vIt->dwEventID == KGCUIScene::GC_MBOX_OLYMPIC_DLG ) {

                    // 꺼주자..
                    OnEndBtn();

                    if( false == SiGCOlympicManager()->IsSetInfoState() ) { 
                        Result_OlympicEven_Data = INT_MAX;
                        KP2P::GetInstance()->Send_OlympicScheduleReq();
                        g_MyD3D->WaitForServerAck( Result_OlympicEven_Data, INT_MAX, 3000, TIME_OUT_VALUE ); 
                    }

                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OLYMPIC_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                }
                else if ( vIt->dwEventID == KGCUIScene::GC_MBOX_GC9th_EVENT_DLG ) {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GC9th_EVENT_DLG );
                    return true;
				}
                else if( vIt->dwEventID == KGCUIScene::GC_MBOX_OLYMPIC_DLG ) {

					this->Destroy();

                    if( false == SiGCOlympicManager()->IsSetInfoState() ) { 
                        Result_OlympicEven_Data = INT_MAX;
                        KP2P::GetInstance()->Send_OlympicScheduleReq();
                        g_MyD3D->WaitForServerAck( Result_OlympicEven_Data, INT_MAX, 3000, TIME_OUT_VALUE ); 
                    }

                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OLYMPIC_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                }
                else if ( vIt->dwEventID == KGCUIScene::GC_MBOX_ITEM_COMBINATION_DLG ) {
                    if ( SiGCItemCombinationManager()->GetEventPeriod() ) {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_COMBINATION_DLG );
                    }
                    else {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_HAPPY_NEWYEAR_NOT_EVENT_TIME ),
                        L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
                    }
                    return true;
                }
                else if( vIt->dwEventID == KGCUIScene::GC_MBOX_MAGIC_BOX ) { 
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MAGIC_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
                    return true;
                }
				else if (vIt->dwEventID == KGCUIScene::GC_MBOX_LOTTERY)
				{
					this->Destroy();
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_LOTTERY);
				}				
                
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_GC_ADVENTURE_DLG )
                {
                    Result_GcAdventureEvent = INT_MAX;
                    KP2P::GetInstance()->Send_GcAdventureDateReq();
                    g_MyD3D->WaitForServerAck( Result_GcAdventureEvent, INT_MAX, 1000, TIME_OUT_VALUE );
                    return true;
                }
				else if (vIt->dwEventID == KGCUIScene::GC_MBOX_RITASCHRISTMAS_DLG)
				{
					OnEndBtn();

					Result_ReceiveChristmasUserInfo = INT_MAX;
					KP2P::GetInstance()->Send_RitasChristmas_UserInfo_Req();
					g_MyD3D->WaitForServerAck(Result_ReceiveChristmasUserInfo,INT_MAX, 3000, TIME_OUT_VALUE);

					Result_ReceiveChristmasRankInfo = INT_MAX;
					KP2P::GetInstance()->Send_RitasChristmas_RankInfo_Req();
					g_MyD3D->WaitForServerAck( Result_ReceiveChristmasRankInfo, INT_MAX, 3000, TIME_OUT_VALUE );

					if ( false == SiGCRitasChrismasManager()->HaveInfo() )
					{
						Result_ReceiveChristmasRewardItemList = INT_MAX;
						KP2P::GetInstance()->Send_RitasChristmas_RewardItemList_Req();
						g_MyD3D->WaitForServerAck( Result_ReceiveChristmasRewardItemList, INT_MAX, 3000, TIME_OUT_VALUE );

						g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->GetRitasHelp()->SetRewardItemImg();

						g_pkUIScene->m_pkRitasChristmasDlg->GetRankingDlg()->GetRitasHelp()->ToggleRender(true);
						Result_ReceiveChristmasPlayInfo = INT_MAX;
						KP2P::GetInstance()->Send_RitasChristmas_PlayInfo_Req();
						g_MyD3D->WaitForServerAck( Result_ReceiveChristmasPlayInfo, INT_MAX, 3000, TIME_OUT_VALUE );

						
					}

					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_RITASCHRISTMAS_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL);
				}
                else if ( vIt->dwEventID == KGCUIScene::GC_MBOX_ATTENDANCE_STAMP_DLG )
                {
                    // 서버에 이벤트 정보 요청
                    Result_ReceiveAttendanceStampInfo = INT_MAX;
                    KP2P::GetInstance()->Send_AccumulateAttendanceInfoReq();
                    g_MyD3D->WaitForServerAck( Result_ReceiveAttendanceStampInfo, INT_MAX, 3000, TIME_OUT_VALUE );

                    return true;
                }
				else if ( vIt->dwEventID == KGCUIScene::GC_MBOX_CHALLENGE_ERRAND )
                {

                    if( SiGCChallengeErrandManager()->ErrandIsGetInfo() != true )
                    {
                        KP2P::GetInstance()->Send_ErrandInitInfoReq();
                        KP2P::GetInstance()->Send_ErrandUserDataReq();
                    }
                    else
                    {
                        KP2P::GetInstance()->Send_ErrandUserDataReq();
                    }

                    return true;
                }
				else if (vIt->dwEventID == KGCUIScene::GC_MBOX_SOCIAL_COMMERCE )
                {
                    // 서버에 이벤트 정보 요청
                    Result_ReceiveSocialCommerceInfo = INT_MAX;
                    KP2P::GetInstance()->Send_SocialCommerceInfoReq();
                    g_MyD3D->WaitForServerAck( Result_ReceiveSocialCommerceInfo, INT_MAX, 3000, TIME_OUT_VALUE );

                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SOCIAL_COMMERCE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL);
                    return true;
                }
                else if (vIt->dwEventID == KGCUIScene::GC_MBOX_VIP_REWARD )
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_VIP_REWARD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL);
                    return true;
                }
                else { 
                    g_pkUIScene->MessageBox( static_cast<KGCUIScene::EGCMsgBoxType>( vIt->dwEventID ) );
                    return true;
                }
            }
        }
    }

    return false;
}

void KGCEventMinigameList::PostChildDraw( void )
{
    for(std::vector<SEventMiniGameList>::iterator vIt = m_vecEventBoxList.begin();vIt != m_vecEventBoxList.end();vIt++)
    {
        vIt->pkImage->Render();
    }
}

void KGCEventMinigameList::ClearEventList()
{
    for(std::vector<SEventMiniGameList>::iterator vIt = m_vecEventBoxList.begin();vIt != m_vecEventBoxList.end();vIt++)
    {
        if(vIt->pkImage){
            vIt->pkImage->RemoveAllTexture();
            SAFE_DELETE(vIt->pkImage);
        }

        if(vIt->pkEventBox)
        {
            DeleteChildByPtr(vIt->pkEventBox);
        }
    }

    m_vecEventBoxList.clear();
    g_kGlobalValue.m_setEventAlarmItemList.clear();

}