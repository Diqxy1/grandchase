// Common 폴더를 중심으로 만들기.

#include "..\Common\ActorManager.cpp"
#include "..\Common\BaseServer.cpp"

#include "..\Common\ChecksumManager.cpp"

#include "..\Common\KncBitStream.cpp"
#include "..\Common\SimObject.cpp"
#include "..\Common\StringTable.cpp"

// Common/FSM
#include "..\Common\FSM\FSMclass.cpp"
#include "..\Common\FSM\FSMstate.cpp"
#include "..\GameServer\RoomFSM.cpp"
#include "..\Common\DefaultFSM.cpp"

// Packet
#include "..\Common\CenterPacket.cpp"
#include "..\Common\CommonPacket.cpp"
#include "..\Common\NUserPacket.cpp"

// event
#include "..\Common\CenterEvent.cpp"
#include "..\Common\Event.cpp"

#include "NUserEvent.cpp"
#include "OperatorEvent.cpp"

// Performer
#include "..\Common\Actor.cpp"
#include "..\Common\Performer.cpp"

// NetError
#include "..\Common\NetError.cpp"

// Layer
#include "..\Common\DBLayer.cpp"
#include "..\Common\NetLayer.cpp"
#include "..\Common\SimLayer.cpp"

// Socket 
#include "..\Common\Socket\AccepterInterface.cpp"
#include "..\Common\Socket\CircularQueue.cpp"
#include "..\Common\Socket\IOCP.cpp"
#include "..\Common\Socket\IOThread.cpp"
#include "..\Common\Socket\NetCommon.cpp"
#include "..\Common\Socket\Session.cpp"
#include "..\Common\Socket\SocketObject.cpp"

//DB
#include "..\Common\Odbc\Odbc.cpp"

//SubThread
#include "..\Common\ThreadManager.cpp"

//lua
#include "..\Common\Lua\KLuaManager.cpp"

//Area 
#include "CnSquare.cpp"
#include "CnSquareManager.cpp"

// Center
#include "Center.cpp"

#include "CenterNetLayer.cpp"
#include "CenterSimLayer.cpp"

//
#include "BlockIP.cpp"
#include "CenterServer.cpp"
#include "CnClientContentsManager.cpp"
#include "CnClientScriptManager.cpp"
#include "CnCouponManager.cpp"

#include "CnGuildManager.cpp"
#include "CnGWCSupport.cpp"
#include "CnItemManager.cpp"
#include "CnMissionEvent.cpp"
#include "CnNewDonationManager.cpp"
#include "CnNpcGift.cpp"
#include "CnQuizBingo.cpp"
#include "CnRainBow.cpp"

#include "CnSubscriptionManager.cpp"

#include "..\Common\LogManager.cpp"
#include "..\Common\GenericLogfile.cpp"
#include "..\Common\CenterLogManager.cpp"