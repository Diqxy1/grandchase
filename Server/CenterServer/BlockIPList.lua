TRUE = 1
FALSE = 0
IPList = {
    -- TRUE  : ��ġ�ð� �϶�. ���� ����� ������ IP��
    -- FALSE : ��ġ�ð� �ƴҶ�.
    IsPatchTime = FALSE,

    -- Block IP üũ�� ���� ����
    CheckEnable = FALSE,

    -- ���IP üũ����
    AdminIPCheck = FALSE,

    Condition = {
    -- CC, Old User, Block = [ ���� �ڵ�,  ���� ������ , ������ ]
    -- �ʼ� ���, OUTBOUND, INBOUND
        { "OUTBOUND", FALSE, FALSE },
        { "INBOUND", FALSE, TRUE },
    },

    -- ��� ��� IP
    AdminIPRange = {
        { "116.120.238.0", "116.120.238.255", "KOG" },
    },

    -- Block ����IP
    ExceptIPRange = {
        { "116.120.238.0", "116.120.238.255", "KOG" },
        { "221.148.85.234", "221.148.85.234", "cj" },
    },

    IPBlockFile = "BlockIPList.txt",
	
    -- BlockIP Country Code
    BlockCountryCode = { },

    -- Exception UserID ��� ����
    CheckExceptionUserID = FALSE,
    -- Exception UserID
    ExceptionUserID = { },
}

