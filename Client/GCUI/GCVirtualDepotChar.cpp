#include "stdafx.h"
#include "GCVirtualDepotChar.h"

IMPLEMENT_CLASSNAME( KGCVirtualDepotChar );
IMPLEMENT_WND_FACTORY( KGCVirtualDepotChar );
IMPLEMENT_WND_FACTORY_NAME( KGCVirtualDepotChar, "gc_depot_char_info" );

KGCVirtualDepotChar::KGCVirtualDepotChar( void )
:m_iCharType(-1)
{
    for(int i =0 ; i< GC_CHAR_NUM ; i++ ){
        char strChar[128] = {0,};
        sprintf(strChar, "char%d",i);
        m_mapCharInfoVD[i] = NULL;
        LINK_CONTROL( strChar,          m_mapCharInfoVD[i] );
    }
}

KGCVirtualDepotChar::~KGCVirtualDepotChar( void )
{
}

void KGCVirtualDepotChar::ActionPerformed( const KActionEvent& event )
{

}

void KGCVirtualDepotChar::OnCreate( void )
{

    for(std::map<int, KD3DWnd*>::iterator mIt = m_mapCharInfoVD.begin();mIt != m_mapCharInfoVD.end();mIt++)
    {
        if(mIt->first == 0 ){
            mIt->second->InitState(true,false,NULL);
        }
        else{
            mIt->second->InitState(false,false,NULL);
        }
    }
}

void KGCVirtualDepotChar::SetChar(int iChar)
{
    for(std::map<int, KD3DWnd*>::iterator mIt = m_mapCharInfoVD.begin();mIt != m_mapCharInfoVD.end();mIt++)
    {
        if(mIt->first == iChar){
            mIt->second->InitState(true,false,NULL);
        }
        else{
            mIt->second->InitState(false,false,NULL);
        }
    }
}