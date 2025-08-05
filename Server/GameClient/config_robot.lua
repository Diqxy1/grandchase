--[[
아래는 새로 만들어진 robot을 위한 설정부분
ip등과 같은 기본정보와
Dlg에 사용되는 TEXT와 상수값을 지정하는 부분

 luabind::def("SetCommandIdName",&MainDlg::SetCommandID_Name), - 명령어와 각각의 상수를 지정하는 부분 RobotAction_def.h파일에 나와있는 액션값들과 매칭되야함
 luabind::def("SetProtocolVer",&MainDlg::SetProtocolVer),-프로토콜 버전을 초기화하는 부분
 luabind::def("SetmscIP",&MainDlg::Setms_cIP),-ip를 초기화하는 부분
 luabind::def("SetmsusPort",&MainDlg::Setms_usPort),-포트를 초기화하는부분
 luabind::def("SetmscMsgIP",&MainDlg::Setms_cMsgIP),
 luabind::def("SetmsusMsgPort",&MainDlg::Setms_usMsgPort),
 luabind::def("SetmscMsgIP2nd",&MainDlg::Setms_cMsgIP2nd),
 luabind::def("SetmsusMsgPort2nd",&MainDlg::Setms_usMsgPort2nd),
 luabind::def("SetmsdTestMsgTime",&MainDlg::Setms_dTestMsgTime),-대기시간을 초기화

 luabind::def("SetAutoSelectChannel",&MainDlg::SetAutoSelectChennel),
 luabind::def("SetMassiveNum",&MainDlg::SetMassiveNum),
 luabind::def("SetClientNum",&MainDlg::SetClientNum),

 luabind::def("SetClientNameStartKey",&MainDlg::SetClientNameStartKey)

 루아를 통해 바인딩된 함수들 목록은 위와 같다. 위의 함수들만 사용가능하다. 만약 허용되지 않은 함수를 사용할경우 루아 파일 전체가 로드되지 못한다.

]]--

--초기 초기화부분
--singleParam.m_cIP = NULL;
--singleParam.m_cMsgIP = NULL;;
--singleParam.m_cMsgIP2nd = NULL;
--singleParam.m_dTestMsgTime = 0;
--singleParam.m_iProtocolVer = -1;
--singleParam.m_usMsgPort = 0;
--singleParam.m_usMsgPort2nd = 0;
--singleParam.m_usPort = 0;//초기화
--위와같이 초기화되지만, 아이피와 메신저는 내부에서 또다시 null임을 확인하여 128.0.0.1과 메신저는 192.168.199.104로 초기화된다. 포트번호도 초기화된다

SetProtocolVer(328)
SetmscIP('192.168.199.104')
SetmsusPort(9400)
SetmscMsgIP('192.168.199.104')
SetmsusMsgPort(9300)
SetmscMsgIP2nd('192.168.199.104')
SetmsusMsgPort2nd(9310)
SetmsdTestMsgTime(1500)






--RobotAction_def.h 파일에 나와있는 액션값들과 매칭이 되어야 합니다.

SetCommandIdName(0,'Connect')
SetCommandIdName(1,'Verify')
SetCommandIdName(7,'EnterChannel')
SetCommandIdName(11,'LeaveChannel')
SetCommandIdName(13,'CreateRoom')
SetCommandIdName(15,'BuyItemForGP')
SetCommandIdName(16,'BuyItemForGem')
SetCommandIdName(17,'BuyItemForCrystal')
SetCommandIdName(21,'SellItem')
SetCommandIdName(24,'LeaveRoom')
SetCommandIdName(28,'Disconnect')
SetCommandIdName(100,'RegisterMission')
SetCommandIdName(101,'CompleteMission')






--아래값의 기본 디폴트는 'test'이다. 즉 따로 설정을 해주지 않으면 'test'로 초기화된다.
--이부분은 클라이언트의 아이디시작부분을 정하는 부분이다. 'test'로 초기화시 클라이언트의 아이디는 'test0001, test0002'등으로 초기화가 이루어지게 된다.

SetClientNameStartKey('test')






--자동 설정 지정부

SetAutoSelectChannel(true)
SetMassiveNum(10)
SetClientNum(10)
