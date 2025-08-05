#pragma once

enum GCDeviceState
{
	GCDS_NOT_LOADED = 0,		//로딩 안됨
	GCDS_LOADED = 1,			//로딩 됨
	GCDS_DISABLE = 2,			//찾을 수 없는 파일임
};

enum GCDeviceType
{
	GCDT_NULL=-1,
	GCDT_TEXTURE = 0,
	GCDT_MOTION_FRM,
	GCDT_MODEL_P3M,
	GCDT_INDEX_BUFFER,
	GCDT_VERTEX_BUFFER,
	GCDT_SPRITE,
	GCDT_FONT,
	GCDT_SHADER,
	GCDT_TEMP_TEXTURE,
	GCDT_TEMP_MESH,

	GCDT_TOTAL,
};

#define     P3M_VERSIONSIZE     (27)
#define     P3M_TEXTURENAMESIZE (260)
#define     MAX_BONECHILDS      (10)
#define     MAX_BONE            (100)
#define     MAX_BONENAME        (20)
class GCDevice
{
	friend class GCDeviceManager;
public:
	GCDevice(void);
	virtual ~GCDevice(void);

	std::string& GetDeviceID() { return m_strDeviceID; }
	void SetDeviceID( std::string& strDeviceID_ );
	void Release();
	void AddRefCount();

	//Device State
	bool IsLoaded() { return m_eDeviceState == GCDS_LOADED; }
	bool IsDisable() { return m_eDeviceState == GCDS_DISABLE; }
	GCDeviceState GetDeviceState() { return m_eDeviceState; }
	GCDeviceType  GetDeviceType() { return m_eDeviceType; }

	virtual void OnResetDevice() = 0;
	virtual void OnLostDevice() = 0;

	// 2007/1/22. iridology. 우리도 리소스 드래그로 리로드 해보아요..
	virtual void Reload();

protected:
	virtual unsigned long GetDeviceSize() { return 0; }
	virtual bool Load() = 0;
	virtual void UnLoad() = 0;
protected:
	GCDeviceType		m_eDeviceType;
	int					m_iRefCount;
	int					m_iLoadCount;		//전체 로딩되었던 기록을 남긴다.
	std::string			m_strDeviceID;			//파일명이 된다

	GCDeviceState		m_eDeviceState;
};
