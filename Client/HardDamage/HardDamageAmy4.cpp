#include "stdafx.h"
#include ".\HardDamageAmy4.h"
#include "DamageManager.h"
#include "KGC3DObject.h"


void CHardDamageAmy4CannonBall::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove(pDamageInstance);

	if( pDamageInstance->m_pAnim != NULL )
	{			
		int iLife = pDamageInstance->m_Life;

		float fX = pDamageInstance->m_X + pDamageInstance->m_fAnimScaleX;
		float fY = pDamageInstance->m_Y + pDamageInstance->m_fAnimScaleY;
		float fTransX = ( pDamageInstance->m_IsRight ? 0.01942f : -0.01942f );
		float fTransY = 0.00426f;
		float fScaleX = ( pDamageInstance->m_IsRight ? 0.000109f : -0.000109f )*iLife;
		float fScaleY = 0.000175f*iLife;

		pDamageInstance->m_pAnim->GetMatrix()->Move( fX+fScaleX+fTransX-1.0f, fY+fScaleY+fTransY-0.5f, 0.5f );

		if ( 0 != o3 )
			pDamageInstance->m_pAnim->SetShow( pDamageInstance->m_IsShow );
		else
			pDamageInstance->m_pAnim->SetShow( false );
		

		if(pDamageInstance->m_IsRight)
			pDamageInstance->m_pAnim->GetMatrix()->Scale(  pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );
		else
			pDamageInstance->m_pAnim->GetMatrix()->Scale( -pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );				
	}
}

CHardDamageAmy4RandomBoy::CHardDamageAmy4RandomBoy( void )
{
    m_vecNameList.push_back("LAS");
    m_vecNameList.push_back("RYAN");
    m_vecNameList.push_back("RONAN");
    m_vecNameList.push_back("JIN");
}

void CHardDamageAmy4RandomBoy::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
    int r = rand() % m_vecNameList.size();
    pDamageInstance->m_p3DObject->GetGCObject()->ToggleBaseMeshAll( false );
    pDamageInstance->m_p3DObject->GetGCObject()->ToggleExtraMesh( m_vecNameList[r], true );    

}

void CHardDamageAmy4RandomBoy::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );
    int r = rand() % m_vecNameList.size();
    pDamageInstance->m_p3DObject->GetGCObject()->ToggleBaseMeshAll( false );
    pDamageInstance->m_p3DObject->GetGCObject()->ToggleExtraMesh( m_vecNameList[r], true );    
}