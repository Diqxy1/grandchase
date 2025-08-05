#include "stdafx.h"
#include ".\gclinkablestatic.h"

IMPLEMENT_CLASSNAME( KGCLinkableStatic );
IMPLEMENT_WND_FACTORY( KGCLinkableStatic );
IMPLEMENT_WND_FACTORY_NAME( KGCLinkableStatic, "gc_link_static" );

KGCLinkableStatic::KGCLinkableStatic(void)
{
}

KGCLinkableStatic::~KGCLinkableStatic(void)
{
}

void KGCLinkableStatic::AddLinkText( std::wstring strText )
{
    std::wstring strOrigin = m_strText;
    while( true )
    {
        if( m_strText.find( strText ) == m_strText.npos )
        {
            break;
        }
    }
}

void KGCLinkableStatic::CalcStringPosition()
{
	
}