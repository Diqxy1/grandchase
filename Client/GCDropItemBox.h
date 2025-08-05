#pragma once
//#include "GCDeviceManager2.h"
//#include "GCDeviceLib/GCDeviceTexture.h"

class GCDeviceTexture;

#define DI_DROPITEM_BOX_FRAME	(4)
class KGCDropItemBox
{
public:
	KGCDropItemBox(void);
	~KGCDropItemBox(void);

	enum DropBoxState {
		DIB_SHOW,
		DIB_OPENING,
		DIB_OPENED,
		DIB_CLOSING,
		DIB_DISAPPEAR,
		DIB_DISABLE,
	};
public:
	void Init();
	void Render(); 
	void FrameMove();	
	void LoadBoxTexture();
	DropBoxState GetCurrentState()	{ return m_eState; }

	void OpenBox();
	void CloseBox();
private:
	KSafeArray<GCDeviceTexture*,DI_DROPITEM_BOX_FRAME> m_pTexture;
	GCDeviceTexture*	m_pCurTexture;
	int		m_iFrame;
	int		m_iFrameCount;
	int		m_iCurTex;
	DropBoxState	m_eState;

	float			    m_fx,m_fy;
	float               m_fXscale, m_fYscale;
	BYTE				m_cAlpha;
};
