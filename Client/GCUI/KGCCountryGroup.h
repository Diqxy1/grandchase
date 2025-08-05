#pragma once

class KGCCountryGroup :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCCountryGroup );
    /// Default constructor
    KGCCountryGroup( void );
    /// Default destructor
    virtual ~KGCCountryGroup( void );
    /// Copy constructor
    KGCCountryGroup( const KGCCountryGroup& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCountryGroup& operator=( const KGCCountryGroup& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EGCCountryGroup
    {
        COUNTRY_TYPE_A = 0,
        COUNTRY_TYPE_B = 1,
        COUNTRY_TYPE_C = 2,
        COUNTRY_TYPE_D = 3,
        COUNTRY_TYPE_E = 4,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd* m_pkCountryGroupA;
    KD3DWnd* m_pkCountryGroupB;

public:
    void SetCountryGroup( std::wstring& strCountryName_, bool bUseToolTip_ = false, int iStringID_ = -1, ED3DWndToolTipAlignType eAlignType_ = ED3DWND_TOOLTIP_ALIGN_NORMAL);
    void AllRenderOnOff( bool bOnOff_ );
    void AllRenderToolTipOnOff( bool bOnOff_);

};

DEFINE_WND_FACTORY( KGCCountryGroup );
DECLARE_WND_FACTORY( KGCCountryGroup );
DECLARE_WND_FACTORY_NAME( KGCCountryGroup );