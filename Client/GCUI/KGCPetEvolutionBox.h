#ifndef _KGCPETEVOLUTIONBOX_H_
#define _KGCPETEVOLUTIONBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KGCPetCardSelect;
class KGCPetEvolutionWnd;

namespace EGCPETEVOLUTIONTYPE
{
    enum
    {
        EVOLUTION   = 0,
        DEVOLUTION  = 1,
        HATCH       = 2,
        NUM,
    };
};

class KGCPetEvolutionBox: public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCPetEvolutionBox );

    KGCPetEvolutionBox(void);
    virtual ~KGCPetEvolutionBox(void);
    /// Copy constructor
    KGCPetEvolutionBox( const KGCPetEvolutionBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPetEvolutionBox& operator=( const KGCPetEvolutionBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EPetEvolutionBoxResult
    {
        DESTROYED,
        EXIT,
        TRANSFORMATION,
        HATCH,
        CURRENT_PET_PREVIEW,
        NEXT_PET_PREVIEW,
    };

    enum
    {
        MAX_CHAR_NAME_LEN   = 8,
        VIEWPORT_OFFSET_X   = 25,
        VIEWPORT_OFFSET_Y   = 70,
        VIEWPORT_WIDTH      = 150,
        VIEWPORT_HEIGHT     = 160,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );	
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete( void );

private:
    KD3DWnd*            m_pkTextEvolution; // 진화
    KD3DWnd*            m_pkTextDevolution; // 퇴화
    KD3DWnd*            m_pkTextHatch; // 부화

    KD3DWnd*            m_pkTextEvolutionSamll; // 진화
    KD3DWnd*            m_pkTextDevolutionSmall; // 퇴화
    KD3DWnd*            m_pkTextHatchSmall; // 부화

    KD3DStatic*         m_pkCurrentPetStatic;
    KD3DStatic*         m_pkNextPetStatic;
    KD3DStatic*         m_pkCurrentPetLevelStatic;
    KD3DStatic*         m_pkNextPetLevelStatic;
    KD3DWnd*            m_pkExit;
    KD3DWnd*            m_pkCurrentPetPreview;
    KD3DWnd*            m_pkNextPetPreview;

    KD3DWnd*            m_pkEvolutionButton;
    KD3DWnd*            m_pkDevolutionButton;
    KD3DWnd*            m_pkHatchButton;
    KD3DWnd*            m_pkDisableEvolutionButton;
    KD3DWnd*            m_pkDisableDevolutionButton;
    KD3DWnd*            m_pkDisableHatchButton;

    KD3DWnd*            m_pkCardLeft;
    KD3DWnd*            m_pkCardRight;

    KD3DWnd*            m_pkInvalidEvolutionMark;

    KGCPetCardSelect*   m_pkPetCardSelect;

    KGCPetEvolutionWnd* m_pkCurrentPetWnd;
    KGCPetEvolutionWnd* m_pkNextPetWnd;

    GCITEMUID               m_dwPetUID;
    DWORD               m_dwPetGoodsID;

    int                 m_iEvolutionType;
    
    DWORD               m_dwUseItemID;

    int                 m_iMinNeedPetLevel;

    bool                m_bClose;

public:
    void SetEvolutionPet( GCITEMUID dwPetUID, int iEvolutionType, DWORD dwUseItemID );
    void Release();
    void ChangePet( bool bRight );
    GCITEMUID GetPetUID(){ return m_dwPetUID; }
    int GetEvolutionType(){ return m_iEvolutionType; }
    DWORD GetUseItemID(){ return m_dwUseItemID; }
    int GetMinNeedPetLevel( DWORD dwUseItemID );

    int GetCurrentPetID();
    int GetNextPetID();
    int GetCurrentPetEvolutionLevel();
    int GetNextPetEvolutionLevel();
};

DEFINE_WND_FACTORY( KGCPetEvolutionBox );
DECLARE_WND_FACTORY( KGCPetEvolutionBox );
DECLARE_WND_FACTORY_NAME( KGCPetEvolutionBox );

#endif	//_KGCPETEVOLUTIONBOX_H_
