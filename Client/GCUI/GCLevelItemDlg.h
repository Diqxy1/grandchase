#pragma once

class KGCLevelItemDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCLevelItemDlg );
    /// Default constructor
    KGCLevelItemDlg( void );
    /// Default destructor
    virtual ~KGCLevelItemDlg( void );
    /// Copy constructor
    KGCLevelItemDlg( const KGCLevelItemDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLevelItemDlg& operator=( const KGCLevelItemDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
    enum { 
        MAX_LINE         = 2,       // 아이콘 최대 2줄
        MAX_CHAR_IN_LINE = 9,       // 한출에 최대 캐릭터 아이콘 9개 
        ICON_OFFSET      = 33,      // 캐릭터 아이콘 간격 33
        START_POSX       = 391,     // 캐릭터 아이콘 시작 위치x 393
        START_POSY       = 358,     // 캐릭터 아이콘 시작 위치y 358
    };


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DStatic* m_pkStaticMent0;
    KD3DStatic* m_pkStaticMent1;
    KD3DWnd*    m_pkBtnOK;
    KSafeArray< KD3DWnd*, GC_CHAR_NUM > m_apkIcon;

private:
    void OnClickBtnOK( void );
    void ClearLevelItemDlg( void );
    bool SetIconPosition( std::set< char >& setCharIndex_ );

public:
    void SetLevelItemInfo( int iLevel_, std::set< char >& setCharIndex_ );
};

DEFINE_WND_FACTORY( KGCLevelItemDlg );
DECLARE_WND_FACTORY( KGCLevelItemDlg );
DECLARE_WND_FACTORY_NAME( KGCLevelItemDlg );
