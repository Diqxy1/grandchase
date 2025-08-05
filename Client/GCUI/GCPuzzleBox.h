#ifndef _GCPuzzleBox_H_
#define _GCPuzzleBox_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "../uifx/D3DButtonCombo.h"
//#include <UserPacket.h>

class KGCPuzzleBox: public KD3DDialog,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPuzzleBox );
    /// Default constructor
    KGCPuzzleBox( void );
    /// Default destructor
    virtual ~KGCPuzzleBox( void );
    /// Copy constructor
    KGCPuzzleBox( const KGCPuzzleBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPuzzleBox& operator=( const KGCPuzzleBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
	virtual void OnDestroy();
	virtual void PostChildDraw( void );

	KD3DWnd*		m_pkBtnClose;

	KD3DButtonCombo* m_pkPuzzleBoard;
	KD3DButtonCombo* m_pkRewardBoard;

	KD3DWnd*		m_pkBtnOpen;
	KD3DWnd*		m_pkBtnBuyPackage;
    
	std::vector<GCDeviceTexture*>	m_vecpTextureItem;
	KGCItemImgWnd*	m_pkItemWnd[2];	

	void UpdateBoards();
	void WaitForServer();
	void ToggleParticle( bool bToggle );
	void DestroyParticle();
	std::vector<CParticleEventSeqPTR> m_vecParticle;

    void DrawInfoWindow();
    bool CheckPosInPieceBound( D3DXVECTOR4& vPieceRect_, D3DXVECTOR2& vMousePos_ );



public:
	virtual void FrameMoveInEnabledState( void );
	virtual void FrameMoveInDisabledState( void );

    void close();


	std::map<DWORD, std::vector< KDropItemInfo >> m_mapTradeList; // 조각 교환 아이템 리스트.
	std::vector< KDropItemInfo >                  m_vecRewardList; // 상자 보상 아이템 리스트.

	int m_iWaitTimeForShow;

	bool m_bComplete;
	
	int m_iOK;


};

DEFINE_WND_FACTORY( KGCPuzzleBox );
DECLARE_WND_FACTORY( KGCPuzzleBox );
DECLARE_WND_FACTORY_NAME( KGCPuzzleBox );

#endif // _GCPuzzleBox_H_