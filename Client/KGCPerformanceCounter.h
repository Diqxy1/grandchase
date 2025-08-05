#pragma once

//#include<map>
//#include<queue>
//#include<string>
//#include<windows.h>

#if defined(PERFORMANCE_TEST)
#define KGCPC_BEGIN(p)  KGCPerformanceCounter::Begin(p) 
#define KGCPC_END() KGCPerformanceCounter::End()
#define KGCPC_VALUE( key, val ) { \
    KGCPerformanceCounter::Begin( key );\
    KGCPerformanceCounter::End( val );\
    }
#define KGCPC_WRITE() KGCPerformanceCounter::WriteCounter()
#define KGCPC_MEMO(memo) \
{\
    std::stringstream stm;\
    stm<<memo;\
    KGCPerformanceCounter::AddMemo(stm.str());\
}
#elif defined( PERFORMANCE_TEST_P2P )
#define KGCPC_BEGIN(p)
#define KGCPC_END()
#define KGCPC_VALUE( key, val ) { \
    KGCPerformanceCounter::Begin( key );\
    KGCPerformanceCounter::End( val );\
    }
#define KGCPC_WRITE() KGCPerformanceCounter::WriteCounter()
#define KGCPC_MEMO(memo) \
{\
    std::stringstream stm;\
    stm<<memo;\
    KGCPerformanceCounter::AddMemo(stm.str());\
}
#else
#define KGCPC_BEGIN(p)
#define KGCPC_END()
#define KGCPC_VALUE( key, val )
#define KGCPC_WRITE()
#define KGCPC_MEMO(str)
#endif


class KGCPerformanceCounter
{
	static FILE* fp;	

	static KGCPerformanceCounter* current;	
	static int frequent;
	static std::map<std::string,KGCPerformanceCounter*> countermap;
    static std::stringstream memo;
    static std::stringstream title;
    static bool ms_bWriteTitle;
    static std::set<std::string> titlelist;

	KGCPerformanceCounter(KGCPerformanceCounter* p,std::string key);
	~KGCPerformanceCounter(void);

	KGCPerformanceCounter* parent;
	std::map<std::string,KGCPerformanceCounter*> child;

    void Write();
    void WriteTitle();

	LARGE_INTEGER pre;
	LARGE_INTEGER now;
	LARGE_INTEGER total;	
	
	std::string key;
	int calls;
public:
	static void BeginCounter(std::string filename,int f);
	static void EndCounter();
	static void Begin(std::string key);
    static void End( int iValue_ = -1 );
    static void WriteAllTitle();
	static void WriteCounter();
    static void AddMemo( std::string str );
};



