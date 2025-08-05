#pragma once

//#include "../uifx/D3DWnd.h"

enum
{	
    CBB_LEFT_UP,
    CBB_LEFT_MIDDLE,
    CBB_LEFT_DOWN,    
    CBB_MIDDLE_UP,
    CBB_MIDDLE_MIDDLE,
    CBB_MIDDLE_DOWN,    
    CBB_RIGHT_UP,
    CBB_RIGHT_MIDDLE,
    CBB_RIGHT_DOWN,    
    CBB_COMPONENT_NUM,
};

class KGCChatBoxBackground : public KD3DWnd
{
public:
    /// Default constructor
    DECLARE_CLASSNAME( KGCChatBoxBackground );
    KGCChatBoxBackground( void );
    /// Default destructor
    virtual ~KGCChatBoxBackground( void );
    /// Copy constructor
    KGCChatBoxBackground( const KGCChatBoxBackground& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChatBoxBackground& operator=( const KGCChatBoxBackground& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    DWORD GetMiddleHeight();
    void SetMiddleHeight(DWORD dwHeight);

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

    KSafeArray<KD3DWnd*,CBB_COMPONENT_NUM> m_pBackgroundComponent;
};



DEFINE_WND_FACTORY( KGCChatBoxBackground );
DECLARE_WND_FACTORY( KGCChatBoxBackground );
DECLARE_WND_FACTORY_NAME( KGCChatBoxBackground );
