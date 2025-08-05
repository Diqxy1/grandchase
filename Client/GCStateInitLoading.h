#ifndef _GCSTATEINITLOADING_H_
#define _GCSTATEINITLOADING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

#define ALL_RESOURCE_LOADED	(100)	//100%�� ���̶�� ����.

class KGCStateInitLoading : public KGCState
{
public:
	KGCStateInitLoading(void);
	virtual ~KGCStateInitLoading(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
    virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();

	HRESULT InitDevice();

	HRESULT LoadTexture_FromMassFile();
	void	Render2D( LPDIRECT3DTEXTURE9 pTexture , RECT rtScreen ,D3DXVECTOR4 rtUv, float fWidthRatio = 1.0f, bool bRotate=false );
	void	DeleteDevice();

	//bool        LoadManufactureScript();                // ���� ��ũ��Ʈ �Լ��� �и�
	bool		LoadResource();							//���ҽ��� �ε��ϴ� �Լ��̴�.
	bool		ShowloadingBar(int iLoadedResource);	//���� �󸶳� �ε��ǰ� �ִ°�, �ٸ� �����ִ� �Լ��̴�.
	HRESULT		FinishLoading();

    void LoadCTMissionString();
    void LoadSkillString();

private:
	std::vector<LPDIRECT3DTEXTURE9>		m_pLoadingTexture;
	LPDIRECT3DTEXTURE9					m_pLoadingBarTexture;
	std::vector<GCDeviceTexture*>		m_pTexLoadingTexture_from_massfile;
	GCDeviceTexture*					m_pTexLoadingBarTexture_from_massfile;
	LPDIRECT3DTEXTURE9					m_pLoadingExtraTexture;
	KD3DUIVertex						m_rect[4];
	int									m_iJobStep;
    GCDeviceVertexBuffer*				m_pVertex;

	GCDeviceTexture*					m_pLoadingBGTexture;

public:	
	int									m_iLoadedResource;

    FILE *m_pLoadingTimeFile;
    DWORD m_dwTotalTime;
	std::wstring strLoadingPercent;
};

extern KGCStateInitLoading* g_pStateInitLoading;

#endif //_GCSTATEINITLOADING_H_
