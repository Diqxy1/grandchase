#pragma once

#define GUILD_MAX_LEVEL (10)

class KD3DStatic;
class KD3DListBox;
class KD3DScrollbar;
class KGCCommonEditBox;
class KD3DImageIcon;

class KGCGuildInfoDlg :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildInfoDlg );
    /// Default constructor
    KGCGuildInfoDlg( void );
    /// Default destructor
    virtual ~KGCGuildInfoDlg( void );
    /// Copy constructor
    KGCGuildInfoDlg( const KGCGuildInfoDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildInfoDlg& operator=( const KGCGuildInfoDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    GCDeviceTexture*			m_pGuildMarkTexture;

	KD3DStatic*					m_pkGuildName;
	KD3DStatic*					m_pkGuildBirtyday;
	KD3DStatic*					m_pkGuildPoint;
	KD3DStatic*					m_pkGuildActivity;
	KD3DStatic*					m_pkGuildMasterName;
	KD3DStatic*					m_pkGuildUrl;
    KD3DStatic*                 m_pkBattlePoint;
    KD3DStatic*                 m_pkGuildLevelStr;
	KD3DListBox*				m_pkGeneralListBox;
	KD3DScrollbar*				m_pkGeneralListScroll;

	KD3DSizingBox*				m_pkGeneralListBG;

    KGCCommonEditBox*			m_pkGuildNotice1;
	KGCCommonEditBox*			m_pkGuildNotice2;
	KGCCommonEditBox*			m_pkGuildIntroduce;

	KD3DImageIcon*				m_pkGuildMarkIcon;

	std::wstring				m_strGuildNotice1;
	std::wstring				m_strGuildNotice2;

	KD3DWnd*					m_pkGuildGradeNew;
	KD3DWnd*					m_pkGuildGradeRegular;

    KD3DWnd*                    m_pkUpIcon; 
    KD3DWnd*                    m_pkGuildLevelIcon;
    KGCSizingThreeBox*          m_pkGaugeBackground;
    KGCSizingThreeBox*          m_pkGaugeBar;
    DWORD	            		m_dwGuageWidth;	 //레벨 게이지 UI 픽셀 길이

public:
    int iGlevel; //길드 레벨
public:
    
    void OnClose( void );
	void SetGuildInfo();
	void SetGuildNotice( std::wstring strGuildNotice, int iType_);
	void UpdateGuildMark();
    void UpdateGuildLevel( int iGPoint );
    void GPointDiff ( void );
};

DEFINE_WND_FACTORY( KGCGuildInfoDlg );
DECLARE_WND_FACTORY( KGCGuildInfoDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildInfoDlg );