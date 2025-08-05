-- 출석부 시스템 설정
TRUE = 1
FALSE = 0

GCPointSwitch = TRUE -- GCPoint 시스템 사용여부(미적용 국가용).
AccUpdateTime = 60 * 1000 -- 출석 누적시간 갱신주기(ms)
AttendTime = 70 -- 출석 인정시간(분)
GCPointRecommend = 0 -- 추천 받았을때 GP 포인트 지급 (미사용)

-- 1일 출석시 지급되는 GC포인트
GCPointDay = {
    Default = 100, -- 기본획득GCPoint
	Event = {
		-- 특정기간GCPoint
		-- { startdate, enddate, gcpoint },
        { {2011,6,3,00,00}, {2011,6,6,23,59}, 300 },
	},
}

-- 주개근시 지급되는 GC포인트
GCPointWeek = {
	Default = 1000, -- 기본획득GCPoint
	Event = {
		-- 특정주 GCPoint
		-- { Month, WeekID, gcpoint },
        -- { 6, 1, 2000 }, -- test
	},
}

-- 출석부오픈시 일일지급 GC포인트
GCPointOpenCalendar = {
	Default = 100, -- 기본획득GCPoint
	Event = {
		-- 특정기간GCPoint
		-- { startdate, enddate, gcpoint },
        -- { {2011,6,3,00,00}, {2011,12,29,23,59}, 500 }, -- test
	},
}

