#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GCForgeManager.h"
struct RecipeSimpleInfo;

class KGCRecipeInfoBar : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRecipeInfoBar );
    /// Default constructor
    KGCRecipeInfoBar( void );
    /// Default destructor
    virtual ~KGCRecipeInfoBar( void );
    /// Copy constructor
    KGCRecipeInfoBar( const KGCRecipeInfoBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRecipeInfoBar& operator=( const KGCRecipeInfoBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event_ );
	virtual void FrameMoveInEnabledState();

private:
    KD3DWnd*        m_pkRecipeBar;
    KD3DStatic*     m_pkStaticItemLevel;
    KD3DStatic*     m_pkStaticItemName;
	KD3DStatic*		m_pkStaticItemGrade;
    KD3DStatic*     m_pkStaticPrice;
    D3DCOLOR        m_dwColor;

    //unsigned short  m_usIndex;
    //GCITEMID        m_giItemID;
    RecipeSimpleInfo	m_kRecipeInfo;

    bool            m_bEnable;
public:
    void SetRecipe(RecipeSimpleInfo recipe){m_kRecipeInfo = recipe;}
    GCITEMID GetItemID(){return m_kRecipeInfo.ItemID;}

    //void SetIndex(unsigned short usIndex){m_usIndex = usIndex;}
    unsigned short GetIndex(){return m_kRecipeInfo.usIndex;}

	void SetTextRecipeInfoBar( std::wstring strItemLevel_, std::wstring strItemName_, std::wstring strPrice_, std::wstring& strGrade_ = std::wstring(L"") );
    void SetFontColorRecipeInfoBar( D3DCOLOR dwColor_ );
    void SetHeightDirect( DWORD dwHeight );
    D3DCOLOR GetFontColor() { return m_dwColor; }

    bool IsEnable( void ) { return m_bEnable; }
};

DEFINE_WND_FACTORY( KGCRecipeInfoBar );
DECLARE_WND_FACTORY( KGCRecipeInfoBar );
DECLARE_WND_FACTORY_NAME( KGCRecipeInfoBar );


