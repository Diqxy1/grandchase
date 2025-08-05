#include "StdAfx.h"
#include "DebugOutputReporter.h"
//


namespace UnitTest
{

DebugOutputReporter::DebugOutputReporter(void)
{
}

DebugOutputReporter::~DebugOutputReporter(void)
{
}

void DebugOutputReporter::ReportTestStart(TestDetails const& /*test*/)
{
}

void DebugOutputReporter::ReportTestFinish(TestDetails const& /*test*/, float)
{
}

void DebugOutputReporter::ReportFailure(TestDetails const& details, char const* failure)
{
	char buffer[1024] = {0,};
    char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";
    sprintf(buffer, errorFormat, details.filename, details.lineNumber, details.testName, failure);

#ifdef _UNICODE
	wchar_t wbuffer[1024] = {0,};
	MultiByteToWideChar(CP_ACP, 0, buffer, (int)strlen(buffer), wbuffer, 1024);
	OutputDebugString(wbuffer);
#else
	OutputDebugString(buffer);
#endif
	
}

void DebugOutputReporter::ReportSummary(int const totalTestCount, int const failedTestCount,
                                       int const failureCount, float secondsElapsed)
{
	TCHAR buffer[1024] = {0,};
    if (failureCount > 0)
	{
		_stprintf(buffer, 
			_T("FAILURE: %d out of %d tests failed (%d failures).\n"), 
			failedTestCount, totalTestCount, failureCount);
	}
    else
	{
		_stprintf( buffer, 
			_T("Success: %d tests passed.\n"), totalTestCount);
	}
	OutputDebugString(buffer);
	_stprintf(buffer, _T("Test time: %.2f seconds.\n"), secondsElapsed);
	OutputDebugString(buffer);
}


}