#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../GCItem.h"

class KD3DWnd;
class KD3DSizingBox;
class KD3DStatic;
struct KInventoryItem;

class KGCItemInformationBreakupBox : public KD3DWnd,          // extends
                                     public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCItemInformationBreakupBox );
	/// Default constructor
	KGCItemInformationBreakupBox( void );
	/// Default destructor
	virtual ~KGCItemInformationBreakupBox( void );
	/// Copy constructor
	KGCItemInformationBreakupBox( const KGCItemInformationBreakupBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCItemInformationBreakupBox& operator=( const KGCItemInformationBreakupBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );
	virtual void PostChildDraw();

    enum GROUPENUM
    {
        EITEM_NAME,
        EITEM_INFO,
        EITEM_BREAKUP1,
        EITEM_BREAKUP2,
        EITEM_BREAKUP3,
        EITEM_MAX,
    };

    enum BREAKUPITEMENUM
    {
        MATERIAL_1,
        MATERIAL_2,
        MATERIAL_3,
        MATERIAL_MAX,
    };

    enum ITEM_GRADE
    {
        GRADE_NORMAL   = 0,
        GRADE_RARE     = 1,
        GRADE_EPIC     = 2,
        GRADE_LEGEND = 3,
        GRADE_MYSTIC = 4,
        GRADE_NUM,
    };

private:
    bool LoadScript( std::string strFileName );
    std::wstring MakeItemNameString();
    void ReleaseItemTexture();

public:
    void SetUIPosition();
    void SetHeightGroup( GROUPENUM eItemInfoGroup_, KD3DStatic* psStanDardStatic_);
    void SetPositionGroup( GROUPENUM eItemInfoGroup_, D3DXVECTOR2 vNewPos_ );
    bool SetInfoString( GCItem* pkItem, GCITEMUID itemUID_ );
    int PlusPosY( KD3DStatic* pkStatic_ );

private:
    struct BreakUpItemInfo
    {
        std::pair<int,int> m_iMaterial_min[ MATERIAL_MAX ];
        std::pair<int,int> m_iMaterial_max[ MATERIAL_MAX ];
    };

    struct ItemInfoGroup
    {
        KD3DWnd*            m_pkItemInfoGroup;
        KD3DSizingBox*      m_pkBackGround;
        DWORD               m_dwExtraSize;
        DWORD               m_dwExtraPlus;

        ItemInfoGroup() : m_pkItemInfoGroup( NULL ), m_dwExtraPlus(0){}
        DWORD   GetHeight() { return m_pkItemInfoGroup->GetHeight(); }
    };

    ItemInfoGroup  m_stItemInfoGroup[ EITEM_MAX ];
    KD3DSizingBox* m_pkBackground;
    KD3DSizingBox* m_pkCenterBackground;

    std::vector<DWORD> m_vecBreakupGoosID;
    std::vector<GCDeviceTexture*> m_vecpTexBreakUpItem;
    int m_iTextureIndex[ MATERIAL_MAX ];

    std::map< int, BreakUpItemInfo > m_mapBreakupInfo[ GRADE_NUM ];

    GCItem*     m_pkItem;
    KD3DStatic* m_pkName;
    KD3DStatic* m_pkCost;
    KD3DStatic* m_pkInfo;
    KD3DStatic* m_pkBreakUpItem[ MATERIAL_MAX ];
    KD3DStatic* m_pkMyItem[ MATERIAL_MAX ];
};

DEFINE_WND_FACTORY( KGCItemInformationBreakupBox );
DECLARE_WND_FACTORY( KGCItemInformationBreakupBox );
DECLARE_WND_FACTORY_NAME( KGCItemInformationBreakupBox );
