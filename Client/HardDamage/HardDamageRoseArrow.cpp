#include "stdafx.h"
#include ".\HardDamageRoseArrow.h"
#include "Controls.h"
#include "Headup Display.h"

#include "Spark.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "GCSKT.h"

CHardDamageRoseArrow::CHardDamageRoseArrow(void)
{
}

CHardDamageRoseArrow::~CHardDamageRoseArrow(void)
{
}

void CHardDamageRoseArrow::CustomizeBeforeCreate( CDamageInstance* pDamageInstance )
{
	if( pDamageInstance->m_IsMonsterDamage )
		return;

	if(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_Elemental == ITEM_ELEMENTAL_ROSE
		&& g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_StrongLevel >= 8 )
	{
		if( pDamageInstance->m_pOriginalDamage->m_particleSeqName == "loveItem_arrow01" )
			pDamageInstance->m_pOriginalDamage->m_particleSeqName = "Rozen_Arrow";
	}

}