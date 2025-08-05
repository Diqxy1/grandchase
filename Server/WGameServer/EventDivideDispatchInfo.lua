--[[
DB에 한꺼번에 많은 요청이 몰리는 Just In Time 이벤트를 위해 개발되었다
일정 시간마다 DB 큐 크기를 확인하여 유저 스레드에서 DB 스레드로 큐잉하는
이벤트를 조절하여 순서대로 전달하는 기능을 한다

현재 유저 스레드에서 DB 스레드에 전달하는 부분이 구현되어 있다
반대의 경우는 미 구현 상태이다
--]]


-- UserEvent_def.h의 이벤트 값과 매칭시키는 작업을 코드에서 따로 해줘야 한다
DB_EVENT_JUST_IN_TIME_REWARD_REQ = 0

EventInfo =
{
    {
        ProcessTimeInterval = 5 * 1000,
        EventID = "DB_EVENT_JUST_IN_TIME_REWARD_REQ",
        QueueSizeThreshold = 600, -- 추가 큐잉을 위한 조건(여기서 지정된 DB queue size 를 초과하지 않게 추가큐잉함)
        ProcessCount = 12, -- 한번에 처리할 최대 유저 이벤트량(DBQueueSize 설정을 우선하여 처리함)
    },
    {
        ProcessTimeInterval = 5 * 1000,
        EventID = "DB_EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ",
        QueueSizeThreshold = 600, -- 추가 큐잉을 위한 조건(여기서 지정된 DB queue size 를 초과하지 않게 추가큐잉함)
        ProcessCount = 12, -- 한번에 처리할 최대 유저 이벤트량(DBQueueSize 설정을 우선하여 처리함)
    },
}
