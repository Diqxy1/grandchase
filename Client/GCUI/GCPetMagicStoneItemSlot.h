#pragma once

class KGCPetMagicStoneItemSlot : public KD3DWnd,		// extends
							 public KActionListener // implements
{
public:
	DECLARE_CLASSNAME(KGCPetMagicStoneItemSlot);
	/// Default constructor
	KGCPetMagicStoneItemSlot( void );
	/// Default destructor
	virtual ~KGCPetMagicStoneItemSlot( void );
	/// Copy constructor
	KGCPetMagicStoneItemSlot(const KGCPetMagicStoneItemSlot&) = delete;
	/// Assignment operator
	const KGCPetMagicStoneItemSlot& operator=(const KGCPetMagicStoneItemSlot&) = delete;


public:
	virtual void ActionPerformed( const KActionEvent& event ) override;
	virtual void OnCreate() override;
	virtual void FrameMoveInEnabledState() override;

public:
	void AddItem(GCITEMUID dwItemUID);
	void SetIndex(int iIndex) { m_iIndex = iIndex; }

private:
	void OnEquip();

private:
	KGCItemImgWnd*	m_pkItemImg;
	KD3DStatic*		m_pkItemName;
	KD3DWnd*		m_pkBtnEquip;
	KD3DStatic*		m_pkItemDesc;

	GCITEMUID		m_dwItemUID;

	int				m_iIndex;
};

DEFINE_WND_FACTORY(KGCPetMagicStoneItemSlot);
DECLARE_WND_FACTORY(KGCPetMagicStoneItemSlot);
DECLARE_WND_FACTORY_NAME(KGCPetMagicStoneItemSlot);