#ifndef _GCOPTION_H_
#define _GCOPTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCGraphicQuality;
class KGCOptionSound;
class KGCOptionMusic;
class KD3DStatic;
class KGCKeySettingWnd;
class KGCSkillKeySettingWnd;
class KGCCheckBox;
class KD3DComboBox;


#define NUM_SAMPLE_TYPE 3 //셈플링 타입 (None, 2배 ,4배) 3개임

class KGCOption: public KD3DDialog, // extends
    public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCOption );
    /// Default constructor
    KGCOption( void );
    /// Default destructor
    virtual ~KGCOption( void );
    /// Copy constructor
    KGCOption( const KGCOption& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOption& operator=( const KGCOption& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete();

protected:
    KGCGraphicQuality* m_pkGraphicQuality;
    KGCOptionSound* m_pkSound;
    KGCOptionMusic* m_pkMusic;
    KD3DWnd* m_pkWhisperCheckBox;
    KD3DWnd* m_pkWhisperCheck;
    KD3DWnd* m_pkHitEffectCheckBox;
    KD3DWnd* m_pkHitEffectCheck;
    KD3DWnd* m_pkMotionBlurCheckBox;
	KD3DWnd* m_pkAutoGraphicCheckBox;
	KD3DWnd* m_pkAutoGraphicCheck;
    KD3DWnd* m_pkClose;	
    KD3DWnd* m_pkUsingVSCheck;
    KD3DWnd* m_pkUsingFPCheck;
    KD3DWnd* m_pkUsingVSCheckBox;
    KD3DWnd* m_pkUsingFPCheckBox;
    KGCCheckBox* m_pkDenyInviteCheckBox;
    KGCCheckBox* m_pkItemBoxAlphaAniCheckBox;
    KD3DComboBox* m_pkVideoComboBox;
	KD3DComboBox* m_pkMultiSamplingComboBox;

#if defined( USE_JOYSTICK )
	KD3DWnd* m_pkJoystickBtn;
	KD3DWnd* m_pkKeyBoardBtn;
#endif

    KD3DWnd* m_pkTitleStr;
    KD3DWnd* m_pkGraphicStr;
    KD3DWnd* m_pkAudioStr;
    KD3DWnd* m_pkSettingStr;
    KD3DStatic* m_pkWhisperStr;
    KD3DStatic* m_pkHitEffectStr;
    KD3DStatic* m_pkMotionBlurStr;
    KD3DStatic* m_pkAutoGraphicStr;
    KD3DStatic* m_pkVertexShaderStr;
    KD3DStatic* m_pkFixedPipeStr;
    KD3DStatic* m_pkDenyInviteStr;
    KD3DStatic* m_pkVideoStr;
	KD3DStatic* m_pkMultiSamplingStr;
    KD3DStatic* m_pkItemBoxAlphaAniStr;

#if defined( USE_JOYSTICK )
	KD3DWnd* m_pkControllStr;
#endif

	bool m_bRFirst;

    // 2007.03.05 wony
    // 키 설정 관련
    KD3DWnd* m_pkKeySettingStr;
    KGCKeySettingWnd* m_pkKeySettingWnd;
	KGCSkillKeySettingWnd* m_pkSkillKeySettingWnd;
	

    bool m_bFriendReject;

public:
    // 2007/5/18. daeuk. for 조선컴
    KD3DStatic* m_pkSkillEffectStr;
    KD3DWnd* m_pkSkillEffectCheck;
    KD3DWnd* m_pkSkillEffectCheckBox;
    KD3DWnd* m_pkMotionBlurCheck;

public:
    KGCKeySettingWnd* GetKeySettingWnd() { return m_pkKeySettingWnd; }

public:
	KD3DStatic* m_pkPetViewStr;
	KGCCheckBox* m_pkPetViewCheckBox;
    KD3DStatic* m_pkBasicEquipStr;
    KGCCheckBox* m_pkBasicEquipCheckBox;
    KD3DStatic* m_pkRoomAnimationStr;
    KGCCheckBox* m_pkRoomAnimationCheckBox;
};

DEFINE_WND_FACTORY( KGCOption );
DECLARE_WND_FACTORY( KGCOption );
DECLARE_WND_FACTORY_NAME( KGCOption );

#endif // _GCOPTION_H_
