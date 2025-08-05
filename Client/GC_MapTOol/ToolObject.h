#pragma once

#include "stdafx.h"
#include "FootHold.h"
#include "CRenderEyeInfo.h"

class CToolObject
{
public:
	struct FootHoldVertex
	{
		FootHoldVertex()
		{
			Pos.x = 0.0f;
			Pos.y = 0.0f;
			Pos.z = 0.0f;
			Diff = 0xffffffff;
			tv = 0.0f;
			tu = 0.0f;
		}

		D3DXVECTOR3 Pos;
		DWORD Diff;
		FLOAT       tu, tv; // Vertex texture coordinates

		void Set(float X, float Y, float Z, float ftu = 0.0f, float ftv = 0.0f)
		{
			Pos.x = X;
			Pos.y = Y;
			Pos.z = Z;

			tu = ftu;
			tv = ftv;

			Diff = 0xffffffff;
		}

		void SetDiff(DWORD Type)
		{
			switch (Type)
			{
			case CFootHold::FHT_HEAD:
				Diff = 0xffff0000;
				break;
			case CFootHold::FHT_INIT:
				Diff = 0xff919191;
				break;
			}
		}

		enum _FVF
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		};

		const FootHoldVertex& operator = (const FootHoldVertex& rvar)
		{
			this->Pos.x = rvar.Pos.x;
			this->Pos.y = rvar.Pos.y;
			this->Pos.z = rvar.Pos.z;
			this->Diff = rvar.Diff;

			return *this;
		}
	};

	enum DIRECTION
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
	};

private:
	int m_iIndexX;
	int m_iIndexY;
	int m_iIndexX_2;
	int m_iIndexY_2;
	DWORD m_dwType;
	DWORD m_dwProp;

public:
	CToolObject(void);
	CToolObject(CRenderEyeInfo* pEyeInfo, CFHBlockInfo* pBlockInfo);
	~CToolObject(void);

	void Render();
	void SetPos(int iIndexX, int iIndexY, DWORD Type, int iIndexX_2 = 0, int iIndexY_2 = 0);
	void SetProp(DWORD Prop, std::string Mode = "XOR");
	DWORD GetProp() { return m_dwProp; }
	DWORD GetType() { return m_dwType; }
	void MoveRight(bool bScroll = true, bool bPerOnePixel = false);
	void MoveLeft(bool bScroll = true, bool bPerOnePixel = false);
	void MoveUp(bool bScroll = true, bool bPerOnePixel = false);
	void MoveDown(bool bScroll = true, bool bPerOnePixel = false);
	std::string GetValue() const { return m_FH.GetValue(); }
	void SetValue(const std::string& strValue) { m_FH.SetValue(strValue); }

	int GetIndexX() { return m_iIndexX; }
	int GetIndexY() { return m_iIndexY; }
	int GetIndexX_2() { return m_iIndexX_2; }
	int GetIndexY_2() { return m_iIndexY_2; }

	std::string ToString();
	std::string GetPropString();

	bool operator == (const CToolObject& _Right) const
	{
		return (m_iIndexX == _Right.m_iIndexX) &&
			(m_iIndexY == _Right.m_iIndexY) &&
			(m_iIndexX_2 == _Right.m_iIndexX_2) &&
			(m_iIndexY_2 == _Right.m_iIndexY_2) &&
			(m_dwProp == _Right.m_dwProp) &&
			(m_dwType == _Right.m_dwType);
	}

	void SetEyeInfo(CRenderEyeInfo* pEyeInfo) { m_pEyeInfo = pEyeInfo; }
	void SetBlockInfo(CFHBlockInfo* pBlockInfo) { m_pBlockInfo = pBlockInfo; }

	static void PreRenderProcess();
	static void PostRenderProcess();

	bool IsIndexValid();

	void MoveGap(int iIndexX, int iIndexY, int iIndexX_2 = 0, int iIndexY_2 = 0);

private:
	CFootHold m_FH;
	FootHoldVertex	m_Vertex[5];
	FootHoldVertex	m_TexVertex[4];
	static GCDeviceTexture* m_pTexture[4];
	static GCDeviceTexture* m_pTexValue;
	static GCDeviceTexture* m_pTexDead;

	CRenderEyeInfo* m_pEyeInfo;
	CFHBlockInfo* m_pBlockInfo;

	static DWORD    m_dwAlphaEnable;
	static DWORD    m_dwZEnable;
	static DWORD    m_dwSrcBlend;
	static DWORD    m_dwDestBlend;
};

class KFindToolObject
{
public:
	KFindToolObject(int iIndexX, int iIndexY) : m_iIndexX(iIndexX), m_iIndexY(iIndexY) {}

	bool operator()(CToolObject* comp) const
	{
		return (comp->GetIndexX() == m_iIndexX) && (comp->GetIndexY() == m_iIndexY);
	}

	int m_iIndexX;
	int m_iIndexY;
};
