#include "stdafx.h"
#include "FindSpecificationFilters.h"

HRESULT ProgressConnectionMP3(IGraphBuilder* pIGraphBuilder)
{
	if( pIGraphBuilder == NULL )
	{
		return VFW_E_NOT_FOUND;
	}
    static WCHAR* szFilterName[] =
    {
        L"MPEG-I Stream Splitter",
        L"MPEG Layer-3 Decoder",
        L"Default DirectSound Device" //마지막에 추가할 필터는 항상 AudioRenderer
    };

    HRESULT hr = S_OK;
    IBaseFilter* pIBaseFilter = NULL;
    for(int i = 0; i < 2; i++)
    {
        hr = FindSpecificationDirectShowFilters(&pIBaseFilter, szFilterName[i]);
        if(SUCCEEDED(hr))
        {
            hr = pIGraphBuilder->AddFilter(pIBaseFilter, szFilterName[i]);
        }
        else
        {
            return VFW_E_NOT_FOUND;
        }
    }
    hr = FindSpecificationAudioRendererFilters(&pIBaseFilter, szFilterName[2]);
    if(SUCCEEDED(hr))
    {
        hr = pIGraphBuilder->AddFilter(pIBaseFilter, szFilterName[2]);
    }
    else
    {
        return VFW_E_NOT_FOUND;
    }

    return hr;
}


HRESULT ProgressConnectionWMV(IGraphBuilder* pIGraphBuilder)
{
    static WCHAR* szFilterName[] =
    {
        L"WMAudio Decoder DMO",
        L"WMVideo Decoder DMO",
        L"Default DirectSound Device"
    };

    HRESULT hr = S_OK;
    IBaseFilter* pIBaseFilter = NULL;
    for(int i = 0; i < 2; i++)
    {
        hr = FindSpecificationDirectShowFilters(&pIBaseFilter, szFilterName[i]);
        if(SUCCEEDED(hr))
        {
            hr = pIGraphBuilder->AddFilter(pIBaseFilter, szFilterName[i]);
        }
        else
        {
            return VFW_E_NOT_FOUND;
        }
    }
    hr = FindSpecificationAudioRendererFilters(&pIBaseFilter, szFilterName[2]);
    if(SUCCEEDED(hr))
    {
        hr = pIGraphBuilder->AddFilter(pIBaseFilter, szFilterName[2]);
    }
    else
    {
        return VFW_E_NOT_FOUND;
    }

    return hr;
}


HRESULT FindSpecificationDirectShowFilters(IBaseFilter** gottaFilter, WCHAR* matchName)
{
    return FindSpecificationFilters(CLSID_LegacyAmFilterCategory, gottaFilter, matchName);
}


HRESULT FindSpecificationAudioRendererFilters(IBaseFilter** gottaFilter, WCHAR* matchName)
{
    return FindSpecificationFilters(CLSID_AudioRendererCategory, gottaFilter, matchName);
}


HRESULT FindSpecificationFilters(REFCLSID clsidDeviceClass, IBaseFilter** gottaFilter, WCHAR* matchName)
{
    BOOL done = FALSE;

    ICreateDevEnum *pSysDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
        IID_ICreateDevEnum, reinterpret_cast<void **>(&pSysDevEnum));
    if (FAILED(hr)) return hr;

    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumCat, 0);
//    hr = pSysDevEnum->CreateClassEnumerator(CLSID_LegacyAmFilterCategory, &pEnumCat, 0);

    if (hr == S_OK) 
    {
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
        {
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                reinterpret_cast<void **>(&pPropBag));
            if (SUCCEEDED(hr))
            {
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    if (wcsncmp(varName.bstrVal, matchName, wcslen(matchName)) == 0)
                    {
                        hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
                            reinterpret_cast<void**>(gottaFilter));
                        done = TRUE;
                    }
                }
                VariantClear(&varName);	
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();
    if (done)   return hr;
    else    return VFW_E_NOT_FOUND;
}