#include "StdAfx.h"
#include ".\gcdummyskillbox.h"

IMPLEMENT_CLASSNAME( KGCDummySkillBox );
IMPLEMENT_WND_FACTORY( KGCDummySkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCDummySkillBox, "gc_skillbox_dummy" );

KGCDummySkillBox::KGCDummySkillBox(void)
{
}

KGCDummySkillBox::~KGCDummySkillBox(void)
{
}

void KGCDummySkillBox::SetGroup( EGCSkillGroup eGroupID )
{
	m_eGroupID = eGroupID;
}

void KGCDummySkillBox::SetSkill( EGCSkillTree eSkillID )
{
	m_eSkillID = SiGCSKT()->GetMaxSID();

	m_kSkillImage.RemoveAllTexture();

	m_kSkillImage.CameraMatrixOn();
	m_kSkillImage.AddTexture( "KAMIKICIRCLE.DDS" );

	SetActive( false );
	SetBoxState();
}

void KGCDummySkillBox::SetBoxState()
{
	m_pkLocked->ToggleRender( false );
	m_pkNormal->ToggleRender( true );
	m_pkActive->ToggleRender( false );

	m_pkPlus->Lock( true );
	m_pkMinus->Lock( true );
	m_kSkillImage.SetRGB( 0xFF, 0xFF, 0xFF );
}

// 아무것도 없는 스킬 박스이기 때문에 동작하지 않도록 바꿈.
void KGCDummySkillBox::OnPlus()
{
	// 아무것도 하지 않아요.
}

void KGCDummySkillBox::OnMinus()
{
	// 아무것도 하지 않아요.
}