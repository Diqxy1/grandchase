#include "Actor.h"
#include "FSM/FSMclass.h"
#include "dbg/dbg.hpp"

NiImplementRTTI(KActor, KSession);

KActor::KActor() : KSession(false)
{
}

KActor::~KActor()
{
}

ImplToStringW(KActor)
{
	return START_TOSTRING_PARENTW(KSession)
		<< TOSTRINGW(GetStateIDString());
}