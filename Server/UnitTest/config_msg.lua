Server = GetMsgServer()

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
                    
-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9300

Init_MSDB( 'odbc_internal.dsn' )

DBHandler = GetDBLayer()
DBHandler:Init( 3 )

log( 2 ) -- loglevel : 0 ~ 2
