#include "stdafx.h"
#include "MainDlg.h"

#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment(lib, "../Common/Crashrpt/CrashRpt.lib")

BOOL CALLBACK CrashCallback( LPVOID lpvState )
{
    AppendToMailContent( lpvState, NetCommon::GetLocalIP() );
    return true;
}

LPVOID lpvState = Install( CrashCallback, "lucidust@kog.co.kr", __TIMESTAMP__ );
//LPVOID lpvState = Install( CrashCallback, "tgkwon@kog.co.kr, lucidust@kog.co.kr, nodefeat@kog.co.kr", __TIMESTAMP__ );
#endif

IMPLEMENT_DYNAMIC(MainDlg, CDialog)

std::map<int,std::string> MainDlg::m_vStaticCommandNameTest;
int MainDlg::ms_iProtocolVer(-1);
std::string MainDlg::ms_cIP("127.0.0.1");
unsigned short MainDlg::ms_usPort(9400);
std::string MainDlg::ms_cMsgIP("192.168.199.104");
unsigned short MainDlg::ms_usMsgPort(9300);
std::string MainDlg::ms_cMsgIP2nd("192.168.199.104");
unsigned short MainDlg::ms_usMsgPort2nd(9310);
DWORD MainDlg::ms_dTestMsgTime(0);
bool MainDlg::ms_bAutoSelectChannel(false);
int MainDlg::ms_iNumMassive(-1);
int MainDlg::ms_iNumMassiveClient(-1);
std::string MainDlg::ms_sClientIDStart("test");

MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
    : CDialog(MainDlg::IDD, pParent)
{
}

MainDlg::~MainDlg()
{

}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_COMBO1,m_cCombo1Channel);
    DDX_Control(pDX,IDC_COMBO2,m_cCombo2MassiveClient);
    DDX_Control(pDX,IDC_LIST1_NOOFMASSIVEANDCLIENT,m_lNoOfMassiveAndClient);
    DDX_Control(pDX,IDC_LIST1_DETAILMASSIVE,m_lDetailMassive);
    DDX_Control(pDX,IDC_LIST1_RESULT,m_lResult);
    DDX_Control(pDX,IDC_LIST_CHOICE,m_lChoise);
    DDX_Control(pDX,IDC_LIST_COMLIST,m_lCom);
    DDX_Control(pDX,IDC_COMBO_COM,m_comboCom);
    DDX_Control(pDX,IDC_COMBO_AUTOCREATE,m_comboAutoCreate);
}

BOOL MainDlg::OnInitDialog(){
    CDialog::OnInitDialog();

    //if(0 != AfxOleInit()){
    //    return FALSE;
    //}

    singleParam.m_cIP = NULL;
    singleParam.m_cMsgIP = NULL;;
    singleParam.m_cMsgIP2nd = NULL;
    singleParam.m_dTestMsgTime = 0;
    singleParam.m_iProtocolVer = -1;
    singleParam.m_usMsgPort = 0;
    singleParam.m_usMsgPort2nd = 0;
    singleParam.m_usPort = 0;//초기화

    if(!SetDlgItemID_Text()){//기본적인 정보를 지정하는 부분
        SetDlgItemText(IDC_STATIC_LUA_LOAD,TEXT("루아파일 로드 실패"));
        SetDlgItemText(IDC_CHANNEL_OK_STR,TEXT("채널정보 갱신 오류!"));

        return FALSE;
    }

    SetDlgItemText(IDC_STATIC_LUA_LOAD,TEXT("루아파일 로드 완료!"));

    if(FirstRunCheckServerConnectChannel()){//채널 정보 받아옴
        SetDlgItemText(IDC_CHANNEL_OK_STR,TEXT("채널정보 갱신 완료!"));
    }
    else{
        SetDlgItemText(IDC_CHANNEL_OK_STR,TEXT("채널정보 갱신 오류!"));
        MessageBox(TEXT("서버에 접속 할 수 없습니다. 확인해 주세요."),TEXT("WRONG"),MB_OK);

        return FALSE;
    }

    SetChoiseList();//명령어 선택문. 이후에 확장성을 고려하기위해 스크립트로 뺄 예정
    DWORD m_dwExStyle = m_lCom.GetExtendedStyle();
    m_lCom.SetExtendedStyle(m_dwExStyle | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);//해드 드래그드랍과 아이템 클릭시 전체 한줄 선택가능하게 속성수정

    m_lNoOfMassiveAndClient.InsertColumn(0,TEXT("Massive"),LVCFMT_CENTER,60);//각각의 매시브와 그 매시브내의 클라이언트 숫자를 표시
    m_lNoOfMassiveAndClient.InsertColumn(1,TEXT("Client"),LVCFMT_CENTER,60);//클라이언트 숫자 표시

    m_lDetailMassive.InsertColumn(0,TEXT("Value"),LVCFMT_CENTER,60);//하나의 매시브를 선택했을때 그 매시브 내의 클라이언트 id를 표시 id값은 수정가능
    m_lDetailMassive.InsertColumn(1,TEXT("Field"),LVCFMT_CENTER,60);

    int indexm_lCom = 0;
    CString HeadStr;
    HeadStr.Format(TEXT("COM"));
    m_lCom.InsertColumn(indexm_lCom++,HeadStr,LVCFMT_CENTER,100);

    HeadStr.Format(TEXT("COUNT"));
    m_lCom.InsertColumn(indexm_lCom++,HeadStr,LVCFMT_CENTER,100);

    HeadStr.Format(TEXT("OP1"));
    m_lCom.InsertColumn(indexm_lCom++,HeadStr,LVCFMT_CENTER,100);

    HeadStr.Format(TEXT("OP2"));
    m_lCom.InsertColumn(indexm_lCom++,HeadStr,LVCFMT_CENTER,100);

    int indexAutoCreate = 0;

    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("CreateRoom"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("BuyItembyGP"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("BuyItembyGem"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("BuyItembyCrystal"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("SellItem"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("RegisterMission"));
    m_comboAutoCreate.InsertString(indexAutoCreate++,TEXT("CompleteMission"));

    SetLuaBasicSetting();

    m_cfData = RegisterClipboardFormat(TEXT("MyData"));
    m_bStartBotTest = false;
    m_iNumMaxCom = 0;
    m_bFlagSetBotWork = false;
    m_bDragging = false;
    m_iIndexComListChoise = -1;//현재 명령어 지정리스트에서 선택된 인덱스
    m_iNumMaxComofAllClient = 0;//가장 긴 명령어갯수를 파악하기 위해

    return TRUE;
}

bool MainDlg::SetLuaBasicSetting(){//클라이언트 숫자랑 매시숫자랑 등 기본적인것들 새팅하는거 도와줌
    if(ms_iNumMassive != -1){//루아를 통해 초기화가 된경우
        CString aat;
        aat.Format(TEXT("%d"),ms_iNumMassive);
        SetDlgItemText(IDC_EDIT_MASSIVE,aat);
    }

    if(ms_iNumMassiveClient != -1){
        CString aat;
        aat.Format(TEXT("%d"),ms_iNumMassiveClient);
        SetDlgItemText(IDC_EDIT_MASSIVECLIENT,aat);
    }

    if(ms_bAutoSelectChannel){//채널을 임의로 설정하는게 정해져있다면
        m_cCombo1Channel.SetCurSel(0);
    }
    /*
    적용되는 부분들이 있는반면 명령어 반복횟수등을 저장하는 배열이 초기화가 되지 않아서 뺀부분 
    이후 작동에서는 사용자가 버튼을 눌러주어야 한다.
    */
    //SendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO1,CBN_SELENDOK),0L);
    //SendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON1_SAVE,BN_CLICKED),0L);
    //SendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_MASSIVE,BN_CLICKED),0L);

    return true;
}

bool MainDlg::SetDlgItemID_Text(){//초기 루아파일을 읽어와서 초기화하는 부분
    boost::shared_ptr<lua_State> Lua = boost::shared_ptr<lua_State>(lua_open(),lua_close);

     luabind::open( Lua.get() );
     luabind::module( Lua.get() )
     [
         luabind::def("SetCommandIdName",&MainDlg::SetCommandID_Name),
         luabind::def("SetProtocolVer",&MainDlg::SetProtocolVer),
         luabind::def("SetmscIP",&MainDlg::Setms_cIP),
         luabind::def("SetmsusPort",&MainDlg::Setms_usPort),
         luabind::def("SetmscMsgIP",&MainDlg::Setms_cMsgIP),
         luabind::def("SetmsusMsgPort",&MainDlg::Setms_usMsgPort),
         luabind::def("SetmscMsgIP2nd",&MainDlg::Setms_cMsgIP2nd),
         luabind::def("SetmsusMsgPort2nd",&MainDlg::Setms_usMsgPort2nd),
         luabind::def("SetmsdTestMsgTime",&MainDlg::Setms_dTestMsgTime),
         luabind::def("SetAutoSelectChannel",&MainDlg::SetAutoSelectChennel),
         luabind::def("SetMassiveNum",&MainDlg::SetMassiveNum),
         luabind::def("SetClientNum",&MainDlg::SetClientNum),
         luabind::def("SetClientNameStartKey",&MainDlg::SetClientNameStartKey)
     ];

     if( 0 != lua_dofile( Lua.get(), "config_robot.lua" ) ){
         MessageBox(TEXT("config_robot.lua 로드 실패"),TEXT("WRONG"),MB_OK);
         return false;
     }

     std::map<int,std::string>::iterator it;
     for(it = m_vStaticCommandNameTest.begin() ; it != m_vStaticCommandNameTest.end() ; it++){
         CString aat;
         aat = CString::CStringT(CA2CT(it->second.c_str()));
        m_vStaticCommandName[it->first] = aat;
     }

     //ip등을 초기화
     singleParam.m_iProtocolVer = ms_iProtocolVer;

     char *retIPPrt(new char[ms_cIP.length() + 1]);
     copy(ms_cIP.begin(),ms_cIP.end(),retIPPrt);
     retIPPrt[ms_cIP.length()] = '\0';
     singleParam.m_cIP = retIPPrt;

     singleParam.m_usPort = ms_usPort;//초기화

     char *retMsgIPPrt(new char[ms_cMsgIP.length() + 1]);
     copy(ms_cMsgIP.begin() , ms_cMsgIP.end(), retMsgIPPrt);
     retMsgIPPrt[ms_cMsgIP.length()] = '\0';
     singleParam.m_cMsgIP = retMsgIPPrt;

     singleParam.m_usMsgPort = ms_usMsgPort;

     char *retMsgIP2ndPrt(new char[ms_cMsgIP2nd.length() + 1]);
     copy(ms_cMsgIP2nd.begin(), ms_cMsgIP2nd.end(), retMsgIP2ndPrt);
     retMsgIP2ndPrt[ms_cMsgIP2nd.length()] = '\0';
     singleParam.m_cMsgIP2nd = retMsgIP2ndPrt;

     singleParam.m_usMsgPort2nd = ms_usMsgPort2nd;
     singleParam.m_dTestMsgTime = ms_dTestMsgTime;	
 
    // lua_interpreter( Lua.get() ); <- 필요하지 않다.

    return true;
}


BEGIN_MESSAGE_MAP(MainDlg, CDialog)
    ON_WM_CREATE()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON1_SAVE, &MainDlg::OnBnClickedButton1Save)
    ON_CBN_SELENDOK(IDC_COMBO1, &MainDlg::OnCbnSelendokCombo1)
    ON_BN_CLICKED(IDC_BUTTON_MASSIVE, &MainDlg::OnBnClickedButtonMassive)
    ON_BN_CLICKED(IDC_BUTTON_OK, &MainDlg::OnBnClickedButtonOk)
    ON_NOTIFY(NM_CLICK, IDC_LIST1_NOOFMASSIVEANDCLIENT, &MainDlg::OnNMClickList1Noofmassiveandclient)
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1_DETAILMASSIVE, &MainDlg::OnLvnEndlabeleditList1Detailmassive)
    ON_BN_CLICKED(IDC_BUTTON2_TOTALSUBMITE, &MainDlg::OnBnClickedButton2Totalsubmite)
    ON_BN_CLICKED(IDC_BUTTON3_MASSIVESUBMITE, &MainDlg::OnBnClickedButton3Massivesubmite)
    ON_BN_CLICKED(IDC_BUTTON4_CLIENTSUBMITE, &MainDlg::OnBnClickedButton4Clientsubmite)
    ON_BN_CLICKED(IDC_BUTTON5_CLEAR, &MainDlg::OnBnClickedButton5Clear)
    ON_BN_CLICKED(IDC_BUTTON2_RUN, &MainDlg::OnBnClickedButton2Run)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &MainDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_COM, &MainDlg::OnBnClickedButtonCom)
    ON_BN_CLICKED(IDC_BUTTON_SELECTDELETE, &MainDlg::OnBnClickedButtonDelete)
    ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_COMLIST, &MainDlg::OnLvnBegindragListComlist)
    ON_NOTIFY(NM_CLICK, IDC_LIST_COMLIST, &MainDlg::OnNMClickListComlist)
    ON_CBN_SELCHANGE(IDC_COMBO_AUTOCREATE, &MainDlg::OnCbnSelchangeComboAutocreate)
    ON_BN_CLICKED(IDC_BUTTON_CLEARPARAM, &MainDlg::OnBnClickedButtonClearparam)
END_MESSAGE_MAP()


// MainDlg 메시지 처리기입니다.

void MainDlg::ClearParam(){
    comque.clear();
    clientcomque.clear();
    pkMgrParam.masscomque.clear();
    pkMgrParam.clientbymass.clear();
    pkMgrParam.ChannelID.clear();
}

bool MainDlg::SetChoiseList(){
    int ListIndex = 0;

    std::map<int,CString>::iterator it;
    for(it = m_vStaticCommandName.begin() ; it != m_vStaticCommandName.end() ; it++,ListIndex++){
        m_lChoise.InsertItem(ListIndex,it->second);
    }

    return true;
}

bool MainDlg::FirstRunCheckServerConnectChannel(){
    pkMgr = SiKRobotManager();

    if ( pkMgr->CheckMacAddress() == false ) {
        START_LOG( cerr, L"Macaddress check failed" ) << END_LOG;

        return false;
    }

    pkMgr->Init();

    comque.push_back(0);//CONNECT
    comque.push_back(1);//VERIFY
    comque.push_back(-1);//파일에 채널 이름 씀
    comque.push_back(28);//DISCONNECT

    clientcomque[0] = comque;

    pkMgrParam.masscomque[0] = clientcomque;

    pkMgrParam.clientbymass.push_back(1);

    pkMgrParam.RoomMode = new int*[1];
    pkMgrParam.RoomMode[0] = new int[1];

    pkMgrParam.RoomMode[0][0] = 7;

    pkMgr->SetMassiveRobotNum(pkMgrParam,0,singleParam);

    pkMgr->Run();

    pkMgr->ShutDown();

    ClearParam();

    std::ifstream fin;
    fin.open("BotTestChannel.txt");

    if(!fin.is_open())
        return false;

    m_cCombo1Channel.ResetContent();

    while(!fin.eof()){
        char *buffer_ = new char[256];
        wchar_t *wbuffer_ = new wchar_t[256];

        fin.getline(buffer_,256);

        if(buffer_[0]){
            for(int i = 0 ; i < 256 ; i++){
                if(buffer_[i] == '.'){
                    wbuffer_[i] = '\0';
                    break;
                }

                wbuffer_[i] = buffer_[i];
            }

            m_cCombo1Channel.AddString(wbuffer_);//채널 목록 추가

            char* pToken = NULL;
            char* pToken1 = NULL;
            char* pToken2 = NULL;
            char* pToken3 = NULL;
            char* pToken4 = NULL;
            char* pToken5 = NULL;
            char* pSeparator = ".";
            char* pNextToken = NULL;

            pToken = strtok_s(buffer_,pSeparator, &pNextToken);//name
            pToken1 = strtok_s(NULL,pSeparator, &pNextToken);//id
            pToken2 = strtok_s(NULL,pSeparator, &pNextToken);//type
            pToken3 = strtok_s(NULL,pSeparator, &pNextToken);//maxuser
            pToken4 = strtok_s(NULL,pSeparator, &pNextToken);//ususer
            pToken5 = strtok_s(NULL,pSeparator, &pNextToken);//hid

            KChannelInfo m_ch;
            m_ch.m_strChannelName = wbuffer_;
            m_ch.m_dwChannelUID = atoi(pToken1);
            m_ch.m_usMaxUsers = atoi(pToken3);
            m_ch.m_usUsers = atoi(pToken4);
            if(pToken2)
                m_ch.m_cType = pToken2[0];
            else
                m_ch.m_cType = ' ';
            int ttype = atoi(pToken5);
            m_ch.m_bHide = (ttype == 0) ? false : true;

            ChannelNamenId.push_back(m_ch);
        }
    }

    fin.close();

    remove("BotTestChannel.txt");


    return true;
}

void MainDlg::OnBnClickedButton1Save()//매시봇수와 매시당 클라이언트의 수를 저장
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString combo2Str;
    int NumberOfClient;
    NumMassive = GetDlgItemInt(IDC_EDIT_MASSIVE);//숫자아닌경우 0을 리턴
    NumMassiveClient = GetDlgItemInt(IDC_EDIT_MASSIVECLIENT);//매시당 클라이어트의 수
    
    if((NumMassive > 0) && (NumMassiveClient > 0)){
        NumberOfClient = 1;
        Array_Massive = new int[NumMassive];
        pkMgrParam.m_cppClient_Name = new CString*[NumMassive];
        pkMgrParam.RoomMode = new int*[NumMassive];

        //char *retNamePrt(new char[ms_sClientIDStart.length() + 1]);
        //copy(ms_sClientIDStart.begin(),ms_sClientIDStart.end(),retNamePrt);
        //retNamePrt[ms_sClientIDStart.length()] = '\0';

        for(int i = 0 ; i < NumMassive ; i++){
            pkMgrParam.m_cppClient_Name[i] = new CString[NumMassiveClient];
            pkMgrParam.RoomMode[i] = new int[NumMassiveClient];

            for(int j = 0 ; j < NumMassiveClient ; j++){
                pkMgrParam.m_cppClient_Name[i][j].Format(TEXT("%S%04d"),ms_sClientIDStart.c_str(),NumberOfClient++);
            }
        }

        m_cCombo2MassiveClient.ResetContent();

        for(int i = 0 ; i < NumMassive ; i++){
            Array_Massive[i] = NumMassiveClient;
            combo2Str.Format(TEXT("%d Massive"),i+1);
            m_cCombo2MassiveClient.AddString(combo2Str);
        }	
    }
}

void MainDlg::OnCbnSelendokCombo1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    
    std::vector<KChannelInfo>::iterator it;
    CString itemStr;
    m_cCombo1Channel.GetLBText(m_cCombo1Channel.GetCurSel(),itemStr);

    for(it = ChannelNamenId.begin() ; it != ChannelNamenId.end() ; it++){
        if((LPCTSTR)itemStr == it->m_strChannelName)
        {
            SetDlgItemText(IDC_CHANNEL_NAME,itemStr);
            itemStr.Format(TEXT("%d"),it->m_dwChannelUID);
            SetDlgItemText(IDC_CHANNEL_ID,itemStr);
            itemStr.Format(TEXT("%c"),it->m_cType);
            SetDlgItemText(IDC_CHANNEL_TYPE,itemStr);
            itemStr.Format(TEXT("%d"),it->m_usMaxUsers);
            SetDlgItemText(IDC_CHANNEL_MAXUSER,itemStr);
            itemStr.Format(TEXT("%d"),it->m_usUsers);
            SetDlgItemText(IDC_CHANNEL_USUSER,itemStr);	
            if(it->m_bHide)
                SetDlgItemText(IDC_CHANNEL_HIDE,TEXT("1"));
            else
                SetDlgItemText(IDC_CHANNEL_HIDE,TEXT("0"));

            break;
        }
    }
}

void MainDlg::OnBnClickedButtonMassive()//MASSIVEGO버튼클릭시 발생하는 이벤트 설정된갯수만큼의 매시를 리스트에 보여줌
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if((NumMassive > 0) && (NumMassiveClient > 0)){
        m_lNoOfMassiveAndClient.DeleteAllItems();
        m_iNumMaxCom = new int*[NumMassive];//리스트에서 헤더수로 명령어를 셈
        m_iNumCom = new int**[NumMassive];//리스트내의 각각의 명령어 반복횟수를 셈

        for(int i = 0 ; i < NumMassive ; i++){
            CString num1, num2;

            num1.Format(TEXT("Massi-%d"),i);
            num2.Format(TEXT("%d"),Array_Massive[i]);
            m_iNumMaxCom[i] = new int[Array_Massive[i]];
            m_iNumCom[i] = new int*[Array_Massive[i]];

            m_lNoOfMassiveAndClient.InsertItem(i,num1);
            m_lNoOfMassiveAndClient.SetItemText(i,1,num2);
        }

        
    }else{
        MessageBox(TEXT("기본 설정을 해주세요."),TEXT("WRONG"));
    }
}

void MainDlg::OnBnClickedButtonOk()//각각의 메시에서 클라수를 조정
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int CountClient;
    
    if(m_cCombo2MassiveClient.GetCurSel() >= 0){
        if(GetDlgItemInt(IDC_EDIT_SISNGLEMASSIVECLIENT) > 0){
            CountClient = 0;

            Array_Massive[m_cCombo2MassiveClient.GetCurSel()] = GetDlgItemInt(IDC_EDIT_SISNGLEMASSIVECLIENT);
            delete[] pkMgrParam.m_cppClient_Name[m_cCombo2MassiveClient.GetCurSel()];
            delete[] pkMgrParam.RoomMode[m_cCombo2MassiveClient.GetCurSel()];
            pkMgrParam.m_cppClient_Name[m_cCombo2MassiveClient.GetCurSel()] = new CString[GetDlgItemInt(IDC_EDIT_SISNGLEMASSIVECLIENT)];
            pkMgrParam.RoomMode[m_cCombo2MassiveClient.GetCurSel()] = new int[GetDlgItemInt(IDC_EDIT_SISNGLEMASSIVECLIENT)];

            for(int i = 0 ; i < m_cCombo2MassiveClient.GetCurSel() ; i++){
                CountClient += Array_Massive[i];
            }

            for(int i = m_cCombo2MassiveClient.GetCurSel() ; i < NumMassive ; i++){
                for(int j = 0 ; j < Array_Massive[i] ; j++){
                    pkMgrParam.m_cppClient_Name[i][j].Format(TEXT("test%04d"),++CountClient);
                }
            }

        }else
        {
            MessageBox(TEXT("매시당 클라수를 0이상으로 해주세요."),TEXT("WRONG"));
        }
    }else{
        MessageBox(TEXT("클라이언트 수를 수정하려는 Massive를 선택해주세요."),TEXT("WRONG"));
    }
}

void MainDlg::OnNMClickList1Noofmassiveandclient(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString num1;

    int indexOfClient = m_lNoOfMassiveAndClient.GetNextItem(-1,LVNI_SELECTED); //pDispInfo->item.iItem;
    CString strKey = m_lNoOfMassiveAndClient.GetItemText(indexOfClient,0);
    
    if(strKey != ""){
        
        m_lDetailMassive.DeleteAllItems();

        for(int i = 0 ; i < Array_Massive[indexOfClient] ; i++){
            num1.Format(TEXT("%s"),pkMgrParam.m_cppClient_Name[indexOfClient][i]);
            m_lDetailMassive.InsertItem(i,num1);
            m_lDetailMassive.SetItemText(i,1,TEXT("ID"));
        }	
    }

    *pResult = 0;
}

void MainDlg::OnLvnEndlabeleditList1Detailmassive(NMHDR *pNMHDR, LRESULT *pResult)//각각의 클라이언트의 이름수정하는 기능부
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    CString strText = TEXT("");
    CEdit *pEdit = m_lDetailMassive.GetEditControl();
    pEdit->GetWindowText(strText);

    int indexOfMassive = m_lNoOfMassiveAndClient.GetNextItem(-1,LVNI_SELECTED); //pDispInfo->item.iItem;
    int indexOfClient = m_lDetailMassive.GetNextItem(-1,LVNI_SELECTED);
    
    pkMgrParam.m_cppClient_Name[indexOfMassive][indexOfClient] = strText;

    m_lDetailMassive.SetItemText(pDispInfo->item.iItem,0,strText);

    *pResult = 0;
}

void MainDlg::OnBnClickedButton2Totalsubmite()//전체적용 모든 매시브와 각 매시브의 클라이언트가 같은 동작을 하게 된다.
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    ClearParam();
    CString ChannelName_1;

    int m_iComCount = m_lCom.GetItemCount();

    int RoomMode;

    if(m_iComCount > 0){

        Setm_iNumCom(0,NumMassive,0,NumMassiveClient,m_iComCount);

        Setcommandque(&comque,m_iComCount,&ChannelName_1,&RoomMode);

        pkMgrParam.clientbymass.clear();

        Setcommandque_MassiveClient(0,NumMassive,0,NumMassiveClient,comque,clientcomque,ChannelName_1,RoomMode);

        m_bStartBotTest = true;
    }
}

void MainDlg::OnBnClickedButton3Massivesubmite()//매시에만 적용 현재 massive리스트에서 보이는 메시브를 선택하면 해당 매시브와 그 안에 소속된 클라이언트에게 현재 명령들을 지정
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString ChannelName_1;

    int m_iComCount = m_lCom.GetItemCount();

    int indexOfMassive = m_lNoOfMassiveAndClient.GetNextItem(-1,LVNI_SELECTED); //pDispInfo->item.iItem;
    CString strMassiveName = m_lNoOfMassiveAndClient.GetItemText(indexOfMassive,0);

    int RoomMode;

    if(m_iComCount > 0){
        if(indexOfMassive > -1){
            if(strMassiveName != ""){

                Setm_iNumCom(indexOfMassive,indexOfMassive+1,0,Array_Massive[indexOfMassive],m_iComCount);

            }

            std::vector<int> Massive_comque;//명령

            Setcommandque(&Massive_comque, m_iComCount, &ChannelName_1,&RoomMode);

            std::map<int,std::vector<int>> massive_clientcomque;//클라당명령

            Setcommandque_MassiveClient(indexOfMassive, indexOfMassive+1,0,Array_Massive[indexOfMassive],Massive_comque, massive_clientcomque,ChannelName_1,RoomMode);

            m_bStartBotTest = true;
        }
    }
}

void MainDlg::OnBnClickedButton4Clientsubmite()//현재 선택된 해당 메시브에서 해당 클라이언트에게 명령을 적용
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString ChannelName_1;

    int m_iComCount = m_lCom.GetItemCount();

    int indexOfMassive = m_lNoOfMassiveAndClient.GetNextItem(-1,LVNI_SELECTED); //pDispInfo->item.iItem;
    int indexOfClient = m_lDetailMassive.GetNextItem(-1,LVNI_SELECTED);

    CString strMassiveName = m_lNoOfMassiveAndClient.GetItemText(indexOfMassive,0);
    CString strClientName = m_lDetailMassive.GetItemText(indexOfClient,0);

    int RoomMode;

    if(m_iComCount > 0){
        if(indexOfMassive > -1){
            if(indexOfClient > -1){
                if(strMassiveName != ""){
                    if(strClientName != ""){
                        Setm_iNumCom(indexOfMassive,indexOfMassive+1,indexOfClient,indexOfClient+1,m_iComCount);
                    }
                }

                std::vector<int> Massive_cleint_comque;//명령

                Setcommandque(&Massive_cleint_comque, m_iComCount, &ChannelName_1,&RoomMode);

                std::map<int,std::vector<int>> massive_clientcomque;//클라당명령

                Setcommandque_MassiveClient(indexOfMassive, indexOfMassive+1,indexOfClient,indexOfClient+1,Massive_cleint_comque, massive_clientcomque,ChannelName_1,RoomMode);

                m_bStartBotTest = true;
            }
        }
    }
}

bool MainDlg::Setm_iNumCom(int firstStart, int firstEnd, int SecondStart, int SecondEnd,int m_iComCount){

    if(m_iNumMaxComofAllClient < m_iComCount){
        m_iNumMaxComofAllClient = m_iComCount;

        m_vecPrintComName.clear();

        for(int i = 0 ; i < m_iNumMaxComofAllClient ; i++){
            m_vecPrintComName.push_back(m_lCom.GetItemText(i,0));
        }
    }

    for(int i = firstStart ; i < firstEnd ; i++){
        for(int j = SecondStart ; j < SecondEnd ; j++){
            m_iNumMaxCom[i][j] = m_iComCount;//모든 클라이언트의 명령동작 수 저장
            m_iNumCom[i][j] = new int[m_iComCount];
            for(int k = 0 ; k < m_iComCount ; k++){
                CString countStr = m_lCom.GetItemText(k,1);
                CString comNameStr = m_lCom.GetItemText(k,0);

                std::map<int,CString>::iterator it;
                for(it = m_vStaticCommandName.begin() ; it != m_vStaticCommandName.end() ; it++){
                    if(comNameStr == it->second){
                        m_iNumCom[i][j][k] = _ttoi(countStr);

                        break;
                    }
                }
            }
        }
    }

    return true;
}

bool MainDlg::Setcommandque(std::vector<int> *comque1, int m_iComCount, CString *ChannelName_1, int *RoomMode){
    for(int k = 0 ; k < m_iComCount ; k++){
        CString countStr = m_lCom.GetItemText(k,1);
        CString comNameStr = m_lCom.GetItemText(k,0);

        std::map<int,CString>::iterator it;
        for(it = m_vStaticCommandName.begin() ; it != m_vStaticCommandName.end() ; it++){
            if(comNameStr == it->second){
                for(int i = 0 ; i < _ttoi(countStr) ; i++){
                    (*comque1).push_back(it->first);

                    if(comNameStr == m_vStaticCommandName[7]){//enterchannel
                        (*ChannelName_1) = m_lCom.GetItemText(k,2);
                    }

                    if(comNameStr == m_vStaticCommandName[13]){//createroom
                        if(_ttoi(m_lCom.GetItemText(k,2))){
                            (*RoomMode) = _ttoi(m_lCom.GetItemText(k,2));
                        }else{
                            if(m_lCom.GetItemText(k,2) == TEXT("일반")){
                                (*RoomMode) = -1;//GC_GM_INVALID 이경우 각각의 채널에 맞게 룸모드가 자동으로 세팅된다. -1이 디폴트
                            }else{
                                MessageBox(TEXT("숫자를 입력해주세요."),TEXT("WRONG"),MB_OK);
                            }
                        }
                    }
                }

                break;
            }
        }
    }
    return true;
}

bool MainDlg::Setcommandque_MassiveClient(int FirstStart, int FirstEnd, int SecondStart, int SecondEnd, std::vector<int> comque1, std::map<int,std::vector<int>> clientcomque1,CString ChannelName_1,int RoomMode){

    std::vector<KChannelInfo>::iterator it;

    for(it = ChannelNamenId.begin() ; it != ChannelNamenId.end() ; it++){
        if((LPCTSTR)ChannelName_1 == it->m_strChannelName){
            for(int i = FirstStart ; i < FirstEnd ; i++){//massive의 숫자만큼 반복하면서
                clientcomque1 = pkMgrParam.masscomque[i];

                if(clientcomque1.size() == 0){
                    for(int j = SecondStart ; j < SecondEnd ; j++){//각각의 massive안에서 client의 숫자만큼
                        clientcomque1[j] = comque1;//클라이언트단위 명령
                        pkMgrParam.RoomMode[i][j] = RoomMode;
                    }

                    pkMgrParam.masscomque[i] = clientcomque1;//매시의 명령
                    pkMgrParam.clientbymass.push_back(Array_Massive[i]);//매시당 클라수

                    pkMgrParam.ChannelID.push_back(it->m_dwChannelUID);
                    
                }else{

                    for(int j = SecondStart ; j < SecondEnd ; j++){//각각의 massive안에서 client의 숫자만큼
                        clientcomque1[j] = comque1;//클라이언트단위 명령
                        pkMgrParam.RoomMode[i][j] = RoomMode;
                    }

                    pkMgrParam.masscomque[i] = clientcomque1;//매시의 명령
                    pkMgrParam.clientbymass[i] = Array_Massive[i];//매시당 클라수

                    pkMgrParam.ChannelID[i] = it->m_dwChannelUID;
                }
            }
            pkMgr->SetMassiveRobotNum(pkMgrParam,2,singleParam);//singleParam은 기본적인 data들 아이피주소나 포트번호와 같이 기본적인 것들.
        }
    }

    return true;
}

void MainDlg::OnBnClickedButton5Clear()//초기화
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_lCom.DeleteAllItems();
}

void MainDlg::OnBnClickedButton2Run()//테스트 시작 버튼 눌렀을때
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    if(m_bStartBotTest){
        CHeaderCtrl *ResultHeader = m_lResult.GetHeaderCtrl();
        m_lResult.DeleteAllItems();

        int headcount_1 = ResultHeader->GetItemCount();

        for(int i = 0 ; i < headcount_1 ; i++){
            m_lResult.DeleteColumn(0);
        }

        m_bStartBotTest = false;

        SetDlgItemText(IDC_STATIC_TEST_RESULT,TEXT("테스트를 시작합니다."));

        m_tBotThread = AfxBeginThread(ThreadFuncBotStart,this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
        
        m_tBotCheckThread = AfxBeginThread(ThreadFuncBotCheckCom,this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
    }
}

void MainDlg::StartBot(){//봇 움직이는 전용 쓰레드에 의해 호출되는 함수

    pkMgr->Run();

    m_bFlagSetBotWork = true;

    pkMgr->ShutDown();

    ClearParam();

    SetDlgItemText(IDC_STATIC_TEST_RESULT,TEXT("테스트가 완료되었습니다."));

    MessageBox(TEXT("테스트가 완료되었습니다."), TEXT("NOTIFY"),MB_OK);
}

void MainDlg::SetBotWork(){//현재 진행되고있는 매시브와 클라이언트들의 작업상황을 확인하여 result 리스트에 보여줌- 쓰레드가 사용하는 함수

    m_lResult.DeleteAllItems();

    CHeaderCtrl *DeleteCol = m_lResult.GetHeaderCtrl();

    int countDeleteCol = DeleteCol->GetItemCount();

    for(int i = 0 ; i < countDeleteCol ; i++){
        m_lResult.DeleteColumn(0);
    }

    m_bFlagSetBotWork = false;

    m_lResult.InsertColumn(0,TEXT("Massive"),LVCFMT_CENTER,60);

    for(int i = 0 ; i < m_iNumMaxComofAllClient ; i++){
        m_lResult.InsertColumn(i+1,m_vecPrintComName[i],LVCFMT_CENTER,60);
    }

    for(int i = 0 ; i < NumMassive ; i++){
        CString Mass;
        Mass.Format(TEXT("Massive-%d"),i);
        m_lResult.InsertItem(i,Mass);
    }

    int **m_iMassiveCom = pkMgr->GetCheckiAllMassiveCom();
    int **iCompleteCount;

    iCompleteCount = new int*[NumMassive];

    for(int i = 0 ; i < NumMassive ; i++){
        iCompleteCount[i] = new int[m_iNumMaxComofAllClient];
        for(int j = 0 ; j < m_iNumMaxComofAllClient ; j++)
            iCompleteCount[i][j] = 0;
    }

    while(!m_bFlagSetBotWork){		

        if(m_iMassiveCom){

            for(int i = 0 ; i < NumMassive ; i++){
                for(int j = 1 ; j <= Array_Massive[i] ; j++){
                    int countClientComple = 0;
                    for(int k = 0 ; k < m_iNumMaxCom[i][j] ; k++){
                        countClientComple += m_iNumCom[i][j][k];

                        if(m_iMassiveCom[i][j] > countClientComple){
                            if(iCompleteCount[i][k] < Array_Massive[i])
                                iCompleteCount[i][k]++;
                        }

                        CString NumClie;
                        NumClie.Format(TEXT("%d"),iCompleteCount[i][k]);
                        m_lResult.SetItemText(i,k+1,NumClie);
                    }
                }
            }
        }else{
            m_iMassiveCom = pkMgr->GetCheckiAllMassiveCom();
        }
    }

    countDeleteCol = DeleteCol->GetItemCount();//재사용

    for(int i = 0 ; i < NumMassive ; i++){
            CString aat;
            aat.Format(TEXT("%d"),Array_Massive[i]);			
            m_lResult.SetItemText(i,countDeleteCol-1,aat);
    }//마무리되면서 갯수맞추어주는 부분. 보여주기 위한 부분이지 큰 의미는 없다.

    m_vecPrintComName.clear();
    m_iNumMaxComofAllClient = 0;
}

unsigned int ThreadFuncBotStart(LPVOID dParameter){//테스트를 진행하는 봇 전용 쓰레드 동작함수
    MainDlg* m_maindlg = (MainDlg*)dParameter;

    m_maindlg->StartBot();

    return 0;
}

unsigned int ThreadFuncBotCheckCom(LPVOID dParameter){//결과화면 보여주는 쓰레드 동작함수
    MainDlg* m_maindlg = (MainDlg*)dParameter;

    m_maindlg->SetBotWork();

    return 0;
}

void MainDlg::OnBnClickedButtonAdd()//추가 버튼 눌렀을때, choice의 항목을 com리스트에 추가하는 버튼
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int indexOfChoice;
    int comindex = 0;

    int m_iSelectCountOfChoise = m_lChoise.GetSelectedCount();

    if(m_iSelectCountOfChoise != 0){
        indexOfChoice = m_lChoise.GetNextItem(-1,LVNI_SELECTED);

        while(indexOfChoice != -1){
            CString strKey = m_lChoise.GetItemText(indexOfChoice,0);
            comindex = m_lCom.GetItemCount();
            m_lCom.InsertItem(comindex,TEXT(""));

            m_lCom.SetItemText(comindex,0,strKey);
            m_lCom.SetItemText(comindex,1,TEXT("1"));

            if(strKey == m_vStaticCommandName[7]){//EnterChannel
                CString itemStr;

                if(m_cCombo1Channel.GetCurSel() >= 0){
                    m_cCombo1Channel.GetLBText(m_cCombo1Channel.GetCurSel(),itemStr);
                    m_lCom.SetItemText(comindex,2,itemStr);
                }else{
                    MessageBox(TEXT("좌측상단의 채널을 선택해주세요."),TEXT("WRONG"),MB_OK);
                    m_lCom.SetItemText(comindex,2,TEXT("채널"));
                }
            }else if(strKey == m_vStaticCommandName[13]/*TEXT("CreateRoom")*/){
                m_lCom.SetItemText(comindex,2,TEXT("일반"));
            }

            comindex++;

            indexOfChoice = m_lChoise.GetNextItem(indexOfChoice,LVNI_SELECTED);
        }

    }else{
        MessageBox(TEXT("명령어를 선택하세요."),TEXT("WRONG"),MB_OK);
    }
}

void MainDlg::OnBnClickedButtonCom()//명령어 반복횟수등을 지정할때
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString itemStr;

    if(m_comboCom.GetCurSel() > -1){
        m_comboCom.GetLBText(m_comboCom.GetCurSel(),itemStr);

        if(itemStr == TEXT("Count")){//COUNT를 정하는 부분
            int NumComEDIT = GetDlgItemInt(IDC_EDIT_COM);
            
            if(NumComEDIT > -1){
                CString editCom;

                editCom.Format(TEXT("%d"),NumComEDIT);

                m_lCom.SetItemText(m_iIndexComListChoise,1,editCom);
            }
        }else if(itemStr == TEXT("ChannelName")){//입장 채널등과 같이 부가적인 옵션을 추가하는 부분
        
            if(m_cCombo1Channel.GetCurSel() > -1){
                CString ChannelStr_1;
                m_cCombo1Channel.GetLBText(m_cCombo1Channel.GetCurSel(),ChannelStr_1);
                
                m_lCom.SetItemText(m_iIndexComListChoise,2,ChannelStr_1);
            }
            else{
                MessageBox(TEXT("좌측상단의 채널을 선택해주세요."),TEXT("WRONG"),MB_OK);
            }
        }else if(itemStr == TEXT("일반")){
            int NumComEDIT = GetDlgItemInt(IDC_EDIT_COM);

            if(NumComEDIT > 0){
                CString editCom;

                editCom.Format(TEXT("%d"),NumComEDIT);

                m_lCom.SetItemText(m_iIndexComListChoise,2,editCom);
            }else{
                MessageBox(TEXT("0보다 크거나 숫자를 입력해주세요."),TEXT("WRONG"),MB_OK);
            }
        }
    }else{
        MessageBox(TEXT("명령의 수정하려는 부분을 선택하고 내용을 입력해주세요."),TEXT("WRONG"),MB_OK);
    }
}

void MainDlg::OnNMClickListComlist(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

    int ComIndex = phdr->iItem;
    m_iIndexComListChoise = ComIndex;
    int DetailCom = 0;

    CString itemText = m_lCom.GetItemText(ComIndex,0);

    m_comboCom.ResetContent();
    SetDlgItemText(IDC_STATIC_HELPSTATIC,TEXT(""));

    if(itemText == m_vStaticCommandName[7]){//enterchannel
        m_comboCom.InsertString(DetailCom++,TEXT("Count"));
        m_comboCom.InsertString(DetailCom++,TEXT("ChannelName"));
        SetDlgItemText(IDC_STATIC_HELPSTATIC,TEXT("채널선택은 좌측상단에서 채널을 선택하세요."));
    }else if(itemText == m_vStaticCommandName[13]){//createroom
        m_comboCom.InsertString(DetailCom++,TEXT("Count"));
        m_comboCom.InsertString(DetailCom++,TEXT("일반"));
        SetDlgItemText(IDC_STATIC_HELPSTATIC,TEXT("일반 or 모드번호(숫자) 입력후 적용을 누르세요. 일반은 기본값"));
    }else{//나머지 부분들
        m_comboCom.InsertString(DetailCom++,TEXT("Count"));
    }

    *pResult = 0;
}

void MainDlg::OnBnClickedButtonDelete()//현재 COM리스트에서 선택된 녀석을 지운다.
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    int indexCom = m_lCom.GetNextItem(-1,LVNI_SELECTED);

    if(indexCom > -1){
        m_lCom.DeleteItem(indexCom);
    }
}


void MainDlg::OnBnClickedButtonClearparam()//현재까지 지정한 명령어를 초기화하는 부분
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    ClearParam();//파라미터 초기화
    /*
    여기서 초기화 안되는 부분이
    룸모드랑
    클라이언트 이름부분
    */
}

//아래부분들은 m_lcom에서 드래그 사용위해 추가된 부분들

INT MainDlg::GetHitIndex(CPoint point){//현재 드래그되고 있는 위치계산 함수
    CRect rcList;
    m_lCom.GetWindowRect(&rcList);
    ScreenToClient(reinterpret_cast<LPPOINT>(&rcList));

    LVHITTESTINFO HitInfo;
    HitInfo.pt.x = point.x - rcList.left;
    HitInfo.pt.y = point.y - rcList.top;

    return m_lCom.HitTest(&HitInfo);
}

void MainDlg::OnMouseMove(UINT nFlags, CPoint point){//마우스 이동함수

    if(m_bDragging){
        if(m_pDragImage == NULL) return;

        CPoint ptCursor(point);
        ClientToScreen(&ptCursor);
        m_pDragImage->DragMove(ptCursor);

        INT nTarget = GetHitIndex(point);

        if(nTarget != -1){
            if(nTarget != m_iDragIndex){
                m_pDragImage->DragLeave(NULL);

                m_pDragImage->DragEnter(NULL, ptCursor);
            }
        }
        m_lCom.UpdateWindow();
    }

    CDialog::OnMouseMove(nFlags,point);
}

void MainDlg::OnLButtonUp(UINT nFlags, CPoint point)//마우스 왼쪽을 올릴때
{
    if(m_bDragging){
        if(m_pDragImage == NULL) return;

        m_lCom.SetItemState(m_iDragIndex,0,LVIS_DROPHILITED);

        m_pDragImage->DragLeave(NULL);
        m_pDragImage->EndDrag();

        ReleaseCapture();

        m_bDragging = false;
        delete m_pDragImage;
        m_pDragImage = NULL;

        INT nDest = GetHitIndex(point);
        if(-1 != nDest)
            MoveListItem(m_iDragIndex, nDest);
    }

    CDialog::OnLButtonUp(nFlags, point);
}

void MainDlg::MoveListItem(const INT _nSource, const INT _nDest){//드래그 드랍중 드랍시 위치아이템에 추가
    if((_nSource == _nDest) || (_nDest < 0)) return;

    INT nSource = _nSource;

    INT nColumn = m_lCom.GetItemCount();
    TCHAR Name[256+1] = {0,};

    LVITEM item;
    item.iItem = nSource;
    item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM|LVIF_STATE;
    item.stateMask = LVIS_STATEIMAGEMASK|LVIS_OVERLAYMASK;
    item.pszText = Name;
    item.cchTextMax = 256;
    item.iSubItem = 0;
    m_lCom.GetItem(&item);

    item.iItem = _nDest;
    m_lCom.InsertItem(&item);

    if(nSource > _nDest) nSource++;

    for(INT i = 1; i < nColumn ; i++){
        CString sText = m_lCom.GetItemText(nSource,i);
        m_lCom.SetItemText(_nDest,i,sText);
    }
    m_lCom.DeleteItem(nSource);
}


void MainDlg::OnLvnBegindragListComlist(NMHDR *pNMHDR, LRESULT *pResult)//m_lcom리스트에서 드래그 시작될때
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다

    if(m_lCom.GetSelectedCount() <= 0) return;

    m_iDragIndex = pNMLV->iItem;

    POINT ptImg;
    m_pDragImage = m_lCom.CreateDragImage(m_iDragIndex, &ptImg);
    if(m_pDragImage == NULL) return;

    CPoint ptSpot;
    ptSpot.x = pNMLV->ptAction.x - ptImg.x;
    ptSpot.y = pNMLV->ptAction.y - ptImg.y;
    m_pDragImage->BeginDrag(0,ptSpot);

    ptImg.x = pNMLV->ptAction.x;
    ptImg.y = pNMLV->ptAction.y;
    ClientToScreen(&ptImg);
    m_pDragImage->DragEnter(NULL,ptImg);

    m_bDragging = TRUE;

    m_pDropWnd = m_lCom.GetWindow(GW_OWNER);

    *pResult = 0;

    SetCapture();
}

void MainDlg::OnCbnSelchangeComboAutocreate()//AutoCreate콤보박스를 통해서 명령 선택시 해당 선택에 맞게 명령 자동생성
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int NowSelectAutoCreate = m_comboAutoCreate.GetCurSel();

    if(NowSelectAutoCreate > -1){
        CString itemStr;
        m_comboAutoCreate.GetLBText(NowSelectAutoCreate,itemStr);

        if(itemStr == TEXT("CreateRoom")){
            int NumOfCreateRoom = 7;//명령어 갯수
            int comindex = 0;
            CString *ComName;

            ComName = new CString[NumOfCreateRoom];
            int indexComName = 0;
            ComName[indexComName++] = m_vStaticCommandName[0];//TEXT("Connect");
            ComName[indexComName++] = m_vStaticCommandName[1];//TEXT("Verify");
            ComName[indexComName++] = m_vStaticCommandName[7];//TEXT("EnterChannel");
            ComName[indexComName++] = m_vStaticCommandName[13];//TEXT("CreateRoom");
            ComName[indexComName++] = m_vStaticCommandName[24];//TEXT("LeaveRoom");
            ComName[indexComName++] = m_vStaticCommandName[11];//TEXT("LeaveChannel");
            ComName[indexComName++] = m_vStaticCommandName[28];//TEXT("Disconnect");
            m_lCom.DeleteAllItems();

            for(int i = 0 ; i < indexComName ; i++){
                comindex = m_lCom.GetItemCount();
                m_lCom.InsertItem(comindex,TEXT(""));
                m_lCom.SetItemText(comindex,0,ComName[i]);
                m_lCom.SetItemText(comindex,1,TEXT("1"));
        
                if(ComName[i] == m_vStaticCommandName[7]/*TEXT("EnterChannel")*/){
                    CString itemStr;
        
                    if(m_cCombo1Channel.GetCurSel() >= 0){
                        m_cCombo1Channel.GetLBText(m_cCombo1Channel.GetCurSel(),itemStr);
                        m_lCom.SetItemText(comindex,2,itemStr);
                    }else{
                        MessageBox(TEXT("좌측상단의 채널을 선택해주세요."),TEXT("WRONG"),MB_OK);
                        m_lCom.SetItemText(comindex,2,TEXT("채널"));
                    }
                }else if(ComName[i] == m_vStaticCommandName[13]/*TEXT("CreateRoom")*/){
                    m_lCom.SetItemText(comindex,2,TEXT("일반"));
                }

                comindex++;
            }
        }else if(itemStr == TEXT("BuyItembyGP")){

        }else if(itemStr == TEXT("BuyItembyGem")){

        }else if(itemStr == TEXT("BuyItembyCrystal")){

        }else if(itemStr == TEXT("SellItem")){

        }else if(itemStr == TEXT("RegisterMission")){

        }else if(itemStr == TEXT("CompleteMission")){

        }		
    }
}
