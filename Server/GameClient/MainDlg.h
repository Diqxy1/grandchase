#pragma once

#include "stdafx.h"
#include <afxole.h>
#include <afxpriv.h>
#include <WinSock2.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <KncLua.h>
#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#include "UserProxy.h"
#include "RobotManager.h"

#include "luabind/luabind.hpp"
#include "resource2.h"

#pragma comment(lib, "iphlpapi.lib" )


SmartPointer( MainDlg );
class MainDlg : public CDialog
{
    DECLARE_DYNAMIC(MainDlg)

public:
    MainDlg(CWnd* pParent = NULL);
    virtual ~MainDlg();

    enum { IDD = IDD_DIALOG1 };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    
    std::map<int,std::vector<int>> clientcomque;//클라당명령
    std::vector<int> comque;//명령
    Botparam pkMgrParam;//매시브에서 넘겨주는 파라미터 변수
    std::map<int,CString> m_vStaticCommandName;//클라이언트에게 줄 모든 명령어와 명령어상수를 담고있을 변수

    KRobotManager* pkMgr;

    dpParam singleParam;//서버 ip, port등과 같이 기본적인 정보를 지정해주는 변수
    //이후에 robotmanager의 SetMassiveRobotNum()메소드 호출시 매개변수로 념겨준다.

    std::vector<KChannelInfo> ChannelNamenId;

    CWinThread* m_tBotThread;
    CWinThread* m_tBotCheckThread; 

    int NumMassive;
    int NumMassiveClient;
    int *Array_Massive;//사용자의 입력에 의해 동적할당되는 매시브컨트롤의 갯수저장
    
    bool m_bStartBotTest;

    int **m_iNumMaxCom;//각각의 클라이언트당의 명령어 갯수를 세기위한 변수
    int ***m_iNumCom;//각각의 클라이언트 명령어의 실제 세분화된 갯수셈
    bool m_bFlagSetBotWork;

    int m_iNumMaxComofAllClient;//가장 긴 명령어갯수를 파악하기 위해
    std::vector<CString> m_vecPrintComName;

    int m_iIndexComListChoise;//명령어 리스트에서 선택된 인덱스

    int m_iDragIndex;//m_lcom리스트에서 드래그시 처음 드래그적용 인덱스용 변수
    int m_idropIndex;//m_lcom리스트에서 드랍시 적용 인덱스용 변수
    CImageList *m_pDragImage;//m_lcom리스트에서드래그시 이미지위한 변수
    bool m_bDragging;//m_lcom리스트에서현재 드래그중인지를 나타내는 변수
    CWnd *m_pDropWnd;//m_lcom리스트에서 사용하느 변수

    UINT m_cfData;//m_lCom리스트에서 드래그시 사용되는 변수 클립보드 포맷을 나타내는 값
    //아래변수들은 루아를 통해 초기화되는 변수들
    static std::map<int,std::string> m_vStaticCommandNameTest;//루아 파일을 통해 임시로 설정되어 m_vstaticCommandName으로 초기화된다.
    static int						 ms_iProtocolVer;
    static std::string				 ms_cIP;
    static unsigned short			 ms_usPort;
    static std::string				 ms_cMsgIP;
    static unsigned short			 ms_usMsgPort;
    static std::string				 ms_cMsgIP2nd;
    static unsigned short			 ms_usMsgPort2nd;
    static DWORD					 ms_dTestMsgTime;
    static bool						 ms_bAutoSelectChannel;
    static int						 ms_iNumMassive;
    static int						 ms_iNumMassiveClient;
    static std::string				 ms_sClientIDStart;//사용자들의 아이디 초기 시작스트링 부분
    //예를 들어 test0001에서 'test'에 해당하는 부분
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1Save();
    CComboBox m_cCombo1Channel;
    bool FirstRunCheckServerConnectChannel();
    afx_msg void OnCbnSelendokCombo1();
    afx_msg void OnBnClickedButtonMassive();
    CComboBox m_cCombo2MassiveClient;
    afx_msg void OnBnClickedButtonOk();
    CListCtrl m_lNoOfMassiveAndClient;//각각의 매시이름과 매시당 클라수를 보여줌
    CListCtrl m_lDetailMassive;//매시를 선택시 매시의 정보를 보여줌
    CListCtrl m_lResult;//현재 돌아가고있는 시스템의 상태. 매시와 클라의 정보를 보여줌
    CListCtrl m_lChoise;//클라이언트와 매시의 작업 설정에 사용됨
    CListCtrl m_lCom;//결과 지정 리스트
    afx_msg void OnNMClickList1Noofmassiveandclient(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnEndlabeleditList1Detailmassive(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButton2Totalsubmite();
    afx_msg void OnBnClickedButton3Massivesubmite();
    afx_msg void OnBnClickedButton4Clientsubmite();
    afx_msg void OnBnClickedButton5Clear();
    afx_msg void OnBnClickedButton2Run();
    void StartBot();
    void ClearParam();
    void SetBotWork();
    bool SetChoiseList();
    CComboBox m_comboCom;
    CComboBox m_comboAutoCreate;
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonCom();
    bool Setm_iNumCom(int firstStart, int firstEnd, int SecondStart, int SecondEnd, int ComCount);
    bool Setcommandque(std::vector<int> *comque, int m_iHeadCount, CString *ChannelName_1, int *RoomMode);
    bool Setcommandque_MassiveClient(int FirstStart, int FirstEnd, int SecondStart, int SecondEnd, std::vector<int> comque1, std::map<int,std::vector<int>> clientcomque1,CString ChannelName_1,int RoomMode);
    afx_msg void OnBnClickedButtonDelete();
    bool SetDlgItemID_Text();//ip,포드번호 Dlg에 사용되는 기본적인 텍스트와 그에 대한 id를 지정하는 총괄적인 함수
    //아래는 새부적으로 초기화하는 부분
    static void SetCommandID_Name(int id, std::string name){ m_vStaticCommandNameTest[id] = name; }//명령지정시 사용되는 명령어와 상수값을 지정하는 부분
    static void SetProtocolVer(int inProtocolVer){ ms_iProtocolVer = inProtocolVer;}
    static void Setms_cIP(std::string inIP){ms_cIP = inIP;}
    static void Setms_usPort(unsigned short inPort){ms_usPort = inPort;}
    static void Setms_cMsgIP(std::string inMsgIP){ms_cMsgIP = inMsgIP;}
    static void Setms_usMsgPort(unsigned short inMsgPort){ms_usMsgPort = inMsgPort;}
    static void Setms_cMsgIP2nd(std::string inMsgIP2nd){ms_cMsgIP2nd = inMsgIP2nd;}
    static void Setms_usMsgPort2nd(unsigned short inMsgPort2nd){ms_usMsgPort2nd = inMsgPort2nd;}
    static void Setms_dTestMsgTime(int inMsgTime){ms_dTestMsgTime = inMsgTime;}
    static void SetAutoSelectChennel(bool inflag){ms_bAutoSelectChannel = inflag;}
    static void SetMassiveNum(int inNumberOfMassive){ms_iNumMassive = inNumberOfMassive;}
    static void SetClientNum(int inNumberOfClient){ms_iNumMassiveClient = inNumberOfClient;}
    static void SetClientNameStartKey(std::string inNameStart){ms_sClientIDStart = inNameStart;}

    bool SetLuaBasicSetting();//루아를 통해서 다이얼로그중 채널 선택과 매시숫자 클라숫자를 초기화하는 부분

    afx_msg void OnLvnBegindragListComlist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    INT GetHitIndex(CPoint point);
    void MoveListItem(const INT _nSource, const INT _nDest);
    afx_msg void OnNMClickListComlist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeComboAutocreate();
    afx_msg void OnBnClickedButtonClearparam();
};

unsigned int ThreadFuncBotStart(LPVOID dParameter);
unsigned int ThreadFuncBotCheckCom(LPVOID dParameter);
