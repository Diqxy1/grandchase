#include "stdafx.h"
#include "GCItemImgUIWnd.h"

IMPLEMENT_CLASSNAME( KGCItemImgUIWnd );
IMPLEMENT_WND_FACTORY( KGCItemImgUIWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCItemImgUIWnd, "gc_itemimguiwnd" );

KGCItemImgUIWnd::KGCItemImgUIWnd( void )
{

     for( int i = 0 ; i < MAX_IMG_UI_WND; ++i )
     {
         char szTemp[MAX_PATH] = "";
         m_pkDescWnd[i] = NULL;
         sprintf_s( szTemp, MAX_PATH ,"item_img_wnd%d", i );
         LINK_CONTROL( szTemp, m_pkDescWnd[i]);   
     }
     for( int i = 0 ; i < MAX_IMG_UI_NUMBER; ++i )
     {
         char szTemp[MAX_PATH] = "";
         m_pkDescNumber[i] = NULL;
         sprintf_s( szTemp, MAX_PATH ,"item_img_number%d", i );
         LINK_CONTROL( szTemp, m_pkDescNumber[i]);   
     }
     
}
KGCItemImgUIWnd::~KGCItemImgUIWnd( void )
{

}
void KGCItemImgUIWnd::OnCreate( void )
 {
     for( int i = 0 ; i < MAX_IMG_UI_WND; ++i )
     {
         m_pkDescWnd[i]->InitState( false );
     }
     for( int i = 0 ; i < MAX_IMG_UI_NUMBER; ++i )
     {
         m_pkDescNumber[i]->InitState( false );
     }
 }

void KGCItemImgUIWnd::OnInit( void )
{
     for( int i = 0 ; i < MAX_IMG_UI_WND; ++i )
     {
         m_pkDescWnd[i]->ToggleRender( false );
     }
     for( int i = 0 ; i < MAX_IMG_UI_NUMBER; ++i )
     {
         m_pkDescNumber[i]->ToggleRender( false );
     }
     
}