#pragma once

class KGCRankPlayerInfoBar : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRankPlayerInfoBar );
    /// Default constructor
    KGCRankPlayerInfoBar( void );
    /// Default destructor
    virtual ~KGCRankPlayerInfoBar( void );
    /// Copy constructor
    KGCRankPlayerInfoBar( const KGCRankPlayerInfoBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRankPlayerInfoBar& operator=( const KGCRankPlayerInfoBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    void SetPlayerRankInfo(IN int iIndex , OUT bool& bRender);
protected:
    KD3DStatic* m_pkNickName;
    KD3DStatic* m_pkRankGrade;

};

DEFINE_WND_FACTORY( KGCRankPlayerInfoBar );
DECLARE_WND_FACTORY( KGCRankPlayerInfoBar );
DECLARE_WND_FACTORY_NAME( KGCRankPlayerInfoBar );
