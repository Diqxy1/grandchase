#pragma once

class KGCMatchInfoBox;
class KGCMatchMapBox;


class KGCRoomMatchPanelS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomMatchPanelS6 );
    /// Default constructor
    KGCRoomMatchPanelS6( void );
    /// Default destructor
    virtual ~KGCRoomMatchPanelS6( void );
    /// Copy constructor
    KGCRoomMatchPanelS6( const KGCRoomMatchPanelS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomMatchPanelS6& operator=( const KGCRoomMatchPanelS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void UpdateGameData();

public:
    KGCTeamBox*                 m_pkTeamBox;
    KGCMatchInfoBox*            m_pkMatchInfo;
    KGCMatchMapBox*             m_pkMatchMapBox;

};

DEFINE_WND_FACTORY( KGCRoomMatchPanelS6 );
DECLARE_WND_FACTORY( KGCRoomMatchPanelS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomMatchPanelS6 );
