
VERSION = 1  -- 가위바위보 이벤트 버전. ( 새로 진행할 때마다 버전을 1씩 올려야 한다. 보상을 바꾸고 패치하는 경우에도 +1 해야한다. )

BeginTime   = { 2010, 10, 06, 09 }   -- Y, M, D, H
EndTime     = { 2010, 10, 28, 09 }   -- Y, M, D, H

DAILY_FREE = 5            -- 하루 최대 지급 포인트
FREE_TICK_GAP = 1        -- X 분마다 포인트 지급
RETRY_POINT_PER_PLAY = 3  -- X 번의 플레이마다 재도전 포인트 

--TICK_SYNC_GAP = 1  -- 포인트 지급 시간 간격
RETRY_POSITION_VALUE = 128  -- 패배시 말 위치에 재도전 표시할 값

TRY_POINT_ITEM = 1
RETRY_POINT_ITEM = 2

TryPointItemTrade = { 413430, 1 }  -- 도전 포인트 교환 아이템, 포인트 수
RetryPointItemTrade = { 413440, 1 } -- 재도전 포인트 교환 아이템, 포인트 수

DEFAULT_REWARD_STAGE = 0  -- 클라이언트로 전달(초기 정보 전송시에)

LoseReward = 
{
    { 58710, 1, -1 },
    { 58720, 1, -1 },
}

WinReward =
{
    {
        STAGE = 0,
        FINAL_BOSS = 1,
        { 
            BOSS = 0,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
        { 
            BOSS = 1,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
    },
    {
        STAGE = 1,
        FINAL_BOSS = 1,
        { 
            BOSS = 0,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
        { 
            BOSS = 1,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
    },

    {
        STAGE = 2,
        FINAL_BOSS = 1,
        { 
            BOSS = 0,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
        { 
            BOSS = 1,
            Win = 33,
            Lose = 33,
            ItemList = 
	    {
            { 8880, 1, -1, 50 },
		    { 413440, 1, -1, 50 },

	    }
        },
    },
    

}
