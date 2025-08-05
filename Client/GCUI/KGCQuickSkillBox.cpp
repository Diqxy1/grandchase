#include "stdafx.h"
#include "gcui/KGCQuickSkillBox.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCQuickSkillBox );
IMPLEMENT_WND_FACTORY( KGCQuickSkillBox );
IMPLEMENT_WND_FACTORY_NAME( KGCQuickSkillBox, "gc_quick_skill_box" );

KGCQuickSkillBox::KGCQuickSkillBox(void)
: m_pkStaticCurSlotType( NULL )
, m_pkBtnPrev( NULL )
, m_pkBtnNext( NULL )
, m_iCurSelectedSlotIndex( 0 )
{

    char str[MAX_PATH] = {0,};
    for( int i = 0; i < ESKILL_SLOT_COUNT; ++i )
    {
        m_pkQuickSkillSlot[i] = NULL;
        sprintf( str, "quick_skill_slot_%d", i );  
        LINK_CONTROL( str, m_pkQuickSkillSlot[i] ); 
    }

    LINK_CONTROL( "static_current_slot_type",   m_pkStaticCurSlotType );
    LINK_CONTROL( "btn_prev",                   m_pkBtnPrev );
    LINK_CONTROL( "btn_next",                   m_pkBtnNext );
}

KGCQuickSkillBox::~KGCQuickSkillBox(void)
{
}

void KGCQuickSkillBox::OnCreate( void )
{
    for( int i = 0; i < ESKILL_SLOT_COUNT; i++ ) {
        m_pkQuickSkillSlot[i]->InitState( true, true, this );
    }

    m_pkStaticCurSlotType->InitState( true );
    m_pkStaticCurSlotType->SetAlign( DT_CENTER );
    m_pkStaticCurSlotType->SetText( SiKGCSkillTree()->GetCurrentSlotType() + 1 );
    m_pkBtnPrev->InitState( true, true, this );
    m_pkBtnNext->InitState( true, true, this );
}

void KGCQuickSkillBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPrev && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnPrev();
        SpeakToActionListener( KActionEvent( this, KD3DWnd::D3DWE_BUTTON_CLICK , ECLICK_MOVE_BTN ) );
        return;
    }

    if( event.m_pWnd == m_pkBtnNext && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnNext();
        SpeakToActionListener( KActionEvent( this, KD3DWnd::D3DWE_BUTTON_CLICK , ECLICK_MOVE_BTN ) );
        return;
    }


    for( int i = 0; i < ESKILL_SLOT_COUNT; i++ )
    {        
        if( event.m_pWnd == m_pkQuickSkillSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            if( SiKGCSkillTree()->GetCurrentSlotType() < KSkillSlot::INDEX_MAX ) {
                SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] = i;
            }
            
            // 이 상황일 때는 셀렉트 윈도우 끄도록 이벤트 보내 주자.
            if( m_pkQuickSkillSlot[m_iCurSelectedSlotIndex]->IsSelectedSlot() && m_iCurSelectedSlotIndex == i ) {
                m_pkQuickSkillSlot[m_iCurSelectedSlotIndex]->SetSelectSlot( false );
                SpeakToActionListener( KActionEvent( this, i , ESELECT_SKILL_WND_OFF ) );
            } else {                                
                m_pkQuickSkillSlot[m_iCurSelectedSlotIndex]->SetSelectSlot( false );
                m_iCurSelectedSlotIndex = i;
                m_pkQuickSkillSlot[i]->SetSelectSlot( true );
                SpeakToActionListener( KActionEvent( this, i , ECLICK_SKILL_SLOT ) );
            }
            g_KDSound.Play( "31" );
            return;
        }
    }
}


void KGCQuickSkillBox::FrameMoveInEnabledState( void )
{   
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    for( int i = 0; i < ESKILL_SLOT_COUNT; i++ ) {
        if( m_pkQuickSkillSlot[i]->CheckPosInWindowBound(vMousePos) ) {
            m_pkQuickSkillSlot[i]->HoverQuickSlot( true );
        } else {
            m_pkQuickSkillSlot[i]->HoverQuickSlot( false );
        }
    }
}

void KGCQuickSkillBox::PostChildDraw( void )
{
}

void KGCQuickSkillBox::OnPrev()
{
    if( SiKGCSkillTree()->GetCurrentSlotType() <= KSkillSlot::INDEX_A ) {
        return;
    }

    SiKGCSkillTree()->SetCurrentSlotType( SiKGCSkillTree()->GetCurrentSlotType() - 1 );
    UpdateSkillSlot();
}

void KGCQuickSkillBox::OnNext()
{
    if( SiKGCSkillTree()->GetCurrentSlotType() >= KSkillSlot::INDEX_MAX - 1 ) {
        return;
    }

    SiKGCSkillTree()->SetCurrentSlotType( SiKGCSkillTree()->GetCurrentSlotType() + 1 );
    UpdateSkillSlot();
}

void KGCQuickSkillBox::UpdateSkillSlot()
{
    if( SiKGCSkillTree()->GetCurrentSlotType() >= KSkillSlot::INDEX_MAX ) {
        return;
    }

    m_pkStaticCurSlotType->SetText( SiKGCSkillTree()->GetCurrentSlotType() + 1 );

    std::vector<EGCSkillTree> set = SiKGCSkillTree()->GetHotKeySkillVector( SiKGCSkillTree()->GetCurUICharType(), SiKGCSkillTree()->GetCurUIPromotion() );

    if( !set.empty() )  {
        std::vector<EGCSkillTree>::iterator sit = set.begin();
        int iIndex = 0;
        for( sit; sit != set.end(); ++sit ) {
            if( *sit != SID_DIO1_EMPTY_SLOT ) {
                m_pkQuickSkillSlot[iIndex]->SetSkillImage( *sit );
            } else {
                m_pkQuickSkillSlot[iIndex]->InitSkillImage();
            }
            iIndex++;

            if( iIndex >= ESKILL_SLOT_COUNT ) {
                break;
            }
        }
    } else {
        for( int i = 0; i < ESKILL_SLOT_COUNT; i++ ) {
            m_pkQuickSkillSlot[i]->InitSkillImage();
        }
    }
}

void KGCQuickSkillBox::SetQuickSkillSlot( EGCSkillTree eSkillID_ )
{
    const GCSkill* pSkill = SiKGCSkillTree()->GetSkill( eSkillID_ );

    if( pSkill == NULL ) {
        return;
    }

    for( int i = 0; i < ESKILL_SLOT_COUNT; i++ ) {
        if( SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] != i ) {
            const GCSkill* pPrevSkill = SiKGCSkillTree()->GetSkill( m_pkQuickSkillSlot[ i ]->GetSkillID() );
            if( pPrevSkill && ( pPrevSkill->GetID() == pSkill->GetGroupID() || pPrevSkill->GetGroupID() == pSkill->GetGroupID() ) ) {
                m_pkQuickSkillSlot[ i ]->UnEquipSlot();
                break;
            }
        }
    }

    if( SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] < ESKILL_SLOT_COUNT ) {
        if( m_pkQuickSkillSlot[ SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] ]->GetSkillID() == eSkillID_ ) {
            m_pkQuickSkillSlot[ SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] ]->UnEquipSlot();
        } else {
            m_pkQuickSkillSlot[ SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] ]->EquipSlot( eSkillID_ );
            m_pkQuickSkillSlot[ SiKGCSkillTree()->m_iHotKeyIndex[SiKGCSkillTree()->GetCurrentSlotType()] ]->SetSelectSlot( false );
        }                
    }    
}
