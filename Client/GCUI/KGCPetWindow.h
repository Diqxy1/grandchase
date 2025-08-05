#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <map>

class KD3DScrollbar;
class KD3DSizingBox;

class KGCPetWindow : public KD3DWnd,          // extends
                        public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPetWindow );
    /// Default constructor
    KGCPetWindow( void );
    /// Default destructor
    virtual ~KGCPetWindow( void );
    /// Copy constructor
    KGCPetWindow( const KGCPetWindow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPetWindow& operator=( const KGCPetWindow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum { NUM_OF_BOX = 12, NUM_OF_ROW = 4, NUM_OF_COL = 3 };
    enum COLLECTION_STATE { CTS_CLOSE = 0, CTS_OPEN, CTS_HALF_OPEN };
    enum { PCT_NO_BLEND = 255, PCT_HALF_BLEND = 128 };

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );

    void SetCollectionState( int iIndex, COLLECTION_STATE enState );
    void SetItem( int iIndex, int iItemID, char cPromotion );
    int GetFirstPetIndex(void);
    int GetSlotIndexByIndex(int iIndex);
    void LoadingItemList( void );

    int GetPetIndex( int iItemID, char cPromotion );
    void RefreshPetSlot( void );

	void CreatePetShopImg(void);
	void DestroyPetShopImg(void);
    bool IsEqualPet( int iItemID, char cPromotion );

private:

    KSafeArray<KD3DSizingBox*,NUM_OF_BOX> m_pkPetBox;
    KSafeArray<KD3DWnd*,NUM_OF_BOX> m_pkQuestionMark;
    KSafeArray<KSafeArray<KD3DWnd*,NUM_OF_COL-1>,NUM_OF_ROW> m_pkEvolutionArrow;
    KSafeArray<GCDeviceTexture*,NUM_OF_BOX> m_pTextureItem;
    KSafeArray<COLLECTION_STATE,NUM_OF_BOX> m_enState;

    KD3DWnd*                m_pkClose;
    KD3DScrollbar*          m_pkScrollBar;

    std::vector< std::pair< std::pair<int, char>, bool> >	m_vecPetList;     // ( <펫아이디, 진화여부>, 소유여부 )
	std::map< std::pair<int, char>, GCDeviceTexture* >		m_mapPetShopImg;  // ( <펫아이디, 진화여부>, GCDeviceTexture )

    KD3DWnd*    m_pkEmoticonBtn;
    KD3DWnd*    m_pkTitleBtn;
    KD3DWnd*    m_pkSelPetBtn;
    KD3DWnd*    m_pkMonsterCardBtn;

    int         m_iRow;
};

DEFINE_WND_FACTORY( KGCPetWindow );
DECLARE_WND_FACTORY( KGCPetWindow );
DECLARE_WND_FACTORY_NAME( KGCPetWindow );
