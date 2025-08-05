#ifndef _NPCACTION_H_
#define _NPCACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Packet.h"

//#define NPCBUFFER   (MAX_MONSTER)

// 2007.09.19 wony
// 원래 버퍼 사이즈는 MAX_MONSTER인데..
// 넓게 잡아도 되지 않나..??
//#define NPCBUFFER   (MAX_MONSTER * 2)

class kNPC
{
	public:
//		KSafeArray<NPC,NPCBUFFER> MyNPC;  // 총 10칸의 NPC 버퍼가 있다.
		std::vector<NPC> m_vecNPC;
		kNPC();
		void ClearNPC_Action();
		bool PushNPC_Action(NPC *); // 푸쉬가 성공했는지 리턴한다.
		void PopNPC_Action();
};

#endif // _NPCACTION_H_