#ifndef _OPENPROCESS_H_
#define _OPENPROCESS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#endif // _OPENPROCESS_H_

//#include <tlhelp32.h>
#pragma comment(lib , "pdh.lib")
#include <boost/thread.hpp>
#include <Pdh.h>

class KDLLChecker
{
    KDLLChecker();
    
    static KDLLChecker* m_pInstace;

    //등록되어있는 dll리스트
    std::set<std::string>        m_setDllList;
    //로드된 리스트
    std::set<std::string>        m_setLoadList;
    //리스트에 없는 Dll 
    std::set<std::string>        m_setNoList;  
	//리스트에 없는 Dll -psapi.dll
	std::set<std::string>        m_setNoListBypsapi;

    bool m_bSnapCheck;

	bool m_bCheckedMainName;
	bool m_bIsNT;
	HINSTANCE m_hPsapiDLL;


    void SetDllList();                          //dll리스트 등록

	void GetDllByPsapi();
	
	void GetDLLToProcess();

public:
    ~KDLLChecker();
    static KDLLChecker *GetInstance()
    {
        if (m_pInstace==NULL)
            m_pInstace = new KDLLChecker();

        return m_pInstace;
    }

	void CheckDll();
    std::set<std::string> GetNoList()			{ return m_setNoList; }
	std::set<std::string> GetNoListByPsapi()	{ return m_setNoListBypsapi; }
    bool GetbSnapCheck()                        { return m_bSnapCheck; }
	bool GetbIsNT()								{ return m_bIsNT; }
	bool HaveHiddenDll();

    void CheckBlackDllByBlackList(std::string strDll_);
    void CheckBlackDllBypattern(std::string strDll_);
    void SettingRandShutdownCount(int iAddFrameValue_);

	void CheckProcessList();

};

class KSHAChecker {

    enum { LIMIT_INIT_FAIL = 100 , 
           FAIL_GET_CPUUSAGE = -1,
           LIMIT_CHECK_TIME = 60000 /*프레임카운트*/ ,
           REST_TIME = 600 /*초단위*/ };

    KSHAChecker();

    static boost::shared_ptr<KSHAChecker> m_pInstace;

    HQUERY m_hQuery;
    HCOUNTER m_hCounter;
    bool m_bWork;
    
    //On/off 기능을 위한 변수 (server에서 받는다)
    bool m_bON;

    boost::shared_ptr<boost::thread> m_pThread;

    LONG m_lHighestCPUUsage;                  //가장 높은 CPU 사용치
    LONG m_lLowestCPUUsage;                   //가장 낮은 CPU 사용치
    LONG m_lStandardCPUUsage;                 //SHA Check 기준치
    LONG m_lCurrenCPUUsage;                   //현재 CPU 사용량

    LONG m_lCntLimitTime;                     //임계치 시간 체크 변수 ( 프레임 단위)

    KSHAChecker(const KSHAChecker&) {}
    KSHAChecker operator=(const KSHAChecker&) {}

    void ThreadFunc();
    void GetCPUUsage();
    void WorkFunc();

    void CheckSHA();

    bool Init();
    void DeInit();

    void RecordAndSendErrUserInfo();

public:
    //소멸자 호출을 위해 public에 두었음
    ~KSHAChecker();
    static KSHAChecker* GetInstance() {
        if (m_pInstace.get() == NULL) {
            m_pInstace.reset(new KSHAChecker);
        }
        return m_pInstace.get();
    }
    void StartThread();

    void CountingLimitTime()                          { if (m_bON) m_lCntLimitTime++; }
    
    void SetbWork(const bool bWork_ )                 { m_bWork = bWork_; }
    void SerbOn(const bool bOn_ )                     { m_bON = bOn_; }

};

class KHackCheckManager{

	enum { LIMIT_INC_DAMAGE = 4 };

	KHackCheckManager();

	static boost::shared_ptr<KHackCheckManager> m_pInstace;


	KHackCheckManager(const KHackCheckManager&) {}
	KHackCheckManager operator=(const KHackCheckManager&) {}


	float m_fOriginValue;

public:
	~KHackCheckManager();
	static KHackCheckManager* GetInstance() {
		if (m_pInstace.get() == NULL) {
			m_pInstace.reset(new KHackCheckManager);
		}
		return m_pInstace.get();
	}

	void BeforeCheckApRatio(KProtectedType<float> *pfIncApRatio_ , float *pfOriginValue);

	void AfterCheckApRatio(float *pfResult_ );

};