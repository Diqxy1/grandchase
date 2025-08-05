#include "stdafx.h"
#include ".\gcskillboxmain.h"
#include "GCSKT.h"
#include "GCSkill.h"
//

IMPLEMENT_CLASSNAME( KGCSkillBoxMain );
IMPLEMENT_WND_FACTORY( KGCSkillBoxMain );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillBoxMain, "gc_skillbox_main" );

KGCSkillBoxMain::KGCSkillBoxMain(void)
: m_eSkillGroup( (EGCSkillGroup)0 )
, m_iTitleID( 0 )
{
	m_pkName = NULL;
	m_pkActive = NULL;
	m_pkNormal = NULL;
	m_pkBlink	= NULL;

	LINK_CONTROL( "static_skillname", m_pkName );
	LINK_CONTROL( "active", m_pkActive );
	LINK_CONTROL( "normal", m_pkNormal );
	LINK_CONTROL( "blink",	m_pkBlink );

    m_eSkillSlot = SiGCSKT()->GetMaxSID();
}

KGCSkillBoxMain::~KGCSkillBoxMain(void)
{
}

void KGCSkillBoxMain::ActionPerformed( const KActionEvent& event )
{

}

void KGCSkillBoxMain::SetSkill( EGCSkillTree eSkillID )
{
	const GCSkill* pSkill = SiGCSKT()->GetSkill( eSkillID );

	if( pSkill == NULL )
	{
		m_pkActive->ToggleRender( false );
		m_pkNormal->ToggleRender( true );
		return;
	}

	if( pSkill->GetGroupID() != m_eSkillGroup )
	{
		ASSERT( !"This skill is not in this group!!" );
		return;
	}

	m_pkActive->ToggleRender( true );
	m_pkNormal->ToggleRender( false );

	m_eSkillSlot = eSkillID;

	m_kSkillImage.RemoveAllTexture();

	float vPosX = ( this->GetFixedWindowPos().x + 8.0f ) / (400.0f * GC_SCREEN_DIV_WIDTH);
	float vPosY = -( this->GetFixedWindowPos().y - 10.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f ;

	m_kSkillImage.CameraMatrixOn();
	//	m_kSkillImage.SetPosition( ( this->GetFixedWindowPos().x + 7.0f ) / (400.0f * GC_SCREEN_DIV_WIDTH), -( this->GetFixedWindowPos().y + 10.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
	m_kSkillImage.SetPosition( vPosX, vPosY );
	m_kSkillImage.SetScale( 0.115f*GC_SCREEN_OLD_DIV_WIDTH, 0.115f*GC_SCREEN_OLD_DIV_WIDTH );
	//  m_kSkillImage.AddTexture( "asdfasdf" );
	m_kSkillImage.AddTexture( pSkill->GetTextureFileName().c_str() );
}

void KGCSkillBoxMain::SetSkillGroup( EGCSkillGroup eGroup )
{
	m_eSkillGroup = eGroup;
}

void KGCSkillBoxMain::PostChildDraw()
{
	m_kSkillImage.Render();
}

void KGCSkillBoxMain::OnCreate()
{
	m_pkName->SetAlign( DT_CENTER );
    m_pkName->SetAutoScroll(true);

	m_pkBlink->InitState( false );
}

void KGCSkillBoxMain::Update()
{

}

void KGCSkillBoxMain::SetBlank()
{
	//m_pkName->SetText();
	m_kSkillImage.RemoveAllTexture();
	m_pkActive->ToggleRender( false );
	m_pkNormal->ToggleRender( true );
	m_eSkillSlot = SiGCSKT()->GetMaxSID();
	return;
}

EGCSkillTree KGCSkillBoxMain::GetEquipedSkill()
{
	return m_eSkillSlot;
}

void KGCSkillBoxMain::SetTitleID( int iTitleID )
{
	m_iTitleID = iTitleID;
	m_pkName->SetText( SiGCSKT()->GetString( m_iTitleID ) );
}

void KGCSkillBoxMain::FrameMoveInEnabledState()
{
	ASSERT( g_pkInput != NULL );

	D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if( CheckPosInWindowBound( vMousePos ) == true && m_pkActive->IsRenderOn() )
	{
		m_pkBlink->ToggleRender( true );
	}
	else
	{
		m_pkBlink->ToggleRender( false );
	}
}