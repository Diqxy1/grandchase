#pragma once

//#include "../uifx/D3DWnd.h"

enum
{	
    CSB_UP,
    CSB_MIDDLE,
    CSB_DOWN,    
    CSB_COMPONENT_NUM,
};

class KGCChatScrollBackground : public KD3DWnd
{
public:
    /// Default constructor
    DECLARE_CLASSNAME( KGCChatScrollBackground );
    KGCChatScrollBackground( void );
    /// Default destructor
    virtual ~KGCChatScrollBackground( void );
    /// Copy constructor
    KGCChatScrollBackground( const KGCChatScrollBackground& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChatScrollBackground& operator=( const KGCChatScrollBackground& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    DWORD GetMiddleHeight();
    void SetMiddleHeight(DWORD dwHeight);

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

    KSafeArray<KD3DWnd*,CSB_COMPONENT_NUM> m_pBackgroundComponent;
};



DEFINE_WND_FACTORY( KGCChatScrollBackground );
DECLARE_WND_FACTORY( KGCChatScrollBackground );
DECLARE_WND_FACTORY_NAME( KGCChatScrollBackground );
