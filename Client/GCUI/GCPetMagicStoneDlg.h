#pragma once

class KGCPetMagicStoneItemSlot;

class KGCPetMagicStoneDlg : public KD3DDialog,		// extends
							 public KActionListener // implements
{
public:
	DECLARE_CLASSNAME(KGCPetMagicStoneDlg);
	/// Default constructor
	KGCPetMagicStoneDlg( void );
	/// Default destructor
	virtual ~KGCPetMagicStoneDlg( void );
	/// Copy constructor
	KGCPetMagicStoneDlg(const KGCPetMagicStoneDlg&) = delete;
	/// Assignment operator
	const KGCPetMagicStoneDlg& operator=(const KGCPetMagicStoneDlg&) = delete;

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual bool InitDialog(IN const KDialogInfo& kInfo_);
	virtual void PostChildDraw();

	enum // Kawan> 5
	{
		MAX_STONE_EQUIP_SLOT = 6,
		MAX_STONE_TYPE = 6,
		MAX_STONE_ITEM_SLOT = 6,
		MAX_STONE_ITEM_TAB = MAX_STONE_ITEM_SLOT + 1
	};

public:
	void UpdatePetGlyphData(bool bAllEffect);

private:
	void OnClose();
	void OnScrollPos();
	void InitStoneInfo();
	void OnClickTab(int iTab);
	void FilterGlyph(int iGlyphEffect);
	void OnClickGlyphSlot(int iSlot);
	void OnEquipPetGlyph(int iIndex, GCITEMUID dwUID, bool bSendPkt = false);
	void RemovePetGlyph(int iIndex, GCITEMUID dwUID, bool bUpdateTab = false);
	void EnableGlyphEffect(int iIndex);
	void UpdateStoneList();

private:
	KD3DWnd* m_pkBtnClose;
	
	KSafeArray<KGCCommonItemWnd*, MAX_STONE_EQUIP_SLOT>	m_arrEquipSlot;
	KSafeArray<KD3DWnd*, MAX_STONE_ITEM_TAB>			m_arrTabOn;
	KSafeArray<KD3DWnd*, MAX_STONE_ITEM_TAB>			m_arrTabOff;
	
	KSafeArray<KGCPetMagicStoneItemSlot*, MAX_STONE_ITEM_SLOT> m_arrItemSlot;

	std::map<int, std::vector<CParticleEventSeqPTR>>	m_mapEffect;
	CParticleEventSeqPTR								m_pCompleteEffect;

	std::vector<GCITEMUID>	m_vecInvGlyph;

	KD3DScrollbar* m_pkScrollBar;
	int m_iTopIndex;
	KD3DWnd* m_pkNoStone;
	int m_iActiveTab;
};

DEFINE_WND_FACTORY(KGCPetMagicStoneDlg);
DECLARE_WND_FACTORY(KGCPetMagicStoneDlg);
DECLARE_WND_FACTORY_NAME(KGCPetMagicStoneDlg);