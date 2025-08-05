#pragma once

class KGCGachaSelectSlot;

class KGCSuperGachaSelectWnd : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSuperGachaSelectWnd );
    /// Default constructor
    KGCSuperGachaSelectWnd( void );
    /// Default destructor
    virtual ~KGCSuperGachaSelectWnd( void );
    /// Copy constructor
    KGCSuperGachaSelectWnd( const KGCSuperGachaSelectWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSuperGachaSelectWnd& operator=( const KGCSuperGachaSelectWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    enum { NUM_GACHA_SLOT = 7,
               GACHA_TYPE_DIAMOND = 2,
			   GACHA_TYPE_COORDI = 1, // Kawan> porra DIG, s�rio isso bugou o bagulho botando 11 //1,
			   GACHA_TYPE_EQUIP = 0
			};
private:

	struct KGachaInfo {
		int GachaID;
		int GachaType;
		std::pair<GCITEMID, int> PetInfo;
	};

	//Gambiarra para nao ficar rodando varias vezes o vetor
    std::vector< KGachaNewList > m_vecGachaDiamondList;
    std::vector< KGachaNewList > m_vecGachaCommonList;
	bool m_bFirstTime;
    std::vector< KGachaInfo >     m_vecGachaList; // < ����, < ������۾��̵�, ���̸� ��Ʈ��> > 

    KD3DScrollbar*  m_pkScroll;

    int         m_iSelectSlotIndex;
    int         m_iSelectVersion;
    GCITEMID    m_SelectPetID; 
    int         m_iSelectType;
    KSafeArray< KGCGachaSelectSlot*, NUM_GACHA_SLOT >   m_apkSlot;

	KD3DWnd* m_btnCoord;
	KD3DWnd* m_btnEquip;

public:
    void SetGachaListInfo( const IN std::vector< KGachaNewList >& vecInfo_, int GachaType = GACHA_TYPE_EQUIP );
    void InitSelectWnd( void );
    void OnScroll( void );
    void SetSelectSlot( const GCITEMID petID_ );

    bool IsSetGachaList( void ) { return m_vecGachaList.empty() == false; }
    int  GetSelectVersion( void ) { return m_iSelectVersion; }

	void OnClickEquip();
	void OnClickCoord();
private:
	void OnChangeGachaType(int tab);
};

DEFINE_WND_FACTORY( KGCSuperGachaSelectWnd );
DECLARE_WND_FACTORY( KGCSuperGachaSelectWnd );
DECLARE_WND_FACTORY_NAME( KGCSuperGachaSelectWnd );
