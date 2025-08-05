#include "stdafx.h"
#include "GCDeviceTexture.h"
#include "GCDeviceMeshP3M.h"
#include "GCDeviceMotionFRM.h"
#include "GCDeviceFontTH.h"
#include "GCDeviceShader.h"
#include "GCDeviceBuildTexture.h"
#include "GCDeviceBuildMesh.h"
#include "GCDeviceVertexBuffer.h"
#include "GCDeviceIndexBuffer.h"
#include "GCMatrix.h"
#include "GCDynamicVBManager.h"

std::stack<GCDeviceManager::KGCStateBlock::KGCState, std::deque<GCDeviceManager::KGCStateBlock::KGCState, boost::pool_allocator<GCDeviceManager::KGCStateBlock::KGCState> > > GCDeviceManager::KGCStateBlock::m_stackState;

GCDeviceManager*	g_pGCDeviceManager = NULL;
#define     FIND_DEVICE_RETURN(type, id)   GCUTIL_STR::ToLower(id );\
	GCDevice* pDevice = FindDevice(id);\
	if( pDevice != NULL )\
{\
	pDevice->AddRefCount();\
	return (type*)pDevice;\
}

GCDeviceManager::GCDeviceManager()
{
	//g_pGCDeviceManager = this;
	//m_pMassFileManager = new GCMassFileManager();
	//m_pSprite = new
	assert(0);

	/*
	g_pGCDeviceManager = this;
	m_pMassFileManager = new GCMassFileManager();

	//m_pSprite = new GCDeviceSprite(d3ddevice);
	m_mapDevice["SPRITE"] = m_pSprite;
	*/
}

GCDeviceManager::GCDeviceManager(LPDIRECT3DDEVICE9 d3ddevice)
	:m_pMassFileManager(new KomfileManager())
{
	g_pGCDeviceManager = this;
	m_d3ddevice = d3ddevice;
	m_pEdgeTexture = NULL;

	GCMatrix::SetDevice( d3ddevice );

	m_pCurTex = NULL;
	m_pWorldVB = NULL;
	m_pScreenVB = NULL;
	m_pIB = NULL;
	m_windowScaleX = 1.0f;
	m_windowScaleY = 1.0f;

	D3DXMatrixIdentity(&m_mat_id);

	m_d3ddevice->CreateIndexBuffer( sizeof(WORD) * max(WORLD_VB_SIZE,SCREEN_VB_SIZE) * 6, D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pIB, NULL );
	m_d3ddevice->CreateVertexBuffer( WORLD_VB_SIZE * 4 * sizeof(SLVERTEX), D3DUSAGE_WRITEONLY, SLVERTEX::FVF, D3DPOOL_MANAGED, &m_pWorldVB, NULL );
	m_d3ddevice->CreateVertexBuffer( SCREEN_VB_SIZE * 4 * sizeof(KD3DUIVertex), D3DUSAGE_WRITEONLY, KD3DUIVertex::FVF, D3DPOOL_MANAGED, &m_pScreenVB, NULL );

	WORD        *index;
	m_pIB->Lock(0, 0, (void**)&index, 0);
	{
		for(int i=0;i<max(WORLD_VB_SIZE,SCREEN_VB_SIZE);i++)
		{
			index[0]=4*i+0;
			index[1]=4*i+1;
			index[2]=4*i+2;
			index[3]=4*i+1;
			index[4]=4*i+3;
			index[5]=4*i+2;
			index+=6;
		}
	}
	m_pIB->Unlock();

	m_iWorldVBSize = 0;
	m_iScreenVBSize = 0;

	int t=sizeof(KGCStateBlock);

	m_pDVBManager = new KGCDynamicVBManager();
}

GCDeviceManager::~GCDeviceManager(void)
{
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pWorldVB);
	SAFE_RELEASE(m_pScreenVB);

	//	SAFE_DELETE( m_pMassFileManager );
	g_pGCDeviceManager = NULL;

	std::set<GCDevice*> setDestroy;

	stdext::hash_map<std::string, GCDevice* >::iterator i = m_mapDevice.begin();
	while( i != m_mapDevice.end() )
	{
		setDestroy.insert( i->second );
		++i;
	}
	m_mapDevice.clear();

	std::set<GCDevice*>::iterator sit = setDestroy.begin();
	while( sit != setDestroy.end() )
	{
		delete *sit;
		++sit;
	}

	SAFE_DELETE( m_pDVBManager );

	//SAFE_RELEASE( m_pXFile );
}

//D3DPOOL_MANAGED로 Create될 Resource들은 여기서 Create하도록 한다.
//여기에서 Create된 Resource들은 OnDestroyDevice에서 Release된다.
void GCDeviceManager::OnCreateDevice()
{
}

//OnCreateDevice에서 Create된 Resource들은 여기서 Release되어 진다.
//OnDestroyDevice가 불리우고 이후에 OnCreateDevice로 다시 Create된다.
void GCDeviceManager::OnDestroyDevice()
{
}

//D3DPOOL_DEFAULT로 Create될 Resource들은 여기에서 Create하도록 한다.
//여기에서 Create된 Resource들은 OnLostDevice에서 Release된다.
void GCDeviceManager::OnResetDevice()
{
	stdext::hash_map<std::string, GCDevice* >::iterator i = m_mapDevice.begin();
	while( i != m_mapDevice.end() )
	{
		i->second->OnResetDevice();
		++i;
	}

	m_pDVBManager->OnResetDevice();
}

//OnResetDevice에서 Create된 Resource들은 여기서 Release되어 진다.
//OnLostDevice가 불리우고 이후에 OnResetDevice로 다시 Create된다.
void GCDeviceManager::OnLostDevice()
{
	stdext::hash_map<std::string, GCDevice* >::iterator i = m_mapDevice.begin();
	while( i != m_mapDevice.end() )
	{
		i->second->OnLostDevice();
		++i;
	}

	m_pDVBManager->OnLostDevice();
}

//Release를 해도 포인터는 살아있음
bool GCDeviceManager::ReleaseDevice( GCDevice* pDevice )
{
	if( NULL == pDevice )
		return true;

	//assert( m_mapDevice.find( pDevice->GetDeviceID() ) != m_mapDevice.end() );
	if( m_mapDevice.find( pDevice->GetDeviceID() ) == m_mapDevice.end() )
		return true;

	--pDevice->m_iRefCount;

	if( pDevice->m_iRefCount <= 0 )
	{
		pDevice->UnLoad();
		pDevice->m_iRefCount = 0;
	}
	return true;
}

bool GCDeviceManager::ReleaseDevice( std::string DeviceID )
{
	return ReleaseDevice(FindDevice(DeviceID));
}

std::string FileSizeToString( double dSize )
{
	char strTemp[MAX_PATH];

	/*
	if( dSize < 1024 )
	{
	sprintf( strTemp, "%d Byte", (int)dSize );
	return strTemp;
	}
	*/
	dSize /= 1024;

	//if( dSize < 1024 )
	//{
	sprintf( strTemp, "%d KB", (int)dSize );

	//	return strTemp;
	//}
	//dSize /= 1024;

	//sprintf( strTemp, "%01f MB", dSize );
	return strTemp;
}

void GCDeviceManager::WriteDeviceList( FILE* fp )
{
	stdext::hash_map<std::string,GCDevice*>::iterator i = m_mapDevice.begin();
	std::string strSize;
	std::vector<GCDevice*> vecDeviceType[GCDT_TOTAL];		//Type별로 나눠줌

	double dTotalSize = 0;
	double dTypeTotalSize = 0;
	double dTemp = 0;

	//==========================================================
	dTemp = 15.0f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "FrameWork : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 4.6f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "UI Class : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 10.0f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "P2P Connection : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 3.0f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "Server Connection : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 3.0f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "Font Data : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 1.0f * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "String : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = 1.5 * 1024 * 1024;
	dTotalSize += dTemp;
	fprintf( fp, "SortFile : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	/*dTemp = GCMassFile::m_BufferManager1.GetBufferSize();
	dTotalSize += dTemp;
	fprintf( fp, "BufferManager1 : %-45s\r\n", FileSizeToString(dTemp).c_str() );

	dTemp = GCMassFile::m_BufferManager2.GetBufferSize();
	dTotalSize += dTemp;
	fprintf( fp, "BufferManager2 : %-45s\r\n", FileSizeToString(dTemp).c_str() );*/

	//==========================================================
	fprintf( fp, "==========================================================\r\n", FileSizeToString(dTemp).c_str() );

	//==========================================================

	while( i != m_mapDevice.end() )
	{
		GCDevice* pDevice = i->second;

		if( pDevice->GetDeviceType() == GCDT_NULL )
		{
			assert( "pDevice->GetDeviceType() == GCDT_NULL" );
			fprintf( fp, "%-40s\t:\tInvalid DeviceType\r\n", pDevice->GetDeviceID().c_str() );
		}
		else
		{
			vecDeviceType[pDevice->GetDeviceType()].push_back( pDevice );
		}
		i++;
	}

	GCDevice* pDevice;

	for(int j = 0; j < GCDT_TOTAL; j++ )
	{
		fprintf( fp, "%d : %d\n", j, vecDeviceType[j].size() );
	}

	fprintf( fp, "\n\n" );

	for(int j = 0; j < GCDT_TOTAL; j++ )
	{
		dTypeTotalSize = 0;

		for(int k = 0; k < (int)vecDeviceType[j].size(); k++ )
		{
			pDevice = vecDeviceType[j][k];
			if( pDevice->IsDisable() )
				fprintf( fp, "%-40s\t:\tDisable File\r\n", pDevice->GetDeviceID().c_str());
			else
			{
				dTotalSize += pDevice->GetDeviceSize();
				dTypeTotalSize += pDevice->GetDeviceSize();
				strSize = FileSizeToString( pDevice->GetDeviceSize() );
				fprintf( fp, "%-25s\t:\tLoad(%d)\t:\tRef(%d)\t:\t%s\r\n", pDevice->GetDeviceID().c_str(), pDevice->m_iLoadCount, pDevice->m_iRefCount, strSize.c_str() );
			}
		}

		strSize = FileSizeToString( dTypeTotalSize );
		fprintf( fp, "==============================Total : %s\r\n", strSize.c_str() );
	}

	strSize = FileSizeToString( dTotalSize );
	fprintf( fp, "\r\n##############################Total : %s\r\n", strSize.c_str() );
}

bool GCDeviceManager::MakeDeviceList( std::string strFileName )
{
	FILE* fp = fopen( strFileName.c_str(), "wb" );
	if( fp == NULL )
		return false;

	WriteDeviceList(fp);

	fclose(fp);
	return true;
}

GCDevice* GCDeviceManager::FindDevice( std::string DeviceID )
{
	GCUTIL_STR::ToLower(DeviceID );
	if( m_mapDevice.find(DeviceID) == m_mapDevice.end() )
		return NULL;
	return m_mapDevice[DeviceID];
}

GCDeviceFontTH* GCDeviceManager::CreateFontTHA( std::string DeviceID, int iFontSize, int iOutLineSize, bool bRHW, int iWeight, float fFontScale)
{
	std::string FontDeviceID = GCDeviceFontBase::MakeFontID(DeviceID, iFontSize, iWeight, iOutLineSize, bRHW,fFontScale);
	GCDevice* pDevice = FindDevice(FontDeviceID);

	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceFontTH*)pDevice;
	}

	GCDeviceFontTH* pNewDevice = new GCDeviceFontTH( m_d3ddevice, DeviceID, iFontSize, iWeight, iOutLineSize, bRHW,fFontScale );
	pNewDevice->AddRefCount();
	m_mapDevice[pNewDevice->GetDeviceID()] = pNewDevice;

	return pNewDevice;
}

GCDeviceFontBase* GCDeviceManager::CreateFontA( std::string DeviceID, int iFontSize, int iOutLineSize, bool bRHW, int iWeight, float fFontScale)
{
	std::string FontDeviceID = GCDeviceFontBase::MakeFontID(DeviceID, iFontSize, iWeight, iOutLineSize, bRHW,fFontScale);
	GCDevice* pDevice = FindDevice(FontDeviceID);

	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceFontBase*)pDevice;
	}

	GCDeviceFontBase* pNewDevice = new GCDeviceFontBase( m_d3ddevice, DeviceID, iFontSize, iWeight, iOutLineSize, bRHW,fFontScale );
	pNewDevice->AddRefCount();
	m_mapDevice[pNewDevice->GetDeviceID()] = pNewDevice;

	return pNewDevice;
}

GCDeviceTexture* GCDeviceManager::CreateTexture( std::string DeviceID, bool bLowQuality/* = false*/ , bool bRealPath)
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceTexture*)pDevice;
	}

	GCDeviceTexture* pNewDevice = new GCDeviceTexture(m_d3ddevice, DeviceID, bLowQuality, bRealPath);
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCImageIconTexture* GCDeviceManager::CreateImageIconTexture( std::string DeviceID, bool bLowQuality/* = false*/ , bool bRealPath)
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCImageIconTexture*)pDevice;
	}

	GCImageIconTexture* pNewDevice = new GCImageIconTexture(m_d3ddevice, DeviceID, bLowQuality, bRealPath);
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCTmpTexture* GCDeviceManager::CreateTmpTexture( std::string DeviceID )
{
	GCUTIL_STR::ToLower(DeviceID );
	std::string tmpid = DeviceID+"tmp";
	GCDevice* pDevice = FindDevice(tmpid);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCTmpTexture*)pDevice;
	}

	if( g_pGCDeviceManager->GetMassFileManager()->IsIn(DeviceID) == false )
		return NULL;

	GCTmpTexture* pNewDevice = new GCTmpTexture(m_d3ddevice, DeviceID);
	pNewDevice->AddRefCount();
	m_mapDevice[tmpid] = pNewDevice;
	return pNewDevice;
}

GCDeviceTexture* GCDeviceManager::CreateTextureThread( std::string DeviceID, bool bLowQuality /*= false */, bool bLoadThread )
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceTexture*)pDevice;
	}

	GCDeviceTexture* pNewDevice = new GCDeviceTexture(m_d3ddevice, DeviceID, bLowQuality, false, bLoadThread );
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCDeviceTexture* GCDeviceManager::CreateTextureWithNoLoad( std::string DeviceID, bool bLowQuality /*= false */ )
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		return (GCDeviceTexture*)pDevice;
	}

	GCDeviceTexture* pNewDevice = new GCDeviceTexture(m_d3ddevice, DeviceID, bLowQuality, false );
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCDeviceMeshP3M* GCDeviceManager::CreateMeshP3M( std::string DeviceID, char cIgBoneIdx/* = -1*/ )
{
	GCUTIL_STR::ToLower( DeviceID );
	std::string tmpid = DeviceID;
	if( cIgBoneIdx != -1 )
	{
		char tmpStr[100];
		sprintf_s( tmpStr, 100, "%s__%d", DeviceID.c_str(), (int)cIgBoneIdx );
		tmpid = tmpStr;
	}
	GCDevice* pDevice = FindDevice(tmpid);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceMeshP3M*)pDevice;
	}

	GCDeviceMeshP3M* pNewDevice = new GCDeviceMeshP3M( m_d3ddevice, DeviceID, cIgBoneIdx );
	pNewDevice->AddRefCount();
	m_mapDevice[tmpid] = pNewDevice;

	return pNewDevice;
}

GCTmpMeshP3M* GCDeviceManager::CreateTmpMeshP3M( std::string DeviceID, char cIgBoneIdx/* = -1*/ )
{
	GCUTIL_STR::ToLower(DeviceID);
	std::string tmpid = DeviceID + "tmp";
	if( cIgBoneIdx != -1 )
	{
		char tmpStr[100];
		sprintf_s( tmpStr, 100, "%stmp__%d", DeviceID.c_str(), (int)cIgBoneIdx );
		tmpid = tmpStr;
	}

	GCDevice* pDevice = FindDevice(tmpid);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCTmpMeshP3M*)pDevice;
	}

	GCTmpMeshP3M* pNewDevice = new GCTmpMeshP3M( DeviceID, cIgBoneIdx );
	pNewDevice->AddRefCount();
	m_mapDevice[tmpid] = pNewDevice;

	return pNewDevice;
}

GCDeviceMotionFRM* GCDeviceManager::CreateMotionFRM( std::string DeviceID )
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceMotionFRM*)pDevice;
	}

	GCDeviceMotionFRM* pNewDevice = new GCDeviceMotionFRM( DeviceID );
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCDeviceVertexBuffer* GCDeviceManager::CreateVertexBuffer( std::string DeviceID, UINT uiLength_, DWORD dwUsage_, DWORD dwFVF_, D3DPOOL Pool_, void* pData )
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceVertexBuffer*)pDevice;
	}

	GCDeviceVertexBuffer* pNewDevice = new GCDeviceVertexBuffer( DeviceID, uiLength_, dwUsage_, dwFVF_, Pool_, pData );
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}
GCDeviceIndexBuffer* GCDeviceManager::CreateIndexBuffer( std::string DeviceID, UINT uiLength_, DWORD dwUsage_, D3DFORMAT Format_, D3DPOOL Pool_, void* pData_ )
{
	GCUTIL_STR::ToLower(DeviceID );
	GCDevice* pDevice = FindDevice(DeviceID);
	if( pDevice != NULL )
	{
		pDevice->AddRefCount();
		return (GCDeviceIndexBuffer*)pDevice;
	}

	GCDeviceIndexBuffer* pNewDevice = new GCDeviceIndexBuffer( DeviceID, uiLength_, dwUsage_, Format_, Pool_, pData_ );
	pNewDevice->AddRefCount();
	m_mapDevice[DeviceID] = pNewDevice;

	return pNewDevice;
}

GCDeviceShader*	 GCDeviceManager::CreateShader( std::string strDeviceID )
{
	FIND_DEVICE_RETURN( GCDeviceShader, strDeviceID);

	GCDeviceShader* pNewDevice = new GCDeviceShader(m_d3ddevice, strDeviceID);
	pNewDevice->AddRefCount();
	m_mapDevice[strDeviceID] = pNewDevice;

#if defined( USE_SHADER_LOAD_HLSL )
	pNewDevice->RegisterFileMoniter();
#endif

	return pNewDevice;
}

void GCDeviceManager::DrawInWorld(const  GCBaseTexture* tex,const SLVERTEX* vert,int size)
{
	if(!vert)
		return;

	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize+size >= WORLD_VB_SIZE)
		FlushWorld();

	m_pCurTex = (GCBaseTexture*)tex;
	for( int i = 0 ; i < (int)4*size ; ++i )
	{
		m_pWorldVertex[4*m_iWorldVBSize + i] = vert[i];
	}
	m_iWorldVBSize+=size;
}

void GCDeviceManager::DrawInWorld( const GCBaseTexture* tex,const D3DXMATRIX *mat,
								  float Size_X, float Size_Y, float tu1, float tv1, float tu2, float tv2,
								  float fOffsetX/*=0*/, float fOffsetY/*=0*/,float fOffsetZ, D3DCOLOR col /*=0xffffffff*/)
{
	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize + 1 >= WORLD_VB_SIZE)
		FlushWorld();

	Size_X*=.5f;
	Size_Y*=.5f;

	m_pWorldVertex[4*m_iWorldVBSize+0].SetSLVER(-Size_X+ fOffsetX ,Size_Y + fOffsetY,fOffsetZ,tu1,tv1,col);
	m_pWorldVertex[4*m_iWorldVBSize+1].SetSLVER(Size_X+ fOffsetX,Size_Y+ fOffsetY,fOffsetZ,tu2,tv1,col);
	m_pWorldVertex[4*m_iWorldVBSize+2].SetSLVER(-Size_X + fOffsetX,-Size_Y+ fOffsetY,fOffsetZ,tu1,tv2,col);
	m_pWorldVertex[4*m_iWorldVBSize+3].SetSLVER(Size_X+ fOffsetX,-Size_Y+ fOffsetY,fOffsetZ,tu2,tv2,col);

	if(mat)
	{
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,mat);
	}

	m_iWorldVBSize+=1;
	m_pCurTex = (GCBaseTexture*)tex;
}

void GCDeviceManager::DrawInWorld( const GCBaseTexture* tex,const D3DXMATRIX *mat,
								  D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, D3DXVECTOR3& pos3, D3DXVECTOR3& pos4,
								  float fOffsetX/*=0*/, float fOffsetY/*=0*/,float fOffsetZ/*=0.0f*/,D3DCOLOR col /*=0xffffffff*/)
{
	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize + 1 >= WORLD_VB_SIZE)
		FlushWorld();

	m_pWorldVertex[4*m_iWorldVBSize+0].SetSLVER( pos1.x + fOffsetX, pos1.y + fOffsetY, pos1.z + fOffsetZ, 0, 1 ,col);
	m_pWorldVertex[4*m_iWorldVBSize+1].SetSLVER( pos2.x + fOffsetX, pos2.y + fOffsetY, pos2.z + fOffsetZ, 0, 0 ,col);
	m_pWorldVertex[4*m_iWorldVBSize+2].SetSLVER( pos3.x + fOffsetX, pos3.y + fOffsetY, pos3.z + fOffsetZ, 1, 1 ,col);
	m_pWorldVertex[4*m_iWorldVBSize+3].SetSLVER( pos4.x + fOffsetX, pos4.y + fOffsetY, pos4.z + fOffsetZ, 1, 0 ,col);

	if(mat)
	{
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,mat);
	}

	m_iWorldVBSize+=1;
	m_pCurTex = (GCBaseTexture*)tex;
}

void GCDeviceManager::DrawInWorld( const GCBaseTexture* tex,const D3DXMATRIX *mat,const SLVERTEX* vert ,int size)
{
	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize+size >= WORLD_VB_SIZE)
		FlushWorld();

	for( int i = 0 ; i < (int)4*size ; ++i )
	{
		m_pWorldVertex[4*m_iWorldVBSize+i] = vert[i];
	}

	if(mat)
	{
		for(int i=0;i<4*size;i++)
			D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+i].Pos,&m_pWorldVertex[4*m_iWorldVBSize+i].Pos,mat);
	}

	m_pCurTex = (GCBaseTexture*)tex;
	m_iWorldVBSize+=size;
}

void GCDeviceManager::DrawInWorldWithStrip(const GCBaseTexture* tex,const D3DXMATRIX *mat,const SLVERTEX* vert,int size)
{
	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize+size >= WORLD_VB_SIZE)
		FlushWorld();

	int dest = 0;
	int vertindex = 0;
	for(int i=0;i<size;i++)
	{
		for( int i = 0 ; i < 4 ; ++i )
		{
			m_pWorldVertex[4*m_iWorldVBSize + dest+i] = vert[vertindex+i];
		}
		dest+=4;
		vertindex+=2;
	}

	if(mat)
	{
		for(int i=0;i<4*size;i++)
			D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+i].Pos,&m_pWorldVertex[4*m_iWorldVBSize+i].Pos,mat);
	}

	m_pCurTex = (GCBaseTexture*)tex;
	m_iWorldVBSize+=size;
}

void GCDeviceManager::DrawInWorldResize( const GCBaseTexture* tex,const D3DXMATRIX *mat,
										D3DXVECTOR3& pos1, D3DXVECTOR3& pos2,
										D3DXVECTOR3& pos3, D3DXVECTOR3& pos4,
										float fU1, float fV1, float fU2, float fV2, D3DCOLOR col /*=0xffffffff*/)
{
	FlushScreen();

	if(m_pCurTex!=tex || m_iWorldVBSize + 1 >= WORLD_VB_SIZE)
		FlushWorld();

	m_pWorldVertex[4*m_iWorldVBSize+0].SetSLVER( pos1.x, pos1.y, pos1.z, fU1, fV1, col);    // 좌상
	m_pWorldVertex[4*m_iWorldVBSize+1].SetSLVER( pos2.x, pos2.y, pos2.z, fU2, fV1, col);    // 우상
	m_pWorldVertex[4*m_iWorldVBSize+2].SetSLVER( pos3.x, pos3.y, pos3.z, fU1, fV2, col);    // 좌하
	m_pWorldVertex[4*m_iWorldVBSize+3].SetSLVER( pos4.x, pos4.y, pos4.z, fU2, fV2, col);    // 우하

	if( mat )
	{
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,&m_pWorldVertex[4*m_iWorldVBSize+0].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,&m_pWorldVertex[4*m_iWorldVBSize+1].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,&m_pWorldVertex[4*m_iWorldVBSize+2].Pos,mat);
		D3DXVec3TransformCoord (&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,&m_pWorldVertex[4*m_iWorldVBSize+3].Pos,mat);
	}

	m_iWorldVBSize+=1;
	m_pCurTex = (GCBaseTexture*)tex;
}

void GCDeviceManager::FlushWorld()
{
	if(!m_iWorldVBSize)
		return;

	if(m_iWorldVBSize >= WORLD_VB_SIZE)
	{
		ASSERT( !"overflow" );
		m_iWorldVBSize=0;
		return;
	}

	// 먼저 락을 걸고
	SLVERTEX  *vertex;
	m_pWorldVB->Lock( 0, 0, (void**)&vertex, 0 );
	memcpy(vertex,m_pWorldVertex.as_array(), 4 * sizeof(SLVERTEX) * m_iWorldVBSize);
	m_pWorldVB->Unlock();// 락 풀고

	//실제로 렌더

	if(m_pCurTex != nullptr)
		m_pCurTex->SetWithLoad();
	else
		m_d3ddevice->SetTexture(0,NULL);

	m_d3ddevice->SetTransform(D3DTS_WORLD,&m_mat_id);
	m_d3ddevice->SetFVF( SLVERTEX::FVF );
	m_d3ddevice->SetStreamSource(0,m_pWorldVB,0,sizeof(SLVERTEX));
	m_d3ddevice->SetIndices(m_pIB);
	m_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iWorldVBSize*4, 0, m_iWorldVBSize*2 );

	m_pCurTex=NULL;
	m_iWorldVBSize=0;
}

void GCDeviceManager::DrawInScreen( const GCBaseTexture* tex,const KD3DUIVertex* vert,int size )
{
	ASSERT((void*)0xcdcdcdcd!=(void*)tex);
	if(!vert)
		return;

	FlushWorld();

	if(m_pCurTex!=tex || m_iScreenVBSize+size >= SCREEN_VB_SIZE)
		FlushScreen();

	m_pCurTex = (GCBaseTexture*)tex;
	for( int i = 0 ; i < 4 * size ; ++i )
	{
		m_pScreenVertex[i + 4*m_iScreenVBSize] = vert[i];
	}

	m_iScreenVBSize+=size;
}

void GCDeviceManager::DrawInScreenResize( const GCBaseTexture* tex,float X1, float Y1, float X2, float Y2, float tu1, float tv1, float tu2, float tv2, bool Rotate/*=false*/, float Z/*=10.0f*/, D3DCOLOR col/*= 0xffffffff*/)
{
	DrawInScreen( tex, static_cast<float>( X1 * m_windowScaleX ), static_cast<float>( Y1 * m_windowScaleY ),
		static_cast<float>( X2 * m_windowScaleX ), static_cast<float>( Y2 * m_windowScaleY ),
		tu1,  tv1,  tu2, tv2,Rotate,  Z,col);
}
void GCDeviceManager::DrawInScreen( const GCBaseTexture* tex,float X1, float Y1, float X2, float Y2, float tu1, float tv1, float tu2, float tv2, bool Rotate/*=false*/, float Z/*=10.0f*/, D3DCOLOR col/*= 0xffffffff*/ ,bool bFixed_ /*= true*/)
{
	ASSERT((void*)0xcdcdcdcd!=(void*)tex);
	FlushWorld();

	if(m_pCurTex!=tex || m_iScreenVBSize+1 >= SCREEN_VB_SIZE)
	{
		FlushScreen();
	}

	if (tex)
	{
		if (bFixed_)
		{
			m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.x=static_cast<long>(X1)-0.5f;
			m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.x=static_cast<long>(X2)-0.5f;
			m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.y=static_cast<long>(Y1)-0.5f;
			m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.y=static_cast<long>(Y2)-0.5f;
		}
		else
		{
			m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.x=X1;
			m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.x=X2;
			m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.y=Y1;
			m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.y=Y2;
		}
	}
	else
	{
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.x=X1;
		m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.x=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.x=X2;
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.y=Y1;
		m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.y=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.y=Y2;
	}

	m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.z=m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.z=m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.z=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.z=Z;
	m_pScreenVertex[4*m_iScreenVBSize+0].m_vPos.w=m_pScreenVertex[4*m_iScreenVBSize+1].m_vPos.w=m_pScreenVertex[4*m_iScreenVBSize+2].m_vPos.w=m_pScreenVertex[4*m_iScreenVBSize+3].m_vPos.w=1.0f;

	if(Rotate){
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.x=m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.x=tu1;
		m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.x=m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.x=tu2;
		m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.y=m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.y=tv1;
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.y=m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.y=tv2;
	}else{
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.x=m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.x=tu1;
		m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.x=m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.x=tu2;
		m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.y=m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.y=tv1;
		m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.y=m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.y=tv2;
	}
	m_pScreenVertex[4*m_iScreenVBSize+0].m_diff=m_pScreenVertex[4*m_iScreenVBSize+1].m_diff=m_pScreenVertex[4*m_iScreenVBSize+2].m_diff=m_pScreenVertex[4*m_iScreenVBSize+3].m_diff=col;

	m_pCurTex = (GCBaseTexture*)tex;
	m_iScreenVBSize+=1;
}

void GCDeviceManager::FlushScreen()
{
	if(!m_iScreenVBSize)
		return;

	if( m_iScreenVBSize >= SCREEN_VB_SIZE)
	{
		ASSERT(!"overflow" );
		return;
	}

	// 먼저 락을 걸고
	KD3DUIVertex  *vertex;
	m_pScreenVB->Lock( 0, 0, (void**)&vertex, 0 );
	memcpy(vertex,m_pScreenVertex.as_array(), 4 * sizeof(KD3DUIVertex) * m_iScreenVBSize);
	m_pScreenVB->Unlock();// 락 풀고

	//실제로 렌더

	if(m_pCurTex)
		m_pCurTex->SetWithLoad();
	else
		m_d3ddevice->SetTexture(0,NULL);

	/*m_d3ddevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_d3ddevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );*/
	/*m_d3ddevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	m_d3ddevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );*/
	/*m_d3ddevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_d3ddevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	m_d3ddevice->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );*/

	//------------

	m_d3ddevice->SetFVF( KD3DUIVertex::FVF );
	m_d3ddevice->SetStreamSource(0,m_pScreenVB,0,sizeof(KD3DUIVertex));
	m_d3ddevice->SetIndices(m_pIB);
	m_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iScreenVBSize*4, 0, m_iScreenVBSize*2 );

	m_pCurTex=NULL;
	m_iScreenVBSize=0;
}

void GCDeviceManager::SetRenderState( D3DRENDERSTATETYPE state, DWORD value )
{
	DWORD old;
	KGCStateBlock * cur = GetCurrentStateBlock();

	m_d3ddevice->GetRenderState(state,&old);

	if(old!=value)
	{
		if(cur)
			cur->Push(KGCStateBlock::STATE_RENDER,0,state,old);
		Flush();
		m_d3ddevice->SetRenderState(state,value);
	}
}

void GCDeviceManager::SetSamplerState( int stage,D3DSAMPLERSTATETYPE state, DWORD value )
{
	DWORD old;
	KGCStateBlock * cur = GetCurrentStateBlock();

	m_d3ddevice->GetSamplerState(stage,state,&old);

	if(old!=value)
	{
		if(cur)
			cur->Push(KGCStateBlock::STATE_SAMPLER,stage,state,old);
		Flush();
		m_d3ddevice->SetSamplerState(stage, state,value);
	}
}
void GCDeviceManager::SetTextureStageState( DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value )
{
	DWORD old;
	KGCStateBlock * cur = GetCurrentStateBlock();

	m_d3ddevice->GetTextureStageState(stage,state,&old);
	if(old!=value)
	{
		if(cur)
			cur->Push(KGCStateBlock::STATE_TEXTURESTAGE,stage,state,old);
		Flush();
		m_d3ddevice->SetTextureStageState(stage,state,value);
	}
}

#if !defined( __PATH__ )
void GCDeviceManager::OutputLoadedDeviceList()
{
	std::fstream file;

	file.open( "LoadedDevice.txt", std::ios_base::out );

	stdext::hash_map<std::string,GCDevice*>::iterator mit;

	for( mit = m_mapDevice.begin() ; mit != m_mapDevice.end() ; ++mit )
	{
		if( mit->second->IsLoaded() == false )
			continue;

		file<<(mit->second)->GetDeviceID()<<"\t"<<(mit->second)->GetDeviceSize()<<std::endl;
	}

	file.close();
}
#endif

namespace{
	int DEFAULT_SCREEN_WIDTH = GC_SCREEN_WIDTH;
	int DEFAULT_SCREEN_HEIGHT = GC_SCREEN_HEIGHT;
}

float GCDeviceManager::GetWindowScaleX()
{
	if( IsWindowMode() )
	{
		int iScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
		int iCreationWidth = static_cast<int>( DEFAULT_SCREEN_WIDTH * m_windowScaleX );

		if( iScreenWidth < iCreationWidth )
		{
			return 1.0f;
		}
	}
	return m_windowScaleX;
}

float GCDeviceManager::GetWindowScaleY()
{
	if( IsWindowMode() )
	{
		int iScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
		int iCreationHeight = static_cast<int>( DEFAULT_SCREEN_HEIGHT * m_windowScaleY );

		if( iScreenHeight < iCreationHeight )
		{
			return 1.0f;
		}
	}
	return m_windowScaleY;
}

void GCDeviceManager::SetWindowScale( float x_, float y_ )
{
	SetWindowScaleX( x_ );
	SetWindowScaleY( y_ );
}

void GCDeviceManager::SetWindowScaleX( float x_ )
{
	m_windowScaleX = x_;
}

void GCDeviceManager::SetWindowScaleY( float y_ )
{
	m_windowScaleY = y_;
}

void GCDeviceManager::SetReferenceResource( std::string strSource, std::string strTarget )
{
	GCUTIL_STR::ToLower(strSource );
	GCUTIL_STR::ToLower(strTarget );
	if ( m_mapDevice.find( strSource ) == m_mapDevice.end() )
	{
		return;
	}

	if ( m_mapDevice.find( strTarget ) != m_mapDevice.end() )
	{
		return;
	}
	m_mapDevice[strTarget] = m_mapDevice[strSource];
}

void GCDeviceManager::SetScissorRect( RECT* pRect )
{
	m_d3ddevice->SetScissorRect( pRect );
}

void GCDeviceManager::GetCapsInfo( D3DCAPS9* caps )
{
	if( m_d3ddevice == NULL ) {
		return;
	}

	m_d3ddevice->GetDeviceCaps( caps );
}

bool GCDeviceManager::IsSoftwareVertexProcess( void )
{
	if( m_d3ddevice == NULL ) {
		return false;
	}

	if( m_d3ddevice->GetSoftwareVertexProcessing() == TRUE ) {
		return true;
	}

	return false;
}

void GCDeviceManager::SetDeviceErrorLog( IN DeviceManagerErr errType_, IN HRESULT hResult_ )
{
	if( errType_ < ERR_CREATE_WORLD_VB || errType_ > ERR_FSCREEN_DEVICE_DRAW ) {
		return;
	}

	std::map< DeviceManagerErr, HRESULT >::iterator mit = m_mapError.find( errType_ );
	if( mit == m_mapError.end() ) {
		m_mapError.insert( std::make_pair( errType_, hResult_ ) );
	}
	else {
		mit->second = hResult_;
	}
}

std::string GCDeviceManager::GetDeviceErrorLog( void )
{
	if( m_mapError.empty() ) {
		return "";
	}

	std::ostringstream stmLog;
	stmLog << "=============================================================================" << std::endl;
	stmLog << "                         Device Error Log" << std::endl;
	stmLog << "=============================================================================" << std::endl;

	std::map< DeviceManagerErr, HRESULT >::iterator mit;
	for( mit = m_mapError.begin() ; mit != m_mapError.end() ; ++mit ) {
		stmLog << "HRESULT : " << mit->second << "\t" << GetErrorString( mit->first ) << std::endl;
	}

	return stmLog.str();
}

std::string GCDeviceManager::GetErrorString( DeviceManagerErr errType_ )
{
	switch( errType_ ) {
	case ERR_CREATE_WORLD_VB:
		return "Error!!    GCDeviceManager::GCDeviceManage()   m_d3ddevice->CreateVertexBuffer( WORLD_VB_SIZE * 4 * sizeof(SLVERTEX), D3DUSAGE_WRITEONLY, SLVERTEX::FVF, D3DPOOL_MANAGED, &m_pWorldVB, NULL )";
	case ERR_CREATE_SCREEN_VB:
		return "Error!!    GCDeviceManager::GCDeviceManage()   m_d3ddevice->CreateVertexBuffer( SCREEN_VB_SIZE * 4 * sizeof(KD3DUIVertex), D3DUSAGE_WRITEONLY, KD3DUIVertex::FVF, D3DPOOL_MANAGED, &m_pScreenVB, NULL )";
	case ERR_CREATE_IB:
		return "Error!!    GCDeviceManager::GCDeviceManage()   m_d3ddevice->CreateIndexBuffer( sizeof(WORD) * max(WORLD_VB_SIZE,SCREEN_VB_SIZE) * 6, D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pIB, NULL )";
	case ERR_FWORLD_VB_LOCK:
		return "Error!!    GCDeviceManager::FlushWorld()   m_pWorldVB->Lock( 0, 0, (void**)&vertex, 0 )";
	case ERR_FWORLD_VB_UNLOCK:
		return "Error!!    GCDeviceManager::FlushWorld()   m_pWorldVB->Unlock()";
	case ERR_FWORLD_DEVICE_TRANS:
		return "Error!!    GCDeviceManager::FlushWorld()   m_d3ddevice->SetTransform(D3DTS_WORLD,&m_mat_id)";
	case ERR_FWORLD_DEVICE_FVF:
		return "Error!!    GCDeviceManager::FlushWorld()   m_d3ddevice->SetFVF( SLVERTEX::FVF )";
	case ERR_FWROLD_DEVICE_STREAM:
		return "Error!!    GCDeviceManager::FlushWorld()   m_d3ddevice->SetStreamSource(0,m_pWorldVB,0,sizeof(SLVERTEX))";
	case ERR_FWROLD_DEVICE_INDEX:
		return "Error!!    GCDeviceManager::FlushWorld()   m_d3ddevice->SetIndices(m_pIB)";
	case ERR_FWROLD_DEVICE_DRAW:
		return "Error!!    GCDeviceManager::FlushWorld()   m_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iWorldVBSize*4, 0, m_iWorldVBSize*2 )";
	case ERR_FSCREEN_VB_LOCK:
		return "Error!!    GCDeviceManager::FlushScreen()   m_pScreenVB->Lock( 0, 0, (void**)&vertex, 0 )";
	case ERR_FSCREEN_VB_UNLOCK:
		return "Error!!    GCDeviceManager::FlushScreen()   m_pScreenVB->Unlock()";
	case ERR_FSCREEN_DEVICE_FVF:
		return "Error!!    GCDeviceManager::FlushScreen()   m_d3ddevice->SetFVF( KD3DUIVertex::FVF );";
	case ERR_FSCREEN_DEVICE_STREAM:
		return "Error!!    GCDeviceManager::FlushScreen()   m_d3ddevice->SetIndices(m_pIB)";
	case ERR_FSCREEN_DEVICE_INDEX:
		return "Error!!    GCDeviceManager::FlushScreen()   m_d3ddevice->CreateVertexBuffer( SCREEN_VB_SIZE * 4 * sizeof(KD3DUIVertex), D3DUSAGE_WRITEONLY, KD3DUIVertex::FVF, D3DPOOL_MANAGED, &m_pScreenVB, NULL )";
	case ERR_FSCREEN_DEVICE_DRAW:
		return "Error!!    GCDeviceManager::FlushScreen()   m_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iScreenVBSize*4, 0, m_iScreenVBSize*2 )";
	default:
		return "";
	}
}
