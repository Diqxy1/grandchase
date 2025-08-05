#ifndef _KGCLUCKYDAYRECEIVEBOX_H_
#define _KGCLUCKYDAYRECEIVEBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "KTDGParticleSystem.h"

class KD3DStatic;
class GCDeviceTexture;

class KGCLuckyDayReceiveBox: public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCLuckyDayReceiveBox );

    KGCLuckyDayReceiveBox(void);
    virtual ~KGCLuckyDayReceiveBox(void);
    /// Copy constructor
    KGCLuckyDayReceiveBox( const KGCLuckyDayReceiveBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLuckyDayReceiveBox& operator=( const KGCLuckyDayReceiveBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete( void );
    virtual void PostChildDraw( void );

    int                 m_iItemID;
    KD3DWnd*            m_pkOk;
    KD3DStatic*         m_pkItemName;
    GCDeviceTexture*    m_pTextureItem;
    GCDeviceTexture*    m_pTextureLuckyDay;

public:
    void SetItem( DWORD dwItemID );
};

DEFINE_WND_FACTORY( KGCLuckyDayReceiveBox );
DECLARE_WND_FACTORY( KGCLuckyDayReceiveBox );
DECLARE_WND_FACTORY_NAME( KGCLuckyDayReceiveBox );

#endif //_KGCLUCKYDAYRECEIVEBOX_H_