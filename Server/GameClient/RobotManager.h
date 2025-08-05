#pragma once

#include "MassiveRobot.h"
#include <KNCSingleton.h>

typedef struct BotParam{
	std::map<int,std::map<int,std::vector<int>>> masscomque;//매시당 명령
	std::vector<DWORD> ChannelID;
	int **RoomMode;

	std::vector<int> clientbymass;//매시당 클라수
	
	CString **m_cppClient_Name;
}Botparam;

class KRobotManager
{
    DeclareSingleton( KRobotManager );
    DeclToStringW;
public:
    KRobotManager(void);
    ~KRobotManager(void);

    void Init();
    void Run();
    void ShutDown();
    //void SetName();// std::string strLogin_ );
    DWORD GetMyUID(); // for messenger test
    void FindZombie();
    bool CheckMacAddress();

	//void SetNum( std::vector<int> iNum_ );
	//2013  8   9
	void SetMassiveRobotNum(Botparam parm,int paramcase,dpParam singleParam);
	void StartMassiveRobot(int Massiveindex);
	int** GetCheckiAllMassiveCom();
	int GetMassiveRobotNum();
	int* GetMassiveRobotClientNum();

protected:
    std::vector< KMassiveRobotPtr >       m_vecMassiveRobot;
	HANDLE								  *m_vecItMassiveThread;//massive로봇용 쓰레드변수
	int                                   m_iMassiveRobotNum;
	int									  *m_ipMassiveRobotClientNum;
    DWORD                                 m_dwUserUID;
    KncCriticalSection                    m_csUserUID;
	bool								  **m_bAllMassiveCom;//모든 massive로봇용 명령완료단계 확인 변수 최종 클라이언트완료 판단
	Botparam							  m_sBotParam;
	int									  **m_iAllMassiveCom;//모든 massive로봇용 명령어완료단계 확인 변수 숫자로 판단
	bool								  m_bCheckiAllMassiveCom;
};

typedef struct ThreadParam{
	KRobotManager						  *m_robot;
	int									  m_indexMassive;
}Tparam;

unsigned int CALLBACK ThreadMassiveRobot(LPVOID dParameter);

DefSingletonInline( KRobotManager );
