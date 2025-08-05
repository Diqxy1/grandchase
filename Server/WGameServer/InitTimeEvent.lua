EventList =
{
    -- Sample
--[[
    {
        -- std::pair<EventID,CharType> prKey
        EventID = 49,
        CharType = -1,
        
        -- Custum Data(can't set in D-bot)
        ItemIDA = {
            {
                charType = 0,
                Equips = { 6030,6040,6050,6060,6070,6080, },
            }
        },

        ItemIDB = {
            {
                charType = 1,
                Equips = { 6090,6100,6110,6120,6130,6140, },
            }
        },
        SetModeList = {},
        PartyNum = -1,
        ActiveRate = 1.0,
    },
]]--
	-- ���� �Բ� �ϸ�..
	{
    	EventID = 72,     -- Dbot�� ���� ����(�ĺ�Ű)
	    CharType = 7,   -- �� ĳ����(7:��)�� ���� �÷����� ��Ƽ���� ���ʽ��� �޴´�. Dbot�� ���� ����(�ĺ�Ű)
	    ItemIDA = {},
	    ItemIDB = {},
	    SetModeList = {},
	    SetMapList = {},
	    PartyNum = -1,
	    -- ��Ƽ��(�� ����)�� 50������ Exp/Gp���ʽ��� �޴´�.
	    -- ���� Dbot���� ������ ��ġ�� ���ʽ��� �޴´�.(=���� ��Ƽ���� ���ʽ��� �ٸ��� ���� �����ϴ�)
	    PartyExpBonus = 0.5,
	    PartyGpBonus = 0.5,
	    ActiveRate = 1.0,
	},
    
}

