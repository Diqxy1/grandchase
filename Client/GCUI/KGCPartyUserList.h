#pragma once

#define PARTYUSER_MAXNUM    6

class KGCPartyUserList_Element;
class KGCMultiMenu;

class KGCPartyUserList : public KD3DWnd,        // extends
    public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCPartyUserList );
    /// Default constructor
    KGCPartyUserList( void );
    /// Default destructor
    virtual ~KGCPartyUserList( void );
    /// Copy constructor
    KGCPartyUserList( const KGCPartyUserList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyUserList& operator=( const KGCPartyUserList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );    
    
    enum EKPULPM{
        EKPULPM_CHANGE_PARTY_MASTER     = 1,
        EKPULPM_CHANGE_PARTY_LEAVE      = 2,
        EKPULPM_CHANGE_PARTY_OUT_MEMBER = 3,
    };

private:
    std::vector<KGCPartyUserList_Element*> m_vecPartyUserList;

    std::wstring		m_strNickName;
    std::wstring        m_strClickedUserName;
    DWORD               m_dwSelectedUID;
    

public:    
    KGCMultiMenu*       m_pkPopupBox;

public:
    void SetPartyMember();

    void SetMenu ( int index );
    void CloseMenu( void );
    void OnClickMenu( const KActionEvent& event );
    void OnRClickMember(const KActionEvent& event);
    void OnLeftClick(void);
    void MouseHover(const KActionEvent& event);
    void MouseHoverLeave(const KActionEvent& event);
    void OffPopupbox(const KActionEvent& event);
};

DEFINE_WND_FACTORY( KGCPartyUserList );
DECLARE_WND_FACTORY( KGCPartyUserList );
DECLARE_WND_FACTORY_NAME( KGCPartyUserList );