#ifndef _KGCMONSTERHUNTPANEL_H_
#define _KGCMONSTERHUNTPANEL_H_

//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class KGCChangeBox;

class KGCMonsterHuntPanel : public KD3DWnd,
                            public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCMonsterHuntPanel );
    /// Default constructor
    KGCMonsterHuntPanel( void );
    /// Default destructor
    virtual ~KGCMonsterHuntPanel( void );
    /// Copy constructor
    KGCMonsterHuntPanel( const KGCMonsterHuntPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterHuntPanel& operator=( const KGCMonsterHuntPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

    KD3DStatic*     m_pkStaticEnable;
    KD3DStatic*     m_pkStaticDesc;
    KGCChangeBox*   m_pkLevelChangeBox;

public:
    void UpdateGameData();
    void ChangeLevel( bool bRight );
};

DEFINE_WND_FACTORY( KGCMonsterHuntPanel );
DECLARE_WND_FACTORY( KGCMonsterHuntPanel );
DECLARE_WND_FACTORY_NAME( KGCMonsterHuntPanel );

#endif // _KGCMONSTERHUNTPANEL_H_
