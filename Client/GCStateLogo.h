#ifndef _GCSTATELOGO_H_
#define _GCSTATELOGO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "KGCState.h"

class GCDeviceTexture;

class KGCStateLogo : public KGCState
{
public:
	KGCStateLogo(void);
	virtual ~KGCStateLogo(void);

public:

	enum LOGO_STATE{
		STATE_INIT = 0,
		STATE_WAIT = 1,
		STATE_READY = 2,
		STATE_LOGO = 3,
		STATE_PROGRESS = 4,
		STATE_OUT = 5,
		STATE_ELSE = 6,
	};
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();

	HRESULT InitDevice();
	HRESULT LoadTexture();

	void DeleteDevice();

private:
	GCDeviceTexture*	m_pKOGLogo;
	GCDeviceTexture*	m_pPublisherLogo;
    GCDeviceTexture*	m_pPanhoLogo;

	D3DXMATRIX			m_matWorld;

	DWORD				m_dwLastFrameTime;
	LOGO_STATE			m_eLogoState;
	float				m_fPitch;
	bool				m_bPublisherLogoState;
    bool				m_bPanhoLogoState;
};

#endif //_GCSTATELOGO_H_
