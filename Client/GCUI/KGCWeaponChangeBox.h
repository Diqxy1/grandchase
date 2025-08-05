#ifndef _GCKGCWEAPON_CHANGE_BOX_H_
#define _GCKGCWEAPON_CHANGE_BOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCWeaponChangeBox : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCWeaponChangeBox );
    /// Default constructor
    KGCWeaponChangeBox( void );
    /// Default destructor
    virtual ~KGCWeaponChangeBox( void );
    /// Copy constructor
    KGCWeaponChangeBox( const KGCWeaponChangeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWeaponChangeBox& operator=( const KGCWeaponChangeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void OnClickClsoe();
    void OnClickSelectItem();
    bool OnClickInvenItem( const KActionEvent& event );

public:
    void RefreshWeaponItemList();
    void SetSelectItem( GCITEMUID dwItemUID );
    GCITEMUID GetSelectItemUID();
    GCITEMID GetSelectItemID();

private:
    KD3DWnd*                       m_pkBtn_Clsoe;
    KD3DWnd*                       m_pkBtn_Ok;
    KD3DStatic*                    m_pkStatic_Explain;
    KGCItemImgWnd*                 m_pkImg_SelectItem;
    KGCUserItemBox*                m_pkInven;

private:
    GCITEMUID                          m_dwSelectItemUID;
};

DEFINE_WND_FACTORY( KGCWeaponChangeBox );
DECLARE_WND_FACTORY( KGCWeaponChangeBox );
DECLARE_WND_FACTORY_NAME( KGCWeaponChangeBox );

#endif  // _GCKGCWEAPON_CHANGE_BOX_H_