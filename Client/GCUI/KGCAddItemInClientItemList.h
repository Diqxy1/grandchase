
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "GCCheckBox.h"

#define ITEMLISTNUM 10
////////////////////////////////////////////////////////////////////////////////////////////

class KGCAddItemInClientItemListElement : public KD3DWnd,
    public KActionListener

{
public:
    DECLARE_CLASSNAME( KGCAddItemInClientItemListElement );
    /// Default constructor
    KGCAddItemInClientItemListElement( void );
    /// Default destructor
    virtual ~KGCAddItemInClientItemListElement( void );
    /// Copy constructor
    KGCAddItemInClientItemListElement( const KGCAddItemInClientItemListElement& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAddItemInClientItemListElement& operator=( const KGCAddItemInClientItemListElement& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();


#ifndef __PATH__

public:
    void SetCheckBoxState(bool bcheck);
    void SetBookMarkState(bool bcheck);
    bool GetBookMarkState();
    bool GetCheckBoxState();
    void SetItemDataText(const WCHAR* text);
    void SetText(const std::wstring& text);
    void SetItemIdText(DWORD dwItemId);
    std::vector<const WCHAR*> GetItemData();
    void SetItemData( std::vector<int> vecItemData);

    KD3DStatic*         m_pkName;
    KD3DStatic*         m_pkItemId;
    KD3DEdit*           m_pkGrade;
    KD3DEdit*           m_pkPeriod;
    KD3DEdit*           m_pkDuration;
    KD3DEdit*           m_pkLevel;
    KGCCheckBox*       m_apkCheckBox;
    KGCCheckBox*        m_apkBookMarkCheckBox;
private:
  

    std::vector<const WCHAR*>  m_vecItemInfo;
#endif
};

DEFINE_WND_FACTORY( KGCAddItemInClientItemListElement );
DECLARE_WND_FACTORY( KGCAddItemInClientItemListElement );
DECLARE_WND_FACTORY_NAME( KGCAddItemInClientItemListElement );


class KGCAddItemInClientItemList : public KD3DWnd,
    public KActionListener

{
public:
    DECLARE_CLASSNAME( KGCAddItemInClientItemList );
    /// Default constructor
    KGCAddItemInClientItemList( void );
    /// Default destructor
    virtual ~KGCAddItemInClientItemList( void );
    /// Copy constructor
    KGCAddItemInClientItemList( const KGCAddItemInClientItemList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAddItemInClientItemList& operator=( const KGCAddItemInClientItemList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

#ifndef __PATH__

public:
    void SetText( std::vector<GCItem*> vecwstring);
    void SetItemDataText(const WCHAR* text);
    bool GetCheckBoxState(int icheck);
    bool GetBookMarkState(int icheck);
    void SetCheckBoxState(int icheck, bool bcheck);
    void SetBookMarkState(int icheck, bool bcheck);
    std::map<int, std::vector<const WCHAR*>> GetItemData();
    void SetItemData( std::vector<int> vecItemData);

       KGCAddItemInClientItemListElement* m_pkItemListElement[ITEMLISTNUM];
   // KSafeArray< KGCCheckBox*, ITEMLISTNUM >     m_apkCheckBox;
   // KSafeArray< KGCCheckBox*, ITEMLISTNUM >     m_apkBookMarkCheckBox;
    void SetUpDownFocusControl();
    void SetLeftRightFocusControl();
    void SetCurrentCursorPos(int iColumn, int iRow);
    void OnKeyDown( const KActionEvent& event );
private:
  //  enum{ ITEMLISTNUM = 10,};
  
     
     KD3DStatic*         m_pkStaticGrade;
     KD3DStatic*         m_pkStaticPeriod;
     KD3DStatic*         m_pkStaticDuration;
     KD3DStatic*         m_pkStaticLevel;
     KD3DStatic*         m_pkStaticItemId;
     int m_iColumn;
     int m_iRow;        
     
     std::map< int, std::vector<const WCHAR*>>                  m_mapItemInfo;
#endif

};

DEFINE_WND_FACTORY( KGCAddItemInClientItemList );
DECLARE_WND_FACTORY( KGCAddItemInClientItemList );
DECLARE_WND_FACTORY_NAME( KGCAddItemInClientItemList );


