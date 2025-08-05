#include "stdafx.h"
#include ".\gcdevicebuildmesh.h"

GCTmpMeshP3M::GCTmpMeshP3M( std::string strFileName, char cIgBoneIdx/* = -1*/ )
{
	m_eDeviceType = GCDT_TEMP_MESH;
	m_strFileName = strFileName;
	m_cIgBoneIdx = cIgBoneIdx;
	if( cIgBoneIdx != -1 )
	{
		char tmpStr[100];
		sprintf_s( tmpStr, 100, "%stmp__%d", strFileName.c_str(), (int)m_cIgBoneIdx );
		m_strDeviceID = tmpStr;
	}
	else
	{
		m_strDeviceID = strFileName+"tmp";
	}
	ix= NULL;
	v1= NULL;
	m_dwNumVertex = 0;
	m_dwNumFace = 0;
	m_cIgBoneIdx = cIgBoneIdx;
}

GCTmpMeshP3M::~GCTmpMeshP3M( void )
{
	UnLoad();
}

bool GCTmpMeshP3M::Load()
{
	GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(),m_strFileName);

	if(!memfile.IsLoaded())
		return false;

	char        strVersion[27];
	memfile.ReadFile( strVersion,sizeof(char)*27 );

	// 뼈 정보를 연다.
	BYTE		NumPositionBone;
	memfile.ReadFile( &(NumPositionBone), sizeof(BYTE));
	m_dwNumPositionBone = NumPositionBone;

	BYTE		NumAngleBone;
	memfile.ReadFile( &(NumAngleBone), sizeof(BYTE));
	m_dwNumAngleBone = NumAngleBone;

	KAngleBoneFromFile *pTempABone = new KAngleBoneFromFile[m_dwNumAngleBone];

	KPositionBone* pPositionBone = new KPositionBone[m_dwNumPositionBone];
	KAngleBoneOnMemory* pAngleBone    = new KAngleBoneOnMemory[m_dwNumAngleBone];

	memfile.ReadFile( pPositionBone, sizeof(KPositionBone) * m_dwNumPositionBone );
	memfile.ReadFile( pTempABone,	     sizeof(KAngleBoneFromFile)	* m_dwNumAngleBone );

	for( DWORD l = 0; l < m_dwNumAngleBone; l++ )
	{
		for( int i = 0 ; i < (int)pAngleBone[l].acChildIndex.size() ; ++i )
		{
			pAngleBone[l].acChildIndex[i] = pTempABone[l].acChildIndex[i];
		}
	}
	SetAngleBon(pAngleBone,m_dwNumAngleBone);
	SetPositionBone(pPositionBone,m_dwNumPositionBone);

	SAFE_DELETE_ARRAY(pTempABone);

	SAFE_DELETE_ARRAY(ix);
	SAFE_DELETE_ARRAY(v1);

	// 메쉬 정보를 연다.
	WORD wTemp;
	memfile.ReadFile( &wTemp, sizeof(WORD));
	m_dwNumVertex = wTemp;
	memfile.ReadFile( &wTemp, sizeof(WORD));
	m_dwNumFace = wTemp;

	//Texture파일 이름은 사용하지 않음
	char        strTextureFileName[260];
	memfile.ReadFile( strTextureFileName, sizeof(char) * 260 );

	// [8/21/2008] breadceo. 장갑 안읽기 시작
	std::vector< ONE_TRIANGLE > vecIndex;
	vecIndex.reserve( m_dwNumFace );
	for( DWORD i = 0; i < m_dwNumFace; ++i )
	{
		ONE_TRIANGLE triangle;
		memfile.ReadFile( &triangle, sizeof(ONE_TRIANGLE) );
		vecIndex.push_back( triangle );
	}

	std::set< DWORD > setIgIdx;
	std::vector< SKINVERTEX > vecVertex;

	// 뼈가 있는 원본을 연다.
	if (memfile.GetDataPointer() == NULL)
		return false;

	SKINVERTEX_SOURCE* v2 = (SKINVERTEX_SOURCE*)memfile.GetDataPointer();
	for( DWORD i = 0; i < m_dwNumVertex; ++i )
	{
		BYTE idx = static_cast<BYTE>(v2[i].index - m_dwNumPositionBone);
		if( m_cIgBoneIdx != -1 && (idx == (BYTE)m_cIgBoneIdx || idx == (BYTE)m_cIgBoneIdx + 1) )
		{
			setIgIdx.insert( i );
		}

		SKINVERTEX v;
		v.Pos			= v2[i].Pos;
		v.w[0]			= 1.0f;
		v.indexByte[0] 	= v.indexByte[1] = v.indexByte[2] = v.indexByte[3] = idx;
		v.Nor   		= v2[i].Nor;
		v.tu    		= v2[i].tu;
		v.tv    		= v2[i].tv;

		//v.dwDiffuse		= 0xffffffff;

		vecVertex.push_back( v );
	}

	//m_dwNumVertex -= (DWORD)setIgIdx.size();
	vecIndex.erase( std::remove_if( vecIndex.begin(), vecIndex.end(), RemoveFunctor( setIgIdx ) ), vecIndex.end() );
	m_dwNumFace = (DWORD)vecIndex.size();

	ix = new WORD[m_dwNumFace * 3];
	v1 = new SKINVERTEX[m_dwNumVertex];
	memcpy( ix, &(*vecIndex.begin()), sizeof(ONE_TRIANGLE) * m_dwNumFace );
	memcpy( v1, &(*vecVertex.begin()), m_dwNumVertex * sizeof(SKINVERTEX) );

	// [8/21/2008] breadceo. 장갑 안읽기 끝

	m_eDeviceState = GCDS_LOADED;
	return true;
}

void GCTmpMeshP3M::UnLoad()
{
	SAFE_DELETE_ARRAY(ix);
	SAFE_DELETE_ARRAY(v1);
	ClearPositionBone();
	ClearAngleBon();
	m_dwNumVertex = 0;
	m_dwNumFace = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
}

DWORD GCTmpMeshP3M::GetDeviceSize()
{
	return sizeof(SKINVERTEX) * m_dwNumVertex + sizeof(WORD) * m_dwNumFace * 3;
}

GCBuildMesh::GCBuildMesh(LPDIRECT3DDEVICE9 d3ddevice )
	:GCBaseMesh(d3ddevice)
{
	m_bBuilded = false;
}

GCBuildMesh::~GCBuildMesh(void)
{
	while(!m_vecMeshForBuild.empty())
	{
		SAFE_DELETE( m_vecMeshForBuild.back() );
		m_vecMeshForBuild.pop_back();
	}
}

void GCBuildMesh::AddMesh( std::string strmesh,D3DXVECTOR4 *uv, char cIgBoneIdx/* = -1*/ )
{
	GCTmpMeshP3M* mesh = g_pGCDeviceManager->CreateTmpMeshP3M(strmesh, cIgBoneIdx);
	if(mesh->IsLoaded())
	{
		m_vecMeshForBuild.push_back(new GCMeshForBuild(mesh,m_dwNumVertex,uv));
		m_dwNumVertex+= mesh->GetNumVertex();
		m_dwNumFace+= mesh->GetNumFace();
	}
}

void GCBuildMesh::Build()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	if( FAILED( m_pd3dDevice->CreateIndexBuffer( sizeof(WORD) * m_dwNumFace * 3, D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pIndexBuffer, NULL ) ) )
	{
		return;
	}

	if( FAILED( m_pd3dDevice->CreateVertexBuffer( m_dwNumVertex * sizeof(SKINVERTEX), D3DUSAGE_WRITEONLY, SKINVERTEX::FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL ) ) )
	{
		SAFE_RELEASE( m_pIndexBuffer );
		return;
	}

	GCTmpMeshP3M* bonemesh=NULL;
	for(DWORD i=0;i<m_vecMeshForBuild.size();i++)
	{
		if ( m_dwNumAngleBone < m_vecMeshForBuild[i]->GetMesh()->GetNumAngleBone() )
		{
			m_dwNumAngleBone = m_vecMeshForBuild[i]->GetMesh()->GetNumAngleBone();
			bonemesh=m_vecMeshForBuild[i]->GetMesh();
		}
	}

	if(bonemesh)
	{
		ClearPositionBone();
		ClearAngleBon();

		m_dwNumAngleBone    = bonemesh->GetNumAngleBone();
		m_dwNumPositionBone = bonemesh->GetNumPositionBone();
		SetPositionBone(new KPositionBone[m_dwNumPositionBone],m_dwNumPositionBone );
		SetAngleBon(new KAngleBoneOnMemory[m_dwNumAngleBone],m_dwNumAngleBone);

		CopyPositionBone(bonemesh->GetPositionBone());
		CopyAngleBon(bonemesh->GetAngleBone());
	}

	WORD        *index;

	// 인덱스 버퍼를 연다.
	m_pIndexBuffer->Lock(0, 0, (void**)&index, 0);
	{
		for(DWORD i=0;i<m_vecMeshForBuild.size();i++)
		{
			index+=m_vecMeshForBuild[i]->CopyIndex(index);
		}
	}
	m_pIndexBuffer->Unlock();

	SKINVERTEX  *vertex;
	m_pVertexBuffer->Lock( 0, 0, (void**)&vertex, 0 );
	{
		for(DWORD i=0;i<m_vecMeshForBuild.size();i++)
		{
			vertex+=m_vecMeshForBuild[i]->CopyVertex(vertex);
			SAFE_DELETE(m_vecMeshForBuild[i]);
		}
	}
	m_pVertexBuffer->Unlock();
	m_vecMeshForBuild.clear();

	m_bBuilded=true;
}

void GCBuildMesh::Render()
{
	if(!m_bBuilded)
		Build();

	GCBaseMesh::Render();
}

GCMeshForBuild::GCMeshForBuild( GCTmpMeshP3M * mesh,int offset,D3DXVECTOR4 *uv )
	:m_Mesh(mesh),m_offset(offset),m_uv(*uv)
{
}

GCMeshForBuild::~GCMeshForBuild()
{
	SAFE_RELEASE(m_Mesh);
}

int	GCMeshForBuild::CopyIndex(WORD        *index)
{
	WORD * src = m_Mesh->GetIndecies();
	int size = m_Mesh->GetNumFace()*3;

	for( int i = 0; i < size; i++ )
		index[i]=src[i]+m_offset;

	return size;
}

int	GCMeshForBuild::CopyVertex(SKINVERTEX  *vertex)
{
	SKINVERTEX * src = m_Mesh->GetVertecies();
	int size = m_Mesh->GetNumVertex();

	for( int i = 0; i < size; i++ )
	{
		vertex[i]=src[i];
		vertex[i].tu = src[i].tu*m_uv.x+m_uv.z;
		vertex[i].tv = src[i].tv*m_uv.y+m_uv.w;
	}
	return size;
}
