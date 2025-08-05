#pragma once

//#include "../uifx/D3DWnd.h"

class KGCCheckButton : public KD3DWnd,
                       public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCheckButton );

    enum ECheckClickType
    {
        GC_CBUTTON_BTN,        
        GC_CBUTTON_CHK,
    };
    /// Default constructor
    
    KGCCheckButton( void );
    /// Default destructor
    virtual ~KGCCheckButton( void );
    /// Copy constructor
    KGCCheckButton( const KGCCheckButton& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCheckButton& operator=( const KGCCheckButton& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    virtual void ActionPerformed( const KActionEvent& event );
    DWORD GetMiddleHeight();
    void SetMiddleHeight(DWORD dwHeight);
    inline bool IsChecked() const {return m_bChecked;}
    void Click();
    void Check(bool bChk);

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

    KD3DWnd* m_pCheckBtn;
    bool m_bChecked;
};



DEFINE_WND_FACTORY( KGCCheckButton );
DECLARE_WND_FACTORY( KGCCheckButton );
DECLARE_WND_FACTORY_NAME( KGCCheckButton );
