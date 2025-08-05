#include "stdafx.h"

GCDevice::GCDevice(void)
{
	m_iRefCount = 0;
	m_iLoadCount = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
	m_eDeviceType = GCDT_NULL;
}

GCDevice::~GCDevice(void)
{
}

void GCDevice::Release()
{
	if (g_pGCDeviceManager)
	{
		g_pGCDeviceManager->ReleaseDevice( this );
	}
}

void GCDevice::AddRefCount()
{
	//아무런 참조도 없었다면 리소스가 로딩되지 않았으므로 로딩한다
	if( m_iRefCount <= 0 )
	{
		m_iRefCount = 0;

		m_iLoadCount++;
		Load();
	}
	++m_iRefCount;
}
void GCDevice::SetDeviceID( std::string& strDeviceID_ )
{
	m_strDeviceID = strDeviceID_;
}

void GCDevice::Reload()
{
	if( m_iRefCount <= 0 )
	{
		m_iRefCount = 0;
		Load();
	}
	else
	{
		UnLoad();
		Load();
	}
}
