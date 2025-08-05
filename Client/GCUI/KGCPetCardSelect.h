#ifndef _KGCPETCARDSELECT_H_
#define _KGCPETCARDSELECT_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCPetCardSelect: public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCPetCardSelect );

    KGCPetCardSelect(void);
    virtual ~KGCPetCardSelect(void);
    /// Copy constructor
    KGCPetCardSelect( const KGCPetCardSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPetCardSelect& operator=( const KGCPetCardSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EPETCARDSELECT
    {
        CHANGE_PET_CARD_LEFT,
        CHANGE_PET_CARD_RIGHT,
    };

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );	
    virtual void PostChildDraw( void );

private:
    KD3DWnd*            m_pkLeft;
    KD3DWnd*            m_pkRight;
    KD3DStatic*         m_pkPetName;

    GCDeviceTexture*    m_pTexturePet;
    GCDeviceTexture*    m_pTextureLeftPet;
    GCDeviceTexture*    m_pTextureRightPet;
    
    GCITEMUID               m_dwPetUID;
    int                 m_iPetGoodsID;
    int                 m_iPetEvolutionLevel;

    int                 m_iLeftPetGoodsID;
    int                 m_iLeftPetEvolutionLevel;
    int                 m_iRightPetGoodsID;
    int                 m_iRightPetEvolutionLevel;

public:
    void SetCard( GCITEMUID dwPetUID, GCITEMUID dwLeftPetUID, GCITEMUID dwRightPetUID );
    void Release();
};

DEFINE_WND_FACTORY( KGCPetCardSelect );
DECLARE_WND_FACTORY( KGCPetCardSelect );
DECLARE_WND_FACTORY_NAME( KGCPetCardSelect );

#endif	//_KGCPETCARDSELECT_H_