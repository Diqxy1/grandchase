#include "stdafx.h"
#include ".\igccomputepalettep3m.h"
#include "GCDeviceMeshP3M.h"
#include "GCDeviceMotionFRM.h"

IGCComputePaletteP3M g_DefaultIGCComputePaletteP3M;

D3DXMATRIX* IGCComputePaletteP3M::ComputePositionBone( D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone )
{
	if(vecPositionBone.empty() || (int)vecPositionBone.size() <= Bone_Num)
		return NULL;
	D3DXMATRIX matTemp;
	D3DXMatrixTranslation( &matTemp, vecPositionBone[Bone_Num]->vPos.x, vecPositionBone[Bone_Num]->vPos.y, vecPositionBone[Bone_Num]->vPos.z );
	D3DXMatrixMultiply(matOut, &matTemp, &matBasic);
	return matOut;
}

D3DXMATRIX* IGCComputePaletteP3M::ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic )
{
	if( pFrame == NULL || pFrame->A_Bone == NULL )	//이게 널로 오니까 죽는다...
		return matOut;

	if ( pFrame->dwNumAngleBone <= (DWORD)Bone_Num  )
	{
		*matOut = matBasic; //D3DXMatrixIdentity( matOut );
	}
	else
	{
		D3DXMatrixMultiply(matOut, &pFrame->A_Bone[Bone_Num], &matBasic);
	}

	return matOut;
}
