#include "stdafx.h"
#include "KGCGradeMark.h"

IMPLEMENT_CLASSNAME( KGCGradeMark );
IMPLEMENT_WND_FACTORY( KGCGradeMark );
IMPLEMENT_WND_FACTORY_NAME( KGCGradeMark, "gc_grade_mark" );

KGCGradeMark::KGCGradeMark( void )
{
    for( int i = 0; i < GC_GRADE_NUM * 3; ++i )
        m_apkGradeMark[i] = NULL;

    LINK_CONTROL( "vagabond_bronze",    m_apkGradeMark[ GC_GRADE_VAGABOND * 3 + 0 ] );
    LINK_CONTROL( "vagabond_silver",    m_apkGradeMark[ GC_GRADE_VAGABOND * 3 + 1 ] );
    LINK_CONTROL( "vagabond_gold",      m_apkGradeMark[ GC_GRADE_VAGABOND * 3 + 2 ] );
    LINK_CONTROL( "mercenary_bronze",   m_apkGradeMark[ GC_GRADE_MERCENARY * 3 + 0 ] );
    LINK_CONTROL( "mercenary_silver",   m_apkGradeMark[ GC_GRADE_MERCENARY * 3 + 1 ] );
    LINK_CONTROL( "mercenary_gold",     m_apkGradeMark[ GC_GRADE_MERCENARY * 3 + 2 ] );
    LINK_CONTROL( "royal_bronze",       m_apkGradeMark[ GC_GRADE_ROYAL * 3 + 0 ] );
    LINK_CONTROL( "royal_silver",       m_apkGradeMark[ GC_GRADE_ROYAL * 3 + 1 ] );
    LINK_CONTROL( "royal_gold",         m_apkGradeMark[ GC_GRADE_ROYAL * 3 + 2 ] );
    LINK_CONTROL( "holy_bronze",        m_apkGradeMark[ GC_GRADE_HOLY * 3 + 0 ] );
    LINK_CONTROL( "holy_silver",        m_apkGradeMark[ GC_GRADE_HOLY * 3 + 1 ] );
    LINK_CONTROL( "holy_gold",          m_apkGradeMark[ GC_GRADE_HOLY * 3 + 2 ] );
}

KGCGradeMark::~KGCGradeMark(void)
{
}

void KGCGradeMark::OnCreate()
{
    for( int i = 0; i < GC_GRADE_NUM * 3; ++i )
        m_apkGradeMark[i]->InitState( false );
}

void KGCGradeMark::FrameMoveInEnabledState( void )
{
}

void KGCGradeMark::SetGrade( int iGrade )
{
    for( int i = 0; i < GC_GRADE_NUM * 3; ++i )
        m_apkGradeMark[i]->ToggleRender( false );

	if( iGrade == -1 )
		return;

    if ( iGrade <= 1 )
        iGrade = 1;
    if( iGrade > GC_GRADE_NUM * 3 )
        iGrade = GC_GRADE_NUM * 3;

    --iGrade;

    m_apkGradeMark[iGrade]->ToggleRender( true );
}

void KGCGradeMark::SetGrade( void )
{
    for( int i = 0; i < GC_GRADE_NUM * 3; ++i )
        m_apkGradeMark[i]->ToggleRender( false );
}
