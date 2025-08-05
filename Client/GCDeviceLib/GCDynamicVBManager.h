#pragma once

#define D3DFVF_XYZ_DIFFUSE          ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define D3DFVF_XYZ_DIFFUSE_TEX1     ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define D3DFVF_XYZ_TEX1             ( D3DFVF_XYZ | D3DFVF_TEX1 )
#define D3DFVF_XYZRHW_DIFFUSE_TEX1  ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#include <boost/array.hpp>


class KGCDynamicVBManager
{
public:

	enum EDYNAMIC_VB_MODE
	{
		DYNAMIC_VB_MODE_MANAGED,
		DYNAMIC_VB_MODE_PRIMITIVEUP,
		DYNAMIC_VB_MODE_DYNAMICVB,
		DYNAMIC_VB_MODE_INDEXED_DYNAMICVB,
		DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH,
		DYNAMIC_VB_MODE_INDEXED_DYNAMICVB_WITH_TRY_CATCH,
		DYNAMIC_VB_MODE_NUMS
	}; //enum    EDynamicVBOptMode

	enum EDYNAMIC_VB_TYPE
	{
		DVB_TYPE_XYZ_DIFFUSE,
		DVB_TYPE_XYZ_DIFFUSE_TEX1,
		DVB_TYPE_XYZ_TEX1,
		DVB_TYPE_XYZRHW_DIFFUSE_TEX1,
		DVB_END
	}; //enum EDynamicVBType

public:

	KGCDynamicVBManager();
	virtual ~KGCDynamicVBManager();

private:

	void CustomInitialize(bool bInConstructor_);
	virtual void CustomFinalize(bool bInDestructor_);

public:

	virtual HRESULT OnFrameMove(float fElapsedTime);

	virtual void OnResetDevice();
	virtual void OnLostDevice();

	bool RegisterVBType(EDYNAMIC_VB_TYPE eType, DWORD dwFVF, unsigned uNumVertices, bool bGenVB = false);

	void DrawPrimitive(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumPrimitives,
		const void* pData, unsigned uNumDraws = 1) throw()
	{
		if (m_pDrawPrimitiveFn != NULL)
			(this->*m_pDrawPrimitiveFn)(eType, d3dPrimitiveType, uNumPrimitives, pData, uNumDraws);
	} //KGCDynamicVBManager::DrawPrimitive()


	static void GetOSVersion(DWORD& dwOutMajor, DWORD& dwOutMinor);

	bool SetDynamicVBMode(EDYNAMIC_VB_MODE eDynamicVBMode);
	EDYNAMIC_VB_MODE GetDynamicVBMode() const { return m_eDynamicVBMode; }

	void ResetNumExceptionsOccurred() { m_uNumExceptionsOccurred = 0; }

private:

	void _DrawPrimitiveManaged(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumPrimitives,
		const void* pData, unsigned uNumDraws) throw();
	void _DrawPrimitiveUP(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumPrimitives,
		const void* pData, unsigned uNumDraws) throw();
	void _DrawPrimitiveDynamicVB(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumPrimitives,
		const void* pData, unsigned uNumDraws) throw();
	void _DrawPrimitiveDynamicVB_Quad(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType, unsigned uNumQuads,
		const void* pData, unsigned uNumDraws) throw();
#ifdef KTDGDEVICEFONT_SIZE_CACHE
	void _DrawPrimitiveDynamicVBWithTryCatch(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
		unsigned uNumPrimitives, const void* pData, unsigned uNumDraws) throw();
	void _DrawPrimitiveDynamicVBWithTryCatch_Quad(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
		unsigned uNumQuads, const void* pData, unsigned uNumDraws) throw();
#endif

private:

	struct DynamicVBInfo
	{
		unsigned m_uNumVertices; /// 정점 개수
		DWORD m_dwFVF; /// 정점 정보
		unsigned m_dwVertexStride; /// 스트림에 연결하고자하는 버텍스 버퍼 내 각 요소의 바이트 수
		unsigned m_uCurVertexIndex; /// 현재 정점 인덱스
		LPDIRECT3DVERTEXBUFFER9
			m_pVB; /// DX 버텍스 버퍼

		DynamicVBInfo()
			: m_uNumVertices(0)
			, m_dwFVF(0)
			, m_dwVertexStride(0)
			, m_uCurVertexIndex(0)
			, m_pVB(nullptr)
		{
		}

		~DynamicVBInfo();

		bool IsRegistered() const { return m_uNumVertices != 0; }

		void Reset()
		{
			m_uNumVertices = 0;
			m_dwFVF = 0;
			m_dwVertexStride = 0;
			m_uCurVertexIndex = 0;
			SAFE_RELEASE(m_pVB);
		}

		void OnResetDevice(EDYNAMIC_VB_MODE eMode);
		void OnLostDevice(EDYNAMIC_VB_MODE eMode);
	}; //struct  DynamicVBInfo

	boost::array<DynamicVBInfo, DVB_END>
		m_aDynamicVBInfo;

	LPDIRECT3DINDEXBUFFER9 m_pQuadIndices; // 인덱스버퍼

	unsigned m_uNumMaxPrimitiveCount;
	EDYNAMIC_VB_MODE m_eDynamicVBMode;
	unsigned m_uNumExceptionsOccurred;

	void (KGCDynamicVBManager::* m_pDrawPrimitiveFn)(EDYNAMIC_VB_TYPE eType, D3DPRIMITIVETYPE d3dPrimitiveType,
		unsigned uNumPrimitives, const void* pData,
		unsigned uNumDraws) throw();


private:
	HRESULT m_hCheckLostDevice;

public:
	void SetDeviceLost(HRESULT hr_) { m_hCheckLostDevice = hr_; }
}; //class   KGCDynamicVBManager
