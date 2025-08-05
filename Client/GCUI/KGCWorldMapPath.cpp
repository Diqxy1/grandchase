#include "stdafx.h"
#include "KGCWorldMapPath.h"

IMPLEMENT_CLASSNAME( KGCWorldMapPath );
IMPLEMENT_WND_FACTORY( KGCWorldMapPath );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapPath, "gc_worldmap_path" );

KGCWorldMapPath::KGCWorldMapPath( void )
{
    m_iObjectType = WORLDMAP_PATH;


    m_pkPath = NULL;

    LINK_CONTROL("spot", m_pkPath );
    //LINK_CONTROL("pathimpossible", m_pkPathImPossible );


}

KGCWorldMapPath::~KGCWorldMapPath(void)
{
}

void KGCWorldMapPath::OnCreate()
{
    if( m_pkPath )
        m_pkPath->InitState( true );
}

void KGCWorldMapPath::RenderPath( bool bpath )
{
    if( bpath )
        m_pkPath->SetWndMode( D3DWM_SELECT );
    else
        m_pkPath->SetWndMode( D3DWM_DEFAULT );
}
