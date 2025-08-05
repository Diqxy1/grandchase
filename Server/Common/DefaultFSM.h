#pragma     once

#include "FSM/FSMclass.h"

SmartPointer(KCenterFSM);

class KDefaultFSM : public FSMclass
{
public:
#undef _ENUM
#define _ENUM( id ) id,
	enum ENUM_STATE
	{
#       include "DefaultFSMState_def.h"
	};
	enum ENUM_INPUT
	{
#       include "DefaultFSMInput_def.h"
	};

	KDefaultFSM();
	virtual ~KDefaultFSM();

	virtual const wchar_t* GetStateIDString(int iStateID) const;
	virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:
	static const wchar_t*  ms_szState[];
	static const wchar_t*  ms_szInput[];
};