#include "stdafx.h"
#include "GCRitasPlayer.h"

IMPLEMENT_CLASSNAME( KGCRitasPlayer );
IMPLEMENT_WND_FACTORY( KGCRitasPlayer );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasPlayer, "gc_ritasplayer" );

KGCRitasPlayer::KGCRitasPlayer( void ):
m_pkCatchRectRight(NULL),
m_pkCatchRectLeft(NULL),
m_pkLimeRight(NULL),
m_pkLimeLeft(NULL)
{

	LINK_CONTROL( "catch_rect_right", m_pkCatchRectRight );
	LINK_CONTROL( "catch_rect_left", m_pkCatchRectLeft );
	LINK_CONTROL( "lime_right", m_pkLimeRight );
	LINK_CONTROL( "lime_left", m_pkLimeLeft );

	m_bRight = true;
	m_iAniFrame = 0;

	m_pkLimePt = NULL;
	m_pkCatchRectPt = NULL;
}

KGCRitasPlayer::~KGCRitasPlayer( void )
{
}

void KGCRitasPlayer::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasPlayer::OnCreate( void )
{
	m_pkCatchRectRight->InitState(false);

	m_pkCatchRectLeft->InitState(false);

	m_pkLimeRight->InitState(true);
	m_pkLimeRight->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
	m_pkLimeLeft->InitState(true);
	m_pkLimeLeft->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));


	m_bRight = true;
	m_pkLimePt = m_pkLimeRight;
	m_pkCatchRectPt = m_pkCatchRectRight;

}

void KGCRitasPlayer::SetDirectionRight( bool bRight_ )
{
	if (m_bRight == bRight_)
	{
		//애니메이션용 텍스쳐는 4장
		if (m_iAniFrame < NUM_WINDOW_MODE - 1)
		{
			m_iAniFrame++;
			if (m_pkLimePt)
				m_pkLimePt->SetWndMode(static_cast<ED3DWndMode>(m_iAniFrame));
		}
		else
		{
			m_iAniFrame = 0;
		}
		return;
	}

	m_bRight = bRight_;
	m_iAniFrame = 0;

	if (m_bRight)
	{
		m_pkLimeRight->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
		//m_pkLimeRight->ToggleRender(true);
		m_pkLimeLeft->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));
		//m_pkLimeLeft->ToggleRender(false);
		
		

		m_pkLimePt = m_pkLimeRight;
		m_pkCatchRectPt = m_pkCatchRectRight;
	}
	else
	{
		m_pkLimeRight->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));
		//m_pkLimeRight->ToggleRender(false);
		m_pkLimeLeft->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
		//m_pkLimeLeft->ToggleRender(true);
		
		
		
		m_pkLimePt = m_pkLimeLeft;
		m_pkCatchRectPt = m_pkCatchRectLeft;
	}
	
}

void KGCRitasPlayer::SetFixedWindowLocalBasketPos( const D3DXVECTOR2& vPos_ )
{
	m_vWindowPos = vPos_;
	if (m_bRight)
		m_vWindowPos.x -= m_pkCatchRectRight->GetFixedWindowLocalPos().x;
	else
		m_vWindowPos.x -= m_pkCatchRectLeft->GetFixedWindowLocalPos().x;
	
}

void KGCRitasPlayer::FrameMoveInEnabledState()
{

}

DWORD KGCRitasPlayer::GetBasketWidth()
{
	if (m_bRight)
		return m_pkCatchRectRight->GetWidth();
	else
		return m_pkCatchRectLeft->GetWidth();

}

D3DXVECTOR2 KGCRitasPlayer::GetBasketPos()
{
	D3DXVECTOR2 vPos = GetFixedWindowLocalPos();
	if (m_bRight)
		return vPos + m_pkCatchRectRight->GetFixedWindowLocalPos();
	else
		return vPos + m_pkCatchRectLeft->GetFixedWindowLocalPos();
}

bool KGCRitasPlayer::CheckCatchObj(const KGCRitasObj* pkObj_)
{
	if ( m_pkCatchRectPt == NULL )
		return false;

	D3DXVECTOR2 vBasketPos = GetFixedWindowLocalPos() + m_pkCatchRectPt->GetFixedWindowLocalPos();

	//체크하는 오브젝트의 50% 초과시 바구니와 겹쳐졌을시에 잡았다는 신호를 준다.
	float fObjCheckPosX = pkObj_->GetFixedWindowLocalPos().x + (static_cast<float>(pkObj_->GetWidth())/2.0f);
	if ( fObjCheckPosX > vBasketPos.x && 
		fObjCheckPosX < vBasketPos.x + static_cast<float>(m_pkCatchRectPt->GetWidth()))
		return true;
	else
		return false;

}

