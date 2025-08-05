#ifndef _KGCMATCHMAPBOX_H_
#define _KGCMATCHMAPBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
class KGCSingleImage;

class KGCMatchMapBox : public KD3DWnd,           // extends
    public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCMatchMapBox );
    /// Default constructor
    KGCMatchMapBox( void );
    /// Default destructor
    virtual ~KGCMatchMapBox( void );
    /// Copy constructor
    KGCMatchMapBox( const KGCMatchMapBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMatchMapBox& operator=( const KGCMatchMapBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnInitialize( void );

protected:
    KD3DStatic* m_pkGoalStatic;
    KD3DStatic* m_pkTimeStatic;
    KD3DStatic* m_pkStatBalanceStatic;
    
    KD3DWnd* m_pkRandomMapStage;
    KGCChangeBox* m_pkMapSelectBox;

    KGCSingleImage* m_pkMapImage;
    
    //std::map<EGCGameStage, KD3DWnd*>    m_mapMapStage;
    std::map<EGCGameStage, std::pair<int, int>>         m_mapMapNameID;

public:
    void UpdateGameData();
    void ChangeMap( bool bRightMove );
    void SendRoomInfo();
    void CreateMapStringContainer();
    void UpdateMapImage( EGCGameStage eGameStage );
    bool GetTextureFileNum(IN EGCGameStage eGameStage, OUT int& iFileNum, OUT int& iIndex);

};

DEFINE_WND_FACTORY( KGCMatchMapBox );
DECLARE_WND_FACTORY( KGCMatchMapBox );
DECLARE_WND_FACTORY_NAME( KGCMatchMapBox );

#endif // _KGCMAPBOX_H_
