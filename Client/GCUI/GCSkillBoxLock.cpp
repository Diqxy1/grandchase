#include "stdafx.h"
//
#include ".\gcskillboxlock.h"
//
#include "GCSKT.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
//

IMPLEMENT_CLASSNAME( KGCSkillBoxLock );
IMPLEMENT_WND_FACTORY( KGCSkillBoxLock );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillBoxLock, "gc_skillbox_lock" );

KGCSkillBoxLock::KGCSkillBoxLock(void)
{
	m_pkLock = NULL;
	m_pkRegion = NULL;

	LINK_CONTROL( "btn_locker", m_pkLock );
	LINK_CONTROL( "region", m_pkRegion );
}

KGCSkillBoxLock::~KGCSkillBoxLock(void)
{
}

void KGCSkillBoxLock::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( event.m_pWnd, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickLockBtn );
}

void KGCSkillBoxLock::SetSize( DWORD dwWidth, DWORD dwHeight )
{
	m_pkRegion->SetSize( dwWidth, dwHeight );
}

void KGCSkillBoxLock::OnClickLockBtn()
{
	if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_GROUP_OPEN_ERROR3 ),
			L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

#if defined( __OPEN_TEST__ )
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
		g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY1 ),
		g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY2 ) );

	return;
// 	SiGCSKT()->SetBuyingReqGroup( 0 );
// 	if( SiGCSKT()->GetBuyingReqGroup() != -1 )
// 	{
// 		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_GROUP_OPEN_UNKNOWNERROR ),
// 			g_pkStrLoader->GetString( STR_ID_FRIEND_ERROR6 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
// 		return;
// 	}
#endif

	if( false == SiGCSKT()->Process_UnlockGroupItem( m_iLockID, false ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_GROUP_OPEN_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}

void KGCSkillBoxLock::SetLockID( int iLockID )
{
	m_iLockID = iLockID;
}

void KGCSkillBoxLock::OnCreate()
{
	m_pkLock->InitState( true, true, this );
	m_pkRegion->InitState( true );
	SetUnlockButtonPosition( SBL_LEFT_TOP );
	m_pkLock->SetToolTip( g_pkStrLoader->GetString( STR_ID_SKILLTREE20 ) );
}

void KGCSkillBoxLock::SetUnlockButtonPosition( int iPositionID )
{
	switch( iPositionID )
	{
	case SBL_LEFT_TOP:
		m_pkLock->SetWindowPosDirect( D3DXVECTOR2( 6.0f, 6.0f) );
		break;
	case SBL_RIGHT_TOP:
		m_pkLock->SetWindowPosDirect( D3DXVECTOR2( m_pkRegion->GetWidth() - m_pkLock->GetWidth() -6.0f, 6.0f) );
		break;
	case SBL_LRFT_BOTTOM:
		m_pkLock->SetWindowPosDirect( D3DXVECTOR2( 6.0f, m_pkRegion->GetHeight()-6.0f) );
		break;
	case SBL_RIGHT_BOTTOM:
		m_pkLock->SetWindowPosDirect( D3DXVECTOR2( m_pkRegion->GetWidth() - m_pkLock->GetWidth() -6.0f, m_pkRegion->GetHeight()-6.0f) );
		break;
	}
}