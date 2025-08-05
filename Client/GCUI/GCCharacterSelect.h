#pragma once

class KD3DWnd;
class KGCCharacterUI;
class KGCCharacterSelect : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCCharacterSelect );
    /// Default constructor
    KGCCharacterSelect( void );
    /// Default destructor
    virtual ~KGCCharacterSelect( void );
    /// Copy constructor
    KGCCharacterSelect( const KGCCharacterSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterSelect& operator=( const KGCCharacterSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        POS_UPPER,
        POS_LOWER,
    };

    void SetPositionEnum( int iPosEnum );
    void OnSelectChar( int iCharType_ );

private:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );

    bool OnClickOpen();
    void OnSendEvent();
    bool OnClickOutRange();

private:
    KGCCharacterUI*     m_pkCharList;
    D3DXVECTOR2         m_vOpenPosition;
    int                 m_iMode;
};

DEFINE_WND_FACTORY( KGCCharacterSelect );
DECLARE_WND_FACTORY( KGCCharacterSelect );
DECLARE_WND_FACTORY_NAME( KGCCharacterSelect );
