#ifndef _KGCITEMSLOT_H_
#define _KGCITEMSLOT_H_

//#include "../uifx/D3DWnd.h"

#define COOLTIME_PANNEL_NUM		8
#define DECIMAL_SIZE			10

class KGCItemSlot :public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCItemSlot );

	KGCItemSlot();
	virtual ~KGCItemSlot(void);
	// Copy constructor
	KGCItemSlot( const KGCItemSlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	// Assignment operator
	const KGCItemSlot& operator=( const KGCItemSlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void InitData(void) {};
	void SetIndex(int index);
	void SetItemPos( float Pos ){
									m_pItemVertex[0].m_vPos.x += Pos;
									m_pItemVertex[3].m_vPos.x += Pos;
								}
	void SetItemAlpha( int Var ){	m_iItemAlpha+=Var;  }

protected:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void PreDraw(void);
	virtual void PostDraw(void);

protected:
	// CoolTime ������ ���� ����Ǵ� �Լ�
	void OnCoolTime();
	// CoolTime�� 0�� �Ǿ��� �� ��¦�̴� ȿ���� ���� �Լ�
	void OnFlash();
	// Item�� ��۹�� ȸ���ϵ��� �ϴ� �Լ�
	void OnItemSpin();
	
	
	// Item ������ ǥ�� ���ִ� �Լ�
	void OnChangeQuantity();
	// ���� ǥ�� Wnd�� ��ġ�� ����ִ� �Լ�
	void SetQuantityPos();
	// ���� ǥ�� �ϰ� ���� �Լ�
	void SetQuantityOff();

	// �� CoolTime Pannel���� ����� ���ҽ�Ű�� �Լ���
	void ResetCoolTimePos();
	void DecreaseCoolTime_0();
	void DecreaseCoolTime_1();
	void DecreaseCoolTime_2();
	void DecreaseCoolTime_3();
	void DecreaseCoolTime_4();
	void DecreaseCoolTime_5();
	void DecreaseCoolTime_6();
	void DecreaseCoolTime_7();

protected:
	bool	m_bDrawItem;			// Item�� �׸��� ����
	bool	m_bItemSpin;			// Item�� ����ϰ� Item�� ȸ���ϴ� �����ΰ�
	bool	m_bOnFlash;				// Cool Time�� ������ ��¦�Ÿ��� �ִ°�
	bool	m_bEnabled;				// Could be used in current map
	int		m_iItemID;				// ���� Item Slot�� ǥ�õǰ� �ִ� Item�� ID
	int		m_iQuantity;			// ���� Item�� ����
	int		m_iCoolTime;			// ���� Item�� �ִ�� ���� �� �ִ� CoolTime
	int		m_iCoolTime_Now;		// ���� Item�� ������ �ִ� CoolTime
	int		m_MyIndex;				// ���� Item Slot�� ��ġ�ϰ� �ִ� Box�� �ε���
	int		m_iFlash;				// Cooltime�� ������ �� ��¦�̴� �ð�
	int		m_iSpinCount;			// ������ ȸ����Ű�� Ƚ��

protected:
	// ��Ÿ�� �г�
	KSafeArray<KD3DWnd*,COOLTIME_PANNEL_NUM> m_pkCoolTime_Pannel;
	// ��Ÿ�� �г� ��ǥ ��ü
	KSafeArray<D3DXVECTOR2,COOLTIME_PANNEL_NUM> m_VecCoolTime_Pannel_Pos;

	// ��Ÿ�� ���� �г�
	KD3DWnd* m_pkendCoolTime_Flash;

	// ���� �г�
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber1;	    // 1	���� �ڸ� ���� ǥ��
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber10;	    // 10	���� �ڸ� ���� ǥ��
    KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber100;	// 100	���� �ڸ� ���� ǥ��


	// ���� �г� ��ǥ ��ü

	// Ʋ �̹���
	KD3DWnd* m_pkBack_Pannel;
	KD3DWnd* m_pkFrame;
	KD3DWnd* m_pkCharacter;

	// ������ �ؽ�ó ǥ�� �г�
	GCDeviceTexture* m_pkItem_Pannel;
	// ������ �ؽ��� �г� ��ǥ
	KSafeArray<KD3DUIVertex,4> m_pItemVertex;
	// ������ �ؽ��� Alpha ��
	int		m_iItemAlpha;
};

DEFINE_WND_FACTORY( KGCItemSlot );
DECLARE_WND_FACTORY( KGCItemSlot );
DECLARE_WND_FACTORY_NAME( KGCItemSlot );

#endif
