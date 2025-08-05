#include "StdAfx.h"
#include <wbemidl.h>
//#include <multimon.h>


HRESULT GetDeviceIDFromHMonitor( HMONITOR hm, WCHAR* strDeviceID, int cchDeviceID ); // from vidmemviaddraw.cpp
typedef BOOL ( WINAPI* PfnCoSetProxyBlanket )( IUnknown* pProxy, DWORD dwAuthnSvc, DWORD dwAuthzSvc,
                                              OLECHAR* pServerPrincName, DWORD dwAuthnLevel, DWORD dwImpLevel,
                                              RPC_AUTH_IDENTITY_HANDLE pAuthInfo, DWORD dwCapabilities );

HRESULT GetSoundDeviceName( std::wstring& strName )
{
    HRESULT hr;
    bool bFind = false;
    HRESULT hrCoInitialize = S_OK;
    IWbemLocator* pIWbemLocator = NULL;
    IWbemServices* pIWbemServices = NULL;
    BSTR pNamespace = NULL;

    strName = L"";
    hrCoInitialize = CoInitialize( 0 );

    hr = CoCreateInstance( CLSID_WbemLocator,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        ( LPVOID* )&pIWbemLocator );

    if( SUCCEEDED( hr ) && pIWbemLocator )
    {
        // Using the locator, connect to WMI in the given namespace.
        pNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );

        hr = pIWbemLocator->ConnectServer( pNamespace, NULL, NULL, 0L,
            0L, NULL, NULL, &pIWbemServices );

        if( SUCCEEDED( hr ) && pIWbemServices != NULL )
        {
            HINSTANCE hinstOle32 = NULL;

            hinstOle32 = LoadLibraryW( L"ole32.dll" );
            if( hinstOle32 )
            {
                PfnCoSetProxyBlanket pfnCoSetProxyBlanket = NULL;

                pfnCoSetProxyBlanket = ( PfnCoSetProxyBlanket )GetProcAddress( hinstOle32, "CoSetProxyBlanket" );
                if( pfnCoSetProxyBlanket != NULL )
                {
                    // Switch security level to IMPERSONATE. 
                    pfnCoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );
                }

                FreeLibrary( hinstOle32 );
            }

            IEnumWbemClassObject* pEnumSoundControllers = NULL;
            BSTR pClassName = NULL;

            pClassName = SysAllocString( L"Win32_SoundDevice" );

            hr = pIWbemServices->CreateInstanceEnum( pClassName, 0,
                NULL, &pEnumSoundControllers );

            if( SUCCEEDED( hr ) && pEnumSoundControllers )
            {
                IWbemClassObject* pSoundControllers[10] = {0};
                DWORD uReturned = 0;
                BSTR pPropName = NULL;

                // Get the first one in the list
                pEnumSoundControllers->Reset();
                hr = pEnumSoundControllers->Next( 5000,             // timeout in 5 seconds
                    10,                  // return the first 10
                    pSoundControllers,
                    &uReturned );

                VARIANT var;
                if( SUCCEEDED( hr ) )
                {
                    for( UINT iController = 0; iController < uReturned; iController++ )
                    {
                        pPropName = SysAllocString( L"PNPDeviceID" );
                        hr = pSoundControllers[iController]->Get( pPropName, 0L, &var, NULL, NULL );

                        VariantClear( &var );
                        if( pPropName ) SysFreeString( pPropName );


                        pPropName = SysAllocString( L"Name" );
                        hr = pSoundControllers[iController]->Get( pPropName, 0L, &var, NULL, NULL );

                        if( SUCCEEDED( hr ) )
                        {
                            bFind = true;
                            strName = var.bstrVal;
                        }

                        VariantClear( &var );
                        if( pPropName ) 
                            SysFreeString( pPropName );

                        SAFE_RELEASE( pSoundControllers[iController] );

                        if ( bFind ) 
                            break;
                    }
                }
            }

            if( pClassName )
                SysFreeString( pClassName );
            SAFE_RELEASE( pEnumSoundControllers );
        }

        if( pNamespace )
            SysFreeString( pNamespace );
        SAFE_RELEASE( pIWbemServices );
    }

    SAFE_RELEASE( pIWbemLocator );

    if( SUCCEEDED( hrCoInitialize ) )
        CoUninitialize();

    if( bFind )
        return S_OK;
    else
        return E_FAIL;
}

