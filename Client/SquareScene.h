#pragma once

//#include "../uifx/D3DWnd.h"

class KGCSquareScene : public KGCScene
{
public:
    /// Default constructor
    DECLARE_CLASSNAME( KGCSquareScene );
    KGCSquareScene( void );
    /// Default destructor
    virtual ~KGCSquareScene( void );
    /// Copy constructor
    KGCSquareScene( const KGCSquareScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSquareScene& operator=( const KGCSquareScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void SparkRender();
    /*inline void SetChatMode(bool bChatMode) {
        m_bIsChatMode=bChatMode;
    }*/
	void ResetUIforResolution();

protected:
    virtual void InitScene( void ) {}
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );    
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostDraw(void);    
    virtual void OnEnterScene();
    /*bool m_bIsChatMode;*/
};



DEFINE_WND_FACTORY( KGCSquareScene );
DECLARE_WND_FACTORY( KGCSquareScene );
DECLARE_WND_FACTORY_NAME( KGCSquareScene );
