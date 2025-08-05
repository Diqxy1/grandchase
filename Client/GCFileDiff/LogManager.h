#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "KAssert.h"

#pragma once

#define LOG_TYPE                        8
#define LOG_SAMEFILE                    0
#define LOG_SAMEFILE_MKOM               1
#define LOG_ADDFILE                     2
#define LOG_ADDFILE_MKOM                3
#define LOG_DELETEFILE                  4
#define LOG_DELETEFILE_MKOM             5
#define LOG_UPDATEFILE                  6
#define LOG_UPDATEFILE_MKOM             7


class LogManager
{
public:
    LogManager(void);
    ~LogManager(void);
    std::vector<std::string> m_vecLogData[LOG_TYPE];
    void PushLogData(int n_type,std::string strDesc);
    void ResetLogData();
    void PrintLogDate(std::fstream *out);
    //int m_iPatchSizeKom,m_iPatchSizeMKom;
};
