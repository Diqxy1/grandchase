#pragma once

#include "KGCGPCapsuleItemList.h"

#define MAX_CAPSULE_NUM 4
#define MAX_VERTICAL_LINE 12
#define CURRENT_CHAR_NUM 8

class KGCItemImgWnd;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;
class KGCGPCapsuleItemList;

class KGCGPCapsuleDlg :	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGPCapsuleDlg );
	/// Default constructor
	KGCGPCapsuleDlg( void );
	/// Default destructor
	virtual ~KGCGPCapsuleDlg( void );
	/// Copy constructor
	KGCGPCapsuleDlg( const KGCGPCapsuleDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGPCapsuleDlg& operator=( const KGCGPCapsuleDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	enum EItemKind {
		EIK_WEAPON = 0,     // 무기
		EIK_ARMOR,          // 방어구
		EIK_ACCESSORIES,    // 악세사리
		EIK_RUNE,           // 강화룬
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

protected:	
	KD3DWnd*	m_pkClose;

    //지크하트가 들어가면 다시 아래 주석으로 막은 코드를 해제
    //KD3DWnd*	m_apkChar[GC_CHAR_NUM + 1 ];
    //KD3DWnd*	m_pkCharBtnOutline[GC_CHAR_NUM + 1 ];
    KSafeArray<KD3DWnd*,GC_CHAR_NUM + 1> m_apkChar;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM + 1> m_pkCharBtnOutline;

	KSafeArray<KD3DWnd*,MAX_VERTICAL_LINE> m_pkLine;

	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkWeaponCapsuleBtn;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkArmorCapsuleBtn;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkAccCapsuleBtn;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkRuneCapsuleBtn;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkSelectWeaponCapsule;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkSelectArmorCapsule;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkSelectAccCapsule;
	KSafeArray<KD3DWnd*,MAX_CAPSULE_NUM> m_pkSelectRuneCapsule;

	KD3DWnd*	m_pkWeapon;
	KD3DWnd*	m_pkArmor;
	KD3DWnd*	m_pkAccessory;
	KD3DWnd*	m_pkRune;

	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkWeaponLvText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkWeaponGPText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkArmorLvText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkArmorGPText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkAccLvText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkAccGPText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkRuneLvText;
	KSafeArray<KD3DStatic*,MAX_CAPSULE_NUM> m_pkRuneGPText;
	KD3DStatic*	m_pkGP;

	std::map< std::pair<int,int>, std::vector<KCapsule>> m_mapCapsuleInfo;

	int m_iCurSelectChar;
	int m_iWeaponCapsuleNum;
	int m_iArmorCapsuleNum;
	int m_iRuneCapsuleNum;
	int m_iAccCapsuleNum;

public:
	KGCGPCapsuleItemList*	m_pkGPCapsuleItemListDlg;
	int m_iCharIndex;
	int	m_iTotalCharNum;

public:
	void SetCapsuleInfo( std::map< std::pair<int,int>, std::vector<KCapsule>>& mapCapsuleInfo_ );
	void OnClickCapsule( int iCharIndex_, int iItemKind_, int iCapsuleIndex_, bool bRender_ );
	void DrawCapsuleInfo( int iCharIndex_ );
	void UpdateGP();
	void SetParticleTime( int iParticleTime_ );
	void SetCapsuleBtnToolTipOff( bool bOff_ );
	void DrawObtainedItem( int iItemGrade_, DWORD dwItemID );

	KGCGPCapsuleItemList*	GetGPCapsuleItemListDlgPtr(){ return m_pkGPCapsuleItemListDlg; }
};


DEFINE_WND_FACTORY( KGCGPCapsuleDlg );
DECLARE_WND_FACTORY( KGCGPCapsuleDlg );
DECLARE_WND_FACTORY_NAME( KGCGPCapsuleDlg );
