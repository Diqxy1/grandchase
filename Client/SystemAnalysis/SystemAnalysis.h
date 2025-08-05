#pragma once 
#include <dxdiag.h>
#pragma comment( lib, "d3d9" )
#pragma comment( lib, "dsound" )
#pragma comment( lib, "dxguid" )


class KSystemAnalysis
{
public:
    KSystemAnalysis();
    ~KSystemAnalysis( void );
    bool StoreMySystemAnalysis();
    std::wstring GetMySystemAnalysis();

    std::wstring GetSystemCpuData();
    std::wstring GetSystemCpuCoreData();
    std::wstring GetSystemDirectXVersion();
    std::wstring GetSystemDriverVersion();
    std::wstring GetSystemFreeHardCapacity();
    
    std::wstring GetSystemGraphicCardData();
    std::wstring GetSystemGraphicMemData();

    std::wstring GetSystemOsData();
    std::wstring GetSystemRamData();
    std::wstring GetSystemSoundData();
    std::wstring GetSystemSoundDataWMI();


    int GetSystemRam();
    int GetSystemCpuCore();
    int GetSystemCpuClock();
    int GetVideoMemory();

public:
    //사운드정보는 callback함수로 부르기때문에 전역 변수가 필요하다.
    std::wstring m_strSystemSoundData;
};

