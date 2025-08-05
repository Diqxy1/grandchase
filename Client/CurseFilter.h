#ifndef _CURSEFILTER_H_
#define _CURSEFILTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <vector>
#include <atlstr.h>

struct SFilter
{
	CString src;
	CString dest;
};

class KCurseFilter
{
public:
	KCurseFilter();
	~KCurseFilter();
public:
	void LoadCurses(const char *filename);
	void ConvertString(WCHAR* str, UINT bufSize);
	CString StrFull2Half(CString str);
private:
	std::vector<SFilter> m_vecFilterTable;
};

extern KCurseFilter g_kCurseFilter;

#endif // _CURSEFILTER_H_
