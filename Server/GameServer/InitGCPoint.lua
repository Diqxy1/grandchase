-- �⼮�� �ý��� ����
TRUE = 1
FALSE = 0

GCPointSwitch = TRUE -- GCPoint �ý��� ��뿩��(������ ������).
AccUpdateTime = 60 * 1000 -- �⼮ �����ð� �����ֱ�(ms)
AttendTime = 70 -- �⼮ �����ð�(��)
GCPointRecommend = 0 -- ��õ �޾����� GP ����Ʈ ���� (�̻��)

-- 1�� �⼮�� ���޵Ǵ� GC����Ʈ
GCPointDay = {
    Default = 100, -- �⺻ȹ��GCPoint
	Event = {
		-- Ư���ⰣGCPoint
		-- { startdate, enddate, gcpoint },
        { {2011,6,3,00,00}, {2011,6,6,23,59}, 300 },
	},
}

-- �ְ��ٽ� ���޵Ǵ� GC����Ʈ
GCPointWeek = {
	Default = 1000, -- �⺻ȹ��GCPoint
	Event = {
		-- Ư���� GCPoint
		-- { Month, WeekID, gcpoint },
        -- { 6, 1, 2000 }, -- test
	},
}

-- �⼮�ο��½� �������� GC����Ʈ
GCPointOpenCalendar = {
	Default = 100, -- �⺻ȹ��GCPoint
	Event = {
		-- Ư���ⰣGCPoint
		-- { startdate, enddate, gcpoint },
        -- { {2011,6,3,00,00}, {2011,12,29,23,59}, 500 }, -- test
	},
}

