
VERSION = 1  -- ���������� �̺�Ʈ ����. ( ���� ������ ������ ������ 1�� �÷��� �Ѵ�. ������ �ٲٰ� ��ġ�ϴ� ��쿡�� +1 �ؾ��Ѵ�. )

BeginTime   = { 2010, 10, 06, 09 }   -- Y, M, D, H
EndTime     = { 2010, 10, 28, 09 }   -- Y, M, D, H

DAILY_FREE = 5            -- �Ϸ� �ִ� ���� ����Ʈ
FREE_TICK_GAP = 1        -- X �и��� ����Ʈ ����
RETRY_POINT_PER_PLAY = 3  -- X ���� �÷��̸��� �絵�� ����Ʈ 

--TICK_SYNC_GAP = 1  -- ����Ʈ ���� �ð� ����
RETRY_POSITION_VALUE = 128  -- �й�� �� ��ġ�� �絵�� ǥ���� ��

TRY_POINT_ITEM = 1
RETRY_POINT_ITEM = 2

TryPointItemTrade = { 413430, 1 }  -- ���� ����Ʈ ��ȯ ������, ����Ʈ ��
RetryPointItemTrade = { 413440, 1 } -- �絵�� ����Ʈ ��ȯ ������, ����Ʈ ��

DEFAULT_REWARD_STAGE = 0  -- Ŭ���̾�Ʈ�� ����(�ʱ� ���� ���۽ÿ�)

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
