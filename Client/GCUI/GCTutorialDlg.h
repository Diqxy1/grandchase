#ifndef _GCTUTORIALDLG_H_
#define _GCTUTORIALDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../UIFX/D3DListBox.h"
//#include "GCSkillCheckBtn.h"

#define MAX_SKILLLIST_PAGE    2

class KD3DStatic;
class KGCSkillCheckBtn;

class KGCTutorialDlg : public KD3DWnd 
{
public:
    DECLARE_CLASSNAME( KGCTutorialDlg );
    /// Default constructor
    KGCTutorialDlg( void );
    /// Default destructor
    virtual ~KGCTutorialDlg( void );
    /// Copy constructor
    KGCTutorialDlg( const KGCTutorialDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTutorialDlg& operator=( const KGCTutorialDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    // 변수
    bool                m_bEnable;
    UINT                m_uiUsedBtnCount;
    KSafeArray<KGCSkillCheckBtn*,15> m_pkSkillCheckBtn;
    KD3DStatic*         m_pkCharName;
    KD3DStatic*         m_pkTab;
    KD3DStatic*         m_pkExit;
    KD3DStatic*         m_pkNewTutorialOption;

    DWORD                   m_dwPastTime;
    DWORD                   m_dwCurTime;
    DWORD                   m_dwElapsedTime;

    int                 m_iPrevForm;
    int                 m_iPrevPage;
    UINT                m_uiWaitMotion;
    UINT                m_uiPlayerPastMotion;
    int                 m_iLocalPlayerIndex;
    std::map< int, std::vector<KGCSkillCheckBtn*>> m_mapTutorialSkillCheckBtn;
    std::map< int, bool> m_mapTutorialSkillClearList;

    int                 m_iNowPage;

    // 함수
    virtual void OnCreate( void );

    void FrameMove();

    void AddStringToList( std::wstring str_, DWORD dwColor_ = 0xFFFFFFFF );
    void AddCharNameToList( std::wstring str_, DWORD dwColor_ = 0xFFFFFFFF );
    void ClearList();

    void Init();
    void Enable();
    void Disable();
    bool IsEnable() { return m_bEnable; }
    UINT GetUsedBtnCount(){ return (m_uiUsedBtnCount + 1); }
    KGCSkillCheckBtn* GetUnUsedBtn();
    void CalcElapsedTime();

    void InitSkillList();
    void SetSkillBox();
    void SetSkillBox_Common();
    void LinkSkillToCheckButton( std::wstring str_, std::vector<UINT> uiMotionID, DWORD dwColor_ = 0xFFFFFFFF );
    void LinkSkillToCheckButtonWithBeforeColor( std::wstring str_, UINT uiMotionID );
    void LinkSkillToCheckButton( std::wstring str_, UINT uiMotionID, DWORD dwColor_ = 0xFFFFFFFF );
    void LinkSkillToCheckButtonWithBeforeColor( std::wstring str_, std::vector<UINT> uiMotionID );
    bool IsSkillClear(UINT uiMotion);
    void SetLocalPlayerIndex();
    void NextSkillListPage();
};

DEFINE_WND_FACTORY( KGCTutorialDlg );
DECLARE_WND_FACTORY( KGCTutorialDlg );
DECLARE_WND_FACTORY_NAME( KGCTutorialDlg );
extern KGCTutorialDlg* g_pkTutorialDlg;

#endif // _GCTUTORIALDLG_H_