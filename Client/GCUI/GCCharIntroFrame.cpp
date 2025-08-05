#include "stdafx.h"
#include "GCCharIntroFrame.h"
#include "MyD3D.h"

#include "KGCRoomManager.h"
#include "GCIconImageManager.h"

IMPLEMENT_CLASSNAME( KGCCharIntroFrame );
IMPLEMENT_WND_FACTORY( KGCCharIntroFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCCharIntroFrame, "gc_char_intro_frame" );

KGCCharIntroFrame::KGCCharIntroFrame( void )
{
	for (int i=0 ; i<GC_CHAR_NUM ; i++ )
	{
		m_pkCharIcon[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "character_icon%d", i );
		LINK_CONTROL( temp, m_pkCharIcon[i] );

	}
	m_pkCharFocus = NULL;
	LINK_CONTROL( "char_select_focus", m_pkCharFocus );
	m_pkFocusingIcon = NULL;

	m_pkCharIconArrayRect = NULL;
	LINK_CONTROL( "char_icon_draw_rect", m_pkCharIconArrayRect );

	m_pkBtnIconPageUp = NULL;
	LINK_CONTROL( "btn_icon_page_up", m_pkBtnIconPageUp );
	m_pkBtnIconPageDown = NULL;
	LINK_CONTROL( "btn_icon_page_down", m_pkBtnIconPageDown );
	m_pkBtnBefore = NULL;
	LINK_CONTROL( "btn_before", m_pkBtnBefore );

	m_pkNoSelectedCharFrame = NULL;
	LINK_CONTROL( "no_selected_char_frame", m_pkNoSelectedCharFrame );

	m_pkSelectedCharFrame = NULL;
	LINK_CONTROL( "selected_char_frame", m_pkSelectedCharFrame );

	m_cntBlinkTime = 0;

	m_mapCashParticle.clear();

	m_bEmptyNoHaveChar = false;
    m_bEmptyNoCreateChar = false;


}

KGCCharIntroFrame::~KGCCharIntroFrame( void )
{

}

void KGCCharIntroFrame::ActionPerformed( const KActionEvent& event )
{
	if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN)
	{

		for (int i=0 ; i<GC_CHAR_NUM ; i++)
		{
			if (event.m_pWnd == m_pkCharIcon[i])
			{
				m_pkFocusingIcon = m_pkCharIcon[i];
				SetSelectedCharIntro();
				g_KMci.GetShowForMovie().Clear();
				m_pkSelectedCharFrame->SetSelectedCharFrame(i);
				break;
			}
		}
	
	}

	GCWND_MSG_MAP( m_pkBtnBefore,   KD3DWnd::D3DWE_BUTTON_CLICK, GoPreScene )
}

void KGCCharIntroFrame::OnCreate( void )
{
	for (int i=0 ; i<GC_CHAR_NUM ; i++ )
	{
		m_pkCharIcon[i]->InitState(false,true,this);
	}
	m_pkCharIconArrayRect->InitState(false);
	m_pkBtnIconPageUp->InitState(false,true,this);
	m_pkBtnIconPageDown->InitState(false,true,this);

	m_pkBtnBefore->InitState(true,true,this);
	m_pkBtnBefore->SetHotKey( DIK_ESCAPE );

	m_pkNoSelectedCharFrame->InitState(false);

	m_pkSelectedCharFrame->InitState(true,true,this);

	m_pkCharFocus->InitState(false);

//	ArrayCharIcon();

	CreatCashParticle();
}

void KGCCharIntroFrame::FrameMoveInEnabledState( void )
{
	if (m_pkFocusingIcon == NULL && !m_bEmptyNoHaveChar && !m_bEmptyNoCreateChar)
	{
		m_pkCharIconArrayRect->ToggleRender(true);
		m_pkCharFocus->ToggleRender(false);
	}
	else
	{
		m_pkCharIconArrayRect->ToggleRender(false);
	}

	if (m_pkCharIconArrayRect->IsRenderOn())
	{
		
		if (m_cntBlinkTime < BLINK_LIMIT_TIME)
		{
			m_cntBlinkTime++;
		}
		else
		{
			
			if (m_pkCharIconArrayRect->GetWndMode() == D3DWM_DEFAULT)
			{
				m_pkCharIconArrayRect->SetWndMode(D3DWM_HOVER);

			}
			else if (m_pkCharIconArrayRect->GetWndMode() == D3DWM_HOVER)
			{
				m_pkCharIconArrayRect->SetWndMode(D3DWM_DEFAULT);	
			}
			m_cntBlinkTime = 0;
		}

		float fAlpha = 0.0f;
		if (m_pkCharIconArrayRect->GetWndMode() == D3DWM_DEFAULT)
		{
			fAlpha = static_cast<float>(m_cntBlinkTime)/BLINK_LIMIT_TIME*255.0f;
		}
		else if (m_pkCharIconArrayRect->GetWndMode() == D3DWM_HOVER)
		{
			fAlpha = 255.0f - static_cast<float>(m_cntBlinkTime)/BLINK_LIMIT_TIME*255.0f;	
		}

		m_pkCharIconArrayRect->SetAllWindowAlpha(static_cast<int>(fAlpha));
	}
}


void KGCCharIntroFrame::OnInitialize()
{
	OffCashParticle();
	ArrayCharIcon();

    //신규 유져 여부 체크는 닉네임의 존재 여부로 한다.
    //신규 계정인 경우 반드시 케릭터를 생성해야 된다.
    if( g_kGlobalValue.m_kUserInfo.strNickName.empty() )
        m_pkBtnBefore->ToggleRender(false);
    else
        m_pkBtnBefore->ToggleRender(true);

	m_bEmptyNoHaveChar = GCFUNC::GetCharListWithoutHave().empty();
}

void KGCCharIntroFrame::ArrayCharIcon()
{
	m_pkFocusingIcon = NULL;

	for (int i=0 ; i< GC_CHAR_NUM ; i++)
		m_pkCharIcon[i]->ToggleRender(false);

	D3DXVECTOR2 vPos = m_pkCharIconArrayRect->GetFixedWindowLocalPos();

	vPos.x += ICON_STAR_GAP;
	vPos.y += ICON_STAR_GAP;

	KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

	std::map<int,DWORD> mapOpenCharInfo = g_kGlobalValue.GetCharOpenType();
	KCharInfoVector::iterator vIter = vecChar.begin();

	for(;vIter != vecChar.end(); ++vIter)
	{
		mapOpenCharInfo.erase(static_cast<int>(vIter->iCharType));
	}

	DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
	DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();

	int cntColumn = 0;
	int cntRow = 0;

	std::map<int,DWORD>::iterator mit = mapOpenCharInfo.begin();
	for (; mit != mapOpenCharInfo.end() ; ++mit)
	{
		if (cntColumn >= CHAR_ICON_COLUMN)
		{
			//나중에 버튼으로 인한 페이징을 위해 필요한 조건.
			if (cntRow >= CHAR_ICON_ROW)
			{
				break;
			}
			else
			{
				cntRow++;
				cntColumn = 0;
			}		
		}

		float fPosX = vPos.x + cntColumn*(dwIconStandardWidth + BETWEEN_ICON_GAP);
		float fPosY = vPos.y + cntRow*(dwIconStandardHeight + BETWEEN_ICON_GAP);
		m_pkCharIcon[mit->first]->SetFixedWindowLocalPos(D3DXVECTOR2(fPosX,fPosY));
		//New Icon or Cash Icon check
		m_pkCharIcon[mit->first]->InitMark();
		m_pkCharIcon[mit->first]->ToggleRender(true);

		cntColumn++;
	}

	//신규계정이 아닌경우 가장 앞에 있는 케릭터에 포커싱을 맞춘다....요청사항부분임.
	if ( !g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() )
	{
		if (!mapOpenCharInfo.empty())
		{
			std::map<int,DWORD>::iterator mitFocusing = mapOpenCharInfo.begin();

			if (m_pkCharIcon[mitFocusing->first])
			{
				m_pkFocusingIcon = m_pkCharIcon[mitFocusing->first];
				SetSelectedCharIntro();
				m_pkSelectedCharFrame->SetSelectedCharFrame(mitFocusing->first);
			}
		}
	}

	if (m_pkFocusingIcon == NULL)
	{
        if ( mapOpenCharInfo.empty() )
        {
            m_bEmptyNoCreateChar = true;
            m_pkCharIconArrayRect->ToggleRender(false);
        }
        else
        {
            int nCntRow = ( (mapOpenCharInfo.size() - 1) / CHAR_ICON_COLUMN ) + 1;

		    DWORD dwRectWidth = ICON_STAR_GAP + ICON_END_GAP + CHAR_ICON_COLUMN * (dwIconStandardWidth + BETWEEN_ICON_GAP ) - BETWEEN_ICON_GAP;
		    DWORD dwRectHeight = ICON_STAR_GAP + ICON_END_GAP + nCntRow*(dwIconStandardHeight + BETWEEN_ICON_GAP ) - BETWEEN_ICON_GAP;

		    m_pkCharIconArrayRect->SetSize(dwRectWidth,dwRectHeight);
		    m_pkCharIconArrayRect->ToggleRender(true);

		    m_pkNoSelectedCharFrame->ToggleRender(true);
		    m_pkSelectedCharFrame->ToggleRender(false);
        }
	}
	else
	{
		m_pkCharIconArrayRect->ToggleRender(false);
	}
	
}

void KGCCharIntroFrame::SetSelectedCharIntro()
{

	if (m_pkFocusingIcon && m_pkFocusingIcon->IsRenderOn())
	{
		D3DXVECTOR2 vFocusingPos = m_pkFocusingIcon->GetFixedWindowLocalPos();
		vFocusingPos.x -= (m_pkCharFocus->GetWidth() - m_pkFocusingIcon->GetWidth())/2;
		vFocusingPos.y -= (m_pkCharFocus->GetHeight() - m_pkFocusingIcon->GetHeight())/2;

		m_pkCharFocus->SetFixedWindowLocalPos(vFocusingPos);
		m_pkCharFocus->ToggleRender(true);

		m_pkCharIconArrayRect->ToggleRender(false);
		m_cntBlinkTime = 0;
		m_pkNoSelectedCharFrame->ToggleRender(false);
		m_pkSelectedCharFrame->ToggleRender(true);
	}
}

void KGCCharIntroFrame::GoPreScene()
{
	g_MyD3D->m_pStateMachine->GoPrev();

}

void KGCCharIntroFrame::CreatCashParticle()
{
	for (int i=0 ; i<GC_CHAR_NUM ; i++)
	{
		std::vector<CParticleEventSeqPTR> vecParticle;
		vecParticle.clear();
		CParticleEventSeqPTR pParticle1 = g_ParticleManager->CreateSequence( "UI_Character_Ring_01", 0, 0, 0.2f );
		if (pParticle1)
		{
			pParticle1->SetShow(false);
			g_ParticleManager->SetLayer(pParticle1,GC_LAYER_UI);
			vecParticle.push_back(pParticle1);
		}
		CParticleEventSeqPTR pParticle2 = g_ParticleManager->CreateSequence( "UI_Character_Ring_02", 0, 0, 0.2f );
		if (pParticle2)
		{
			pParticle2->SetShow(false);
			g_ParticleManager->SetLayer(pParticle2,GC_LAYER_UI);
			vecParticle.push_back(pParticle2);
		}
		m_mapCashParticle.insert(std::make_pair(i ,vecParticle));
	}	
}

void KGCCharIntroFrame::OffCashParticle()
{
	if (m_mapCashParticle.empty())
		return;
	std::map<int,std::vector<CParticleEventSeqPTR>>::iterator mit = m_mapCashParticle.begin();
	for (; mit != m_mapCashParticle.end() ; ++mit)
	{
		if (mit->second.empty())
			continue;
		std::vector<CParticleEventSeqPTR>::iterator vit = mit->second.begin();
		for (; vit != mit->second.end() ; ++vit)
		{
			if ((*vit))
				(*vit)->SetShow(false);
		}
	}

}

void KGCCharIntroFrame::PostChildDraw()
{
}
