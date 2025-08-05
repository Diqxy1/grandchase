#pragma once

class KGCGachaSelectSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGachaSelectSlot );
    /// Default constructor
    KGCGachaSelectSlot( void );
    /// Default destructor
    virtual ~KGCGachaSelectSlot( void );
    /// Copy constructor
    KGCGachaSelectSlot( const KGCGachaSelectSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGachaSelectSlot& operator=( const KGCGachaSelectSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    enum
    { 
        FONT_COLOR_DEFAULT = 0xffffffff,
        FONT_COLOR_ACTIVE = 0xff01ffb7,
    };

private:
    KD3DWnd*        m_pkActiveBack;
    KD3DWnd*        m_pkCardBack;
    KD3DWnd*        m_pkCardFrame;
    KD3DWnd*        m_pkActiveMark;
    KD3DStatic*     m_pkStaticCardName;
    KD3DStatic*     m_pkStaticType;
    KGCItemImgWnd*  m_pkItemImg;

    GCITEMID        m_PetID;
    int             m_iPetNameID;
    int             m_iGachaVer;

public:
    void ClearSlot( void );
    void SetGachaSelectSlotInfo( const GCITEMID petID_, const int iPetNameID_, const int iGachaVer_, const int iGachaType_ );

    void SetMouseIn( void );
    void SetMouseOut( void );
    void SetClick( const bool bSet_ );
    bool IsClick( void ) { return m_pkActiveMark->IsRenderOn(); }

    GCITEMID GetPetID( void ) { return m_PetID; }
    int      GetPetNameID( void ) { return m_iPetNameID; }
    int      GetGachaVer( void ) { return m_iGachaVer; }
};

DEFINE_WND_FACTORY( KGCGachaSelectSlot );
DECLARE_WND_FACTORY( KGCGachaSelectSlot );
DECLARE_WND_FACTORY_NAME( KGCGachaSelectSlot );
