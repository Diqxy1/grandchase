#pragma once
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"
#include "GCUI/KGCRoomBar.h"

class KGCGuildMatchRoomBar :public KGCRoomBar
{
public:
    DECLARE_CLASSNAME( KGCGuildMatchRoomBar );
    /// Default constructor
    KGCGuildMatchRoomBar( void );
    /// Default destructor
    virtual ~KGCGuildMatchRoomBar( void );
    /// Copy constructor
    KGCGuildMatchRoomBar( const KGCGuildMatchRoomBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMatchRoomBar& operator=( const KGCGuildMatchRoomBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


protected:
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*        m_pkGuildVs;

    enum GUILD_MARK_TEXTURE_TYPE
    {
        GMTT_FIRST,
        GMTT_SECOND,
        GMTT_MAX,
    };

    KD3DWnd*		m_pkGuildMarkTexureBack[GMTT_MAX];

	KD3DImageIcon*	m_pkGuildMarkLeftTeam;	
	KD3DImageIcon*	m_pkGuildMarkRightTeam;	

public:
	virtual void SetRoomInfo( KSimpleRoomInfo kRoomInfo );
    void SetGuildMarkTexture(std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ );
};

DEFINE_WND_FACTORY( KGCGuildMatchRoomBar );
DECLARE_WND_FACTORY( KGCGuildMatchRoomBar );
DECLARE_WND_FACTORY_NAME( KGCGuildMatchRoomBar );