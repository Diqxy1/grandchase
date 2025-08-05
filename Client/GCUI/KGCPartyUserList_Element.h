#pragma once

class KD3DStatic;

class KGCPartyUserList_Element : public KD3DWnd,
                                 public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCPartyUserList_Element );
    /// Default constructor
    KGCPartyUserList_Element( void );
    /// Default destructor
    virtual ~KGCPartyUserList_Element( void );
    /// Copy constructor
    KGCPartyUserList_Element( const KGCPartyUserList_Element& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyUserList_Element& operator=( const KGCPartyUserList_Element& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void initElement( void );

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

    void SetPartyMember(const KPartyMember& memberInfo);
    DWORD GetPartyMemberUID(){return m_dwUID;}
    void CursorEnter( bool bCursorEnter );
public:
    KD3DStatic*     m_pkUserName;
    DWORD           m_dwUID;
    KGCSizingThreeBox*  m_pkNameBG;

    //KD3DWnd *m_pkHostMark;
};

DEFINE_WND_FACTORY( KGCPartyUserList_Element );
DECLARE_WND_FACTORY( KGCPartyUserList_Element );
DECLARE_WND_FACTORY_NAME( KGCPartyUserList_Element );