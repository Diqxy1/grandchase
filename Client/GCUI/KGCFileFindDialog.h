#pragma once
#include "kDXFileDialog.h"

class KD3DSizingBox;

class KGCFileFindDialog :	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCFileFindDialog );
	/// Default constructor
	KGCFileFindDialog( void );
	/// Default destructor
	virtual ~KGCFileFindDialog( void );
	/// Copy constructor
	KGCFileFindDialog( const KGCFileFindDialog& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCFileFindDialog& operator=( const KGCFileFindDialog& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	enum FILEDIALOG_END_TYPE { FET_OK = 20004, FET_CANCEL = 20005, };
    
	inline std::wstring GetSelectedFileName()
	{
		return m_strSelectFile;
	}
    std::wstring GetSelectedDir( void );

private:
	void UpdateList();

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );
protected:
	KD3DSizingBox *m_pkBackGround1;
	KD3DSizingBox *m_pkBackGround2;
	KD3DSizingBox *m_pkBackGround3;
	KD3DSizingBox *m_pkBackGround4;
	KD3DSizingBox *m_pkBackGround5;
	KD3DSizingBox *m_pkBackGround6;

	KD3DWnd       *m_pkOpenBtn;      
	KD3DWnd       *m_pkCancelBtn;      
	KD3DWnd       *m_pkCloseBtn;      
	KD3DWnd       *m_pkupFolderBtn;      
	KD3DWnd       *m_pkupPrevBtn;      

	KD3DStatic*	   m_pkStaticName1;
	KD3DStatic*	   m_pkStaticName2;
	KD3DStatic*	   m_pkStaticFilter;
	KD3DStatic*	   m_pkStaticDir;

	KD3DEdit*		m_pkEdit;
	KD3DListBox*    m_pkList;
	KD3DScrollbar*  m_pkScroll;

	typedef void (KGCFileFindDialog::*HANDLER)( const KActionEvent& event_ );
	std::map<std::string, HANDLER>  m_mapEvent;

	void OnEdit( const KActionEvent& event_ );
	void OnList( const KActionEvent& event_ );
	void OnScroll( const KActionEvent& event_ );
	void OnCancel();
	void OnUpFolder();
	void OnPrevBtn();
	void OnOK();
    void OnReplayDir();

	std::vector<std::wstring> m_vecstrFileList;
	std::vector<std::wstring> m_vecstrDirList;

	kFileDlgManager m_FileDlgManager;
	std::wstring m_strSelectFile;
};


DEFINE_WND_FACTORY( KGCFileFindDialog );
DECLARE_WND_FACTORY( KGCFileFindDialog );
DECLARE_WND_FACTORY_NAME( KGCFileFindDialog );