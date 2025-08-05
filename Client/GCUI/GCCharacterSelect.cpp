#include "stdafx.h"
#include "GCCharacterUI.h"
#include "GCCharacterSelect.h"

IMPLEMENT_CLASSNAME( KGCCharacterSelect );
IMPLEMENT_WND_FACTORY( KGCCharacterSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterSelect, "gc_characterselect" );

KGCCharacterSelect::KGCCharacterSelect( void )
{
    m_iMode = KGCCharacterSelectBox::STATE_POSITION_DEFAULT;

    m_pkCharList = NULL;
    LINK_CONTROL( "char_list", m_pkCharList );

    SetPositionEnum( POS_LOWER );
}

KGCCharacterSelect::~KGCCharacterSelect( void )
{
}

void KGCCharacterSelect::OnCreate( void )
{
 
    m_pkCharList->InitState( true );
}

bool KGCCharacterSelect::OnClickOpen()
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return true;
    if(g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn())
        return true;

    D3DXVECTOR2 vpos(0.f,0.f);
    g_pkUIScene->m_pkCharacterSelectBox->InitCharacterSelectBox(
            g_MyD3D->m_pStateMachine->GetState(),
            boost::bind(&KGCCharacterSelect::OnSelectChar,this,_1),
            boost::bind(&KGCCharacterSelect::OnSendEvent,this),
            vpos,
            KGCCharacterSelectBox::STATE_POSITION_DEFAULT
        );

    g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect( m_vOpenPosition );
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);

    g_pkUIMgr->DisconnectGlobalEventProcedure( GetUID(), KD3DWnd::D3DWE_BUTTON_CLICK );
    g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCCharacterSelect::OnClickOutRange );

    return true;
}

void KGCCharacterSelect::OnSelectChar( int iCharType_ )
{
    m_pkCharList->SetChar( iCharType_ );
}

void KGCCharacterSelect::OnSendEvent()
{
    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_CHARACTER_SELECT, m_pkCharList->GetNowChar() ));
}

void KGCCharacterSelect::SetPositionEnum( int iPosEnum )
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return;

    D3DXVECTOR2 vPos = GetFixedWindowPos();

    m_vOpenPosition.x = 
        static_cast<float>(( vPos.x - GetWidth() / 2 ) + ( g_pkUIScene->m_pkCharacterSelectBox->GetWidth() / 2 ));
    switch( iPosEnum ) {
        case POS_UPPER:
            m_vOpenPosition.y = vPos.y;
            m_iMode = KGCCharacterSelectBox::STATE_POSITION_DEFAULT;
            break;
        case POS_LOWER:
        default:
            m_vOpenPosition.y = 
                static_cast<float>(vPos.y + GetHeight() );
            m_iMode = KGCCharacterSelectBox::STATE_POSITION_DOWN;
            break;
    }
}

bool KGCCharacterSelect::OnClickOutRange()
{
    D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    return true;
}
void KGCCharacterSelect::OnDestroy( void )
{
    g_pkUIScene->m_pkCharacterSelectBox->OnClose();
}