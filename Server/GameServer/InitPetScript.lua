PetHelper = GetPetHelper()
PetHelper:ClearPetFeed()

-- 아이템 아이디, 해당 아이템 하나가 만복 시킬 값
PetHelper:AddPetFeed( 38850, 3000 ) -- 펫용 싸구려 사료
PetHelper:AddPetFeed( 38860, 6000 ) -- 펫용 고급 사료
PetHelper:AddPetFeed( 8880, 200 ) -- 저가 젬
PetHelper:AddPetFeed( 12490, 150 ) -- 초저가 크리스탈
PetHelper:AddPetFeed( 12520, 100 ) -- 저질 빛을 잃은 크리스탈

PetHelper:PrintPetFeed()

--경험치 테이블
--레벨, 승리시 받는 EXP, 패배시 받는 EXP, 레벨의 시작 총 EXP
PetHelper:ClearExpInfo()
PetHelper:AddExpInfo( 0,    4,  2,   100 )
PetHelper:AddExpInfo( 1,    3,  2,   104 )
PetHelper:AddExpInfo( 2,    6,  2,   119 )
PetHelper:AddExpInfo( 3,    9,  3,   152 )
PetHelper:AddExpInfo( 4,   12,  3,   232 )
PetHelper:AddExpInfo( 5,   15,  4,   358 )
PetHelper:AddExpInfo( 6,   18,  5,   566 )
PetHelper:AddExpInfo( 7,   21,  5,   845 )
PetHelper:AddExpInfo( 8,   24,  6,  1241 )
PetHelper:AddExpInfo( 9,   27,  6,  1733 )
PetHelper:AddExpInfo( 10,  30,  7,  2377 )
PetHelper:AddExpInfo( 11,  33,  8,  3142 )
PetHelper:AddExpInfo( 12,  36,  8,  4094 )
PetHelper:AddExpInfo( 13,  39,  9,  5192 )
PetHelper:AddExpInfo( 14,  42,  9,  6512 )
PetHelper:AddExpInfo( 15,  45, 10,  8003 )
PetHelper:AddExpInfo( 16,  48, 11,  9751 )
PetHelper:AddExpInfo( 17,  51, 11, 11695 )
PetHelper:AddExpInfo( 18,  54, 12, 13931 )
PetHelper:AddExpInfo( 19,  57, 12, 16388 )
PetHelper:AddExpInfo( 20,  60, 13, 19172 )
PetHelper:AddExpInfo( 21,  63, 14, 22202 )
PetHelper:AddExpInfo( 22,  66, 14, 25594 )
PetHelper:AddExpInfo( 23,  69, 15, 29257 )
PetHelper:AddExpInfo( 24,  72, 15, 33317 )
PetHelper:AddExpInfo( 25,  75, 16, 37673 )
PetHelper:AddExpInfo( 26,  78, 17, 42461 )
PetHelper:AddExpInfo( 27,  81, 17, 47570 )
PetHelper:AddExpInfo( 28,  84, 18, 53146 )
PetHelper:AddExpInfo( 29,  87, 18, 59068 )
PetHelper:AddExpInfo( 30,  90, 19, 65492 )

PetHelper:AddExpInfo( 31,  93, 20,  72287 )
PetHelper:AddExpInfo( 32,  96, 20,  86795 )
PetHelper:AddExpInfo( 33,  99, 21, 102251 )
PetHelper:AddExpInfo( 34, 102, 21, 118685 )
PetHelper:AddExpInfo( 35, 105, 22, 136127 )
PetHelper:AddExpInfo( 36, 108, 23, 154607 )
PetHelper:AddExpInfo( 37, 111, 23, 174155 )
PetHelper:AddExpInfo( 38, 114, 24, 194801 )
PetHelper:AddExpInfo( 39, 117, 24, 216575 )
PetHelper:AddExpInfo( 40, 120, 25, 239507 )
--PetHelper:AddExpInfo( 41, 123, 26, 168397 )

PetHelper:SortExpInfo() -- 입력 완료후 반드시 호출

-- 만복도 수치 감소량
-- Playtime Min, 감소하는 만복도 수치
PetHelper:ClearDecreseTable()
PetHelper:AddDecreseRate( 5, -10 )
PetHelper:AddDecreseRate( 6, -12 )
PetHelper:AddDecreseRate( 7, -14 )
PetHelper:AddDecreseRate( 8, -16 )
PetHelper:AddDecreseRate( 9, -18 )
PetHelper:AddDecreseRate( 10, -20 )

PetHelper:PrintDecreseSatiation()

-- 만복도 패널티 & 최대치 설정
PetHelper:SetMaxFeed( 6000 )
PetHelper:ClearExpPanaltyTable()
PetHelper:AddExpPanalty( 200, 0.5 )
PetHelper:AddExpPanalty( 0, 0 )
PetHelper:AddExpPanalty( 6000, 1.0 )

PetHelper:SortExpPanaltyTable() -- 입력 완료후 반드시 호출
PetHelper:PrintExpPanaltyTable()

-- 경험치에 도움을 주는 아이템 아이디와 얼만큼 도와 주는지
PetHelper:ClearAssistItemTable()
-- Item ID, Exp 도움 30%
--PetHelper:AddAssistItem( 8880, 1.3 )
PetHelper:AddAssistItem( 38870, 1.5 )
PetHelper:AddAssistItem( 38880, 1.5 )

PetHelper:PrintAssistItemTable()
--진화 상태에 따른 패널티.
PetHelper:ClearPromotionPanaltyTable()
PetHelper:AddPromotionPanalty( 0, 1.0 )
PetHelper:AddPromotionPanalty( 1, 0.8 )
PetHelper:AddPromotionPanalty( 2, 0.7 )
PetHelper:PrintPromotionPanaltyTable()

-- Pet Evolution/Devolution Item Info...
PetHelper:ClearTransformItem()
PetHelper:AddTransformItem( 42370, 0, 50, 0 ) -- 진화, 마지막 인수는 0
PetHelper:AddTransformItem( 42390, 0, 50, 0 )
PetHelper:AddTransformItem( 43480, 20, 50, 0 )
PetHelper:AddTransformItem( 43490, 40, 50, 0 )

PetHelper:AddTransformItem( 42380, 0, 50, 1 ) -- 퇴화 , 마지막 인수는 1
PetHelper:AddTransformItem( 42400, 0, 50, 1 )

PetHelper:ClearHatchingInfo()
-- 알펫, 진화 레벨, 부화 후 펫
PetHelper:AddHatchingInfo( 43510, 7, 42360 )  -- 고르고스의 알 ->   곤 봉인카드
PetHelper:AddHatchingInfo( 43520, 7, 38820 )  -- 페페의 알     ->   페페 봉인카드
PetHelper:AddHatchingInfo( 43530, 7, 38830 )  -- 시트의 알     ->   시트 봉인카드
PetHelper:AddHatchingInfo( 43540, 7, 43120 )  -- 슬라임의 알   ->   슬라임 봉인카드
PetHelper:AddHatchingInfo( 78500, 7, 79050 )  -- 큐피드의알   ->   큐피드 봉인카드

PetHelper:PrintHatchingInfo()

-- 펫 코스튬
PetHelper:ClearPetCostume()
PetHelper:AddPetCostume( 42350, 0, 107980 ) -- 뉴닌코
PetHelper:AddPetCostume( 38810, 0, 105110 ) -- 색상 바뀐 루나 (CD용)
PetHelper:AddPetCostume( 38840, 0, 105120 ) -- 색상 바뀐 블루페어리 (CD용)
PetHelper:AddPetCostume( 96240, 0, 105130 ) -- 색상 바뀐 펭펭 (CD용)
PetHelper:AddPetCostume( 79060, 0, 112900 ) -- 뉴밍밍

