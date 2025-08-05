#include "StdAfx.h"
#include "KGCSubjectList.h"


IMPLEMENT_CLASSNAME( KGCSubjectList );
IMPLEMENT_WND_FACTORY( KGCSubjectList );
IMPLEMENT_WND_FACTORY_NAME( KGCSubjectList, "gc_subject_list" );

KGCSubjectList::KGCSubjectList()
{
    m_pkBack = NULL;
    m_pkStatic = NULL;
    m_pkTitle = NULL;

    LINK_CONTROL( "back", m_pkBack );
    LINK_CONTROL( "static_desc", m_pkStatic );
    LINK_CONTROL( "title", m_pkTitle );
}

KGCSubjectList::~KGCSubjectList(){}

void KGCSubjectList::OnCreateComplete( void )
{
    m_pkBack->InitState( false );
    m_pkStatic->InitState( false );
    m_pkTitle->InitState( false );
}

void KGCSubjectList::ActionPerformed( const KActionEvent& event ){}

void KGCSubjectList::FrameMoveInEnabledState( void )
{
    const std::map<DWORD, KGCSubject*>& pMapInstance = SiKGCSubjectManager()->GetAllSubjectInstance();
    std::map<DWORD, KGCSubject*>::const_iterator mapIter = pMapInstance.begin();
    if( mapIter == pMapInstance.end() )
    {
        m_pkBack->ToggleRender( false );
        m_pkStatic->ToggleRender( false );
        m_pkTitle->ToggleRender( false );
        return;
    }

    std::wstringstream strAllDesc;
    KGCStringLoader* pStringLoader = SiKGCSubjectManager()->GetStringLoader();
    strAllDesc.clear();
    for(int iLoop=1; mapIter != pMapInstance.end(); ++mapIter)
    {
        KGCSubject* pSubject = mapIter->second;
        if( NULL == pSubject )
            continue;
        
        std::wstring strDesc = pStringLoader->GetString( pSubject->GetSubjectSimpleStringID() );
        
        if( (-1 != pSubject->GetProgressNow()) && (-1 != pSubject->GetProgressMax()) )
            strAllDesc << L"("<< pSubject->GetProgressNow() << L"/" << pSubject->GetProgressMax() << L") ";

        strAllDesc << strDesc << L"\n";

        if( (iLoop++) < static_cast<int>(pMapInstance.size()) )
            strAllDesc << L"\n";
    }
    
    //m_pkStatic->SetAlign( DT_RIGHT );
    m_pkStatic->SetWidthDirect( 215 );
    m_pkStatic->SetWordWrap( true );
    m_pkStatic->SetTextAutoMultiline( strAllDesc.str().c_str() );
    POINT ptSize = m_pkStatic->GetStringWidthAndHeightInPixel( false );
    m_pkBack->OnSetHeight( (ptSize.y + 1) );

    m_pkBack->ToggleRender( true );
    m_pkStatic->ToggleRender( true );
    m_pkTitle->ToggleRender( true );
}