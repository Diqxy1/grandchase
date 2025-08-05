#ifndef _CHARGETINFOPANEL_H_
#define _CHARGETINFOPANEL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCCharGetInfoPanel : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCCharGetInfoPanel );
    /// Default constructors
    KGCCharGetInfoPanel( void );
    /// Default destructor
    ~KGCCharGetInfoPanel( void );
    /// Copy constructor
    KGCCharGetInfoPanel( const KGCCharGetInfoPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharGetInfoPanel& operator=( const KGCCharGetInfoPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetNewCharInfo(int iCharType);
    void SetShow(bool bShow);

private:
    virtual void OnCreate( void );

private:
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_apkCharName;
    KD3DSizingBox* m_pkStringBack;
    KD3DStatic* m_pkCharString;
    int m_iCharType;
};

DEFINE_WND_FACTORY( KGCCharGetInfoPanel );
DECLARE_WND_FACTORY( KGCCharGetInfoPanel );
DECLARE_WND_FACTORY_NAME( KGCCharGetInfoPanel );

#endif 