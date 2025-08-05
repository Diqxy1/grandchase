SHCheckPoint = GetSHCheckPoint()
SHCheckPoint:ClearAll()

CheckPoint = { 
   ID = 2,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 1,      -- 받는 레벨
   ItemID = 44030,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 1,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 1,      -- 받는 레벨
   ItemID = 44020,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 3,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 1,      -- 받는 레벨
   ItemID = 44040,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 4,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 1,      -- 받는 레벨
   ItemID = 44050,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 5,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 1,      -- 받는 레벨
   ItemID = 44060,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 6,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 2,      -- 받는 레벨
   ItemID = 44070,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 7,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 2,      -- 받는 레벨
   ItemID = 44080,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 8,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 2,      -- 받는 레벨
   ItemID = 44090,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 9,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 2,      -- 받는 레벨
   ItemID = 44100,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 10,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 2,      -- 받는 레벨
   ItemID = 44110,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 11,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 3,      -- 받는 레벨
   ItemID = 44170,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 12,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 3,      -- 받는 레벨
   ItemID = 44180,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 13,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 3,      -- 받는 레벨
   ItemID = 44190,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 14,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 3,      -- 받는 레벨
   ItemID = 44200,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 15,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 3,      -- 받는 레벨
   ItemID = 44210,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 16,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44220,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 17,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44230,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 18,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44240,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 19,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44250,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 20,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44260,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 21,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44740,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 22,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44750,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 23,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44760,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 24,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44770,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 25,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 4,      -- 받는 레벨
   ItemID = 44780,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 26,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 5,      -- 받는 레벨
   ItemID = 44270,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 27,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 5,      -- 받는 레벨
   ItemID = 44280,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 28,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 5,      -- 받는 레벨
   ItemID = 44290,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 29,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 5,      -- 받는 레벨
   ItemID = 44300,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 30,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 5,      -- 받는 레벨
   ItemID = 44310,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 31,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44790,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 32,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44800,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 33,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44810,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 34,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44820,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 35,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44830,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 36,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44320,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 37,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44330,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 38,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44340,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 39,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44350,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 40,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44360,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 41,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44370,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 42,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44380,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 43,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44390,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 44,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44400,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 45,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 6,      -- 받는 레벨
   ItemID = 44410,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 46,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44470,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 47,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44480,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 48,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44490,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 49,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44500,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 50,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44510,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 51,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44840,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 52,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44850,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 53,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44860,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 54,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44870,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 55,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 7,      -- 받는 레벨
   ItemID = 44880,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 56,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44520,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 57,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44530,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 58,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44540,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 59,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44550,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 60,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44560,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 61,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44420,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 62,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44430,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 63,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44440,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 64,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44450,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 65,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 8,      -- 받는 레벨
   ItemID = 44460,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 66,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 9,      -- 받는 레벨
   ItemID = 44890,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 67,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 9,      -- 받는 레벨
   ItemID = 44900,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 68,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 9,      -- 받는 레벨
   ItemID = 44910,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 69,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 9,      -- 받는 레벨
   ItemID = 44920,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 70,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 9,      -- 받는 레벨
   ItemID = 44930,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 71,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44570,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 72,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44580,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 73,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44590,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 74,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44600,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 75,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44610,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 76,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 11,      -- 받는 레벨
   ItemID = 44940,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 77,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 11,      -- 받는 레벨
   ItemID = 44950,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 78,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44960,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 79,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44970,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 80,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 10,      -- 받는 레벨
   ItemID = 44980,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 81,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 13,      -- 받는 레벨
   ItemID = 44990,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 82,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 13,      -- 받는 레벨
   ItemID = 45000,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 83,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 13,      -- 받는 레벨
   ItemID = 45010,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 84,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 13,      -- 받는 레벨
   ItemID = 45020,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 85,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 13,      -- 받는 레벨
   ItemID = 45030,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 86,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 15,      -- 받는 레벨
   ItemID = 45040,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 87,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 15,      -- 받는 레벨
   ItemID = 45050,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 88,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 15,      -- 받는 레벨
   ItemID = 45060,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 89,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 15,      -- 받는 레벨
   ItemID = 45070,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 90,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 15,      -- 받는 레벨
   ItemID = 45080,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 91,         -- 중첩되지 않는 순차적인 번호
   charID = 0,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 17,      -- 받는 레벨
   ItemID = 45090,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 92,         -- 중첩되지 않는 순차적인 번호
   charID = 1,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 17,      -- 받는 레벨
   ItemID = 45100,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 93,         -- 중첩되지 않는 순차적인 번호
   charID = 2,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 17,      -- 받는 레벨
   ItemID = 45110,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 94,         -- 중첩되지 않는 순차적인 번호
   charID = 3,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 17,      -- 받는 레벨
   ItemID = 45120,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

CheckPoint = { 
   ID = 95,         -- 중첩되지 않는 순차적인 번호
   charID = 4,     -- 캐릭터 번호 (-1 = 베스트캐릭, 0 = 엘리시스, 1=리르, 2=아르메, 3=라스, 4=라이언 )
   level = 17,      -- 받는 레벨
   ItemID = 32520,     -- 아이템 아이디
   factor = -1,     -- 영구 = -1, 기간이나 수량이면 해당 숫자
   regist = true ,  -- 미션 스크롤인경우 바로 등록해야 하면 true, 그냥 받으면 false
}

SHCheckPoint:AddCheckCondition( CheckPoint )

