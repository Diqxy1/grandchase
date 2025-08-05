#pragma once

struct SFrame;
struct KPositionBone;
struct KAngleBoneOnMemory;

//Interface였는데.. 기본 구현이 필요한듯하여..
//

class IGCComputePaletteP3M
{
public:
	virtual D3DXMATRIX* ComputePositionBone( D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone );
	virtual D3DXMATRIX* ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic );
};

extern IGCComputePaletteP3M g_DefaultIGCComputePaletteP3M;
