#pragma once

enum EGCRENDER_STATE
{
	EGCRENDER_STATE_INVALID = -1,

	//EGCRENDER_STATE_ZENABLE_TFACTOR = 0,
	EGCRENDER_STATE_FAR_MAP_LINEAR = 0,

	//EGCRENDER_STATE_NOT_USED = 2,
	//EGCRENDER_STATE_TOP_FADE = 3,
	//EGCRENDER_STATE_ZENABLE_ALPHA = 4,
	EGCRENDER_STATE_NEAR_MAP_LINEAR = 1,
	EGCRENDER_STATE_FAR_MAP_POINT = 2,
	EGCRENDER_STATE_NEAR_MAP_POINT = 3,

	//EGCRENDER_STATE_CARTOON = 6,
	//EGCRENDER_STATE_NO_CARTOON = 7,
	//EGCRENDER_STATE_EFFECT = 8,
	//EGCRENDER_STATE_CARTOON2 = 9,
	//EGCRENDER_STATE_3DBLUR = 10,
	//EGCRENDER_STATE_GRAYSCALE = 11,
	//EGCRENDER_STATE_GRAYSCALE_INVERSE = 12,
};

class GCDeviceShader : public GCDevice
{
public:
	GCDeviceShader(LPDIRECT3DDEVICE9 pd3ddevice, std::string strFileName);
	virtual ~GCDeviceShader(void);

	void OnResetDevice();
	void OnLostDevice();

	ID3DXEffect* GetEffect() { return m_pEffect; }

	bool SetTechnique( char* strTechnique );
	void BeginPass( UINT iIndex );
	void EndPass();

	void Reload() { UnLoad(); Load(); }

protected:
	bool Load();
	void UnLoad();

private:
	ID3DXEffect*        m_pEffect;
	bool				m_bBegin;
	std::string         m_strTechnique;
	LPDIRECT3DDEVICE9   m_pD3DDevice;

#if defined( USE_SHADER_LOAD_HLSL )
public:// 개발용 함수들
	void OnReLoadShader();
	void RegisterFileMoniter();
	void CheckFileMonitor();
#endif
};
