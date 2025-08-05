//STATE ex-CHANNELLOBBY
KENUM( ACTION_CONNECT )  //0
KENUM( ACTION_VERIFY )//1
KENUM( ACTION_MSG_CONNECT )//2 

//STATE CHANNELLOBBY
KENUM( ACTION_CH_LIST )//3
KENUM( ACTION_NOTIFY )//4
KENUM( ACTION_CHATTING )//5
KENUM( ACTION_WHISPER )//6
KENUM( ACTION_ENTER_CH )//7

//STATE CHANNEL
KENUM( ACTION_ROOM_LIST )//8
KENUM( ACTION_USER_LIST )//9
KENUM( ACTION_WHERE_USER )//10
KENUM( ACTION_LEAVE_CH )//11
KENUM( ACTION_JOIN_ROOM )//12
KENUM( ACTION_CREATE_ROOM )//13

//BUY Item
KENUM( ACTION_BUY_CERT )//14
KENUM( ACTION_BUY_FOR_GP )//15
KENUM( ACTION_BUY_FOR_GEM )//16
KENUM( ACTION_BUY_FOR_CRYSTAL )//17
KENUM( ACTION_BUY_COUNT_FOR_GP )//18
KENUM( ACTION_QUERY_INVENTORY )//19
KENUM( ACTION_EQUIP_ITEM )//20
KENUM( ACTION_SELL_ITEM )//21
KENUM( ACTION_STRONG )//22

//STATE ROOM
KENUM( ACTION_INVITE )//23
KENUM( ACTION_LEAVE_ROOM )//24
KENUM( ACTION_START_GAME )//25

//STATE GAME
KENUM( ACTION_USE_ITEM )//26
KENUM( ACTION_END_GAME )//27

//etc
KENUM( ACTION_DISCONNECT )//28
KENUM( ACTION_CHANGE_ROOM )//29
KENUM( ACTION_LEAVE_GAME )//30
KENUM( ACTION_REG_USER )//31
KENUM( ACTION_DUMP_INFO )//32
// 050121
//KENUM( ACTION_BUY_ITEM )
//KENUM( ACTION_DROP_ITEM )
//KENUM( ACTION_DEL_ITEM )
//KENUM( ACTION_EQP_ITEM )

KENUM( ACTION_SENTINEL )//33

KENUM( ACTION_CHANGE_STATE )//34
KENUM( ACTION_UPDATE_MYDATA ) //35
KENUM( ACTION_WAIT )//36
/*
    //ACTION_CONNECT,   ACTION_VERIFY,    ACTION_CREATE_ROOM, ACTION_JOIN_ROOM,  ACTION_CHATTING, ACTION_LEAVE_ROOM,
    //ACTION_ROOM_LIST, ACTION_USER_LIST, ACTION_CH_LIST,     ACTION_START_GAME, ACTION_END_GAME, ACTION_DISCONNECT,
    //ACTION_ENTER_CH,  ACTION_LEAVE_CH,  ACTION_CHANGE_ROOM, ACTION_LEAVE_GAME, ACTION_REG_USER, ACTION_WHERE_USER,
    //ACTION_WHISPER,   ACTION_INVITE,    ACTION_SENTINEL

//STATE_INIT 
{ .3,               0,                0,                   0,                0,                0,               
   0,               0,                0,                   0,                0,                0,  
   0,               0,                0,                   0,                0,                0,
   0,               0
},
//STATE_CONNECTED 
{  0,              .2,                0,                   0,                0,                0,               
   0,               0,                0,                   0,                0,                0,  
   0,               0,                0,                   0,                0,                0,
   0,               0

},
//STATE_LOGINING 
{  0,               0,                0,                   0,                0,                0,               
   0,               0,                0,                   0,                0,                0,  
   0,               0,                0,                   0,                0,                0,
   0,               0

},    
//STATE_LOGINED 
{  0,               0,                0,                   0,                0,                0,               
   0,               0,              .05,                   0,                0,             .005,  
  .2,               0,                0,                   0,                0,                0,
   0,               0

},
//STATE_DB_EXIT 
{  0,               0,                0,                   0,                0,                0,               
   0,               0,                0,                   0,                0,                0,  
   0,               0,                0,                   0,                0,                0,
   0,               0

},
//STATE_CHANNEL
{  0,               0,              .03,                 .03,               .1,                0,               
 .02,             .03,                0,                   0,                0,             .005,  
   0,             .01,                0,                   0,                0,              .03,
 .03,               0

},
//STATE_ROOM 
{  0,               0,                0,                   0,               .1,              .03,               
   0,               0,                0,                 .05,                0,             .005,  
   0,               0,              .03,                   0,                0,              .03,
 .03,             .03

},
//STATE_PLAYING 
{  0,               0,                0,                   0,                0,                0,               
   0,               0,                0,                   0,              .03,             .005,  
   0,               0,                0,                 .03,                0,                0,
   0,               0
},
//STATE_EXIT
{.02,               0,                0,                   0,                0,                0,               
   0,               0,                0,                   0,                0,                0,  
   0,               0,                0,                   0,                0,                0,
   0,               0
}
*/