#include "StdAfx.h"
#include ".\GCSkillSlotBox.h"

IMPLEMENT_CLASSNAME( KGCSkillSlotBox );
IMPLEMENT_WND_FACTORY( KGCSkillSlotBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillSlotBox, "gc_SlotSkillBox" );

KGCSkillSlotBox::KGCSkillSlotBox(void)
{
	LINK_CONTROL( "skillSlot_A", m_apkSkillSlot[0] );
	LINK_CONTROL( "skillSlot_S", m_apkSkillSlot[1] );
	LINK_CONTROL( "skillSlot_D", m_apkSkillSlot[2] );
	LINK_CONTROL( "skillSlot_F", m_apkSkillSlot[3] );
	LINK_CONTROL( "skillSlot_G", m_apkSkillSlot[4] );

    m_pkBtnSlotA = NULL;
	m_pkBtnSlotB = NULL;    

    LINK_CONTROL( "btn_slot_A", m_pkBtnSlotA );
    LINK_CONTROL( "btn_slot_B", m_pkBtnSlotB );
}

KGCSkillSlotBox::~KGCSkillSlotBox(void)
{
}

void KGCSkillSlotBox::OnCreate()
{    
	for( int i = 0 ; i < MAX_SLOT_COUNT ; ++i )
	{
		m_apkSkillSlot[i]->InitState(true,true,this);
		m_apkSkillSlot[i]->m_dwOptionalData = SID_DIO1_EMPTY_SLOT;
	}
	KD3DStatic* title = (KD3DStatic*)GetChildByName("static_title");

	title->ToggleRender( true );
	title->SetAlign( DT_CENTER );
	title->SetText( g_pkStrLoader->GetString( 6389 ) );
}

void KGCSkillSlotBox::SetSkillImage( int iIndex, std::string strTexture )
{
	m_apkSkillSlot[iIndex]->SetSkillImage( strTexture );
}

void KGCSkillSlotBox::ActionPerformed( const KActionEvent& event )
{
	for( int i = 0 ; i < MAX_SLOT_COUNT ; ++i )
	{
		if( m_apkSkillSlot[i] == event.m_pWnd )
		{
			if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
			{
				if ( m_apkSkillSlot[i]->UnequipSlot() )
				{
					g_pkUIScene->m_pkSkillTree->Update();
				}

#if defined( USE_AP_SLOT_CHANGE )
                if( SiGCSKT()->GetCurrentSlotType() < KSkillSlot::INDEX_MAX ) {
				    SiGCSKT()->m_iHotKeyIndex[SiGCSKT()->GetCurrentSlotType()] = i;
                }
#else
                SiGCSKT()->m_iHotKeyIndex = i;
#endif

				Update();
			}
			else if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
			{				
				g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( true );

				const GCSkill* pSkill = SiGCSKT()->GetSkill( (EGCSkillTree)m_apkSkillSlot[i]->m_dwOptionalData );
				if( pSkill != NULL && pSkill->GetID() != SID_DIO1_EMPTY_SLOT )
				{
					g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetTitle( g_pkUIScene->m_pkSkillTree->GetSkillTitle( pSkill ) );
					g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetDesc( SiGCSKT()->GetString( pSkill->GetDescID() ) );
					g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetMiddleDesc( g_pkUIScene->m_pkSkillTree->GetMiddleDesc( pSkill ) );

					g_pkUIScene->m_pkSkillTree->m_pkDescBox->SetTargetPosition( m_apkSkillSlot[i]->GetCurrentWindowPos(), m_apkSkillSlot[i]->GetWidth(), m_apkSkillSlot[i]->GetHeight() );
				}
				else
					g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( false );
			}
			else if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE )
			{				
				g_pkUIScene->m_pkSkillTree->m_pkDescBox->ToggleRender( false );
			}

		}
	}

#if defined( USE_AP_SLOT_CHANGE )
    if( g_kGlobalValue.GetOpenSkillSlotIndex( SiGCSKT()->GetCurUICharType() ) > KSkillSlot::INDEX_A ) {
        if( event.m_pWnd == m_pkBtnSlotA && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {               
            SiGCSKT()->SetCurrentSlotType( KSkillSlot::INDEX_A );
            g_pkUIScene->m_pkSkillTree->Update();
        } else if( event.m_pWnd == m_pkBtnSlotB && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
            SiGCSKT()->SetCurrentSlotType( KSkillSlot::INDEX_B );
            g_pkUIScene->m_pkSkillTree->Update();
        }
    }
#endif
}

void KGCSkillSlotBox::Update()
{
#if defined( USE_AP_SLOT_CHANGE )
    if( SiGCSKT()->GetCurrentSlotType() >= KSkillSlot::INDEX_MAX ) {
        return;
    }

	std::vector<EGCSkillTree> set = SiGCSKT()->GetHotKeySkillVector( SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType, 0 );

	std::vector<EGCSkillTree>::iterator sit = set.begin();

	int index = 0;
	while( sit != set.end() && index < MAX_SLOT_COUNT ) 
	{
		SetSkillImage( index, SiGCSKT()->GetSkill( *sit )->GetTextureFileName() );
		m_apkSkillSlot[index]->Activate( SiGCSKT()->m_iHotKeyIndex[ SiGCSKT()->GetCurrentSlotType() ] == index );
		m_apkSkillSlot[index]->m_dwOptionalData = (DWORD) *sit;
		index++;
		sit++;
	}

	while( index < MAX_SLOT_COUNT )
	{
		m_apkSkillSlot[index]->Activate( SiGCSKT()->m_iHotKeyIndex[ SiGCSKT()->GetCurrentSlotType() ] == index );
		SetSkillImage( index, "special75.dds" );
		index++;
	}
#else
    std::vector<EGCSkillTree> set = SiGCSKT()->GetHotKeySkillVector( SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType, 0 );

    std::vector<EGCSkillTree>::iterator sit = set.begin();

    int index = 0;
    while( sit != set.end() && index < MAX_SLOT_COUNT ) 
    {
        SetSkillImage( index, SiGCSKT()->GetSkill( *sit )->GetTextureFileName() );
        m_apkSkillSlot[index]->Activate( SiGCSKT()->m_iHotKeyIndex == index );
        m_apkSkillSlot[index]->m_dwOptionalData = (DWORD) *sit;
        index++;
        sit++;
    }

    while( index < MAX_SLOT_COUNT )
    {
        m_apkSkillSlot[index]->Activate( SiGCSKT()->m_iHotKeyIndex == index );
        SetSkillImage( index, "special75.dds" );
        index++;
    }
#endif

    UpdateChangeSkillSlot();
}

void KGCSkillSlotBox::UpdateChangeSkillSlot()
{    
#if defined( USE_AP_SLOT_CHANGE )
    if( g_kGlobalValue.GetOpenSkillSlotIndex( SiGCSKT()->GetCurUICharType() ) > KSkillSlot::INDEX_A ) {
        m_pkBtnSlotA->InitState( true, true, this );
        m_pkBtnSlotB->InitState( true, true, this );
        if( SiGCSKT()->GetCurrentSlotType() ==  KSkillSlot::INDEX_A ) {                
            m_pkBtnSlotA->Lock( true );            
            m_pkBtnSlotB->Lock( false );
        } else if( SiGCSKT()->GetCurrentSlotType() ==  KSkillSlot::INDEX_B ) {
            m_pkBtnSlotA->Lock( false );            
            m_pkBtnSlotB->Lock( true );
        }
    } else {
        m_pkBtnSlotA->InitState( false );
        m_pkBtnSlotB->InitState( false );
    }    
#else
    if( m_pkBtnSlotA && m_pkBtnSlotB ) {
        m_pkBtnSlotA->InitState( false );
        m_pkBtnSlotB->InitState( false );
    }    
#endif
}


IMPLEMENT_CLASSNAME( KGCSkillSlot );
IMPLEMENT_WND_FACTORY( KGCSkillSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillSlot, "gc_SkillSlot" );

KGCSkillSlot::KGCSkillSlot(void)
{
	m_pSkillImage = NULL;
	m_Active = NULL;
	LINK_CONTROL( "check", m_Equipped );	
	LINK_CONTROL( "active", m_Active );
}

KGCSkillSlot::~KGCSkillSlot(void)
{
}

void KGCSkillSlot::SetSkillImage( std::string strTexture )
{
	if (m_pSkillImage)
	{
		m_pSkillImage->Release();
	}

	m_pSkillImage = g_pGCDeviceManager2->CreateTexture( strTexture );

	m_Equipped->ToggleRender( strTexture != "special75.dds" );


}

void KGCSkillSlot::PostChildDraw()
{
	if ( !m_pSkillImage )
	{
		return;
	}
	
	const KSafeArray<KD3DUIVertex,4>& pVertex = GetWindowRect();
	g_pGCDeviceManager2->DrawInScreen(m_pSkillImage,
		(pVertex[0].m_vPos.x + 5 ) * m_fWindowScaleX,
		(pVertex[0].m_vPos.y + 5 ) * m_fWindowScaleY,
		(pVertex[3].m_vPos.x - 5 ) * m_fWindowScaleX,
		(pVertex[3].m_vPos.y - 5 ) * m_fWindowScaleY,
		0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f );
}

void KGCSkillSlot::OnActivate( void )
{
	if (m_Active)
	{
		m_Active->ToggleRender( IsActive() );
	}
	
	
}

bool KGCSkillSlot::UnequipSlot()
{
	if ( m_dwOptionalData == SID_DIO1_EMPTY_SLOT ||
		m_dwOptionalData == 0 )
	{
		return false;
	}
	
	SiGCSKT()->UnequipSkill( SiGCSKT()->GetSkill( (EGCSkillTree)m_dwOptionalData )->GetGroupID(), SiGCSKT()->GetSetting( SiGCSKT()->GetOwnerPlayer()->GetCurrentChar().iCharType, 0 ) );		
	m_dwOptionalData = SID_DIO1_EMPTY_SLOT;	
	return true;
}



