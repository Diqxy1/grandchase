#pragma     once

#include "FSM/FSMclass.h"

class KRoomFSM : public FSMclass
{
public:
    #undef _ENUM
    #define _ENUM( id ) id,
    enum ENUM_STATE
    {
#       include "RoomFSMState_def.h"
    };
    enum ENUM_INPUT
    {
#       include "RoomFSMInput_def.h"
    };

    KRoomFSM();
    virtual ~KRoomFSM();

    virtual const wchar_t* GetStateIDString(int iStateID) const;
    virtual const wchar_t* GetInputIDString(int iInputID) const;

protected:    
    static const wchar_t*  ms_szState[];
    static const wchar_t*  ms_szInput[];
};