#ifndef _GCKGCMISSIONSUBINFOBOX_S4_H_
#define _GCKGCMISSIONSUBINFOBOX_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMissionSubInfoBox_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionSubInfoBox_S4 );
    /// Default constructor
    KGCMissionSubInfoBox_S4( void );
    /// Default destructor
    virtual ~KGCMissionSubInfoBox_S4( void );
    /// Copy constructor
    KGCMissionSubInfoBox_S4( const KGCMissionSubInfoBox_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionSubInfoBox_S4& operator=( const KGCMissionSubInfoBox_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DStatic      *m_pkStatic_Title;
    KD3DStatic      *m_pkStatic_Desc;
    KD3DStatic      *m_pkStatic_Exp;
    KD3DStatic      *m_pkStatic_Gp;
    KD3DStatic      *m_pkStatic_Complate_State;

public:
    void CleanUp();
    void SetTitleText( int iTitleID );
    void SetComplateStateText( int iComplateCnt, int iMaxCnt );
    void SetDescriptionText( int iDescID );
    void SetExpText( int iExp );
    void SetGpText( int iGp );

public:
    void SetPosY( float fY );
    float GetPosY() {   return this->GetFixedWindowLocalPos().y;    }
};

DEFINE_WND_FACTORY( KGCMissionSubInfoBox_S4 );
DECLARE_WND_FACTORY( KGCMissionSubInfoBox_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionSubInfoBox_S4 );

#endif  // _GCKGCMISSIONSUBINFOBOX_S4_H_