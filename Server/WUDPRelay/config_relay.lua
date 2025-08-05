Server = GetRelayMgr()
--Server.m_usPort = 9600

Server:SetSendTick( 1 )
Server:SetLogTick( 60*60*1000 )
Server:Add(9600)

--log( 2 ) -- loglevel : 0 ~ 2
