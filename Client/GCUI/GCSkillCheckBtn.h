#ifndef _GCSKILLCHECKBTN_H_
#define _GCSKILLCHECKBTN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
class KD3DStatic;

#define                 SKILL_CHECK_BUTTON_NUMBER               15
#define                 ANIMATION_COUNT                         4
class KGCSkillCheckBtn : public KD3DWnd 
{
public:
    DECLARE_CLASSNAME( KGCSkillCheckBtn );
    /// Default constructor
    KGCSkillCheckBtn( void );
    /// Default destructor
    virtual ~KGCSkillCheckBtn( void );
    /// Copy constructor
    KGCSkillCheckBtn( const KGCSkillCheckBtn& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSkillCheckBtn& operator=( const KGCSkillCheckBtn& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
// 변수
    KD3DStatic*     m_pkSkillDesc;
    KSafeArray<KD3DWnd*,ANIMATION_COUNT> m_pkStar;
    KSafeArray<KD3DWnd*,ANIMATION_COUNT> m_pkBar;
    UINT            m_uiBarAniCount;
    UINT            m_uiStarAniCount;
    bool            m_bBarAni;
    bool            m_bStarAni;

    DWORD           m_dwStarAniAccumulateTime;
    DWORD           m_dwBarAniAccumulateTime;
    UINT            m_uiBtnIndex;
    int             m_iLinkMotionID;
// 함수
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void         Init();
    void         AddString( std::wstring str_, DWORD dwColor_ );
    void         ChangeStringColor( DWORD dwColor_ );
    void         FrameMove( DWORD dwElapsedTime );
    void         FrameMoveStarAni( DWORD dwElapsedTime );
    void         FrameMoveBarAni( DWORD dwElapsedTime );

    void         EnableStarAni();
    void         EnableBarAni();

    void         SetMotionID( int iMotionID_ );
    void         SetBtnIndex( UINT uiIndex_ );
    int          GetMotionID(){ return m_iLinkMotionID;}
    UINT         GetBtnIndex(){ return m_uiBtnIndex;}
};

DEFINE_WND_FACTORY( KGCSkillCheckBtn );
DECLARE_WND_FACTORY( KGCSkillCheckBtn );
DECLARE_WND_FACTORY_NAME( KGCSkillCheckBtn );

extern KGCSkillCheckBtn* g_pkSkillCheckBtn;

#endif // _GCSKILLCHECKBTN_H_