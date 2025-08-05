#pragma once
#include "gcdevicemeshp3m.h"

class GCTmpMeshP3M : public IGCBone,  public GCDevice
{
public:
	GCTmpMeshP3M(std::string strFileName, char cIgBoneIdx = -1 );
	~GCTmpMeshP3M(void);

	DWORD GetNumVertex() { return m_dwNumVertex; }
	DWORD GetNumFace()   { return m_dwNumFace; }

	void SetMeshComputePaletteInterface( IGCComputePaletteP3M* IMeshComputePalette )
	{
		if( IMeshComputePalette == NULL )
			m_pIMeshComputePalette = &g_DefaultIGCComputePaletteP3M;
		else
			m_pIMeshComputePalette = IMeshComputePalette;
	}

	SKINVERTEX * GetVertecies()   { return v1; }
	WORD  * GetIndecies() { return ix; }

	void OnResetDevice() {};
	void OnLostDevice() {};
protected:

	//Interface Device
	virtual bool Load();
	virtual void UnLoad();
	virtual DWORD GetDeviceSize();

	std::string m_strFileName;
	IGCComputePaletteP3M* m_pIMeshComputePalette;

	DWORD						m_dwNumVertex, m_dwNumFace;	//파일에는 WORD형으로 저장되어 있음
	WORD						*ix;
	SKINVERTEX					*v1;
	char						m_cIgBoneIdx;
};

class GCMeshForBuild
{
public:
	GCMeshForBuild(GCTmpMeshP3M * mesh,int offset,D3DXVECTOR4 *uv);
	~GCMeshForBuild();
	GCTmpMeshP3M * GetMesh(){return m_Mesh;}

	int	CopyIndex(WORD* index);
	int	CopyVertex(SKINVERTEX* vertex);

protected:
	GCTmpMeshP3M * m_Mesh;
	int m_offset;
	D3DXVECTOR4 m_uv;
};

class GCBuildMesh :
	public GCBaseMesh
{
public:
	GCBuildMesh(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCBuildMesh(void);

	virtual void Render();

	void AddMesh(std::string mesh,D3DXVECTOR4 *uv, char cIgBoneIdx = -1);
	void Build();
	void Release(){delete this;}
	bool IsBuilded() {return m_bBuilded;}

protected:

	bool m_bBuilded;
	std::vector< GCMeshForBuild* >    m_vecMeshForBuild;
};
