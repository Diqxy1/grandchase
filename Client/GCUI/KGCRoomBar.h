#ifndef _KGCROOMBAR_H_
#define _KGCROOMBAR_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"

class KD3DStatic;
class KGCCategoryBox;
class KGCCountryGroup;

class KGCRoomBar : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRoomBar );
    /// Default constructor
    KGCRoomBar( void );
    /// Default destructor
    virtual ~KGCRoomBar( void );
    /// Copy constructor
    KGCRoomBar( const KGCRoomBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomBar& operator=( const KGCRoomBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState();
    void TurnOnCategoryBox( bool m_bUse );

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DStatic*     m_pkRoomNumber;
    KD3DStatic*     m_pkRoomName;
    KD3DStatic*     m_pkNumOfPeople;

    KD3DWnd*        m_pkKeyIcon;
    KD3DWnd*        m_pkStatePlay;
    KD3DWnd*        m_pkStateWait;
    KD3DWnd*        m_pkStateFull;
    KD3DWnd*        m_pkStateJoinable;

    KSimpleRoomInfo m_kRoomInfo;

    KGCCategoryBox* m_pkCategoryBox;

    DWORD           m_dwPremium;
	KD3DWnd*        m_pkPremiumTitle;
	KD3DWnd*        m_pkPremiumBlog; 
    KD3DWnd*        m_pkPremiumGoldStage;
    
    KD3DWnd*        m_pkNoEnter;
    KD3DWnd*        m_pkGuildBorderR;
    KD3DWnd*        m_pkGuildBorderL;
    KD3DWnd*        m_pkQuestionIconR;
    KD3DWnd*        m_pkQuestionIconL;
    KD3DWnd*        m_pkGuildmarkIconR;
    KD3DWnd*        m_pkGuildmarkIconL;
    KD3DWnd*        m_pkVersusIcon;

    KD3DImageIcon*  m_pkGuildImage_R;
    KD3DImageIcon*  m_pkGuildImage_L;

    KGCCountryGroup* m_pkCountryGroup;

public:
    virtual void SetRoomInfo( KSimpleRoomInfo kRoomInfo );
    KSimpleRoomInfo GetRoomInfo(){ return m_kRoomInfo; }
    std::wstring GetCategoryName();
    inline void SetPremiumType( DWORD dwType ) { m_dwPremium = dwType; }
    void SetPremium();
    void CheckGuildMark( std::pair<std::wstring, std::wstring>& prGuildMarkName_ , std::pair<std::wstring, std::wstring>& prGuildName_, std::pair< DWORD, DWORD > m_pairBattlePoint_ );
    void SetGuildMarkTexture( std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ , std::pair< DWORD, DWORD > m_pairBattlePoint_ );
};

DEFINE_WND_FACTORY( KGCRoomBar );
DECLARE_WND_FACTORY( KGCRoomBar );
DECLARE_WND_FACTORY_NAME( KGCRoomBar );

#endif //_KGCROOMBAR_H_