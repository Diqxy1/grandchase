#ifndef _GCCOLORITEMUSE_H_
#define _GCCOLORITEMUSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCColorItemUse
{
public:
    KGCColorItemUse() {}
    virtual ~KGCColorItemUse() {}

    virtual void SetColor( std::wstring strColor ) = 0;
    virtual bool IsUseColorItem() = 0;
    virtual bool IsUseColorItemCash() = 0;
    virtual void SetFocus( void ) = 0;
};

#endif