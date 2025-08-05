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
	// CoolTime 상태인 동안 실행되는 함수
	void OnCoolTime();
	// CoolTime이 0이 되었을 때 반짝이는 효과를 내는 함수
	void OnFlash();
	// Item이 뱅글뱅글 회전하도록 하는 함수
	void OnItemSpin();
	
	
	// Item 수량을 표시 해주는 함수
	void OnChangeQuantity();
	// 수량 표시 Wnd의 위치를 잡아주는 함수
	void SetQuantityPos();
	// 수량 표시 일괄 끄기 함수
	void SetQuantityOff();

	// 각 CoolTime Pannel들의 사이즈를 감소시키는 함수들
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
	bool	m_bDrawItem;			// Item을 그릴지 말지
	bool	m_bItemSpin;			// Item을 사용하고 Item이 회전하는 상태인가
	bool	m_bOnFlash;				// Cool Time이 끝나고 반짝거리고 있는가
	bool	m_bEnabled;				// Could be used in current map
	int		m_iItemID;				// 현재 Item Slot에 표시되고 있는 Item의 ID
	int		m_iQuantity;			// 현재 Item의 수량
	int		m_iCoolTime;			// 현재 Item이 최대로 가질 수 있는 CoolTime
	int		m_iCoolTime_Now;		// 현재 Item이 가지고 있는 CoolTime
	int		m_MyIndex;				// 현재 Item Slot이 위치하고 있는 Box의 인덱스
	int		m_iFlash;				// Cooltime이 끝났을 때 반짝이는 시간
	int		m_iSpinCount;			// 아이템 회전시키는 횟수

protected:
	// 쿨타임 패널
	KSafeArray<KD3DWnd*,COOLTIME_PANNEL_NUM> m_pkCoolTime_Pannel;
	// 쿨타임 패널 좌표 객체
	KSafeArray<D3DXVECTOR2,COOLTIME_PANNEL_NUM> m_VecCoolTime_Pannel_Pos;

	// 쿨타임 종료 패널
	KD3DWnd* m_pkendCoolTime_Flash;

	// 숫자 패널
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber1;	    // 1	단위 자리 숫자 표시
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber10;	    // 10	단위 자리 숫자 표시
    KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber100;	// 100	단위 자리 숫자 표시


	// 숫자 패널 좌표 객체

	// 틀 이미지
	KD3DWnd* m_pkBack_Pannel;
	KD3DWnd* m_pkFrame;
	KD3DWnd* m_pkCharacter;

	// 아이템 텍스처 표시 패널
	GCDeviceTexture* m_pkItem_Pannel;
	// 아이템 텍스쳐 패널 좌표
	KSafeArray<KD3DUIVertex,4> m_pItemVertex;
	// 아이템 텍스쳐 Alpha 값
	int		m_iItemAlpha;
};

DEFINE_WND_FACTORY( KGCItemSlot );
DECLARE_WND_FACTORY( KGCItemSlot );
DECLARE_WND_FACTORY_NAME( KGCItemSlot );

#endif
