#pragma once

class KGCDungeonTitleS6;
class KGCDungeonSubInfoS6;
class KGCDungeonBossInfoS6;
class KGCHeroDungeonSubInfoS6;
class KGCRaidDungeonInfoS6;

class KGCDungeonInfoS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonInfoS6 );
    /// Default constructor
    KGCDungeonInfoS6( void );
    /// Default destructor
    virtual ~KGCDungeonInfoS6( void );
    /// Copy constructor
    KGCDungeonInfoS6( const KGCDungeonInfoS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonInfoS6& operator=( const KGCDungeonInfoS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void UpdateInfoDlg( void );
private:
	KGCDungeonTitleS6 *m_pkTitle;
	KGCDungeonSubInfoS6 *m_pkSubInfo;
	KGCDungeonBossInfoS6 *m_pkBossInfo;
	KGCHeroDungeonSubInfoS6 *m_pkHeroSubInfo;
	KGCRaidDungeonInfoS6 *m_pkRaidSubInfo;
};

DEFINE_WND_FACTORY( KGCDungeonInfoS6 );
DECLARE_WND_FACTORY( KGCDungeonInfoS6 );
DECLARE_WND_FACTORY_NAME( KGCDungeonInfoS6 );
