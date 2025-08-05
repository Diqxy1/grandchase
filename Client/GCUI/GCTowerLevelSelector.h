#pragma once

class KGCTowerLevelSelector : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTowerLevelSelector );
    /// Default constructor
    KGCTowerLevelSelector( void );
    /// Default destructor
    virtual ~KGCTowerLevelSelector( void );
    /// Copy constructor
    KGCTowerLevelSelector( const KGCTowerLevelSelector& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTowerLevelSelector& operator=( const KGCTowerLevelSelector& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void OnLeftButton();
	void OnRightButton();
    void ChangeSubGameMode( bool bRight );
	void SetDungeonLevel();
	void SetImageLevel(int iLevel);
	void SetNumberTex( int index, KD3DWnd *pWnd );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );

private:
	KD3DSizingBox *m_pkLevelBackground;
	KD3DWnd *m_pkLeftBtn;
	KD3DWnd *m_pkRightBtn;
	KD3DWnd *m_pkLevelNum0;
	KD3DWnd *m_pkLevelNum00;
};

DEFINE_WND_FACTORY( KGCTowerLevelSelector );
DECLARE_WND_FACTORY( KGCTowerLevelSelector );
DECLARE_WND_FACTORY_NAME( KGCTowerLevelSelector );
