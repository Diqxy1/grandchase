#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Lua/KLuaManager.h"
#include "PrimitiveTypedef.h"
#include "UserPacket.h"

class KJobMissionGuide
{
    DeclareSingleton( KJobMissionGuide );
    NiDeclareRootRTTI( KJobMissionGuide );
    DeclToStringW;

public:
    KJobMissionGuide(void);
    ~KJobMissionGuide(void);

public:
    struct KJobMissionInfo
    {
        DWORD           m_dwGPMissionID;
        bool            m_bUseGPMission;
        DWORD           m_dwCashMissionID;
        bool            m_bUseCashMission;

        KJobMissionInfo() {
            m_dwGPMissionID = KEVENT_JOB_MISSION_GUIDE_NOT::NO_USE;
            m_bUseGPMission = false;
            m_dwCashMissionID = KEVENT_JOB_MISSION_GUIDE_NOT::NO_USE;
            m_bUseCashMission = false;
        }
    };

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    bool GetJobMissionGuide( IN const PAIR_CHAR_INT& prKey_, OUT KEVENT_JOB_MISSION_GUIDE_NOT& kPacket_ );

private:
    bool LoadMissionGuideTable( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_CHAR_INT, KJobMissionInfo >& mapJobMissionInfo_ );

protected:
    mutable KncCriticalSection                              m_csJobMissionInfo;

    bool                                                    m_bUseJobMissionGuide;
    std::map< PAIR_CHAR_INT, KJobMissionInfo >              m_mapJobMissionInfo;
};

DefSingletonInline( KJobMissionGuide );
DeclOstmOperatorA( KJobMissionGuide );


