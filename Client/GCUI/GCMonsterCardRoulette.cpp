#include "stdafx.h"
#include "GCMonsterCardRoulette.h"

IMPLEMENT_CLASSNAME( KGCMonsterCardRoulette );
IMPLEMENT_WND_FACTORY( KGCMonsterCardRoulette );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCardRoulette, "gc_monstercardroulette" );

static const int nCARD_CHANGE_FRAME = 5;
static const int nCARD_CHANGE_FRAME_MAX = 225;

KGCMonsterCardRoulette::KGCMonsterCardRoulette( void )
{
	m_nCardMaxFrame = 0;
	m_nCardChangeFrame = 0;
	m_emState = EM_CARD_COMPOSE;
	m_itemID = 0;
	m_vecRewardList.clear();

	m_pkItemSlot = NULL;
	m_pkOKBtn = NULL;
	m_pkCardImageOn = NULL;
	m_pkCardImageOnBack = NULL;
	
	m_pkTitleFusing = NULL;
	m_pkTitleGetCard = NULL;
	m_bCriticalRate = false;
    m_iOrderComma = 1; 

	LINK_CONTROL( "item_wnd", m_pkItemSlot );
    LINK_CONTROL( "title_fusing", m_pkTitleFusing );
    LINK_CONTROL( "title_get_card", m_pkTitleGetCard );
	LINK_CONTROL( "btn_ok", m_pkOKBtn );
	LINK_CONTROL( "item_img_on", m_pkCardImageOn );
	LINK_CONTROL( "back_item_img_on", m_pkCardImageOnBack );

    LINK_CONTROL( "item_wnd_dummy", m_pkMonsterCardContentsDummy );
    LINK_CONTROL( "item_img_on_dummy", m_pkMonsterCardContentsBigDummy );
    
}

KGCMonsterCardRoulette::~KGCMonsterCardRoulette( void )
{
}

void KGCMonsterCardRoulette::OnCreate( void )
{
	m_pkItemSlot->InitState( true, true, this );
    m_pkItemSlot->SetMonsterCardSignPos(m_pkMonsterCardContentsDummy->InitMonsterCardSignPos());

    if ( m_pkTitleFusing ) 
        m_pkTitleFusing->InitState( true, true, this );

    if ( m_pkTitleGetCard ) 
        m_pkTitleGetCard->InitState( false, true, this );
	m_pkOKBtn->InitState( true, true, this );
	m_pkOKBtn->Lock( true );
	m_pkCardImageOn->InitState(true, false, this);
    m_pkCardImageOn->SetMonsterCardSignPos(m_pkMonsterCardContentsBigDummy->InitMonsterCardSignPos());
  
   
	m_pkCardImageOnBack->InitState(false);
}

void KGCMonsterCardRoulette::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkOKBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
}

void KGCMonsterCardRoulette::FrameMoveInEnabledState( void )
{
	static GCITEMID _BackitemID = 0;

	switch( m_emState )
	{
	case EM_CARD_COMPOSE:
	case EM_CARD_WAIT:
		{
			_BackitemID = 0;
			m_nCardChangeFrame++;
			m_nCardMaxFrame++;

			//	결과 표시중일때는 카드 섞지 않는다.
			if ( m_nCardChangeFrame >= nCARD_CHANGE_FRAME ) 
			{
				if ( m_vecRewardList.empty() ) 
					return;


				int nMax = m_vecRewardList.size();
				int nCardIndex = rand()%nMax;

                m_pkItemSlot->SetMonsterCardItemID(m_vecRewardList[nCardIndex] / 10 );
                m_pkItemSlot->SetMonsterCardDecimalSignOffSet(5.f);
                m_pkItemSlot->SetMonsterCardSignOffSet(13.f);
                m_pkItemSlot->SetMonsterCardContents();
                m_pkItemSlot->ShowDuration( false );
				m_nCardChangeFrame = 0;				
			}

			//	카드 섞을때만 메세지 전송한다.
			if ( m_nCardMaxFrame >= nCARD_CHANGE_FRAME_MAX && m_emState == EM_CARD_COMPOSE ) 
			{
				SendComposeReq();
			}
		}
		break;
	case EM_CARD_RESULT:
		{
            m_pkItemSlot->AllOff();
            m_pkItemSlot->SetMonsterCardItemID( m_itemID );
            m_pkItemSlot->SetMonsterCardDecimalSignOffSet(5.f);
            m_pkItemSlot->SetMonsterCardSignOffSet(13.f);
            m_pkItemSlot->SetMonsterCardContents();
            m_pkItemSlot->ShowDuration( false );

			const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

			if( m_pkItemSlot->CheckPosInWindowBound( vMousePos ) )
			{			
                m_pkCardImageOn->SetMonsterCardItemID( m_itemID );
                m_pkCardImageOn->SetMonsterCardContents();
                m_pkCardImageOn->SetMonsterCardDecimalSignOffSet(12.f);
                m_pkCardImageOn->SetMonsterCardSignOffSet(24.f);
                m_pkCardImageOn->SetMonsterCardLVNumScale(0.6f);
                m_pkCardImageOn->SetMonsterCardNumScale(0.7f);
                m_pkCardImageOn->SetMonsterCardPrimeNumScale(0.7f);
                m_pkCardImageOn->SetMonsterCardAllNumShadow(false);

                
                m_pkCardImageOn->ToggleRender( true );
                m_pkCardImageOnBack->ToggleRender( true );
                m_pkOKBtn->ToggleRender( false );

				if ( _BackitemID != m_itemID )
				{
					g_KDSound.Play( "MonsterCardMix01" );
					_BackitemID = m_itemID;
				}
			}
			else
			{
				
                m_pkCardImageOn->AllOff();
				m_pkCardImageOnBack->ToggleRender( false );
                m_pkOKBtn->ToggleRender( true );
				_BackitemID = 0; 
			}

		}
		break;
	}
}

void KGCMonsterCardRoulette::Init()
{
	m_nCardChangeFrame = 0;
	m_nCardMaxFrame = 0;
	m_emState = EM_CARD_COMPOSE;
	m_itemID = 0;
	m_vecRewardList.clear();
    
    if ( m_pkTitleFusing ) 
        m_pkTitleFusing->ToggleRender( true );
    
    if ( m_pkTitleGetCard ) 
        m_pkTitleGetCard->ToggleRender( false );
	m_pkOKBtn->Lock( true );
	
    m_pkCardImageOn->AllOff();
    m_pkCardImageOn->ToggleRender( false );
    
}

void KGCMonsterCardRoulette::SendComposeReq()
{
	m_nCardMaxFrame = 0;
	m_emState = EM_CARD_WAIT;

	//	서버로 메세지 전송
	g_pkUIScene->m_pkMonsterCardCompose->ReqMonsterCardMix();
}

void KGCMonsterCardRoulette::SetResult( GCITEMID _itemID )
{
	float fPosX = 0.0f;
    float fPosY = 0.0f;
#if defined (NATION_KOREA)
	fPosX = 0.01f;
	fPosY = 0.12f;
#else
	fPosX = 0.015f;
	fPosY = 0.16f;
#endif
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_01", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_02", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_03", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_04", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_05", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Upgrade_Card_06", fPosX, fPosY, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);

	g_KDSound.Stop( "MonsterCardMix04" );
	g_KDSound.Play( "MonsterCardMix05" );

	m_nCardChangeFrame = 0;
	m_nCardMaxFrame = 0;
	m_emState = EM_CARD_RESULT;
	m_itemID = _itemID;

    m_pkItemSlot->SetMonsterCardItemID(_itemID);
    m_pkItemSlot->SetMonsterCardContents();
    


    if ( m_pkTitleFusing ) 
        m_pkTitleFusing->ToggleRender( false );
    
    if ( m_pkTitleGetCard ) 
        m_pkTitleGetCard->ToggleRender( true );
	m_pkOKBtn->Lock( false );
}

void KGCMonsterCardRoulette::PostChildDraw( void )
{
	g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );
}

void KGCMonsterCardRoulette::OnOK()
{
	ToggleRender( false );

	g_pkUIScene->m_pkMonsterCardCompose->OnClickOkCard();
}
	