#pragma once

#include "IGCComputePaletteP3M.h"
#include "../GCUTIL/KJohnGen.h"
#include "IGCBone.h"
#include "GCBaseMesh.h"

namespace
{
	struct ONE_TRIANGLE
	{
		WORD w[3];
	};

	template<class ArgumentType, class ResultType>
	struct unary_function
	{
		typedef ArgumentType argument_type;
		typedef ResultType result_type;
	};

	class RemoveFunctor : public unary_function< ONE_TRIANGLE, bool >
	{
	public:
		RemoveFunctor( std::set< DWORD >& setIgIdx ) : m_setIgIdx( setIgIdx )
		{}
		~RemoveFunctor() {}

		bool operator()(const ONE_TRIANGLE& _Left) const
		{
			return m_setIgIdx.end() != m_setIgIdx.find( _Left.w[0] ) &&
				m_setIgIdx.end() != m_setIgIdx.find( _Left.w[1] ) &&
				m_setIgIdx.end() != m_setIgIdx.find( _Left.w[2] );
		}

		std::set< DWORD >& m_setIgIdx;
	};
};

class GCDeviceMeshP3M :  public GCDevice, public GCBaseMesh
{
public:
	GCDeviceMeshP3M(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, char cIgBoneIdx = -1 );
	virtual ~GCDeviceMeshP3M(void);

	void OnResetDevice() {};
	void OnLostDevice() {};

	virtual void Render();
	virtual void Release(){GCDevice::Release();};

protected:

	//Interface Device
	virtual bool Load();
	virtual void UnLoad();
	virtual DWORD GetDeviceSize();
protected:
	std::string m_strFileName;
	char m_cIgBoneIdx;
};
