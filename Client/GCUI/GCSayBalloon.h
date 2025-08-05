#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KGCSayBalloon :   public KD3DWnd,
                        public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCSayBalloon );
    /// Default constructor
    KGCSayBalloon( void );
    /// Default destructor
    virtual ~KGCSayBalloon( void );
    /// Copy constructor
    KGCSayBalloon( const KGCSayBalloon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSayBalloon& operator=( const KGCSayBalloon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		GC_SP_QUARTER_LEFT,
		GC_SP_MIDDLE,
		GC_SP_QUARTER_RIGHT,
	};
protected:
    virtual void OnCreate();
    virtual void ActionPerformed(const KActionEvent& event );
public:
    void SetWidth( DWORD dwWidth );
    void SetHeight( DWORD dwHeight );
    void SetWidthHeight( DWORD dwWidth, DWORD dwHeight );
    void SetSayWhoPos( DWORD dwPos );	// 말풍선에서 말꼬리가 가지는 위치 ( 0이면 왼쪽 끝 )
	void SetSayWhoPos( D3DXVECTOR2 vPos );	// 말풍선 꼬리가 가리키는 위치 셋팅
protected:
    KD3DSizingBox*  m_pkMessageBalloon;
    KD3DWnd*    m_pkSayWho;
    DWORD       m_dwSayWhoPos;//말풍선의 X 위치에서 부터 m_dwSayWhoPos 만큼 되는 위치

	int m_eSayWhoPos;
};

DEFINE_WND_FACTORY( KGCSayBalloon );
DECLARE_WND_FACTORY( KGCSayBalloon );
DECLARE_WND_FACTORY_NAME( KGCSayBalloon );

