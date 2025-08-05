#include "stdafx.h"
#include "KGCBingo.h"
#include "GCMyInfoScene.h"

#include "../Procedure.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCBingo );
IMPLEMENT_WND_FACTORY( KGCBingo );
IMPLEMENT_WND_FACTORY_NAME( KGCBingo, "gc_bingo" );

KGCBingo::KGCBingo(void)
{
	LINK_CONTROL( "close_button", m_pkCloseBtn );
	LINK_CONTROL( "active_bingo", m_pkActiveBingo);
	LINK_CONTROL( "static_remain_coin",m_pkStaticRemainCoin);
	LINK_CONTROL( "static_remain_time",m_pkStaticRemainTime);
	LINK_CONTROL( "static_bingo_number",m_pkStaticBingoNumber);
	LINK_CONTROL( "static_bingo_desc",m_pkStaticBingoDesc);


	char str[256] = "";
	for( unsigned int i = 0; i < MAX_BOX_SIZE; ++i ) {
		for( unsigned int j =0; j < MAX_BOX_SIZE; ++j ) {
			m_pkBingoBox[i][j] = NULL;

			sprintf_s( str, sizeof(str), "bingo_button%d%d", i, j );
			LINK_CONTROL( str, m_pkBingoBox[i][j] );
		}
	}

	m_iRemainCoin = 0;
	m_iRemainTime = 0;
	m_iBingoCoinCount =0;


}

void KGCBingo::OnCreate(void)
{
	for( unsigned int i = 0; i < MAX_BOX_SIZE ; ++i )
	{
		for( unsigned int j =0; j < MAX_BOX_SIZE; ++j )
		{
			m_pkBingoBox[i][j]->InitState(true,true,this);
		}
	}
	m_pkCloseBtn->InitState(true,true,this);
	m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

	m_pkActiveBingo->InitState(false,true,this);

	m_pkStaticRemainCoin->InitState(true);
	m_pkStaticRemainTime->InitState(true);
	m_pkStaticBingoNumber->InitState(true);
	m_pkStaticBingoDesc->InitState(true);

	m_pkStaticRemainCoin->SetFontColor( 0xFFFFFFFF );
	m_pkStaticRemainTime->SetFontColor( 0xFFFFFFFF );
	m_pkStaticBingoNumber->SetFontColor( 0xFFFFFFFF );
	m_pkStaticBingoDesc->SetFontColor( 0xFFFFFFFF );

	m_pkStaticRemainCoin->SetAlign( DT_CENTER );
	m_pkStaticRemainTime->SetAlign( DT_CENTER );
	m_pkStaticBingoNumber->SetAlign( DT_CENTER );

	m_pkStaticBingoDesc->SetText(g_pkStrLoader->GetString(STR_ID_BINGO_STRING_DESC));
	m_pkStaticBingoDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_BINGO_STRING_DESC));

	for ( unsigned int i = 0 ; i < MAX_BOX_SIZE ; ++i ) {
		for ( unsigned int j = 0 ; j < MAX_BOX_SIZE ; ++j ) {
			m_pkBingoBox[i][j]->Lock( false );
			m_pkBingoBox[i][j]->Activate( false );
		}
	}


	std::vector<std::pair<int,int>>::iterator vitBingoBoardPosXY = m_vecBingoBoardPosXY.begin();
	for (; vitBingoBoardPosXY != m_vecBingoBoardPosXY.end(); ++vitBingoBoardPosXY ) {
		//일단 푼 문제는 락 걸고, 빙고인지 아닌지 체크하자.
		m_pkBingoBox[vitBingoBoardPosXY->first][vitBingoBoardPosXY->second]->Lock(true);
		CheckBingo(vitBingoBoardPosXY->first,vitBingoBoardPosXY->second);
	}


	m_iActiveBingoTime = 0;
	m_bBingoEffect = false;
	m_bPlaybingoSound = false;
}

void KGCBingo::SetStatic()
{
	WCHAR strTemp[20];
	swprintf( strTemp, sizeof(strTemp), L"%02d", (int)m_iRemainCoin );
	m_pkStaticRemainCoin->SetText( strTemp );

	swprintf( strTemp, sizeof(strTemp), L"%02d", (int)m_iAccPlayTime );
	m_pkStaticRemainTime->SetText( strTemp );

	swprintf( strTemp, sizeof(strTemp), L"%02d", (int)m_iBingoCoinCount );
	m_pkStaticBingoNumber->SetText( strTemp );
}


void KGCBingo::FrameMoveInEnabledState(void)
{
	m_bPlaybingoSound = true;
	DrawBingoEffect();
	SetStatic();
}

void KGCBingo::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );

    if ( event.m_dwCode == D3DWE_BUTTON_CLICK ) {
		for ( unsigned int i = 0; i < MAX_BOX_SIZE ; ++i ) {    
			for ( unsigned int j = 0; j < MAX_BOX_SIZE ; ++j ) {

				if ( event.m_pWnd == m_pkBingoBox[i][j] ){
					if ( m_pkBingoBox[i][j]->IsActive() == true )
						return;

					m_pBingoPosition.first = i;
					m_pBingoPosition.second = j;
					//문제를 얻어온다.
					if(m_iRemainCoin > 0 )
					{
						Result_BingoRequestQuestion = INT_MAX;
						KP2P::GetInstance()->Send_GetBingoQuestion( i, j );
						g_MyD3D->WaitForServerAck( Result_BingoRequestQuestion, INT_MAX, 5000, TIME_OUT_VALUE );
					}
					else
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_BINGO_ERROR3),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
					}
				}
			}
		}
	}
}

KGCBingo::~KGCBingo()
{

}

void KGCBingo::OnDestroy()
{

}

void KGCBingo::OnExit()
{
	for ( unsigned int i = 0 ; i < MAX_BOX_SIZE ; ++i ) {
		for ( unsigned int j = 0 ; j < MAX_BOX_SIZE ; ++j ) {
			m_pkBingoBox[i][j]->Activate( false );
		}
	}

	ClearBingoVector();
	m_vecCorrectQuestions.clear();	

	m_iBingoCoinCount = 0;

	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	ReleaseCapture();

	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCBingo::CheckBingo(int iWidth, int iHeight)
{
	//가로,세로칸 체크
	LineCheckBingo(iWidth,-1,m_vecHeightBingo);
	LineCheckBingo(-1,iHeight,m_vecWidthBingo);

	//대각선 체크
	if( iWidth == iHeight )
		LineCheckBingo(-1,-1,m_vecCrossBingo);
	if( iWidth + iHeight == MAX_BOX_SIZE - 1)
		LineCheckBingo(-1,-1,m_vecCrossBingo,true);
}

void KGCBingo::LineCheckBingo( int iWidth, int iHeight,std::vector<int>& vecBingo, bool bRightCross )
{
	bool Bingo = true;
	//해당 칸
	int SelectBox = 0;
	int BoxWidth;
	int BoxHeight;

	if ( iWidth == -1 && iHeight != -1 ) {
		SelectBox = iHeight;
		BoxHeight = iHeight;
	}
	else if ( iWidth != -1 && iHeight == -1 ) {
		SelectBox = iWidth;
		BoxWidth = iWidth;
	}
	else if ( iWidth == -1 && iHeight == -1 && bRightCross == false ) {
		SelectBox = 0;
	}
	else if ( iWidth == -1 && iHeight == -1 && bRightCross == true ) {
		SelectBox = 1;
	}

	if ( find( vecBingo.begin(), vecBingo.end(), SelectBox ) == vecBingo.end() || vecBingo.empty() ) {
		for ( int i = 0 ; i < MAX_BOX_SIZE ; ++i ) {
			if ( iWidth == -1)
				BoxWidth = i;

			if ( iHeight == -1 && bRightCross == false)
				BoxHeight = i;
			else if( bRightCross == true)
				BoxHeight = MAX_BOX_SIZE - 1 - i;

			if ( !m_pkBingoBox[BoxWidth][BoxHeight]->IsLocked() == true && m_pkBingoBox[BoxWidth][BoxHeight]->IsActive() == false) {
				Bingo = false;
				break;
			}
		}
		if ( Bingo ) {
			m_iActiveBingoTime = 120;
			m_bBingoEffect = false;
			for (int i= 0; i < MAX_BOX_SIZE; ++i ) {
				if( iWidth == -1)
					BoxWidth = i;

				if ( iHeight == -1 && bRightCross == false)
					BoxHeight = i;
				else if( bRightCross == true)
					BoxHeight = MAX_BOX_SIZE - 1 - i;

				m_pkBingoBox[BoxWidth][BoxHeight]->Lock(false);
				m_pkBingoBox[BoxWidth][BoxHeight]->Activate(true);
			}
			vecBingo.push_back(SelectBox);
			m_iBingoCoinCount++;
		}
	}
}

void KGCBingo::ClearBingoVector()
{
	//std::vector<int>::iterator vit = m_vecHeightBingo.begin();
	//while ( vit != m_vecHeightBingo.end() ) {
	//	m_vecHeightBingo.erase( vit );
	//}
	//m_vecHeightBingo.clear();

	//vit = m_vecWidthBingo.begin();
	//while ( vit != m_vecWidthBingo.end() ) {
	//	m_vecWidthBingo.erase( vit );
	//}
	//m_vecWidthBingo.clear();

	//vit = m_vecCrossBingo.begin();
	//while (vit != m_vecCrossBingo.end() ) {
	//	m_vecCrossBingo.erase( vit );
	//}
	//m_vecCrossBingo.clear();

	m_vecHeightBingo.clear();
	m_vecWidthBingo.clear();
	m_vecCrossBingo.clear();
}

void KGCBingo::ShowBingoEffect()
{
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_03", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_04", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_05", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_06", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
}

void KGCBingo::DrawBingoEffect()
{
	if(m_iActiveBingoTime > 0){
		m_iActiveBingoTime--;
		if(m_bBingoEffect == false)
		{
			if(m_bPlaybingoSound == true)
				g_KDSound.Play("GPC_OBTAIN_RARE_ITEM");

			ShowBingoEffect();
			m_bBingoEffect = true;
		}
	}
}