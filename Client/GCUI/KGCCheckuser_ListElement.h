#pragma once

//#include "../uifx/D3DWnd.h"

class KGCGradeMark;
class KD3DStatic;
class KD3DSizingBox;

class KGCCheckuser_ListElement : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCheckuser_ListElement );
    /// Default constructor
    KGCCheckuser_ListElement( void );
    /// Default destructor
    virtual ~KGCCheckuser_ListElement( void );
    /// Copy constructor
    KGCCheckuser_ListElement( const KGCCheckuser_ListElement& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCheckuser_ListElement& operator=( const KGCCheckuser_ListElement& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetUserInfo( std::wstring strUserName, int iGradeMark, DWORD dwUID );
    std::wstring GetUserName( void ) const;
    void initElement( void );
    bool IsEmpty( void );
    DWORD GetUserUID( void ) const { return m_dwUID; }
    bool GetIsInNameBg( void ) const { return m_bIsInNameBG; }
    void SetCheck( bool bCheck );
    bool GetCheck()
    {
        return m_pkCheckBox->GetCheck();
    }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DStatic*     m_pkUserName;
    KGCGradeMark*   m_pkGradeMark;
    KD3DWnd*        m_pkNameBG;
    DWORD           m_dwUID;
    bool            m_bIsInNameBG;
    KD3DCheckBox*   m_pkCheckBox;
};

DEFINE_WND_FACTORY( KGCCheckuser_ListElement );
DECLARE_WND_FACTORY( KGCCheckuser_ListElement );
DECLARE_WND_FACTORY_NAME( KGCCheckuser_ListElement );