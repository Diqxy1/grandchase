// QuestScripter.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//
#pragma once
#include "stdafx.h"

#ifndef __AFXWIN_H__
#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호

// CQuestScripterApp:
// 이 클래스의 구현에 대해서는 QuestScripter.cpp을 참조하십시오.
//

class CQuestScripterApp : public CWinApp
{
public:
	CQuestScripterApp();

	// 재정의
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

	// 구현

	//    virtual int Run();
};

extern CQuestScripterApp theApp;
