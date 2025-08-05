#ifndef _MODELS_H_
#define _MODELS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_PATTERN 2

//#include "KTLib/KTDXDeviceTexture.h"
//#include "UKTool/KGCMassFileManager.h"
//#include "KTDXDeviceManager.h"
//#include "GCEnum.h"
//#include "define.h"
//#include "GCDeviceManager2.h"

class PLAYER;


class Models
{
public:
	Models();	
	KSafeArray<GCObject*,GC_CHAR_NUM> m_pMeshPlayerShadow;	

	void InitDevice();
	void DeleteDevice();
};

#endif // _MODELS_H_