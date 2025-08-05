#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "GCSayBalloon.h"

class KGCSubMissionBar :    public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCSubMissionBar );
    /// Default constructor
    KGCSubMissionBar( void );
    /// Default destructor
    virtual ~KGCSubMissionBar( void );
    /// Copy constructor
    KGCSubMissionBar( const KGCSubMissionBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSubMissionBar& operator=( const KGCSubMissionBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum SMB_STATE {
        SMB_NORMAL = 0,
        SMB_HOVER,
        SMB_DOWN,
        SMB_NUM,
    };
protected:
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState();
    virtual void ActionPerformed(const KActionEvent& event );

    void SetCurrentState( SMB_STATE eState );
    bool IsCorrectState( SMB_STATE eState );
public:
    void SetTitleString( std::wstring& strTitle );
    void SetCount( int iCount, int iCompleteCount );
    void SetSubMissionID( int iSubMissionID );
    void Reset();
private:
    std::wstring m_strCount;
    std::wstring m_strTitle;
    int         m_iSubMissionID;
    SMB_STATE   m_eBarState;

protected:
    KSafeArray<KD3DWnd*,SMB_NUM> m_pkBar;
    KD3DStatic* m_pkTitle;
    KD3DStatic* m_pkCount;
};


DEFINE_WND_FACTORY( KGCSubMissionBar );
DECLARE_WND_FACTORY( KGCSubMissionBar );
DECLARE_WND_FACTORY_NAME( KGCSubMissionBar );