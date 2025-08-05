#pragma once
#include <KNCSingleton.h>

#include "DBLayer.h"
#include "ToString.h"
#include "AgentPacket.h"

struct lua_State;
class KODBC;
class KAGDBLayer : public KDBLayer
{
	DeclareSingleton(KAGDBLayer);
	DeclToStringW;
	NiDeclareRTTI;

protected:
	KAGDBLayer(void);
public:
	virtual ~KAGDBLayer(void);
	virtual void RegToLua(std::shared_ptr<lua_State> sptlua);
	void Init_ODBC(const char* szMainODBC, const char* szStatODBC);

protected:
	// util function
	virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
};

DeclOstmOperatorA(KAGDBLayer);
DefSingletonInline(KAGDBLayer);