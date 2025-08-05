fr = GetFailRate()
fr:SetWriteToDB( false )

SimLayer = GetSimLayer()
SimLayer.m_uiNickNameAvailable = 12
SimLayer.m_dwShutdownTimeLimit = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_iMaxGamePoint        = 499999
SimLayer.m_dwIndigoLevel        = 0
SimLayer.m_bRecommendKREnable 	= false
SimLayer.m_bWriteClientInfo		= true -- Ŭ���̾�Ʈ ��� ����

-- ������ ��� ���� ������.
SimLayer:SetSHAdhustTime( 2007, 04, 30, 09, 30 )

--�⼮�� �ý���
SimLayer.m_dwAccUpdateTime 	    = 60 * 1000 -- �޷� ���� �ð� ms 
SimLayer.m_nAttendTime   	    = 70 -- �⼮ ���� �ð�, ����:��
SimLayer.m_nGCPointDay		    = 100 -- ���� �⼮�� ���޵Ǵ� ��������Ʈ(�ӽ�)
SimLayer.m_nGCPointWeek		    = 100 -- �ְ��ٽ� ���޵Ǵ� ��������Ʈ(�ӽ�)
SimLayer.m_nGCPointRecommend    = 700 -- ��õ ������ �������� �ִ� ����Ʈ


--�ű� ���� ����.
SimLayer:SetForceNewUser( true, 2020, 3, 22 )
SimLayer:ClearForceMode()
SimLayer:AddForceMode( 26 ) -- Quest Zero
SimLayer:AddForceMode( 29 ) -- tutorial
SimLayer:SetTableChecksum( 0 )


SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP  + KGSSimLayer.ST_GUILD ) -- + KGSSimLayer.ST_BROADCASTING )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_NPGG_ON + KGSSimLayer.ST_NPGG_KILL )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_BROADCASTING )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_GUILD )
-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9400
NetLayer.m_usUdpPort    = 9401


NetLayer:AddURServerAddress( '59.25.186.181', 9600 )
NetLayer:AddURServerAddress( '59.25.186.181', 9610 )
NetLayer:AddTRServerAddress( '127.0.0.1', 9700 ) --�α׳����� �ʱ����� 

CenterProxy = GetCenterProxy()
--CenterProxy:SetRemoteAddress( '218.153.10.201', 9500 )

DBLayer   = GetDBLayer()
DBLayer:Init_ODBC( 'odbc_internal.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 2 )

function msg( m )
    ChannelManager:Send_NotMsg( m )
    print( m )
end

Adv = KPCBangAdvantage()
Server = GetGameServer()

Adv:EnablePCBang( true )
for itemid = 17020, 17220, 10 do -- ���, �ü� ����� pc�� �����Ʈ(�� 21��).
    Adv:AddNormal( itemid )
end
Adv:AddDuration( 17510, 30 ) -- ��ȣ�� ����(30ȸ)

Adv:SetMsg( 'str_00' )
Server:AddPublisherPCBangAdv( KSimLayer.AT_NETMARBLE, Adv, '__odbcNMPCBang' ) -- �ݸ��� PC�� ����

Adv:SetMsg( 'str_08' )
Server:SetSelfPCBangAdv( Adv, '__odbcGCPCBang' )      -- GC PC�� ����

DBLayer:AddODBC( '__odbcNMPCBang', 'odbc_internal_pcbang.dsn' )
DBLayer:AddODBC( '__odbcGCPCBang', 'odbc_internal_pcbang.dsn' )

-- ����� ����.
SetHackingProtectionMode( true, true, 30000 )

log( 2 ) -- loglevel : 0 ~ 2
Server:ClearChecksum()

Server:AddChecksum( 0 )             -- 051108. 98������ Ŭ���̾�Ʈ�� üũ���� ����� ������ ���Ѵ�!


if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then -- ����Ǽ� ������ ���

    tm = GetOpenTime()
    tm:SetEnable( true )
    
end

-- ���
StatManager = GetStatisticsManager()
StatManager:AddStatistics( KStatisticsManager.SI_CPL, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_PLAY_TIME, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_MAP_COUNT, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_LOGINOUT_STAT, 30 * 60 * 1000, false, 10 ) -- tslayer
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHECK_IN, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_P2P_CRITICAL, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_MINIGAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_LEAVE_GAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAMELOADING, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DEATH, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_SPX, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DISCONNECT, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_COUPLE_ROOM, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAME_START_GAP, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DUNGEON, 60 * 1000, true, 10 )