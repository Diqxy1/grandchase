#include "stdafx.h"
#include "KGCEmoticonListBox.h"
//

IMPLEMENT_CLASSNAME( KGCEmoticonListBox );
IMPLEMENT_WND_FACTORY( KGCEmoticonListBox );
IMPLEMENT_WND_FACTORY_NAME( KGCEmoticonListBox, "gc_emoticon_list" );


KGCEmoticonListBox::KGCEmoticonListBox( void )
{

}

KGCEmoticonListBox::~KGCEmoticonListBox( void )
{
    m_mapEvent.clear();
}

void KGCEmoticonListBox::ActionPerformed( const KActionEvent& event )
{
    (this->*m_mapEvent[event.m_pWnd->GetWindowName()])( event.m_dwCode );
}
