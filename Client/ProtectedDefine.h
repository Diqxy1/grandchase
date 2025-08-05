#include "dbg/assert.hpp"
#include "Versiondef.h"

// 2007/4/16. iridology. Hacking Tool Log
#define _VERIFY_BEGIN_()    static std::wstring strFunc = __WFUNCTION__;\
                            bool bRet = true;\
                            bool bTemp;

#define _VERIFY_AND_LOG_(p) do\
                            {\
                                bTemp = (p.Verify());\
                                bRet &= bTemp;\
                                if( bTemp == false )\
                                {\
                                    std::wstringstream stm;\
                                    stm << CLIENT_VER_STR << strFunc << L"():" << L#p;\
                                    g_kGlobalValue.m_stmHackComment.str( stm.str().c_str() );\
                                    return bTemp;\
                                }\
                            } while(0);
