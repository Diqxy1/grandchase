#ifndef _GCBUYCASHITEMWND_H_
#define _GCBUYCASHITEMWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CHAGE_CYOU_POINT        9999.f
#define CYOU_POINT_PER_CASH_RATE    5.f

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class KD3DStatic;
class KGCItemBar;
class GCDeviceTexture;
class KD3DSizingBox;
class KD3DComboBox;
class KD3DEdit;
class KGCItemImgWnd;
class KGCSetItemImgWnd;
class KD3DCheckBox;

class KGCBuyCashItemWnd : public KD3DDialog,          // extends
    public KActionListener   // implements
{
public:
    enum
    {
        ID_CLOSE = 0,
    };
    DECLARE_CLASSNAME( KGCBuyCashItemWnd );
    /// Default constructor
    KGCBuyCashItemWnd( void );
    /// Default destructor
    virtual ~KGCBuyCashItemWnd( void );
    /// Copy constructor
    KGCBuyCashItemWnd( const KGCBuyCashItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBuyCashItemWnd& operator=( const KGCBuyCashItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum CASH_TYPE { CT_CASH = 0, CT_GC_POINT, CT_GP };

    void CheckCashType(CASH_TYPE enCashType);
    void EnableBuyCashItemWnd( GCItem*& pkItem_, bool bPresent_ );
protected:
    void ActionPerformed( const KActionEvent& event );
    void OnCreate( void );
    void FrameMoveInEnabledState( void );
    void ClearEditControl();
protected:
    KD3DWnd*            m_pNickNameSectionBar;

    // 구입, 선물
    KD3DWnd*            m_pBuyWnd;
    KD3DWnd*            m_pPresentWnd;
    // 각 종 버튼들
    KD3DWnd*            m_pBuyBtn;
    KD3DWnd*            m_pPresentBtn;
    KD3DWnd*            m_pCancelBtn;
    KD3DWnd*            m_pSlash;
    KD3DWnd*            m_pExitBtn;
    KD3DWnd*            m_pkCash_img;

    KD3DSizingBox*      m_pSizingBox;
    KD3DStatic*         m_pStaticItemName;
    KD3DStatic*         m_pStaticPrice;

    KD3DStatic*         m_pStaticRemainCash;
    KD3DStatic*         m_pValueRemainCash;

    KD3DStatic*         m_pStaticTakeUser;
    KD3DEdit*           m_pEditTakeUser;
    KD3DWnd*            m_pBackTakeUser;

    KD3DStatic*         m_pStaticComment;
    KD3DEdit*           m_pEditComment;
    KD3DWnd*            m_pBackComment;

    KD3DComboBox*       m_pComboBox;
    KGCItemImgWnd*      m_pItemImgWnd;
    KGCSetItemImgWnd*   m_pSetItemImgWnd;

    KD3DStatic*         m_pStaticLv;
    KD3DStatic*         m_pStaticAtk;
    KD3DStatic*         m_pStaticDef;
    KD3DStatic*         m_pStaticVit;
    KD3DStatic*         m_pStaticDesc;

    D3DXVECTOR2         m_vBuyBtnInitPos;
    GCItem*             m_kItem;

    KD3DCheckBox*       m_pkUseVCashCheckBox;
    KD3DStatic*         m_pkStaticVCash;
};

DEFINE_WND_FACTORY( KGCBuyCashItemWnd );
DECLARE_WND_FACTORY( KGCBuyCashItemWnd );
DECLARE_WND_FACTORY_NAME( KGCBuyCashItemWnd );

class KGCChangeCash : public KD3DDialog,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChangeCash );
    /// Default constructor
    KGCChangeCash( void );
    /// Default destructor
    virtual ~KGCChangeCash( void );
    /// Copy constructor
    KGCChangeCash( const KGCChangeCash& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChangeCash& operator=( const KGCChangeCash& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    void OnClickCancel( void );
    void OnClickMaxCash( void );
    void OnClickChangeCash( void );

private:
    KD3DWnd* m_pkBtnMaxWebCash;
    KD3DWnd* m_pkBtnChangeCash;
    KD3DWnd* m_pkBtnCancel;
    KD3DWnd* m_pkBtnExit;
    KD3DEdit* m_pEditWebCash;
    KD3DStatic* m_pkNeedCash;
    KD3DStatic* m_pkCash;

};

DEFINE_WND_FACTORY( KGCChangeCash );
DECLARE_WND_FACTORY( KGCChangeCash );
DECLARE_WND_FACTORY_NAME( KGCChangeCash );

#endif//_GCBUYCASHITEMWND_H_
