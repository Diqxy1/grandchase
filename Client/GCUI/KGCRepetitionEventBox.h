#ifndef _KGCREPETITIONEVENTBOX_H_
#define _KGCREPETITIONEVENTBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCRepetitionEventBox: public KD3DDialog,
                             public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRepetitionEventBox );

    KGCRepetitionEventBox(void);
    virtual ~KGCRepetitionEventBox(void);
    /// Copy constructor
    KGCRepetitionEventBox( const KGCRepetitionEventBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRepetitionEventBox& operator=( const KGCRepetitionEventBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*        m_pkOk;
    KD3DWnd*        m_pkBar;
    KD3DStatic*     m_pkEventTitle;
    KD3DStatic*     m_pkItemName;
    KD3DStatic*     m_pkItemDesc;
    KD3DStatic*     m_pkMessage;
    KD3DSizingBox*  m_pkBackground;

    GCITEMID    m_ItemID;
    GCITEMUID   m_ItemUID;
	char		m_cEnchantEquipGradeID;
	char		m_cEnchantLevel;

    int m_iSetNumber;
    GCDeviceTexture* m_pTextureItem;

    D3DXVECTOR4 GetTextureCoords( int iSetNum );

public:
    void SetText( GCITEMID iItemID, GCITEMUID _ItemUID, std::wstring strEventDesc, std::wstring strEventTitle );
};

DEFINE_WND_FACTORY( KGCRepetitionEventBox );
DECLARE_WND_FACTORY( KGCRepetitionEventBox );
DECLARE_WND_FACTORY_NAME( KGCRepetitionEventBox );

#endif	//_KGCREPETITIONEVENTBOX_H_
