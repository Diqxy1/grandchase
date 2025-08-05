#pragma once

class KGCCheckBox;
class KD3DSizingBox;
class KD3DImageIcon;

class KGCSurveyExample : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSurveyExample );
    /// Default constructor
    KGCSurveyExample( void );
    /// Default destructor
    virtual ~KGCSurveyExample( void );
    /// Copy constructor
    KGCSurveyExample( const KGCSurveyExample& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSurveyExample& operator=( const KGCSurveyExample& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        ESURVEY_EXAMPLE_NUM = 4,
    };

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();
    virtual void OnCreate( void );

public:
    void InitInfo();

    void SetExampleNumber( int iNum_ );
    void SetExampleType( int iType_ );
    void SetCheckBoxTitle( std::wstring wsTitle_ );
    void SetCheckBoxLock( bool bLock_, D3DCOLOR dwColor_ = 0xFFFFFFFF, D3DCOLOR OutlineColor = D3DCOLOR_ARGB( 255, 51, 76, 97 ) );
    void SetExampleInfo( int iType_, std::wstring& wstrExample_ );

    void SetFontColor( D3DCOLOR dwColor_ );
    void SetFontOutline( bool bFontOutline = false, D3DCOLOR OutlineColor = 0xFF000000 );

    void RenderHoverBox( bool bRender_ );
    void RenderSelectBox( bool bRender_ );

    bool IsRenderOnSelectBox();

    void GetEditAnswer( std::wstring& wstrAnswer_ );
    bool IsSetExample( void ) { return m_bIsSetExample; }

private:
    //common contorl
    KSafeArray< KD3DWnd* , ESURVEY_EXAMPLE_NUM > m_pkExampleNum;
    KGCCheckBox* m_pkCheckBox;
    KD3DWnd* m_pkSelectBox;
    KD3DSizingBox* m_pkHoverBox;

    KD3DEdit* m_pkEditAnswer;
    KD3DStatic* m_pkStaticText;

    KD3DImageIcon* m_pkImgExample;

    int m_iExampleType;
    int m_iNumber;
    bool m_bIsSetExample;
};

DEFINE_WND_FACTORY( KGCSurveyExample );
DECLARE_WND_FACTORY( KGCSurveyExample );
DECLARE_WND_FACTORY_NAME( KGCSurveyExample );
