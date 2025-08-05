#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCGuildBenefitListBar : public KD3DDialog      // extends    
{
public:
    DECLARE_CLASSNAME( KGCGuildBenefitListBar );
    /// Default constructor
    KGCGuildBenefitListBar( void );
    /// Default destructor
    virtual ~KGCGuildBenefitListBar( void );
    /// Copy constructor
    KGCGuildBenefitListBar( const KGCGuildBenefitListBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildBenefitListBar& operator=( const KGCGuildBenefitListBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:    
    virtual void OnCreateComplete( void );

private:
    KD3DStatic*     m_pkGuildLevel;
    KD3DStatic*     m_pkGuildBenefit;
    KD3DStatic*     m_pkGuildPoint;
    KD3DStatic*     m_pkGuildContribution;

public:
    void ClearText();
    void SetLevelText ( std::wstring strLevel_ );
    void SetStrNumberText ( int strNumber_ );
    void SetGpointText ( int iGpoint_ );
    void SetContributionText (int iContribution );
    void SetListFontColor( bool bFontOn );
};

DEFINE_WND_FACTORY( KGCGuildBenefitListBar );
DECLARE_WND_FACTORY( KGCGuildBenefitListBar );
DECLARE_WND_FACTORY_NAME( KGCGuildBenefitListBar );


