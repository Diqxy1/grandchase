#pragma once
//#include <vector>
#include "KNCSingleton.h"
#include "../DumpUploader/IniReader.h"

class KGCMissionToolSetting
{
    DeclareSingleton(KGCMissionToolSetting);
public:
    KGCMissionToolSetting(void);
    ~KGCMissionToolSetting(void);

    void GetMissionKind( std::vector< std::string >& vecMissionKind );
    void GetCharacters( std::vector< std::string >& vecCharacters );
};

DefSingletonInline(KGCMissionToolSetting);