--[[
�Ʒ��� ���� ������� robot�� ���� �����κ�
ip��� ���� �⺻������
Dlg�� ���Ǵ� TEXT�� ������� �����ϴ� �κ�

 luabind::def("SetCommandIdName",&MainDlg::SetCommandID_Name), - ��ɾ�� ������ ����� �����ϴ� �κ� RobotAction_def.h���Ͽ� �����ִ� �׼ǰ���� ��Ī�Ǿ���
 luabind::def("SetProtocolVer",&MainDlg::SetProtocolVer),-�������� ������ �ʱ�ȭ�ϴ� �κ�
 luabind::def("SetmscIP",&MainDlg::Setms_cIP),-ip�� �ʱ�ȭ�ϴ� �κ�
 luabind::def("SetmsusPort",&MainDlg::Setms_usPort),-��Ʈ�� �ʱ�ȭ�ϴºκ�
 luabind::def("SetmscMsgIP",&MainDlg::Setms_cMsgIP),
 luabind::def("SetmsusMsgPort",&MainDlg::Setms_usMsgPort),
 luabind::def("SetmscMsgIP2nd",&MainDlg::Setms_cMsgIP2nd),
 luabind::def("SetmsusMsgPort2nd",&MainDlg::Setms_usMsgPort2nd),
 luabind::def("SetmsdTestMsgTime",&MainDlg::Setms_dTestMsgTime),-���ð��� �ʱ�ȭ

 luabind::def("SetAutoSelectChannel",&MainDlg::SetAutoSelectChennel),
 luabind::def("SetMassiveNum",&MainDlg::SetMassiveNum),
 luabind::def("SetClientNum",&MainDlg::SetClientNum),

 luabind::def("SetClientNameStartKey",&MainDlg::SetClientNameStartKey)

 ��Ƹ� ���� ���ε��� �Լ��� ����� ���� ����. ���� �Լ��鸸 ��밡���ϴ�. ���� ������ ���� �Լ��� ����Ұ�� ��� ���� ��ü�� �ε���� ���Ѵ�.

]]--

--�ʱ� �ʱ�ȭ�κ�
--singleParam.m_cIP = NULL;
--singleParam.m_cMsgIP = NULL;;
--singleParam.m_cMsgIP2nd = NULL;
--singleParam.m_dTestMsgTime = 0;
--singleParam.m_iProtocolVer = -1;
--singleParam.m_usMsgPort = 0;
--singleParam.m_usMsgPort2nd = 0;
--singleParam.m_usPort = 0;//�ʱ�ȭ
--���Ͱ��� �ʱ�ȭ������, �����ǿ� �޽����� ���ο��� �Ǵٽ� null���� Ȯ���Ͽ� 128.0.0.1�� �޽����� 192.168.199.104�� �ʱ�ȭ�ȴ�. ��Ʈ��ȣ�� �ʱ�ȭ�ȴ�

SetProtocolVer(328)
SetmscIP('192.168.199.104')
SetmsusPort(9400)
SetmscMsgIP('192.168.199.104')
SetmsusMsgPort(9300)
SetmscMsgIP2nd('192.168.199.104')
SetmsusMsgPort2nd(9310)
SetmsdTestMsgTime(1500)






--RobotAction_def.h ���Ͽ� �����ִ� �׼ǰ���� ��Ī�� �Ǿ�� �մϴ�.

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






--�Ʒ����� �⺻ ����Ʈ�� 'test'�̴�. �� ���� ������ ������ ������ 'test'�� �ʱ�ȭ�ȴ�.
--�̺κ��� Ŭ���̾�Ʈ�� ���̵���ۺκ��� ���ϴ� �κ��̴�. 'test'�� �ʱ�ȭ�� Ŭ���̾�Ʈ�� ���̵�� 'test0001, test0002'������ �ʱ�ȭ�� �̷������ �ȴ�.

SetClientNameStartKey('test')






--�ڵ� ���� ������

SetAutoSelectChannel(true)
SetMassiveNum(10)
SetClientNum(10)
