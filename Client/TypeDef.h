#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "define.h"
//#include "GCEnum.h"
// GC P2P에서 사용하는 PacketID에 관한 TypeDef
typedef     UCHAR           GCPID;
typedef     USHORT          PACKED_ELEMENT_SIZE;
typedef         std::vector<DWORD>          KIPVector;
typedef         std::vector<USHORT>         KPortVector;
typedef		INT64						GCEXPTYPE;
typedef     DWORD          GCITEMID;
typedef     INT64          GCITEMUID;
#endif//_TYPEDEF_H_