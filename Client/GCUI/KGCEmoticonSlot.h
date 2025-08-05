#ifndef _KGCEMOTICONSLOT_H_
#define _KGCEMOTICONSLOT_H_

//#include "../uifx/D3DWnd.h"

#define DECIMAL_SIZE			10

class KGCEmoticon;
class KGCEmoticonSlot : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCEmoticonSlot );

	KGCEmoticonSlot();
	virtual ~KGCEmoticonSlot(void);
	/// Copy constructor
	KGCEmoticonSlot( const KGCEmoticonSlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCEmoticonSlot& operator=( const KGCEmoticonSlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void InitData(void);
    void ActionPerformed( const KActionEvent& event );


public:
	void SetIndex(int index){	m_MyIndex=index;   }
	void SetItemAlpha( int Var ){	m_iItemAlpha+=Var;  }
	void SetItemPos( float Pos ){
		m_pItemVertex[0].m_vPos.x+=Pos;
		m_pItemVertex[3].m_vPos.x+=Pos;
	}

protected:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void PostDraw(void);

protected:
	// Item 수량을 표시 해주는 함수
	void OnChangeQuantity();
	// 수량 표시 Wnd의 위치를 잡아주는 함수
	void SetQuantityPos();
	// 수량 표시 일괄 끄기 함수
	void SetQuantityOff();

protected:
	int		m_iQuantity;
	int		m_MyIndex;

protected:
	// 틀 이미지
	KD3DWnd* m_pkFrame;

	// 숫자 이미지
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber1;		// 1	단위 자리 숫자 표시
	KSafeArray<KD3DWnd*,DECIMAL_SIZE> m_pkNumber10;	// 10	단위 자리 숫자 표시

	// 아이템 텍스처 표시 패널
	KGCEmoticon* m_pkEmoticon;
    GCDeviceTexture* m_pkCooltime_Pannel;

	// 아이템 텍스쳐 패널 좌표
	KSafeArray<KD3DUIVertex,4> m_pItemVertex;

	// 아이템 텍스쳐 Alpha 값
	int		m_iItemAlpha;
    int     m_iRealCoolTime;

    bool    m_bIsSpecialItem;
};

DEFINE_WND_FACTORY( KGCEmoticonSlot );
DECLARE_WND_FACTORY( KGCEmoticonSlot );
DECLARE_WND_FACTORY_NAME( KGCEmoticonSlot );

#endif
