#pragma once

class KGCNameTag : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCNameTag );
    /// Default constructor
    KGCNameTag( void );
    /// Default destructor
    virtual ~KGCNameTag( void );
    /// Copy constructor
    KGCNameTag( const KGCNameTag& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNameTag& operator=( const KGCNameTag& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	//enum {
	//	GAP_LEFT = 10,
	//	GAP_RIGHT = 10,
	//	GAP_TOP = 1,
	//	GAP_BOTTOM = 1
	//};

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );


	/*void SetName(const std::wstring& strName_ );
	void SetName(const int& iStringID_ );*/

private:

	//void OptimaizeBG_();

	//KD3DSizingBox*      m_pkBG;
	KD3DWnd*			m_pkName;
};

DEFINE_WND_FACTORY( KGCNameTag );
DECLARE_WND_FACTORY( KGCNameTag );
DECLARE_WND_FACTORY_NAME( KGCNameTag );
