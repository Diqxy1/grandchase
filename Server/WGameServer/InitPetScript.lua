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
PetHelper:AddExpInfo( 0,12,6,100 )
PetHelper:AddExpInfo( 1,9,6,104 )
PetHelper:AddExpInfo( 2,18,6,119 )
PetHelper:AddExpInfo( 3,27,9,152 )
PetHelper:AddExpInfo( 4,36,9,232 )
PetHelper:AddExpInfo( 5,45,12,358 )
PetHelper:AddExpInfo( 6,54,15,566 )
PetHelper:AddExpInfo( 7,63,15,845 )
PetHelper:AddExpInfo( 8,72,18,1241 )
PetHelper:AddExpInfo( 9,81,18,1733 )
PetHelper:AddExpInfo( 10,90,21,2377 )
PetHelper:AddExpInfo( 11,99,24,3142 )
PetHelper:AddExpInfo( 12,108,24,4094 )
PetHelper:AddExpInfo( 13,117,27,5192 )
PetHelper:AddExpInfo( 14,126,27,6512 )
PetHelper:AddExpInfo( 15,135,30,8003 )
PetHelper:AddExpInfo( 16,144,33,9751 )
PetHelper:AddExpInfo( 17,153,33,11695 )
PetHelper:AddExpInfo( 18,162,36,13931 )
PetHelper:AddExpInfo( 19,171,36,16388 )
PetHelper:AddExpInfo( 20,180,39,19172 )
PetHelper:AddExpInfo( 21,189,42,22202 )
PetHelper:AddExpInfo( 22,198,42,25594 )
PetHelper:AddExpInfo( 23,207,45,29257 )
PetHelper:AddExpInfo( 24,216,45,33317 )
PetHelper:AddExpInfo( 25,225,48,37673 )
PetHelper:AddExpInfo( 26,234,51,42461 )
PetHelper:AddExpInfo( 27,243,51,47570 )
PetHelper:AddExpInfo( 28,252,54,53146 )
PetHelper:AddExpInfo( 29,261,54,59068 )
PetHelper:AddExpInfo( 30,270,57,65492 )
PetHelper:AddExpInfo( 31,279,60,72287 )
PetHelper:AddExpInfo( 32,288,60,86795 )
PetHelper:AddExpInfo( 33,297,63,102251 )
PetHelper:AddExpInfo( 34,306,63,118685 )
PetHelper:AddExpInfo( 35,315,66,136127 )
PetHelper:AddExpInfo( 36,324,69,154607 )
PetHelper:AddExpInfo( 37,333,69,174155 )
PetHelper:AddExpInfo( 38,342,72,194801 )
PetHelper:AddExpInfo( 39,351,72,216575 )
PetHelper:AddExpInfo( 40,360,75,239507 )
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
PetHelper:AddPetCostume( 197370, 0, 378470 ) -- GWC2010 기사단장

-- 펫 귀속&공용 구분
PetHelper:ClearPetVestedItem()
PetHelper:AddPetVestedItem( 657800 )