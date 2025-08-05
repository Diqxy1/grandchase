#include "stdafx.h"
#include "GCRitasGoalBasket.h"

IMPLEMENT_CLASSNAME( KGCRitasGoalBasket );
IMPLEMENT_WND_FACTORY( KGCRitasGoalBasket );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasGoalBasket, "gc_ritasgoalbasket" );

KGCRitasGoalBasket::KGCRitasGoalBasket( void )
{
	char szName[MAX_PATH];
	for (int i=0 ; i<OBJ_KIND_NUM ; i++)
	{
		m_pkStaticObj[i] = NULL;
		sprintf( szName, "static_obj%d", i );
		LINK_CONTROL( szName, m_pkStaticObj[i] );
	}

	for (int i=0 ; i<OBJ_KIND_NUM ; i++)
	{
		m_pkObjIcon[i] = NULL;
		sprintf( szName, "obj_icon%d", i );
		LINK_CONTROL( szName, m_pkObjIcon[i] );
	}
}

KGCRitasGoalBasket::~KGCRitasGoalBasket( void )
{
}

void KGCRitasGoalBasket::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasGoalBasket::OnCreate( void )
{
	for (int i=0 ; i<OBJ_KIND_NUM ; i++)
		m_pkObjIcon[i]->InitState(true);

	for (int i=0 ; i<OBJ_KIND_NUM ; i++)
	{
		m_pkStaticObj[i]->InitState(true);
		m_pkStaticObj[i]->SetAlign(DT_LEFT);
		m_pkStaticObj[i]->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
	}
}

void KGCRitasGoalBasket::UpDateComboInfo()
{
	std::vector<RitasComboInfo>	mapComboInfo = SiGCRitasChrismasManager()->GetComboInfo();
	
	std::vector<RitasComboInfo>::iterator vit = mapComboInfo.begin();
	int i=0;

	char szName[MAX_PATH];

	while(vit != mapComboInfo.end() && i<OBJ_KIND_NUM)
	{
		sprintf( szName, "x%d", vit->m_iNum );
		m_pkStaticObj[i]->SetText( GCUTIL_STR::GCStrCharToWide(szName) );

		if (vit->m_bClear)
			m_pkObjIcon[i]->SetWndMode(D3DWM_HOVER);
		else
			m_pkObjIcon[i]->SetWndMode(D3DWM_DEFAULT);

		++vit;
		i++;
	}

}
