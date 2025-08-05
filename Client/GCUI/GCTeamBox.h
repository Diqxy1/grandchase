#ifndef _GCTEAMBOX_H_
#define _GCTEAMBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCTeamBox : public KD3DWnd // extends
				, public KActionListener 
{
public:
    DECLARE_CLASSNAME( KGCTeamBox );
    /// Default constructor
    KGCTeamBox( void );
    /// Default destructor
    virtual ~KGCTeamBox( void );
    /// Copy constructor
    KGCTeamBox( const KGCTeamBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTeamBox& operator=( const KGCTeamBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void OnInitialize( void );
    void EnableTeamBox( bool bEnable );
	void UpdateGameData();									//UI에 현재 상태를 적용합니다

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

	virtual void ActionPerformed( const KActionEvent& event );


    void SetTeam( bool bSherdinTeam );

protected:
    bool        m_bEnableTeamSel;
    KD3DWnd*    m_pkSherdinBtn;
    KD3DWnd*    m_pkCanabanBtn;
};

DEFINE_WND_FACTORY( KGCTeamBox );
DECLARE_WND_FACTORY( KGCTeamBox );
DECLARE_WND_FACTORY_NAME( KGCTeamBox );

#endif // _GCTEAMBOX_H_