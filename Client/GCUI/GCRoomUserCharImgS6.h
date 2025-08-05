#pragma once

class KGCRoomUserCharImgS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomUserCharImgS6 );
    /// Default constructor
    KGCRoomUserCharImgS6( void );
    /// Default destructor
    virtual ~KGCRoomUserCharImgS6( void );
    /// Copy constructor
    KGCRoomUserCharImgS6( const KGCRoomUserCharImgS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomUserCharImgS6& operator=( const KGCRoomUserCharImgS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void SetChar( int iCharType );
    void UpdateLineSel( bool bRender );

public:
    KSafeArray<KD3DWnd*, GC_CHAR_NUM> m_apkCharImg;
    KD3DWnd*                          m_pkLineSel;

};

DEFINE_WND_FACTORY( KGCRoomUserCharImgS6 );
DECLARE_WND_FACTORY( KGCRoomUserCharImgS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomUserCharImgS6 );
