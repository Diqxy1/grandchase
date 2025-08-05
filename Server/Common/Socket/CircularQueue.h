#pragma once

#include <vector>
#include <algorithm>

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class KCircularQueue : protected std::vector<unsigned char>
{
public:
	KCircularQueue();
	virtual ~KCircularQueue();

	using std::vector<value_type>::empty;
	using std::vector<value_type>::size;

	void Resize(IN int nSize);
	int Push(IN const char* pData, IN int nSize);
	int Pop(IN int nSize, OUT char* pBuffer);
	int GetDataSize();

protected:
	int GetFreeSize();
	int GetRearDataSize();
	int GetFrontDataSize();

protected:
	int     m_nHead;
	int     m_nTail;
};