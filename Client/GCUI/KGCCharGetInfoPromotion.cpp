#include "stdafx.h"
#include "KGCCharGetInfoPromotion.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCharGetInfoPromotion );
IMPLEMENT_WND_FACTORY( KGCCharGetInfoPromotion );
IMPLEMENT_WND_FACTORY_NAME( KGCCharGetInfoPromotion, "gc_char_get_info_promotion" );

KGCCharGetInfoPromotion::KGCCharGetInfoPromotion( void ) 
: m_pkment(NULL)
{
    int iCount = 0;
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        for( int j = 0 ; j < NUM_JOB_LEVEL ; ++j )
        {
            m_apkPromotionName[iCount] = NULL;

            char szTemp[MAX_PATH];
            sprintf_s(szTemp, MAX_PATH - 1, "name%d_%d", i, j);
            LINK_CONTROL( szTemp, m_apkPromotionName[iCount] );
            ++iCount;
        }
    }

    LINK_CONTROL("promotion_change_ment",m_pkment);
}

KGCCharGetInfoPromotion::~KGCCharGetInfoPromotion( void )
{

}

void KGCCharGetInfoPromotion::OnCreate( void )
{
    for( int i = 0 ; i < GC_CHAR_NUM * NUM_JOB_LEVEL ; ++i )
    {
        if( m_apkPromotionName[i] ) {
            m_apkPromotionName[i]->ToggleRender( false );
        }
    }
    if(m_pkment)
        m_pkment->ToggleRender(false);

}

void KGCCharGetInfoPromotion::SetShow( bool bShow, int iCharType, int iPromotion )
{
    if( iCharType < 0 || iCharType > GC_CHAR_NUM || iPromotion < 0 || iPromotion > NUM_JOB_LEVEL-1 )
        return;

    this->ToggleRender( bShow );

    int iCount = 0;
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        for( int j = 0 ; j < NUM_JOB_LEVEL ; ++j )
        {
            if( i == iCharType && j == iPromotion )
            {
                if( m_apkPromotionName[iCount] )
                {
                    m_apkPromotionName[iCount]->ToggleRender( bShow );
                    if(bShow && m_pkment)
                    {
                        int width_name = m_apkPromotionName[iCount]->GetWidth();
                        int width_ment = m_pkment->GetWidth();

                        int width_string = width_name + width_ment + 10;
#if defined(NATION_IDN) || defined(NATION_THAILAND)
                        m_apkPromotionName[iCount]->SetWindowPosX(151.0f - static_cast<float>(width_string/2) );
                        m_pkment->SetWindowPosX(151.0f - static_cast<float>(width_string/2 - width_name - 10) );
#else
                        m_apkPromotionName[iCount]->SetWindowPosX(161.0f - static_cast<float>(width_string/2) );
                        m_pkment->SetWindowPosX(161.0f - static_cast<float>(width_string/2 - width_name - 10) );
#endif
                        m_pkment->ToggleRender(true);

                    }
                    break;
                }
            }
            ++iCount;
        }
    }
}