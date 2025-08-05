#include ".\logmanager.h"

LogManager::LogManager(void)
{
}

LogManager::~LogManager(void)
{
}


void LogManager::ResetLogData()
{
    int i;    
    for (i=0;i < LOG_TYPE;i++)
    {
        m_vecLogData[i].clear();
    }
    //m_iPatchSizeKom=0;
    //m_iPatchSizeMKom=0;
}

void LogManager::PushLogData(int n_type, std::string strDesc)
{
    m_vecLogData[n_type].push_back(strDesc);
}

void LogManager::PrintLogDate(std::fstream *out)
{
    int i,j;
    for (i=0;i<LOG_TYPE;i++)
        for (j=0;j<(int)m_vecLogData[i].size();j++)
        {
            *out << m_vecLogData[i][j] << std::endl;
        }      
        return;
}