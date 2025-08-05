#ifndef _KGCPETEVOLUTIONWND_H_
#define _KGCPETEVOLUTIONWND_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KGCPetCardSelect;
class KPet;

class KGCPetEvolutionWnd: public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCPetEvolutionWnd );

    KGCPetEvolutionWnd(void);
    virtual ~KGCPetEvolutionWnd(void);
    /// Copy constructor
    KGCPetEvolutionWnd( const KGCPetEvolutionWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPetEvolutionWnd& operator=( const KGCPetEvolutionWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_CHAR_NAME_LEN   = 8,
        VIEWPORT_OFFSET_X   = 0,
        VIEWPORT_OFFSET_Y   = 0,
        VIEWPORT_WIDTH      = 190,
        VIEWPORT_HEIGHT     = 210,
    };

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );	
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState();

    void ProcessCharWndAngle();
    void ChangeCharWndZoom( bool _bZoomUp );

    bool GetEvolutionPet( std::string sPetName );

private:
    
    D3DVIEWPORT9        m_viewport;
    float               m_fAngleMove;
    float               m_fStartAngle;
    float				m_fZoom;
    POINT				m_ptStartMouse;

    bool                m_bInternal;

    int                 m_iMotionCount;

    KPet*				m_pkPetModel;
    D3DXVECTOR3         m_vPetAxisCorrection;

public:
    void                InitPetWnd( int iPetID, int iPetEvolutionLevel );
    void                Release();
    void                SetInvalidPet();
    int                 GetPetID();
    int                 GetPetEvolutionLevel();
};

DEFINE_WND_FACTORY( KGCPetEvolutionWnd );
DECLARE_WND_FACTORY( KGCPetEvolutionWnd );
DECLARE_WND_FACTORY_NAME( KGCPetEvolutionWnd );

#endif	//_KGCPETEVOLUTIONWND_H_