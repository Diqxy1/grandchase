#ifndef _KGCSELECTPETNAMEBOX_H_
#define _KGCSELECTPETNAMEBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KPet;
class KGCRadioButtonBox;

class KGCSelectPetNameBox: public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCSelectPetNameBox );

    KGCSelectPetNameBox(void);
    virtual ~KGCSelectPetNameBox(void);
    /// Copy constructor
    KGCSelectPetNameBox( const KGCSelectPetNameBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSelectPetNameBox& operator=( const KGCSelectPetNameBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum ESelectPetNameBoxResult
    {
        DESTROYED,
        EXIT,
        TRANSFORM_SELECTNAME,
        TRANSFORM_HATCH,
    };

    enum
    {
        VIEWPORT_OFFSET_X   = -50,
        VIEWPORT_OFFSET_Y   = -30,
        VIEWPORT_WIDTH      = 300,
        VIEWPORT_HEIGHT     = 300,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );	
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete();

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
    void                InitPet( int iPetID, int iPetEvolutionLevel );
    void                Release();
    int                 GetPetID();
    int                 GetPetEvolutionLevel();

private:
    KD3DWnd*            m_pkTextEvolution;
    KD3DWnd*            m_pkTextDevolution;
    KD3DWnd*            m_pkTextHatch;
    KD3DWnd*            m_pkExit;
    KD3DWnd*            m_pkEvolutionButton;
    KD3DWnd*            m_pkDevolutionButton;
    KD3DWnd*            m_pkHatchButton;
    KD3DStatic*         m_pkSelectPetNameDesc;

    KGCRadioButtonBox*  m_pkRadioButtonBox;

    int                 m_iEvolutionType;
    std::wstring        m_strOriginalName;
    std::wstring        m_strNewName;
    std::wstring        m_strCurrentPetName;
    GCITEMUID               m_dwPetUID;
    DWORD               m_dwUseItemID;

public:
    void                SetSelectPetNameBox( GCITEMUID dwPetUID, std::wstring strOriginalName, std::wstring strNewName, DWORD dwUseItemID, int iEvolutionType );
    GCITEMUID           GetPetUID(){ return m_dwPetUID; }
    DWORD               GetUseItemID(){ return m_dwUseItemID; }
    int                 GetEvolutionType(){ return m_iEvolutionType; }
    std::wstring        GetSelectedPetName();

};

DEFINE_WND_FACTORY( KGCSelectPetNameBox );
DECLARE_WND_FACTORY( KGCSelectPetNameBox );
DECLARE_WND_FACTORY_NAME( KGCSelectPetNameBox );

#endif	//_KGCSELECTPETNAMEBOX_H_