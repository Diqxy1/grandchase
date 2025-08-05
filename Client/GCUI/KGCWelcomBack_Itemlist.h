#ifndef _KGCWELCOMEBACKITEMLIST_H_
#define _KGCWELCOMEBACKITEMLIST_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KD3DStatic;

class KGCWelcomeBackItemList :    public KD3DWnd,
                                  public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCWelcomeBackItemList );
	/// Default constructor
	KGCWelcomeBackItemList( void );
	/// Default destructor
	virtual ~KGCWelcomeBackItemList( void );
	/// Copy constructor
	KGCWelcomeBackItemList( const KGCWelcomeBackItemList& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCWelcomeBackItemList& operator=( const KGCWelcomeBackItemList& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    std::wstring m_strItemList;
    void SetItemList( DWORD iItemID, int iItemNum );
    void ListHeightChange( void );
    KD3DSizingBox*      m_pkBack;

    KGCItemInfoBox*     m_pkItemInfoBox;
    std::vector< DWORD >  m_vecItemID;
    DWORD               m_CuriItemID;
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroyComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
	void InitItemList();

};

DEFINE_WND_FACTORY( KGCWelcomeBackItemList );
DECLARE_WND_FACTORY( KGCWelcomeBackItemList );
DECLARE_WND_FACTORY_NAME( KGCWelcomeBackItemList );

#endif