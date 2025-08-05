#pragma once

#include "ThreadManager.h"
#include "ToString.h"

class KODBC;
struct lua_State;
class KDBLayer : public KThreadManager
{
	DeclToStringW;
	NiDeclareRTTI;

protected:
	KDBLayer(void);
public:
	virtual ~KDBLayer(void);

	virtual void RegToLua(std::shared_ptr<lua_State> sptlua);

	// 이전 ODBCManager의 역할도 담당한다.
	bool AddODBC(const char* szName, const char* szFileDSN);
	void RemoveODBC(const char* szName);
	KODBC* GetODBC(const char* szName);

protected:

	std::map<std::wstring, std::shared_ptr<KODBC> >   m_mapODBC;
	KncCriticalSection                                  m_csMapODBC;
};

template< typename T >
class KTDBLayer : public KDBLayer
{
	NiDeclareRTTI;
	DeclToStringW;

protected:
	KTDBLayer(void) {}
public:
	virtual ~KTDBLayer(void) {}

	static KTDBLayer<T>* GetInstance()
	{
		if (ms_selfInstance == NULL)
			ms_selfInstance = new KTDBLayer<T>;
		return ms_selfInstance;
	}
	static void ReleaseInstance()
	{
		if (ms_selfInstance != NULL)
		{
			delete ms_selfInstance;
			ms_selfInstance = NULL;
		}
	}

protected:
	// util function
	virtual std::shared_ptr<KThread> CreateThread()
	{
		std::shared_ptr<T> spThread(new T);
		spThread->SetThreadManager(this);
		return spThread;
	}

	static KTDBLayer<T>*   ms_selfInstance;
};

template < typename T > KTDBLayer<T>* KTDBLayer<T>::ms_selfInstance = NULL;
template < typename T > NiImplementRTTI(KTDBLayer<T>, KTDBLayer);
template < typename T > ImplOstmOperatorW2A(KTDBLayer<T>);
template < typename T > ImplToStringW(KTDBLayer<T>)
{
	return START_TOSTRING_PARENTW(KDBLayer);
}
