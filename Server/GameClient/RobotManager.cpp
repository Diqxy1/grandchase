#define LUABIND_NO_RTTI
#include "robotmanager.h"
#include "tchar.h"
#include "NetError.h"
#include "Socket/IOCP.h"
#include "Socket/NetCommon.h"
#include "DefaultFSM.h"
#include <IPHlpApi.h>                       // for GetAdaptersInfo()

ImplementSingleton( KRobotManager );

ImplToStringW( KRobotManager )
{
    return stm_;
}

KRobotManager::KRobotManager(void)
{
    m_iMassiveRobotNum = 0;
    m_bCheckiAllMassiveCom = false;
}

KRobotManager::~KRobotManager(void)
{
    m_vecMassiveRobot.erase( m_vecMassiveRobot.begin(), m_vecMassiveRobot.end() );
}

ImplOstmOperatorW2A( KRobotManager );
ImplOstmOperatorW2A( KSingleRobot );

void KRobotManager::Init()
{

    srand( (unsigned) time( NULL ) );

    //dbg::logfile::SetFileNameAtTime( true );

    SiKIocp()->Init( 4 );

    m_dwUserUID = 10243;

    START_LOG( clog, L"RobotManager Start" ) << END_LOG;
}

bool KRobotManager::CheckMacAddress()
{
    char strMac[256];
    DWORD size = sizeof(PIP_ADAPTER_INFO);

    PIP_ADAPTER_INFO Info;
    ZeroMemory( &Info, size );
    int result = GetAdaptersInfo( Info, &size );        // 첫번째 랜카드 MAC address 가져오기
    if (result == ERROR_BUFFER_OVERFLOW)    // GetAdaptersInfo가 메모리가 부족하면 재 할당하고 재호출
    {
        Info = (PIP_ADAPTER_INFO)malloc(size);
        GetAdaptersInfo( Info, &size );
    }
    if(!Info)        return 0;
    sprintf_s(strMac, "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", 
        Info->Address[0], Info->Address[1], Info->Address[2], Info->Address[3], Info->Address[4], Info->Address[5] );

    printf(strMac);
    std::vector< std::string > vecMacAddress;
    vecMacAddress.push_back( "00-0F-FE-CC-FF-F1" );
    
    vecMacAddress.push_back ("10-60-4B-63-C5-8C" );
    vecMacAddress.push_back ("10-60-4B-5F-41-0A" );
    vecMacAddress.push_back ("E8-39-35-5C-92-94" );
    vecMacAddress.push_back ("08-2E-5F-0F-FC-EC" );
    vecMacAddress.push_back ("10-60-4B-60-0E-F2" );
    vecMacAddress.push_back ("10-60-4B-60-0E-76" );
    vecMacAddress.push_back ("10-60-48-83-09-8A" );
    vecMacAddress.push_back ("B4-B5-2F-bF-F3-88" );
    vecMacAddress.push_back ("E8-39-35-3E-0C-64" );
    vecMacAddress.push_back ("10-40-4B-63-C4-B2" );
    vecMacAddress.push_back ("14-CF-92-0E-DF-DE" );
    vecMacAddress.push_back ("14-CF-92-0E-E2-71" );
    vecMacAddress.push_back ("78-E7-D1-C7-8B-E2" );
    vecMacAddress.push_back ("6C-3B-E5-1E-F3-BC" );
    vecMacAddress.push_back ("78-E7-D1-C6-D8-81" );
    vecMacAddress.push_back ("00-FF-0A-08-31-71" );
    vecMacAddress.push_back ("00-FF-C5-24-B2-7B" );
    vecMacAddress.push_back ("E8-39-35-4E-84-FE" );
    vecMacAddress.push_back ("2C-27-D7-41-CE-4E" );

    std::vector< std::string >::iterator vit;
    vit = std::find( vecMacAddress.begin(), vecMacAddress.end(), strMac  );
    //if = strMac
    if ( vit == vecMacAddress.end() ) {
        START_LOG( cerr, L"Not registered macaddress : " << strMac ) << END_LOG;

        return false;
    }

    return true;
}

void KRobotManager::Run()//수정 필요
{
    SiKIocp()->BeginThread();
    for ( int i = 0; i < m_iMassiveRobotNum; i++ )
    {
        ResumeThread(m_vecItMassiveThread[i]);
    }

    int MassiveComEnd = 0;
    int *countClientEnd = new int[m_iMassiveRobotNum];

    for(int i = 0 ; i < m_iMassiveRobotNum ; i++)
        countClientEnd[i] = 0;

    while(MassiveComEnd != m_iMassiveRobotNum){
        for(int i = 0 ; i < m_iMassiveRobotNum ; i++){
            int *m_iAllMassiveCom_1 = m_vecMassiveRobot[i]->GetAllClientCom();

            m_iAllMassiveCom[i] = m_iAllMassiveCom_1;
            
            for(int j = 0 ; j < m_sBotParam.clientbymass[i] ; j++){//하나의 매시안의 클라이언트수만큼
                
                if(m_iAllMassiveCom_1[j] == m_sBotParam.masscomque[i][j].size()){//각각의 매시안의 클라이언트의 작업 진행 현황이 최종 작업진행도와 같다면
                    if(!m_bAllMassiveCom[i][j]){
                        countClientEnd[i]++;
                        m_bAllMassiveCom[i][j] = true;
                    }
                }	

            }	
            if(countClientEnd[i] == m_sBotParam.clientbymass[i]){
                MassiveComEnd++;
            }
        }	

        m_bCheckiAllMassiveCom = true;
    }
//MessageBox(NULL,TEXT("1"),TEXT("1"),MB_OK);
    WaitForMultipleObjectsEx(m_iMassiveRobotNum,m_vecItMassiveThread,TRUE,INFINITE,TRUE);
        //MessageBox(NULL,TEXT("2"),TEXT("2"),MB_OK);
    ShutDown();
            //MessageBox(NULL,TEXT("3"),TEXT("4"),MB_OK);
        //WaitForSingleObjectEx(m_vecItMassiveThread,INFINITE,TRUE);
    //}
}

int KRobotManager::GetMassiveRobotNum(){
    return m_iMassiveRobotNum;
}

int* KRobotManager::GetMassiveRobotClientNum(){
    return m_ipMassiveRobotClientNum;
}

int** KRobotManager::GetCheckiAllMassiveCom(){
    if(m_bCheckiAllMassiveCom)
        return m_iAllMassiveCom;
    else
        return NULL;
}

void KRobotManager::StartMassiveRobot(int Massiveindex)
{
        m_vecMassiveRobot[Massiveindex]->Run();
}

unsigned int CALLBACK ThreadMassiveRobot(LPVOID dParameter){
    
    Tparam* ThreadParam = (Tparam*)dParameter;

    ThreadParam->m_robot->StartMassiveRobot(ThreadParam->m_indexMassive);

    return 0;
}

void KRobotManager::SetMassiveRobotNum(Botparam parm,int paramcase,dpParam singleParam) 
{										     
    m_sBotParam = parm;
    if(paramcase == 0){//초기설정. 프로그램 시작시 서버에 접속하는 과정에서 사용하는 부분
        m_iMassiveRobotNum = parm.clientbymass.size(); // 매시브 로봇의 수 
        m_ipMassiveRobotClientNum = new int[m_iMassiveRobotNum];

        char massName[256];

        m_vecMassiveRobot.clear();
        m_vecItMassiveThread = new HANDLE[(int)parm.clientbymass.size()];
        m_bAllMassiveCom = new bool*[(int)parm.clientbymass.size()];
        m_iAllMassiveCom = new int*[(int)parm.clientbymass.size()];
        START_LOG( cerr, L"매시브 로봇의 수 : " << m_iMassiveRobotNum ) << END_LOG;
        for ( int i = 0; i < (int)parm.clientbymass.size() ; i++ )
        {
            KMassiveRobotPtr sp( new KMassiveRobot );
            HANDLE pt_Massive;
            Tparam *th_param = new Tparam;
            sp->Init();
            sprintf_s(massName, "%s","test" );
            sp->m_strRobotLogin = massName;
            sp->SetDefaultData(singleParam);
            sp->SetNum(parm.clientbymass[i],parm.masscomque[i],parm.RoomMode[i]);

            m_vecMassiveRobot.push_back( sp );
            th_param->m_robot = this;
            th_param->m_indexMassive = i;
            pt_Massive = (HANDLE)_beginthreadex(NULL,0,ThreadMassiveRobot,th_param,CREATE_SUSPENDED,NULL);
            m_vecItMassiveThread[i] = pt_Massive;
            m_bAllMassiveCom[i] = new bool[parm.clientbymass[i]];
            m_ipMassiveRobotClientNum[i] = parm.clientbymass[i];
            for(int j = 0 ; j < parm.clientbymass[i] ; j++)
                m_bAllMassiveCom[i][j] = false;
        }
    }else if(paramcase == 1){//초기설정에 채널과 방모드를 줌
        m_iMassiveRobotNum = parm.clientbymass.size(); // 매시브 로봇의 수 
        m_ipMassiveRobotClientNum = new int[m_iMassiveRobotNum];
        char massName[256];

        m_vecMassiveRobot.clear();
        m_bAllMassiveCom = new bool*[(int)parm.clientbymass.size()];
        m_iAllMassiveCom = new int*[(int)parm.clientbymass.size()];
        m_vecItMassiveThread = new HANDLE[(int)parm.clientbymass.size()];
        START_LOG( cerr, L"매시브 로봇의 수 : " << m_iMassiveRobotNum ) << END_LOG;
        for ( int i = 0; i < (int)parm.clientbymass.size() ; i++ )
        {
            KMassiveRobotPtr sp( new KMassiveRobot );
            HANDLE pt_Massive;
            Tparam *th_param = new Tparam;
            sp->Init();
            sprintf_s(massName, "%s","test" );
            sp->m_strRobotLogin = massName;
            sp->SetDefaultData(singleParam);
            sp->SetNum(parm.clientbymass[i],parm.masscomque[i], parm.ChannelID[i],parm.RoomMode[i]);

            m_vecMassiveRobot.push_back( sp );
            th_param->m_robot = this;
            th_param->m_indexMassive = i;
            pt_Massive = (HANDLE)_beginthreadex(NULL,0,ThreadMassiveRobot,th_param,CREATE_SUSPENDED,NULL);
            m_vecItMassiveThread[i] = pt_Massive;
            m_bAllMassiveCom[i] = new bool[parm.clientbymass[i]];
            m_ipMassiveRobotClientNum[i] = parm.clientbymass[i];
            for(int j = 0 ; j < parm.clientbymass[i] ; j++)
                m_bAllMassiveCom[i][j] = false;
        }
    }else if(paramcase == 2){//마지막 버전. 채널 id와 클라이언트 id 그리고 방 모드를 준다.
        m_iMassiveRobotNum = parm.clientbymass.size(); // 매시브 로봇의 수 
        m_ipMassiveRobotClientNum = new int[m_iMassiveRobotNum];
        char massName[256];

        m_vecMassiveRobot.clear();
        m_bAllMassiveCom = new bool*[(int)parm.clientbymass.size()];
        m_iAllMassiveCom = new int*[(int)parm.clientbymass.size()];
        m_vecItMassiveThread = new HANDLE[(int)parm.clientbymass.size()];
        START_LOG( cerr, L"매시브 로봇의 수 : " << m_iMassiveRobotNum ) << END_LOG;
        for ( int i = 0; i < (int)parm.clientbymass.size() ; i++ )
        {
            KMassiveRobotPtr sp( new KMassiveRobot );
            HANDLE pt_Massive;
            Tparam *th_param = new Tparam;
            sp->Init();
            sprintf_s(massName, "%s","test" );
            sp->m_strRobotLogin = massName;
            sp->SetDefaultData(singleParam);
            sp->SetNum(parm.clientbymass[i],parm.masscomque[i], parm.ChannelID[i], parm.m_cppClient_Name[i],parm.RoomMode[i]);

            m_vecMassiveRobot.push_back( sp );
            th_param->m_robot = this;
            th_param->m_indexMassive = i;
            pt_Massive = (HANDLE)_beginthreadex(NULL,0,ThreadMassiveRobot,th_param,CREATE_SUSPENDED,NULL);
            m_vecItMassiveThread[i] = pt_Massive;
            m_bAllMassiveCom[i] = new bool[parm.clientbymass[i]];
            m_ipMassiveRobotClientNum[i] = parm.clientbymass[i];
            for(int j = 0 ; j < parm.clientbymass[i] ; j++)
                m_bAllMassiveCom[i][j] = false;
        }
    }
}

DWORD KRobotManager::GetMyUID()
{
    KLocker lock( m_csUserUID );
   /* if( m_dwUserUID > 12242)
        return;*/
    return m_dwUserUID++;
}
void KRobotManager::ShutDown()
{
    ::WSACleanup();
    for ( int i = 0; i < m_iMassiveRobotNum; i++ )
    {
        m_vecMassiveRobot[i]->ShutDown();
    }
}

void KRobotManager::FindZombie()
{
    for ( int i = 0; i < m_iMassiveRobotNum; i++ )
    {
        m_vecMassiveRobot[i]->FindZombie();
    }
}
