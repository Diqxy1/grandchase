#pragma once

#include "../KGCMap.h"
#include "../WOGMapTOol.h"
#include "../ToolObject.h"

class KGridGuide
{
public:
	KGridGuide(void);
	virtual ~KGridGuide(void);

private:
	D3DXVECTOR2 m_vPos;         // 3D 위치
	bool m_bVertical;           // 세로인가
	bool m_bDefault;            // 기본으로 그리는것?
	CToolObject::FootHoldVertex m_Vertex[2];    // for drawing
	static DWORD m_dwAlphaEnable;
	static DWORD m_dwZEnable;

public:
	HRESULT Render();
	HRESULT RenderFH();
	bool    IsMouseOver(const D3DVECTOR& vMousePos);

	static void    PreRenderProcess();
	static void    PostRenderProcess();

	// getter, setter
	void        SetVertical(bool bVerteical) { m_bVertical = bVerteical; }
	bool        GetVertical() const { return m_bVertical; }
	void        SetDefault(bool bDefault) { m_bDefault = bDefault; }
	bool        GetDefault() const { return m_bDefault; }
	void        SetGridGuidePos(const D3DXVECTOR2& vPos_) { m_vPos = vPos_; }
	D3DXVECTOR2 GetGridGuidePos() const { return m_vPos; }
};
