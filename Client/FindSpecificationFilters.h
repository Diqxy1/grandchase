#ifndef _FINDSPECIFICATIONFILTERS_H_
#define _FINDSPECIFICATIONFILTERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <dshow.h>

HRESULT ProgressConnectionMP3(IGraphBuilder* pIGraphBuilder);

HRESULT ProgressConnectionWMV(IGraphBuilder* pIGraphBuilder);

HRESULT FindSpecificationDirectShowFilters(IBaseFilter** gottaFilter, WCHAR* matchName);

HRESULT FindSpecificationAudioRendererFilters(IBaseFilter** gottaFilter, WCHAR* matchName);

HRESULT FindSpecificationFilters(REFCLSID clsidDeviceClass, IBaseFilter** gottaFilter, WCHAR* matchName);

#endif // _FINDSPECIFICATIONFILTERS_H_