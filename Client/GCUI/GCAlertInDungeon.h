#pragma once

class KGCAlertInDungeon : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAlertInDungeon );
    /// Default constructor
    KGCAlertInDungeon( void );
    /// Default destructor
    virtual ~KGCAlertInDungeon( void );
    /// Copy constructor
    KGCAlertInDungeon( const KGCAlertInDungeon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAlertInDungeon& operator=( const KGCAlertInDungeon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetStaticDesc(const int iStringNum_ );
	void SetStaticDesc(const std::wstring& strTitleText_ );
	void SetTime(const float fTime_ )				{ m_fTime = fTime_; }
	void ProcessTitle();
  
	KD3DWnd*    m_pkBack;
	KD3DStatic* m_pkStaticDesc;
	float		m_fTime;

};

DEFINE_WND_FACTORY( KGCAlertInDungeon );
DECLARE_WND_FACTORY( KGCAlertInDungeon );
DECLARE_WND_FACTORY_NAME( KGCAlertInDungeon );
