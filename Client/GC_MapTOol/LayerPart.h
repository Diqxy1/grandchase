#pragma once

//#include <string>
#include "KLayerPart.h"
#include "ToolObject.h"
#include "CRenderEyeInfo.h"

class KGCEffect;
class CToolObject;

class CLayerPart : public KLayerPart<CLayerPart>
	, public CFHBlockInfo
{
public:
	CLayerPart(void);
	virtual ~CLayerPart(void);

	virtual void        Render();
	virtual void        FrameMove(float fElapsedTime);
	//virtual void        RenderEffect();
	virtual void        Flush();

	virtual CLayerPart* Clone();
	virtual void        CopyMember(CLayerPart* pSrc);

	std::string         ToString();
	std::string         ToObjectString();

	bool                IsMouseIn(D3DVECTOR MPos)
	{
		if ((int)MPos.x >= m_fLayerX + m_fScrollX && (int)MPos.x <= m_fLayerX + m_fWidth + m_fScrollX)
		{
			if (m_fLayerY + m_fScrollY <= (int)MPos.y && m_fLayerY + m_fHeight + m_fScrollY >= (int)MPos.y)
			{
				return true;
			}
		}

		return false;
	}

	virtual void        Inverse(bool bLeftRight)
	{
		if (bLeftRight)
		{
			// 좌우
			m_iTexX = m_iTexX + m_iTexWidth;
			m_iTexWidth = -m_iTexWidth;
		}
		else
		{
			// 상하
			m_iTexY = m_iTexY + m_iTexHeight;
			m_iTexHeight = -m_iTexHeight;
		}

		SetTexel();
		SetVertex();
	}

	void                MoveDown(int iTerm = 1);
	void                MoveUp(int iTerm = 1);
	void                MoveLeft(int iTerm = 1);
	void                MoveRight(int iTerm = 1);

	void                SetOutLineObject(bool bOutLine) { m_bOutLineObject = bOutLine; }
	void                AddToolObject(int i_, int j_, DWORD dwProp, std::string strValue);
	inline void         SetFHWidth(SHORT sWidth) { m_sFHWidth = sWidth; }
	inline SHORT        GetFHWidth() const { return m_sFHWidth; }
	inline void         SetFHHeight(SHORT sHeight) { m_sFHHeight = sHeight; }
	inline SHORT        GetFHHeight() const { return m_sFHHeight; }
	void                InitTOVector();

	int                 GetHeadFootHoldSize();
	void                SetFootHoldVector(std::vector< CToolObject >& vecToolObj) { m_vecToolObject.clear(); m_vecToolObject = vecToolObj; }

	std::vector< CToolObject >& GetToolObjectVector() { return m_vecToolObject; }

	virtual SHORT GetSBlockWidth() { return m_sFHWidth; }
	virtual SHORT GetSBlockHeight() { return m_sFHHeight; }

	void                RenderFH();
	void                InitEyeBlockInfo(CFHBlockInfo* pBlockInfo, CRenderEyeInfo* pRenderEyeInfo);
	void                InitEyeBlockInfo(CRenderEyeInfo* pRenderEyeInfo);

	virtual void        SetLayerPos(const KLayerRect<float>& kRect);

	void                RenderHeadFH();

private:
	bool                                m_bOutLineObject;
	SHORT                               m_sFHWidth;
	SHORT                               m_sFHHeight;
	std::vector< CToolObject >          m_vecToolObject;

public:
	void DrawRectangle(float tx, float ty, float bx, float by);
	void SetRectangle(void);
	void SetStartVertex(float tx, float ty) { vStart.x = tx, vStart.y = ty; }
	void SetEndVertex(float bx, float by) { vEnd.x = bx, vEnd.y = by; }
	D3DXVECTOR2 GetStartVertex(void) { return vStart; }
	D3DXVECTOR2 GetEndVertex(void) { return vEnd; }
	bool GetValidateVertex() { return (vStart.x != -1.0f || vStart.y != -1.0f); }
	void AdjustVertex(float &tx, float &ty, float &bx, float &by);
	void SaveAndResetVertex(void);
private:
	D3DXVECTOR2 vStart;
	D3DXVECTOR2 vEnd;
};
