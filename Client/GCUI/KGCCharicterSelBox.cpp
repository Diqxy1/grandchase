#include "stdafx.h"
#include "KGCCharicterSelBox.h"
//


IMPLEMENT_CLASSNAME( KGCCharicterSelBox );
IMPLEMENT_WND_FACTORY( KGCCharicterSelBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCharicterSelBox, "gc_char_sel_box" );

KGCCharicterSelBox::KGCCharicterSelBox( void )
{   
    //////////////////////////////////////////////////////////////////////////
    // 컨트롤 설정
    char strTemp[40];
    for( int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; i++ )
    {
        m_pkChar[i] = NULL;
        sprintf( strTemp, "tutorial_char%d_%d", i/NUM_JOB_LEVEL ,i%NUM_JOB_LEVEL);
        LINK_CONTROL( strTemp,     m_pkChar[i] );
    }

    m_iCharIndex = 0;
}

KGCCharicterSelBox::~KGCCharicterSelBox( void )
{
    // empty
}

void KGCCharicterSelBox::OnCreate( void )
{  
    for(int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; i++)
    {
        if( m_pkChar[i] == NULL )
            continue;

        m_pkChar[i]->InitState( false );
    }

    SelectCharicter(m_iCharIndex);
}

void KGCCharicterSelBox::SetOnlySelectedDraw(bool bDraw)
{

}

void KGCCharicterSelBox::SelectCharicter(int index)
{
    // 드루이드 2차 전직이 없으므로 현재는 -1로 2차전직을 못고르게 한다.
    if( index < 0 || index >= (GC_CHAR_NUM * NUM_JOB_LEVEL-1) ) return;

    for(int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; i++)
    {
        if( index == i )
        {
            m_pkChar[i]->ToggleRender(true);
            m_pkChar[i]->SetWndMode(D3DWM_HOVER);
            m_pkChar[i]->SetSelfInputCheck(false);
        }
        else
        {
            m_pkChar[i]->SetWndMode(D3DWM_DEFAULT);
            m_pkChar[i]->ToggleRender(false);
            m_pkChar[i]->SetSelfInputCheck(true);
        }

        m_pkChar[i]->ParentRendeOff();
    }

    m_iCharIndex = index;
}

void KGCCharicterSelBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCCharicterSelBox::FrameMoveInEnabledState()
{

}