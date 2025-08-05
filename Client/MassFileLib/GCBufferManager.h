#pragma once

class GCBufferManager  
{
public:
	GCBufferManager();
	virtual ~GCBufferManager();
	char* GetBuffer(int iSize);
	int   GetBufferSize() { return m_iMemorySize; }

private:
	int m_iMemorySize;
	char* m_pData;
};