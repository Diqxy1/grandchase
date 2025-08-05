#pragma once

class KGCPromotionChangePanel : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCPromotionChangePanel );
    /// Default constructor
    KGCPromotionChangePanel( void );
    /// Default destructor
    virtual ~KGCPromotionChangePanel( void );
    /// Copy constructor
    KGCPromotionChangePanel( const KGCPromotionChangePanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPromotionChangePanel& operator=( const KGCPromotionChangePanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    void SetPromotionInfo(int iPromotion);
    void SetShow(bool bShow);

private:
    KD3DWnd* m_pkAtk;
    KD3DWnd* m_pkDef;
    KD3DWnd* m_pkHP;
    KD3DWnd* m_pkCri;
    KD3DSizingBox* m_pkAtkBack;
    KD3DSizingBox* m_pkDefBack;
    KD3DSizingBox* m_pkHPBack;
    KD3DSizingBox* m_pkCriBack;
    KD3DStatic* m_pkStaticAtk;
    KD3DStatic* m_pkStaticDef;
    KD3DStatic* m_pkStaticHP;
    KD3DStatic* m_pkStaticCri;

};

DEFINE_WND_FACTORY( KGCPromotionChangePanel );
DECLARE_WND_FACTORY( KGCPromotionChangePanel );
DECLARE_WND_FACTORY_NAME( KGCPromotionChangePanel );
