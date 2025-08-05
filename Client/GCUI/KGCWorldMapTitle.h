#pragma once


class KGCWorldMapTitle : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCWorldMapTitle );
    /// Default constructor
    KGCWorldMapTitle( void );
    /// Default destructor
    virtual ~KGCWorldMapTitle( void );
    /// Copy constructor
    KGCWorldMapTitle( const KGCWorldMapTitle& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapTitle& operator=( const KGCWorldMapTitle& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void SetControlWndMode( ED3DWndMode eMode );
    void SetContentsType(CONTINENT_CONTENTS eContentsType);
    CONTINENT_CONTENTS GetContentsType();
    //enum{ AvailabletoHeroShop };    
private:
    int m_iCurrentCharacterLv;
    CONTINENT_CONTENTS m_eContentsType;
    KD3DWnd* m_pkIcon;
    KD3DWnd* m_pkName;
};

DEFINE_WND_FACTORY( KGCWorldMapTitle );
DECLARE_WND_FACTORY( KGCWorldMapTitle );
DECLARE_WND_FACTORY_NAME( KGCWorldMapTitle );


class KGCWorldMapUpperTitle : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCWorldMapUpperTitle );
    /// Default constructor
    KGCWorldMapUpperTitle( void );
    /// Default destructor
    virtual ~KGCWorldMapUpperTitle( void );
    /// Copy constructor
    KGCWorldMapUpperTitle( const KGCWorldMapUpperTitle& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapUpperTitle& operator=( const KGCWorldMapUpperTitle& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void SetRenderIcon( bool bRender);
    void SetRenderName( bool bRender);
    void SetRenderCharIcon( bool bRender);
    void SetRenderQuestIcon( bool bRender);
    void SetCharIconTexNum(int iTexNum);
    void SetCharIconTexCoord(D3DXVECTOR4 vecTexCoord);
    void MoveCharIconUpAndDown();
private:
    KD3DWnd* m_pkIcon;
    KD3DWnd* m_pkName;
    KD3DWnd* m_pkCharIcon;
    KD3DWnd* m_pkQuestIcon;

    D3DXVECTOR2 m_d3dVecCreateCharIconPos;
    D3DXVECTOR2 m_d3dVecCharIconPos;

    bool m_bUp;
};

DEFINE_WND_FACTORY( KGCWorldMapUpperTitle );
DECLARE_WND_FACTORY( KGCWorldMapUpperTitle );
DECLARE_WND_FACTORY_NAME( KGCWorldMapUpperTitle );

