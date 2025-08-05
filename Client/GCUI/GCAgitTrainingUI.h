#pragma once

class KGCAgitTrainingUI : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
    enum { MAX_TILE = 10 };

public:
    DECLARE_CLASSNAME( KGCAgitTrainingUI );
    /// Default constructor
    KGCAgitTrainingUI( void );
    /// Default destructor
    virtual ~KGCAgitTrainingUI( void );
    /// Copy constructor
    KGCAgitTrainingUI( const KGCAgitTrainingUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitTrainingUI& operator=( const KGCAgitTrainingUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DComboBox*   m_pkComboChar;

    KD3DStatic*     m_pkStaticChar;
    KD3DStatic*     m_pkStaticLevel;
    KD3DStatic*     m_pkStaticBrokenTile;
    KD3DStatic*     m_pkStaticRemainTile;
    KD3DStatic*     m_pkStaticLevelInfo;
    KD3DStatic*     m_pkStaticBrokenTileInfo;
    KD3DStatic*     m_pkStaticRemainTileInfo;
    
    KD3DWnd*        m_pkBtnOK;
    KD3DWnd*        m_pkBtnCancle;
    KD3DWnd*        m_pkBtnClose;

    std::vector<SCharInfo> m_vecCharInfo;
    int                    m_iComboCurSel;
    GCITEMUID              m_itemUID;

public:
    void InitUI( GCITEMUID itemUID );
    void OnClose( void );
    void OnOK( void );

    void SetEXPLevelInfo( void );
    void SetBrokenTileInfo( int iNumtile_ );
    void SetRemainTileInfo( void );

    GCITEMUID GetTrainigObjUID( void ) { return m_itemUID; }

private:
    void SetComboList( void );
    void OnComboChange( void );
    std::wstring GetCharName( int iChaarType_ );
};

DEFINE_WND_FACTORY( KGCAgitTrainingUI );
DECLARE_WND_FACTORY( KGCAgitTrainingUI );
DECLARE_WND_FACTORY_NAME( KGCAgitTrainingUI );
