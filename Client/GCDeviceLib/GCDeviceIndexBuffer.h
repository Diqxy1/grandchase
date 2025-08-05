#pragma once

class GCDeviceIndexBuffer : public GCDevice
{
public:
	LPDIRECT3DINDEXBUFFER9          m_pBuffer;
	UINT                            m_uiLength;
	D3DFORMAT                       m_Format;
	DWORD                           m_dwUsage;
	BYTE*							m_pData;
	D3DPOOL                         m_Pool;
	GCDeviceIndexBuffer( std::string& strDeviceID_,	UINT uiLength_, DWORD dwUsage_, D3DFORMAT Format, D3DPOOL Pool_, void* pData = NULL );
	~GCDeviceIndexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 GetBuffer() { return m_pBuffer; }
	virtual void OnResetDevice();
	virtual void OnLostDevice();

	BYTE* Lock( BYTE* ppbData );
	void Unlock();

	HRESULT VideoLock( UINT offsetToLock, UINT sizeToLock, BYTE** ppbData, DWORD flag );
	HRESULT VideoUnlock();

protected:
	void SystemToVideo();
	virtual bool Load();
	virtual void UnLoad();
};
