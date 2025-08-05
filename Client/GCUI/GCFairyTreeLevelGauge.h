#pragma once

class KGCFairyTreeLevelGauge : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCFairyTreeLevelGauge );
    /// Default constructor
    KGCFairyTreeLevelGauge( void );
    /// Default destructor
    virtual ~KGCFairyTreeLevelGauge( void );
    /// Copy constructor
    KGCFairyTreeLevelGauge( const KGCFairyTreeLevelGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFairyTreeLevelGauge& operator=( const KGCFairyTreeLevelGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DSizingBox*  m_pkFrame;
    KD3DSizingBox*  m_pkGauge;
    KD3DStatic*     m_pkStaticLevel;

    int         m_iLevel;
    DWORD       m_dwCurExp;
    DWORD       m_dwNextLevelExp;
    
    D3DXVECTOR2 m_dxvPos;
    D3DXVECTOR2 m_dxvOffset;
    D3DXVECTOR2 m_dxvRenderPos;

public:
    void SetFairyTreeLevelGauge( int iLevel_, DWORD dwCurExp_, DWORD dwNextLevelExp_, D3DXVECTOR2& dxvPos_, D3DXVECTOR2& dxvOffset_ );
    void UpdateFairyTreeLevelGaugePos( void );

private:
    void SetLevelStatic( void );
    void SetGauge( void );
};

DEFINE_WND_FACTORY( KGCFairyTreeLevelGauge );
DECLARE_WND_FACTORY( KGCFairyTreeLevelGauge );
DECLARE_WND_FACTORY_NAME( KGCFairyTreeLevelGauge );
