#pragma once
#include "thread/Thread.h"
#include "KncException.h"
#include "KncSmartPtr.h"

SmartPointer(KIOThread);

class KIOThread : public KThread
{
	_DeclareException;

public:
	KIOThread(void);
	virtual ~KIOThread(void);

protected:
	virtual void Run();
	void Loop();
};
