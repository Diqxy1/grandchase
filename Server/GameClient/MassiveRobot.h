#pragma once
#include <WinSock2.h>
#include "Thread/Thread.h"
#include "SingleRobot.h"
#include <boost/shared_ptr.hpp>
//#include <KNCSingleton.h>
#include <ToString.h>
#include <vector>
#include <map>

SmartPointer( KMassiveRobot );

typedef struct DepualtParam{
	int						m_iProtocolVer;
	char					*m_cIP;
	unsigned short			m_usPort;
	char					*m_cMsgIP;
	unsigned short			m_usMsgPort;
	char					*m_cMsgIP2nd;
	unsigned short			m_usMsgPort2nd;
	DWORD					m_dTestMsgTime;
}dpParam;

class KMassiveRobot 
{
    //DeclareSingleton( KMassiveRobot );
    DeclToStringW;
public:
    KMassiveRobot();
    virtual ~KMassiveRobot();

    void Init();
    void Run();
    void ShutDown();

public:
    //void IncreaseClient( int nIncrease );
    //void DecreaseClient( int nDecrease );
	void SetDefaultData(dpParam sdData);
	void SingSet(KSingleRobotPtr spClient);
	void SetNum( size_t nClientNum, std::map<int,std::vector<int>> comque, int *RoomMode );
	void SetNum( size_t nClientNum, std::map<int,std::vector<int>> comque, DWORD ChannelID, int *RoomMode );
    void SetNum( size_t nClientNum, std::map<int,std::vector<int>> comque, DWORD ChannelID, CString *ClientID, int *RoomMode );
	void FindZombie();
    void DumpRobot();
    KSingleRobot* GetSingleRobot( int nID );

    void PrintUserState( int nID );

protected:  // util function
    void Tick();

public:
    std::string             m_strRobotLogin;    // 로봇의 아이디로 사용할 문자열. lua에서 입력받는다.

    static UINT             ms_uiSingleRobotCount;
    static UINT             ms_uiMassiveRobotCount;

    static UINT             ms_uiMassiveRobotUIDDiff;

	/*
	2013.   7.    29
	추가내용
	*/
protected:
	int						m_CheckCountDeadClient;
	int					    *m_iAllClients;
	int						m_iProtocolVer;
	char*					m_cIP;
	unsigned short			m_usPort;
	char*					m_cMsgIP;
	unsigned short			m_usMsgPort;
	char*					m_cMsgIP2nd;
	unsigned short			m_usMsgPort2nd;
	DWORD					m_dTestMsgTime;

public:
	int* GetAllClientCom();

protected:

    FSMclassPtr             m_spFSM;

    boost::shared_ptr< KTThread<KMassiveRobot> >    m_pthWorker;  
    mutable CRITICAL_SECTION                        m_csClient;
    std::vector< KSingleRobotPtr >                  m_vecClient;    

    friend class KSingleRobot;
};

//DefSingletonInline( KMassiveRobot );
DeclOstmOperatorA( KMassiveRobot );