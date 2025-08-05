#include "MassFileLib/GCMassFile.h"
#include "LogManager.h"

#pragma once

class GenerateMKom
{
public:
    GenerateMKom(void);
    ~GenerateMKom(void);
    void SetMKomPath(std::string str);
    bool AddNewFile(std::string dir,std::string file, LogManager *logmanager );
    bool UpdateFile(std::string dirOld,std::string dirNew, std::string file, LogManager *logmanager );
    bool m_bMakeMKOM;

private:
    std::string m_strMKomPath;
    bool MakeDir(std::string dir);
};
