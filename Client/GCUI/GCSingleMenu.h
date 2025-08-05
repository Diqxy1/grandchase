#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/ActionListener.h"

class KGCSingleMenu : public KD3DWnd,
                      public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCSingleMenu );

    KGCSingleMenu(void);
    ~KGCSingleMenu(void);
    /// Copy constructor
    KGCSingleMenu( const KGCSingleMenu& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSingleMenu& operator=( const KGCSingleMenu& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void SetBadGuy( std::wstring strNickName_ );

private:
    KD3DStatic* m_pkReport;
    std::wstring m_strBadGuy;
};


DEFINE_WND_FACTORY( KGCSingleMenu );
DECLARE_WND_FACTORY( KGCSingleMenu );
DECLARE_WND_FACTORY_NAME( KGCSingleMenu );