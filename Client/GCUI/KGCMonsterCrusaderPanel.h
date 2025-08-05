#ifndef _KGCMonsterCrusaderPanel_H_
#define _KGCMonsterCrusaderPanel_H_

//#include "../uifx/D3DWnd.h"

class KGCChangeBox;

class KGCMonsterCrusaderPanel : public KD3DWnd,
                                public KActionListener 
{
public:
    DECLARE_CLASSNAME( KGCMonsterCrusaderPanel );
    /// Default constructor
    KGCMonsterCrusaderPanel( void );
    /// Default destructor
    virtual ~KGCMonsterCrusaderPanel( void );
    /// Copy constructor
    KGCMonsterCrusaderPanel( const KGCMonsterCrusaderPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCrusaderPanel& operator=( const KGCMonsterCrusaderPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void PostChildDraw( void );
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void Destroy();
    virtual void FrameMoveInEnabledState();

    void ChangeMonster( bool bRight );

    KGCChangeBox*       m_pkMonsterChangeBox;

    int                 m_iSelectMonsterIndex;
    int                 m_iListSize;

    GCDeviceTexture*    m_pMonsterTexture;
    D3DXVECTOR4         m_vTextureCoord;

public:
    void UpdateGameData();
    void Init();
};

DEFINE_WND_FACTORY( KGCMonsterCrusaderPanel );
DECLARE_WND_FACTORY( KGCMonsterCrusaderPanel );
DECLARE_WND_FACTORY_NAME( KGCMonsterCrusaderPanel );

#endif // _KGCMonsterCrusaderPanel_H_
