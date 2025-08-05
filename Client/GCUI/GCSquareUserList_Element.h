#pragma once

//#include "../uifx/D3DWnd.h"

class KGCGradeMark;
class KD3DStatic;
class KD3DSizingBox;

class KGCSquareUserList_Element : public KD3DWnd,
                                  public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSquareUserList_Element );
	/// Default constructor
	KGCSquareUserList_Element( void );
	/// Default destructor
	virtual ~KGCSquareUserList_Element( void );
	/// Copy constructor
	KGCSquareUserList_Element( const KGCSquareUserList_Element& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSquareUserList_Element& operator=( const KGCSquareUserList_Element& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetUserInfo( std::wstring strUserName, int iGradeMark, DWORD dwUID );
    std::wstring GetUserName( void ) const;
    void initElement( void );
    bool IsEmpty( void );
    DWORD GetUserUID( void ) const { return m_dwUID; }
    bool GetIsInNameBg( void ) const { return m_bIsInNameBG; }
    bool IsRBtnClickOnList( void ) const { return m_bIsRBtnClick; }
    void SetRClick( bool IsClick );
    bool IsLBtnClickOnList( void ) const { return m_bIsLBtnClick; }
    void SetLClick( bool IsClick );

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DStatic*     m_pkUserName;
    KGCGradeMark*   m_pkGradeMark;
    KD3DSizingBox*  m_pkNameBG;
    DWORD           m_dwUID;
    bool            m_bIsInNameBG;
    bool            m_bIsRBtnClick;
    bool            m_bIsLBtnClick;
};

DEFINE_WND_FACTORY( KGCSquareUserList_Element );
DECLARE_WND_FACTORY( KGCSquareUserList_Element );
DECLARE_WND_FACTORY_NAME( KGCSquareUserList_Element );