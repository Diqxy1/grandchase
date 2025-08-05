#pragma once

class KGCIntroNickNameFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCIntroNickNameFrame );
    /// Default constructor
    KGCIntroNickNameFrame( void );
    /// Default destructor
    virtual ~KGCIntroNickNameFrame( void );
    /// Copy constructor
    KGCIntroNickNameFrame( const KGCIntroNickNameFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCIntroNickNameFrame& operator=( const KGCIntroNickNameFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();
	bool CanRegist()		{ return m_pkPositive->IsRenderOn(); }
	std::wstring GetNickNameInEditBox()	{ return m_pkEditNickname->GetText(); }
    bool IsValidNickName() { return m_bIsValidNickName; }
    bool IsClickNickNameEdit();
    void LoadDefaultValue();
    bool IsPositive()           { return m_bPositive; }
private:
	void KeydownEditNick();
	void OnClickEditNick();
	KD3DWnd*			m_pkPositive;
	KD3DWnd*			m_pkSpacialChar;
	KD3DWnd*			m_pkCussWord;

protected:
	KD3DEdit*           m_pkEditNickname;
    bool                m_bIsValidNickName;
    bool                m_bClickedNicknameEdit;
    bool                m_bPositive;

};

DEFINE_WND_FACTORY( KGCIntroNickNameFrame );
DECLARE_WND_FACTORY( KGCIntroNickNameFrame );
DECLARE_WND_FACTORY_NAME( KGCIntroNickNameFrame );
