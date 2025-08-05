#include "stdafx.h"
#include ".\gcdevicemeshp3m.h"

static int ms_iTest = 1;
GCDeviceMeshP3M::GCDeviceMeshP3M(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, char cIgBoneIdx/* = -1*/ )
	:GCBaseMesh(d3ddevice)
{
	m_strFileName = strFileName;
	m_cIgBoneIdx = cIgBoneIdx;
	if( cIgBoneIdx != -1 )
	{
		char tmpStr[100];
		sprintf_s( tmpStr, 100, "%s__%d", strFileName.c_str(), (int)m_cIgBoneIdx );
		m_strDeviceID = tmpStr;
	}
	else
	{
		m_strDeviceID = strFileName;
	}
	m_eDeviceType = GCDT_MODEL_P3M;
}

GCDeviceMeshP3M::~GCDeviceMeshP3M(void)
{
	UnLoad();
}

void GCDeviceMeshP3M::Render()
{
	if( false == IsLoaded() )
		return;

	GCBaseMesh::Render();
}

bool GCDeviceMeshP3M::Load()
{
	GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(),m_strFileName);

	if( !memfile.IsLoaded() )
	{
		m_eDeviceState = GCDS_DISABLE;
		return false;
	}

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
	WORD        *ix;

	std::set< DWORD > setIgIdx;
	std::vector< SKINVERTEX > vecVertex;
	SKINVERTEX_SOURCE* v2 = (SKINVERTEX_SOURCE*)memfile.GetDataPointer();
	for( DWORD i = 0; i < m_dwNumVertex; ++i )
	{
		BYTE idx = (BYTE)(v2[i].index - m_dwNumPositionBone);
		if( m_cIgBoneIdx != -1 && (idx == (BYTE)m_cIgBoneIdx || idx == (BYTE)m_cIgBoneIdx + 1) ) // 최후의 하드코딩..
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

		vecVertex.push_back( v );
	}

	//m_dwNumVertex -= (DWORD)setIgIdx.size();
	vecIndex.erase( std::remove_if( vecIndex.begin(), vecIndex.end(), RemoveFunctor( setIgIdx ) ), vecIndex.end() );
	m_dwNumFace = (DWORD)vecIndex.size();

	// 인덱스 버퍼를 연다.
	SAFE_RELEASE( m_pIndexBuffer );
	if( D3D_OK != m_pd3dDevice->CreateIndexBuffer( sizeof(ONE_TRIANGLE) * m_dwNumFace, D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pIndexBuffer, NULL ) )
		goto Failed;
	m_pIndexBuffer->Lock(0, 0, (void**)&ix, 0);
	memcpy( ix, &(*vecIndex.begin()), sizeof(ONE_TRIANGLE) * m_dwNumFace );
	m_pIndexBuffer->Unlock();

	// 뼈가 있는 원본을 연다.
	SKINVERTEX  *v1;
	SAFE_RELEASE(m_pVertexBuffer);
	if( D3D_OK != m_pd3dDevice->CreateVertexBuffer( m_dwNumVertex * sizeof(SKINVERTEX), D3DUSAGE_WRITEONLY, SKINVERTEX::FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL ) )
		goto Failed;
	m_pVertexBuffer->Lock( 0, 0, (void**)&v1, 0 );
	memcpy( v1, &(*vecVertex.begin()), m_dwNumVertex * sizeof(SKINVERTEX) );
	m_pVertexBuffer->Unlock();

	// [8/21/2008] breadceo. 장갑 안읽기 끝
	m_eDeviceState = GCDS_LOADED;
	return true;

Failed:
	m_eDeviceState = GCDS_DISABLE;
	return false;
}

void GCDeviceMeshP3M::UnLoad()
{
	if(m_eDeviceState!= GCDS_LOADED)
		return;

	ClearPositionBone();
	ClearAngleBon();

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	m_dwNumVertex = 0;
	m_dwNumFace = 0;

	m_eDeviceState = GCDS_NOT_LOADED;
}

DWORD GCDeviceMeshP3M::GetDeviceSize()
{
	return sizeof( KPositionBone ) * m_dwNumPositionBone +
		sizeof( KAngleBoneOnMemory ) * m_dwNumAngleBone +
		sizeof(SKINVERTEX) * m_dwNumVertex +
		sizeof(WORD) * m_dwNumFace * 3;
}
