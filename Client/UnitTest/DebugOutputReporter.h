#pragma once

#include "./UnitTest/UnitTest++.h"
#include "TestReporter.h"

namespace UnitTest
{

class DebugOutputReporter : public TestReporter
{
public:
	DebugOutputReporter(void);
	~DebugOutputReporter(void);

	virtual void ReportTestStart(TestDetails const& test);
	virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed);
    virtual void ReportFailure(TestDetails const& test, char const* failure);
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
};


}