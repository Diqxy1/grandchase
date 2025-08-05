#pragma once

#include <vector>
#include <string>


class CListBoxLog;

#include "EditStreamBuf.h"

// CListBoxLog

class CListBoxLog : public CListBox
{
	DECLARE_DYNAMIC(CListBoxLog)

public:
	CListBoxLog();
	virtual ~CListBoxLog();

	void AddMsg(char* strMsg);
	void AddMsg(wchar_t* strMsg);

	void Display();

	//void BeginRedirect();
	//void EndRedirect();

private:
	std::vector<std::wstring>	m_vecQ;
	std::vector<std::wstring>	m_vecDisplay;
	CRITICAL_SECTION	m_csLock;	

	//EditStream::editstreambuf m_EditStrBuf;
	//EditStream::weditstreambuf m_EditStrBufW;
	//EditStream::editstreambuf m_EditStrBufError;
	//EditStream::weditstreambuf m_EditStrBufErrorW;

	//std::basic_streambuf<char>*	m_pOldBuf;
	//std::basic_streambuf<wchar_t>*	m_pOldBufW;
	//std::basic_streambuf<char>*	m_pOldBufError;
	//std::basic_streambuf<wchar_t>*	m_pOldBufErrorW;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
};


