#ifndef _GCITEMBAR_H_
#define _GCITEMBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "GCItem.h"
//#include "../uifx/D3DWnd.h"

class KGCItemBar : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCItemBar );
    /// Default constructor
    KGCItemBar( void );
    /// Default destructor
    virtual ~KGCItemBar( void );
    /// Copy constructor
    KGCItemBar( const KGCItemBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemBar& operator=( const KGCItemBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum ECertType
    {
        CERT_TYPE_NONE,
        CERT_TYPE_ANGEL,
        CERT_TYPE_CROSSBOW,
        CERT_TYPE_SOUL,
    };

public:
    void SetMoneyType( EMONEYTYPE eMoneyType );
    void SetCertType( ECertType eType );
    void SetPointType();
    void SetItem( GCItem*& ItemInfo, bool bShop = false);

protected:
    virtual void OnCreate( void );

protected:
    KD3DWnd* m_pkMoneyType;
    KD3DWnd* m_pkVipType;
    KD3DWnd*    m_pkCert;
    KD3DWnd*    m_pkPointMark;
    EMONEYTYPE  m_eMoneyType;
};

DEFINE_WND_FACTORY( KGCItemBar );
DECLARE_WND_FACTORY( KGCItemBar );
DECLARE_WND_FACTORY_NAME( KGCItemBar );

#endif // _GCITEMBAR_H_