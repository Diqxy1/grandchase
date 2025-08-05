#pragma once

#include "GCBaseMesh.h"

#define SIZE_IMAGEICON	32

class GCImageIconTexture:public GCDevice, public GCBaseTexture
{
public:
	GCImageIconTexture(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, bool bLowQuality, bool bLoadFromRealpath = false);
	virtual ~GCImageIconTexture(void);

	void OnResetDevice() {};
	void OnLostDevice() {};

	virtual void Release(){GCDevice::Release();};

	virtual void SetWithLoad(int stage =0);

	D3DFORMAT GetFileFormat() { return m_dwFormat; }
	D3DXIMAGE_FILEFORMAT    GetImageFileFormat() { return m_dwImageFileFormat; }

protected:
	virtual bool Load();
	virtual void UnLoad();
	DWORD GetDeviceSize();

	bool				    m_bLowQuality;
	D3DFORMAT			    m_dwFormat;
	D3DXIMAGE_FILEFORMAT    m_dwImageFileFormat;
	bool					m_bLoadFromRealpath;	    //KOM파일이 아닌 외부 파일에 대해 로드할지 여부 확인
};

class GCDeviceTexture :public GCBaseTexture,public GCDevice
{
public:
	GCDeviceTexture(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, bool bLowQuality, bool bLoadFromRealpath = false,
		bool bLoadThread = false );
	virtual ~GCDeviceTexture(void);

	void OnResetDevice() {};
	void OnLostDevice() {};
	void GetThreadTexture();

	virtual void Release(){GCDevice::Release();};

	virtual void SetWithLoad(int stage =0);
	virtual void SetDeviceTexture( int stage = 0 );

	D3DFORMAT GetFileFormat() { return m_dwFormat; }
	D3DXIMAGE_FILEFORMAT    GetImageFileFormat() { return m_dwImageFileFormat; }

protected:
	virtual bool Load();
	virtual void UnLoad();

public:
	DWORD GetDeviceSize();

protected:
	bool				    m_bLowQuality;
	bool                    m_bLoadThread;
	bool                    m_bWaitLoad;
	D3DFORMAT			    m_dwFormat;
	D3DXIMAGE_FILEFORMAT    m_dwImageFileFormat;
	bool					m_bLoadFromRealpath;	    //KOM파일이 아닌 외부 파일에 대해 로드할지 여부 확인
};
