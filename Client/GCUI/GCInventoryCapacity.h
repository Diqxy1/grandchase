#ifndef _KGCINVENTORYCAPACITY_H_
#define _KGCINVENTORYCAPACITY_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCInventoryCapacity : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCInventoryCapacity );
    /// Default constructor
    KGCInventoryCapacity( void );
    /// Default destructor
    virtual ~KGCInventoryCapacity( void );
    /// Copy constructor
    KGCInventoryCapacity( const KGCInventoryCapacity& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInventoryCapacity& operator=( const KGCInventoryCapacity& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void UpdateCapacity( const bool bCoordi_ );

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DStatic* m_pkInventoryCapacity;

    std::wstring GetColor( int iUserItemSize, int iInventoryCapacity );
};

DEFINE_WND_FACTORY( KGCInventoryCapacity );
DECLARE_WND_FACTORY( KGCInventoryCapacity );
DECLARE_WND_FACTORY_NAME( KGCInventoryCapacity );

#endif //_KGCInventoryCapacity_H_