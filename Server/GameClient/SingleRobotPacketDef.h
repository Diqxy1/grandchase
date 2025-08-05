#pragma once
#include "KncP2P.h"
enum GC_GAME_MSG_ID
{
    GC_GAME_MSG_ID_STARTGAME = 0,
    GC_GAME_MSG_ID_ENDGAME, 
    GC_GAME_MSG_ID_KNOCK,
    GC_GAME_MSG_ID_END
};
struct GC_GAME_MSG_HEADER
{
    UINT        m_uiPacketID;
};
#define     DECL_GC_GAME_PACKET( id )        struct K##id : public GC_GAME_MSG_HEADER

DECL_GC_GAME_PACKET( GC_GAME_MSG_ID_STARTGAME )
{
};

DECL_GC_GAME_PACKET( GC_GAME_MSG_ID_ENDGAME )
{
};

DECL_GC_GAME_PACKET( GC_GAME_MSG_ID_KNOCK )
{
    KNCID       kncPeerID;
};