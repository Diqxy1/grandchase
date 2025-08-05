#pragma once

class KGCItemImgUIWnd : public KGCItemImgWnd
{
public:
    DECLARE_CLASSNAME( KGCItemImgUIWnd );
    /// Default constructor
    KGCItemImgUIWnd( void );
    /// Default destructor
    virtual ~KGCItemImgUIWnd( void );
    /// Copy constructor
    KGCItemImgUIWnd( const KGCItemImgUIWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemImgUIWnd& operator=( const KGCItemImgUIWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

protected:
     virtual void OnCreate( void );

private:
    enum 
    {
        MAX_IMG_UI_WND = 10,
        MAX_IMG_UI_NUMBER = 6,
    };
    KD3DWnd* m_pkDescWnd[MAX_IMG_UI_WND];
    KGCUINumber*    m_pkDescNumber[MAX_IMG_UI_NUMBER];

public:
    KD3DWnd* GetDestWnd(int index)    { return m_pkDescWnd[index]; }
    KGCUINumber* GetDestNumber(int index)    { return m_pkDescNumber[index]; }
    void OnInit( void );
};

DEFINE_WND_FACTORY( KGCItemImgUIWnd );
DECLARE_WND_FACTORY( KGCItemImgUIWnd );
DECLARE_WND_FACTORY_NAME( KGCItemImgUIWnd );
