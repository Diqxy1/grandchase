#pragma once

class GCDeviceVertexBuffer : public GCDevice
{
public:
	LPDIRECT3DVERTEXBUFFER9         m_pBuffer;
	UINT                            m_uiLength;
	DWORD                           m_dwFVF;
	DWORD                           m_dwUsage;
	BYTE*							m_pData;
	D3DPOOL                         m_Pool;
	GCDeviceVertexBuffer( std::string& strDeviceID_, UINT uiLength_,DWORD dwUsage_,DWORD dwFVF_,D3DPOOL Pool_, void* pData = NULL );
	~GCDeviceVertexBuffer(void);
	LPDIRECT3DVERTEXBUFFER9	GetBuffer() { return m_pBuffer; }
	virtual void OnResetDevice();
	virtual void OnLostDevice();

	BYTE* Lock( BYTE* ppbData, UINT offsetToLock = 0, UINT sizeToLock = 0, DWORD flag = 0 );
	void Unlock();

	HRESULT VideoLock( UINT offsetToLock, UINT sizeToLock, BYTE** ppbData, DWORD flag );
	HRESULT VideoUnlock();

protected:
	void SystemToVideo();
	virtual bool Load();
	virtual void UnLoad();
};
