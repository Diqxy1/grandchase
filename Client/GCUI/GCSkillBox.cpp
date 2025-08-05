#include "StdAfx.h"
#include ".\gcskillbox.h"

#include "GCSKT.h"
#include "GCSkill.h"
#include "GCSkillBoxSub.h"


IMPLEMENT_CLASSNAME( KGCSkillBox );
IMPLEMENT_WND_FACTORY( KGCSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillBox, "gc_skillbox" );

KGCSkillBox::KGCSkillBox(void)
: m_pkActive( NULL )
, m_pkNormal( NULL )
, m_pkLocked( NULL )
, m_pkSelected( NULL )
, m_pkPlus( NULL )
, m_pkMinus( NULL )
, m_eSkillID( (EGCSkillTree)0 )
{
	LINK_CONTROL( "normal", m_pkNormal );
	LINK_CONTROL( "active", m_pkActive );
	LINK_CONTROL( "locked", m_pkLocked );
	LINK_CONTROL( "selected", m_pkSelected );

	LINK_CONTROL( "btn_plus", m_pkPlus );
	LINK_CONTROL( "btn_minus", m_pkMinus );
}

KGCSkillBox::~KGCSkillBox(void)
{
}

void KGCSkillBox::OnCreate()
{
	m_pkNormal->InitState( true );
	m_pkActive->InitState( false );
	m_pkLocked->InitState( false );
	m_pkSelected->InitState( false );

	m_pkPlus->InitState( true, true, this );
	m_pkMinus->InitState( true, true, this );

#if !defined(NATION_KOREA)
    m_pkPlus->SetToolTip( g_pkStrLoader->GetString(STR_ID_SKILLTREE3) );
    m_pkMinus->SetToolTip( g_pkStrLoader->GetString(STR_ID_SKILLTREE4) );   
#endif
}

void KGCSkillBox::SetSkill( EGCSkillTree eSkillID )
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( eSkillID );

	if( pSkill == NULL )
		return;

	m_eSkillID = eSkillID;
	m_eGroupID = pSkill->GetGroupID();

	m_kSkillImage.RemoveAllTexture();

	m_kSkillImage.CameraMatrixOn();
	//m_kSkillImage.AddTexture( "asdfasdf" );
	m_kSkillImage.AddTexture( pSkill->GetTextureFileName().c_str() );

	SetActive( false );
	SetBoxState();
}

void KGCSkillBox::SetBoxState()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );
	if( SiGCSKT()->IsLearnedSkill( m_eSkillID ) == true )
	{
		m_pkLocked->ToggleRender( false );
		m_pkNormal->ToggleRender( true );
		m_pkActive->ToggleRender( false );

		m_pkPlus->Lock( true );
		m_pkMinus->Lock( false );
		m_kSkillImage.SetRGB( 0xFF, 0xFF, 0xFF );
	}
	else
	{
		m_pkLocked->ToggleRender( true );
		m_pkNormal->ToggleRender( false );
		m_pkActive->ToggleRender( false );

		if( SiGCSKT()->IsSkillTrainable( m_eSkillID ) == false )
		{
			m_pkLocked->ToggleRender( true );
			m_pkNormal->ToggleRender( false );
			m_pkPlus->Lock( true );
			m_kSkillImage.SetRGB( 50, 50, 50 );
		}
		else
		{
			m_pkPlus->Lock( false );
			//m_kSkillImage.SetRGB( 50, 50, 50 );
			m_kSkillImage.SetRGB( 0xFF, 0xFF, 0xFF );
		}
		m_pkMinus->Lock( true );
	}

	if( pSkill->IsCash() == true )
	{
		m_pkPlus->Lock( true );
		m_pkMinus->Lock( true );
	}
}

void KGCSkillBox::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkPlus,	KD3DWnd::D3DWE_BUTTON_CLICK, OnPlus		);
	GCWND_MSG_MAP( m_pkMinus,	KD3DWnd::D3DWE_BUTTON_CLICK, OnMinus	);
}

void KGCSkillBox::PostChildDraw()
{
	m_kSkillImage.Render();
}

void KGCSkillBox::FrameMoveInEnabledState()
{

	ASSERT( g_pkInput != NULL );

	D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if( CheckPosInWindowBound( vMousePos ) == true && m_pkNormal->IsRenderOn() )
	{
		m_pkActive->ToggleRender( true );
	}
	else
	{
		m_pkActive->ToggleRender( false );
	}
}

void KGCSkillBox::OnPlus()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );
	assert( pSkill != NULL );


	if ( SiGCSKT()->IsUnlockAll() )
	{
		SiGCSKT()->_D_TrainSkill( pSkill->GetCharType(), pSkill->GetPromotion(), m_eSkillID );
		SiGCSKT()->_D_Equip_Skill( pSkill->GetCharType(), pSkill->GetPromotion(), SiGCSKT()->GetSetting( pSkill->GetCharType(), pSkill->GetPromotion() ),m_eSkillID );
		g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
		return;
	}
	if( 0 == SiGCSKT()->GetExtraSP( (int)pSkill->GetCharType() ) ||
		SiGCSKT()->GetMaxSPinPromotion( (int)pSkill->GetCharType(), pSkill->GetPromotion() ) <= SiGCSKT()->GetUsedSPinPromotion( (int)pSkill->GetCharType(), pSkill->GetPromotion() ) )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOMORE_SKILL_POINT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}
	else
	{
#if defined( NO_MSG_BOX_SKILL_LEARN_AND_REMOVE )
		//g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE1, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_LEARN_SKILL, 0, 0, false, true );
        KGCSkillBoxSub::SetRequestedSkill( m_eSkillID );
        m_pkPlus->Lock( true );		
        KP2P::GetInstance()->Send_SkillTrainingReq( KGCSkillBoxSub::GetRequestedSkill() );        
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE1, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_LEARN_SKILL, 0, 0, false, true );
        KGCSkillBoxSub::SetRequestedSkill( m_eSkillID );
#endif
	}
}

void KGCSkillBox::OnMinus()
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( m_eSkillID );
	assert( pSkill != NULL );

	if ( SiGCSKT()->IsUnlockAll() )
	{
		SiGCSKT()->_D_UnTrainSkill( pSkill->GetCharType(), pSkill->GetPromotion(), m_eSkillID );
		SiGCSKT()->_D_UnEquip_Skill( pSkill->GetCharType(), pSkill->GetPromotion(), SiGCSKT()->GetSetting( pSkill->GetCharType(), pSkill->GetPromotion() ),m_eSkillID );
		g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
		return;
	}

	std::pair<bool,DWORD> UntrainItem = SiGCSKT()->IsExistSkillUntrainItem();

	KGCSkillBoxSub::SetUntrainItemID(UntrainItem.second);
	if( false == UntrainItem.first )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}
	else
	{
        const SkillSet& setNextID = pSkill->GetNextSet();
        for( std::set< EGCSkillTree >::const_iterator sit = setNextID.begin(); sit != setNextID.end(); ++sit ) {
            const GCSkill* pNextSkill = SiGCSKT()->GetSkill( *sit );
            if( pNextSkill && pNextSkill->m_bOrCheckPreList && SiGCSKT()->IsLearnedSkill( *sit ) ) {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_SKILL_UNTRAIN_CONDITION_ERROR1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                return;
            }
        }

		//g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE15, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_REMOVE_SKILL, 0, 0, false, true );
#if defined( NO_MSG_BOX_SKILL_LEARN_AND_REMOVE )
        m_pkMinus->Lock( true );
		KGCSkillBoxSub::SetRequestedSkill( m_eSkillID );
        KP2P::GetInstance()->Send_RemoveSkill( KGCSkillBoxSub::GetRequestedSkill(), KGCSkillBoxSub::GetUntrainItemID() );
#else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SKILLTREE15, "li", SiGCSKT()->GetString( pSkill->GetTitleID() ), pSkill->GetLearnSP() ), L"",KGCUIScene::GC_MBOX_USE_REMOVE_SKILL, 0, 0, false, true );
        KGCSkillBoxSub::SetRequestedSkill( m_eSkillID );
#endif
	}
}

void KGCSkillBox::SetActive( bool bActive )
{
	m_pkPlus->ToggleRender( !bActive );
	m_pkMinus->ToggleRender( !bActive );
	if( bActive == false )
	{
		SetBoxState();
		m_kSkillImage.SetPosition( ( this->GetFixedWindowPos().x + 10.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -( this->GetFixedWindowPos().y - 46.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
		m_kSkillImage.SetScale( 0.07f * GC_SCREEN_OLD_DIV_WIDTH, 0.07f * GC_SCREEN_OLD_DIV_WIDTH);
	}
	else
	{
		m_pkNormal->ToggleRender( false );
		m_kSkillImage.SetPosition( ( this->GetFixedWindowPos().x + 4.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -( this->GetFixedWindowPos().y - 36.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
		m_kSkillImage.SetScale( 0.1f * GC_SCREEN_OLD_DIV_WIDTH, 0.1f * GC_SCREEN_OLD_DIV_WIDTH);
	}
	m_pkSelected->ToggleRender( bActive );

}