TRUE = 1
FALSE = 0
IPList = {
    -- TRUE  : 패치시간 일때. 예외 빼고는 무조건 IP블럭
    -- FALSE : 패치시간 아닐때.
    IsPatchTime = FALSE,

    -- Block IP 체크를 할지 여부
    CheckEnable = FALSE,

    -- 운영자IP 체크여부
    AdminIPCheck = FALSE,

    Condition = {
    -- CC, Old User, Block = [ 국가 코드,  기존 유저도 , 막을지 ]
    -- 필수 요소, OUTBOUND, INBOUND
        { "OUTBOUND", FALSE, FALSE },
        { "INBOUND", FALSE, TRUE },
    },

    -- 운영자 사용 IP
    AdminIPRange = {
        { "116.120.238.0", "116.120.238.255", "KOG" },
    },

    -- Block 예외IP
    ExceptIPRange = {
        { "116.120.238.0", "116.120.238.255", "KOG" },
        { "221.148.85.234", "221.148.85.234", "cj" },
    },

    IPBlockFile = "BlockIPList.txt",
	
    -- BlockIP Country Code
    BlockCountryCode = { },

    -- Exception UserID 사용 여부
    CheckExceptionUserID = FALSE,
    -- Exception UserID
    ExceptionUserID = { },
}

