#pragma once

class CRenderEyeInfo
{
public:
	CRenderEyeInfo()
		: m_fEyeX(0.0f)
		, m_fEyeY(0.0f)
		, m_fWidth(0.0f)
		, m_fHeight(0.0f)
	{}
	virtual ~CRenderEyeInfo() {}

	virtual float GetEyeX() { return m_fEyeX; }
	virtual float GetEyeY() { return m_fEyeY; }
	virtual void SetEyeX(float fEye) { m_fEyeX = fEye; }
	virtual void SetEyeY(float fEye) { m_fEyeY = fEye; }
	virtual float GetScreenWidth() { return m_fWidth; }
	virtual float GetScreenHeight() { return m_fHeight; }

protected:
	float m_fEyeX;
	float m_fEyeY;
	float m_fWidth;
	float m_fHeight;
};

class CFHBlockInfo
{
public:
	CFHBlockInfo() {}
	virtual ~CFHBlockInfo() {}

	virtual SHORT GetSBlockWidth() = 0;
	virtual SHORT GetSBlockHeight() = 0;
};