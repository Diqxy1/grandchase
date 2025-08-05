#ifndef _CHARGETINFOPROMOTION_H_
#define _CHARGETINFOPROMOTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCCharGetInfoPromotion : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCCharGetInfoPromotion );
    /// Default constructors
    KGCCharGetInfoPromotion( void );
    /// Default destructor
    ~KGCCharGetInfoPromotion( void );
    /// Copy constructor
    KGCCharGetInfoPromotion( const KGCCharGetInfoPromotion& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharGetInfoPromotion& operator=( const KGCCharGetInfoPromotion& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetShow( bool bShow, int iCharType, int iPromotion );

private:
    virtual void OnCreate( void );

private:
    KSafeArray<KD3DWnd*, (GC_CHAR_NUM * NUM_JOB_LEVEL) > m_apkPromotionName;
    KD3DWnd *m_pkment;
};

DEFINE_WND_FACTORY( KGCCharGetInfoPromotion );
DECLARE_WND_FACTORY( KGCCharGetInfoPromotion );
DECLARE_WND_FACTORY_NAME( KGCCharGetInfoPromotion );

#endif 