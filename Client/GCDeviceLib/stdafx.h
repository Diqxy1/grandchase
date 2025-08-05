#pragma once



#include "../stdafx_stl.h"
#include "../stdafx_boost.h"
#include <d3dx9.h>
#include <d3d9.h>
#include <assert.h>

#include "../ProtectedType_new.h"
#include "../KSafe_Array.h"
//#include "../MassFileLib/GCMassFileManager.h"
#include "../kom/manager.h"
#include "../GCUtil/GCUtil.h"
#include "../Vertex.h"
#include "gcdevicemanager.h"
#include "KExceptionSafeUtil.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif

// SAFE_DELETE_ARRAY
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif

// SAFE_RELEASE
//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// SAFE_FILE_CLOSE
#ifndef SAFE_FILE_CLOSE
#define SAFE_FILE_CLOSE( p ) { if( p ) { fclose( p ); } }
#endif 
