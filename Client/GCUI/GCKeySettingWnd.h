// ------------------------------
// 2007.03.07 wony
// 키 커스터마이징 하는 부분..
// 제법 하드코딩이 많음-ㅅ-
// ------------------------------
#ifndef _KGCKEYSETTINGWND_H_
#define _KGCKEYSETTINGWND_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCKeySettingWnd : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCKeySettingWnd );
    /// Default constructor
    KGCKeySettingWnd( void );
    /// Default destructor
    virtual ~KGCKeySettingWnd( void );
    /// Copy constructor
    KGCKeySettingWnd( const KGCKeySettingWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCKeySettingWnd& operator=( const KGCKeySettingWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    static std::wstring GetKeyStr( BYTE cKey );
	static int GetKeyIndex( int iKey );
	void SetDefaultKey( bool bDefault );    

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();        
    
    void UpdateKey();
    void CheckKeyInputEvent();

#ifdef USE_JOYSTICK
    void CheckJoystickIputEvent();
    void SetDefaultJoystickKey( bool bDefault );
    void UpdateJoystickKey();
    void CheckJoystickIputEvent_Frame();
#endif
    void SetSelectInput(int iIndex);
	void LoadKeySettingString( void );

private:
    // 키 설명
    KSafeArray<KD3DStatic*,NUM_USE_KEY> m_pkKeyStr;

    // 현재 선택된 키
    KSafeArray<KD3DStatic*,NUM_USE_KEY> m_pkKey;
    KSafeArray<KD3DStatic*,NUM_USE_KEY> m_pkKeyBack;

    // 현재 상태
    KD3DStatic* m_pkStatus;

    // 테두리
    KSafeArray<KD3DWnd*,NUM_USE_KEY> m_pkGrith;

    int m_iSelectedKey;

    // 기본설정 / 내설정
    KD3DWnd* m_pkDefaultKeyBtn;
    KD3DWnd* m_pkCustomKeyBtn;

    KD3DWnd* m_pkKeyboardRadio;
    KD3DWnd* m_pkjoystickRadio;
    KD3DStatic* m_pkKeyboardRadioStatic;
    KD3DStatic* m_pkjoystickRadioStatic;


    KD3DWnd* m_pkDefaultKeySelected;
    KD3DWnd* m_pkCustomKeySelected;


};

DEFINE_WND_FACTORY( KGCKeySettingWnd );
DECLARE_WND_FACTORY( KGCKeySettingWnd );
DECLARE_WND_FACTORY_NAME( KGCKeySettingWnd );



class KGCSkillKeySettingWnd : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillKeySettingWnd );
	/// Default constructor
	KGCSkillKeySettingWnd( void );
	/// Default destructor
	virtual ~KGCSkillKeySettingWnd( void );
	/// Copy constructor
	KGCSkillKeySettingWnd( const KGCSkillKeySettingWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillKeySettingWnd& operator=( const KGCSkillKeySettingWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	KD3DWnd* m_pkClose;
	void SetDefaultKey( bool bDefault );
protected:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void FrameMoveInEnabledState();
	
	void UpdateKey();

#ifdef USE_JOYSTICK
    void CheckJoystickIputEvent();
    void SetDefaultJoystickKey( bool bDefault );
    void UpdateJoystickKey();
    void CheckJoystickIputEvent_Frame();
#endif
    void SetSelectInput(int iIndex);
    void CheckKeyInputEvent();

private:
	// 키 설명
	KSafeArray<KD3DStatic*,NUM_USE_SKILL_KEY> m_pkKeyStr;

	// 현재 선택된 키
	KSafeArray<KD3DStatic*,NUM_USE_SKILL_KEY> m_pkKey;
	KSafeArray<KD3DStatic*,NUM_USE_SKILL_KEY> m_pkKeyBack;

	// 현재 상태
	KD3DStatic* m_pkStatus;

	// 테두리
	KSafeArray<KD3DWnd*,NUM_USE_SKILL_KEY> m_pkGrith;

	int m_iSelectedKey;

	// 기본설정 / 내설정
	KD3DWnd* m_pkDefaultKeyBtn;
	KD3DWnd* m_pkCustomKeyBtn;

	KD3DWnd* m_pkDefaultKeySelected;
	KD3DWnd* m_pkCustomKeySelected;

    KD3DWnd* m_pkKeyboardRadio;
    KD3DWnd* m_pkjoystickRadio;
    KD3DStatic* m_pkKeyboardRadioStatic;
    KD3DStatic* m_pkjoystickRadioStatic;

};

DEFINE_WND_FACTORY( KGCSkillKeySettingWnd );
DECLARE_WND_FACTORY( KGCSkillKeySettingWnd );
DECLARE_WND_FACTORY_NAME( KGCSkillKeySettingWnd );

#endif //_KGCKEYSETTINGWND_H_
