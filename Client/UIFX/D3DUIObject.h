#ifndef _D3DUIOBJECT_H_
#define _D3DUIOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>

class KD3DUIObject
{
public:
    virtual const char* GetClassName( void ) const
    {
        return NULL;
    }
};

#ifdef DECLARE_CLASSNAME
    #undef DECLARE_CLASSNAME
#endif // DECLARE_CLASSNAME

#ifdef IMPLEMENT_CLASSNAME
    #undef IMPLEMENT_CLASSNAME
#endif // IMPLEMENT_CLASSNAME

#define DECLARE_CLASSNAME(s)    static char ms_szClassName[];  \
                                virtual const char* GetClassName( void ) const \
                                { \
                                    return ms_szClassName; \
                                }

#define IMPLEMENT_CLASSNAME(s)  char s##::ms_szClassName[] = (#s)

#endif // _D3DUIOBJECT_H_