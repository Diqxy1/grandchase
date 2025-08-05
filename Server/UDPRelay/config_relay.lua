Server = GetRelayMgr()
--Server.m_usPort = 9600

Server:SetSendTick( 1 )
Server:SetLogTick( 60*60*1000 )
Server:Add(9600)
--Server:Add(9610)
--Server:Add(9620)
--Server:Add(9630)
--Server:Add(9640)
--Server:Add(9650)

--log( 0 ) -- loglevel : 0 ~ 2
