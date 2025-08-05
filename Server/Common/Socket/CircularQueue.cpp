#include "CircularQueue.h"

KCircularQueue::KCircularQueue() : m_nHead(0), m_nTail(0)
{
}

KCircularQueue::~KCircularQueue()
{
}

void KCircularQueue::Resize(IN int nSize)
{
	resize(nSize);
	m_nHead = 0;
	m_nTail = 0;
}

int KCircularQueue::Push(IN const char* pData, IN int nSize)
{
	if (empty()) return 0;

	nSize = std::min<int>(nSize, GetFreeSize());

	if ((int)size() - m_nTail <= nSize)  // queue의 처음으로 다시 돌아가야 하는 경우
	{
		int nRearSize = size() - m_nTail;

		// 앞으로 돌아가기 전까지 데이터 복사.
		::memcpy(&at(m_nTail), pData, nRearSize);
		// 앞으로 돌아간 후의 데이터 복사.
		::memcpy(&(*begin()), pData + nRearSize, nSize - nRearSize);

		m_nTail = nSize - nRearSize;
	}
	else    // 간단히 메모리 복사.
	{
		::memcpy(&at(m_nTail), pData, nSize);
		m_nTail += nSize;
	}

	return nSize;
}

int KCircularQueue::Pop(IN int nSize, OUT char* pBuffer)
{
	if (empty()) return 0;

	int nRDSize = std::min<int>(nSize, GetRearDataSize());
	int nFDSize = std::min<int>(nSize - nRDSize, GetFrontDataSize());

	// rear data 영역에서 pop.
	::memcpy(pBuffer, &at(m_nHead), nRDSize);

	m_nHead += nRDSize;
	if (m_nHead == size())
		m_nHead = 0;

	if (nRDSize < nSize)
	{
		//front data 영역에서 추가로 pop.
		::memcpy(pBuffer + nRDSize, &at(m_nHead), nFDSize);
		m_nHead += nFDSize;
	}

	return nRDSize + nFDSize;
}

int KCircularQueue::GetDataSize()
{
	if (empty()) return 0;

	return (m_nTail - m_nHead + size()) % size();
}

int KCircularQueue::GetFreeSize()
{
	if (empty()) return 0;

	// circular-queue에서는 원래 마지막 한칸을 사용할 수 없다.
	return (m_nHead - m_nTail + size() - 1) % size();
}

int KCircularQueue::GetRearDataSize()
{
	if (m_nHead <= m_nTail)
		return m_nTail - m_nHead;

	return size() - m_nHead;
}

int KCircularQueue::GetFrontDataSize()
{
	if (m_nHead <= m_nTail)
		return 0;

	return m_nTail;
}