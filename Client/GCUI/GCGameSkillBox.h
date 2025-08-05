#ifndef _GCGAMESKILLBOX_H_
#define _GCGAMESKILLBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"

class KD3DStatic;

class KGCGameSkillBox : public KD3DWnd // extends
{
	

public:
    DECLARE_CLASSNAME( KGCGameSkillBox );
    /// Default constructor
    KGCGameSkillBox( void );
    /// Default destructor
    virtual ~KGCGameSkillBox( void );
    /// Copy constructor
    KGCGameSkillBox( const KGCGameSkillBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGameSkillBox& operator=( const KGCGameSkillBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:    
	void OnInitialize();
	void SetSkillOnly();
	void ResetComboSkill();
    bool LoadSkillIcon( void );
    void CalcComboAlpha();

protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected: 
    KD3DWnd*               m_pkComboSelector;
    KSafeArray<KD3DWnd*,6> m_pkSkillPos;
	KSafeArray<KD3DStatic*,6> m_pkText;
	KSafeArray<KD3DStatic*,6> m_pkTextCoolTime;
	KSafeArray<KD3DWnd*,5> m_pkHotKeyFrame;
	KSafeArray<KD3DWnd*,5> m_pkHotKey;
	std::map< int, GCDeviceTexture* >  m_mapSkillTexture;
	GCDeviceTexture*  m_pItemImageHealingBall;		//힐링볼
    std::map< int, std::vector<KD3DWnd*> > m_mapComboFrame;    // 스킬순서(A,S,D,F,G), 프레임UI
	int m_iOffsetY;
	bool m_bSkillOnly;
    int m_iComboSelectorAlpha;
    bool m_bComboSelectorAlpha;

    D3DXVECTOR2 m_vOriginalPos;

    KD3DWnd*               m_pkFrameLockSP4;
};

DEFINE_WND_FACTORY( KGCGameSkillBox );
DECLARE_WND_FACTORY( KGCGameSkillBox );
DECLARE_WND_FACTORY_NAME( KGCGameSkillBox );

#endif // _GCGAMESKILLBOX_H_
