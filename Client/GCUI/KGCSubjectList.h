#pragma once

class KD3DSizingBox;
class KGCSubjectList : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCSubjectList );
    /// Default constructor
    KGCSubjectList( void );
    /// Default destructor
    virtual ~KGCSubjectList( void );
    /// Copy constructor
    KGCSubjectList( const KGCSubjectList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSubjectList& operator=( const KGCSubjectList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DSizingBox         *m_pkBack;
    KD3DStatic            *m_pkStatic;
    KD3DWnd               *m_pkTitle;
};

DEFINE_WND_FACTORY( KGCSubjectList );
DECLARE_WND_FACTORY( KGCSubjectList );
DECLARE_WND_FACTORY_NAME( KGCSubjectList );