#pragma once

#include "GCRoomUserSlotS6.h"

class KGCVitalityWorldmap : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVitalityWorldmap );
    /// Default constructor
    KGCVitalityWorldmap( void );
    /// Default destructor
    virtual ~KGCVitalityWorldmap( void );
    /// Copy constructor
    KGCVitalityWorldmap( const KGCVitalityWorldmap& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVitalityWorldmap& operator=( const KGCVitalityWorldmap& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkWndTooltip;
    KD3DWnd*            m_pkGuageVitality;
    KD3DWnd*            m_pkGuageVitalityRect;
    KD3DWnd*            m_pkGuageExp;
    KD3DWnd*            m_pkGuageExpRect;
    KD3DWnd*            m_pkBtnRecharge;
    KD3DWnd*            m_pkLevel;
    KD3DStatic*         m_pkStaticVitality;
    KD3DStatic*         m_pkStaticExp;

    KGCRoomUserCharImgS6*   m_pkCharImg;

    KSafeArray<KD3DWnd*, KGCRoomUserSlotS6::EM_LEVEL_NUM>    m_apkUserLevel;

    void OnClickRechargeBtn();
    void SetNumberTex( IN int index, IN KD3DWnd *pWnd );
    void ChangeUVtoTexel( OUT D3DXVECTOR4& vUV );


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void SetCurrCharInfo();


private:
};

DEFINE_WND_FACTORY( KGCVitalityWorldmap );
DECLARE_WND_FACTORY( KGCVitalityWorldmap );
DECLARE_WND_FACTORY_NAME( KGCVitalityWorldmap );
