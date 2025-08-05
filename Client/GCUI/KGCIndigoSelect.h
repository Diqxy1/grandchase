#ifndef _KGCINDIGOSELECT_H_
#define _KGCINDIGOSELECT_H_

//#include "../uifx/D3DWnd.h"
#include "Packet.h"

class KGCIndigoSelect : public KD3DWnd
						, public KActionListener 
{
public:
	DECLARE_CLASSNAME( KGCIndigoSelect );
	/// Default constructor
	KGCIndigoSelect( void );
	/// Default destructor
	virtual ~KGCIndigoSelect( void );
	/// Copy constructor
	KGCIndigoSelect( const KGCIndigoSelect& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCIndigoSelect& operator=( const KGCIndigoSelect& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	//void SlotAlign();
	char* GetSlot(){ return m_pCharSlot; };
	void UpdateSlot();
	void Init();
	
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

private:
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppkCharFace;
	KSafeArray<KD3DWnd*,3> m_ppkSelectChar;
	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_ppkCandChar;

	char m_pCharSlot[3];

	int m_iCharNum;
	KCharInfoVector m_vecChar;

    KD3DWnd* m_pkBtnRight;
    KD3DWnd* m_pkBtnLeft;
    int m_iCurSelectIndex;
    std::vector< int > m_vecNowVisible;
    KSafeArray<D3DXVECTOR2,3> m_vecPos;
};

DEFINE_WND_FACTORY( KGCIndigoSelect );
DECLARE_WND_FACTORY( KGCIndigoSelect );
DECLARE_WND_FACTORY_NAME( KGCIndigoSelect );

extern KGCIndigoSelect* g_pIndigoSelect;

#endif // _KGCINDIGOSELECT_H_
