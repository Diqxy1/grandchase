#ifndef _KGCTAGMATCHSELECT_H_
#define _KGCTAGMATCHSELECT_H_

//#include "../uifx/D3DWnd.h"
#include "Packet.h"

#define TAG_CHAR_NUM 2

class KGCTagMatchSelect : public KD3DWnd
						, public KActionListener 
{
public:
	DECLARE_CLASSNAME( KGCTagMatchSelect );
	/// Default constructor
	KGCTagMatchSelect( void );
	/// Default destructor
	virtual ~KGCTagMatchSelect( void );
	/// Copy constructor
	KGCTagMatchSelect( const KGCTagMatchSelect& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCTagMatchSelect& operator=( const KGCTagMatchSelect& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	//void SlotAlign();
	const KSafeArray<int,TAG_CHAR_NUM>& GetSlot(){ return m_pCharSlot; };
	void UpdateSlot();
	void Init();
    bool IsReadyAvailable();
    void ChangeTagMatchGameMode( void );
    void UpdateGameData();

protected:
	virtual void PreDraw( void );
	virtual void PostChildDraw( void );
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void OnCandChar( int iIndex, DWORD dwCode );
	void OnSelectChar( int iIndex, DWORD dwCode );
	int FindCharSlot();
    int GetUsedSlotCount();

    void InitNowVisible();
    void SelectNowVisible( int iSelectIndex, bool bIncrease );
    void CandidateVisible();
    bool ProhibitChar(int iChar);

private:
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppk1stCharFace;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppk2ndCharFace;
	KSafeArray<KD3DWnd*,TAG_CHAR_NUM> m_ppkSelectChar;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppkDisableChar;
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppk1stChar;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppk2ndChar;
    KD3DWnd* m_pkModeTeam;
    KD3DWnd* m_pkModeSurvival;

	KSafeArray<int,TAG_CHAR_NUM> m_pCharSlot;

	int m_iCharNum;
	KCharInfoVector m_vecChar;

    bool m_bIsReadyAvailable;

    KD3DWnd* m_pkBtnRight;
    KD3DWnd* m_pkBtnLeft;
    int m_iCurSelectIndex;
    std::vector< int > m_vecNowVisible;
    KSafeArray<D3DXVECTOR2,3> m_vecPos;
};

DEFINE_WND_FACTORY( KGCTagMatchSelect );
DECLARE_WND_FACTORY( KGCTagMatchSelect );
DECLARE_WND_FACTORY_NAME( KGCTagMatchSelect );

extern KGCTagMatchSelect* g_pTagMatchSelect;

#endif // _KGCTAGMATCHSELECT_H_
