#ifndef _KD3DBalloonBox_H_
#define _KD3DBalloonBox_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DSizingBox.h"

enum
{	
	BBC_LEFT_TOP,
	BBC_TOP,
	BBC_RIGHT_TOP,
	BBC_LEFT,
	BBC_CENTER,	
	BBC_RIGHT,
	BBC_LEFT_BOTTOM,	
	BBC_BOTTOM,
	BBC_RIGHT_BOTTOM,
    BBC_LEFT_BOTTOM_ARROW,
    BBC_RIGHT_BOTTOM_ARROW,	
	BALLOONBOX_COMPONENT_NUM,
};

class KD3DBalloonBox : public KD3DSizingBox
{
public:
    enum EArrowType
    {
        EAT_NONE,
        EAT_LEFT,
        EAT_RIGHT,
    };
	DECLARE_CLASSNAME( KD3DBalloonBox );
	/// Default constructor
	KD3DBalloonBox( void );
	/// Default destructor
	virtual ~KD3DBalloonBox( void );
	/// Copy constructor
	KD3DBalloonBox( const KD3DBalloonBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DBalloonBox& operator=( const KD3DBalloonBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void SetSize(int iWidth, int iHeight);
	void OnSetWidth( DWORD dwWidth );
    void OnSetHeight( DWORD dwHeight );
    void ChangeRightArrow(bool bArrow);
    void ChangeLeftArrow(bool bArrow);
    void DeleteArrow();

    inline const D3DXVECTOR2& GetCenterFixedWindowLocalPos()
    {
        return m_pBoxComponent[BBC_CENTER]->GetFixedWindowLocalPos();
    }

    inline DWORD GetCenterWidth()
    {
        return m_pBoxComponent[BBC_CENTER]->GetWidth();
    }
    
    inline DWORD GetCenterHeight()
    {
        return m_pBoxComponent[BBC_CENTER]->GetHeight();
    }

    inline EArrowType GetArrowType() const
    {
        return m_eType;
    }

    inline void SetArrowType(EArrowType eType) 
    {
        m_eType=eType;
    }

    inline bool GetFirstDraw() const
    {
        return m_bFirstDraw;
    }

    inline void SetFirstDraw(bool bFirstDraw) 
    {
        m_bFirstDraw=bFirstDraw;
    }


protected:
	virtual void OnCreate( void );
    
    EArrowType m_eType;    
    bool m_bFirstDraw;        //첫 드로잉 체크 하는 함수.. 처음에 생성 위치에 따라 방향을 정하기 위해 사용된다.
    KSafeArray<KD3DWnd*,BALLOONBOX_COMPONENT_NUM> m_pBoxComponent;
};

DEFINE_WND_FACTORY( KD3DBalloonBox );
DECLARE_WND_FACTORY( KD3DBalloonBox );
DECLARE_WND_FACTORY_NAME( KD3DBalloonBox );

#endif // _KD3DBalloonBox_H_
