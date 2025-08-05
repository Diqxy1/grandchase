#pragma once
class KD3DImageIcon;

class KGCGuildMarkRegistDlg :	public KD3DDialog,
                                public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildMarkRegistDlg );
    /// Default constructor
    KGCGuildMarkRegistDlg( void );
    /// Default destructor
    virtual ~KGCGuildMarkRegistDlg( void );
    /// Copy constructor
    KGCGuildMarkRegistDlg( const KGCGuildMarkRegistDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMarkRegistDlg& operator=( const KGCGuildMarkRegistDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

private:
    void OnOK();    
    void OnFileDialogOpen();
    void FileDialogOK();
    void FileDialogCancel();
    
    GCDeviceTexture*        m_pPreViewTexture;
    KD3DWnd*                m_pkPreViewBackGround;
    KD3DWnd*                m_pkOKBtn;
    KD3DWnd*                m_pkCancelBtn;
    KD3DWnd*                m_pkCloseBtn;
    KD3DWnd*                m_pkFileDialogOpenBtn;
    KD3DStatic*             m_pkStaticFilePath; 

    std::wstring            m_strTextureFileName;
	KGCFileFindDialog*		m_pkFileDialog;
	KD3DImageIcon*			m_pkImageIcon;
    std::set< D3DXIMAGE_FILEFORMAT > m_setImageFileFormatFilterList;

	DWORD					m_dwMarkChangeItemID;

public:
    void SelectTexture( std::wstring strTextureFileName_ );
    void DragTexture( std::string strTextureFileName_ );
    bool IsEnableDrag();
    bool IsEnableRegist();
	void UploadGuildMark();
	void OnClose();
	void SetMarkChangeItemID( DWORD dwItemID_ ) { m_dwMarkChangeItemID = dwItemID_; }
};

DEFINE_WND_FACTORY( KGCGuildMarkRegistDlg );
DECLARE_WND_FACTORY( KGCGuildMarkRegistDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildMarkRegistDlg );