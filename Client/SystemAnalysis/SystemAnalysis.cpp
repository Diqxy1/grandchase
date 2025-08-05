#include "StdAfx.h"
#include "SystemAnalysis.h"
#include "utf16_cvt.h"
#include "KncUtil.h"




KSystemAnalysis* g_Dlg = NULL; ///< KSystemAnalysis 객체에 대한 전역 포인터
HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName, VOID* pContext );

HRESULT GetVideoMemoryViaDxDiag( HMONITOR hMonitor, DWORD* pdwDisplayMemory );
HRESULT GetVideoMemoryViaDirectDraw( HMONITOR hMonitor, DWORD* pdwAvailableVidMem );
HRESULT GetVideoMemoryViaWMI( HMONITOR hMonitor, DWORD* pdwAdapterRam );
HRESULT GetVideoMemoryViaDXGI( HMONITOR hMonitor, SIZE_T* pDedicatedVideoMemory, SIZE_T* pDedicatedSystemMemory,
                              SIZE_T* pSharedSystemMemory );
HRESULT GetVideoMemoryViaD3D9( HMONITOR hMonitor, UINT* pdwAvailableTextureMem );
HRESULT GetSoundDeviceName( std::wstring& strName );

HRESULT GetHMonitorFromD3D9Device( IDirect3DDevice9* pd3dDevice, HMONITOR hMonitor );

KSystemAnalysis::KSystemAnalysis()
{
    g_Dlg = this;
}
KSystemAnalysis::~KSystemAnalysis()
{
    g_Dlg = NULL;  // 전역 포인터 초기화 
}

bool KSystemAnalysis::StoreMySystemAnalysis()
{
    std::wstring allSystemData;

    allSystemData = L"=============================================================================";
    allSystemData += L"\r\n                  나의 컴퓨터 시스템 정보 \r\n";
    allSystemData +=L"=============================================================================";
    allSystemData += L"\r\n\r\n컴퓨터 구성 \t         내 컴퓨터 사양";
    allSystemData += L"\r\n\r\nCPU                      ";
    allSystemData += GetSystemCpuData().c_str();
    allSystemData += GetSystemCpuCoreData().c_str();
    allSystemData += L"\r\n\r\nOS                       ";
    allSystemData += GetSystemOsData().c_str();
    allSystemData += L"\r\n\r\n남은 하드 공간           ";
    allSystemData += GetSystemFreeHardCapacity().c_str();
    allSystemData += L"\r\n\r\nRAM                      ";
    allSystemData += GetSystemRamData().c_str();
    allSystemData += L"\r\n\r\n그래픽카드               ";
    allSystemData += GetSystemGraphicCardData().c_str();
    allSystemData += GetSystemGraphicMemData().c_str();
    allSystemData += L"\r\n\r\n드라이버 버젼            ";
    allSystemData += GetSystemDriverVersion().c_str();
    allSystemData += L"\r\n\r\nDirect X 버전            ";
    allSystemData += GetSystemDirectXVersion().c_str();
    allSystemData += L"\r\n\r\n사운드 드라이버          ";
    allSystemData += GetSystemSoundDataWMI().c_str();
    allSystemData += L"\r\n\r\n";
    allSystemData += L"\0";
    
	std::wofstream wFile;
	IMBUE_NULL_CODECVT( wFile );
	wFile.open( L"HardwareInfo.txt", std::ios::binary | std::ios::out | std::ios::trunc );
	if( !wFile.is_open() )
		return false;
	wchar_t cBom = 0xfeff;
	wFile.write( &cBom, 1 );
	wFile << allSystemData;
	wFile.close();
    
    return true;
}

std::wstring KSystemAnalysis::GetMySystemAnalysis()
{
    std::wstring allSystemData;

    allSystemData = L"=============================================================================";
    allSystemData += L"\r\n                  나의 컴퓨터 시스템 정보 \r\n";
    allSystemData +=L"=============================================================================";
    allSystemData += L"\r\n\r\n컴퓨터 구성 \t         내 컴퓨터 사양";
    allSystemData += L"\r\n\r\nCPU                      ";
    allSystemData += GetSystemCpuData().c_str();
    allSystemData += GetSystemCpuCoreData().c_str();
    allSystemData += L"\r\n\r\nOS                       ";
    allSystemData += GetSystemOsData().c_str();
    allSystemData += L"\r\n\r\n남은 하드 공간           ";
    allSystemData += GetSystemFreeHardCapacity().c_str();
    allSystemData += L"\r\n\r\nRAM                      ";
    allSystemData += GetSystemRamData().c_str();
    allSystemData += L"\r\n\r\n그래픽카드               ";
    allSystemData += GetSystemGraphicCardData().c_str();
    allSystemData += GetSystemGraphicMemData().c_str();
    allSystemData += L"\r\n\r\n드라이버 버젼            ";
    allSystemData += GetSystemDriverVersion().c_str();
    allSystemData += L"\r\n\r\nDirect X 버전            ";
    allSystemData += GetSystemDirectXVersion().c_str();
    allSystemData += L"\r\n\r\n사운드 드라이버          ";
    allSystemData += GetSystemSoundDataWMI().c_str();
    allSystemData += L"\r\n\r\n";
    allSystemData += L"\0";

    return allSystemData;
}

std::wstring KSystemAnalysis::GetSystemCpuData()
{
    std::string strSystemCpuData;
    std::wstring wcsSystemCpuData;

    CPUInfo cpu;

    if (cpu.DoesCPUSupportCPUID())
    {
        strSystemCpuData = cpu.GetExtendedProcessorName();
    }
    else
    {
        strSystemCpuData = "ERROR! NOT FOUND CPU DATA";
    }

    wcsSystemCpuData = KncUtil::toWideString(strSystemCpuData);

    return wcsSystemCpuData;
}

std::wstring KSystemAnalysis::GetSystemCpuCoreData()
{
    std::string strSystemCpuData;
    std::wstring wcsSystemCpuData;
    
    int nCore = GetSystemCpuCore();
    int nMHZ = GetSystemCpuClock();

    strSystemCpuData = boost::str(boost::format( " ( CPU_Core : %1%, CPU_Clock : %2%MHz ) " )%nCore%nMHZ);

    wcsSystemCpuData = KncUtil::toWideString(strSystemCpuData);

    return wcsSystemCpuData;
}

int KSystemAnalysis::GetSystemCpuClock()
{
    int nMHZ = -1;

    CPUInfo cpu;

    if (cpu.DoesCPUSupportCPUID())
    {
        nMHZ = cpu.GetProcessorClockFrequency();
    }	

    return nMHZ;
}

int KSystemAnalysis::GetSystemCpuCore()
{
    int nCore = -1;

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    nCore = info.dwNumberOfProcessors;
    
    return nCore;
}

int KSystemAnalysis::GetVideoMemory()
{
    DWORD dwAdapterRAM = -1;
    DWORD dwAvailableVidMem = -1;


    IDirect3D9* pD3D9 = NULL;
    pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
    if ( !pD3D9 )
        return -1;

    D3DADAPTER_IDENTIFIER9 id;
    ZeroMemory( &id, sizeof( D3DADAPTER_IDENTIFIER9 ) );
    pD3D9->GetAdapterIdentifier( D3DADAPTER_DEFAULT , D3DENUM_WHQL_LEVEL, &id );

    HMONITOR hMonitor = pD3D9->GetAdapterMonitor( D3DADAPTER_DEFAULT );

    MONITORINFOEX mi;
    mi.cbSize = sizeof( MONITORINFOEX );
    GetMonitorInfo( hMonitor, &mi );
    
    if( SUCCEEDED( GetVideoMemoryViaWMI( hMonitor, &dwAdapterRAM ) ) )
    {
        dwAdapterRAM = dwAdapterRAM / 1024 / 1024;
        pD3D9->Release();
        
        return dwAdapterRAM;
    }

    if( SUCCEEDED( GetVideoMemoryViaDirectDraw( hMonitor, &dwAvailableVidMem ) ) )
    {
        dwAvailableVidMem = dwAdapterRAM / 1024 / 1024;
        pD3D9->Release();
        
        return dwAvailableVidMem;
    }

    pD3D9->Release();
    return -1;
}

std::wstring KSystemAnalysis::GetSystemDirectXVersion()
{
    std::wstring strSystemDirectXVersion;
    HRESULT hr;
    TCHAR strResult[128];
    
    DWORD dwDirectXVersion = 0;
    TCHAR strDirectXVersion[10];
    
    hr = GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10 );
    if( SUCCEEDED(hr) )
    {
        if( dwDirectXVersion > 0 )
            _sntprintf( strResult, 128, TEXT("DirectX %s"), strDirectXVersion );
        else
            _tcsncpy( strResult, TEXT(""), 128 );
        strResult[127] = 0;
    }
    else
    {
        _sntprintf( strResult, 128, TEXT(""), hr );
        strResult[127] = 0;
        
    }
    
    strSystemDirectXVersion = strResult;

    return strSystemDirectXVersion;
}

std::wstring KSystemAnalysis::GetSystemDriverVersion()
{
    std::wstring strDriverVersion;
    D3DADAPTER_IDENTIFIER9 identifier;
    
    IDirect3D9* pD3d;
    pD3d = Direct3DCreate9( D3D_SDK_VERSION ); 
    
    pD3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT  ,D3DENUM_WHQL_LEVEL , &identifier );
    
	strDriverVersion = KncUtil::toWideString(identifier.Driver);
    
    pD3d->Release();

    return strDriverVersion;
}

std::wstring KSystemAnalysis::GetSystemFreeHardCapacity()
{
    std::wstring strSystemFreeHardCapacity;
    ULARGE_INTEGER AvailableToCaller; 
    ULARGE_INTEGER Disk, Free;
    std::wstring strMyFreeHardCapacity;
    WCHAR HardDiskLetters[4]  = { L"c:\\" };
    int iFreeMB;
    WCHAR buffer[200];
    
    GetDiskFreeSpaceEx( HardDiskLetters, &AvailableToCaller, &Disk, &Free);

    iFreeMB = static_cast<int>(Free.QuadPart /1024/1024);
    _itow( iFreeMB, buffer, 10);

    strSystemFreeHardCapacity = buffer;
    strSystemFreeHardCapacity += L" MB";

    return strSystemFreeHardCapacity;
}

std::wstring KSystemAnalysis::GetSystemGraphicCardData()
{
    std::wstring strSystemGraphicCardData;

    D3DADAPTER_IDENTIFIER9 identifier;
    
    IDirect3D9* pD3d;
    pD3d = Direct3DCreate9( D3D_SDK_VERSION );     
    
    pD3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT  ,D3DENUM_WHQL_LEVEL , &identifier );
    pD3d->Release();

    
    strSystemGraphicCardData = KncUtil::toWideString(identifier.Description); 

    return strSystemGraphicCardData;
}

std::wstring KSystemAnalysis::GetSystemGraphicMemData()
{
    std::string strVideoMemory;
    std::wstring wstrVideoMemory;
    int nMemory = GetVideoMemory();

    strVideoMemory = boost::str(boost::format(" ( VideoMemory : %1%MB ) ")%nMemory);
    wstrVideoMemory = KncUtil::toWideString(strVideoMemory);   

    return wstrVideoMemory;
}

std::wstring KSystemAnalysis::GetSystemOsData()
{
    OSVERSIONINFOEX osVersion;
    std::wstring strSystemOsData;

    ZeroMemory( &osVersion, sizeof( OSVERSIONINFOEX ) );
    
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    if ( GetVersionEx( (OSVERSIONINFO*)&osVersion ) )
    {
        switch( osVersion.dwPlatformId )
        {
        case VER_PLATFORM_WIN32s:
            {
                strSystemOsData = L"Windows 3.1 ";
                break;
            }
        case VER_PLATFORM_WIN32_WINDOWS:
            {
                switch( osVersion.dwMinorVersion )
                {
                case 0:
                    strSystemOsData = L"windows 95 ";
                    break;
                case 10:
                    strSystemOsData = L"windows 98 ";
                    break;
                case 90:
                    strSystemOsData = L"Windows Me ";
                default:
                    strSystemOsData = L"Windows ?? ";
                    break;
                }
                break;
            }
        case VER_PLATFORM_WIN32_NT:
            {
                switch( osVersion.dwMajorVersion )
                {
                case 3:
                    strSystemOsData = L"Windows  NT 3.51 ";
                    break;
                case 4:
                    strSystemOsData = L"Windows NT 4.0 ";
                    break;
                case 5:
                
					switch( osVersion.dwMinorVersion)
                    {
                    case 0:
                        strSystemOsData = L"Windows 2000 ";
                        break;
                    case 1:
                        strSystemOsData = L"Windows XP ";
                        break;
                    case 2:
                        strSystemOsData = L"Windows Server 2003 family ";
                        break;
                    default:
                        strSystemOsData = L"Windows NT 계열 ";
                        break;
                    }
                    break;
				case 6:
					{
                        if ( osVersion.wProductType == VER_NT_WORKSTATION ) 
                        {

						    switch( osVersion.dwMinorVersion)
						    {
						    case 0:
							    strSystemOsData = L"Windows Vista ";
							    break;
						    case 1:
							    strSystemOsData = L"Windows 7 ";
							    break;
                            case 2:
                                strSystemOsData = L"Windows 8 ";
                                break;
						    default:
							    strSystemOsData = L"Unknow OS ";
							    break;
						    }
                        }
                        else
                        {
                            switch( osVersion.dwMinorVersion)
                            {
                            case 0:
                                strSystemOsData = L"Windows Server 2008 ";
                                break;
                            case 1:
                                strSystemOsData = L"Windows Server 2008 R2 ";
                                break;
                            case 2:
                                strSystemOsData = L"Windows Server 2012";
                                break;
                            default:
                                strSystemOsData = L"Unknow OS ";
                                break;
                            }
                        }
					}
					break;
				default:
					strSystemOsData = L"Windows NT 계열 ";
                    break;
                }
                break;
            }
        default:
            {
				WCHAR buffer[100];				
                strSystemOsData = L"OS를 찾울수 없습니다.";
				wsprintf( buffer, L"%s (%d)", strSystemOsData.c_str() , (int)osVersion.dwPlatformId );
				strSystemOsData = buffer;
                break;
            }
        } // end switch
        
        strSystemOsData += osVersion.szCSDVersion;
        strSystemOsData += L"(Build ";
        
        std::wstring buildNumber;
        WCHAR buffer[100];
        _itow( osVersion.dwBuildNumber, buffer, 10 );
        buildNumber = buffer;
        
        strSystemOsData += buildNumber;
        strSystemOsData += L")";
    } // end if 
    else
        strSystemOsData = L"OS정보를 읽어올수 없습니다.";
    
   return strSystemOsData;
}

std::wstring KSystemAnalysis::GetSystemRamData()
{
    WCHAR buffer[200];
    std::wstring strSystemRamData;
    MEMORYSTATUS memoryStatus;
    memoryStatus.dwLength = sizeof (MEMORYSTATUS);
    GlobalMemoryStatus(&memoryStatus);
    
    double tempTotalMemory = memoryStatus.dwTotalPhys;
    
    tempTotalMemory /= 1024; //byte를 KB로 
    tempTotalMemory /= 1024; //KB를 MB로
    
    int totalMemory = (int )( tempTotalMemory + 0.999 ); // 소숫점이하는  올림으로 

    _itow( totalMemory, buffer, 10 );

    strSystemRamData = buffer;
    strSystemRamData += L" MB";
        
    return strSystemRamData;
}

int KSystemAnalysis::GetSystemRam()
{
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof (MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memoryStatus);

	double tempTotalMemory = static_cast<double>( memoryStatus.ullTotalPhys / ( 1024 * 1024 ) );

	int totalMemory = (int )( tempTotalMemory + 0.999 ); // 소숫점이하는  올림으로 
	return totalMemory;
}
std::wstring KSystemAnalysis::GetSystemSoundData()
{
    HRESULT hr;
    
    HWND hSoundDeviceCombo = NULL;
    
    if( FAILED( hr = DirectSoundEnumerate( (LPDSENUMCALLBACK)DSoundEnumCallback, //api함수 사용한것 
        (VOID*)hSoundDeviceCombo ) ) )
    {
        m_strSystemSoundData = L"사운드를 찾을수 없습니다. ";
    }

    return m_strSystemSoundData;
}

std::wstring KSystemAnalysis::GetSystemSoundDataWMI()
{
    std::wstring strSoundName;

    if( SUCCEEDED( GetSoundDeviceName( strSoundName ) ) )
    {
        return strSoundName;
    }

    return L"";
}

/// 사운드카드의 정보를 호출하는 함수이다. 이함수가 1번불리면 "주사운드카드"글자가 strDesc에 저장 
/// 2번재 불릴때는 strDesc에 내 컴퓨터의 사운드 카드 정보가 저장된다. 
/// @param pGUID  뭐하는것인지 잘 모른다. 
/// @param strDesc 사운드카드의 정보를 가진다.
/// @param strDrvName 뭐하는것인지 잘 모른다. 
/// @param strDrvname 뭐하는 것인지 잘 모른다.
/// @return void
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName, VOID* pContext )
{
    // Set aside static storage space for 20 audio drivers
    static int nCounter =0;
    static GUID  AudioDriverGUIDs[20];
    static DWORD dwAudioDriverIndex = 0;
    
    GUID* pTemp  = NULL;
    
    if( pGUID )
    {
        if( dwAudioDriverIndex >= 20 )
            return TRUE;
        
        pTemp = &AudioDriverGUIDs[dwAudioDriverIndex++];
        memcpy( pTemp, pGUID, sizeof(GUID) );
    }
    
    HWND hSoundDeviceCombo = (HWND)pContext;
    
    if ( nCounter == 1)
    {
        g_Dlg->m_strSystemSoundData = KncUtil::toWideString(strDesc);
    }
    
    nCounter++;
    return TRUE;
}

