#pragma once
#include <thread/thread.h>

class KUdpEcho : public KThread
{
public:
	KUdpEcho(void);
	virtual ~KUdpEcho(void);

	bool Init(USHORT usPort);
	virtual void Run();     // derived from KThread
	void ShutDown();

protected:
	inline void Loop();

	SOCKET      m_sock;
};
