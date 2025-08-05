#pragma once

class KGCUserItemWnd;

class KGCRewardItemListBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRewardItemListBox );
    /// Default constructor
    KGCRewardItemListBox( void );
    /// Default destructor
    virtual ~KGCRewardItemListBox( void );
    /// Copy constructor
    KGCRewardItemListBox( const KGCRewardItemListBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRewardItemListBox& operator=( const KGCRewardItemListBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:	
    void AddItem( GCITEMID ItemUID_, int iDuration = 0 );
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

    virtual void RefreshPage(void);
    void    SetDesc( const std::wstring& strText );
    void ClearItemList(void);
    KD3DStatic*                                 m_pkStaticGuide;
    KD3DWnd*									m_pkImgTitleVipUser;          
    KD3DWnd*									m_pkImgTitleNewBuyUser;           
protected:
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );	
;	
    void OnClose();
    void ResizeWindow( int _row, bool _scroll );
    void InitVariable( void );
protected:	


    KD3DWnd*									m_pkClose;
    KD3DWnd*									m_pkimgItemBackground;
    
    
    KD3DScrollbar*								m_pkScroll;
    KD3DSizingBox*                              m_pkSboxMainBackground;   
    KD3DSizingBox*                              m_pkSboxInnerBak;
    KD3DSizingBox*                              m_pkSboxTitleBackground;

    std::vector< KGCUserItemWnd* >				m_vecItemWnd; 
    std::vector< std::pair< GCITEMID, int > >  m_vecItem;       // <ItemID, NumOfItem>	


    int m_iCols;
    int m_iRows;

    D3DXVECTOR2 m_vtClosePos;
    D3DXVECTOR2 m_vtStaticGuidePos;       //m_pkStaticGuide            

    DWORD m_dwScrollHeight;
    POINT m_ptSboxMainBackgroundWH;
    POINT m_ptSboxInnerBakWH;
    POINT m_ptimgItemBackgroundWH;

    //std::set<GCITEMID> m_setItemBoxList;
};

DEFINE_WND_FACTORY( KGCRewardItemListBox );
DECLARE_WND_FACTORY( KGCRewardItemListBox );
DECLARE_WND_FACTORY_NAME( KGCRewardItemListBox );
