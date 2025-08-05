#ifndef _GCKGCMISSIONLISTBAR_S4_H_
#define _GCKGCMISSIONLISTBAR_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCPrograssBar.h"

#define MAX_MISSION_TYPE    5

class KGCMissionListBar_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionListBar_S4 );
    /// Default constructor
    KGCMissionListBar_S4( void );
    /// Default destructor
    virtual ~KGCMissionListBar_S4( void );
    /// Copy constructor
    KGCMissionListBar_S4( const KGCMissionListBar_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionListBar_S4& operator=( const KGCMissionListBar_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );

private:
    KD3DWnd                         *m_pkMark_New;
    KD3DWnd                         *m_pkMark_Complate;
    KD3DStatic                      *m_pkStatic_Level;
    KD3DStatic                      *m_pkStatic_Title;
    KD3DStatic                      *m_pkStatic_Chain;
    KD3DSizingBoxBtn                *m_pkBackground;
    KGCPrograssBar                  m_pkPrograssBar;

    KSafeArray< KD3DWnd*, MAX_MISSION_TYPE > m_pkMissionTypeIcon;

public:
    int                             m_iAlphaCnt;
    int                             m_iSwitch;
    int                             m_iBarIdx;
    bool                            m_bPrograssMode;

private:
    boost::function1<void,int>      m_FuncCallBack;

private:
    void OnClickBar();
    void OnPressBar();
    void OnEnterCursor();

public:
    void SetNewMark( bool bEnable );
    void SetComplateMark( bool bEnable );
    void SetLevelText( std::wstring szText );
    void SetTitleText( std::wstring szText );
    void SetChainText( std::wstring szText );
    void SetFontColor( D3DCOLOR dwColor );
    void SetOutlineColor( D3DCOLOR dwColor );
    void ClearText();
    void SetMissionTypeIcon( int iconType);

public:
    void SetProgressMode( bool bSwitch );
    void SetProgressBarToggle( bool bSwitch );
    void SetProgressBarValue( int iNowValue, int iMinValue=-1, int iMaxValue=-1 );

public:
    void SetBarIndex( int iIdx ) {  m_iBarIdx = iIdx;   }
    void SetCallBack( boost::function1<void,int> Func ) 
    {  m_FuncCallBack = Func;   }
    void SwapBackGround( bool bChange );
};

DEFINE_WND_FACTORY( KGCMissionListBar_S4 );
DECLARE_WND_FACTORY( KGCMissionListBar_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionListBar_S4 );

#endif  // _GCKGCMISSIONLISTBAR_S4_H_