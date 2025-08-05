#ifndef _KGCGIFTBOX_H_
#define _KGCGIFTBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int EVENT_REWARD_DISPLAY_MAX = 3;
const int GIFT_BOX_GAP = 2;
const int GIFT_BOX_NEW_USER = 0;
const int GIFT_BOX_EVENT_USER = 1;

class KGCGiftBoxState;

class KGCGiftBox : public KD3DWnd // extends
    , public KActionListener 
{
public:
    DECLARE_CLASSNAME( KGCGiftBox );
    /// Default constructor
    KGCGiftBox( void );
    /// Default destructor
    virtual ~KGCGiftBox( void );
    /// Copy constructor
    KGCGiftBox( const KGCGiftBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGiftBox& operator=( const KGCGiftBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    void Tick();
    void InitNewUser();
    void InitEventReward();

protected:
    KD3DWnd* m_pkTimeBack;
    KD3DWnd* m_pkNewUserBack;

    KGCGiftBoxState* m_pNewUserGiftBox;
    KSafeArray<KGCGiftBoxState*,EVENT_REWARD_DISPLAY_MAX > m_pEventGiftBox;

};

DEFINE_WND_FACTORY( KGCGiftBox );
DECLARE_WND_FACTORY( KGCGiftBox );
DECLARE_WND_FACTORY_NAME( KGCGiftBox );

#endif // _KGCGIFTBOX_H_